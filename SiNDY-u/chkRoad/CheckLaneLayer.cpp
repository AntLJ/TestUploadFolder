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
#include "CheckLaneLayer.h"
#include "INAttrInfo.h"
#include "ErrorInfo.h"

CCheckLaneLayer::CCheckLaneLayer(void) : m_bIsError( false )
{
}

CCheckLaneLayer::~CCheckLaneLayer(void)
{
}

//////////////////////////////////////////////////////////////////////
// レーン情報レイヤをチェック
//////////////////////////////////////////////////////////////////////
bool CCheckLaneLayer::check(CString &rFilePath, int rMeshCode, LaneAttrList &rLaneAttr)
{
	// レイヤが開けるか
	ZLaneLoad aLaneLoad;
	if( aLaneLoad.load(rFilePath, rMeshCode) == false )
	{
		cout << rMeshCode << "\tERROR" << rLaneAttr.eError[error::lane::kLayer] << endl;
		return false;
	}

	ErrorLane aError;
	aError.eMesh = rMeshCode;
	aError.eError = rLaneAttr.eError;

	for(int i = 0; i < (int)aLaneLoad.getCount(); ++i)
	{
		INLane* pLane = aLaneLoad[i];
		aError.eID.clear();
		SetID( pLane, aError );

		// Reserved1のチェック
		if( pLane->eReserved1 != 0 )
			PrintError( error::lane::kReserve1, aError, false );

		// Reserved2のチェック
		if( pLane->eReserved2 != 0 )
			PrintError( error::lane::kReserve2, aError, false );

		// 流出先レーン数(Reserved3)のチェック
		if( pLane->eToCount != 0)
			PrintError( error::lane::kReserve3, aError, false );

		// 左側増減レーン数のチェック
		if( pLane->eLeftDiff < -32 || 32 < pLane->eLeftDiff )
			PrintError( error::lane::kLeftDiff, aError, pLane->eLeftDiff, true ); 

		// 右側増減レーン数のチェック
		if( pLane->eRightDiff < -32 || 32 < pLane->eRightDiff )
			PrintError( error::lane::kRightDiff, aError, pLane->eRightDiff, true ); 

		// 交差点進入レーン数のチェック
		if( pLane->eFromCount <= 0 || 32 < pLane->eFromCount )
			PrintError( error::lane::kFromCount, aError, pLane->eFromCount, true );

		// レーン情報のチェック
		unsigned int  aMask = 0xffffffff >> pLane->eFromCount;
		if((pLane->eInfo & aMask ) != 0 ) 
			PrintHexError( error::lane::kInfo, aError, pLane->eInfo, true );
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値有り:16進数)
//////////////////////////////////////////////////////////////////////
void CCheckLaneLayer::PrintHexError(int rKey, ErrorLane &rError, int rCode, bool bError)
{
	CString cstrCode;
	SetObjectID( cstrCode, rCode );

	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	for( int i = 0; i < (int)rError.eID.size(); ++i )
		cout<< "\t" << static_cast<LPCTSTR>(rError.eID[i]);
	cout << "\t" << static_cast<LPCTSTR>(cstrCode) << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値有り:10進数)
//////////////////////////////////////////////////////////////////////
void CCheckLaneLayer::PrintError(int rKey, ErrorLane &rError, int rCode, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	for( int i = 0; i < (int)rError.eID.size(); ++i )
		cout<< "\t" << static_cast<LPCTSTR>(rError.eID[i]);
	cout << "\t" << rCode << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値無し)
//////////////////////////////////////////////////////////////////////
void CCheckLaneLayer::PrintError(int rKey, ErrorLane &rError, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	for( int i = 0; i < (int)rError.eID.size(); ++i )
		cout<< "\t" << static_cast<LPCTSTR>(rError.eID[i]);
	cout << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// エラー対象のノードID集合を入力する。
//////////////////////////////////////////////////////////////////////
void CCheckLaneLayer::SetID(INLane *pLane, ErrorLane &rError)
{
	CString cstrFrom, cstrID, cstrTo;
	if( pLane->eFrom != 0 )
	{
		SetObjectID( cstrFrom, pLane->eFrom );
		rError.eID.push_back(cstrFrom);
	}
	SetObjectID( cstrID, pLane->eID );
	rError.eID.push_back(cstrID);

	if( pLane->eCount > 0 )
	{
		for( int i = 0; i < pLane->eCount; ++i )
		{
			SetObjectID( cstrTo, pLane->eNode[i].eNode );
			rError.eID.push_back(cstrTo);
		}
	}
}
