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
#include "Smp_db_obj_attr.h"

/**
	構造体にSdeRowからデータをセットする
	@return	bool
*/
bool	Smp_db_obj_attr::
Attr_Set_AOBJ	(	IFeaturePtr		c_Feature )		///< フェッチしてきたフィーチャ[IN]
{					
	LONG			a_Col_Size	= 0; 
	IFieldsPtr		a_Col_Defs;					// カラムの定義
	c_Feature->get_Fields		( &a_Col_Defs );
	a_Col_Defs->get_FieldCount	( &a_Col_Size );
	
	LONG		i = 0;
	CComBSTR	a_Col_Name;
	for( i = 0 ; i < a_Col_Size ; i++ )
	{
		IFieldPtr	a_Col_Def;
		
		CComVariant	a_Value;
		a_Col_Defs->get_Field	( i, &a_Col_Def );
		a_Col_Name.Empty();

		esriFieldType	a_Col_Type;				// フィールドタイプ
		a_Col_Def->get_Type	( &a_Col_Type );	// カラムタイプ
		a_Col_Def->get_Name	( &a_Col_Name );	// カラム名称
		c_Feature->get_Value( i, &a_Value );	// 属性値の取得

		if(a_Col_Name == O_ID) {	// オブジェクトID
			e_Obj_ID		= a_Value.intVal;
		}else
		if( (a_Col_Name == DSC1_F) || (a_Col_Name == SC1DT_C) ){	// 表示スケール１（ベース、ミドル、トップ）背景・注記
			e_Sc1_Disp_Type_C= a_Value.intVal;
		}else
		if( (a_Col_Name == DSC2_F) || (a_Col_Name == SC2DT_C) ){	// 表示スケール２
			e_Sc2_Disp_Type_C= a_Value.intVal;
		}else
		if( (a_Col_Name == DSC3_F) || (a_Col_Name == SC3DT_C) || (a_Col_Name == DST_C) ){	// 表示スケール３（都市地図注記も対応）
			e_Sc3_Disp_Type_C= a_Value.intVal;
		}else
		if( (a_Col_Name == DSC4_F) || (a_Col_Name == SC4DT_C) ){	// 表示スケール４
			e_Sc4_Disp_Type_C= a_Value.intVal;
		}else
		if( a_Col_Name == SC1BGC_C ){	// 表示種別コード・スケール１（ベース、ミドル、トップ）背景
			e_Sc1_Disp_Code_C	= a_Value.intVal;
			e_Cur_Attr_Cls		= BG_C;
		}else
		if( a_Col_Name == SC2BGC_C ){	// 表示種別コード・表示スケール２
			e_Sc2_Disp_Code_C	= a_Value.intVal;
			e_Cur_Attr_Cls		= BG_C;
		}else
		if( a_Col_Name == SC3BGC_C ){	// 表示種別コード・表示スケール３
			e_Sc3_Disp_Code_C	= a_Value.intVal;
			e_Cur_Attr_Cls		= BG_C;
		}else
		if( a_Col_Name == SC4BGC_C ){	// 表示種別コード・表示スケール４
			e_Sc4_Disp_Code_C	= a_Value.intVal;
			e_Cur_Attr_Cls		= BG_C;
		}else
		if(a_Col_Name == UNDER_F){				// 地上・地下フラグ（鉄道関係）
			e_Under_Ground_F= a_Value.intVal;
		}else
		if(a_Col_Name == CNT_C){				// 段彩種別
			e_Class_C		= a_Value.intVal;	///< 段彩
			e_Cur_Attr_Cls	= CNT_C;
		}else
		if(a_Col_Name == CLR_CODE) {			///< 街区色番号
			e_Color_Code	= a_Value.intVal;
		}else
		if(a_Col_Name == BG_C){					///< 背景種別（都市、ベース、ミドル、トップ）
			e_Class_C		= a_Value.intVal;	///< 都市地図、中縮尺種別
			e_Cur_Attr_Cls	= BG_C;
		}else
		if(a_Col_Name == RAIL_C){				// 鉄道路線種別
			e_Class_C		= a_Value.intVal;	///< 鉄道路線
			e_Cur_Attr_Cls	= RAIL_C;
		}else
		if(a_Col_Name == STAT_C){				// 鉄道駅舎種別
			e_Class_C		= a_Value.intVal;	///< 駅舎
			e_Cur_Attr_Cls	= STAT_C;
		}else
		if(a_Col_Name == BLD_C){				// 建物種別
			e_Class_C		= a_Value.intVal;	///< 建物ポリゴン
			e_Cur_Attr_Cls	= BLD_C;
		}else
		if(a_Col_Name == ROAD_C){				// 道路表示種別
			e_Class_C		= a_Value.intVal;	///< 道路
			e_Cur_Attr_Cls	= ROAD_C;
		}else
		if(a_Col_Name == C_CODE){				// 市区町村コード
			// 行政界のみ
			string			a_City_Code_Str;
			a_City_Code_Str	= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
			e_City_Code		= a_City_Code_Str;
		}else
		if(a_Col_Name == A_CODE){				// 町丁目コード
			string			a_Addr_Code_Str;
			a_Addr_Code_Str	= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
			e_Addr_Code		= a_Addr_Code_Str;
		}else
		if(a_Col_Name == N_STR_1){				// 名称文字列１
			e_Str_1			= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
		}else
		if(a_Col_Name == N_STR_2){				// 名称文字列２
			e_Str_2			= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
		}else
		if(a_Col_Name == N_STR_3){				// 名称文字列３
			e_Str_3			= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
		}else
		if(a_Col_Name == S_NUM_1){				// 名称文字数１
			e_Str_Num_1		= a_Value.intVal;
		}else
		if(a_Col_Name == S_NUM_2){				// 名称文字数２
			e_Str_Num_2		= a_Value.intVal;
		}else
		if(a_Col_Name == S_NUM_3){				// 名称文字数３
			e_Str_Num_3		= a_Value.intVal;
		}else
		if(a_Col_Name == AN_CLS_C){				// 注記種別
			e_Anno_Class_C	= a_Value.intVal;
			e_Cur_Attr_Cls	= AN_CLS_C;
		}else
		if(a_Col_Name == TMP_AN_CLS){			// テンポラリ種別
			e_Tmp_Anno_Class= Str_to_SJIS ((CComBSTR *)(&a_Value.bstrVal));
		}else
		if(a_Col_Name == M_TYPE){				//  図形IDにあたる
//			e_Mark_Type		= a_Value.intVal;
		}else
		if(a_Col_Name == CHINUM) {				// 地番番号
			e_Chiban_No		= a_Value.intVal;
		}else
		if(a_Col_Name == A_F) {					// 地番注記フラグ
			e_Anno_F		= a_Value.intVal;
		}else
		if(a_Col_Name == F_SIZE) {				// フォントサイズ
			e_FontSize		= a_Value.intVal;
		}else
		if(a_Col_Name == A_ID) {				// ラインに対応するID
			e_Note_ID		= a_Value.intVal;
		}else
		if(a_Col_Name == TARGET_S) {			// 表示文字列がどれか
			e_Target_Str_Num= a_Value.intVal;
		}else
		if(a_Col_Name == MESH_C) {				// メッシュコード矩形用
			e_MeshCode		= a_Value.intVal;
		}else
		if(a_Col_Name == SPEC_F) {
			e_Spec_F		= a_Value.intVal;	// 段差情報
		}else
		if(a_Col_Name == SHP){					// シェイプデータ
		//	if( a_Col_Type == SE_SHAPE_TYPE ){
			//	IGeometryPtr	a_Tmp_Shp;
			//	c_Feature->get_ShapeCopy	( &a_Tmp_Shp );
			//	e_Geometry_Ptr	= a_Tmp_Shp;
			//	c_Feature->get_Shape		( &c_Shape );			// シェイプを取得
		//	}
		}
	} // End for
	return	( true );
}

