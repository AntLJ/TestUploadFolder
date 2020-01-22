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
#include "main_rootin.h"

// 2009.10.23 追加
#include <atlstr.h>
#include <sindy/workspace.h>
#include "..\..\LIB\WinLib\VersionInfo.h"

//	2003.07.04	main_rootin.hから定義を分離

/**
* @brief メッセージの出力
*/
void PrnMsg	( FILE* c_cpFp, FILE* c_cpFp2, const char* c_cpFormat, ... )
{						
	va_list	a_vArgList = NULL;

	va_start(a_vArgList, c_cpFormat );	// 初期化

	vfprintf_s ( c_cpFp, c_cpFormat, a_vArgList );
	if( c_cpFp2 != NULL && c_cpFp != c_cpFp2) {
		vfprintf_s ( c_cpFp2, c_cpFormat, a_vArgList );
	}
	fflush	( c_cpFp );
}

/** ------------------------------------------------------------------------------
	SiNDYDBへの接続オープン
	@return	true	成功
	@return	false	失敗
--------------------------------------------------------------------------------*/
bool	main_rootin::
SiNDY_DB_Open ( IWorkspacePtr&	c_ipWorkspace_Src,
				IWorkspacePtr&	c_ipWorkspace_Tgt )
{
	CString csWorkStrSrc;
	csWorkStrSrc.Format(_T("%s/%s/%s/%s/%s"), e_pTool_Init.get_UserName(), e_pTool_Init.get_Password(),
					  e_pTool_Init.get_Version(), e_pTool_Init.get_Instance(), e_pTool_Init.get_Server() );

	PrnMsg(e_fErr_Fp, stdout, "#接続プロパティ(src)：%s\n", csWorkStrSrc );

	c_ipWorkspace_Src = sindy::create_workspace(csWorkStrSrc);
	if(!c_ipWorkspace_Src ){
		PrnMsg(e_fErr_Fp, stderr, "接続出来なかった\n" );
		return false;
	}else {
		PrnMsg(e_fErr_Fp, stdout, "#接続完了\n" );
	}

	CString csWorkStrTgt;
	csWorkStrTgt.Format(_T("%s/%s/%s/%s/%s"), e_pTool_Init_Tgt.get_UserName(), e_pTool_Init_Tgt.get_Password(),
					  e_pTool_Init_Tgt.get_Version(), e_pTool_Init_Tgt.get_Instance(), e_pTool_Init_Tgt.get_Server() );

	PrnMsg(e_fErr_Fp, stdout, "#接続プロパティ(tgt)：%s\n", csWorkStrTgt );

	c_ipWorkspace_Tgt = sindy::create_workspace(csWorkStrTgt);
	if(!c_ipWorkspace_Tgt ){
		PrnMsg(e_fErr_Fp, stderr, "接続出来なかった\n" );
		return false;
	}else {
		PrnMsg(e_fErr_Fp, stdout, "#接続完了\n" );
	}
	return	( true );
}

/** ------------------------------------------------------------------------------
	エラーログファイルをオープンする
	@return	true	オープン成功
	@return false	オープン失敗
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Err_Log	( void )
{
	if( e_cpLog_File == NULL ) {
		return	( true );
	}else {
		e_fErr_Fp	= fopen ( e_cpLog_File, "w" );
		if( e_fErr_Fp == NULL ) {
			PrnMsg(stderr, NULL, "エラーログファイル<%s>がオープンできない\n", e_cpLog_File );
			return	( false );
		}
	}
	return	( true );
}

/** ------------------------------------------------------------------------------
	出力対応テーブルファイルをオープンする
	@return	true	オープン成功
	@return false	オープン失敗
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Match_Tbl	( void )
{
	if( e_cpMatch_Tbl_File == NULL ) {
		return	( true );
	}else {
		e_fpMatch_Fp	= fopen ( e_cpMatch_Tbl_File, "w" );
		if( e_fpMatch_Fp == NULL ) {
			PrnMsg(e_fErr_Fp, stderr, "出力対応テーブルファイル<%s>がオープンできない\n", e_cpMatch_Tbl_File );
			return	( false );
		}
	}
	return	( true );
}


#define	OPTION_MAX	7
#define	BMAX		512

/** ------------------------------------------------------------------------------
// 引数チェック
--------------------------------------------------------------------------------*/
bool	main_rootin::
arg_check	(	int		argc,		// 引数個数
				char	**argv	)	// 引数列
{
	int		a_optcount[OPTION_MAX];	// オプションの重複のカウント
	int		i;
	int		a_exeflag	= 0;

	//-----------------------------------------------------------------------
	// ツールバージョン表示
	CVersion	a_cvTool_Var;
	CString		a_csVar;
	a_csVar		= a_cvTool_Var.GetFileVersion();
	m_sVersion	= (char*)(_bstr_t(a_csVar));
	//-----------------------------------------------------------------------

	for(i = 0; i < OPTION_MAX; i++)
	{
		a_optcount[i] = 0;
	}

	// 引数文字列数のチェック
	for(i = 0; i < argc; i++)
	{
		if(strlen(argv[i]) > BMAX) {
			PrnMsg(e_fErr_Fp, stderr, "引数文字列が長すぎます!!\n" );
			return	( false );
		}
		// バージョン情報表示チェック
		if(strcmp(argv[i], "-v") == 0) {
			print_version();
			return	( false );
		}
	}


	if(argc < 5) {
		return( false );
	}

	e_cpInit_File		= argv[1];	///< 初期化ファイル(住所DB接続)も含む
	e_cpCheck_Src_Layer	= argv[2];	///< チェック元レイヤ
	e_cpInit_File_Tgt	= argv[3];	///< 初期化ファイル(住所DB接続)も含む
	e_cpCheck_Tgt_Layer	= argv[4];	///< チェック先レイヤ

	// オプションだから、今のところ使用しない
	for(i = 5; i < argc;)
	{
		if(strcmp(argv[i], "-l") == 0) { // ログファイル設定（設定が無い場合は、エラーはstderrへ垂れ流しになる）
			if(argc < i+2) return( false );
			e_cpLog_File	= argv[i+1];	///< メッシュコードリストファイル
			a_optcount[0]++;
			i+=2;
		} else if(strcmp(argv[i], "-m") == 0) {
			if(argc < i+2) return( false );
			e_cpMeshCode_File	= argv[i+1];	///< メッシュコードリストファイル
			a_exeflag			|= ON_MESH;
			a_optcount[1]++;
			i+=2;
		} else if(strcmp(argv[i], "-M") == 0) {
			if(argc < i+2) return( false );
			a_exeflag			|= ON_MONO_MESH;
			e_iMeshCode			= atoi(argv[i+1]);
			a_optcount[2]++;
			i+=2;
		} else if(strcmp(argv[i], "-t") == 0) {
			if(argc < i+2) return( false );
			e_cpMatch_Tbl_File	= argv[i+1];		///< 対応リストファイル
			a_exeflag			|= ON_MATCH_FILE;
			a_optcount[4]++;
			i+=2;
		}else if(strcmp(argv[i], "-lowacc") == 0) {		///< 低精度一致モード 2006.06.12
			a_exeflag			|= ON_LOWACC_MODE;
			a_optcount[5]++;
			i+=1;
		}else if(strcmp(argv[i], "-v") == 0) {
			print_version();
			a_optcount[6]++;
			i+=1;
			return	( false );	// 仮にこうする？
		} else {
			PrnMsg(e_fErr_Fp, stderr, "オプション %s は間違ってます!!\n", argv[i]);
			return	( false );
		}
	}

	// オプション重複のチェック
	for(i = 0; i < OPTION_MAX; i++)
	{
		if(a_optcount[i] > 1) {
			PrnMsg(e_fErr_Fp, stderr, "オプションの指定が重複しています!!\n");
			return	( false );
		}
	}

	// 初期化ファイル設定
	if( e_pTool_Init.read_init_file ( e_cpInit_File ) == -1 ) {
		return	( false );	// メッセージは内部で出している
	}
	if( e_pTool_Init_Tgt.read_init_file ( e_cpInit_File_Tgt) != 0 ) {
		return ( false );
	}

	// 出力対応テーブルファイルオープン
	if( a_exeflag & ON_MATCH_FILE ) {
		Open_Match_Tbl ();
	}

	// エラーログファイルのオープン
	if( !Open_Err_Log () ) {
		return	( false );
	}else {
		if( !e_sPrnInfo.Init(e_fErr_Fp) ) {
			return	( false );
		}
	}
	eExe_Flag	= a_exeflag;

	//--------------------------------------------------------------------------
	// 実行情報を出力
	PrnMsg( e_fErr_Fp, stdout, "#ツールバージョン                : <%s>\n", m_sVersion.c_str() );
	if( eExe_Flag & ON_MESH ) {
	PrnMsg( e_fErr_Fp, stdout,  "#チェック対象メッシュリスト      : <%s>\n", e_cpMeshCode_File );
	}else {
	PrnMsg( e_fErr_Fp, stdout,  "#チェック対象メッシュコード      : <%02d>\n", e_iMeshCode );
	}
	PrnMsg( e_fErr_Fp, stdout,  "#比較元用接続初期化ファイル      : <%s>\n", e_cpInit_File );

	PrnMsg( e_fErr_Fp, stdout,  "#比較元DB接続情報                : <%s/%s/%s/%s/%s>\n",
		e_pTool_Init.get_UserName(), e_pTool_Init.get_Password(), e_pTool_Init.get_Version(), e_pTool_Init.get_Instance(), e_pTool_Init.get_Server() );
	PrnMsg( e_fErr_Fp, stdout,  "#比較元レイヤ                    : <%s>\n", e_cpCheck_Src_Layer );
	PrnMsg( e_fErr_Fp, stdout,  "#比較先用接続初期化ファイル      : <%s>\n", e_cpInit_File_Tgt );
	PrnMsg( e_fErr_Fp, stdout,  "#比較先DB接続情報                : <%s/%s/%s/%s/%s>\n",
		e_pTool_Init_Tgt.get_UserName(), e_pTool_Init_Tgt.get_Password(), e_pTool_Init_Tgt.get_Version(), e_pTool_Init_Tgt.get_Instance(), e_pTool_Init_Tgt.get_Server() );
	PrnMsg( e_fErr_Fp, stdout,  "#比較先レイヤ                    : <%s>\n", e_cpCheck_Tgt_Layer );
	PrnMsg( e_fErr_Fp, stdout,  "#出力ファイル                    : <%s>\n", e_cpMatch_Tbl_File );
	// モード表示
	if( eExe_Flag & ON_LOWACC_MODE ) {
		PrnMsg( e_fErr_Fp, stdout, "#低精度一致モード\n" );
	}else {
		PrnMsg( e_fErr_Fp, stdout, "#高精度一致モード\n" );
	}
	//--------------------------------------------------------------------------


	return		( true );

}

