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
//통신을 하는 프로시저, 즉 데이터가 들어왔을 때 감시하는
//루틴. 본 루틴은 OpenComPort 함수 실행시 프로시저로 연결됨
//OpenComPort 함수 참조
DWORD CommWatch_Thread(LPVOID lpData)
{
	DWORD	dwEvtMask;
	OVERLAPPED	os;
	RS232*	npComm = (RS232*) lpData;
	char	InData[MAXBLOCK+1];
	int		nLength;

	//idCommDev라는 핸들에 아무런 컴포트가 안 붙어 있으면 에러 리턴
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
					//이곳에서temp 데이터를 받는다
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

BOOL RS232::CloseConnection()	//연결을 닫는다
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
//컴포트 정보를 만든다
//이것을 만들 때 이전에
//SetComPort();->SetXonOff()->SetDtrRts()한 다음 설정한다

	osWrite.Offset		= 0;
	osWrite.OffsetHigh	= 0;

	// overlapped structure 변수 초기화.

	osRead.Offset		 = 0;
	osRead.OffsetHigh	 = 0;
	//이벤트 창구 설정
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

BOOL RS232::OpenComPort()		//컴포트를 열고 연결을 시도한다
{
	//char			szPort[15];
	CString			strPort;
	BOOL			fRetVal;
	COMMTIMEOUTS	CommTimeOuts;

	strPort.Format(_T("\\\\.\\COM%d"), bPort);

	//COMM device를 파일 형식으로 연결한다
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
		//컴포트에서 데이터를 교환하는 방법을 char 단위를 기본으로 설정하자
		SetCommMask(idComDev, EV_RXCHAR);
		SetupComm(idComDev, 4096, 4096);
		//디바이스에 쓰레기가 있을지 모르니까 깨끗이 청소를 하자
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
	
	if(fRetVal)	//연결이 되었다면 fRetVal이므로
	{
		fConnected=TRUE;	//연결되었다고 말해 줌
		//프로시저를 CommWatchProc에 연결하니까 나중에 데이터가 왔다갔다
		//하면 모든 내용은 CommWatchProc가 담당한다
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
		//	AfxMessageBox("에러입니다");
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

void RS232::SetReadData(LPSTR data, int length)	//데이터를 읽고 데이터를 읽었다는 메세지를 리턴한다
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
//파일로 설정된 컴포트와 실질 포트를 연결시킨다
//SetupConnection 이전에 CreateComPort를 해주어야 한다

	BOOL fRetVal;
//	BYTE bSet;
	DCB  dcb;

	dcb.DCBlength=sizeof(DCB);

	GetCommState(idComDev, &dcb);	//dcb의 기본값을 받는다

//////////////////////////////////////////////////////////////////////////////

	//이 부분을 수정해야 한다.
	dcb.BaudRate	= dwBaudRate;	//전송 속도
	dcb.ByteSize	= bByteSize;	//데이터 비트
	dcb.Parity		= bParity;		//패리티 체크
	dcb.StopBits	= bStopBits;	//스톱 비트

/*	dcb.fOutxDsrFlow	= 0;					//Dsr Flow
	dcb.fDtrControl		= DTR_CONTROL_ENABLE;	//Dtr Control
	dcb.fOutxCtsFlow	= 0;					//Cts Flow
	dcb.fRtsControl		= RTS_CONTROL_ENABLE;	//Ctr Control
	dcb.fInX=dcb.fOutX	= 1;					//XON/XOFF 관한 것
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
	fRetVal = (BOOL)SetCommState(idComDev, &dcb);	//변경된 dcb 설정

	return(fRetVal);

}

void RS232::SetXonOff(BOOL chk)		 //XonOff, 즉 리턴값 더블 설정
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
		//컴포트에 데이터를 제대로 써넣지 못했을 경우이다
		//다시 보내고 싶으면 재귀송출을(무한루프 주의)
		//AfxMessageBox("Write Error!");
	}

	return (true);



	if( !WriteFile( idComDev, IpByte, dwBytesToWrite, &dwBytesWritten, &osWrite))
	{
		//--> 아직 전송할 문자가 남았을 경우..
		if (GetLastError() == ERROR_IO_PENDING)
		{
			// 읽을 문자가 남아 있거나 전송할 문자가 남아 있을 경우 Overapped IO의
			// 특성에 따라 ERROR_IO_PENDING 에러 메시지가 전달된다.
			//timeouts에 정해준 시간만큼 기다려준다.
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
