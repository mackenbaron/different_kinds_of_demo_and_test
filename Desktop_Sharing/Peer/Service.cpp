#include "stdafx.h"
#include "Service.h"
#include "ViewerDlg.h"
#include "Message.h"


CService::CService(void)
{
	bLockMode=FALSE;
	pSock=NULL;
	pSharingSession=NULL;
	pEmployer=NULL;
	pViewerDlg=NULL;
}


CService::~CService(void)
{
	StopRDPService();
	StopSockService();
	
}

void CService::SetEmployer(CWnd* pWnd){
	this->pEmployer=pWnd;
}

void CService::SetViewerDlg(CWnd* pWnd){
	this->pViewerDlg=pWnd;	
}


void CService::SetHook(BOOL use){//*************************************************δ������
	typedef BOOL (* HookFunc)(BOOL);
	static HMODULE hModule=LoadLibrary(_T("GlobalHook.dll"));//�������ִֻ��һ����

	if(use){
		if(bLockMode) return;//�Ѿ�����
		if(hModule==NULL) hModule=LoadLibrary(_T("GlobalHook.dll"));
		if(hModule){
			HookFunc SetKeyHook=(HookFunc)GetProcAddress(hModule,"SetKeyHook");
			if(SetKeyHook==NULL ||  SetKeyHook(TRUE)==FALSE) return ;
			//�ڳɹ����ü��̹��ӵ�����°�װ��깳��
			HookFunc SetMouseHook=(HookFunc)GetProcAddress(hModule,"SetMouseHook");
			if(SetMouseHook) SetMouseHook(TRUE);
			bLockMode=TRUE;
		}
	}
	else{
		if(bLockMode==FALSE) return;
		if(hModule){
			HookFunc SetKeyHook=(HookFunc)GetProcAddress(hModule,"SetKeyHook");
			if(SetKeyHook) SetKeyHook(FALSE);
			HookFunc SetMouseHook=(HookFunc)GetProcAddress(hModule,"SetMouseHook");
			if(SetMouseHook) SetMouseHook(FALSE);
			FreeLibrary(hModule);
			hModule=NULL;
			bLockMode=FALSE;
		}//����hModuleΪNULL����Ҫ�������Ƿ�Ҫ��װ�أ��ٵ���SetKeyHook(FALSE),����ͷ����أ�
	}
}



BOOL CService::GetTicket(CString* ticket){

	if(!pSharingSession) return FALSE;
	IRDPSRAPIInvitationManager * pInvitationManager;
	HRESULT  hr=pSharingSession->get_Invitations(&pInvitationManager);
	if(FAILED(hr)) return FALSE;
	IRDPSRAPIInvitation* pInvitation;
	hr=pInvitationManager->CreateInvitation(B("Trial"),B("Group"),B(""),10,&pInvitation);
	if(FAILED(hr)){
		return FALSE;
	}
	BSTR bstr=0;
	hr=pInvitation->get_ConnectionString(&bstr);
	//SetDlgItemText(IDC_EDIT1,bstr);
	*ticket=bstr;							//�ðɣ�δ����֤
	SysFreeString(bstr);

	return TRUE;	
}

void CService::StartRDPService(){
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(pSharingSession) return;

	HRESULT hr=CoCreateInstance(__uuidof(RDPSession),NULL,CLSCTX_ALL,__uuidof(IRDPSRAPISharingSession),reinterpret_cast<void**>(&pSharingSession));
	if(FAILED(hr)) return;

	//����¼�֪ͨ
	IConnectionPointContainer* pConnectionPointContainer=NULL;
	IConnectionPoint* pConnectionPoint=NULL;
	unsigned long cid;//connection id, critical,use it to unadvise
	hr=pSharingSession->QueryInterface(IID_IConnectionPointContainer,reinterpret_cast<void**>(&pConnectionPointContainer));

	if(FAILED(hr)) return;
	if(pConnectionPointContainer){
		pConnectionPointContainer->FindConnectionPoint(__uuidof(_IRDPSessionEvents),&pConnectionPoint);
		if(pConnectionPoint){
			pConnectionPoint->Advise(&myEvents,&cid);
		}
	}


	//��δ����������ʲô��

	//����Ƶ��
	wchar_t* chanelName=L"channel1";
	bool compress=false;
	int priority=3;


	
	IRDPSRAPIVirtualChannelManager* cm = 0;
	if(pSharingSession){			
		pSharingSession->get_VirtualChannelManager(&cm);
		if(cm){
			IRDPSRAPIVirtualChannel* c = 0;
			cm->CreateVirtualChannel(B(chanelName),(CHANNEL_PRIORITY)priority,compress ? 0 : CHANNEL_FLAGS_UNCOMPRESSED,&c);
		}
	}
	

	pSharingSession->Open();
}



