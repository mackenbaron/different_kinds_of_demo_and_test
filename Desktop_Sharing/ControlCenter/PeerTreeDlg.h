#pragma once
//#include "afxcmn.h"
#include "stdafx.h"
#include "resource.h"
#include "Service.h"
#include "Message.h"

using namespace std;

typedef std::map<CString,std::list<CString>* > TreeDataType;
typedef std::list<CString> PeerListType;


//#define UNKNOWN_GROUP (_T("δ����"))

// CPeerTreeDlg �Ի���
// ������ʾ��Ա����ṹ�����ؼ����ڵĶԻ���������ķ�������ǩҳ��Ӧ����Ի���
// ͬʱ���ڴ����µĹ㲥�Ự���ػỰʱ����ѡ���Ա���Ӵ���Ҳ������Ի����ʵ��

class CPeerTreeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPeerTreeDlg)

public:
	CPeerTreeDlg(CWnd* pParent = NULL, BOOL bFilterTree=FALSE);   // ��׼���캯��	
	virtual ~CPeerTreeDlg();

// �Ի�������
	enum { IDD = IDD_PEERTREE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:

	virtual BOOL OnInitDialog();

private:

	/* ���ڷ������ݴ洢��˵����
	ÿ�������Ӧһ���ļ����ļ�����Ϊ������ÿ���ļ���һ�����ݼ�Ϊ�����ڵ�һ����Ա��
	*/
	BOOL bFilterTree;

	CService* pService;//�����ò���

	//�ؼ�
	CTreeCtrl m_PeerTree;
	CImageList m_ImageList;

	CString UNKNOWN_GROUP;//��Ӧ����һ������������������Դ
	HTREEITEM hUnGroup;

	BOOL bTreeCheckable;//��ʶ��ǰ���ؼ��Ƿ��Ǹ�ѡģʽ

	//����
	TreeDataType * pTreeData;

	CString treeDataPath;
	

	void InitTreeControl();
	void BuildTree();
//	void FilterItems(HTREEITEM);//����FilterTree�ĵݹ�ʵ��
//	void SetTreeCheckable(BOOL bCheckable=TRUE);//�����Ƿ���и�ѡ��

	BOOL IsGroupItem(HTREEITEM);
	BOOL IsPeerItem(HTREEITEM);

	// ���ݹ���
	BOOL LoadTreeData();
	void FreeTreeData();
	void VerifyLogPeer(CString peerName);
	BOOL GroupHasPeer(CString group, CString peer);
	HTREEITEM ItemStringToHandle(CString text, HTREEITEM root);
	HTREEITEM GetGroupItem(CString groupName);


	//�ļ�����
	BOOL DeleteStringFromFile(CString str, CString fileTitle);
	BOOL AppendStringToFile(CString str, CString fileTitle);
	BOOL CreateGroupFile(CString fileTitle);
	BOOL DeleteGroupFile(CString fileTitle);
	BOOL RenameGroupFile(CString fileTitle,CString newFileTitle);


	//ע�⣺����Ӧ����Ψһ�ģ������ظ�
	BOOL HasGroup(CString groupName);
//	HTREEITEM GetGroupItemByText(CString groupName);

	/*
	// �����
	BOOL InsertGroup(HTREEITEM,CString);
	BOOL DeleteGroup(HTREEITEM);
	void RenameGroup(HTREEITEM,CString);

	// ��Ա����
	void DeletePeer(HTREEITEM);
	void MovePeer(HTREEITEM sourGroup,HTREEITEM desGroup,HTREEITEM peer);
	*/
	//������
	void TreeTravesal(HTREEITEM hStartItem,BOOL bEvent);
	void ChangePrtItemAcdByChild( HTREEITEM hItemClicked,BOOL bEvent );



public:
	void SetService(CService*);
	void FilterTree(CPeerTreeDlg&);//����Ի��������ؼ������ɷ�ʽ�����֣�һ����BuildTree���ļ���ȡ���ݹ���������һ����FilterTree������һ���Ѵ��ڵ����ؼ�, ��������ֻ�������߽ڵ�
	
//	void FilterOffline();//ֻ�������ߵģ�ɾ�����ߵĽڵ�
	void SetTreeCheckable(BOOL bCheckable=TRUE);//�����Ƿ���и�ѡ��
	void GetCheckedPeerNames(list<CString>*, HTREEITEM root);//ע�⣺���Ƿ��ض����ռ��
	void GetCheckedPeerNames(list<CString>*);//����Ը������е�
	CString GetSelectedPeerName();


	afx_msg void OnNMRClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMovePeer();
	afx_msg void OnDelPeer();
	afx_msg void OnRenameGroup();
	afx_msg void OnInsertGroup();
	afx_msg void OnDelGroup();
	afx_msg void OnSetTreeCheckable();
	afx_msg LRESULT OnLogName(WPARAM,LPARAM);
	afx_msg LRESULT OnLogoutName(WPARAM,LPARAM);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
