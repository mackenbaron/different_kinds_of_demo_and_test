
// PeerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Peer.h"
#include "PeerDlg.h"
#include "afxdialogex.h"
#include "Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MY_TRAY_NOTIFICATION WM_USER+1



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


// CPeerDlg �Ի���



CPeerDlg::CPeerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPeerDlg::IDD, pParent),m_trayIcon(IDR_TRAY_MENU),pViewerDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	pService=new CService();
	
	
}

CPeerDlg::~CPeerDlg(){
//	delete pDoorSock;
	if(pService!=NULL) delete pService;
	if(pViewerDlg!=NULL){
		pViewerDlg->Disconnect();
		delete pViewerDlg;
	}
}


void CPeerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPeerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MY_TRAY_NOTIFICATION,OnTrayNotification)
	ON_MESSAGE(WM_MY_SOCKCLOSE,OnSockClose)
	ON_MESSAGE(WM_MY_STARTVIEWER,OnStartViewer)
	ON_MESSAGE(WM_MY_STOPVIEWER,OnStopViewer)
	ON_MESSAGE(WM_MY_LOCK,OnLockViewer)
	ON_MESSAGE(WM_MY_UNLOCK,OnUnlockViewer)
	ON_MESSAGE(WM_MY_CONFIG,OnConfig)
	ON_COMMAND(ID_CONNECT,OnSockConnect)
	ON_COMMAND(ID_VIEWER,OnOpenViewer)
	ON_COMMAND(ID_HAND,OnHand)
	ON_COMMAND(ID_EXIT,OnExit)
	
END_MESSAGE_MAP()


// CPeerDlg ��Ϣ�������

BOOL CPeerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_trayIcon.SetNotificationWnd(this, WM_MY_TRAY_NOTIFICATION);	
	m_trayIcon.SetIcon(IDI_OFF_ICON);	
	pService->SetEmployer(this);

	SockConnectWithConfig();

	//��ʾ����
	ShowWarningOnConfig();
	


	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPeerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPeerDlg::OnPaint()
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

void CPeerDlg::OnNcPaint(){
	static int i=2;
	if(i>0){
		ShowWindow(SW_HIDE);
		i--;
	}
	else{
		CDialogEx::OnNcPaint();
	}
}


BOOL CPeerDlg::SockConnectWithConfig(){

	//�������ļ��л�÷��˵�ip��ַ�Ͷ˺�
	TCHAR* fileName=_T(".\\Config.ini");//��������Ŀ¼�µ������ļ�

	TCHAR addr[35];
	UINT port;
	::GetPrivateProfileString(_T("Connection"),_T("address"),_T("localhost"),addr,35,fileName);
	port=::GetPrivateProfileInt(_T("Connection"),_T("port"),5000,fileName);
	//���û�ж�ȡ�����ݣ�û��ini�ļ���ini�ļ����ݲ��ԣ������� localhost 5000

	if(pService->StartSockService(addr,port)){
		m_trayIcon.SetIcon(IDI_ON_ICON);
		return TRUE;
	}
	else{
		m_trayIcon.SetIcon(IDI_OFF_ICON);
		return FALSE;
	}
}

void CPeerDlg::ShowWarningOnConfig(){
	//�������ļ��л����ʾ���������
	TCHAR* fileName=_T(".\\Config.ini");//��������Ŀ¼�µ������ļ�	
	UINT bWarn=::GetPrivateProfileInt(_T("Prompt"),_T("warning"),1,fileName);//���û�ж���������Ϊ1

	if(bWarn){
		CString sWarnCap,sWarnText;
		if(!sWarnCap.LoadString(IDS_WARNING_CAP)) sWarnCap=_T("����");
		if(!sWarnText.LoadString(IDS_WARNING_TEXT)) sWarnText=_T("������������ɹ����ӷ������󣬱����潫��ʱ���ܱ�������أ�");
		::MessageBox(NULL,sWarnText,sWarnCap,MB_OK|MB_ICONWARNING);
	}
}





