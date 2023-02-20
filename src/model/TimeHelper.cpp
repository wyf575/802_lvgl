#include "TimeHelper.h"
#include <stdio.h>


static long long TimeHelper::getCurrentTime()
{
	long long msec = time((time_t *)NULL);

	return msec * 1000;
}

static struct tm* TimeHelper::getDateTime()
{
	time_t timep;
	struct tm *p;

	time(&timep);

	p = gmtime(&timep);

	return p;
}

static bool TimeHelper::setDateTime(const char * pDate)
{
	time_t timep;
	struct tm stm;
	
	if(NULL == pDate){
		return false;
	}
	
	scanf(pDate,"%d-%d-%d %d:%d:%d", stm.tm_year, stm.tm_mon, stm.tm_mday, stm.tm_hour, stm.tm_min, stm.tm_sec);

	timep = mktime(&stm);

	if(-1 == stime(timep))	return false;
	else					return true;
}

static bool TimeHelper::setDateTime(struct tm * ptm)
{
	time_t timep;

	if(NULL == ptm){
		return false;
	}

	timep = mktime(ptm);

	if(-1 == stime(timep))	return false;
	else					return true;
}