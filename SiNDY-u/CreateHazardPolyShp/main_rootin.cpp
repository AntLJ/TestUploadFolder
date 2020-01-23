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
#include "PointRotate.h"
#include <sindy/workspace.h>


/** ------------------------------------------------------------------------------
	SiNDYDBへの接続オープン
	@return	true	成功
	@return	false	失敗
--------------------------------------------------------------------------------*/
bool	main_rootin::
SiNDY_DB_Open	(	IWorkspacePtr&			c_ipWorkspace_Note	)
{

	// 注記用DB
	m_sPrnInfo.Prn_Info			( "#災害ポリゴンデータ：\t", stdout );
	if( !DB_Open	( c_ipWorkspace_Note,
					  m_pTool_Init_NP.get_UserName(), m_pTool_Init_NP.get_Password(),
					  m_pTool_Init_NP.get_Version(), m_pTool_Init_NP.get_Instance(),
					  m_pTool_Init_NP.get_Server() )) {
		m_sPrnInfo.Prn_Info	( "接続出来なかった\n", stderr );
		return	( false );
	}else {
		PrnMsg	( stdout, "接続完了\n" );
	}	
	return	( true );
}

/** ------------------------------------------------------------------------------
	DBへの接続オープン
	@return	true	成功
	@return	false	失敗
--------------------------------------------------------------------------------*/
bool	main_rootin::
DB_Open	(	IWorkspacePtr&			c_ipWorkspace,
			char*					c_cpUserName,		///< ユーザ名
			char*					c_cpPassword,		///< パスワード
			char*					c_cpVersion,		///< バージョン
			char*					c_cpInstance,		///< インスタンス
			char*					c_cpServer )		///< サーバ名
{
	CString csUserName = c_cpUserName;
	CString csPassword = c_cpPassword;
	CString csVersion  = c_cpVersion;
	CString csInstance = c_cpInstance;
	CString csServer   = c_cpServer;
	CString csConnStr;

	// 接続文字列作成
	csConnStr.Format(_T("%s/%s/%s/%s/%s"), csUserName, csPassword, csVersion, csInstance, csServer);

	char	a_cpPrnMsg[512];

	sprintf	( a_cpPrnMsg,	"#接続プロパティ：%s/%s/%s/%s/%s\n",
							c_cpUserName, c_cpPassword, c_cpVersion, c_cpInstance, c_cpServer );

//	printf	( "%s", a_cpPrnMsg );

	m_sPrnInfo.Prn_Info			( a_cpPrnMsg, stdout );

	// DBへ接続
	printf("接続中...");
	c_ipWorkspace = NULL;
	c_ipWorkspace = sindy::create_workspace(csConnStr);
	if (!c_ipWorkspace) {
		return	( false );
	}
	return	(true);
}

