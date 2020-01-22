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

#include "main_rootin.h"

/**
	エラーログファイルをオープンする
	@return	true	オープン成功
	@return false	オープン失敗
*/
bool	main_rootin::
Open_Err_Log	( void )
{
	if( eExe_Flag & ON_LOG_FILE ) {
	
		e_fErr_Fp	= fopen ( e_cpLog_File, "w" );
		if( e_fErr_Fp == NULL ) {
			fprintf	( stderr, "エラーログファイル<%s>がオープンできない[main_rootin]\n", e_cpLog_File );
			return	( false );
		}
		
	}else {

		return	( true );

	}
	return	( true );
}

#define	OPTION_MAX	6
#define	BMAX		512

bool	main_rootin::
arg_check	(	int		argc,		// 引数個数
				char	**argv	)	// 引数列
{
	int		a_optcount[OPTION_MAX];	// オプションの重複のカウント
	int		i;
	int		a_exeflag	= 0;
//	int		digit		= 0;	// メッシュの桁数

	for(i = 0; i < OPTION_MAX; i++)
	{
		a_optcount[i] = 0;
	}

	// 引数文字列数のチェック
	for(i = 0; i < argc; i++)
	{
		if(strlen(argv[i]) > BMAX) {
			fprintf	( stderr, "引数文字列が長すぎます!!\n" );
			return	( false );
		}
	}

	if(argc < 3) {
		return( false );
	}

//	char				*eMake_Type_Str		= NULL;

	e_sCheck_Layer		= argv[1];	///< チェックレイヤ

	// オプションがなにも無いとき
	if( argc == 2 ) {
		return	( true );
	}

	// オプションだから、今のところ使用しない
	for(i = 2; i < argc;)
	{
		if(strcmp(argv[i], "-l") == 0) { // ログファイル設定（設定が無い場合は、エラーはstderrへ垂れ流しになる）
			if(argc < i+2) return( false );
			e_cpLog_File	= argv[i+1];	///< メッシュコードリストファイル
			a_exeflag		|= ON_LOG_FILE;
			a_optcount[0]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-m") == 0) {
			if(argc < i+2) return( false );
			e_cpMeshCode_File	= argv[i+1];	///< メッシュコードリストファイル
			a_exeflag		|= ON_MESH;
			a_optcount[1]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-v") == 0) {
			//if(argc < i+1) return( false );
			print_version();
			a_optcount[2]++;
			i+=1;
			return	( false );	// 仮にこうする？
		} else
		if(strcmp(argv[i], "-M") == 0) {
			if(argc < i+2) return( false );
			e_iMeshCode		= atoi(argv[i+1]);	///< メッシュコード
			a_exeflag		|= ON_MONO_MESH;
			a_optcount[3]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-pgdb") == 0) {	/// 2007.05.21
			if(argc < i+2) return( false );
			e_cpPGDB_File	= argv[i+1];
			a_exeflag		|= ON_PGDB_MODE;
			a_optcount[4]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-i") == 0) {	///< 2007.10.25
			if(argc < i+2) return( false );
			e_cpInit_File	= argv[i+1];	///< 初期化ファイル
			a_exeflag		|= ON_INIT_FILE;
			a_optcount[5]++;
			i+=2;
		} else{
			fprintf	(stderr, "オプション %s は間違ってます!!\n", argv[i]);
			return	( false );
		}
	}

	// オプション重複のチェック
	for(i = 0; i < OPTION_MAX; i++)
	{
		if(a_optcount[i] > 1) {
			fprintf	(stderr, "オプションの指定が重複しています!!\n");
			return	( false );
		}
	}
	if( a_exeflag & ON_MONO_MESH && a_exeflag & ON_MESH ) {
		fprintf	(stderr, "オプション'-m'と'-M'は同時に指定できません!!\n");
		return	( false );	
	}
	eExe_Flag	= a_exeflag;
//	return		( a_exeflag );
	return		( true );

}

void		main_rootin::
print_usage	(char **argv)
{
	fprintf(	stdout,
				"=== 【背景スケール間整合チェック】===\n"
				"<usage>\n"
				"%s <arg1>\n"
				"\t<引数>\n"
				"\t      <arg1>             : チェックレイヤ名(ex. SiNDY.City_Site)\n"
				"\t<オプション>\n"
				"\t      '-i'               : 設定ファイル名\n"
				"\t      '-l'               : ログファイル名を指定\n"
				"\t      '-m'               : メッシュコードリストファイルを指定\n"
				"\t      '-M'               : メッシュコード\n"
				"\t      '-pgdb'            : PGDBファイル名(ex, -pgdb C:\\test\\テスト.mdb)\n"
				"\t      '-v'               : バージョン情報表示\n"

			//	"\t<データ作成タイプ>\n"
			//	"\t      'note'             : 注記データを作成\n"
			//	"\t   ※データ作成タイプとメッシュコードリスト内のメッシュコード\n"
			//	"\t     の桁数から、作成するデータを決定している。\n"
			//	"\t     拡張子等については、内部で自動判別している。\n"
			//	"\t<環境変数>\n"
			//	"\t      DBMS_ATTR_PATH		: 属性定義ファイルのあるディレクトリ名\n"
			//	"\t      DBMS_ATTR	    	: 属性定義ファイル名\n"
			//	"\t<説明>\n"
			//	"\t      地面ポリゴンと、行政界データの論理積をとり、地面形の行政界ポリゴンを作成する\n"
			, argv[0]);
}