/**
	Unicodeの文字列からSJISにする
*/
string		Smp_db_obj_attr::
Str_to_SJIS		(	CComBSTR*	c_CStr	)	// 文字列(Unicode)
{	
	string		ret_str;
#ifdef	_ARC_9_1_
	USES_CONVERSION;
	if (*c_CStr != 0) {
		ret_str = OLE2T(*c_CStr);
	}
#else	
	USES_CONVERSION;
#ifndef _SINDY_A_
	long		length	= c_CStr->Length() * 2;

	char*		tmp_moji	= new char[length+1];

	memset		(tmp_moji, '\0', length+1);
	_bstr_t		a_bTmp ( *c_CStr, false );
//	wchar_t*	tmp_wide	= OLE2W(*c_CStr);
	wchar_t*	tmp_wide	= (wchar_t *)a_bTmp;

	if(tmp_wide == NULL) {
		ret_str	= "";
	}else {
		if(wcstombs( tmp_moji, tmp_wide, length+1 ) == -1){
			ret_str	= "";
		}else {
			ret_str	= tmp_moji;
		}
	}
	delete [] tmp_moji;
#else
	if (*c_CStr != 0) {
		ret_str = OLE2T(*c_CStr);
	}
#endif
#endif	// end of _ARC_9_1_
	return	(ret_str);
}

