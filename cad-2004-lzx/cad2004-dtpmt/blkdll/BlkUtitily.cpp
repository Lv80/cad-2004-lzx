#include "StdAfx.h"
#include "blkutitily.h"

CBlkUtility::CBlkUtility(void)
{
}

CBlkUtility::~CBlkUtility(void)
{
}

CString CBlkUtility::GetErrorDescription(_com_error& e)
{
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	_TCHAR szTemp[1024];

	CString strInfo ;
	wsprintf(szTemp, _T("Message : %s\n"), e.ErrorMessage());
	strInfo = szTemp;
	wsprintf(szTemp, _T("Code : 0x%08lx\n"), e.Error());
	strInfo += szTemp;
	wsprintf(szTemp, _T("Source : %s\n"), bstrSource.length() ? (LPCTSTR)bstrSource : _T("null"));
	strInfo += szTemp;
	wsprintf(szTemp, _T("Description : %s\n"), bstrDescription.length() ? (LPCTSTR)bstrDescription : _T("null"));
	strInfo += szTemp;
	return strInfo;
}

BOOL CBlkUtility::IsStrMatch(const CString strReg, const CString strToCheck)
{
	BOOL bRet = FALSE ;

	match_results results;
	//std::string strToCheck("");
	//std::string strReg("");
	_tstring sreg = (LPCTSTR)strReg ;
	_tstring stocheck = (LPCTSTR)strToCheck ;

	// 	TCHAR chToCheck[128] ;
	// 	_tcscpy_s(chToCheck, strToCheck) ;
	// 	//strToCheck = chToCheck ;
	// 	
	// 	TCHAR chReg[128] ;
	// 	_tcscpy_s(chReg, strReg) ;
	// 	//strReg = chReg ;

	rpattern pat(sreg);  
	// Match a dollar sign followed by one or more digits,
	// optionally followed by a period and two more digits.
	// The double-escapes are necessary to satisfy the compiler.
	match_results::backref_type br = pat.match( stocheck, results );
	if( br.matched ) 
	{
		bRet = TRUE ;
	} 

	return bRet ;
}

int CBlkUtility::RegReplace(const CString strReg, const CString strSub, const CString strSrc, CString &strResult)
{
	REGEX_FLAGS dw = GLOBAL | ALLBACKREFS;
	// 	if( m_bCase ) dw |= NOCASE;
	// 	if( m_bMulti ) dw |= MULTILINE;
	// 	if( m_bSingle ) dw |= SINGLELINE;
	// 	double tmS = clock();
	//
	rpattern pat((LPCTSTR)strReg, (LPCTSTR)strSub, dw);
	subst_results subResult;

	_tstring str((LPCTSTR)strSrc);
	int nCount = pat.substitute(str, subResult);
	strResult = str.c_str();

	return 0 ;
}

//罗马字符转成数字，这里罗马字符只是使用普通字符拼凑的形式：I IV
BOOL CBlkUtility::RomanToDig(const CString strRoman, CString &strDig)
{
	BOOL bRet = FALSE ;
	CString strTmp[10] = {_T("I"),_T("II"),_T("III"),_T("IV"),_T("V"),_T("VI"),_T("VII"),_T("VIII"),_T("IX"),_T("X")} ;
	for (int i=0; i<10; i++)
	{
		if (strRoman==strTmp[i])
		{
			if(i==9)
			{
				strDig.Format(_T("10")) ;
			}
			else
			{
				strDig.Format(_T("%d"), i+1) ;
			}
			bRet = TRUE ;
			break ;
		}		
	}	
	return bRet ;
}

//数字转成罗马字符，默认itype=0，转换成非百分号形式，itype=1，转换成%%161形式(10以后字体不支持用非百分号代替)
BOOL CBlkUtility::DigToRoman(int iDig, CString &strRoman, int iType)
{

	BOOL bRet = TRUE ;
	if (iDig>15)
	{
		return FALSE ;
	}

	CString strTmp[2][15] = {{_T("I"),_T("II"),_T("III"),_T("IV"),_T("V"),_T("VI"),_T("VII"),_T("VIII"),_T("IX"),_T("X"), _T("XI"), _T("XII"), _T("XII"), _T("XIV"), _T("XV")},
		{_T("%%161"),_T("%%162"),_T("%%163"),_T("%%164"),_T("%%165"),_T("%%166"),_T("%%167"),_T("%%168"),_T("%%169"),_T("%%170"), _T("XI"), _T("XII"), _T("XII"), _T("XIV"), _T("XV")} 
	};

	strRoman = strTmp[iType][iDig-1] ;

	return bRet ;
}

//将类似DIK110+111形式的字符串转换为类似110.111的数值
BOOL CBlkUtility::ConvertStrToDigital(CString strToConv, double &dDigital)
{
	BOOL bRet = TRUE ;	

	int iPlusLoc = strToConv.Find(_T("+")) ;
	CString strBefPlus, strAfterPlus ;
	strBefPlus = strToConv.Left(iPlusLoc) ;
	strAfterPlus = strToConv.Mid(iPlusLoc+1) ;

	int iNumStartLoc = -1 ;
	for(int i=iPlusLoc-1;i>=0; i--)
	{
		TCHAR cc =  strBefPlus.GetAt(i) ;
		if((int)cc>=48 && (int)cc<=57)
		{
			continue ;
		}
		else
		{
			iNumStartLoc = i ;
			break ;
		}
	}
	if (iNumStartLoc == -1)
	{
		bRet = FALSE ;
		return bRet ;
	}

	CString strDigital ;
	strDigital.Format(_T("%s%s"),strBefPlus.Mid(iNumStartLoc+1), strAfterPlus ) ;
	dDigital = _tstof(strDigital) ; 
	if (dDigital==0.0)
	{
		bRet = FALSE ;
	}

	return bRet ;
}

