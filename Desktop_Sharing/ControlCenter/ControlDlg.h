#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ShareSession.h"

// CControlDlg �Ի���
// �Ҽ����һ���㲥�Ự����Ŀʱ����һ����Ȩ����ѡ������Ȩ���Ƶ�������Ի�������ѡ��
// �ۿ��ߣ��������Ȩ��

class CControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlDlg)

public:
	CControlDlg(CShareSession* pSession=NULL, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CControlDlg();

// �Ի�������
	enum { IDD = IDD_CONTROL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_SharerNameEdit;
	CListCtrl m_ViewerListCtrl;

	CShareSession* pShareSession;


private:
	void InitControls();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
