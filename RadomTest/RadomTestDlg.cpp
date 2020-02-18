
// RadomTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RadomTest.h"
#include "RadomTestDlg.h"
#include "afxdialogex.h"
#include "Gpib.h"
#include "Num.h"
#include <direct.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef int (__stdcall *INP32)(const int ProtAddress);
typedef int (__stdcall *OUT32)(const int ProtAddress, const int Value);
int changenumber,h1,pass,sp2,listhang,testtype,dcefftype,shuntv,shunti,ithdno,sp3;
Num num;
long t11,t12;
float ithdshow;
float *ithdspec;
float *ithdw;
float *OV;
float *zuloadmax;
float *specmax;
float *specmin;
float *noloadmax;
float *noloadmin;
float **unload;
float **daizaif;
float **currecord;
float vin,iin,pin,pout,pf,ithd,eff,vthd,pmax,vrampon,vrampoff,vonmax,vonmin,voffmax,voffmin;
float vonstart,vonend,voffstart,voffend,stept,stepv,trigcl,vontrigv,vofftrigv;
int loadadd[3];
void Load_Int();
int testno,voltno,acadd,dcadd,freqno,signal,selmask,agilent_add;
int *volt;
int *voltrecord;
int *freq;
int *freqrecord;
int filter;
void lineset(int Vin,int Freq);
void Curr_Set(float Curr,int Chan,float Volt);
void PM_Init();
float Meas_Chan_Volt(int Chan);
float Meas_Chan_Curr(int Chan);
float DC_Inp_Curr();
int findstr(char s1[256],char s2[256]);
void Outportb(int Address,int Data);
int port_add,relay,delaytime,pass_item,stoponfail,relay2,relaychangetime,pmadd,vrelay,vrelay2,vrelaychangetime;
int strtohex(char str[256]);
CString Path,path3,path4,number1;
CString strFilePath1,filename;
//char path2[256];
wchar_t *path2 = new wchar_t[256];
typedef struct 
{char Name[25],Message[50];
 float Vin;
 int Freq,Delay,Relaydelay,R1A,R1B,Pause,Pause_Adress;
 float Curr[20],V_H[20],V_L[20];
 //float INP_W_MAX,INP_W_MIN,INP_I_MAX,INP_I_MIN,Eff_MAX,Eff_MIN,PF_MIN,ENERG_MIN;
 }Reg;
  Reg  reg;

 int Run_Reg(Reg );

 typedef struct 
{char Name[25],Message[50],Trig_State[250],Test_State[250];
 float Vin_on_Start,Vin_on_End,Vin_off_Start,Vin_off_End;
 int Freq,Delay,R1A,R1B,Trig_L,Step_Time,Relaydelay;
 float Curr[20],V_H[20],V_L[20],Step_Volt,Trig_Von,Trig_Voff;
 float Vin_on_MAX,Vin_on_MIN,Vin_off_MAX,Vin_off_MIN;
 }Vin_Ramp;
 Vin_Ramp  VinRamp;
int  Run_Vin_Ramp(Vin_Ramp );
 
 
 /*typedef char* (__stdcall *SETINFO)(const char*, const char*);
typedef char* (__stdcall *CHECKROUTING)(const char *, const char *,const char *, const char *);
typedef char* (__stdcall *UPDATEROUTING)(const char *, const char *,const char *, const char *);*/



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTesttypeInpout();
	afx_msg void OnUpdateTesttypeInpout(CCmdUI *pCmdUI);
	afx_msg void OnTesttypeVinramp();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_COMMAND(ID_TESTTYPE_INPOUT, &CAboutDlg::OnTesttypeInpout)
	ON_UPDATE_COMMAND_UI(ID_TESTTYPE_INPOUT, &CAboutDlg::OnUpdateTesttypeInpout)
	ON_COMMAND(ID_TESTTYPE_VINRAMP, &CAboutDlg::OnTesttypeVinramp)
END_MESSAGE_MAP()


// CRadomTestDlg dialog



CRadomTestDlg::CRadomTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRadomTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRadomTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Item, m_list1);
	DDX_Control(pDX, IDC_Test, m_btn1);
	DDX_Control(pDX, IDC_LIST1, m_ListBox1);
	DDX_Control(pDX, IDC_CHECK1, m_check1);
	DDX_Control(pDX, IDC_STOP, m_btn2);
	DDX_Control(pDX, IDCANCEL, m_btn3);
	DDX_Control(pDX, IDC_Signal, m_btn4);
	DDX_Control(pDX, IDC_STATUE, m_statue);
	DDX_Control(pDX, IDC_Time, m_time);
}

BEGIN_MESSAGE_MAP(CRadomTestDlg, CDialogEx)
ON_MESSAGE(WM_MYUPDATEDATA,OnUpdateMyData)
ON_MESSAGE(WM_MYUPDATEDATA2,OnUpdateMyData2)
ON_MESSAGE(WM_MYUPDATEDATA3,OnUpdateMyData3)	
ON_MESSAGE(WM_MYUPDATEDATA4,OnUpdateMyData4)
ON_MESSAGE(WM_MYUPDATEDATA5,OnUpdateMyData5)
ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN32771, &CRadomTestDlg::OnFileOpen32771)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_Test, &CRadomTestDlg::OnBnClickedTest)
	ON_BN_CLICKED(IDC_CHECK1, &CRadomTestDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDCANCEL, &CRadomTestDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_STOP, &CRadomTestDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_Signal, &CRadomTestDlg::OnBnClickedSignal)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_COMMAND(ID_TESTTYPE_INPOUT, &CRadomTestDlg::OnTesttypeInpout)
	ON_COMMAND(ID_TESTTYPE_VINRAMP, &CRadomTestDlg::OnTesttypeVinramp)
END_MESSAGE_MAP()


// CRadomTestDlg message handlers

BOOL CRadomTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	GetDlgItem(IDC_Test)->EnableWindow(FALSE);
	stoponfail=0;
	h1=0;
	 SYSTEMTIME DateTime;
     char data[256],month[256],day[256];//,*month,*day;
	int m50;
	char bufx[256];
	char bufx1[256]="Radom Test ";
	char *equlist;
	Gpib_Init();
	//equlist=Gpiblist();
	//AfxMessageBox(CString(equlist));
	m_btn1.SetShade(SHS_HSHADE,8,20,5,RGB(0,0,255));
	m_btn1.SetIcon(IDI_ICON1,BS_CENTER,IDI_ICON1,IDI_ICON1);	
	m_btn2.SetShade(SHS_HSHADE,8,20,5,RGB(0,0,255));
	m_btn2.SetIcon(IDI_ICON2,BS_CENTER,IDI_ICON2,IDI_ICON2);
	m_btn3.SetShade(SHS_HSHADE,8,20,5,RGB(0,0,255));
	m_btn3.SetIcon(IDI_ICON3,BS_CENTER,IDI_ICON3,IDI_ICON3);
	m_btn4.SetShade(SHS_HSHADE,8,20,5,RGB(0,0,255));
	m_btn4.SetIcon(IDI_ICON4,BS_CENTER,IDI_ICON4,IDI_ICON4);
	LONG lStyle;
      lStyle = GetWindowLong(m_list1.m_hWnd, GWL_STYLE);//获取当前窗口style
      lStyle &= ~LVS_TYPEMASK; //清除显示方式位
      lStyle |= LVS_REPORT; //设置style
      SetWindowLong(m_list1.m_hWnd, GWL_STYLE, lStyle);//设置style
 
      DWORD dwStyle = m_list1.GetExtendedStyle();
      dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
      dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
      dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
      // dwStyle |= LVS_SHOWSELALWAYS;//item前生成checkbox控件
	  m_list1.SetExtendedStyle(dwStyle); //设置扩展风格
	 /* m_list1.InsertColumn( 0, _T("ID"), LVCFMT_LEFT, 40 );//插入列
      m_list1.InsertColumn( 1, _T("NAME"), LVCFMT_LEFT, 50 );
      int nRow = m_list1.InsertItem(0, _T("11"));//插入行
      m_list1.SetItemText(nRow, 1, _T("jacky"));//设置数据*/
	m_list1.InsertColumn(0,_T("Seq"));//添加列   
m_list1.InsertColumn(1,_T("Item"));  
m_list1.InsertColumn(2,_T("Result"));	
/*m_list1.InsertColumn(3,_T("DateCode"));
m_list1.InsertColumn(4,_T("Line"));
m_list1.InsertColumn(5,_T("ProductDate"));
m_list1.InsertColumn(6,_T("Customer"));
m_list1.InsertColumn(7,_T("Supplyer"));
m_list1.InsertColumn(8,_T("WorkOrder"));
m_list1.InsertColumn(9,_T("Quantity"));
m_list1.InsertColumn(10,_T("NGItem"));*/
m_list1.SetColumnWidth(0, 50);//设置列宽   
m_list1.SetColumnWidth(1, 500);  
m_list1.SetColumnWidth(2, 60);//设置列宽   
/*m_list1.SetColumnWidth(3, 150);  
m_list1.SetColumnWidth(4, 150);//设置列宽   
m_list1.SetColumnWidth(5, 150);  
m_list1.SetColumnWidth(6, 150);//设置列宽   
m_list1.SetColumnWidth(7, 150);  
m_list1.SetColumnWidth(8, 150);//设置列宽   
m_list1.SetColumnWidth(9, 150);  
m_list1.SetColumnWidth(10, 150); */
/*for(m50=0;m50<50;m50++)
{
m_list1.InsertItem(m50,_T(""));
_itoa_s(m50+1,bufx,10);
    strcat_s(bufx1,bufx);
m_list1.SetItemText(m50,0,CString(bufx));	
m_list1.SetItemText(m50,1,CString(bufx1));	
strcpy_s(bufx,"");	
strcpy_s(bufx1,"Radom Test ");	
}	
	*/
//m_list1.SetItemState(1,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);	
//m_list1.SetItemState(0,LVIS_SELECTED,LVIS_SELECTED);  
	GetLocalTime(&DateTime);
    sprintf_s(data,"%d",DateTime.wYear);
      if (DateTime.wMonth<10)
   {
	   sprintf_s(month,"0%d",DateTime.wMonth);
    }
   else
   {
	  sprintf_s(month,"%d",DateTime.wMonth);
   }
   //data=data+month+DateTime.wDay;
  if (DateTime.wDay<10)
   {sprintf_s(day,"0%d",DateTime.wDay);
    
   }
   else
   {
	  sprintf_s(day,"%d",DateTime.wDay);
   }
   strcat_s(data,month);
   strcat_s(data,day);
  // data1=data+DateTime.wHour+DateTime.wMinute;
     //dir2=data;
    _mkdir(data);
  GetModuleFileName(NULL,path2,256);
//AfxMessageBox(CString(path2));
  path3=path2;
 path3=path3.Left(path3.ReverseFind('\\')); 	
path4=path3.Right(path3.GetLength()-path3.ReverseFind('\\')-1);	
//AfxMessageBox(path4);	
 CFont *m_font4;
m_font4=new CFont;	

m_font4->CreateFont(45,30,0,0,700,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS, _T("Calibri"));  
//m_font.CreatePointFont(800,"Ш砰");
GetDlgItem(IDC_STATUE)->SetFont(m_font4);		
GetDlgItem(IDC_STATUE)->ShowWindow(SW_HIDE);	
 CFont *m_font5;
m_font5=new CFont;	

m_font5->CreateFont(30,20,0,0,700,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS, _T("Calibri"));  
//m_font.CreatePointFont(800,"Ш砰");
GetDlgItem(IDC_Model)->SetFont(m_font5);		
GetDlgItem(IDC_Model)->ShowWindow(SW_HIDE);		
 
CFont *m_font6;
m_font6=new CFont;	

m_font6->CreateFont(30,20,0,0,700,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS, _T("Calibri"));  
//m_font.CreatePointFont(800,"Ш砰");
GetDlgItem(IDC_Time)->SetFont(m_font6);		
GetDlgItem(IDC_Time)->ShowWindow(SW_HIDE);		

