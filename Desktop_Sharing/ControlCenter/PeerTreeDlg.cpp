// PeerTreeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PeerTreeDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "GroupNameDlg.h"
#include "Message.h"
#include "GroupChooseDlg.h"





// CPeerTreeDlg �Ի���

IMPLEMENT_DYNAMIC(CPeerTreeDlg, CDialogEx)

	CPeerTreeDlg::CPeerTreeDlg(CWnd* pParent /*=NULL*/, BOOL bFilterTree)
	: CDialogEx(CPeerTreeDlg::IDD, pParent)
{
	pService=NULL;
	pTreeData=NULL;
	hUnGroup=0;
	CString sTemp;
	if(sTemp.LoadString(IDS_PEERTREE_UNGROUP)){
		UNKNOWN_GROUP=sTemp;
	}
	else{
		UNKNOWN_GROUP=_T("δ����");
	}

	
	bTreeCheckable=FALSE;//��ʼΪ�Ǹ�ѡģʽ
	treeDataPath=_T("Group\\*.dat");
	this->bFilterTree=bFilterTree;
}

CPeerTreeDlg::~CPeerTreeDlg()
{
	//FreeTreeData();
}

void CPeerTreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PEER_TREE, m_PeerTree);
}

#define CHANGE_PARENT_ITEM_CHECK WM_USER+999

BEGIN_MESSAGE_MAP(CPeerTreeDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_PEER_TREE, &CPeerTreeDlg::OnNMRClickTree)
	ON_NOTIFY(NM_CLICK,IDC_PEER_TREE,&CPeerTreeDlg::OnNMClickTree)
	ON_COMMAND(IDM_MOVE_PEER,OnMovePeer)
	ON_COMMAND(IDM_DEL_PEER,OnDelPeer)
	ON_COMMAND(IDM_RENAME_GROUP,OnRenameGroup)
	ON_COMMAND(IDM_INSERT_GROUP,OnInsertGroup)
	ON_COMMAND(IDM_DEL_GROUP,OnDelGroup)
	ON_COMMAND(IDM_CHECKABLE,OnSetTreeCheckable)
	ON_MESSAGE(WM_MY_LOGNAME,OnLogName)
	ON_MESSAGE(WM_MY_SOCKCLOSE,OnLogoutName)

	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPeerTreeDlg ��Ϣ�������

void CPeerTreeDlg::SetService(CService* pService){
	this->pService=pService;
}


BOOL CPeerTreeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	InitTreeControl();


	return TRUE;
}


void CPeerTreeDlg::SetTreeCheckable(BOOL bCheckable){
	if(bCheckable){
		m_PeerTree.ModifyStyle(0,TVS_CHECKBOXES);
		bTreeCheckable=TRUE;
	}
	else{
		m_PeerTree.ModifyStyle(TVS_CHECKBOXES,0);
		bTreeCheckable=FALSE;
	}
}

void CPeerTreeDlg::GetCheckedPeerNames(list<CString>* pPeerList, HTREEITEM root){
	if(root==NULL || pPeerList==NULL ) return;	

	if(IsPeerItem(root) && m_PeerTree.GetCheck(root)) pPeerList->push_back(m_PeerTree.GetItemText(root));
	else {
		if(!m_PeerTree.ItemHasChildren(root)) return;
		HTREEITEM hItem=m_PeerTree.GetChildItem(root);
		while(hItem!=NULL){
			GetCheckedPeerNames(pPeerList,hItem);
			hItem=m_PeerTree.GetNextSiblingItem(hItem);
		}
	}
}

void CPeerTreeDlg::GetCheckedPeerNames(list<CString>* pPeerList){
	if(pPeerList==NULL) return;
	//	pPeerList->clear();//�����������Ҫ��������
	GetCheckedPeerNames(pPeerList,m_PeerTree.GetRootItem());
}

CString CPeerTreeDlg::GetSelectedPeerName(){
	CString peerName(_T(""));
	HTREEITEM hPeer=m_PeerTree.GetSelectedItem();

	if(hPeer && IsPeerItem(hPeer))
		peerName=m_PeerTree.GetItemText(hPeer);
	return peerName;
}



