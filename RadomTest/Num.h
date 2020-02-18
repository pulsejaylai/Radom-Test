#pragma once
#include "afxwin.h"


// Num dialog

class Num : public CDialog
{
	DECLARE_DYNAMIC(Num)

public:
	Num(CWnd* pParent = NULL);   // standard constructor
	virtual ~Num();

// Dialog Data
	enum { IDD = IDD_Num };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
   afx_msg CString GetNumber();



   CEdit m_num;
   CString m_num2;
   afx_msg void OnBnClickedOk();
   virtual BOOL OnInitDialog();
};