/** ------------------------------------------------------------------------------
	エラーログファイルをオープンする
	@return	true	オープン成功
	@return false	オープン失敗
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Err_Log	( void )
{
	if( m_sLog_File == "" ) {
		return	( true );
	}else {
		m_fErr_Fp	= fopen ( (char*)m_sLog_File.c_str(), "w" );
		if( m_fErr_Fp == NULL ) {
			PrnMsg	( stderr, "エラーログファイル<%s>がオープンできない[main_rootin]\n", m_sLog_File.c_str() );
			return	( false );
		}
	}
	return	( true );
}

#define	OPTION_MAX	15
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
	// ツールバージョン表示 2009.08.20
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
			PrnMsg	( stderr, "引数文字列が長すぎます!!\n" );
			return	( false );
		}
	}

	if(argc < 2) {
		return( false );
	}

	// オプションだから、今のところ使用しない
	for(i = 1; i < argc;)
	{
		if(strcmp(argv[i], "-hpi") == 0) {	// 災害エリア初期化情報
			if(argc < i+2) return( false );
			m_sInit_Str_HZ_AREA	= argv[i+1];
			a_exeflag			|= ON_INIT_HZ_AREA;
			a_optcount[0]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-hpf") == 0) {	// 災害エリアフィーチャクラス指定
			if(argc < i+2) return( false );
			m_sHZ_AREA_FC		= argv[i+1];
			a_exeflag			|= ON_FC_HZ_AREA;
			a_optcount[1]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-mi") == 0) {	// 災害エリア初期化情報
			if(argc < i+2) return( false );
			m_sInit_Str_MESH	= argv[i+1];
			a_exeflag			|= ON_INIT_MESH;
			a_optcount[2]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-mf") == 0) {	// メッシュフィーチャクラス指定
			if(argc < i+2) return( false );
			m_sMesh_FC			= argv[i+1];
			a_exeflag			|= ON_FC_MESH;
			a_optcount[4]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-l") == 0) { // ログファイル設定（設定が無い場合は、エラーはstderrへ垂れ流しになる）
			if(argc < i+2) return( false );
			m_sLog_File	= argv[i+1];	///< メッシュコードリストファイル
			a_exeflag		|= ON_LOGFILE;
			a_optcount[5]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-v") == 0) {
			print_version();
			a_optcount[6]++;
			i+=1;
			return	( false );	// 仮にこうする？
		}else
		if(strcmp(argv[i], "-m") == 0) { // 2002.9.24
			if(argc < i+2) return( false );
			m_sMeshCode_File	= argv[i+1];	///< メッシュコードリストファイル
			a_exeflag		|= ON_MESH;
			a_optcount[7]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-M") == 0) { // 2003.4.1
			if(argc < i+2) return( false );
			a_exeflag		|= ON_MONO_MESH;
			m_iMeshCode		= atoi(argv[i+1]);	
			a_optcount[8]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-pgdb") == 0) {
			a_exeflag		|= ON_PGDB_MODE;
			m_sPGDB_File	= argv[i+1];
			a_optcount[9]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-o") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_OUTPATH;
			m_sOutPath		= argv[i+1];	
			a_optcount[10]++;
			i+=2;
		} else {
			PrnMsg	(stderr, "オプション %s は間違ってます!!\n", argv[i]);
			return	( false );
		}
	}

	// オプション重複のチェック
	for(i = 0; i < OPTION_MAX; i++)
	{
		if(a_optcount[i] > 1) {
			PrnMsg	(stderr, "オプションの指定が重複しています!!\n");
			return	( false );
		}
	}

	if( !(a_exeflag & ON_MESH) && !(a_exeflag & ON_MONO_MESH) ) {
		PrnMsg	(stderr, "'-m'か'-M'でメッシュの指定をしてください。\n");
		return	( false );
	}

	if( !(a_exeflag & ON_FC_HZ_AREA) ) {		///< 注記フィーチャクラス
		PrnMsg	(stderr, "災害エリアのフィーチャクラスが指定されていません\n");
		return	( false );
	}
	if( !(a_exeflag & ON_INIT_MESH) ) {
		PrnMsg	(stderr, "メッシュの接続情報指定がされていません\n");
		return	( false );
	}
	if( !(a_exeflag & ON_FC_MESH) ) {		///< メッシュフィーチャクラス
		PrnMsg	(stderr, "メッシュのフィーチャクラスが指定されていません\n");
		return	( false );
	}
	if( !(a_exeflag & ON_OUTPATH) ) {		///< 出力パス
		PrnMsg	(stderr, "データ出力パスが指定されていません\n");
		return	( false );
	}
	if( !(a_exeflag & ON_PGDB_MODE) ) {		///< PGDB
		PrnMsg	(stderr, "災害エリアのPGDBファイルが指定されていません\n");
		return	( false );
	}
	// 初期化設定
//	if( !e_pTool_Init_Note.read_init_str ( e_cpInit_Str_HZ_AREA ) ) {
//		return	( false );	// メッセージは内部で出している
//	}

	// エラーログファイルのオープン	2006.05.17 実行情報も出力したいのでここへ移動
	if( !Open_Err_Log () ) {
		return	( false );
	}else {
		if( !m_sPrnInfo.Init(m_fErr_Fp) ) {
			return	( false );
		}
	}

	//--------------------------------------------------------------------------
	// 実行情報を出力 2009.09.30 追加
	PrnMsg( m_fErr_Fp, "#ツールバージョン                               : <%s>\n", m_sVersion.c_str() );
	if(a_exeflag & ON_PGDB_MODE) {
	PrnMsg( m_fErr_Fp, "#PGDBファイル                                   : <%s>\n", m_sPGDB_File.c_str() );
	}
	PrnMsg( m_fErr_Fp, "#災害エリアフィーチャクラス                     : <%s>\n", m_sHZ_AREA_FC.c_str() );
	PrnMsg( m_fErr_Fp, "#メッシュフィーチャクラス用DB接続情報           : <%s>\n", m_sInit_Str_MESH.c_str() );
	PrnMsg( m_fErr_Fp, "#メッシュフィーチャクラス                       : <%s>\n", m_sMesh_FC.c_str() );
	if( a_exeflag & ON_MESH ) {
	PrnMsg( m_fErr_Fp, "#変換対象メッシュリスト                         : <%s>\n", m_sMeshCode_File.c_str() );
	}else {
	PrnMsg( m_fErr_Fp, "#変換対象メッシュコード                         : <%02d>\n", m_iMeshCode );
	}
	PrnMsg( m_fErr_Fp, "#データ出力パス                                 : <%s>\n", m_sOutPath.c_str() );
	//--------------------------------------------------------------------------

	if( !m_pTool_Init_Mesh.read_init_str ( (char*)m_sInit_Str_MESH.c_str() ) ) {
		return	( false );	// メッセージは内部で出している
	}

	m_iExe_Flag	= a_exeflag;

	return		( true );
}

/** ------------------------------------------------------------------------------
// 使用法表示
--------------------------------------------------------------------------------*/
void		main_rootin::
print_usage	(char **argv)
{
	PrnMsg(	stdout,
				"=== 【災害エリアポリゴンShape変換ツール】(%s) ===\n"
				"<usage>\n"
				"%s <オプション>... \n"
				"\t<引数>\n"
				"\t      '-m'           : メッシュリストファイル名を指定(ex. -m index.base)\n"
				"\t      '-M'           : 単メッシュを指定(ex. -M 594141)\n"
				"\t      '-v'           : バージョン情報表示\n"
				"\t      '-hpi'         : 災害エリアポリゴンレイヤ接続情報 ex) -hpi ronly/ronly/SDE.DEFAULT/ruby2\n"
				"\t      '-hpf'         : 災害エリアフィーチャクラス指定 ex) -hpf HPAREA.City_Site\n"
				"\t      '-mi'          : メッシュフィーチャクラス接続情報 ex) -mi ronly/ronly/SDE.DEFAULT/ruby2\n"
				"\t      '-mf'          : メッシュフィーチャクラス指定  ex) -mf REFERENCE.CityMesh\n"
				"\t      '-l'           : ログファイル名を指定 ex) -l .\\test\\make.log\n"
				"\t      '-pgdb'        : PGDBファイル名を指定 ex) -pgdb .\\data\\test.mdb\n"
				"\t      '-o'           : データ出力パス ex) -pgdb .\\data\\Citymap\n"
				"\t                       ※指定したデータパス以下にメッシュのフォルダを自動作成し出力する。\n"
				"\t<補足>\n"
				"\t      ・'-m','-M'は同時指定できません\n"
			, m_sVersion.c_str(), argv[0]);
}

/** ------------------------------------------------------------------------------
	バージョン情報の表示
	@return		無し
---------------------------------------------------------------------------------*/
void	main_rootin::
print_version	( void )
{
	PrnMsg(	stdout,
			"=== 【災害エリアポリゴンShape変換ツール】(%s) ===\n"
				"\t<バージョン情報>\n"
				"\t2011/05/13        1.0.0.1            : 新規作成\n", m_sVersion.c_str()
			);
}

/** ------------------------------------------------------------------------------
	実行情報を出力する 2006.05.17
	@return	char*
--------------------------------------------------------------------------------*/
void	main_rootin::
Print_Check_Info	( void )
{
//	sprintf	( a_cpPrnMsg,	"#都市地図注記検索範囲	%0.2f(m)\n", e_dRange_City );
//	m_sPrnInfo.Prn_Info	( a_cpPrnMsg, stdout );
//	m_sPrnInfo.Prn_Info			( a_cpPrnMsg, stdout );
}

/** ------------------------------------------------------------------------------
	メッシュ矩形をDBから獲得するメソッドの初期化
	※get_mesh_rect をやる前に必ず呼ぶ
------------------------------------------------------------------------------==*/

