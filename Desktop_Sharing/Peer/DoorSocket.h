#pragma once
#include "afxsock.h"

// CDoorSocket��
// �������׽�����

class CService;

class CDoorSocket :
	public CSocket
{
public:
	CDoorSocket(CService *);

	~CDoorSocket(void);

	CService * pService;

	void OnReceive(int);

	void OnClose(int);

};

