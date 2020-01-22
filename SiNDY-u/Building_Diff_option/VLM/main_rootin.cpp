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
#include <sindy/workspace.h>

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
SiNDY_DB_Open	( IWorkspacePtr& c_ipWorkspace_Src )
{
	c_ipWorkspace_Src = sindy::create_workspace(m_sConnectStr);
	if(!c_ipWorkspace_Src ){
		return false;
	}
	return	true;
}

/** ------------------------------------------------------------------------------
	エラーログファイルをオープンする
	@return	true	オープン成功
	@return false	オープン失敗
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Err_Log	( void )
{
	if( e_cpLog_File.size() == 0 ) {
		return	( true );
	}else {
		e_fErr_Fp	= fopen ( (char*)e_cpLog_File.c_str(), "w" );
		if( e_fErr_Fp == NULL ) {
			PrnMsg(stderr, NULL, "エラーログファイル<%s>がオープンできない\n", e_cpLog_File.c_str() );
			return	( false );
		}
	}
	return	( true );
}

/** ------------------------------------------------------------------------------
	属性テーブルファイルをオープンする
	@return	true	オープン成功
	@return false	オープン失敗
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Attr_Tbl	( void )
{
	if( e_cpAttr_Tbl_File.size() == 0 ) {
		return	( true );
	}else {
		e_fpAttr_Fp	= fopen ( (char*)e_cpAttr_Tbl_File.c_str(), "w" );
		if( e_fpAttr_Fp == NULL ) {
			PrnMsg(e_fErr_Fp, stderr, "出力対応テーブルファイル<%s>がオープンできない\n", e_cpAttr_Tbl_File.c_str() );
			return	( false );
		}
	}
	return	( true );
}


#define	OPTION_MAX	6
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

	for(i = 0; i < OPTION_MAX; i++)
	{
		a_optcount[i] = 0;
	}

	// 引数文字列数のチェック
	for(i = 0; i < argc; i++)
	{
		if(strlen(argv[i]) > BMAX) {
			PrnMsg(stderr, NULL, "引数文字列が長すぎます!!\n" );
			return	( false );
		}
		// バージョン情報表示チェック
		if(strcmp(argv[i], "-v") == 0) {
			print_version();
			return	( false );
		}
	}


	if(argc < 3) {
		return( false );
	}

	e_cpInit_File		= argv[1];	///< 初期化ファイル(住所DB接続)も含む
	e_cpCheck_Src_Layer	= argv[2];	///< チェック元レイヤ

	// オプションだから、今のところ使用しない
	for(i = 3; i < argc;)
	{
		if(strcmp(argv[i], "-l") == 0) { // ログファイル設定（設定が無い場合は、エラーはstderrへ垂れ流しになる）
			if(argc < i+2) return( false );
			e_cpLog_File	= argv[i+1];	///< メッシュコードリストファイル
			a_optcount[0]++;
			i+=2;
		} else if(strcmp(argv[i], "-m") == 0) {
			if(argc < i+2) return( false );
			e_cpMeshCode_File	= argv[i+1];	///< メッシュコードリストファイル
			eExe_Flag			|= ON_MESH;
			a_optcount[1]++;
			i+=2;
		} else if(strcmp(argv[i], "-M") == 0) {
			if(argc < i+2) return( false );
			eExe_Flag			|= ON_MONO_MESH;
			e_iMeshCode			= atoi(argv[i+1]);
			a_optcount[2]++;
			i+=2;
		} else if(strcmp(argv[i], "-t") == 0) {
			if(argc < i+2) return( false );
			e_cpAttr_Tbl_File	= argv[i+1];		///< 対応リストファイル
			eExe_Flag			|= ON_ATTR_FILE;
			a_optcount[4]++;
			i+=2;
		} else if(strcmp(argv[i], "-v") == 0) {
			print_version();
			a_optcount[5]++;
			i+=1;
			return	( false );	// 仮にこうする？
		} else {
			PrnMsg(stderr, NULL, "オプション %s は間違ってます!!\n", argv[i]);
			return	( false );
		}
	}

	// オプション重複のチェック
	for(i = 0; i < OPTION_MAX; i++)
	{
		if(a_optcount[i] > 1) {
			PrnMsg(stderr, NULL, "オプションの指定が重複しています!!\n");
			return	( false );
		}
	}

	// 初期化ファイル設定
	if( e_pTool_Init.read_init_file ( (char*)e_cpInit_File.c_str() ) == -1 ) {
		return	( false );	// メッセージは内部で出している
	}else {
		m_sConnectStr.Format(_T("%s/%s/%s/%s/%s"), e_pTool_Init.get_UserName(), e_pTool_Init.get_Password(),
			  e_pTool_Init.get_Version(), e_pTool_Init.get_Instance(), e_pTool_Init.get_Server() );
	}

	// 属性テーブルファイルオープン
	if( eExe_Flag & ON_ATTR_FILE ) {
		if( !Open_Attr_Tbl () ) {
			return false;
		}
	}else {
		PrnMsg(stderr, NULL, "出力属性テーブルファイルを\'-t\'で指定してください\n");
		return false;
	}

	return		( true );

}

/** ------------------------------------------------------------------------------
// 使用法表示
--------------------------------------------------------------------------------*/
void		main_rootin::
print_usage	(char **argv)
{
	PrnMsg(stdout, NULL,
				"=== 【建物差分抽出ツール】 ===\n"
				"<usage>\n"
				"%s <arg1> <arg2> <arg3> <arg4>\n"
				"\t<引数>\n"
				"\t      <arg1>         : チェック元用初期化ファイル\n"
				"\t      <arg2>         : チェック元レイヤ名	(ex. TECHMAP.Building_mc)\n"
				"\t      <arg3>         : チェック元用初期化ファイル\n"
				"\t      <arg4>         : チェック先レイヤ名	(ex. SiNDY2004.Building)\n"
				"\t<オプション>\n"
				"\t      '-l'           : ログファイル名を指定\n"
				"\t      '-t'           : 出力属性リストファイルを指定\n"
				"\t      '-m'           : メッシュコードリストファイルを指定\n"
				"\t      '-M'           : メッシュコードを指定（1次、2次、8桁メッシュ）\n"
				"\t      '-v'           : バージョン情報表示\n"
				"\t<補足>\n"
				"\t      ・'-m','-M',はどちらか必ず指定して下さい\n"
				"\t      ・'-m','-M'は同時指定できません\n"
			, argv[0]);
}