/** ------------------------------------------------------------------------------
// 使用法表示
--------------------------------------------------------------------------------*/
void		main_rootin::
print_usage	(char **argv)
{
	PrnMsg(stdout, NULL,
			"=== 【建物差分抽出ツール】(%s) ===\n"
				"<usage>\n"
				"%s <arg1> <arg2> <arg3> <arg4>\n"
				"\t<引数>\n"
				"\t      <arg1>         : チェック元用初期化ファイル\n"
				"\t      <arg2>         : チェック元レイヤ名	(ex. TECHMAP.Building_mc)\n"
				"\t      <arg3>         : チェック元用初期化ファイル\n"
				"\t      <arg4>         : チェック先レイヤ名	(ex. SiNDY2004.Building)\n"
				"\t<オプション>\n"
				"\t      '-l'           : ログファイル名を指定\n"
				"\t      '-t'           : 対応リストファイルを指定\n"
				"\t      '-m'           : メッシュコードリストファイルを指定\n"
				"\t      '-M'           : メッシュコードを指定（1次、2次、8桁メッシュ）\n"
				"\t      '-lowacc'      : 低精度一致モード\n"
				"\t      '-v'           : バージョン情報表示\n"
				"\t<補足>\n"
				"\t      ・'-m','-M',はどちらか必ず指定して下さい\n"
				"\t      ・'-m','-M'は同時指定できません\n"
			, argv[0], m_sVersion.c_str());
}

/** ------------------------------------------------------------------------------
	バージョン情報の表示
	@return		無し
---------------------------------------------------------------------------------*/
void	main_rootin::
print_version	( void )
{
	PrnMsg(stdout, NULL,
			"=== 【建物差分抽出ツール】(%s) ===\n"
				"\t<バージョン情報>\n"
				"\t2004/05/14        1.00               : 公開バージョン\n"
				"\t2005/12/21        1.10               : 低精度モード組み込み。\n"
				"\t2006/06/12        1.20               : 低精度モードをオプション'-lowacc'で切り替え可能に修正。\n"
				"\t2006/09/28        1.30               : 低精度一致モードの一致条件を見直し\n"
				"\t2008/05/27        2.0.0.1            : メッシュフィールド名が未修正だったので対応\n"
				"\t                                     : VC8対応\n"
				"\t2009/10/21        2.1.0.2            : TMP_PRIMIDを見ないように修正\n"
				"\t                                     : ログに実行時の情報を追加\n"
				"\t2014/02/10        2.2.0.4            : [but 10158][u][要望]14秋仕様に対応して欲しい(Building_Diff) に対応\n"
				"\t                                     : DB接続をsindy/workspace.hを使用\n"
				, m_sVersion.c_str()
//				"\t2004/03/25        1.00               : 実運用バージョン\n"
			);
}

/** ------------------------------------------------------------------------------
	メッシュ矩形をDBから獲得するメソッドの初期化
	※get_mesh_rect をやる前に必ず呼ぶ
------------------------------------------------------------------------------==*/
bool	main_rootin::
get_mesh_init	(	mk_input_list&	c_mMList,			// 入力リストクラス
					IWorkspacePtr&	c_ipWorkSpace	)	// 指定するワークスペース
{

	if( eExe_Flag & ON_MESH ) {
		if( e_cpMeshCode_File == NULL ) {
			PrnMsg(e_fErr_Fp, stderr, "メッシュコードリストファイル名がとりだせませんでした[check_exe]\n");
			return	( false );
		}	
		if( c_mMList.read_mesh_list ( e_cpMeshCode_File ) != 0 ) {	// ここでリストを読み込む
			PrnMsg(e_fErr_Fp, stderr, "メッシュコードリストファイル<%s>の読み込みに失敗[check_exe]\n", e_cpMeshCode_File );
			return	( false );
		}
	}else {
		if(c_mMList.read_mesh_list ( e_iMeshCode ) == -1) {
			PrnMsg(e_fErr_Fp, stderr, "メッシュ<%d>の設定に失敗しました\n", e_iMeshCode );
			return	( false );
		}
	}

	char*		a_cpMeshLayer	= NULL;
	int			a_iMDigit		= 0;
	a_iMDigit	= c_mMList.get_mesh_digit ();
	
	switch	( a_iMDigit )
	{
	case	8:
		a_cpMeshLayer	= e_pTool_Init.get_City_Mesh ();
		break;
	case	6:
		a_cpMeshLayer	= e_pTool_Init.get_Base_Mesh();
		break;
	case	4:
		a_cpMeshLayer	= e_pTool_Init.get_Middle_Mesh();
		break;
	default:
		PrnMsg(e_fErr_Fp, stderr, "リストのメッシュ桁数には対応していません[check_exe]\n");
		return	( false );
	}


	IFeatureWorkspacePtr	a_F_Workspace	( c_ipWorkSpace );	// フィーチャワークスペース

	// レイヤのオープン
	if( FAILED(a_F_Workspace->OpenFeatureClass(CComBSTR(a_cpMeshLayer), &e_ipF_Class )) ) {
		PrnMsg(e_fErr_Fp, stderr, "レイヤ <%s> のオープンに失敗\n", a_cpMeshLayer);
		return	(false);
	}else {
		PrnMsg(e_fErr_Fp, stderr,	"#<%s> レイヤをオープン\n", a_cpMeshLayer);
	}

	// 2002.12.09 データがある部分のメッシュリスト
	eDigit	= a_iMDigit;

	return	( true );
}

/** ------------------------------------------------------------------------------
	メッシュ矩形をGeoDBから取得する \
	※GeoDBへの接続が確立されていることが前提
--------------------------------------------------------------------------------*/
IEnvelopePtr	main_rootin::
get_mesh_rect	( int	c_iMeshCode	)	///< メッシュコード
{

	CComBSTR	a_SpatialCol;
	e_ipF_Class->get_ShapeFieldName	( &a_SpatialCol );
	BSTR		a_Where_Str;
	char		a_tmpstr[30];	//　ここはなんとかしたい気がする

	crd_cnv		crd;
	int			a_iDigit	= 0;
	a_iDigit	= crd.GetMeshDigit( c_iMeshCode );
	sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// メッシュコードを文字列に
	eDigit	= a_iDigit;

	a_Where_Str	= T2BSTR(_T(a_tmpstr));		// stringをBSTRに変換

	IQueryFilterPtr		a_Sp_Filter	( CLSID_QueryFilter );			// 空間検索フィルタ
//	a_Sp_Filter->put_SearchOrder	( esriSearchOrderAttribute );	// 属性検索優先
	a_Sp_Filter->put_WhereClause	( a_Where_Str);					// Where句の設定
	
	// クリップ矩形作成
	IEnvelopePtr	a_ipMesh_Rect;	// メッシュ矩形
	a_ipMesh_Rect	= NULL;

	// サーチしてくる
	IFeatureCursorPtr	a_F_Cursor;
	if( FAILED	(e_ipF_Class->Search	( a_Sp_Filter, VARIANT_FALSE, &a_F_Cursor )) ) {
		PrnMsg(e_fErr_Fp, stderr, "データの属性検索に失敗\n");
		return	(NULL);
	}else {
		IFeaturePtr				a_Feature;

		while(a_F_Cursor->NextFeature ( &a_Feature ) == S_OK)
		{		
			IGeometryPtr		a_Clip_Shp;		// こいつにデータをコピーしてクリップする

			if( FAILED(a_Feature->get_ShapeCopy	( &a_Clip_Shp )) ) {	// フィーチャから
				PrnMsg(e_fErr_Fp, stderr, "シェイプのコピーに失敗\n");
				return  ( NULL );
			}
			
			VARIANT_BOOL	a_bResult;
			a_Clip_Shp->get_IsEmpty( &a_bResult );
			if(a_bResult == VARIANT_TRUE) {	// クリップ結果がNULLならメッシュ外
				continue;
			}
			if( FAILED(a_Clip_Shp->get_Envelope( &a_ipMesh_Rect) )) {
				PrnMsg(e_fErr_Fp, stderr, "該当メッシュ<%d>の矩形がみつかりませんでした\n", c_iMeshCode);
				return	( NULL );
			}else {
				return	( a_ipMesh_Rect );
			}
		}
	}
	return	( a_ipMesh_Rect );
}

/** ------------------------------------------------------------------------------
	矩形のポリゴンを返す
	@return	IPolygonPtr
--------------------------------------------------------------------------------*/
IPolygonPtr	main_rootin::
Get_Rect_Polygon	(	IEnvelopePtr&	c_ipEnv	)		///< メッシュ矩形
{
	//
	// 2002.07.01 Intersect用のジオメトリを作成
	IPolygonPtr		a_ipRectPoly(CLSID_Polygon);	// 矩形用ジオメトリ
	IPointCollectionPtr	a_ipRecPts	(a_ipRectPoly);		// 矩形用点列

	IPointPtr			a_Pt[5];

	c_ipEnv->get_LowerLeft (&a_Pt[0]);
	c_ipEnv->get_UpperLeft (&a_Pt[1]);
	c_ipEnv->get_UpperRight(&a_Pt[2]);
	c_ipEnv->get_LowerRight(&a_Pt[3]);
	c_ipEnv->get_LowerLeft (&a_Pt[4]);

	int	roop = 0;
	for ( roop = 0; roop < 5; roop++ )
	{
		a_ipRecPts->AddPoint(a_Pt[roop]);
	}

	// 梶浦さんのところから抜粋 2004.07.15
	double aMinX, aMinY, aMaxX, aMaxY;
	aMinX = aMinY = 1000000.0;
	aMaxX = aMaxY = -1000000.0;
	for( roop = 0; roop < 5; roop++ )
	{
		double	a_dTmpX	= 0.0, a_dTmpY = 0.0;
		a_Pt[roop]->get_X ( &a_dTmpX );
		a_Pt[roop]->get_Y ( &a_dTmpY );

		if(aMinX > a_dTmpX)
			aMinX = a_dTmpX;
		if(aMinY > a_dTmpY)
			aMinY = a_dTmpY;
		if(aMaxX < a_dTmpX)
			aMaxX = a_dTmpX;
		if(aMaxY < a_dTmpY)
			aMaxY = a_dTmpY;
	}

	m_CompMaxX = aMaxX;
	m_CompMaxY = aMaxY;
	m_CompMinX = aMinX;
	m_CompMinY = aMinY;
	//　抜粋ここまで

	// シンプリファイをかける
	ITopologicalOperator2Ptr	a_ipTopo;

	a_ipTopo	= a_ipRectPoly;

	// スナップする　2004.07.14
	ISpatialReferencePtr	a_ipSpRef;
	c_ipEnv->get_SpatialReference( &a_ipSpRef );
	a_ipRectPoly->putref_SpatialReference( a_ipSpRef );
	a_ipRectPoly->SnapToSpatialReference();

	if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
		PrnMsg(e_fErr_Fp, stderr, "Simplifyに失敗\n");
		return	( NULL );
	}
	if(FAILED ( a_ipRectPoly->SimplifyPreserveFromTo() ) ) {
		PrnMsg(e_fErr_Fp, stderr, "SimplifyPreserveFromToに失敗" );	// エラーメッセージ
		return	( NULL );
	}
	return ( a_ipRectPoly );
}