/**
	バージョン情報の表示
	@return		無し
*/
void	main_rootin::
print_version	( void )
{
	fprintf(	stdout,
				"=== 【背景スケール間整合チェック】 ===\n"
				"\t<バージョン情報>\n"
				"\t2006/09/08	1.0.0.1		・新規作成\n"
				"\t2007/05/21	1.0.1.2		・スケール種別に未調査属性が入っている場合、全てのスケールの種別が「1」の場合に\n"
				"\t							　エラーとして検出されていなかった問題を修正\n"
				"\t							・PGDBのチェックに対応('-pgdb'オプションの追加)\n"
				"\t2007/09/27   1.1.0.3		・VC8対応\n"
				"\t                    		・メッシュコードフィールド名変更への対応\n"
				"\t2007/10/26   1.2.0.5		・[Bug 5633]への対応（環境変数からの設定に対応）\n"
				"\t                    		・設定ファイルでの設定をオプションでの設定に変更('-i'オプション)\n"
				"\t                         ・環境変数等の統一化\n"

			);
}

/**
	メッシュ矩形をDBから獲得するメソッドの初期化
	※get_mesh_rect をやる前に必ず呼ぶ
*/
bool	main_rootin::
get_mesh_init	(	string			c_sMeshLayer,		// メッシュレイヤ名
					IWorkspacePtr	c_ipWorkSpace	)	// 指定するワークスペース
{
	IFeatureWorkspacePtr	a_F_Workspace	( c_ipWorkSpace );	// フィーチャワークスペース

	// レイヤのオープン
	char*	a_Layer_Name		= &c_sMeshLayer[0];		// こんなんでいいのか？？2002/4/30

	if( FAILED(a_F_Workspace->OpenFeatureClass(CComBSTR(a_Layer_Name), &e_ipF_Class )) ) {
		fprintf	(stderr, "レイヤ <%s> のオープンに失敗\n", a_Layer_Name);
		return	(false);
	}else {
		fprintf(stdout,	"<%s> レイヤをオープン\n", a_Layer_Name);
	}
	
	return	( true );
}

/**
	メッシュ矩形をGeoDBから取得する \
	※GeoDBへの接続が確立されていることが前提
*/
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
	if(a_iDigit == 2) {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// メッシュコードを文字列に
		eDigit	= 2;
	}else
	if(a_iDigit == 4) {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// メッシュコードを文字列に 2007.09.03修正
		eDigit	= 4;
	}else
	if(a_iDigit == 6) {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// メッシュコードを文字列に
		eDigit	= 6;
	}else {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);		// メッシュコードを文字列に 2007.09.03修正
		eDigit	= 8;
	}

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
		fprintf	(stderr, "データの属性検索に失敗\n");
		return	(NULL);
	}else {
		IFeaturePtr				a_Feature;
//		long					a_lSp_InitFlag	= 0;	// 空間参照を初期化したかどうかのフラグ

		for(;;)
		{
			if( a_F_Cursor->NextFeature ( &a_Feature ) != S_OK) {	// データをひとつ取り出す
				break;
			}
				
			IGeometryPtr		a_Clip_Shp;		// こいつにデータをコピーしてクリップする

			if( FAILED(a_Feature->get_ShapeCopy	( &a_Clip_Shp )) ) {	// フィーチャから
				fprintf	(stderr, "シェイプのコピーに失敗\n");
				continue;
			}
			
			VARIANT_BOOL	a_bResult;
			a_Clip_Shp->get_IsEmpty( &a_bResult );
			if(a_bResult == VARIANT_TRUE) {	// クリップ結果がNULLならメッシュ外
				continue;
			}
			if( FAILED(a_Clip_Shp->get_Envelope( &a_ipMesh_Rect) )) {
				fprintf	( stderr, "該当メッシュ<%d>の矩形がみつかりませんでした\n", c_iMeshCode);
				return	( NULL );
			}else {
				return	( a_ipMesh_Rect );
			}
		}
	}
	return	( a_ipMesh_Rect );
}


