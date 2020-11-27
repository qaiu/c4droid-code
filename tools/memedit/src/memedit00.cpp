#include <cstdio>
#include <cstdlib>
#include <string>

#include <pthread.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#include "memedit00.h"
namespace kk
{
	static void *edit_freeze(void *);
	static void *edit_interval(void *);

	class MemEdit
	{
		//地址列表(搜索结果)
		struct num_list
		{
			uint8_t *number;
			long addr;
			size_t size;
			num_list *next;
		};
		
		struct number_list
		{
			uint8_t *number0;
			uint8_t *number;
			long addr;
			size_t size;
			number_list *next;
		}*interval_list = NULL,*interval_head = NULL,*interval_node = NULL;
		
		//基址模块(内存范围)
		struct module_base
		{
			long vm_start;
			long vm_end;
			char vm_flags[8];
			char vm_name[128];
			module_base *next;
		} *module_base_addr = NULL, *module_base_head = NULL;

		struct number_addrs
		{
			long addr;
			number_addrs *next = NULL;
		} *n_addr = NULL, *n_addr_head = NULL;

		MemEdit(char **argv, const char *str,int mode); //构造函数
		MemEdit(const MemEdit & other);

		int module_name[10] = { };		  //内存模块基址名字
		char packname_app_path[128] = { }; //应用app路径
		char packname_data_path[128] = { }; //应用data路径
		num_list *freeze_list, *freeze_node, *freeze_head; //冻结相关链表
		char mem_path[24] = { }; //mem文件路径
		pid_t target_pid;		// 全局pid
		bool new_search_flag = 1;	// 新搜索flag
		
		
		int addr_sum, base_sum; //计数: 基址模块个数和地址数值个数
		long freeze_time;	   //冻结时间间隔
		long interval_write_time;	 //写入时间间隔
		long interval_restore_time;    //恢复时间间隔
		
		template <class T> void delete_list(T * &list);
		template <class T> void delete_node(T * &list, T * &list_last, T * &list_head);
		int get_pid(const char *process_name);
		int strscmp(const char *s1, const char *s2);
		module_base *get_maps();
		bool get_pack_path(const char *pack_name, char *pack_path);
		template <class T> void add_freeze(T value, int offset);
		template <class T> void add_interval(T value, int offset);
		
	  public:
		template <class T> void search_data(T value, T max_value, int offset, bool range_flag);
		template <class T> void search_data_range(T value, int16_t offset);
		void clear_results();
		template <class T> void edit_all(T value, int sum, int offset, bool is_freeze);
		template <class T> void edit_all_interval(T value, int offset);

	
		void clear_freeze_results();
		void set_freeze_time(long tim);
		void set_interval_time(long,long);
		void clear_interval_results();
		void set_interval_time(long tim);
		void init(char **argv, const char *str);
		void print_addr();
		void print_base_module();
		void print_addr_value(int *val);
		template <class T> T read_addr(long addr);
		template <class T> void write_addr(long addr, T value);
		
		friend void *edit_freeze(void *);
		friend void *edit_interval(void *);
		friend int search_number(const char *value_str, int type, int offset);
		friend int search_number_range(const char*value_str,int type, int16_t offset);
		friend void set_ranges(int arg, ...);
		
		static MemEdit *GetInstance(char **argv, const char *str,int mode);
		~MemEdit();
	} *mem_p=NULL;

	MemEdit::MemEdit(char **argv, const char *str,int mode)
	{
		if (mode == MODE_ROOT)
		{
			std::string su = "su -c ";
			su += *argv;
			if (getuid() != 0)
			{
				system(su.c_str());
				exit(1);
			}
			system("echo 0 > /proc/sys/fs/inotify/max_user_watches");
		}
		
		target_pid = get_pid(str);
		sprintf(mem_path, "/proc/%d/mem", target_pid);
		strcpy(packname_data_path, str);
		get_pack_path(str, packname_app_path);
		if (-1 == target_pid)
		{
			printf("初始化失败\n");
			exit(-1);
		}
		else
			printf("初始化成功！pid:%d\n", target_pid);
			
		freeze_time = 36000;
		interval_write_time=interval_restore_time=36000;
		freeze_list=freeze_node=freeze_head=NULL;
		pthread_t pid = 0;
		pthread_create(&pid, NULL, edit_freeze, NULL);
		pthread_t pid2 = 0;
		pthread_create(&pid2, NULL, edit_interval, NULL);
	}

