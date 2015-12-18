#include "stdafx.h"
#include "ShareSession.h"
#include "ControlCenter.h"


CShareSession::CShareSession(void)
{
	bStateOn=FALSE;
	bPause=FALSE;
	bLockMode=FALSE;
	oldCtrlViewer=_T("");
	newCtrlViewer=_T("");
	pService=theApp.GetService();
}


CShareSession::~CShareSession(void)
{
	if(bStateOn){//������ڻỰ��
		StopViewers();
		StopSharer();
	}
}

void CShareSession::StartSharer(){	
	pService->SockSend(sharer,_T("start sharer"));
	//Ӧ�����յ� sharer started �� bStateOn ��Ϊ TRUE
}

void CShareSession::StopSharer(){
	pService->SockSend(sharer,_T("stop sharer"));
	//ע�⣬Ӧ�����յ� sharer stopped ʱ ��bStateOn��Ϊ FALSE
}

void CShareSession::StartViewers(){
	CString cmd=_T("start viewer")+ticket;
	ViewerListType::iterator pViewer=viewerList.begin();
	while(pViewer!=viewerList.end()){
		pService->SockSend(*pViewer,cmd);
		pViewer++;
	}
}

void CShareSession::StopViewers(){
	CString cmd=_T("stop viewer");
	ViewerListType::iterator pViewer=viewerList.begin();
	while(pViewer!=viewerList.end()){
		pService->SockSend(*pViewer,cmd);
		pViewer++;
	}
}


void CShareSession::GrantControl(CString name, BOOL grant){
	if(name==_T("")) return;
	CString cmd;
	if(grant){
		cmd=_T("request control interactive");
		//������
		if(bLockMode) pService->SockSend(name,_T("unlock"));

	}
	else{
		cmd=_T("request control view");
		//�ָ�����
		if(bLockMode) pService->SockSend(name,_T("lock"));
	}
	pService->SockSend(name,cmd);

}

void CShareSession::BlindViewers(BOOL enable){
	CString cmd;
	if(enable){
		bPause=TRUE;//�յ��ظ���Ϣʱ���ã�not now
		cmd=_T("request control none");
	}
	else {
		bPause=FALSE;
		cmd=_T("request control view");
	}
	ViewerListType::iterator pViewer;
	for(pViewer=viewerList.begin();pViewer!=viewerList.end();pViewer++){
		pService->SockSend(*pViewer,cmd);
	}

	oldCtrlViewer=_T("");
	newCtrlViewer=_T("");
}


void CShareSession::LockViewers(BOOL lock){
	if(bStateOn==FALSE) return;
	CString cmd;
	if(lock){
		bLockMode=TRUE;//��ʵ��ЩӦ�����յ�socket�Ļ�Ӧʱ���ã�����������
		cmd=_T("lock");
	}
	else {
		bLockMode=FALSE;
		cmd=_T("unlock");
	}

	ViewerListType::iterator pViewer;
	for(pViewer=viewerList.begin();pViewer!=viewerList.end();pViewer++){
		pService->SockSend(*pViewer,cmd);
	}
}





void CShareSession::ChangeControl(){

	GrantControl(oldCtrlViewer,FALSE);
	GrantControl(newCtrlViewer,TRUE);
}


/* 
���������������ôд��������ģ�����û�й���bStateOn��
��ιرջỰ�������������Ƿ���ģ��������޷�д��StartSession()һ��
*/
void CShareSession::StopSession(){
	if(bStateOn){
		StopViewers();
		StopSharer();//����д�������ǿ���sharer���� viewer�ر�
	}
}