/**
	ジオメトリを内部点列に変換
	@return	bool
*/
bool		Smp_db_obj_attr::
Inport_Geo	(	IGeometryPtr	c_ipGeo	)	///< ジオメトリ
{

//	HRESULT				hr;

	esriGeometryType	a_eGeoType;

	if( c_ipGeo == NULL ) {
		fprintf	( stderr, "ジオメトリがNULLです\n");
		return	( false );
	}

	if( FAILED(	c_ipGeo->get_GeometryType( &a_eGeoType )) ) {
		fprintf	( stderr, "[Inport_Geo] ジオメトリタイプがとれません\n" );
		return	( false );
	}

	// ジオメトリのタイプを取得する
	if( a_eGeoType == esriGeometryPolygon ) {

		e_Pts_Type	= PTS_POLY;
	
	}else
	if( a_eGeoType == esriGeometryPolyline ) {
		
		e_Pts_Type	= PTS_LINE;

	}else
	if( a_eGeoType == esriGeometryPoint ) {
	
		e_Pts_Type	= PTS_POINT;

	}else {
		fprintf	( stderr, "未対応のジオメトリタイプです<msi_get_parts_info>\n" );
		return	( false );
	}


	// ジオメトリコレクションにしてパート、サブパート毎にばらばらの点列にする
	IGeometryCollectionPtr	a_ipGeoColl ( c_ipGeo );
	long					a_lGeo_Count	= 0;	// ジオメトリの個数
	
	a_ipGeoColl->get_GeometryCount ( &a_lGeo_Count );

	int	i = 0;
	for ( i = 0; i < a_lGeo_Count; i++ )
	{
		IGeometryPtr	a_ipCurGeo;
		
		// ジオメトリを取り出す
		a_ipGeoColl->get_Geometry ( i, &a_ipCurGeo );

		IPointCollectionPtr	a_ipTmpPts;	// 補間点列

		if( a_eGeoType == esriGeometryPolygon ) {	// ポリゴン
			a_ipTmpPts	= (IRingPtr)a_ipCurGeo;
		}else
		if( a_eGeoType == esriGeometryPolyline ) {	// ポリライン
			a_ipTmpPts	= (IPathPtr)a_ipCurGeo;
		}else {										// ポイント
			a_ipTmpPts	= (IPointPtr)a_ipCurGeo;
		}

		long	a_lPtCount	= 0;	// 補間点数
		
		a_ipTmpPts->get_PointCount ( &a_lPtCount );

		// 内部点列の領域を確保しておく
		v_Dpos		a_TmpPts;		// １ジオメトリの点列
		int	i = 0;
		for ( i = 0; i < a_lPtCount; i++ )
		{
			IPointPtr	a_ipPt;
			Dpos		a_dPos;
			a_ipTmpPts->get_Point ( i, &a_ipPt );

			a_ipPt->get_X ( &a_dPos.xpos );
			a_ipPt->get_Y ( &a_dPos.ypos );
			a_TmpPts.push_back ( a_dPos );		// パートごとの点列に追加
		}
		e_vDPtsArray.push_back ( a_TmpPts );	// 点列をベクタに追加 
	}
	return ( true );
}