	MemEdit::~MemEdit(void)
	{
		puts("已退出，欢迎下次使用");
	}
	
	
	MemEdit *MemEdit::GetInstance(char **argv, const char *str, int mode)
	{
		static MemEdit instance(argv,str,mode);
		return &instance;
	}
		
	template <> // 特化模板函数用于delete冻结链表
	void MemEdit::delete_list(num_list * &list)
	{
		num_list *head;
		while (list)
		{
			head = list->next;
			delete(list->number);
			delete(list);
			list = head;
		}
	}
	template <> // 特化模板函数用于delete冻结链表
	void MemEdit::delete_list(number_list * &list)
	{
		number_list *head;
		while (list)
		{
			head = list->next;
			delete(list->number);
			delete(list);
			list = head;
		}
	}

	template <class T> 
	void MemEdit::delete_list(T * &list)
	{
		T *head;
		while (list)
		{
			head = list->next;
			free(list);
			list = head;
		}
	}
	/* 删除链表某个节点 */
	template <class T> 
	void MemEdit::delete_node(T * &list, T * &list_last, T * &list_head)
	{
		if (list == list_head)	// 如果是头节点，直接向后偏移
			list_head = list = list->next;
		else					// 删除节点
		{
			list_last->next = list_last->next->next;
			free(list);
			list->next = NULL;
			list = list_last->next;
		}
	}

	// 通过进程的文件路径名称，获取进程的pid
	int MemEdit::get_pid(const char *process_name)
	{
		int id;
		pid_t pid = -1;
		DIR *dir;
		FILE *fp;
		char filename[32];
		char cmdline[256];
		struct dirent *entry;

		if (process_name == NULL)
			return -1;

		dir = opendir("/proc");
		if (dir == NULL)
			return -1;
		while ((entry = readdir(dir)) != NULL)
		{
			id = atoi(entry->d_name);
			if (id != 0)
			{
				sprintf(filename, "/proc/%d/cmdline", id);
				fp = fopen(filename, "r");
				if (fp)
				{
					fgets(cmdline, sizeof(cmdline), fp);
					fclose(fp);

					if (strcmp(process_name, cmdline) == 0)
					{
						pid = id;
						break;
					}
				}
			}
		}
		closedir(dir);
		return pid;
	}

	int MemEdit::strscmp(const char *s1, const char *s2)
	{
		return strncmp(s1, s2, strlen(s2));
	}