/** ------------------------------------------------------------------------------
	レイヤオープン
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Layers		( 	IWorkspacePtr&	ipWorkspace_Src,	// チェック元ワークスペース[IN]
					IWorkspacePtr&	ipWorkspace_Tgt )	// チェック先ワークスペース[IN]
{

	if( !e_mChk_Src_WS.Init ( ipWorkspace_Src, e_cpCheck_Src_Layer ) ) {
		PrnMsg(e_fErr_Fp, stderr, "チェック元レイヤ<%s>の読み込みに失敗\n", e_cpCheck_Src_Layer );
		return	( false );
	}
	if( !e_mChk_Tgt_WS.Init ( ipWorkspace_Tgt, e_cpCheck_Tgt_Layer ) ) {
		PrnMsg(e_fErr_Fp, stderr, "チェック先レイヤ<%s>の読み込みに失敗\n", e_cpCheck_Tgt_Layer );
		return	( false );	
	}
	return	( true );
}

/**
	読み込んだデータをセットに落とす
	@return	データ数
*/
int		main_rootin::
Set_Data_Infos (	IFeatureCursorPtr&	c_ipChk_Cursor,	///< チェック元カーソル[IN]
					ms_data_info&		c_ipChk_Data )	///< チェック元データのセット[OUT]
{
	int	a_iCount	= 0;
	// データをマルチセットのデータに落とす
	if( c_ipChk_Cursor != NULL ) {
		IFeaturePtr	a_ipNote_Feat;
		// 検索された注記のループ
		while(c_ipChk_Cursor->NextFeature ( &a_ipNote_Feat ) == S_OK)
		{
			data_info	a_dChk_Data;

			Feature_All_Attrs		a_fChk_Src_Attrs;
			a_fChk_Src_Attrs.Init	( a_ipNote_Feat );
			// オブジェクトID
			Feature_Attr a_fObjID, /*a_fMeshCode, a_fPrim_ID,*/ a_fAttr, a_fCORRIDOR_F, a_fNOWALL_F;
			a_fObjID     = a_fChk_Src_Attrs.Get_Attr ( "OBJECTID" );
			a_fAttr      = a_fChk_Src_Attrs.Get_Attr ( "BLDCLASS_C" );
			a_fCORRIDOR_F= a_fChk_Src_Attrs.Get_Attr ( "CORRIDOR_F" );
			a_fNOWALL_F  = a_fChk_Src_Attrs.Get_Attr ( "NOWALL_F" );

			// 除外種別の判定
			int ret = mAKAttrHandler.fnIsExcluded ( a_fAttr.e_ccvAttr.lVal, a_fNOWALL_F.e_ccvAttr.lVal, a_fCORRIDOR_F.e_ccvAttr.lVal );
			if( ret != 0 ) {
				// 14秋以前のバージョンでも使用するのと、混乱を招く要素があるので除外データの情報は出力しません。
				continue;
			}
			// 除外でない場合は、属性値の変換
			mAKAttrHandler.fnAttrConv ( a_fAttr.e_ccvAttr.lVal );

			a_dChk_Data.e_iOID		= a_fObjID.e_ccvAttr.intVal;
			a_dChk_Data.e_iMeshCode	= e_iMeshCode;
			a_dChk_Data.e_iAttr		= a_fAttr.e_ccvAttr.intVal;
			a_dChk_Data.e_ipGeo		= a_fChk_Src_Attrs.Ret_Geo();

			c_ipChk_Data.insert ( a_dChk_Data );
			a_iCount++;
		}
	}
	return	( a_iCount );
}

/** ------------------------------------------------------------------------------
	形状のマッチング情報を取得する
	@return	STAT_EQUAL	一致
	@return	STAT_MODIFY	形状変更
	@return	STAT_LOWA_EQUAL	形状一致（低精度）
	@return	STAT_LOWA_MODIFY	形状変更（低精度）
	@return	0	不一致
--------------------------------------------------------------------------------*/
int		main_rootin::
Get_Match_Info ( data_info&		c_dSrcDat,			///< チェック元データ[IN]
				 data_info&		c_dTgtDat,			///< チェック先データ[IN]
				 ms_data_info&	c_ipChk_Tgt_Data )	///< チェック元データのセット[IN]
{
	// 形状一致のチェック
	int	a_iRetStat = 0;
	a_iRetStat	= Shape_Chk_and_Ret_Stat ( c_dSrcDat, c_dTgtDat );
	c_dSrcDat.e_iStat	= a_iRetStat;
	if( a_iRetStat != 0 ) {
		if( a_iRetStat == STAT_MODIFY ) {	// 「形状変更」の場合
			ims_data_info	a_iIndx;
			a_iIndx	= c_ipChk_Tgt_Data.find ( c_dSrcDat );
			if( a_iIndx == c_ipChk_Tgt_Data.end() ) { //　メッシュで引っ掛けたレコードに存在しないものと一致している場合
				c_dSrcDat.e_iStat	|=	STAT_MESHOUTER;	// 「メッシュ外」を追加
			}
		}
	}
	return	( a_iRetStat );
}

/** ------------------------------------------------------------------------------
	形状一致のチェックを行い、状態を返す
	@return	STAT_EQUAL	一致
	@return	STAT_MODIFY	形状変更
	@return	STAT_LOWA_EQUAL	形状一致（低精度）
	@return	STAT_LOWA_MODIFY	形状変更（低精度）
	@return	0	不一致
--------------------------------------------------------------------------------*/
int		main_rootin::
Shape_Chk_and_Ret_Stat ( data_info&		c_dSrcDat,			///< チェック元データ[IN]
						 data_info&		c_dTgtDat )			///< チェック先データ[IN]
{
	int	a_iRetStat	= 0;
	if( Shape_Chk ( c_dSrcDat.e_ipGeo, c_dTgtDat.e_ipGeo ) ) {
		a_iRetStat	= STAT_EQUAL;	// 形状一致
	}else {
		// 形状一致（低精度）,形状変更（低精度）,不一致の判定
		a_iRetStat	= Shape_Chk_LowAccuracy ( c_dSrcDat.e_ipGeo, c_dTgtDat.e_ipGeo );
		if( a_iRetStat == 0 ) {	// 不一致と判定されたの場合
			if( c_dSrcDat.e_iOID == c_dTgtDat.e_iOID ) {	// OBJECTIDが一致する場合は、「形状変更」扱いに
				a_iRetStat	= STAT_MODIFY;
			}
		}
	}
	return	( a_iRetStat );
}

/** ------------------------------------------------------------------------------
	形状一致のチェック
	@return	true	一致
	@return	false	不一致
--------------------------------------------------------------------------------*/
bool	main_rootin::
Shape_Chk ( IGeometryPtr&	c_dSrcGeo,		///< チェック元データ[IN]
		    IGeometryPtr&	c_dTgtGeo	)	///< チェック先データ[IN]
{
	IRelationalOperatorPtr	a_ipEqual_Chk;

	//---------------------------------------------------------------------------
	// 2006.06.17 比較の前に、形状同士が関係あるかを調べる
	VARIANT_BOOL	a_vResult;
	IRelationalOperatorPtr	a_ipSrcRel;
	a_ipSrcRel	= c_dSrcGeo;
	if( FAILED(a_ipSrcRel->Disjoint ( c_dTgtGeo, &a_vResult ) )) {
		e_sPrnInfo.Prn_SiNDY_Err ( 0, "Disjoint関係取得に失敗", NULL );
	}else {
		if( a_vResult == VARIANT_TRUE ) {
			return ( false );
		}
	}
	// 2006.06.17 ---------------------------------------------------------------

	// テスト用------------------------------------
	IPointCollectionPtr	a_ipPts_Src, a_ipPts_Tgt;
	a_ipPts_Src	= c_dSrcGeo;
	a_ipPts_Tgt	= c_dTgtGeo;

	long	a_lSrc_Count = 0, a_lTgt_Count = 0;
	a_ipPts_Src->get_PointCount ( &a_lSrc_Count );
	a_ipPts_Tgt->get_PointCount ( &a_lTgt_Count );

	// 補間点数の差分
	if( a_lSrc_Count != a_lTgt_Count ) {
		return ( false );
	}

	IGeometryCollectionPtr	a_ipSrcGeoCol, a_ipTgtGeoCol;
	a_ipSrcGeoCol	= c_dSrcGeo;
	a_ipTgtGeoCol	= c_dTgtGeo;
	
	// おそらくほとんど意味はないであろうが念のためチェックし、検出された場合はエラーログに出力しておく
	long	a_lSrc_Col_Count = 0, a_lTgt_Col_Count = 0;
	if( a_lSrc_Col_Count != a_lTgt_Col_Count ) {
		return ( false );
	}

	WKSPoint*	a_wpSrcWKSPoints = NULL;
	WKSPoint*	a_wpTgtWKSPoints = NULL;
	a_wpSrcWKSPoints	= new WKSPoint[a_lSrc_Count];
	a_wpTgtWKSPoints	= new WKSPoint[a_lTgt_Count];

	a_ipPts_Src->QueryWKSPoints ( 0, a_lSrc_Count, a_wpSrcWKSPoints );
	a_ipPts_Tgt->QueryWKSPoints ( 0, a_lTgt_Count, a_wpTgtWKSPoints );
	
	bool	a_bRet = false;
	if( memcmp ( a_wpSrcWKSPoints, a_wpTgtWKSPoints, sizeof(WKSPoint) * a_lSrc_Count ) == 0 ) {
		a_bRet	= true;
	}

	delete [] a_wpSrcWKSPoints;
	delete [] a_wpTgtWKSPoints;

	return	( a_bRet );
	//--------------------------------------------
}

