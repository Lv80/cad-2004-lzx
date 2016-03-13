// (C) Copyright 1990-2002 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- StdAfx.h : include file for standard system include files,
//-----      or project specific include files that are used frequently,
//-----      but are changed infrequently
//-----------------------------------------------------------------------------
#pragma once

// -----------------------------------------------------------------------------
// #ifdef _FULLDEBUG_
// #pragma message ("Warning! You should not run this application in a Standard AutoCAD release.")
// #pragma message ("         Due to the memory managment constraints.")
// #endif

#pragma warning(disable: 4786)
//#pragma warning(disable: 4098)

//-----------------------------------------------------------------------------
#define STRICT

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN			//----- Exclude rarely-used stuff from Windows headers
#endif

//----- Modify the following defines if you have to target a platform prior to the ones specified below.
//----- Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER					//----- Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501			//----- Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT			//----- Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501		//----- Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS			//----- Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0500	//----- Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE				//----- Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0600		//----- Change this to the appropriate value to target IE 5.0 or later.
#endif

//-----------------------------------------------------------------------------
//----- 'DEBUG workaround' below prevents the MFC or ATL #include-s from pulling 
//----- in "Afx.h" that would force the debug CRT through #pragma-s.
// #if defined(_DEBUG) && !defined(_FULLDEBUG_)
// #define _DEBUG_WAS_DEFINED
// #undef _DEBUG
// #pragma message ("     Compiling MFC / STL / ATL header files in release mode.")
// #endif

//-----------------------------------------------------------------------------
#include <afxwin.h>				//----- MFC core and standard components
#include <afxext.h>				//----- MFC extensions

//#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>				//----- MFC OLE classes
#include <afxodlgs.h>			//----- MFC OLE dialog classes
#include <afxdisp.h>			//----- MFC Automation classes
//#endif //----- _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>				//----- MFC ODBC database classes
#endif //----- _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>				//----- MFC DAO database classes
#endif //----- _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>			//----- MFC support for Internet Explorer 4 Common Controls
//#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>				//----- MFC support for Windows Common Controls
//#endif //----- _AFX_NO_AFXCMN_SUPPORT

//-----------------------------------------------------------------------------
// #ifdef _DEBUG_WAS_DEFINED
// #define _DEBUG
// #undef _DEBUG_WAS_DEFINED
// #endif

//-----------------------------------------------------------------------------
//----- Include ObjectDBX/ObjectARX headers
//----- Uncomment one of the following lines to bring a given library in your project.
//#define _BREP_SUPPORT_			//- Support for the BRep API
//#define _HLR_SUPPORT_				//- Support for the Hidden Line Removal API
//#define _IE_SDK_SUPPORT_			//- Support for the Autodesk Image Engine API
//#define _AMODELER_SUPPORT_		//- Support for the AModeler API
//#define _ASE_SUPPORT_				//- Support for the ASI/ASE API
//#define _RENDER_SUPPORT_			//- Support for the AutoCAD Render API
//#define _ARX_CUSTOM_DRAG_N_DROP_	//- Support for the ObjectARX Drag'n Drop API
//#define _INC_LEAGACY_HEADERS_		//- Include legacy headers in this project
#include "arxHeaders.h"
//-----------------------------------------------------------------------------
#include "DocData.h"			//----- Your document specific data

//----- Declare it as an extern here so that it becomes available in all modules
extern AcApDataManager<CDocData> DocVars ;

//////////////////////////////////////////////////////////////////////////
//begin zhf modify
#define VER_FOR_2002 //���ڴ���cad2002���ϵİ汾�����ã�����ֻ��cad2002���ϰ汾���н�������֧��

typedef CArray<long, long> CLongArray;

#include "../blkdll/BlkUtitily.h"
// #include "../common/DBUtility.h"
#include "../blkuserdll/UserSystem.h"
#include "resource.h"
#include <atlbase.h>
#include <complex>

extern BOOL bFirst ;
extern CString g_strBasePath ;


#define PI 3.14159265358979323846

 #import "c:\\program files\\common files\\system\\ado\\msado21.tlb" rename_namespace("EasyAdoNS") rename ( "EOF", "EndOfFile" )
using namespace EasyAdoNS;

#include "../blkdll/MyAdo.h"

/*
//for office2000
#import "c:\\Program Files\\Microsoft Office\\Office\\mso9.dll" rename_namespace("Office")
using namespace Office;

#import "c:\\Program Files\\Common Files\\Microsoft Shared\\VBA\\VBA6\\VBE6EXT.olb" rename_namespace("VBE6")
using namespace VBE6;

// Excel�����
#import "c:\\Program Files\\Microsoft Office\\Office\\EXCEL9.olb" rename("RGB","RGBEx"),rename("DialogBox","DialogBoxEx"),named_guids,rename_namespace("MSExcel")
using namespace MSExcel;
*/

#ifndef IMPORTOFFICE
//for office2003

#import "C:\\Program Files\\Common Files\\Microsoft Shared\\OFFICE11\\mso.dll" rename_namespace("Office")
using namespace Office;

#import "c:\\Program Files\\Common Files\\Microsoft Shared\\VBA\\VBA6\\VBE6EXT.olb" rename_namespace("VBE6")
using namespace VBE6;

// Excel�����
#import "d:\\Program Files\\Microsoft Office\\OFFICE11\\EXCEL.EXE" rename("RGB","RGBEx"),rename("DialogBox","DialogBoxEx"),named_guids,rename_namespace("MSExcel")
using namespace MSExcel;

#define IMPORTOFFICE
#endif

extern CUserSystem g_cSystem ; 
extern AcDbDatabase * g_pCurDb  ;
extern	_ConnectionPtr g_PtrCon ;


extern CString g_strMaXu_1[9] ;
extern CString g_strMaXu_CeXiang_1[8] ;
extern CString g_strMaXu_CeXiang2_1[8] ;
extern CString g_strMaXu_YinDao_1[8] ;
extern CString g_strLinZhanState_1[9] ;
extern CString g_strFreq_Down_1[4] ;
extern CString g_strFreq_Up_1[4] ;



BOOL InitSystem() ;
//CString GetErrorDescription(_com_error& e) ;
BOOL ExecSqlString(CString strSql) ;
void Find(CString strStart, CString strEnd, CStringArray &strArray, CArray<CStringArray *, CStringArray *> &arrayJinLu) ;
BOOL GetGuiDaoBlockPosition(CString strTag, CString strValue, AcGePoint3d &pt_3d, AcGePoint3d &pt_3d_attribute ) ;

int getFile(const TCHAR* pCmdAsk, const TCHAR* pDiaAsk,const TCHAR* pExt,TCHAR* pResult) ;


//end zhf modify
//////////////////////////////////////////////////////////////////////////
