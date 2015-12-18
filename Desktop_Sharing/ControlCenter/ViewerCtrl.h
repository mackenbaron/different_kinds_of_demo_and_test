#pragma once
#include "rdpviewer1.h"
#include "afxwin.h"
#include "resource.h"


// CViewerCtrl �Ի���
// ��������CMonitorViewerDlg���Ӵ��壬����һ��RDP viewer �ؼ���һ����̬�ؼ�
// ������Ϊһ��RDP�ͻ�����ʾ��ض��������

class CViewerCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CViewerCtrl)

public:
	CViewerCtrl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CViewerCtrl();

// �Ի�������
	enum { IDD = IDD_VIEWERCTRL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CRdpviewer1 m_Viewer;
	CStatic m_Name;


public:
	void SetMonitorName(CString name){m_Name.SetWindowText(name);}
	CString GetMonitorName(){ CString name; m_Name.GetWindowText(name);return name;}
	void Connect(LPCTSTR ticket, LPCTSTR name, LPCTSTR pass){ m_Viewer.Connect(ticket,name,pass);}
	void Connect();
	void Disconnect(){m_Viewer.Disconnect();}
	void Repaint();
	void RefreshViewer();
	void put_SmartSizing(BOOL sm){ m_Viewer.put_SmartSizing(sm);}

	afx_msg void OnStnClickedNameStatic();
	afx_msg void OnSize(UINT nType, int cx, int cy);

};