	// 获取内存范围指定的基址模块
	MemEdit::module_base * MemEdit::get_maps()
	{
		base_sum = 0;
		module_base *addrs = NULL, *head = NULL, *addr;
		char filename[32];
		char line[1024];
		snprintf(filename, sizeof(filename), "/proc/%d/maps", target_pid);
		FILE *fp = fopen(filename, "r");
		if (fp != NULL)
		{
			while (fgets(line, sizeof(line), fp))
			{
				addr = (module_base *) calloc(1, sizeof(module_base));
				sscanf(line, "%p-%p %s %*p %*p:%*p %*p   %[^\n]%s", &addr->vm_start, &addr->vm_end,
					   addr->vm_flags, addr->vm_name);
				int flag = 0;
				for (int i = 0; flag == 0 && module_name[i]; i++)
				{
					switch (module_name[i])
					{
					case REGION_C_BSS:
						if (!strcmp(addr->vm_name, "[anon:.bss]"))
							flag = 1;
						break;
					case REGION_C_HEAP:
						if (!strcmp(addr->vm_name, "[heap]"))
							flag = 1;
						break;
					case REGION_C_ALLOC:
						if (!strcmp(addr->vm_name, "[anon:libc_malloc]"))
							flag = 1;
						break;
					case REGION_C_DATA:
						{
							char temp[128] = { };

							strcpy(temp, "/data/app/");
							strcat(temp, packname_app_path);
							strcat(temp, "/");
							if (strstr(addr->vm_name, ".so") && !strscmp(addr->vm_name, temp)
								&& (addr->vm_flags[1] == 'w' || !strcmp(addr->vm_flags,"r--p")))
								flag = 1;
							strcpy(temp, "/data/data/");
							strcat(temp, packname_data_path);
							if (strstr(addr->vm_name, ".so") && !strscmp(addr->vm_name, temp)
								&& (addr->vm_flags[1] == 'w' || !strcmp(addr->vm_flags,"r--p")))
								flag = 1;
							break;

						}
					case REGION_STACK:
						if (!strscmp(addr->vm_name, "[stack"))
							flag = 1;
						break;
					case REGION_BAD:
						if (!strscmp(addr->vm_name, "/dev/kgsl-3d0"))
							flag = 1;
						break;
					case REGION_JAVA_HEAP:
						if (strscmp(addr->vm_name, "/dev/ashmem/dalvik-large") == 0 ||
							strscmp(addr->vm_name, "/dev/ashmem/dalvik-main") == 0)
							flag = 1;
						break;
					case REGION_ANONY:
						if (strlen(addr->vm_name) == 0 &&
							addr->vm_flags[0] == 'r' && addr->vm_flags[2] != 'x')
							flag = 1;
						break;
					case REGION_CODE_APP:
						{
							if (addr->vm_flags[2] == 'x')
							{
								char temp[128] = { };
								strcpy(temp, "/data/app/");
								strcat(temp, packname_app_path);
								if (!strscmp(addr->vm_name, temp))
									flag = 1;

								strcpy(temp, "/data/data/");
								strcat(temp, packname_data_path);
								if (!strscmp(addr->vm_name, temp))
									flag = 1;
								if (!strlen(addr->vm_name))
									flag = 1;
							}
							break;
						}
					case REGION_CODE_SYS:
						if (addr->vm_flags[2] == 'x')
						{
							if (strscmp(addr->vm_name, "/system/") == 0 ||
								strscmp(addr->vm_name, "/vendor/") == 0 ||
								strscmp(addr->vm_name, "/dev/ashmem/") == 0)
								flag = 1;
						}
						break;
					}
				}

				if (flag)
				{
					if (addrs == NULL)
					{
						head = addrs = addr;
					}
					else
					{
						addrs->next = addr;
						addrs = addrs->next;
					}
					base_sum++;
				}
				else
					free(addr);
			}
			fclose(fp);
		}
		return head;
	}


	bool MemEdit::get_pack_path(const char *pack_name, char *pack_path)
	{
		DIR *dir;
		FILE *fp;
		struct dirent *entry;
		
		char buf[123]={};
		strcpy(buf,pack_name);
		strcat(buf,"-");
		if (pack_name == NULL)
			return false;

		dir = opendir("/data/app");
		if (dir == NULL)
			return false;
		while ((entry = readdir(dir)) != NULL)
		{
			if (strstr(entry->d_name, buf))
			{
				strcpy(pack_path, entry->d_name);
				return true;
			}
		}
		return false;
	}

	template <class T>
	void MemEdit::search_data(T value, T max_value, int offset, bool range_flag)
	{
		if (!module_base_head)
		{
			module_base_head = module_base_addr = get_maps();
		}
		if (n_addr_head)
			n_addr = n_addr_head;

		number_addrs *n_addr_last = n_addr_head;
		module_base *module_last = module_base_head;
		int k = 0, sum = base_sum;

		int fd = open(mem_path, O_RDWR);	// 打开文件
		long src, size;
		int block_size = sizeof(T) > 4 ? 4 : sizeof(T);

		while (module_base_addr)
		{
			src = module_base_addr->vm_start;
			size = module_base_addr->vm_end - module_base_addr->vm_start;
			void *value_list = calloc(1, size);
			pread64(fd, value_list, size, src);

			bool kk = 0;
			// printf("\e[4H\e[33;5m>> %d%%\e[0m\n", ++k*100/sum);

			if (new_search_flag)
			{
				for (int i = 0; i < size; i += block_size)
				{
					if ((range_flag == 0 && *(T *) (value_list + i) == value)
						|| (range_flag && *(T *) (value_list + i) >= value
							&& *(T *) (value_list + i) <= max_value))
					{
						if (n_addr == NULL)
							n_addr_head = n_addr =
								(number_addrs *) calloc(1, sizeof(number_addrs));
						else
						{
							n_addr->next = (number_addrs *) calloc(1, sizeof(number_addrs));
							n_addr = n_addr->next;
						}
						n_addr->next = NULL;		
						n_addr->addr = src;
						kk = 1;
						addr_sum++;
					}
					src += block_size;
				}
			}
			else
			{
				while (n_addr && n_addr->addr < module_base_addr->vm_end)
				{
					long z = n_addr->addr - src + offset;
					if ((range_flag == 0 && *(T *) (value_list + z) == value)
						|| (range_flag && *(T *) (value_list + z) >= value
							&& *(T *) (value_list + z) <= max_value))
					{
						n_addr->addr = offset + n_addr->addr;
						n_addr_last = n_addr;
						n_addr = n_addr->next;
						kk = 1;
					}
					else
					{
						addr_sum--;
						delete_node(n_addr, n_addr_last, n_addr_head);
					}
				}
			}
			
			// 动态调整模块基址链表
			if (!kk)
			{
				base_sum--;
				delete_node(module_base_addr, module_last, module_base_head);
			}
			else
			{
				module_last = module_base_addr;
				module_base_addr = module_base_addr->next;
			}
			free(value_list);
		}

		close(fd);

		module_base_addr = module_base_head;
		n_addr = n_addr_head;
		new_search_flag = 0;
	}

