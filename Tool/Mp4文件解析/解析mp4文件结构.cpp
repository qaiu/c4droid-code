/*
解析mp4
宏定义部分来自互联网
仅做少量修改

1，综述
mp4文件结构由一系列box构成
每个box均由box header和box data
box header由box size表示box大小box type表示box类型
有两类特殊box:扩展box和嵌套box
扩展box在header多了一个4字节表示版本64bit或32bit
嵌套box主要在moov box里构成moov数据信息

2，已知box
(星号表示为box目录下的文件夹，本身没有data，要依赖子box)
1.ftyp 必选box 文件头 大部分为28字节 有些为32字节有些24字节
2.moov *必选box 视频信息描述嵌套box 大小不固定
2.1mvhd 
2.2trak*
2.2.1tkhd
2.2.2mdia*
2.2.2.1mdhd
2.2.2.2hdlr
2.2.2.3minf*
2.2.2.3.1vmhd/smhd/hmhd/nmhd
2.2.2.3.2stbl*重点
{
    2.2.2.3.2.1 stsd
    2.2.2.3.2.2 stsz/sts2
    2.2.2.3.2.3 stsc
    2.2.2.3.2.4 stco/co64
    2.2.2.3.2.5 padb
    2.2.2.3.2.6 subs
    2.2.2.3.2.7 stts
    2.2.2.3.2.8 ctts
}
2.2.2.3.3dinf*
2.2.2.3.3.1 url/urn/dref
2.3下一个 trak*
......
3 mdata 视频数据
4.free 
5.uuid 可选box 用户自定义box根据实际需要解析
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//mp4文件box类型宏定义
#define FTYP 0x66747970 //@
#define MDAT_BOX 0x6D646174//@
#define MOOV_BOX 0x6D6F6F76//@
#define MVHD_BOX 0x6D766864//@
#define TRAK_BOX 0x7472616B//@
#define IODS_BOX 0x696F6473//?
#define UDTA_BOX 0x75647461//?
#define FREE_BOX 0x66726565//?
#define TKHD_BOX 0x746B6864//@
#define MDIA_BOX 0x6D646961//@
#define MDHD_BOX 0x6D646864//@
#define HDLR_BOX 0x68646C72//@
#define MINF_BOX 0x6D696E66//@
#define DINF_BOX 0x64696E66//@
#define DREF_BOX 0x64726566 
#define TREF_BOX 0x74726566//?
#define MP4S_BOX 0x6D703473
#define ESDS_BOX 0x65736473

#define STBL_BOX 0x7374626C//@
#define STSD_BOX 0x73747364//@
#define STTS_BOX 0x73747473
#define STSC_BOX 0x73747363//@
#define STSZ_BOX 0x7374737A
#define STCO_BOX 0x7374636F
#define STSS_BOX 0x73747373
#define CTTS_BOX 0x63747473
#define EDTS_BOX 0x65647473

#define VMHD_BOX 0x766D6864//@
#define HMHD_BOX 0x686D6864//@
#define SMHD_BOX 0x736D6864//@
#define NMHD_BOX 0xffffffff//@空媒体标题，仅限歌曲文件

#define ODSM	0x6F64736D
#define SDSM	0x7364736D
#define VIDE	0x76696465
#define HINT	0x68696E74
#define SOUN	0x736F756E

FILE *fp;//文件指针必须全局变量便于递归
typedef struct BOX_HEAD
{
	unsigned int size;
	unsigned int type;
}BOX_HEAD;

typedef struct FTYP_BOX
{
	char major_brand[4];
	unsigned int minor_version;
	char compatible_brands[12];
}FTYP_BOX;

typedef struct MOOV_HEADER_BOX
{
	unsigned int version;    //box版本
	unsigned int create_time;    //创建时间
	unsigned int modification_time;    //修改时间
	unsigned int time_scale;    //时间尺度
	unsigned int duration;    //时间长度
	unsigned int preferred_rate;//推荐播放速率
	unsigned short preferred_volume;//最大音量
	char reserved[10];    //保留位
	char matrix_structure[36];    //视频变换矩阵
	char pre_defined[24];    //预设
	unsigned int next_track_id;   //下一个track的ID
}MOOV_HEADER_BOX;

typedef struct Track_head_box
{
	unsigned int version;
	unsigned int create_time;//创建时间
	unsigned int modification_time;//修改时间
	unsigned int track_id;  //track ID
	unsigned int reserved1;//保留位
	unsigned int duration;  //该trak时间长度
	unsigned int reserved2[2];//保留位
	unsigned short layer;//视频层
	unsigned short alternate_group;//分组信息
	unsigned short volume;//音量
	unsigned short reserved3;//保留位
	char matrix[36];//视频转换帧
	unsigned int width;//视频宽度
	unsigned int height;//视频高度
}TRACK_HEAD_BOX;

typedef struct Mdhd_head_box
{
	unsigned int create_time; //创建时间
	unsigned int modification_time;  //修改时间
	unsigned int time_scale;  //时间尺度
	unsigned int duration;  //时间长度
	unsigned short language;  //
	unsigned short quality;
}MDHD_HEAD_BOX;

struct MP4_INFO
{
	unsigned int create_time;
	unsigned int modification_time;
}mp4_info;

/*
下面的几个函数进行视频信息时间数据转日期
*/
//时间结构体
typedef struct
{//儒略历 年 月 日 时 分 秒 (浮点型)
	double Y,M,D,h,m,s;
}Date;
double toJD(Date date)
{ //公历转儒略日
   double y=date.Y, m=date.M, n=0; //取出年月
 //  if (y<=0) y++; //处理公元前
   if(m<=2) m+=12,y--;
   if(date.Y*372+date.M*31+date.D>=588829)
   //判断是否为格里高利历日1582*372+10*31+15
   n =(int)(y/100), n =2-n+(int)(n/4);//加百年闰
   n +=(int)(365.25*(y+4716)+0.01);    //加上年引起的偏移日数
   n +=(int)(30.6*(m+1))+date.D;       //加上月引起的偏移日数及日偏移数
   n +=((date.s/60+date.m)/60+date.h)/24 - 1524.5;
   return n;
}

