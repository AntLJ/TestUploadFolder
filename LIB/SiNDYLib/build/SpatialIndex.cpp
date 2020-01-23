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

/**
 * @file SpatialIndex.cpp
 * @brief <b>CSpatialIndexクラス実装ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @vertion $Id$
 */
#include "stdafx.h"
#include "SpatialIndex.h"
#include "meshutil.h"
#include "util.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

// 空間インデックスのサイズを指定する
void CSpatialIndex::Init( int nMeshCode, int nDivideX, int nDivideY )
{
	// メッシュコード->矩形
	MESH2ENV( nMeshCode, m_cRect.XMin, m_cRect.YMin, m_cRect.XMax, m_cRect.YMax );
	Init( m_cRect, nDivideX, nDivideY );
}

// インデックスに要素を追加する
sindyErrCode CSpatialIndex::Add( const WKSEnvelope& cRect, long lID )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	// 配列の開始・終了インデックス番号取得
	int nStartX = GetIndex( cRect.XMin, true, true );
	int nEndX   = GetIndex( cRect.XMax, true, false );
	int nStartY = GetIndex( cRect.YMin, false, true );
	int nEndY   = GetIndex( cRect.YMax, false, false );

	LOGASSERTE_IF( nStartX >= 0 && nEndX >= 0 && nStartY >= 0 && nEndY >= 0, sindyErr_AlgorithmFailed )
	{
		for( int i = nStartX; i <= nEndX; ++i )
		{
			for( int j = nStartY; j <= nEndY; ++j )
				m_pIndex[i][j].push_back( lID );
		}
	}

	return emErr;
}

// インデックスからIDを検索する
long CSpatialIndex::Search( const WKSEnvelope& cRect, std::list<long>& listIDs ) const
{
	// 初期化
	listIDs.clear();

	// 配列の開始・終了インデックス番号取得
	int nStartX = GetIndex( cRect.XMin, true, true );
	int nEndX   = GetIndex( cRect.XMax, true, false );
	int nStartY = GetIndex( cRect.YMin, false, true );
	int nEndY   = GetIndex( cRect.YMax, false, false );

	if( nStartX >= 0 && nEndX >= 0 && nStartY >= 0 && nEndY >= 0 )
	{
		for( int i = nStartX; i <= nEndX; ++i )
		{
			for( int j = nStartY; j <= nEndY; ++j )
				listIDs.insert( listIDs.end(), m_pIndex[i][j].begin(), m_pIndex[i][j].end() );
		}
	}
	// ユニークにする
	listIDs.sort();
	listIDs.unique();

	return listIDs.size();
}

// 初期インデックスを作成する
void CSpatialIndex::CreateIndex()
{
	// 初期化
	m_pIndex = new std::list<long>*[m_nDivideX];
	for( int i = 0; i < m_nDivideX; ++i )
		m_pIndex[i] = new std::list<long>[m_nDivideY];
}

// インデックスを削除する
void CSpatialIndex::DeleteIndex()
{
	if( m_pIndex )
	{
		for( int i = 0; i < m_nDivideX; ++i )
		{
			for( int j = 0; j < m_nDivideY; ++j )
				m_pIndex[i][j].clear();
			delete [] m_pIndex[i];
		}
		delete [] m_pIndex;
		m_pIndex = NULL;
	}
}

// インデックスループのXYの開始・終了インデックス番号を取得する
int CSpatialIndex::GetIndex( const double& d, const bool bIsX, const bool bFlag, double* dMin/* = NULL*/, double* dMax/* = NULL*/, int* nStart/* = NULL*/, int* nEnd/* = NULL*/ ) const
{
	int nRet = -1;	// 返り値

	bool bFirst = false;
	if( ! dMin && ! dMax && ! nStart && ! nEnd )
	{
		dMin = new double;
		*dMin = ( bIsX ) ? m_cRect.XMin : m_cRect.YMin;
		dMax = new double; *dMax = ( bIsX ) ? m_cRect.XMax : m_cRect.YMax;
		nStart = new int;  *nStart = 0;
		nEnd   = new int;  *nEnd   = ( bIsX ) ? m_nDivideX-1 : m_nDivideY-1;
		bFirst = true;
	}
	
	if( *dMin <= d && d <= *dMax )
	{
		// 最小値と
		if( *dMin == d )
		{
//			if( bFlag )
				nRet = *nStart;
//			else
//				nRet = *nStart;
		}
		else if( *dMax == d )
		{
//			if( bFlag )
				nRet = *nEnd;
//			else
//				nRet = ( *nEnd == m_nDivide-1 || ) ? *nEnd : *nEnd+1;
		}
		else if( *nStart == *nEnd )
			nRet = *nStart;
		else if( *nEnd-*nStart == 1 )
			nRet = ( bFlag ) ? *nStart : *nEnd;
		else {
			// dMin - dMax の前半・後半でチェック
			double dHalf = (*dMax-*dMin)/(double)2.0;
			div_t divt = div(*nEnd+*nStart,2);
			double dTmp = *dMin + dHalf;								// 前半の場合はMax、後半の場合はMinとして使用される
			int nStartTmp = (divt.rem == 0 ) ? divt.quot : divt.quot+1;	// 後半の場合のStartとして使用される
			int nEndTmp   = (divt.rem == 0 ) ? nStartTmp : nStartTmp-1;	// 前半の場合のEndとして使用される

			SASSERT( nStartTmp >= 0 && nEndTmp >= 0 && nStartTmp >= nEndTmp && *nStart <= nEndTmp && nStartTmp <= *nEnd );

			// 前半でチェック
			int nBefore = GetIndex( d, bIsX, bFlag, dMin, &dTmp, nStart, &nEndTmp );
			// 後半でチェック
			int nAfter  = GetIndex( d, bIsX, bFlag, &dTmp, dMax, &nStartTmp, nEnd );

			// 前半と後半で取得できたインデックス番号を比較して、正しいほうを採用する
			if( nBefore < 0 && nAfter < 0 )
			{
				ERRORLOG(sindyErr_AlgorithmFailed); // アルゴリズムエラー
				SASSERT(false);
			}
			else {
				if( nBefore < 0 )
					nRet = nAfter;
				else if( nAfter < 0 )
					nRet = nBefore;
				else {
					if( bFlag )
						nRet = min( nBefore, nAfter );
					else
						nRet = max( nBefore, nAfter );
				}
			}
		}
	}

	if( bFirst )
	{
		delete dMin;
		delete dMax;
		delete nStart;
		delete nEnd;
	}

	return nRet;
}

} // sindy
