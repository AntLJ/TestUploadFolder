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

#include <atlstr.h>
#include "..\..\LIB\WinLib\VersionInfo.h"

//	2003.07.04	main_rootin.hから定義を分離

/**
	DBへの接続オープン
	@return	true	成功
	@return	false	失敗
*/
bool	main_rootin::
DB_Open	(	IWorkspacePtr&			c_ipWorkspace	)
{
	USES_CONVERSION;	// OLE2T 等の一部のマクロを使用する場合には必ず必要

	if( eExe_Flag & ON_PGDB_MODE ){	// 2007.03.02
		// パーソナルジオDB用です
		IWorkspaceFactoryPtr	ipWorkspaceFactory	( CLSID_AccessWorkspaceFactory );
		IPropertySetPtr			ipPropertySet		( CLSID_PropertySet );					// 接続プロパティー格納用
		IWorkspacePtr			ipWorkspace;
		// カレント直下にオープンしてみる

		_bstr_t					a_bPGDG_FName;

		a_bPGDG_FName			= e_sConnect_Info.c_str();

		ipWorkspaceFactory->OpenFromFile( a_bPGDG_FName, 0, &ipWorkspace );
		if( ipWorkspace == NULL )	{
			fprintf ( stderr, "パーソナルジオDBがオープンできませんでした\n"); 
			return	( false );
		}else {
			printf(_T("接続完了"));
			c_ipWorkspace			= ipWorkspace;
		}
		printf(_T("\n"));
		return	( true );
	}else {

		

		CString connectStr;
		connectStr.Format(_T("%s/%s/%s/%s/%s"),
			e_pTool_Init.get_UserName(), e_pTool_Init.get_Password(),
			e_pTool_Init.get_Version(), e_pTool_Init.get_Instance(), e_pTool_Init.get_Server());

		printf(_T(	"#接続プロパティ：<%s>\n"),	connectStr);

		c_ipWorkspace = sindy::create_workspace(connectStr);

		// DBへ接続
		printf(_T("接続中..."));
		if( !c_ipWorkspace) {
			return	( false );
		}else {
			return	( true );
		}
	}
}

/**
	住所DB関係のオープン
	@return 成功	true
	@return	失敗	false
*/
bool	main_rootin::
Connect_to_Adm_DB	( void )	///< 接続文字列[IN]
{
	fprintf( stdout,	"#住所DB接続プロパティ：%s\n", e_sConnectAddrDB.c_str() );
	fprintf( stdout,	"#接続中...");

	if( !e_aAddr_DB_Hndl.Init_DB ( (char*)e_sConnectAddrDB.c_str() ) ) {
		fprintf	( stderr, "#サービス<%s>への接続に失敗\n", e_sConnectAddrDB.c_str() );
		return	( false );
	}
	fprintf( stdout,	"接続完了\n");
	return	( true );
}


/**
	エラーログファイルをオープンする
	@return	true	オープン成功
	@return false	オープン失敗
*/
bool	main_rootin::
Open_Err_Log	( void )
{
	if( e_sLog_File == "" ) {
		return	( true );
	}else {
		e_fpErr_Fp	= fopen ( e_sLog_File.c_str(), "w" );
		if( e_fpErr_Fp == NULL ) {
			fprintf	( stderr, "エラーログファイル<%s>がオープンできない[main_rootin]\n", e_sLog_File.c_str() );
			return	( false );
		}
	}
	return	( true );
}

#define	OPTION_MAX	14
#define	BMAX		512

