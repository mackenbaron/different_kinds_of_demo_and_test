#pragma once
#include "afxcmn.h"

typedef std::list<CString> HandListType;

// CHandDlg �Ի���
// ������Ϣ�Ի���

class CHandDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHandDlg)

public:
	CHandDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHandDlg();

// �Ի�������
	enum { IDD = IDD_HAND_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


private:
	CListCtrl m_listCtrl;
	HandListType handList;

public:
	
	virtual BOOL OnInitDialog();

	int GetItemCount(){ return handList.size();}

	afx_msg void OnClose();
	afx_msg LRESULT OnHandUp(WPARAM,LPARAM);	
	afx_msg void OnNMRClickHandList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHandListClear();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
