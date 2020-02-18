// Num.cpp : implementation file
//

#include "stdafx.h"
#include "RadomTest.h"
#include "Num.h"
#include "afxdialogex.h"


// Num dialog

IMPLEMENT_DYNAMIC(Num, CDialog)

Num::Num(CWnd* pParent /*=NULL*/)
	: CDialog(Num::IDD, pParent)
	, m_num2(_T(""))
{

}

Num::~Num()
{
}

void Num::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SNum, m_num);
	DDX_Text(pDX, IDC_SNum, m_num2);
}


BEGIN_MESSAGE_MAP(Num, CDialog)
	ON_BN_CLICKED(IDOK, &Num::OnBnClickedOk)
END_MESSAGE_MAP()


// Num message handlers
CString Num::GetNumber()
{
CString number;
//fault="0";
m_num.GetWindowText(number);
AfxMessageBox(number);
return number;
}

void Num::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
CDialog::OnOK();



}


BOOL Num::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_num.SetFocus();
	m_num.SetWindowTextW(_T(""));

	return FALSE;  // return TRUE unless you set the focus to a control

	// EXCEPTION: OCX Property Pages should return FALSE
}
