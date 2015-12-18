// GroupChooseDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ControlCenter.h"
#include "GroupChooseDlg.h"
#include "afxdialogex.h"


// CGroupChooseDlg �Ի���

IMPLEMENT_DYNAMIC(CGroupChooseDlg, CDialogEx)

CGroupChooseDlg::CGroupChooseDlg(CString * groupName, list<CString>* pDataList, CWnd* pParent /*=NULL*/)
	: CDialogEx(CGroupChooseDlg::IDD, pParent)
{
	this->groupName=groupName;
	this->pDataList=pDataList;
}

CGroupChooseDlg::~CGroupChooseDlg()
{
}

void CGroupChooseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_GroupCombo);
}


BEGIN_MESSAGE_MAP(CGroupChooseDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGroupChooseDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGroupChooseDlg ��Ϣ�������


void CGroupChooseDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_GroupCombo.GetWindowText(*groupName);
	CDialogEx::OnOK();
}


BOOL CGroupChooseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	list<CString>::iterator pData=pDataList->begin();
	while(pData!=pDataList->end()){
		m_GroupCombo.AddString(*pData);
		pData++;
	}

	return TRUE;
}