/** ------------------------------------------------------------------------------
	ジオメトリから点列の情報を取得する
	@return		無し
--------------------------------------------------------------------------------*/
void	main_rootin::
Get_PtsWithOrientation	( IGeometryPtr&	c_ipGeo,		///< ジオメトリ[IN]
						  set<Real_Pt2>& c_srPts	)	///< 点列[OUT]
{
	IGeometryCollectionPtr	a_ipGeoCol;

	a_ipGeoCol	= c_ipGeo;

	long	a_lGeoCount = 0;

	a_ipGeoCol->get_GeometryCount ( &a_lGeoCount );

	int	a_iGRoop = 0;
	for( a_iGRoop = 0; a_iGRoop < a_lGeoCount; a_iGRoop++ )
	{
		IGeometryPtr	a_iTmpGeo;
		a_ipGeoCol->get_Geometry (a_iGRoop, &a_iTmpGeo);

		IRingPtr		a_iTmpRing;
		a_iTmpRing	= a_iTmpGeo;
		
		// リングがインテリアかの判定
		int	a_iInterior_Flag = 0;
		VARIANT_BOOL	a_vIsExterior;
		a_iTmpRing->get_IsExterior( &a_vIsExterior );
		if( a_vIsExterior == VARIANT_FALSE ) {
			a_iInterior_Flag	= 1;
		}

		IPointCollectionPtr	a_ipTmpPts;
		a_ipTmpPts	= a_iTmpGeo;

		// 現在Geometryの補間点数
		long	a_lSrc_Count = 0;
		a_ipTmpPts->get_PointCount ( &a_lSrc_Count );

		// リングの点列を取得
		WKSPoint*	a_wpSrcWKSPoints = NULL;
		a_wpSrcWKSPoints	= new WKSPoint[a_lSrc_Count];

		a_ipTmpPts->QueryWKSPoints ( 0, a_lSrc_Count, a_wpSrcWKSPoints );

		// 点列をセットに格納する
		set<Real_Pt2>	a_sPts;
		int i = 0;
		for( i = 0; i < a_lSrc_Count; i++ ) {
			Real_Pt2	a_iPt;
			a_iPt		= a_wpSrcWKSPoints[i];
			a_iPt.e_iInteriorFlag	= a_iInterior_Flag;
			c_srPts.insert ( a_iPt );
		}
		delete [] a_wpSrcWKSPoints;
	}
}

/** ------------------------------------------------------------------------------
	形状一致のチェック（低精度版） 2006.09.25
	※オブジェクトの重複部の面積を算出し、重複面積がチェック元データの1/10以下なら
	　不一致として判定するようにした。
	　中抜き部分は、そのポイントが外側か内側のものかの情報をみて判定するようにした。
	@return	STAT_LOWA_EQUAL		形状一致（低精度）
	@return	STAT_LOWA_MODIFY	形状変更（低精度）
	@return	0	不一致
--------------------------------------------------------------------------------*/
int		main_rootin::
Shape_Chk_LowAccuracy ( IGeometryPtr&	c_ipSrcGeo,		///< チェック元データ[IN]
						IGeometryPtr&	c_ipTgtGeo	)	///< チェック先データ[IN]
{
	// オブジェクトの重複部の面積を算出し、重複面積がチェック元データの1/10以下なら不一致として判定
	//--------------------------------------------------------------------------------------------
	ITopologicalOperatorPtr	a_ipTmpTopo(c_ipSrcGeo);
	IGeometryPtr	a_ipDupliPart;
	
	if( FAILED ( a_ipTmpTopo->Intersect ( c_ipTgtGeo, esriGeometryNoDimension, &a_ipDupliPart) )) {
		return ( 0 );
	}else {
		IAreaPtr			a_ipSrcPolyArea, a_ipDupliPolyArea;
		a_ipSrcPolyArea		= c_ipSrcGeo;
		a_ipDupliPolyArea	= a_ipDupliPart;

		double	a_dSrcArea = 0.0, a_dDupliArea = 0.0;

		if( FAILED ( a_ipSrcPolyArea->get_Area ( &a_dSrcArea ) )) {
			return ( 0 );
		}
		if( FAILED ( a_ipDupliPolyArea->get_Area ( &a_dDupliArea ) )) {
			return ( 0 );
		}

		if( a_dDupliArea < (a_dSrcArea / 10.0) ) {
			return ( 0 );
		}
	}	
	//--------------------------------------------------------------------------------------------

	// 閾値定義（正規化XYで11pt相当（検索のためちょっと広めに））
	const double a_cdTH_X = 0.000000171875;
	const double a_cdTH_Y = 0.000000114583336;

	// 点列をセットに格納する
	set<Real_Pt2>	a_sSrcPts, a_sTgtPts;

	// リングの回り向き付きで点列を取得
	Get_PtsWithOrientation ( c_ipSrcGeo, a_sSrcPts );
	Get_PtsWithOrientation ( c_ipTgtGeo, a_sTgtPts );

	// 80%以上一致の場合は形状変更とする(小数点以下は切り捨て）
	int	a_iBorderCount = 0;
	a_iBorderCount	= (double)a_sSrcPts.size() * 0.80;

	// 比較元点列とのヒット数を調べる
	int	a_iHit_Count	= 0;
	set<Real_Pt2>::iterator	a_iSrcIndx, a_iLowerIndx, a_iUpperIndx;
	for( a_iSrcIndx = a_sSrcPts.begin(); a_iSrcIndx != a_sSrcPts.end(); a_iSrcIndx++ )
	{
		Real_Pt2	a_rKey	= *a_iSrcIndx;

		// 検索区間の作成
		Real_Pt2			a_sLowerKey, a_sUpperKey;
		a_sLowerKey	= a_rKey;
		a_sUpperKey	= a_rKey;

		// X方向に近いオブジェクトをひっかけてくるので、X方向だけ変えたキーでよい
		a_sLowerKey.e_iX	-= a_cdTH_X;
		a_sUpperKey.e_iX	+= a_cdTH_X;

		a_iLowerIndx	= a_sTgtPts.lower_bound ( a_sLowerKey );
		a_iUpperIndx	= a_sTgtPts.upper_bound ( a_sUpperKey );

		int	a_iEnd_Flag	= 0;	// 作業終了フラグ

		set<Real_Pt2>::iterator	a_iChkIndx;
		for( a_iChkIndx = a_iLowerIndx; ; a_iChkIndx++ )
		{
			// a_iChkIndx が
			if( a_iEnd_Flag == 1 || a_iChkIndx == a_sTgtPts.end() ) {
				break;
			}else if( a_iChkIndx == a_iUpperIndx ) {	// 次ループで終了
				a_iEnd_Flag	= 1;
			}
			Real_Pt2	a_rCur	= *a_iChkIndx;
			
			// 比較元と比較先のデータの座標差分値が、X方向<=a_cdTH_X Y方向<=a_cdTH_Y ならOKとする
			double		a_dDiff_X = 0.0, a_dDiff_Y = 0.0;
			a_dDiff_X = fabs ( a_rKey.e_iX - a_rCur.e_iX );
			a_dDiff_Y = fabs ( a_rKey.e_iY - a_rCur.e_iY );

			if( (a_dDiff_X <= a_cdTH_X) && (a_dDiff_Y <= a_cdTH_Y) && 
				(a_rKey.e_iInteriorFlag == a_rCur.e_iInteriorFlag) )
			{
				a_iHit_Count++;
				break;
			}
		}
	}

	int	a_iRet	= 0;
	// 比較元点列のサイズと比較先点列のサイズとヒット数が一致した場合は一致
	if( a_sSrcPts.size() == a_sTgtPts.size() && a_sSrcPts.size() == a_iHit_Count ) {
		a_iRet	= STAT_LOWA_EQUAL;	// 低精度一致
	}else {
		if( a_iHit_Count >= a_iBorderCount ) {
			a_iRet	= STAT_LOWA_MODIFY;
		}
	}
	return	( a_iRet );
}

#ifdef _DEBUG
void	main_rootin::
Print_Geo	( data_info&	c_dSrc,
			  data_info&	c_dTgt )
{
	IPointCollectionPtr	a_ipSrcPts( c_dSrc.e_ipGeo );
	IPointCollectionPtr	a_ipTgtPts( c_dTgt.e_ipGeo );

	long	a_lSrcCount	= 0, a_lTgtCount = 0;
	a_ipSrcPts->get_PointCount ( &a_lSrcCount );
	a_ipTgtPts->get_PointCount ( &a_lTgtCount );

	PrnMsg(e_fErr_Fp, stdout, "元点数（%d)\n", a_lSrcCount );
	int	i = 0, j = 0;
	for( i = 0; i < a_lSrcCount; i++ )
	{
		double		x=0.0, y=0.0;
		IPointPtr	a_iPt;
		a_ipSrcPts->get_Point ( i, &a_iPt );
		a_iPt->get_X ( &x );
		a_iPt->get_Y ( &y );
		PrnMsg(e_fErr_Fp, stdout, "pt[%d]\t%0.20f\t%0.20f\n", i, x, y );
	}
	PrnMsg(e_fErr_Fp, stdout, "先点数（%d)\n", a_lTgtCount );
	for( j = 0; j < a_lTgtCount; j++ )
	{
		double		x=0.0, y=0.0;
		IPointPtr	a_iPt;
		a_ipTgtPts->get_Point ( j, &a_iPt );
		a_iPt->get_X ( &x );
		a_iPt->get_Y ( &y );
		PrnMsg(e_fErr_Fp, stdout, "pt[%d]\t%0.20f\t%0.20f\n", j, x, y );
	}
}
#endif

