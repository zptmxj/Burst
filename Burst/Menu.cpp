// Menu.cpp: 구현 파일
//

#include "pch.h"
#include "Burst.h"
#include "Menu.h"
#include "afxdialogex.h"
#include "OkClose.h"
#include "PointTo.h"


// Menu 대화 상자

IMPLEMENT_DYNAMIC(Menu, CDialogEx)

Menu::Menu(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MENU, pParent)
{
 
}

Menu::~Menu()
{
}

void Menu::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MENU_EDIT01, m_edNick);
    DDX_Control(pDX, IDC_MENU_BT01, m_btCheckIn);
    DDX_Control(pDX, IDC_MENU_BT02, m_btCheckOut);
    DDX_Control(pDX, IDC_MENU_BT03, m_btPoint);
    DDX_Control(pDX, IDC_MENU_BT04, m_btJoin);
}


BEGIN_MESSAGE_MAP(Menu, CDialogEx)
    ON_BN_CLICKED(IDC_MENU_BT01, &Menu::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_MENU_BT02, &Menu::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_MENU_BT03, &Menu::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_MENU_BT04, &Menu::OnBnClickedButton3)
END_MESSAGE_MAP()


// Menu 메시지 처리기


BOOL Menu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
    m_ftNick.CreateFontW(
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
    m_edNick.SetFont(&m_ftNick);

    m_ftBtn.CreateFontW(
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
    m_btCheckIn.SetFont(&m_ftBtn);
    m_btCheckOut.SetFont(&m_ftBtn);
    m_btPoint.SetFont(&m_ftBtn);
    m_btJoin.SetFont(&m_ftBtn);
    m_btCheckIn.SetWindowTextW(_T("입실"));
    m_btCheckOut.SetWindowTextW(_T("퇴실"));
    m_btPoint.SetWindowTextW(_T("포인트 받기"));
    m_btJoin.SetWindowTextW(_T("회원가입"));

    m_strMemberKey.Format("%s", m_MemberData[1]);

    CStringA strA;
    CString str;
    strA.Format("%s", m_MemberData[4]);
    m_strNickName = strA;
    str = strA;
    m_edNick.SetWindowTextW(str);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void Menu::setMemberData(MYSQL_ROW data)
{
    m_MemberData = data;
}

void Menu::setMysql(MYSQL* pMysql, RS232* pRs232, int nPort)
{
    m_pMYSQL = pMysql;
    m_pCom = pRs232;
    m_nPort = nPort;
}

void Menu::setFingerKey(int nkey)
{
    m_nFingerKey = nkey;
}

void Menu::OnBnClickedButton1()
{
    // TODO: 입실
    CTime TTime = CTime::GetCurrentTime();
    CStringA date;
    CStringA time;
    CStringA yestDay;
    CStringA toDay;
    CStringA uid;

    int getHour = TTime.GetHour();


    if (getHour < 7)
    {
        CTime YTime = TTime + CTimeSpan(-1, 0, 0, 0);
        yestDay.Format("%04d-%02d-%02d", YTime.GetYear(), YTime.GetMonth(), YTime.GetDay());
        toDay.Format("%04d-%02d-%02d", TTime.GetYear(), TTime.GetMonth(), TTime.GetDay());
    }
    else
    {
        CTime TmTime = TTime + CTimeSpan(1, 0, 0, 0);
        yestDay.Format("%04d-%02d-%02d", TTime.GetYear(), TTime.GetMonth(), TTime.GetDay());
        toDay.Format("%04d-%02d-%02d", TmTime.GetYear(), TmTime.GetMonth(), TmTime.GetDay());
    }

    int query_status;
    CStringA query;
    //query.Format("select * from checkin where uid='%s'", m_MemberData[1]);
    //query.Format("select * from checkin where date = '%s'", date);
    //query = "select * from checkin where date = '" + date + "'";

    query.Format("SELECT* FROM checkin WHERE(DATE(date) = %s AND TIME(checkin) > '07:00:00') OR(DATE(DATE) = '%s' AND TIME(checkin) < '07:00:00')", yestDay, toDay);
    TRACE(query+ "\r\n");

    query_status = mysql_query(m_pMYSQL, query.GetBuffer());
    m_Sql_Result = mysql_store_result(m_pMYSQL);
    int field = mysql_num_fields(m_Sql_Result);

    if (query_status == 0 && !(field == 0))
    {
        date = "";
        bool fail = false;

        while ((m_Sql_Row = mysql_fetch_row(m_Sql_Result)) != 0)
        {
            uid.Format("%s", m_Sql_Row[1]);
            TRACE("m_Sql_Row : "+ uid +"\r\n");
            if (uid == m_strMemberKey)
            {
                fail = true;
                break;
            }
        }

   
        OkClose dlg;
        if(fail)
        {
            dlg.setDialog(_T("오늘은 이미 입실처리 되었습니다."), 1);
            dlg.DoModal();
        }
        else
        {
            Temp temp;
            int nPoint = 20;
            int return_value = temp.DoModal();  // 온도 체크
            if (return_value == IDOK)
            {
                query.Format("SELECT* FROM attend WHERE ((DATE(attday) = '%s' AND TIME(atttime) > '07:00:00') OR (DATE(attday) = '%s' AND TIME(atttime) < '07:00:00')) and activity = 1 and uid = %s ", yestDay, toDay, m_MemberData[1]);
                TRACE(query+ "\r\n");
                query_status = mysql_query(m_pMYSQL, query.GetBuffer());
                m_Sql_Result = mysql_store_result(m_pMYSQL);
                int field = mysql_num_fields(m_Sql_Result);
                if (query_status == 0 && !(field == 0) && m_Sql_Result->row_count > 0)
                {
                    CStringA strAhstory;
                    CStringA strA;
                    CString str;
                    CString atTime;
                    CString atH;
                    CString atM;

                    while ((m_Sql_Row = mysql_fetch_row(m_Sql_Result)) != 0)
                    {
                        strAhstory.Format("%s", m_Sql_Row[6]);
                        strA.Format("%s", m_Sql_Row[3]);
                        TRACE(strAhstory + "\r\n");
                        if (strAhstory == "(NULL)" || strAhstory == "(null)")
                        {
                            str = strA;
                            AfxExtractSubString(atH, str, 0, ':');
                            AfxExtractSubString(atM, str, 1, ':');

                            CTime curTime(TTime.GetYear(), TTime.GetMonth(), TTime.GetDay(), TTime.GetHour(), TTime.GetMinute(), 0);
                            CTime attendTime(TTime.GetYear(), TTime.GetMonth(), TTime.GetDay(), _ttoi(atH), _ttoi(atM), 0);

                            if(curTime>attendTime) nPoint = 15;
                        }
                        else
                            nPoint = 10;
                    }
                }
                else
                    nPoint = 5;

                date.Format("%04d-%02d-%02d", TTime.GetYear(), TTime.GetMonth(), TTime.GetDay());
                time.Format("%02d:%02d:%02d", TTime.GetHour(), TTime.GetMinute(), TTime.GetSecond());
                query.Format("insert into `checkin` (`uid`,`name`,`point`,`temp`,`date`,`checkin`) value ('%s','%s', %d, '%2.1f', '%s', '%s')", m_strMemberKey, m_strNickName, nPoint,temp.m_nTemp, date, time);
                query_status = mysql_query(m_pMYSQL, query.GetBuffer());
                if (query_status == 0)
                    dlg.setDialog(_T("입실처리 되었습니다."), 1);
                else
                    dlg.setDialog(_T("데이터 입력실패!!!"), 1);
                dlg.DoModal();
            }
            CDialogEx::OnOK();
        }

    }
    else
    {
        //setTextView(_T("Fail (query_status == 0)"));
    }

}

void Menu::OnBnClickedButton2()
{
    // TODO: 퇴실
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

    query = "select * from checkin where date = '" + date + "'";

    query_status = mysql_query(m_pMYSQL, query.GetBuffer());
    m_Sql_Result = mysql_store_result(m_pMYSQL);
    int field = mysql_num_fields(m_Sql_Result);

    if (query_status == 0 && !(field == 0))
    {
        date = "";
        bool fail = true;

        while ((m_Sql_Row = mysql_fetch_row(m_Sql_Result)) != 0)
        {
            uid.Format("%s", m_Sql_Row[1]);
            if (uid == m_strMemberKey)
            {
                fail = false;
                break;
            }
        }
        OkClose dlg;
        if (fail)
        {
            dlg.setDialog(_T("입실정보가 없습니다."), 1);
            dlg.DoModal();
            //MessageBox(_T("오늘은 이미 입실처리 되었습니다."));
        }
        else
        {
            CStringA idx;
            idx.Format("%s", m_Sql_Row[0]);
            time.Format("%02d:%02d:%02d", cTime.GetHour(), cTime.GetMinute(), cTime.GetSecond());

            query = "update checkin set checkout = '" + time + "' where idx = '" + idx + "'";

            query_status = mysql_query(m_pMYSQL, query.GetBuffer());

            if (query_status == 0)
            {
                dlg.setDialog(_T("퇴실처리 되었습니다."), 1);
            }
            else
                dlg.setDialog(_T("데이터 입력실패!!!"), 1);
            dlg.DoModal();
            CDialogEx::OnOK();
        }
    }
}

void Menu::OnBnClickedButton3()
{
    // TODO: 회원가입



}

void Menu::OnBnClickedButton4()
{
    CTime cTime = CTime::GetCurrentTime();
    bool fail = true;
    int query_status;
    CString str;
    CStringA strA;
    CStringA query;
    CStringA date;
    CStringA time;
    CStringA uid;
    OkClose dlg;

    int sysDay = cTime.GetDay();
    int sysMonth = cTime.GetMonth();
    int sysYear = cTime.GetYear();
    date.Format("%04d-%02d-%02d", sysYear, sysMonth, sysDay);
    strA.Format("%s", m_MemberData[1]);
    
    query = "select * from checkin where date = '" + date + "' and uid = '" + strA + "'";

    query_status = mysql_query(m_pMYSQL, query.GetBuffer());
    m_Sql_Result = mysql_store_result(m_pMYSQL);

    if (query_status == 0)
    {
        int field = mysql_num_fields(m_Sql_Result);
        if ((m_Sql_Row = mysql_fetch_row(m_Sql_Result)) != 0)
        {
            fail = false;
            PointTo pointTo;
            pointTo.setMemberData(m_MemberData, m_Sql_Row);
            pointTo.setMysql(m_pMYSQL, m_pCom, m_nPort);
            if(pointTo.DoModal() == IDOK) CDialogEx::OnOK();
        }
        else
        {
            strA = "인수자의 출석기록을 찾을 수 없습니다.";
            str = strA;
            dlg.setDialog(str, 1);
            dlg.DoModal();
        }
    }


    // TODO: 포인트 받기
    
}



