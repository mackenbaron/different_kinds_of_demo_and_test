
// Peer.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Peer.h"
#include "PeerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPeerApp

BEGIN_MESSAGE_MAP(CPeerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPeerApp ����

CPeerApp::CPeerApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CPeerApp ����

CPeerApp theApp;


// CPeerApp ��ʼ��

BOOL CPeerApp::InitInstance()
{
	WSADATA data;
	AfxSocketInit(&data);

	HRESULT hr=CoInitializeEx(NULL,COINIT_APARTMENTTHREADED|COINIT_DISABLE_OLE1DDE);
	if(FAILED(hr)) return FALSE;


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

	/*
	CPeerDlg * pDlg=new CPeerDlg;
	m_pMainWnd = pDlg;
	pDlg->Create(CPeerDlg::IDD);
	pDlg->ShowWindow(SW_HIDE);
	pDlg->UpdateWindow();
	*/

	CPeerDlg dlg;
	dlg.Create(CPeerDlg::IDD);
	dlg.ShowWindow(SW_HIDE);
	dlg.RunModalLoop();

	





	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	CoUninitialize();

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
//	return TRUE;// caution : when to shut the app down ?���������������������������������� ע�⣺�����ֹ����
}

