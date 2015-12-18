
// PeerDlg.h : ͷ�ļ�
//

#pragma once
#include "Service.h"
#include "TrayIcon.h"
#include "ViewerDlg.h"


// CPeerDlg �Ի���
// �ͻ��˳�������Ի����࣬�������Ľ���ʼ��Ϊ����״̬
// ϵͳ����ͼ��Ĳ˵��¼�Ҳ������ദ��
class CPeerDlg : public CDialogEx
{
// ����
public:
	CPeerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CPeerDlg();

// �Ի�������
	enum { IDD = IDD_PEER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	DECLARE_MESSAGE_MAP()



public:

	CService* pService;
	CTrayIcon m_trayIcon;
	CViewerDlg* pViewerDlg;
	
	

	//normal member functions

	
	

	//my message handle
	afx_msg LRESULT OnTrayNotification(WPARAM,LPARAM);
	afx_msg void OnSockConnect();
	afx_msg void OnOpenViewer();
	afx_msg void OnHand();
	afx_msg void OnExit();
	afx_msg LRESULT OnSockClose(WPARAM,LPARAM);
	afx_msg LRESULT OnStartViewer(WPARAM,LPARAM);
	afx_msg LRESULT OnStopViewer(WPARAM,LPARAM);
	afx_msg LRESULT OnLockViewer(WPARAM,LPARAM);
	afx_msg LRESULT OnUnlockViewer(WPARAM,LPARAM);
	afx_msg LRESULT OnConfig(WPARAM,LPARAM);


private:
	BOOL SockConnectWithConfig();//��������Ƕ�ȡ�����ļ��еĵ�ַ��˿���Ϣ���ٵ���CService��StartSockService����
	void ShowWarningOnConfig();
};
