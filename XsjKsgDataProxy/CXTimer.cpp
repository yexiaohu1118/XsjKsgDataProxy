#include "CXTimer.h"
#include <stdio.h>
#include <pthread.h>    // pthread_create()
#include <unistd.h>     // sleep()
//#include <sys/types.h>  // getpid()


void* func(void *arg)
{
	st_key*  tk =(st_key*)arg;
	while (!tk->bend)
	{
		sleep(tk->ntimer);
		tk->mbc();
	}
}


CXTimer::~CXTimer()
{
	if (m_tk)
	{
		delete(m_tk);
	}
}
int CXTimer::SetTimer(int ntimecount, TimerCallback cb)
{
	m_tk = new st_key;
	m_tk->bend = false;
	m_tk->mbc = cb;
	m_tk->ntimer = ntimecount;
	
	int pid= pthread_create(&t1, NULL, func, (void*)m_tk);

}

void CXTimer::KillTimer()
{
	if (m_tk->mbc)
	{
		m_tk->bend = true;
		pthread_join(t1, NULL);
		delete(m_tk);
	}
}