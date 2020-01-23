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

#include "stdafx.h"
#include "msi_handle_ws.h"


/**
	ファイル内
*/
bool	msi_handle_ws::
Init	(	IWorkspacePtr	c_ipWSpace,		///< オープンしたワークスペース
			char*			c_cpLayer	)	///< レイヤ名称
{
	HRESULT					hr;

	long	a_lName_Len	= 0;

	a_lName_Len	= strlen ( c_cpLayer );

	e_cpLayer	= new char[a_lName_Len+1];
	if( e_cpLayer == NULL ) {
		return ( false );
	}
	strcpy ( e_cpLayer, c_cpLayer );

	IFeatureWorkspacePtr	a_FeWspace	( c_ipWSpace );	// フィーチャワークスペース

	// フィーチャ（レイヤ？）のオープン
	hr	= a_FeWspace->OpenFeatureClass ( CComBSTR ( c_cpLayer ), &e_ipFeCls );
	if( FAILED( hr ) ) {
		fprintf	( stderr, "レイヤ<%s>のオープンに失敗\n", c_cpLayer );
		return	( false );
	}else {
		fprintf	( stdout,	"<%s> レイヤをオープン\n", c_cpLayer );
	}

	return ( true );
}

/**
	指定フィーチャに対してesriSpatialRelEnumで指定した関係を持つデータを
	とってくる
*/
IFeatureCursorPtr	msi_handle_ws::
Search_Feature	(	IGeometryPtr		c_ipGeo,			///< 指定ジオメトリ
					esriSpatialRelEnum	c_ipSpatialRel,		///< 関係指定
					std::string			c_sWhere,			///< Where句をつける
					int					c_iRetType		)	///< 何を返すか
{
	HRESULT				hr;

	ISpatialFilterPtr	a_ipFilter(CLSID_SpatialFilter);
	CComBSTR			a_ccSpCol;

	hr	= e_ipFeCls->get_ShapeFieldName ( &a_ccSpCol );
	if (FAILED(hr)) {
		Print_Err	( "シェイプフィールド名が取得できない", "Search_Feature" );
		return	( NULL );
	}

	hr	= a_ipFilter->putref_Geometry	( c_ipGeo );
	if (FAILED(hr))	{
		Print_Err	( "サーチ用のジオメトリが設定できない", "Search_Feature" );
		return	( NULL );
	}

	hr	= a_ipFilter->put_GeometryField	( a_ccSpCol );
	if (FAILED(hr)) {
		Print_Err	( "ジオメトリフィールドが設定できない", "Search_Feature" );
		return	( NULL );
	}

	hr	= a_ipFilter->put_SpatialRel	( c_ipSpatialRel );	// 指定シェイプと隣接するもの
	if (FAILED(hr)) {
		Print_Err	( "SpatialRel()が設定できない", "Search_Feature" );
		return	( NULL );
	}
	hr	= a_ipFilter->put_SearchOrder	( esriSearchOrderSpatial );
	if (FAILED(hr))	{
		Print_Err	( "サーチオーダーの設定ができない", "Search_Feature" );
		return	( NULL );
	}

	// Where句が１文字だとなんかやばそうなので
	if( c_sWhere.size() > 1 ) {

		char	a_tmpstr[1024];	//　ここはなんとかしたい気がする
		sprintf	( a_tmpstr, "%s", &c_sWhere[0]);	// メッシュコードを文字列に
		_bstr_t	a_Where_Str ( a_tmpstr );

		hr	= a_ipFilter->put_WhereClause	( a_Where_Str );
		if ( FAILED ( hr ) )	{
			Print_Err	( "検索用WHERE句の設定ができない", "Search_Feature" );
			return		( NULL );
		}
	}

	// データカウントをやる
	if( c_iRetType & RET_COUNT ) {
		//---------------------------------------------------------
		long		a_lData_Count	= 0;
		hr	= e_ipFeCls->FeatureCount ( (IQueryFilterPtr)a_ipFilter, &a_lData_Count );
		if ( FAILED ( hr ) )	{
			Print_Err	( "検索データ数が取得できない", "Search_Feature" );
			return	( NULL );
		}
		e_lData_Count	= a_lData_Count;
		//---------------------------------------------------------
	}
	IFeatureCursorPtr	a_ipFeCsr;		///< 比較元フィーチャカーソル
	if( c_iRetType & RET_FCURSOR ) {// 比較元データを取得する（クエリ自体はあまり意味を持たず、ファイル内データを全部取る）
		hr = e_ipFeCls->Search((IQueryFilterPtr)a_ipFilter, VARIANT_FALSE, &a_ipFeCsr);
		if (FAILED(hr)) {
			Print_Err	( "データのサーチができない", "Search_Feature" );
			return	( NULL );
		}
	}
	return	( a_ipFeCsr );
}

/**
	エラーを出力する
	@return	無し
*/
void	msi_handle_ws::
Print_Err	(	char*	c_cpAddMsg,		///< 基本エラー情報に追加したいメッセージ
				char*	c_cpFuncName )	///< 関数名
{
	fprintf	( stderr,	"<%s>レイヤ ： %s : [msi_handle_ws::%s]\n",
						e_cpLayer,
						c_cpAddMsg,
						c_cpFuncName );
}

