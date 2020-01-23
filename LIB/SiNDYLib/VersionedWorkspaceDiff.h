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
 * @file VersionedWorkspaceDiff.h
 * @brief CWorkspaceクラス定義ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * $Id$
 */
#ifndef _VERSIONEDWORKSPACEDIFF_H_
#define _VERSIONEDWORKSPACEDIFF_H_

#include "Workspace.h"
#include "ErrorCode.h"
#include <map>
#include <list>

namespace sindy {
class CRow;
class SINDYLIB_API CVersionedWorkspaceDiff : public CWorkspace
{
public:
	/**
	 * @enum sindyDifferenceType
	 * @brief <b>esriDifferenceTypeを複数指定できるようナンバリングし直したもの</b>\n
	 * Differencesの引数として使用します。
	 */
	enum sindyDifferenceType {
		sindyDifferenceTypeInsert = 1,
		sindyDifferenceTypeDeleteNoChange = 2,
		sindyDifferenceTypeUpdateNoChange = 4,
		sindyDifferenceTypeUpdateUpdate = 8,
		sindyDifferenceTypeUpdateDelete = 16,
		sindyDifferenceTypeDeleteUpdate = 32,
	};

	explicit CVersionedWorkspaceDiff();
	virtual ~CVersionedWorkspaceDiff();

	explicit CVersionedWorkspaceDiff( IWorkspace* lp )
	{
		*this = lp;
	}
	explicit CVersionedWorkspaceDiff( LPCTSTR lpcszConnectString )
	{
		Connect( lpcszConnectString );
	}
	CVersionedWorkspaceDiff& operator=( IWorkspace* lp )
	{
		AtlComPtrAssign( (IUnknown**)&p, lp );
		m_spNameString.reset( (CWorkspaceNameString*)NULL );
		clear();

		return *this;
	}

	/**
	 * @brief <b>ワークスペース間での差分を取得する</b>\n
	 * @param cWork			[in]			比較ワークスペース
	 * @param mapDiff		[out]			取得された差分
	 * @param ipProgress	[in,optional]	進捗確認プログレス（デフォルト：NULL）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Differences( IFeatureWorkspace* ipWork, sindyDifferenceType emDiffType, IProgressor* ipProgress = NULL );

	/**
	 * @brief <b>取得差分にコンフリクトが含まれるかどうかをチェックする</b>\n
	 * @retval true コンフリクトがある
	 * @retval false コンフリクトなし
	 */
	bool HasConflict() const;

	typedef std::list<CRow> CRowList;
	typedef std::map<esriDifferenceType,CRowList> CDiffRowMap;
	//@{ @name イテレータ
	typedef CRowList::const_iterator const_iterator;
	const_iterator end( esriDifferenceType emType ) const;
	const_iterator begin( esriDifferenceType emType ) const;
	//@}
	void clear();
private:
	errorcode::sindyErrCode Differences( IEnumDatasetName* ipDatasetNames, IFeatureWorkspace* ipWork, sindyDifferenceType emDiffType, IProgressor* ipProgress = NULL );
	long DiffCur2Map( esriDifferenceType emType, IDifferenceCursor* ipCursor, IFields* ipFields );
private:
#pragma warning(push)
#pragma warning(disable:4251)
	CDiffRowMap m_mapDiff;	// 差分格納用
#pragma warning(pop)
};

} // sindy

#endif // ifndef _VERSIONEDWORKSPACEDIFF_H_
