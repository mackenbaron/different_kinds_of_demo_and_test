#pragma once

#include "PeerTreeDlg.h"
#include "ShareSession.h"
#include "afxwin.h"


// CNewShareSessionDlg �Ի���
// ��Ҫ�����µĹ㲥�Ựʱ��������Ի������ڻ�÷����ߺ͹ۿ���

class CNewShareSessionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewShareSessionDlg)

public:
	CNewShareSessionDlg(CShareSession* pShareSession, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewShareSessionDlg();

// �Ի�������
	enum { IDD = IDD_SHARESESSION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


private:
	CPeerTreeDlg m_ShareTreeDlg;
	CPeerTreeDlg m_ViewTreeDlg;

	BOOL bViewChanged;//ָʾ�༭�Ựʱ��û���޸Ĺۿ���

	CShareSession *pShareSession;

	void InitControls();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
//	CButton m_LockCheckbox;
};