void CPeerTreeDlg::InitTreeControl(){
	m_ImageList.Create(16,16,ILC_COLOR32|ILC_MASK,0,1);
	m_ImageList.Add(LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_WORLD_ICON)));
	m_ImageList.Add(LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_GROUP_ICON)));
	m_ImageList.Add(LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_ONLINEPEER_ICON)));
	m_ImageList.Add(LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_OFFLINEPEER_ICON)));

	m_PeerTree.SetImageList(&m_ImageList,TVSIL_NORMAL);

	if(!bFilterTree) BuildTree();
}

// { �ļ�����

/* �������Ч�ʺܵͣ����ҿɿ���Ҳֵ�û��� */
BOOL CPeerTreeDlg::DeleteStringFromFile(CString str, CString fileTitle){//ɾ���ļ���ָ��һ��
	int count=treeDataPath.Find(_T("*.dat"));
	if(count<0) return FALSE;	
	CString fileName=treeDataPath.Left(count)+fileTitle+_T(".dat");
	CFile file;
	if(!file.Open(fileName,CFile::modeRead)) return FALSE;
	CString temp;
	CArchive arin(&file,CArchive::load);
	std::list<CString> peerList;
	while(arin.ReadString(temp)){
		if(temp!=str) peerList.push_back(temp);//���ļ�����ȫ�������ڴ棬����Ҫɾ����
	}
	arin.Close();
	file.Close();
	CFile::Remove(fileName);//ɾ��ԭ�ļ�

	if(!file.Open(fileName,CFile::modeCreate|CFile::modeWrite)){
		CString sCap,sText;
		sCap.LoadString(IDS_PEERTREE_CRIERROR);
		sText.LoadString(IDS_PEERTREE_DATALOST);
		MessageBox(sText/*_T("�������ݶ�ʧ��")*/,sCap/*_T("���ش���")*/);
		return FALSE;
	}
	CArchive arout(&file,CArchive::store);
	std::list<CString>::iterator pString=peerList.begin();
	while(pString!=peerList.end()){
		arout.WriteString(*pString+_T("\n"));
		pString++;
	}
	arout.Close();
	file.Close();

	return TRUE;
}

BOOL CPeerTreeDlg::AppendStringToFile(CString str, CString fileTitle){//un test
	int count=treeDataPath.Find(_T("*.dat"));
	if(count<0) return FALSE;

	CString fileName=treeDataPath.Left(count)+fileTitle+_T(".dat");
	CFile file;
	if(!file.Open(fileName,CFile::modeWrite)) return FALSE;
	file.SeekToEnd();
	CArchive ar(&file, CArchive::store);
	ar.WriteString(str+_T('\n'));
	ar.Close();
	file.Close();
	return TRUE;
}

BOOL CPeerTreeDlg::CreateGroupFile(CString fileTitle){
	int count=treeDataPath.Find(_T("*.dat"));
	if(count<0) return FALSE;	
	CString fileName=treeDataPath.Left(count)+fileTitle+_T(".dat");
	CFile file;
	if(!file.Open(fileName,CFile::modeCreate)) return FALSE;
	file.Close();
	return TRUE;
}


BOOL CPeerTreeDlg::DeleteGroupFile(CString fileTitle){
	int count=treeDataPath.Find(_T("*.dat"));
	if(count<0) return FALSE;	
	CString fileName=treeDataPath.Left(count)+fileTitle+_T(".dat");
	CFile::Remove(fileName);
	return TRUE;
}

BOOL CPeerTreeDlg::RenameGroupFile(CString fileTitle, CString newFileTitle){
	int count=treeDataPath.Find(_T("*.dat"));
	if(count<0) return FALSE;	
	CString fileName=treeDataPath.Left(count)+fileTitle+_T(".dat");
	CString newFileName=treeDataPath.Left(count)+newFileTitle+_T(".dat");

	CFile::Rename(fileName,newFileName);	

	return TRUE;
}
// } �ļ�����




