#pragma once
#include "afxsock.h"

// ���ǻ�������Ӧ�׽�����

class CService;
class CDoorSock :
	public CSocket
{
public:
	CDoorSock(CService*);
	~CDoorSock(void);


	CService* pService;
//	CString data;

	BOOL bSharer;//�Ƿ����˷������
	BOOL bViewer;//�Ƿ����˹ۿ�����
	BOOL bMonitor;//�Ƿ��ڼ�ػỰ��
	CString ticket;//��Ϊ���������ַ���Ʊ��

	void OnReceive(int nErrorCode);

	void OnClose(int nErrorCode);



};