/** ------------------------------------------------------------------------------
	バージョン情報の表示
	@return		無し
---------------------------------------------------------------------------------*/
void	main_rootin::
print_version	( void )
{
	PrnMsg(stdout, NULL,
				"=== 【属性対応リスト作成ツール】 ===\n"
				"\t<バージョン情報>\n"
				"\t2004/05/14        1.00               : 公開バージョン\n"
				"\t2014/02/04        2.1.0.3            : [bug 10159] [u][要望]14秋仕様に対応して欲しい(Building_Diff_option) への対応\n"
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
		if( e_cpMeshCode_File.size() == 0 ) {
			PrnMsg(e_fErr_Fp, stderr, "メッシュコードリストファイル名がとりだせませんでした[check_exe]\n");
			return	( false );
		}	
		if( c_mMList.read_mesh_list ( (char*)e_cpMeshCode_File.c_str() ) != 0 ) {	// ここでリストを読み込む
			PrnMsg(e_fErr_Fp, stderr, "メッシュコードリストファイル<%s>の読み込みに失敗[check_exe]\n", e_cpMeshCode_File.c_str() );
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
		PrnMsg(stdout, NULL, "<%s> レイヤをオープン\n", a_cpMeshLayer);
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
				continue;
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
	レイヤオープン
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Layers		( 	IWorkspacePtr&	ipWorkspace_Src	)	// チェック元ワークスペース[IN]
{

	if( !e_mChk_Src_WS.Init ( ipWorkspace_Src, (char*)e_cpCheck_Src_Layer.c_str() ) ) {
		PrnMsg(e_fErr_Fp, stderr, "チェック元レイヤ<%s>の読み込みに失敗\n", e_cpCheck_Src_Layer );
		return	( false );
	}
	return	( true );
}

/**
	読み込んだデータをセットに落とす
	@return	データ数
*/
int		main_rootin::
Set_Data_Info (	IFeatureCursorPtr&	c_ipChk_Cursor,	///< チェック元カーソル[IN]
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
			Feature_Attr   a_fObjID, a_fMeshCode, a_fPrim_ID, a_fAttr, a_fCORRIDOR_F, a_fNOWALL_F; // [Bug 10159]対応
			a_fObjID       = a_fChk_Src_Attrs.Get_Attr ( "OBJECTID" );
			a_fMeshCode    = a_fChk_Src_Attrs.Get_Attr ( "TMP_MESHCODE" );
			a_fPrim_ID     = a_fChk_Src_Attrs.Get_Attr ( "TMP_PRIMID" );
			a_fAttr        = a_fChk_Src_Attrs.Get_Attr ( "BLDCLASS_C" );
			a_fCORRIDOR_F  = a_fChk_Src_Attrs.Get_Attr ( "CORRIDOR_F" );
			a_fNOWALL_F    = a_fChk_Src_Attrs.Get_Attr ( "NOWALL_F" );

			// [Bug 10159]対応
			// 除外種別の判定
			int ret = mAKAttrHandler.fnIsExcluded ( a_fAttr.e_ccvAttr.lVal, a_fNOWALL_F.e_ccvAttr.lVal, a_fCORRIDOR_F.e_ccvAttr.lVal );
			if( ret != 0 ) {
				continue;
			}
			// 除外でない場合は、属性値の変換
			mAKAttrHandler.fnAttrConv ( a_fAttr.e_ccvAttr.lVal );

			a_dChk_Data.e_iOID		= a_fObjID.e_ccvAttr.intVal;
			a_dChk_Data.e_iMeshCode	= e_iMeshCode;			
			a_dChk_Data.e_iPrim_ID	= a_fPrim_ID.e_ccvAttr.intVal;
			a_dChk_Data.e_iAttr		= a_fAttr.e_ccvAttr.intVal;
			a_dChk_Data.e_ipGeo		= a_fChk_Src_Attrs.Ret_Geo();

			c_ipChk_Data.insert ( a_dChk_Data );
			a_iCount++;
		}
	}
	return	( a_iCount );
}

/** ------------------------------------------------------------------------------
	対応テーブルの出力
	@return	無し
--------------------------------------------------------------------------------*/
void	main_rootin::
Print_Table		(	ms_data_info&		c_ipChk_Src_Data )		///< チェック元データのセット[IN]
{
	FILE*	a_fpOutFp	= NULL;

	if( e_fpAttr_Fp == NULL ) {
		a_fpOutFp	= stdout;	// 指定がない場合は標準出力に出力
	}else {
		a_fpOutFp	= e_fpAttr_Fp;
	}

	// データの出力
	ims_data_info	a_iSrc_Indx;
	for( a_iSrc_Indx = c_ipChk_Src_Data.begin(); a_iSrc_Indx != c_ipChk_Src_Data.end(); a_iSrc_Indx++ )
	{
		data_info	a_dCur_Dat	= *a_iSrc_Indx;
		fprintf	( a_fpOutFp, "%d\t%d\t%d\n", a_dCur_Dat.e_iMeshCode, a_dCur_Dat.e_iOID, a_dCur_Dat.e_iAttr );
	}
}

/** ------------------------------------------------------------------------------
	処理ループ
--------------------------------------------------------------------------------*/
void	main_rootin::
Exec_Roops ( IFeatureCursorPtr&	c_ipChk_Src_Cursor ) ///< チェック元となるデータのカーソル[IN]
{
	// 行政界のループ
	int	i=0;
	IFeaturePtr		a_ipCity_Admin_Feat;
	int				a_iCityName_Count	= 0;
	int				a_iCityOffice_Count	= 0;

	ms_data_info	a_eChk_Src_Data;

	// チェック元と先のデータをセットにする
	Set_Data_Info	( c_ipChk_Src_Cursor, a_eChk_Src_Data );
	// 対応テーブルの出力
	Print_Table		( a_eChk_Src_Data );
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

	// 開始時間の設定
	e_eExeTime.Set_Start();

	// エラーログファイルのオープン
	if( !Open_Err_Log () ) {
		return	( false );
	}

	// ログに設定情報を出力
	//--------------------------------------------------------------------------------------
	PrnMsg(e_fErr_Fp, stdout, "#接続プロパティ：%s\n", m_sConnectStr );
	PrnMsg(e_fErr_Fp, stdout, "#接続レイヤ    ：%s\n", e_cpCheck_Src_Layer.c_str() );
	if( eExe_Flag & ON_MESH ) {
		PrnMsg(e_fErr_Fp, stdout, "#メッシュリスト：%s\n", e_cpMeshCode_File.c_str() );
	}else {
		PrnMsg(e_fErr_Fp, stdout, "#メッシュコード：%d\n", e_iMeshCode );
	}
	PrnMsg(e_fErr_Fp, stdout, "#出力ファイル  ：%s\n", e_cpAttr_Tbl_File.c_str() );
	//--------------------------------------------------------------------------------------

	// 接続
	IWorkspacePtr	ipWorkspace_Src;

	if( !SiNDY_DB_Open	( ipWorkspace_Src )) {
		PrnMsg(e_fErr_Fp, stderr, "接続出来なかった\n");
		return	( false );
	}else {
		PrnMsg(stdout, NULL, "#接続完了\n" );
	}

	// DB接続にかかった時間の表示
	PrnMsg(stdout, NULL, "SiNDY-DB接続処理時間 = %0.2f(s)でした\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );

	// レイヤオープン
	if( !Open_Layers ( ipWorkspace_Src ) ){	// 地図用ワークスペース
		PrnMsg(e_fErr_Fp, stderr, "レイヤオープンに失敗しています\n");
		return	( false );
	}

	// メッシュリストの読み込み
	mk_input_list		a_mMesh_List;
	if( !Read_Chk_List( ipWorkspace_Src, a_mMesh_List ) ) {
		PrnMsg(e_fErr_Fp, stderr, "メッシュリストが取得できませんでした\n");
		return	( false );	
	}

	ISpatialCacheManagerPtr		a_ipSpCacheMgr_Src( ipWorkspace_Src );

	// メッシュのループ
	int					a_iCur_Mesh;
	bool bErrorStat = false;
	for( a_iCur_Mesh = a_mMesh_List.get_mesh_first(); a_iCur_Mesh != -1; a_iCur_Mesh = a_mMesh_List.get_mesh_next() )
	{
		PrnMsg(stdout, NULL, "#メッシュコード [%d] 処理中...", a_iCur_Mesh );
	
		e_eExeTime.Set_Lap_Start();

		IFeatureCursorPtr	a_ipChk_Src_Datas, a_ipChk_Tgt_Datas;
	
		// メッシュ矩形の取り出し
		IGeometryPtr	a_ipMeshRect;
		a_ipMeshRect	= get_mesh_rect( a_iCur_Mesh );
		if( a_ipMeshRect == NULL ) {	// メッシュ矩形が無い場合
			PrnMsg(e_fErr_Fp, stderr, "メッシュコード<%d>の矩形が見つかりませんでした\n", a_iCur_Mesh );
			bErrorStat = true;
			continue;
		}
		// クラス内で使用できるようにしておく
		e_ipMeshRect	= a_ipMeshRect;
		e_iMeshCode		= a_iCur_Mesh;
		
		IEnvelopePtr	a_ipRect;
		a_ipMeshRect->get_Envelope ( &a_ipRect );

		// メッシュ矩形でサーチする
		a_ipChk_Src_Datas	= e_mChk_Src_WS.Search_Feature ( a_ipMeshRect, esriSpatialRelIntersects, "", RET_FCURSOR|RET_COUNT );
		if (a_ipChk_Src_Datas == NULL ) {
			PrnMsg(e_fErr_Fp, stderr, "<%d>メッシュ矩形でデータが取れません\n", a_iCur_Mesh );
			bErrorStat = true;
			continue;
		}

		//-----------<チェック進行度表示>--------------------
		long		a_lTCount	= 0;
		a_lTCount	= e_mChk_Src_WS.Get_Data_Count ();
		e_mChk_Src_WS.Clear_Data_Count();

		PrnMsg(stdout, NULL,	"%d 件 ", a_lTCount);

		Exec_Roops ( a_ipChk_Src_Datas );

		DWORD			a_dMake_Finish	= 0;
		a_dMake_Finish	= timeGetTime();

		PrnMsg(stdout, NULL, "処理時間 = %0.2f(s)\n", e_eExeTime.Get_Lap() / 1000.0 );
	}

	//　実行部終了
	// 終了時間の取得
	PrnMsg(stdout, NULL, "データチェック総時間 = %0.2f(s)でした\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );

	if( bErrorStat == true ) {
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
		if( e_cpMeshCode_File.size() == 0 ) {
			PrnMsg(e_fErr_Fp, stderr, "メッシュコードリストファイル名がとりだせませんでした\n");
			return	( false );
		}	
		if( c_aChk_List_Cls.read_mesh_list ( (char*)e_cpMeshCode_File.c_str() ) != 0 ) {	// ここでリストを読み込む
			PrnMsg(e_fErr_Fp, stderr, "メッシュコードリストファイル<%s>の読み込みに失敗[check_exe]\n", e_cpMeshCode_File.c_str() );
			return	( false );
		}
	}else if( eExe_Flag & ON_MONO_MESH ) {	// 単メッシュ指定の場合
		if( c_aChk_List_Cls.read_mesh_list ( e_iMeshCode ) != 0 ) {	// ここでリストを読み込む
			PrnMsg(e_fErr_Fp, stderr, "メッシュ<%d>の設定に失敗しました\n", e_iMeshCode );
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
