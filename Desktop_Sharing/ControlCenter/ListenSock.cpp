#include "stdafx.h"
#include "ListenSock.h"

#include "Service.h"

CListenSock::CListenSock(CService* service)
{
	this->pService=service;
}


CListenSock::~CListenSock(void)
{
}


void CListenSock::OnAccept(int nErrorCode){
	//���ErrorCode
	pService->OnSockAccept();	
}
