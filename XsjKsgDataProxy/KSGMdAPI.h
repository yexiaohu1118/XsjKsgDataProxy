#ifndef KSG_MD_API_H
#define KSG_MD_API_H

#include <stdio.h>

#ifdef KSGMDAPI_EXPORTS
#define KSGMDAPI_LIB __declspec(dllexport)
#else
#ifdef WIN32
#define KSGMDAPI_LIB __declspec(dllimport)
#else
#define KSGMDAPI_LIB 
#endif
#endif
#include "KSGUserApiStruct.h"

namespace KingstargoldMdAPI
{
class CKSGMdSpi 
{
public:
	//������
	virtual void OnFrontConnected(){};
	//δ����
	virtual void OnFrontDisconnected(){};
	//�߳��û�
	virtual void OnKickOff(){};
	//��¼Ӧ��
	virtual void OnRspUserLogin(const CKSG_RspInfoField *pRspMsg,const CKSG_RspUserLoginField *pRspUserLogin,bool bIsLast){};
	//�ǳ�Ӧ��
	virtual void OnRspUserLogout(const CKSG_RspInfoField *pRspMsg,const CKSG_RspUserLogoutField *pRspUserLogout,bool bIsLast){};
	//���������������֪ͨ
	virtual void OnMarketDataMblQuot(const CKSG_DepthMarketDataField *pfldDepthMarketData){};
			 
};

class KSGMDAPI_LIB CKSGMdApi
{
public: 
	//�����ӿ�
	static CKSGMdApi *CreateKSGMdApi();
	//�ͷŽӿ�
	virtual void Release()=0;
public:
	//ע��ص��ӿ�
	virtual void RegisterSpi(CKSGMdSpi *pKSGMdSpi)=0;
	//����˽����
	virtual int SubscribePrivateFlow(bool restartflag=false)=0;
	//���Ĺ�����
	virtual int SubscribePublicFlow(bool restartflag=false)=0;
	//ע��ǰ�õ�ַ
	virtual int RegisterServer(char *ip,int port)=0;
	//��ʼ��
	virtual void Init(bool logflag=true)=0;
	//�ȴ��߳̽���
	virtual void Join()=0;
	//��¼����
	virtual int ReqUserLogin(CKSG_ReqUserLoginField *pUserLoginReq)=0;
	//�ǳ�����
	virtual int ReqUserLogout(CKSG_ReqUserLogoutField *pUserLogoutReq)=0;


protected:
	virtual ~CKSGMdApi();
};
}
#endif