// { ���ݹ���
void CPeerTreeDlg::BuildTree(){

	CString sAllPeers;
	sAllPeers=(sAllPeers.LoadString(IDS_PEERTREE_ALLPEERS)?sAllPeers:_T("���г�Ա"));
	HTREEITEM hRoot=m_PeerTree.InsertItem(sAllPeers,0,0);

	if(LoadTreeData() && pTreeData!=NULL) {

		TreeDataType::iterator pGroupPeers=pTreeData->begin();
		while(pGroupPeers!=pTreeData->end()){

			HTREEITEM hGroup=m_PeerTree.InsertItem(pGroupPeers->first,1,1,hRoot);//��

			PeerListType* pPeerList=pGroupPeers->second;

			PeerListType::iterator pPeer=pPeerList->begin();
			while(pPeer!=pPeerList->end()){
				m_PeerTree.InsertItem(*pPeer,3,3,hGroup);
				pPeer++;
			}

			pGroupPeers++;
		}

		FreeTreeData();
	}

	hUnGroup=m_PeerTree.InsertItem(UNKNOWN_GROUP,1,1,hRoot);//���δ����

}


BOOL CPeerTreeDlg::IsGroupItem(HTREEITEM hItem){
	if(m_PeerTree.GetParentItem(hItem)==m_PeerTree.GetRootItem()) return TRUE;
	return FALSE;
}


BOOL CPeerTreeDlg::IsPeerItem(HTREEITEM hItem){
	HTREEITEM hParentItem=m_PeerTree.GetParentItem(hItem);
	if(hParentItem!=NULL && hParentItem!=m_PeerTree.GetRootItem()) return TRUE;
	return FALSE;
}




BOOL CPeerTreeDlg::LoadTreeData(){
	TreeDataType* pGroupData=NULL;

	CFileFind find;
	CFile file;

	BOOL bWork=find.FindFile(treeDataPath);

	if(bWork) pGroupData=new TreeDataType;

	while(bWork){
		bWork=find.FindNextFileW();
		CString filePath=find.GetFilePath();

		PeerListType* pPeerList=NULL;

		if(file.Open(filePath,CFile::modeRead)){//��ʼ�����ļ��Ķ�ȡ
			pPeerList=new PeerListType;

			CArchive ar(&file,CArchive::load);

			CString peerName;
			while(ar.ReadString(peerName)){//��ʼ����ÿ�еĴ��� caution������δ����֤��������������������������
				pPeerList->push_back(peerName);
			}

			ar.Close();
			file.Close();
		}		


		pGroupData->insert(TreeDataType::value_type(find.GetFileTitle(),pPeerList));
	}

	find.Close();
	pTreeData= pGroupData;


	return !(pTreeData==NULL);	
}

void CPeerTreeDlg::FreeTreeData(){
	if(pTreeData==NULL) return;
	TreeDataType::iterator pGroupPeers=pTreeData->begin();
	while(pGroupPeers!=pTreeData->end()){

		PeerListType* pPeerList=pGroupPeers->second;
		delete pPeerList;
		pGroupPeers->second=NULL;

		pGroupPeers++;
	}

	delete pTreeData;
	pTreeData=NULL;
}




void CPeerTreeDlg::VerifyLogPeer(CString peerName){
	HTREEITEM hPeerItem=ItemStringToHandle(peerName,m_PeerTree.GetRootItem());
	if(hPeerItem)	
		m_PeerTree.SetItemImage(hPeerItem,2,2);
	else {		
		if(hUnGroup) m_PeerTree.InsertItem(peerName,2,2,hUnGroup);		
	}

	m_PeerTree.UpdateWindow();	
}



void CPeerTreeDlg::FilterTree(CPeerTreeDlg& dlg){
	CTreeCtrl& tree=dlg.m_PeerTree;
	HTREEITEM hRoot=tree.GetRootItem();
	if(!hRoot) return;
	HTREEITEM mhRoot=m_PeerTree.InsertItem(tree.GetItemText(hRoot),0,0);

	if(!tree.ItemHasChildren(hRoot)) return;

	int imageNum;
	HTREEITEM hGroup=tree.GetChildItem(hRoot);
	while(hGroup){
		HTREEITEM mhGroup=m_PeerTree.InsertItem(tree.GetItemText(hGroup),1,1,mhRoot);
		if(tree.ItemHasChildren(hGroup)){
			HTREEITEM hPeer=tree.GetChildItem(hGroup);
			while(hPeer){
				tree.GetItemImage(hPeer,imageNum,imageNum);
				if(imageNum==2){
					m_PeerTree.InsertItem(tree.GetItemText(hPeer),imageNum,imageNum,mhGroup);
				}
				hPeer=tree.GetNextSiblingItem(hPeer);
			}
		}

		hGroup=tree.GetNextSiblingItem(hGroup);
	}

}


