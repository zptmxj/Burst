// RS232.cpp: implementation of the RS232 class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "RS232.h"

#include "BurstDlg.h"
#include "join.h"
#include "PointTo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
//CComm message handlers
//CommWatch_Thread()
//����� �ϴ� ���ν���, �� �����Ͱ� ������ �� �����ϴ�
//��ƾ. �� ��ƾ�� OpenComPort �Լ� ����� ���ν����� �����
//OpenComPort �Լ� ����
DWORD CommWatch_Thread(LPVOID lpData)
{
	DWORD	dwEvtMask;
	OVERLAPPED	os;
	RS232*	npComm = (RS232*) lpData;
	char	InData[MAXBLOCK+1];
	int		nLength;

	//idCommDev��� �ڵ鿡 �ƹ��� ����Ʈ�� �� �پ� ������ ���� ����
	if(npComm == NULL)// || !npComm->IsKindOf(RUNTIME_CLASS(RS232)))
		return (DWORD)(-1);
	memset(&os, 0, sizeof(OVERLAPPED));

	os.hEvent = CreateEvent(	NULL,	//no security
								TRUE,	//explicit reset req
								FALSE,	//initial event reset
								NULL);	//no name
	if(os.hEvent == NULL)
	{
		MessageBox(NULL, _T("Failed to create event for thread!"), _T("comm Error"),
			MB_ICONEXCLAMATION | MB_OK);
		return (FALSE);
	}

	if(!SetCommMask(npComm->idComDev, EV_RXCHAR))
		return (FALSE);
	
	while (npComm->fConnected)
	{
		dwEvtMask = 0;

		WaitCommEvent(npComm->idComDev, &dwEvtMask, NULL);

		if((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
		{
			do
			{
				memset(InData, 0, MAXBLOCK);
				nLength = npComm->ReadCommBlock((LPSTR)InData, MAXBLOCK);
				if(nLength > 0)
				{
					npComm->SetReadData(InData, nLength);
					//�̰�����temp �����͸� �޴´�
					if(npComm->m_nSwitch == 0)
						npComm->m_pDlg00->Receive_RS232(nLength);
					else if(npComm->m_nSwitch == 1)
						npComm->m_pDlg01->Receive_RS232(nLength);
					else if (npComm->m_nSwitch == 2)
						npComm->m_pDlg02->Receive_RS232(nLength);

				}
			}
			while(nLength > 0);
		}
	}
	CloseHandle(os.hEvent);

	return(TRUE);
}

RS232::RS232()
{
	idComDev	= NULL;
	bFlowCtrl	= FC_XONXOFF;
	fConnected	= FALSE;
}

RS232::~RS232()
{
	if(fConnected)
		DestroyComm();
}

BOOL RS232::DestroyComm()
{
	if(fConnected)
		CloseConnection();

	CloseHandle(osRead.hEvent);
	CloseHandle(osWrite.hEvent);

	return (true);
}

BOOL RS232::CloseConnection()	//������ �ݴ´�
{
	//set connected flag to FALSE
	fConnected=FALSE;
	//disable event notification and wait for thread
	//to halt
	SetCommMask(idComDev, 0);

	EscapeCommFunction(idComDev, CLRDTR);

	PurgeComm(idComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	CloseHandle(idComDev);

	return true;
}

BOOL RS232::CreateCommInfo()
{
//����Ʈ ������ �����
//�̰��� ���� �� ������
//SetComPort();->SetXonOff()->SetDtrRts()�� ���� �����Ѵ�

	osWrite.Offset		= 0;
	osWrite.OffsetHigh	= 0;

	// overlapped structure ���� �ʱ�ȭ.

	osRead.Offset		 = 0;
	osRead.OffsetHigh	 = 0;
	//�̺�Ʈ â�� ����
	osRead.hEvent=CreateEvent(NULL, TRUE, FALSE, NULL);
	if(osRead.hEvent == NULL)
	{
		return false;
	}

	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(NULL == osWrite.hEvent)
	{
		CloseHandle(osRead.hEvent);
		return false;
	}

	return true;
}

BOOL RS232::OpenComPort()		//����Ʈ�� ���� ������ �õ��Ѵ�
{
	//char			szPort[15];
	CString			strPort;
	BOOL			fRetVal;
	COMMTIMEOUTS	CommTimeOuts;

	strPort.Format(_T("\\\\.\\COM%d"), bPort);

	//COMM device�� ���� �������� �����Ѵ�
	idComDev = CreateFile(strPort, GENERIC_READ | GENERIC_WRITE,
		0,						//exclusive access
		NULL,					//no security attrs
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL |
		FILE_FLAG_OVERLAPPED,	//overlapped I/O
		NULL);

	if( idComDev == (HANDLE) -1) return false;
	else
	{
		//����Ʈ���� �����͸� ��ȯ�ϴ� ����� char ������ �⺻���� ��������
		SetCommMask(idComDev, EV_RXCHAR);
		SetupComm(idComDev, 4096, 4096);
		//����̽��� �����Ⱑ ������ �𸣴ϱ� ������ û�Ҹ� ����
		PurgeComm(idComDev, PURGE_TXABORT | PURGE_RXABORT |
			PURGE_TXCLEAR | PURGE_RXCLEAR);

		CommTimeOuts.ReadIntervalTimeout		= 40;
		CommTimeOuts.ReadTotalTimeoutMultiplier	= 0;
		CommTimeOuts.ReadTotalTimeoutConstant	= 1000;
	
		CommTimeOuts.WriteTotalTimeoutMultiplier= 0;
		CommTimeOuts.WriteTotalTimeoutConstant	= 1000;
		SetCommTimeouts(idComDev, &CommTimeOuts);
	}

	fRetVal	= SetupConnection();
	
	if(fRetVal)	//������ �Ǿ��ٸ� fRetVal�̹Ƿ�
	{
		fConnected=TRUE;	//����Ǿ��ٰ� ���� ��
		//���ν����� CommWatchProc�� �����ϴϱ� ���߿� �����Ͱ� �Դٰ���
		//�ϸ� ��� ������ CommWatchProc�� ����Ѵ�
		AfxBeginThread((AFX_THREADPROC)CommWatch_Thread, (LPVOID)this, THREAD_PRIORITY_HIGHEST);
	}
	else
	{
		fConnected=FALSE;
		CloseHandle(idComDev);
	}

	return (fRetVal);
}

int RS232::ReadCommBlock(LPSTR lpszBlock, int nMaxLength)
{
	BOOL	fReadStat;
	COMSTAT ComStat;
	DWORD	dwErrorFlags;
	DWORD	dwLength;

	//only try to read number of bytes in queue
	ClearCommError(idComDev, &dwErrorFlags, &ComStat);

	dwLength = min((DWORD) nMaxLength, ComStat.cbInQue);

	if(dwLength > 0)
	{
		fReadStat = ReadFile(idComDev, lpszBlock, dwLength, &dwLength, &osRead);
		if(!fReadStat)
		{
		//	AfxMessageBox("�����Դϴ�");
		}
	}
	return(dwLength);
}

void RS232::SetComPort(int port, DWORD rate, BYTE bytesize, BYTE stop, BYTE parity)
{
	bPort		= port;
	dwBaudRate	= rate;
	bByteSize	= bytesize;
	bStopBits	= stop;
	bParity		= parity;
}

void RS232::SetDtrRts(BYTE chk)
{
	bFlowCtrl = chk;
}

void RS232::SetHwnd(CBurstDlg *pDlg)
{
	m_nSwitch = 0;
   m_pDlg00 = pDlg;
}

void RS232::SetHwnd(Join* pDlg)
{
	m_nSwitch = 1;
	m_pDlg01 = pDlg;
}

void RS232::SetHwnd(PointTo* pDlg)
{
	m_nSwitch = 2;
	m_pDlg02 = pDlg;
}

void RS232::SetReadData(LPSTR data, int length)	//�����͸� �а� �����͸� �о��ٴ� �޼����� �����Ѵ�
{
	//--> Clear Buf
	int i;
	for(i = 0; i < MAXBLOCK; i++)
	{
		abln[i] = 0;
	}
	//--> Buf Copy..
	for(i = 0; i < length; i++)
	{
		abln[i] = data[i];
	}
}

BOOL RS232::SetupConnection()
{
//���Ϸ� ������ ����Ʈ�� ���� ��Ʈ�� �����Ų��
//SetupConnection ������ CreateComPort�� ���־�� �Ѵ�

	BOOL fRetVal;
//	BYTE bSet;
	DCB  dcb;

	dcb.DCBlength=sizeof(DCB);

	GetCommState(idComDev, &dcb);	//dcb�� �⺻���� �޴´�

//////////////////////////////////////////////////////////////////////////////

	//�� �κ��� �����ؾ� �Ѵ�.
	dcb.BaudRate	= dwBaudRate;	//���� �ӵ�
	dcb.ByteSize	= bByteSize;	//������ ��Ʈ
	dcb.Parity		= bParity;		//�и�Ƽ üũ
	dcb.StopBits	= bStopBits;	//���� ��Ʈ

/*	dcb.fOutxDsrFlow	= 0;					//Dsr Flow
	dcb.fDtrControl		= DTR_CONTROL_ENABLE;	//Dtr Control
	dcb.fOutxCtsFlow	= 0;					//Cts Flow
	dcb.fRtsControl		= RTS_CONTROL_ENABLE;	//Ctr Control
	dcb.fInX=dcb.fOutX	= 1;					//XON/XOFF ���� ��
	dcb.XonChar			= ASCII_XOFF;
	dcb.XonLim			= 100;
	dcb.XoffLim			= 100;
	dcb.fBinary			= TRUE;
	dcb.fParity			= TRUE;
*/

/*	dcb.BaudRate = 9600;					//Current baud
	dcb.fBinary = TRUE;						//Binary mode; no EOF check
	dcb.fParity = TRUE;						//Enable parity checking
	dcb.fOutxCtsFlow = FALSE;				//No CTS output flow control
	dcb.fOutxDsrFlow = FALSE;				//No DSR output flow control
	dcb.fDtrControl = DTR_CONTROL_ENABLE;	//DTR flow control type
	dcb.fDsrSensitivity = FALSE;			//DSR sensitivity
	dcb.fTXContinueOnXoff = TRUE;			//XOFF continues Tx
	dcb.fOutX = FALSE;						//No XON/XOFF out flow control
	dcb.fInX = FALSE;						//No XON/XOFF in flow control
	dcb.fErrorChar = FALSE;					//Disable error replacement
	dcb.fNull = FALSE;						//Disable null stripping
	dcb.fRtsControl = RTS_CONTROL_ENABLE;	//RTS flow control
	dcb.fAbortOnError = FALSE;				//Do not abort reads/writes on error
	dcb.ByteSize = 8;						//Number of bits/byte, 4-8
	dcb.Parity = NOPARITY;					//0-4=no, odd, even, mark, space
	dcb.StopBits = ONESTOPBIT;				//0,1,2,=1, 1.5, 2
*/
	fRetVal = (BOOL)SetCommState(idComDev, &dcb);	//����� dcb ����

	return(fRetVal);

}

void RS232::SetXonOff(BOOL chk)		 //XonOff, �� ���ϰ� ���� ����
{
	fXonXoff=chk;
}

BOOL RS232::WriteCommBlock(LPSTR IpByte, DWORD dwBytesToWrite)
{
	BOOL	fWriteStat;
	DWORD	dwBytesWritten, dwError, dwErrorFlags;
	COMSTAT	comstat;
	fWriteStat=WriteFile(idComDev, IpByte, dwBytesToWrite, &dwBytesWritten, &osWrite);
	if(!fWriteStat)
	{
		//����Ʈ�� �����͸� ����� ����� ������ ����̴�
		//�ٽ� ������ ������ ��ͼ�����(���ѷ��� ����)
		//AfxMessageBox("Write Error!");
	}

	return (true);



	if( !WriteFile( idComDev, IpByte, dwBytesToWrite, &dwBytesWritten, &osWrite))
	{
		//--> ���� ������ ���ڰ� ������ ���..
		if (GetLastError() == ERROR_IO_PENDING)
		{
			// ���� ���ڰ� ���� �ְų� ������ ���ڰ� ���� ���� ��� Overapped IO��
			// Ư���� ���� ERROR_IO_PENDING ���� �޽����� ���޵ȴ�.
			//timeouts�� ������ �ð���ŭ ��ٷ��ش�.
			while (! GetOverlappedResult( idComDev, &osWrite, &dwBytesWritten, TRUE))
			{
				dwError = GetLastError();
				if (dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError( idComDev, &dwErrorFlags, &comstat);
					break;
				}
			}
		}
		else
		{
			dwBytesWritten = 0;
			ClearCommError( idComDev, &dwErrorFlags, &comstat);
		}
	}

	return (true);

}
