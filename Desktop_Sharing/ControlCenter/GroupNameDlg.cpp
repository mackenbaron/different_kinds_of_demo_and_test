// GroupNameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ControlCenter.h"
#include "GroupNameDlg.h"
#include "afxdialogex.h"


// CGroupNameDlg �Ի���

IMPLEMENT_DYNAMIC(CGroupNameDlg, CDialogEx)

CGroupNameDlg::CGroupNameDlg(CString* groupName,CWnd* pParent /*=NULL*/)
	: CDialogEx(CGroupNameDlg::IDD, pParent)
{
	this->groupName=groupName;
}

CGroupNameDlg::~CGroupNameDlg()
{
}

void CGroupNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGroupNameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGroupNameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGroupNameDlg ��Ϣ�������


void CGroupNameDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_EDIT1,*groupName);
	CDialogEx::OnOK();
}
