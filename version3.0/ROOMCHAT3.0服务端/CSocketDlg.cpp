// CSocketDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CSocket.h"
#include "CSocketDlg.h"
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
// CCSocketDlg dialog

CCSocketDlg::CCSocketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCSocketDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCSocketDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCSocketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSocketDlg)
	DDX_Control(pDX, IDC_BUTTON1, m_button);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST2, m_user);
}

BEGIN_MESSAGE_MAP(CCSocketDlg, CDialog)
	//{{AFX_MSG_MAP(CCSocketDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_FRESH, OnFresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSocketDlg message handlers


//初始化对话框
BOOL CCSocketDlg::OnInitDialog()
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

	count=0;
	countuser=0;
	m_list.InsertColumn(0,"消息");
	m_list.SetColumnWidth(0,435);
	m_edit.SetLimitText(99);
	m_user.InsertColumn(0,"USER");
	m_user.SetColumnWidth(0,435);
	//m_edit.SetLimitText(99);
	for (int i=0;i<50;i++)
		msgsock[i]=NULL;
	char clinickname[50][100]={0};
	memset(roomshow,0,sizeof(roomshow));
	memset(onlinecount,0,sizeof(onlinecount));
	//char t[100]={0};
	//itoa(onlinecount[1],t,10);
						//MessageBox("大厅人数");
						//MessageBox(t);
	CString start="大厅";
	for(int i=0;i<strlen(start);i++) roomshow[1][i]=start[i];
	roomshow[1][strlen(start)]='\0';
	//MessageBox(roomshow[1]);
	//设定地址
	serv.sin_addr.s_addr=htonl(INADDR_ANY);//绑定
	serv.sin_family=AF_INET;
	serv.sin_port=htons(6666);//htons(5000);
	addlen=sizeof(serv);
	m_button.EnableWindow(FALSE);
	//创建socket
	sock=socket(AF_INET,SOCK_STREAM,0);
	//绑定
	if (bind(sock,(sockaddr*)&serv,addlen))
	{
		m_edit.SetWindowText("绑定错误");
	}else
	{
	m_list.InsertItem(count++,inet_ntoa(serv.sin_addr));
	m_edit.SetWindowText("服务器创建成功");
	//开始侦听	
	listen(sock,5);
	//调用线程
	AfxBeginThread(&thread,0);

	}
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCSocketDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCSocketDlg::OnPaint() 
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
HCURSOR CCSocketDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCSocketDlg::OnOK() 
{

	//	CDialog::OnOK();
}