/**
	メッシュ境界に存在するデータを取得し、マージした図形を作成する
	※マージ後のジオメトリとマージした中で最も若いメッシュ番号とPrimIDを代表として返す
	※メッシュ内部のデータについても内部でメッシュ境界との判定をし、メッシュ境界でなければ
	　falseの値を返すものとする
	@return	true	メッシュ境界で隣接するデータがあった場合
	@return	false	隣接データが無い場合
*/
bool	main_rootin::
Get_Neibour_Obj ( 	data_info&			c_ipChk_Src_Data,	///< チェック元データ[IN]
					bool				c_iMFlag,			///< 一度マージしたデータかどうかのフラグ
					msi_handle_ws&		c_mChk_Src_WS,		///< 自分のワークスペースハンドラ[IN]
					char*				c_cpCheck_Src_Layer,///< チェックレイヤ名[IN]
					data_info&			c_ipMerged_Data	)	///< 隣接形状をマージしたもの[OUT]
{
	HRESULT	hr;
	
	// 初期化
	c_ipMerged_Data	= c_ipChk_Src_Data;

	// 一度もマージしていないデータについてはメッシュ境界との接触判定を行う
	if( c_iMFlag == false ) {
		// メッシュ境界とオブジェクトの接触チェック
		VARIANT_BOOL			a_vBool;
		IRelationalOperatorPtr	a_ipRel;
		a_ipRel	= e_ipMeshRect_Line;
		hr	= a_ipRel->Touches( c_ipChk_Src_Data.e_ipGeo, &a_vBool );
		
		if( FAILED (hr) )				{ return ( false ); }
		if( a_vBool != VARIANT_TRUE )	{ return ( false ); }
	}
	
	// 自分に接触するオブジェクトを検索してくる
	IFeatureCursorPtr	a_ipTouch_Datas;

	a_ipTouch_Datas	= c_mChk_Src_WS.Search_Feature ( c_ipChk_Src_Data.e_ipGeo, esriSpatialRelTouches, "", RET_FCURSOR);
	if (a_ipTouch_Datas == NULL ) { 
		return ( false );
	}

	// データをセットに落とす
	ms_data_info		a_msTouch_Datas;
	ims_data_info		a_imsIndx;
	int					a_iTouches_Count	= 0;
	a_iTouches_Count	= Set_Data_Infos ( a_ipTouch_Datas, a_msTouch_Datas );
	if( a_iTouches_Count == 0 ) {
		return	( false );
	}

	IGeometryPtr	a_ipMrgDat;
	a_ipMrgDat		= c_ipChk_Src_Data.e_ipGeo;
	// 初期化
	data_info		a_ipTmpMerged_Data;
	a_ipTmpMerged_Data	= c_ipChk_Src_Data;
	// メッシュ境界と接するものだけマージしていく

	// DEBUG------------------------------
	ISpatialReferencePtr	a_ipSpRef;
	c_ipChk_Src_Data.e_ipGeo->get_SpatialReference ( &a_ipSpRef );
	//------------------------------------

	int	a_iEqual_F	= 0;
	for( a_imsIndx = a_msTouch_Datas.begin(); a_imsIndx != a_msTouch_Datas.end(); a_imsIndx++ )
	{
		data_info				a_dTouchDat	= *a_imsIndx;
		// マージ判定
		if( fnIsNeedMergePolygon2(	c_ipChk_Src_Data.e_ipGeo, a_dTouchDat.e_ipGeo ) ) {
			IGeometryPtr	a_ipTmpGeo;
			ITopologicalOperatorPtr	a_ipMrgTopo;
			a_ipMrgTopo	= a_ipMrgDat;
			hr	= a_ipMrgTopo->Union ( a_dTouchDat.e_ipGeo, &a_ipTmpGeo );
			if( FAILED (hr) ) {
				continue;
			}
			a_ipMrgDat	= a_ipTmpGeo;
			// 一番若いメッシュ番号の一番若いPrimIDを振る
			if( a_ipTmpMerged_Data.e_iMeshCode != a_dTouchDat.e_iMeshCode ) {
				if( a_ipTmpMerged_Data.e_iMeshCode > a_dTouchDat.e_iMeshCode ) {
					a_ipTmpMerged_Data	= a_dTouchDat;
				}
			}else if( a_ipTmpMerged_Data.e_iOID != a_dTouchDat.e_iOID ) {
				if( a_ipTmpMerged_Data.e_iOID > a_dTouchDat.e_iOID ) {
					a_ipTmpMerged_Data	= a_dTouchDat;
				}		
			}
			// DEBUG------------------------------
			a_ipTmpMerged_Data.e_ipGeo->putref_SpatialReference ( a_ipSpRef );
			//------------------------------------
		}
	}
	a_ipTmpMerged_Data.e_ipGeo	= a_ipMrgDat;
	if( a_ipTmpMerged_Data.e_ipGeo == c_ipChk_Src_Data.e_ipGeo ) {
		return	( false );
	}
	// ここで再帰させて、マージすべきオブジェクトを全てマージした後の形状を返す
	if( !Get_Neibour_Obj ( a_ipTmpMerged_Data, true, c_mChk_Src_WS, c_cpCheck_Src_Layer, c_ipMerged_Data )) {
		return	( true );	// ここに入っている場合はマージしているのでtrueを返却
	}
	return ( true );	
}

/** ------------------------------------------------------------------------------
	低精度オブジェクトの作成（ポリゴン）
--------------------------------------------------------------------------------*/
IGeometryPtr	main_rootin::
Get_LowAccuracy_Obj ( IGeometryPtr&	c_ipSrcObj,	// オブジェクト
					  int			c_iValue )	// 小数点以下何分の１まで見るか
{
	//----------------------------------------------------------
	// リング数を取得
	IGeometryCollectionPtr	a_ipGeoCol;
	a_ipGeoCol	= c_ipSrcObj;
	long	a_lGeoCount	= 0;	
	a_ipGeoCol->get_GeometryCount ( &a_lGeoCount );

	// ポリゴンを作成する
	IPolygonPtr				a_ipRetPoly (CLSID_Polygon);
	IGeometryPtr			a_ipRetGeo ( a_ipRetPoly );
	IGeometryCollectionPtr	a_ipRetCol ( a_ipRetGeo );

	int	a_iGRoop = 0;
	for( a_iGRoop = 0; a_iGRoop < a_lGeoCount; a_iGRoop++ )
	{
		IGeometryPtr	a_iTmpGeo;
		a_ipGeoCol->get_Geometry (a_iGRoop, &a_iTmpGeo);

		IPointCollectionPtr	a_ipTmpPts;
		a_ipTmpPts	= a_iTmpGeo;

		// 現在Geometryの補間点数
		long	a_lSrc_Count = 0;
		a_ipTmpPts->get_PointCount ( &a_lSrc_Count );

		// リングの点列を取得
		WKSPoint*	a_wpSrcWKSPoints = NULL;
		a_wpSrcWKSPoints	= new WKSPoint[a_lSrc_Count];

		a_ipTmpPts->QueryWKSPoints ( 0, a_lSrc_Count, a_wpSrcWKSPoints );

		// 精度を落とす
		int	i = 0;
		_int64	a_iTmpX = 0, a_iTmpY = 0;
		for( i = 0; i < a_lSrc_Count; i++ )
		{
			a_iTmpX = a_wpSrcWKSPoints[i].X * c_iValue;
			a_iTmpY = a_wpSrcWKSPoints[i].Y * c_iValue;
			// 精度を落としたので元の点列に値を戻す
			a_wpSrcWKSPoints[i].X	= (double)a_iTmpX / c_iValue;
			a_wpSrcWKSPoints[i].Y	= (double)a_iTmpY / c_iValue;
		}

		// 精度落としたリングを作成
		IRingPtr			a_ipNewRing (CLSID_Ring);
		IPointCollectionPtr	a_ipNewPolyPts ( a_ipNewRing );
		IGeometryPtr		a_ipNewGeo ( a_ipNewRing );

		a_ipNewPolyPts->SetWKSPoints ( a_lSrc_Count, a_wpSrcWKSPoints );
		// リングを追加
		a_ipRetCol->AddGeometry ( (IGeometryPtr)a_ipNewRing );

		delete [] a_wpSrcWKSPoints;
	
	}

	//----------------------------------------------------------

	// 空間参照を付与する
	ISpatialReferencePtr	a_ipSpRef;
	c_ipSrcObj->get_SpatialReference( &a_ipSpRef );
	a_ipRetPoly->putref_SpatialReference( a_ipSpRef );
	// スナップする　2004.07.14
	a_ipRetPoly->SnapToSpatialReference();

	// シンプリファイをかける
	ITopologicalOperator2Ptr	a_ipTopo;
	a_ipTopo	= a_ipRetPoly;

	if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
		e_sPrnInfo.Prn_SiNDY_Err ( 0, "Simplifyに失敗", NULL );
		return	( NULL );
	}
	if( FAILED ( a_ipTopo->Simplify () ) ) {
		e_sPrnInfo.Prn_SiNDY_Err ( 0, "Simplifyに失敗", NULL );
		return	( NULL );
	}

	//----------------------------------------------------------
	IGeometryCollectionPtr	a_ipGeoCol2;
	a_ipGeoCol2	= a_ipRetGeo;
	long	a_lGeoCount2	= 0;	
	a_ipGeoCol2->get_GeometryCount ( &a_lGeoCount2 );

	if( a_lGeoCount > 1 ) {
		char	tmpstr[256];
		sprintf(tmpstr, "中抜き 元 = %d : 作成後 = %d", a_lGeoCount,  a_lGeoCount2 );
		e_sPrnInfo.Prn_SiNDY_Err ( 0, tmpstr, NULL );
	}

	//----------------------------------------------------------

	return	( a_ipRetGeo );
}

/** ------------------------------------------------------------------------------
	合致したデータをマッチテーブルに登録する
	@return	無し
--------------------------------------------------------------------------------*/
void	main_rootin::
Entry_to_Match_Tbl(	data_info&	c_dData1,	// 比較元
					data_info&	c_dData2 )	// 比較先
{
	match_record		a_mEntry;
	a_mEntry.e_dData1	= c_dData1;
	a_mEntry.e_dData2	= c_dData2;
	e_msMatch_Tbl.insert ( a_mEntry );
}

