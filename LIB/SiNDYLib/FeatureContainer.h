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
 * @file FeatureContainer.h
 * @brief <b>CFeatureContainerクラス定義ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @vertion $Id$
 */
#ifndef _FEATURECONTAINER_H_
#define _FEATURECONTAINER_H_

#include "Container.h"
#include "FeatureClass.h"

namespace sindy {
/**
 * @class CFeatureContainer
 * @brief <b>フィーチャを格納する汎用コンテナクラス</b>\n
 * このクラスは、単一種類の複数のフィーチャを格納するためのコンテナです。\n
 * このクラスで、複数のフィーチャを一括に処理することができます。\n
 * 他のフィーチャと特に関連しないものに関しては、このクラスをベースに
 * 処理を行ってください。
 */
class SINDYLIB_API CFeatureContainer :
	public CContainer,
	public CFeatureClass
{
public:
	// CModelContainContainerから直接 m_listFeature を扱いたいが、
	// protectedにすると継承して使用できてしまう恐れがあるためやむなく
	friend class CModelContainContainer;

	CFeatureContainer(){}
	CFeatureContainer( sindyTableType::ECode emType ){ m_emTableType = emType; }
	CFeatureContainer( IFeatureClass* lp ) : CFeatureClass( lp ){}
	CFeatureContainer( ITable* lp ) : CFeatureClass( lp ){}
	virtual ~CFeatureContainer(){}

	sindyTableType::ECode GetCurrentTableType() const {return m_emTableType;}
	//@{ @name CFeatureContainerImpl インターフェース定義

	/**
	 * @brief <b>形状で検索する</b>\n
	 * 内部でバッファリングなどはしないので、ポイントで検索する場合は
	 * バッファリングした形状を渡してください。\n
	 * @param ipGeom		[in]			検索形状
	 * @param bRecycling	[in,optional]	trueにするとキャッシュを作成して切り離し
	 * @param bCreateCache	[in,optional]	キャッシュを作成する（bRecyclingがtrueの場合はこのフラグに関係なく必ず作成される）
	 * @param pErrs			[out,optional]	エラー格納コンテナ（オブジェクト同士の関連付けに失敗したもののみ入る）
	 *
	 * @return sindyErrCode	エラーがある場合はcErrsのGetDefaultErrorCodeの結果が返る
	 */
	virtual errorcode::sindyErrCode Select( IGeometry* ipGeom, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true );
	errorcode::sindyErrCode Select( IGeometry* ipGeom, const type_info& info, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true );
	errorcode::sindyErrCode Select( IQueryFilter* ipFileter, const type_info& info, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true );
	errorcode::sindyErrCode Select( LPCTSTR lpcszTargetTableName, const std::list<long> listIDs, const type_info& info, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true, LPCTSTR lpcszWhereFieldName = NULL, LPCTSTR lpcszParentTableName = NULL );
	errorcode::sindyErrCode Add( _ICursor* ipCursor, const type_info& info, sindyTableType::ECode emType, const CSPFieldMap& cFields, const CSPTableNameString& cNames, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true, LPCTSTR lpcszWhereFieldName = NULL, LPCTSTR lpcszParentTableName = NULL );
	/**
	 * @brief <b>形状で検索する</b>\n
	 * 内部でバッファリングなどはしないので、ポイントで検索する場合は
	 * バッファリングした形状を渡してください。\n
	 * @param ipGeom		[in]			検索形状
	 * @param bRecycling	[in,optional]	trueにするとキャッシュを作成して切り離し
	 * @param bCreateCache	[in,optional]	キャッシュを作成する（bRecyclingがtrueの場合はこのフラグに関係なく必ず作成される）
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode SelectByObject( _IRow* ipRow, bool bRecycling = false, bool bCreateCache = false, bool bInit = true );
};

} // sindy

#endif // ifndef _FEATURECONTAINER_H_
