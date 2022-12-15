
// BurstDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Burst.h"
#include "BurstDlg.h"
#include "afxdialogex.h"
#include "OkClose.h"
#include "PointTo.h"

#pragma comment(lib, "libmysql.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//#define DB_PORT 3333
#define DB_HOST "168.126.179.44"
//#define DB_USER "sing"
//#define DB_PASS "12345"

#define DB_PORT 302
//#define DB_HOST "192.168.10.5"
#define DB_USER "sing"
#define DB_PASS "12345"

#define DB_NAME "test"
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBurstDlg 대화 상자



CBurstDlg::CBurstDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BURST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBurstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BURST_EDIT01, m_MainView);
	DDX_Control(pDX, IDC_BURST_LIST01, m_lcList);
	DDX_Control(pDX, IDC_BURST_EDIT02, m_edTimeView);
}

BEGIN_MESSAGE_MAP(CBurstDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()

CStringArray m_arrPortName;
CStringArray m_arrDriver;
CArray <int, int> m_arrPortNum;

// CBurstDlg 메시지 처리기

BOOL CBurstDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_ftTime.CreateFontW(
		20,                  // 문자 폭
		0,                   // 문자 높이
		0,                   // 문자 기울기
		0,                   // 문자 방향
		FW_BOLD,           // 문자 굵기
		FALSE,               // 기울기
		FALSE,               // 밑줄
		0,                   // 취소선
		DEFAULT_CHARSET,     // 문자셋
		OUT_DEFAULT_PRECIS,  // 출력 정확도
		CLIP_DEFAULT_PRECIS, // 클리핑 정확도
		DEFAULT_QUALITY,     // 출력의 질
		DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_ftList.CreateFontW(
		20,                  // 문자 폭
		0,                   // 문자 높이
		0,                   // 문자 기울기
		0,                   // 문자 방향
		FW_NORMAL,           // 문자 굵기
		FALSE,               // 기울기
		FALSE,               // 밑줄
		0,                   // 취소선
		DEFAULT_CHARSET,     // 문자셋
		OUT_DEFAULT_PRECIS,  // 출력 정확도
		CLIP_DEFAULT_PRECIS, // 클리핑 정확도
		DEFAULT_QUALITY,     // 출력의 질
		DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	m_edTimeView.SetFont(&m_ftTime);
	CTime cTime = CTime::GetCurrentTime();
	m_strTime.Format(_T("%04d-%02d-%02d\r\n%02d:%02d:%02d"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());
	m_edTimeView.SetWindowTextW(m_strTime);
	m_nHour = cTime.GetHour();

	setTextView(_T("Welcome."));
	bool blDB_Check = false;
	bool blPort_Check = false;
	CStringA strA;
	CString str;
	setTextView(_T(""));
	setTextView(_T("== DB Check."));

	mysql_init(&m_MYSQL);
	m_pMYSQL = mysql_real_connect(&m_MYSQL, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);

	if (m_pMYSQL)
	{
		mysql_set_character_set(m_pMYSQL, "euckr");
		strA.Format("%s \r\n%s", DB_HOST, DB_USER);
		str = strA;
		setTextView(_T("DB Connected."));
		setTextView(str);
		blDB_Check = true;
		mysql_close(m_pMYSQL);
	}
	else
	{
		setTextView(_T("DB Connect Fail"));
	}

	CEnumDevices Enum;

	m_arrPortName.RemoveAll();
	m_arrDriver.RemoveAll();
	m_arrPortNum.RemoveAll();

	Enum.EnumSerialPorts(NULL, CallbackWrapper, 1);

	int i, Cunt = (int)m_arrPortName.GetCount();

	setTextView(_T(""));
	setTextView(_T("== Port Check."));
	for (i = 0; i < Cunt; i++)
	{
		str = m_arrDriver.GetAt(i);
		if (_T("Silicon") == str.Left(7))
		{
			str = m_arrPortName.GetAt(i);
			str.Delete(0, 3);
			m_nPort = _ttoi(str);
			setTextView(_T("Port Connected."));
			str.Format(_T("COM%d."), m_nPort);
			setTextView(str);
			blPort_Check = true;
		}
	}
	if (!blPort_Check)
	{
		setTextView(_T("Port Connect Fail."));
	}

	if (blPort_Check && blDB_Check)
	{
		setTextView(_T(""));
		setTextView(_T("Running..."));

		OnFingerprint(CMD_COMP_I_N);
	}
	else
	{
		setTextView(_T(""));
		setTextView(_T("Running Fail."));
	}

	m_lcList.SetFont(&m_ftList);
	m_lcList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_lcList.InsertColumn(0, _T("idx"), LVCFMT_CENTER, 50); // 첫번째
	m_lcList.InsertColumn(1, _T("Name"), LVCFMT_CENTER, 90); // 첫번째
	m_lcList.InsertColumn(2, _T("Point"), LVCFMT_CENTER, 60); // 두번째
	m_lcList.InsertColumn(3, _T("Check In"), LVCFMT_CENTER, 100); // 세번째
	m_lcList.InsertColumn(4, _T("Check Out"), LVCFMT_CENTER, 100); // 세번째
	m_lcList.InsertColumn(5, _T("Note"), LVCFMT_CENTER, 100); // 세번째

	//m_lcList.InsertItem(0, _T("1"));
	//m_lcList.SetItem(0, 1, LVIF_TEXT, TEXT("test1"), NULL, NULL, NULL, NULL);
	//m_lcList.SetItem(0, 2, LVIF_TEXT, TEXT("test1-1"), NULL, NULL, NULL, NULL);
	//m_lcList.InsertItem(1, _T("2"));
	//m_lcList.SetItem(1, 1, LVIF_TEXT, TEXT("test2"), NULL, NULL, NULL, NULL);

	SetTimer(5001, 1000, NULL);

	if (blDB_Check)
	{
		reflash();
	}

	//Join join;
	//join.setMysql(m_pMYSQL, &m_Com, m_nPort);
	//join.DoModal();

	//PointTo pointTo;
	//pointTo.setMysql(m_pMYSQL, &m_Com, m_nPort);
	//pointTo.DoModal();

	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}
void CBurstDlg::setTextView(CString str)
{
	int nLen = m_MainView.GetWindowTextLength();
	m_MainView.SetFocus();
	m_MainView.SetSel(nLen, nLen);
	m_MainView.ReplaceSel(str+_T("\r\n"));
}


void CBurstDlg::CallbackWrapper(CObject* pObject, const CEnumDevInfo* pInfo)
{
	m_arrPortName.Add(pInfo->m_strPort);
	m_arrDriver.Add(pInfo->m_strName);
	m_arrPortNum.Add(pInfo->m_nPortNum);
}

void CBurstDlg::OnFingerprint(int cmd)
{
	unsigned char cData[8] = { 0xF5, CMD_COMP_I_N, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5 };

	cData[1] = cmd;

	for (int i = 1; i < 6; i++)
		cData[6] ^= cData[i];

	switch (cmd)
	{
	case CMD_COMP_I_N:

		break;
	}


	if (!Connect_232(m_nPort))
	{
		setTextView(_T("Connection Fail"));
		return;
	}

	Write_RS232(cData, 8);
}

bool CBurstDlg::Connect_232(int nPort)
{
	m_Com.CloseConnection();
	//m_Com.SetComPort(nPort, CBR_115200, 8, ONESTOPBIT, NOPARITY);
	m_Com.SetComPort(nPort, CBR_19200, 8, ONESTOPBIT, NOPARITY);
	m_Com.SetXonOff(true);
	m_Com.SetDtrRts(true);
	m_Com.CreateCommInfo();

	if (m_Com.OpenComPort())
	{
		m_Com.SetHwnd(this);
		return true;
	}
	return false;
}

BOOL CBurstDlg::Write_RS232(unsigned char* ch, byte bCnt)
{
	return m_Com.WriteCommBlock((char*)ch, bCnt);
}


void CBurstDlg::Receive_RS232(int nLength)
{
	int n = nLength;
	if (n <= 0) return;

	int i = 0;
	int m_bitidx = 0;
	bool OnData = false;
	for (i; i < n; i++)
	{
		if (m_Com.abln[i] == 0xF5)
			OnData = true;

		if (OnData)
			m_RecvBuff[m_bitidx++] = m_Com.abln[i];

		//str.Format(_T("%02X"), m_Com.abln[i]);	//--> 두자리 헥사 코드..공백 한자리..
	}
	CString str;
	if (0xF5 == m_RecvBuff[0])
	{
		if (m_RecvBuff[1] == CMD_COMP_I_N)
		{
			if (m_RecvBuff[4] < 4)
			{
				mysql_init(&m_MYSQL);
				m_pMYSQL = mysql_real_connect(&m_MYSQL, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);
				mysql_set_character_set(m_pMYSQL, "euckr");

				int query_status;
				CStringA query;

				//query.Format("insert into test01 value ('%d', '%d')", 10, 20);
				query.Format("select * from member where fingerkey='%d'", m_RecvBuff[3]);

				query_status = mysql_query(m_pMYSQL, query.GetBuffer());
				m_Sql_Result = mysql_store_result(m_pMYSQL);

				if (query_status == 0) 
				{
					int field = mysql_num_fields(m_Sql_Result);
					m_Sql_Row = mysql_fetch_row(m_Sql_Result);

					Menu menu;
					menu.setMemberData(m_Sql_Row);
					menu.setMysql(m_pMYSQL, &m_Com, m_nPort);
					menu.DoModal();
					m_nCount = 0;

				}
				else 
				{
					setTextView(_T("Fail (query_status == 0)"));
				}

				reflash();
				
			}
			else if (m_RecvBuff[4] != ACK_SUCCESS)
			{
				m_nCount++;
				if (m_nCount == 5)
				{
					Join join;
					join.setMysql(m_pMYSQL, &m_Com, m_nPort);
					join.DoModal();
					m_nCount = 0;
				}
			}
		}
	}

	OnFingerprint(CMD_COMP_I_N);
}

void CBurstDlg::addListCtrl(CStringA nick, CStringA point, CStringA checkIn, CStringA checkOut, CStringA note)
{
	int idx = m_lcList.GetItemCount();
	CString str;
	str.Format(_T("%d"), idx+1);
	m_lcList.InsertItem(idx, str);
	str = nick;
	m_lcList.SetItem(idx, 1, LVIF_TEXT, str, NULL, NULL, NULL, NULL);
	str = point;
	m_lcList.SetItem(idx, 2, LVIF_TEXT, str, NULL, NULL, NULL, NULL);
	str = checkIn;
	m_lcList.SetItem(idx, 3, LVIF_TEXT, str, NULL, NULL, NULL, NULL);
	str = checkOut;
	m_lcList.SetItem(idx, 4, LVIF_TEXT, str, NULL, NULL, NULL, NULL);
	str = note;
	m_lcList.SetItem(idx, 5, LVIF_TEXT, str, NULL, NULL, NULL, NULL);
}

void CBurstDlg::reflash()
{
	mysql_init(&m_MYSQL);
	m_pMYSQL = mysql_real_connect(&m_MYSQL, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, (char*)NULL, 0);
	mysql_set_character_set(m_pMYSQL, "euckr");
	CTime cTime = CTime::GetCurrentTime();
	CStringA date;
	CStringA time;
	CStringA uid;

	int sysDay = cTime.GetDay();
	int sysMonth = cTime.GetMonth();
	int sysYear = cTime.GetYear();

	date.Format("%04d-%02d-%02d", sysYear, sysMonth, sysDay);
	int query_status;
	CStringA query;
	//query.Format("select * from checkin where uid='%s'", m_MemberData[1]);
	//query.Format("select * from checkin where date = '%s'", date);
	query = "select * from checkin where date = '" + date + "'";

	query_status = mysql_query(m_pMYSQL, query.GetBuffer());
	m_Sql_Result = mysql_store_result(m_pMYSQL);
	int field = mysql_num_fields(m_Sql_Result);

	if (query_status == 0)
	{
		m_lcList.DeleteAllItems();
		while ((m_Sql_Row = mysql_fetch_row(m_Sql_Result)) != 0)
		{
			CStringA nick, point, checkIn, checkOut, note;
			nick.Format("%s", m_Sql_Row[2]);
			point.Format("%s", m_Sql_Row[3]);
			checkIn.Format("%s", m_Sql_Row[6]);
			checkOut.Format("%s", m_Sql_Row[7]);
			note.Format("%s", m_Sql_Row[4]);

			addListCtrl(nick, point, checkIn, checkOut, note);
		}
	}
	else
	{
		m_edTimeView.SetWindowTextW(_T("reflash() DB Error"));
	}
}

void CBurstDlg::openMenu(int number)
{


}


void CBurstDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CBurstDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CBurstDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBurstDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 5001)
	{
		CTime cTime = CTime::GetCurrentTime();
		int getHour = cTime.GetHour();

		m_strTime.Format(_T("%04d-%02d-%02d\r\n%02d:%02d:%02d"), cTime.GetYear(), cTime.GetMonth(), cTime.GetDay(), cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());
		m_edTimeView.SetWindowTextW(m_strTime);

		if (m_nHour != getHour)
		{
			if (getHour == 7)
			{
				reflash();
			}
			m_nHour = getHour;
		}
		
	}
	CDialogEx::OnTimer(nIDEvent);
}
