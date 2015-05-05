// CSocketcliDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CSocketcli.h"
#include "CSocketcliDlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSocketcliDlg dialog

CCSocketcliDlg::CCSocketcliDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCSocketcliDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCSocketcliDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCSocketcliDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSocketcliDlg)
	DDX_Control(pDX, IDC_BUTTON3, m_disconnect);
	DDX_Control(pDX, IDC_BUTTON2, m_connect);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_EDIT4, m_edit4);
	DDX_Control(pDX, IDC_LIST2, m_user);
	DDX_Control(pDX, IDC_LIST3, m_room);
	DDX_Control(pDX, IDC_CREAT, m_buttoncreat);
	DDX_Control(pDX, IDC_IN, m_buttonin);
}

BEGIN_MESSAGE_MAP(CCSocketcliDlg, CDialog)
	//{{AFX_MSG_MAP(CCSocketcliDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnDisconnect)
	ON_BN_CLICKED(IDC_FRESH, OnFresh)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CREAT, &CCSocketcliDlg::OnClickedCreat)
	ON_BN_CLICKED(IDC_IN, &CCSocketcliDlg::OnClickedIn)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSocketcliDlg message handlers
void InitConsole()
{
    int nRet= 0;
    FILE* fp;
    AllocConsole();
    nRet= _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
    fp = _fdopen(nRet, "w");
    *stdout = *fp;
    setvbuf(stdout, NULL, _IONBF, 0);
}

BOOL CCSocketcliDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	int roomcount=0;
	clitag="01";
	m_edit.SetLimitText(99);
	m_list.InsertColumn(0,"消息");
	m_user.InsertColumn(0,"online");
	m_room.InsertColumn(0,"room list");
	m_user.SetColumnWidth(0,435);
	m_list.SetColumnWidth(0,435);
	m_room.SetColumnWidth(0,435);
	m_room.InsertItem(roomcount++,"大厅");
	m_button1.EnableWindow(FALSE);
	m_disconnect.EnableWindow(FALSE);
	//MessageBox("in the thread");
	m_edit2.SetWindowText("211.80.63.162");
	count=0;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCSocketcliDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCSocketcliDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCSocketcliDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCSocketcliDlg::OnOK() 
{
	
//	CDialog::OnOK();
}

//发送信息
void CCSocketcliDlg::OnButton1() 
{
    CCSocketcliDlg::Getlocalip();
    CCSocketcliDlg::GetTime();
	

	char buff[100]={0};
	CSize size; 
	size.cx=0;
	size.cy=30;
	//获得发送信息
	m_edit.GetWindowText(buff,99);

	CString a;
	a=buff[0];  
	for(int i=1;i<100;i++)
	{
    	a=a+buff[i];
	} //把buff中的值传给a

	m_edit.SetWindowText("");
	CString nickname="";
	if(a!=""){
		GetDlgItem(IDC_EDIT3)->GetWindowText(nickname); //获取昵称
		msg = nickname+" ("+strTime+"):";
		//m_list.InsertItem(count++,msg);
		send(clisock,"1"+clitag+msg,100,0);
		msg = "    "+a;
		//m_list.InsertItem(count++,msg);
		send(clisock,"1"+clitag+msg,100,0);
		//m_list.Scroll(size);
		UpdateData(FALSE);
	}
}