/** ------------------------------------------------------------------------------
	対応テーブルの出力
	@return	無し
--------------------------------------------------------------------------------*/
void	main_rootin::
Print_Table		(	ms_data_info&		c_ipChk_Src_Data,		///< チェック元データのセット[IN]
					ms_data_info&		c_ipChk_Tgt_Data	)	///< チェック元データのセット[IN]
{
	FILE*	a_fpOutFp	= NULL;

	if( e_fpMatch_Fp == NULL ) {
		a_fpOutFp	= stdout;	// 指定がない場合は標準出力に出力
	}else {
		a_fpOutFp	= e_fpMatch_Fp;
	}

	char	a_cpPrintBuf[512];

	ims_match_record	a_iIndx;
	for ( a_iIndx = e_msMatch_Tbl.begin(); a_iIndx != e_msMatch_Tbl.end(); a_iIndx++ )
	{
		match_record	a_mCur_Rec	= *a_iIndx;

		// メッシュコードが無い場合には現在着目中のメッシュを付与する
		if( a_mCur_Rec.e_dData1.e_iMeshCode == 0) {
			a_mCur_Rec.e_dData1.e_iMeshCode	= e_iMeshCode;
		}

		// メッシュ＋PrimIDと、ターゲットのオブジェクトIDを表示
		sprintf	( a_cpPrintBuf, "%d\t%d\t%d\t%d\t%d", a_mCur_Rec.e_dData1.e_iMeshCode,
					a_mCur_Rec.e_dData1.e_iOID, a_mCur_Rec.e_dData2.e_iOID,
					a_mCur_Rec.e_dData1.e_iAttr, a_mCur_Rec.e_dData2.e_iAttr );
		//形状の一致について
		if( a_mCur_Rec.e_dData1.e_iStat & STAT_EQUAL ) {
			strcat	( a_cpPrintBuf, "\t形状一致１" );
		}else {
			if( a_mCur_Rec.e_dData1.e_iStat & STAT_LOWA_EQUAL ) {
				strcat	( a_cpPrintBuf, "\t形状一致２" );			
			}else if( a_mCur_Rec.e_dData1.e_iStat & STAT_LOWA_MODIFY ) { 
				strcat	( a_cpPrintBuf, "\t形状変更２" );
			}else {
				strcat	( a_cpPrintBuf, "\t形状変更１" );
			}
		}

		// 隣接情報
		if( a_mCur_Rec.e_dData1.e_iStat & STAT_RINOBJ ) {
			strcat	( a_cpPrintBuf, "\t隣接複数\n" );
		}else {
			strcat	( a_cpPrintBuf, "\n" );
		}
		// 出力
		fprintf ( a_fpOutFp, "%s", a_cpPrintBuf);
	}
	// 削除データの出力
	ims_data_info	a_iSrc_Indx;
	for( a_iSrc_Indx = c_ipChk_Src_Data.begin(); a_iSrc_Indx != c_ipChk_Src_Data.end(); a_iSrc_Indx++ )
	{
		data_info	a_dCur_Dat	= *a_iSrc_Indx;
		if( a_dCur_Dat.e_iMeshCode == 0) {
			a_dCur_Dat.e_iMeshCode	= e_iMeshCode;
		}
		fprintf	( a_fpOutFp, "%d\t%d\t%d\t削除データ\n", a_dCur_Dat.e_iMeshCode, 0, a_dCur_Dat.e_iOID );
	}

	e_sPrnInfo.Set_Layer_Name	( e_cpCheck_Tgt_Layer );

	// 追加データの出力
	ims_data_info	a_iTgt_Indx;
	for( a_iTgt_Indx = c_ipChk_Tgt_Data.begin(); a_iTgt_Indx != c_ipChk_Tgt_Data.end(); a_iTgt_Indx++ )
	{
		data_info	a_dCur_Dat	= *a_iTgt_Indx;
		if( a_dCur_Dat.e_iMeshCode == 0) {
			a_dCur_Dat.e_iMeshCode	= e_iMeshCode;
		}
		fprintf	( a_fpOutFp, "%d\t%d\t%d\t追加データ\n", a_dCur_Dat.e_iMeshCode, 0, a_dCur_Dat.e_iOID ); 
	
		e_sPrnInfo.Set_OID	( a_dCur_Dat.e_iOID );
	}

	// 表示後にテーブルをクリア
	e_msMatch_Tbl.clear();
}

/** ------------------------------------------------------------------------------
	ソースとターゲットのチェックを行う
	※ソースとターゲットを入れ替えてチェックをできるようにしたい（逆検査のため）
--------------------------------------------------------------------------------*/
void	main_rootin::
Chk_Src_Tgt_Data ( 		ms_data_info&		c_ipChk_Src_Data,		///< チェック元データのセット[IN]
						ms_data_info&		c_ipChk_Tgt_Data,		///< チェック元データのセット[IN]
						msi_handle_ws&		c_mChk_Src_WS,			///< ターゲットのワークスペースハンドラ[IN]
						msi_handle_ws&		c_mChk_Tgt_WS,			///< ターゲットのワークスペースハンドラ[IN]
						char*				c_cpCheck_Src_Layer ) 	///< チェックレイヤ名
{
	char	a_cpMsg[512];	// メッセージ用

	e_sPrnInfo.Set_Layer_Name( c_cpCheck_Src_Layer );
	e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_ERROR );

	// 対応がとれたデータのイテレータのテーブル
	m_ims_data_info	a_mMatch_Ite_Tbl_Src, a_mMatch_Ite_Tbl_Tgt;
	ims_data_info	a_iIndx;

	int	a_iCount	= 0;
	// 
	for( a_iIndx = c_ipChk_Src_Data.begin(); a_iIndx != c_ipChk_Src_Data.end(); a_iIndx++, a_iCount++ )
	{
		if( a_iCount % 500 == 0 ) {
			PrnMsg(stdout, NULL, "count = %d/%d\r", a_iCount, c_ipChk_Src_Data.size() );
		}
		data_info			a_dCurDat	= *a_iIndx;

		e_sPrnInfo.Set_OID	( a_dCurDat.e_iOID );

		// メッシュ境界に接するオブジェクトについては隣接部も取得し、マージした形状を作成する
		data_info			a_dCurDat2;	// 比較用オブジェクト
		bool				a_bMrgFlag	= false;
		a_bMrgFlag	= Get_Neibour_Obj	( a_dCurDat, false, c_mChk_Src_WS, c_cpCheck_Src_Layer, a_dCurDat2 );

		// OBJECTIDでサーチ
		ims_data_info		a_iFind_Indx;
		int					a_iID_Search_Count	= 0;	// 同じメッシュコードとPrimIDのものの数
		a_iFind_Indx		= c_ipChk_Tgt_Data.find( a_dCurDat2 );
		a_iID_Search_Count	= c_ipChk_Tgt_Data.count( a_dCurDat2 );

		// データが見つかった場合は形状の一致を見る
		if( a_iFind_Indx != c_ipChk_Tgt_Data.end() ) {
			int		a_iSC	= 0;
			ims_data_info	a_iFind_Cur_Indx	= a_iFind_Indx;
	
			for( a_iSC = 0; a_iSC < a_iID_Search_Count; a_iFind_Cur_Indx++, a_iSC++ )
			{
				data_info			a_lSearched	= *a_iFind_Cur_Indx;

				// 形状チェックを行う
				if( Get_Match_Info ( a_dCurDat,	a_lSearched, c_ipChk_Tgt_Data ) != 0 ) {
					Entry_to_Match_Tbl ( a_dCurDat, a_lSearched );	// 対応が取れたデータをコンテナに登録
					a_mMatch_Ite_Tbl_Src.insert	( m_ims_data_info::value_type( (long)(&(*a_iIndx)), a_iIndx ) ); // 検索元データのインデックスをコンテナに登録
					a_mMatch_Ite_Tbl_Tgt.insert	( m_ims_data_info::value_type( (long)(&(*a_iFind_Cur_Indx)), a_iFind_Cur_Indx ) ); // 検索先データのインデックスをコンテナに登録
				}
			}
		}else {	// OBJECTIDが一致するものが見つからない場合
		
			IFeatureCursorPtr	a_ipChk_Tgt_Datas;
			// IDが一致するものがない場合は空間検索でサーチする
			a_ipChk_Tgt_Datas	= c_mChk_Tgt_WS.Search_Feature ( a_dCurDat2.e_ipGeo, esriSpatialRelIntersects, "", RET_FCURSOR);
			if (a_ipChk_Tgt_Datas != NULL ) {
				// 検索で引っかかったデータをセットに落とす
				ms_data_info		a_mIntersects_Datas;
				ims_data_info		a_mIs_Indx;
				int					a_iIntersects_Count	= 0;
				a_iIntersects_Count	= Set_Data_Infos ( a_ipChk_Tgt_Datas, a_mIntersects_Datas );
				
				for( a_mIs_Indx = a_mIntersects_Datas.begin(); a_mIs_Indx != a_mIntersects_Datas.end(); a_mIs_Indx++ )
				{
					data_info	a_dSearch_Tgt	= *a_mIs_Indx;
					// 空間検索で引っかかったデータと一致を見る
					// 形状チェックを行う
					if( Get_Match_Info ( a_dCurDat,	a_dSearch_Tgt, c_ipChk_Tgt_Data ) != 0 ) {
						Entry_to_Match_Tbl ( a_dCurDat, a_dSearch_Tgt );	// 対応が取れたデータをコンテナに登録
						a_mMatch_Ite_Tbl_Src.insert	( m_ims_data_info::value_type( (long)(&(*a_iIndx)), a_iIndx ) ); // 検索元データのインデックスをコンテナに登録
						//--------------------------------------------------------------------------
						ims_data_info		a_iMatch_Indx;
						int	a_iMatch_Count	= c_ipChk_Tgt_Data.count ( a_dSearch_Tgt );
						a_iMatch_Indx	= c_ipChk_Tgt_Data.find( a_dSearch_Tgt  );
						if( a_iMatch_Indx != c_ipChk_Tgt_Data.end() ) {
							ims_data_info		a_iSame_Indx;
							int					a_iSame_Count	= 0;
							// 同じIDを持つものがあるので、オブジェクトIDが一致するものを探し、そのインデックスを削除するようにする
							for( a_iSame_Indx = a_iMatch_Indx; a_iSame_Count < a_iMatch_Count; a_iSame_Indx++, a_iSame_Count++)
							{
								data_info	a_iCurSame	= *a_iSame_Indx;
								if( a_iCurSame.e_iOID == a_dSearch_Tgt.e_iOID ) {
									// データテーブルに該当データがあったということなので、削除リストに登録する
									a_mMatch_Ite_Tbl_Tgt.insert ( m_ims_data_info::value_type( (long)(&(*a_iSame_Indx)), a_iSame_Indx ) );
								}
							}
						}else {
							// 見つからないならメッシュで引っかかっていないものなので、削除リストには登録しない
#ifdef _DEBUG
							e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_NOTICE );
							sprintf	( a_cpMsg,	"Tgtの削除テーブルにないデータと一致\t%d\t%d", a_dCurDat.e_iOID, a_dSearch_Tgt.e_iOID );
							e_sPrnInfo.Prn_SiNDY_Err ( 0, a_cpMsg, NULL );
							e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_ERROR );
#endif
						}
						//--------------------------------------------------------------------------
					}
				}
			}
		}
	}
	PrnMsg(stdout, NULL, "\n");
	// 対応がとれたデータをソースとターゲットのテーブルから消しておく
	im_ims_data_info	a_imMatch_Ite_Src, a_imMatch_Ite_Tgt;
	for( a_imMatch_Ite_Src = a_mMatch_Ite_Tbl_Src.begin(); a_imMatch_Ite_Src != a_mMatch_Ite_Tbl_Src.end(); a_imMatch_Ite_Src++ )
	{
		c_ipChk_Src_Data.erase ( (*a_imMatch_Ite_Src).second );
	}
	for( a_imMatch_Ite_Tgt = a_mMatch_Ite_Tbl_Tgt.begin(); a_imMatch_Ite_Tgt != a_mMatch_Ite_Tbl_Tgt.end(); a_imMatch_Ite_Tgt++ )
	{
		c_ipChk_Tgt_Data.erase ( (*a_imMatch_Ite_Tgt).second );
	}
	PrnMsg(stdout, NULL, "src = 残り%d件 tgt = 残り%d\n", c_ipChk_Src_Data.size(), c_ipChk_Tgt_Data.size() );
}

