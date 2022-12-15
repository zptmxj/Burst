// join.cpp: 구현 파일
//

#include "pch.h"
#include "Burst.h"
#include "join.h"
#include "afxdialogex.h"
#include "OkClose.h"


// join 대화 상자

IMPLEMENT_DYNAMIC(Join, CDialogEx)

Join::Join(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_JOIN, pParent)
{

}

Join::~Join()
{
}

void Join::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_JOIN_OK, m_btJoin);
    DDX_Control(pDX, IDC_JOIN_EDIT01, m_edKey);
    DDX_Control(pDX, IDC_JOIN_BT00, m_btNum00);
    DDX_Control(pDX, IDC_JOIN_BT01, m_btNum01);
    DDX_Control(pDX, IDC_JOIN_BT02, m_btNum02);
    DDX_Control(pDX, IDC_JOIN_BT03, m_btNum03);
    DDX_Control(pDX, IDC_JOIN_BT04, m_btNum04);
    DDX_Control(pDX, IDC_JOIN_BT05, m_btNum05);
    DDX_Control(pDX, IDC_JOIN_BT06, m_btNum06);
    DDX_Control(pDX, IDC_JOIN_BT07, m_btNum07);
    DDX_Control(pDX, IDC_JOIN_BT08, m_btNum08);
    DDX_Control(pDX, IDC_JOIN_BT09, m_btNum09);
    DDX_Control(pDX, IDC_JOIN_BACK, m_btBack);
    DDX_Control(pDX, IDC_JOIN_EDIT03, m_edNick);
    DDX_Control(pDX, IDC_PROGRESS1, m_pgProgress);
    DDX_Control(pDX, IDC_JOIN_EDIT02, m_edText);
    DDX_Control(pDX, IDC_JOIN_ADD, m_btAddnDel);
}


BEGIN_MESSAGE_MAP(Join, CDialogEx)
    ON_EN_CHANGE(IDC_JOIN_EDIT01, &Join::OnEnChangeEdit1)
    ON_BN_CLICKED(IDC_JOIN_BT00, &Join::OnBnClickedNumber00)
    ON_BN_CLICKED(IDC_JOIN_BT01, &Join::OnBnClickedNumber01)
    ON_BN_CLICKED(IDC_JOIN_BT02, &Join::OnBnClickedNumber02)
    ON_BN_CLICKED(IDC_JOIN_BT03, &Join::OnBnClickedNumber03)
    ON_BN_CLICKED(IDC_JOIN_BT04, &Join::OnBnClickedNumber04)
    ON_BN_CLICKED(IDC_JOIN_BT05, &Join::OnBnClickedNumber05)
    ON_BN_CLICKED(IDC_JOIN_BT06, &Join::OnBnClickedNumber06)
    ON_BN_CLICKED(IDC_JOIN_BT07, &Join::OnBnClickedNumber07)
    ON_BN_CLICKED(IDC_JOIN_BT08, &Join::OnBnClickedNumber08)
    ON_BN_CLICKED(IDC_JOIN_BT09, &Join::OnBnClickedNumber09)
    ON_BN_CLICKED(IDC_JOIN_BACK, &Join::OnBnClickedButtonBack)
    ON_BN_CLICKED(IDC_JOIN_OK, &Join::OnBnClickedButtonOk)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_JOIN_ADD, &Join::OnBnClickedDelete)
END_MESSAGE_MAP()


// join 메시지 처리기


