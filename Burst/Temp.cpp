// Temp.cpp: 구현 파일
//

#include "pch.h"
#include "Burst.h"
#include "Temp.h"
#include "afxdialogex.h"


// Temp 대화 상자

IMPLEMENT_DYNAMIC(Temp, CDialogEx)

Temp::Temp(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEMP, pParent)
{

}

Temp::~Temp()
{
}

void Temp::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TEMP_EDIT01, m_edTemp);
    DDX_Control(pDX, IDC_TEMP_BT07, m_btKo);
    DDX_Control(pDX, IDC_TEMP_BT01, m_bt10P);
    DDX_Control(pDX, IDC_TEMP_BT02, m_bt01P);
    DDX_Control(pDX, IDC_TEMP_BT03, m_bt_1P);
    DDX_Control(pDX, IDC_TEMP_BT04, m_bt10M);
    DDX_Control(pDX, IDC_TEMP_BT05, m_bt01M);
    DDX_Control(pDX, IDC_TEMP_BT06, m_bt_1M);
}


BEGIN_MESSAGE_MAP(Temp, CDialogEx)
    ON_BN_CLICKED(IDC_TEMP_BT01, &Temp::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_TEMP_BT02, &Temp::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_TEMP_BT03, &Temp::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_TEMP_BT04, &Temp::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_TEMP_BT05, &Temp::OnBnClickedButton5)
    ON_BN_CLICKED(IDC_TEMP_BT06, &Temp::OnBnClickedButton6)
    ON_BN_CLICKED(IDC_TEMP_BT07, &Temp::OnBnClickedButton7)
END_MESSAGE_MAP()


// Temp 메시지 처리기


BOOL Temp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
    m_ftTemp.CreateFontW(
        70,                  // 문자 폭
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
    m_edTemp.SetFont(&m_ftTemp);
    m_edTemp.SetWindowTextW(_T("36.5"));

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
    m_btKo.SetFont(&m_ftTemp);
    m_btKo.SetWindowTextW(_T("체온 체크"));

    m_bt10P.SetFont(&m_ftTemp);
    m_bt01P.SetFont(&m_ftTemp);
    m_bt_1P.SetFont(&m_ftTemp);
    m_bt10M.SetFont(&m_ftTemp);
    m_bt01M.SetFont(&m_ftTemp);
    m_bt_1M.SetFont(&m_ftTemp);

    m_nTemp = 36.5;
    m_strTemp = _T("36.5");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void Temp::OnBnClickedButton1()
{
    // TODO: 10up
}


void Temp::OnBnClickedButton2()
{
    // TODO: 1up
    m_nTemp += 1;
    CString str;
    str.Format(_T("%2.1f"), m_nTemp);
    m_edTemp.SetWindowTextW(str);
    m_strTemp = str;
}


void Temp::OnBnClickedButton3()
{
    // TODO: 0.1up
    m_nTemp += 0.1;
    CString str;
    str.Format(_T("%2.1f"), m_nTemp);
    m_edTemp.SetWindowTextW(str);
    m_strTemp = str;
}


void Temp::OnBnClickedButton4()
{
    // TODO: 10down
}


void Temp::OnBnClickedButton5()
{
    // TODO: 1down
    m_nTemp -= 1;
    CString str;
    str.Format(_T("%2.1f"), m_nTemp);
    m_edTemp.SetWindowTextW(str);
    m_strTemp = str;
}


void Temp::OnBnClickedButton6()
{
    // TODO: 0.1down
    m_nTemp -= 0.1;
    CString str;
    str.Format(_T("%2.1f"), m_nTemp);
    m_edTemp.SetWindowTextW(str);
    m_strTemp = str;
}


void Temp::OnBnClickedButton7()
{
    // TODO: 확인
    EndDialog(1);
}
