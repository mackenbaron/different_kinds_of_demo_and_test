#pragma once
#include "afxsock.h"

// ���Ǽ����׽�����

class CService;

class CListenSock :
	public CSocket
{
public:
	CListenSock(CService*);
	~CListenSock(void);

	CService * pService;

	void OnAccept(int);
};