BOOL Join::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
    m_ftKey.CreateFontW(
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
    m_edKey.SetFont(&m_ftKey);
    m_edNick.SetFont(&m_ftKey);

    m_ftBtn.CreateFontW(
        40,                  // 문자 폭
        0,                   // 문자 높이
        0,                   // 문자 기울기
        0,                   // 문자 방향
        FW_SEMIBOLD,           // 문자 굵기
        FALSE,               // 기울기
        FALSE,               // 밑줄
        0,                   // 취소선
        DEFAULT_CHARSET,     // 문자셋
        OUT_DEFAULT_PRECIS,  // 출력 정확도
        CLIP_DEFAULT_PRECIS, // 클리핑 정확도
        DEFAULT_QUALITY,     // 출력의 질
        DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    m_ftText.CreateFontW(
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

    m_btNum00.SetFont(&m_ftBtn);
    m_btNum01.SetFont(&m_ftBtn);
    m_btNum02.SetFont(&m_ftBtn);
    m_btNum03.SetFont(&m_ftBtn);
    m_btNum04.SetFont(&m_ftBtn);
    m_btNum05.SetFont(&m_ftBtn);
    m_btNum06.SetFont(&m_ftBtn);
    m_btNum07.SetFont(&m_ftBtn);
    m_btNum08.SetFont(&m_ftBtn);
    m_btNum09.SetFont(&m_ftBtn);
    m_btBack.SetFont(&m_ftBtn);
    m_btJoin.SetFont(&m_ftBtn);

    m_edText.SetFont(&m_ftText);
    m_edText.SetWindowTextW(_T("----"));

    m_btAddnDel.SetFont(&m_ftText);

    //SELECT min([seq_field]+1) FROM [table_name] WHERE ([seq_field]+1) NOT IN (SELECT [seq_field] FROM [table_name])
    SetTimer(5002, 300, NULL);

    m_pgProgress.SetRange(0, 100);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void Join::OnEnChangeEdit1()
{
    // TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
    // CDialogEx::OnInitDialog() 함수를 재지정 
    //하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
    // 이 알림 메시지를 보내지 않습니다.
    m_edKey.GetWindowTextW(m_strText);
    if (m_strText.GetLength() > 4)
    {
        m_strText = m_strText.Left(4);
        m_edKey.SetWindowTextW(m_strText);
    }
    // TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void Join::addTextNumber(int nNum)
{
    CString str;
    str.Format(_T("%d"), nNum);
    m_edKey.GetWindowTextW(m_strText);
    m_strText += str;
    m_edKey.SetWindowTextW(m_strText);
}

void Join::OnBnClickedNumber00()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    addTextNumber(0);
}


void Join::OnBnClickedNumber01()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    addTextNumber(1);
}


void Join::OnBnClickedNumber02()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    addTextNumber(2);
}


void Join::OnBnClickedNumber03()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    addTextNumber(3);
}


void Join::OnBnClickedNumber04()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    addTextNumber(4);
}


void Join::OnBnClickedNumber05()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    addTextNumber(5);
}


void Join::OnBnClickedNumber06()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    addTextNumber(6);
}


void Join::OnBnClickedNumber07()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    addTextNumber(7);
}


void Join::OnBnClickedNumber08()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    addTextNumber(8);
}


void Join::OnBnClickedNumber09()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    addTextNumber(9);
}


void Join::OnBnClickedButtonBack()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_edKey.GetWindowTextW(m_strText);
    m_strText.Delete(m_strText.GetLength()-1);
    m_edKey.SetWindowTextW(m_strText);
}

void Join::setMysql(MYSQL* pMysql, RS232* pRs232, int nPort)
{
    m_nPort = nPort;
    m_pMYSQL = pMysql;
    m_pCom = pRs232;
}

void Join::OnBnClickedButtonOk()
{
    //mysql_set_character_set(m_pMYSQL, "euckr");
    m_edKey.GetWindowTextW(m_strText);

    int query_status;
    CStringA query;
    CStringA strA;
    CString str;
    strA = m_strText;

    //query.Format("insert into test01 value ('%d', '%d')", 10, 20);
    query = "select * from member where uid = '" + strA + "'";

    query_status = mysql_query(m_pMYSQL, query.GetBuffer());
    m_Sql_Result = mysql_store_result(m_pMYSQL);

    if (query_status == 0)
    {
        int field = mysql_num_fields(m_Sql_Result);
        m_Sql_Row = mysql_fetch_row(m_Sql_Result);
        if (m_Sql_Row != 0)
        {
            strA.Format("%s", m_Sql_Row[4]);
            m_strNcik = strA;

            m_edNick.SetWindowTextW(m_strNcik);

            //m_OnCount = 2;
        }
    }
    else
    {

    }
}

