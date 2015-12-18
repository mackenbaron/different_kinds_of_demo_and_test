// ShareSessionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ControlCenter.h"
#include "ShareSessionDlg.h"
#include "afxdialogex.h"
#include "NewShareSessionDlg.h"
#include "Message.h"
#include "ControlDlg.h"



// CShareSessionDlg �Ի���

IMPLEMENT_DYNAMIC(CShareSessionDlg, CDialogEx)

CShareSessionDlg::CShareSessionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShareSessionDlg::IDD, pParent)
{
	pService=NULL;
}

CShareSessionDlg::~CShareSessionDlg()
{
	ShareSessionListType::iterator pShareSession=sSessionList.begin();
	while(pShareSession!=sSessionList.end()){
		delete *pShareSession;
		pShareSession++;
	}
//	sSessionList.clear();
}

void CShareSessionDlg::SetService(CService* pService){
	this->pService=pService;
}

CService* CShareSessionDlg::GetService(){
	return pService;
}

CShareSession* CShareSessionDlg::GetShareSessionBySharerName(CString name){
	ShareSessionListType::iterator pShareSession=sSessionList.begin();
	while(pShareSession!=sSessionList.end()){
		if((*pShareSession)->sharer==name) break;
		pShareSession++;
	}
	if(pShareSession==sSessionList.end()) return NULL;
	return *pShareSession;
}



void CShareSessionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SESSION_LIST, m_list);
}


BEGIN_MESSAGE_MAP(CShareSessionDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK,IDC_SESSION_LIST,OnNMRClickSessionList)
	ON_COMMAND(IDM_ITEM_EDIT,OnSessionEdit)
	ON_COMMAND(IDM_ITEM_STOP,OnSessionStop)
	ON_COMMAND(IDM_ITEM_START,OnSessionStart)
	ON_COMMAND(IDM_ITEM_DELETE,OnSessionDel)
	ON_COMMAND(IDM_ITEM_CONTROL,OnSessionControl)
	ON_COMMAND(IDM_ITEM_LOCK,OnSessionLock)
	ON_COMMAND(IDM_WHITE_ADD,OnSessionAdd)	

	ON_MESSAGE(WM_MY_SHARERSTARTED,OnSharerStarted)
	ON_MESSAGE(WM_MY_SHARERNOTSTARTED,OnSharerNotStarted)
	ON_MESSAGE(WM_MY_SHARERSTOPPED,OnSharerStopped)
	ON_MESSAGE(WM_MY_SOCKCLOSE,OnSockClose)

	ON_WM_SIZE()
END_MESSAGE_MAP()


// CShareSessionDlg ��Ϣ�������


BOOL CShareSessionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	pService->SetShareSessionDlg(this);//ע���¼�
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CString sColName;

	sColName.LoadString(IDS_SHARELIST_COLNAME_STATE);
	m_list.InsertColumn(0,sColName/*_T("״̬")*/,0,90);

	sColName.LoadString(IDS_SHARELIST_COLNAME_SHARER);
	m_list.InsertColumn(1,sColName/*_T("������")*/,0,150);

	sColName.LoadString(IDS_SHARELIST_COLNAME_VIEWER);
	m_list.InsertColumn(2,sColName/*_T("�ۿ���")*/,0,150);

	m_images.Create(16,16,ILC_COLOR32|ILC_MASK,1,0);//?
	m_images.Add(AfxGetApp()->LoadIconW(IDI_SESSIONSTARTED_ICON));
	m_images.Add(AfxGetApp()->LoadIconW(IDI_SESSIONSTOPPED_ICON));

	m_list.SetImageList(&m_images,LVSIL_SMALL);
	
	return TRUE;
}



