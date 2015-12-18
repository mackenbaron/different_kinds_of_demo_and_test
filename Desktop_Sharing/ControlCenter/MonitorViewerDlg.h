#pragma once
#include "ViewerCtrl.h"

// CMonitorViewerDlg �Ի���
// ��������ͬʱ�ۿ����еļ������ĶԻ���

typedef std::list<CViewerCtrl*> ViewerCtrlListType;

class CMonitorViewerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorViewerDlg)

public:
	CMonitorViewerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMonitorViewerDlg();

// �Ի�������
	enum { IDD = IDD_MONITORVIEWER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


private:
	CRect m_ViewerRect;
	int m_ViewerWidth;
	int m_ViewerHeight;
	int m_ViewerMargin;

	ViewerCtrlListType m_ViewerList;

	std::map<CString,CString> monitorList;

	void NextRect();


public:
	
	void AddViewer(CString name, CString ticket);
	void DeleteViewer(CString name);
	void RepaintAllViewers();
	void RefreshAllViewers();
	void ConnectAllViewers();
	void DisconnectAllViewers();
	int GetViewerCount(){ return m_ViewerList.size();}
	CViewerCtrl* GetViewerCtrl(CString name);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
};