bool CBlkUtility::Compute_Pre_Bh(CString strBh, CString &strPre, int &iBh)
{
	bool bChange = true;
	int iLength=strBh.GetLength();
	int i=iLength;
	if (_tcscmp(strBh,_T(""))==0)
	{
		strPre=_T("");
		iBh=0;
		return false;
	}
	else 
	{
		while(bChange)
		{
			if (i >= 0)
			{
				iBh= _tstoi(strBh.Right(i));
				strPre=strBh.Left(iLength-i);
				if (iBh<=0)
				{
					i=i-1;
				}
				else
				{
					CString strTempBh;
					int iTempBh;
					strTempBh.Format(_T("%d"),iBh);
					iTempBh=strTempBh.GetLength();
					if (iTempBh!=i)
					{
						i=i-1;
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
	}
	return true ;
}

CString CBlkUtility::GetExcelFieldName(int iNum)
{
	CString strReturn ;
	//1~26 分别对应A~Z ;
	TCHAR chPre[2] , chAft[2]  ;
	int iPre = 0 , iAft = 0  ;
	iPre = iNum / 26 ;
	iAft = iNum% 26 ;
	if( iPre > 0 )
	{

		if( iAft > 0 )
		{		
			chPre[0] = 'A' + iPre - 1 ;	
			chAft[0] = 'A' + iAft - 1 ;
		}
		else
		{
			if (iPre == 1)
			{
				chPre[0] = 0  ;
			}
			else
			{
				chPre[0] = 'A' + iPre -2 ;
			}

			chAft[0] = 'Z' ;
		}
	}
	else 
	{
		chPre[0] = 0 ;
		chAft[0] = 'A' + iAft - 1  ;
	}
	chPre[1] = 0 ;
	chAft[1] = 0 ;

	strReturn .Format( _T("%s%s") , chPre , chAft ) ;
	return strReturn ;
}

//返回excel表格的列所对应的数值
//
int CBlkUtility::GetNumFromExcelColName(TCHAR ch[])
{
	int iReturn =0 ;
	int iChLenth = _tcsclen(ch) ;
	for(int i=0; i<iChLenth; i++ )
	{
		int iPow = (int)pow(int(26),float(iChLenth-i-1)) ;
		iReturn = iReturn + (ch[i]-'A'+1)*iPow ;
	}

	return iReturn ;
}

//字符串替换，大小写区分/不区分
//
int CBlkUtility::ReplaceTxt(CString &strSource, CString strOld, CString strNew, BOOL bCaseSensitive)
{
	int iReturn ;
	//区分大小写时，CString的自带函数Replace即可实现
	if (bCaseSensitive) 
	{
		iReturn = strSource.Replace(strOld, strNew) ;

	}
	//不区分大小写
	else
	{
		CString strSourceCopy ;
		strSourceCopy = strSource ;
		CUIntArray intarrayFindStrAt ;
		CStringArray strarrayFindStr ;


		strSourceCopy.MakeLower() ;
		strOld.MakeLower() ;
		int iStart = 0 ;
		int istrOldLength = strOld.GetLength() ;
		int iFindStrAt = -1 ;
		//将找出得字符串所在位置写入intarrayFindStrAt容器中
		while( ( iFindStrAt=strSourceCopy.Find(strOld, iStart))!=-1 ) 
		{
			intarrayFindStrAt.Add(iFindStrAt) ;
			strSource.Delete(iFindStrAt, istrOldLength) ;
			strSource.Insert(iFindStrAt,strNew) ;
			iStart = iFindStrAt + istrOldLength  ;
		}

		iReturn = intarrayFindStrAt.GetSize() ;
	}
	return iReturn ;
}

//查找指定目录下的、指定后缀名的文件,iMode_SearchDepth为1时,只查找当前目录,iMode_SearchFileOrFolder为0时返回文件,为1时返回目录
void CBlkUtility::FindFile(CString strDirName, CString strFileExten, CStringArray &strArrayFilePath, int iMode_SearchDepth, int iMode_SearchFileOrFolder)
{
	strFileExten.MakeLower() ;

	CString strInsert ;
	CFileFind cMyFinder ;
	CString strFileName ;
	if( strDirName.GetLength() == 3 )
	{
		strFileName = strDirName + _T("*.*") ;
	}
	else
	{
		strFileName = strDirName + _T("\\*.*") ;
	}
	BOOL bFindFile ;
	bFindFile = cMyFinder.FindFile( strFileName ) ;

	if( bFindFile )
	{
		while( bFindFile )
		{
			bFindFile = cMyFinder.FindNextFile() ;
			if(cMyFinder.IsDots())
			{
				continue ;
			}
			if(cMyFinder.IsDirectory ())
			{
				if (iMode_SearchDepth==0)
				{
					if (iMode_SearchFileOrFolder==1)
					{
						strArrayFilePath.Add(cMyFinder.GetFilePath()) ;
					}
					FindFile(cMyFinder.GetFilePath(), strFileExten, strArrayFilePath, iMode_SearchDepth, iMode_SearchFileOrFolder) ;
				}
				else
				{
					if (iMode_SearchFileOrFolder==1)
					{
						strArrayFilePath.Add(cMyFinder.GetFilePath()) ;
					}
				}				
			}
			else
			{
				if (iMode_SearchFileOrFolder==0)
				{
					CString strTempFileName ;
					strTempFileName = cMyFinder.GetFilePath() ;
					//strTempFileName.MakeLower() ;

					// 				const TCHAR *p = NULL ;
					// 				p = _tcsrchr( strTempFileName , _T('.') ) ;
					// 				if( p!=NULL )
					// 				{
					// 					if( _tcscmp( p+1 , strFileExten) == 0 )
					// 					{
					// 						strArrayFilePath.Add( strTempFileName ) ;
					// 					}
					// 				}
					int iMarkLoc = strTempFileName.ReverseFind(_T('.')) ;
					CString strFileExtenTmp = strTempFileName.Mid(iMarkLoc+1) ;
					strFileExtenTmp.MakeLower() ;
					if (strFileExtenTmp==strFileExten||strFileExten==_T(""))
					{
						strArrayFilePath.Add( strTempFileName ) ;
					}
				}
			}
		}
	}
}

// 从文件全路径获取文件名称，默认不带后缀，iOption=1则带后缀
BOOL CBlkUtility::GetFileName(const CString& strFilePath, CString& strFileName, int iOption)
{
	if (strFilePath.IsEmpty())
	{
		return FALSE ;
	}

	BOOL bRet = FALSE ;
	int iFindLoc = strFilePath.ReverseFind(_T('\\')) ;
	if (iFindLoc!=-1)
	{
		CString strNameWithTail = strFilePath.Mid(iFindLoc+1) ;
		iFindLoc = strNameWithTail.ReverseFind(_T('.')) ;
		if (iFindLoc!=-1)
		{
			CString strNameWithoutTail = strNameWithTail.Left(iFindLoc) ;
			if (!(strNameWithTail.IsEmpty()||strNameWithoutTail.IsEmpty()))
			{
				strFileName = (iOption==0?strNameWithoutTail:strNameWithTail) ;
				bRet = TRUE ;
			}
		}
		else
		{
			strFileName = strNameWithTail ;
			bRet  = TRUE ;
		}
	}
	else
	{
		strFileName = strFilePath ;
		bRet  = TRUE ;
	}
	return bRet ;
}

// 从文件全路径名获取文件路径
BOOL CBlkUtility::GetFilePath(const CString& strFileFullPath, CString& strFilePath)
{
	if (strFileFullPath.IsEmpty())
	{
		return FALSE ;
	}

	BOOL bRet = FALSE ;
	int iFindLoc = strFileFullPath.ReverseFind(_T('\\')) ;
	if (iFindLoc!=-1)
	{
		strFilePath = strFileFullPath.Left(iFindLoc) ;
		bRet = TRUE ;
	}
	else
	{
		strFilePath = strFileFullPath ;
		bRet  = TRUE ;
	}
	return bRet ;
}

//1~9转换为大写
CString CBlkUtility::DigitToCapsString(int iNum)
{
	{
		CString strNum ;

		switch ( iNum ) 
		{
		case 1:
			{
				strNum = "一" ;
				break ;
			}
		case 2:
			{
				strNum = "二" ;
				break ;
			}
		case 3:
			{
				strNum = "三" ;
				break ;
			}
		case 4:
			{
				strNum = "四" ;
				break ;
			}
		case 5:
			{
				strNum = "五" ;
				break ;
			}
		case 6:
			{
				strNum = "六" ;
				break ;
			}
		case 7:
			{
				strNum = "七" ;
				break ;
			}
		case 8:
			{
				strNum = "八" ;
				break ;
			}
		case 9:
			{
				strNum = "九" ;
				break ;
			}
		}
		return strNum ;
	}
}

//数字转换为大写（100以内）
CString CBlkUtility::NumToCapsString(int iNum)
{
	CString strNum ;
	if( iNum < 100 && iNum > 0 )
	{
		int iTens = 0 ;//十位　；
		int iDigit = 0 ; //个位　；
		iTens = iNum / 10 ;
		iDigit = iNum % 10 ;
		CString strTensStr , strDigitStr;

		strTensStr = DigitToCapsString( iTens ) ;
		strDigitStr = DigitToCapsString( iDigit ) ;
		if( iTens > 1 )
		{
			strNum.Format( _T("%s十%s") , strTensStr , strDigitStr ) ;
		}
		else if( iTens == 1 )
		{
			strNum.Format( _T("十%s") ,  strDigitStr ) ;
		}
		else
		{
			strNum.Format( _T("%s") ,  strDigitStr ) ;
		}
	}
	else if ( iNum == 0 )
	{
		strNum = "0" ;
	}
	return strNum ;
}
//返回时间字符窜 ;
CString CBlkUtility::GetDateStr()
{
	CString strDate ;
	CTime cCurTime ;
	cCurTime = CTime::GetCurrentTime() ;
	strDate.Format(_T("%d.%d"), cCurTime.GetYear(),cCurTime.GetMonth()) ;
	return strDate ;
}

//删除字符窜中以strStrart为头至strEnd的部分,例如;
//CutString("111ss333ed44rrrss000ed3ed33","ss","ed", strResult);
//strResult = "11144rrr3ed33" ;
//
void CBlkUtility::CutString(CString strSrc, CString strStart, CString strEnd, CString &strResult)
{
	BOOL bOk = false ;
	int iEndStrLength = strEnd.GetLength() ;
	while (!bOk)
	{
		int iStartLoc = strSrc.Find(strStart) ;
		int iEndLoc = strSrc.Find(strEnd) ;
		int iSrcLength = strSrc.GetLength() ;
		CString strTmp1,strTmp2 ;

		if (iStartLoc != -1 && iEndLoc != -1 )
		{
			strTmp1 = strSrc.Left(iStartLoc) ;
			strTmp2 = strSrc.Right(iSrcLength-iEndLoc-iEndStrLength) ;
			strSrc.Format(_T("%s%s"),strTmp1,strTmp2) ;
			strResult = strSrc ;
			if (strSrc.Find(strStart) != -1 && strSrc.Find(strEnd) != -1)
			{
				CutString(strSrc, strStart, strEnd, strResult ) ;
			}
			else
			{
				bOk = true ;
			}
		}
		else
		{
			strResult = strSrc ;
			bOk = true ;
		}
	}
}

//分解含有特定分割符号的字符串,比如"aaa\pbbb\pccc",以"\p"为分隔符,分解为"aaa" "bbb" "ccc" ;
/*
*	strSrc         待分解的字符串;
*  strMark        分隔符;
*  strArrayResult 分割后的结果字符串数组 ;
*/
//////////////////////////////////////////////////////////////////////////
void CBlkUtility::DivideString(CString strSrc, CString strMark, CStringArray &strArrayResult)
{
	if (strSrc.IsEmpty())
	{
		return ;
	}
	CString strTmp  ;
	int iSrcLength = 0 ;
	int iMarkLength = 0 ;
	int iTmp = 0 ;
	int iMarkLocation = 0 ;
	BOOL bHaveMark = true ;

	iMarkLength = strMark.GetLength() ;

	while (bHaveMark) 
	{	
		iSrcLength = strSrc.GetLength() ;

		if (iSrcLength > iMarkLength) 
		{
			iTmp = iSrcLength - iMarkLength ;
			iMarkLocation = strSrc.Find(strMark) ;

			if (iMarkLocation != -1)
			{
				strArrayResult.Add(strSrc.Left(iMarkLocation)) ;
				strSrc = strSrc.Right(iTmp-iMarkLocation) ;
			}
			else
			{
				strArrayResult.Add(strSrc) ;
				bHaveMark = false ;
			}
		}
		else
		{
			strArrayResult.Add(strSrc) ;			
			bHaveMark = false ;
		}
	}
}

// 带百分号的罗马字符形式，转换成字母I、V组合的形式
BOOL CBlkUtility::ConvertRomanToChar(const CString & strRoman, CString & strRet)
{
	BOOL bRet = TRUE ;

	CString strTmp[10] = {_T("I"),_T("II"),_T("III"),_T("IV"),_T("V"),_T("VI"),_T("VII"),_T("VIII"),_T("IX"),_T("X")} ;
	if (strRoman.Left(2)==_T("%%"))
	{
		CString strDigit ;
		strDigit = strRoman.Mid(2) ;
		int iDig = 0 ;
		iDig = _ttoi(strDigit) ;
		if (iDig>=161 && iDig<=170)
		{
			strRet = strTmp[iDig-161] ;
		}
		else
		{
			bRet = FALSE ;
		}
	}
	else
	{
		bRet = FALSE ;
	}

	return bRet;
}

// 将strArray数组内容用strMark链接成字符串,iAddQuota默认为0：表示不加引号，为1表示加单引号，为2表示加双引号
BOOL CBlkUtility::ConvertArrayToString(const CStringArray & strArray, const CString & strMark, CString & strRet,  int iAddQuota)
{
	CString strQuota = _T("") ;
	if (iAddQuota==1)
	{
		strQuota = _T("\'") ;
	}
	else if (iAddQuota==2)
	{
		strQuota = _T("\"") ;
	}

	int iCount = strArray.GetCount() ;
	for (int i=0; i<iCount; i++)
	{
		CString strTmp = strArray.GetAt(i) ;

		if (iAddQuota!=0)
		{
			strTmp.Insert(0, strQuota) ;
			strTmp.Append(strQuota) ;
		}
		strRet.Append(strTmp) ;
		if (i!=(iCount-1))
		{
			strRet.Append(strMark) ;
		}
	}

	return 0;
}
BOOL CBlkUtility::ConvertArrayToString(const CLongArray & nArray, const CString & strMark, CString & strRet,  int iAddQuota)
{
	CString strQuota = _T("") ;
	if (iAddQuota==1)
	{
		strQuota = _T("\'") ;
	}
	else if (iAddQuota==2)
	{
		strQuota = _T("\"") ;
	}

	int iCount = nArray.GetCount() ;
	for (int i=0; i<iCount; i++)
	{
		strRet.AppendFormat(_T("%s%d%s"), strQuota, nArray.GetAt(i),  strQuota) ;
		if (i!=(iCount-1))
		{
			strRet.Append(strMark) ;
		}
	}

	return 0;
}

// 查找指定字符串在字符串数组中的位置，索引以0开始，未找到返回-1
int CBlkUtility::FindStrInArray(const CStringArray & strArraySrc, LPCTSTR lpszToFind)
{
	int iRet = -1 ;

	int iCount = 0 ;
	iCount = strArraySrc.GetCount() ;
	for (int i=0; i<iCount; i++)
	{
		if (_tcscmp(lpszToFind,strArraySrc.GetAt(i))==0)
		{
			iRet = i ;
			break;
		}
	}

	return iRet;
}
// 查找指定字符串在字符串数组中的位置，索引以0开始，未找到返回-1
int CBlkUtility::FindStrInVector(const  vector<CString> & strVecSrc, LPCTSTR lpszToFind)
{
	int iRet =-1 ;

	int iCount = 0 ;
	iCount = strVecSrc.size();
	for (int i=0; i<iCount; i++)
	{
		if (_tcscmp(lpszToFind,strVecSrc[i])==0)
		{
			iRet = i ;
			break;
		}
	}

	return iRet;
}
//查找指定int在整形数组中的位置，索引以0开始，未找到返回-1
int CBlkUtility::FindLongInArray(const  CLongArray& nArraySrc, long nToFind)
{
	int iRet = -1 ;

	int iCount = 0 ;
	iCount = nArraySrc.GetCount() ;
	for (int i=0; i<iCount; i++)
	{
		if (nToFind==nArraySrc.GetAt(i))
		{
			iRet = i ;
			break;
		}
	}

	return iRet;
}

// 查找字符串
int CBlkUtility::FindString(const CString & strSrc, const CString & strPreMark, const CString & strBackMark, CStringArray & strArrayFind)
{
	int iRet = 0 ;

	int iCurLoc = 0 ;
	int iFindLoc = strSrc.Find(strPreMark, iCurLoc) ;
	while (iFindLoc!=-1)
	{
		int iFindLoc2 = strSrc.Find(strBackMark, iCurLoc) ;
		if(iFindLoc2!=-1)
		{
			CString strFindStr ;
			strFindStr = strSrc.Mid(iFindLoc+1,iFindLoc2-iFindLoc-1) ;
			strArrayFind.Add(strFindStr) ;
			iRet++ ;
			//acutPrintf(_T("\n%d-%d"), iFindLoc, iFindLoc2) ;
			iCurLoc = iFindLoc2+1 ;
			iFindLoc = strSrc.Find(strPreMark, iCurLoc) ;
			//acutPrintf(_T("\n%s"), strFindStr) ;
		}
	}
	return iRet;
}

// 从注册表中HKEY_LOCAL_MACHINE\\SOFTWARE\\ZHFCAD\\[strToOpenSub]读取strItem的值
BOOL CBlkUtility::QueryValueReg(const CString & strToOpenSub,const CString & strItem, CString & strResult)
{
	BOOL bRet = TRUE ;

	CRegKey cReg ;
	CString strToOpen ;
	strToOpen.Format(_T("SOFTWARE\\ZHFCAD\\%s"), strToOpenSub) ;
	if (ERROR_SUCCESS == cReg.Open(HKEY_LOCAL_MACHINE , strToOpen ))
	{		
		TCHAR  chSN[200] ;
		chSN[0] = _T('\0') ;

		DWORD lSize = sizeof(chSN) ;

		if (ERROR_SUCCESS == cReg.QueryValue(chSN, strItem, &lSize)) 
		{
			strResult = chSN ;
		}
		else
		{
			bRet = FALSE ;
		}
	}
	else
	{
		bRet = FALSE ;
	}
	cReg.Close() ;

	return bRet;
}

BOOL CBlkUtility::ReadReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, CString & strResult)
{
	BOOL bRet = TRUE ;

	CRegKey cReg ;
	if (ERROR_SUCCESS == cReg.Open(hKeyParent , strKeyName ))
	{		
		TCHAR  chSN[200] ;
		chSN[0] = _T('\0') ;

		DWORD lSize = sizeof(chSN) ;

		if (ERROR_SUCCESS == cReg.QueryValue(chSN, strItem, &lSize)) 
		{
			strResult = chSN ;
		}
		else
		{
			bRet = FALSE ;
		}
	}
	else
	{
		bRet = FALSE ;
	}
	cReg.Close() ;

	return bRet;
}

BOOL CBlkUtility::QueryValueReg(const CString & strToOpenSub,const CString & strItem, DWORD & dwResult)
{
	BOOL bRet = TRUE ;

	CRegKey cReg ;
	CString strToOpen ;
	strToOpen.Format(_T("SOFTWARE\\ZHFCAD\\%s"), strToOpenSub) ;
	if (ERROR_SUCCESS == cReg.Open(HKEY_LOCAL_MACHINE , strToOpen ))
	{
		if (ERROR_SUCCESS == cReg.QueryValue(dwResult, strItem)) 
		{
		}
		else
		{
			bRet = FALSE ;
		}
	}
	else
	{
		bRet = FALSE ;
	}
	cReg.Close() ;

	return bRet;
}

BOOL CBlkUtility::ReadReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, DWORD & dwResult)
{
	BOOL bRet = TRUE ;

	CRegKey cReg ;

	if (ERROR_SUCCESS == cReg.Open(hKeyParent , strKeyName ))
	{
		if (ERROR_SUCCESS == cReg.QueryValue(dwResult, strItem)) 
		{
		}
		else
		{
			bRet = FALSE ;
		}
	}
	else
	{
		bRet = FALSE ;
	}
	cReg.Close() ;

	return bRet;
}


BOOL CBlkUtility::QueryValueReg(const CString & strToOpenSub,const CString & strItem, int & iResult)
{
	BOOL bRet = TRUE ;
	DWORD dwResult = 0 ;
	bRet = QueryValueReg(strToOpenSub, strItem, dwResult) ;
	if (bRet)
	{
		iResult = (int)dwResult ;
	}

	return bRet;
}

BOOL CBlkUtility::ReadReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, int & iResult)
{
	BOOL bRet = TRUE ;
	DWORD dwResult = 0 ;
	bRet = ReadReg(hKeyParent, strKeyName, strItem, dwResult) ;
	if (bRet)
	{
		iResult = (int)dwResult ;
	}

	return bRet;
}
BOOL CBlkUtility::QueryValueReg(const CString & strToOpenSub,const CString & strItem, double & dResult)
{
	BOOL bRet = TRUE ;
	CString strResult  ;
	bRet = QueryValueReg(strToOpenSub, strItem, strResult) ;
	if (bRet)
	{
		dResult = _tstof(strResult) ;
	}

	return bRet;
}

BOOL CBlkUtility::ReadReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, double & dResult)
{
	BOOL bRet = TRUE ;
	CString strResult  ;
	bRet = ReadReg(hKeyParent, strKeyName, strItem, strResult) ;
	if (bRet)
	{
		dResult = _tstof(strResult) ;
	}

	return bRet;
}


