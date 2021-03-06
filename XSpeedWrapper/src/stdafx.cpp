// stdafx.cpp : 只包括标准包含文件的源文件
// CTPWrapper.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "..\header\stdafx.h"

using namespace std;  

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

int main(int argc, char* argv[])
{

  //交易接口
  DFITCTraderApi* pTrader = DFITCTraderApi::CreateDFITCTraderApi();

  //C#回调函数
  //XSpeedResponseCallback callback = (XSpeedResponseCallback)pReqData;

  //创建交易回调
  //XSpeedTraderSpi *spi = new XSpeedTraderSpi(callback);
  XSpeedTraderSpi spi;

  //注册回调
  pTrader->Init( "tcp://203.187.171.250:10910",&spi);

  //设置回调函数
  //spi.SetCallback(callback);

  //在设置callback之前可能已经与服务器建立，但OnFrontConnected响应却无法抛到C#端
  //因此再次手工引发该响应
  if(spi.connected)
  {
    spi.OnFrontConnected();


    struct DFITCUserLoginField value;

    value.lRequestID = 1;
    strcpy( value.accountID,"000100000421");
    strcpy( value.passwd,"123");

    pTrader->ReqUserLogin(&value);

    while(true)
    {
      Sleep(50);
      //OutputLog("wait login \n");
      if(spi.login)
      {
        break;
      }
    }

    struct DFITCExchangeInstrumentField exchange;

    exchange.lRequestID = 2;
    strcpy( exchange.accountID,"000100000421");
    strcpy( exchange.exchangeID,"DCE");

    pTrader->ReqQryExchangeInstrument(&exchange);
  }

  while(true)
  {
    Sleep(1000);
    printf("while\n");
  }

	return 0;
}



typedef void (WINAPI *PTRFUN)(const char*);

PTRFUN debug_output;

extern "C" CTPWRAPPER_API void WINAPI SetOutputCallback(PTRFUN pfun) 
{
	debug_output = pfun;
}

void OutputLog(const char* msg)
{
  if(debug_output)
	  debug_output(msg);
}