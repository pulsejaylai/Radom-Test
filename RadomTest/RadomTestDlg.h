
// RadomTestDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "xshdaeButton.h"

// CRadomTestDlg dialog
void ThreadFunc();
#define WM_MYUPDATEDATA WM_USER+100
#define WM_MYUPDATEDATA2 WM_USER+101
#define WM_MYUPDATEDATA3 WM_USER+102
#define WM_MYUPDATEDATA4 WM_USER+103
#define WM_MYUPDATEDATA5 WM_USER+104
class CRadomTestDlg : public CDialogEx
{
// Construction
public:
	CRadomTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RADOMTEST_DIALOG };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
		HANDLE hThread;
 DWORD ThreadID;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg  LRESULT OnUpdateMyData(WPARAM wParam, LPARAM lParam);
	afx_msg  LRESULT OnUpdateMyData2(WPARAM wParam, LPARAM lParam);	
	afx_msg  LRESULT OnUpdateMyData3(WPARAM wParam, LPARAM lParam);	
	afx_msg  LRESULT OnUpdateMyData4(WPARAM wParam, LPARAM lParam);	
	afx_msg  LRESULT OnUpdateMyData5(WPARAM wParam, LPARAM lParam);	
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list1;
	afx_msg void OnFileOpen32771();
	afx_msg void OnClose();
	xShdaeButton m_btn1;
	afx_msg void OnBnClickedTest();
	CListBox m_ListBox1;
	afx_msg void OnBnClickedCheck1();
	CButton m_check1;
	afx_msg void OnBnClickedCancel();
	xShdaeButton m_btn2;
	afx_msg void OnBnClickedStop();
	xShdaeButton m_btn3;
	afx_msg void OnBnClickedSignal();
	xShdaeButton m_btn4;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_statue;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_time;
	afx_msg void OnTesttypeInpout();
	afx_msg void OnTesttypeVinramp();
};
