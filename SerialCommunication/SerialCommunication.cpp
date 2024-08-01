// SerialCommunication.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "SerialCommunication.h"

using namespace std;

CComPort::CComPort(void)
	: hSendPort(NULL)
	, hRecvPort(NULL)
	, dcb(dcb)
	, timeout(timeout)
	, dwSendBufferSize(0)
	, isOpenPort(false)
{


}


CComPort::~CComPort()
{

}


int main()
{
	isInputEnd = false;




	CComPort comport1;

	while (comport1.GetEndFlag() == false)
	{
		isFuncError = false;
		memset(sOpenSendPortNum, 0, strlen(sOpenSendPortNum));
		memset(sOpenRecvPortNum, 0, strlen(sOpenRecvPortNum));
		memset(sOpenSendPortName, 0, strlen(sOpenSendPortName));
		memset(sOpenRecvPortName, 0, strlen(sOpenRecvPortName));

		InputData();

		if (isInputEnd == true)
		{
			cout << "プログラムを終了します。" << endl;
			break;
		}

		isFuncError = comport1.CheckOpenPort(sOpenSendPortNum, sOpenRecvPortNum);

		if (isFuncError == false)
		{
			cout << "COMポートが開かれていないので再入力していください。" << endl;
		}

		isFuncError = comport1.Open(sOpenSendPortName, sOpenRecvPortName);

		if (isFuncError == false)
		{
			cout << "プログラムを終了します。" << endl;
			break;
		}

		isFuncError = comport1.Send();

		if (isFuncError == false)
		{
			cout << "プログラムを終了します。" << endl;
			break;
		}


		isFuncError = comport1.Recv();

		if (isFuncError == false)
		{
			cout << "プログラムを終了します。" << endl;
			break;
		}


		OutputData();




		isFuncError = comport1.Close();
	}
}

bool CComPort::GetStatus(bool _whichCheckPort)
{
	if (_whichCheckPort == true)
	{

	}
	return true;
}

bool CComPort::GetEndFlag()
{
	return isInputEnd;
}

void InputData()
{
	cout << "送信するデータを入力していください。" << endl;
	cin >> sInputStr;

	if (strcmp(sInputStr, "End") == 0)
	{
		isInputEnd = true;
		return;
	}

	cout << "送信側の開くポートの番号を入力していください。" << endl;
	cin  >> sOpenSendPortNum;

	cout << "受信側の開くポートの番号を入力していください。" << endl;
	cin >> sOpenRecvPortNum;

	strcat(sOpenSendPortName, "COM");
	strcat(sOpenRecvPortName, "COM");



	strcat(sOpenSendPortName, sOpenSendPortNum);
	strcat(sOpenRecvPortName, sOpenRecvPortNum);
}



void OutputData()
{
	cout << "sInputStr        :" << sInputStr << endl;
	cout << "sRecvStr         :" << sRecvStr << endl;
}



bool CComPort::Open(char* _sOpenSendPortName, char* _sOpenRecvPortName)
{
	hSendPort = CreateFile(
		_sOpenSendPortName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);


	if (hSendPort == INVALID_HANDLE_VALUE)
	{
		cout << "送信側のポートが開けませんでした。" << endl;
		return false;
	}
	else
	{
		cout << "送信側のポートが開けました。" << endl;
	}



	hRecvPort = CreateFile(
		_sOpenRecvPortName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);


	if (hRecvPort == INVALID_HANDLE_VALUE)
	{
		cout << "受信側のポートが開けませんでした。" << endl;
		return false;
	}
	else
	{
		cout << "受信側のポートが開けました。" << endl;
		
	}



	if (InitializeCommParam() == false)
	{
		return false;
	}

	if (ClearBuffer() == false)
	{
		return false;
	}

	if (SetCommDevice() == false)
	{
		return false;
	}

	if (SetTimeoutParam() == false)
	{
		return false;
	}

	return true;
}



bool CComPort::CheckOpenPort(char* _iOpenSendPortNum, char* _iOpenRecvPortNum)
{

	unsigned long resPort;                               //ポートを取得しようとした結果
	constexpr size_t uPortNumbersCount = 10;
	unsigned long lpPortNumbers[uPortNumbersCount]{ 0 }; //取得したポート番号の配列
	unsigned long puPortNumbersFound;                    //取得したポートの数


	//設定されているポート番号を取得 lpPortNumbersに格納
	resPort = GetCommPorts(lpPortNumbers, uPortNumbersCount, &puPortNumbersFound);

	bool isHit = false;

	//送信側のポート番号を検索
	isHit = find(lpPortNumbers, lpPortNumbers + uPortNumbersCount, (int)(*_iOpenSendPortNum - '0'))
		!= lpPortNumbers + uPortNumbersCount;


	if (isHit == true)
	{

		cout << "送信側のポートが設定されています。" << endl;
	}
	else
	{
		cout << "送信側のポートが設定されていません。" << endl;
		return false;
	}

	//受信側のポート番号を検索
	isHit = find(lpPortNumbers, lpPortNumbers + uPortNumbersCount, (int)(*_iOpenRecvPortNum - '0'))
		!= lpPortNumbers + uPortNumbersCount;

	if (isHit == true)
	{

		cout << "受信側のポートが設定されています。" << endl;
		return true;
	}
	else
	{
		cout << "受信側のポートが設定されていません。" << endl;
		return false;
	}
}