/*

BOOL CPeerTreeDlg::GroupHasPeer(CString groupName, CString peerName){
TreeDataType::iterator pGroupPeers=pTreeData->find(groupName);
if(pGroupPeers==pTreeData->end()) return FALSE;
PeerListType* pPeerList=pGroupPeers->second;
if(pPeerList==NULL) return FALSE;
PeerListType::iterator pPeer=pPeerList->begin();
while(pPeer!=pPeerList->end()){
if(*pPeer==peerName) return TRUE;

pPeer++;
}
return FALSE;
}
*/

HTREEITEM CPeerTreeDlg::ItemStringToHandle(CString name, HTREEITEM root){
	if(root==NULL || name==_T("") ) return NULL;

	if(m_PeerTree.GetItemText(root)==name) return root;
	if(!(m_PeerTree.ItemHasChildren(root))) return NULL;
	HTREEITEM hItem=m_PeerTree.GetChildItem(root);
	while(hItem!=NULL){
		HTREEITEM hTemp=ItemStringToHandle(name,hItem);
		if(hTemp) return hTemp;

		hItem=m_PeerTree.GetNextSiblingItem(hItem);
	}
	return NULL;
}


//��������ǵ�Ч�ʵ��ڲ���δ����ʱ����Ϊ�����������һ��
HTREEITEM CPeerTreeDlg::GetGroupItem(CString groupName){
	HTREEITEM hRoot=m_PeerTree.GetRootItem();
	if(hRoot==NULL || !m_PeerTree.ItemHasChildren(hRoot)) return NULL;
	HTREEITEM hGroup=m_PeerTree.GetChildItem(hRoot);	

	while(hGroup){
		if(m_PeerTree.GetItemText(hGroup)==groupName) return hGroup;
		hGroup=m_PeerTree.GetNextSiblingItem(hGroup);
	}
	return NULL;
}


// } ���ݹ���



/*

// { �����:
BOOL CPeerTreeDlg::InsertGroup(HTREEITEM hItem,CString name){

if(!IsGroupItem(hItem)) return FALSE;

//�ļ����ڴ桢����
//	pService->CreateFile(name);
CString itemText=m_PeerTree.GetItemText(hItem);	

TreeDataType::iterator p=pTreeData->find(itemText);//where to insert
if(p==pTreeData->end()) return FALSE;
pTreeData->insert(p,TreeDataType::value_type(name,new PeerListType));

m_PeerTree.InsertItem(name,1,1,m_PeerTree.GetRootItem(),hItem);

return TRUE;
}

BOOL CPeerTreeDlg::DeleteGroup(HTREEITEM hGroup){
//�ļ����ڴ桢����
CString groupName=m_PeerTree.GetItemText(hGroup);
if(groupName==_T("δ����")) return FALSE;//δ������ɾ�������ģ�

//1.�ƶ���Ա��δ����
HTREEITEM hPeerItem=m_PeerTree.GetChildItem(hGroup);
while(hPeerItem){
this->MovePeer(hGroup,GetGroupItemByText(_T("δ����")),hPeerItem);
hPeerItem=m_PeerTree.GetNextSiblingItem(hPeerItem);
}

//pService->DeleteFile(m_PeerTree.GetItemText(hGroup));



m_PeerTree.DeleteItem(hGroup);
return TRUE;
}

void CPeerTreeDlg::RenameGroup(HTREEITEM hGroup,CString name){

}
*/

/* ע�⣺ ��������Ǳ��صģ�����FALSEʱ˵�����û��ͬ�����飬����TRUEʱ�������У�Ҳ�����ǳ���Bug. */
BOOL CPeerTreeDlg::HasGroup(CString groupName){
	HTREEITEM hRootItem=m_PeerTree.GetRootItem();
	if(hRootItem==NULL) return TRUE;
	HTREEITEM hItem=m_PeerTree.GetChildItem(hRootItem);
	if(hItem==NULL) return TRUE;
	while(hItem){
		if(m_PeerTree.GetItemText(hItem)==groupName) return TRUE;
		hItem=m_PeerTree.GetNextSiblingItem(hItem);
	}
	return FALSE;
}

