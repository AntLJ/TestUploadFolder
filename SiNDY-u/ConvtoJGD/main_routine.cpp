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
#include "main_routine.h"

#include <atlstr.h>
#include "..\..\LIB\WinLib\VersionInfo.h"
#include <sindy/workspace.h>
#include <boost/program_options.hpp>

#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

namespace {
	typedef boost::char_separator<char> char_separator;
	typedef boost::tokenizer<char_separator> tokenizer;
}

// DBへの接続オープン
bool	main_routine::
DB_Open ( const CString&  c_csConnectStr, //!< [in] 接続文字列
          IWorkspacePtr&  c_ipWorkspace ) //!< [in,out] Workspaceの参照
{
	IWorkspacePtr ipWorkspace =  sindy::create_workspace(c_csConnectStr);
	if(!ipWorkspace ){
		return false;
	}
	c_ipWorkspace	= ipWorkspace;
	return true;
}

//	実行ログファイルをオープンする
bool	main_routine::
Open_Exe_Log ( void )
{
	if( m_sExeLog_File == "" ) {
		return	true;
	}else {
		m_fExe_Fp	= fopen ( m_sExeLog_File.c_str(), "w" );
		if( m_fExe_Fp == NULL ) {
			return false;
		}
	}
	return true;
}


//	エラーログファイルをオープンする
bool	main_routine::
Open_Err_Log ( void )
{
	if( m_sLog_File == "" ) {
		return	true;
	}else {
		m_fErr_Fp	= fopen ( m_sLog_File.c_str(), "w" );
		if( m_fErr_Fp == NULL ) {
			return false;
		}
	}
	return true;
}

// 使用法表示
void		main_routine::
print_usage	(char **argv)
{
	PrnMsg(	stdout, NULL,
		"=== 【測地系変換ツール】(v%s) ===\n"
				"<usage>\n"
				"%s <オプション>... \n"
				"\t      '-d'(db)        : ★処理対象DB接続(ex. -d SINDY2012B/SINDY2012B/SDE.SNP_delete_130220/5151/ruby2 )\n"
				"\t      '-f'(fclass)    : ★処理対象フィーチャクラス(ex. -f SINDY2012B.Building )\n"
				"\t      '-l'(errlog)    : ★エラーログファイル名を指定(ex. -l .\\test\\ConvtoJGD.log)\n"
				"\t      '-x'(exelog)    : ★実行ログファイル名を指定(ex. -l .\\test\\ConvtoJGD_Exe.log)\n"
				"\t      '-p'(parameter) : ★地域パラメータファイル名を指定(ex. -p TKY2JGD.par)\n"
				"\t      '-u'(update)    :   更新モード指定（未指定時はテストモード）\n"
				"\t      ※★印がついているものは必須オプション。\n"
				"\t      \n", m_sVersion, argv[0] );
}

using boost::program_options::options_description;
using boost::program_options::value;
using boost::program_options::variables_map;
using boost::program_options::store;
using boost::program_options::parse_command_line;
using boost::program_options::notify;