bool CComPort::Close()
{
	isApiError = CloseHandle(
		hSendPort
	);
	if (isApiError == 0)
	{
		cout << "ファイルが閉じられませんでした。。" << endl;
		return false;
	}
	else
	{
		cout << "ファイルが閉じられました。" << endl;
	}


	isApiError = CloseHandle(
		hRecvPort
	);
	if (isApiError == 0)
	{
		cout << "ファイルが閉じられませんでした。。" << endl;
		return false;
	}
	else
	{
		cout << "ファイルが閉じられました。" << endl;
		return true;
	}

}



bool CComPort::Send()
{
	isApiError = WriteFile(
		hSendPort,
		sInputStr,
		sizeof(sInputStr),
		&dwSendBufferSize,
		NULL
	);

	if (isApiError == 0)
	{
		cout << "WriteFile関数が失敗しました。" << endl;
		return false;
	}
	else
	{
		cout << "WriteFile関数が成功しました。" << endl;
		return true;
	}
}



bool CComPort::Recv()
{
	isApiError = ReadFile(
		hRecvPort,
		sRecvStr,
		sizeof(sRecvStr),
		&dwRecvBufferSize,
		NULL
	);

	if (isApiError == 0)
	{
		cout << "ReadFile関数が失敗しました。" << endl;
		return false;
	}
	else
	{
		cout << "ReadFile関数が成功しました。" << endl;
		return true;
	}


}


bool CComPort::InitializeCommParam()
{
	isApiError = SetupComm(
		hSendPort,
		BUFFER_SIZE,
		BUFFER_SIZE
	);

	if (isApiError == 0)
	{
		cout << "SetupComm関数が失敗しました。" << endl;
		return false;
	}
	else
	{
		cout << "SetupComm関数が成功しました。" << endl;
	}


	isApiError = SetupComm(
		hRecvPort,
		BUFFER_SIZE,
		BUFFER_SIZE
	);

	if (isApiError == 0)
	{
		cout << "SetupComm関数が失敗しました。" << endl;
		return false;
	}
	else
	{
		cout << "SetupComm関数が成功しました。" << endl;
		return true;
	}
}


bool CComPort::ClearBuffer()
{
	isApiError = PurgeComm(
		hSendPort,
		PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
	);

	if (isApiError == 0)
	{
		cout << "PurgeComm関数が失敗しました。" << endl;
		return false;
	}
	else
	{
		cout << "PurgeComm関数が成功しました。" << endl;
	}



	isApiError = PurgeComm(
		hRecvPort,
		PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
	);

	if (isApiError == 0)
	{
		cout << "PurgeComm関数が失敗しました。" << endl;
		return false;
	}
	else
	{
		cout << "PurgeComm関数が成功しました。" << endl;
		return true;
	}

}

bool CComPort::SetCommDevice()
{
	isApiError = GetCommState(
		hSendPort,
		&dcb
	);

	if (isApiError == 0)
	{
		cout << "GetCommState関数が失敗しました。" << endl;
		return false;
	}
	else
	{
		cout << "GetCommState関数が成功しました。" << endl;
	}

	dcb.BaudRate = COMMUNICATION_SPEED;
	dcb.ByteSize = BIT_SIZE;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	isApiError = SetCommState(
		hSendPort,
		&dcb
	);

	if (isApiError == 0)
	{
		cout << "SetCommState関数が失敗しました。" << endl;
		return false;
	}
	else
	{
		cout << "SetCommState関数が成功しました。" << endl;
		return true;
	}

	isApiError = SetCommState(
		hRecvPort,
		&dcb
	);

	if (isApiError == 0)
	{
		cout << "SetCommState関数が失敗しました。" << endl;
		return false;
	}
	else
	{
		cout << "SetCommState関数が成功しました。" << endl;
		return true;
	}



}

bool CComPort::SetTimeoutParam()
{
	timeout.ReadIntervalTimeout = WAIT_TIMEOUT_MILLISECOND;
	
	timeout.ReadTotalTimeoutMultiplier = 0;
	timeout.ReadTotalTimeoutConstant = WAIT_TIMEOUT_MILLISECOND;

	timeout.WriteTotalTimeoutMultiplier = 0;
	timeout.WriteTotalTimeoutConstant = WAIT_TIMEOUT_MILLISECOND;

	isApiError = SetCommTimeouts(
		hSendPort,
		&timeout
	);

	if (isApiError == 0)
	{
		cout << "SetTimeoutParam関数が失敗しました。" << endl;
		return false;
	}
	else
	{
		cout << "SetTimeoutParam関数が成功しました。" << endl;
	}



	isApiError = SetCommTimeouts(
		hRecvPort,
		&timeout
	);

	if (isApiError == 0)
	{
		cout << "SetTimeoutParam関数が失敗しました。" << endl;
		return false;
	}
	else
	{
		cout << "SetTimeoutParam関数が成功しました。" << endl;
		return true;
	}

}