Date  setFromJD(double jd)
{ //儒略日数转公历
   jd+=0.5;
   Date date;
   double A=(int)(jd), F=jd-A, D;  //取得日数的整数部份A及小数部分F
   if(A>=2299161)
   D=(int)((A-1867216.25)/36524.25),A+=1+D-(int)(D/4);
   A += 1524; //向前移4年零2个月
   date.Y =(int)((A-122.1)/365.25);//年
   D=A-(int)(365.25*date.Y); //去除整年日数后余下日数
   date.M =(int)(D/30.6001);       //月数
   date.D =D-(int)(date.M*30.6001);//去除整月日数后余下日数
   date.Y-=4716;
   date.M--;
   if (date.M>12) date.M-=12;
   if (date.M<=2) date.Y++;
   //日的小数转为时分秒
   F*=24; date.h=(int)(F); F-=date.h;
   F*=60; date.m=(int)(F); F-=date.m;
   F*=60; date.s=F;
   return date;
}

Date s2d(unsigned int s)//转1904为基点秒数为日期(北京时间)
{
	Date d0={1904,1,1,0,0,0};
	return setFromJD((double)s/3600/24+8.0/24+toJD(d0));	
}

int b2s(int const big)
{
	int data = big;
	int ret = 0;
	ret += (data>>0 & 0xff) << 24;
	ret += (data>>8 & 0xff) << 16;
	ret += (data>>16 & 0xff) << 8;
	ret += (data>>24 & 0xff) << 0;
	return ret;
}