//线程
UINT thread(LPVOID v)
{
	char buff[100];
	char array[5][16]=
	{"192.168.186.71",
	 "192.168.186.72",
	 "192.168.186.73",
	 "192.168.186.74",
	 "192.168.186.75",
	};
	CSize size;
	size.cx=0;
	size.cy=30;
	int s=1,addcount=0;
	CCSocketcliDlg *dlg=(CCSocketcliDlg*) AfxGetApp()->GetMainWnd();
	dlg->m_connect.EnableWindow(FALSE);
	dlg->m_disconnect.EnableWindow(TRUE);
	while(connect(dlg->clisock,(sockaddr*)&(dlg->cli),sizeof(dlg->cli)) && dlg->ee!=0)//若连接失败&&想要连接时进入。
	{
		//dlg->MessageBox("come in");
		if (addcount==5)
			addcount=0;
		dlg->cli.sin_addr.s_addr=inet_addr(array[addcount++]);
	}
		if (dlg->ee==1)	dlg->m_list.InsertItem(dlg->count++,"连接成功");
		//========显示连接成功===========
		CString nickname="";
		dlg->GetDlgItem(IDC_EDIT3)->GetWindowText(nickname); //获取昵称
		dlg->msg="---- "+nickname+" come in the room";

		CString temp;
		temp="4"+(dlg->clitag+nickname);
		char te[100]={0};
		for(int i=0;temp[i];i++)te[i]=temp[i];
		//dlg->MessageBox(te);

		send(dlg->clisock,"4"+(dlg->clitag+nickname),100,0);//用于在对话框中通知所有用户
		
		dlg->msg=nickname;
		int usercount=0;
		//dlg->m_user.InsertItem(usercount++,dlg->msg);
		send(dlg->clisock,"2"+dlg->clitag+nickname,100,0);//用于处理useronline in
  		dlg->m_button1.EnableWindow(TRUE);	
        dlg->m_edit.SetWindowText("");
	    //dlg->SetForegroundWindow();

	//循环获得数据
	while(s!=SOCKET_ERROR && dlg->ee!=0)
	{
		//调用recv函数接收数据
		s=recv(dlg->clisock,buff,100,0);
		//dlg->MessageBox("第一次信息");
		//dlg->MessageBox(buff);
	    //dlg->SetForegroundWindow();
		//从服务端获得在线列表
		if(buff[0]=='5'){
			//dlg->MessageBox(buff);
			for(int i=dlg->m_user.GetItemCount()-1; i>=0;i--)
					dlg->m_user.DeleteItem(i);
			//dlg->MessageBox("清除完毕");
			//dlg->MessageBox(buff);
			int usercount=0;
			while(buff[0]=='5'&&s!=SOCKET_ERROR){
				char temp[100]={0};
				for(int j=3;j<strlen(buff);j++) 
						temp[j-3]=buff[j];
				if(buff[1]==dlg->clitag[0]&&buff[2]==dlg->clitag[1])
					dlg->m_user.InsertItem(usercount++,temp);
				//dlg->MessageBox(buff);
				//dlg->m_user.Scroll(size);
				s=recv(dlg->clisock,buff,100,0);
				if(buff[0]=='b'||buff[0]=='f')break;
			}
		}
		if(buff[0]=='8'){
			//dlg->MessageBox(buff);
			for(int i=dlg->m_room.GetItemCount()-1; i>=0;i--)
					dlg->m_room.DeleteItem(i);
			int roomcount=0;
			while(buff[0]=='8'&&s!=SOCKET_ERROR){
				char temp[100]={0};
				for(int j=3;j<strlen(buff);j++) 
						temp[j-3]=buff[j];
				dlg->m_room.InsertItem(roomcount++,temp);
				//dlg->MessageBox(buff);
				//dlg->m_user.Scroll(size);
				s=recv(dlg->clisock,buff,100,0);
				if(buff[0]=='b'||buff[0]=='f')break;
			}
		}
		if(buff[0]=='c'){
			char temp[100]={0};
			for(int j=1;j<strlen(buff);j++) 
				temp[j-1]=buff[j];
			dlg->clitag=temp;
			//dlg->MessageBox("换TAG");
			//dlg->MessageBox(dlg->clitag);
			//更改之后应当重新显示进入
			dlg->GetDlgItem(IDC_EDIT3)->GetWindowText(nickname); //获取昵称
			dlg->msg="---- "+nickname+" come in the room";
			send(dlg->clisock,"4"+(dlg->clitag+nickname),100,0);//用于在对话框中通知所有用户
			continue;
		}
		if(buff[0]=='f'){
			dlg->msg="5"+dlg->clitag+nickname;
			send(dlg->clisock,dlg->msg,100,0);
			dlg->MessageBox("发送刷新信息");
			dlg->MessageBox(dlg->msg);
			continue;
		}
		if(buff[0]!='5'&&buff[0]!='8'&&buff[0]!='b'){
			char temp[100]={0};
			for(int j=3;j<strlen(buff);j++) 
				temp[j-3]=buff[j];
			//&&buff[1]==dlg->clitag[0]&&buff[2]==dlg->clitag[1]
			if (s!=SOCKET_ERROR && dlg->ee!=0&&buff[1]==dlg->clitag[0]&&buff[2]==dlg->clitag[1])	
				dlg->m_list.InsertItem(dlg->count++,temp);
			//dlg->MessageBox(buff);
		}
		dlg->m_list.Scroll(size);
		
	}
	//发送断开命令
	//send(dlg->clisock,"001Disconnected",100,0);
	dlg->m_button1.EnableWindow(FALSE);
	dlg->m_connect.EnableWindow(TRUE);
	dlg->m_disconnect.EnableWindow(FALSE);
	closesocket(dlg->clisock);
	AfxEndThread(0);
	return 0;
}
CCSocketcliDlg::~CCSocketcliDlg()
{
	send(clisock,"0"+clitag+"Disconnected",100,0);
}

