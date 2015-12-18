#pragma once
#include "afxwin.h"
#include <list>

// CGroupChooseDlg �Ի���
// ��Ҫ�ƶ���Աʱ������ѡ��Ŀ�ķ���ĶԻ���
using namespace std;

class CGroupChooseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupChooseDlg)

public:
	CGroupChooseDlg(CString *groupName, list<CString> * pDataList,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGroupChooseDlg();

// �Ի�������
	enum { IDD = IDD_GROUPCHOOSE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CString *groupName;
	list<CString>* pDataList;
public:
	CComboBox m_GroupCombo;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
