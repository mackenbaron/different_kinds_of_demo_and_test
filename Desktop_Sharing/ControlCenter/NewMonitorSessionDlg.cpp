// NewMonitorSessionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ControlCenter.h"
#include "NewMonitorSessionDlg.h"
#include "afxdialogex.h"


// CNewMonitorSessionDlg �Ի���

IMPLEMENT_DYNAMIC(CNewMonitorSessionDlg, CDialogEx)

CNewMonitorSessionDlg::CNewMonitorSessionDlg(CMonitorSession* pMonitorSession, CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewMonitorSessionDlg::IDD, pParent),m_PeerTreeDlg(this,TRUE)
{
	this->pMonitorSession=pMonitorSession;
}

CNewMonitorSessionDlg::~CNewMonitorSessionDlg()
{
}

void CNewMonitorSessionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewMonitorSessionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNewMonitorSessionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewMonitorSessionDlg ��Ϣ�������


BOOL CNewMonitorSessionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitControls();

	return TRUE;
}

void CNewMonitorSessionDlg::InitControls(){
	
	CRect rt;
	GetClientRect(&rt);

	int x,y,width,height;

	x=rt.Width()/8;
	width=rt.Width()*6/8;
	y=rt.Height()/7;
	height=rt.Height()*5/7;

	m_PeerTreeDlg.Create(CPeerTreeDlg::IDD,this);
	m_PeerTreeDlg.SetTreeCheckable();
	m_PeerTreeDlg.FilterTree(theApp.GetMainDlg()->GetPeerTreeDlg());

	m_PeerTreeDlg.MoveWindow(x,y,width,height);
	m_PeerTreeDlg.ShowWindow(SW_SHOW);

	if(pMonitorSession) LoadSession();
}

void CNewMonitorSessionDlg::LoadSession(){//��pMonitorSession�е�����������


}

void CNewMonitorSessionDlg::StoreSession(){//�����������д��pMonitorSession��
	m_PeerTreeDlg.GetCheckedPeerNames(&(pMonitorSession->GetMonitorList()));
}


void CNewMonitorSessionDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	StoreSession();
	CDialogEx::OnOK();
}
