#pragma once


// OkClose 대화 상자

class OkClose : public CDialogEx
{
	DECLARE_DYNAMIC(OkClose)

public:
	OkClose(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~OkClose();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OK_CLOSE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void setDialog(CString strMain, int nBtn = 2);
	CFont m_ftButton;
	CString m_strMain;
	CFont m_ftTextView;

	CButton m_btOk;
	CRect m_rcOk;
	bool m_bClose = true;
	CButton m_btClose;
	CEdit m_edTextView;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
