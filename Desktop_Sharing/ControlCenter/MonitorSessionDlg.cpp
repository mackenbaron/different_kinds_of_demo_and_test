// MonitorSessionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ControlCenter.h"
#include "MonitorSessionDlg.h"
#include "afxdialogex.h"
#include "NewMonitorSessionDlg.h"
#include "Message.h"
#include "ViewerCtrl.h"
#include "ViewerDlg.h"

// CMonitorSessionDlg �Ի���

IMPLEMENT_DYNAMIC(CMonitorSessionDlg, CDialogEx)

CMonitorSessionDlg::CMonitorSessionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMonitorSessionDlg::IDD, pParent)
{
	pService=NULL;
	pMonitorViewerDlg=NULL;
}

CMonitorSessionDlg::~CMonitorSessionDlg()
{	
	if(pMonitorViewerDlg)
		delete pMonitorViewerDlg;
	
}

void CMonitorSessionDlg::SetService(CService* pService){
	this->pService=pService;
}



void CMonitorSessionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_SESSION_LIST,m_list);
}


BEGIN_MESSAGE_MAP(CMonitorSessionDlg, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_SESSION_LIST, &CMonitorSessionDlg::OnNMRClickSessionList)
	ON_MESSAGE(WM_MY_MONITOROPENED,OnMonitorOpened)
	ON_MESSAGE(WM_MY_MONITORNOTOPENED,OnMonitorNotOpened)
	ON_MESSAGE(WM_MY_MONITORCLOSED,OnMonitorClosed)
	ON_MESSAGE(WM_MY_SOCKCLOSE,OnSockClose)
	//�Ҽ��˵�
	ON_COMMAND(IDM_MITEM_OPENWINDOW,OnItemOpenViewerDlg)
	ON_COMMAND(IDM_MITEM_STOP,OnItemStop)
	ON_COMMAND(IDM_MITEM_DELETE,OnItemDelete)
	ON_COMMAND(IDM_MWHITE_CREATE,OnWhiteCreate)
	ON_COMMAND(IDM_MWHITE_OPENWINDOW,OnWhiteOpenViewerDlg)
END_MESSAGE_MAP()


// CMonitorSessionDlg ��Ϣ�������


void CMonitorSessionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	static int i=0;
	static CRect rt;

	if(i++>0){
		GetClientRect(&rt);
		m_list.MoveWindow(&rt);
	}
}


BOOL CMonitorSessionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	pService->SetMonitorSessionDlg(this);//ע���¼�
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CString sColNameState,sColNameWho;
	if(!sColNameState.LoadString(IDS_MONITORLIST_COLNAME_STATE)) sColNameState=_T("״̬");
	if(!sColNameWho.LoadString(IDS_MONITORLIST_COLNAME_WHO)) sColNameWho=_T("�࿴����");
	m_list.InsertColumn(0,sColNameState,0,90);
	m_list.InsertColumn(1,sColNameWho,0,160);
	

	m_images.Create(16,16,ILC_COLOR32|ILC_MASK,1,0);//?
	m_images.Add(AfxGetApp()->LoadIconW(IDI_SESSIONSTARTED_ICON));
	m_images.Add(AfxGetApp()->LoadIconW(IDI_SESSIONSTOPPED_ICON));

	m_list.SetImageList(&m_images,LVSIL_SMALL);

	pMonitorViewerDlg=new CMonitorViewerDlg;
	pMonitorViewerDlg->Create(CMonitorViewerDlg::IDD,this);
	
	return TRUE;
}


int CMonitorSessionDlg::GetItemByText(CString name){
	int count=m_list.GetItemCount();
	int i=0;
	for(i=0;i<count;i++){
		if(name==m_list.GetItemText(i,1)) return i;
	}
	return -1;
}


void CMonitorSessionDlg::OnNMRClickSessionList(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint cp;
	GetCursorPos(&cp);
	m_list.ScreenToClient(&cp);
	
	CMenu menu;
	menu.LoadMenuW(IDR_MONITORLIST_MENU);

	int nItem=m_list.HitTest(cp);
	m_list.ClientToScreen(&cp);

	if(nItem>=0){
		m_list.SetHotItem(nItem);
		CMenu* pMenu=menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(0,cp.x,cp.y,this);

	}
	else{
		CMenu* pMenu=menu.GetSubMenu(1);
		pMenu->TrackPopupMenu(0,cp.x,cp.y,this);
	}


	*pResult = 0;
}





LRESULT CMonitorSessionDlg::OnMonitorOpened(WPARAM wParam, LPARAM lParam){
	CString name=*(CString*)wParam;
	CString ticket=pService->GetSockByName(name)->ticket;

	if(pMonitorViewerDlg==NULL) return 0;
	
	

	pMonitorViewerDlg->AddViewer(name,ticket);

	//����
	CString sState;
	sState.LoadString(IDS_MONITORSTATE_STARTED);
	int nItem=m_list.GetItemCount();
	m_list.InsertItem(nItem,sState,0);
	m_list.SetItemText(nItem,1,name);

	return 1;
}

