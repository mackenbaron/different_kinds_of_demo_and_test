#include "stdafx.h"
#include "Service.h"
#include "Message.h"


CService::CService(void)
{
	pEmployer=NULL;
	pLisSock=NULL;
	pTreeDlg=NULL;
	pShareSessionDlg=NULL;
	pMonitorSessionDlg=NULL;
	pHandDlg=NULL;
}


CService::~CService(void)
{
}


void CService::SetEmployer(CWnd* pWnd){
	this->pEmployer=pWnd;
}

void CService::SetTreeDlg(CWnd* pWnd){
	this->pTreeDlg=pWnd;	
}

void CService::SetShareSessionDlg(CWnd* pWnd){
	this->pShareSessionDlg=pWnd;
}

void CService::SetMonitorSessionDlg(CWnd* pWnd){
	this->pMonitorSessionDlg=pWnd;
}

CWnd* CService::GetMonitorSessionDlg(){
	return this->pMonitorSessionDlg;
}

void CService::SetHandDlg(CWnd* pWnd){
	this->pHandDlg=pWnd;
}



CString CService::GetNameBySock(CDoorSock* pSock){

	NameSockMapType::iterator pNameSock=nameSockMap.begin();
	while(pNameSock!=nameSockMap.end()){
		if(pNameSock->second==pSock) return pNameSock->first;
		pNameSock++;
	}
	return _T("");
}


CDoorSock* CService::GetSockByName(CString name){
	NameSockMapType::iterator pNameSock=nameSockMap.find(name);
	if(pNameSock==nameSockMap.end()) return NULL;
	return pNameSock->second;
}



// {  socket ����:

BOOL CService::StartSockService(int port){
	if(pLisSock!=NULL) return TRUE;	//�Ѿ������˷���,  ��ʲô�������أ�������������ִ���أ�
	pLisSock=new CListenSock(this);
	if(!(pLisSock->Create(port))){
		delete pLisSock;
		pLisSock=NULL;
		//::MessageBox(NULL,_T("�����׽���ʧ�ܣ�"),_T("����"),MB_OK);
		return FALSE;
	}
	if(!(pLisSock->Listen(20))){
		pLisSock->Close();//CAUTION: ��� close ����Ҫ����
		delete pLisSock;
		pLisSock=NULL;
		//::MessageBox(NULL,_T("�׽��ּ���ʧ��!"),_T("����"),MB_OK);
		return FALSE;
	}
	return TRUE;
}

void CService::StopSockService(){
	if(pLisSock==NULL) return;
	pLisSock->Close();
	delete pLisSock;
	pLisSock=NULL;
}

void CService::OnSockAccept(){
	CDoorSock* pDoorSock=new CDoorSock(this);

	if(!pLisSock->Accept(*pDoorSock)){
		delete pDoorSock;
		return;
	}

	SockSend(pDoorSock,_T("config disable exit"));//��Ȼ������д����������������������������

	doorSockList.push_back(pDoorSock);
}


void CService::OnSockReceive(CDoorSock* pDoorSock,CString str){
	if(pDoorSock==NULL) return;


//	::MessageBox(NULL,_T("OnSockReceive: ")+str,_T("�յ�"),MB_OK);

	//������
	if(str.Find(_T("logname"))==0){				//����ע������
		CString name=str.Mid(7);
		nameSockMap.insert(NameSockMapType::value_type(name,pDoorSock));
		//֪ͨ�����������µ��û���¼
	
		if(pTreeDlg){		
			SendMessage(pTreeDlg->GetSafeHwnd(),WM_MY_LOGNAME,0,(LPARAM)&name);// caution : ���ǲ��õģ�������������
		}
		if(pEmployer){
			PostMessage(pEmployer->GetSafeHwnd(),WM_MY_LOGNAME,0,0);//�����Ի�����Ϣ��������������
		}
	}

	else if(str.Find(_T("logoff"))==0){//���û�õ�
		delete pDoorSock;
		doorSockList.remove(pDoorSock);
		CString name=GetNameBySock(pDoorSock);
		if(name!=_T("")) nameSockMap.erase(name);
		//֪ͨ�������
	}

	else if(str.Find(_T("sharer started"))==0){//����RDP����
		pDoorSock->bSharer=TRUE;

		CString ticket=str.Mid(str.Find(_T("<E><A")));//��ȡticket��ע����ȡ�ķ����ǻ���ticket�ĸ�ʽ������<E><A��ͷ�������������������
		pDoorSock->ticket=ticket;
		CString name=GetNameBySock(pDoorSock);
		if(name==_T("")) return;//����˭�����ģ�
		//֪ͨ���淢Ʊ��viewer
		if(pShareSessionDlg==NULL) return;
		SendMessage(pShareSessionDlg->GetSafeHwnd(),WM_MY_SHARERSTARTED,(WPARAM)&name,(LPARAM)&ticket);//SendMessage���ܴ����������⣬����ĺ�����PostMessage
	}

	else if(str.Find(_T("sharer not started"))==0){//����start sharer����ʧ�ܻ�Ӧ
		//pDoorSock->bSharer=FALSE;

		CString name=GetNameBySock(pDoorSock);
		SendMessage(pShareSessionDlg->GetSafeHwnd(),WM_MY_SHARERNOTSTARTED,(WPARAM)&name,0);
	}

	else if(str.Find(_T("sharer stopped"))==0){

		pDoorSock->bSharer=FALSE;
//		pDoorSock->bMonitor=FALSE;//����д��α���Ӧ�ǳ�ͻ��
//		pDoorSock->ticket=_T("");
		CString name=GetNameBySock(pDoorSock);
		if(name!=_T("")) SendMessage(pShareSessionDlg->GetSafeHwnd(),WM_MY_SHARERSTOPPED,(WPARAM)&name,0);
	}

	else if(str.Find(_T("viewer started"))==0){//������RDP�ۿ�
		pDoorSock->bViewer=TRUE;
		
	}

	else if(str.Find(_T("viewer stopped"))==0){
		pDoorSock->bViewer=FALSE;	
	}

	else if(str.Find(_T("monitor opened"))==0){//�������
		pDoorSock->bMonitor=TRUE;
		//��ȡticket
		pDoorSock->ticket=str.Mid(str.Find(_T("<E><A")));		
		if(pMonitorSessionDlg==NULL) return;
		CString name=GetNameBySock(pDoorSock);
		if(name==_T("")) return;
		SendMessage(pMonitorSessionDlg->GetSafeHwnd(),WM_MY_MONITOROPENED,(WPARAM)&name,0);

	}

	else if(str.Find(_T("monitor not opened"))==0){
		//pDoorSock->bMonitor=FALSE;
		CString name=GetNameBySock(pDoorSock);
		if(name==_T("")) return ;
		SendMessage(pMonitorSessionDlg->GetSafeHwnd(),WM_MY_MONITORNOTOPENED,(WPARAM)&name,0);
	}

	else if(str.Find(_T("monitor closed"))==0){
//		pDoorSock->bSharer=FALSE;
		pDoorSock->bMonitor=FALSE;

//		pDoorSock->ticket=_T("");

		CString name=GetNameBySock(pDoorSock);
		if(name==_T("")) return;
		SendMessage(pMonitorSessionDlg->GetSafeHwnd(),WM_MY_MONITORCLOSED,(WPARAM)&name,0);

	}

	else if(str.Find(_T("hand up"))==0){
	
		CString name=GetNameBySock(pDoorSock);
		if(name==_T("")) return;
		SendMessage(pHandDlg->GetSafeHwnd(),WM_MY_HANDUP,(WPARAM)&name,0);
		PostMessage(AfxGetApp()->m_pMainWnd->GetSafeHwnd(),WM_MY_HANDUP,0,0);//ע���������һ��ִ�к�������ִ����ȷ
	}


}