/*
HTREEITEM CPeerTreeDlg::GetGroupItemByText(CString groupName){
HTREEITEM hRoot=m_PeerTree.GetRootItem();
if(hRoot==NULL) return NULL;
HTREEITEM hGroupItem=m_PeerTree.GetChildItem(hRoot);
while(hGroupItem){
if(m_PeerTree.GetItemText(hGroupItem)==groupName) return hGroupItem;
hGroupItem=m_PeerTree.GetNextSiblingItem(hGroupItem);
}
return NULL;
}
*/


// }�����




/*
// { ��Ա����
void CPeerTreeDlg::DeletePeer(HTREEITEM hPeer){
if(!IsPeerItem(hPeer)) return;
if(m_PeerTree.GetParentItem(hPeer)==hUnGroup) return;//δ����ĳ�Ա��ɾ������

//1.�ļ�����


//2.�������
int imageNum;
m_PeerTree.GetItemImage(hPeer,imageNum,imageNum);
if(imageNum==2){//�������ߵ�ͼ��

if(hUnGroup==NULL) return;
m_PeerTree.InsertItem(m_PeerTree.GetItemText(hPeer),imageNum,imageNum,hUnGroup);
}

m_PeerTree.DeleteItem(hPeer);

}

void CPeerTreeDlg::MovePeer(HTREEITEM hSourGroup,HTREEITEM hDesGroup,HTREEITEM hPeer){
if(hSourGroup==hDesGroup) return;
if(!IsGroupItem(hSourGroup) || !IsGroupItem(hDesGroup) || !IsPeerItem(hPeer) ) return;

//1.�ļ�����

//2.�������
int imageNum;
m_PeerTree.GetItemImage(hPeer,imageNum,imageNum);
m_PeerTree.InsertItem(m_PeerTree.GetItemText(hPeer),imageNum,imageNum,hDesGroup);
m_PeerTree.DeleteItem(hPeer);
}
// } ��Ա����
*/


void CPeerTreeDlg::OnNMRClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CPoint cp;
	GetCursorPos(&cp);
	m_PeerTree.ScreenToClient(&cp);
	HTREEITEM hItem=m_PeerTree.HitTest(cp);

	CMenu menu;
	menu.LoadMenuW(IDR_TREE_MENU);
	CMenu* pMenu=NULL;

	if(hItem){//�����ڸ���Ŀ���һ��������������ڿհ״��һ���hItemΪNULL��


		m_PeerTree.SelectItem(hItem);
		m_PeerTree.ClientToScreen(&cp);

		if(IsPeerItem(hItem)){
			pMenu=menu.GetSubMenu(0);
		}
		else if(IsGroupItem(hItem)){
			pMenu=menu.GetSubMenu(1);
		}	

	}
	/*
	else {
	m_PeerTree.ClientToScreen(&cp);
	pMenu=menu.GetSubMenu(2);
	if(pMenu){
	if(bTreeCheckable) pMenu->CheckMenuItem(IDM_CHECKABLE,MF_CHECKED);
	else pMenu->CheckMenuItem(IDM_CHECKABLE,MF_UNCHECKED);
	}		
	}
	*/

	if(pMenu) pMenu->TrackPopupMenu(0,cp.x,cp.y,this);

	*pResult = 0;
}

void CPeerTreeDlg::OnMovePeer(){
	HTREEITEM hPeerItem=m_PeerTree.GetSelectedItem();
	HTREEITEM hSourGroup=m_PeerTree.GetParentItem(hPeerItem);
	CString sourGroupName=m_PeerTree.GetItemText(hSourGroup);
	CString desGroupName;

	std::list<CString> groupList;
	HTREEITEM hGroup=m_PeerTree.GetChildItem(m_PeerTree.GetRootItem());//�����ȥ�˺ܶలȫ���
	while(hGroup){
		groupList.push_back(m_PeerTree.GetItemText(hGroup));
		hGroup=m_PeerTree.GetNextSiblingItem(hGroup);
	}
	CGroupChooseDlg dlg(&desGroupName,&groupList);
	if(dlg.DoModal()!=IDOK || desGroupName==_T("")) return;

	if(sourGroupName==desGroupName) return;

	HTREEITEM hDesGroup=GetGroupItem(desGroupName);
	CString peerName=m_PeerTree.GetItemText(hPeerItem);
	int imageNum;
	m_PeerTree.GetItemImage(hPeerItem,imageNum,imageNum);

	if(sourGroupName==UNKNOWN_GROUP){
		AppendStringToFile(peerName,desGroupName);//�ļ�			
	}
	else if(desGroupName==UNKNOWN_GROUP){
		DeleteStringFromFile(peerName,sourGroupName);		
	}
	else{
		DeleteStringFromFile(peerName,sourGroupName);
		AppendStringToFile(peerName,desGroupName);
	}
	m_PeerTree.DeleteItem(hPeerItem);
	m_PeerTree.InsertItem(peerName,imageNum,imageNum,hDesGroup);

	m_PeerTree.UpdateWindow();
}