void CService::StopRDPService(){
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(pSharingSession){
		pSharingSession->Close();
		pSharingSession->Release();//����Ǳ�Ҫ������������comָ���أ��ڴ�й¶��
		pSharingSession=NULL;
	}
}



BOOL CService::StartSockService(LPTSTR addr,int port){
	if(pSock!=NULL) return TRUE;//�Ѿ�������socket����

	pSock=new CDoorSocket(this);	// caution : �ͷ���Դ

	if(!pSock->Create()){
		
		CString sCreateSockFailText,sCreateSockFailCap;
		if(!sCreateSockFailText.LoadString(IDS_CREATESOCKFAIL_TEXT)) sCreateSockFailText=_T("�׽��ִ���ʧ��!");
		if(!sCreateSockFailCap.LoadString(IDS_ERROR_CAP)) sCreateSockFailCap=_T("����");
		::MessageBox(NULL,sCreateSockFailText,sCreateSockFailCap,MB_OK|MB_ICONERROR);

		delete pSock;
		pSock=NULL;
		return FALSE;
	}

	if(!pSock->Connect(addr,port)){
		CString sConnectSockFailText,sConnectSockFailCap;
		if(!sConnectSockFailText.LoadString(IDS_CONNECTSOCKFAIL_TEXT)) sConnectSockFailText=_T("�׽�������ʧ��!");
		if(!sConnectSockFailCap.LoadString(IDS_ERROR_CAP)) sConnectSockFailCap=_T("����");
		::MessageBox(NULL,sConnectSockFailText,sConnectSockFailCap,MB_OK|MB_ICONWARNING);
		delete pSock;
		pSock=NULL;
		return FALSE;
	}

	//������������û���
	TCHAR hostname[100];
	TCHAR username[100];
	DWORD hostlen=100;
	DWORD userlen=100;

	if(::GetComputerName(hostname,&hostlen) && ::GetUserName(username,&userlen)){
		hostname[hostlen]=_T('/');
		hostname[hostlen+1]=_T('\0');
		CString name(hostname);
		name+=username;
		name=_T("logname")+name;

		pSock->Send(name.GetBuffer(),sizeof(TCHAR)*name.GetLength());
	}
	else{
		CString sNameFailText,sNameFailCap;
		if(!sNameFailText.LoadString(IDS_GETNAMEFAIL_TEXT)) sNameFailText=_T("��ȡ���������û���ʧ�ܣ�");
		if(!sNameFailCap.LoadString(IDS_ERROR_CAP)) sNameFailCap=_T("����");
		::MessageBox(NULL,sNameFailText/*_T("��ȡ���������û���ʧ�ܣ�")*/,sNameFailCap/*_T("����")*/,MB_OK|MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}



void CService::StopSockService(){
	if(pSock!=NULL){
		pSock->Close();
		delete pSock;
		pSock=NULL;
	}
}

void CService::OnReceive(CString str){

	if(str.Compare(_T("start sharer"))==0){
		this->StartRDPService();
		CString ticket;
		if(!this->GetTicket(&ticket)){
			
			ticket=_T("sharer not started ticket failed");
			this->StopRDPService();	//��ֹ��ع�
			Send(ticket);

			CString sTickFailText,sTickFailCap;
			if(!sTickFailText.LoadString(IDS_GETTICKETFAIL_TEXT)) sTickFailText=_T("���� Ticket ʧ�ܣ�");
			if(!sTickFailCap.LoadString(IDS_ERROR_CAP)) sTickFailCap=_T("����");
			::MessageBox(NULL,sTickFailText,sTickFailCap,MB_OK|MB_ICONERROR);

			//MessageBox(NULL,_T("����ticketʧ�ܣ�"),_T("����"),MB_OK|MB_ICONERROR);
		}else{
			ticket=_T("sharer started ticket:")+ticket;
			Send(ticket);
		}

		//֪ͨ����̨
		
	
	}


	else if(str.Find(_T("stop sharer"))==0){

		if(pSharingSession!=NULL){
			pSharingSession->Close();
			pSharingSession->Release();
			pSharingSession=NULL;
		}
		Send(_T("sharer stopped"));
	}

	else if(str.Find(_T("start viewer"))==0){

		//pViewerDlg=new CViewerDlg;//�ͷ���Դ---------------------------------------
		CString ticket=str.Mid(str.Find(_T("<E><A")));//��ȡticket��ע����ȡ�ķ����ǻ���ticket�ĸ�ʽ������<E><A��ͷ�������������������
		//if(pEmployer==NULL) MessageBox(NULL,_T("ticket waisted, I am not ready now"),_T("����"),MB_OK|MB_ICONERROR);
		SendMessage(pEmployer->GetSafeHwnd(),WM_MY_STARTVIEWER,0,(LPARAM)&ticket);

		Send(_T("viewer started"));
	}

	else if(str.Find(_T("stop viewer"))==0){

		PostMessage(pEmployer->GetSafeHwnd(),WM_MY_STOPVIEWER,0,0);
		
		Send(_T("viewer stopped"));
	}

	else if(str.Find(_T("request control"))==0){
		CString param;
		if(str.Find(_T("interactive"))>0) param=_T("interactive");
		else if(str.Find(_T("view"))>0) param=_T("view");
		else if(str.Find(_T("none"))>0) param=_T("none");

		if(pViewerDlg)
		SendMessage(pViewerDlg->GetSafeHwnd(),WM_MY_RDPCONTROL,0,(LPARAM)&param);

	}


	else if(str.Find(_T("open monitor"))==0){//����start server����Ĵ�����һ����
		this->StartRDPService();
		CString ticket;

		if(!this->GetTicket(&ticket)){
			
			//֪ͨ����̨
			ticket=_T("monitor not opened ticket failed");
			this->StopRDPService();	//��ֹ��ع�
			Send(ticket);

			CString sTickFailText,sTickFailCap;
			if(!sTickFailText.LoadString(IDS_GETTICKETFAIL_TEXT)) sTickFailText=_T("���� Ticket ʧ�ܣ�");
			if(!sTickFailCap.LoadString(IDS_ERROR_CAP)) sTickFailCap=_T("����");
			::MessageBox(NULL,sTickFailText,sTickFailCap,MB_OK|MB_ICONERROR);

		}else{
			ticket=_T("monitor opened ticket:")+ticket;//��Ӧ��������start server���в�ͬ
			Send(ticket);
		}

		
	}

	else if(str.Find(_T("close monitor"))==0){
		if(pSharingSession!=NULL){
			pSharingSession->Close();
			pSharingSession->Release();
			pSharingSession=NULL;
		}

		Send(_T("monitor closed"));

	}

	



	else if(str.Find(_T("lock"))==0){		
		PostMessage(pEmployer->GetSafeHwnd(),WM_MY_LOCK,0,0);
	}

	else if(str.Find(_T("unlock"))==0){
		PostMessage(pEmployer->GetSafeHwnd(),WM_MY_UNLOCK,0,0);
	}

	else if(str.Find(_T("silence"))==0){

	}
	else if(str.Find(_T("unsilence"))==0){

	}

	else if(str.Find(_T("config"))==0){

		SendMessage(pEmployer->GetSafeHwnd(),WM_MY_CONFIG,0,(LPARAM)&str);		
	}


}

void CService::OnSockClose(){//���֪ͨ������������أ�
	StopSockService();
	StopRDPService();
	SetHook(FALSE);
	PostMessage(pEmployer->GetSafeHwnd(),WM_MY_SOCKCLOSE,0,0);//it does work	
}


BOOL CService::Send(CString str){
	if(pSock==NULL) return FALSE;
	int count=pSock->Send(str.GetBuffer(str.GetLength()),sizeof(TCHAR)*str.GetLength());
	return (count==sizeof(TCHAR)*str.GetLength());
}