#pragma once
#include "Service.h"


// CMonitorSessionDlg �Ի���
// �������С�������桱��ǩҳ��Ӧ�ĶԻ���ֻ����һ���б���ͼ�ؼ���
// �ṩ�˶��ڼ�صĻỰ�Ĳ����ӿ�

typedef std::list<CMonitorSession*> MonitorSessionListType;

class CMonitorSessionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMonitorSessionDlg)

public:
	CMonitorSessionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMonitorSessionDlg();

// �Ի�������
	enum { IDD = IDD_SESSIONLIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:

	CListCtrl m_list;
	CImageList m_images;
	CService* pService;

	CMonitorViewerDlg* pMonitorViewerDlg;

	CMonitorSession mSession;

public:
	void SetService(CService*);

	virtual BOOL OnInitDialog();

	int GetItemByText(CString name);

	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg void OnNMRClickSessionList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemOpenViewerDlg();
	afx_msg void OnItemStop();
	afx_msg void OnItemDelete();
	afx_msg void OnWhiteCreate();
	afx_msg void OnWhiteOpenViewerDlg();

	afx_msg LRESULT OnMonitorOpened(WPARAM,LPARAM);
	afx_msg LRESULT OnMonitorNotOpened(WPARAM pName, LPARAM);
	afx_msg LRESULT OnMonitorClosed(WPARAM,LPARAM);
	afx_msg LRESULT OnSockClose(WPARAM,LPARAM);
};