/**
	メッシュリストをGeoDB上から取得する
	@return	bool
*/
bool	main_rootin::
get_mesh_list	( long	c_lDigit )	// メッシュコードの桁数
{

	CComBSTR	a_SpatialCol;
	e_ipF_Class->get_ShapeFieldName	( &a_SpatialCol );
	BSTR		a_Where_Str;
	char		a_tmpstr[60];	//　ここはなんとかしたい気がする
	BSTR		a_bMesh_Str;	// サーチ用メッシュ文字列

//	crd_cnv		crd;
	int			a_iDigit	= 0;
	a_iDigit	= c_lDigit;
	// 中縮尺はただメッシュコードがあるだけなので、存在がどうとかはあまり意味がない？
	if(a_iDigit == 2) {
		a_bMesh_Str	= T2BSTR(_T("MESHCODE"));
		sprintf		(a_tmpstr, "MESHCODE <> 0");		// メッシュコードを文字列に
		eDigit	= 2;
	}else
	if(a_iDigit == 4) {
		a_bMesh_Str	= T2BSTR(_T("MESHCODE"));
		sprintf		(a_tmpstr, "MESHCODE <> 0");		// メッシュコードを文字列に
		eDigit		= 4;
	}else
	if(a_iDigit == 6) {
		a_bMesh_Str	= T2BSTR(_T("MESHCODE"));
		sprintf		(a_tmpstr, "MESHCODE <> 0");		// メッシュコードを文字列に
		eDigit		= 6;
	}else {		// 都市地図メッシュの場合
		// メッシュコードを文字列に
		a_bMesh_Str	= T2BSTR(_T("MESHCODE"));
		sprintf		(a_tmpstr, "COMPLETE_F = 1 or COMPLETE_F = 0" );
		eDigit		= 8;
	}

	a_Where_Str	= T2BSTR(_T(a_tmpstr));		// stringをBSTRに変換

	IQueryFilterPtr		a_Sp_Filter	( CLSID_QueryFilter );			// 空間検索フィルタ
//	a_Sp_Filter->put_SearchOrder	( esriSearchOrderAttribute );	// 属性検索優先
	a_Sp_Filter->put_WhereClause	( a_Where_Str);					// Where句の設定
	
	// クリップ矩形作成
	IEnvelopePtr	a_ipMesh_Rect;	// メッシュ矩形
	a_ipMesh_Rect	= NULL;

	// メッシュコードのフィールドを取得する
	IFieldsPtr	a_ipFields;
	LONG		a_lFieldCount;

	// 属性コピー
	if ( FAILED (e_ipF_Class->FindField ( a_bMesh_Str, &a_lFieldCount ) ) ) {
		fprintf	(stderr, "データの属性検索に失敗\n");
		return	(false);
	}

	// サーチしてくる
	IFeatureCursorPtr	a_F_Cursor;
	if( FAILED	(e_ipF_Class->Search	( a_Sp_Filter, VARIANT_FALSE, &a_F_Cursor )) ) {
		fprintf	(stderr, "データの属性検索に失敗\n");
		return	(false);
	}else {
		IFeaturePtr				a_Feature;

		for(;;)
		{
			if( a_F_Cursor->NextFeature ( &a_Feature ) != S_OK) {	// データをひとつ取り出す
				break;
			}
			
			CComVariant	a_cCur_Value;		// 取得する属性値
			
			a_Feature->get_Value ( a_lFieldCount, &a_cCur_Value);

			int	a_iCur_Mesh	= 0;
			a_iCur_Mesh		= a_cCur_Value.intVal;

			e_sExi_Mesh_List.insert ( a_iCur_Mesh );	
		}
	}
	return	( true );

}

/**
	指定メッシュが存在するかのチェック
	@return	bool
*/
bool	main_rootin::
chk_mesh_exist	( int	c_iMeshCode	)		///< メッシュコード
{
	set<int, less<int> >::iterator	a_isIndx;

	a_isIndx	= e_sExi_Mesh_List.find( c_iMeshCode );
	if( a_isIndx != e_sExi_Mesh_List.end() ) {
		return	( true );
	}

	return	( false );
}

/**
	スケール種別の判定
	@return	0		スケール種別でない
	@return	IS_SCCLS(1)	スケール種別
	@return	IS_SCFLG(2)	スケールフラグ
*/
int	main_rootin::
Scale_Attr_Chk	( string&	c_sFieldName,	///< フィールド名
				  int*		c_ipScale )		///< 該当スケール
{
// スケール別の値の妥当性のためにスケール別属性を追加
	if( c_sFieldName.find ( "SC" ) == -1 ) {
		return	( 0 );
	}else {
		if( c_sFieldName == "SC1BGCLASS_C" ) {
			*c_ipScale	= 1;
			return	( IS_SCCLS );
		}else
		if( c_sFieldName == "SC2BGCLASS_C" ) {
			*c_ipScale	= 2;
			return	( IS_SCCLS );
		}else
		if( c_sFieldName == "SC3BGCLASS_C" ) {
			*c_ipScale	= 3;
			return	( IS_SCCLS );
		}else
		if( c_sFieldName == "SC4BGCLASS_C" ) {
			*c_ipScale	= 4;
			return	( IS_SCCLS );
		}else
		if( c_sFieldName == "DISPSCALE1_F" ) {
			*c_ipScale	= 1;
			return	( IS_SCFLG );
		}else
		if( c_sFieldName == "DISPSCALE2_F" ) {
			*c_ipScale	= 2;
			return	( IS_SCFLG );
		}else
		if( c_sFieldName == "DISPSCALE3_F" ) {
			*c_ipScale	= 3;
			return	( IS_SCFLG );
		}else
		if( c_sFieldName == "DISPSCALE4_F" ) {
			*c_ipScale	= 4;
			return	( IS_SCFLG );
		}
	}
	return ( 0 );
}