void CShareSessionDlg::OnNMRClickSessionList(NMHDR* pNMHDR, LRESULT* pResult){

	CPoint cp;
	GetCursorPos(&cp);
	m_list.ScreenToClient(&cp);
	
	CMenu menu;
	menu.LoadMenuW(IDR_SHARELIST_MENU);

	int nItem=m_list.HitTest(cp);
	m_list.ClientToScreen(&cp);

	if(nItem>=0){


		m_list.SetHotItem(nItem);

		CString name=m_list.GetItemText(nItem,1);
		CShareSession* pShareSession=this->GetShareSessionBySharerName(name);
		BOOL bState=pShareSession->bStateOn;
		BOOL bLock=pShareSession->bLockMode;

		CMenu* pMenu=menu.GetSubMenu(0);

		CString sMenuItemText;
		if(bState){
			sMenuItemText.LoadString(IDS_SHARELIST_MENUTEXT_STOPSESSION);
			pMenu->ModifyMenu(0,MF_BYPOSITION,IDM_ITEM_STOP,sMenuItemText/*_T("ֹͣ�Ự")*/);
		}
		else{
			sMenuItemText.LoadString(IDS_SHARELIST_MENUTEXT_STARTSESSION);
			pMenu->ModifyMenu(0,MF_BYPOSITION,IDM_ITEM_START,sMenuItemText/*_T("�����Ự")*/);	
		}
		if(bLock){
			pMenu->CheckMenuItem(IDM_ITEM_LOCK,MF_CHECKED);
		}
		else {
			pMenu->CheckMenuItem(IDM_ITEM_LOCK,MF_UNCHECKED);
		}


		pMenu->TrackPopupMenu(0,cp.x,cp.y,this);

	}
	else{
		CMenu* pMenu=menu.GetSubMenu(1);
		pMenu->TrackPopupMenu(0,cp.x,cp.y,this);
	}
}

void CShareSessionDlg::OnSessionAdd(){
	if(pService==NULL) return;

	CShareSession* pShareSession=new CShareSession();
	CNewShareSessionDlg dlg(pShareSession,this);


	if(dlg.DoModal()!=IDOK) return ;

	/*
	CString s;
	s.Format(_T("size = %d"),pShareSession->viewerList.size());
	MessageBox(s);
	*/

	//У���ɶԻ����õ������Ƿ�Ϸ�
	if(pShareSession->sharer==_T("")) return;

	//�����еĻỰ�Ĺۿ��߶��ŵ�һ������������ѯ�µĹۿ����Ƿ��ظ�
	//��һ�ֿ��Ƿ�����ֱ�ӿ�����ۿ��߶�Ӧ��DoorSock��bViewer������ֱ���ж�
	set<CString> allViewerList;

	ShareSessionListType::iterator pSession=sSessionList.begin();
	while(pSession!=sSessionList.end()){
		allViewerList.insert((*pSession)->viewerList.begin(),(*pSession)->viewerList.end());
		pSession++;
	}

	list<CString> & viewerList=pShareSession->viewerList;
	ViewerListType::iterator pViewer=viewerList.begin();
	while(pViewer!=viewerList.end()){
		if(allViewerList.count(*pViewer)>0) break;
		pViewer++;
	}

	
	/*
	CString sa;
	sa.Format(_T(" size = %d"),pShareSession->viewerList.size());
	MessageBox(sa);
	if(pViewer!=viewerList.end()){
		MessageBox(*pViewer);
	}
	*/



	if(pViewer!=viewerList.end()){//viewer �ظ�
		CString sText,sCap;
		sText.Format(IDS_SHARELIST_COLLISION_VIEWERREPEAT_TEXT,*pViewer);
		sCap.Format(IDS_SHARELIST_TIP);
		//MessageBox(_T("�ۿ���")+*pViewer+_T("�Ѿ��ڱ�Ĺ㲥�Ự�д���!"));//��������һ�䲻�ܵߵ�����������˷���������Сʱ
		MessageBox(sText,sCap);
		delete pShareSession;
		return;
	}
	//���������Ƿ�Ϸ�

	

	//ԭ���Ƿ��Ѿ�����������ߵĻỰ��
	
	for(pSession=sSessionList.begin();pSession!=sSessionList.end();pSession++){
		if((*pSession)->sharer==pShareSession->sharer) break;
	}

	if(pSession==sSessionList.end()){//��ͨ���
		this->sSessionList.push_back(pShareSession);
		pShareSession->StartSharer();
	}
	else{//�Ѵ��ڣ��ϲ�
		(*pSession)->viewerList.insert((*pSession)->viewerList.end(),pShareSession->viewerList.begin(),pShareSession->viewerList.end());
		
		CString append;
		for(pViewer=pShareSession->viewerList.begin();pViewer!=pShareSession->viewerList.end();pViewer++){
			append+=*pViewer+_T(";");
		}		

		int count=m_list.GetItemCount();
		int nItem;
		for(nItem=0;nItem<count;nItem++){
			if(m_list.GetItemText(nItem,1)==pShareSession->sharer) break;
		}

		m_list.SetItemText(nItem,2,m_list.GetItemText(nItem,2)+append);

		if((*pSession)->bStateOn){
			pShareSession->ticket=(*pSession)->ticket;
			pShareSession->StartViewers();
			//������˵Ļ�����ӵĲ��ᱻ����
		}

	
		delete pShareSession;
	}

	
}

