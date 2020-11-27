/**
 * memeditool>小工具<
 * 全杀v2.0
 * by.QAIU 
 * 
 **/

#include <cstdio>
#include <cstdlib>
#include <string>

#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

// AUTOUID：全杀级别，为0(默认)时表示允许全部系统应用运行
// 为n(n>0)表示可以运行n个(根据重要级别)系统应用，建议不超过120
// 为负数时表示不允许运行系统应用 
// 部分手机杀死系统应用可能会出问题，建议允许全部系统应用运行
#define AUTOUID 0

//为1表示调试模式，可以直接运行此源文件，检验效果
//为0表示仅仅作为头文件，只允许被调用
#define Debug 1


struct kill_list
{
	char packname[128];
	int pid;
	kill_list *next;
} *kill_head = NULL, *kill_next = NULL;


template <class T> 
void delete_list(T * &list)
{
	T *head;
	while (list)
	{
		head = list->next;
		delete list;
		list = head;
	}
}

const char *kill_whitelist[] = {
	"com.n0n3m4.droidd",
	"com.tencent.mobileqq",
	"com.android.chrome",
	"com.sf.ALuaGuide",
	"com.tencent.mm",
	"com.n0n3m4.droida",
	"com.n0n3m4.droidc",
	"com.ZSZZ.NEW",
	"com.tencent.ig",
	"com.tencent.tmgp.pubgmhd",
	"com.miui.home",
	"com.google.android.apps.translate",
	"sh",
	"su",
	"com.miui.notes",                                              
	"com.xiaomi.simactivate.service",
	"com.google.android.gms.persistent",
	"com.xiaomi.metoknlp",
	"/data/user/0/com.n0n3m4.droidd/files/busybox",
	"/data/user/0/com.n0n3m4.droidd/files/temp",
	"debuggerd",
	"/data/user/0/com.n0n3m4.droida/files/busybox",
	"/data/user/0/com.n0n3m4.droida/files/temp",
	"com.baidu.input",
	NULL
};

void get_root(char **argv);
void kill_all();
void kill_adb();

#if Debug
int main(int c, char **v)
{
	get_root(v);
	clock_t t0=clock();
	kill_all();
	
	clock_t t = clock();
	printf("用时%f秒", (double)(t-t0) / CLOCKS_PER_SEC);
	getchar();
	return 0;
}
#endif 

void get_root(char **argv)
{
	std::string su = "su -c ";
	su += *argv;
	if (getuid() != 0)
	{
		system(su.c_str());
		exit(1);
	}
}

int get_max_uid()
{
	#if AUTOUID == 0
	const char *sh ="pm list package -U -s";
	char buf[128]={};
	int uid;
	int max;
	FILE *fp = popen(sh,"r");
	
	while (fgets(buf,120,fp))
	{
		sscanf(buf,"%*s%*[^:]:%d",&uid);
		if (uid>max)
			max=uid;
	}
	if (max>10200&&max<10000)
	max=10050;
	pclose(fp);
	return max;
	#elif AUTOUID > 0
	return AUTOUID+10000;
	#else 
	return 10000;
	#endif
}

int get_kill_pid()
{
	int id;
	pid_t pid = -1;
	DIR *dir;
	FILE *fp;
	char filename[32];
	char cmdline[128];
	struct dirent *entry;
	int max=get_max_uid();
//	printf("%d\n",max);
	dir = opendir("/proc");
	if (dir == NULL)
		return -1;
	while ((entry = readdir(dir)) != NULL)
	{
		id = atoi(entry->d_name);
		std::string spid = "/proc/";
		spid += entry->d_name;

		struct stat mystat;
		stat(spid.c_str(), &mystat);
		if (mystat.st_uid > max && id >100 )
		{	
			if (kill_head == NULL)
			{
				kill_head = kill_next = new kill_list;
			}
			else
			{
				kill_next->next = new kill_list;
				kill_next = kill_next->next;
			}

			kill_next->pid = id;
			sprintf(filename, "/proc/%d/cmdline", id);
			fp = fopen(filename, "r");
			if (fp)
			{
				fgets(cmdline, 128, fp);
				strcpy(kill_next->packname, cmdline);
			}
		}
	}
	closedir(dir);
	kill_next = kill_head;
	return 0;
}

void kill_adb()
{
	const char *cmd="ps -A|grep adb";
	FILE *fp = popen(cmd,"r");
	char buf[256]={};
	int pid=0;
	
	fscanf(fp,"%*s%d",&pid);
	pclose(fp);
	if (pid>100)
	{
		kill(pid,SIGKILL);
	}
}

void kill_all()
{
	get_kill_pid();
	while (kill_next)
	{
		int kk=0;
		while (kill_whitelist[kk])
		{
			std::string service=kill_whitelist[kk];
			service += ":";
			if (
				!strcmp(kill_next->packname,kill_whitelist[kk])||
				!strncmp(kill_next->packname,service.c_str(),strlen(service.c_str())))
			{
				#if Debug
				printf("白名单免杀：%s\n",kill_next->packname);
				#endif
				goto k;
			}
			kk++;
		}
		kill(kill_next->pid,SIGKILL);
		#if Debug
		printf("杀死：%s\n",kill_next->packname);
		#endif
		k:
		kill_next=kill_next->next;
	}
	kill_adb();
	kill_next=kill_head;
	delete_list(kill_next);	
	kill_head=kill_next=NULL;
}
