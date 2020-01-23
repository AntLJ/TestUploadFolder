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
#include "CheckNodeLayer.h"
#include "Z2NodeLoad.h"
#include "INAttrInfo.h"
#include "ErrorInfo.h"
#include "SetRoadAttrList.h"

CCheckNodeLayer::CCheckNodeLayer(void) : m_bIsError( false )
{
}

CCheckNodeLayer::~CCheckNodeLayer(void)
{
}

//////////////////////////////////////////////////////////////////////
// 道路ノードレイヤをチェック
//////////////////////////////////////////////////////////////////////
bool CCheckNodeLayer::check(CString &rFilePath, int rMeshCode, NodeAttrList &rNodeAttr)
{
	Z2NodeLoad aNodeLoad;
	if( aNodeLoad.load(rFilePath, rMeshCode) == false )
	{
		cout << rMeshCode << "\tERROR" << rNodeAttr.eError[error::node::kLayer] << endl;
		return false;
	}

	ErrorNode aError;
	aError.eError = rNodeAttr.eError;
	aError.eMesh = rMeshCode;

	for(int i = 0; i < (int)aNodeLoad.getCount(); ++i)
	{
		IN2Node* pNode = aNodeLoad[i];
		SetObjectID(aError.eID, pNode->eID);

		// 28bit(2^28(268,435,456))以上のデータの場合はNG
		if(pNode->eID & 0xf0000000)
			PrintHexError( error::node::kNodeID, aError, pNode->eID, true );

		// 信号機フラグのチェック
		set<int>::iterator iter_signal = rNodeAttr.eSignal.find( pNode->eSignal );
		if( iter_signal == rNodeAttr.eSignal.end() )
			PrintError( error::node::kSignal, aError, pNode->eSignal, true);

		// 種別コードのチェック
		set<int>::iterator iter_class = rNodeAttr.eNodeClass.find( pNode->eClass );
		if( iter_class == rNodeAttr.eNodeClass.end() )
			PrintHexError( error::node::kClass, aError, pNode->eClass, true );

		// 正規化X座標
		if(pNode->eX < 0 || pNode->eX > 1000000)
			PrintError( error::node::kXY, aError, pNode->eX, true );

		// 正規化Y座標
		if(pNode->eY < 0 || pNode->eY > 1000000)
			PrintError( error::node::kXY, aError, pNode->eY, true );

		// 道路高さ有効フラグ
		set<int>::iterator iter_height = rNodeAttr.eHeight.find( pNode->eRoadAvail );
		if( iter_height == rNodeAttr.eHeight.end() )
			PrintError( error::node::kHeight1, aError, pNode->eRoadAvail, true );

		// 道路相対高
		if( pNode->eRoadAvail == 0 && pNode->eRoadLevel != 0 )
			PrintError( error::node::kHeight2, aError, pNode->eRoadLevel, true );

		// Reserved1のチェック
		if( pNode->eReserved1 != 0 )
			PrintError( error::node::kReserve1, aError, false );

		// Reserved2のチェック
		if( pNode->eReserved2 != 0 )
			PrintError( error::node::kReserve2, aError, false );

		// Reserved4のチェック
		if( pNode->eReserved4 != 0 )
			PrintError( error::node::kReserve4, aError, false );

		// Reserved5のチェック
		if( pNode->eReserved5 != 0 )
			PrintError( error::node::kReserve5, aError, false );

		// 接続リンク本数のチェック
		if( pNode->eJointCount < 1 || pNode->eJointCount > 8 )
		{
			PrintError( error::node::kConnectCount, aError, pNode->eJointCount, true );
			continue;
		}

		for( int iCount = 0; iCount < pNode->eJointCount; ++ iCount )
		{
			// 接続ノードチェック
			IN2Node* aTemp = aNodeLoad.search( pNode->eJoint[iCount].eID );
			if( !aTemp )
				PrintHexError( error::node::kConnectNode, aError, pNode->eJoint[iCount].eID, true );

			// ノードID(24->28bit)対応	2010/5/12
			// 上位4bitはReservedになるので値が入っている場合はNG
			long aReserved = pNode->eJoint[iCount].eReserved;
			if(aReserved != 0)
				PrintHexError( error::node::kConnectNodeIDReserved, aError, pNode->eJoint[iCount].eReserved, true );
			// 28bit(2^28(268,435,456))以上のデータの場合はNG
			if(pNode->eJoint[iCount].eID & 0xf0000000)
				PrintHexError( error::node::kConnectNodeID, aError, pNode->eJoint[iCount].eID, true );
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値有り:16進数)
//////////////////////////////////////////////////////////////////////
void CCheckNodeLayer::PrintHexError(int rKey, ErrorNode &rError, int rCode, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	CString cstrCode;
	SetObjectID(cstrCode, rCode);
	cout << rError.eMesh << aError << rError.eError[rKey] << "\t";
	cout << static_cast<LPCTSTR>(rError.eID) << "\t" << static_cast<LPCTSTR>(cstrCode) << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値有り:10進数)
//////////////////////////////////////////////////////////////////////
void CCheckNodeLayer::PrintError(int rKey, ErrorNode &rError, int rCode, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey] << "\t";
	cout << static_cast<LPCTSTR>(rError.eID) << "\t" << rCode << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値なし)
//////////////////////////////////////////////////////////////////////
void CCheckNodeLayer::PrintError(int rKey, ErrorNode &rError, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey] << "\t";
	cout << static_cast<LPCTSTR>(rError.eID) << endl;

	m_bIsError = true;
}
