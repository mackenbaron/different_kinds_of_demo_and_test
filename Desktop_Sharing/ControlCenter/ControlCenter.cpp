
// ControlCenter.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ControlCenter.h"
#include "ControlCenterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CControlCenterApp

BEGIN_MESSAGE_MAP(CControlCenterApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CControlCenterApp ����

CControlCenterApp::CControlCenterApp():pService(NULL),pDlg(NULL)
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CControlCenterApp ����

CControlCenterApp theApp;


// CControlCenterApp ��ʼ��

BOOL CControlCenterApp::InitInstance()
{
	
	WSADATA data;
	AfxSocketInit(&data);

	HRESULT hr=CoInitializeEx(NULL,COINIT_APARTMENTTHREADED|COINIT_DISABLE_OLE1DDE);
	if(FAILED(hr)) return FALSE;

	/*
	HMODULE hModule=::LoadLibrary(_T("ControlCenterENU.dll"));
	if(hModule) ::AfxSetResourceHandle(hModule);
	else{
		AfxMessageBox(_T("failed to load language lib"));
	}
	*/
	

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	// { ���й��ģ��
	HMODULE hAdModule=::LoadLibrary(_T("AdModule.dll"));
	if(hAdModule){
		typedef void (*FuncType)(void);
		FuncType ShowProductDlg=(FuncType)::GetProcAddress(hAdModule,"ShowProductDlg");
		if(ShowProductDlg) ShowProductDlg();

		::FreeLibrary(hAdModule);
	}
	// } ���й��ģ��

	pDlg=new CControlCenterDlg();
	m_pMainWnd = pDlg;//Serviceͨ������ҵ������ڣ����Բ�Ҫ����ɾ��
	INT_PTR nResponse =pDlg->DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	delete pDlg;
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

void CControlCenterApp::SetService(CService* pService){
	this->pService=pService;
}

CService* CControlCenterApp::GetService(){
	return pService;
}