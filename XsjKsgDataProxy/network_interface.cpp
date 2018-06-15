#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include "debug_log.h"
#include "network_interface.h"

Network_Interface *Network_Interface::m_network_interface = NULL;

Network_Interface::Network_Interface():
		epollfd_(0),
		listenfd_(0),
		websocket_handler_map_()
{

// 	if(0 != init())
// 		exit(1);
	tmpleng = 230;
	m_brun = true;
	m_binit = false;
}

Network_Interface::~Network_Interface()
{
	m_brun = false;
}

int Network_Interface::init(){
	listenfd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(listenfd_ == -1)
	{
		DEBUG_LOG("create socket false");
		return -1;
	}
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);///
	if(-1 == bind(listenfd_, (struct sockaddr *)(&server_addr), sizeof(server_addr)))
	{
		DEBUG_LOG("bind socket false?");
		return -1;//
	}
	if(-1 == listen(listenfd_, MAXEVENTSSIZE))
	{
		DEBUG_LOG("monitor false!");
		return -1;
	}
	epollfd_ = epoll_create(MAXEVENTSSIZE);

	ctl_event(listenfd_, true);	
	DEBUG_LOG("websocket server start succes");
	return 0;
}

int Network_Interface::epoll_loop()
{
	struct sockaddr_in client_addr;
	socklen_t clilen;
	int nfds = 0;
	int fd = 0;
	int bufflen = 0;
	struct epoll_event events[MAXEVENTSSIZE];
	m_binit = true;
	while(m_brun)
	{
		try
		{
			nfds = epoll_wait(epollfd_, events, MAXEVENTSSIZE, -1);
			for (int i = 0; i < nfds; i++) {
				if (events[i].data.fd == listenfd_)
				{
					fd = accept(listenfd_, (struct sockaddr *)&client_addr, &clilen);
					ctl_event(fd, true);
					printf("socket connect on");
				}
				else if (events[i].events & EPOLLIN)
				{
					if ((fd = events[i].data.fd) < 0)
						continue;
					Websocket_Handler *handler = websocket_handler_map_[fd];
					if (handler == NULL)
						continue;
					bufflen = read(fd, handler->getbuff(), BUFFLEN);
					if (bufflen < 0)
					{
						ctl_event(fd, false);//
					}
					else if (bufflen == 0)
					{
						//连接断开
						ctl_event(fd, false);//
						DEBUG_LOG("client disclient");
					}
					else
					{
						handler->process();
						DEBUG_LOG("client connect handle");
					}
				}
				else
				{
					ctl_event(fd, false);//ffff
					DEBUG_LOG("epoll exception------------------------");
				}
				
			}
		}
		catch (const std::exception&)
		{
			DEBUG_LOG("catch the exception");
		}
		
		usleep(5000);
	}
	
	return 0;
}

int Network_Interface::set_noblock(int fd){
	int flags;
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

Network_Interface *Network_Interface::get_share_network_interface(){
	if(m_network_interface == NULL)
		m_network_interface = new Network_Interface();
	return m_network_interface;
}

void Network_Interface::ctl_event(int fd, bool flag)
{
	pthread_mutex_lock(&m_lock);
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = flag ? EPOLLIN : 0;
	epoll_ctl(epollfd_, flag ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &ev);
	if(flag){
		set_noblock(fd);
		websocket_handler_map_[fd] = new Websocket_Handler(fd);
// 		if(fd != listenfd_)
// 			DEBUG_LOG("fd: %d 加入epoll循环", fd);
	}
	else{
		close(fd);
		delete websocket_handler_map_[fd];
		websocket_handler_map_.erase(fd);
		//DEBUG_LOG("fd: %d 退出epoll循环", fd);
	}
	pthread_mutex_unlock(&m_lock);
}

void Network_Interface::run()
{
	pthread_mutex_init(&m_lock, NULL);
	if(0 != init())
	{
		pthread_mutex_destroy(&m_lock);
		exit(1);
	}
		
	
	epoll_loop();
	pthread_mutex_destroy(&m_lock);
}


void Network_Interface::SendWebSocketDate(char* buf,int len)
{
	if (!m_binit)
		return;
	pthread_mutex_lock(&m_lock);

	Websocket_Handler * phandle = NULL;
	std::map<int, Websocket_Handler*>::iterator iter;
	iter = websocket_handler_map_.begin();
	while (iter != websocket_handler_map_.end())
	{
		if (iter->first == listenfd_)
		{
			iter++;
			continue;
		}
		phandle = iter->second;
		packData(phandle->fd_, len, buf);

		iter++;
	}
	
	pthread_mutex_unlock(&m_lock);
}

bool  Network_Interface::packData(int fd, int len, const char* buf)
{
	char * data = NULL;
	int head_length = 0;
	unsigned long n;
	n = len;
	int num = 0;
	if (n < 126)
	{
		data = (char *)malloc(n+2);
		memset(data, 0, n + 2);
		data[0] = 0x81;
		data[1] = n;
		head_length = n+2;
		memcpy(data + 2, buf, n);
	}
	else if (n < 0xFFFF)
	{
		uint8_t headp[] = { 0 };
		memset(headp, 0, 4);
		data = (char *)malloc(n + 4);
//		data = (char *)malloc(n + 4);
		memset(data, 0, n + 4);
		headp[0] = 0x81;
		headp[1] = 126;
		headp[2] = (n >> 8) & 0xff;
		headp[3] = (n >> 0) & 0xff;
		/////
// 		data[2] = (n >> 8 );
// 		data[3] = (n & 0xFF);
		head_length = n+4;
		memcpy(data, headp, 4);
		memcpy(data + 4, buf, n);
	}
	else
	{
		// 暂不处理超长内容    
		int nn = 0;
	}
	int lenss = write(fd, data, head_length);
	if (lenss <= 0)
	{
		free(data);
		return false;
	}
	free(data);
	return true;
}
