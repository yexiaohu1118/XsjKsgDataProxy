#pragma once
#include <stdint.h>
#include <functional>
#include <unordered_map>
#include <pthread.h>
typedef std::function<void()> TimerCallback;
typedef struct st_key
{
	bool bend;
	int ntimer;
	int pid;
	TimerCallback mbc;
};
class CXTimer
{
public:
	pthread_t t1;
	st_key* m_tk;
	int mpid;
public:
	~CXTimer();
	
	int SetTimer(int ntimecount, TimerCallback cb);
	void KillTimer();

};

