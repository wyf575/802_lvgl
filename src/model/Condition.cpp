#include "Condition.h"

static long long tm_to_ns(struct timespec tm)
{
	return tm.tv_sec * 1000000000 + tm.tv_nsec;
}

static struct timespec ns_to_tm(long long ns)
{
	struct timespec tm;

	tm.tv_sec = ns / 1000000000;
	tm.tv_nsec = ns - (tm.tv_sec * 1000000000);
	return tm;
}

Condition::Condition()
{
	pthread_cond_init(&mCond, NULL);
}

Condition::~Condition()
{
	pthread_cond_destroy(&mCond);
}

void Condition::wait(Mutex & mutex)
{
 	pthread_cond_wait(&mCond, &mutex.mMutex);
}

void Condition::waitRelative(Mutex & mutex, long long reltime)
{
	struct timespec start_tm;
	struct timespec end_tm;

	clock_gettime(CLOCK_REALTIME, &start_tm);
	end_tm = ns_to_tm(tm_to_ns(start_tm) + reltime * 1000000);
	
 	pthread_cond_timedwait(&mCond, &mutex.mMutex, &end_tm);
}

void Condition::signal()
{
 	pthread_cond_signal(&mCond);
}

void Condition::broadcast()
{
 	pthread_cond_broadcast(&mCond);
}