void CShareSessionDlg::OnSessionEdit(){
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	if(pos==NULL) return;
	int nItem=m_list.GetNextSelectedItem(pos);
	//do with nItem;

	

}

void CShareSessionDlg::OnSessionStop(){
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	if(pos==NULL) return;
	int nItem;
	while(pos){
		nItem=m_list.GetNextSelectedItem(pos);	
		//do what you want
		CString sharer=m_list.GetItemText(nItem,1);
		CShareSession* pShareSession=GetShareSessionBySharerName(sharer);
//		pShareSession->LockViewers(FALSE);//��ֹͣ�Ựǰһ��Ҫ�Ƚ������,�����ͻ������
		pShareSession->StopSession();
	}
}


void CShareSessionDlg::OnSessionStart(){
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	if(pos==NULL) return;
	int nItem=m_list.GetNextSelectedItem(pos);
	CString sharer=m_list.GetItemText(nItem,1);
	CShareSession* pShareSession=GetShareSessionBySharerName(sharer);
	pShareSession->StartSharer();
}


void CShareSessionDlg::OnSessionDel(){
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	if(pos==NULL) return;
	int nItem=m_list.GetNextSelectedItem(pos);	
		
	CString sharer=m_list.GetItemText(nItem,1);
	CShareSession* pShareSession=GetShareSessionBySharerName(sharer);
	if(pShareSession->bStateOn){
		CString sText,sCap;
		sText.LoadString(IDS_SHARELIST_DELNOTE);
		sCap.LoadString(IDS_SHARELIST_TIP);
		//MessageBox(_T("����ֹͣ�Ự����ɾ����"));
		MessageBox(sText,sCap);
		return;
	}

//	pShareSession->StopSession();
	delete pShareSession;
	sSessionList.remove(pShareSession);
	m_list.DeleteItem(nItem);//
}


void CShareSessionDlg::OnSessionControl(){
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	if(pos==NULL) return;
	int nItem=m_list.GetNextSelectedItem(pos);
	CString name=m_list.GetItemText(nItem,1);
	CShareSession* pShareSession=GetShareSessionBySharerName(name);

	CControlDlg dlg(pShareSession,this);
	if(dlg.DoModal()!=IDOK) return;
	if(pShareSession==FALSE) return;

	if(pShareSession->bStateOn==FALSE){
		MessageBox(_T("�Ự��ֹͣ���޷��������Ȩ�ޣ�"));
		return;
	}

	if(pShareSession->bLockMode) {
		MessageBox(_T("���Ƚ�������ʹ��������ܣ�"));
		return;
	}// ����ķ����Ǳ��������Ȩ�޵Ĺۿ����Զ����������ָ�ΪviewȨ�޵Ĺۿ����Զ������������ǰ������ģʽ�Ļ���

	pShareSession->ChangeControl();
}


void CShareSessionDlg::OnSessionLock(){
	POSITION pos=m_list.GetFirstSelectedItemPosition();
	if(pos==NULL) return;
	int nItem=m_list.GetNextSelectedItem(pos);
	CString name=m_list.GetItemText(nItem,1);
	CShareSession* pShareSession=GetShareSessionBySharerName(name);
	if(pShareSession==NULL) return;
	if(pShareSession->bStateOn==FALSE){
		CString sText,sCap;
		sText.LoadString(IDS_SHARELIST_CTRLNOTE);
		sCap.LoadString(IDS_SHARELIST_TIP);
		//MessageBox(_T("�Ự��ֹͣ���޷�������"));
		MessageBox(sText,sCap);
		return;
	}

	BOOL bLock=pShareSession->bLockMode;

	if(bLock){
		pShareSession->LockViewers(FALSE);
	}
	else {	
		pShareSession->LockViewers();
	}

}


void CShareSessionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	static int i=0;

	if(i++>0){
		CRect rt;
		GetClientRect(&rt);
		m_list.MoveWindow(&rt);
	}	
}


