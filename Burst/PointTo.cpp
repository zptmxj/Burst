// PointTo.cpp: 구현 파일
//

#include "pch.h"
#include "Burst.h"
#include "PointTo.h"
#include "afxdialogex.h"
#include "OkClose.h"

// PointTo 대화 상자

IMPLEMENT_DYNAMIC(PointTo, CDialogEx)

PointTo::PointTo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POINT, pParent)
{

}

PointTo::~PointTo()
{
}

void PointTo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POINT_EDIT01, m_edNick);
	DDX_Control(pDX, IDC_POINT_EDIT02, m_edPoint);
	DDX_Control(pDX, IDC_BURST_LIST01, m_lcList);
	DDX_Control(pDX, IDC_POINT_BT01, m_bt10P);
	DDX_Control(pDX, IDC_POINT_BT02, m_bt01P);
	DDX_Control(pDX, IDC_POINT_BT03, m_bt10M);
	DDX_Control(pDX, IDC_POINT_BT04, m_bt01M);
	DDX_Control(pDX, IDC_POINT_BT05, m_btSend);

}


BEGIN_MESSAGE_MAP(PointTo, CDialogEx)
	ON_BN_CLICKED(IDC_POINT_BT01, &PointTo::OnBnClickedPointBt01)
	ON_BN_CLICKED(IDC_POINT_BT02, &PointTo::OnBnClickedPointBt02)
	ON_BN_CLICKED(IDC_POINT_BT03, &PointTo::OnBnClickedPointBt03)
	ON_BN_CLICKED(IDC_POINT_BT04, &PointTo::OnBnClickedPointBt04)
	ON_BN_CLICKED(IDC_POINT_BT05, &PointTo::OnBnClickedPointBt05)
END_MESSAGE_MAP()


// PointTo 메시지 처리기