/**
	スケール種別の整合性の判別
*/
int	main_rootin::
Chk_Scale_Class_Adjust ( int*	c_ipScaleAttr )
{
	// スケール１、スケール２、スケール３、スケール４が下記の関係以外はだめ
	// 1 1 1 A	非表示 非表示 非表示 属性A
	if( c_ipScaleAttr[0] == 1 &&
		c_ipScaleAttr[1] == 1 &&
		c_ipScaleAttr[2] == 1 &&
		c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// 1 1 A A	非表示 非表示 属性A 属性A
	if( c_ipScaleAttr[0] == 1 &&
		c_ipScaleAttr[1] == 1 &&
		(c_ipScaleAttr[2] == c_ipScaleAttr[3]) &&
		c_ipScaleAttr[2] > 1 ) {
		return	( true );
	}else
	// 1 1 B A	非表示 非表示 属性B 属性A
	if( c_ipScaleAttr[0] == 1 &&
		c_ipScaleAttr[1] == 1 &&
		(c_ipScaleAttr[2] != c_ipScaleAttr[3]) &&
		c_ipScaleAttr[2] > 1 && c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// 1 A A A	非表示 属性A 属性A 属性A
	if( c_ipScaleAttr[0] == 1 &&
		(c_ipScaleAttr[1] == c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] == c_ipScaleAttr[3]) &&
		c_ipScaleAttr[1] > 1 ) {
		return	( true );
	}else
	// 1 B A A	非表示 属性B 属性A 属性A
	if( c_ipScaleAttr[0] == 1 &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] == c_ipScaleAttr[3]) &&
		c_ipScaleAttr[1] > 1 && c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// 1 B B A	非表示 属性B 属性B 属性A
	if( c_ipScaleAttr[0] == 1 &&
		(c_ipScaleAttr[1] == c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] != c_ipScaleAttr[3]) &&
		c_ipScaleAttr[1] > 1 && c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// 1 C B A	非表示 属性C 属性B 属性A
	if( c_ipScaleAttr[0] == 1 &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] != c_ipScaleAttr[3]) &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[3]) &&
		c_ipScaleAttr[1] > 1 && c_ipScaleAttr[2] > 1 && c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// A A A A	属性A 属性A 属性A 属性A
	if( (c_ipScaleAttr[0] == c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[1] == c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] == c_ipScaleAttr[3]) && c_ipScaleAttr[0] > 1 ) {
		return	( true );
	}else
	// B A A A	属性B 属性A 属性A 属性A
	if( (c_ipScaleAttr[0] != c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[1] == c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] == c_ipScaleAttr[3]) &&
		c_ipScaleAttr[0] > 1 && c_ipScaleAttr[1] > 1 ) {
		return	( true );
	}else
	// B B A A	属性B 属性B 属性A 属性A
	if( (c_ipScaleAttr[0] == c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] == c_ipScaleAttr[3]) &&
		c_ipScaleAttr[0] > 1 && c_ipScaleAttr[2] > 1 ) {
		return	( true );
	}else
	// B B B A	属性B 属性B 属性B 属性A
	if( (c_ipScaleAttr[0] == c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[1] == c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] != c_ipScaleAttr[3]) &&
		c_ipScaleAttr[0] > 1 && c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// C B A A	属性C 属性B 属性A 属性A
	if( (c_ipScaleAttr[0] != c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[0] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] == c_ipScaleAttr[3]) &&
		c_ipScaleAttr[0] > 1 && c_ipScaleAttr[1] > 1 &&c_ipScaleAttr[2] > 1 ) {
		return	( true );
	}else
	// C B B A	属性C 属性B 属性B 属性A
	if( (c_ipScaleAttr[0] != c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[1] == c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] != c_ipScaleAttr[3]) &&
		(c_ipScaleAttr[0] != c_ipScaleAttr[3]) &&
		c_ipScaleAttr[0] > 1 && c_ipScaleAttr[1] > 1 &&c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// C C B A	属性C 属性C 属性B 属性A
	if( (c_ipScaleAttr[0] == c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[2] != c_ipScaleAttr[3]) &&
		(c_ipScaleAttr[0] != c_ipScaleAttr[3]) &&
		c_ipScaleAttr[0] > 1 && c_ipScaleAttr[2] > 1 &&c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else
	// D C B A	属性D 属性C 属性B 属性A
	if( (c_ipScaleAttr[0] != c_ipScaleAttr[1]) &&
		(c_ipScaleAttr[0] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[0] != c_ipScaleAttr[3]) &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[2]) &&
		(c_ipScaleAttr[1] != c_ipScaleAttr[3]) &&
		(c_ipScaleAttr[2] != c_ipScaleAttr[3]) &&
		c_ipScaleAttr[0] > 1 && c_ipScaleAttr[1] > 1 && c_ipScaleAttr[2] > 1 && c_ipScaleAttr[3] > 1 ) {
		return	( true );
	}else{
		return ( false );
	}
}

/**
	スケールフラグの整合性の判別
*/
int	main_rootin::
Chk_Scale_Flag_Adjust ( int*	c_ipScaleAttr )
{
	// スケール１、スケール２、スケール３、スケール４が下記の関係以外はだめ
	// 0 0 0 1	非表示 非表示 非表示 表示
	// 0 0 1 1	非表示 非表示 表示 表示
	// 0 1 1 1	非表示 表示 表示 表示
	// 1 1 1 1	表示 表示 表示 表示
	if( c_ipScaleAttr[0] == 0 && c_ipScaleAttr[1] == 0 && c_ipScaleAttr[2] == 0 && c_ipScaleAttr[3] == 1 ) {
		return	( true );
	}else
	if( c_ipScaleAttr[0] == 0 && c_ipScaleAttr[1] == 0 && c_ipScaleAttr[2] == 1 && c_ipScaleAttr[3] == 1 ) {
		return	( true );
	}else
	if( c_ipScaleAttr[0] == 0 && c_ipScaleAttr[1] == 1 && c_ipScaleAttr[2] == 1 && c_ipScaleAttr[3] == 1 ) {
		return	( true );
	}else
	if( c_ipScaleAttr[0] == 1 && c_ipScaleAttr[1] == 1 && c_ipScaleAttr[2] == 1 && c_ipScaleAttr[3] == 1 ) {
		return	( true );
	}
	return ( false );
}

