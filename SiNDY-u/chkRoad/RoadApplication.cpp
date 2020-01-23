/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#include "StdAfx.h"
#include "RoadApplication.h"
#include "CheckINHead.h"
#include "CheckLinkLayer.h"
#include "CheckNodeLayer.h"
#include "CheckRegLayer.h"
#include "CheckNodeInfoLayer.h"
#include "CheckLaneLayer.h"
#include "CheckLinkInfoLayer.h"
#include "CheckLinkExLayer.h"
#include "SetRoadAttrList.h"

CRoadApplication::CRoadApplication(void)
{
}

CRoadApplication::~CRoadApplication(void)
{
}

//////////////////////////////////////////////////////////////////////
// INデータのエラーチェック
//////////////////////////////////////////////////////////////////////
long CRoadApplication::run(CString& rConfig, LPCTSTR lpcszCustomer, bool bLatest )
{
	long lReturn = 0;

	GetConf(rConfig);
	Customer eCustomer = GetCustomer( lpcszCustomer );				// 仕向け先決定
	m_setDataMeshlist = CreateList( m_INPathData.eMntDir );			// data用メッシュリスト
	m_setLatestMeshlist = CreateList( m_INPathData.eLatestDir );	// B版以降用メッシュリスト
	if( m_setLatestMeshlist.empty() )
		m_setLatestMeshlist = m_setDataMeshlist;

	AttrList aAttrList;
	CSetRoadAttrList aTotalAttr;

	// pol_code.txtが存在しない場合は、異常終了を返す
	if( !aTotalAttr.setList( m_INPathData.ePolDir, aAttrList ))
		return 1;

	// .mntが指定フォルダ以下に一つも存在しない場合は、異常終了を返す
	if( (int)m_setDataMeshlist.size() == 0 )
	{
		cout << "# ERROR\t指定パス(MNT_DIR)以下に道路系INデータが存在しません。\t" << static_cast<LPCTSTR>(m_INPathData.eMntDir) << endl;
		return 1;
	}

	for( set<int>::iterator p = m_setDataMeshlist.begin(); p != m_setDataMeshlist.end(); ++ p )
	{
		cerr << *p << "をチェック中・・・・・" << endl;
		// INデータのヘッダ情報をチェック
		CCheckINHead aCheckHead;
		if( !aCheckHead.check( m_INPathData.eMntDir, *p, aAttrList.eINHead ) )
			lReturn = 1;

		if( lReturn != 1 && aCheckHead.IsError() )
			lReturn = 3;

		// 道路リンクレイヤをチェック
		CCheckLinkLayer aCheckLink;
		if( !aCheckLink.check( m_INPathData.eMntDir, *p, aAttrList.eLinkLayer, eCustomer, m_setLatestMeshlist ) )
			lReturn = 1;

		if( lReturn != 1 && aCheckLink.IsError() )
			lReturn = 3;

		// 道路ノードレイヤをチェック
		CCheckNodeLayer aCheckNode;
		if( !aCheckNode.check( m_INPathData.eMntDir, *p, aAttrList.eNodeLayer ) )
			lReturn = 1;

		if( lReturn != 1 && aCheckNode.IsError() )
			lReturn = 3;

		// 新交通規制レイヤをチェック
		CCheckRegLayer aCheckReg;
		if( !aCheckReg.check( m_INPathData.eMntDir, *p, aAttrList.eRegLayer ) )
			lReturn = 1;

		if( lReturn != 1 && aCheckReg.IsError() )
			lReturn = 3;

		// ノード情報レイヤをチェック
		CCheckNodeInfoLayer aCheckNInfo;
		if( !aCheckNInfo.check( m_INPathData.eMntDir, *p, aAttrList.eNInfoLayer ) )
			lReturn = 1;

		if( lReturn != 1 && aCheckNInfo.IsError() )
			lReturn = 3;

		// レーンレイヤをチェック
		CCheckLaneLayer aCheckLane;
		if( !aCheckLane.check( m_INPathData.eMntDir, *p, aAttrList.eLaneLayer ) )
			lReturn = 1;

		if( lReturn != 1 && aCheckReg.IsError() )
			lReturn = 3;

		// リンク情報レイヤをチェック
		// 現在リリースされないレイヤ
		CCheckLinkInfoLayer aCheckLInfo;
		if( !aCheckLInfo.check( m_INPathData.eMntDir, *p, aAttrList.eLInfoLayer ) )
			lReturn = 1;

		// リンク拡張レイヤをチェック
		CCheckLinkExLayer aCheckLinkEx;
		if( !aCheckLinkEx.check( m_INPathData.eMntDir, *p, aAttrList.eLinkExLayer ) )
			lReturn = 1;

		if( lReturn != 1 && aCheckLinkEx.IsError() )
			lReturn = 3;
	}

	return lReturn;
}

