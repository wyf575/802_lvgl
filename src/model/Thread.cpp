#include <unistd.h>
#include <sys/prctl.h>
#include <stdio.h>
#include "Thread.h"



Thread::Thread()
{
	mIsRunning = false;
	mExitPending = false;
}

Thread::~Thread()
{

}

bool Thread::run(const char * name = 0)
{
	pthread_t tid;
	SThreadData *data;

	data->threadName = name;
	data->userData = (Thread *)this;

	mLock.lock();
	
	if(mIsRunning){
		fprintf(stderr, "thread has running\n");
		mLock.unlock();
		return true;
	}
	
	if(0 != pthread_create(&tid, NULL, _threadLoop, (SThreadData *)data)){
		if(NULL != name)	
			printf("[%s]create thread %s fail\n", __FUNCTION__, name);
		mLock.unlock();
		return false;
	}else{
		mLock.unlock();
		return true;
	}
}

void Thread::requestExit()
{
	mExitPending = true;
}

void Thread::requestExitAndWait()
{
	mExitPending = true;
	mThreadExitedCondition.wait(mLock);
}

bool Thread::isRunning() const
{
	return mIsRunning;
}

static void Thread::sleep(int msec)
{
	usleep(msec * 1000);
}

bool Thread::exitPending() const
{
	return mExitPending;
}

static void *Thread::_threadLoop(void * user)
{
	bool isLoop = true;
	SThreadData *data = (SThreadData *)user;
	Thread *parent = (Thread *)data->userData;
	
	if(NULL != data->threadName)
  		prctl(PR_SET_NAME, (char *)data->threadName);

	parent->readyToRun();

	parent->mIsRunning = true;
	
	do{
		isLoop = parent->threadLoop();
	}while(isLoop && !parent->mExitPending);

	parent->mIsRunning = false;

	parent->mThreadExitedCondition.broadcast();

	return;
}


