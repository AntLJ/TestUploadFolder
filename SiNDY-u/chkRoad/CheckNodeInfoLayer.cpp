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
#include "CheckNodeInfoLayer.h"
#include "INAttrInfo.h"
#include "ErrorInfo.h"

CCheckNodeInfoLayer::CCheckNodeInfoLayer(void) : m_bIsError( false )
{
}

CCheckNodeInfoLayer::~CCheckNodeInfoLayer(void)
{
}

//////////////////////////////////////////////////////////////////////
// ノード情報レイヤをチェック
//////////////////////////////////////////////////////////////////////
bool CCheckNodeInfoLayer::check(CString &rFilePath, int rMeshCode, NInfoAttrList &rNInfoAttr)
{
	// レイヤが開けるか
	ZNInfoLoad aNInfoLoad;
	if( aNInfoLoad.load(rFilePath, rMeshCode ) == false )
	{
		cout << rMeshCode << "\tERROR" << rNInfoAttr.eError[error::node_info::kLayer] << endl;
		return false;
	}

	ErrorNInfo aError;
	aError.eMesh = rMeshCode;
	aError.eError = rNInfoAttr.eError;

	for(int i = 0; i < (int)aNInfoLoad.getCount(); ++i)
	{
		INNInfo* pNInfo = aNInfoLoad[i];
		aError.eID.clear();
		SetID( pNInfo, aError );

		if( rMeshCode == 534041)
			int a=0;

		// Reserved1のチェック
		if( pNInfo->eReserved1 != 0 )
			PrintError( error::node_info::kReserve1, aError, false );

		// Reserved2のチェック
		if( pNInfo->eReserved2 != 0 )
			PrintError( error::node_info::kReserve2, aError, false );

		// Reserved3のチェック
		if( pNInfo->eReserved3 != 0 )
			PrintError( error::node_info::kReserve3, aError, false );

		// Reserved4のチェック
		if( pNInfo->eReserved4 != 0 )
			PrintError( error::node_info::kReserve4, aError, false );

		// Reserved5のチェック
		if( pNInfo->eReserved5 != 0 )
			PrintError( error::node_info::kReserve5, aError, false );

		// Reserved6のチェック
		if( pNInfo->eReserved6 != 0 )
			PrintError( error::node_info::kReserve6, aError, false );

		// ノード列のチェック
		if( !CheckNode( pNInfo, aError))
			PrintError( error::node_info::kNodes, aError, true );

		// ノード情報種別コードのチェック
		set<int>::const_iterator iter_class = rNInfoAttr.eClass.find(pNInfo->eClass);
		if( iter_class == rNInfoAttr.eClass.end() )
			PrintHexError(error::node_info::kInfoClass, aError, pNInfo->eClass, true );

		// 地先種別のチェック
		if(pNInfo->eClass == 0x01000070 || pNInfo->eClass == 0x01000060) // 方面案内(一般道方面案内) または、高速道路方面案内
		{
			CString cstrCode, cstrGuideClass;
			cstrCode.Format("%x", pNInfo->eCode);
			cstrGuideClass = cstrCode.Left(cstrCode.GetLength() -4);
			int iGuideClass = strtol(cstrGuideClass, NULL, 16);
			set<int>::const_iterator iter_dirclass = rNInfoAttr.eGuideClass.find(iGuideClass);
			if( iter_dirclass == rNInfoAttr.eGuideClass.end() )
				PrintHexError(error::node_info::kGuideClass, aError, iGuideClass, true );
		}
		else
		{
			// 方面案内以外で地先種別が入っていないかをチェック
			CString cstrCode, cstrGuideClass;
			cstrCode.Format("%x", pNInfo->eCode);
			cstrGuideClass = cstrCode.Left(cstrCode.GetLength() -4);
			int iGuideClass = strtol(cstrGuideClass, NULL, 16);
			if( iGuideClass != 0 )
				PrintHexError(error::node_info::kBothClass, aError, iGuideClass, true );
		}

		// 案内コードのチェック
		if(pNInfo->eClass == 0x01000090 || pNInfo->eClass == 0x01000091 ) // 強制誘導
		{
			CString cstrCode, cstrGuideCode;
			cstrCode.Format("%x", pNInfo->eCode);
			cstrGuideCode = cstrCode.Right(4);
			int iGuideCode = strtol(cstrGuideCode, NULL, 16);
			set<int>::const_iterator iter_guideclass = rNInfoAttr.eGuideCode.find(iGuideCode);
			if( iter_guideclass == rNInfoAttr.eGuideCode.end() )
				PrintHexError(error::node_info::kGuideCode, aError, iGuideCode, true );
		}
		else
		{
			// 強制誘導以外で案内コードが入っていないかをチェック
			CString cstrCode, cstrGuideCode;
			cstrCode.Format("%x", pNInfo->eCode);
			cstrGuideCode = cstrCode.Right(4);
			int iGuideCode = strtol(cstrGuideCode, NULL, 16);
			if( iGuideCode != 0 )
				PrintHexError(error::node_info::kBothCode, aError, iGuideCode, true );
		}

		// 文字列チェック
		int iNameCount = aNInfoLoad.getNameCount(i);
		CString cstrName(aNInfoLoad.getName(i), iNameCount);

		for(int iCntKanji = 0; iCntKanji < iNameCount; iCntKanji += 2)
		{
			int aTmp = ((int)cstrName[iCntKanji] << 8) & 0x0000ffff;
			int aTmp2 = (int)(cstrName[iCntKanji + 1]) & 0x00ff;
			int aCode = aTmp | aTmp2;

			// 使用OK文字チェック
			if(!(( 0x824f <= aCode && aCode <=0x8258 ) ||	//（全角数字）
				( 0x8260 <= aCode && aCode <=0x8279 ) ||	//（全角大文字英字）
				( 0x8281 <= aCode && aCode <=0x829a ) ||	//（全角小文字英字）
				( 0x829f <= aCode && aCode <=0x82f1 ) ||	//（全角かな）
				( 0x8340 <= aCode && aCode <=0x8394 ) ||	//（全角カナ）
				( 0x889f <= aCode && aCode <=0x9872 ) ||	//（漢字第1水準）
				( 0x989f <= aCode && aCode <=0xeaa4 )		//（漢字第2水準）
				)){
					// 使用OK特殊文字チェック
					if(0x01000060 == pNInfo->eClass || 0x01000070 == pNInfo->eClass){	// 方面案内、案内看板の場合
						if(rNInfoAttr.eUseOKCharDirCode.end() == rNInfoAttr.eUseOKCharDirCode.find(aCode)){
							PrintStringError( error::node_info::kKanjiNone, aError, cstrName, true);
							break;
						}
					} else {	// 方面案内以外の場合（現在は交差点名称のみ）今後必要であれば処理を分ける
						if(rNInfoAttr.eUseOKCharNodeCode.end() == rNInfoAttr.eUseOKCharNodeCode.find(aCode)){
							PrintStringError( error::node_info::kKanjiNoneN, aError, cstrName, true);
							break;
						}
					}
			}
		}

		// カナ文字列チェック
		int iYomiCount = aNInfoLoad.getYomiCount(i);
		CString aYomiName(aNInfoLoad.getYomi(i), iYomiCount);

		for(int iCntYomi = 0; iCntYomi < iYomiCount; ++iCntYomi )
		{
			int aChar = (int)aYomiName[iCntYomi] & 0x00ff;
			if(!(0x00a6 <= aChar && aChar <= 0x00df)){
				// 使用OK半角文字チェック
				if(!(aChar == 0x00a5 ||	// '･'
					aChar == 0x002e ||	// '.'
					aChar == 0x007e ||	// '~'
					aChar == 0x0028 ||	// '('
					aChar == 0x0029		// ')'
					)){
						if(pNInfo->eClass == 0x01000060 || pNInfo->eClass == 0x01000070 ) { // 方面案内、案内看板の場合
							if(!(	aChar == 0x00a4 || // '､'
									aChar == 0x002d    // '-'
								)) {
									PrintStringError( error::node_info::kKanaNone, aError, aYomiName, true );
									break;
							}
						} else {
							PrintStringError( error::node_info::kKanaNone, aError, aYomiName, true );
							break;
						}
				}
			}
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// エラー対象のノードID集合を入力する。
//////////////////////////////////////////////////////////////////////
void CCheckNodeInfoLayer::SetID(INNInfo *pNInfo, ErrorNInfo &rError)
{
	CString cstrFrom, cstrID, cstrTo;
	if( pNInfo->eFrom != 0 )
	{
		SetObjectID( cstrFrom, pNInfo->eFrom );
		rError.eID.push_back(cstrFrom);
	}
	SetObjectID( cstrID, pNInfo->eID );
	rError.eID.push_back( cstrID );

	if( pNInfo->eCount > 0 )
	{
		for( int i = 0; i < pNInfo->eCount; ++i )
		{
			SetObjectID( cstrTo, pNInfo->eList[i].eNode );
			rError.eID.push_back(cstrTo);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値有り:文字列)
//////////////////////////////////////////////////////////////////////
void CCheckNodeInfoLayer::PrintStringError(int rKey, ErrorNInfo &rError, CString& rCode, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	for( int i = 0; i < (int)rError.eID.size(); ++i )
		cout<< "\t" << static_cast<LPCTSTR>(rError.eID[i]);
	cout << "\t" << static_cast<LPCTSTR>(rCode) << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(属性値有り:16進数)
//////////////////////////////////////////////////////////////////////
void CCheckNodeInfoLayer::PrintHexError(int rKey, ErrorNInfo &rError, int rCode, bool bError)
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
void CCheckNodeInfoLayer::PrintError(int rKey, ErrorNInfo &rError, int rCode, bool bError)
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
void CCheckNodeInfoLayer::PrintError(int rKey, ErrorNInfo &rError, bool bError)
{
	char* aError = (bError) ? "\tERROR" : "\tWARNING";
	cout << rError.eMesh << aError << rError.eError[rKey];
	for( int i = 0; i < (int)rError.eID.size(); ++i )
		cout<< "\t" << static_cast<LPCTSTR>(rError.eID[i]);
	cout << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// ノード列の重複チェック
//////////////////////////////////////////////////////////////////////
bool CCheckNodeInfoLayer::CheckNode(INNInfo *pNInfo, ErrorNInfo& rError)
{
	if( pNInfo->eFrom == pNInfo->eID )
		return false;

	typedef pair<int,int>	NodeData;

	// NodeSet		･･･ ノード数比較用
	// NodeSetCheck	･･･ 最終ノード重複判定用
	set<NodeData>			NodeSet, NodeSetCheck;

	if( pNInfo->eFrom > 0 )
	{
		NodeSet.insert(NodeData(pNInfo->eFrom, rError.eMesh));
		NodeSetCheck.insert(NodeData(pNInfo->eFrom, rError.eMesh));
	}
	NodeSet.insert(NodeData(pNInfo->eID, rError.eMesh));
	NodeSetCheck.insert(NodeData(pNInfo->eID, rError.eMesh));

	for(int i = 0; i < pNInfo->eCount; ++i)
	{
		// 最終点以外で重複が無いかをチェックする。
		if( i!= pNInfo->eCount - 1 )
		{
			NodeData aNode(pNInfo->eList[i].eNode, pNInfo->eList[i].eMesh);
			set<NodeData>::iterator iter_set = NodeSet.find(aNode);
			if( iter_set != NodeSet.end() )
				return false;
			NodeSetCheck.insert(aNode);
		}

		// メッシュ境界ノードのIDが隣接メッシュで同一かをチェック
		if( i> 0 && pNInfo->eList[i-1].eNode != pNInfo->eList[i].eNode &&
			pNInfo->eList[i-1].eMesh != pNInfo->eList[i].eMesh )
			return false;

		// メッシュ境界ノード間で行き来していないかをチェック
		if( i>1 && pNInfo->eList[i-2].eNode == pNInfo->eList[i].eNode )
			return false;

		// 終点と終点一つ前の点のIDが同一でないかをチェック
		if( i>0 && i == pNInfo->eCount - 1 && pNInfo->eList[i-1].eNode == pNInfo->eList[i].eNode )
			return false;

		// 調査済み流出ノード情報を格納
		NodeSet.insert(NodeData(pNInfo->eList[i].eNode, pNInfo->eList[i].eMesh));
	}

	// 格納した情報数が終点を除いたノード数と一致するかをチェック
	if( pNInfo->eFrom > 0 && (int)NodeSet.size() == pNInfo->eCount + 2 )
		return true;
	else if( pNInfo->eFrom == 0 && (int)NodeSet.size() == pNInfo->eCount + 1)
		return true;
	else
	{
		NodeData aNode(pNInfo->eList[pNInfo->eCount-1].eNode, pNInfo->eList[pNInfo->eCount-1].eMesh);
		set<NodeData>::iterator iter_check = NodeSetCheck.find(aNode);
		if( iter_check == NodeSetCheck.end() )
			return false;
	}

	return true;
}
