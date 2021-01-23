/**
 *  Analysis/midi.c[v1.0]
 *  by 白狼圣狂
 *  act 解析MIDI乐谱(音频)文件
 */

#ifndef ANALYSIS_MIDI_C
#define ANALYSIS_MIDI_C

#ifdef __cplusplus
extern "C" {
#endif

#include "midi.h"

#define GETAUTOLEN(x)((1 + (((x >> 7) & 0x7F) ? 1 : 0) + (((x >> 14) & 0x7F) ? 1 : 0) + (((x >> 21) & 0x7F) ? 1 : 0)))

static long _ANS_GetFileSurplus(FILE* fp)
{
	long offset;
	long surplus;
	
	if (!fp)
		return ANS_MIDI_ARGSERR;
	
	offset = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	surplus = ftell(fp) - offset;
	fseek(fp, offset, SEEK_SET);
	
	return surplus;
}

static int _ANS_GetMidText(FILE* fpmid, char* sbuf)
{
	if (!(fpmid && sbuf))
		return ANS_MIDI_ARGSERR;
	
	if (_ANS_GetFileSurplus(fpmid) < 4)
		return ANS_MIDI_SURPLUSERR;
		
	fscanf(fpmid, "%4s", sbuf);
	
	return ANS_MIDI_NORMAL;
}

static int _ANS_GetFileBigByte(FILE* fp, void* pbuf, int isize)
{
	int i;
	
	if (!(fp && pbuf && isize > 0))
		return ANS_MIDI_ARGSERR;
	
	if (_ANS_GetFileSurplus(fp) < isize)
		return ANS_MIDI_SURPLUSERR;
	
	for (i = isize - 1; i >= 0; i--)
		fread((char*)pbuf + i, 1, 1, fp);
	
	return ANS_MIDI_NORMAL;
}

static int _ANS_GetMidAutoByte(FILE* fpmid)
{
	int tmp;
	int size = 0;
	
	if (!fpmid)
		return ANS_MIDI_ARGSERR;
	
	while ((tmp = fgetc(fpmid)) >= 0x80)
	{
		if (tmp == EOF)
			return ANS_MIDI_SURPLUSERR;
		size = (size << 7) | (tmp & 0x7F);
	}
	size = (size << 7) | (tmp & 0x7F);
	
	return size;
}

static int _ANS_CheckMid(FILE* fpmid)
{
	char name[5];
	uint32_t size;
	uint16_t type;
	uint16_t channels;
	uint16_t unittick;
	
	int i;
	
	if (!fpmid)
		return ANS_MIDI_ARGSERR;
	
	if (_ANS_GetFileSurplus(fpmid) < 14)
		return ANS_MIDI_SURPLUSERR;
	
	_ANS_GetMidText(fpmid, name);
	_ANS_GetFileBigByte(fpmid, &size, 4);
	_ANS_GetFileBigByte(fpmid, &type, 2);
	_ANS_GetFileBigByte(fpmid, &channels, 2);
	_ANS_GetFileBigByte(fpmid, &unittick, 2);
	
	if (strcmp(name, "MThd") || size != 6 || type > 1 || !unittick || unittick >= 0x8000)
	{
		rewind(fpmid);
		return ANS_MIDI_OTHERERR;
	}
	
	for (i = 0; i < channels; i++)
	{
		if (_ANS_GetFileSurplus(fpmid) < 8)
		{
			rewind(fpmid);
			return ANS_MIDI_SURPLUSERR;
		}
		
		_ANS_GetMidText(fpmid, name);
		_ANS_GetFileBigByte(fpmid, &size, 4);
		
		if (strcmp(name, "MTrk") || !size)
		{
			rewind(fpmid);
			return ANS_MIDI_OTHERERR;
		}
		
		if (_ANS_GetFileSurplus(fpmid) < size)
		{
			rewind(fpmid);
			return ANS_MIDI_SURPLUSERR;
		}
		
		fseek(fpmid, (long )size, SEEK_CUR);
	}
	
	rewind(fpmid);
	return ANS_MIDI_NORMAL;
}

static int _ANS_GetMidChunkCount(FILE* fpmid, int isize)
{
	int count = 0;
	long offset;
	uint8_t last;
	uint8_t byte;
	
	int tmp;
	
	if (!(fpmid && isize ))
		return ANS_MIDI_ARGSERR;
	
	offset = ftell(fpmid);
	
	while (isize > 0)
	{
		last = byte;
		
		tmp = GETAUTOLEN(_ANS_GetMidAutoByte(fpmid)) + 1;
		_ANS_GetFileBigByte(fpmid, &byte, 1);
		tmp++;
		
		switch (byte & 0xF0)
		{
			case 0xC0:
			case 0xD0:
				fseek(fpmid, 1L, SEEK_CUR);
				tmp++;
				break;
			
			case 0x80:
				count++;
				fseek(fpmid, 2L, SEEK_CUR);
				tmp += 2;
				break;
			
			case 0x90:
				count++;
				fseek(fpmid, 2L, SEEK_CUR);
				tmp += 2;
				break;
			
			case 0xA0:
			case 0xB0:
				fseek(fpmid, 2L, SEEK_CUR);
				tmp += 2;
				break;
			
			case 0xF0:
				if (byte == 0xFF)
				{
					fseek(fpmid, 1L, SEEK_CUR);
					isize -= tmp + 1;
					fseek(fpmid, (long )(tmp = _ANS_GetMidAutoByte(fpmid)), SEEK_CUR);
					tmp = GETAUTOLEN(tmp) + 1;
				}
				else if (byte == 0xF0)
				{
					isize -= tmp;
					fseek(fpmid, (long )((tmp = _ANS_GetMidAutoByte(fpmid)) + 1), SEEK_CUR);
					tmp = GETAUTOLEN(tmp) + 2;
				}
				else
				{
					fseek(fpmid, offset, SEEK_SET);
					return count;
				}
				break;
			
			default:
				byte = last;
				continue;
		}
		
		isize -= tmp;
	}
	
	fseek(fpmid, offset, SEEK_SET);
	return count;
}

// TODO
static int _ANS_GetMidChunk(FILE* fpmid, ANS_MidChunk* chunk, int count)
{
	long offset;
	uint32_t ticks = 0;
	uint8_t last;
	uint8_t byte;
	
	int i;
	
	if (!(fpmid && chunk && count > 0))
		return ANS_MIDI_ARGSERR;
	
	offset = ftell(fpmid);
	
	for (i = 0; i < count; i++)
	{
		last = byte;
		
		ticks += _ANS_GetMidAutoByte(fpmid);
		_ANS_GetFileBigByte(fpmid, &byte, 1);
		chunk[i].tickstamp = ticks;
		
		switch (byte & 0xF0)
		{
			case 0xC0:
			case 0xD0:
				fseek(fpmid, 1L, SEEK_CUR);
				i--;
				break;
			
			case 0x80:
				chunk[i].action = ANS_MIDI_ACTION_PRESSED;
				_ANS_GetFileBigByte(fpmid, &chunk[i].musicnote, 1);
				fseek(fpmid, 1L, SEEK_CUR);
				break;
			
			case 0x90:
				chunk[i].action = ANS_MIDI_ACTION_RELEASED;
				_ANS_GetFileBigByte(fpmid, &chunk[i].musicnote, 1);
				fseek(fpmid, 1L, SEEK_CUR);
				break;
			
			case 0xA0:
			case 0xB0:
				fseek(fpmid, 2L, SEEK_CUR);
				i--;
				break;
			
			case 0xF0:
				if (byte == 0xFF)
				{
					fseek(fpmid, 1L, SEEK_CUR);
					fseek(fpmid, (long ) _ANS_GetMidAutoByte(fpmid), SEEK_CUR);
				}
				else if (byte == 0xF0)
				{
					fseek(fpmid, (long )( _ANS_GetMidAutoByte(fpmid) + 1), SEEK_CUR);
				}
				else
				{
					fseek(fpmid, offset, SEEK_SET);
					return count;
				}
				i--;
				break;
			
			default:
				byte = last;
				continue;
		}
	}
	
	fseek(fpmid, offset, SEEK_SET);
	return count;
}

static ANS_Mid* _ANS_ReadMid(FILE* fpmid)
{
	ANS_Mid* mid;
	
	uint32_t size;
	int i, j;
	
	if (!fpmid)
		return NULL;
	
	if (!(mid = (ANS_Mid*)malloc(sizeof (ANS_Mid))))
		return NULL;
	
	fseek(fpmid, 10L, SEEK_SET);
	_ANS_GetFileBigByte(fpmid, &mid->channels, 2);
	_ANS_GetFileBigByte(fpmid, &mid->unittick, 2);
	mid->unittick *= mid->channels;
	
	if (!(mid->counts = (uint32_t*)malloc(sizeof (uint32_t) * mid->channels)))
	{
		free(mid);
		return NULL;
	}
	
	if (!(mid->chunks = (ANS_MidChunk**)malloc(sizeof (ANS_MidChunk*) * mid->channels)))
	{
		free(mid->counts);
		free(mid);
		return NULL;
	}
	
	for (i = 0; i < mid->channels; i++)
	{
		fseek(fpmid, 4L, SEEK_CUR);
		_ANS_GetFileBigByte(fpmid, &size, 4);
		
		mid->counts[i] = _ANS_GetMidChunkCount(fpmid, size);
		if (!(mid->chunks[i] = (ANS_MidChunk*)malloc(sizeof (ANS_MidChunk) * mid->counts[i])))
		{
			for (j = 0; j < i; j++)
				free(mid->chunks[i]);
			free(mid->chunks);
			free(mid->counts);
			free(mid);
			return NULL;
		}
		_ANS_GetMidChunk(fpmid, mid->chunks[i], mid->counts[i]);
		fseek(fpmid, (long )size, SEEK_CUR);
	}
	
	return mid;
}

ANS_Mid* ANS_LoadMid(const char* path)
{
	FILE* fpmid;
	ANS_Mid* mid;
	
	if (!path)
		return NULL;
	
	if (!(fpmid = fopen(path, "rb")))
		return NULL;
	
	if (_ANS_CheckMid(fpmid) != ANS_MIDI_NORMAL)
	{
		fclose(fpmid);
		return NULL;
	}
	
	mid = _ANS_ReadMid(fpmid);
	
	fclose(fpmid);
	return mid;
}

int ANS_FreeMid(ANS_Mid* mid)
{
	int i;
	
	if (!mid)
		return ANS_MIDI_ARGSERR;
	
	free(mid->counts);
	for (i = 0; i < mid->channels; i++)
		free(mid->chunks[i]);
	free(mid->chunks);
	
	return ANS_MIDI_NORMAL;
}

static int _ANS_QuickSortCallBack(const void* a, const void* b)
{
	return ((ANS_MidChunk*)a)->tickstamp < ((ANS_MidChunk*)b)->tickstamp ? -1 : 1;
}

int ANS_GetMidStream(const ANS_Mid* mid, ANS_MidChunk** chunks)
{
	int i;
	int tmp = 0;
	int count = 0;
	
	if (!(mid && mid->channels && chunks))
		return ANS_MIDI_ARGSERR;
	
	for (i = 0; i < mid->channels; i++)
		count += mid->counts[i];
	
	if (!(*chunks = (ANS_MidChunk*)malloc(sizeof (ANS_MidChunk) * count)))
		return ANS_MIDI_STORAGEERR;
	
	for (i = 0; i < mid->channels; i++)
	{
		memcpy(*chunks + tmp, mid->chunks[i], sizeof (ANS_MidChunk) * mid->counts[i]);
		tmp += mid->counts[i];
	}
	
	qsort(*chunks, count, sizeof (ANS_MidChunk), _ANS_QuickSortCallBack);
	
	return count;
}

int ANS_DelMidStream(ANS_MidChunk* chunks)
{
	if (!chunks)
		return ANS_MIDI_ARGSERR;
	
	free(chunks);
	
	return ANS_MIDI_NORMAL;
}

#ifdef __cplusplus
}
#endif

#endif