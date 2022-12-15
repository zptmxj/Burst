// OkClose.cpp: 구현 파일
//

#include "pch.h"
#include "Burst.h"
#include "OkClose.h"
#include "afxdialogex.h"


// OkClose 대화 상자

IMPLEMENT_DYNAMIC(OkClose, CDialogEx)

OkClose::OkClose(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OK_CLOSE, pParent)
{

}

OkClose::~OkClose()
{
}

void OkClose::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OKCL_BT01, m_btOk);
	DDX_Control(pDX, IDC_OKCL_BT02, m_btClose);
	DDX_Control(pDX, IDC_OKCL_EDIT01, m_edTextView);
}


BEGIN_MESSAGE_MAP(OkClose, CDialogEx)
    ON_BN_CLICKED(IDC_OKCL_BT01, &OkClose::OnBnClickedOk)
    ON_BN_CLICKED(IDC_OKCL_BT02, &OkClose::OnBnClickedCancel)
END_MESSAGE_MAP()


// OkClose 메시지 처리기


BOOL OkClose::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
    m_ftButton.CreateFontW(
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
    m_btOk.SetFont(&m_ftButton);
    m_btClose.SetFont(&m_ftButton);

    m_ftTextView.CreateFontW(
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
    m_edTextView.SetFont(&m_ftTextView);
    m_edTextView.SetWindowTextW(m_strMain);

    if (!m_bClose)
    {
        m_btClose.ShowWindow(SW_HIDE);
        m_btOk.GetClientRect(m_rcOk);
        //m_rcOk.MoveToX(120);
        m_rcOk.MoveToXY(175,155);
        m_btOk.MoveWindow(m_rcOk);
    }
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void OkClose::setDialog(CString strMain, int nBtn)
{
    m_strMain = strMain;

    if (nBtn != 2)
    {
        m_bClose = false;
    }
}

void OkClose::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CDialogEx::OnOK();
}


void OkClose::OnBnClickedCancel()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CDialogEx::OnCancel();
}
