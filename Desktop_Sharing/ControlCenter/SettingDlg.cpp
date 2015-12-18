// SettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SettingDlg.h"
#include "afxdialogex.h"


// CSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingDlg::IDD, pParent)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WLIST, m_wlist);
	DDX_Control(pDX, IDC_BLIST, m_blist);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
END_MESSAGE_MAP()


// CSettingDlg ��Ϣ�������




// ��ȡϵͳ��װ�����б�   
BOOL CSettingDlg::AllRegKey(HKEY hKey, LPCTSTR szDesKeyItem)   
{   
     HKEY hSubKey;   
     DWORD dwIndex = 0;   
     long lResult;   
     int i = 1;   
    
     lResult = RegOpenKeyEx(hKey, szDesKeyItem, 0, KEY_READ, &hSubKey);      // ��ע���   
     if(ERROR_SUCCESS == lResult) {   
         TCHAR szKeyName[256] = { 0 };   
         DWORD cbName = 256*sizeof(TCHAR);

		 TCHAR s_name[256] = {0};        // �����            
		 TCHAR s_DisplayName[256] = {0};         // ��ʾ���� 
		 DWORD typeSZ = REG_SZ;          // SZ ע������� 

    
         lResult = RegEnumKeyEx(hSubKey, dwIndex, szKeyName, &cbName, 0, NULL, NULL, NULL);      // ��ȡ�ֶ�   
         while (lResult == ERROR_SUCCESS && lResult != ERROR_MORE_DATA){   
             OutputDebugString(szKeyName);   
    
			
			m_wlist.InsertItem(i++,szKeyName);
			 


			 HKEY hItem;     // ��һ��ע�����  
			 if(::RegOpenKeyEx(hSubKey, s_name, 0, KEY_READ, &hItem) == ERROR_SUCCESS){      // ��ȡ��һ��ע�������������Ϣ  
                if (::RegQueryValueEx(hItem, _T("DisplayName"), 0, &typeSZ, (LPBYTE)s_DisplayName, &cbName) == ERROR_SUCCESS) {  
                   // m_wlist.SetItemText(i-1,0,s_DisplayName);
				
                }
				      
                ::RegCloseKey(hItem);   // �رն���ע�������������Ϣ��  
            }  
  
    
		
    
             dwIndex = dwIndex + 1;          // �ı�dwIndex��ѭ����ȡ�����ֶ�   
             cbName = 256*sizeof(TCHAR);   
             lResult = RegEnumKeyEx(hSubKey, dwIndex, szKeyName, &cbName, 0, NULL, NULL, NULL);   
    
            
    
         }   
         RegCloseKey(hSubKey);       // �ر�ע���   
         return TRUE;   
     }   
     RegCloseKey(hSubKey);   
     return FALSE;   
 }   




/*

BOOL CSettingDlg::AllRegKey(HKEY hKey, LPCTSTR szDesKeyItem)  
{  
    HKEY hSubKey;  
    DWORD dwIndex = 0;  
    long lResult;  
  
    lResult = RegOpenKeyEx(hKey, szDesKeyItem, 0, KEY_READ, &hSubKey);      // ��ע���  
    if(ERROR_SUCCESS == lResult) {  
        int id = 1;                     // ���  
        TCHAR s_name[256] = {0};        // �����            
        TCHAR s_DisplayName[256] = {0};         // ��ʾ����  
        TCHAR s_Publisher[256] = {0};           // ������  
        TCHAR s_InstallDate[12] = {0};          // ��װ����  
        DWORD s_EstimatedSize;                  // ��С  
        TCHAR s_DisplayVersion[256] = { 0 };    // �汾  
        DWORD cbName = 256*sizeof(TCHAR);     
        DWORD cbName2 = sizeof(TCHAR);  
  
        DWORD typeSZ = REG_SZ;          // SZ ע�������  
        DWORD typeDWORD = REG_DWORD;    // DWORD ע�������  
  
        lResult = RegEnumKeyEx(hSubKey, dwIndex, s_name, &cbName, 0, NULL, NULL, NULL);     // ��ȡ�ֶ�  
        while (lResult == ERROR_SUCCESS && lResult != ERROR_MORE_DATA){  
            OutputDebugString(s_name);  
  
            CString str;  
            str.Format(_T("%d"), id);  
  
            id = m_blist.InsertItem(id, str);        // ���뵽List�ؼ���ʾ  
            m_blist.SetItemText(id, 0, s_name);  
  
  
			
            HKEY hItem;     // ��һ��ע�����  
            if(::RegOpenKeyEx(hSubKey, s_name, 0, KEY_READ, &hItem) == ERROR_SUCCESS){      // ��ȡ��һ��ע�������������Ϣ  
                if (::RegQueryValueEx(hItem, _T("DisplayName"), 0, &typeSZ, (LPBYTE)s_DisplayName, &cbName) == ERROR_SUCCESS) {  
                    m_blist.SetItemText(id, 1, s_DisplayName);  
				
                }  
  
                if (::RegQueryValueEx(hItem, _T("Publisher"), 0, &typeSZ, (LPBYTE)s_Publisher, &cbName) == ERROR_SUCCESS) {  
                    m_blist.SetItemText(id, 2, s_Publisher);  
                }  
  
                if (::RegQueryValueEx(hItem, _T("InstallDate"), 0, &typeSZ, (LPBYTE)s_InstallDate, &cbName) == ERROR_SUCCESS) {  
                    if (s_InstallDate != NULL) {        // �ж��Ƿ�Ϊ��  
                        m_blist.SetItemText(id, 3, s_InstallDate);         
                    }  
                }  
  
                if (::RegQueryValueEx(hItem, _T("EstimatedSize"), 0, &typeDWORD, (LPBYTE)&s_EstimatedSize, &cbName2) == ERROR_SUCCESS) {  
                    float f_Size = s_EstimatedSize/1024.0;  // DWORD ת��Ϊ CString ����  
                    CString p_Size;  
                    p_Size.Format(_T("%.2f M"), f_Size);  
                      
                    m_blist.SetItemText(id, 4, p_Size);  
                }  
  
                if (::RegQueryValueEx(hItem, _T("DisplayVersion"), 0, &typeSZ, (LPBYTE)s_DisplayVersion, &cbName) == ERROR_SUCCESS) {  
                    m_blist.SetItemText(id, 5, s_DisplayVersion);  
                }  
                  
                ::RegCloseKey(hItem);   // �رն���ע�������������Ϣ��  
            }  
		
			
  
            dwIndex = dwIndex + 1;          // �ı�dwIndex��ѭ����ȡ�����ֶ�  
            cbName = 256*sizeof(TCHAR);  
            lResult = RegEnumKeyEx(hSubKey, dwIndex, s_name, &cbName, 0, NULL, NULL, NULL);  
  
            id++;  
  
        }  
        RegCloseKey(hSubKey);   // �ر�һ��ע�����ȡ������Ϣ��  
        return TRUE;  
    }  
    RegCloseKey(hSubKey);  
    return FALSE;  
}  

*/



BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_wlist.InsertColumn(1,_T("��������"));
	m_blist.InsertColumn(1,_T("��������"));//��û��


	AllRegKey(HKEY_LOCAL_MACHINE, TEXT("SoftWare\\Microsoft\\Windows\\CurrentVersion\\Uninstall"));  

	return TRUE;
}