	template <class T> 
	void MemEdit::search_data_range(T value, int16_t offset)
	{
		number_addrs *temp_head = NULL,*temp=NULL;
		addr_sum = 0;
		
		if (!n_addr_head)
			return;
		n_addr = n_addr_head;	

		number_addrs *n_addr_last = n_addr_head;
		module_base *module_last = module_base_head;
		
		int fd = open(mem_path, O_RDONLY);	// 打开文件

		void *range_buf = calloc(1, offset + 8);
		int block_size = sizeof(T) > 4 ? 4 : sizeof(T);
		int range_size=0;
		long last_addr=0;
		int i = 0;
		int k=0;
		while (module_base_addr)
		{
			int kk=0;
			long src = module_base_addr->vm_start;
			long size = module_base_addr->vm_end - module_base_addr->vm_start;
			void *value_list = calloc(1, size);
			pread64(fd, value_list, size, src);
			
			while (n_addr && n_addr->addr < module_base_addr->vm_end)
			{
				long z = n_addr->addr - src;
				if (n_addr->addr+offset > module_base_addr->vm_end)
					range_size=module_base_addr->vm_end-n_addr->addr;
				else 
					range_size=offset;
				if (n_addr->addr<last_addr)
				{
					i=last_addr - n_addr->addr + block_size;
				//	printf(">>>>%d\n",i);
				}
				else i = 0;
				for (; i <= range_size; i += block_size)
				{
					if (*(T *) (value_list +z + i) == value)
					{
						if (temp == NULL)
							temp_head=temp = (number_addrs *) calloc(1, sizeof(number_addrs));
						else 
						{
							temp->next = (number_addrs *) malloc(sizeof(number_addrs));
							temp = temp->next;
						}
						
						temp->next=NULL;	
						last_addr=temp->addr = n_addr->addr + i;
						
						kk=1;
						addr_sum++;
					}
				}
				if(kk)
				{
					n_addr_last = n_addr;
					n_addr = n_addr->next;
				}
				else
				delete_node(n_addr, n_addr_last, n_addr_head);
			}
			
			// 动态调整模块基址链表
			if (kk)
			{
				module_last = module_base_addr;
				module_base_addr = module_base_addr->next;
			}
			else
			{
				base_sum--;
				delete_node(module_base_addr, module_last, module_base_head);
			}
			
			free(value_list);
		}
		
		delete_list <number_addrs> (n_addr);
		
		n_addr = n_addr_head = temp_head;
		module_base_addr = module_base_head;
	}

	void MemEdit::clear_results()
	{
		new_search_flag = 1;
		delete_list < number_addrs > (n_addr_head);
		delete_list <module_base > (module_base_addr);
		n_addr = n_addr_head = NULL;
		module_base_addr = module_base_head = NULL;
		addr_sum = 0;
	}

