#include "KsHq.h"
#include "KSGMdAPI.h"
#include "debug_log.h"

#include <string.h>
#include "network_interface.h"
KsHq::KsHq()
{
	KsApi = NULL;
}


KsHq::~KsHq()
{
}

bool KsHq::InitEnv(KingstargoldMdAPI::CKSGMdApi *api)
{
	KsApi = api;
	KsApi->RegisterSpi(this);
	return true;
}
//已连接
void KsHq::OnFrontConnected()
{
}
//未连接
void KsHq::OnFrontDisconnected()
{
//	WriteLog(2, "KStart diconnect");
	KsApi->RegisterServer(ip, port);
}
//踢除用户  
void KsHq::OnKickOff()
{
	printf( "KStart ccount kick off");
}

void	KsHq::ReLogin()
{
	KsApi->RegisterServer("219.140.77.102", 6900);
	KsApi->Init();
	CKSG_ReqUserLoginField loginreq;
	memset(&loginreq, 0, sizeof(loginreq));
	strcpy(loginreq.TraderNo, "200017");
	strcpy(loginreq.Pwd, "200017");
	int rcode = KsApi->ReqUserLogin(&loginreq);
	DEBUG_LOG("KStart account have relogin");
}
//登录应答
void KsHq::OnRspUserLogin(const CKSG_RspInfoField *pRspMsg, const CKSG_RspUserLoginField *pRspUserLogin, bool bIsLast)
{
	if (pRspMsg->ErrCode == 0)
	{
		printf("KStart account login sucess");
	}
	else
	{
		printf( pRspMsg->RspMsg);
	}
};
//登出应答
void KsHq::OnRspUserLogout(const CKSG_RspInfoField *pRspMsg, const CKSG_RspUserLogoutField *pRspUserLogout, bool bIsLast)
{
	if (0 == pRspMsg->ErrCode)
	{
		printf("KStart account logout ");
	}
	else
	{
		printf(pRspMsg->RspMsg);
	}
};
int getcharlen(char* buf)
{
	int len = 0;
	while (buf[len] != '/' &&buf[len] != '\000')
	{
		++len;

	}
	return len;
}
//行情服务的深度行情通知
void KsHq::OnMarketDataMblQuot(const CKSG_DepthMarketDataField *pfldDepthMarketData)
{
 	char str[1020] = {0};
	memset(str, 0, 1020);
 	sprintf(str,"K\t2\t%s,%s,%.4f,%.4f,%.4f,%d,%.4f,%.2f,%d,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f, %.4f,%d,%.4f,%d, %.4f,%d,%.4f,%d, %.4f,%d,%.4f,%d, %.4f,%d,%.4f,%d, %.4f,%d,%.4f,%d,%s,%s", pfldDepthMarketData->TradingDay, pfldDepthMarketData->ContractID, pfldDepthMarketData->LastPrice, pfldDepthMarketData->HighPrice, pfldDepthMarketData->LowPrice, pfldDepthMarketData->MatchTotQty, pfldDepthMarketData->MatchWeight,pfldDepthMarketData->Turnover,
		pfldDepthMarketData->OpenInterest, pfldDepthMarketData->OpenPrice, pfldDepthMarketData->ClearPrice, pfldDepthMarketData->ClearPrice, pfldDepthMarketData->LastClearPrice, pfldDepthMarketData->LastClose, pfldDepthMarketData->RiseLimit, pfldDepthMarketData->FallLimit,pfldDepthMarketData->UpDown, pfldDepthMarketData->UpDownRate, pfldDepthMarketData->AveragePrice,
		pfldDepthMarketData->BidPrice1, pfldDepthMarketData->BidVolume1, pfldDepthMarketData->AskPrice1, pfldDepthMarketData->AskVolume1,
		pfldDepthMarketData->BidPrice2, pfldDepthMarketData->BidVolume2, pfldDepthMarketData->AskPrice2, pfldDepthMarketData->AskVolume2,
		pfldDepthMarketData->BidPrice3, pfldDepthMarketData->BidVolume3, pfldDepthMarketData->AskPrice3, pfldDepthMarketData->AskVolume3,
		pfldDepthMarketData->BidPrice4, pfldDepthMarketData->BidVolume4, pfldDepthMarketData->AskPrice4, pfldDepthMarketData->AskVolume4,
		pfldDepthMarketData->BidPrice5, pfldDepthMarketData->BidVolume5, pfldDepthMarketData->AskPrice5, pfldDepthMarketData->AskVolume5,
		pfldDepthMarketData->QuoteDate, pfldDepthMarketData->QuoteTime);
	char* tmp = str;
	
	NETWORK_INTERFACE->SendWebSocketDate(str, getcharlen(tmp));
};