// 向注册表中HKEY_LOCAL_MACHINE\\SOFTWARE\\ZHFCAD\\[strToOpenSub]的strItem写入值strValue
BOOL CBlkUtility::SetValueReg(const CString & strToOpenSub, const CString & strItem, const CString & strValue)
{
	BOOL bRet = TRUE ;

	CRegKey cReg ;
	CString strToOpen ;
	strToOpen.Format(_T("SOFTWARE\\ZHFCAD\\%s"), strToOpenSub) ;
	if (ERROR_SUCCESS == cReg.Open(HKEY_LOCAL_MACHINE ,strToOpen ))
	{
		if (ERROR_SUCCESS != cReg.SetValue(strValue, strItem) )
		{
			bRet = FALSE ;
		}
	}
	else
	{
		if (ERROR_SUCCESS == cReg.Create(HKEY_LOCAL_MACHINE , strToOpen))
		{
			if (ERROR_SUCCESS == cReg.Open(HKEY_LOCAL_MACHINE ,strToOpen ))
			{
				if (ERROR_SUCCESS != cReg.SetValue(strValue, strItem) )
				{
					bRet = FALSE ;
				}
			}
		}
		else
		{
			bRet = FALSE ;
		}
	}
	cReg.Close() ;

	return bRet ;
}

