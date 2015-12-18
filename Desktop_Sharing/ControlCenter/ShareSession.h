#pragma once
#include "afx.h"

// CShareSession��
// ����������ڴ����µĹ㲥�Ựʱ���ݸ�CNewShareSessionDlg�Ի��򲢻������
// ������Ҳ��ʾ�˹㲥�Ự��������ʵ�壬���ṩ�˹���㲥�滭��һЩ����


typedef std::list<CString> ViewerListType;

class CService;

class CShareSession :
	public CObject
{
public:	
	CShareSession(void);
	~CShareSession(void);


private:
	CService* pService;


public:
	BOOL bStateOn;//��ʾ�Ự�Ƿ��ڽ����У�ȡ����sharer�Ƿ���RDP����
	BOOL bPause;//��ʶ�Ự�Ƿ���ͣ����ִ��BlindViewers��TRUE�������е�RDPviewer���ڵ���none�Ŀ��Ƽ�����ʱ��Ϊpause
	CString sharer;
	ViewerListType viewerList;
	CString newCtrlViewer;//��ǰӵ�н������Ƽ���Ĺۿ��ߣ�Ĭ��Ϊ_T("")
	CString oldCtrlViewer;
	CString ticket;

	BOOL bLockMode;


	//�����������Լ������Լ�����ô��������һ��Service
	
	void StartSharer();
	void StopSharer();
	void StartViewers();
	void StopViewers();

	void GrantControl(CString name, BOOL grantOrRevoke=TRUE);
	void ChangeControl();
	void BlindViewers(BOOL enable=TRUE);
	void LockViewers(BOOL lock=TRUE);
	

	void StopSession();

	
	
};