bool	main_rootin::
arg_check	(	int		argc,		// 引数個数
				char	**argv	)	// 引数列
{
	int		a_optcount[OPTION_MAX];	// オプションの重複のカウント
	int		i;
	int		a_exeflag	= 0;
//	int		digit		= 0;	// メッシュの桁数

	//-----------------------------------------------------------------------
	// ツールバージョン表示
	CVersion	a_cvTool_Var;
	CString		a_csVar;
	a_csVar		= a_cvTool_Var.GetFileVersion();
	e_sVersion	= (char*)(_bstr_t(a_csVar));
	//-----------------------------------------------------------------------

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

	if(argc < 2) {
		return( false );
	}

//	char				*eMake_Type_Str		= NULL;

//	e_sInit_File		= argv[1];	///< 初期化ファイル
//	e_sCheck_Layer		= argv[2];	///< チェックレイヤ

	// オプションがなにも無いとき
	if( argc == 1 ) {
		return	( true );
	}

	// オプションだから、今のところ使用しない
	for(i = 1; i < argc;)
	{
		if(strcmp(argv[i], "-cnct") == 0) { // ログファイル設定（設定が無い場合は、エラーはstderrへ垂れ流しになる）
			if(argc < i+2) return( false );
			e_sConnect_Info		= argv[i+1];	///< 初期化ファイル
			a_exeflag			|= ON_DB_CNCT;
			a_optcount[0]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-fc") == 0) { // チェックレイヤ
			if(argc < i+2) return( false );
			e_sCheck_Layer	= argv[i+1];	///< 初期化ファイル
			a_exeflag		|= ON_CHKLAYER;
			a_optcount[1]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-mf") == 0) { // メッシュレイヤ
			if(argc < i+2) return( false );
			e_sMesh_Layer	= argv[i+1];	///< 初期化ファイル
			a_exeflag		|= ON_MESHLAYER;
			a_optcount[2]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-l") == 0) { // ログファイル設定（設定が無い場合は、エラーはstderrへ垂れ流しになる）
			if(argc < i+2) return( false );
			e_sLog_File	= argv[i+1];		///< メッシュコードリストファイル
			a_optcount[3]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-m") == 0) {	// 2002.9.24
			if(argc < i+2) return( false );
			e_sMeshCode_File	= argv[i+1];///< メッシュコードリストファイル
			a_exeflag	|= ON_MESH;
			a_optcount[4]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-M") == 0) {	// 2003.4.1
			if(argc < i+2) return( false );
			a_exeflag	|= ON_MONO_MESH;
			e_iMeshCode	= atoi(argv[i+1]);	
			a_optcount[5]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-w") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_WHERE_STR;
			e_sWhere_Str	= argv[i+1];	// エリアを絞る条件を取得する
			a_optcount[6]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-pgdb") == 0) {
			a_exeflag		|= ON_PGDB_MODE;
			a_optcount[7]++;
			i+=1;
		} else
		if(strcmp(argv[i], "-v") == 0) {
			print_version();
			a_optcount[8]++;
			i+=1;
			return	( false );
		} else
		if(strcmp(argv[i], "-o") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_OUTFILE;
			e_sOut_File		= argv[i+1];	// エリアを絞る条件を取得する
			a_optcount[9]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-addrdb") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_ADDRDB;
			e_sConnectAddrDB= argv[i+1];	// 住所DBへの接続
			a_optcount[10]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-field") == 0) {
			a_exeflag		|= ON_FIELDS;
			e_sFields		= argv[i+1];
			a_optcount[11]++;
			i+=2;
		} else
		if(strcmp(argv[i], "-ku") == 0) {
			a_exeflag		|= ON_KU_MODE;
			a_optcount[12]++;
			i+=1;
		} else
		if(strcmp(argv[i], "-gun") == 0) {
			a_exeflag		|= ON_GUN_MODE;
			a_optcount[13]++;
			i+=1;
		} else {
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

	// 初期化ファイル設定
	if( !(a_exeflag & ON_DB_CNCT) ) {
		fprintf	(stderr, "DBの接続設定を'-cnct'で行ってください\n");
		return	( false );
	}else {
		if( !(a_exeflag & ON_PGDB_MODE ) ) {
			string	a_sTmpConnect_Info;	// 内部でカットされるのが嫌なので・・
			a_sTmpConnect_Info	= e_sConnect_Info;
			if( !e_pTool_Init.read_init_str ( (char*)a_sTmpConnect_Info.c_str() ) ) {
				return	( false );	// メッセージは内部で出している
			}
		}
	}

	// メッシュ指定
	if( a_exeflag & ON_MESH && a_exeflag & ON_MONO_MESH ) {
		fprintf	(stderr, "'-m','-M'は同時指定できません\n");
		return	( false );
	}else {
		if( !(a_exeflag & ON_MESH || a_exeflag & ON_MONO_MESH) ) {
			fprintf	(stderr, "'-m','-M'のどちらかを指定して下さい\n");
			return	( false );
		}
	}

	// メッシュレイヤ
	if( !(a_exeflag & ON_MESHLAYER) ) {
		fprintf	(stderr, "メッシュフィーチャクラスを'-mf'で行ってください\n");
		return	( false );
	}

	// チェックレイヤ
	if( !(a_exeflag & ON_CHKLAYER) ) {
		fprintf	(stderr, "ポリゴンフィーチャクラスを'-fc'で行ってください\n");
		return	( false );
	}

	// 出力ファイルオープン
	if( a_exeflag & ON_OUTFILE ) {
		e_fpOut_Fp	= fopen ( e_sOut_File.c_str() , "w" );
		if ( e_fpOut_Fp == NULL ) {
			fprintf	( stderr, "出力ファイル<%s>がオープンできません\n", e_sOut_File.c_str() );
			return	( false );
		}
	}

	// フィールド情報を切り分ける
	if( a_exeflag & ON_FIELDS && a_exeflag & ON_ADDRDB ) {
		fprintf	( stderr, "'-field'と'-addrdb'の同時指定は出来ません\n" );
		return	( false );
	}else {
		if( a_exeflag & ON_FIELDS ) {
			// フィールドは','で分ける
			size_t	a_sBegin	= 0;			// 文字列開始位置
			size_t	a_sPos		= 0;			// デリミタ検出位置
			string	a_sTmpStr	= e_sFields;	// 分割処理用
			for(;;)
			{
				a_sPos	= e_sFields.find ( ",", a_sBegin );
				if( a_sPos != e_sFields.npos ) {
					e_vsGetField.push_back ( e_sFields.substr ( a_sBegin, (a_sPos-a_sBegin) ) );
					a_sBegin	= a_sPos+1;
				}else {
					e_vsGetField.push_back ( e_sFields.substr ( a_sBegin, (e_sFields.size()-a_sBegin) ) );
					break;
				}
			}
		}else
		if( a_exeflag & ON_ADDRDB ) {
			if( !Connect_to_Adm_DB ( )  ) {
				fprintf	( stderr, "住所DBの初期化に失敗しています\n" );
				return	( false );
			}	
		}else {
			fprintf	( stderr, "'-field'又は'-addrdb'を指定し、どのタイプの出力を行うかを決めてください\n" );
			return	( false );
		}
	}

	eExe_Flag	= a_exeflag;
	return		( true );
}

void		main_rootin::
print_usage	(char **argv)
{
	fprintf(	stdout,
			"=== 【SiNDY-DB面積値算出ツール】(%s) ===\n"
				"<usage>\n"
				"\t%s [option]...\n"
				"\t<オプション>\n"
				"\t      '-cnct'            : DB接続情報(ex. -cnct ronly/ronly/SDE.DEFAULT/ruby2)\n"
				"\t      '-fc'              : ポリゴンフィーチャクラス(ex. -fc SINDY2009A.City_Admin)\n"
				"\t      '-mf'              : メッシュフィーチャクラス(ex. -mf SINDY2009A.City_Admin)\n"
				"\t      '-l'               : ログデータ出力パスを指定\n"
				"\t      '-m'               : メッシュコードリストファイルを指定(ex. -m .\\list\\mesh.txt）\n"
				"\t      '-M'               : メッシュコードを指定(ex. -M 59414130)\n"
				"\t      '-w'               : 条件指定のWHERE句指定(ex. OBJECTID > 0)\n"
				"\t      '-pgdb'            : PGDBのフィーチャクラスを読み込む場合に指定\n"
				"\t      '-o'               : 出力ファイル指定(ex. -o out.txt)\n"
				"\t                           (指定しない場合はstdoutへ出力)\n"
				"\t      '-addrdb'          : 住所DB接続設定(ex. y0901e/y0901e@delltcp)\n" 
				"\t                           ※'-field'とは同時指定できない。市区町村別面積値を求める際に使用\n"
				"\t      '-field'           : 出力フィールド設定(ex -field CITYCODE,ADDRCODE)\n"
				"\t                           ※'-addrdb'とは同時指定不可\n"
				"\t      '-ku'              : 区を纏めて市にして出力する\n"
				"\t      '-gun'             : 郡名も出力する(未指定時はカットする)\n"
				"\t      '-v'               : バージョン情報表示\n"
				"\t<補足>\n"
				"\t ※'-cnct','-fc','-mf','-m'又は'-M','-addrdb'又は'-field'は必須です。\n"
				"\t ※市区町村別面積値を求める場合は'-addrdb'を、メッシュ単位でポリゴンの面積＋フィールドの情報\n"
				"\t   を出力する場合はfieldを使用してください。\n"

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
			, e_sVersion.c_str(), argv[0]);
}

/** ------------------------------------------------------------------------------
	バージョン情報の表示
	@return		無し
---------------------------------------------------------------------------------*/
void	main_rootin::
print_version	( void )
{
	fprintf(	stdout,
				"=== 【SiNDY-DB面積値算出ツール】(%s) ===\n"
				"\t<バージョン情報>\n"
				"\t2009/02/23     1.0.0.1	: 新規作成\n", 
				"\t2009/06/01     1.0.1.2	: [bug 7057]への対応(政令市の都市コードが適切に出力されない)\n", 
				e_sVersion.c_str()
			);
}

/** ---------------------------------------------------------------------------
//	エラーログのヘッダを出力する
//	@return	無し
-----------------------------------------------------------------------------*/
void	main_rootin::
Output_Log_Header	( void )
{
	fprintf	( e_fpErr_Fp,		"#SINDYSTDLOG\n" );
	fprintf	( e_fpErr_Fp,		"#ツールバージョン          : <%s>\n", e_sVersion.c_str() );
	if( eExe_Flag & ON_PGDB_MODE ) {
		fprintf	( e_fpErr_Fp,	"#接続情報(PGDB)            : <%s>\n", e_sConnect_Info.c_str() );
	}else {
		fprintf	( e_fpErr_Fp,	"#接続情報(DB_ANY)          : <%s>\n", e_sConnect_Info.c_str() );
	}
	fprintf	( e_fpErr_Fp,		"#出力ファイル              : <%s>\n", e_sOut_File.c_str() );
	fprintf	( e_fpErr_Fp,		"#メッシュフィーチャクラス  : <%s>\n", e_sMesh_Layer.c_str() );
	fprintf	( e_fpErr_Fp,		"#ポリゴンフィーチャクラス  : <%s>\n", e_sCheck_Layer.c_str() );
	if( eExe_Flag & ON_MESH ) {
		fprintf	( e_fpErr_Fp,	"#メッシュリスト            : <%s>\n", e_sMeshCode_File.c_str() );
	}else {
		fprintf	( e_fpErr_Fp,	"#メッシュコード            : <%d>\n", e_iMeshCode );
	}
	if( eExe_Flag & ON_WHERE_STR ) {
		fprintf	( e_fpErr_Fp,	"#抽出条件                  : <%s>\n", e_sWhere_Str.c_str() );
	}
	if( eExe_Flag & ON_ADDRDB ) {
		fprintf	( e_fpErr_Fp,	"#住所DB接続情報            : <%s>\n", e_sConnectAddrDB.c_str() );
		fprintf	( e_fpErr_Fp,	"#特別区の処理              : <%s>\n", eExe_Flag&ON_KU_MODE?"市にまとめる":"そのまま出力" );
		fprintf	( e_fpErr_Fp,	"#郡名出力                  : <%s>\n", eExe_Flag&ON_GUN_MODE?"あり":"なし" );
	}else {
		fprintf	( e_fpErr_Fp,	"#出力フィールド            : <%s>\n", e_sFields.c_str() );
	}

//	fprintf	( e_fErr_Fp,		"#属性取得フィールド        : <%s>\n", e_sSet_Fields.c_str() );
	fflush	( e_fpErr_Fp );
}

/** ------------------------------------------------------------------------------
	メッシュ矩形をDBから獲得するメソッドの初期化
	※get_mesh_rect をやる前に必ず呼ぶ
------------------------------------------------------------------------------==*/
bool	main_rootin::
get_mesh_init	(	mk_input_list&	c_mMList,			// 入力リストクラス
				//	string			c_sMeshLayer,		// メッシュレイヤ名
					IWorkspacePtr	c_ipWorkSpace	)	// 指定するワークスペース
{

	char*			a_cpList_File	= NULL;
	if( eExe_Flag & ON_MESH ) {
		a_cpList_File	= (char*)e_sMeshCode_File.c_str();
		if( a_cpList_File == NULL ) {
			fprintf	( e_fpErr_Fp, "メッシュコードリストファイル名がとりだせませんでした[check_exe]\n");
			return	( false );
		}	
		if( c_mMList.read_mesh_list ( a_cpList_File ) != 0 ) {	// ここでリストを読み込む
			fprintf	( e_fpErr_Fp, "メッシュコードリストファイル<%s>の読み込みに失敗[check_exe]\n", a_cpList_File );
			return	( false );
		}
	}else {
		if(c_mMList.read_mesh_list ( e_iMeshCode ) == -1) {
			fprintf	( e_fpErr_Fp, "メッシュ<%d>の設定に失敗しました\n", e_iMeshCode );
			return	( false );
		}
	}

	int			a_iMDigit		= 0;
	a_iMDigit	= c_mMList.get_mesh_digit ();
	
	switch	( a_iMDigit )
	{
	case	4:
	case	6:
	case	8:
		break;
	default:
		fprintf	( stderr, "リストのメッシュ桁数には対応していません[check_exe]\n");
		return	( false );
	}

	IFeatureWorkspacePtr	a_F_Workspace	( c_ipWorkSpace );	// フィーチャワークスペース

	if( FAILED(a_F_Workspace->OpenFeatureClass(CComBSTR(e_sMesh_Layer.c_str()), &e_ipF_Class )) ) {
		fprintf	(stderr, "レイヤ <%s> のオープンに失敗\n", e_sMesh_Layer.c_str());
		return	(false);
	}else {
		fprintf(stdout,	"<%s> レイヤをオープン\n", e_sMesh_Layer.c_str());
	}

	// 2002.12.09 データがある部分のメッシュリスト
	eDigit	= a_iMDigit;
	return	( true );
}

/** ------------------------------------------------------------------------------
	メッシュ矩形をGeoDBから取得する \
	※GeoDBへの接続が確立されていることが前提
--------------------------------------------------------------------------------*/
IPolygonPtr		main_rootin::
get_mesh_rect	( int	c_iMeshCode	)	///< メッシュコード
{

	CComBSTR	a_SpatialCol;
	e_ipF_Class->get_ShapeFieldName	( &a_SpatialCol );
	BSTR		a_Where_Str;
	char		a_tmpstr[30];	//　ここはなんとかしたい気がする

	crd_cnv		crd;
	int			a_iDigit	= 0;
	a_iDigit	= crd.GetMeshDigit( c_iMeshCode );
	if(a_iDigit == 4) {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// メッシュコードを文字列に
		eDigit	= 4;
	}else 
	if(a_iDigit == 6) {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);			// メッシュコードを文字列に
		eDigit	= 6;
	}else {
		sprintf(a_tmpstr, "MESHCODE = %d", c_iMeshCode);		// メッシュコードを文字列に
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

			return	( a_Clip_Shp );
			
		/*	VARIANT_BOOL	a_bResult;
			a_Clip_Shp->get_IsEmpty( &a_bResult );
			if(a_bResult == VARIANT_TRUE) {	// クリップ結果がNULLならメッシュ外
				continue;
			}
			if( FAILED(a_Clip_Shp->get_Envelope( &a_ipMesh_Rect) )) {
				fprintf	( stderr, "該当メッシュ<%d>の矩形がみつかりませんでした\n", c_iMeshCode);
				return	( NULL );
			}else {
				return	( a_ipMesh_Rect );
			}*/
		}
	}
	return	( a_ipMesh_Rect );
}

