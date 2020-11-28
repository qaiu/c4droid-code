/*
OpenSLES播放音频
by:千百度
修改自CSDN论坛
原文链接:http://m.blog.csdn.net/tanningzhong/article/details/48002101
*/
#ifndef MUSIC_H
#define MUSIC_H

#include <SLES/OpenSLES.h>
#include <stdbool.h>
#include <assert.h>

/* engine interface */
static SLObjectItf engineObject = NULL;
static SLEngineItf engineEngine;

/* output mix interfaces */
static SLObjectItf outputMixObject = NULL;
static SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;

/* aux effect on the output mix */
static const SLEnvironmentalReverbSettings reverbSettings =
SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;

/* uri player interfaces */
static SLObjectItf uriPlayerObject = NULL;
static SLPlayItf uriPlayerPlay;
static SLSeekItf uriPlayerSeek;

void createEngine()
{
    SLresult result;

    // create engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    assert(SL_RESULT_SUCCESS == result);

    // realize the engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);

    // get the engine interface
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    assert(SL_RESULT_SUCCESS == result);

    // create output mix
    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);
    assert(SL_RESULT_SUCCESS == result);

    // realize the output mix
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    assert(SL_RESULT_SUCCESS == result);
    
        #if 1
    // get the environmental reverb interface
    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                              &outputMixEnvironmentalReverb);
    if (SL_RESULT_SUCCESS == result) {
        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(outputMixEnvironmentalReverb, &reverbSettings);
    }
    #endif
}

bool createUriAudioPlayer(char* uri)
{
    SLresult result;

    // config audio source
    SLDataLocator_URI loc_uri = {SL_DATALOCATOR_URI, (SLchar *) uri};
    SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource audioSrc = {&loc_uri, &format_mime};

    // config audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // create audio player
    const SLInterfaceID ids[1] = {SL_IID_SEEK};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &uriPlayerObject, &audioSrc, &audioSnk, 1, ids, req);
    assert(SL_RESULT_SUCCESS == result);

    // realize the player
    result = (*uriPlayerObject)->Realize(uriPlayerObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        (*uriPlayerObject)->Destroy(uriPlayerObject);
        uriPlayerObject = NULL;
        return false;
    }

    // get the play interface
    result = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_PLAY, &uriPlayerPlay);
    assert(SL_RESULT_SUCCESS == result);

    // get the seek interface
 //   result = (*uriPlayerObject)->GetInterface(uriPlayerObject, SL_IID_SEEK, &uriPlayerSeek);
    assert(SL_RESULT_SUCCESS == result);

    // enable whole file looping
    /*
    result = (*uriPlayerSeek)->SetLoop(uriPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
    assert(SL_RESULT_SUCCESS == result);
    */

    return true;
}

void setPlayingUriAudioPlayer(bool played)
{
    SLresult result;
    if (uriPlayerPlay != NULL) {
        result = (*uriPlayerPlay)->SetPlayState(uriPlayerPlay, played ?
                                                SL_PLAYSTATE_PLAYING : SL_PLAYSTATE_PAUSED);
        assert(SL_RESULT_SUCCESS == result);
    }
}

void PlayShutdown()
{
	
	if (uriPlayerObject != NULL)
	{
		(*uriPlayerObject)->Destroy(uriPlayerObject);
		uriPlayerObject = NULL;
		uriPlayerPlay = NULL;
	}
	//destroy mixer
	if (outputMixObject!= NULL)
	{
		(*outputMixObject)->Destroy(outputMixObject);
		outputMixObject= NULL;
		outputMixEnvironmentalReverb = NULL;
	}
	//destroy engine interface 
	if (engineObject!= NULL)
	{
		(*engineObject)->Destroy(engineObject);
		engineObject= NULL;
	}
}

#endif