/**
	内部点列をジオメトリに変換
	@return	IGeometryPtr	成功
	@return	NULL	失敗
*/
IGeometryPtr	Smp_db_obj_attr::
Export_Geo	( void )
{

	IGeometryPtr			a_ipRetGeo	= NULL;
	IGeometryCollectionPtr	a_ipTmpColl;
	IPolygonPtr				a_ipPoly ( CLSID_Polygon );
	IPolylinePtr			a_ipLine ( CLSID_Polyline );
	IPointPtr				a_ipPoint( CLSID_Point );

	if( e_Pts_Type == PTS_POLY ) {
		a_ipTmpColl	= a_ipPoly;
	}else
	if( e_Pts_Type == PTS_LINE ) {
		a_ipTmpColl	= a_ipLine;
	}else {
		a_ipTmpColl	= a_ipPoint;
	}

	iv_v_Dpos	a_ivIndx;	// 点列パートのインデックス
	for( a_ivIndx = e_vDPtsArray.begin(); a_ivIndx != e_vDPtsArray.end(); a_ivIndx++ )
	{
		v_Dpos			a_vCurPts	= *a_ivIndx;
		IGeometryPtr	a_ipTmpGeo;
		IRingPtr		a_ipRing ( CLSID_Ring );
		IPathPtr		a_ipPath ( CLSID_Path );
		IPointCollectionPtr	a_ipPts;
	
		//
		if( e_Pts_Type == PTS_POLY ) {	// ポリゴンの場合
			a_ipTmpGeo	= a_ipPoly;
			a_ipPts		= a_ipPoly;
		}else
		if( e_Pts_Type == PTS_LINE ) {	// ポリラインの場合
			a_ipTmpGeo	= a_ipLine;
			a_ipPts		= a_ipLine;
		}else {							// ポイントの場合
			a_ipTmpGeo	= a_ipPoint;
			a_ipPts		= a_ipPoint;
		}

		// 点列を追加していく
		iv_Dpos	a_ivIndx2;
		for ( a_ivIndx2 = a_vCurPts.begin(); a_ivIndx2 != a_vCurPts.end(); a_ivIndx2++ )
		{
			Dpos	a_TmpPt	= *a_ivIndx2;

			IPointPtr	a_ipPt ( CLSID_Point );
			a_ipPt->put_X ( a_TmpPt.xpos );
			a_ipPt->put_Y ( a_TmpPt.ypos );
			a_ipPts->AddPoint ( a_ipPt );
		}

		// コレクションに追加する
		a_ipTmpColl->AddGeometry ( a_ipTmpGeo );
	}

	// コレクションに追加したデータについてシンプリファイかける
	if( e_Pts_Type == PTS_POLY ) {

		if( FAILED ( a_ipPoly->SimplifyPreserveFromTo () ) ) {
			fprintf	( stderr, "Simplifyに失敗\n" );
			return	( NULL );
		}
		a_ipRetGeo	= a_ipPoly;

	}else
	if( e_Pts_Type == PTS_LINE ) {
		
		if( FAILED ( a_ipLine->SimplifyNetwork () ) ) {
			fprintf	( stderr, "Simplifyに失敗\n" );
			return	( NULL );
		}
		a_ipRetGeo	= a_ipLine;

	}else {
		a_ipRetGeo	= a_ipPoint;
	}
	return	( a_ipRetGeo );
}