/** ------------------------------------------------------------------------------
	矩形のラインを
	@return	ポリライン
--------------------------------------------------------------------------------*/
IPolylinePtr	main_rootin::
Get_Rect_Line	(	IEnvelopePtr	c_ipEnv	)		///< メッシュ矩形
{
	//
	// 2002.07.01 Intersect用のジオメトリを作成
	IPolylinePtr		a_ipRectLine(CLSID_Polyline);	// 矩形用ジオメトリ
	IPointCollectionPtr	a_ipRecPts	(a_ipRectLine);		// 矩形用点列

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

	// シンプリファイをかける
	ITopologicalOperator2Ptr	a_ipTopo;
	a_ipTopo		= a_ipRectLine;

	if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
		fprintf	( e_fpErr_Fp, "Simplifyに失敗\n");
		return	( NULL );
	}
	if(FAILED ( a_ipRectLine->SimplifyNetwork() ) ) {		// 2003.06.13 こっちじゃないとだめなことが判明
		fprintf	( e_fpErr_Fp,	"SimplifyNetworkに失敗" );	// エラーメッセージ
		return	( NULL );
	}
	return ( a_ipRectLine );
}

/** ------------------------------------------------------------------------------
	矩形のポリゴンを
	@return	IPolygonPtr
--------------------------------------------------------------------------------*/
IPolygonPtr	main_rootin::
Get_Rect_Polygon	(	IEnvelopePtr	c_ipEnv	)		///< メッシュ矩形
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

	// シンプリファイをかける
	ITopologicalOperator2Ptr	a_ipTopo;

	a_ipTopo	= a_ipRectPoly;

	if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
		fprintf	( e_fpErr_Fp, "Simplifyに失敗\n");
		return	( NULL );
	}
	if(FAILED ( a_ipRectPoly->SimplifyPreserveFromTo() ) ) {
		fprintf	( e_fpErr_Fp,	"SimplifyPreserveFromToに失敗" );	// エラーメッセージ
		return	( NULL );
	}
	return ( a_ipRectPoly );
}


