#pragma once
#include "PeerTreeDlg.h"
#include "MonitorSession.h"
// CNewMonitorSessionDlg �Ի���
// ��Ҫ�����µļ�ػỰʱ�ᵯ������Ի������ڻ�ü�ض���

class CNewMonitorSessionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewMonitorSessionDlg)

public:
	CNewMonitorSessionDlg(CMonitorSession* pMonitorSession=NULL,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewMonitorSessionDlg();

// �Ի�������
	enum { IDD = IDD_MONITOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CPeerTreeDlg m_PeerTreeDlg;

	CMonitorSession* pMonitorSession;

	void InitControls();
	void LoadSession();
	void StoreSession();
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