BOOL PointTo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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
	m_ftBtn.CreateFontW(
		60,                  // 문자 폭
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
	m_ftSend.CreateFontW(
		40,                  // 문자 폭
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

	m_bt10P.SetFont(&m_ftBtn);
	m_bt01P.SetFont(&m_ftBtn);
	m_bt10M.SetFont(&m_ftBtn);
	m_bt01M.SetFont(&m_ftBtn);

	m_lcList.SetFont(&m_ftList);
	m_lcList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_lcList.InsertColumn(0, _T("Idx"), LVCFMT_CENTER, 50); // 첫번째
	m_lcList.InsertColumn(1, _T("NickName"), LVCFMT_CENTER, 90); // 첫번째
	m_lcList.InsertColumn(2, _T("Point"), LVCFMT_CENTER, 60); // 첫번째

	m_edPoint.SetFont(&m_ftBtn);
	m_edNick.SetFont(&m_ftBtn);
	m_btSend.SetFont(&m_ftSend);

	CStringA strA;
	CString str;
	strA.Format("%s", m_MemberData[1]);
	m_strUID = strA;
	strA.Format("%s", m_MemberData[9]);
	m_nFingerNum = atoi(strA);
	strA.Format("%s", m_MemberData[4]);
	m_strNickName = strA;
	str = strA;
	m_edNick.SetWindowTextW(str);
	m_nPoint = 1;
	str = _T("01");
	m_edPoint.SetWindowTextW(str);
	//CStringA strA;
	//CString str;
	//strA.Format("%s", m_MemberData[6]);
	//m_strNickName = strA;
	//str = strA;
	//m_edNick.SetWindowTextW(str);

	OnFingerprint(CMD_COMP_I_N);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void PointTo::setMemberData(MYSQL_ROW memberdata, MYSQL_ROW checkdata)
{
	CStringA strA;

	m_MemberData = memberdata;
	m_CheckData = checkdata;
	strA.Format("%s", checkdata[0]);
	m_strCheckInIdx = strA;
	strA.Format("%s", checkdata[3]);
	m_nMemberPoint = atoi(strA);
}

void PointTo::setMysql(MYSQL* pMysql, RS232* pRs232, int nPort)
{
	m_nPort = nPort;
	m_pMYSQL = pMysql;
	m_pCom = pRs232;
}


void PointTo::Receive_RS232(int nLength)
{
	int n = nLength;
	if (n <= 0) return;

	int i = 0;
	int m_bitidx = 0;
	bool OnData = false;
	for (i; i < n; i++)
	{
		if (m_pCom->abln[i] == 0xF5)
			OnData = true;

		if (OnData)
			m_RecvBuff[m_bitidx++] = m_pCom->abln[i];
	}
	CString str;
	CStringA strA;
	OkClose dlg;

	bool CMD = false;
	if (0xF5 == m_RecvBuff[0])
	{
		if (m_RecvBuff[1] == CMD_COMP_I_N)
		{
			if (m_RecvBuff[4] == m_nFingerNum)
			{
				strA = "인수 멤버를 추가할 수 없습니다.";
				str = strA;
				dlg.setDialog(str, 1);
				dlg.DoModal();
			}
			else if (m_RecvBuff[4] < 4)
			{
				PointData pd;

				int query_status;
				CStringA query;
				CStringA memberKey;
				CStringA memberNmae;
				CTime cTime = CTime::GetCurrentTime();
				CStringA date;
				CStringA time;
				CStringA uid;
				bool fail = true;

				for (int i = 0; i < m_nMember.GetCount(); i++)
				{
					PointData data = m_nMember.GetAt(i);
					if (data.finger == m_RecvBuff[3])
					{
						strA = "이미 인계리스트에 추가되어 있습니다.";
						str = strA;
						dlg.setDialog(str, 1);
						dlg.DoModal();

						OnFingerprint(CMD_COMP_I_N);
						return;
					}
				}

				query.Format("select * from member where fingerkey='%d'", m_RecvBuff[3]);

				query_status = mysql_query(m_pMYSQL, query.GetBuffer());
				m_Sql_Result = mysql_store_result(m_pMYSQL);

				if (query_status == 0)
				{
					int field = mysql_num_fields(m_Sql_Result);
					m_Sql_Row = mysql_fetch_row(m_Sql_Result);

					memberNmae.Format("%s", m_Sql_Row[4]);
					memberKey.Format("%s", m_Sql_Row[1]);
				}

				int sysDay = cTime.GetDay();
				int sysMonth = cTime.GetMonth();
				int sysYear = cTime.GetYear();
				date.Format("%04d-%02d-%02d", sysYear, sysMonth, sysDay);

				query = "select * from checkin where date = '" + date + "'";

				query_status = mysql_query(m_pMYSQL, query.GetBuffer());
				m_Sql_Result = mysql_store_result(m_pMYSQL);

				if (query_status == 0)
				{
					int field = mysql_num_fields(m_Sql_Result);
					while ((m_Sql_Row = mysql_fetch_row(m_Sql_Result)) != 0)
					{
						uid.Format("%s", m_Sql_Row[1]);
						if (uid == memberKey) {
							fail = false;
							break;
						}
					}

					if (fail)
					{
						strA = "출석 멤버중에 없습니다 [" + memberNmae + ']';
						str = strA;
						dlg.setDialog(str, 1);
						dlg.DoModal();
					}
					else
					{
						pd.name = memberNmae;
						pd.key = memberKey;
						pd.finger = m_RecvBuff[3];
						strA.Format("%s", m_Sql_Row[0]);
						pd.dbidx = strA;
						strA.Format("%s", m_Sql_Row[3]);
						pd.point = atoi(strA);
						m_nMember.Add(pd);
						addListCtrl(pd.name, strA);

					}
				}

			}
		}
	}
	OnFingerprint(CMD_COMP_I_N);
}

void PointTo::OnBnClickedPointBt01()
{
	// TODO: 10 up
	m_nPoint += 10;
	CString str;
	str.Format(_T("%02d"), m_nPoint);
	m_edPoint.SetWindowTextW(str);
	m_strPoint = str;
}


void PointTo::OnBnClickedPointBt02()
{
	// TODO: 1 up
	m_nPoint += 1;
	CString str;
	str.Format(_T("%02d"), m_nPoint);
	m_edPoint.SetWindowTextW(str);
	m_strPoint = str;
}


void PointTo::OnBnClickedPointBt03()
{
	// TODO: 10 Down
	m_nPoint -= 10;
	CString str;
	str.Format(_T("%02d"), m_nPoint);
	m_edPoint.SetWindowTextW(str);
	m_strPoint = str;
}


void PointTo::OnBnClickedPointBt04()
{
	// TODO: 1 Down
	m_nPoint -= 1;
	CString str;
	str.Format(_T("%02d"), m_nPoint);
	m_edPoint.SetWindowTextW(str);
	m_strPoint = str;
}

void PointTo::addListCtrl(CStringA nick, CStringA point)
{
	int idx = m_lcList.GetItemCount();
	CString str;
	str.Format(_T("%d"), idx + 1);
	m_lcList.InsertItem(idx, str);
	str = nick;
	m_lcList.SetItem(idx, 1, LVIF_TEXT, str, NULL, NULL, NULL, NULL);
	str = point;
	m_lcList.SetItem(idx, 2, LVIF_TEXT, str, NULL, NULL, NULL, NULL);
}


void PointTo::OnBnClickedPointBt05()
{
	// TODO: Send
	OkClose dlg;
	CStringA query;
	CString str;
	CStringA strA;
	int query_status;
	int totalPoint = 0;

	bool bFail = false;
	int size = m_nMember.GetCount();
	if (size > 0)
	{
		PointData data;
		for (int i = 0; i < size; i++)
		{
			data = m_nMember.GetAt(i);
			if (data.point < m_nPoint) bFail = true;
		}

		if (!bFail)
		{
			
			for (int i = 0; i < size; i++)
			{
				data = m_nMember.GetAt(i);
				int point = data.point - m_nPoint;
				totalPoint += m_nPoint;
				strA.Format("%d", point);
				query = "update checkin set point = " + strA + " where idx ='" + data.dbidx + "'";

				query_status = mysql_query(m_pMYSQL, query.GetBuffer());
				if (query_status != 0)
				{
					bFail = true;
				}
			}

			if (!bFail)
			{
				totalPoint += m_nMemberPoint;
				strA.Format("%d", totalPoint);
				query = "update checkin set point = " + strA + " where idx ='" + m_strCheckInIdx + "'";
				query_status = mysql_query(m_pMYSQL, query.GetBuffer());
				if (query_status == 0)
				{
					str = _T("점수 정산 성공");
					dlg.setDialog(str, 1);
					dlg.DoModal();
					CDialogEx::OnOK();
				}
			}
			else
			{
				str = _T("점수인계 DB기록 실패");
				dlg.setDialog(str, 1);
				dlg.DoModal();
			}
		}
		else
		{
			str = _T("인계자가 정산할 만큼의 점수가 없습니다.");
			dlg.setDialog(str, 1);
			dlg.DoModal();
		}
	}
	else
	{
		str = _T("점수인계 멤버가 없습니다");
		dlg.setDialog(str, 1);
		dlg.DoModal();
	}

}

void PointTo::OnFingerprint(int cmd)
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
		return;
	}

	Write_RS232(cData, 8);
}

bool PointTo::Connect_232(int nPort)
{
	m_pCom->CloseConnection();
	//m_Com.SetComPort(nPort, CBR_115200, 8, ONESTOPBIT, NOPARITY);
	m_pCom->SetComPort(nPort, CBR_19200, 8, ONESTOPBIT, NOPARITY);
	m_pCom->SetXonOff(true);
	m_pCom->SetDtrRts(true);
	m_pCom->CreateCommInfo();

	if (m_pCom->OpenComPort())
	{
		m_pCom->SetHwnd(this);
		return true;
	}
	return false;
}

bool PointTo::Write_RS232(unsigned char* ch, byte bCnt)
{
	return m_pCom->WriteCommBlock((char*)ch, bCnt);
}
