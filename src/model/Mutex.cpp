#include "Mutex.h"


Mutex::Mutex()
{
	pthread_mutex_init(&mMutex, NULL);
}

Mutex::Mutex(int type)
{

}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&mMutex);
}

void Mutex::lock()
{
	pthread_mutex_lock(&mMutex);
}

void Mutex::unlock()
{
	pthread_mutex_unlock(&mMutex);
}

void Mutex::tryLock()
{
  pthread_mutex_trylock(&mMutex);
}