/** ------------------------------------------------------------------------------
	クリップ図形を返す
--------------------------------------------------------------------------------*/
IGeometryPtr	main_rootin::
Get_Clip_Obj	(	IGeometryPtr	c_ipGeo,		///< ジオメトリ
					IGeometryPtr	c_ipClip	)	///< これでクリップ
{

	IGeometryPtr			a_ipClip_Shp;		// こいつにデータをコピーしてクリップする
	ITopologicalOperatorPtr	a_ipClip_Topo;
	a_ipClip_Topo				= c_ipGeo;
		
	if( FAILED( a_ipClip_Topo->Intersect (c_ipClip, esriGeometryNoDimension, &a_ipClip_Shp)) ) {
		fprintf	( e_fpErr_Fp,"シェイプのクリップに失敗" );	// エラーメッセージ
		return	( NULL );
	}
	
	VARIANT_BOOL	a_bResult;
	a_ipClip_Shp->get_IsEmpty( &a_bResult );
	if(a_bResult == VARIANT_TRUE) {	// クリップ結果がNULLならメッシュ外
		return	( NULL );
	}

	return	( a_ipClip_Shp );

}

/** ------------------------------------------------------------------------------
	長さの計算
	@return	正規化座標値での距離	
--------------------------------------------------------------------------------*/
double	main_rootin::
Get_Length	(	int			c_iPtCount,	// ポイント数
				Norm_Pt*	c_ipPts	)	// データカウント
{

	double	a_dLength	= 0.0;

	int		i = 0;
	for( i = 0; i < c_iPtCount-1; i++ )
	{
		double	a_iDiff_X	= c_ipPts[i+1].e_iX - c_ipPts[i].e_iX;
		double	a_iDiff_Y	= c_ipPts[i+1].e_iY - c_ipPts[i].e_iY;
		a_dLength	+= sqrt ( a_iDiff_X * a_iDiff_X + a_iDiff_Y * a_iDiff_Y );
	}
	return	( a_dLength );	
}