//////////////////////////////////////////////////////////////////////
// Configファイルの設定読み込み
//////////////////////////////////////////////////////////////////////
void CRoadApplication::GetConf(CString &rConfig)
{
	ifstream ifs(rConfig);
	char rStrLine[1024] ={0};
	while( ifs.getline( rStrLine, sizeof(rStrLine) ) )
	{
		CString csLine = rStrLine;
		if( _tcscmp( csLine.Left(1), _T("#") ) == 0 )
			continue;
		if( strstr( csLine, _T("MNT_DIR")) != 0 )
		{
			csLine.Delete(0,7);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			m_INPathData.eMntDir = csLine;
		}
		else if( strstr( csLine, _T("POL_DIR")) != 0 )
		{
			csLine.Delete(0,7);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			m_INPathData.ePolDir = csLine;
		}
		else if( strstr( csLine, _T("LATEST_DIR")) != 0 )
		{
			csLine.Delete(0,10);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			m_INPathData.eLatestDir = csLine;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 仕向け先決定
//////////////////////////////////////////////////////////////////////
Customer CRoadApplication::GetCustomer( LPCTSTR lpcszCustomer )
{
	Customer eCustomer;
	if( _tcscmp( lpcszCustomer, _T("PEC") ) == 0 || _tcscmp( lpcszCustomer, _T("TR") ) == 0 )
		eCustomer = PEC;
	else
		eCustomer = OTHER;

	return eCustomer;
}

//////////////////////////////////////////////////////////////////////
// INデータの2次メッシュリスト作成
//////////////////////////////////////////////////////////////////////
std::set<int> CRoadApplication::CreateList( LPCTSTR lpcszMntDir )
{
	std::set<int> setRet;

	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(lpcszMntDir, &fd);
	if( hFind != INVALID_HANDLE_VALUE )
	{
		if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			CString cstrFind;
			cstrFind.Format(_T("%s\\*"), lpcszMntDir );
			WIN32_FIND_DATA fd2;
			HANDLE hFind2 = FindFirstFile(cstrFind, &fd2);
			FindFile( hFind2, fd2,lpcszMntDir, setRet );
		}
	}
	FindClose(hFind);

	return setRet;
}

void CRoadApplication::FindFile( HANDLE hFind, WIN32_FIND_DATA &fd, LPCTSTR lpPath, std::set<int>& setMesh )
{
	do {
		if( _tcscmp( fd.cFileName, _T(".") ) == 0 || _tcscmp( fd.cFileName, _T("..") ) == 0 )
			continue;

		if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			CString cstrPath;
			cstrPath.Format(_T("%s\\%s"), lpPath, fd.cFileName);
			CString cstrFind = cstrPath + CString(_T("\\*"));

			WIN32_FIND_DATA fd2;
			HANDLE hFind2 = FindFirstFile(cstrFind, &fd2);
			FindFile(hFind2, fd2, cstrPath, setMesh);
			FindClose(hFind2);

		} 
		else 
		{
			// mntファイルのみで検索
			if( _tcsstr( fd.cFileName, _T(".mnt") ) != NULL )
			{
				if( CString(fd.cFileName).GetLength() == 10 )
					setMesh.insert(_ttoi(CString(fd.cFileName).Left(6)));
			}
		}
	} while (FindNextFile(hFind, &fd));
}
