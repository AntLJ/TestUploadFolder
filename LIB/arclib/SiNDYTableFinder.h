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

// SiNDYTableFinder.h: SiNDYTableFinder クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYTABLEFINDER_H__2A38CACC_E465_4557_9F24_D34CEC1EED3F__INCLUDED_)
#define AFX_SINDYTABLEFINDER_H__2A38CACC_E465_4557_9F24_D34CEC1EED3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief テーブル取得系基底クラス
 */
class SiNDYTableFinder  
{
public:
	/**
	 * @brief 指定したテーブル名に相当するテーブルを取得する
	 *
	 * @note テーブル名称は
	 * @note -# テーブル名のみ
	 * @note -# ユーザ名付きテーブル名
	 * @note -# レイヤ名
	 * @note のいずれにも対応できるよう実装するのが望ましい。
	 * @note 最低限1と2は必須である。
	 *
	 * @param lpszTableName [in] テーブル名称
	 * @return テーブル。取得できなかった場合NULL。
	 */
	virtual ITablePtr getTable(LPCTSTR lpszTableName) = 0;

	virtual IWorkspacePtr getWorkspace(LPCTSTR lpszTableName);
	virtual _IRowPtr getRow(LPCTSTR lpszTableName, long nObjectId);
	virtual IGeometryPtr getBaseMeshGeometry(long nMeshcode);
};

#endif // !defined(AFX_SINDYTABLEFINDER_H__2A38CACC_E465_4557_9F24_D34CEC1EED3F__INCLUDED_)