//服务器线程
UINT thread(LPVOID p)
{
	char buff[100]={0};
	char temp[100]={0};
	CString clitag="01";
	CSize size;
	size.cx=0;
	size.cy=30;
	int i=0,j=0;
	int s=1,msgcount,loop=1,flag=0,s2=1,roomcount;
	CCSocketDlg *dlg=(CCSocketDlg*)AfxGetApp()->GetMainWnd();
	//获得客户端数量
	msgcount=dlg->getcount();
	if (msgcount==-1)
		loop=0;
	if(loop)
	{
		s=1;
		dlg->msgsock[msgcount]=accept(dlg->sock,(sockaddr*)&(dlg->serv),&(dlg->addlen));
		if (dlg->msgsock[msgcount]==INVALID_SOCKET)
		{
			dlg->m_edit.SetWindowText("Error accept");
		}
		else
		{
			//启动线程
			AfxBeginThread(thread,0);
			dlg->SetForegroundWindow();
			dlg->m_list.InsertItem(dlg->count++,"连接成功");
			dlg->m_list.InsertItem(dlg->count++,inet_ntoa(dlg->serv.sin_addr));
			dlg->m_list.Scroll(size);
			dlg->m_button.EnableWindow(TRUE);
			//====显示在room
			CString nickname,nickshow;

			while(s!=SOCKET_ERROR)
			{
				//循环接收数据
				s=recv(dlg->msgsock[msgcount],buff,100,0);
				//dlg->SetForegroundWindow();
				if (s!=SOCKET_ERROR)
				{
					//======正常信息通道
					if(buff[0]=='1'){
						//dlg->MessageBox("SEND THE FIREST LOGIN MESSAGE");
						dlg->sendtoall(buff);
						dlg->m_list.InsertItem(dlg->count++,buff);
						dlg->m_list.Scroll(size);
						
					}
					//======系统消息通道
					if(buff[0]=='0'){
					}
					//======ROOM USER IN 
					if(buff[0]=='2'){
						memset(temp,0,sizeof(temp));
						for(int j=3;j<strlen(buff);j++) 
							temp[j-3]=buff[j];//===========去掉标注信息
						for(int j=0;j<strlen(buff)-3;j++)
							dlg->clinickname[msgcount][j+1]=temp[j];
						dlg->clinickname[msgcount][0]=strlen(buff)-3;    //不知原因无法存储的很正常。于是把长度另储存。
						//将tag放在末端
						dlg->clinickname[msgcount][strlen(buff)-3+1]=clitag[0];
						dlg->clinickname[msgcount][strlen(buff)-3+2]=clitag[1];
						//=====room========================================
						for(int i=dlg->m_user.GetItemCount()-1; i>=0;i--)
								dlg->m_user.DeleteItem(i);
						dlg->countuser=0;
						for(int i=0;i<=49;i++){
							memset(temp,0,sizeof(temp));
							if(dlg->clinickname[i][0]>=1){
								for(int j=0;j<dlg->clinickname[i][0];j++)
									temp[j]=dlg->clinickname[i][j+1];
								dlg->m_user.InsertItem(dlg->countuser++,temp);
								dlg->m_user.Scroll(size);
								char gai[100]="5";
								char clitagtemp[10]={0};
								strcat(gai,clitag);
								gai[1]=dlg->clinickname[i][dlg->clinickname[i][0]+1];
								gai[2]=dlg->clinickname[i][dlg->clinickname[i][0]+2];
								strcat(gai,temp);
								dlg->sendtoall(gai);
								//dlg->MessageBox("发送跟新的代码");
								//dlg->MessageBox(gai);
							}
						}
						dlg->sendtoall("b");
						//更新大厅人数
						dlg->onlinecount[1]+=1;
						//char t[100]={0};
						//i//toa(dlg->onlinecount[1],t,10);
						//dlg->MessageBox("大厅人数");
						//dlg->MessageBox(t);
						//给新来的列表
						roomcount=0;
						//dlg->MessageBox("刷新ROOM列表");
						for(int i=1;i<=49;i++){
							if(dlg->onlinecount[i]>=1||i==1){
								memset(temp,0,sizeof(temp));
								for(int j=0;j<strlen(dlg->roomshow[i]);j++)
									temp[j]=dlg->roomshow[i][j];
								//itoa(i,t,10);
								//dlg->MessageBox(temp);
								//dlg->MessageBox(dlg->roomshow[i]);
								char gai[100]="8";
								strcat(gai,clitag);
								strcat(gai,temp);
								dlg->sendtoall(gai);
							}
						}
						dlg->sendtoall("b");
					}
					//======ROOM USER OUT
					if(buff[0]=='3'){
						//memset(,0,sizeof(dlg->clinickname[msgcount]));
						break;
						dlg->clinickname[msgcount][0]=0;
						//dlg->MessageBox("clear");
					}
					//======ROOM USER FIRST IN
					if(buff[0]=='4'){
						//显示在对话框
						char tagtemp[2]={0};
						memset(temp,0,sizeof(temp));
						for(int j=3;j<strlen(buff);j++) 
							temp[j-3]=buff[j];//===========去掉标注信息
						for(int i=1;i<=2;i++) tagtemp[i-1]=buff[i];
						tagtemp[2]='\0';
						//dlg->MessageBox(tagtemp);
						clitag=tagtemp;

						dlg->clinickname[msgcount][dlg->clinickname[msgcount][0]+1]=clitag[0];
						dlg->clinickname[msgcount][dlg->clinickname[msgcount][0]+2]=clitag[1];

						nickname=temp;
						nickshow="4"+clitag+"---- "+nickname+" come in the room";
						//dlg->MessageBox(nickshow);
						dlg->m_list.Scroll(size);
						for(int i=0;nickshow[i];i++) buff[i]=nickshow[i];
						int len=strlen(nickshow);buff[len]='\0';
						dlg->sendtoall(buff);
					}
					if(buff[0]=='9'){//创建一个房间&&刷新房间列表
						memset(temp,0,sizeof(temp));
						//memset(dlg->onlinecount,0,sizeof(dlg->onlinecount));
						roomcount=1;
						for(int j=3;j<strlen(buff);j++) 
							temp[j-3]=buff[j];//===========去掉标注信息
						//dlg->MessageBox(buff);
						//dlg->MessageBox(temp);
						//找出可以放新房间的位置
						do{
							roomcount++;
						}while(dlg->onlinecount[roomcount]!=0);
						//dlg->MessageBox(buff);
						for(int j=0;j<strlen(buff)-3;j++)
							dlg->roomshow[roomcount][j]=temp[j];
						char t[10]={0};
						//itoa(strlen(buff)-3,t,10);
						//dlg->MessageBox(t);
						//itoa(roomcount,t,10);
						//dlg->MessageBox(t);
						//itoa(dlg->onlinecount[1],t,10);
						//dlg->MessageBox(t);
						//dlg->roomshow[roomcount][0]=t[0];
						dlg->roomshow[roomcount][strlen(buff)-3]='\0';
						//dlg->MessageBox(buff);
						//dlg->MessageBox(temp);
						//dlg->MessageBox(dlg->roomshow[roomcount]);
						//dlg->roomshow[roomcount][0]=strlen(buff)-3; 
						dlg->onlinecount[roomcount]=1;//目前创建人一人
						if(roomcount<=9){
							char roomidtemp[2]={0};
							itoa(roomcount,roomidtemp,10);
							memset(buff,0,sizeof(buff));
							buff[0]='0';
							buff[1]=roomidtemp[0];
						}else{
							memset(buff,0,sizeof(buff));
							itoa(roomcount,buff,10);
						}
						i=atoi(clitag);//获得原来的房间
						dlg->onlinecount[i]-=1;//原来的房间人数-1
						clitag=buff;
						//更改储存的tag
						dlg->clinickname[msgcount][dlg->clinickname[msgcount][0]+1]=clitag[0];
						dlg->clinickname[msgcount][dlg->clinickname[msgcount][0]+2]=clitag[1];
						char gai[100]="c";
						strcat(gai,clitag);
						//strcat(gai,temp);
						//dlg->sendtoall(gai);
						//dlg->MessageBox(clitag);
						send(dlg->msgsock[msgcount],gai,100,0);//发送clitag
						//=====刷新房间列表========================================
						//for(int i=dlg->m_user.GetItemCount()-1; i>=0;i--)
						//		dlg->m_user.DeleteItem(i);
						//dlg->MessageBox("刷新ROOM列表");
						roomcount=0;
						for(int i=1;i<=49;i++){
							if(dlg->onlinecount[i]>=1||i==1){
								memset(temp,0,sizeof(temp));
								for(int j=0;j<strlen(dlg->roomshow[i]);j++)
									temp[j]=dlg->roomshow[i][j];
								//itoa(i,t,10);
								//dlg->MessageBox(temp);
							//	dlg->MessageBox(dlg->roomshow[i]);
								char gai[100]="8";
								strcat(gai,clitag);
								gai[1]=dlg->clinickname[i][dlg->clinickname[i][0]+1];
								gai[2]=dlg->clinickname[i][dlg->clinickname[i][0]+2];
								strcat(gai,temp);
								dlg->sendtoall(gai);
								//dlg->MessageBox("发送跟新的代码");
								//dlg->MessageBox(gai);
							}
						}
						dlg->sendtoall("b");
						//为用户刷新新的用户列表
						for(int i=dlg->m_user.GetItemCount()-1; i>=0;i--)
								dlg->m_user.DeleteItem(i);
						dlg->countuser=0;
						for(int i=0;i<=49;i++){
							memset(temp,0,sizeof(temp));
							if(dlg->clinickname[i][0]>=1){
								for(int j=0;j<dlg->clinickname[i][0];j++)
									temp[j]=dlg->clinickname[i][j+1];
								dlg->m_user.InsertItem(dlg->countuser++,temp);
								dlg->m_user.Scroll(size);
								char gai[100]="5";
								strcat(gai,clitag);
								gai[1]=dlg->clinickname[i][dlg->clinickname[i][0]+1];
								gai[2]=dlg->clinickname[i][dlg->clinickname[i][0]+2];
								strcat(gai,temp);
								dlg->sendtoall(gai);
								//dlg->MessageBox("发送跟新的代码");
								//dlg->MessageBox(gai);
							}
						}
						dlg->sendtoall("b");
					}
					if(buff[0]=='a'){//IN the room
						//改变clitag、更改房间人数
						char roomid[2]={0};
						int linecount=0;//有效房间计数
						memset(temp,0,sizeof(temp));
						for(int j=1;j<strlen(buff);j++) 
							roomid[j-1]=buff[j];//===========去掉标注信息
						linecount=atoi(roomid);
						i=0;
						linecount++;//因为零行没被记录
						//itoa(linecount,buff,10);
						//dlg->MessageBox(buff);
						while(linecount){
							if(dlg->onlinecount[i++]||i++==1)linecount--;//根据提供的客户端行号找房间。
						}
						i--;
						//itoa(i,buff,10);
						//dlg->MessageBox("找到的tag");
						//dlg->MessageBox(buff);
						if(i<=9){
							char roomidtemp[2]={0};
							itoa(i,roomidtemp,10);
							memset(buff,0,sizeof(buff));
							buff[0]='0';
							buff[1]=roomidtemp[0];
						}else{
							memset(buff,0,sizeof(buff));
							itoa(i,buff,10);
						}
						i=atoi(clitag);//获得原来的房间
						dlg->onlinecount[i]-=1;
						//dlg->MessageBox("原房间的TAG");
						//dlg->MessageBox(clitag);
						clitag=buff;
						dlg->clinickname[msgcount][dlg->clinickname[msgcount][0]+1]=clitag[0];
						dlg->clinickname[msgcount][dlg->clinickname[msgcount][0]+2]=clitag[1];
						itoa(dlg->onlinecount[i],buff,10);
						//dlg->MessageBox("原房间的人数");
						//dlg->MessageBox(buff);
						//dlg->MessageBox("现在的房间TAG");
						//dlg->MessageBox(clitag);
						//dlg->MessageBox("现在的房间人数");
						i=atoi(clitag);
						dlg->onlinecount[i]+=1;
						itoa(dlg->onlinecount[i],buff,10);
						//dlg->MessageBox(buff);

						char gai[100]="c";
						strcat(gai,clitag);
						send(dlg->msgsock[msgcount],gai,100,0);//发送clitag
						i=atoi(clitag);
						//dlg->onlinecount[i]+=1;
						//刷新列表
						roomcount=0;
						//dlg->MessageBox("刷新ROOM列表");
						for(int i=1;i<=49;i++){
							if(dlg->onlinecount[i]>=1||i==1){
								memset(temp,0,sizeof(temp));
								for(int j=0;j<strlen(dlg->roomshow[i]);j++)
									temp[j]=dlg->roomshow[i][j];
								//itoa(i,t,10);
								//dlg->MessageBox(temp);
								//dlg->MessageBox(dlg->roomshow[i]);
								char gai[100]="8";
								strcat(gai,clitag);
								gai[1]=dlg->clinickname[i][dlg->clinickname[i][0]+1];
								gai[2]=dlg->clinickname[i][dlg->clinickname[i][0]+2];
								strcat(gai,temp);
								dlg->sendtoall(gai);
							}
						}
						dlg->sendtoall("b");
						//为用户刷新新的用户列表
						for(int i=dlg->m_user.GetItemCount()-1; i>=0;i--)
								dlg->m_user.DeleteItem(i);
						dlg->countuser=0;
						for(int i=0;i<=49;i++){
							memset(temp,0,sizeof(temp));
							if(dlg->clinickname[i][0]>=1){
								for(int j=0;j<dlg->clinickname[i][0];j++)
									temp[j]=dlg->clinickname[i][j+1];
								dlg->m_user.InsertItem(dlg->countuser++,temp);
								dlg->m_user.Scroll(size);
								char gai[100]="5";
								strcat(gai,clitag);
								gai[1]=dlg->clinickname[i][dlg->clinickname[i][0]+1];
								gai[2]=dlg->clinickname[i][dlg->clinickname[i][0]+2];
								strcat(gai,temp);
								dlg->sendtoall(gai);
								//dlg->MessageBox("发送跟新的代码");
								//dlg->MessageBox(gai);
							}
						}
						dlg->sendtoall("b");
					}
				}
				
				/*//dlg->SetForegroundWindow();
				if (online!=SOCKET_ERROR)
				{
					dlg->m_user.InsertItem(1,buff);
					dlg->m_user.Scroll(size);
					//dlg->sendtoall(dlg->msgsock[msgcount],buff);
				}*/
			}

			dlg->clinickname[msgcount][0]=0;
			//dlg->MessageBox("clear");
			for(int i=dlg->m_user.GetItemCount()-1; i>=0;i--) dlg->m_user.DeleteItem(i);
			dlg->countuser=0;
			for(int i=0;i<=49;i++){
				memset(temp,0,sizeof(temp));
				if(dlg->clinickname[i][0]>=1){
					for(int j=0;j<dlg->clinickname[i][0];j++)
						temp[j]=dlg->clinickname[i][j+1];
					//dlg->MessageBox("更新用户列表");
					//char t[10]={0};
					//itoa(i,t,10);
					//dlg->MessageBox(t);
					//dlg->MessageBox(temp);
					dlg->m_user.InsertItem(dlg->countuser++,temp);
					dlg->m_user.Scroll(size);
					char gai[100]="5";
					strcat(gai,clitag);
					gai[1]=dlg->clinickname[i][dlg->clinickname[i][0]+1];
					gai[2]=dlg->clinickname[i][dlg->clinickname[i][0]+2];
					strcat(gai,temp);
					//dlg->MessageBox(gai);
					dlg->sendtoall(gai);
				}
			}
			dlg->sendtoall("b");
			i=atoi(clitag);
			dlg->onlinecount[i]-=1;
			//退出后刷新房间列表
			//dlg->MessageBox("刷新ROOM列表");
			roomcount=0;
						for(int i=1;i<=49;i++){
							if(dlg->onlinecount[i]>=1||i==1){
								memset(temp,0,sizeof(temp));
								for(int j=0;j<strlen(dlg->roomshow[i]);j++)
									temp[j]=dlg->roomshow[i][j];
								//itoa(i,t,10);
								//dlg->MessageBox(temp);
								//dlg->MessageBox(dlg->roomshow[i]);
								char gai[100]="8";
								strcat(gai,clitag);
								strcat(gai,temp);
								dlg->sendtoall(gai);
							}
						}
						dlg->sendtoall("b");

			send(dlg->msgsock[msgcount],"601Disconnected",100,0);
			dlg->m_list.InsertItem(dlg->count++,"Disconnected");
			dlg->m_list.Scroll(size);
			dlg->msgsock[msgcount]=NULL;
			for (int i=0;i<50;i++)
				if (dlg->msgsock[i]!=NULL)
					flag=1;
			if (flag!=1)
				dlg->m_button.EnableWindow(FALSE);
			closesocket(dlg->msgsock[msgcount]);
		}
	}
	//终止线程
	AfxEndThread(0);
	return 0;
}