/** ------------------------------------------------------------------------------
	ポリゴンの面積
--------------------------------------------------------------------------------*/
double main_rootin::
Get_Real_Area	(	int			c_count,	// 補間点数
					Norm_Pt*	c_ipPts,	// IN点列
					int			c_meshcode)	// メッシュコード
{
	double		lon		= 0.0;
	double		lat		= 0.0;
	double		xratio	= 0.0;
	double		yratio	= 0.0;

	crd_cnv		crd;

	crd.MeshtoLL(c_meshcode, 0, 0, &lon, &lat);
	xratio	= RA * sin(M_PI / 2.0 - lat * M_PI / 180.0) * 2.0 * M_PI / 23040.0 / 1000000.0; // 23040 = 360*64
	yratio	= RA * 2.0 * M_PI / 34560.0 / 1000000.0;
	return(calc_area(c_count, c_ipPts, xratio, yratio));
}

/** ------------------------------------------------------------------------------
	ポリゴンの面積
--------------------------------------------------------------------------------*/
double main_rootin::
calc_area	(	int			c_count,	// 補間点数
				Norm_Pt*	c_ipPts,	// IN点列
				double		c_xratio,	// x方向比率?
				double		c_yratio)	// x方向比率?
{
	double	S		= 0.0;
	int		i;
	int		s		= 0;
	int		e		= 0;
	for(i = 0; i < c_count - 1; i++)
	{
		s	= (i == 0) ? c_count-2: i-1;
		e	= (i == c_count-2) ? 0: i+1;
		S	+=((c_ipPts[s].e_iX - c_ipPts[e].e_iX) * c_xratio * c_ipPts[i].e_iY * c_yratio);
	}
	S /= (2.0);
	return(fabs(S));
}

/** ------------------------------------------------------------------------------
	情報の追加
	@return	無し
--------------------------------------------------------------------------------*/
void	main_rootin::
Put_Info	(	ms_data_info&	c_mInfos,
				data_info		c_dInfo	)
{
	ims_data_info	a_iIndx;

	a_iIndx	= c_mInfos.find( c_dInfo );
	if( a_iIndx == c_mInfos.end() ) {	// 該当データが見つからない場合は追加
		c_mInfos.insert ( c_dInfo );
	}else {
		data_info	a_dTmp_Dat	= *a_iIndx;
		a_dTmp_Dat.e_dSum	+= c_dInfo.e_dSum;	// 距離を追加する
		// いったんデータセットからデータを削除する
		c_mInfos.erase	( a_iIndx );
		// 距離を修正したデータを追加する
		c_mInfos.insert	( a_dTmp_Dat );
	}
}

/** ------------------------------------------------------------------------------
	情報の出力(集計)
	@return	無し
--------------------------------------------------------------------------------*/
void	main_rootin::
Print_Info	(	ms_data_info&	c_mInfos,		///< 出力情報
				FILE*			c_fpOutFp)		///< 出力ファイルポインタ
{
	ims_data_info	a_iIndx;
	for( a_iIndx = c_mInfos.begin(); a_iIndx != c_mInfos.end(); a_iIndx++ )
	{
		data_info	a_dDat	= *a_iIndx;

		fprintf	( c_fpOutFp, "%s\t%s\t%0.10f\n",
							a_dDat.e_sAddrCode.c_str(),
							a_dDat.e_sChimei.c_str(),
							a_dDat.e_dSum );
	}
}

/** ------------------------------------------------------------------------------
	情報の出力(単発)
	@return	無し
--------------------------------------------------------------------------------*/
void	main_rootin::
Print_Info	(	int				c_iMeshCode,	///< メッシュコード[IN]
				data_info&		c_mInfo,		///< 出力情報[IN]
				FILE*			c_fpOutFp)		///< 出力ファイルポインタ[IN]
{
	fprintf	( c_fpOutFp, "%d\t", c_iMeshCode );
	vector<CComVariant>::iterator	a_iIndx;
	for( a_iIndx = c_mInfo.e_vcAttrs.begin(); a_iIndx != c_mInfo.e_vcAttrs.end(); a_iIndx++ )
	{
		string	a_sTmpStr;
		switch ((*a_iIndx).vt)
		{
		case	VT_NULL:
			fprintf	( c_fpOutFp, "NULL" ); break;
		case	VT_I2:
		case	VT_I4:
		case	VT_INT:
		case	VT_UINT:
			fprintf	( c_fpOutFp, "%d", (*a_iIndx).intVal ); break;
		case	VT_R4:
		case	VT_R8:
			fprintf	( c_fpOutFp, "%0.15f", (*a_iIndx).dblVal ); break;
		case	VT_BSTR:
			a_sTmpStr	= e_cCU.Str_to_SJIS ((CComBSTR&)(*a_iIndx).bstrVal, common_use::UNICODE);
			fprintf	( c_fpOutFp, "%s", (char*)a_sTmpStr.c_str() );
			break;
		case	VT_DATE:
			{
			UDATE	a_uDate;
			VarUdateFromDate( (*a_iIndx).date, 0, &a_uDate );
			fprintf	( c_fpOutFp, "%d/%02d/%02d %02d:%02d:%02d",
				a_uDate.st.wYear, a_uDate.st.wMonth, a_uDate.st.wDay,
				a_uDate.st.wHour, a_uDate.st.wMinute, a_uDate.st.wSecond );
			}
			break;
		default:
			fprintf	( c_fpOutFp, "<未対応の型>" );
			break;
		}
		fprintf	( c_fpOutFp, "\t");
	}
	fprintf	( c_fpOutFp, "%0.10f\n", c_mInfo.e_dSum );
	fflush	( c_fpOutFp );
}