BOOL CBlkUtility::WriteReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, const CString & strValue)
{
	BOOL bRet = TRUE ;

	CRegKey cReg ;
	if (ERROR_SUCCESS == cReg.Open(hKeyParent, strKeyName ))
	{
		if (ERROR_SUCCESS != cReg.SetValue(strValue, strItem) )
		{
			bRet = FALSE ;
		}
	}
	else
	{
		if (ERROR_SUCCESS == cReg.Create(hKeyParent, strKeyName))
		{
			if (ERROR_SUCCESS == cReg.Open(hKeyParent, strKeyName ))
			{
				if (ERROR_SUCCESS != cReg.SetValue(strValue, strItem) )
				{
					bRet = FALSE ;
				}
			}
		}
		else
		{
			bRet = FALSE ;
		}
	}
	cReg.Close() ;

	return bRet ;
}

BOOL CBlkUtility::SetValueReg(const CString & strToOpenSub, const CString & strItem, DWORD dwValue)
{
	BOOL bRet = TRUE ;

	CRegKey cReg ;
	CString strToOpen ;
	strToOpen.Format(_T("SOFTWARE\\ZHFCAD\\%s"), strToOpenSub) ;
	if (ERROR_SUCCESS == cReg.Open(HKEY_LOCAL_MACHINE ,strToOpen ))
	{
		if (ERROR_SUCCESS != cReg.SetValue(dwValue, strItem) )
		{
			bRet = FALSE ;
		}
	}
	else
	{
		if (ERROR_SUCCESS == cReg.Create(HKEY_LOCAL_MACHINE , strToOpen))
		{
			if (ERROR_SUCCESS == cReg.Open(HKEY_LOCAL_MACHINE ,strToOpen ))
			{
				if (ERROR_SUCCESS != cReg.SetValue(dwValue, strItem) )
				{
					bRet = FALSE ;
				}
			}
		}
		else
		{
			bRet = FALSE ;
		}
	}
	cReg.Close() ;

	return bRet ;
}