/** 2007.06.19 追加
	DBへの接続オープン
	@return	true	成功
	@return	false	失敗
*/
bool	main_rootin::
DB_Open	(	IWorkspaceFactoryPtr&	c_ipWorkspaceFactory,
			IPropertySetPtr&		c_ipPropertySet,
			IWorkspacePtr&			c_ipWorkspace	)
{
	USES_CONVERSION;	// OLE2T 等の一部のマクロを使用する場合には必ず必要

	/// 接続用プロパティ設定
	CComVariant		vaUser		(_T(e_pTool_Init.get_UserName()) );
	CComVariant		vaPass		(_T(e_pTool_Init.get_Password()) );
	CComVariant		vaInstance	(_T(e_pTool_Init.get_Instance()) );
	CComVariant		vaServer	(_T(e_pTool_Init.get_Server()) );
	CComVariant		vaVersion	(_T(e_pTool_Init.get_Version()) );

	_bstr_t	a_bUSER, a_bINSTANCE, a_bSERVER, a_bPASSWORD, a_bVERSION;	// T2BSTRから_bstr_tに変更 2007.06.18

	a_bUSER		= "USER";
	a_bINSTANCE	= "INSTANCE";
	a_bSERVER	= "SERVER";
	a_bPASSWORD	= "PASSWORD";
	a_bVERSION	= "VERSION";

	c_ipPropertySet->SetProperty( a_bUSER,		vaUser );
	c_ipPropertySet->SetProperty( a_bINSTANCE,	vaInstance );
	c_ipPropertySet->SetProperty( a_bSERVER,	vaServer );
	c_ipPropertySet->SetProperty( a_bPASSWORD,	vaPass );
	c_ipPropertySet->SetProperty( a_bVERSION,	vaVersion );

	printf(_T(	"接続プロパティ：%s/%s/%s/%s/%s\n"),
				OLE2T( vaUser.bstrVal ), 
				OLE2T( vaPass.bstrVal ),
				OLE2T( vaVersion.bstrVal ),
				OLE2T( vaInstance.bstrVal ),
				OLE2T( vaServer.bstrVal ) );

	// DBへ接続
	printf(_T("ＤＢ接続中..."));
	if( c_ipWorkspaceFactory->Open( c_ipPropertySet, NULL, &c_ipWorkspace ) != S_OK ) {
		return	( false );
	}else {
		return	( true );
	}
}

