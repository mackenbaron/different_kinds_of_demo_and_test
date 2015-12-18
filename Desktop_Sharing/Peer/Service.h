#pragma once
#include "stdafx.h"
#include "DoorSocket.h"
#include "MyRdpSessionEvents.h"

// CService��
// �ṩ�׽��ַ���RDP����Ͱ�װϵͳ���ӵķ���

class CService :
	public CObject
{
public:
	CService(void);
	~CService(void);

	void SetEmployer(CWnd*);
	void SetViewerDlg(CWnd*);

	void SetHook(BOOL use=TRUE);

	

	//rdp
	BOOL GetTicket(CString*);
	void StartRDPService();
	void StopRDPService();

	//socket
	BOOL StartSockService(LPTSTR addr=_T("localhost"),int port=5000);
	void StopSockService();
	void OnReceive(CString cmd);
	void OnSockClose();
	BOOL Send(CString);

	BOOL GetSockState(){return pSock!=NULL;}//�Ƿ�������



private:
	BOOL bLockMode;//�Ƿ�����

	CWnd* pEmployer;
	CWnd* pViewerDlg;

	IRDPSRAPISharingSession* pSharingSession;
	CMyRDPSessionEvents myEvents;
	CDoorSocket* pSock;//�������ֵ�Ƿ�ΪNULL�ж��Ƿ���������socket

	

};

