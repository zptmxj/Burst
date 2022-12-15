#pragma once

#include <mysql.h>
#include "rs232.h"

// join 대화 상자
#define CMD_ADD1		  	0x01
#define CMD_ADD2		  	0x02
#define CMD_ADD3		  	0x03
#define CMD_DEL_USER		0x04
#define CMD_DEL_ALL			0x05


#define ACK_SUCCESS 		0x00
#define ACK_FAIL		  	0x01
#define ACK_FULL		  	0x04
class Join : public CDialogEx
{
	DECLARE_DYNAMIC(Join)

public:
	Join(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Join();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JOIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	MYSQL* m_pMYSQL;
	RS232*	 m_pCom;
	MYSQL_RES* m_Sql_Result;
	MYSQL_ROW m_Sql_Row;

	CFont m_ftKey;
	CFont m_ftBtn;
	CFont m_ftText;
	CEdit m_edKey;
	CString m_strText;
	CButton m_btJoin;

	CString m_strNcik;

	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit1();
	CButton m_btNum00;
	CButton m_btNum01;
	CButton m_btNum02;
	CButton m_btNum03;
	CButton m_btNum04;
	CButton m_btNum05;
	CButton m_btNum06;
	CButton m_btNum07;
	CButton m_btNum08;
	CButton m_btNum09;
	CButton m_btBack;
	void addTextNumber(int nNum);
	afx_msg void OnBnClickedNumber00();
	afx_msg void OnBnClickedNumber01();
	afx_msg void OnBnClickedNumber02();
	afx_msg void OnBnClickedNumber03();
	afx_msg void OnBnClickedNumber04();
	afx_msg void OnBnClickedNumber05();
	afx_msg void OnBnClickedNumber06();
	afx_msg void OnBnClickedNumber07();
	afx_msg void OnBnClickedNumber08();
	afx_msg void OnBnClickedNumber09();
	afx_msg void OnBnClickedButtonBack();
	afx_msg void OnBnClickedButtonOk();
	void setMysql(MYSQL* pMysql, RS232* pRs232, int nPort);
	CEdit m_edNick;
	CProgressCtrl m_pgProgress;
	CEdit m_edText;
	CButton m_btAddnDel;
	unsigned char m_RecvBuff[1024] = { 0, };
	BOOL Write_RS232(unsigned char* ch, byte bCnt);
	bool Connect_232(int nPort);
	void Receive_RS232(int nLength);
	void SendData(int msg);
	int		m_nPort;
	int		m_OnCount = 0;
	bool	m_bCheckF = false;

	int		m_nAddCount = 0;



	void MemberView(CString str);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedDelete();
};