BOOL CBlkUtility::WriteReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, DWORD dwValue)
{
	BOOL bRet = TRUE ;

	CRegKey cReg ;
	if (ERROR_SUCCESS == cReg.Open(hKeyParent, strKeyName ))
	{
		if (ERROR_SUCCESS != cReg.SetValue(dwValue, strItem) )
		{
			bRet = FALSE ;
		}
	}
	else
	{
		if (ERROR_SUCCESS == cReg.Create(hKeyParent, strKeyName))
		{
			if (ERROR_SUCCESS == cReg.Open(hKeyParent, strKeyName ))
			{
				if (ERROR_SUCCESS != cReg.SetValue(dwValue, strItem) )
				{
					bRet = FALSE ;
				}
			}
		}
		else
		{
			bRet = FALSE ;
		}
	}
	cReg.Close() ;

	return bRet ;
}

BOOL CBlkUtility::SetValueReg(const CString & strToOpenSub, const CString & strItem, int iValue)
{
	BOOL bRet = TRUE ;
	DWORD dwValue = (DWORD)iValue ;
	bRet = SetValueReg(strToOpenSub, strItem, dwValue) ;

	return bRet ;
}

BOOL CBlkUtility::WriteReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, int iValue)
{
	BOOL bRet = TRUE ;
	DWORD dwValue = (DWORD)iValue ;
	bRet = WriteReg(hKeyParent, strKeyName, strItem, dwValue) ;

	return bRet ;
}

BOOL CBlkUtility::SetValueReg(const CString & strToOpenSub, const CString & strItem, double dValue)
{
	BOOL bRet = TRUE ;
	CString strValue ;
	strValue.Format(_T("%f"), dValue) ;
	bRet = SetValueReg(strToOpenSub, strItem, strValue) ;

	return bRet ;
}

BOOL CBlkUtility::WriteReg(HKEY hKeyParent, const CString& strKeyName, const CString& strItem, double dValue)
{
	BOOL bRet = TRUE ;
	CString strValue ;
	strValue.Format(_T("%f"), dValue) ;
	bRet = WriteReg(hKeyParent, strKeyName, strItem, strValue) ;

	return bRet ;
}


//取得程序安装目录
BOOL CBlkUtility::GetBasePath(CString &strBasePath, LPCTSTR lpstrProject)
{
	BOOL bReturn = FALSE ;
	CString strKeyName ;
	strKeyName.Format(_T("Software\\%s"), lpstrProject) ;
	CRegKey cReg ;
	if( ERROR_SUCCESS != cReg.Open( HKEY_LOCAL_MACHINE , strKeyName) )
	{
		bReturn = FALSE ;
	}
	else
	{
		TCHAR chBasePath[100] ;
		DWORD dw = 0 ;
		dw = sizeof ( chBasePath ) ;
		if( ERROR_SUCCESS != cReg.QueryValue( chBasePath , _T("path") ,& dw ) )
		{
			bReturn = FALSE ;
		}
		else
		{
			strBasePath = chBasePath ;
			bReturn = TRUE ;
		}
		cReg.Close () ;
	}	
	return bReturn ;
}

// 多字节转宽字节
BOOL CBlkUtility::ConvMultiToWide(const char* szMultiByte, CString& strWideChar, int nSrcCodePage)
{
#ifdef UNICODE
	int iNum = MultiByteToWideChar(nSrcCodePage, 0, szMultiByte, -1, NULL, 0) ;
	WCHAR * pTxt = new WCHAR[iNum+1] ;
	memset(pTxt, 0, (iNum+1)*sizeof(WCHAR)) ;
	if (!pTxt)
	{
		delete []pTxt ;
	}
	MultiByteToWideChar(nSrcCodePage, 0, szMultiByte, -1, pTxt, iNum ) ;		
	//strWideChar = pTxt ; //ok
	strWideChar.Format(L"%s", pTxt) ;
	delete [] pTxt ;
#else
	strWideChar = szMultiByte ;
#endif

	return 0;
}

// 宽字节转多字节
BOOL CBlkUtility::ConvWideToMulti( LPCWSTR strWideChar, CStringA& strMultiByte, int nSrcCodePage)
{
	int  iNum = WideCharToMultiByte(nSrcCodePage, 0, strWideChar, -1, NULL, 0, NULL, NULL) ;
	char * pTxt = new char[iNum+1] ;
	memset(pTxt, 0, (iNum+1)*sizeof(char)) ;
	if (!pTxt)
	{
		delete []pTxt ;
	}
	WideCharToMultiByte(nSrcCodePage, 0, strWideChar, -1, pTxt, iNum, NULL, NULL ) ;
	//strMultiByte = pTxt ; //ok
	strMultiByte.Format("%s", pTxt) ;
	delete [] pTxt ;

	return 0;
}