void CService::OnSockClose(CDoorSock* pDoorSock){
	//1.�ڴ����
	if(pDoorSock==NULL) return;
	delete pDoorSock;
	doorSockList.remove(pDoorSock);
	CString name=GetNameBySock(pDoorSock);
	nameSockMap.erase(name);

	//3.֪ͨ�������
	
	SendMessage(pTreeDlg->GetSafeHwnd(),WM_MY_SOCKCLOSE,(WPARAM)&name,0);
	SendMessage(pShareSessionDlg->GetSafeHwnd(),WM_MY_SOCKCLOSE,(WPARAM)&name,0);
	SendMessage(pMonitorSessionDlg->GetSafeHwnd(),WM_MY_SOCKCLOSE,(WPARAM)&name,0);
	PostMessage(pEmployer->GetSafeHwnd(),WM_MY_SOCKCLOSE,0,0);//doesn't care who

	
}


BOOL CService::SockSend(CDoorSock* pDoorSock, CString str){
	if(pDoorSock==NULL) return FALSE;
	//�������
	//ֻҪ����TRUE���ϲ�ͻ���Ϊ���ͳɹ���α�����ʹ����ʱҲ���ϲ���ֳ�һ�µ���Ϊ
	if(str.Find(_T("start sharer"))==0 && ( pDoorSock->bSharer || pDoorSock->bMonitor)){
		pDoorSock->bSharer=TRUE;
		OnSockReceive(pDoorSock,_T("sharer started")+pDoorSock->ticket);//α��
		return TRUE;	
	}
	if(str.Find(_T("stop sharer"))==0){
		if(pDoorSock->bSharer==FALSE || pDoorSock->bMonitor){
			pDoorSock->bSharer=FALSE;
			OnSockReceive(pDoorSock,_T("sharer stopped"));
			return TRUE;
		}		
	}
	if(str.Find(_T("start viewer"))==0 && pDoorSock->bViewer){
		return FALSE;
	}
	if(str.Find(_T("stop viewer"))==0 && pDoorSock->bViewer==FALSE){
		OnSockReceive(pDoorSock,_T("viewer stopped"));
		return TRUE;
	}
	if(str.Find(_T("open monitor"))==0 && (pDoorSock->bMonitor || pDoorSock->bSharer)){
		pDoorSock->bMonitor=TRUE;
		OnSockReceive(pDoorSock,_T("monitor opened")+pDoorSock->ticket);
		return TRUE;
	}
	if(str.Find(_T("close monitor"))==0){
		if(pDoorSock->bMonitor==FALSE || pDoorSock->bSharer){
			pDoorSock->bMonitor=FALSE;
			OnSockReceive(pDoorSock,_T("monitor closed"));
			return TRUE;
		}
	}


	int count=pDoorSock->Send(str.GetBuffer(str.GetLength()),sizeof(TCHAR)*str.GetLength());
	if(count==sizeof(TCHAR)*str.GetLength()) return TRUE;//caution��������֤����δ����֤
	return FALSE;
}

BOOL CService::SockSend(CString name, CString str){
	NameSockMapType::iterator pNameSock=nameSockMap.find(name);
	if(pNameSock==nameSockMap.end()) return FALSE;
	
	return SockSend(pNameSock->second,str);
}


void CService::SockSendAll(CString cmd){
	for(auto sock:doorSockList){//����C++��׼�е���
		SockSend(sock,cmd);
	}
}

// } socket ����


// { �ļ�����



// } �ļ�����



// { ϵͳ����


// } ϵͳ����