/**
	点列にする
*/	/*
bool	main_rootin::
Get_Norm_Ps	(	IGeometryPtr		c_ipGeo,	///< ジオメトリ[IN]
			 	int					c_iMeshCode,///< メッシュコード[IN]
				int					c_iMode,	///< 0=line,1=Polygon
				vector<Norm_Pt>&	c_vPts	)	///< 点列[OUT]
{
	HRESULT	hr;

	bool		a_iIsClockwise	= true;

	IPointCollectionPtr	a_ipPtCol(c_ipGeo);
	long	a_lCount	= 0;
	a_ipPtCol->get_PointCount ( &a_lCount );

	// WKSPointで取得する
	WKSPoint*	a_wpPoints	= NULL;

	a_wpPoints	= new WKSPoint[a_lCount];	// 点列の確保
	hr	= a_ipPtCol->QueryWKSPoints ( 0, a_lCount, a_wpPoints );
	if ( FAILED( hr ) ){
		fprintf	( stderr, "WKSPoint取得に失敗" );
		return	( true );
	}

	if( c_iMode == 1 ) {

		list<RING>				a_lRings;

		// 初期化する
		InitRing( a_lCount, a_wpPoints, a_lRings );
		// 点列の包含関係をとってくる
		CheckExteriorInterior( a_wpPoints, a_lRings );

		// リングの状態を確認
		RING	a_rForChk	= *a_lRings.begin();
		if( a_rForChk.IsClockwise == TRUE ) {
			a_iIsClockwise	= true;
		}else {
			a_iIsClockwise	= false;
		}
	}

	crd_cnv	a_cCrd;
	
	int	i = 0;
	Norm_Pt	a_nPt;
	for( i = 0; i < a_lCount; i++ )
	{
		int	a_iX = 0, a_iY = 0;
		a_cCrd.LLtoMesh2( a_wpPoints[i].X, a_wpPoints[i].Y, c_iMeshCode, &a_nPt.e_iX, &a_nPt.e_iY);
		c_vPts.push_back( a_nPt );
	}
	delete [] a_wpPoints;
	return	( a_iIsClockwise );
}
*/
/**
	オブジェクトの面積及びアーク長を返す
*//*
double	main_rootin::
Get_Obj_LA	(	IGeometryPtr	c_ipGeo,		///< ジオメトリ[IN]
				int				c_iMeshCode,	///< メッシュコード[IN]
				int*			c_iType	)		///< データタイプ 1=line, 2=polygon
{
	double	a_dRet_Value	= 0.0;	// 算出結果

	// ジオメトリタイぷを求める
	esriGeometryType			a_eType;
	c_ipGeo->get_GeometryType ( &a_eType );

	//　内部ジオメトリの数を得る
	long	a_lGeoNum	= 0;
	IGeometryCollectionPtr	a_ipGeoCol ( c_ipGeo );

	a_ipGeoCol->get_GeometryCount ( &a_lGeoNum );
	int	i = 0;
	if( a_eType == esriGeometryPolyline ||
		a_eType == esriGeometryPolygon  )
	{
		bool	a_iIsClockwise	= 0;
		double	a_dCur_Val		= 0.0;
		vector<Norm_Pt>	a_vLinePts;
		for( i = 0; i < a_lGeoNum; i++ )
		{
			IGeometryPtr	a_ipCurGeo;
			a_ipGeoCol->get_Geometry( i, &a_ipCurGeo );
			// 正規化点列にする
			
			if( a_eType == esriGeometryPolyline ){
				Get_Norm_Ps	(a_ipCurGeo, c_iMeshCode , 0, a_vLinePts);	///< 点列[OUT]
				a_dRet_Value += Get_Length		( a_vLinePts.size(), &a_vLinePts[0] );
			}else {
				a_iIsClockwise	= Get_Norm_Ps	(a_ipCurGeo, c_iMeshCode , 1, a_vLinePts);	///< 点列[OUT]
				a_dCur_Val	= Get_Real_Area	( a_vLinePts.size(), &a_vLinePts[0], c_iMeshCode );
				if( a_iIsClockwise != true ) {
					a_dCur_Val*=-1.0;
				}
				a_dRet_Value += a_dCur_Val;
			}
			a_vLinePts.clear();
		}
		if( a_eType == esriGeometryPolyline ){
			*c_iType	= 1;
		}else {
			*c_iType	= 2;
		}
	}else {
		*c_iType	= 0;
	}
	return	( a_dRet_Value );
}
*/

/**
	面積値を求める
*/
double main_rootin::
CalcArea(	IGeometryPtr	a_ipSrc_Geo,		///< クリップ前のジオメトリ
			IGeometryPtr	a_ipCliped_Geo	)	///< クリップ後のジオメトリ
{
	const double	EARTH_R	= 6370.0;

	IAreaPtr	a_ipSrcArea( a_ipSrc_Geo );
	IAreaPtr	a_ipCliped_Area( a_ipCliped_Geo );
	double a_dArea	= 0.0;
	a_ipCliped_Area->get_Area(&a_dArea);
	if(a_dArea > 0.0){

		// クリップ前のデータから代表点を求める
		IPointPtr	ipPoint(CLSID_Point);
		a_ipSrcArea->QueryCentroid(ipPoint);

		double aY;
		ipPoint->get_Y(&aY);
		double aRatio = EARTH_R * sin(M_PI / 2.0 - aY * M_PI / 180.0) * 2.0 * M_PI/360.0;
		a_dArea *= (aRatio * EARTH_R * 2.0 * M_PI / 360.0);
	}else{
		a_dArea = 0.0;
	}
	return a_dArea;
}