//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPeerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//�ҵ���Ϣ������
LRESULT CPeerDlg::OnTrayNotification(WPARAM wParam,LPARAM lParam){
	return m_trayIcon.OnTrayNotification(wParam,lParam);
}

LRESULT CPeerDlg::OnSockClose(WPARAM wParam,LPARAM lParam){
	
	//�ر�RDP�ͻ��˺ͷ�������
	if(pViewerDlg){
//		pViewerDlg->SetFullScreen(FALSE);
		pViewerDlg->Disconnect();
		delete pViewerDlg;
		pViewerDlg=NULL;
	}

	/* ����service����
	pService->StopRDPService();
	//�ر�socket
	pService->StopSockService();
	*/

	

//	MessageBox(_T("�������Ͽ�������!"));

	
	return 	m_trayIcon.SetIcon(IDI_OFF_ICON);;
}

LRESULT CPeerDlg::OnStartViewer(WPARAM wParam,LPARAM pTicket){
	
	if(pViewerDlg!=NULL) OnStopViewer(0,0);//
	pViewerDlg=new CViewerDlg();	
	
	pViewerDlg->Create(CViewerDlg::IDD);
	pViewerDlg->ShowWindow(SW_NORMAL);

	CString ticket=*(CString*)pTicket;
	pViewerDlg->Connect((LPTSTR)(LPCTSTR)ticket);
	pService->SetViewerDlg(pViewerDlg);//ע���¼�
	return 1;
}

LRESULT CPeerDlg::OnStopViewer(WPARAM wParam, LPARAM lParam){


	if(pViewerDlg==NULL) return 0;

	OnUnlockViewer(wParam,lParam);//
	
	pViewerDlg->SetFullScreen(FALSE);
	pViewerDlg->Disconnect();
	delete pViewerDlg;
	pViewerDlg=NULL;
	pService->SetViewerDlg(NULL);
	return 1;
}





LRESULT CPeerDlg::OnLockViewer(WPARAM wParam, LPARAM lParam){
	if(pViewerDlg==NULL) return 0;
	//1.��viewer��Ϊviewģʽ
	pViewerDlg->m_Viewer.RequestControl(CTRL_LEVEL_VIEW);	
	//2.��viewer��Ϊȫ��
	pViewerDlg->SetFullScreen();
	//3.ʹ�ù���
	pService->SetHook();//�������δ����֤

	return 1;
}

LRESULT CPeerDlg::OnUnlockViewer(WPARAM wParam, LPARAM lParam){
//	if(pViewerDlg)	pViewerDlg->SetFullScreen(FALSE);
	pService->SetHook(FALSE);
	return 1;
}


LRESULT CPeerDlg::OnConfig(WPARAM wParam, LPARAM lParam){
	CString configCmd=*(CString*)lParam;

//	MessageBox(configCmd);

	if(configCmd.Find(_T("disable exit"))>0){
		m_trayIcon.SetDiabledButton(ID_EXIT);
	}
	else if(configCmd.Find(_T("enable exit"))>0){
		m_trayIcon.SetDiabledButton(0);
	}

	return 1;
}




void CPeerDlg::OnSockConnect(){

	SockConnectWithConfig();
}

void CPeerDlg::OnHand(){
	if(!pService->Send(_T("hand up"))){
		//MessageBox(_T("���Ӿ���ʧ�ܣ����н������ˣ���ֱ����ϵ��ʦ��"));
		AfxMessageBox(IDS_HANDUPFAIL_TEXT);
	}
}

void CPeerDlg::OnExit(){
	SendMessage(WM_CLOSE);
}

void CPeerDlg::OnOpenViewer(){
	if(pViewerDlg==NULL){
		//MessageBox(_T("��ǰ������㲥�Ự!"));
		AfxMessageBox(IDS_OPENVIEWERFAIL_TEXT);
		return;
	}
	pViewerDlg->ShowWindow(SW_SHOW);
}
