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

//	IFeatureCursorPtr		e_ipFeCsr;		///< 比較元フィーチャカーソル

	////
	IFeatureWorkspacePtr	a_FeWspace	( c_ipWSpace );	// フィーチャワークスペース

	// フィーチャ（レイヤ？）のオープン
	hr	= a_FeWspace->OpenFeatureClass ( CComBSTR ( c_cpLayer ), &e_ipFeCls );
	if( FAILED( hr ) ) {
		fprintf	( stderr, "レイヤ<%s>のオープンに失敗\n", c_cpLayer );
		return	( false );
	}else {
		fprintf	( stdout,	"<%s> レイヤをオープン\n", c_cpLayer );
	}	
	

	// オンメモリテスト 2002.6.24
/*	IDatasetPtr					a_ipDataset( e_ipFeCls );
	IWorkspacePtr				a_ipWSpace;
	hr	= a_ipDataset->get_Workspace	( &a_ipWSpace );
	if( FAILED( hr ) ) {
		fprintf	(stderr, "ワークスペースの獲得に失敗<msi_handle_ws>\n", c_cpLayer );
		return	(false);
	}

	ISpatialCacheManagerPtr		ipSpCacheMgr( a_ipWSpace );
	// 矩形で指定した領域を、ローカルにキャッシュ
	ipSpCacheMgr->FillCache		( m_Clip_Extent );
*/
/*	CComBSTR	a_cSpatialCol;
	hr	= e_ipFeCls->get_ShapeFieldName	( &a_cSpatialCol );
	if( FAILED( hr ) ) {
		fprintf	(stderr, "フィールド名のオープンに失敗<msi_handle_ws>\n" );
		return	(false);
	}
*/	

	return ( true );
}