	template <class T> 
	void MemEdit::add_freeze(T value, int offset)
	{
		while (n_addr)
		{
			if (freeze_head == NULL)
				freeze_list = freeze_head = freeze_node = new num_list;
			else
			{
				freeze_node->next = new num_list;
				freeze_node = freeze_node->next;
			}		
			freeze_node->next = NULL;	
			freeze_node->addr = n_addr->addr + offset;
			freeze_node->number = new uint8_t[sizeof(T)];

			freeze_node->size = sizeof(T);
			memcpy(freeze_node->number, &value, sizeof(T));
			n_addr = n_addr->next;
		}
		n_addr = n_addr_head;
	}
	
	
	template <class T> 
	void MemEdit::edit_all(T value, int sum, int offset, bool is_freeze)
	{
		if (is_freeze)
			add_freeze <T> (value, offset);
		else
		{
			int fd = open(mem_path, O_WRONLY);	// 打开文件
			int i = 0;
			while ((sum == 0 && n_addr) || (i++ < sum && n_addr))
			{
				pwrite64(fd, &value, sizeof(T), n_addr->addr + offset);
				n_addr = n_addr->next;
			}

			n_addr = n_addr_head;
			close(fd);
		}
		new_search_flag = 0;
	}
	
	template <class T> 
	void MemEdit::edit_all_interval(T value, int offset)
	{
		int fd = open(mem_path, O_RDONLY);	// 打开文件
		while (n_addr)
		{
			if (interval_head == NULL)
				interval_list = interval_head = interval_node = new number_list;
			else
			{
				interval_node->next = new number_list;
				interval_node = interval_node->next;
			}
			
			interval_node->next = NULL;
			interval_node->addr = n_addr->addr + offset;
			interval_node->number0 = new uint8_t[sizeof(T)];
			interval_node->number = new uint8_t[sizeof(T)];
			
			interval_node->size = sizeof(T);
			memcpy(interval_node->number, &value, sizeof(T));
			pread64(fd, interval_node->number0, interval_node->size, interval_node->addr);

			n_addr = n_addr->next;
		}
		n_addr = n_addr_head;
		close (fd);
	}
	
	void MemEdit::set_freeze_time(long tim)
	{
		freeze_time = tim;
	}
	
	void MemEdit::set_interval_time(long tim_write,long tim_restore)
	{
		interval_write_time = tim_write;
		interval_restore_time = tim_restore;
	}
	
	void MemEdit::clear_freeze_results()
	{
		delete_list(freeze_head);
		freeze_list = freeze_head = freeze_node = NULL;
	}
	
	void MemEdit::clear_interval_results()
	{
		delete_list(interval_head);
		interval_list = interval_head = interval_node = NULL;
	}



	void MemEdit::print_addr_value(int *val)
	{
		if (n_addr == NULL || n_addr->addr == 0)
			return;
		if (!module_base_head)
			return;
		bool tty=isatty(fileno(stdout));
		int fd = open(mem_path, O_RDONLY);
		int sum=1;
		while (module_base_addr)
		{
			long src = module_base_addr->vm_start;
			long end = module_base_addr->vm_end;
			long size = end - src;
			uint8_t *value_list = (uint8_t *) calloc(1, size);
			pread64(fd, value_list, size, src);

			while (n_addr && n_addr->addr && n_addr->addr < end)
			{
				printf(tty?"\e[32;1m %d. %p:\e[0m\n ":" %d. %p:\n", sum++, n_addr->addr);
				int offset = n_addr->addr - src;

				for (int i = 0; val[i]; i++)
				{
					switch (val[i])
					{
					case DWORD:
						printf(tty?"\e[36;1m%dD \e[0m":"%dD ", *(int32_t *) (value_list + offset));
						break;
					case QWORD:
						printf(tty?"\e[34;1m%ldQ \e[0m":"%ldQ ", *(int64_t *) (value_list + offset));
						break;
					case WORD:
						printf(tty?"\e[31;1m%dW \e[0m":"%dW ", *(int16_t *) (value_list + offset));
						break;
					case DOUBLE:
						printf(tty?"\e[33;1m%gE \e[0m":"%gE ", *(double *)(value_list + offset));
						break;
					case BYTE:
						printf(tty?"\e[35;1m%dB \e[0m":"%dB ", *(int8_t *) (value_list + offset));
						break;
					case FLOAT:
						printf(tty?"\e[31;1m%gF \e[0m":"%gF ", *(float *)(value_list + offset));
						break;
					case HEX_L:
						if (tty) printf("\e[36;1m");
						for (int i = 0; i < 8; i++)
							printf("%02x", *(value_list + offset + i));
						if (tty) printf("LE\e[0m");
						break;
					case HEX_B:
						if (tty) printf("\e[33;1m");
						for (int i = 7; i >= 0; i--)
							printf("%02x", *(value_list + offset + i));
						if (tty) printf("BE\e[0m");
						break;
					}
				}
				n_addr = n_addr->next;
				printf("\n------------------------------------------------\n");
			}
			module_base_addr = module_base_addr->next;
			free(value_list);
		}
		module_base_addr = module_base_head;
		n_addr = n_addr_head;
		close(fd);
	}

