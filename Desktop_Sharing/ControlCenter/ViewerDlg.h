#pragma once
#include "rdpviewer1.h"
#include "resource.h"
#include "FullScreenOpDlg.h"


// CViewerDlg �Ի���
// ���ǵ�����һ����ػỰ��Ŀ����Ҽ����򿪼�ش���"ʱ�����ĶԻ���
// ���ڵ����Ĳ鿴һ����ض��������

class CViewerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewerDlg)

public:
	CViewerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CViewerDlg();

// �Ի�������
	enum { IDD = IDD_VIEWER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


private:
	BOOL m_bFullScrMode;
	CFullScreenOpDlg m_fsOpDlg;


public:
	CRdpviewer1 m_Viewer;

	

	virtual BOOL OnInitDialog();


	void Connect(LPTSTR ticket,LPTSTR username=_T("sun"),LPTSTR userpass=_T(""));
	void Disconnect();
	void SetFullScreen(BOOL bFull=TRUE);

	afx_msg void OnSize(UINT nType, int cx, int cy);	
	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg LRESULT OnExitFullScreen(WPARAM, LPARAM);
};
