#pragma once
#include "KSGMdAPI.h"
#include "CXTimer.h"
class KsHq:KingstargoldMdAPI::CKSGMdSpi
{
public:
	KsHq();
	~KsHq();
	bool InitEnv(KingstargoldMdAPI::CKSGMdApi *api);
	KingstargoldMdAPI::CKSGMdApi *KsApi;
	int		port;
	char	ip[32];
	CXTimer	m_time;
	void	ReLogin();
	//������
	virtual void OnFrontConnected();
	//δ����
	virtual void OnFrontDisconnected();
	//�߳��û�  
	virtual void OnKickOff();
	//��¼Ӧ��
	virtual void OnRspUserLogin(const CKSG_RspInfoField *pRspMsg, const CKSG_RspUserLoginField *pRspUserLogin, bool bIsLast);
	//�ǳ�Ӧ��
	virtual void OnRspUserLogout(const CKSG_RspInfoField *pRspMsg, const CKSG_RspUserLogoutField *pRspUserLogout, bool bIsLast);
	//���������������֪ͨ
	virtual void OnMarketDataMblQuot(const CKSG_DepthMarketDataField *pfldDepthMarketData);


};