void CPeerTreeDlg::OnDelPeer(){
	HTREEITEM hItem=m_PeerTree.GetSelectedItem();
	HTREEITEM hParentItem=m_PeerTree.GetParentItem(hItem);
	int imageNum;
	m_PeerTree.GetItemImage(hItem,imageNum,imageNum);

	if(imageNum==3){
		if(hParentItem!=hUnGroup)
			DeleteStringFromFile(m_PeerTree.GetItemText(hItem),m_PeerTree.GetItemText(hParentItem));

		m_PeerTree.DeleteItem(hItem);
	}
	else if(imageNum==2){
		if(hParentItem!=hUnGroup){
			DeleteStringFromFile(m_PeerTree.GetItemText(hItem),m_PeerTree.GetItemText(hParentItem));			
			m_PeerTree.InsertItem(m_PeerTree.GetItemText(hItem),imageNum,imageNum,hUnGroup);
			m_PeerTree.DeleteItem(hItem);
		}
	}	
}

void CPeerTreeDlg::OnRenameGroup(){
	CString groupName;
	CGroupNameDlg dlg(&groupName);
	if(dlg.DoModal()!=IDOK) return;
	if(HasGroup(groupName)){
		CString sText,sCap;
		sText.LoadString(IDS_PEERTREE_GROUPREPEAT);
		sCap.LoadString(IDS_PEERTREE_TIP);
		//MessageBox(_T("�Ѿ���������Ƶķ��飬��һ�����ơ�"),_T("��ʾ"));
		MessageBox(sText,sCap);
		return;
	}
	HTREEITEM hItem=m_PeerTree.GetSelectedItem();
	if(hItem==NULL) return;
	RenameGroupFile(m_PeerTree.GetItemText(hItem),groupName);
	m_PeerTree.SetItemText(hItem,groupName);
}

void CPeerTreeDlg::OnInsertGroup(){
	CString groupName;
	CGroupNameDlg dlg(&groupName);
	if(dlg.DoModal()!=IDOK) return;
	if(HasGroup(groupName)){
		CString sText,sCap;
		sText.LoadString(IDS_PEERTREE_GROUPREPEAT);
		sCap.LoadString(IDS_PEERTREE_TIP);
		//MessageBox(_T("�Ѿ�����������Ƶķ��飬��һ����"),_T("��ʾ"));
		MessageBox(sText,sCap);
		return;
	}
	HTREEITEM hItem=m_PeerTree.GetSelectedItem();
	if(hItem==NULL) return;
	CreateGroupFile(groupName);
	m_PeerTree.InsertItem(groupName,1,1,m_PeerTree.GetParentItem(hItem),hItem);
}

void CPeerTreeDlg::OnDelGroup(){
	HTREEITEM hItem=m_PeerTree.GetSelectedItem();
	if(hItem==NULL || hItem==hUnGroup) return ;
	if(!m_PeerTree.ItemHasChildren(hItem)){
		DeleteGroupFile(m_PeerTree.GetItemText(hItem));
		m_PeerTree.DeleteItem(hItem);
		return;
	}
	HTREEITEM hPeer=m_PeerTree.GetChildItem(hItem);
	CString groupName=m_PeerTree.GetItemText(hItem);
	int imageNum;
	HTREEITEM hTemp;
	while(hPeer!=NULL){
		m_PeerTree.GetItemImage(hPeer,imageNum,imageNum);
		m_PeerTree.InsertItem(m_PeerTree.GetItemText(hPeer),imageNum,imageNum,hUnGroup);
		hTemp=m_PeerTree.GetNextSiblingItem(hPeer);
		DeleteStringFromFile(m_PeerTree.GetItemText(hPeer),groupName);
		m_PeerTree.DeleteItem(hPeer);
		hPeer=hTemp;
	}
	DeleteGroupFile(groupName);
	m_PeerTree.DeleteItem(hItem);
}