//连接服务器
void CCSocketcliDlg::OnButton2() 
{
	char ipaddress[35];
	m_edit2.GetWindowText(ipaddress,30);
	cli.sin_addr.s_addr=inet_addr(ipaddress);

	cli.sin_family=AF_INET;
	cli.sin_port=htons(6666);//htons(5000);
	//创建socket
	clisock=socket(AF_INET,SOCK_STREAM,0);	
	ee=1;
	printf("in the thread");
	//启动线程
	AfxBeginThread(thread,0);


   
}

void CCSocketcliDlg::OnDisconnect() 
{
	//send(clisock,"301"+msg,100,0);//用于处理useronline断开
	//ee=0;
	closesocket(clisock);
}


void CCSocketcliDlg::Getlocalip()
{
   char szHostName[128];
   if (gethostname(szHostName,128)==0)
   {
    struct hostent * pHost;
    pHost = gethostbyname(szHostName);
 	
	str=inet_ntoa(*(struct in_addr*)pHost->h_addr_list[0]);
   }
}

void CCSocketcliDlg::GetTime()
{
    //获取系统时间
    CTime tm;
    tm=CTime::GetCurrentTime();
    strTime=tm.Format("%X");//现在时间是%Y年%m月%d日
}

void CCSocketcliDlg::OnFresh() 
{
	// TODO: Add your control notification handler code here
	 m_list.DeleteAllItems();
}

/*
void CCSocketcliDlg::InitConsoleWindow(void)
{
	int nCrt = 0;
    FILE* fp;
    AllocConsole();
    nCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
    fp = _fdopen(nCrt, "w");
    *stdout = *fp;
    setvbuf(stdout, NULL, _IONBF, 0);
}*/


void CCSocketcliDlg::OnClickedCreat()
{
	// TODO: 在此添加控件通知处理程序代码
	char buff[100]={0};
	CString roomname;
	GetDlgItem(IDC_EDIT4)->GetWindowText(roomname); //获取昵称
	//m_list.InsertItem(count++,msg);
	//MessageBox("9"+clitag+roomname);
	send(clisock,"9"+clitag+roomname,100,0);
	//recv(clisock,buff,100,0);
	//MessageBox("接受过了");
	//MessageBox(buff);
	//MessageBox("CREAT还没完啊");
	//clitag=buff;
	OnFresh();   //刷新对话跨
}


void CCSocketcliDlg::OnClickedIn()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str; char buff[100]={0};
	char roomid[10]={0};
	int line;//行号 
	CString nickname;
	//首先得到点击的位置 
	POSITION pos=m_room.GetFirstSelectedItemPosition(); 
	if(pos==NULL) return; 
	//得到行号，通过POSITION转化 
	line=(int)m_room.GetNextSelectedItem(pos); 
	itoa(line,roomid,10);
	for(int i=strlen(roomid);i>=1;i--) roomid[i]=roomid[i-1];
	roomid[0]='a';
	//MessageBox("IN有效行数");
	//MessageBox(roomid);
	send(clisock,roomid,100,0);//通过逐个确认行号获得roomtag
	//recv(clisock,buff,100,0);//get roomtag
	//clitag=buff;
	OnFresh();   //刷新对话跨
}
