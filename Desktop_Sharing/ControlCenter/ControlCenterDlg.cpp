
// ControlCenterDlg.cpp : ʵ���ļ�
//�������ڶԻ�����CAboutDlg�������붨������Ի�����CControlCenterDlg���ʵ��

#include "stdafx.h"
#include "ControlCenter.h"
#include "ControlCenterDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "Message.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MY_TRAY_NOTIFICATION WM_USER+20


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CControlCenterDlg �Ի���




CControlCenterDlg::CControlCenterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CControlCenterDlg::IDD, pParent),pService(NULL),m_TrayIcon(IDR_TRAY_MENU)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CControlCenterDlg::~CControlCenterDlg(){
	StopService();
}



void CControlCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_HANDNUM_STATIC, m_handNumText);
	DDX_Control(pDX, IDC_ONLINENUM_STATIC, m_onlineNumStatic);
}

BEGIN_MESSAGE_MAP(CControlCenterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CControlCenterDlg::OnTcnSelchangeTab1)
	ON_STN_CLICKED(IDC_HANDNUM_STATIC, &CControlCenterDlg::OnStnClickedHandnumStatic)
	ON_MESSAGE(WM_MY_HANDUP,OnHandUp)
	ON_MESSAGE(WM_MY_TRAY_NOTIFICATION,OnTrayNotification)
	ON_MESSAGE(WM_MY_LOGNAME,OnLogOnPeer)
	ON_MESSAGE(WM_MY_SOCKCLOSE,OnLogOffPeer)
	ON_COMMAND(IDM_TRAY_MAIN,OnTrayOpenMainDlg)
	ON_COMMAND(IDM_TRAY_EXIT,OnTrayExit)

//	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CControlCenterDlg ��Ϣ�������

BOOL CControlCenterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	ASSERT((IDM_SETTINGBOX & 0xFFF0) == IDM_SETTINGBOX);
	ASSERT(IDM_SETTINGBOX < 0xF000);

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
			//pSysMenu->AppendMenu(MF_STRING,IDM_SETTINGBOX,_T("����"));//��ʱ��Ҫ�������
			CString strTemp;//���������ַ���
			if(strTemp.LoadString(IDS_CONTROLCENTERDLG_CONFIGCLIENTEXIT))
			pSysMenu->AppendMenu(MF_STRING, IDM_CONFIGCLIENT_ENABLEEXIT, strTemp /*_T("����ͻ����˳�")*/);

			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	StartService();
	//ע�⣺һ��Ҫ�ڸ����ӶԻ����ʼ��ǰ�������񣬷�������

	InitControls();

	/*
	CPeerTreeDlg dlg;
	pService->SetTreeDlg(&dlg);
	dlg.DoModal();
	*/

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


//��������������֮����ΪһЩ�ӶԻ��������Ҫ����
void CControlCenterDlg::InitControls(){

	CString strTemp;//���������ַ�����Դ

	if(strTemp.LoadString(IDS_CONTROLCENTERDLG_MANAGEGROUP))
		m_tab.InsertItem(0,strTemp /*_T("�������")*/);
	if(strTemp.LoadString(IDS_CONTROLCENTERDLG_SHAREDESKTOP))
		m_tab.InsertItem(1,strTemp /*_T("�㲥����")*/);
	if(strTemp.LoadString(IDS_CONTROLCENTERDLG_MONITORDESKTOP))
		m_tab.InsertItem(2,strTemp /*_T("�������")*/);

	m_ShareSessionDlg.SetService(pService);
	m_MonitorSessionDlg.SetService(pService);//��Щ��������Ҫ�ġ�

	m_PeerTreeDlg.Create(CPeerTreeDlg::IDD,&m_tab);
	m_ShareSessionDlg.Create(CShareSessionDlg::IDD,&m_tab);
	m_MonitorSessionDlg.Create(CMonitorSessionDlg::IDD,&m_tab);
	m_HandDlg.Create(CHandDlg::IDD,this);

	pService->SetTreeDlg(&m_PeerTreeDlg);//��������ǳ�ª�ġ�
	pService->SetHandDlg(&m_HandDlg);//��Serviceע���¼�

	CRect rt;
	m_tab.GetClientRect(&rt);
	rt.DeflateRect(2,23,2,2);

	m_PeerTreeDlg.MoveWindow(&rt);
	m_ShareSessionDlg.MoveWindow(&rt);
	m_MonitorSessionDlg.MoveWindow(&rt);

	m_PeerTreeDlg.ShowWindow(SW_SHOW);
	m_tab.SetCurSel(0);

	m_TrayIcon.SetNotificationWnd(this, WM_MY_TRAY_NOTIFICATION);//���ŵ��¼�ע�᷽ʽ
	m_TrayIcon.SetIcon(IDR_MAINFRAME);
	

}

void CControlCenterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if((nID & 0xFFF0)==IDM_SETTINGBOX){
		m_SettingDlg.DoModal();
	}
	
	else if(nID==SC_CLOSE){
		ShowWindow(SW_HIDE);
	}
	else if(nID==IDM_CONFIGCLIENT_ENABLEEXIT){
		CMenu* pMenu=this->GetSystemMenu(FALSE);
		if(pMenu->GetMenuState(IDM_CONFIGCLIENT_ENABLEEXIT,MF_CHECKED)){
			pMenu->CheckMenuItem(IDM_CONFIGCLIENT_ENABLEEXIT,MF_UNCHECKED);
			pService->SockSendAll(_T("config disable exit"));
		}
		else{
			pMenu->CheckMenuItem(IDM_CONFIGCLIENT_ENABLEEXIT,MF_CHECKED);
			pService->SockSendAll(_T("config enable exit"));
		}		
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CControlCenterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CControlCenterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CControlCenterDlg::StartService(){
	
	pService=new CService();

	CString strTemp,strTip,strTitle;
	BOOL bTip,bTitle;
	strTip=((bTip=strTemp.LoadString(IDS_CONTROLCENTERDLG_SOCKFAILTIP))?strTemp:_T("��������ʧ�ܣ���������������"));//��������ʧ�ܻ�ʹ�ú����ַ���
	strTitle=(bTitle=strTemp.LoadString(IDS_CONTROLCENTERDLG_SOCKFAILTITLE))?strTemp:_T("����");
	
	if(pService==NULL){
		
		::MessageBox(NULL,strTip/*_T("��������ʧ�ܣ���������������")*/,strTitle/*_T("����")*/,MB_OK|MB_ICONERROR);
		return;
	}
	theApp.SetService(pService);//ע�����
	pService->SetEmployer(this);

	TCHAR * ConfigFilePath=_T(".\\Config.ini");
	UINT port;
	port=::GetPrivateProfileInt(_T("Connection"),_T("port"),5000,ConfigFilePath);
	
	if(!pService->StartSockService(port)) MessageBox(strTip/*_T("�����׽��ַ���ʧ��,��������������")*/,strTitle);
}


void CControlCenterDlg::StopService(){
	if(pService!=NULL){
		delete pService;
		pService=NULL;
	}
}



void CControlCenterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	static int i=0;
	if(i++>0){

		CRect rt;
		GetClientRect(&rt);


		rt.DeflateRect(15,10,15,15);
		
		m_tab.MoveWindow(rt.left,rt.top,rt.Width(),rt.Height()-15);

		m_handNumText.MoveWindow(rt.left,rt.top+rt.Height()-13,rt.Width()/2,15);
		
		//m_onlineNumStatic.MoveWindow(rt.right-rt.Width()*4/9,rt.top+rt.Height()-13,rt.Width()*4/9,15);//��������

		
		m_tab.GetClientRect(&rt);
		rt.DeflateRect(2,23,2,2);
		m_PeerTreeDlg.MoveWindow(&rt);
		m_ShareSessionDlg.MoveWindow(&rt);
		m_MonitorSessionDlg.MoveWindow(&rt);

	}

	
	if(::IsWindow(m_onlineNumStatic.GetSafeHwnd())){
		CRect rt;
		m_onlineNumStatic.GetWindowRect(&rt);
		ScreenToClient(&rt);
		m_onlineNumStatic.MoveWindow(cx-rt.Width()-15,cy-15-13,rt.Width(),rt.Height());//���Ǿ����˾�ȷ����
	}
	

}


void CControlCenterDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int sel=m_tab.GetCurSel();

	switch(sel){

	case 0:
		m_PeerTreeDlg.ShowWindow(SW_SHOW);
		m_ShareSessionDlg.ShowWindow(SW_HIDE);
		m_MonitorSessionDlg.ShowWindow(SW_HIDE);
		break;

	case 1:
		m_PeerTreeDlg.ShowWindow(SW_HIDE);
		m_ShareSessionDlg.ShowWindow(SW_SHOW);
		m_MonitorSessionDlg.ShowWindow(SW_HIDE);
		break;

	case 2:
		m_PeerTreeDlg.ShowWindow(SW_HIDE);
		m_ShareSessionDlg.ShowWindow(SW_HIDE);
		m_MonitorSessionDlg.ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}


void CControlCenterDlg::OnStnClickedHandnumStatic()
{
//	MessageBox(_T("��ǰû���˾��֣�"));
	m_HandDlg.ShowWindow(SW_NORMAL);
}

LRESULT CControlCenterDlg::OnHandUp(WPARAM wParam, LPARAM lParam){

	CString szHandStatus;
	
	if(wParam==NULL){
		//szHandStatus.Format(_T("��ǰ��������%d"),m_HandDlg.GetItemCount());
		szHandStatus.Format(IDS_CONTROLCENTERDLG_HANDNUM,m_HandDlg.GetItemCount());
	}
	else {
		szHandStatus.Format(IDS_CONTROLCENTERDLG_HANDNUM,0);
	}
	m_handNumText.SetWindowText(szHandStatus);
	m_handNumText.UpdateWindow();
	return 1;
}

LRESULT CControlCenterDlg::OnLogOnPeer(WPARAM wParam, LPARAM lParam){
	int nOnlineNum=pService->GetNameSockMap()->size();
	CString sOnlineNumText;
	sOnlineNumText.Format(IDS_ONLINENUM,nOnlineNum);
	m_onlineNumStatic.SetWindowText(sOnlineNumText);
	return 1;
}

LRESULT CControlCenterDlg::OnLogOffPeer(WPARAM wParam, LPARAM lParam){

	return OnLogOnPeer(wParam,lParam);//������������������һ���ģ�������service��һ�£�д��static��
}

LRESULT CControlCenterDlg::OnTrayNotification(WPARAM wParam, LPARAM lParam){
	return m_TrayIcon.OnTrayNotification(wParam,lParam);	
}

void CControlCenterDlg::OnTrayOpenMainDlg(){
	this->ShowWindow(SW_SHOW);
}

void CControlCenterDlg::OnTrayExit(){	
	PostMessage(WM_CLOSE);
}




/*
BOOL CControlCenterDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(pWnd==&m_handNumText){
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));		
	}
	
	else{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	
	return TRUE;
}
*/