void CBlkUtility::SortStringArray(CStringArray& strArray, int iOrder)
{
	BOOL bNotDone = TRUE;

	while (bNotDone)
	{
		bNotDone = FALSE;
		for(int pos = 0;pos < strArray.GetUpperBound();pos++)
		{
			BOOL bComp = FALSE ;
			CString temp;
			int posFirst = pos;
			int posNext = pos + 1;
			if (strArray.GetAt(posFirst).CompareNoCase(strArray.GetAt(posNext)) > 0)
			{
				temp = strArray.GetAt(posFirst);
				strArray.SetAt(posFirst, strArray.GetAt(posNext));
				strArray.SetAt(posNext, temp);
				bComp = TRUE;
			}

			bNotDone |= bComp ;
		}
	}
}

void CBlkUtility::SortandDistinguishStringArray( CStringArray &strArray,CStringArray &strArrayOdd ,CStringArray &strArrayEven,TCHAR del_char )
{
	for(int pos = 0;pos < strArray.GetCount();pos++)
	{
		CString str_first=strArray.GetAt(pos);
		str_first.Remove(del_char);
		int int_fisrt=StrToIntA(str_first);
		if(int_fisrt%2==1)
		{
			strArrayOdd.Add(strArray.GetAt(pos));

		}
		else
		{
			strArrayEven.Add(strArray.GetAt(pos));
		}
	}
	//排序
	SortStringArray( strArrayOdd,del_char);
	SortStringArray( strArrayEven,del_char);

}