/**
	削除・追加と判定されたものに対して精度を若干落とした形で再チェックを行う
*/
void	main_rootin::
Chk_Retry ( 			ms_data_info&		c_ipChk_Src_Data,		///< チェック元データのセット[IN]
						ms_data_info&		c_ipChk_Tgt_Data,		///< チェック元データのセット[IN]
						msi_handle_ws&		c_mChk_Src_WS,			///< ターゲットのワークスペースハンドラ[IN]
						msi_handle_ws&		c_mChk_Tgt_WS,			///< ターゲットのワークスペースハンドラ[IN]
						char*				c_cpCheck_Src_Layer ) 	///< チェックレイヤ名
{
	char	a_cpMsg[512];	// メッセージ用

	const int	SEIDO	= 10000000;
//	const int	SEIDO	= 1000000;

	e_sPrnInfo.Set_Layer_Name	( c_cpCheck_Src_Layer );
	e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_ERROR );

	// 対応がとれたデータのイテレータのテーブル
	m_ims_data_info	a_mMatch_Ite_Tbl_Src, a_mMatch_Ite_Tbl_Tgt;
	ims_data_info	a_iIndx;

	int	a_iCount	= 0;
	// 
	for( a_iIndx = c_ipChk_Src_Data.begin(); a_iIndx != c_ipChk_Src_Data.end(); a_iIndx++, a_iCount++ )
	{
		if( a_iCount % 500 == 0 ) {
			PrnMsg(stdout, NULL, "count = %d/%d\n", a_iCount, c_ipChk_Src_Data.size() );
		}
		data_info			a_dCurDat	= *a_iIndx;

		e_sPrnInfo.Set_OID	( a_dCurDat.e_iOID );
	
		// メッシュ境界に接するオブジェクトについては隣接部も取得し、マージした形状を作成する
		data_info			a_dCurDat2;	// 比較用オブジェクト
		bool				a_bMrgFlag	= false;
		a_bMrgFlag	= Get_Neibour_Obj	( a_dCurDat, false, c_mChk_Src_WS, c_cpCheck_Src_Layer, a_dCurDat2 );

		IFeatureCursorPtr	a_ipChk_Tgt_Datas;

		// 空間検索でサーチする
		a_ipChk_Tgt_Datas	= c_mChk_Tgt_WS.Search_Feature ( a_dCurDat2.e_ipGeo, esriSpatialRelIntersects, "", RET_FCURSOR);
		if (a_ipChk_Tgt_Datas != NULL ) {
			// 検索で引っかかったデータをセットに落とす
			ms_data_info		a_mIntersects_Datas;
			ims_data_info		a_mIs_Indx;
			int					a_iIntersects_Count	= 0;
			a_iIntersects_Count	= Set_Data_Infos ( a_ipChk_Tgt_Datas, a_mIntersects_Datas );
	
			int	a_iEqual_F	= 0;
			// 引っかかったデータのループ
			for( a_mIs_Indx = a_mIntersects_Datas.begin(); a_mIs_Indx != a_mIntersects_Datas.end(); a_mIs_Indx++ )
			{
				data_info	a_dSearch_Tgt	= *a_mIs_Indx;
				data_info	a_dPutDat		= a_dCurDat;	///　登録用データ 2005.12.21

				// 空間検索で引っかかったデータと一致を見る
				int	a_iChk_Result	= 0;
				a_iChk_Result	= Shape_Chk_LowAccuracy ( a_dCurDat2.e_ipGeo, a_dSearch_Tgt.e_ipGeo );	// 2006.09.27 新規ロジック
				if( a_iChk_Result == STAT_LOWA_EQUAL ||	
					a_iChk_Result == STAT_LOWA_MODIFY ) {

					if( a_iChk_Result == STAT_LOWA_EQUAL ) {
						a_dPutDat.e_iStat	|= STAT_LOWA_EQUAL;
					}else {
						a_dPutDat.e_iStat	|= STAT_LOWA_MODIFY;
					}

					// ターゲットのデータから同じものを見つける！
					//----------------------------------------------------------
					ims_data_info	a_iTgtIndx;
					int	a_iFindTgt	= 0;
					for(a_iTgtIndx = c_ipChk_Tgt_Data.begin(); a_iTgtIndx != c_ipChk_Tgt_Data.end(); a_iTgtIndx++)
					{
						data_info	a_iCurDat	= *a_iTgtIndx;
						if( a_dSearch_Tgt.e_iOID == a_iCurDat.e_iOID ) {
							a_iFindTgt	= 1;
							break;
						}
					}
					// ターゲットの残りにある場合以外は追加しない（一致したものとかぶる場合があるため）
					if( a_iFindTgt == 1 ) {
						// マッチしたデータはテーブルに登録する
						Entry_to_Match_Tbl ( a_dPutDat, a_dSearch_Tgt );
						a_mMatch_Ite_Tbl_Src.insert	( m_ims_data_info::value_type( (long)(&(*a_iIndx)), a_iIndx ) );
						// テーブルに登録
						a_mMatch_Ite_Tbl_Tgt.insert	( m_ims_data_info::value_type( (long)(&(*a_iTgtIndx)), a_iTgtIndx ) );
						// 見つかった場合はループを抜ける
						break;
					}else {
#ifdef _DEBUG
						e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_NOTICE );
						sprintf	( a_cpMsg,	"Tgtの削除テーブルにないデータと一致\t%d", a_dSearch_Tgt.e_iOID );
						e_sPrnInfo.Prn_SiNDY_Err ( 0, a_cpMsg, NULL );
						e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_ERROR );
#endif
					}
					//----------------------------------------------------------

				}else {
					continue;
				}
			}
		}
	}

	// 対応がとれたデータをソースとターゲットのテーブルから消しておく
	im_ims_data_info	a_imMatch_Ite_Src, a_imMatch_Ite_Tgt;
	for( a_imMatch_Ite_Src = a_mMatch_Ite_Tbl_Src.begin(); a_imMatch_Ite_Src != a_mMatch_Ite_Tbl_Src.end(); a_imMatch_Ite_Src++ )
	{
		c_ipChk_Src_Data.erase ( (*a_imMatch_Ite_Src).second );
	}
	PrnMsg(stdout, NULL, "再チェック:残り%d件\n", c_ipChk_Src_Data.size() );
	for( a_imMatch_Ite_Tgt = a_mMatch_Ite_Tbl_Tgt.begin(); a_imMatch_Ite_Tgt != a_mMatch_Ite_Tbl_Tgt.end(); a_imMatch_Ite_Tgt++ )
	{
		c_ipChk_Tgt_Data.erase ( (*a_imMatch_Ite_Tgt).second );
	}
	PrnMsg(stdout, NULL, "再チェック:残り%d件\n", c_ipChk_Tgt_Data.size() );
}

/** ------------------------------------------------------------------------------
	差分チェックのループ
--------------------------------------------------------------------------------*/
void	main_rootin::
Diff_Check_Roops ( IFeatureCursorPtr&	c_ipChk_Src_Cursor,	 	///< チェック元となるデータのカーソル[IN]
				   IFeatureCursorPtr&	c_ipChk_Tgt_Cursor ) 	///< チェック先となるデータのカーソル[IN]
{
	// 行政界のループ
	int	i=0;
	IFeaturePtr		a_ipCity_Admin_Feat;
	int				a_iCityName_Count	= 0;
	int				a_iCityOffice_Count	= 0;

	ms_data_info	a_eChk_Src_Data, a_eChk_Tgt_Data;

	// チェック元と先のデータをセットにする
	Set_Data_Infos	( c_ipChk_Src_Cursor, a_eChk_Src_Data );
	Set_Data_Infos	( c_ipChk_Tgt_Cursor, a_eChk_Tgt_Data );
	// データチェック
	Chk_Src_Tgt_Data ( a_eChk_Src_Data, a_eChk_Tgt_Data, e_mChk_Src_WS, e_mChk_Tgt_WS, e_cpCheck_Src_Layer );
	// 対応テーブルの出力
	Print_Table		( a_eChk_Src_Data, a_eChk_Tgt_Data );
}


