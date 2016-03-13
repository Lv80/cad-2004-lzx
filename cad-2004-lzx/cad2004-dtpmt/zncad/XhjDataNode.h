#pragma once
#include "StdAfx.h"
#include "../blkuserdll/UserSystem.h"
#include "../blkdll/MyAdo.h"

class CXhjDataNode
{
public:
	CXhjDataNode(void);
	CXhjDataNode(long nHandleXhj);
	CUserSystem m_sys ;
	CADODatabase m_AdoDb ;
	CADORecordset m_RecordSet ;
	long m_nHandleXhj;//�źŻ����
	CString m_strXhjName;//�źŻ�����
	CString m_strBlockName;//�źŻ�����
	long m_nHandleXhjLeftLine;//�źŻ����������
	long m_nHandleXhjRightLine;//�źŻ��Ҳ�������
	double m_dXhj_x;
	int m_iToward;
	virtual ~CXhjDataNode(void);
	int GetXhjConLineRight( long nHandleXhj,long &nHandleLine);
	int GetXhjConLineLeft( long nHandleXhj,long &nHandleLine);
	void GetEntBlkNameByHandle( const long nHandle,CString &str_ent_name);
	BOOL GetBlockPosition_X( long nHandle,double &point_x );
	BOOL GetBlockToward( long nHandleXHJ ,int &iToward);
};