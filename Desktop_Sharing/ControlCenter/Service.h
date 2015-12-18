#pragma once
#include "stdafx.h"
#include "ListenSock.h"
#include "DoorSock.h"
#include "ShareSession.h"
#include "MonitorSession.h"



typedef std::map<CString,CDoorSock*> NameSockMapType;
typedef std::list<CDoorSock*> DoorSockListType;

//���ֲ�ͬ�ĻỰ����
typedef std::list<CShareSession*> ShareSessionListType;//����û�õ�


// CService��
// �����࣬�ṩ�������׽������ݷ�������շ��񣬲��ַ���Ϣ��֪ͨ����


class CService :
	public CObject
{
public:
	CService(void);
	~CService(void);


private:
	CWnd* pEmployer;

	CWnd* pTreeDlg;
	CWnd* pShareSessionDlg;
	CWnd* pMonitorSessionDlg;
	CWnd* pHandDlg;
	

	// socket ����
	CListenSock* pLisSock;
	DoorSockListType doorSockList;
	NameSockMapType nameSockMap;



	// �ļ���洢����



	/*
	// �Ự����	
	ShareSessionListType sSessionList;
	MonitorSessionListType mSessionList;
	*/



public:
	void SetEmployer(CWnd* );
	//�����������Լ���Ӧ������������Ϊ��Ϊ��Ӧ�Ľ���ע���¼�֪ͨ
	void SetTreeDlg(CWnd*);
	void SetShareSessionDlg(CWnd*);
	void SetMonitorSessionDlg(CWnd*);
	CWnd* GetMonitorSessionDlg();
	void SetHandDlg(CWnd*);

	NameSockMapType* GetNameSockMap(){ return &nameSockMap; }
	CString GetNameBySock(CDoorSock*);
	CDoorSock* GetSockByName(CString);

	// socket����
	BOOL StartSockService(int port=5000);
	void StopSockService();

	BOOL SockSend(CDoorSock*,CString);
	BOOL SockSend(CString name, CString cmd);
	void SockSendAll(CString cmd);
	void OnSockAccept();
	void OnSockReceive(CDoorSock*,CString);
	void OnSockClose(CDoorSock*);





};