// 引数チェック
bool	main_routine::
arg_check ( int    argc,  //!< [IN] 引数個数
            char** argv ) //!< [IN] 引数列
{
	//-----------------------------------------------------------------------
	// ツールバージョン表示
	CVersion	a_cvTool_Var;
	m_sVersion	= a_cvTool_Var.GetFileVersion();
	//-----------------------------------------------------------------------

	options_description oOption("オプション");

	// 引数の書式を定義
	oOption.add_options()
	//	("db1,d", value<string>(&m_sSrc_Path), "削除処理対象DB接続1")
		("db,d", value<std::string>(&m_sDB_Connect1), "処理対象DB接続")
		("fclass,f", value<std::string>(&m_sFeatureClass1),"処理対象フィーチャクラス")
		("errlog,l", value<std::string>(&m_sLog_File), "エラーログファイル名")
		("exelog,x", value<std::string>(&m_sExeLog_File), "実行ログファイル名")
		("parameter,p", value<std::string>(&m_sParam_File), "地域パラメータファイル名")
		("update,u", "更新モード指定");

	variables_map vArgmap;

	try {
		store(parse_command_line(argc, argv, oOption), vArgmap);
		notify(vArgmap);

		// 接続DB指定が無い場合は終了
		if ( !vArgmap.count("db") ||
			 !vArgmap.count("fclass") ||
			 !vArgmap.count("errlog") ||
			 !vArgmap.count("exelog") )
		{
			PrnMsg ( m_fErr_Fp, stderr, "ERROR:必須オプションが抜けています\n");
			return false;
		}

		if( vArgmap.count("update") ) { m_lExe_Flag |= ON_UPDATEMODE; }
		if( vArgmap.count("parameter") ) { m_lExe_Flag |= ON_RPARAM; }
	}catch (...) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:オプションの指定に間違いがあります\n");
		return false;
	}

	// crd_cnvの初期化
	if(  m_lExe_Flag & ON_RPARAM ) {
		if( !m_cCrd.JGD2000_RP_Init ( (char*)m_sParam_File.c_str() ) ) {
			PrnMsg ( stderr, NULL, "ERROR:地域パラメータファイルがオープンできない : <%s>\n", m_sParam_File.c_str() );
			return	false;
		}
	}

	// 実行ログファイルのオープン
	if( !Open_Exe_Log () ) {
		PrnMsg ( stderr, NULL, "ERROR:実行ログファイルがオープンできない : <%s>\n", m_sExeLog_File.c_str() );
		return	false;
	}


	// エラーログファイルのオープン
	if( !Open_Err_Log () ) {
		PrnMsg ( stderr, NULL, "ERROR:ログファイルがオープンできない : <%s>\n", m_sLog_File.c_str() );
		return	false;
	}

	//-----------------------------------------------------------------------------------------------
	// 実行情報を出力
	PrnMsg( m_fErr_Fp, stderr, "#SINDYSTDLOG\n");
	PrnMsg( m_fErr_Fp, stderr, "#ツールバージョン              : <%s>\n", m_sVersion );
	PrnMsg( m_fErr_Fp, stderr, "#処理対象DB接続                : <%s>\n", m_sDB_Connect1.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#処理対象フィーチャクラス      : <%s>\n", m_sFeatureClass1.c_str() );
	PrnMsg( m_fErr_Fp, stderr, "#実行モード                    : <%s>\n", m_lExe_Flag&ON_UPDATEMODE?"更新モード":"テストモード" );
	//-----------------------------------------------------------------------------------------------
	
	if( m_lExe_Flag & ON_UPDATEMODE ) { // 削除モードの場合
		if( m_sDB_Connect1.find ( ".gdb" ) == std::string::npos && 
			m_sDB_Connect1.find ( ".mdb" ) == std::string::npos ) {
			PrnMsg	( stdout, NULL, "#上記DBに対し更新処理を実行しますが間違いありませんか？<y/n> :" );
			int	a_cChkChar	= 0;
			// コンソールから文字を取得	
			a_cChkChar	= toupper( getchar() );
			PrnMsg	( stdout, NULL, "\n");
			if( a_cChkChar != 'Y' ) {
				PrnMsg ( stdout, NULL, "#更新処理を中断します\n");
				return false;
			}
		}
	}

	// 開始時間の設定
	m_eExeTime.Set_Start();

	//--------------------------------------------------------------------------
	// DB接続関連、フィーチャクラスのオープン処理
	//--------------------------------------------------------------------------
	CString	a_sDB_Connect1 = m_sDB_Connect1.c_str();

	if( !DB_Open ( a_sDB_Connect1, ipWorkspace_1 ) ) {
		PrnMsg( m_fErr_Fp, stderr, "ERROR:DBがオープンできない : <%s>\n", a_sDB_Connect1 );
		return false;
	}

	// FCのオープン処理
	IFeatureWorkspacePtr	ipFeWspace_1 ( ipWorkspace_1 );

	if( FAILED (ipFeWspace_1->OpenFeatureClass ( _bstr_t ( m_sFeatureClass1.c_str() ), &m_ipFeatureClass_1 ) ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:フィーチャクラスがオープンできませんでした : <%s>\n", m_sFeatureClass1.c_str() ); 
		return false;
	}

	// DB接続にかかった時間の表示
	PrnMsg ( m_fErr_Fp, stderr, "#DB接続処理時間 = %0.2f(s)でした\n", (double)((m_eExeTime.Get_From_Start()) / 1000.0) );

	return true;
}

// リストの読み込み
bool main_routine::
Read_List ( const std::string& c_sFileName, //!< [in] ファイル名
            std::set<long>&    c_sList )    //!< [in] リスト（ID等に使用）
{
	const int bmax	= 512;
	FILE*     a_pFp;		
	char      a_cBuf[bmax];
	int       a_StrLen = 0;	

	a_pFp = fopen ( c_sFileName.c_str(), "r" );
	if(a_pFp == NULL) {
		return false;
	}

	while( fgets ( a_cBuf, bmax, a_pFp ) != NULL)
	{
		a_StrLen	= strlen ( a_cBuf );
		// 最後の"\n"を取り除く
		if(a_cBuf[a_StrLen - 1] == '\n') {
			a_cBuf[a_StrLen - 1] = '\0';
		}
		// コメント文を飛ばす
		if(a_cBuf[0] == '#' || a_cBuf[0] == '\0') {
			continue;
		}
		// リストに追加
		c_sList.insert ( atol ( a_cBuf ) );
	}
	fclose ( a_pFp );
	return true;
}

/** ------------------------------------------------------------------------------
	フィーチャを出力用のフィーチャにする
	@return	データ数
--------------------------------------------------------------------------------*/
int main_routine::
Make_Data_Container ( const IFeatureCursorPtr&                               c_ipFCursor,   //!< [IN] 注記カーソル
                      std::map<long, CAdapt<IFeaturePtr>, std::less<long> >& c_mWork_Data ) //!< [IN] 出力用データ
{
	int	a_iCount	= 0;

	// データvectorコンテナに落とす
	if( c_ipFCursor != NULL ) {

		IFeaturePtr		a_ipCur_Feat;
		while ( c_ipFCursor->NextFeature ( &a_ipCur_Feat ) == S_OK )
		{
			long	a_lOID = 0;
			a_ipCur_Feat->get_OID ( &a_lOID );

			IGeometryPtr	a_ipWorkGeo;	//!< 作業用
			a_ipCur_Feat->get_Shape (&a_ipWorkGeo);

			c_mWork_Data.insert ( std::map<long, CAdapt<IFeaturePtr>, std::less<long> >::value_type ( a_lOID, a_ipCur_Feat) );
			a_iCount++;
		}
	}
	return	( a_iCount );
}

//	指定フィーチャに対してesriSpatialRelEnumで指定した関係を持つデータを
//	とってくる
IFeatureCursorPtr   main_routine::
Search_Feature ( const IFeatureClassPtr& c_ipFeCls,     //!< [in] フィーチャクラス
                 const IGeometryPtr&     c_ipGeo,       //!< [in] 検索用ジオメトリ
                 esriSpatialRelEnum      c_ipSpatialRel,//!< [in] 関係指定
                 std::string             c_sWhere,      //!< [in] Where句指定
                 long&                   c_iCount )     //!< [out] 検索数
{
	ISpatialFilterPtr	a_ipFilter(CLSID_SpatialFilter);
	CComBSTR			a_ccSpCol;

	if (FAILED(c_ipFeCls->get_ShapeFieldName ( &a_ccSpCol )) || // シェイプフィールド名取得
	    FAILED(a_ipFilter->putref_Geometry ( c_ipGeo )) ||      // サーチ用のジオメトリ
	    FAILED(a_ipFilter->put_GeometryField ( a_ccSpCol )) ||  // ジオメトリフィールド
	    FAILED(a_ipFilter->put_SpatialRel ( c_ipSpatialRel )) ||// SpatialRel()
	    FAILED(a_ipFilter->put_SearchOrder ( esriSearchOrderSpatial )) ) { // サーチオーダー
		PrnMsg( m_fErr_Fp, stderr, "ERROR:Filterの設定に失敗[Search_Feature]\n");
		return	( NULL );
	}

	if( c_sWhere.size() > 1 ) {
		if ( FAILED ( a_ipFilter->put_WhereClause ( _bstr_t ( c_sWhere.c_str() ) ) ) )	{
			PrnMsg( m_fErr_Fp, stderr, "ERROR:検索用WHERE句の設定ができない[Search_Feature]" );
			return	( NULL );
		}
	}
	// データカウントをやる
	long		a_lData_Count	= 0;
	if ( FAILED ( c_ipFeCls->FeatureCount ( (IQueryFilterPtr)a_ipFilter, &a_lData_Count ) ) ) {
		PrnMsg( m_fErr_Fp, stderr, "ERROR:検索データ数が取得できない[Search_Feature]\n");
		return	( NULL );
	}
	c_iCount	= a_lData_Count;

	IFeatureCursorPtr	a_ipFeCsr;	//	返却用フィーチャカーソル
	if (FAILED( c_ipFeCls->Search((IQueryFilterPtr)a_ipFilter, VARIANT_FALSE, &a_ipFeCsr)) ) {
		PrnMsg( m_fErr_Fp, stderr, "ERROR:データのサーチができない[Search_Feature]\n");
		return	( NULL );
	}
	return	( a_ipFeCsr );
}

// オブジェクトIDリストの作成
bool main_routine::
MakeOBJECTIDList ( const IFeatureClassPtr& c_ipFeCls,         //!< [in] フィーチャクラス
                   std::set<long>&         c_sOBJECTID_List ) //!< [out] OBJECTIDリスト
{
	IQueryFilterPtr	a_ipFilter(CLSID_QueryFilter);
	CComBSTR		a_ccSpCol;

//		if ( FAILED ( a_ipFilter->put_WhereClause ( _bstr_t ( "OBJECTID  ) ) ) )	{
//			PrnMsg( m_fErr_Fp, stderr, "ERROR:検索用WHERE句の設定ができない[Search_Feature]" );
//			return	( NULL );
//		}
//	}
	// データカウントをやる
	long		a_lData_Count	= 0;
	if ( FAILED ( c_ipFeCls->FeatureCount ( (IQueryFilterPtr)a_ipFilter, &a_lData_Count ) ) ) {
		PrnMsg( m_fErr_Fp, stderr, "ERROR:検索データ数が取得できない[Search_Feature]\n");
		return false;
	}

	IFeatureCursorPtr	a_ipFeCsr;	//	返却用フィーチャカーソル
	if (FAILED( c_ipFeCls->Search((IQueryFilterPtr)a_ipFilter, VARIANT_FALSE, &a_ipFeCsr)) ) {
		PrnMsg( m_fErr_Fp, stderr, "ERROR:データのサーチができない[Search_Feature]\n");
		return false;
	}

	long a_lCount = 0;
	IFeaturePtr	a_ipFeature;
	while ( a_ipFeCsr->NextFeature ( &a_ipFeature ) == S_OK )
	{
		if( a_lCount % 100 == 0 ) {
			PrnMsg( stdout, NULL, "%d/%d件処理中\r", a_lCount, a_lData_Count );			
		}
		long a_lOID = 0;
		a_ipFeature->get_OID ( &a_lOID );
		c_sOBJECTID_List.insert ( a_lOID );
		++a_lCount;
	}
	PrnMsg( stdout, NULL, "\n" );			
	return true;
}

// エディットの開始
bool main_routine::
StartEdit ( void )
{
	try {
		// 注記をEditモードに変更
		m_ipWorkspaceEdit_1	= ipWorkspace_1;

		// 編集開始（ArcMap の編集開始と同じ）
		if( FAILED ( m_ipWorkspaceEdit_1->StartEditing( VARIANT_FALSE ) )) { // Undo Redo がいらないなら VARIANT_FALSE
			return false;
		}else {
			if( FAILED ( m_ipWorkspaceEdit_1->StartEditOperation() )) {
				return false;
			}
		}
	}catch ( ... ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:StartEditing時に例外発生\n" );
		return false;	
	}
	return true;
}

// エディットの終了
bool main_routine::
StopEdit ( void )
{
	try {
		// 編集の終了
		if ( FAILED ( m_ipWorkspaceEdit_1->StopEditOperation() )) {
			return false;
		}else {
			if ( FAILED ( m_ipWorkspaceEdit_1->StopEditing( VARIANT_TRUE ) ) ) {
				//-----------------------------------------
				IErrorInfoPtr	ipErrorInfo;
				CString msg = "Unknown Error";
				::GetErrorInfo(0, &ipErrorInfo);
				if(ipErrorInfo != NULL){
					BSTR	hoge;
					ipErrorInfo->GetDescription(&hoge);
					msg	= hoge;
				}
				PrnMsg ( m_fErr_Fp, stderr, "ERROR:StopEditingに失敗しました\t[%s]", msg );
				//-----------------------------------------
				return ( false );
			}
		}
	}catch ( ... ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:StopEditing時に例外発生\n" );
		return false;
	}
	return true;
}

// 元のオブジェクトに対するターゲットの関係チェック
bool main_routine::
RelCheck ( const IGeometryPtr& c_ipSrcGeom, //!< [in] 比較元ジオメトリ
           const IGeometryPtr& c_ipTgtGeom, //!< [in] 比較先ジオメトリ
           long                c_lRel_Type )//!< [in] 関係のタイプ
{
	VARIANT_BOOL a_bRet;
	IRelationalOperatorPtr	a_ipRelOpe ( c_ipSrcGeom );

	try {
		switch ( c_lRel_Type )
		{
		case contains: a_ipRelOpe->Contains ( c_ipTgtGeom, &a_bRet ); break;
		case overlaps: a_ipRelOpe->Overlaps ( c_ipTgtGeom, &a_bRet ); break;
		case within:   a_ipRelOpe->Within ( c_ipTgtGeom, &a_bRet ); break;
		case touches:  a_ipRelOpe->Touches ( c_ipTgtGeom, &a_bRet ); break;
		case equals:   a_ipRelOpe->Equals ( c_ipTgtGeom, &a_bRet ); break;
		case crosses:  a_ipRelOpe->Crosses ( c_ipTgtGeom, &a_bRet ); break;
		}
	}catch ( ... ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:RelCheck時に例外発生\n" );
		return false;
	}
	if( a_bRet == VARIANT_TRUE ) {
		return true;
	}
	return false;
}

/**
 * @brief 代表点を取得
 * @return 
 * @return	成功	true
 * @return	成功	false
 */
bool main_routine::
GetPoint ( const IGeometryPtr& c_ipGeom, //!< [in] ジオメトリ
		   double&             c_dX,     //!< [out] 経度
		   double&             c_dY )    //!< [out] 経度
{
	// 初期化
	c_dX = 0.0;
	c_dY = 0.0;

	// ジオメトリタイプ
	esriGeometryType a_eGeomType;
	if( FAILED ( c_ipGeom->get_GeometryType ( &a_eGeomType ) ) ) {
		return false;
	}

	double a_dX = 0.0, a_dY = 0.0;

	switch ( a_eGeomType )
	{
	case esriGeometryPoint:
		{
			IPointPtr a_ipPoint ( c_ipGeom );
			a_ipPoint->get_X ( &a_dX );
			a_ipPoint->get_Y ( &a_dY );
		}
		break;
	case esriGeometryPolyline:
		{
			IPolylinePtr a_ipPolyline( c_ipGeom );
			double a_dLen = 0.0;
			a_ipPolyline->get_Length( &a_dLen );
			IPointPtr a_ipPt(CLSID_Point);
			a_ipPolyline->QueryPoint( esriNoExtension, a_dLen/2.0, VARIANT_FALSE, a_ipPt );
			a_ipPt->get_X ( &a_dX );
			a_ipPt->get_Y ( &a_dY );
		}
		break;
	case esriGeometryPolygon:
		{
			IAreaPtr a_ipArea ( (IPolygonPtr)c_ipGeom );
			IPointPtr a_ipPt;
			a_ipArea->get_LabelPoint ( &a_ipPt );
			a_ipPt->get_X ( &a_dX );
			a_ipPt->get_Y ( &a_dY );
		}
		break;
	}
	c_dX = a_dX;
	c_dY = a_dY;

	return true;
}

// JGD移行したジオメトリを作成する
IGeometryPtr main_routine::
Make_JGD_Geometry ( IGeometryPtr& c_ipGeom ) //!< [in] ジオメトリ
{
	// 空間参照を取得
	ISpatialReferencePtr a_ipSpRef;
	if( FAILED (c_ipGeom->get_SpatialReference ( &a_ipSpRef )) ) {
		return NULL;
	}

	if( m_ipGeom_Type == esriGeometryPoint ) {

		try {
	
			IPointPtr	a_ipPt(c_ipGeom);

			double	dBefore_X = 0, dBefore_Y = 0;
			a_ipPt->QueryCoords ( &dBefore_X, &dBefore_Y );

			double dAfter_He = 0, dAfter_X = 0, dAfter_Y = 0;
			if( m_lExe_Flag & ON_RPARAM ) {
				m_cCrd.TOKYOtoJGD2000_RP ( dBefore_X,  dBefore_Y, &dAfter_X, &dAfter_Y, true ); // IPC用変換
			}else { // 3パラメータ
				m_cCrd.TOKYOtoJGD2000 ( dBefore_X,  dBefore_Y, 0, &dAfter_X, &dAfter_Y, &dAfter_He );
			}

			a_ipPt->put_X ( dAfter_X );
			a_ipPt->put_Y ( dAfter_Y );

		}catch (...) {
			return NULL;
		}
	}else {

	//	IGeometryBagPtr	a_ipNewGeom(CLSID_GeometryBag);
		try {
			// 中抜きポリゴンがあるので、リングを考慮した処理をする

			IGeometryCollectionPtr	a_ipGeoColl(c_ipGeom);
		//	IGeometryCollectionPtr	a_ipNewGeoColl(a_ipNewGeom);
			long	a_ipGeomCount = 0;
			a_ipGeoColl->get_GeometryCount ( &a_ipGeomCount );
			for( long count = 0; count < a_ipGeomCount; ++count )
			{
				IGeometryPtr	a_ipCurGeom;
				a_ipGeoColl->get_Geometry ( count, &a_ipCurGeom ); // ジオメトリ数

				IPointCollectionPtr	a_ipPts(a_ipCurGeom);

				long	a_lPtsCount = 0;
				a_ipPts->get_PointCount ( &a_lPtsCount );

				WKSPoint *pWKSPoint = new WKSPoint[a_lPtsCount];
				a_ipPts->QueryWKSPoints(0, a_lPtsCount, pWKSPoint);

				// 測地系変換処理
				for ( long i = 0; i < a_lPtsCount; ++i )
				{
					double dAfter_He = 0, dAfter_X = 0, dAfter_Y = 0;
					if( m_lExe_Flag & ON_RPARAM ) {
						m_cCrd.TOKYOtoJGD2000_RP ( pWKSPoint[i].X,  pWKSPoint[i].Y, &dAfter_X, &dAfter_Y, true ); // IPC用変換
						pWKSPoint[i].X = dAfter_X;
						pWKSPoint[i].Y = dAfter_Y;
					}else { // 3パラメータ
						m_cCrd.TOKYOtoJGD2000 ( pWKSPoint[i].X,  pWKSPoint[i].Y, 0, &dAfter_X, &dAfter_Y, &dAfter_He );
						pWKSPoint[i].X = dAfter_X;
						pWKSPoint[i].Y = dAfter_Y;
					}
				}
				a_ipPts->SetWKSPoints( a_lPtsCount, pWKSPoint );

			//	a_ipNewGeoColl->AddGeometry ( a_ipCurGeom );
				delete [] pWKSPoint;
			}
		//	a_ipNewGeom->putref_SpatialReference ( a_ipSpRef );
		//	a_ipNewGeom->SnapToSpatialReference ();
		} catch (...) {
			return NULL;
		}
	}
	return c_ipGeom;
//	return a_ipNewGeom;
}

// 変換実行部
bool main_routine::
update_exec	( void )
{
	// 開始時間の設定
	m_eExeTime.Set_Start();

	MakeOBJECTIDList ( m_ipFeatureClass_1, m_sOBJECTID_List );

	// 削除モードの場合のみ
	if( m_lExe_Flag & ON_UPDATEMODE ) {
		// エディット開始
		if( !StartEdit () ) {
			PrnMsg ( m_fErr_Fp, stderr, "ERROR:StartEdit処理に失敗しました\n" );
			return false;
		}
	}

	if( FAILED ( m_ipFeatureClass_1->get_ShapeType ( &m_ipGeom_Type ) ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:ジオメトリタイプの取得に失敗しました\n" );
		return false;	
	}

	PrnMsg( m_fErr_Fp, stderr, "#処理オブジェクト数 %d\n", m_sOBJECTID_List.size() );

	long a_lCount = 0;
	std::set<long>::const_iterator a_iIter;
	for( a_iIter = m_sOBJECTID_List.begin(); a_iIter != m_sOBJECTID_List.end(); ++a_iIter, ++a_lCount )
	{
		if( a_lCount % 100 == 0 ) {
			PrnMsg( stdout, NULL, "%d/%d件処理中\r", a_lCount, m_sOBJECTID_List.size() );			
		}
		long a_lCur_OBJECTID = *a_iIter;

		try {

			// OBJECTIDで対象オブジェクトを取得
			IFeaturePtr a_ipSrcFeature;
			m_ipFeatureClass_1->GetFeature(a_lCur_OBJECTID, &a_ipSrcFeature);

			IGeometryPtr a_ipGeom;
			if( FAILED ( a_ipSrcFeature->get_Shape ( &a_ipGeom ) ) ) {
				PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\t形状取得失敗\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
				continue;
			}

			IGeometryPtr a_ipNewGeom = Make_JGD_Geometry ( a_ipGeom );
			if( a_ipNewGeom == NULL ) {
				PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\t形状作成失敗\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
				continue;				
			}

			HRESULT hr;
			hr = a_ipSrcFeature->putref_Shape ( a_ipNewGeom );
			if( FAILED ( hr ) ) {
				PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\tputref_Shape()に失敗\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
			}

			// データ更新
			if( m_lExe_Flag & ON_UPDATEMODE ) {
				if( FAILED ( a_ipSrcFeature->Store () ) ) {
					PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\tStore()に失敗\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
				}
			}

			double a_dX = 0.0, a_dY = 0.0;
			GetPoint ( a_ipGeom, a_dX, a_dY );
		//	PrnMsg( m_fErr_Fp, NULL, "0\t%s\t%d\t%0.10f\t%0.10f\tNOTICE\t\t座標変換\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID, a_dX, a_dY );
		}catch (...) {
			PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\t例外発生\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
			continue;
		}
	}
	PrnMsg( stdout, NULL, "\n" );			

	// 削除モードの場合のみ
	if( m_lExe_Flag & ON_UPDATEMODE ) {
		// エディット終了
		if( !StopEdit () ) {
			PrnMsg ( m_fErr_Fp, stderr, "ERROR:StopEdit処理に失敗しました\n" );
			return false;
		}
	}

	//　実行部終了
	// 終了時間の取得
	PrnMsg	( m_fErr_Fp, stderr, "#データ変換総時間 = %0.2f(s)でした\n",
						(double)((m_eExeTime.Get_From_Start()) / 1000.0) );
	return	( true );
}

// 検索句を指定し、アップデート用のカーソルを取って来る
bool	main_routine::
Update_Where ( const IFeatureClassPtr&  c_ipTable,    ///< [in] 検索元テーブル
               CString&                 c_sWhere,    ///< [in] 指定WHERE句
               int                      c_iRetType,  ///< [in] 何を返すか
               long&                    c_lCount,    ///< [out] カウント
               IFeatureCursorPtr&       c_ipCursor ) ///< [out] カーソル
{
	HRESULT			hr;

	// 初期化
	c_ipCursor = NULL;
	
	IQueryFilterPtr		ipQ_Filter ( CLSID_QueryFilter );
	
	hr	= ipQ_Filter->put_WhereClause ( CComBSTR(c_sWhere) );
	if ( FAILED ( hr ) ) {
		PrnMsg ( m_fErr_Fp, NULL, _T("%s検索用WHERE句の設定ができない\t<%s>\n"), ERR_STAT, c_sWhere );
		return false;
	}

	// データカウントをやる
	if( c_iRetType & RET_COUNT ) {
		//---------------------------------------------------------
		long lData_Count = 0;
		hr	= c_ipTable->FeatureCount ( ipQ_Filter, &lData_Count );
		if ( FAILED ( hr ) )	{
			PrnMsg ( m_fErr_Fp, NULL, _T("%s検索データ数が取得できない\t<%s>\n"), ERR_STAT, c_sWhere );
			return false;
		}
		c_lCount = lData_Count;
		//---------------------------------------------------------
	}

	// サーチしてくる
	IFeatureCursorPtr	ipCursor;
	if( c_iRetType & RET_CURSOR ) {
		if( FAILED(c_ipTable->Update( ipQ_Filter, VARIANT_FALSE, &ipCursor )) ) {
			PrnMsg ( m_fErr_Fp, NULL, _T("%sデータ検索に失敗\t<%s>\n"), ERR_STAT, c_sWhere );
			return false;
		}
		c_ipCursor = ipCursor;
	}
	return true;
}


// 変換実行部
bool main_routine::
update_exec_nolist ( void )
{
	// 開始時間の設定
	m_eExeTime.Set_Start();

//	MakeOBJECTIDList ( m_ipFeatureClass_1, m_sOBJECTID_List );

	//--------------------------------------------------------------------------------
	long a_lData_Count	= 0;

	IFeatureCursorPtr	a_ipFeCsr;	//	返却用フィーチャカーソル
	if( !Update_Where( m_ipFeatureClass_1, CString(_T("")), RET_CURSOR|RET_COUNT, a_lData_Count, a_ipFeCsr ) )
		return false;

/*	long a_lCount = 0;
	IFeaturePtr	a_ipFeature;
	while ( a_ipFeCsr->NextFeature ( &a_ipFeature ) == S_OK )
	{
		if( a_lCount % 100 == 0 ) {
			PrnMsg( stdout, NULL, "%d/%d件処理中\r", a_lCount, a_lData_Count );			
		}
		long a_lOID = 0;
		a_ipFeature->get_OID ( &a_lOID );
		c_sOBJECTID_List.insert ( a_lOID );
		++a_lCount;
	}*/
	//--------------------------------------------------------------------------------

	// 削除モードの場合のみ
	if( m_lExe_Flag & ON_UPDATEMODE ) {
		// エディット開始
		if( !StartEdit () ) {
			PrnMsg ( m_fErr_Fp, stderr, "ERROR:StartEdit処理に失敗しました\n" );
			return false;
		}
	}

	if( FAILED ( m_ipFeatureClass_1->get_ShapeType ( &m_ipGeom_Type ) ) ) {
		PrnMsg ( m_fErr_Fp, stderr, "ERROR:ジオメトリタイプの取得に失敗しました\n" );
		return false;	
	}

	PrnMsg( m_fErr_Fp, stderr, "#処理オブジェクト数 %d\n", a_lData_Count );

	long a_lCount = 0;
//	std::set<long>::const_iterator a_iIter;
//	for( a_iIter = m_sOBJECTID_List.begin(); a_iIter != m_sOBJECTID_List.end(); ++a_iIter, ++a_lCount )
//	{
	IFeaturePtr a_ipSrcFeature;
	while ( a_ipFeCsr->NextFeature ( &a_ipSrcFeature ) == S_OK )
	{
		if( a_lCount % 100 == 0 ) {
			PrnMsg( stdout, NULL, "%d/%d件処理中\r", a_lCount, a_lData_Count );			
		}
		++a_lCount; // ここでやっても特に問題ない

		long a_lCur_OBJECTID = 0;
		if( FAILED (a_ipSrcFeature->get_OID ( &a_lCur_OBJECTID ) ) )
			return false;

		try {

			// OBJECTIDで対象オブジェクトを取得
		//	m_ipFeatureClass_1->GetFeature(a_lCur_OBJECTID, &a_ipSrcFeature);

			IGeometryPtr a_ipGeom;
			if( FAILED ( a_ipSrcFeature->get_Shape ( &a_ipGeom ) ) ) {
				PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\t形状取得失敗\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
				continue;
			}

			IGeometryPtr a_ipNewGeom = Make_JGD_Geometry ( a_ipGeom );
			if( a_ipNewGeom == NULL ) {
				PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\t形状作成失敗\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
				continue;				
			}

			HRESULT hr;
			hr = a_ipSrcFeature->putref_Shape ( a_ipNewGeom );
			if( FAILED ( hr ) ) {
				PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\tputref_Shape()に失敗\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
			}

			// データ更新
			if( m_lExe_Flag & ON_UPDATEMODE ) {
				if( FAILED ( a_ipSrcFeature->Store() ) ) {
					PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\tStore()に失敗\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
				}
			}

		//	double a_dX = 0.0, a_dY = 0.0;
		//	GetPoint ( a_ipGeom, a_dX, a_dY );
		//	PrnMsg( m_fErr_Fp, NULL, "0\t%s\t%d\t%0.10f\t%0.10f\tNOTICE\t\t座標変換\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID, a_dX, a_dY );
		}catch (...) {
			PrnMsg( m_fErr_Fp, stderr, "0\t%s\t%d\t\t\tERROR\t\t例外発生\n", m_sFeatureClass1.c_str(), a_lCur_OBJECTID );
			continue;
		}
	}
	PrnMsg( stdout, NULL, "\n" );			

	// 削除モードの場合のみ
	if( m_lExe_Flag & ON_UPDATEMODE ) {
		// エディット終了
		if( !StopEdit () ) {
			PrnMsg ( m_fErr_Fp, stderr, "ERROR:StopEdit処理に失敗しました\n" );
			return false;
		}
	}

	//　実行部終了
	// 終了時間の取得
	PrnMsg	( m_fErr_Fp, stderr, "#データ変換総時間 = %0.2f(s)でした\n",
						(double)((m_eExeTime.Get_From_Start()) / 1000.0) );
	return	( true );
}