LRESULT CMonitorSessionDlg::OnMonitorNotOpened(WPARAM pName, LPARAM lParam){
	CString name=*(CString*)pName;
	//�ڴ�
	mSession.GetMonitorList().remove(name);

	//MonitorViewerDlg
	if(pMonitorViewerDlg){
		pMonitorViewerDlg->DeleteViewer(name);
	}

	//�б���ͼ�ؼ�
	CString sState;
	sState.LoadString(IDS_MONITORSTATE_TODELETE);

	int nItem=GetItemByText(name);
	if(nItem==-1){//�б�ؼ���û�ҵ�
		int n=m_list.GetItemCount();
		m_list.InsertItem(n,sState,1);
		m_list.SetItemText(n,1,name);

	}
	else {//�ҵ���
		m_list.SetItemText(nItem,0,sState);
		m_list.SetItem(nItem,0,LVIF_IMAGE,0,1,0,0,0,0);
	}

	return 1;
}


LRESULT CMonitorSessionDlg::OnMonitorClosed(WPARAM wParam, LPARAM lParam){
	CString name=*(CString*)wParam;
	int nItem=GetItemByText(name);
	mSession.GetMonitorList().remove(name);
	if(nItem==-1) return 0;
	m_list.DeleteItem(nItem);

	if(pMonitorViewerDlg){
		pMonitorViewerDlg->DeleteViewer(name);
	}

	return 1;
}

LRESULT CMonitorSessionDlg::OnSockClose(WPARAM wParam, LPARAM lParam){
	CString name=*(CString*)wParam;
	int nItem=GetItemByText(name);
	if(nItem==-1) return 0;
	CString sOffTip;
	sOffTip.Format(IDS_MONITOROFFLINETIP,name);
	MessageBox(sOffTip/*name+_T(" ���ߣ���Ӧ�ļ�ؼ����ر�!")*/);
	mSession.GetMonitorList().remove(name);	
	m_list.DeleteItem(nItem);
	return 1;
}

void CMonitorSessionDlg::OnWhiteCreate(){
	int count=mSession.GetMonitorList().size();
	MonitorListType& monitorList=mSession.GetMonitorList();
	set<MonitorListType::value_type> monitorSet(monitorList.begin(),monitorList.end());
	
	CMonitorSession tMonitorSession;
	CNewMonitorSessionDlg dlg(&tMonitorSession,this);
	if(dlg.DoModal()!=IDOK) return;
	if(tMonitorSession.GetMonitorList().size()==0) return;

	// ��֤��ѡ�ļ�ض����Ƿ��Ѵ��ڼ���б���
	MonitorListType& tMonitorList=tMonitorSession.GetMonitorList();
	MonitorListType::iterator pMonitor=tMonitorList.begin();
	while(pMonitor!=tMonitorList.end()){
		if(monitorSet.count(*pMonitor)>0) break;
		pMonitor++;
	}
	if(pMonitor!=tMonitorList.end()){//˵����ض������ظ�
		CString sMonitorRedundant;
		sMonitorRedundant.Format(IDS_MONITORREDUNDANTTIP,*pMonitor);
		//MessageBox(*pMonitor+_T(" ���ڼ���б���!"));
		MessageBox(sMonitorRedundant);
		return;
	}
	//��֤...
	
	monitorList.insert(monitorList.end(),tMonitorList.begin(),tMonitorList.end());//caution: not sure
	mSession.StartMonitors(count);	//���ǲ��е�
}

void CMonitorSessionDlg::OnWhiteOpenViewerDlg(){
	if(pMonitorViewerDlg==NULL) return;
	if(pMonitorViewerDlg->GetViewerCount()==0){
		CString sText,sCap;
		sText.LoadString(IDS_MONITORNOSESSION);
		sCap.LoadString(IDS_MONITORCOMMONCAP);
		MessageBox(sText/*_T("��ǰû�н����κμ��")*/,sCap);
	}
	else{		
		pMonitorViewerDlg->ConnectAllViewers();
		pMonitorViewerDlg->ShowWindow(SW_SHOW);
		
	}
	
}

void CMonitorSessionDlg::OnItemOpenViewerDlg(){
	
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	int nItem=m_list.GetNextSelectedItem(pos);
	CString name=m_list.GetItemText(nItem,1);

	if(pService->GetSockByName(name)->bMonitor==FALSE) return ;

	CString ticket(pService->GetSockByName(name)->ticket);
	
	CViewerDlg dlg;	
	
	dlg.Create(CViewerDlg::IDD);
	dlg.SetWindowText(name);
	dlg.ShowWindow(SW_NORMAL);	
	dlg.Connect((LPTSTR)(LPCTSTR)ticket);
	
	dlg.RunModalLoop();

}

void CMonitorSessionDlg::OnItemStop(){


}

void CMonitorSessionDlg::OnItemDelete(){
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	int nItem=m_list.GetNextSelectedItem(pos);
	CString name=m_list.GetItemText(nItem,1);

	if(pMonitorViewerDlg!=NULL){		
		pMonitorViewerDlg->DeleteViewer(name);
	}

	CDoorSock* pDoorSock=pService->GetSockByName(name);
	if(pDoorSock->bSharer){
		pDoorSock->bMonitor=FALSE;
		SendMessage(WM_MY_MONITORCLOSED,(WPARAM)&name,0);
	}
	else{
		pService->SockSend(name,_T("close monitor"));
	}
	CString sState;
	sState.LoadString(IDS_MONITORSTATE_TOSTOP);
	m_list.SetItemText(nItem,0,sState);
}