((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);
CMenu* mmenu = GetMenu();
CMenu* submenu = mmenu->GetSubMenu(1);
submenu->EnableMenuItem(ID_TESTTYPE_INPOUT,MF_BYCOMMAND | MF_DISABLED | MF_GRAYED); 
submenu->EnableMenuItem(ID_TESTTYPE_VINRAMP,MF_BYCOMMAND | MF_DISABLED | MF_GRAYED); 
//pPopMenu->EnableMenuItem(ID_MENU_ITEM2,MF_BYCOMMAND | MF_ENABLED);

	
	
	
	//return false;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRadomTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRadomTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRadomTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRadomTestDlg::OnFileOpen32771()
{
	// TODO: Add your command handler code here
	int i,j,m,panduan,m1; 
	char * userc1;
	  char *pnext=NULL;
	char *p2;
	char str4[256]; 
	
	LPCTSTR pp1;
	 CString strFilter,strFilePath,changenumnercs,loadnumber,DLoads,DMaxs,daizai,Dnos,debugdaizais,Dadds,loadadds;
	 CString testnos,volts,acadds,dcadds,specmaxs,specmins,pmadds,pmaxs,ithdnos,ithdspecs,ithdws,filteron;
	 CString noloadmaxs,noloadmins,portadds,relays,delays,freqs,models,dcefftypes,agilents,shuntvs,shuntis,bufs;
	strFilter.Format(_T("Ini Files (*ini)|*ini||"));
	CFileDialog dlg(TRUE, NULL,_T(""), NULL, strFilter);
	DLoads="Load";	
DMaxs="Max";
Dnos="No";
Dadds="Add";	
if(dlg.DoModal()==IDOK)
	{
strFilePath=dlg.GetPathName();	
strFilePath1=strFilePath;
GetPrivateProfileString(_T("Info"),_T("LoadChange"),_T("2"),changenumnercs.GetBuffer(256),256,strFilePath);
changenumnercs.ReleaseBuffer(256);
pp1=changenumnercs.GetBuffer(0);
	changenumnercs.ReleaseBuffer();
	userc1 = new char [changenumnercs.GetLength()+1]; 
	strcpy_s(userc1,changenumnercs.GetLength()+1, CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	changenumber=atoi(userc1);	
	zuloadmax=new float[changenumber];
	for(i=0;i<changenumber;i++)
	{
loadnumber.Format(_T("%d"),i+1);
loadnumber=DLoads+loadnumber+DMaxs;
//AfxMessageBox(loadnumber);
GetPrivateProfileString(_T("Info"),loadnumber,_T("2"),daizai.GetBuffer(256),256,strFilePath);
daizai.ReleaseBuffer(256);
pp1=daizai.GetBuffer(0);
	daizai.ReleaseBuffer();
	userc1 = new char [daizai.GetLength()+1]; 
	strcpy_s(userc1,daizai.GetLength()+1, CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	zuloadmax[i]=atof(userc1);
	}
	unload=new float*[changenumber];	
for(i=0;i<changenumber;i++)
{
unload[i] = new float[20];
}
for(i=0;i<changenumber;i++)
{
for(j=0;j<20;j++)
{
unload[i][j]=zuloadmax[i]/100*5*(j+1);
}	
}
daizaif=new float*[changenumber];	
for(i=0;i<changenumber;i++)
{
daizaif[i] = new float[1000];
}
m=0;
panduan=0;
m1=0;
i=0;
j=0;

do
{
do
{
daizaif[i][j]=zuloadmax[i]/1000*(m1+1);

/*do
{

	if(daizaif[i][j]==unload[i][m])
{
panduan=1;
}
if(daizaif[i][j]!=unload[i][m])
{
	m=m+1;
}
//m=m+1;
}while((panduan==0)&&(m<20));*/
/*if(panduan==1)
{
panduan=0;
m=0;
m1=m1+1;
}*/
//if((m==20)||(m>20))
//{
//m=0;
j=j+1;
m1=m1+1;
//}
//j=j+1;
}while(j<1000);
j=0;
i=i+1;
m1=0;
}while(i<changenumber);	
	

GetPrivateProfileString(_T("Info"),_T("PMax"),_T("1000"),pmaxs.GetBuffer(256),256,strFilePath);
pmaxs.ReleaseBuffer(256);
pp1=pmaxs.GetBuffer(0);
	pmaxs.ReleaseBuffer();
	userc1 = new char [pmaxs.GetLength()+1]; 
	strcpy_s(userc1,pmaxs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	pmax=atof(userc1);	
/*for(i=0;i<changenumber;i++)
{
for(j=0;j<980;j++)
{
debugdaizais.Format(_T("%.3f"),daizaif[i][j]);
AfxMessageBox(debugdaizais);
}
}		
*/	
for(i=0;i<3;i++)	
{
loadnumber.Format(_T("%d"),i+1);
loadnumber=DLoads+loadnumber+Dadds;
GetPrivateProfileString(_T("Info"),loadnumber,_T("2"),loadadds.GetBuffer(256),256,strFilePath);
loadadds.ReleaseBuffer(256);
pp1=loadadds.GetBuffer(0);
	daizai.ReleaseBuffer();
	userc1 = new char [loadadds.GetLength()+1]; 
	strcpy_s(userc1,loadadds.GetLength()+1, CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	loadadd[i]=atoi(userc1);
}
GetPrivateProfileString(_T("Info"),_T("TestNo"),_T("12"),testnos.GetBuffer(256),256,strFilePath);
testnos.ReleaseBuffer(256);	
pp1=testnos.GetBuffer(0);
	testnos.ReleaseBuffer();
	userc1 = new char [testnos.GetLength()+1]; 
	strcpy_s(userc1,testnos.GetLength()+1, CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	testno=atoi(userc1);	
	OV=new float[testno];
	currecord=new float*[300];	
for(i=0;i<300;i++)
{
currecord[i] = new float[testno];
}
	Load_Int();
	voltrecord=new int[300];		
	GetPrivateProfileString(_T("Info"),_T("Volt"),_T("0,0,0,0"),volts.GetBuffer(256),256,strFilePath);
volts.ReleaseBuffer(256);
pp1=volts.GetBuffer(0);
	volts.ReleaseBuffer();
	userc1 = new char [volts.GetLength()+1]; 
	strcpy_s(userc1,volts.GetLength()+1, CT2CA(pp1));
//AfxMessageBox(CString(userc1));
p2=strtok_s(userc1,",",&pnext);
//MessageBox(p,"Title",MB_OK);
//Curr[0]=atof(p2);
//strpath[1]=CString(pnext);
//AfxMessageBox(Curr[0]);
strcpy_s(str4,pnext);
voltno=1;
do{
  p2=strtok_s(str4,",",&pnext);
 // AfxMessageBox(CString(p2));
  if(p2!=NULL)
 {
   //MessageBox(p,"Title",MB_OK);
    //strcpy(cfgpath,p);
	//MessageBox(cfgpath,"Title",MB_OK);  
	 //Curr[y1]=atof(p2);
	 strcpy_s(str4,pnext);
	
   voltno++;
  }
}
 while(p2!=NULL);
 //debugdaizais.Format(_T("%d"),voltno);
//AfxMessageBox(debugdaizais);
volt=new int[voltno];

GetPrivateProfileString(_T("Info"),_T("Volt"),_T("0,0,0,0"),volts.GetBuffer(256),256,strFilePath);
volts.ReleaseBuffer(256);
pp1=volts.GetBuffer(0);
	volts.ReleaseBuffer();
	userc1 = new char [volts.GetLength()+1]; 
	strcpy_s(userc1,volts.GetLength()+1, CT2CA(pp1));
//AfxMessageBox(CString(userc1));
p2=strtok_s(userc1,",",&pnext);
volt[0]=atoi(p2);
//MessageBox(p,"Title",MB_OK);
//Curr[0]=atof(p2);
//strpath[1]=CString(pnext);
//AfxMessageBox(Curr[0]);
strcpy_s(str4,pnext);
i=1;
do{
  p2=strtok_s(str4,",",&pnext);
  //AfxMessageBox(CString(p2));
  if(p2!=NULL)
 {
   //MessageBox(p,"Title",MB_OK);
    //strcpy(cfgpath,p);
	//MessageBox(cfgpath,"Title",MB_OK);  
	 volt[i]=atoi(p2);
	 strcpy_s(str4,pnext);
	
   i++;
  }
}
 while(p2!=NULL);
	
 GetPrivateProfileString(_T("Info"),_T("ITHDNo"),_T("0,0,0,0"),ithdnos.GetBuffer(256),256,strFilePath);
ithdnos.ReleaseBuffer(256);
pp1=ithdnos.GetBuffer(0);
	ithdnos.ReleaseBuffer();
	userc1 = new char [ithdnos.GetLength()+1]; 
	strcpy_s(userc1,ithdnos.GetLength()+1, CT2CA(pp1));
//AfxMessageBox(CString(userc1));
p2=strtok_s(userc1,",",&pnext);
//MessageBox(p,"Title",MB_OK);
//Curr[0]=atof(p2);
//strpath[1]=CString(pnext);
//AfxMessageBox(Curr[0]);
strcpy_s(str4,pnext);
ithdno=1;
do{
  p2=strtok_s(str4,",",&pnext);
 // AfxMessageBox(CString(p2));
  if(p2!=NULL)
 {
   //MessageBox(p,"Title",MB_OK);
    //strcpy(cfgpath,p);
	//MessageBox(cfgpath,"Title",MB_OK);  
	 //Curr[y1]=atof(p2);
	 strcpy_s(str4,pnext);
	
  ithdno++;
  }
}
 while(p2!=NULL);
 //debugdaizais.Format(_T("%d"),voltno);
//AfxMessageBox(debugdaizais);
ithdspec=new float[ithdno+2];
 
GetPrivateProfileString(_T("Info"),_T("ITHDSpec"),_T("0,0,0,0"),ithdspecs.GetBuffer(256),256,strFilePath);
ithdspecs.ReleaseBuffer(256);
pp1=ithdspecs.GetBuffer(0);
	ithdspecs.ReleaseBuffer();
	userc1 = new char [ithdspecs.GetLength()+1]; 
	strcpy_s(userc1,ithdspecs.GetLength()+1, CT2CA(pp1));
//AfxMessageBox(CString(userc1));
p2=strtok_s(userc1,",",&pnext);
ithdspec[0]=atof(p2);
//MessageBox(p,"Title",MB_OK);
//Curr[0]=atof(p2);
//strpath[1]=CString(pnext);
//AfxMessageBox(Curr[0]);
strcpy_s(str4,pnext);
i=1;
do{
  p2=strtok_s(str4,",",&pnext);
  //AfxMessageBox(CString(p2));
  if(p2!=NULL)
 {
   //MessageBox(p,"Title",MB_OK);
    //strcpy(cfgpath,p);
	//MessageBox(cfgpath,"Title",MB_OK);  
	 ithdspec[i]=atof(p2);
	 strcpy_s(str4,pnext);
	
   i++;
  }
}
 while(p2!=NULL);
 /*for(j=0;j<ithdno+1;j++)
{
debugdaizais.Format(_T("%.3f"),ithdspec[j]);
AfxMessageBox(debugdaizais);
}
 */
 
ithdw=new float[ithdno];
GetPrivateProfileString(_T("Info"),_T("ITHDNo"),_T("0,0,0,0"),ithdws.GetBuffer(256),256,strFilePath);
ithdws.ReleaseBuffer(256);
pp1=ithdws.GetBuffer(0);
	ithdws.ReleaseBuffer();
	userc1 = new char [ithdws.GetLength()+1]; 
	strcpy_s(userc1,ithdws.GetLength()+1, CT2CA(pp1));
//AfxMessageBox(CString(userc1));
p2=strtok_s(userc1,",",&pnext);
ithdw[0]=atof(p2);
//MessageBox(p,"Title",MB_OK);
//Curr[0]=atof(p2);
//strpath[1]=CString(pnext);
//AfxMessageBox(Curr[0]);
strcpy_s(str4,pnext);
i=1;
do{
  p2=strtok_s(str4,",",&pnext);
  //AfxMessageBox(CString(p2));
  if(p2!=NULL)
 {
   //MessageBox(p,"Title",MB_OK);
    //strcpy(cfgpath,p);
	//MessageBox(cfgpath,"Title",MB_OK);  
	 ithdw[i]=atof(p2);
	 strcpy_s(str4,pnext);
	
   i++;
  }
}
 while(p2!=NULL); 
/* for(j=0;j<ithdno;j++)
{
debugdaizais.Format(_T("%.3f"),ithdw[j]);
AfxMessageBox(debugdaizais);
}
 */
 
 
 
 
 freqrecord=new int[300];		
	GetPrivateProfileString(_T("Info"),_T("Freq"),_T("0,0,0,0"),freqs.GetBuffer(256),256,strFilePath);
freqs.ReleaseBuffer(256);
pp1=freqs.GetBuffer(0);
	freqs.ReleaseBuffer();
	userc1 = new char [freqs.GetLength()+1]; 
	strcpy_s(userc1,freqs.GetLength()+1, CT2CA(pp1));
//AfxMessageBox(CString(userc1));
p2=strtok_s(userc1,",",&pnext);
//MessageBox(p,"Title",MB_OK);
//Curr[0]=atof(p2);
//strpath[1]=CString(pnext);
//AfxMessageBox(Curr[0]);
strcpy_s(str4,pnext);
freqno=1;
do{
  p2=strtok_s(str4,",",&pnext);
 // AfxMessageBox(CString(p2));
  if(p2!=NULL)
 {
   //MessageBox(p,"Title",MB_OK);
    //strcpy(cfgpath,p);
	//MessageBox(cfgpath,"Title",MB_OK);  
	 //Curr[y1]=atof(p2);
	 strcpy_s(str4,pnext);
	
   freqno++;
  }
}
 while(p2!=NULL);
 //debugdaizais.Format(_T("%d"),freqno);
//AfxMessageBox(debugdaizais);
freq=new int[freqno];

GetPrivateProfileString(_T("Info"),_T("Freq"),_T("0,0,0,0"),freqs.GetBuffer(256),256,strFilePath);
freqs.ReleaseBuffer(256);
pp1=freqs.GetBuffer(0);
	freqs.ReleaseBuffer();
	userc1 = new char [freqs.GetLength()+1]; 
	strcpy_s(userc1,freqs.GetLength()+1, CT2CA(pp1));
//AfxMessageBox(CString(userc1));
p2=strtok_s(userc1,",",&pnext);
//AfxMessageBox(CString(p2));
freq[0]=atoi(p2);
//MessageBox(p,"Title",MB_OK);
//Curr[0]=atof(p2);
//strpath[1]=CString(pnext);
//AfxMessageBox(Curr[0]);
strcpy_s(str4,pnext);
i=1;
do{
  p2=strtok_s(str4,",",&pnext);
 
  if(p2!=NULL)
 {
   //MessageBox(p,"Title",MB_OK);
    //strcpy(cfgpath,p);
	//MessageBox(cfgpath,"Title",MB_OK);  
	// AfxMessageBox(CString(p2));
	 freq[i]=atoi(p2);
	 strcpy_s(str4,pnext);
	
   i++;
  }
}
 while(p2!=NULL); 
 
 
 
 
 
 
 
 
 
 
 
 GetPrivateProfileString(_T("Info"),_T("Von_Start"),_T("220"),bufs.GetBuffer(256),256,strFilePath);
bufs.ReleaseBuffer(256);
pp1=bufs.GetBuffer(0);
	bufs.ReleaseBuffer();
	userc1 = new char [bufs.GetLength()+1]; 
	strcpy_s(userc1,bufs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	vonstart=atof(userc1);	 
GetPrivateProfileString(_T("Info"),_T("Von_End"),_T("220"),bufs.GetBuffer(256),256,strFilePath);
bufs.ReleaseBuffer(256);
pp1=bufs.GetBuffer(0);
	bufs.ReleaseBuffer();
	userc1 = new char [bufs.GetLength()+1]; 
	strcpy_s(userc1,bufs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	vonend=atof(userc1);	  
GetPrivateProfileString(_T("Info"),_T("Von_Max"),_T("220"),bufs.GetBuffer(256),256,strFilePath);
bufs.ReleaseBuffer(256);
pp1=bufs.GetBuffer(0);
	bufs.ReleaseBuffer();
	userc1 = new char [bufs.GetLength()+1]; 
	strcpy_s(userc1,bufs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	vonmax=atof(userc1);	  
GetPrivateProfileString(_T("Info"),_T("Von_Min"),_T("220"),bufs.GetBuffer(256),256,strFilePath);
bufs.ReleaseBuffer(256);
pp1=bufs.GetBuffer(0);
	bufs.ReleaseBuffer();
	userc1 = new char [bufs.GetLength()+1]; 
	strcpy_s(userc1,bufs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	vonmin=atof(userc1);	   
GetPrivateProfileString(_T("Info"),_T("V_StepTime"),_T("220"),bufs.GetBuffer(256),256,strFilePath);
bufs.ReleaseBuffer(256);
pp1=bufs.GetBuffer(0);
	bufs.ReleaseBuffer();
	userc1 = new char [bufs.GetLength()+1]; 
	strcpy_s(userc1,bufs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	stept=atoi(userc1);	    
GetPrivateProfileString(_T("Info"),_T("V_StepVolt"),_T("220"),bufs.GetBuffer(256),256,strFilePath);
bufs.ReleaseBuffer(256);
pp1=bufs.GetBuffer(0);
	bufs.ReleaseBuffer();
	userc1 = new char [bufs.GetLength()+1]; 
	strcpy_s(userc1,bufs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	stepv=atof(userc1);	  
GetPrivateProfileString(_T("Info"),_T("Voff_Start"),_T("220"),bufs.GetBuffer(256),256,strFilePath);
bufs.ReleaseBuffer(256);
pp1=bufs.GetBuffer(0);
	bufs.ReleaseBuffer();
	userc1 = new char [bufs.GetLength()+1]; 
	strcpy_s(userc1,bufs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	voffstart=atof(userc1);	  
GetPrivateProfileString(_T("Info"),_T("Voff_End"),_T("220"),bufs.GetBuffer(256),256,strFilePath);
bufs.ReleaseBuffer(256);
pp1=bufs.GetBuffer(0);
	bufs.ReleaseBuffer();
	userc1 = new char [bufs.GetLength()+1]; 
	strcpy_s(userc1,bufs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	voffend=atof(userc1);	  	
GetPrivateProfileString(_T("Info"),_T("Voff_Max"),_T("220"),bufs.GetBuffer(256),256,strFilePath);
bufs.ReleaseBuffer(256);
pp1=bufs.GetBuffer(0);
	bufs.ReleaseBuffer();
	userc1 = new char [bufs.GetLength()+1]; 
	strcpy_s(userc1,bufs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	voffmax=atof(userc1);	  	
GetPrivateProfileString(_T("Info"),_T("Voff_Min"),_T("220"),bufs.GetBuffer(256),256,strFilePath);
bufs.ReleaseBuffer(256);
pp1=bufs.GetBuffer(0);
	bufs.ReleaseBuffer();
	userc1 = new char [bufs.GetLength()+1]; 
	strcpy_s(userc1,bufs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	voffmin=atof(userc1);	
GetPrivateProfileString(_T("Info"),_T("V_TrigChannl"),_T("1"),bufs.GetBuffer(256),256,strFilePath);
bufs.ReleaseBuffer(256);
pp1=bufs.GetBuffer(0);
	bufs.ReleaseBuffer();
	userc1 = new char [bufs.GetLength()+1]; 
	strcpy_s(userc1,bufs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	trigcl=atoi(userc1);		
GetPrivateProfileString(_T("Info"),_T("Voff_TrigVolt"),_T("3"),bufs.GetBuffer(256),256,strFilePath);
bufs.ReleaseBuffer(256);
pp1=bufs.GetBuffer(0);
	bufs.ReleaseBuffer();
	userc1 = new char [bufs.GetLength()+1]; 
	strcpy_s(userc1,bufs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	vofftrigv=atof(userc1);		
GetPrivateProfileString(_T("Info"),_T("Von_TrigVolt"),_T("3"),bufs.GetBuffer(256),256,strFilePath);
bufs.ReleaseBuffer(256);
pp1=bufs.GetBuffer(0);
	bufs.ReleaseBuffer();
	userc1 = new char [bufs.GetLength()+1]; 
	strcpy_s(userc1,bufs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	vontrigv=atof(userc1);			

	

	
	
GetPrivateProfileString(_T("Info"),_T("AC_Add"),_T("220"),acadds.GetBuffer(256),256,strFilePath);
acadds.ReleaseBuffer(256);
pp1=acadds.GetBuffer(0);
	acadds.ReleaseBuffer();
	userc1 = new char [acadds.GetLength()+1]; 
	strcpy_s(userc1,acadds.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	acadd=atoi(userc1);	
GetPrivateProfileString(_T("Info"),_T("DC_Add"),_T("30"),dcadds.GetBuffer(256),256,strFilePath);
dcadds.ReleaseBuffer(256);
pp1=dcadds.GetBuffer(0);
	dcadds.ReleaseBuffer();
	userc1 = new char [dcadds.GetLength()+1]; 
	strcpy_s(userc1,dcadds.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	dcadd=atoi(userc1);	

GetPrivateProfileString(_T("Info"),_T("PM_Add"),_T("30"),pmadds.GetBuffer(256),256,strFilePath);
pmadds.ReleaseBuffer(256);
pp1=pmadds.GetBuffer(0);
	pmadds.ReleaseBuffer();
	userc1 = new char [pmadds.GetLength()+1]; 
	strcpy_s(userc1,pmadds.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	pmadd=atoi(userc1);	
GetPrivateProfileString(_T("Info"),_T("Filter"),_T("1"),filteron.GetBuffer(256),256,strFilePath);
filteron.ReleaseBuffer(256);
pp1=filteron.GetBuffer(0);
	filteron.ReleaseBuffer();
	userc1 = new char [filteron.GetLength()+1]; 
	strcpy_s(userc1,filteron.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	filter=atoi(userc1);		
	
	PM_Init();
	
GetPrivateProfileString(_T("Info"),_T("Agilent_Add"),_T("3"),agilents.GetBuffer(256),256,strFilePath);
agilents.ReleaseBuffer(256);
pp1=agilents.GetBuffer(0);
	agilents.ReleaseBuffer();
	userc1 = new char [agilents.GetLength()+1]; 
	strcpy_s(userc1,agilents.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	agilent_add=atoi(userc1);		
	
GetPrivateProfileString(_T("Info"),_T("Shunt_V"),_T("100"),shuntvs.GetBuffer(256),256,strFilePath);
shuntvs.ReleaseBuffer(256);
pp1=shuntvs.GetBuffer(0);
	shuntvs.ReleaseBuffer();
	userc1 = new char [shuntvs.GetLength()+1]; 
	strcpy_s(userc1,shuntvs.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	shuntv=atoi(userc1);	
	
GetPrivateProfileString(_T("Info"),_T("Shunt_I"),_T("100"),shuntis.GetBuffer(256),256,strFilePath);
shuntis.ReleaseBuffer(256);
pp1=shuntis.GetBuffer(0);
	shuntis.ReleaseBuffer();
	userc1 = new char [shuntis.GetLength()+1]; 
	strcpy_s(userc1,shuntis.GetLength()+1,CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	shunti=atoi(userc1);		
	
	
	
	specmax=new float[testno];
GetPrivateProfileString(_T("Info"),_T("Spec_Max"),_T("0,0,0,0"),specmaxs.GetBuffer(256),256,strFilePath);
specmaxs.ReleaseBuffer(256);
pp1=specmaxs.GetBuffer(0);
	specmaxs.ReleaseBuffer();
	userc1 = new char [specmaxs.GetLength()+1]; 
	strcpy_s(userc1,specmaxs.GetLength()+1, CT2CA(pp1));
//AfxMessageBox(CString(userc1));
p2=strtok_s(userc1,",",&pnext);
specmax[0]=atof(p2);
//MessageBox(p,"Title",MB_OK);
//Curr[0]=atof(p2);
//strpath[1]=CString(pnext);
//AfxMessageBox(Curr[0]);
strcpy_s(str4,pnext);
i=1;
do{
  p2=strtok_s(str4,",",&pnext);
  //AfxMessageBox(CString(p2));
  if(p2!=NULL)
 {
   //MessageBox(p,"Title",MB_OK);
    //strcpy(cfgpath,p);
	//MessageBox(cfgpath,"Title",MB_OK);  
	 specmax[i]=atof(p2);
	 strcpy_s(str4,pnext);
	
   i++;
  }
}
 while(p2!=NULL);

 specmin=new float[testno];
 GetPrivateProfileString(_T("Info"),_T("Spec_Min"),_T("0,0,0,0"),specmins.GetBuffer(256),256,strFilePath);
specmins.ReleaseBuffer(256);
pp1=specmins.GetBuffer(0);
	specmins.ReleaseBuffer();
	userc1 = new char [specmins.GetLength()+1]; 
	strcpy_s(userc1,specmins.GetLength()+1, CT2CA(pp1));
//AfxMessageBox(CString(userc1));
p2=strtok_s(userc1,",",&pnext);
specmin[0]=atof(p2);
//MessageBox(p,"Title",MB_OK);
//Curr[0]=atof(p2);
//strpath[1]=CString(pnext);
//AfxMessageBox(Curr[0]);
strcpy_s(str4,pnext);
i=1;
do{
  p2=strtok_s(str4,",",&pnext);
  //AfxMessageBox(CString(p2));
  if(p2!=NULL)
 {
   //MessageBox(p,"Title",MB_OK);
    //strcpy(cfgpath,p);
	//MessageBox(cfgpath,"Title",MB_OK);  
	 specmin[i]=atof(p2);
	 strcpy_s(str4,pnext);
	
   i++;
  }
}
 while(p2!=NULL);

noloadmax=new float[testno];
GetPrivateProfileString(_T("Info"),_T("NoLoad_Max"),_T("0,0,0,0"),noloadmaxs.GetBuffer(256),256,strFilePath);
noloadmaxs.ReleaseBuffer(256);
pp1=noloadmaxs.GetBuffer(0);
	noloadmaxs.ReleaseBuffer();
	userc1 = new char [noloadmaxs.GetLength()+1]; 
	strcpy_s(userc1,noloadmaxs.GetLength()+1, CT2CA(pp1));
//AfxMessageBox(CString(userc1));
p2=strtok_s(userc1,",",&pnext);
noloadmax[0]=atof(p2);
//MessageBox(p,"Title",MB_OK);
//Curr[0]=atof(p2);
//strpath[1]=CString(pnext);
//AfxMessageBox(Curr[0]);
strcpy_s(str4,pnext);
i=1;
do{
  p2=strtok_s(str4,",",&pnext);
  //AfxMessageBox(CString(p2));
  if(p2!=NULL)
 {
   //MessageBox(p,"Title",MB_OK);
    //strcpy(cfgpath,p);
	//MessageBox(cfgpath,"Title",MB_OK);  
	 noloadmax[i]=atof(p2);
	 strcpy_s(str4,pnext);
	
   i++;
  }
}
 while(p2!=NULL);

 noloadmin=new float[testno];
 GetPrivateProfileString(_T("Info"),_T("NoLoad_Min"),_T("0,0,0,0"),noloadmins.GetBuffer(256),256,strFilePath);
noloadmins.ReleaseBuffer(256);
pp1=noloadmins.GetBuffer(0);
	noloadmins.ReleaseBuffer();
	userc1 = new char [noloadmins.GetLength()+1]; 
	strcpy_s(userc1,noloadmins.GetLength()+1, CT2CA(pp1));
//AfxMessageBox(CString(userc1));
p2=strtok_s(userc1,",",&pnext);
noloadmin[0]=atof(p2);
//MessageBox(p,"Title",MB_OK);
//Curr[0]=atof(p2);
//strpath[1]=CString(pnext);
//AfxMessageBox(Curr[0]);
strcpy_s(str4,pnext);
i=1;
do{
  p2=strtok_s(str4,",",&pnext);
  //AfxMessageBox(CString(p2));
  if(p2!=NULL)
 {
   //MessageBox(p,"Title",MB_OK);
    //strcpy(cfgpath,p);
	//MessageBox(cfgpath,"Title",MB_OK);  
	 noloadmin[i]=atof(p2);
	 strcpy_s(str4,pnext);
	
   i++;
  }
}
 while(p2!=NULL);
 GetPrivateProfileString(_T("Info"),_T("Port"),_T("340"),portadds.GetBuffer(256),256,strFilePath);
portadds.ReleaseBuffer(256);
pp1=portadds.GetBuffer(0);
	portadds.ReleaseBuffer();
	userc1 = new char [portadds.GetLength()+1]; 
	strcpy_s(userc1,portadds.GetLength()+1, CT2CA(pp1));
	port_add=strtohex(userc1);
//AfxMessageBox(CString(userc1));
  if (strncmp(userc1,"340",3)==0)
  {
Outportb(0x343,0x80);
  }
 if (strncmp(userc1,"380",3)==0)
  {
Outportb(0x383,0x80);
  }
 GetPrivateProfileString(_T("Info"),_T("Relay"),_T("01"),relays.GetBuffer(256),256,strFilePath);
relays.ReleaseBuffer(256);
pp1=relays.GetBuffer(0);
	relays.ReleaseBuffer();
	userc1 = new char [relays.GetLength()+1]; 
	strcpy_s(userc1,relays.GetLength()+1, CT2CA(pp1));
	relay=strtohex(userc1);
GetPrivateProfileString(_T("Info"),_T("Relay2"),_T("01"),relays.GetBuffer(256),256,strFilePath);
relays.ReleaseBuffer(256);
pp1=relays.GetBuffer(0);
	relays.ReleaseBuffer();
	userc1 = new char [relays.GetLength()+1]; 
	strcpy_s(userc1,relays.GetLength()+1, CT2CA(pp1));
	relay2=strtohex(userc1);
	
 GetPrivateProfileString(_T("Info"),_T("V_Relay"),_T("01"),relays.GetBuffer(256),256,strFilePath);
relays.ReleaseBuffer(256);
pp1=relays.GetBuffer(0);
	relays.ReleaseBuffer();
	userc1 = new char [relays.GetLength()+1]; 
	strcpy_s(userc1,relays.GetLength()+1, CT2CA(pp1));
	vrelay=strtohex(userc1);
GetPrivateProfileString(_T("Info"),_T("V_Relay2"),_T("01"),relays.GetBuffer(256),256,strFilePath);
relays.ReleaseBuffer(256);
pp1=relays.GetBuffer(0);
	relays.ReleaseBuffer();
	userc1 = new char [relays.GetLength()+1]; 
	strcpy_s(userc1,relays.GetLength()+1, CT2CA(pp1));
	vrelay2=strtohex(userc1);
	
GetPrivateProfileString(_T("Info"),_T("V_Relaydelay"),_T("3000"),delays.GetBuffer(256),256,strFilePath);
delays.ReleaseBuffer(256);
pp1=delays.GetBuffer(0);
	delays.ReleaseBuffer();
	userc1 = new char [delays.GetLength()+1]; 
	strcpy_s(userc1,delays.GetLength()+1, CT2CA(pp1));
vrelaychangetime=atoi(userc1);	
	
	
	GetPrivateProfileString(_T("Info"),_T("Delay"),_T("3000"),delays.GetBuffer(256),256,strFilePath);
delays.ReleaseBuffer(256);
pp1=delays.GetBuffer(0);
	delays.ReleaseBuffer();
	userc1 = new char [delays.GetLength()+1]; 
	strcpy_s(userc1,delays.GetLength()+1, CT2CA(pp1));
delaytime=atoi(userc1);	
GetPrivateProfileString(_T("Info"),_T("Relaydelay"),_T("3000"),delays.GetBuffer(256),256,strFilePath);
delays.ReleaseBuffer(256);
pp1=delays.GetBuffer(0);
	delays.ReleaseBuffer();
	userc1 = new char [delays.GetLength()+1]; 
	strcpy_s(userc1,delays.GetLength()+1, CT2CA(pp1));
relaychangetime=atoi(userc1);	

GetPrivateProfileString(_T("Info"),_T("Model"),_T("Radom Test"),models.GetBuffer(256),256,strFilePath);
models.ReleaseBuffer(256);
pp1=models.GetBuffer(0);
	models.ReleaseBuffer();
	userc1 = new char [models.GetLength()+1]; 
	strcpy_s(userc1,models.GetLength()+1, CT2CA(pp1));
//relaychangetime=atoi(userc1);	
//m_ListBox1.GetCurSel();
GetPrivateProfileString(_T("Info"),_T("DC_EffType"),_T("34401"),dcefftypes.GetBuffer(256),256,strFilePath);
dcefftypes.ReleaseBuffer(256);
if(dcefftypes=="34401")	
{
	dcefftype=1;
}
if(dcefftypes=="66202")
{
dcefftype=2;
}

int num = MultiByteToWideChar(0,0,userc1,-1,NULL,0);
wchar_t *wide = new wchar_t[num];
MultiByteToWideChar(0,0,userc1,-1,wide,num);
	GetDlgItem(IDC_Model)->ShowWindow(FALSE);
	GetDlgItem(IDC_Model)->SetWindowText(wide);
	GetDlgItem(IDC_Model)->ShowWindow(TRUE);


GetDlgItem(IDC_STATUE)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATUE)->SetWindowText(_T("Ready"));
	GetDlgItem(IDC_STATUE)->ShowWindow(TRUE);


}
	
CMenu* mmenu = GetMenu();
CMenu* submenu = mmenu->GetSubMenu(1);
submenu->EnableMenuItem(ID_TESTTYPE_INPOUT,MF_BYCOMMAND | MF_ENABLED); 	
submenu->EnableMenuItem(ID_TESTTYPE_VINRAMP,MF_BYCOMMAND | MF_ENABLED); 	
GetDlgItem(IDC_Test)->EnableWindow(TRUE);	

}


	void CRadomTestDlg::OnClose()
	{
		// TODO: Add your message handler code here and/or call default
int i=0;
		delete []zuloadmax;	
for (int i=0;i<changenumber;i++) 
{
delete[] daizaif[i]; //先撤销指针元素所指向的数组
}                      
delete[] daizaif; //（别人的一个例子，我改得更易读点）。
for (int i=0;i<changenumber;i++) 
{
delete[] unload[i]; //先撤销指针元素所指向的数组
}                      
delete[] unload; //（别人的一个例子，我改得更易读点）。		
delete []volt;			
delete []specmax;		
delete []specmin;		
delete []ithdspec;
delete []ithdw;
delete []noloadmax;		
delete []noloadmin;		
delete []OV;			
delete []voltrecord;		
for (int i=0;i<listhang;i++) 
{
delete[] currecord[i]; //先撤销指针元素所指向的数组
}                 	
delete[] currecord;

CDialogEx::OnClose();
	}


void Load_Int()
{
 char L_AD[256]="GPIB0::";
 char L_AD1[256]="::INSTR";
 char buf1[256],buf[256];
 int i,m,testno1,Load_No,n;
 char reading[256];
for(i=0;i<3;i++)
 {
  if(loadadd[i]!=0)
  {
  _itoa_s(loadadd[i],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
   //AfxMessageBox(CString(L_AD));
	Gpread(L_AD,"*IDN?",reading);
  if (strncmp(reading,"Chroma,632",10)==0)
 {
  Gpwrite(L_AD, "MODE CCH;:LOAD ON;:CURR:STAT:L1 0.00;:MEAS:INP UUT");
 h1=1;
 }
   }
    strcpy_s(L_AD,"GPIB0::");
 }

if(h1==1)
 {
  testno1=testno-1;
  if(testno1%4>0)
 {
	 Load_No=testno1/4+1;
  }
  else
  { 
	  Load_No=testno1/4;
  }
  for(i=0;i<Load_No;i++)
  { 
	  _itoa_s(loadadd[i+1],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
  Gpwrite(L_AD,"*CLS");
 
    for(n=0;n<4;n++)
    {
		m=n*2+1;
     sprintf_s(buf,"chan %d",m);
     Gpwrite(L_AD, buf);
     Gpwrite(L_AD, "MODE CCH;:LOAD ON;:CURR:STAT:L1 0.00;:MEAS:INP UUT");


    }
    strcpy_s(L_AD,"GPIB0::");
    }
  


 }

if(h1!=1)
{

  if(testno%4>0)
 {
 Load_No=testno/4+1;
  }
  else
  {
  Load_No=testno/4;
  }

 for(i=0;i<Load_No;i++)
  {
  _itoa_s(loadadd[i],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
  Gpwrite(L_AD,"*CLS");
     // MessageBox(NULL,L_AD,"ERROR",MB_OK);
    for(n=0;n<4;n++)
    {
    m=n*2+1;
     sprintf_s(buf,"chan %d",m);
     Gpwrite(L_AD, buf);
     Gpwrite(L_AD, "MODE CCH;:LOAD ON;:CURR:STAT:L1 0.00;:MEAS:INP UUT");
     //Gpwrite(L_AD, "LOAD ON");
     //Gpwrite(L_AD, "CURR:STAT:L1 0.00");
     //Gpwrite(L_AD, "MEAS:INP UUT");

    }
    strcpy_s(L_AD,"GPIB0::");
    }


    }








}

void PM_Init()
{
char reading[256];
char POW1[256]="GPIB0::";
char POW3[256]="::INSTR";
char POW2[256];
 _itoa_s(pmadd,POW2,10);
strcat_s(POW1,POW2);
  strcat_s(POW1,POW3);
   Gpread(POW1,"*IDN?",reading);
     if (strncmp(reading,"Chroma ATE,66202",16)==0)
	 {
 Gpwrite(POW1,"VOLT:RANG AUTO;:CURR:RANG AUTO;:CURR:SHUN AUTO;:MEAS:MODE WINDOW;:MEAS:WIND 2.5");
 Gpwrite(POW1,"THD:MODE ORDER;:THD:ORD 40");
	if(filter==1)
	{
 Gpwrite(POW1,"FILT ON");
	}
	if(filter==0)
	{
 Gpwrite(POW1,"FILT OFF");
	} 
	 }


}





void lineset(int Vin,int Freq)
{
char buf[256];
 char buf1[256];
 char buf2[256];  
 char DC[256]="GPIB0::";
 char DC1[256]="::INSTR";
 char AC[256]="GPIB0::";
 char AC1[256]="::INSTR";
 char reading[256];
 _itoa_s(dcadd,buf1,10);
 strcat_s(DC,buf1);
 strcat_s(DC,DC1);
 _itoa_s(acadd,buf,10);
 strcat_s(AC,buf);
 strcat_s(AC,AC1);
 if(dcadd==0)
 {
 sprintf_s(buf2,"VOLT:%d FREQ:%5d" ,Vin,Freq);
Gpwrite(AC,buf2);
Gpwrite(AC,"OUTP:ON");
                                     }

 else
 {
Gpread(DC,"*IDN?",reading);
if ((findstr(reading,"62050")!=-1)||(findstr(reading,"62012")!=-1))
     {
     sprintf_s(buf2,"SOUR:VOLT %d",Vin);
     Gpwrite(DC,buf2);
     sprintf_s(buf2,"SOUR:CURR %5d",Freq);
     Gpwrite(DC,buf2);
     Gpwrite(DC,"CONF:OUTP ON");
        }
   else
   {
   sprintf_s(buf2,":VOLT %d;:CURR %5d" ,Vin,Freq);
   Gpwrite(DC,buf2);
Gpwrite(DC,"OUT ON");
 }
 
 
 }



}


int findstr(char s1[256],char s2[256])
 { int len1,len2,a,b,c,d;
    len1=strlen(s1);
    len2=strlen(s2);
    if (len1-len2<0)
    {return -1;}
   else
    {
    for(a=0;a<=(len1-len2);a++)
    {  b=a;
    for(c=0;c<len2;c++)
    {  if (s1[b]!=s2[c])
       { break;
       }
       b=b+1;
       }

      if (c==len2)
      {break;}
      }
       if(a>(len1-len2))
      {return -1;}
       else
        {return a;}
       }

}

void CRadomTestDlg::OnBnClickedTest()
{
	// TODO: Add your control notification handler code here
	//m_list1.EnsureVisible(0,FALSE);
   // m_list1.SetItemState(0,LVIS_SELECTED,LVIS_SELECTED);
   // m_list1.SetSelectionMark(0); 
	//SetTimer(1,10,NULL); 
	/*CString type;
	type=m_list1.GetItemText(0,1);
AfxMessageBox(type);*/
	int nHeadNum; 
	nHeadNum=m_list1.GetItemCount();
	signal=0;
	 if(num.DoModal()==IDOK);
	 {
	// number1=num.GetNumber();
	//AfxMessageBox(num.m_num2);
	 number1=num.m_num2;
	 }
	//AfxMessageBox(number1);
	if(number1!="")
	{
		t11=GetTickCount();
	listhang=m_list1.GetItemCount();
	GetDlgItem(IDC_Time)->ShowWindow(SW_HIDE);	
	int ip;
	GetDlgItem(IDC_Test)->EnableWindow(FALSE);	
	m_ListBox1.ResetContent();
	GetDlgItem(IDC_STATUE)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATUE)->SetWindowText(_T("Testing"));
	GetDlgItem(IDC_STATUE)->ShowWindow(TRUE);
	for(ip=0;ip<nHeadNum;ip++)
	{
m_list1.SetItemText(ip,2,_T(""));
m_list1.SetItemState(ip,0,LVIS_SELECTED|LVIS_FOCUSED);	
	}
		stoponfail=0;
	if (BST_CHECKED==IsDlgButtonChecked( IDC_CHECK1 ) )
{
stoponfail=1;
}
	pass=1;
pass_item=1;
	hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFunc,NULL,0,&ThreadID);
	}
if(number1=="")
{
GetDlgItem(IDC_STATUE)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATUE)->SetWindowText(_T("Stop"));
	GetDlgItem(IDC_STATUE)->ShowWindow(TRUE);

}

}


void Outportb(int Address,int Data)
 { INP32 Inp32;
    OUT32 Out32;

    HINSTANCE hDll = ::LoadLibrary(_T("e:\\INPOUT32.dll"));
     if (hDll)
    {
        Inp32 = (INP32)::GetProcAddress(hDll, "Inp32");
        Out32 = (OUT32)::GetProcAddress(hDll, "Out32");

        if (!Inp32)
        {
           /* ::MessageBox(0, TEXT("Inp32 ERROR"),
                    TEXT("Error"), MB_OK | MB_ICONERROR);*/

            ::FreeLibrary(hDll);

            return;
            // ...
        }
        if (!Out32)
        {
           // AfxMessageBox(_T("InpOut32dll err"));

            ::FreeLibrary(hDll);

            return;
        }
       
        Out32(Address,Data);
     

    }
   if (!hDll)
     { //AfxMessageBox(_T("InpOut32dll err"));
       ::FreeLibrary(hDll);
    }
     ::FreeLibrary(hDll);

     }

int strtohex(char str[256])
 {int result;
  int i;
  int num[56];
  int count;
  char buf[256];
  result=0;
  count=1;
  for(i=strlen(str)-1;i>=0;i--)
  {if((str[i]>='0')&&(str[i]<='9'))
    {num[i]=str[i]-48;
      //itoa(num[i],buf,10);
     //MessageBox(NULL,buf,"ERROR",MB_OK);

     }
    if((str[i]>='a')&&(str[i]<='f'))
    {num[i]=str[i]-'a'+10; }
     if((str[i]>='A')&&(str[i]<='F'))
    {num[i]=str[i]-'A'+10; }

    result=result+num[i]*count;
    count=count*16;
    //itoa(result,buf,10);
     //MessageBox(NULL,buf,"ERROR",MB_OK);
    }
   // itoa(result,buf,10);
    // MessageBox(NULL,buf,"ERROR",MB_OK);
    return(result);
    }
void ThreadFunc()
{
	//AfxMessageBox(_T("tt"));
	char bufxx[256]="";
	char bufxx1[256]="Radom Test----Inpout";
	char bufxx2[256]="Radom Test----VinRamp";
	char bufload[256]="Load:";
	char bufv[256]="";
	char buff[256]="";
	char buf2[256]="";
	char bufmax[256]="V_Max :";
	//char bufmax[256]="V_Max :";
	
	
	char bufmin[256]="V_Min :";
   char bufmean[256]="V_Mean:";
	char bufmz[256]="";
   char testitems[256];
   int i,j,suijidebug1,testitem,SelMark,iend;
int m2,m3,currno,j1,sp;
float suijidebug;
float **actload;
float *curr;
char nitex[256];
int *actvolt;
int *actfreq;
char * userc1;
	  char *pnext=NULL;
	char *p2;
LPCTSTR pp1;
CString loadnumber1,DLoads,Dnos,currnos,debugdaizais,loadbuf;
HWND hwnd; //希望找到资源管理器
hwnd=::FindWindow(NULL,_T("RadomTest"));
//loadbuf="Load:";
DLoads="Load";	
Dnos="No";
actload=new float*[changenumber];	
for(i=0;i<changenumber;i++)
{
actload[i] = new float[listhang];
}

i=0;
j=0;
srand((unsigned)time(NULL));
do
{
//AfxMessageBox(_T("tt"));
	do
{

	suijidebug=rand()/float(RAND_MAX+1.0)*980;
	//AfxMessageBox(_T("89"));
	suijidebug1=int(suijidebug);
/*debugdaizais.Format(_T("%d"),suijidebug1);
AfxMessageBox(debugdaizais);*/
if(suijidebug1!=1000)
{
	actload[i][j]=daizaif[i][suijidebug1];
j=j+1;
}
}while(j<listhang);

j=0;
i=i+1;

}while(i<changenumber);


j=0;
actvolt=new int[listhang];
srand((unsigned)time(NULL));
do
{
suijidebug=rand()/float(RAND_MAX+1.0)*voltno;
suijidebug1=int(suijidebug);
/*debugdaizais.Format(_T("%d"),suijidebug1);
AfxMessageBox(debugdaizais);*/
actvolt[j]=volt[suijidebug1];


j=j+1;
}while(j<listhang);

/*for(j=0;j<freqno;j++)
{
debugdaizais.Format(_T("%d"),freq[j]);
AfxMessageBox(debugdaizais);
}*/
j=0;
actfreq=new int[listhang];
srand((unsigned)time(NULL));
do
{
suijidebug=rand()/float(RAND_MAX+1.0)*freqno;
suijidebug1=int(suijidebug);
/*debugdaizais.Format(_T("%d"),freq[suijidebug1]);
AfxMessageBox(debugdaizais);*/
actfreq[j]=freq[suijidebug1];


j=j+1;
}while(j<listhang);





/*for(j=0;j<50;j++)
{
debugdaizais.Format(_T("%d"),actvolt[j]);
AfxMessageBox(debugdaizais);
}*/
/*for(j=0;j<listhang;j++)
{
debugdaizais.Format(_T("%d"),actfreq[j]);
AfxMessageBox(debugdaizais);
}
*/
lineset(actvolt[0],actfreq[0]);


  //Outportb(0x343,0x80);

Outportb(port_add,0x00);
 delay(relaychangetime);
 Outportb(port_add,relay2);

curr=new float[testno];
if(signal==0)
{
	sp2=0;
	
	for(testitem=0;testitem<listhang;testitem++)
{
	_itoa_s(testitem,nitex,10);
	SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA2,  // 发送自己的窗口消息   
           (WPARAM)&nitex,     // 设置发送的内容   
           NULL);
	//sp2=sp2+1;
	m2=0;
	for(i=0;i<changenumber;i++)
{
loadnumber1.Format(_T("%d"),i+1);
loadnumber1=DLoads+loadnumber1+Dnos;
//AfxMessageBox(loadnumber);
GetPrivateProfileString(_T("Info"),loadnumber1,_T("2"),currnos.GetBuffer(256),256,strFilePath1);
currnos.ReleaseBuffer(256);
pp1=currnos.GetBuffer(0);
	currnos.ReleaseBuffer();
	userc1 = new char [currnos.GetLength()+1]; 
	strcpy_s(userc1,currnos.GetLength()+1, CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	currno=atoi(userc1);
for(j=0;j<currno;j++)
{
curr[m2]=actload[i][testitem]/currno;
m2=m2+1;
}



}
for(m3=m2;m3<testno;m3++)
{
curr[m3]=0;
}
for(j1=0;j1<testno;j1++)
{
	currecord[testitem][j1]=curr[j1];
//debugdaizais.Format(_T("%.3f"),curr[j1]);
//AfxMessageBox(debugdaizais);
}

//sp=m_ListBox1.GetCurSel();
_itoa_s(testitem,testitems,10);
//AfxMessageBox(CString(testitems));
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA5,  // 发送自己的窗口消息   
           (WPARAM)&testitems,     // 设置发送的内容   
           NULL);
if(testtype==1)
{

_itoa_s(testitem+1,bufxx,10);
    strcat_s(bufxx1,bufxx);
	//AfxMessageBox(CString(bufxx1));
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&bufxx1,     // 设置发送的内容   
           NULL);
sp3=sp2;
sp2=sp2+1;
voltrecord[testitem]=actvolt[testitem];
freqrecord[testitem]=actfreq[testitem];
reg.Vin=actvolt[testitem];
reg.Delay=delaytime;
reg.Relaydelay=relaychangetime;
reg.Freq=actfreq[testitem];
reg.R1A=relay;
reg.R1B=relay2;
sprintf_s(bufv,"%dV/",actvolt[testitem]);
sprintf_s(buff,"%dHz",actfreq[testitem]);
strcat_s(bufv,buff);
//AfxMessageBox(CString(bufv));
for(j1=0;j1<testno;j1++)
{
	reg.Curr[j1]=curr[j1];
	sprintf_s(buf2," %.3f",curr[j1]);
	strcat_s(bufload,buf2);
	//debugdaizais.Format(_T("%.3f"),curr[j1]);
//AfxMessageBox(debugdaizais);
}
for(j1=0;j1<testno;j1++)
{
	reg.V_H[j1]=specmax[j1];
	sprintf_s(buf2," %.3f",specmax[j1]);
	strcat_s(bufmax,buf2);
	//debugdaizais.Format(_T("%.3f"),curr[j1]);
//AfxMessageBox(debugdaizais);
}
for(j1=0;j1<testno;j1++)
{
	reg.V_L[j1]=specmin[j1];
	sprintf_s(buf2," %.3f",specmin[j1]);
	strcat_s(bufmin,buf2);
//debugdaizais.Format(_T("%.3f"),curr[j1]);
//AfxMessageBox(debugdaizais);
}
//AfxMessageBox(CString(bufload));
/*SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA2,  // 发送自己的窗口消息   
           NULL,     // 设置发送的内容   
           NULL);*/
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&bufv,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&bufload,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&bufmax,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&bufmin,     // 设置发送的内容   
           NULL);
 sp2=sp2+1;
pass=Run_Reg(reg);

	for(j1=0;j1<testno;j1++)
{
	//reg.V_L[j1]=specmin[j1];
	sprintf_s(buf2," %.3f",OV[j1]);
	strcat_s(bufmean,buf2);
//debugdaizais.Format(_T("%.3f"),curr[j1]);
//AfxMessageBox(debugdaizais);
}
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&bufmean,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
sprintf_s(buf2,"Mean Vin: %.3f Iin: %.3f Pin: %.3f Pout: %.3f Eff: %.4f PF: %.3f Vthd: %.3f",vin,iin,pin,pout,eff,pf,vthd);
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&buf2,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
sprintf_s(buf2,"ITHD SPEC: %.3f",ithdshow);
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&buf2,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
sprintf_s(buf2,"ITHD MEAN: %.3f",ithd);
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&buf2,     // 设置发送的内容   
           NULL);
sp2=sp2+1;


strcpy_s(bufxx,"");	
strcpy_s(bufxx1,"Radom Test----Inpout");	
strcpy_s(bufload,"Load:");	
  strcpy_s(bufmax,"V_Max :");	
  strcpy_s(bufmin,"V_Min :");
 strcpy_s(bufmean,"V_Mean:");
strcpy_s(bufv,"");
strcpy_s(buff,"");

// if(pass==0)
//{
//_itoa_s(testitem,nitex,10);
	SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA3,  // 发送自己的窗口消息   
           (WPARAM)&nitex,     // 设置发送的内容   
           NULL);
//}
	if((pass==0)&&(stoponfail==1))
 {
break;
 }


}
if(testtype==2)
{
_itoa_s(testitem+1,bufxx,10);
    strcat_s(bufxx2,bufxx);
	//AfxMessageBox(CString(bufxx1));
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&bufxx2,     // 设置发送的内容   
           NULL);
sp3=sp2;
sp2=sp2+1;
freqrecord[testitem]=actfreq[testitem];
VinRamp.Freq=actfreq[testitem];
for(j1=0;j1<testno;j1++)
{
	VinRamp.Curr[j1]=curr[j1];
	sprintf_s(buf2," %.3f",curr[j1]);
	strcat_s(bufload,buf2);

}
VinRamp.R1A=vrelay;
VinRamp.R1B=vrelay2;
VinRamp.Relaydelay=vrelaychangetime;
VinRamp.Step_Time=stept;
VinRamp.Step_Volt=stepv;
VinRamp.Trig_L=trigcl;
VinRamp.Trig_Voff=vofftrigv;
VinRamp.Trig_Von=vontrigv;
VinRamp.Vin_off_End=voffend;
VinRamp.Vin_off_MAX=voffmax;
VinRamp.Vin_off_MIN=voffmin;
VinRamp.Vin_off_Start=voffstart;
VinRamp.Vin_on_End=vonend;
VinRamp.Vin_on_MAX=vonmax;
VinRamp.Vin_on_MIN=vonmin;
VinRamp.Vin_on_Start=vonstart;
pass=Run_Vin_Ramp(VinRamp);
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&bufload,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
sprintf_s(buf2,"VonMax: %.3f",vonmax);
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&buf2,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
sprintf_s(buf2,"VonMin: %.3f",vonmin);
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&buf2,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
sprintf_s(buf2,"VonAct: %.3f",vrampon);
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&buf2,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
sprintf_s(buf2,"VoffMax: %.3f",voffmax);
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&buf2,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
sprintf_s(buf2,"VoffMin: %.3f",voffmin);
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&buf2,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
sprintf_s(buf2,"VoffAct: %.3f",vrampoff);
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&buf2,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
strcpy_s(bufxx,"");	
strcpy_s(bufxx2,"Radom Test----VinRamp");	
strcpy_s(bufload,"Load:");	
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA3,  // 发送自己的窗口消息   
           (WPARAM)&nitex,     // 设置发送的内容   
           NULL);
//}
	if((pass==0)&&(stoponfail==1))
 {
break;
 }


}




}

/*for(j1=0;j1<50;j1++)
{
for(j=0;j<testno;j++)
{
debugdaizais.Format(_T("%.3f"),currecord[j1][j]);
AfxMessageBox(debugdaizais);
}
}
*/
//if(pass==0)
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA4,  // 发送自己的窗口消息   
           NULL,     // 设置发送的内容   
           NULL);




lineset(0,47);
for(iend=1;iend<=testno;iend++)
  { Curr_Set(0,iend,0);}

  //Outportb(0x343,0x80);
 Outportb(port_add,0x00);
delete []curr;	
for (int i=0;i<changenumber;i++) 
{
delete[] actload[i]; //先撤销指针元素所指向的数组
}                      
delete[] actload; //（别人的一个例子
delete[] actvolt;
delete[] actfreq;
}






if(signal==1)
{
	sp2=0;
	_itoa_s(selmask,nitex,10);
	//AfxMessageBox(CString(nitex));
	SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA2,  // 发送自己的窗口消息   
           (WPARAM)&nitex,     // 设置发送的内容   
           NULL);
	
	reg.Vin=voltrecord[selmask];
reg.Delay=delaytime;
reg.Freq=freqrecord[selmask];
reg.R1A=relay;
for(j1=0;j1<testno;j1++)
{
	reg.Curr[j1]=currecord[selmask][j1];
	sprintf_s(buf2," %.3f",currecord[selmask][j1]);
	strcat_s(bufload,buf2);
	//debugdaizais.Format(_T("%.3f"),curr[j1]);
//AfxMessageBox(debugdaizais);
}
for(j1=0;j1<testno;j1++)
{
	reg.V_H[j1]=specmax[j1];
	sprintf_s(buf2," %.3f",specmax[j1]);
	strcat_s(bufmax,buf2);
	//debugdaizais.Format(_T("%.3f"),curr[j1]);
//AfxMessageBox(debugdaizais);
}
for(j1=0;j1<testno;j1++)
{
	reg.V_L[j1]=specmin[j1];
	sprintf_s(buf2," %.3f",specmin[j1]);
	strcat_s(bufmin,buf2);
//debugdaizais.Format(_T("%.3f"),curr[j1]);
//AfxMessageBox(debugdaizais);
}
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&bufxx1,     // 设置发送的内容   
           NULL);
sp3=sp2;
sp2=sp2+1;

sprintf_s(bufv,"%dV/",actvolt[selmask]);
sprintf_s(buff,"%dHz",actfreq[selmask]);
strcat_s(bufv,buff);
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&bufv,     // 设置发送的内容   
           NULL);
sp2=sp2+1;

SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&bufload,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&bufmax,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&bufmin,     // 设置发送的内容   
           NULL);
sp2=sp2+1; 
pass=Run_Reg(reg);
for(j1=0;j1<testno;j1++)
{
	//reg.V_L[j1]=specmin[j1];
	sprintf_s(buf2," %.3f",OV[j1]);
	strcat_s(bufmean,buf2);
//debugdaizais.Format(_T("%.3f"),curr[j1]);
//AfxMessageBox(debugdaizais);
}
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&bufmean,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
sprintf_s(buf2,"Mean Vin: %.3f Iin: %.3f Pin: %.3f Pout: %.3f Eff: %.4f PF: %.3f Vthd: %.3f",vin,iin,pin,pout,eff,pf,vthd);
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&buf2,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
sprintf_s(buf2,"ITHD SPEC: %.3f",ithdshow);
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&buf2,     // 设置发送的内容   
           NULL);
sp2=sp2+1;
sprintf_s(buf2,"ITHD MEAN: %.3f",ithd);
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA,  // 发送自己的窗口消息   
           (WPARAM)&buf2,     // 设置发送的内容   
           NULL);
sp2=sp2+1;


strcpy_s(bufxx,"");	
strcpy_s(bufxx1,"Radom Test----Inpout");	
strcpy_s(bufload,"Load:");	
  strcpy_s(bufmax,"V_Max :");	
  strcpy_s(bufmin,"V_Min :");
 strcpy_s(bufmean,"V_Mean:");
strcpy_s(bufv,"");
strcpy_s(buff,"");
// if(pass_item==0)
//{
//_itoa_s(testitem,nitex,10);
	SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA3,  // 发送自己的窗口消息   
           (WPARAM)&nitex,     // 设置发送的内容   
           NULL);
//}
SendMessage(hwnd, // 获得主窗口在句柄   
           WM_MYUPDATEDATA4,  // 发送自己的窗口消息   
           NULL,     // 设置发送的内容   
           NULL);












}

}

void Curr_Set(float Curr,int Chan,float Volt)
 {
	 char buf2[256];
  int i;
  int n;
  int m;
  char buf[256];
  char buf1[256];
  char L_AD[256]="GPIB0::";
 char L_AD1[256]="::INSTR";

 if(h1==1)
 {
  if(Chan==1)
{
  _itoa_s(loadadd[0],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);

     //  Gpwrite(L_AD, "LOAD OFF");
      Gpwrite(L_AD, "MODE CCH");
 
      sprintf_s(buf2,"CURR:STAT:L1 %f",Curr);
        Gpwrite(L_AD, buf2);
        Gpwrite(L_AD, "LOAD ON");




}
else
{
 Chan=Chan-1;
   if(Chan%4>0)
      {n=Chan/4+1;
       m=Chan%4;
       _itoa_s(loadadd[n],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
     sprintf_s(buf,"chan %d",(m-1)*2+1);
     Gpwrite(L_AD, buf);
    
 
     // Gpwrite(L_AD, "LOAD OFF");
      Gpwrite(L_AD, "MODE CCH");
    
      sprintf_s(buf2,"CURR:STAT:L1 %f",Curr);
       Gpwrite(L_AD, buf2);
      Gpwrite(L_AD, "LOAD ON");

      }
    else
      {
      n=Chan/4;
         _itoa_s(loadadd[n],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
     sprintf_s(buf,"chan 7");
     Gpwrite(L_AD, buf);
    
    

     //  Gpwrite(L_AD, "LOAD OFF");
      Gpwrite(L_AD, "MODE CCH");
 
      sprintf_s(buf2,"CURR:STAT:L1 %f",Curr);
       Gpwrite(L_AD, buf2);
       Gpwrite(L_AD, "LOAD ON");
                           }





}



 }








   if(h1!=1)
   {
 //  AfxMessageBox(_T("331"));
	   if(Chan%4>0)
      {
		  n=Chan/4;
       m=Chan%4;
       _itoa_s(loadadd[n],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
     sprintf_s(buf,"chan %d",(m-1)*2+1);
     Gpwrite(L_AD, buf);
     


     // Gpwrite(L_AD, "LOAD OFF");
      Gpwrite(L_AD, "MODE CCH");
  
      sprintf_s(buf2,"CURR:STAT:L1 %f",Curr);
       Gpwrite(L_AD, buf2);
       Gpwrite(L_AD, "LOAD ON");

      }
    else
      {
		  n=Chan/4;
         _itoa_s(loadadd[n-1],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
     sprintf_s(buf,"chan 7");
     Gpwrite(L_AD, buf);
     


     // Gpwrite(L_AD, "LOAD OFF");
      Gpwrite(L_AD, "MODE CCH");

      sprintf_s(buf2,"CURR:STAT:L1 %f",Curr);
       Gpwrite(L_AD, buf2);
    Gpwrite(L_AD, "LOAD ON");
}
 if(Curr*Volt>300)
 {
	// AfxMessageBox(_T("POWER OVER SPEC"),MB_OK|MB_ICONSTOP); 
 }

    }
 }

int Run_Reg(Reg sta)
{
int chan,m2,currno;
int i,currmatch,ic;
//float OP,IP,eff,IIN,VIN,pf;
float mcurr,ppercent;
char buf[256],buf1[256];
 char reading[256];
char POW1[256]="GPIB0::";
char POW3[256]="::INSTR";
char POW2[256];
char Agilent1[256]="GPIB0::";
 char Agilent3[256]="::INSTR";
 char Agilent2[256];
 char DC1[256]="GPIB0::";
 char DC3[256]="::INSTR";
 char DC2[256];
 char * userc1;
char * userc2;
char * userc3;
char *p2;
LPCTSTR pp1;
CString panduans,DLoads,Dmatchs,loadnumber1,currmatchs,currnos,Dnos,loadnumber2;
char bufload[256]="Load:";
	char bufv[256]="";
	char buff[256]="";
	char buf2[256]="";
DLoads="Load";	
Dmatchs="Match";
Dnos="No";	
_itoa_s(pmadd,POW2,10);
strcat_s(POW1,POW2);
  strcat_s(POW1,POW3);
lineset(sta.Vin,sta.Freq);


  //Outportb(0x343,0x80);

Outportb(port_add,sta.R1A);
 delay(sta.Relaydelay);
 Outportb(port_add,sta.R1B);
  //Outportb(IO_ADD,sta.R1B);
   if (sta.Pause==1)
    {
		AfxMessageBox(CString(sta.Message),MB_OK); 
   }
for(i=1;i<=testno;i++)
  { Curr_Set(sta.Curr[i-1],i,sta.V_H[i-1]);}
 delay(sta.Delay);
   if (sta.Pause==2)
    {
		 AfxMessageBox(CString(sta.Message),MB_OK); 
   }
 for(i=1;i<=testno;i++)
   {OV[i-1]=Meas_Chan_Volt(i);
      sprintf_s(buf,"%f",OV[i-1]);

      }

  for(i=1;i<=testno;i++)
  {
	  if((sta.V_L[i-1]>OV[i-1])||(sta.V_H[i-1]<OV[i-1]))
   {
	  pass=0;
     pass_item=0;
     //MessageBox(NULL,"ov","ERROR",MB_OK);
     }
    }
  pout=0;
  GetPrivateProfileString(_T("Info"),_T("Load_SenceMatch"),_T("0"),panduans.GetBuffer(256),256,strFilePath1);
panduans.ReleaseBuffer(256);
pp1=panduans.GetBuffer(0);
	panduans.ReleaseBuffer();
	userc1 = new char [panduans.GetLength()+1]; 
	strcpy_s(userc1,panduans.GetLength()+1, CT2CA(pp1));
	m2=0;
	
	if(findstr(userc1,"1")!=-1)
 {
	//AfxMessageBox(_T("33")); 
	 for(i=0;i<changenumber;i++)
 {
loadnumber1.Format(_T("%d"),i+1);
loadnumber1=DLoads+loadnumber1+Dmatchs;
//AfxMessageBox(loadnumber);
GetPrivateProfileString(_T("Info"),loadnumber1,_T("2"),currmatchs.GetBuffer(256),256,strFilePath1);
currmatchs.ReleaseBuffer(256);
pp1=currmatchs.GetBuffer(0);
	currmatchs.ReleaseBuffer();
	userc2 = new char [currmatchs.GetLength()+1]; 
	strcpy_s(userc2,currmatchs.GetLength()+1, CT2CA(pp1));
	//AfxMessageBox(CString(userc1));
	currmatch=atoi(userc2);
loadnumber2.Format(_T("%d"),i+1);
loadnumber2=DLoads+loadnumber2+Dnos;
//AfxMessageBox(loadnumber2);
GetPrivateProfileString(_T("Info"),loadnumber2,_T("2"),currnos.GetBuffer(256),256,strFilePath1);
currnos.ReleaseBuffer(256);
pp1=currnos.GetBuffer(0);
	currnos.ReleaseBuffer();
	userc3 = new char [currnos.GetLength()+1]; 
	strcpy_s(userc3,currnos.GetLength()+1, CT2CA(pp1));
	//AfxMessageBox(CString(userc3));
	currno=atoi(userc3);
	mcurr=0;
	for(ic=0;ic<currno;ic++)
{
mcurr=mcurr+Meas_Chan_Curr(m2+1);
m2=m2+1;
}
	
pout=pout+Meas_Chan_Volt(currmatch)*mcurr;


 }
  

	}
  
if(findstr(userc1,"1")==-1)  
{	


 for(i=1;i<=testno;i++)
 {
pout=pout+OV[i-1]*Meas_Chan_Curr(i);
 }	
	}
	
	
	if(dcadd==0)
 {
	 Gpread(POW1,"MEAS:VOLT:RMS?",buf);
      vin=atof(buf);
 Gpread(POW1,"MEAS:CURR:RMS?",buf);
 iin=atof(buf);
 Gpread(POW1,"MEAS:POW:REAL?",buf);
 pin=atof(buf);
 eff=pout/pin;
Gpread(POW1,"MEAS:VOLT:THD?",buf);	
vthd=atof(buf);	
 Gpread(POW1,"MEAS:POW:PFAC?",buf);
     pf=atof(buf);	
ppercent=pout/pmax;	
Gpread(POW1,"MEAS:CURR:THD?",buf);	
ithd=atof(buf);		
if(ppercent<ithdw[0])
{
 ithdshow=ithdspec[0];
	if(ithd>ithdspec[0])
 {
	 pass=0;
     pass_item=0;
 }
 }
for(i=0;i<ithdno-1;i++)	
{
if((ppercent>=ithdw[i])&&(ppercent<ithdw[i+1]))
{
ithdshow=ithdspec[i+1];
	if(ithd>ithdspec[i+1])
 {
	 pass=0;
     pass_item=0;
 }
}


}
	
if(ppercent>=ithdw[ithdno-1])	
{
ithdshow=ithdspec[ithdno];
	if(ithd>ithdspec[ithdno])
 {
	 pass=0;
     pass_item=0;
 }
}
	}
 if(dcadd!=0)
 {
	 if(dcefftype==1)
	 {
_itoa_s(agilent_add,Agilent2,10);
strcat_s(Agilent1,Agilent2);
 strcat_s(Agilent1,Agilent3);
_itoa_s(dcadd,DC2,10);
strcat_s(DC1,DC2);
strcat_s(DC1,DC3);
Gpread(DC1,"MEAS:VOLT?",buf);
   vin=atof(buf);	 
 Gpread(Agilent1,"MEAS:VOLT:DC?",reading);
  iin=atof(reading);
  iin=fabs((iin*shunti*1000)/shuntv);	 
pin=fabs(vin*iin);
  eff=pout/pin;
	 }
if(dcefftype==2)
{
_itoa_s(dcadd,DC2,10);
strcat_s(DC1,DC2);
strcat_s(DC1,DC3);
Gpread(DC1,"MEAS:VOLT?",buf);
   vin=atof(buf);	 
 Gpread(POW1,"MEAS:CURR:RMS?",reading);
  iin=atof(reading);
pin=fabs(vin*iin);
  eff=pout/pin;
}
 
 
 
 } 
  
 return(pass);
 }

int Run_Vin_Ramp(Vin_Ramp VRa)
{
int chan;
int i;
//float OP,IP,eff,IIN,VIN,pf;
char buf[256],buf1[256];
 char reading[256];
 char Agilent1[256]="GPIB0::";
 char Agilent3[256]="::INSTR";
 char Agilent2[256];
  char DC1[256]="GPIB0::";
 char DC3[256]="::INSTR";
 char DC2[256];
 char POW1[256]="GPIB0::";
char POW3[256]="::INSTR";
char POW2[256];
 float TV;
_itoa_s(pmadd,POW2,10);
strcat_s(POW1,POW2);
  strcat_s(POW1,POW3);
  lineset(VRa.Vin_on_Start,VRa.Freq);


  //Outportb(0x343,0x80);

Outportb(port_add,VRa.R1A);
 delay(VRa.Relaydelay);
 Outportb(port_add,VRa.R1B);
 for(i=1;i<=testno;i++)
  { Curr_Set(VRa.Curr[i-1],i,VRa.V_H[i-1]);}
if(dcadd==0)
{
 lineset(VRa.Vin_on_Start,VRa.Freq);

  TV=Meas_Chan_Volt(VRa.Trig_L);
  vrampon=VRa.Vin_on_Start;
 do
  {
  // Gpread(POW1,"MEAS:VOLT:RMS?",reading);
 //VRamp=atof(reading);
   vrampon=vrampon+VRa.Step_Volt;
   lineset(vrampon,VRa.Freq);
    delay(VRa.Step_Time);
   TV=Meas_Chan_Volt(VRa.Trig_L);
   
   }while((vrampon<VRa.Vin_on_End)&&(TV<VRa.Trig_Von));
     Gpread(POW1,"MEAS:VOLT:RMS?",reading);
 vrampon=atof(reading);
    if((VRa.Vin_on_MIN>vrampon)||(VRa.Vin_on_MAX<vrampon)||(TV<VRa.Trig_Von))
    {
    pass=0;
    pass_item=0;
    }
lineset(VRa.Vin_off_Start,VRa.Freq);
 TV=Meas_Chan_Volt(VRa.Trig_L);
  vrampoff=VRa.Vin_off_Start;
 do
  {
   // Gpread(POW1,"MEAS:VOLT:RMS?",reading);
 //VRamp=atof(reading);
   vrampoff=vrampoff-VRa.Step_Volt;
   lineset(vrampoff,VRa.Freq);
       delay(VRa.Step_Time);
   TV=Meas_Chan_Volt(VRa.Trig_L);

   }while((vrampoff>VRa.Vin_off_End)&&(TV>VRa.Trig_Voff));
    Gpread(POW1,"MEAS:VOLT:RMS?",reading);
 vrampoff=atof(reading);
    if((VRa.Vin_off_MIN>vrampoff)||(VRa.Vin_off_MAX<vrampoff)||(TV>VRa.Trig_Voff))
    {
    pass=0;
    pass_item=0;
    }


}

if(dcadd!=0)
{
_itoa_s(dcadd,DC2,10);
strcat_s(DC1,DC2);
strcat_s(DC1,DC3);
lineset(VRa.Vin_on_Start,VRa.Freq);

  TV=Meas_Chan_Volt(VRa.Trig_L);
  vrampon=VRa.Vin_on_Start;
 do
  {
  // Gpread(POW1,"MEAS:VOLT:RMS?",reading);
 //VRamp=atof(reading);
   vrampon=vrampon+VRa.Step_Volt;
   lineset(vrampon,VRa.Freq);
    delay(VRa.Step_Time);
   TV=Meas_Chan_Volt(VRa.Trig_L);
   
   }while((vrampon<VRa.Vin_on_End)&&(TV<VRa.Trig_Von));
    Gpread(DC1,"MEAS:VOLT?",reading);
 vrampon=atof(reading);
    if((VRa.Vin_on_MIN>vrampon)||(VRa.Vin_on_MAX<vrampon)||(TV<VRa.Trig_Von))
    {
    pass=0;
    pass_item=0;
    }
lineset(VRa.Vin_off_Start,VRa.Freq);
 TV=Meas_Chan_Volt(VRa.Trig_L);
  vrampoff=VRa.Vin_off_Start;
 do
  {
   // Gpread(POW1,"MEAS:VOLT:RMS?",reading);
 //VRamp=atof(reading);
   vrampoff=vrampoff-VRa.Step_Volt;
   lineset(vrampoff,VRa.Freq);
       delay(VRa.Step_Time);
   TV=Meas_Chan_Volt(VRa.Trig_L);

   }while((vrampoff>VRa.Vin_off_End)&&(TV>VRa.Trig_Voff));
  Gpread(DC1,"MEAS:VOLT?",reading);
 vrampoff=atof(reading);
    if((VRa.Vin_off_MIN>vrampoff)||(VRa.Vin_off_MAX<vrampoff)||(TV>VRa.Trig_Voff))
    {
    pass=0;
    pass_item=0;
    }





}


 return(pass);
}


 
 
 
 
 
 
 
 
float Meas_Chan_Volt(int Chan)
{char buf2[256];
  int i;
  int n;
  int m;
  char buf[256];
  char buf1[256];
  char L_AD[256]="GPIB0::";
 char L_AD1[256]="::INSTR";
 char read[256];
 float Volt;

 if(h1==1)
 {
  if(Chan==1)
 {
  _itoa_s(loadadd[0],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
  Gpwrite(L_AD, "MEAS:INP UUT");
    Gpread(L_AD,"MEAS:VOLT?",read);
    Volt=atof(read);
 }
 else
{
  Chan=Chan-1;
    if(Chan%4>0)
      {
	   n=Chan/4+1;
       m=Chan%4;
       _itoa_s(loadadd[n],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
     sprintf_s(buf,"chan %d",(m-1)*2+1);
     Gpwrite(L_AD, buf);
     Gpwrite(L_AD, "MEAS:INP UUT");
    Gpread(L_AD,"MEAS:VOLT?",read);
    Volt=atof(read);
                     }
    else
      {
		  n=Chan/4;
         _itoa_s(loadadd[n],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
     sprintf_s(buf,"chan 7");
     Gpwrite(L_AD, buf);
      Gpwrite(L_AD, "MEAS:INP UUT");
    Gpread(L_AD,"MEAS:VOLT?",read);
    Volt=atof(read);}




    }






 }






  if(h1!=1)
  {

   if(Chan%4>0)
      {
		  n=Chan/4;
       m=Chan%4;
       _itoa_s(loadadd[n],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
     sprintf_s(buf,"chan %d",(m-1)*2+1);
     Gpwrite(L_AD, buf);
     Gpwrite(L_AD, "MEAS:INP UUT");
    Gpread(L_AD,"MEAS:VOLT?",read);
    Volt=atof(read);
                     }
    else
      {
		  n=Chan/4;
         _itoa_s(loadadd[n-1],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
     sprintf_s(buf,"chan 7");
     Gpwrite(L_AD, buf);
      Gpwrite(L_AD, "MEAS:INP UUT");
    Gpread(L_AD,"MEAS:VOLT?",read);
    Volt=atof(read);}
                    }
  return(Volt);
  
  }

float Meas_Chan_Curr(int Chan)
{
 float Curr;
char buf2[256];
  int i;
  int n;
  int m;
  char buf[256];
  char buf1[256];
  char L_AD[256]="GPIB0::";
 char L_AD1[256]="::INSTR";
 char read[256];

 if(h1==1)
 {
  if(Chan==1)
 {
  _itoa_s(loadadd[0],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
  //Gpwrite(L_AD, "MEAS:INP UUT");
     Gpread(L_AD,"MEAS:CURR?",read);
    Curr=atof(read);
 }
 else
{
  Chan=Chan-1;
    if(Chan%4>0)
      {
	   n=Chan/4+1;
       m=Chan%4;
       _itoa_s(loadadd[n],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
     sprintf_s(buf,"chan %d",(m-1)*2+1);
     Gpwrite(L_AD, buf);
     //Gpwrite(L_AD, "MEAS:INP UUT");
     Gpread(L_AD,"MEAS:CURR?",read);
    Curr=atof(read);
                     }
    else
      {
		  n=Chan/4;
         _itoa_s(loadadd[n],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
     sprintf_s(buf,"chan 7");
     Gpwrite(L_AD, buf);
      //Gpwrite(L_AD, "MEAS:INP UUT");
    Gpread(L_AD,"MEAS:CURR?",read);
    Curr=atof(read);}




    }






 }






  if(h1!=1)
  {

   if(Chan%4>0)
      {
		  n=Chan/4;
       m=Chan%4;
       _itoa_s(loadadd[n],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
     sprintf_s(buf,"chan %d",(m-1)*2+1);
     Gpwrite(L_AD, buf);
    // Gpwrite(L_AD, "MEAS:INP UUT");
   Gpread(L_AD,"MEAS:CURR?",read);
    Curr=atof(read);
                     }
    else
      {
		  n=Chan/4;
         _itoa_s(loadadd[n-1],buf1,10);
    strcat_s(L_AD,buf1);
    strcat_s(L_AD,L_AD1);
     sprintf_s(buf,"chan 7");
     Gpwrite(L_AD, buf);
     // Gpwrite(L_AD, "MEAS:INP UUT");
    Gpread(L_AD,"MEAS:CURR?",read);
    Curr=atof(read);}
                    }
  return(Curr);
  



}
  
LRESULT CRadomTestDlg::OnUpdateMyData2(WPARAM wParam, LPARAM lParam)
{
    char *str22= (char*)wParam;
	int titem;
	titem=atoi(str22);
if(titem==0)
{
	m_list1.EnsureVisible(titem,FALSE);//默认选中第一行
m_list1.SetItemState(titem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
m_list1.SetFocus();
}
if(titem>0)
{
m_list1.EnsureVisible(titem,FALSE);//默认选中第一行
m_list1.SetItemState(titem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
m_list1.SetItemState(titem-1,0,LVIS_SELECTED|LVIS_FOCUSED);
m_list1.SetFocus();

}
return 0; 
}

LRESULT CRadomTestDlg::OnUpdateMyData(WPARAM wParam, LPARAM lParam)
{
int sp;
int count=0;
CString* strEdit = (CString*)wParam;  
char *str22= (char*)wParam;
//sp=m_ListBox1.GetCurSel();
//m_ListBox1.InsertString(sp,(LPCTSTR)strEdit);  
int num = MultiByteToWideChar(0,0,str22,-1,NULL,0);
wchar_t *wide = new wchar_t[num];
MultiByteToWideChar(0,0,str22,-1,wide,num);
count=m_ListBox1.GetCount();
m_ListBox1.SetCurSel(count-2);
m_ListBox1.InsertString(sp2,wide);  


	
	
	
	
	
	
	
	
	
return 0; 
}

void CRadomTestDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
	if (BST_CHECKED==IsDlgButtonChecked( IDC_CHECK1 ) )
{
stoponfail=1;
}
	else
	{
stoponfail=0;
	}
}
LRESULT CRadomTestDlg::OnUpdateMyData3(WPARAM wParam, LPARAM lParam)
{
 char *str22= (char*)wParam;
	int titem;
	CString neirong,neirong1;
	titem=atoi(str22);
	if(pass_item==0)
	{
		m_ListBox1.GetText(sp3,neirong);
		neirong1="--------FAIL";
		neirong=neirong+neirong1;
		m_ListBox1.DeleteString(sp3);
		//m_ListBox1.SetItemText(sp3,neirong);
		m_ListBox1.InsertString(sp3,neirong);
		m_list1.SetItemText(titem,2,_T("Fail"));
	}
if(pass_item==1)
	{
		m_ListBox1.GetText(sp3,neirong);
		neirong1="--------PASS";
		neirong=neirong+neirong1;
		m_ListBox1.DeleteString(sp3);
		m_ListBox1.InsertString(sp3,neirong);
		m_list1.SetItemText(titem,2,_T("Pass"));
	}
	//GetDlgItem(IDC_Test)->EnableWindow(TRUE);

pass_item=1;


return 0; 

}

void CRadomTestDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	int iend;
	lineset(0,47);
for(iend=1;iend<=testno;iend++)
  { Curr_Set(0,iend,0);}

  //Outportb(0x343,0x80);
 Outportb(port_add,0x00);
	
	
	CDialogEx::OnCancel();
}


void CRadomTestDlg::OnBnClickedStop()
{
	// TODO: Add your control notification handler code here
	// KillTimer(1);
	int iend;	
	DWORD code; 
GetExitCodeThread(hThread,&code); 
if(code == STILL_ACTIVE) 
     {   
TerminateThread(hThread,0); 
                       } 	

lineset(0,47);
for(iend=1;iend<=testno;iend++)
  { Curr_Set(0,iend,0);}

  //Outportb(0x343,0x80);
 Outportb(port_add,0x00);
GetDlgItem(IDC_STATUE)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATUE)->SetWindowText(_T("Stop"));
	GetDlgItem(IDC_STATUE)->ShowWindow(TRUE);

GetDlgItem(IDC_Test)->EnableWindow(TRUE);
}

LRESULT CRadomTestDlg::OnUpdateMyData4(WPARAM wParam, LPARAM lParam)
{
SYSTEMTIME DateTime;
int ix,yx;
float ct;
CString file1,file2,file3,str,huanhang,file4;	
char data[256],month[256],day[256],hour[256],sec[256],min[256],times[256],timelabel[256];
file1="\\";
file4="_";
GetLocalTime(&DateTime);
    sprintf_s(data,"%d",DateTime.wYear);
  
	if (DateTime.wMonth<10)
   {
	sprintf_s(month,"0%d",DateTime.wMonth);
    }
   else
   {
	  sprintf_s(month,"%d",DateTime.wMonth);
   }
   
  if (DateTime.wDay<10)
   {
	sprintf_s(day,"0%d",DateTime.wDay);
    }
   else
   {
	  sprintf_s(day,"%d",DateTime.wDay);
   }
  
   if (DateTime.wHour<10)
  {
sprintf_s(hour,"0%d",DateTime.wHour);
}
  else
   {
	  sprintf_s(hour,"%d",DateTime.wHour);
   } 
  
   if (DateTime.wMinute<10)
  {
sprintf_s(min,"0%d",DateTime.wMinute);
}
  else
   {
	  sprintf_s(min,"%d",DateTime.wMinute);
   } 
  
 if (DateTime.wSecond<10)
  {
sprintf_s(sec,"0%d",DateTime.wSecond);
}
  else
   {
	  sprintf_s(sec,"%d",DateTime.wSecond);
   }  
  
  strcat_s(data,month);
   strcat_s(data,day);
  
   filename=path3+file1+data+file1;
 strcat_s(data,hour);
   strcat_s(data,min);
   strcat_s(data,sec);
	file2="_PASS.txt";
  file3="_FAIL.txt";
	if(pass==0)
	{
		filename=filename+number1+file4+data+file3;
	GetDlgItem(IDC_STATUE)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATUE)->SetWindowText(_T("FAIL"));
	GetDlgItem(IDC_STATUE)->ShowWindow(TRUE);
	}
if(pass==1)
	{
		filename=filename+number1+file4+data+file2;
	GetDlgItem(IDC_STATUE)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATUE)->SetWindowText(_T("PASS"));
	GetDlgItem(IDC_STATUE)->ShowWindow(TRUE);
}
if(signal==0)
{
	huanhang="\r\n";	
CStdioFile file(filename,CFile::modeCreate|CFile::modeWrite); 
ix=m_ListBox1.GetCount();
for(yx=0;yx<ix;yx++)
 {
m_ListBox1.GetText(yx,str);
//AfxMessageBox(str,MB_OK|MB_ICONSTOP); 
str=str+huanhang;
//AfxMessageBox(str,MB_OK|MB_ICONSTOP); 
//file.Write(str.GetBuffer(0),str.GetLength());
file.WriteString(str);
}
 file.Close();	
}
GetDlgItem(IDC_Test)->EnableWindow(TRUE);
t12=GetTickCount();
//_itoa_s(t12-t11,times,10);
//times.Format("time:%dms",t12-t11);//
ct=t12-t11;
sprintf_s(timelabel,"CostTime:%.3fS",ct/1000);

int num = MultiByteToWideChar(0,0,timelabel,-1,NULL,0);
wchar_t *wide = new wchar_t[num];
MultiByteToWideChar(0,0,timelabel,-1,wide,num);

GetDlgItem(IDC_Time)->ShowWindow(FALSE);
	GetDlgItem(IDC_Time)->SetWindowText(wide);
	GetDlgItem(IDC_Time)->ShowWindow(TRUE);

return 0; 

}

LRESULT CRadomTestDlg::OnUpdateMyData5(WPARAM wParam, LPARAM lParam)
{
 char *str22= (char*)wParam;
char *userc1;	
LPCTSTR pp1; 
int titem;
	titem=atoi(str22);
CString type;
type=m_list1.GetItemText(titem,1);
//AfxMessageBox(type);
pp1=type.GetBuffer(0);
	type.ReleaseBuffer();
	userc1 = new char [type.GetLength()+1]; 
	strcpy_s(userc1,type.GetLength()+1, CT2CA(pp1));
	if(findstr(userc1,"Inpout")!=-1)
	{
testtype=1;
	}
if(findstr(userc1,"VinRamp")!=-1)
	{
testtype=2;
	}
	return 0;
}

void CRadomTestDlg::OnBnClickedSignal()
{
	// TODO: Add your control notification handler code here
	char ss[256];
int nHeadNum; 
	nHeadNum=m_list1.GetItemCount();
	signal=1;
	selmask=m_list1.GetSelectionMark();
_itoa_s(selmask,ss,10);
//AfxMessageBox(CString(ss));
GetDlgItem(IDC_STATUE)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATUE)->SetWindowText(_T("Testing"));
	GetDlgItem(IDC_STATUE)->ShowWindow(TRUE);
int ip;
	//GetDlgItem(IDC_Test)->EnableWindow(FALSE);	
	m_ListBox1.ResetContent();
	for(ip=0;ip<nHeadNum;ip++)
	{
m_list1.SetItemText(ip,2,_T(""));
m_list1.SetItemState(ip,0,LVIS_SELECTED|LVIS_FOCUSED);	
	}

	hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadFunc,NULL,0,&ThreadID);



}


BOOL CRadomTestDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
GetClientRect(&rect);   
CDC dcMem;   
//CDC * pdc = GetDC() ;
dcMem.CreateCompatibleDC(pDC);   
CBitmap bmpBackground;   
bmpBackground.LoadBitmap(IDB_BITMAP1);   
BITMAP bitmap;		
bmpBackground.GetBitmap(&bitmap);   
CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);   
pDC->StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,   
bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);	
	
	
	
	return TRUE;
	
	
	//return CDialogEx::OnEraseBkgnd(pDC);
}


HBRUSH CRadomTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  Return a different brush if the default is not desired
CString tr;	
	
if (pWnd->GetDlgCtrlID()==IDC_CHECK1) 
	{
	
pDC->SetBkMode(TRANSPARENT);//

pDC->SetTextColor(RGB(255,255,0));	
	
return (HBRUSH)::GetStockObject(NULL_BRUSH);

	}
if (pWnd->GetDlgCtrlID()==IDC_STATUE) 
	{
	
pDC->SetBkMode(TRANSPARENT);//

GetDlgItemText(IDC_STATUE,tr);
if(tr=="FAIL")
{
	pDC->SetTextColor(RGB(255,0,0));	
}
 if(tr=="PASS")
		{
			pDC->SetTextColor(RGB(0,255,0));
		}

		 

	/*if(tr=="Shop_FAIL")
		{
			pDC->SetTextColor(RGB(255,0,0));
		}	
	if(tr=="Debug")
		{
			pDC->SetTextColor(RGB(0,0,255));
		}*/		
	if(tr=="Testing")
		{
			pDC->SetTextColor(RGB(255,255,255));
		}			
		
	if(tr=="Stop")
		{
			pDC->SetTextColor(RGB(255,0,255));
		}				
	if(tr=="Ready")
		{
			pDC->SetTextColor(RGB(255,255,255));
		}		
	
return (HBRUSH)::GetStockObject(NULL_BRUSH);

	}	
if (pWnd->GetDlgCtrlID()==IDC_Model) 
	{
	
pDC->SetBkMode(TRANSPARENT);//

pDC->SetTextColor(RGB(255,255,0));	
	
return (HBRUSH)::GetStockObject(NULL_BRUSH);

	}	
	
if (pWnd->GetDlgCtrlID()==IDC_Time) 
	{
	
pDC->SetBkMode(TRANSPARENT);//

pDC->SetTextColor(RGB(255,255,255));	
	
return (HBRUSH)::GetStockObject(NULL_BRUSH);

	}		
	



return hbr;
}


void CRadomTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
//CTime mntime = CTime::GetCurrentTime(); 
char times[256];
int time,m,s;

switch(nIDEvent)
{
case 1:
 time =GetCurrentTime();
 m=time/60000; 
time %=60000; 
 s=time/1000; 
time %=1000; 
sprintf_s(times,"%02d:%02d.:%d",m,s,time/100);
//m_time.Format( "%02d:%02d.:%d ", m,s,time/100); 
int num = MultiByteToWideChar(0,0,times,-1,NULL,0);
wchar_t *wide = new wchar_t[num];
MultiByteToWideChar(0,0,times,-1,wide,num);

GetDlgItem(IDC_Time)->ShowWindow(FALSE);
	GetDlgItem(IDC_Time)->SetWindowText(wide);
	GetDlgItem(IDC_Time)->ShowWindow(TRUE);
break;
}



	
	
	CDialogEx::OnTimer(nIDEvent);
}


void CAboutDlg::OnTesttypeInpout()
{
	// TODO: Add your command handler code here



}


void CRadomTestDlg::OnTesttypeInpout()
{
	// TODO: Add your command handler code here
	//AfxMessageBox(_T("test"));
UINT flag;	
CMenu* mmenu = GetMenu();
CMenu* submenu = mmenu->GetSubMenu(1);	
int nHeadNum,m50; 
char bufx[256];
char bufx1[256]="Radom Test----Inpout";
flag=GetMenu()->GetSubMenu(1)->GetMenuState(0,MF_CHECKED);
if(flag&MF_CHECKED) 
{
GetMenu()->GetSubMenu(1)->CheckMenuItem(0,MF_CHECKED|MF_BYPOSITION); 	
//GetMenu()->GetSubMenu(1)->CheckMenuItem(0,MF_UNCHECKED|MF_BYPOSITION);	
nHeadNum=m_list1.GetItemCount();
for(m50=nHeadNum;m50<50+nHeadNum;m50++)
{
m_list1.InsertItem(m50,_T(""));
_itoa_s(m50+1,bufx,10);
 strcat_s(bufx1,bufx);
m_list1.SetItemText(m50,0,CString(bufx));	
m_list1.SetItemText(m50,1,CString(bufx1));	
strcpy_s(bufx,"");	
strcpy_s(bufx1,"Radom Test----Inpout");	
}	




}	
submenu->EnableMenuItem(ID_TESTTYPE_INPOUT,MF_BYCOMMAND | MF_DISABLED | MF_GRAYED); 
}


void CAboutDlg::OnUpdateTesttypeInpout(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
//  pCmdUI->Enable(0);


}


void CAboutDlg::OnTesttypeVinramp()
{
	// TODO: Add your command handler code here
}


void CRadomTestDlg::OnTesttypeVinramp()
{
	// TODO: Add your command handler code here
UINT flag;	
CMenu* mmenu = GetMenu();
CMenu* submenu = mmenu->GetSubMenu(1);	
int nHeadNum,m50; 
char bufx[256];
char bufx1[256]="Radom Test----VinRamp";
flag=GetMenu()->GetSubMenu(1)->GetMenuState(1,MF_CHECKED);
if(flag&MF_CHECKED) 
{
GetMenu()->GetSubMenu(1)->CheckMenuItem(1,MF_CHECKED|MF_BYPOSITION); 	
//GetMenu()->GetSubMenu(1)->CheckMenuItem(0,MF_UNCHECKED|MF_BYPOSITION);	
nHeadNum=m_list1.GetItemCount();
for(m50=nHeadNum;m50<50+nHeadNum;m50++)
{
m_list1.InsertItem(m50,_T(""));
_itoa_s(m50+1,bufx,10);
 strcat_s(bufx1,bufx);
m_list1.SetItemText(m50,0,CString(bufx));	
m_list1.SetItemText(m50,1,CString(bufx1));	
strcpy_s(bufx,"");	
strcpy_s(bufx1,"Radom Test----VinRamp");	
}	




}	
submenu->EnableMenuItem(ID_TESTTYPE_VINRAMP,MF_BYCOMMAND | MF_DISABLED | MF_GRAYED); 


}

