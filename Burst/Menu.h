#pragma once

#include <mysql.h>
#include "join.h"
#include "Temp.h"

// Menu 대화 상자

class Menu : public CDialogEx
{
	DECLARE_DYNAMIC(Menu)

public:
	Menu(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Menu();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	MYSQL* m_pMYSQL;
	RS232* m_pCom;
	int		m_nPort;

	MYSQL_RES* m_Sql_Result;
	MYSQL_ROW m_Sql_Row;
	int Query_Status;

	CFont m_ftNick;
	CEdit m_edNick;
	CFont m_ftBtn;

	CButton m_btCheckIn;
	CButton m_btCheckOut;
	CButton m_btPoint;
	CButton m_btJoin;

	MYSQL_ROW m_MemberData;

	CStringA m_strMemberKey;
	CStringA m_strNickName;

	int m_nFingerKey;
	void setMemberData(MYSQL_ROW data);
	
	void setFingerKey(int nkey);
	void setMysql(MYSQL* pMysql, RS232* pRs232, int nPort);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
};
