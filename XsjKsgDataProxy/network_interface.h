#ifndef __NETWORK_INTERFACE__
#define __NETWORK_INTERFACE__

#include "websocket_handler.h"
#include <pthread.h>

#define PORT 1213
#define TIMEWAIT 1000
#define BUFFLEN 2048
#define MAXEVENTSSIZE 1000		//最大监听数量

typedef std::map<int, Websocket_Handler *> WEB_SOCKET_HANDLER_MAP;

class Network_Interface {
private:
	Network_Interface();
	~Network_Interface();
	int init();
	int epoll_loop();
	int set_noblock(int fd);
	void ctl_event(int fd, bool flag);
public:
	void run();
	static Network_Interface *get_share_network_interface();
	void SendWebSocketDate(char* buf,int len);
	bool packData(int fd, int  len, const char* buf);
	int tmpleng;
	bool m_brun;
	bool m_binit;
	pthread_mutex_t	m_lock;
private:
	int epollfd_;
	int listenfd_;
	WEB_SOCKET_HANDLER_MAP websocket_handler_map_;
	static Network_Interface *m_network_interface;
};

#define NETWORK_INTERFACE Network_Interface::get_share_network_interface()

#endif
