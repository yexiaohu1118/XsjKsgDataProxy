#include <cstdio>
#include "network_interface.h"
#include<stdio.h>
#include<dlfcn.h>
#include "KsHq.h"
#include "KSGMdAPI.h"

KingstargoldMdAPI::CKSGMdApi*G_ksd_Api = NULL;
KsHq  G_ksSpi;

void IsJsdShouldCon()
{
	time_t t;
	time(&t);
	struct tm *t1;
	t1 = localtime(&t);
	int time = t1->tm_hour * 10000 + t1->tm_min * 100 + t1->tm_sec;
	if ((time > 132300 && time < 132805) || (time > 195300 && time < 195803) || (time > 85300 && time < 85805))
	{
		//ÏÈµÇ³ö£¬ÔÙµÇÂ¼
		G_ksSpi.ReLogin();
	}
}

int main()
{
	if (!G_ksd_Api)
	{
 		G_ksd_Api = KingstargoldMdAPI::CKSGMdApi::CreateKSGMdApi();
		G_ksSpi.port = 6900;
		strcpy(G_ksSpi.ip, "219.140.77.102");
		G_ksSpi.InitEnv(G_ksd_Api);
		int ret = G_ksd_Api->RegisterServer("219.140.77.102", 6900);
		if (ret == -8)
		{
			printf( "Address have registed");
		}
		else
		{
			printf("Address regist sucess");
		}
		G_ksd_Api->Init();
		CKSG_ReqUserLoginField loginreq;
		memset(&loginreq, 0, sizeof(loginreq));
		strcpy(loginreq.TraderNo, "200017");
		strcpy(loginreq.Pwd, "200017");
	
		int rcode = G_ksd_Api->ReqUserLogin(&loginreq);
		printf("KStart Login on");
		G_ksSpi.m_time.SetTimer(300, IsJsdShouldCon);
 	}
	NETWORK_INTERFACE->run();
	
    return 0;
}