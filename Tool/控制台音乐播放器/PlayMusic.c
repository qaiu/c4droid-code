/*
           
     源码：音乐播放
     作者：傻傻_痴痴
     来源：c4droid吧（欢迎加入交流）
     
     日期：2014.3.30
     
        若要转载源码，请注明作者出处，禁止修改为自己名义发布源码
 
 
*/


#include <SLES/OpenSLES.h>
#include <stdbool.h>
#include <assert.h>
#include "finddir.c"

/*播放状态：未播放、正在播放、暂停*/
#define NOPLAY 1
#define PLAYING 2
#define PAUSED 3

/*当前播放歌曲编号*/
int ListNum = 0; 
/*播放状态，初始化为未播放*/
int PLAYSTATE=NOPLAY;

/* 声音对象接口*/
static SLObjectItf engine_m = NULL;
/*声音引擎接口*/
static SLEngineItf engine_e;

/* 混音器对象接口 */
static SLObjectItf output_mix = NULL;
/*混音器输出池接口*/
static SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;

	/* 环境回响音效*/
static const SLEnvironmentalReverbSettings reverbSettings =	SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

	/* 播放声音源对象接口*/
static SLObjectItf uriPlayerObject = NULL;
static SLPlayItf uriPlayerPlay;

void PlayNext(int *Num);
void PlayShutdown();

