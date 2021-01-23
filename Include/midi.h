/**
 *  Analysis/midi.h[v1.0]
 *  by 白狼圣狂
 *  act 解析MIDI乐谱(音频)文件
 */ 

#ifndef ANALYSIS_MIDI_H
#define ANALYSIS_MIDI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *  函数返回值
 */
typedef enum ANS_MIDIVALUESTATETYPE
{
	ANS_MIDI_NORMAL = 0, // 正常
	ANS_MIDI_ARGSERR = -1, // 参数错误
	ANS_MIDI_STORAGEERR = -2, // 内存不足
	ANS_MIDI_SURPLUSERR = -3, // 文件剩余量不够/EOF
	ANS_MIDI_OTHERERR = -4 // 其它错误
}ANS_MidValueStateType;

/*
 *  动作类型
 */
typedef enum ANS_MIDIACTIONTYPE
{
	ANS_MIDI_ACTION_RELEASED, // 松开(停止)
	ANS_MIDI_ACTION_PRESSED // 按下(开始)
} ANS_MidActionType;

/*
 *  音符块
 */
typedef struct ANS_MIDICHUNK
{
	uint32_t tickstamp; // 时间戳
	ANS_MidActionType action; // 动作
	uint8_t musicnote; // 音符
} ANS_MidChunk;

/*
 *  乐谱结构体
 */
typedef struct ANS_MIDI
{
	uint16_t unittick; // 单位时间(一拍)
	uint16_t channels; // 并行频道数
	uint32_t* counts; // 每个频道的音乐块数
	ANS_MidChunk** chunks; // 音乐块堆
} ANS_Mid;

/*
 *  加载MIDI文件(.mid)
 *  成功返回乐谱结构体，失败返回NULL
 */
extern ANS_Mid* ANS_LoadMid(const char* path);

/*
 *  释放乐谱结构体
 *  成功返回ANS_MIDI_NORMAL
 */
extern int ANS_FreeMid(ANS_Mid* mid);

/*
 *  从乐谱结构体里提取音符流并保证时间顺序是从前到后
 *  成功返回音符块数量
 * 注意传入chunks的是个空的ANS_MidChunk*的地址
 */
extern int ANS_GetMidStream(const ANS_Mid* mid, ANS_MidChunk** chunks);

/*
 *  删除提取的流
 *  成功返回ANS_MIDI_NORMAL
 */
extern int ANS_DelMidStream(ANS_MidChunk* chunks);

#include "midi.c"

#ifdef __cplusplus
}
#endif

#endif