LRESULT CShareSessionDlg::OnSharerStarted(WPARAM pName,LPARAM pTicket){
	CString name=*(CString*)pName;
	CString ticket=*(CString*)pTicket;
	CShareSession* pShareSeession=GetShareSessionBySharerName(name);
	if(pShareSeession==NULL) return 0;
	pShareSeession->ticket=ticket;
	pShareSeession->bStateOn=TRUE;
	CString cmd=_T("start viewer")+ticket;

	ViewerListType & viewerList=pShareSeession->viewerList;
	CString szNames;

	ViewerListType::iterator pViewer=viewerList.begin();
	while(pViewer!=viewerList.end()){
		szNames+=*pViewer+_T(";");
		pService->SockSend(*pViewer,cmd);
		pViewer++;
	}



	//���½��棺



	int count=m_list.GetItemCount();
	int i;
	for(i=0;i<count;i++){
		if(m_list.GetItemText(i,1)==name) break;
	}

	CString sState;
	if(!sState.LoadString(IDS_SHARESTATE_STARTED)) sState=_T("������");

	if(i>=count){
		
		
		m_list.InsertItem(count,sState,0);
		m_list.SetItemText(count,1,pShareSeession->sharer);
		m_list.SetItemText(count,2,szNames);
	}
	else {
		m_list.SetItemText(i,0,sState);
		m_list.SetItem(i,0,LVIF_IMAGE,0,0,0,0,0);
	}
	
	return 1;
}

LRESULT CShareSessionDlg::OnSharerNotStarted(WPARAM pName, LPARAM lParam){//�Ự����ʧ��........δ����
	CString name=*(CString*)pName;
	CShareSession* pShareSession=GetShareSessionBySharerName(name);
	if(pShareSession==NULL) return 0;

	int count=m_list.GetItemCount();
	int i=0;
	for(i=0;i<count;i++){
		if(name==m_list.GetItemText(i,1)) break;
	}

	CString sStopState;
	if(!sStopState.LoadString(IDS_SHARESTATE_STOPPED)) sStopState=_T("��ֹͣ");

	if(i<count){//���б���ͼ�ؼ����ҵ���		
		m_list.SetItemText(i,0,sStopState);
		m_list.SetItem(i,0,LVIF_IMAGE,0,1,0,0,0,0);
	}
	else{//�б���ͼ��û��


		CString szNames;

		for(CString name:pShareSession->viewerList){
			szNames+=(name+_T(";"));
		}

		m_list.InsertItem(count,sStopState,1);
		m_list.SetItemText(count,1,pShareSession->sharer);
		m_list.SetItemText(count,2,szNames);
	}

	return 1;
}

LRESULT CShareSessionDlg::OnSharerStopped(WPARAM pName, LPARAM lParam){


	CString name=*(CString*)pName;	
	CShareSession* pShareSession=GetShareSessionBySharerName(name);
	if(pShareSession==NULL) return 0;
	pShareSession->bStateOn=FALSE;
	pShareSession->bLockMode=FALSE;

	int count=m_list.GetItemCount();
	int i=0;
	for(i=0;i<count;i++){
		if(name==m_list.GetItemText(i,1)) break;
	}
	if(i<count){
		CString sStopState;
		if(!sStopState.LoadString(IDS_SHARESTATE_STOPPED)) sStopState=_T("��ֹͣ");
		m_list.SetItemText(i,0,sStopState);
		m_list.SetItem(i,0,LVIF_IMAGE,0,1,0,0,0,0);
	}

	return 1;
}

LRESULT CShareSessionDlg::OnSockClose(WPARAM wParam, LPARAM lParam){
	CString name=*(CString*)wParam;	
	CShareSession* pShareSeession=GetShareSessionBySharerName(name);
	if(pShareSeession==NULL) return 0;
	pShareSeession->bStateOn=FALSE;
	pShareSeession->StopViewers();


	int count=m_list.GetItemCount();
	int i=0;
	for(i=0;i<count;i++){
		if(name==m_list.GetItemText(i,1)) break;
	}
	if(i<count){
		CString sState;
		if(!sState.LoadString(IDS_SHARESTATE_TODEL)) sState=_T("��ɾ��");
		m_list.SetItemText(i,0,sState);
		m_list.SetItem(i,0,LVIF_IMAGE,0,1,0,0,0,0);
	}


	return 1;
}