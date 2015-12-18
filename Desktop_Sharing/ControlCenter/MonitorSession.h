#pragma once
#include "afx.h"
#include "MonitorViewerDlg.h"

// CMonitorSession��
// ���ڷ�װ�����ػỰ���࣬���ڴ�CNewMonitorSessionDlg�Ի��������ݣ���������ض����RDP����

typedef std::list<CString> MonitorListType;


class CMonitorSession :
	public CObject
{
public:
	CMonitorSession(void);
	~CMonitorSession(void);

private:
	BOOL bStateOn;//not used
	MonitorListType monitorList;
//	CMonitorViewerDlg* pMonitorViewerDlg;

public:
	void StartMonitors(int startIndex=0);
	void StartMonitors(MonitorListType::iterator startPointer);

	MonitorListType & GetMonitorList(){ return monitorList;}

};