/**
	フィーチャから属性を取り出す
*/
void	main_rootin::
GetFAttrs	(	Feature_All_Attrs&	c_fAttrs,	///< 属性値[IN]
				data_info&			c_dInfo	)	///< 取り出した情報[OUT]
{
	vector<string>::iterator	a_iIndx;
	for( a_iIndx = e_vsGetField.begin(); a_iIndx != e_vsGetField.end(); a_iIndx++ )
	{
		// 属性取り出し
		Feature_Attr	a_fAtt;
		a_fAtt	= c_fAttrs.Get_Attr ( (*a_iIndx).c_str() );
		c_dInfo.e_vcAttrs.push_back( a_fAtt.e_ccvAttr );
	}
}

/** ------------------------------------------------------------------------------
	チェック実行部
	※内部でエラーファイルの処理等も行う
	@return	成功	true
	@return 失敗	false
--------------------------------------------------------------------------------*/
bool	main_rootin::
check_exe	(	IWorkspacePtr	c_ipWSpace )	///< ワークスペース
{
	HRESULT		hr;
	crd_cnv		crd;
	// ログファイルのオープン
	if( !Open_Err_Log () ) {
		return	( false );
	}

	// ログヘッダの出力
	Output_Log_Header();

	// SiNDY用のログ
	smp_sindy_err_info	a_sPrnInfo;
	a_sPrnInfo.Init		( e_fpErr_Fp );

	mk_input_list	a_mChk_List;

	// メッシュリスト関係の初期化
	if( !get_mesh_init( a_mChk_List , c_ipWSpace ) ) {
		fprintf	( stderr, "メッシュレイヤの獲得の初期化に失敗[check_exe]\n");
		return	( false );
	}
	
	// チェックするレイヤ名の獲得
	char*			a_cpCheck_Layer	= NULL;
	a_cpCheck_Layer	= (char*)e_sCheck_Layer.c_str();		// 都市地図モードの場合

	if( a_cpCheck_Layer == NULL ) {
		fprintf	( stderr, "チェックレイヤ名がとりだせませんでした\n");
		return	( false );
	}

	// レイヤの読み込み
	msi_handle_ws	a_mHandle_WS;
	if( !a_mHandle_WS.Init ( c_ipWSpace, a_cpCheck_Layer) ) {
		fprintf	( stderr, "チェックレイヤ<%s>の読み込みに失敗[check_exe]\n", a_cpCheck_Layer );
		return	( false );		
	}

	// キャッシュ上にとってくる
	// オンメモリテスト 2002.6.24
	ISpatialCacheManagerPtr		a_ipSpCacheMgr( c_ipWSpace );
	char	a_cpPrnBuf[512];
	int		a_iCur_Mesh;
	ms_data_info	a_mData_Info;
	for( a_iCur_Mesh = a_mChk_List.get_mesh_first(); a_iCur_Mesh != -1; a_iCur_Mesh = a_mChk_List.get_mesh_next() )
	{
		/// データ作成開始時間
		DWORD				a_dMesh_Begin_Time	= 0;
		a_dMesh_Begin_Time	= timeGetTime();

		// メッシュごとにキャッシュマネージャをクリアしておく
	/*	hr	= a_ipSpCacheMgr->EmptyCache( );
		if( FAILED ( hr ) ) {
			fprintf	( stderr, "キャッシュマネージャをクリアできませんでした\n");
			continue;
		}*/

		fprintf	( stdout, "メッシュコード [%d] チェック中\n", a_iCur_Mesh );
	
		// メッシュ矩形の取り出し
		IPolygonPtr		a_ipRect;
		a_ipRect		= get_mesh_rect( a_iCur_Mesh );
		if( a_ipRect == NULL ) {	// メッシュ矩形が無い場合
			sprintf	( a_cpPrnBuf, "メッシュコード<%d>の矩形が見つかりませんでした\n", a_iCur_Mesh );
			a_sPrnInfo.Prn_Info	( a_cpPrnBuf, stderr );
			continue;
		}
	
		// 空間キャッシュマネージャにメッシュ内のデータをとりだす
	//	a_ipSpCacheMgr->FillCache( a_ipRect );
	
		IPolygonPtr			a_ipRect_Poly;
	//	a_ipRect_Poly		= Get_Rect_Polygon	( a_ipRect );
		a_ipRect_Poly		= a_ipRect;
		if( a_ipRect_Poly == NULL ) {
			sprintf	( a_cpPrnBuf, "<%d>メッシュ矩形ポリゴン作成に失敗\n", a_iCur_Mesh );
			a_sPrnInfo.Prn_Info	( a_cpPrnBuf, stderr );
			continue;
		}
		IGeometryPtr		a_ipRect_Poly_Geo;
		a_ipRect_Poly_Geo	= a_ipRect_Poly;

		// メッシュ矩形の境界線でサーチする
		IFeatureCursorPtr	a_ipDatas;
		a_ipDatas	= a_mHandle_WS.Search_Feature	( a_ipRect_Poly_Geo, esriSpatialRelIntersects, e_sWhere_Str, (RET_COUNT|RET_FCURSOR) );
		if (a_ipDatas == NULL ) {
			sprintf	( a_cpPrnBuf, "<%d>メッシュ矩形でデータが取れません\n", a_iCur_Mesh );
			a_sPrnInfo.Prn_Info	( a_cpPrnBuf, stderr );
			continue;
		}

		long		a_lTCount	= 0;
		a_lTCount	= a_mHandle_WS.Get_Data_Count ();

		//-----------<チェック進行度表示>--------------------
		long		i			= 0;
		fprintf	( stdout,	"メッシュコード [%d] 総データ件数 %d 件\n", a_iCur_Mesh, a_lTCount );
		//--------------------------------------------------

		IFeaturePtr	a_ipCurFeat;
		for ( ; ; i++)
		{
			//　メッシュで引っ掛けてきたフィーチャ
			if( a_ipDatas->NextFeature ( &a_ipCurFeat ) == S_OK) {

				// フィーチャの属性を取り出す
				Feature_All_Attrs	a_fFeat_Attrs;
				a_fFeat_Attrs.Init	( a_ipCurFeat);
			
				// ジオメトリを取得する
				IGeometryPtr			a_ipCurGeo;			// 現在着目中のジオメトリ

				a_ipCurGeo	= a_fFeat_Attrs.Ret_Geo();
				if( a_ipCurGeo == NULL ) {
					sprintf	( a_cpPrnBuf, "ジオメトリのコピーが取得できない[check_exe]\n");
					a_sPrnInfo.Prn_Info	( a_cpPrnBuf, stderr );
					continue;
				}

				//---------------------------------------------------------------------------------------
				// クリップする
				IGeometryPtr	a_ipCliped;
				a_ipCliped		= Get_Clip_Obj(	a_ipCurGeo,	a_ipRect_Poly_Geo );

				if( a_ipCliped	== NULL ) {
				//	sprintf	( a_cpPrnBuf, "クリップ結果がNULL\n");
				//	a_sPrnInfo.Prn_Info	( a_cpPrnBuf, stderr );				
					continue;
				}

				// 面積・アーク長計算する
				double	a_dArea			= CalcArea ( a_ipCurGeo, a_ipCliped );
				//---------------------------------------------------------------------------------------

				if( eExe_Flag & ON_FIELDS ) {
					data_info		a_dTmp;
					a_dTmp.e_dSum	= a_dArea;
					GetFAttrs		( a_fFeat_Attrs, a_dTmp );
					Print_Info		( a_iCur_Mesh, a_dTmp, e_fpOut_Fp );
				}else {
					// 属性取り出し
					Feature_Attr	a_fAtt;
					a_fAtt	= a_fFeat_Attrs.Get_Attr ( "CITYCODE" );
					if( a_fAtt.e_ccvAttr.vt == VT_EMPTY ) {
						sprintf	( a_cpPrnBuf, "CITYCODEの情報が取得できません\n");
						a_sPrnInfo.Prn_Info	( a_cpPrnBuf, stderr );	
						return	( false );
					}

					string	a_sTmpCode5;
					a_sTmpCode5	= e_cCU.Str_to_SJIS ((CComBSTR&)a_fAtt.e_ccvAttr.bstrVal, common_use::UNICODE);

					// セットに追加
					addr_dat				a_aOraInfo;
					e_aAddr_DB_Hndl.GetOraDBStr((char*)a_sTmpCode5.c_str(), a_aOraInfo);
				
					// 政令指定市対応
					data_info	a_dObj_Info;
					addr_dat	a_aWardtoCity;
					a_dObj_Info.e_dSum		= a_dArea;
					if( eExe_Flag & ON_KU_MODE && e_aAddr_DB_Hndl.GetCitybyWard( a_aOraInfo, a_aWardtoCity ) ) {
						a_dObj_Info.e_sAddrCode	= a_aWardtoCity.e_sCode;
						a_dObj_Info.e_sChimei	= a_aWardtoCity.e_sShi_Kanji;
					}else {
						a_dObj_Info.e_sAddrCode	= a_sTmpCode5;
						a_dObj_Info.e_sChimei	= a_aOraInfo.e_sShi_Kanji;
					}
					// 郡名が付く場合はカットする
					if( !(eExe_Flag & ON_GUN_MODE) ) {
						string	a_sCutStr;
						long	a_lPos	= 0;
						a_lPos	= a_dObj_Info.e_sChimei.find ( "郡　" );
						if( a_lPos != a_dObj_Info.e_sChimei.npos ) {	// 全角スペースで検索
							long	a_lLen	= 0, a_lSubLen = 0;
							a_lLen	= a_dObj_Info.e_sChimei.length();
							a_lSubLen	= a_lLen - (a_lPos+4);
							if(a_lSubLen > 0) {
								a_sCutStr	= a_dObj_Info.e_sChimei.substr ( a_lPos+4, a_lSubLen);
								a_dObj_Info.e_sChimei	=a_sCutStr;
							}
						}
					}
					// 登録用データ
					Put_Info	( a_mData_Info,	a_dObj_Info	);
				}

				//--------------------------------------------<ここまで>
			}else {
				break;
			}
		
			//----------------
			if( i > 0 && (i % 100) == 0) {	// 
				// カウント100件ごとにメッセージを表示
				PrnMsg( stdout,	"メッシュコード [%d]  %d 件中　%d 件 チェック終了 \n", a_iCur_Mesh, a_lTCount, i );
			}
			//----------------
		}

		//--------------------------------------------<ここまで>
		fflush ( e_fpErr_Fp );	// データをフラッシュする
		//----------------
		PrnMsg	( stdout, "メッシュコード [%d] チェック終了\n", a_iCur_Mesh );
		
		DWORD			a_dMake_Finish	= 0;
		a_dMake_Finish	= timeGetTime();

		sprintf	( a_cpPrnBuf,	"<%d> : チェック時間 = %0.2f(s)でした\n",
							a_iCur_Mesh, (double)((a_dMake_Finish - a_dMesh_Begin_Time) / 1000.0) );

		a_sPrnInfo.Prn_Info_Std	( a_cpPrnBuf, stdout );
		//-----------------
	}
	// 市区町村単位で出力する
	if( eExe_Flag & ON_ADDRDB ) {
		// データ出力
		Print_Info	( a_mData_Info, e_fpOut_Fp );		///< 出力ファイルポインタ
		a_mData_Info.clear();
	}
	return	( true );
}
