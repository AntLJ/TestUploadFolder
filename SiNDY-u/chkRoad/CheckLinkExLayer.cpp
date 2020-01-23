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
#include "CheckLinkExLayer.h"
#include "ZLExtLoad.h"
#include "INAttrInfo.h"
#include "ErrorInfo.h"

CCheckLinkExLayer::CCheckLinkExLayer(void) : m_bIsError( false )
{
}

CCheckLinkExLayer::~CCheckLinkExLayer(void)
{
}

//////////////////////////////////////////////////////////////////////
// リンク拡張レイヤをチェック
//////////////////////////////////////////////////////////////////////
bool CCheckLinkExLayer::check(CString &rFilePath, int rMeshCode, LinkExAttrList &rLinkExAttr)
{
	// レイヤが開けるかをチェックする。
	ZLExtLoad aLExtLoad;
	if( aLExtLoad.load(rFilePath, rMeshCode) == false )
	{
		cout << rMeshCode << "\tERROR" << rLinkExAttr.eError[error::linkex::kLayer] << endl;
		return false;
	}

	ErrorLExt aError;
	aError.eMesh = rMeshCode;
	aError.eError = rLinkExAttr.eError;

	for(int i = 0; i < (int)aLExtLoad.getCount(); ++i)
	{
		INLExt* pLExt = aLExtLoad[i];
		SetObjectID( aError.eFrom, pLExt->eID1 );
		SetObjectID( aError.eTo, pLExt->eID2 );
		
		for(int iInfoCnt = 0; iInfoCnt < pLExt->eCount; ++iInfoCnt )
		{
			// キーIDをチェック
			set<int>::const_iterator iter_key = rLinkExAttr.eKey.find( pLExt->eInfo[iInfoCnt].eKey );
			if( iter_key == rLinkExAttr.eKey.end() )
			{
				PrintError(error::linkex::kKeyID, aError, pLExt->eInfo[iInfoCnt].eKey, true );
				continue;
			}

			// リンク情報をチェック
			CheckLinkInfo(pLExt->eInfo[iInfoCnt].eKey, aError, rLinkExAttr, pLExt->eInfo[iInfoCnt].eCode ); 
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値有り:16進数)
//////////////////////////////////////////////////////////////////////
void CCheckLinkExLayer::PrintHexError(int rKey, ErrorLExt &rError, int rCode, bool bError)
{
	CString cstrCode;
	SetObjectID(cstrCode, rCode);

	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	cout<< "\t" << static_cast<LPCTSTR>(rError.eFrom);
	cout<< "\t" << static_cast<LPCTSTR>(rError.eTo);
	cout << "\t" << static_cast<LPCTSTR>(cstrCode) << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値有り:10進数)
//////////////////////////////////////////////////////////////////////
void CCheckLinkExLayer::PrintError(int rKey, ErrorLExt &rError, int rCode, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	cout<< "\t" << static_cast<LPCTSTR>(rError.eFrom);
	cout<< "\t" << static_cast<LPCTSTR>(rError.eTo);
	cout << "\t" << rCode << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値無し)
//////////////////////////////////////////////////////////////////////
void CCheckLinkExLayer::PrintError(int rKey, ErrorLExt &rError, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	cout<< "\t" << static_cast<LPCTSTR>(rError.eFrom);
	cout<< "\t" << static_cast<LPCTSTR>(rError.eTo) << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// リンク情報をチェック
//////////////////////////////////////////////////////////////////////
void CCheckLinkExLayer::CheckLinkInfo(int rKey, ErrorLExt &rError, LinkExAttrList& rLinkExAttr, int rCode)
{
	switch( rKey )
	{
	case in::road::lext_key::kubun_code:	// 主従区分チェック
		{
			set<int>::const_iterator iter_section = rLinkExAttr.eSection.find( rCode );
			if( iter_section == rLinkExAttr.eSection.end() )
				PrintError(error::linkex::kSection, rError, rCode, true);
		}
		break;
	case in::road::lext_key::limit_speed:	// 規制速度チェック
		{
			set<int>::const_iterator iter_speed = rLinkExAttr.eSpeed.find( rCode );
			if( iter_speed == rLinkExAttr.eSpeed.end() )
				PrintError(error::linkex::kSpeed, rError, rCode, true );
		}
		break;
	case in::road::lext_key::island_flag:	// 浮島フラグチェック
		{
			set<int>::const_iterator iter_island = rLinkExAttr.eIsland.find( rCode );
			if( iter_island == rLinkExAttr.eIsland.end() )
				PrintError(error::linkex::kIsLand, rError, rCode, true );
		}
		break;
	case in::road::lext_key::area_flag:		// 細街路エリア判別フラグチェック
		{
			set<int>::const_iterator iter_area = rLinkExAttr.eDetail.find( rCode );
			if( iter_area == rLinkExAttr.eDetail.end() )
				PrintHexError(error::linkex::kArea, rError, rCode, true );
		}
		break;
	case in::road::lext_key::trip_speed:	// 旅行速度(チェック無し)
	case in::road::lext_key::oneside_lane:	// 片側車線情報(チェック無し)
		break;
	default:
		break;
	}
}
