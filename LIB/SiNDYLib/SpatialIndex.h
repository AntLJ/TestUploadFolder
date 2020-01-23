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
 * @file SpatialIndex.h
 * @brief <b>CSpatialIndexクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @vertion $Id$
 */
#ifndef _SPATIALINDEX_H_
#define _SPATIALINDEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AheWKSFunctions.h"
#include "ErrorCode.h"
#include <list>

namespace sindy {

/**
 * @class CSpatialIndex
 * @brief <b>空間インデックスクラス</b>\n
 * このクラスは擬似的な空間インデックスの作成・管理を
 * するクラスです。
 * ArcObjectsの空間インデックスとは関係ありません。
 */
class SINDYLIB_API CSpatialIndex
{
public:
	explicit CSpatialIndex()
	{
		m_cRect.XMin = m_cRect.YMin = m_cRect.XMax = m_cRect.YMax = 0;
		m_nDivideX = m_nDivideY = 0;
		m_pIndex = NULL;
	}
	virtual ~CSpatialIndex()
	{
		DeleteIndex();
	}

	/**
	 * @brief <b>空間インデックスのサイズを指定する</b>\n
	 * 既存のインデックスは削除されます。
	 * @param nMeshCode	[in]	メッシュコード
	 * @param nDivideX	[in]	矩形の横分割数
	 * @param nDivideY	[in]	矩形の縦分割数
	 */
	void Init( int nMeshCode, int nDivideX, int nDivideY );

	/**
	 * @brief <b>空間インデックスのサイズを指定する</b>\n
	 * 既存のインデックスは削除されます。
	 * @param cRect		[in]	インデックス作成対象矩形
	 * @param nDivideX	[in]	インデックス作成対象矩形の横分割数
	 * @param nDivideY	[in]	インデックス作成対象矩形の縦分割数
	 */
	void Init( const WKSEnvelope& cRect, int nDivideX, int nDivideY )
	{
		DeleteIndex();
		m_cRect = cRect;
		m_nDivideX = nDivideX;
		m_nDivideY = nDivideY;
		CreateIndex();
	}

	/**
	 * @brief <b>インデックスに要素を追加する</b>\n
	 * @param cRect		[in]	インデックスを作成したい形状を包含する矩形
	 * @param lID		[in]	インデックスを作成したい形状のID
	 */
	errorcode::sindyErrCode Add( const WKSEnvelope& cRect, long lID );

	/**
	 * @brief <b>インデックスからIDを検索する</b>\n
	 * @param cRect		[in]	検索矩形
	 * @param listIDs	[out]	インデックスに引っかかったIDのリスト
	 * @retval listIDsの個数
	 */
	long Search( const WKSEnvelope& cRect, std::list<long>& listIDs ) const;
#ifdef _DEBUG
	void Trace() const
	{
		for( int i = 0; i < m_nDivideX; ++i )
		{
			for( int j = 0; j < m_nDivideY; ++j )
				ATLTRACE(_T("m_pIndex[%d][%d] = %d\n"), i, j, m_pIndex[i][j].size() );
		}
	}
#endif // ifdef _DEBUG
private:
	/**
	 * @brief <b>初期インデックスを作成する</b>\n
	 */
	void CreateIndex();

	/**
	 * @brief <b>インデックスを削除する</b>\n
	 */
	void DeleteIndex();

	/**
	 * @brief <b>インデックスループのXYの開始・終了インデックス番号を取得する</b>\n
	 * @param d			[in]	比較対象
	 * @param bIsX		[in]	X座標値かどうか
	 * @param bFlag		[in]	trueならインデックスの境である場合に小さいほうを返す
	 * @retval -1	管理メッシュ対象外
	 * @retval 0以上 インデックスループのインデックス番号
	 */
	int GetIndex( const double& d, const bool bIsX, const bool bFlag, double* dMin = NULL, double* dMax = NULL, int* nStart = NULL, int* nEnd = NULL ) const;

private:
	int					m_nDivideX;		//!< インデックス作成対象矩形の横分割数
	int					m_nDivideY;		//!< インデックス作成対象矩形の縦分割数
	WKSEnvelope			m_cRect;		//!< インデックス作成対象矩形
	std::list<long>**	m_pIndex;		//!< インデックスリスト
};

} // sindy

#endif // ifndef _SPATIALINDEX_H_