/** ------------------------------------------------------------------------------
	チェック実行部
	※内部でエラーファイルの処理等も行う
	@return	成功	true
	@return 失敗	false
--------------------------------------------------------------------------------*/
bool	main_rootin::
check_exe	( void )
{
	char	a_cpErrMsg[512];

	// 開始時間の設定
	e_eExeTime.Set_Start();

	// 接続
	IWorkspacePtr			ipWorkspace_Src;
	IWorkspacePtr			ipWorkspace_Tgt;

	if( !SiNDY_DB_Open	( ipWorkspace_Src, ipWorkspace_Tgt )) {
		PrnMsg(e_fErr_Fp, stderr, "接続出来なかった\n" );
		return	( false );
	}

	// DB接続にかかった時間の表示
	PrnMsg(stdout, NULL, "SiNDY-DB接続処理時間 = %0.2f(s)でした\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );


	// レイヤオープン
	if( !Open_Layers ( ipWorkspace_Src, ipWorkspace_Tgt ) ){	// 地図用ワークスペース
		PrnMsg(e_fErr_Fp, stderr, "レイヤオープンに失敗しています\n");
		return	( false );
	}

	// メッシュリストの読み込み
	mk_input_list		a_mMesh_List;
	if( !Read_Chk_List( ipWorkspace_Src, a_mMesh_List ) ) {
		PrnMsg(e_fErr_Fp, stderr, "メッシュリストが取得できませんでした\n" );
		return	( false );	
	}

	// メッシュのループ
	int					a_iCur_Mesh;
	int	a_iRoopCount	= 0;
	bool isErrorStat = false;
	for( a_iCur_Mesh = a_mMesh_List.get_mesh_first(); a_iCur_Mesh != -1; a_iCur_Mesh = a_mMesh_List.get_mesh_next(), a_iRoopCount++ )
	{
		PrnMsg(stdout, NULL, "#メッシュコード [%d] 処理中...", a_iCur_Mesh );
	
		e_eExeTime.Set_Lap_Start();

		IFeatureCursorPtr	a_ipChk_Src_Datas, a_ipChk_Tgt_Datas;
	
		// メッシュ矩形の取り出し
		IGeometryPtr	a_ipMeshRect;
		a_ipMeshRect	= get_mesh_rect( a_iCur_Mesh );
		if( a_ipMeshRect == NULL ) {	// メッシュ矩形が無い場合
			PrnMsg(e_fErr_Fp, stderr, "メッシュコード<%d>の矩形が見つかりませんでした\n", a_iCur_Mesh );
			isErrorStat = true;
			continue;
		}
		// クラス内で使用できるようにしておく
		e_ipMeshRect	= a_ipMeshRect;
		e_iMeshCode		= a_iCur_Mesh;

		// メッシュの境界線を取り出す
		ITopologicalOperatorPtr	a_ipMeshRectTopo;
		a_ipMeshRectTopo		= Get_Rect_Polygon( IEnvelopePtr(a_ipMeshRect) );
		if( FAILED (a_ipMeshRectTopo->get_Boundary ( &e_ipMeshRect_Line ) )) {
			PrnMsg(e_fErr_Fp, stderr, "メッシュコード<%d>の矩形の境界線が取得できない\n", a_iCur_Mesh );
			isErrorStat = true;
			continue;
		}

		//-----------------------------------------------------------		
		IEnvelopePtr	a_ipRect, a_ipLargeRect;
		a_ipMeshRect->get_Envelope ( &a_ipRect );

		// メッシュ矩形でサーチする
		a_ipChk_Src_Datas	= e_mChk_Src_WS.Search_Feature ( a_ipMeshRect, esriSpatialRelIntersects, "", RET_FCURSOR|RET_COUNT );
		if (a_ipChk_Src_Datas == NULL ) {
			PrnMsg(e_fErr_Fp, stderr, "<%d>メッシュ矩形でデータが取れません(比較元)\n", a_iCur_Mesh );
			isErrorStat = true;
			continue;
		}
		a_ipChk_Tgt_Datas	= e_mChk_Tgt_WS.Search_Feature ( a_ipMeshRect, esriSpatialRelIntersects, "", RET_FCURSOR|RET_COUNT );
		if ( a_ipChk_Tgt_Datas == NULL ) {
			PrnMsg(e_fErr_Fp, stderr, "<%d>メッシュ矩形でデータが取れません(比較先)\n", a_iCur_Mesh );
			isErrorStat = true;
			continue;
		}

		//-----------<チェック進行度表示>--------------------
		long		a_lTCount	= 0;
		a_lTCount	= e_mChk_Src_WS.Get_Data_Count ();
		e_mChk_Src_WS.Clear_Data_Count();

		PrnMsg(stdout, NULL, "総データ件数 src=%d 件, tgt=%d 件\n", a_lTCount, e_mChk_Tgt_WS.Get_Data_Count() );

		Diff_Check_Roops ( a_ipChk_Src_Datas, a_ipChk_Tgt_Datas );

		DWORD			a_dMake_Finish	= 0;
		a_dMake_Finish	= timeGetTime();

		PrnMsg(stdout, NULL, "#メッシュコード [%d] 処理時間 = %0.2f(s)でした\n", a_iCur_Mesh, e_eExeTime.Get_Lap() / 1000.0 );
	}

	//　実行部終了
	// 終了時間の取得
	PrnMsg(stdout, NULL, "データ処理総時間 = %0.2f(s)でした\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );
	if( isErrorStat == true ) {
		return false;
	}else {
		return true;
	}
}

/**
	チェック用行政界リスト、メッシュコードリストの読み込み
*/
bool	main_rootin::
Read_Chk_List	(	IWorkspacePtr&	c_ipWSpace,			///< ワークスペース
					mk_input_list&	c_aChk_List_Cls	)	///< リスト管理クラスの参照
{

	if( eExe_Flag & ON_MESH ) {		//	メッシュコードリスト指定の場合
		if( e_cpMeshCode_File == NULL ) {
			PrnMsg(e_fErr_Fp, stderr, "メッシュコードリストファイル名がとりだせませんでした\n");
			return	( false );
		}	
		if( c_aChk_List_Cls.read_mesh_list ( e_cpMeshCode_File ) != 0 ) {	// ここでリストを読み込む
			PrnMsg(e_fErr_Fp, stderr, "メッシュコードリストファイル<%s>の読み込みに失敗[check_exe]\n", e_cpMeshCode_File );
			return	( false );
		}
	}else
	if( eExe_Flag & ON_MONO_MESH ) {	// 単メッシュ指定の場合
		if( c_aChk_List_Cls.read_mesh_list ( e_iMeshCode ) != 0 ) {	// ここでリストを読み込む
			PrnMsg(e_fErr_Fp, stderr, "メッシュコードリストファイル<%s>の読み込みに失敗[check_exe]\n", e_cpMeshCode_File );
			return	( false );
		}
	}else {
		return ( false );
	}

	if( !get_mesh_init( c_aChk_List_Cls , c_ipWSpace ) ) {
		PrnMsg(e_fErr_Fp, stderr, "メッシュレイヤの獲得の初期化に失敗[check_exe]\n");
		return	( false );
	}
		
	switch	( eDigit )
	{
	case	8:
	case	6:
	case	4:
		break;
	default:
		PrnMsg(e_fErr_Fp, stderr, "リストのメッシュ桁数には対応していません[check_exe]\n");
		return	( false );
	}

	return	( true );
}

IEnvelopePtr	main_rootin::
Get_Expand_Rect	(	IEnvelopePtr&	c_iRect,	///< 入力矩形[IN]
					int				c_iMode,	///< 作成モード（0=元矩形と左下原点を一緒に作成、1=元矩形を中心に作成)[IN]
					double			c_dX,		///< X方向の倍率[IN]
					double			c_dY	)	///< Y方向の倍率[IN]
{
	IEnvelopePtr	a_ipExpand_Rect ( CLSID_Envelope );
	double			a_dMin_Lon	= 0.0,	a_dMin_Lat	= 0.0;
	double			a_dMax_Lon	= 0.0,	a_dMax_Lat	= 0.0;

	HRESULT			hr;

	hr	= c_iRect->get_XMin( &a_dMin_Lon );
	if( FAILED ( hr ) ) {
		return	( NULL );
	}
	hr	= c_iRect->get_YMin( &a_dMin_Lat );
	if( FAILED ( hr ) ) {
		return	( NULL );
	}
	hr	= c_iRect->get_XMax( &a_dMax_Lon );
	if( FAILED ( hr ) ) {
		return	( NULL );
	}
	hr	= c_iRect->get_YMax( &a_dMax_Lat );
	if( FAILED ( hr ) ) {
		return	( NULL );
	}

	double	a_dLon_Wid	= a_dMax_Lon - a_dMin_Lon;
	double	a_dLat_Wid	= a_dMax_Lat - a_dMin_Lat;

	if( c_iMode == 0 ) {
		// 左下は変わらない
		a_dMax_Lon	= a_dMin_Lon + (a_dLon_Wid * c_dX);
		a_dMax_Lat	= a_dMin_Lat + (a_dLat_Wid * c_dY);
	}else {
		a_dMin_Lon	-=	(a_dLon_Wid * c_dX / 2.0);
		a_dMax_Lon	+=	(a_dLon_Wid * c_dX / 2.0);
		a_dMin_Lat	-=	(a_dLat_Wid * c_dY / 2.0);
		a_dMax_Lat	+=	(a_dLat_Wid * c_dY / 2.0);
	}

	hr	= a_ipExpand_Rect-> PutCoords ( a_dMin_Lon, a_dMin_Lat, a_dMax_Lon, a_dMax_Lat );
	if( FAILED ( hr ) ) {
		return	( NULL );
	}

	// 空間参照を合わせる
	ISpatialReferencePtr	a_ipSpRef;
	hr	=c_iRect->get_SpatialReference ( &a_ipSpRef );
	if( FAILED ( hr ) ) {
		return ( NULL );
	}
	hr	= a_ipExpand_Rect->putref_SpatialReference	( a_ipSpRef );
	if( FAILED ( hr ) ) {
		return ( NULL );
	}
	return ( a_ipExpand_Rect );
}

bool main_rootin::
fnIsNeedMergePolygon2(	IGeometryPtr& ipSrcGeom,
						IGeometryPtr& ipTgtGeom)
{
	ITopologicalOperatorPtr  ipSrcLine(ipSrcGeom);
	ITopologicalOperatorPtr  ipTgtLine2(ipTgtGeom);
	IGeometryPtr ipTgtLine;

	IPointCollectionPtr ipPointCollectionS(ipSrcGeom);
	ATLASSERT( ipPointCollectionS != NULL );
	IPointCollectionPtr ipPointCollectionT(ipTgtGeom);
	ATLASSERT( ipPointCollectionT != NULL );
	LONG lPointCountS = 0;
	LONG lPointCountT = 0;
	IPointPtr ipPoint = NULL;
	ipPointCollectionS->get_PointCount( &lPointCountS );
	ipPointCollectionT->get_PointCount( &lPointCountT );

	if(lPointCountS > 3 && lPointCountT > 3){
		WKSPoint *aSrcPoints = new WKSPoint[lPointCountS];
		ipPointCollectionS->QueryWKSPoints(0, lPointCountS, aSrcPoints);
		WKSPoint *aTgtPoints = new WKSPoint[lPointCountT];
		ipPointCollectionT->QueryWKSPoints(0, lPointCountT, aTgtPoints);
		for(LONG i = 0; i < lPointCountS -1; i++){
			if(aSrcPoints[i].X == m_CompMinX && aSrcPoints[i+1].X == m_CompMinX){
				for(LONG j = 0; j < lPointCountT - 1; j++){
					if(aTgtPoints[j].X == m_CompMinX  && aTgtPoints[j+1].X == m_CompMinX){
						if(aSrcPoints[i].Y == aTgtPoints[j+1].Y && aSrcPoints[i+1].Y == aTgtPoints[j].Y){
							delete []aSrcPoints;
							delete []aTgtPoints;
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].X == m_CompMaxX && aSrcPoints[i+1].X == m_CompMaxX){
				for(LONG j = 0; j < lPointCountT - 1; j++){
					if(aTgtPoints[j].X == m_CompMaxX  && aTgtPoints[j+1].X == m_CompMaxX){
						if(aSrcPoints[i].Y == aTgtPoints[j+1].Y && aSrcPoints[i+1].Y == aTgtPoints[j].Y){
							delete []aSrcPoints;
							delete []aTgtPoints;
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].Y == m_CompMinY && aSrcPoints[i+1].Y == m_CompMinY){
				for(LONG j = 0; j < lPointCountT - 1; j++){
					if(aTgtPoints[j].Y == m_CompMinY  && aTgtPoints[j+1].Y == m_CompMinY){
						if(aSrcPoints[i].X == aTgtPoints[j+1].X && aSrcPoints[i+1].X == aTgtPoints[j].X){
							delete []aSrcPoints;
							delete []aTgtPoints;
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].Y == m_CompMaxY && aSrcPoints[i+1].Y == m_CompMaxY){
				for(LONG j = 0; j < lPointCountT - 1; j++){
					if(aTgtPoints[j].Y == m_CompMaxY  && aTgtPoints[j+1].Y == m_CompMaxY){
						if(aSrcPoints[i].X == aTgtPoints[j+1].X && aSrcPoints[i+1].X == aTgtPoints[j].X){
							delete []aSrcPoints;
							delete []aTgtPoints;
							return 1;
						}
					}
				}
			}
		}
		delete []aSrcPoints;
		delete []aTgtPoints;
	}
	return 0;
}
