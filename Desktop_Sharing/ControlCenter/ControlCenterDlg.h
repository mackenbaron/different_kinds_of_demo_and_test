
// ControlCenterDlg.h : ͷ�ļ�
//���ǳ������������CControlCenterDlg

#pragma once

#include "Service.h"
#include "afxcmn.h"
#include "PeerTreeDlg.h"
#include "ShareSessionDlg.h"
#include "MonitorSessionDlg.h"
#include "HandDlg.h"
#include "SettingDlg.h"
#include "TrayIcon.h"
#include "afxwin.h"

// CControlCenterDlg �Ի���
class CControlCenterDlg : public CDialogEx
{
// ����
public:
	CControlCenterDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CControlCenterDlg();

// �Ի�������
	enum { IDD = IDD_CONTROLCENTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	CService * pService;

	void StartService();
	void StopService();

	void InitControls();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CTrayIcon m_TrayIcon;

	CTabCtrl m_tab;
	CPeerTreeDlg m_PeerTreeDlg;
	CShareSessionDlg m_ShareSessionDlg;
	CMonitorSessionDlg m_MonitorSessionDlg;
	CHandDlg m_HandDlg;
	CSettingDlg m_SettingDlg;

	CStatic m_handNumText;
	CStatic m_onlineNumStatic;

public:
	CPeerTreeDlg& GetPeerTreeDlg(){return m_PeerTreeDlg;}


public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnStnClickedHandnumStatic();
	afx_msg LRESULT OnHandUp(WPARAM,LPARAM);
	afx_msg LRESULT OnTrayNotification(WPARAM,LPARAM);
	afx_msg LRESULT OnLogOnPeer(WPARAM,LPARAM);//�ͻ��˵�½ʱ�������¼�
	afx_msg LRESULT OnLogOffPeer(WPARAM,LPARAM);//�ͻ�������ʱ���¼�
	afx_msg void OnTrayOpenMainDlg();
	afx_msg void OnTrayExit();

	
//	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