void print_other_box(unsigned int type)
{
	char s[5]={0};
	int i=3;
	while (type)
	{
		s[i--]=type%0x100;
		type /= 0x100;
	}
	printf("%s",s);
}
void fread_mp4(FILE *fp)
{

	BOX_HEAD box_head;
	int flag=0;
	long int xsize=0;
	if (fread(&box_head,sizeof(BOX_HEAD),1,fp)==0)
	{
		return;
	}
	box_head.size=b2s(box_head.size);
	box_head.type=b2s(box_head.type);

	if (box_head.size==0)
	{
  	//fread(&xsize,sizeof(long),1,fp);
  	//box_head.size=xsize;
	}else if(box_head.size==1)
	{
		printf("dmata size=1 read over!");
		return ;
	}
	
	switch (box_head.type)
	{
		case FTYP:
		{
			printf("读取ftyp");
			FTYP_BOX ftyp;
			fread(&ftyp,sizeof(FTYP_BOX),1,fp);
			fseek(fp,-sizeof(FTYP_BOX),SEEK_CUR);

			printf("\nmajor_brand:%s",ftyp.major_brand);
			printf("\ncompatible_brands:%s",ftyp.compatible_brands);
		}
		break;
		case MOOV_BOX:
		{
			printf("读取MOOV");
			flag=1;
		}
		break;
		case MVHD_BOX:
		{
			printf("读取mdhd");
			MOOV_HEADER_BOX mvhd;
			fread(&mvhd,sizeof(MOOV_HEADER_BOX),1,fp);
			fseek(fp,-sizeof(MOOV_HEADER_BOX),SEEK_CUR);
			Date d = s2d(b2s(mvhd.create_time));
			printf("\n创建时间:%g/%g/%g  %g:%g:%0.0lf",d.Y,d.M,d.D,d.h,d.m,d.s);
		}
		break;
		case TRAK_BOX:
		{
			flag=1;
			printf("读取TRAK");
		}
		break;
		case TKHD_BOX:
		{
			printf("读取tkhd");
		}
		break;
		case EDTS_BOX:
		{
			printf("读取edts");
		}
		break;
		case MDIA_BOX:
		{
			flag=1;
			printf("读取MDIA");
		}
		break;
		
		case MDHD_BOX:
		{
			printf("读取mdhd");
		}
		break;
		case HDLR_BOX:
		{
			printf("读取hdlr");
		}
		break;
		
		case MINF_BOX:
		{
			flag=1;
			printf("读取MINF");
		}
		break;
		
		case VMHD_BOX:
		{
			printf("读取vmhd");
		}
		break;
		case SMHD_BOX:
		{
			printf("读取smhd");
		}
		break;
		case HMHD_BOX:
		{
			printf("读取hmhd");
		}
		break;
		case NMHD_BOX:
		{
			printf("读取nmhd");
		}
		break;
		
		case STBL_BOX:
		{
			flag=1;
			printf("读取STBL");
		}
		break;
		
		case STSD_BOX:
		{
			printf("读取stsd");
		}
		break;
		case STTS_BOX:
		{
			printf("读取stts");
		}
		break;
		case STSC_BOX:
		{
			printf("读取stsc");
		}
		break;
		case STSZ_BOX:
		{
			printf("读取stsz");
		}
		break;
		case STCO_BOX:
		{
			printf("读取stco");
		}
		break;
		case STSS_BOX:
		{
			printf("读取stss");
		}
		break;
		case CTTS_BOX:
		{
			printf("读取ctts");
		}
		break;
		case DREF_BOX:
		{
	//		flag=1;
			printf("读取dref");
		}
		break;
		case DINF_BOX:
		{
			flag=1;
			printf("读取DINF");
		}
		break;
		case FREE_BOX:
		{
			printf("读取free");
		}
		break;
		
		case MDAT_BOX:
		{
			printf("读取mdat");
		}
		break;
		default :
		{
			printf("未知box:");
			print_other_box(box_head.type);
		}
		break;
	}
	printf("\tsize %ld\n",box_head.size);
	if (!flag)
	fseek(fp,box_head.size-8,SEEK_CUR);
	fread_mp4(fp);
}

int main(void )
{
	Date d;
	FTYP_BOX *ftyp = (FTYP_BOX*)malloc(sizeof(FTYP_BOX));
	MOOV_HEADER_BOX *moov_header=(MOOV_HEADER_BOX*)malloc(sizeof(MOOV_HEADER_BOX));
	char fname[128]="/storage/emulated/0/Code/Code/56C61532E5EB6A252DFE3E69DC7A9F18.mp4";//读取路径
	if ((fp = fopen(fname,"rb"))==NULL)
	{
		printf("error:No file!");
		return -1;
	}
	fread_mp4(fp);
	printf("OK");
	return 0;
}