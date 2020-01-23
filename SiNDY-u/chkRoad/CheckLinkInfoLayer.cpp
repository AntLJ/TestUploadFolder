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
#include "CheckLinkInfoLayer.h"
#include "ZRInfoLoad.h"
#include "INAttrInfo.h"
#include "ErrorInfo.h"

CCheckLinkInfoLayer::CCheckLinkInfoLayer(void)
{
}

CCheckLinkInfoLayer::~CCheckLinkInfoLayer(void)
{
}

//////////////////////////////////////////////////////////////////////
// リンク情報レイヤをチェック
//////////////////////////////////////////////////////////////////////
bool CCheckLinkInfoLayer::check(CString &rFilePath, int rMeshCode, LInfoAttrList &rLInfoAttr)
{
	// レイヤが作成されていないかをチェックする。作成されていたら警告を表示する。
	// このレイヤは2001年春データより使用していない。
	ZRInfoLoad aLInfoLoad;
	if( aLInfoLoad.load(rFilePath, rMeshCode) == true )
	{
		cout << rMeshCode << "\tWARNING\t" << rLInfoAttr.eError[error::link_info::kLayer] << endl;
		return false;
	}
	else
		return true;

	ErrorLInfo aError;
	aError.eMesh = rMeshCode;
	aError.eError = rLInfoAttr.eError;

	for(int i = 0; i < (int)aLInfoLoad.getCount(); ++i)
	{
		INRInfo* pLInfo = aLInfoLoad[i];
		SetObjectID(aError.eFrom, pLInfo->eID1);
		SetObjectID(aError.eTo, pLInfo->eID2);

		// 主路線::表示種別をチェック
		set<int>::const_iterator iter_maindisp = rLInfoAttr.eDisp.find(pLInfo->eMainInfo.eClass);
		if( iter_maindisp == rLInfoAttr.eDisp.end() )
			PrintHexError( error::link_info::kMainDisp, aError, pLInfo->eMainInfo.eClass, true ); 

		// 主路線::Reserve をチェック
		if( pLInfo->eMainInfo.eReserved != 0 )
			PrintError( error::link_info::kMainReserve, aError, false );

		// 主路線::主従区分コードをチェック
		set<int>::const_iterator iter_mainsection = rLInfoAttr.eSection.find(pLInfo->eMainInfo.eCode);
		if( iter_mainsection == rLInfoAttr.eSection.end() )
			PrintError( error::link_info::kMainSection, aError, pLInfo->eMainInfo.eCode, true ); 

		// リンク長をチェック
		if( pLInfo->eLength == 0) 
			PrintError( error::link_info::kLength, aError, false );

		// 規制速度をチェック
		set<int>::const_iterator iter_speed = rLInfoAttr.eSpeed.find(pLInfo->eLimitSpeed);
		if( iter_speed == rLInfoAttr.eSection.end() )
			PrintError( error::link_info::kSpeed, aError, pLInfo->eLimitSpeed, true );

		// 全ての値が0で無いかをチェック
		if( pLInfo->eMainInfo.eRoadNo && pLInfo->eMainInfo.eCode == 0 &&
			pLInfo->eWidth == 0 && pLInfo->eCenterArea == 0 &&
			pLInfo->eTripSpeed == 0 && pLInfo->eLimitSpeed == 0 )
			PrintError( error::link_info::kAllNone, aError, true);

		for( int iCount = 0; iCount < pLInfo->eCount; ++iCount )
		{
			// 重要路線::表示種別コードのチェック
			set<int>::const_iterator iter_subdisp = rLInfoAttr.eDisp.find(pLInfo->eInfo[iCount].eClass);
			if( iter_subdisp == rLInfoAttr.eDisp.end() )
				PrintHexError( error::link_info::kSubDisp, aError, pLInfo->eInfo[iCount].eClass, true ); 

			// 重要路線::Reserveのチェック
			if( pLInfo->eInfo[iCount].eReserved != 0 )
				PrintError( error::link_info::kSubReserve, aError, false );

			// 重要路線::主従区分のチェック
			set<int>::const_iterator iter_subsection = rLInfoAttr.eSection.find(pLInfo->eInfo[iCount].eCode);
			if( iter_subsection != rLInfoAttr.eSection.end() )
				PrintError( error::link_info::kSubSection, aError, pLInfo->eInfo[iCount].eCode, true );
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値有り:16進数)
//////////////////////////////////////////////////////////////////////
void CCheckLinkInfoLayer::PrintHexError(int rKey, ErrorLInfo &rError, int rCode, bool bError)
{
	CString cstrCode;
	SetObjectID(cstrCode, rCode);

	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	cout<< "\t" << static_cast<LPCTSTR>(rError.eFrom);
	cout<< "\t" << static_cast<LPCTSTR>(rError.eTo);
	cout << "\t" << static_cast<LPCTSTR>(cstrCode) << endl;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値有り:10進数)
//////////////////////////////////////////////////////////////////////
void CCheckLinkInfoLayer::PrintError(int rKey, ErrorLInfo &rError, int rCode, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	cout<< "\t" << static_cast<LPCTSTR>(rError.eFrom);
	cout<< "\t" << static_cast<LPCTSTR>(rError.eTo);
	cout << "\t" << rCode << endl;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値無し)
//////////////////////////////////////////////////////////////////////
void CCheckLinkInfoLayer::PrintError(int rKey, ErrorLInfo &rError, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	cout<< "\t" << static_cast<LPCTSTR>(rError.eFrom);
	cout<< "\t" << static_cast<LPCTSTR>(rError.eTo) << endl;
}
