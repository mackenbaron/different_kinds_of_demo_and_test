#pragma once


// CGroupNameDlg �Ի���
// ��Ҫ�������������������ʱ���������ڻ�ȡ�µķ������ĶԻ���

class CGroupNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupNameDlg)

public:
	CGroupNameDlg(CString* groupName,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGroupNameDlg();

// �Ի�������
	enum { IDD = IDD_GROUPNAME_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CString * groupName;
	
public:
	afx_msg void OnBnClickedOk();
};
