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

	//���͂��ꂽ�|�[�g�ԍ����J���邩�m�F����
	bool CheckOpenPort(char* _iOpenSendPortNum, char* _iOpenRecvPortNum);
	bool Close();
	bool Send();
	bool Recv();
	bool InitializeCommParam();
	bool ClearBuffer();
	bool SetCommDevice();
	bool SetTimeoutParam();
	


//-------------------------------------------------�����o�ϐ�-------------------------------------------------
private:
	//Send���̃n���h��
	HANDLE hSendPort;

	//Recv���̃n���h��
	HANDLE hRecvPort;

	//DCB�\���̂��i�[����ϐ�
	DCB dcb;

	//COMMTIMEOUTS�\���̂��i�[����ϐ�
	COMMTIMEOUTS timeout;

	//���M����f�[�^�̃T�C�Y
	DWORD dwSendBufferSize;

	//��M����f�[�^�̃T�C�Y
	DWORD dwRecvBufferSize;



	//�|�[�g���J���Ă��邩�̃t���O
	bool isOpenPort;



	//Windows API�֐��̖߂�l���i�[����ϐ�
	bool isApiError;

	//�f�[�^�̑��M�������������̃t���O
	bool isSuccessSend;




//-------------------------------------------------�����o�ϐ�-------------------------------------------------

};

void InputData();

void OutputData();



//���M���̃|�[�g�̐���
char sOpenSendPortNum[10];

//��M���̃|�[�g�̐���
char sOpenRecvPortNum[10];

char sOpenSendPortName[MAX_ELEMENTS];
char sOpenRecvPortName[MAX_ELEMENTS];

//���͂��ꂽ������
char sInputStr[20];

//��M���镶����
char sRecvStr[20];

//�v���O�������I�����邩�̃t���O
bool isInputEnd;

//�O���֐��̖߂�l���G���[�ł��邩
bool isFuncError;