void Join::MemberView(CString str)

{
    m_edText.SetWindowTextW(str);
}


void Join::Receive_RS232(int nLength)
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

    if (0xF5 == m_RecvBuff[0])
    {
        if (m_RecvBuff[1] == CMD_DEL_USER)
        {
            m_nAddCount = 1;
            SendData(m_nAddCount);
        }
        else if (m_RecvBuff[4] == ACK_SUCCESS)
        {
            if (m_RecvBuff[1] == CMD_ADD1)
            {
                m_pgProgress.SetPos(33);
                m_nAddCount = 2;
                SendData(m_nAddCount);
            }
            else if (m_RecvBuff[1] == CMD_ADD2)
            {
                m_pgProgress.SetPos(66);
                m_nAddCount = 3;
                SendData(m_nAddCount);
            }
            else if (m_RecvBuff[1] == CMD_ADD3)
            {
                m_pgProgress.SetPos(100);
                m_edText.SetWindowTextW(_T("지문등록 완료."));
            }
            else if (m_RecvBuff[1] == CMD_DEL_ALL)
            {
                m_edText.SetWindowTextW(_T("지문정보를 모두 지웠습니다."));
            }
        }
        else
        {
            m_pgProgress.SetPos(0);
            m_edText.SetWindowTextW(_T("지문등록 실패."));

        }
    }
}

BOOL Join::Write_RS232(unsigned char* ch, byte bCnt)
{
    return m_pCom->WriteCommBlock((char*)ch, bCnt);
}

bool Join::Connect_232(int nPort)
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

void Join::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    if (nIDEvent == 5002)
    {
        if (m_OnCount > 1)
            m_OnCount--;
        else if (m_OnCount == 1)
        {
            MemberView(_T("----"));
            m_OnCount = 0;
        }
    }

    CDialogEx::OnTimer(nIDEvent);
}

void Join::SendData(int msg)
{
    unsigned char cData[8] = { 0xF5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5 };
    CStringA strA;
    
    if (msg != 5)
    {
        strA.Format("%s", m_Sql_Row[7]);
        cData[3] = atoi(strA);
        //strA.Format("%s", m_Sql_Row[9]);
        //cData[4] = atoi(strA);
        cData[4] = 1;
    }

    //cData[3] = *m_Sql_Row[5];

    switch (msg)
    {
    case 0:
        cData[1] = CMD_DEL_USER;
        break;
    case 1:
        m_edText.SetWindowTextW(_T("지문을 등록합니다 (1/3)"));
        cData[1] = CMD_ADD1;
        break;
    case 2:
        m_edText.SetWindowTextW(_T("지문을 등록합니다 (2/3)"));
        cData[1] = CMD_ADD2;
        break;
    case 3:
        m_edText.SetWindowTextW(_T("지문을 등록합니다 (3/3)"));
        cData[1] = CMD_ADD3;
        break;
    case 5:

        cData[1] = CMD_DEL_ALL;
        break;
    }

    for (int i = 1; i < 6; i++)
        cData[6] ^= cData[i];

    Write_RS232(cData, 8);
}

void Join::OnBnClickedDelete()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


    OkClose OkDlg;

    if (!Connect_232(m_nPort))
    {
        m_edText.SetWindowTextW(_T("디바이스 연결 실패"));
        return;
    }
    m_bCheckF = false;


    OkDlg.setDialog(_T("기존 지문을 덮고 기록합니다. (Y/N)"));
    if (OkDlg.DoModal() != IDOK) return;
    m_nAddCount = 0;
    SendData(m_nAddCount);

}