void CBlkUtility::SortStringArray( CStringArray &strArray,TCHAR del_char)
{
	/*long t1=GetTickCount();*/
	BOOL bNotDone = TRUE;
	while (bNotDone)
	{
		bNotDone = FALSE;
		for(int pos = 0;pos < strArray.GetUpperBound();pos++)
		{
			BOOL bComp = FALSE ;
			CString temp;
			int posFirst = pos;
			int posNext = pos + 1;
			CString str_first=strArray.GetAt(posFirst);
			str_first.Remove(del_char);
			int int_fisrt=StrToIntA(str_first);
			CString strNext=strArray.GetAt(posNext);
			strNext.Remove(del_char);
			int int_next=StrToIntA(strNext);
			//比较排序
			if (int_fisrt>int_next)
			{
				temp = strArray.GetAt(posFirst);
				strArray.SetAt(posFirst, strArray.GetAt(posNext));
				strArray.SetAt(posNext, temp);
				bComp = TRUE;
			}
			bNotDone |= bComp ;
		}
	}
}
//排序出站兼调车信号机
void CBlkUtility::SortStringArray_CZXHJ( CStringArray &strArray,TCHAR del_char)
{
	/*long t1=GetTickCount();*/
	BOOL bNotDone = TRUE;
	while (bNotDone)
	{
		bNotDone = FALSE;
		for(int pos = 0;pos < strArray.GetUpperBound();pos++)
		{
			BOOL bComp = FALSE ;
			CString temp;
			//long ntemp=0;
			int posFirst = pos;
			int posNext = pos + 1;
			CString str_first=strArray.GetAt(posFirst);
			str_first.Remove(del_char);
			str_first.Replace(_T("%%16"),_T(""));
			int int_fisrt=StrToIntA(str_first);
			CString strNext=strArray.GetAt(posNext);
			strNext.Remove(del_char);
			strNext.Replace(_T("%%16"),_T(""));
			int int_next=StrToIntA(strNext);
			//比较排序
			if (int_fisrt>int_next)
			{
				temp = strArray.GetAt(posFirst);
				//ntemp=nxhj_Array.GetAt(posFirst);
				strArray.SetAt(posFirst, strArray.GetAt(posNext));
				//nxhj_Array.SetAt(posFirst,nxhj_Array.GetAt(posNext));
				strArray.SetAt(posNext, temp);
				//nxhj_Array.SetAt(posNext,ntemp);
				bComp = TRUE;
			}
			bNotDone |= bComp ;
		}
	}
}
//排序出站兼调车信号机
void CBlkUtility::SortStringArray_CZXHJHandle( CStringArray &strArray,TCHAR del_char,CLongArray &nxhj_Array)
{
	/*long t1=GetTickCount();*/
	BOOL bNotDone = TRUE;
	while (bNotDone)
	{
		bNotDone = FALSE;
		for(int pos = 0;pos < strArray.GetUpperBound();pos++)
		{
			BOOL bComp = FALSE ;
			CString temp;
			long ntemp=0;
			int posFirst = pos;
			int posNext = pos + 1;
			CString str_first=strArray.GetAt(posFirst);
			str_first.Remove(del_char);
			str_first.Replace(_T("%%16"),_T(""));
			int int_fisrt=StrToIntA(str_first);
			CString strNext=strArray.GetAt(posNext);
			strNext.Remove(del_char);
			strNext.Replace(_T("%%16"),_T(""));
			int int_next=StrToIntA(strNext);
			//比较排序
			if (int_fisrt>int_next)
			{
				temp = strArray.GetAt(posFirst);
				ntemp=nxhj_Array.GetAt(posFirst);
				strArray.SetAt(posFirst, strArray.GetAt(posNext));
				nxhj_Array.SetAt(posFirst,nxhj_Array.GetAt(posNext));
				strArray.SetAt(posNext, temp);
				nxhj_Array.SetAt(posNext,ntemp);
				bComp = TRUE;
			}
			bNotDone |= bComp ;
		}
	}
}
//排序字符串str,对应调整字符串对应的信号机Handle
void CBlkUtility::SortStringArray_CZXHJHandle( CStringArray &strArray,CLongArray &nxhj_Array)
{
	/*long t1=GetTickCount();*/
	BOOL bNotDone = TRUE;
	while (bNotDone)
	{
		bNotDone = FALSE;
		for(int pos = 0;pos < strArray.GetUpperBound();pos++)
		{
			BOOL bComp = FALSE ;
			CString temp;
			long ntemp=0;
			int posFirst = pos;
			int posNext = pos + 1;
			CString str_first=strArray.GetAt(posFirst);
			//str_first.Remove(del_char);
			str_first.Replace(_T("%%16"),_T(""));
			int int_fisrt=StrToIntA(str_first);
			CString strNext=strArray.GetAt(posNext);
			//strNext.Remove(del_char);
			strNext.Replace(_T("%%16"),_T(""));
			int int_next=StrToIntA(strNext);
			int i_value=str_first.CompareNoCase(strNext);
			//比较排序
			if (i_value>0)
			{
				temp = strArray.GetAt(posFirst);
				ntemp=nxhj_Array.GetAt(posFirst);
				strArray.SetAt(posFirst, strArray.GetAt(posNext));
				nxhj_Array.SetAt(posFirst,nxhj_Array.GetAt(posNext));
				strArray.SetAt(posNext, temp);
				nxhj_Array.SetAt(posNext,ntemp);
				bComp = TRUE;
			}
			bNotDone |= bComp ;
		}
	}
}
//排序字符串str,对应调整字符串对应的信号机Handle
void CBlkUtility::SortStringArray_DCZhongduanxhj( CStringArray &strArray,CLongArray &nxhj_Array)
{
	//先将调车终端按照类别分开
	CStringArray strArrayDC;
	CLongArray  nArrayDc;
	CStringArray strArrayGudao;
	CLongArray nArrayGudao;
	CStringArray strArrayCZxhj;
	CLongArray  nArrayCZxhj;
	CStringArray strArrayJZxhj;
    CLongArray  nArrayJZxhj;
    int i_count=strArray.GetCount();
	for(int i=0;i<i_count;i++)
	{
		if(strArray.GetAt(i).Left(1)==_T('D'))
		{
			//普通调车信号机
			strArrayDC.Add(strArray.GetAt(i));
			nArrayDc.Add(nxhj_Array.GetAt(i));
		}
		else if(strArray.GetAt(i).Right(1)==_T('D'))
		{
			//出站兼调车
			strArrayCZxhj.Add(strArray.GetAt(i));
			nArrayCZxhj.Add(nxhj_Array.GetAt(i));
		}
		else if(strArray.GetAt(i).Right(1)==_T('G'))
		{
			//股道
			strArrayGudao.Add(strArray.GetAt(i));
			nArrayGudao.Add(nxhj_Array.GetAt(i));
		}
		else
		{
			//进站信号机
			strArrayJZxhj.Add(strArray.GetAt(i));
			nArrayJZxhj.Add(nxhj_Array.GetAt(i));
		}
		
	}
	//分别单独排序
	SortStringArray_CZXHJHandle(strArrayDC,_T('D'),nArrayDc);
	SortStringArray_CZXHJHandle(strArrayCZxhj,nArrayCZxhj);
	SortStringArray_CZXHJHandle(strArrayGudao,nArrayGudao);
	SortStringArray_CZXHJHandle(strArrayJZxhj,nArrayJZxhj);
	strArray.RemoveAll();
	nxhj_Array.RemoveAll();
	//合并
	for(int k=0;k<strArrayDC.GetCount();k++)
	{
		strArray.Add(strArrayDC.GetAt(k));
		nxhj_Array.Add(nArrayDc.GetAt(k));
	}
	for(int k=0;k<strArrayCZxhj.GetCount();k++)
	{
		strArray.Add(strArrayCZxhj.GetAt(k));
		nxhj_Array.Add(nArrayCZxhj.GetAt(k));
	}
	for(int k=0;k<strArrayGudao.GetCount();k++)
	{
		strArray.Add(strArrayGudao.GetAt(k));
		nxhj_Array.Add(nArrayGudao.GetAt(k));
	}
	for(int k=0;k<strArrayJZxhj.GetCount();k++)
	{
		strArray.Add(strArrayJZxhj.GetAt(k));
		nxhj_Array.Add(nArrayJZxhj.GetAt(k));
	}

}
//排序出站兼调车信号机
void CBlkUtility::SortStringArray_CZXHJHandle( CStringArray &strArray,TCHAR del_char,CStringArray &sGudaoname,CLongArray &sGudaohandle,CStringArray &xGudaoname,CLongArray &xGuidaohandle)
{
	/*long t1=GetTickCount();*/
	BOOL bNotDone = TRUE;
	while (bNotDone)
	{
		bNotDone = FALSE;
		for(int pos = 0;pos < strArray.GetUpperBound();pos++)
		{
			BOOL bComp = FALSE ;
			CString temp;
			long ntemp=0;
			CString strSgudao;
			CString strXgudao;
			long nShandle=0;
			long nXhangle=0;
			int posFirst = pos;
			int posNext = pos + 1;
			CString str_first=strArray.GetAt(posFirst);
			str_first.Remove(del_char);
			str_first.Replace(_T("%%16"),_T(""));
			int int_fisrt=StrToIntA(str_first);
			CString strNext=strArray.GetAt(posNext);
			strNext.Remove(del_char);
			strNext.Replace(_T("%%16"),_T(""));
			int int_next=StrToIntA(strNext);
			//比较排序
			if (int_fisrt>int_next)
			{
				temp = strArray.GetAt(posFirst);
				//ntemp=nxhj_Array.GetAt(posFirst);
				strSgudao=sGudaoname.GetAt(posFirst);
				nShandle=sGudaohandle.GetAt(posFirst);
				strXgudao=xGudaoname.GetAt(posFirst);
				nXhangle=xGuidaohandle.GetAt(posFirst);
				strArray.SetAt(posFirst, strArray.GetAt(posNext));
				sGudaoname.SetAt(posFirst,sGudaoname.GetAt(posNext));
				xGudaoname.SetAt(posFirst,xGudaoname.GetAt(posNext));
				xGuidaohandle.SetAt(posFirst,xGuidaohandle.GetAt(posNext));
				sGudaohandle.SetAt(posFirst,sGudaohandle.GetAt(posNext));
				//nxhj_Array.SetAt(posFirst,nxhj_Array.GetAt(posNext));
				strArray.SetAt(posNext, temp);
				//nxhj_Array.SetAt(posNext,ntemp);
				sGudaoname.SetAt(posNext,strSgudao);
				xGudaoname.SetAt(posNext,strXgudao);
				sGudaohandle.SetAt(posNext,nShandle);
				xGuidaohandle.SetAt(posNext,nXhangle);
				bComp = TRUE;
			}
			bNotDone |= bComp ;
		}
	}
}
//排序出站兼调车信号机
void CBlkUtility::SortStringArray_CZXHJHandle( CStringArray &strArray,CStringArray &sGudaoname,CLongArray &sGudaohandle,CStringArray &xGudaoname,CLongArray &xGuidaohandle)
{
	/*long t1=GetTickCount();*/
	BOOL bNotDone = TRUE;
	while (bNotDone)
	{
		bNotDone = FALSE;
		for(int pos = 0;pos < strArray.GetUpperBound();pos++)
		{
			BOOL bComp = FALSE ;
			CString temp;
			long ntemp=0;
			CString strSgudao;
			CString strXgudao;
			long nShandle=0;
			long nXhangle=0;
			int posFirst = pos;
			int posNext = pos + 1;
			CString str_first=strArray.GetAt(posFirst);
			//str_first.Remove(del_char);
			str_first.Replace(_T("%%16"),_T(""));
			//int int_fisrt=StrToIntA(str_first);
			CString strNext=strArray.GetAt(posNext);
			//strNext.Remove(del_char);
			strNext.Replace(_T("%%16"),_T(""));
			//int int_next=StrToIntA(strNext);
			int i_value=str_first.CompareNoCase(strNext);
			//比较排序
			if (i_value>0)
			{
				temp = strArray.GetAt(posFirst);
				//ntemp=nxhj_Array.GetAt(posFirst);
				strSgudao=sGudaoname.GetAt(posFirst);
				nShandle=sGudaohandle.GetAt(posFirst);
				strXgudao=xGudaoname.GetAt(posFirst);
				nXhangle=xGuidaohandle.GetAt(posFirst);
				strArray.SetAt(posFirst, strArray.GetAt(posNext));
				sGudaoname.SetAt(posFirst,sGudaoname.GetAt(posNext));
				xGudaoname.SetAt(posFirst,xGudaoname.GetAt(posNext));
				xGuidaohandle.SetAt(posFirst,xGuidaohandle.GetAt(posNext));
				sGudaohandle.SetAt(posFirst,sGudaohandle.GetAt(posNext));
				//nxhj_Array.SetAt(posFirst,nxhj_Array.GetAt(posNext));
				strArray.SetAt(posNext, temp);
				//nxhj_Array.SetAt(posNext,ntemp);
				sGudaoname.SetAt(posNext,strSgudao);
				xGudaoname.SetAt(posNext,strXgudao);
				sGudaohandle.SetAt(posNext,nShandle);
				xGuidaohandle.SetAt(posNext,nXhangle);
				bComp = TRUE;
			}
			bNotDone |= bComp ;
		}
	}
}