/**
	チェック実行部
	※内部でエラーファイルの処理等も行う
	@return	成功	true
	@return 失敗	false
*/
bool	main_rootin::
check_exe	( void )
{
//#ifdef	_DEBUG
	// 開始時間の取得
	DWORD			a_dBegin_Time	= 0;
	a_dBegin_Time	= timeGetTime();
//#endif

	HRESULT		hr;

	// 初期化ファイル読込み
	if( eExe_Flag & ON_INIT_FILE ) {
		if( e_pTool_Init.read_init_file ( Ret_Init_File() ) != 0 ) {
			return	( false );
		}
	} else {
		// 環境変数から読み込む
		if( e_pTool_Init.read_init_env () != 0 ) {
			return	( false );		
		}
	}

	IWorkspacePtr			ipWorkspace_Any;

	// DBオープン処理
	if( eExe_Flag & ON_PGDB_MODE ){
		// パーソナルジオDB用です
		IWorkspaceFactoryPtr	ipWorkspaceFactory_Any	( CLSID_AccessWorkspaceFactory );
		IPropertySetPtr			ipPropertySet_Any		( CLSID_PropertySet );					// 接続プロパティー格納用
		USES_CONVERSION;	// OLE2T 等の一部のマクロを使用する場合には必ず必要
		// カレント直下にオープンしてみる
		ipWorkspaceFactory_Any->OpenFromFile( T2BSTR(_T(e_cpPGDB_File) ), 0, &ipWorkspace_Any );
		if( ipWorkspace_Any == NULL )	{
			fprintf ( stderr, "パーソナルジオDBがオープンできませんでした\n"); 
			return	( false );
		}else {
			printf(_T("接続完了"));
		}
		printf(_T("\n"));
	}else {

		// 都市地図系用接続
		IWorkspaceFactoryPtr	ipWorkspaceFactory_Any( CLSID_SdeWorkspaceFactory );	// 
		IPropertySetPtr			ipPropertySet_Any( CLSID_PropertySet );					// 接続プロパティー格納用

		// DBへの接続
		if( !DB_Open	(	ipWorkspaceFactory_Any, ipPropertySet_Any, ipWorkspace_Any	) ) {
			fprintf	(stderr, "ＤＢに接続出来なかった\n");
			return	( false );
		}else {
			printf(_T("ＤＢ接続完了"));
		}
		ATLASSERT( ipWorkspace_Any != NULL );
	}

//#ifdef	_DEBUG
	// DB接続にかかった時間の表示
	DWORD				a_dConnect_Time		= 0;
	a_dConnect_Time		= timeGetTime();

	fprintf	( stdout,	"SiNDY-DB接続処理時間 = %0.2f(s)でした\n",
						(double)((a_dConnect_Time - a_dBegin_Time) / 1000.0) );
//#endif

	// エラーログファイルのオープン
	if( !Open_Err_Log () ) {
		return	( false );
	}else {
		if( !e_sErr_Log.Init ( Log_Fp() ) ) {
			return ( false );
		}
	}

	mk_input_list	a_mChk_List;
	char*			a_cpList_File	= NULL;
	int				a_iMDigit		= 0;

	if( eExe_Flag & ON_MESH ||		//	メッシュコードリスト指定の場合
		eExe_Flag & ON_MONO_MESH ) {	//	メッシュコード指定の場合

		if( eExe_Flag & ON_MESH ) {
			a_cpList_File	= Ret_Mesh_File ( );
			if( a_cpList_File == NULL ) {
				fprintf	( stderr, "メッシュコードリストファイル名がとりだせませんでした[check_exe]\n");
				return	( false );
			}	
			if( a_mChk_List.read_mesh_list ( a_cpList_File ) != 0 ) {	// ここでリストを読み込む
				fprintf	( stderr, "メッシュコードリストファイル<%s>の読み込みに失敗[check_exe]\n", a_cpList_File );
				return	( false );
			}
		}else {
			if( a_mChk_List.read_mesh_list ( e_iMeshCode ) != 0 ) {	// ここでリストを読み込む
				fprintf	( stderr, "メッシュコード<%s>の読み込みに失敗[check_exe]\n", a_cpList_File );
				return	( false );
			}
		}
	
		char*		a_cpMeshLayer	= NULL;
		a_iMDigit	= a_mChk_List.get_mesh_digit ();
		
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
		case	2:	// 2004.02.24
			a_cpMeshLayer	= e_pTool_Init.get_Top_Mesh ();
			break;
		default:
			fprintf	( stderr, "リストのメッシュ桁数には対応していません[check_exe]\n");
			return	( false );
		}

		if( !get_mesh_init( a_cpMeshLayer , ipWorkspace_Any ) ) {
			fprintf	( stderr, "メッシュレイヤの獲得の初期化に失敗[check_exe]\n");
			return	( false );
		}

		if( !get_mesh_list ( a_iMDigit ) ) {
			fprintf	( stderr, "メッシュリストが作成できませんでした[check_exe]\n");
			return	( false );
		}
	}else {
		fprintf	( stderr, "メッシュリスト'-m'またはメッシュコード'-M'を指定してください\n");
		return	( false );
	}

	// チェックするレイヤ名の獲得
	char*			a_cpCheck_Layer	= NULL;
	a_cpCheck_Layer	= Ret_Check_Layer();
	if( a_cpCheck_Layer == NULL ) {
		fprintf	( stderr, "チェック対象のポイントデータレイヤ名がとりだせませんでした\n");
		return	( false );
	}

	// チェックレイヤの読み込み
	msi_handle_ws	a_mChk_WS;
	fprintf	( stderr, "<%s>レイヤのチェックを行います\n", a_cpCheck_Layer);
	if( !a_mChk_WS.Init ( ipWorkspace_Any, a_cpCheck_Layer) ) {
		fprintf	( stderr, "チェックレイヤ<%s>の読み込みに失敗[check_exe]\n", a_cpCheck_Layer );
		return	( false );		
	}

	// キャッシュ上にとってくる
	// オンメモリテスト 2002.6.24
	ISpatialCacheManagerPtr		a_ipSpCacheMgr_Any( ipWorkspace_Any );

	set<checked_id>				a_sChecked_OID_Set;
	set<checked_id>::iterator	a_sChecked_OID_Indx;

	if( eExe_Flag & ON_MESH	||
		eExe_Flag & ON_MONO_MESH	) {	// メッシュコードリストを読み込んでのチェックの場合

		int		a_iCur_Mesh;
		for( a_iCur_Mesh = a_mChk_List.get_mesh_first(); a_iCur_Mesh != -1; a_iCur_Mesh = a_mChk_List.get_mesh_next() )
		{

			/// データ作成開始時間
			DWORD				a_dMesh_Begin_Time	= 0;
			a_dMesh_Begin_Time	= timeGetTime();

			e_sErr_Log.Set_Mesh			( a_iCur_Mesh );	// メッシュコードの設定
			e_sErr_Log.Set_Layer_Name	( a_cpCheck_Layer ); // レイヤ名称の設定
			e_sErr_Log.Set_Err_Level	( 0 );				// エラーレベルの設定

			// メッシュごとにキャッシュマネージャをクリアしておく
			hr	= a_ipSpCacheMgr_Any->EmptyCache( );
			if( FAILED ( hr ) ) {
				fprintf	( stderr, "キャッシュマネージャをクリアできませんでした\n");
				continue;
			}

			fprintf	( stdout, "メッシュコード [%d] チェック中\n", a_iCur_Mesh );

			IFeatureCursorPtr	a_ipDatas;
		
			// メッシュ矩形の取り出し
			IEnvelopePtr	a_ipRect;
			a_ipRect		= get_mesh_rect( a_iCur_Mesh );
			if( a_ipRect == NULL ) {	// メッシュ矩形が無い場合
				fprintf	( stderr, "メッシュコード<%d>の矩形が見つかりませんでした\n", a_iCur_Mesh );
				continue;
			}
		
			// 空間キャッシュマネージャにメッシュ内のデータをとりだす
//			a_ipSpCacheMgr_Any->FillCache( a_ipRect );

			// メッシュ矩形の境界線でサーチする
			a_ipDatas	= a_mChk_WS.Search_Feature	( a_ipRect, esriSpatialRelIntersects, "", RET_COUNT|RET_FCURSOR );	// メッシュ矩形ラインとポイントの交差を見る

			if (a_ipDatas == NULL ) {
				fprintf	( stderr, "<%d>メッシュ矩形でデータが取れません\n", a_iCur_Mesh );
				continue;
			}

			//-----------<チェック進行度表示>--------------------
			long		i			= 0;
			long		a_lTCount	= 0;

			a_lTCount	= a_mChk_WS.Get_Data_Count ();

			fprintf	( stdout,	"メッシュコード [%d] 総データ件数 %d 件\n", a_iCur_Mesh, a_lTCount );

			//--------------------------------------------------

			IFeaturePtr		a_ipCurFeat;
			ms_geometry_inc	a_msGeometry_MSet;
			char	a_cMsgBuf[512];
			// データをベクタにもってくる
			for ( ; ; i++)
			{
				geometry_inc	a_gGeoInc;
				//　メッシュで引っ掛けてきたフィーチャ
				if( a_ipDatas->NextFeature ( &a_ipCurFeat ) == S_OK) {

					// オブジェクトIDの重複チェックのチェック
			/*		a_sOID_Indx	= a_sOID_Set.find ( (int)a_lOid );
					if( a_sOID_Indx != a_sOID_Set.end() ) {
						continue;	// 一度チェックした奴はやらない
					}else {
						// チェックするIDを登録
						a_sOID_Set.insert ( (int)a_lOid );
					}*/

					Feature_All_Attrs	a_fCurAttrs;

					a_fCurAttrs.Init ( a_ipCurFeat );	// 持っている属性みんな取り出す

					long		a_lOid		= 0;
					long		a_lSrc_Oid	= 0;	// DB上でのOID
					string		a_sPrnMsg	= "";
					long		a_lAttrNum	= 0;

					a_lOid		= a_fCurAttrs.Get_Obj_ID ();
					a_lAttrNum	= a_fCurAttrs.Get_Num();
					e_sErr_Log.Set_OID	( (int)a_lOid );

					int	a_iScaleChk_F	= 0;	// スケール妥当性チェックをするかのフラグ
					int	a_iRet	= 0;
					int	a_iScaleAttr[4]	= {0,0,0,0};

					int j = 0;
					for( j = 0; j < a_lAttrNum; j++ )
					{

						Feature_Attr	a_fCAtt;
						// キーフィールドの情報を取り出す
						a_fCAtt			= a_fCurAttrs.Get_Attr ( j );

						string	a_sFld_Name	= a_fCAtt.e_bFld_Name;
						// 大文字化
						int	c = 0;
						while(a_sFld_Name[c] != '\0' ){
							a_sFld_Name[c]	= (char)toupper(a_sFld_Name[c]);
							c++;
						}

						if( a_sFld_Name == "FID" ||
							a_sFld_Name == "OBJECTID" ||
							a_sFld_Name == "SHAPE" ) {
							if( a_sFld_Name == "OBJECTID" ) {
								a_lSrc_Oid	= a_fCAtt.e_ccvAttr.intVal;
							}
							continue;
						}
						int	a_iAttrVal	= 0;
						
						int	a_iScale= 0;
						// スケール関係種別の取り出し
						a_iRet	= Scale_Attr_Chk ( a_fCAtt.e_bFld_Name, &a_iScale );
						if( a_iRet != 0 ) {
							if( a_iScaleChk_F == 0 ) {
								a_iScaleChk_F				= a_iRet;
							}
							a_iAttrVal					= a_fCAtt.e_ccvAttr.intVal;
							a_iScaleAttr[a_iScale-1]	= a_iAttrVal;	// スケール別の属性を取っておく
						}else {
							continue;
						}
					}

					if ( a_iScaleChk_F == IS_SCFLG ) {	// 表示フラグ形式の場合
						if( !Chk_Scale_Flag_Adjust ( a_iScaleAttr ) ) {
							sprintf( a_cMsgBuf, "スケール間の整合がおかしい\t【%d %d %d %d】",
									a_iScaleAttr[0], a_iScaleAttr[1], a_iScaleAttr[2], a_iScaleAttr[3]);
							e_sErr_Log.Set_Err_Msg	( a_cMsgBuf );	// エラーメッセージの設定
							e_sErr_Log.Prn_SiNDY_Err( 0, NULL, stderr  );
						}
					}else {	// スケール別種別の場合
						if( !Chk_Scale_Class_Adjust ( a_iScaleAttr ) ) {
							sprintf( a_cMsgBuf, "スケール間の整合がおかしい\t【%d %d %d %d】",
									a_iScaleAttr[0], a_iScaleAttr[1], a_iScaleAttr[2], a_iScaleAttr[3]);
							e_sErr_Log.Set_Err_Msg	( a_cMsgBuf );	// エラーメッセージの設定
							e_sErr_Log.Prn_SiNDY_Err( 0, NULL, stderr  );
						}
					}
		/*			//----------------------------------------------------------
					//	if( a_sChecked_OID_Set.size() > 10000000 ) {
						if( a_sChecked_OID_Set.size() > 1000000 ) {
							a_sChecked_OID_Set.clear();
						}
						a_sChecked_OID_Indx	= a_sChecked_OID_Set.find( a_cChk_Test );
						if( a_sChecked_OID_Indx != a_sChecked_OID_Set.end() ){
							continue;	// チェック済みなのでとばす
						}else {
							a_sChecked_OID_Indx	= a_sChecked_OID_Set.find( a_cChk_Test_Rev );
							if( a_sChecked_OID_Indx != a_sChecked_OID_Set.end() ){
								continue;	// チェック済みなのでとばす
							}
							// みつからなかった場合は新規登録
							a_sChecked_OID_Set.insert ( a_cChk_Test );
						}
					}*/
					//----------------
					if( i > 0 && (i % 100) == 0) {	// 
						// カウント100件ごとにメッセージを表示
						fprintf( stdout,	"メッシュコード [%d]  %d 件中　%d 件 チェック終了 \n",
											a_iCur_Mesh,
											a_lTCount,
											i	);
					}
					//----------------

				}else {
					break;
				}
			}

			DWORD			a_dMake_Finish	= 0;
			a_dMake_Finish	= timeGetTime();

			fprintf	( stdout,	"<%d> :  データチェック時間 = %0.2f(s)でした\n",
								a_iCur_Mesh,
								(double)((a_dMake_Finish - a_dMesh_Begin_Time) / 1000.0) );
			//	e_sErr_Log.Set_OID	( a_lCur_Oid );
			//----------------
			fprintf	( stdout, "メッシュコード [%d] チェック終了\n", a_iCur_Mesh );
			//-----------------
		}
	
	}
	return	( true );
}

