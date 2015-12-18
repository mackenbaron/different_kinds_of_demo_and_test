
// ControlCenter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Service.h"
#include "ControlCenterDlg.h"

// CControlCenterApp:
// �йش����ʵ�֣������ ControlCenter.cpp
//

class CControlCenterApp : public CWinApp
{
public:
	CControlCenterApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()


private:
	CService* pService;
	CControlCenterDlg* pDlg;//���Ի���

public:
	void SetService(CService* pService);
	CService* GetService();
	CControlCenterDlg* GetMainDlg(){ return pDlg;}
};

extern CControlCenterApp theApp;