#pragma once

#include "resource.h"

// CFullScreenOpDlg �Ի���
// �ڽ�����ȫ���ۿ���ʱ����������Ի����˳���
// ����Ի���ͣ������Ļ�ϱ�Եƫ��࣬��ǰʵ���ǶԻ�����ֻ��һ���˳���ť

//Ҫ�ɹ���ʹ����������������㣺�����˸����ڣ���������Ϣ


class CFullScreenOpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFullScreenOpDlg)

public:
	CFullScreenOpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFullScreenOpDlg();

// �Ի�������
	enum { IDD = IDD_FULLSCREENOP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


private:

	int nShowPosX;//����Ի���Ӧ�ó��ֵ�xλ��
	UINT nExitMessage;//�������˳���ťʱ�򸸴��ڷ�������Զ�����Ϣ�������ϢӦ���ɸ��������á�

public:
	void SetExitMessage(UINT msg){nExitMessage=msg;}


	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedExitfullscreenButton();
};