	void MemEdit::print_addr()
	{
		int i = 1;
		while (n_addr)
		{
			printf("\e[33;1m%d:%p\e[0m\n", i++, n_addr->addr);
			n_addr = n_addr->next;
		}
		n_addr = n_addr_head;
	}

	void MemEdit::print_base_module()
	{
		module_base_addr = module_base_head;
		int i = 1;
		while (module_base_addr)
		{
			printf("\e[35;1m%d:%p-%p,%s    %d\e[0m\n", i++, module_base_addr->vm_start,
				   module_base_addr->vm_end, module_base_addr->vm_name,
				   strlen(module_base_addr->vm_name));
			module_base_addr = module_base_addr->next;
		}
		module_base_addr = module_base_head;
	}

	template <class T> 
	T MemEdit::read_addr(long addr)
	{
		int fd = open(mem_path, O_RDONLY);	// 打开文件
		T value;
		pread64(fd, &value, sizeof(T), addr);
		close(fd);
		return value;
	}

	template <class T> 
	void MemEdit::write_addr(long addr, T value)
	{
		int fd = open(mem_path, O_WRONLY);	// 打开文件
		char *value_buf = (char *)calloc(1, sizeof(T));
		pwrite64(fd, &value, sizeof(T), addr);
		close(fd);
		free(value_buf);
	}
}

//接口定义

namespace kk
{
	pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
	void *edit_freeze(void *)
	{
		if (!mem_p) return NULL;
		int fd = open(mem_p->mem_path,O_RDWR);	// 打开文件
		usleep(100000);
		while (1)
		{
			
			while (mem_p->freeze_list)	
			{
				pwrite64(fd, mem_p->freeze_list->number, mem_p->freeze_list->size, mem_p->freeze_list->addr);
				mem_p->freeze_list = mem_p->freeze_list->next;
			}			
			mem_p->freeze_list = mem_p->freeze_head;
			usleep(mem_p->freeze_time);
		}
		return NULL;
	}
	
	void *edit_interval(void *)
	{
		if (!mem_p) return NULL;
		int fd = open(mem_p->mem_path,O_RDWR);	// 打开文件
		usleep(100000);
		while (1)
		{
			usleep(mem_p->interval_write_time);
			while (mem_p->interval_list)	
			{
				pwrite64(fd, mem_p->interval_list->number, mem_p->interval_list->size, mem_p->interval_list->addr);
				mem_p->interval_list = mem_p->interval_list->next;
			}
			mem_p->interval_list = mem_p->interval_head;
			usleep(mem_p->interval_restore_time);
	
			while (mem_p->interval_list)
			{
				pwrite64(fd, mem_p->interval_list->number0, mem_p->interval_list->size, mem_p->interval_list->addr);
				mem_p->interval_list = mem_p->interval_list->next;
			}
			mem_p->interval_list = mem_p->interval_head;
		}
		close(fd);
		return NULL;
	}
	
	

	void init(char **argv, const char *str,int mode)
	{
		mem_p = MemEdit::GetInstance(argv,str,mode);
	}
	
	void set_ranges(int arg, ...)
	{
		if (!mem_p) return;
		
		for (int i = 0; i < 10 && mem_p->module_name[i]; i++)
		{
			mem_p->module_name[i]=0;
		}
		
		va_list p_args;
		va_start(p_args, arg);
		int val = arg;
		for (int i = 0; i < 10 && val; i++)
		{
			mem_p->module_name[i] = val;
			val = va_arg(p_args, int);
		}
		va_end(p_args);
	}
	
