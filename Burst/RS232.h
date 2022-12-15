// RS232.h: interface for the RS232 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RS232_H__E827F1BF_3F9E_4844_B2CF_C86F78411B32__INCLUDED_)
#define AFX_RS232_H__E827F1BF_3F9E_4844_B2CF_C86F78411B32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Rs232c를 하기 위한 Class Header
#define MAXBLOCK	80
#define MAXPORTS	4

//Flow control flags
#define FC_DTRDSR	0x01
#define FC_RTSCTS	0x02
#define FC_XONXOFF	0x04

//ascii definitions
#define ASCII_BEL	0x07
#define ASCII_BS	0x08
#define ASCII_LF	0x0A
#define ASCII_CR	0x0D
#define ASCII_XON	0x11
#define ASCII_XOFF	0x13
#define UM_232RECEIVEDATA	WM_USER+232

class CBurstDlg;
class Join;
class PointTo;

class RS232
{	  
public:
	RS232();
	virtual ~RS232();

	HANDLE	idComDev;			//컴포트 디바이스 연결 핸들
	BOOL	fConnected;			//컴포트가 연결되면 1로 설정
	BYTE	abln[MAXBLOCK+1];	//컴포트에서 들어오는 데이터

	int		m_nSwitch = 0;
	CBurstDlg *m_pDlg00;
	Join* m_pDlg01;
	PointTo* m_pDlg02;


//Construction

//Attributes
public:
	BOOL WriteCommBlock(LPSTR IpByte, DWORD dwBytesToWrite);
	void SetXonOff(BOOL chk);
	BOOL SetupConnection();
	void SetReadData(LPSTR data, int length);
	void SetHwnd(CBurstDlg* pDlg);
	void SetHwnd(Join* pDlg);
	void SetHwnd(PointTo* pDlg);
	void SetDtrRts(BYTE chk);
	void SetComPort(int port, DWORD rate, BYTE bytesize, BYTE stop, BYTE parity);
	int ReadCommBlock(LPSTR lpszBlock, int nMaxLength);
	BOOL OpenComPort();
	BOOL CreateCommInfo();
	BOOL CloseConnection();
	BOOL DestroyComm();

public:
	BYTE	bPort;
	BOOL	fXonXoff;
	BYTE	bByteSize, bFlowCtrl, bParity, bStopBits;
	DWORD	dwBaudRate;
	HANDLE	hWatchThread;
	HWND	hTermWnd;
	DWORD	dwThreadID;
	OVERLAPPED	osWrite, osRead;
};

#endif // !defined(AFX_RS232_H__E827F1BF_3F9E_4844_B2CF_C86F78411B32__INCLUDED_)