/**
	比較先データを、比較元データのオブジェクトIDでサーチする
*/
IFeatureCursorPtr	msi_handle_ws::
Search_ID	(	double			c_dObj_ID	)	///< 指定オブジェクトID
{

	HRESULT				hr;

	IQueryFilterPtr		a_ipFilter(CLSID_QueryFilter);
	CComBSTR			a_ccSpCol;

	BSTR		a_bWhere_Str;
	
	char		a_tmpstr[64];
	sprintf	( a_tmpstr, "OBJECTID = %0.1f", c_dObj_ID );
	a_bWhere_Str	= T2BSTR(_T(a_tmpstr));		// stringをBSTRに変換

	hr	= a_ipFilter->put_WhereClause	( a_bWhere_Str );
	if (FAILED(hr))	{
		fprintf	( stderr, "検索用WHERE句の設定ができない[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	// データカウントをやる
	//---------------------------------------------------------
	long		a_lData_Count	= 0;
	hr	= e_ipFeCls->FeatureCount ( a_ipFilter, &a_lData_Count );
	if ( FAILED ( hr ) )	{
		fprintf	( stderr, "検索データ数が取得できない<msi_handle_ws::Search_Where>\n");
		return	( NULL );
	}
	e_lData_Count	= a_lData_Count;
	//---------------------------------------------------------

	IFeatureCursorPtr	a_ipFeCsr;		///< 比較元フィーチャカーソル
	// 比較元データを取得する（クエリ自体はあまり意味を持たず、ファイル内データを全部取る）
	hr = e_ipFeCls->Search(a_ipFilter, VARIANT_FALSE, &a_ipFeCsr);
	if (FAILED(hr)) {
		fprintf	( stderr, "データのサーチができない[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	return	( a_ipFeCsr );
}
	
/**
	比較先データを、比較元データのshapeでサーチする
*/
IFeatureCursorPtr	msi_handle_ws::
Search_shp	(	IGeometryPtr	c_ipGeo		)	///< 指定ジオメトリ
{

	HRESULT				hr;

	ISpatialFilterPtr	a_ipFilter(CLSID_SpatialFilter);
	CComBSTR			a_ccSpCol;

	hr	= e_ipFeCls->get_ShapeFieldName ( &a_ccSpCol );
	if (FAILED(hr)) {
		fprintf	( stderr, "シェイプフィールド名が取得できない[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	hr	= a_ipFilter->putref_Geometry	( c_ipGeo );
	if (FAILED(hr))	{
		fprintf	( stderr, "サーチ用のジオメトリが設定できない[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	hr	= a_ipFilter->put_GeometryField	( a_ccSpCol );
	if (FAILED(hr)) {
		fprintf	( stderr, "ジオメトリフィールドが設定できない[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	BSTR		a_bRel_Str;
	char		a_tmpstr[64];
	sprintf		( a_tmpstr, "TFFFTFFFT" );
//	sprintf		( a_tmpstr, "TTFTTFFFT" );
	a_bRel_Str	= T2BSTR(_T(a_tmpstr));		// stringをBSTRに変換

//	hr	= a_ipFilter->put_SpatialRel	( esriSpatialRelRelation );		// これをやらないとRelDescriptionは使えない！！！
//	hr	= a_ipFilter->put_SpatialRelDescription ( a_bRel_Str );
	hr	= a_ipFilter->put_SpatialRel	( esriSpatialRelIntersects );	// メッシュポリゴンと重なるもの
	if (FAILED(hr)) {
		fprintf	( stderr, "ジオメトリフィールドが取得できない[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}
	hr	= a_ipFilter->put_SearchOrder( esriSearchOrderSpatial );
	if (FAILED(hr))	{
		fprintf	( stderr, "サーチオーダーの設定ができない[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	// データカウントをやる
	//---------------------------------------------------------
	long		a_lData_Count	= 0;
	hr	= e_ipFeCls->FeatureCount ( (IQueryFilterPtr)a_ipFilter, &a_lData_Count );
	if ( FAILED ( hr ) )	{
		fprintf	( stderr, "検索データ数が取得できない<msi_handle_ws::Search_Where>\n");
		return	( NULL );
	}
	e_lData_Count	= a_lData_Count;
	//---------------------------------------------------------

	IFeatureCursorPtr	a_ipFeCsr;		///< 比較元フィーチャカーソル
	// 比較元データを取得する（クエリ自体はあまり意味を持たず、ファイル内データを全部取る）
	hr = e_ipFeCls->Search((IQueryFilterPtr)a_ipFilter, VARIANT_FALSE, &a_ipFeCsr);
	if (FAILED(hr)) {
		fprintf	( stderr, "データのサーチができない[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	return	( a_ipFeCsr );
}

/**
	指定フィーチャに対してesriSpatialRelEnumで指定した関係を持つデータを
	とってくる
*/
IFeatureCursorPtr	msi_handle_ws::
Search_Feature	(	IGeometryPtr		c_ipGeo,			///< 指定ジオメトリ
					esriSpatialRelEnum	c_ipSpatialRel,		///< 関係指定
					std::string			c_sWhere,			///< 指定WHERE句
					int					c_iRetType		)	///< 何を返すか
{

	HRESULT				hr;

	ISpatialFilterPtr	a_ipFilter(CLSID_SpatialFilter);
	CComBSTR			a_ccSpCol;

	hr	= e_ipFeCls->get_ShapeFieldName ( &a_ccSpCol );
	if (FAILED(hr)) {
		fprintf	( stderr, "シェイプフィールド名が取得できない[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	hr	= a_ipFilter->putref_Geometry	( c_ipGeo );
	if (FAILED(hr))	{
		fprintf	( stderr, "サーチ用のジオメトリが設定できない[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	hr	= a_ipFilter->put_GeometryField	( a_ccSpCol );
	if (FAILED(hr)) {
		fprintf	( stderr, "ジオメトリフィールドが設定できない[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	hr	= a_ipFilter->put_SpatialRel	( c_ipSpatialRel );	// 指定シェイプと隣接するもの
	if (FAILED(hr)) {
		fprintf	( stderr, "SpatialRel()が設定できない[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}
	hr	= a_ipFilter->put_SearchOrder	( esriSearchOrderSpatial );
	if (FAILED(hr))	{
		fprintf	( stderr, "サーチオーダーの設定ができない[Shp_Dat_Search::Search]\n");
		return	( NULL );
	}

	// Where句が１文字だとなんかやばそうなので
	if( c_sWhere.size() > 1 ) {

		char	a_tmpstr[1024];	//　ここはなんとかしたい気がする
		sprintf	( a_tmpstr, "%s", &c_sWhere[0]);	// メッシュコードを文字列に
		_bstr_t	a_Where_Str ( a_tmpstr );

		hr	= a_ipFilter->put_WhereClause	( a_Where_Str );
		if ( FAILED ( hr ) )	{
			fprintf	( stderr, "検索用WHERE句の設定ができない[Shp_Dat_Search::Search]" );
			return	( NULL );
		}
	}

	// データカウントをやる
	if( c_iRetType & RET_COUNT ) {
		//---------------------------------------------------------
		long		a_lData_Count	= 0;
		hr	= e_ipFeCls->FeatureCount ( (IQueryFilterPtr)a_ipFilter, &a_lData_Count );
		if ( FAILED ( hr ) )	{
			fprintf	( stderr, "検索データ数が取得できない<msi_handle_ws::Search_Where>\n");
			return	( NULL );
		}
		e_lData_Count	= a_lData_Count;
		//---------------------------------------------------------
	}
	IFeatureCursorPtr	a_ipFeCsr;		///< 比較元フィーチャカーソル
	if( c_iRetType & RET_FCURSOR ) {// 比較元データを取得する（クエリ自体はあまり意味を持たず、ファイル内データを全部取る）
		hr = e_ipFeCls->Search((IQueryFilterPtr)a_ipFilter, VARIANT_FALSE, &a_ipFeCsr);
		if (FAILED(hr)) {
			fprintf	( stderr, "データのサーチができない[Shp_Dat_Search::Search]\n");
			return	( NULL );
		}
	}
	return	( a_ipFeCsr );
}


/**
	WHERE句を指定してデータを取ってくる
	@return	IFeatureCursorPtr
*/
IFeatureCursorPtr	msi_handle_ws::
Search_Where	(	std::string		c_sWhere,		///< 指定WHERE句
					int				c_iOrder	)	///< ソートするかのしてい（１=ソート、その他=ソートしない）
{
	HRESULT			hr;
	
	BSTR			a_bWhere_Str;
	char*			a_tmpstr	= &c_sWhere[0];
	a_bWhere_Str	= T2BSTR( _T(a_tmpstr) );		// stringをBSTRに変換

	IQueryFilterPtr		a_ipQ_Filter	( CLSID_QueryFilter );		// 空間検索フィルタ
	
	hr	= a_ipQ_Filter->put_WhereClause	( a_bWhere_Str );
	if ( FAILED ( hr ) )	{
		fprintf	( stderr, "検索用WHERE句の設定ができない<msi_handle_ws::Search_Where>\n");
		return	( NULL );
	}

	// データカウントをやる
	//---------------------------------------------------------
	long		a_lData_Count	= 0;
	hr	= e_ipFeCls->FeatureCount ( a_ipQ_Filter, &a_lData_Count );
	if ( FAILED ( hr ) )	{
		fprintf	( stderr, "検索データ数が取得できない<msi_handle_ws::Search_Where>\n");
		return	( NULL );
	}
	e_lData_Count	= a_lData_Count;
	//---------------------------------------------------------


	// サーチしてくる
	IFeatureCursorPtr	a_ipFeCursor;
	if( c_iOrder != 1 ) {	// ソートしない

		if( FAILED(e_ipFeCls->Search	( a_ipQ_Filter, VARIANT_FALSE, &a_ipFeCursor )) ) {
			fprintf	( stderr, "データのQUERY検索に失敗\n" );
			return	( NULL );
		}

	}else {					// オブジェクトIDでソートする

		ITableSortPtr						a_ipTableSort ( CLSID_TableSort ); 
		a_ipTableSort->put_Fields			( T2BSTR( _T("OBJECTID") ) );
		a_ipTableSort->putref_Table			( ITablePtr(e_ipFeCls) );
		a_ipTableSort->putref_QueryFilter	( a_ipQ_Filter );
		a_ipTableSort->Sort					( NULL );
		/// 検索
		ICursorPtr	a_ipCursor;
		a_ipTableSort->get_Rows				( &a_ipCursor );
		a_ipFeCursor	= IFeatureCursorPtr	( a_ipCursor );
	}

	return	( a_ipFeCursor );
}
