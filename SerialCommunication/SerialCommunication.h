#pragma once

#include <Windows.h>

#define BUFFER_SIZE 1024
#define BIT_SIZE 8
#define WAIT_TIMEOUT_MILLISECOND 500
#define COMMUNICATION_SPEED 9600
#define MAX_ELEMENTS 32
#define MAX_PORTNUM 3

class CComPort
{
public:
	CComPort(void);
	~CComPort();

	bool GetStatus(bool _whichCheckPort);
	bool GetEndFlag();
	

	bool Open(char* _sOpenSendPortName, char* _sOpenRecvPortName);

	//入力されたポート番号が開けるか確認する
	bool CheckOpenPort(char* _iOpenSendPortNum, char* _iOpenRecvPortNum);
	bool Close();
	bool Send();
	bool Recv();
	bool InitializeCommParam();
	bool ClearBuffer();
	bool SetCommDevice();
	bool SetTimeoutParam();
	


//-------------------------------------------------メンバ変数-------------------------------------------------
private:
	//Send側のハンドル
	HANDLE hSendPort;

	//Recv側のハンドル
	HANDLE hRecvPort;

	//DCB構造体を格納する変数
	DCB dcb;

	//COMMTIMEOUTS構造体を格納する変数
	COMMTIMEOUTS timeout;

	//送信するデータのサイズ
	DWORD dwSendBufferSize;

	//受信するデータのサイズ
	DWORD dwRecvBufferSize;



	//ポートが開いているかのフラグ
	bool isOpenPort;



	//Windows API関数の戻り値を格納する変数
	bool isApiError;

	//データの送信が成功したかのフラグ
	bool isSuccessSend;




//-------------------------------------------------メンバ変数-------------------------------------------------

};

void InputData();

void OutputData();



//送信側のポートの数字
char sOpenSendPortNum[10];

//受信側のポートの数字
char sOpenRecvPortNum[10];

char sOpenSendPortName[MAX_ELEMENTS];
char sOpenRecvPortName[MAX_ELEMENTS];

//入力された文字列
char sInputStr[20];

//受信する文字列
char sRecvStr[20];

//プログラムを終了するかのフラグ
bool isInputEnd;

//外部関数の戻り値がエラーであるか
bool isFuncError;