//发送数据
void CCSocketDlg::OnButton1() 
{
	char buff[100];
	m_edit.GetWindowText(buff,99);
	m_edit.SetWindowText("");
	m_list.InsertItem(count++,buff);
	CSize size;
	size.cx=0;
	size.cy=30;
	m_list.Scroll(size);
	char gai[100]="700";
					
	//循环向所有客户发送信息
	for (int i=0;i<50;i++)
	{
		if (msgsock[i]!=NULL){
			strcat(gai,buff);
			send(msgsock[i],buff,100,0);
		}
	}
}


CCSocketDlg::~CCSocketDlg()
{
	for (int i=0;i<50;i++)
		if (msgsock[i]!=NULL)
			send(msgsock[i],"600Disconnected",100,0);
}

//获得还没有使用的socket数组号
int CCSocketDlg::getcount()
{
	for (int i=0;i<50;i++)
	{
		if (msgsock[i]==NULL)
			return i;
	}
	return -1;
	
}

//向所有客户发送数据
void CCSocketDlg::sendtoall(char *buff)
{
	for (int i=0;i<50;i++)
	{
		if (msgsock[i]!=NULL) {
			char t[10]={0};
			send(msgsock[i],buff,100,0);
		}
	}
}

void CCSocketDlg::OnFresh() 
{
	// TODO: Add your control notification handler code here
    m_list.DeleteAllItems();	
}


void CCSocketDlg::freshuser(void)
{
	for(int i=m_user.GetItemCount()-1; i>=0;i--)
		m_user.DeleteItem(i);
	countuser=0;
	int okcount=0;
	char temp[100]={0},buff[100]={0};
	for (int i=0;i<50;i++)
	{
		if (msgsock[i]!=NULL) {
			memset(temp,0,sizeof(temp));
			char t[10]={0};
			itoa(i,t,10);
			MessageBox("对第几号");
			MessageBox(t);
			send(msgsock[i],"f",100,0);
			recv(msgsock[i],buff,100,0);
			MessageBox("得到的回复是");
			MessageBox(buff);
			for(int j=3;j<strlen(buff);j++)
				temp[j-3]=buff[j];
			m_user.InsertItem(countuser++,temp);
			//m_user.Scroll(size);
			sendtoall(buff);
			//okcount++;
		}
	}
	sendtoall("b");
}