void CPeerTreeDlg::OnSetTreeCheckable(){
	if(bTreeCheckable){
		SetTreeCheckable(FALSE);
	}
	else{
		SetTreeCheckable(TRUE);
	}
}

LRESULT CPeerTreeDlg::OnLogName(WPARAM wParam, LPARAM lParam){

	CString peerName=*(CString*)lParam;
	//	MessageBox(peerName);
	VerifyLogPeer(peerName);
	return 1;
}

LRESULT CPeerTreeDlg::OnLogoutName(WPARAM wParam, LPARAM lParam){
	CString peerName=*(CString*)wParam;
	HTREEITEM hPeer=ItemStringToHandle(peerName,m_PeerTree.GetRootItem());
	m_PeerTree.SetItemImage(hPeer,3,3);
	return 1;
}

void CPeerTreeDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	static	CRect rt;
	if(m_PeerTree.m_hWnd){
		GetClientRect(&rt);
		m_PeerTree.MoveWindow(&rt);
	}


}


// { ������������Ϊ��ʵ��checkʱ�ڵ������



void CPeerTreeDlg::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPoint   pt; 
	::GetCursorPos(&pt); 
	UINT   uFlags=0; 

	m_PeerTree.ScreenToClient(&pt);

	HTREEITEM   hItem   = m_PeerTree.HitTest(pt,&uFlags); 
	if   (hItem!=NULL) 
	{ 
		if(   uFlags   &   TVHT_ONITEMSTATEICON   ) 
		{ 
			// theFirstStartItem = false;
			//m_bSetCheckOrChecked = GetCheck(hItem);
			//�����������ӽ�㶼��setcheck
			TreeTravesal(hItem,TRUE);
			// ��鸸����Ƿ���Ҫ�仯
			ChangePrtItemAcdByChild(hItem,TRUE);


		} 
	} 
	*pResult = 0;
}




// ����һ�����������ӽ�㣬��������Ϊѡ��״̬
//by Xue 20.09 2011
void CPeerTreeDlg::TreeTravesal(HTREEITEM hStartItem,BOOL bEvent)
{
	if(!m_PeerTree.ItemHasChildren(hStartItem)) return;
	//�õ����ӽ��
	HTREEITEM hChildItem = m_PeerTree.GetChildItem(hStartItem);
	BOOL bChecked=(bEvent?!m_PeerTree.GetCheck(hStartItem):m_PeerTree.GetCheck(hStartItem));
	bEvent=FALSE;
	//�����ӽ����б���
	while(hChildItem!=NULL)
	{
		if (bChecked)
		{
			m_PeerTree.SetCheck(hChildItem);
		}
		else
		{
			m_PeerTree.SetCheck(hChildItem,FALSE);
		}

		TreeTravesal(hChildItem,bEvent); //�ݹ�������ӽڵ�   
		//�����ֵܽ����б���
		hChildItem =m_PeerTree.GetNextSiblingItem(hChildItem);   
	}   
}


void CPeerTreeDlg::ChangePrtItemAcdByChild( HTREEITEM hItemClicked, BOOL bEvent )
{
	// �õ����ڵ�
	HTREEITEM hParent = m_PeerTree.GetParentItem(hItemClicked);
	if(hParent==NULL) return;
	BOOL bItemChecked=(bEvent?!m_PeerTree.GetCheck(hItemClicked):m_PeerTree.GetCheck(hItemClicked));
	bEvent=FALSE;



	// ��¼������״̬     
	bool bParentIsChecked = true;

	// ��鸸���������ӽ��
	HTREEITEM hChild =m_PeerTree.GetNextItem(hParent,TVGN_CHILD);
	BOOL bTemp;
	while(NULL != hChild)
	{
		bTemp=(hChild==hItemClicked?bItemChecked:m_PeerTree.GetCheck(hChild));
		if(bTemp==FALSE){
			bParentIsChecked=false;
			break;
		}
		hChild = m_PeerTree.GetNextSiblingItem(hChild);
	}

	// ���ø�����״̬
	m_PeerTree.SetCheck(hParent,bParentIsChecked);

	// ����ϼ��ڵ�
	ChangePrtItemAcdByChild(hParent,bEvent);

}

// } ������������Ϊ��ʵ�ֽڵ�����