	int search_number(const char *value_str, int type, int offset)
	{
		if (!mem_p) return -1;
		int freeze=0;
		char buf[128]={};
		strcpy(buf,value_str);
		const char *buf2="0";
		if (strchr(value_str,'~'))
		{
			freeze=1;			
			buf[strchr(value_str,'~')-value_str]=0;
			buf2=&buf[strchr(value_str,'~')-value_str+1];
		}
		
		switch (type)
		{
		
		case DWORD:
		{
			int value=atoi(buf);
			int max=atoi(buf2);
			mem_p->search_data<int32_t>(value,max,offset,freeze);
			break;
		}
		case WORD:
		{
			int16_t value=atoi(buf);
			int16_t max=atoi(buf);
			mem_p->search_data<int16_t>(value,max,offset,freeze);
			break;
		}
		case FLOAT:
		{
			float value=atof(buf);
			float max=atof(buf2);
			mem_p->search_data<float>(value,max,offset,freeze);
			break;
		}
		case QWORD:
		{
			int64_t value=atoll(buf);
			int64_t max=atoll(buf2);
			mem_p->search_data<int64_t>(value,max,offset,freeze);
			break;
		}
		case DOUBLE:
		{
			double value=atof(buf);
			double max=atof(buf2);
			mem_p->search_data<double>(value,max,offset,freeze);
			break;
		}
		case BYTE:
		{
			int8_t value=atoi(buf);
			int8_t max=atoi(buf2);
			mem_p->search_data<int8_t>(value,max,offset,freeze);
			break;
		}
		}
		return mem_p->addr_sum;
	}
		
	void edit_all(const char*value_str, int type, int sum, int offset,bool freeze)
	{
		if (!mem_p) return;
		switch (type)
		{
		
		case DWORD:
		{
			int value=atoi(value_str);
			mem_p->edit_all<int32_t>(value,sum,offset,freeze);
			break;
		}
		case WORD:
		{
			int16_t value=atoi(value_str);
			mem_p->edit_all<int16_t>(value,sum,offset,freeze);
			break;
		}
		case FLOAT:
		{
			float value=atof(value_str);
			mem_p->edit_all<float>(value,sum,offset,freeze);
			break;
		}
		case QWORD:
		{
			int64_t value=atoll(value_str);
			mem_p->edit_all<long long>(value,sum,offset,freeze);
			break;
		}
		case DOUBLE:
		{
			double value=atof(value_str);
			mem_p->edit_all<double>(value,sum,offset,freeze);
			break;
		}
		case BYTE:
		{
			int8_t value=atoi(value_str);
			mem_p->edit_all<int8_t>(value,sum,offset,freeze);
			break;
		}
		}	
	}
	
	
	void edit_interval(const char*value_str, int type, int offset)
	{
		if (!mem_p) return;
		switch (type)
		{
		
		case DWORD:
		{
			int value=atoi(value_str);
			mem_p->edit_all_interval<int32_t>(value,offset);
			break;
		}
		case WORD:
		{
			int16_t value=atoi(value_str);
			mem_p->edit_all_interval<int16_t>(value,offset);
			break;
		}
		case FLOAT:
		{
			float value=atof(value_str);
			mem_p->edit_all_interval<float>(value,offset);
			break;
		}
		case QWORD:
		{
			int64_t value=atoll(value_str);
			mem_p->edit_all_interval<long long>(value,offset);
			break;
		}
		case DOUBLE:
		{
			double value=atof(value_str);
			mem_p->edit_all_interval<double>(value,offset);
			break;
		}
		case BYTE:
		{
			int8_t value=atoi(value_str);
			mem_p->edit_all_interval<int8_t>(value,offset);
			break;
		}
		}	
	}
	
	int search_number_range(const char*value_str,int type, int16_t offset)
	{
		if (!mem_p) return -1;
		switch (type)
		{
		case DWORD:
		{
			int value=atoi(value_str);
			mem_p->search_data_range<int32_t>(value,offset);
			break;
		}
		case WORD:
		{
			int16_t value=atoi(value_str);
			mem_p->search_data_range<int16_t>(value,offset);
			break;
		}
		case FLOAT:
		{
			float value=atof(value_str);
			mem_p->search_data_range<float>(value,offset);
			break;
		}
		case QWORD:
		{
			int64_t value=atoll(value_str);
			mem_p->search_data_range<long long>(value,offset);
			break;
		}
		case DOUBLE:
		{
			double value=atof(value_str);
			mem_p->search_data_range<double>(value,offset);
			break;
		}
		case BYTE:
		{
			int8_t value=atoi(value_str);
			mem_p->search_data_range<int8_t>(value,offset);
			break;
		}
		}
		
		return mem_p->addr_sum;
	}
	
	void clear_results()
	{
		if (!mem_p) return;
		mem_p->clear_results();
	}
	