bool	main_rootin::
get_mesh_init	(	mk_input_list&	c_mMList,			// 入力リストクラス
					IWorkspacePtr	c_ipWorkSpace	)	// 指定するワークスペース
{

	if( m_iExe_Flag & ON_MESH ) {
		if( m_sMeshCode_File == "" ) {
			PrnMsg( m_fErr_Fp, "メッシュコードリストファイル名がとりだせませんでした[check_exe]\n");
			return	( false );
		}	
		if( c_mMList.read_mesh_list ( (char*)m_sMeshCode_File.c_str() ) != 0 ) {	// ここでリストを読み込む
			PrnMsg( m_fErr_Fp, "メッシュコードリストファイル<%s>の読み込みに失敗[check_exe]\n", m_sMeshCode_File.c_str() );
			return	( false );
		}
	}else {
		if(c_mMList.read_mesh_list ( m_iMeshCode ) == -1) {
			PrnMsg( m_fErr_Fp, "メッシュ<%d>の設定に失敗しました\n", m_iMeshCode );
			return	( false );
		}
	}

	int			a_iMDigit		= 0;
	a_iMDigit	= c_mMList.get_mesh_digit ();

	IFeatureWorkspacePtr	a_F_Workspace	( c_ipWorkSpace );	// フィーチャワークスペース

	// レイヤのオープン
	if( FAILED(a_F_Workspace->OpenFeatureClass(CComBSTR((char*)m_sMesh_FC.c_str()), &m_ipF_Class )) ) {
		PrnMsg( m_fErr_Fp, "レイヤ <%s> のオープンに失敗\n", m_sMesh_FC.c_str());
		return	(false);
	}else {
		PrnMsg( stdout,	"<%s> レイヤをオープン\n", m_sMesh_FC.c_str());
	}

	// 2002.12.09 データがある部分のメッシュリスト
	m_iDigit	= a_iMDigit;

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
	m_ipF_Class->get_ShapeFieldName	( &a_SpatialCol );
	_bstr_t		a_Where_Str;
	char		a_tmpstr[30];	//　ここはなんとかしたい気がする

	crd_cnv		crd;
	int			a_iDigit	= 0;
	a_iDigit	= crd.GetMeshDigit( c_iMeshCode );
	switch (a_iDigit)
	{
	case	2:
	case	4:
	case	6:
	case	8:
		sprintf(a_tmpstr, "MESHCODE = %.2d", c_iMeshCode);		// メッシュコードを文字列に
		m_iDigit	= a_iDigit;
		break;
	default:
		return	( NULL );
		break;
	}

	a_Where_Str	= a_tmpstr;		// stringをBSTRに変換

	IQueryFilterPtr		a_Sp_Filter	( CLSID_QueryFilter );			// 空間検索フィルタ
//	a_Sp_Filter->put_SearchOrder	( esriSearchOrderAttribute );	// 属性検索優先
	a_Sp_Filter->put_WhereClause	( a_Where_Str);					// Where句の設定
	
	// クリップ矩形作成
	IEnvelopePtr	a_ipMesh_Rect;	// メッシュ矩形
	a_ipMesh_Rect	= NULL;

	// サーチしてくる
	IFeatureCursorPtr	a_F_Cursor;
	if( FAILED	(m_ipF_Class->Search	( a_Sp_Filter, VARIANT_FALSE, &a_F_Cursor )) ) {
		PrnMsg	(m_fErr_Fp, "%d\tデータの属性検索に失敗\n", c_iMeshCode);
		return	(NULL);
	}else {
		IFeaturePtr				a_Feature;
		while(a_F_Cursor->NextFeature ( &a_Feature ) == S_OK)
		{				
			IGeometryPtr		a_Clip_Shp;		// こいつにデータをコピーしてクリップする

			if( FAILED(a_Feature->get_ShapeCopy	( &a_Clip_Shp )) ) {	// フィーチャから
				PrnMsg( m_fErr_Fp, "%d\tシェイプのコピーに失敗\n", c_iMeshCode);
				continue;
			}
			
			VARIANT_BOOL	a_bResult;
			a_Clip_Shp->get_IsEmpty( &a_bResult );
			if(a_bResult == VARIANT_TRUE) {	// クリップ結果がNULLならメッシュ外
				continue;
			}
			if( FAILED(a_Clip_Shp->get_Envelope( &a_ipMesh_Rect) )) {
				PrnMsg( m_fErr_Fp, "該当メッシュ<%d>の矩形がみつかりませんでした\n", c_iMeshCode);
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
		PrnMsg( m_fErr_Fp, "%d\tSimplifyに失敗\n", m_iCurMeshCode);
		return	( NULL );
	}
	if(FAILED ( a_ipRectPoly->SimplifyPreserveFromTo() ) ) {
		PrnMsg( m_fErr_Fp,	"%d\tSimplifyPreserveFromToに失敗", m_iCurMeshCode );	// エラーメッセージ
		return	( NULL );
	}
	return ( a_ipRectPoly );
}

/** ------------------------------------------------------------------------------
	レイヤオープン
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Layers		( void )
{
	if( !m_mHZ_AREA_WS.Init ( m_ipWorkspace_HZ_AREA, (char*)m_sHZ_AREA_FC.c_str() ) ) {
		PrnMsg( stderr, "災害エリアフィーチャクラス<%s>の読み込みに失敗\n", m_sHZ_AREA_FC.c_str() );
		return	( false );
	}
	return	( true );
}

/** ------------------------------------------------------------------------------
	メッシュ単位ファイル名の作成
--------------------------------------------------------------------------------*/
std::string	main_rootin::
make_mesh_fname ( int		c_iMeshCode,	///< メッシュコード
				  int		e_iSubPathNum,	///< サブパスの階層
				  char*		c_cpSrcShpPath,	///< ソースシェイプパス
				  char*		c_cpAddStr,		///< ファイル名に追加する文字列
				  string&	c_sMkSubPath )	///< 生成されたサブディレクトリ
{
	std::string	a_sRetStr	= "";
	std::string	a_sSubDir	= "";

	char	a_sTmp[4096];
	char	a_sTmp2[4096];

	int	a_iDigit	= 0;
	a_iDigit	= m_cCrd.GetMeshDigit ( c_iMeshCode );

	if( a_iDigit == 2 ) {
		sprintf	( a_sTmp, "%s\\%.2d\\%.2d%s", c_cpSrcShpPath, c_iMeshCode, c_iMeshCode, c_cpAddStr );
		sprintf	( a_sTmp2,"%s\\%.2d", c_cpSrcShpPath, c_iMeshCode );
		c_sMkSubPath= a_sTmp2;	// パスも返すように
	}else	{
		char	a_sSubStr[256];
		memset	( a_sSubStr, '\0', 256 );
		switch ( e_iSubPathNum )
		{
		case	0:
			if( c_iMeshCode >= 10000000 ) {
				sprintf ( a_sSubStr, "\\%d", c_iMeshCode/10000 );
			}else
			if( c_iMeshCode >= 100000 ) {
				sprintf ( a_sSubStr, "\\%d", c_iMeshCode/100);				
			}
			break;
		case	1:
			sprintf ( a_sSubStr, "\\%.2d", c_iMeshCode );
			break;
		case	2:
			sprintf ( a_sSubStr, "\\%d\\%d", c_iMeshCode / 100, c_iMeshCode );
			break;
		case	3:
			sprintf ( a_sSubStr, "\\%d\\%d\\%d", c_iMeshCode / 10000, c_iMeshCode / 100, c_iMeshCode );
			break;
		default:
			break;
		}
		a_sSubDir	= a_sSubStr;

		sprintf	( a_sTmp2,"%s%s", c_cpSrcShpPath, (char*)a_sSubDir.c_str() );
		c_sMkSubPath= a_sTmp2;	// パスも返すように

		sprintf	( a_sTmp, "%s%s\\%d%s", c_cpSrcShpPath, (char*)a_sSubDir.c_str(), c_iMeshCode, c_cpAddStr );
	}

	a_sRetStr	= a_sTmp;

	return ( a_sRetStr );
}

/** ------------------------------------------------------------------------------
	ファイルのフルパス名から、シェイプファイル名を抽出する関数
	Function to extract the shape file name from the fullpath of the file
	@return	なし
--------------------------------------------------------------------------------*/
bool main_rootin::
GetFeatureClass	(	const std::string&	sourceFile,		///< ソースファイル名
					std::string&		className )		///< パス名
{
	std::string	tempName;
	std::string	temp;

	tempName	= sourceFile;

	char*	p1	= NULL;
	p1	= strrchr( (char*)tempName.c_str(), '\\');
	if( p1 == NULL ) {
		className	= tempName;	// '\'マークがみつからないのでパス名を含まない
	}else {
		className	= p1+1;
	}
	return	( true );
}

/** ------------------------------------------------------------------------------
	フルパス名から、ファイル名を除いて、ディレクトリ名を返す関数
	// Function to remove the file name from the fullpath and return the directory
	@return	なし
--------------------------------------------------------------------------------*/
bool	main_rootin::
GetFilePath	(	const std::string&	sourceFile,		///< ソースファイル名
				std::string&		filePath )		///< パス名
{

	std::string	temp;
	temp	= sourceFile;

	char*		pathEnd;
	pathEnd		= strrchr((char*)temp.c_str(), '\\');
	if( pathEnd == NULL ) {
		return	( false );
	}
	*pathEnd	= '\0';		// 文字を区切る
	
	filePath	= temp;
	return	( true );
}

/** ------------------------------------------------------------------------------
	データを読み込み、Shapeファイルに変換
	@return	成功	true
	@return 失敗	false
--------------------------------------------------------------------------------*/
bool	main_rootin::
Read_HZ_AREA_to_Shape	( void )
{
	IFeatureCursorPtr	a_ipHZ_Area_Cursor;

	// キャッシュ上にとってくる
	// オンメモリテスト 2002.6.24
	int		a_iCur_Mesh;
	for( a_iCur_Mesh = m_sInput_List.get_mesh_first(); a_iCur_Mesh != -1; a_iCur_Mesh = m_sInput_List.get_mesh_next() )
	{
		// メッシュごとにキャッシュマネージャをクリアしておく
/*	
		hr	= a_ipSpCacheMgr_CNote->EmptyCache( );
		if( FAILED ( hr ) ) {
			fprintf	( stderr, "都市地図注記用キャッシュマネージャをクリアできませんでした\n");
			continue;
		}
*/
		PrnMsg( stdout, "メッシュ[%d] データ取得\n", a_iCur_Mesh );

		m_iCurMeshCode	= a_iCur_Mesh;

		// メッシュ矩形の取り出し
		IGeometryPtr	a_ipMeshRect;
		a_ipMeshRect	= get_mesh_rect( a_iCur_Mesh );
		if( a_ipMeshRect == NULL ) {	// メッシュ矩形が無い場合
			PrnMsg( m_fErr_Fp, "%d\tのメッシュ矩形が見つかりませんでした\n", a_iCur_Mesh );
			continue;
		}
		
		IEnvelopePtr	a_ipRect;
		a_ipMeshRect->get_Envelope ( &a_ipRect );

		// 空間参照を取得する 2007.08.22
		ISpatialReferencePtr	a_ipSpRef;
		if( FAILED (a_ipRect->get_SpatialReference ( &a_ipSpRef )) ) {
			PrnMsg( m_fErr_Fp, "%d\t空間参照の取得に失敗\n", a_iCur_Mesh );
			continue;
		}

		// 空間キャッシュマネージャにメッシュ内のデータをとりだす
//		a_ipSpCacheMgr_CNote->FillCache( a_ipRect );
			
		a_ipHZ_Area_Cursor	= m_mHZ_AREA_WS.Search_Feature ( a_ipRect, esriSpatialRelIntersects, "", RET_FCURSOR );
		if ( a_ipHZ_Area_Cursor == NULL ) {
			m_sPrnInfo.Prn_Info	( "災害ポリゴンデータを取得できませんでした\n", stderr );
			return	( false );
		}
		PrnMsg( stdout, "メッシュコード<%d> 災害ポリゴンデータ読み込み中\n", a_iCur_Mesh );
		Set_HZ_AREA_Data	( a_ipMeshRect, a_ipHZ_Area_Cursor, m_mAll_Data, CITY_NOTE );

		// 出力ファイル名作成
		std::string	a_sOutFName;
		int	a_iMkLv	= 0;
		switch (m_iDigit)
		{
		case	2:	a_iMkLv	= 1; break;
		case	4:	a_iMkLv	= 1; break;
		case	6:	a_iMkLv	= 2; break;
		case	8:	a_iMkLv	= 3; break;
			break;
		}
		std::string	a_sOutPath;
		a_sOutFName	= make_mesh_fname ( a_iCur_Mesh, a_iMkLv, (char*)m_sOutPath.c_str(), "_DIS.shp", a_sOutPath );
		// データ出力パスを作成
		if( m_cUC.makedir ( (char*)a_sOutPath.c_str() ) == false ) {
			PrnMsg( m_fErr_Fp, "%d\tディレクトリ作成に失敗<%s>\n", a_iCur_Mesh, (char*)a_sOutPath.c_str() );
			continue;
		}

		HZ_Area_to_Shape (	a_ipSpRef, m_mAll_Data, (char*)a_sOutFName.c_str() );

		m_mAll_Data.clear();
	}
	return	( true );
}

/**
	形状クリップ
*/
IGeometryPtr	main_rootin::
Mk_Clip_Data ( IGeometryPtr&		c_ipClipPoly,	///< クリップ用ポリゴン
			   IGeometryPtr&		c_ipTgtGeo, 	///< クリップされる図形
			   int					c_iClipType,	///< クリップ条件 0=Intersect 0以外=Difference
			   int*					c_ipEmpty_F )	///< 空フラグ
{

	IFeaturePtr				a_Feature;
	long					a_Shape_Number	= 0;	// シェイプテーブルに追加するシェイプのInfo
	long					a_lSp_InitFlag	= 0;	// 空間参照を初期化したかどうかのフラグ
	long					a_lFCount		= 0;	// フィーチャのカウント

	IGeometryPtr		a_Src_Shp;	// 実質使わない？
	
	IGeometryPtr			a_ipCliped_Shp;		// こいつにデータをコピーしてクリップする
	ITopologicalOperatorPtr	a_ipClip_Topo;
	a_ipClip_Topo			= c_ipTgtGeo;
	
	if( c_iClipType == 0 ) {
		if( FAILED( a_ipClip_Topo->Intersect (c_ipClipPoly, esriGeometryNoDimension, &a_ipCliped_Shp)) ) {
			PrnMsg( m_fErr_Fp, "%d\tIntersect失敗\n", m_iCurMeshCode);
			return	( NULL );
		}
	}else {
		if( FAILED( a_ipClip_Topo->Difference (c_ipClipPoly, &a_ipCliped_Shp)) ) {
			PrnMsg( m_fErr_Fp, "%d\tDifference失敗\n", m_iCurMeshCode);
			return	( NULL );
		}
	}

	VARIANT_BOOL	a_bResult;
	a_ipCliped_Shp->get_IsEmpty( &a_bResult );
	if(a_bResult == VARIANT_TRUE) {	// クリップ結果がNULLならメッシュ外
		*c_ipEmpty_F	= 1;	
	//	fprintf( m_fErr_Fp,"クリップ結果が空\n");
		return	( NULL );
	}

	// 空間参照を取得する 2007.08.22
	ISpatialReferencePtr	a_ipSpRef;
	if( FAILED (c_ipTgtGeo->get_SpatialReference ( &a_ipSpRef )) ) {
		PrnMsg( m_fErr_Fp, "%d\t空間参照の取得に失敗\n", m_iCurMeshCode );
		return	( NULL );
	}

	// シンプリファイをかける
	ITopologicalOperator2Ptr	a_ipTopo;

	a_ipTopo	= a_ipCliped_Shp;

//	IGeometryEnvironment2Ptr	a_ipGeoEnv(CLSID_GeometryEnvironment);
//	if( FAILED ( a_ipGeoEnv->put_Pre81Compatibility( VARIANT_TRUE ) ) ) {// add
//		PrnMsg( m_fErr_Fp, "put_Pre81Compatibility\n" );
//	
//	}
	if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
		PrnMsg( m_fErr_Fp, "%d\tSimplifyに失敗\n", m_iCurMeshCode );
		return	( NULL );
	}

	IPolygonPtr	a_ipTmpPoly;
	a_ipTmpPoly = a_ipCliped_Shp;

	if(FAILED ( a_ipTmpPoly->SimplifyPreserveFromTo() ) ) {
		PrnMsg( m_fErr_Fp, "%d\tSimplifyに失敗\n", m_iCurMeshCode );	// エラーメッセージ
		return	( NULL );
	}
	// 空間参照の付与
	if( FAILED (a_ipCliped_Shp->putref_SpatialReference ( a_ipSpRef )) ) {
		PrnMsg( m_fErr_Fp, "%d\t空間参照の付与に失敗\n", m_iCurMeshCode );
		return	( NULL );
	}

	return ( a_ipCliped_Shp );
}

/** ------------------------------------------------------------------------------
	災害エリアデータをセットに落とす
	@return	データ数
--------------------------------------------------------------------------------*/
int		main_rootin::
Set_HZ_AREA_Data ( IGeometryPtr&		c_ipMeshRect,		///< メッシュ矩形
				   IFeatureCursorPtr&	c_ipNoteCursor, 	///< 注記カーソル
				   ms_data_info&		c_mHZ_Area_Data,	///< 災害ポリゴンデータ
				   int					c_iData_Type	)	///< データタイプ
{
	int	a_iCount	= 0;
	// データをマルチセットのデータに落とす
	if( c_ipNoteCursor != NULL ) {
		IFeaturePtr	a_ipNote_Feat;
		// 検索されたポリゴンのループ
		ms_data_info	a_mTmp_HZ_AREA_Data;
		while(c_ipNoteCursor->NextFeature ( &a_ipNote_Feat ) == S_OK)
		{
			data_info	a_dHZ_AREA_Data;
			Feature_All_Attrs	a_fNote_Attrs;
			a_fNote_Attrs.Init	( a_ipNote_Feat );

			Feature_Attr		a_fPolyID, a_fAttrCode;
			// オブジェクトID
			a_fPolyID			= a_fNote_Attrs.Get_Attr ( "OBJECTID" );	// OID
			a_fAttrCode			= a_fNote_Attrs.Get_Attr ( "BGCLASS_C" );	// 属性

			a_dHZ_AREA_Data.e_iOID		= a_fPolyID.e_ccvAttr.intVal;
			a_dHZ_AREA_Data.e_iAttrCode	= a_fAttrCode.e_ccvAttr.intVal;

			// 属性が街区以外の場合はエラーを出力し、スキップする
			if( a_dHZ_AREA_Data.e_iAttrCode == 1 ) {
				a_dHZ_AREA_Data.e_iAttrCode	= 100;	// 浸水エリア属性にする
			}else {
				PrnMsg( m_fErr_Fp, "%d\t%d\t属性が範囲外\t%d\n", m_iCurMeshCode, a_dHZ_AREA_Data.e_iOID, a_dHZ_AREA_Data.e_iAttrCode);
				continue;
			}
			
			// ここでクリップする　2007.10.11
			a_dHZ_AREA_Data.e_ipGeo		= a_fNote_Attrs.Ret_Geo();			// ジオメトリ

			//---test
		//	if( a_dHZ_AREA_Data.e_iOID == 1932 ) {
		//		ITopologicalOperator2Ptr	a_ipTopo(a_dHZ_AREA_Data.e_ipGeo);
		//		if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
		//			return	( NULL );
		//		}
		//		IPolygonPtr	a_iptmp(a_dHZ_AREA_Data.e_ipGeo);
		//		if( FAILED(a_iptmp->SimplifyPreserveFromTo()) ){
		//			return	( false);
		//		}
		//		Out_to_Shape (	a_dHZ_AREA_Data.e_ipGeo, ".\\testdata" );		///< 出力シェイプファイル名
		//		return	( false );
		//	}
			//---test kokomade

			// メッシュでクリップ
			int				a_iEmpty_F	= 0;
			IGeometryPtr	a_ipCliped_Poly;
			a_ipCliped_Poly	= Mk_Clip_Data ( c_ipMeshRect, a_dHZ_AREA_Data.e_ipGeo, 0, &a_iEmpty_F );
			if( a_iEmpty_F == 1 ) {
				PrnMsg( m_fErr_Fp, "%d\t%d\t空になってます\n", m_iCurMeshCode, a_dHZ_AREA_Data.e_iOID);
				continue;
			}
			if( a_ipCliped_Poly == NULL ) {
				PrnMsg( m_fErr_Fp, "%d\t%d\tNULLになってます\n", m_iCurMeshCode, a_dHZ_AREA_Data.e_iOID);
				continue;
			}
			
			// クリップデータと差し替え
			a_dHZ_AREA_Data.e_ipGeo	= a_ipCliped_Poly;	

			a_mTmp_HZ_AREA_Data.insert( a_dHZ_AREA_Data );
			a_iCount++;
		}
		
		// クリップ処理はここでやる
		ims_data_info	a_iCIndx1;
		int	a_iSkip_F	= 0;
		for( a_iCIndx1 = a_mTmp_HZ_AREA_Data.begin(); a_iCIndx1 != a_mTmp_HZ_AREA_Data.end(); a_iCIndx1++ )
		{
			data_info	a_dSrcDat;
			a_dSrcDat	= *a_iCIndx1;
			a_iSkip_F	= 0;

			ims_data_info	a_iCIndx2;
			for( a_iCIndx2 = a_iCIndx1; a_iCIndx2 != a_mTmp_HZ_AREA_Data.end(); a_iCIndx2++ )
			{
				// 自分同士の場合はスキップ
				if( a_iCIndx1 == a_iCIndx2 ) {
					continue;
				}
				data_info	a_dTgtDat;
				a_dTgtDat	= *a_iCIndx2;

				// インターセクトしない場合はスキップ
				//-------------------------------------------------------------------------------------
				VARIANT_BOOL	a_vRetBool;
				IRelationalOperatorPtr	a_ipRel;
				a_ipRel	= a_dSrcDat.e_ipGeo;
				if( FAILED(a_ipRel->Disjoint( a_dTgtDat.e_ipGeo, &a_vRetBool )) ){
					PrnMsg( m_fErr_Fp, "%d\t%d\tDisjoint()関係の取得に失敗\n", m_iCurMeshCode, a_dSrcDat.e_iOID);
					continue;
				}else {
					if( a_vRetBool == VARIANT_TRUE ) {
						continue;
					}else {
						// タッチもスキップ
						if( FAILED(a_ipRel->Touches ( a_dTgtDat.e_ipGeo, &a_vRetBool )) ){
							PrnMsg( m_fErr_Fp, "%d\t%d\tTouches()関係の取得に失敗\n", m_iCurMeshCode, a_dSrcDat.e_iOID);
							continue;
						}else {
							if( a_vRetBool == VARIANT_TRUE ) {
								continue;
							}
						}
					}
				}
				//-------------------------------------------------------------------------------------
			//	int			e_iAttrCode;	///< 属性分類コード (914@(最重点)=2 915@(重点)=1)
			//	int			e_iGroupCode;
				// Tgtの e_iAttrCode が自分より重点度が上の場合、重点度が同じ場合でも識別IDが後の場合はTgtでSrcをクリップ
				int	a_iClip_F	= 0;
				if( (a_dSrcDat.e_iAttrCode > a_dTgtDat.e_iAttrCode) ||
					((a_dSrcDat.e_iAttrCode == a_dTgtDat.e_iAttrCode) && (a_dSrcDat.e_iGroupCode < a_dTgtDat.e_iGroupCode)) )
				{
					int				a_iEmpty_F	= 0;
					IGeometryPtr	a_ipCliped_Poly;
					a_ipCliped_Poly	= Mk_Clip_Data ( a_dTgtDat.e_ipGeo, a_dSrcDat.e_ipGeo, 1, &a_iEmpty_F );
					if( a_iEmpty_F == 1 ) {
						PrnMsg( m_fErr_Fp, "%d\t%d\t空になってます\n", m_iCurMeshCode, a_dSrcDat.e_iOID);
						a_iSkip_F	= 1;
						break;
					}
					if( a_ipCliped_Poly == NULL ) {
						PrnMsg( m_fErr_Fp, "%d\t%d\tNULLになってます\n", m_iCurMeshCode, a_dSrcDat.e_iOID);
						a_iSkip_F	= 1;
						break;
					}
					// クリップしたデータと入れ替え
					a_dSrcDat.e_ipGeo	= a_ipCliped_Poly;
				}
			}
			// 空になる場合はスキップ
			if( a_iSkip_F == 1 ) {
				continue;
			}

			// データ入れ替え
			IPolygonPtr		a_ipTmpPolygon(a_dSrcDat.e_ipGeo);
			a_ipTmpPolygon->SimplifyPreserveFromTo();

			PointRotate		a_pRotate;
			IGeometryPtr	a_ipRotatedGeo;
			a_ipRotatedGeo	= a_pRotate.ExecRotate ( a_dSrcDat.e_ipGeo );

			// マルチパートになっているポリゴンを分割する
			msi_get_parts_info	a_mMP_Poly;
			a_mMP_Poly.Init ( a_ipRotatedGeo, a_dSrcDat.e_iOID, "DIS", m_iCurMeshCode, m_fErr_Fp );
			long	a_lPart_Num	= 0;
			a_lPart_Num	= a_mMP_Poly.Get_Part_Num();
			int i = 0;
			for( i = 0; i < a_lPart_Num; i++ )
			{
				IGeometryPtr	a_ipCurGeo;
				a_ipCurGeo	= a_mMP_Poly.Get_Geo_from_Part ( i+1 );
				data_info	a_dTmpDat;
				a_dTmpDat	= a_dSrcDat;
				a_dTmpDat.e_ipGeo	= a_ipCurGeo;
				c_mHZ_Area_Data.insert ( a_dTmpDat );
			}
		}
	}
	return	( a_iCount );
}

/** ------------------------------------------------------------------------------
	災害エリアデータをシェープファイルに変換
	@return	bool
--------------------------------------------------------------------------------*/
bool	main_rootin::
HZ_Area_to_Shape (	ISpatialReferencePtr&	c_ipSpRef,			///< 空間参照情報
					ms_data_info&			c_mHZ_Area_Data,	///< 災害エリアデータ
					char*					c_cpOutFName )		///< 出力シェイプファイル名
{
	std::string	a_sPathName	= "";	// ディレクトリ名
	std::string	a_sShpName	= "";	// パスを抜いたシェイプファイル名

	if(!GetFeatureClass ( c_cpOutFName, a_sShpName ) ) {
		return	( false );
	}

	if(!GetFilePath	( c_cpOutFName, a_sPathName ) ) {
		return	( false );
	}
		
	//-------------------------------------------------------------------------------
	// Shapeファイルのオープン処理	
	//-------------------------------------------------------------------------------
	IWorkspaceFactoryPtr	a_ipWSFactory(CLSID_ShapefileWorkspaceFactory);
	IPropertySetPtr			a_ipPropertySet(CLSID_PropertySet);
	IWorkspacePtr			a_ipSWSpace;

	if (a_ipWSFactory == 0) {
	//	E_POINTER;
		return	( false );
	}
	// ワークスペースをオープン
	CComVariant		aVar_Dir((char*)a_sPathName.c_str());
	_bstr_t			a_bTStr;
	a_bTStr			= "DATABASE";
	a_ipPropertySet->SetProperty(a_bTStr, aVar_Dir);
	// ワークスペースのオープン
	a_ipWSFactory->Open(a_ipPropertySet, 0, &a_ipSWSpace);
	if(a_ipSWSpace == NULL) {
		PrnMsg( m_fErr_Fp, "%d\tワークスペース（ディレクトリ）のオープンに失敗しました\n", m_iCurMeshCode);
		return	( false );
	}
	// フィーチャクラスのオープン
	IFeatureWorkspacePtr	ipFWorkspace(a_ipSWSpace);
	IFeatureClassPtr		ipFClass;
	_bstr_t					a_bFCStr;
	a_bFCStr	= (char*)a_sShpName.c_str();
	ipFWorkspace->OpenFeatureClass(a_bFCStr, &ipFClass);
	if(ipFClass != NULL) {
		PrnMsg( m_fErr_Fp, "%d\tすでに同名のshapeファイルが存在しています\n", m_iCurMeshCode);
		a_ipSWSpace == NULL;
		return	( false );
	}

	IFieldsPtr				ipFields(CLSID_Fields);

	IGeometryDefPtr			ipGDef(CLSID_GeometryDef);
	IGeometryDefEditPtr		ipGDefEdit(ipGDef);
//	ISpatialReferencePtr	ipSpRef(CLSID_UnknownCoordinateSystem);

	// 格納するShapeのジオメトリタイプを設定
	ipGDefEdit->put_GeometryType(esriGeometryPolygon);
	// 空間参照を設定しないとprjファイルが出力されない
//	if( FAILED (ipGDefEdit->putref_SpatialReference(c_ipSpRef)) ) {
//		PrnMsg( m_fErr_Fp, "%d\t空間参照の付与に失敗\n", m_iCurMeshCode);
//		return	( false );
//	}

	// ジオメトリ(Shape)・フィールド追加
	//-------------------------------------------------------------
	IFieldsEditPtr	ipFieldsEdit(ipFields);
	IFieldPtr		ipField(CLSID_Field);

	IFieldEditPtr	ipFieldEdit1(ipField);
	ipFieldEdit1->put_Name(T2BSTR(_T("Shape")));
	ipFieldEdit1->put_Type(esriFieldTypeGeometry);
	ipFieldEdit1->put_IsNullable(VARIANT_FALSE);
	ipFieldEdit1->putref_GeometryDef(ipGDefEdit);

	if(ipFieldsEdit->AddField(ipFieldEdit1) != S_OK) return false;

	IFieldPtr		ipField2(CLSID_Field);
	IFieldEditPtr	ipFieldEdit2(ipField2);
	ipFieldEdit2->put_Name(T2BSTR(_T("BGCLS_C")));
	ipFieldEdit2->put_Type(esriFieldTypeInteger);
	ipFieldEdit2->put_IsNullable(VARIANT_FALSE);

	// [Arc10.3.1対応] Shapeフィールド長差異対応
	//   Arc10.1までフィールド長設定は実施しておらず、Integerフィールドはデフォルト長の9に設定されていた。
	//   Arc10.3.1から仕様変更により、Integerフィールドのデフォルト長が9→10に変更されたため、
	//   Integerフィールド長設定に有効なput_Precision()にてArc10.1までと同じ値になるフィールド長を指定して設定する。
	ipFieldEdit2->put_Precision(9);

	if(ipFieldsEdit->AddField(ipFieldEdit2) != S_OK) return false;
	//-------------------------------------------------------------

	_bstr_t		a_bSName, a_bGeoFName;
	a_bSName	= (char*)a_sShpName.c_str();
	a_bGeoFName	= "Shape";

	ipFWorkspace->CreateFeatureClass(a_bSName, ipFields,
		NULL, NULL, esriFTSimple, a_bGeoFName, CComBSTR(""), &ipFClass);

	IFeatureCursorPtr	ipFeatureCursor;
	ipFClass->Insert(VARIANT_TRUE, &ipFeatureCursor);

	// 属性フィールドのインデックスを取得
	long	a_lFldIndx[6] = {0,0,0,0,0,0};
	_bstr_t	a_bFldStr[6];
	a_bFldStr[0]	= "BGCLS_C";
	
	long	roop = 0;
	for( roop = 0; roop < 1; roop++ )
	{
		if (FAILED(ipFClass->FindField(a_bFldStr[roop], &a_lFldIndx[roop]))) {
			PrnMsg( m_fErr_Fp, "%d\t属性フィールドのインデックス取得に失敗\n", m_iCurMeshCode );
			return	( false );
		}
	}

	ims_data_info	a_iIndx;
	for( a_iIndx = c_mHZ_Area_Data.begin(); a_iIndx != c_mHZ_Area_Data.end(); a_iIndx++ )
	{
		IFeatureBufferPtr	ipFeatureBuffer;
		ipFClass->CreateFeatureBuffer(&ipFeatureBuffer);

		data_info	a_dCurDat;

		a_dCurDat	= *a_iIndx;

		// 属性付与
		ipFeatureBuffer->putref_Shape(a_dCurDat.e_ipGeo);
		if (FAILED(ipFeatureBuffer->put_Value(a_lFldIndx[0], CComVariant(a_dCurDat.e_iAttrCode))) ) {
			PrnMsg( m_fErr_Fp, "%d\t属性追加に失敗\n", m_iCurMeshCode);
		}
	
		_variant_t va;
		ipFeatureCursor->InsertFeature(ipFeatureBuffer, &va);
		//ipFeatureCursor->Flush();
	}
	ipFeatureCursor->Flush();

	return	( true );
}

/** ------------------------------------------------------------------------------
	変換実行部
	※内部でエラーファイルの処理等も行う
	@return	成功	true
	@return 失敗	false
--------------------------------------------------------------------------------*/
bool	main_rootin::
conv_exe	( void )
{
	// 開始時間の設定
	m_eExeTime.Set_Start();

	USES_CONVERSION;	// OLE2T 等の一部のマクロを使用する場合には必ず必要

	if( m_iExe_Flag & ON_PGDB_MODE ){

		// パーソナルジオDB用です
		IWorkspaceFactoryPtr	ipWorkspaceFactory	( CLSID_AccessWorkspaceFactory );
		IPropertySetPtr			ipPropertySet		( CLSID_PropertySet );					// 接続プロパティー格納用
		USES_CONVERSION;	// OLE2T 等の一部のマクロを使用する場合には必ず必要
		// カレント直下にオープンしてみる

		_bstr_t			a_bOpenFName;	// T2BSTRから_bstr_tに変更 2007.06.18
		a_bOpenFName	= m_sPGDB_File.c_str();
		PrnMsg( stdout, "#災害エリアPGDB<%s>をオープン・・\n", m_sPGDB_File.c_str() ); 
		ipWorkspaceFactory->OpenFromFile( a_bOpenFName, 0, &m_ipWorkspace_HZ_AREA );
		if( m_ipWorkspace_HZ_AREA == NULL )	{
			PrnMsg( m_fErr_Fp, "パーソナルジオDBがオープンできませんでした\n"); 
			return	( false );
		}else {
			PrnMsg( stdout, "接続完了\n");
		}

	}else {

		if( !SiNDY_DB_Open	( m_ipWorkspace_HZ_AREA )) {
			m_sPrnInfo.Prn_Info("DBへ接続出来なかった\n", stderr );
			return	( false );
		}else {
			PrnMsg	( stdout, "接続完了\n" );
		}
	}

	// メッシュ用の
	if( !DB_Open	( m_ipWorkspace_Mesh,
					  m_pTool_Init_Mesh.get_UserName(), m_pTool_Init_Mesh.get_Password(),
					  m_pTool_Init_Mesh.get_Version(), m_pTool_Init_Mesh.get_Instance(),
					  m_pTool_Init_Mesh.get_Server() )) {
		m_sPrnInfo.Prn_Info	( "接続出来なかった\n", stderr );
		return	( false );
	}else {
		PrnMsg	( stdout, "接続完了\n" );
	}	

	Print_Check_Info();	// 2006.05.17 追加

	// DB接続にかかった時間の表示
	PrnMsg	( m_fErr_Fp,	"#SiNDY-DB接続処理時間 = %0.2f(s)でした\n",
						(double)((m_eExeTime.Get_From_Start()) / 1000.0) );

	if( !Read_Mesh_List() ) {
		m_sPrnInfo.Prn_Info	( "メッシュ読み込み失敗");
		return	( false );
	}

	// レイヤオープン
	if( !Open_Layers ( ) ){	// 地図用ワークスペース
		m_sPrnInfo.Prn_Info	( "レイヤオープンに失敗しています", stderr );
		return	( false );
	}

	// データ読み込み
	if( !Read_HZ_AREA_to_Shape ( ) ) {
		m_sPrnInfo.Prn_Info	( "データ処理に失敗しています", stderr );
		return	( false );
	}

	//　実行部終了
	// 終了時間の取得
	PrnMsg	( m_fErr_Fp,	"#データチェック総時間 = %0.2f(s)でした\n",
						(double)((m_eExeTime.Get_From_Start()) / 1000.0) );
	return	( true );
}

/** ------------------------------------------------------------------------------
	メッシュリストの読み込み設定
	@return	true	成功
	@return	false	失敗
--------------------------------------------------------------------------------*/
bool	main_rootin::
Read_Mesh_List ( void )
{
	if( m_iExe_Flag & ON_MESH || m_iExe_Flag & ON_MONO_MESH ) {		//	メッシュコードリスト指定の場合

		if( m_iExe_Flag & ON_MESH ) {
			if( m_sMeshCode_File == "" ) {
				PrnMsg	( stderr, "メッシュコードリストファイル名がとりだせませんでした[check_exe]\n");
				return	( false );
			}	
			if( m_sInput_List.read_mesh_list ( (char*)m_sMeshCode_File.c_str() ) != 0 ) {	// ここでリストを読み込む
				PrnMsg	( stderr, "メッシュコードリストファイル<%s>の読み込みに失敗[check_exe]\n", m_sMeshCode_File.c_str() );
				return	( false );
			}
		}else {
			if( m_sInput_List.read_mesh_list ( m_iMeshCode ) != 0 ) {	// ここでリストを読み込む
				PrnMsg	( stderr, "メッシュコードリストファイル<%s>の読み込みに失敗[check_exe]\n", m_sMeshCode_File.c_str() );
				return	( false );
			}
		}
		if( !get_mesh_init( m_sInput_List , m_ipWorkspace_Mesh ) ) {	// メッシュのデータはどこのDBでも参照できるはず・・・
			PrnMsg	( stderr, "メッシュレイヤの獲得の初期化に失敗[check_exe]\n");
			return	( false );
		}
	}
	return	( true );
}
