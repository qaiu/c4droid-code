#include <iostream>
#include <sys/time.h>
#include <arm_neon.h>

#include <cstring>
#include <chrono>
#define B_SIZE ((long)(128*4096*8))

using namespace std;

// neon优化
void *memcpy_128(void *dest, void *src, size_t count)
{
        int i;
        unsigned long *s = (unsigned long *)src;
        unsigned long *d = (unsigned long *)dest;
        for (i = 0; i < count / 64; i++) {
            vst1q_u64(&d[0], vld1q_u64(&s[0])); 
            vst1q_u64(&d[2], vld1q_u64(&s[2])); 
            vst1q_u64(&d[4], vld1q_u64(&s[4])); 
            vst1q_u64(&d[6], vld1q_u64(&s[6]));
            d += 8; s += 8;
        }
        return dest;
}

// 获取当前时刻(微秒)
auto getNow(){
  using namespace std::chrono;
  return duration_cast<microseconds>(
    system_clock::now().time_since_epoch()
  ).count();
}

//aarch64 neon汇编优化
//uncached区域：
void my_memcpy(volatile void *dst, volatile void *src, int sz)
{
    if (sz & 63) {
        sz = (sz & -64) + 64;
    }
    asm volatile (
        "NEONCopyPLD: \n"
		"sub %[dst], %[dst], #64 \n"
		"1: \n"
		"ldnp q0, q1, [%[src]] \n"
		"ldnp q2, q3, [%[src], #32] \n"
		"add %[dst], %[dst], #64 \n"
		"subs %[sz], %[sz], #64 \n"
		"add %[src], %[src], #64 \n"
		"stnp q0, q1, [%[dst]] \n"
		"stnp q2, q3, [%[dst], #32] \n"
		"b.gt 1b \n"
		: [dst]"+r"(dst), [src]"+r"(src), [sz]"+r"(sz) : : "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "cc", "memory");
}

//cached区域：
void my_memcpy1(volatile void *dst, volatile void *src, int sz)
{
    if (sz & 63) {
        sz = (sz & -64) + 64;
    }
    asm volatile (
        "NEONCopyPLD2: \n"
		"sub %[src], %[src], #32 \n"
		"sub %[dst], %[dst], #32 \n"
		"1: \n"
		"ldp q0, q1, [%[src], #32] \n"
		"ldp q2, q3, [%[src], #64]! \n"
		"subs %[sz], %[sz], #64 \n"
		"stp q0, q1, [%[dst], #32] \n"
		"stp q2, q3, [%[dst], #64]! \n"
		"b.gt 1b \n"
		: [dst]"+r"(dst), [src]"+r"(src), [sz]"+r"(sz) : : "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7", "cc", "memory");
}

template <class Function>
void calcTime(const char* name, Function&& f)
{
    // calculate cpu time
    long st=getNow();
    f();
    long en=getNow();
    
    auto interval = (en - st) / 1000.0;
    std::cout <<"CPU time: " << name << interval << " ms\n";
    //long sum=B_SIZE*100;
    //std::cout<<sum<<"\n";
    std::cout << (B_SIZE*100.0)/1024/1024/interval*1000.0 << "MB/s\n";
}

//测试函数
template <class Function>
auto test(Function&& f){
    return [=] {
    long len = B_SIZE;
	int  loop = 100;
	char* p = new char[len];
	char* q = p;
	
    long st=getNow();
	for (int i =0; i < loop; ++i)
	{
		char* p = new char[len];
		*p = char(i);
		f(p, q, len);
		delete [] p;
	}
  };
    
}
#include <unistd.h>

int main(int argc, char* argv[]) {
   // usleep(1000);
    calcTime("memcpy_128  ", test(memcpy_128));
    calcTime("my_memcpy  ", test(my_memcpy));
    calcTime("my_memcpy1  ", test(my_memcpy1));
    calcTime("sys_memcpy  ", test(memcpy));
    
    calcTime("memcpy_128  ", test(memcpy_128));
    calcTime("my_memcpy  ", test(my_memcpy));
    calcTime("my_memcpy1  ", test(my_memcpy1));
    calcTime("sys_memcpy  ", test(memcpy));
}