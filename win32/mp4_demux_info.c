/**
 * @file mp4_demux_test.c
 * @brief MP4 file library - demuxer test program
 * @date 07/11/2016
 * @author aurelien.barre@akaaba.net
 *
 * Copyright (c) 2016 Aurelien Barre <aurelien.barre@akaaba.net>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *
 *   * Neither the name of the copyright holder nor the names of the
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include <libmp4.h>
#include <mp4.h>
#include "mp4_demux_info.h"

#ifdef USE_BOOL
#define bool char
#define true 1
#define false 0
#endif

typedef struct VodFileInfo
{
	int Duration;    /* 点播文件总时长 单位:ms */
	int FrameCount;  /* 点播文件总帧数 */
	int FrameRate;   /* 点播文件的帧率 */
	int Width;       /* 点播文件的帧高 */
	int Height;      /* 点播文件的帧宽 */
} VODFileInfo_SRUT;

static int mp4_demux_get_tracks_video(struct mp4_demux *demux, VODFileInfo_SRUT *pInfo)
{
	struct mp4_file *mp4 = NULL;
	struct mp4_track *track = NULL;
	VODFileInfo_SRUT vodFileInfo;
	int i= 0;
	mp4 = &demux->mp4;

	list_walk_entry_forward(&mp4->tracks, track, node) {
		{
			printf("Track #%d ID=%d\n", i, track->id);
			switch (track->type) {
			case MP4_TRACK_TYPE_VIDEO:
				printf("  type: video\n");
				printf("  sampleCount=%d\n",
					track->sampleCount);
				printf("  timescale=%d\n",
					track->timescale);
				printf("  duration=%d\n",
					track->duration);
				printf("  width=%d\n",
					track->videoWidth);
				printf("  height=%d\n",
					track->videoHeight);
				printf("  frame_rate=%d\n",
					track->sampleCount*track->timescale / track->duration);

				vodFileInfo.Width = track->videoWidth;
				vodFileInfo.Height = track->videoHeight;
				vodFileInfo.FrameCount = track->sampleCount;
				if (track->duration == 0)
				{
					vodFileInfo.FrameRate = 0;
				}
				else
				{
					vodFileInfo.FrameRate = track->sampleCount*track->timescale / track->duration;
				}
				if (track->timescale == 0)
				{
					vodFileInfo.Duration = 0;
				}
				else
				{
					vodFileInfo.Duration = (track->duration * 1000000 + track->timescale / 2) /
						track->timescale;
					vodFileInfo.Duration /= 1000; //ms
				}
				if (pInfo != NULL)
				{
					*pInfo = vodFileInfo;
				}
				return i;
				break;
			}
			printf("\n");
		}
		i++;
	}
	return -1;
}

bool Vod_GetInfo(char *Mp4FileName, VODFileInfo_SRUT *pInfo)
{
	struct mp4_demux *demux = NULL;
	demux = mp4_demux_open(Mp4FileName);
	if (demux == NULL) {
		fprintf(stderr, "mp4_demux_open() failed\n");
		return false;
	}
	else {
		VODFileInfo_SRUT vodFileInfo;
		int index = mp4_demux_get_tracks_video(demux,&vodFileInfo);

		int ret = mp4_demux_close(demux);
		if (ret < 0) {
			fprintf(stderr, "mp4_demux_close() failed\n");
		}
		if (index != -1)
		{
			if (pInfo != NULL)
			{
				*pInfo = vodFileInfo;
			}
			return true;
		}
	}

	return false;
}

#ifdef MAIN_TEST
int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		exit(-1);
	}
	VODFileInfo_SRUT pInfo;
	bool ret = Vod_GetInfo(argv[1],&pInfo);
	if (ret == true)
	{
		printf("\n");
		printf("  Duration=%d\n",
			pInfo.Duration);
		printf("  FrameCount=%d\n",
			pInfo.FrameCount);
		printf("  FrameRate=%d\n",
			pInfo.FrameRate);
		printf("  Width=%d\n",
			pInfo.Width);
		printf("  Height=%d\n",
			pInfo.Height);
		return 0;
	}
	return -1;
}
#endif