#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"


// CSettingDlg �Ի���
// ���öԻ����������ó���ڰ���������ֹ����exe����Ĭ״̬�ȡ���ǰʵ����û�õ�

class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingDlg();

// �Ի�������
	enum { IDD = IDD_SETTING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:




	BOOL AllRegKey(HKEY hKey, LPCTSTR szDesKeyItem);
	CListCtrl m_wlist;
	CListCtrl m_blist;
	virtual BOOL OnInitDialog();
};
