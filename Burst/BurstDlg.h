
// BurstDlg.h: 헤더 파일
//

#pragma once
#include "rs232.h"
#include "Menu.h"
#include "EnumDevices.h"

#include <mysql.h>

struct Userinfo {
	CStringA name;
	CStringA point;
	CStringA dayin;
	CStringA checkIn;
	CStringA dayout;
	CStringA checkOut;
	CStringA price;
};

// CBurstDlg 대화 상자
class CBurstDlg : public CDialogEx
{
// 생성입니다.
public:
	CBurstDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BURST_DIALOG };
#endif
public:

	unsigned char m_RecvBuff[1024] = { 0, };
	RS232	 m_Com;
	int		m_nPort;
	CString m_strText;
	bool Connect_232(int nPort);
	void Receive_RS232(int nLength);
	BOOL Write_RS232(unsigned char* ch, byte bCnt);
	void setTextView(CString str);
	void OnFingerprint(int idx);

	static void CALLBACK CallbackWrapper(CObject* pObject, const CEnumDevInfo* pInfo);

	MYSQL* m_pMYSQL;
	MYSQL m_MYSQL;
	MYSQL_RES* m_Sql_Result;
	MYSQL_ROW m_Sql_Row;
	int Query_Status;

	int m_nHour;
	int m_nMinute;

	CArray< Userinfo, Userinfo> m_Users;

	//
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	void getQueryData();
	void reflash();
	void openMenu(int number);
	CStringA getUseTime(CStringA dayin, CStringA checkIn, CStringA dayout, CStringA checkOut);
	CTime* getStringToTime(CStringA day, CStringA time);
	void addListCtrl(CStringA nick, CStringA point, CStringA checkIn, CStringA checkOut, CStringA note);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CFont m_ftTime;
	CEdit m_edTimeView;
	CString m_strTime;

	CEdit m_MainView;
	CFont m_ftList;
	CListCtrl m_lcList;

	int		m_nCount;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