/*创建声音引擎函数*/
void createEngine()
{
	SLresult result;
	/*创建声音引擎*/
	result = slCreateEngine(&engine_m, 0, NULL, 0, NULL, NULL);
	/*返回结果，如果失败则出现提示*/
	assert(SL_RESULT_SUCCESS == result);
	/*实现声音引擎*/
	result = (*engine_m)->Realize(engine_m, SL_BOOLEAN_FALSE);
	assert(SL_RESULT_SUCCESS == result);

	/*获取声音引擎接口*/
	result =	(*engine_m)->GetInterface(engine_m, SL_IID_ENGINE,&engine_e);
	assert(SL_RESULT_SUCCESS == result);
	/*设置播放器功能，环境回响效果，音量*/
	const SLInterfaceID ids[] = { SL_IID_ENVIRONMENTALREVERB,
};
	/*设置是否强制实现实现*/
	const SLboolean req[] = {		SL_BOOLEAN_FALSE,	};
	/*从声音引擎创建混音器*/
	result =	(*engine_e)->CreateOutputMix(engine_e, &output_mix,1, ids, req);
	assert(SL_RESULT_SUCCESS == result);

	/*实现混音器效果*/
	result = (*output_mix)->Realize(output_mix, SL_BOOLEAN_FALSE);
	assert(SL_RESULT_SUCCESS == result);

#if 1
	/* 获取接口混音器接口并放到混音器输出池*/
	result =	(*output_mix)->GetInterface(output_mix, SL_IID_ENVIRONMENTALREVERB, &outputMixEnvironmentalReverb);
	if (SL_RESULT_SUCCESS == result)
	{
		result =	(*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties
			(outputMixEnvironmentalReverb, &reverbSettings);
	}
#endif

}

/*监听是否播放结束*/
void playOverEvent(SLPlayItf caller, void *pContext, SLuint32 playevent)
{
	if (playevent ==SL_PLAYEVENT_HEADATEND)
	{
		/*播放结束自动下一曲*/
		
		printf("播放完毕\n");
		PLAYSTATE=NOPLAY;
		PlayNext(&ListNum);
	}
}

/*加载声音源*/

bool LoadMUS(char *uriPath)
{
	SLresult result;
	SLDataLocator_URI loc_uri = { SL_DATALOCATOR_URI, (SLchar *) uriPath};
	/*数据格式*/
	SLDataFormat_MIME format_mime=
		{ SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED };
	SLDataSource audioSrc = { &loc_uri, &format_mime };

	/*混音器数据*/
	SLDataLocator_OutputMix loc_outmix =
		{ SL_DATALOCATOR_OUTPUTMIX, output_mix};
	SLDataSink audioSnk = { &loc_outmix, NULL };
	result =	(*engine_e)->CreateAudioPlayer(engine_e, &uriPlayerObject,&audioSrc, &audioSnk, 0,NULL,NULL);
	assert(SL_RESULT_SUCCESS == result);
	/*实现播放对象*/
	result = (*uriPlayerObject)->Realize(uriPlayerObject, SL_BOOLEAN_FALSE);
	if (SL_RESULT_SUCCESS != result)
	{
		(*uriPlayerObject)->Destroy(uriPlayerObject);
		uriPlayerObject = NULL;
		return false;
	}
	SLBufferQueueItf _aud_buf;
	/*获取播放接口*/
	result =	(*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_PLAY,&uriPlayerPlay);
	assert(SL_RESULT_SUCCESS == result);
	/*设置播放接口监听*/
	result =	(*uriPlayerPlay)->RegisterCallback(uriPlayerPlay, playOverEvent, NULL);
	result =	(*uriPlayerPlay)->SetCallbackEventsMask(uriPlayerPlay,SL_PLAYEVENT_HEADATEND);
	assert(SL_RESULT_SUCCESS == result);
	return true;
}

/*设置播放状态*/
void setPlayingUriAudioPlayer(bool played)
{
	SLresult result;
	if (uriPlayerPlay != NULL)
	{
		result =	(*uriPlayerPlay)->SetPlayState(uriPlayerPlay,played ? SL_PLAYSTATE_PLAYING : SL_PLAYSTATE_PAUSED);
		assert(SL_RESULT_SUCCESS == result);
	}
}


/*播放歌曲*/
void PlayMUS(char *MUSPath)
{
	/*不同状态，执行不同*/
	switch (PLAYSTATE)
	{	
	 case NOPLAY:
		LoadMUS(MUSPath);
		setPlayingUriAudioPlayer(true);
		PLAYSTATE=PLAYING;
		break;
	case PLAYING:
		setPlayingUriAudioPlayer(false);
		PlayShutdown();
		createEngine();
		LoadMUS(MUSPath);
		setPlayingUriAudioPlayer(true);
		break;
	case PAUSED:
		PlayShutdown();
		createEngine();
		LoadMUS(MUSPath);
		setPlayingUriAudioPlayer(true);
		PLAYSTATE=PLAYING;
		break;
	}
}

/*播放下一曲*/
void PlayNext(int *Num)
{
	*Num += 1;
	if (*Num > MusicNum)
	{
		*Num = 1;
	}
	struct song *ps = indexSong(MySong, *Num);
	if (ps!=NULL)
	{
	PlayMUS(ps->path);
	printf("下一曲： %s\n", ps->name);
	}
}

/*播放上一曲*/
void PlayUpper(int *Num)
{
	*Num -= 1;
	if (*Num <= 0)
	{
		*Num = MusicNum;
	}
	struct song *ps = indexSong(MySong, *Num);
	if (ps!=NULL)
	{
	PlayMUS(ps->path);
	printf("上一曲： %s\n", ps->name);
	}
}


/*暂停歌曲*/
void PauseMUS()
{
	switch (PLAYSTATE)
	{
	case PLAYING:
		setPlayingUriAudioPlayer(false);
		printf("暂停播放\n");
		PLAYSTATE=PAUSED;
		break;
	case PAUSED:
			setPlayingUriAudioPlayer(true);
			printf("恢复播放\n");
			PLAYSTATE=PLAYING;
			break;
	case NOPLAY:
			ListNum = 1;
			struct song *ps = indexSong(MySong, ListNum);
			PlayMUS(ps->path);
			printf("播放： %s\n", ps->name);
			PLAYSTATE=PLAYING;
			break;
	}
}

/*停止，关闭声音*/
void PlayShutdown()
{
	if (uriPlayerObject != NULL)
	{
		(*uriPlayerObject)->Destroy(uriPlayerObject);
		uriPlayerObject = NULL;
		uriPlayerPlay = NULL;
	}
	/*销毁混音器*/
	if (output_mix!= NULL)
	{
		(*output_mix)->Destroy(output_mix);
		output_mix= NULL;
		outputMixEnvironmentalReverb = NULL;
	}
	/*销毁声音引擎声音对象接口*/
	if (engine_m!= NULL)
	{
		(*engine_m)->Destroy(engine_m);
		engine_m= NULL;
	}
}

/*启动音乐*/
bool InitMusic()
{
	/*创建声音引擎*/
	createEngine();
	if (findFinish())
	{
#ifdef _TERMINAL_
		printf("扫描666完毕…\n");
		if (MySong==NULL)
		{
			printf("未发现歌曲！\n");
			exit(0);
		}
#endif
	}
        return true;
}

/*搜索歌曲，不符合直接过滤*/
bool FilterSong(char *name, char *key)
{
	if (strstr(name, key) != NULL)
	{
		return false;
	}
	return true;
}

/*搜索歌曲*/
struct song *search(char *key)
{
	struct song *ss = MySong;
	struct song *result, *r1, *r2;
	r1 = r2 = (struct song *)malloc(sizeof(struct song ));
	result = NULL;
	while (ss != NULL)
	{
		if (!FilterSong(ss->name, key))
		{
			r1->name = (char *)malloc(sizeof(ss->name));
			r1->name = ss->name;
			if (result == NULL)
			{
				result = r1;
			}
			else
			{
				r2->next = r1;
			}
			r2 = r1;
			r1 = (struct song *)malloc(sizeof(struct song *));
		}
		ss = ss->next;
	}
	r2->next = NULL;
	return result;
}