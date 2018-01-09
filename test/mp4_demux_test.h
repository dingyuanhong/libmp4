#ifndef MP4_DEMUX_TEST_H
#define MP4_DEMUX_TEST_H

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libmp4.lib")

static inline struct tm * localtime_r(const time_t *timer, struct tm *tm)
{
	struct tm * ret = localtime(timer);
	if (ret != NULL)
	{
		*(tm) = *ret;
		return tm;
	}
	return NULL;
}

#define clock_gettime(A,B) timespec_get(B,TIME_UTC)

#else
#include <arpa/inet.h>
#endif

#endif