/**
	レイヤ名称から必要なワークスペースを判定する
*/
IWorkspacePtr	main_rootin::
Get_Ref_Workspace	(	IWorkspacePtr	c_ipWSpace_Map,		///< ワークスペース
						IWorkspacePtr	c_ipWSpace_Base,	///< ワークスペース
						IWorkspacePtr	c_ipWSpace_Addr,	///< ワークスペース
						char*			c_cpLayerName )
{
	if( c_cpLayerName == NULL ) {
		return	( NULL );
	}
	IWorkspacePtr	a_ipRet_WSpace	= NULL;
	char*			a_cpLayerTmp	= NULL;
	a_cpLayerTmp	= new char[strlen(c_cpLayerName) + 1];
	char*			a_cpLayer	= NULL;

	// レイヤ名をコピー
	strcpy ( a_cpLayerTmp, c_cpLayerName );
	a_cpLayer	= strtok( a_cpLayerTmp, "." );	// ピリオドで切る
	a_cpLayer	= strtok( NULL, "." );
	if( a_cpLayer == NULL ) {
		delete [] a_cpLayerTmp;
		return	( NULL );
	}

	// レイヤ名で接続WSを判定
	string		a_sLayerName	= a_cpLayer;
	if( a_sLayerName == "CityMesh" ||		///< 都市地図メッシュレイヤ	
		a_sLayerName == "BaseMesh" ||		///< 中縮尺メッシュレイヤ	
		a_sLayerName == "MiddleMesh" )		///< ミドルメッシュレイヤ
	{
		a_ipRet_WSpace	= c_ipWSpace_Map;
	}else
	if( a_sLayerName == "City_Annotation" ||///< 都市地図注記ポイントレイヤ
		a_sLayerName == "City_Disp_Line" ||	///< 都市地図注記ラインレイヤ
		a_sLayerName == "City_Line"	||		///< 都市地図背景アークレイヤ
		a_sLayerName == "City_Railway" ||	///< 都市地図鉄道レイヤ
		a_sLayerName == "City_Site" ||		///< 都市地図街区レイヤ
		a_sLayerName == "City_Station" ||	///< 都市地図駅舎レイヤ	
		a_sLayerName == "Building" ||		///< 都市地図建物レイヤ	
		a_sLayerName == "City_Line" ||		///< 都市地図建物アークレイヤ
		a_sLayerName == "Building_Step" )	///< 都市地図kplレイヤ
	{
		a_ipRet_WSpace	= c_ipWSpace_Map;
	}else
	if( a_sLayerName ==	"Sc1Disp_Line"	||	///< 中縮尺注記ラインレイヤ１
		a_sLayerName ==	"Sc2Disp_Line"	||	///< 中縮尺注記ラインレイヤ２
		a_sLayerName ==	"Sc3Disp_Line"	||	///< 中縮尺注記ラインレイヤ３
		a_sLayerName ==	"Sc4Disp_Line"	||	///< 中縮尺注記ラインレイヤ４
		a_sLayerName ==	"Base_Annotation"	||	///< 中縮尺注記
		a_sLayerName ==	"Base_Contour"	||	///< 段彩レイヤ
		a_sLayerName ==	"Base_Line"	||		///< 中縮尺背景アークレイヤ
		a_sLayerName ==	"Base_Railway"	||	///< 中縮尺鉄道レイヤ
		a_sLayerName ==	"Base_Site"	||		///< 中縮尺背景、施設レイヤ
		a_sLayerName ==	"Base_Station"	||	///< 中縮尺駅舎レイヤ
		a_sLayerName ==	"Sc1Disp_Line"	||	///< ミドル注記ラインレイヤ１
		a_sLayerName ==	"Sc2Disp_Line"	||	///< ミドル注記ラインレイヤ２
		a_sLayerName ==	"Sc3Disp_Line"	||	///< ミドル注記ラインレイヤ３
		a_sLayerName ==	"Sc4Disp_Line"	||	///< ミドル注記ラインレイヤ４
		a_sLayerName ==	"Middle_Annotation"	||	///< ミドル注記
		a_sLayerName ==	"Middle_Contour"	||	///< 段彩レイヤ
		a_sLayerName ==	"Middle_Line"	||	///< ミドル背景アークレイヤ
		a_sLayerName ==	"Middle_Site"	)	///< ミドル背景、施設レイヤ
	{
		a_ipRet_WSpace	= c_ipWSpace_Base;
	}else 
	if( a_sLayerName == "Base_Admin"	||	///< 中縮尺行政界
		a_sLayerName == "City_Admin"	||	///< 都市地図行政界
		a_sLayerName == "Base_Rep_Point"||	///< 中縮尺Addrポイント
		a_sLayerName == "City_Rep_Point" ||	///< 地番注記レイヤ
		a_sLayerName == "Gou_Point" ||		///< 号ポイント
		a_sLayerName == "KLink_Point"	)	///< 号ポイント
	{
		a_ipRet_WSpace	= c_ipWSpace_Addr;		
	}

	if( a_cpLayerTmp != NULL ) {
		delete [] a_cpLayerTmp;
	}
	return ( a_ipRet_WSpace );
}
