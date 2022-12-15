#pragma once


// Temp 대화 상자

class Temp : public CDialogEx
{
	DECLARE_DYNAMIC(Temp)

public:
	Temp(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Temp();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEMP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CFont m_ftTemp;
	CFont m_ftBtn;
	CEdit m_edTemp;
	CButton m_btKo;
	float m_nTemp;
	CString m_strTemp;
	CButton m_bt10P;
	CButton m_bt01P;
	CButton m_bt_1P;
	CButton m_bt10M;
	CButton m_bt01M;
	CButton m_bt_1M;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();

	afx_msg void OnBnClickedButton7();
};