	void set_freeze_time(long tim)
	{
		if (!mem_p) return;
		mem_p->set_freeze_time(tim);
	}
	
	void clear_freeze_results()
	{
		if (!mem_p) return;
		mem_p->clear_freeze_results();
	}
	
	void set_interval_time(long write_tim,long restore_tim)
	{
		if (!mem_p) return;
		mem_p->set_interval_time(write_tim,restore_tim);
	}
	
	void clear_interval_results()
	{
		if (!mem_p) return;
		mem_p->clear_interval_results();
	}
	
	void print_addr_value(int type,...)
	{
		if (!mem_p) return;
		va_list p_args;
		va_start(p_args, type);
		int val[7] = { type };
		for (int i = 0; i < 10 && val[i]; i++)
		{
			val[i + 1] = va_arg(p_args, int);
		}
		va_end(p_args);
		mem_p->print_addr_value(val);
	}
	
	void print_addr()
	{
		if (!mem_p) return;
		mem_p->print_addr();
	}

	void write_addr(const char*value_str, int type, long addr)
	{
		if (!mem_p) return;
		switch (type)
		{
		
		case DWORD:
		{
			int value=atoi(value_str);
			mem_p->write_addr<int32_t>(addr, value);
			break;
		}
		case WORD:
		{
			int16_t value=atoi(value_str);
			mem_p->write_addr<int16_t>(addr, value);
			break;
		}
		case FLOAT:
		{
			float value=atof(value_str);
			mem_p->write_addr<float>(addr, value);
			break;
		}
		case QWORD:
		{
			int64_t value=atoll(value_str);
			mem_p->write_addr<int64_t>(addr, value);
			break;
		}
		case DOUBLE:
		{
			double value=atof(value_str);
			mem_p->write_addr<double>(addr, value);
			break;
		}
		case BYTE:
		{
			int8_t value=atoi(value_str);
			mem_p->write_addr<int8_t>(addr, value);
			break;
		}
			
		}
	}

	void read_addr(void *val, int type, long addr)
	{
		if (!mem_p) return;
		switch (type)
		{
		case DWORD:
		{
			int temp= mem_p->read_addr<int32_t>(addr);
			memcpy(val,&temp ,4);
			break;
		}
		case WORD:
		{
			int16_t temp = mem_p->read_addr<int16_t>(addr);
			memcpy(val, &temp,2);
			break;			
		}
		case FLOAT:
		{
			float temp = mem_p->read_addr<float>(addr);
			memcpy(val, &temp, 4);
			break;
		}
		case QWORD:
		{
			int64_t temp = mem_p->read_addr<int64_t>(addr);
			memcpy(val, &temp, 8);
			break;
		}
		case DOUBLE:
		{
			double temp = mem_p->read_addr<double>(addr);
			memcpy(val, &temp,8);
			break;
		}
		case BYTE:
		{
			int8_t temp = mem_p->read_addr<int8_t>(addr);
			memcpy(val, &temp, 1);
			break;
		}
		}
	}

	void print_file(const char *filename)
	{
		if (filename)
		freopen(filename,"w+",stdout); //输出重定向，输出数据将保存在指定文件中 
	}
	
	void print_stdout()
	{		
		freopen("/dev/tty","w",stdout);		
	}
	
}
/*

int main(int argc, char **argv)
{
	
	kk::init(argv, "com.tencent.mobileqq",MODE_ROOT);
	kk::set_ranges(8,0);
	int sum=0;
	
//	sum = kk::search_number("226",DWORD,0); //范围搜索
//	sum = kk::search_number("234",DWORD,4); //范围搜索
	sum = kk::search_number("68",DWORD,0); //范围搜索
//	sum = kk::search_number("267",DWORD,4); //范围搜索
//	freopen("/storage/emulated/0/Code/memedit00/memedit00/cppmem/memedit00/out.txt","w",stdout); //输出重定向，输出数据将保存在D盘根目录下的out.txt文件中 
	kk::search_number_range("2",DWORD,1024);
	kk::print_addr_value(DWORD,WORD,FLOAT,DOUBLE,HEX_L,0);//打印地址的值
	
//	kk::edit_interval("77777",DWORD,0x8);
//	kk::edit_interval("77777",DWORD,0);
	freopen("/dev/tty","w",stdout);
	printf("搜索完成，找到%d个\n",sum);

	getchar();
	//
}

//---------------------------------*/