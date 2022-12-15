#pragma once
#include <mysql.h>
#include "rs232.h"


// PointTo 대화 상자

struct PointData
{
	CStringA name;
	CStringA key;
	CStringA dbidx;
	int		finger;
	int		point;

};

class PointTo : public CDialogEx
{
	DECLARE_DYNAMIC(PointTo)

public:
	PointTo(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~PointTo();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POINT };
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

	MYSQL_ROW m_MemberData;
	MYSQL_ROW m_CheckData;
	CStringA m_strCheckInIdx;
	int m_nMemberPoint;


	unsigned char m_RecvBuff[1024] = { 0, };

	CFont m_ftList;
	CFont m_ftBtn;
	CFont m_ftSend;

	CFont m_ftNick;
	CEdit m_edNick;
	CStringA m_strNickName;
	CStringA m_strUID;
	int m_nFingerNum;
	CEdit m_edPoint;
	int m_nPoint;
	CString m_strPoint;


	CListCtrl m_lcList;

	CButton m_bt10P;
	CButton m_bt01P;
	CButton m_bt10M;
	CButton m_bt01M;
	CButton m_btSend;


	CArray<PointData> m_nMember;



	virtual BOOL OnInitDialog();
	void setMemberData(MYSQL_ROW memberdata, MYSQL_ROW checkdata);
	void setMysql(MYSQL* pMysql, RS232* pRs232, int nPort);
	void Receive_RS232(int nLength);
	void addListCtrl(CStringA nick, CStringA point);
	afx_msg void OnBnClickedPointBt01();
	afx_msg void OnBnClickedPointBt02();
	afx_msg void OnBnClickedPointBt03();
	afx_msg void OnBnClickedPointBt04();
	afx_msg void OnBnClickedPointBt05();
	void OnFingerprint(int cmd);
	bool Connect_232(int nPort);
	bool Write_RS232(unsigned char* ch, byte bCnt);

};