// 根据vc版本适应参数的copy函数
void CBlkUtility::tcscpy(TCHAR * dest, SIZE_T length, const TCHAR* src)
{
#if _MSC_VER >= 1400
	::_tcscpy_s(dest, length, src);
#else
	UNREFERENCED_PARAMETER(length);
	::_tcscpy(dest,src);
# endif
}

// 查找字符串strToFind在strSrc中出现位置
int CBlkUtility::FindString(const CString& strSrc, const CString& strToFind, CUIntArray & nArrayLoc)
{
	int iRet = 0 ;

	int iSizeOfSrc = strSrc.GetLength() ;
	int iSizeOfFind = strToFind.GetLength() ;
	int iStart = 0 ;
	BOOL b = TRUE ;
	while(b) 
	{
		if (iStart<iSizeOfSrc) 
		{
			int iLoc = -1 ;
			iLoc = strSrc.Find(strToFind, iStart) ;
			if (iLoc!=-1)
			{
				nArrayLoc.Add(iLoc) ;
				iStart = iLoc+iSizeOfFind ;
				iRet++ ;
			}
			else
			{
				b = FALSE ;
			}
		}
		else
		{
			b = FALSE ;
		}
	} 

	return iRet;
}

BOOL CBlkUtility::IsFolderExist(const CString& strFolder)
{
	BOOL bRet = FALSE;

	WIN32_FIND_DATA   wfd;
	HANDLE hFind = FindFirstFile(strFolder, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		bRet = TRUE;  
	}
	FindClose(hFind);

	return bRet;
}

// 存储icon为文件
void CBlkUtility::SaveIcon(HICON hIcon, const CString& strFilePath)
{
	if(hIcon==NULL || strFilePath.IsEmpty())
		return;
	//warning: this code snippet is not bullet proof.
	//do error check by yourself [masterz]
	PICTDESC picdesc;
	picdesc.cbSizeofstruct = sizeof(PICTDESC);
	picdesc.picType = PICTYPE_ICON ;            
	picdesc.icon.hicon = hIcon;
	IPicture* pPicture=NULL;
	OleCreatePictureIndirect(&picdesc, IID_IPicture, TRUE,(VOID**)&pPicture);
	LPSTREAM pStream;
	CreateStreamOnHGlobal(NULL,TRUE,&pStream);
	LONG size;
	HRESULT hr=pPicture->SaveAsFile(pStream,TRUE,&size);
	TCHAR pathbuf[1024];
	_tcscpy(pathbuf, strFilePath);
	CFile iconfile;
	iconfile.Open(pathbuf, CFile::modeCreate|CFile::modeWrite);
	LARGE_INTEGER li;
	li.HighPart =0;
	li.LowPart =0;
	ULARGE_INTEGER ulnewpos;
	pStream->Seek( li,STREAM_SEEK_SET,&ulnewpos);
	ULONG uReadCount = 1;
	while(uReadCount>0)
	{
		pStream->Read(pathbuf,sizeof(pathbuf),&uReadCount);
		if(uReadCount>0)
		{
			iconfile.Write(pathbuf,uReadCount);
		}
	}
	pStream->Release();
	iconfile.Close();
}
BOOL  CBlkUtility::DoubleToCString( double d_a,CString &str_a )
{
	if(fabs(d_a)<0.001)
	{
		str_a=_T("");
		return FALSE;
	}
	int i_k=0;
	int i_b=0;
	double d_c=0;
	int temp=(int)d_a;
	double dtemp=temp;
	d_c=d_a-dtemp;
	//acutPrintf(_T("\n%f, %4f",d_a,d_c));
	//d_c=d_c*1000;
	 d_c=(d_c*1000.0) ;
	i_k=(int)(d_a)/1000 ;
	i_b=(int)(d_a)%1000;
	str_a.Format(_T("K%02d+%03d.%03.0f"),i_k,i_b,d_c);
	//acutPrintf(_T("\n%s"),str_a);
	return TRUE;
}
BOOL  CBlkUtility::CStringToDouble( CString str_a,double &d_a)
{
	try
	{
		if(str_a==_T(""))
		{
			d_a=0;
			return FALSE;
		}
		//查找K或k，+,_T('.')的位置
		int i_indexK=-1;
		int i_indexPlus=-1;
		int i_indexP=-1;
		if((i_indexK=str_a.Find(_T('K')))==-1)
		{
			if((i_indexK=str_a.Find(_T('k')))==-1)
			{	
				d_a=0;
				return FALSE;
			}
		}
		if((i_indexPlus=str_a.Find(_T('+')))==-1)
		{
			d_a=0;
			return FALSE;
		}
		if((i_indexP=str_a.Find(_T('.')))==-1)
		{
			d_a=0;
			return FALSE;
		}	
		CString strKfw=str_a.Mid(i_indexK+1,i_indexPlus-1);
	
		CString strBfw=str_a.Mid(i_indexPlus+1,i_indexP-1-i_indexPlus);
		CString strDfw=str_a.Mid(i_indexP+1,str_a.GetLength()-i_indexP-1);
		int temp=0;
		temp=StrToIntA(strDfw);
		if(temp==0)
		{
			d_a=StrToIntA(strKfw)*1000+StrToIntA(strBfw)+0.000;
		}
		else
			d_a=StrToIntA(strKfw)*1000+StrToIntA(strBfw)+temp/1000.000 ;
		//acutPrintf(_T("\n %s  %s %s"),strKfw,strBfw,strDfw);
		//acutPrintf(_T(" %d  %d %d"),i_indexK,i_indexPlus,i_indexP);
		return TRUE;
	}
	catch(_com_error& e)
	{
		CString sBuff = CBlkUtility::GetErrorDescription(e);
		AfxMessageBox(sBuff);   
		return FALSE;
	}
	catch(...)
	{	
		AfxMessageBox(_T("未知的错误!in CStringToDouble"));
		return FALSE;
	}		

}

int CBlkUtility::GetCharCountFromCString( const CString &strSrc,TCHAR cDest )
{
	int iRet=0;
	int iLength=strSrc.GetLength();
	for(int i=0;i<iLength;i++)
	{
		TCHAR cTemp=strSrc.GetAt(i);
		if(cTemp==cDest)
			iRet++;
	}
	return iRet;
}

void CBlkUtility::CopyDataVectorFromLongArray( vector<long> &l_vec,CLongArray &nArray )
{
	//l_vec.resize(0)
	for(int i=0;i<nArray.GetCount();i++)
	{
		l_vec.push_back(nArray.GetAt(i));
	}

}

void CBlkUtility::CopyDataLongArrayFromVector( CLongArray &nArray,vector<long> &l_vec )
{
	for(int i=0;i<l_vec.size();i++)
	{
		nArray.Add(l_vec[i]);
	}

}
