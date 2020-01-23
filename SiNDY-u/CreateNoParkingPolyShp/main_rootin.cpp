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

#include <atlstr.h>
#include "..\..\LIB\WinLib\VersionInfo.h"
#include "PointRotate.h"
//	2003.07.04	main_rootin.hから定義を分離


std::string	main_rootin::
Str_to_SJIS		(	CComBSTR*	c_CStr	)	// 文字列(Unicode)
{	
	std::string		ret_str;

#ifndef	_ARC_9_1_
	USES_CONVERSION;
	if (*c_CStr != 0) {
		_bstr_t a_bTmpStr((*c_CStr));

		ret_str	= a_bTmpStr;
	}
#else
	long		length	= c_CStr->Length();

	char*		tmp_moji	= new char[length+1];

	memset		(tmp_moji, '\0', length+1);
	_bstr_t		a_bTmp ( *c_CStr, false );
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
#endif	// endof _ARC_9_1_
	return	(ret_str);
}

/** ------------------------------------------------------------------------------
	DBへの接続オープン
	@return	ワークスペースのポインタ
--------------------------------------------------------------------------------*/
IWorkspacePtr	main_rootin::
DB_Open	(	char*	c_cpTargetDB )		///< 対象DB名
{
	IUnknownPtr ipUnknown = NULL;
	INamePtr ipName = INamePtr(sindy::create_workspace_name(CA2CT(c_cpTargetDB)));
	if( ! ipName ) {
		return NULL;
	}
	ipName->Open(&ipUnknown);
	if( ! ipUnknown ) {
		return NULL;
	}
	IWorkspacePtr ipWorkspace = ipUnknown;
	return ipWorkspace;
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
			PrnMsg	( stderr, "エラーログファイル<%s>がオープンできない[main_rootin]\n", e_cpLog_File );
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
		if(strcmp(argv[i], "-npi") == 0) {	// 駐車違反取締エリア初期化情報　※未使用
			if(argc < i+2) return( false );
			e_cpInit_Str_NP_AREA	= argv[i+1];
			a_exeflag			|= ON_INIT_NP_AREA;
			a_optcount[0]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-npf") == 0) {	// 駐車違反取締エリアフィーチャクラス指定
			if(argc < i+2) return( false );
			e_cpNP_AREA_FC		= argv[i+1];
			a_exeflag			|= ON_FC_NP_AREA;
			a_optcount[1]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-mi") == 0) {	// 駐車違反取締エリア初期化情報
			if(argc < i+2) return( false );
			e_cpInit_Str_MESH	= argv[i+1];
			a_exeflag			|= ON_INIT_MESH;
			a_optcount[2]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-mf") == 0) {	// メッシュフィーチャクラス指定
			if(argc < i+2) return( false );
			e_cpMesh_FC			= argv[i+1];
			a_exeflag			|= ON_FC_MESH;
			a_optcount[4]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-l") == 0) { // ログファイル設定（設定が無い場合は、エラーはstderrへ垂れ流しになる）
			if(argc < i+2) return( false );
			e_cpLog_File	= argv[i+1];	///< メッシュコードリストファイル
			a_exeflag		|= ON_LOGFILE;
			a_optcount[5]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-v") == 0) {
			//if(argc < i+1) return( false );
			print_version();
			a_optcount[6]++;
			i+=1;
			return	( false );	// 仮にこうする？
		}else
		if(strcmp(argv[i], "-m") == 0) { // 2002.9.24
			if(argc < i+2) return( false );
			e_cpMeshCode_File	= argv[i+1];	///< メッシュコードリストファイル
			a_exeflag		|= ON_MESH;
			a_optcount[7]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-M") == 0) { // 2003.4.1
			if(argc < i+2) return( false );
			a_exeflag		|= ON_MONO_MESH;
			e_iMeshCode		= atoi(argv[i+1]);	
			a_optcount[8]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-pgdb") == 0) {
			a_exeflag		|= ON_PGDB_MODE;
			e_cpPGDB_File	= argv[i+1];
			a_optcount[9]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-o") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_OUTPATH;
			e_cpOutPath		= argv[i+1];	
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

	if( !(a_exeflag & ON_FC_NP_AREA) ) {		///< 注記フィーチャクラス
		PrnMsg	(stderr, "駐車違反取締エリアのフィーチャクラスが指定されていません\n");
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
		PrnMsg	(stderr, "駐車違反取締エリアのPGDBファイルが指定されていません\n");
		return	( false );
	}

	// エラーログファイルのオープン	2006.05.17 実行情報も出力したいのでここへ移動
	if( !Open_Err_Log () ) {
		return	( false );
	}else {
		if( !e_sPrnInfo.Init(e_fErr_Fp) ) {
			return	( false );
		}
	}

	//--------------------------------------------------------------------------
	// 実行情報を出力 2009.09.30 追加
	PrnMsg( e_fErr_Fp, "#ツールバージョン                               : <%s>\n", m_sVersion.c_str() );
	PrnMsg( e_fErr_Fp, "#昭文社駐車違反取締エリアフィーチャクラス       : <%s>\n", e_cpNP_AREA_FC );
	PrnMsg( e_fErr_Fp, "#メッシュフィーチャクラス用DB接続情報           : <%s>\n", e_cpInit_Str_MESH );
	PrnMsg( e_fErr_Fp, "#メッシュフィーチャクラス                       : <%s>\n", e_cpMesh_FC );
	if(a_exeflag & ON_PGDB_MODE) {
	PrnMsg( e_fErr_Fp, "#PGDBファイル                                   : <%s>\n", e_cpPGDB_File );
	}
	PrnMsg( e_fErr_Fp, "#データ出力パス                                 : <%s>\n", e_cpOutPath );
	if( a_exeflag & ON_MESH ) {
	PrnMsg( e_fErr_Fp, "#チェック対象メッシュリスト      : <%s>\n", e_cpMeshCode_File );
	}else {
	PrnMsg( e_fErr_Fp, "#チェック対象メッシュコード      : <%02d>\n", e_iMeshCode );
	}
	//--------------------------------------------------------------------------
	
	eExe_Flag	= a_exeflag;

	return		( true );
}

/** ------------------------------------------------------------------------------
// 使用法表示
--------------------------------------------------------------------------------*/
void		main_rootin::
print_usage	(char **argv)
{
	PrnMsg(	stdout,
				"=== 【駐車違反取締エリアポリゴンShape変換ツール】 ===\n"
				"<usage>\n"
				"%s <オプション>... \n"
				"\t<引数>\n"
			//	"\t      <arg1>         : 中縮尺注記初期設定(ex. <user>/<passwd>/<instance>/<version>/<server>\n)"
			//	"\t      <arg2>         : 中縮尺注記チェックレイヤ名ex. SiNDY2006A.Base_Anno)\n"
			//	"\t      <arg3>         : 都市地図注記初期設定(<user>/<passwd>/<instance>/<version>/<server>\n)"
			//	"\t      <arg4>         : 都市地図注記チェックレイヤ名(ex. SiNDY2006A.Base_Anno)\n"
			//	"\t      <arg5>         : ミドル注記初期設定(<user>/<passwd>/<instance>/<version>/<server>\n)"
			//	"\t      <arg6>         : ミドル注記チェックレイヤ名(ex. SiNDY2006A.Base_Anno)\n"
			//	"\t      <arg7>         : メッシュレイヤ名(ex. REFERENCE.BaseMesh)\n"
			//	"\t      <arg8>         : 辞書ファイル(ex. cross.dic)\n"
			//	"\t<オプション>\n"
				"\t      '-m'           : メッシュリストファイル名を指定(ex. -m index.base)\n"
				"\t      '-M'           : 単メッシュを指定(ex. -M 594141)\n"
				"\t      '-v'           : バージョン情報表示\n"
				"\t      '-npi'         : 昭文社駐車違反取締エリアポリゴンレイヤ接続情報 ex) -npi MQ200709/MQ200709/SDE.DEFAULT/ruby2\n"
				"\t      '-npf'         : 昭文社駐車違反取締エリアフィーチャクラス指定 ex) -npi MQ200709.City_Annotation\n"
				"\t      '-mi'          : メッシュフィーチャクラス接続情報 ex) -mi ronly/ronly/SDE.DEFAULT/ruby2\n"
				"\t      '-mf'          : メッシュフィーチャクラス指定  ex) -mf REFERENCE.CityMesh\n"
				"\t      '-l'           : ログファイル名を指定 ex) -l .\\test\\make.log\n"
				"\t      '-pgdb'        : PGDBファイル名を指定 ex) -pgdb .\\data\\ANNO.mdb\n"
				"\t      '-o'           : データ出力パス ex) -pgdb .\\data\\Citymap\n"
				"\t                       ※指定したデータパス以下にメッシュのフォルダを自動作成し出力する。\n"
				"\t<補足>\n"
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
	PrnMsg(	stdout,
				"=== 【駐車違反取締エリアポリゴンShape変換ツール】 ===\n"
				"\t<バージョン情報>\n"
			//	"\t2007/09/21        1.0.0.1            : 新規作成\n"
				"\t2007/11/15        1.0.0.1            : 新規作成\n"
				"\t2009/09/30        2.0.0.2            : 昭文社の新フォーマットへの対応\n"
				"\t2014/11/27        2.0.0.3            : ツールの戻り値を統一,レプリカ対応\n"
			);
}

/** ------------------------------------------------------------------------------
	メッシュレイヤオープン
------------------------------------------------------------------------------==*/

bool	main_rootin::
Open_Mesh_Layers	(	void	)
{
	IFeatureWorkspacePtr	a_F_Workspace	( e_ipWorkspace_Mesh );	// フィーチャワークスペース

	if( FAILED(a_F_Workspace->OpenFeatureClass(CComBSTR(e_cpMesh_FC), &e_ipF_Class )) ) {
		PrnMsg( e_fErr_Fp, "レイヤ <%s> のオープンに失敗\n", e_cpMesh_FC);
		return	(false);
	}else {
		PrnMsg( stdout,	"<%s> レイヤをオープン\n", e_cpMesh_FC);
	}

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
	if( FAILED(e_ipF_Class->get_ShapeFieldName	( &a_SpatialCol )) ) {
		PrnMsg( e_fErr_Fp, "%d\tシェイプのフィールド名取得に失敗\n", c_iMeshCode);
		return	( NULL );
	}

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
		eDigit	= a_iDigit;
		break;
	default:
		PrnMsg( e_fErr_Fp, "%d\tメッシュコードの桁数が不正\n", c_iMeshCode);
		return	( NULL );
		break;
	}

	a_Where_Str	= a_tmpstr;		// stringをBSTRに変換

	IQueryFilterPtr		a_Sp_Filter	( CLSID_QueryFilter );			// 空間検索フィルタ
	if( FAILED	(a_Sp_Filter->put_WhereClause	( a_Where_Str)) ) {		// Where句の設定
		PrnMsg( e_fErr_Fp, "%d\tWhere句の設定に失敗\n", c_iMeshCode);
		return	( NULL );
	}

	// クリップ矩形作成
	IEnvelopePtr	a_ipMesh_Rect;	// メッシュ矩形
	a_ipMesh_Rect	= NULL;

	// サーチしてくる
	IFeatureCursorPtr	a_F_Cursor;
	if( FAILED	(e_ipF_Class->Search	( a_Sp_Filter, VARIANT_FALSE, &a_F_Cursor )) ) {
		PrnMsg	(e_fErr_Fp, "%d\tデータの属性検索に失敗\n", c_iMeshCode);
		return	(NULL);
	}else {
		IFeaturePtr				a_Feature;

		for(;;)
		{
			if( a_F_Cursor->NextFeature ( &a_Feature ) != S_OK) {	// データをひとつ取り出す
				break;
			}
				
			IGeometryPtr		a_Clip_Shp;		// こいつにデータをコピーしてクリップする

			if( FAILED(a_Feature->get_ShapeCopy	( &a_Clip_Shp )) ) {	// フィーチャから
				PrnMsg( e_fErr_Fp, "%d\tシェイプのコピーに失敗\n", c_iMeshCode);
				return	( NULL );
			}
			
			VARIANT_BOOL	a_bResult;
			a_Clip_Shp->get_IsEmpty( &a_bResult );
			if(a_bResult == VARIANT_TRUE) {	// クリップ結果がNULLならメッシュ外
				continue;
			}
			if( FAILED(a_Clip_Shp->get_Envelope( &a_ipMesh_Rect) )) {
				PrnMsg( e_fErr_Fp, "該当メッシュ<%d>の矩形がみつかりませんでした\n", c_iMeshCode);
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
Open_Layers		( void )
{
	if( !e_mNP_AREA_WS.Init ( e_ipWorkspace_NP_AREA, e_cpNP_AREA_FC ) ) {
		PrnMsg( stderr, "駐車違反取締エリアフィーチャクラス<%s>の読み込みに失敗\n", e_cpNP_AREA_FC );
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
	a_iDigit	= e_cCrd.GetMeshDigit ( c_iMeshCode );

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
		PrnMsg( stderr, "ディレクトリ名取得に失敗\t%s\n", (char*)temp.c_str() );
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
Read_NP_AREA_to_Shape	( void )
{

	IFeatureCursorPtr	a_ipNP_Area_Cursor;

	// キャッシュ上にとってくる
	// オンメモリテスト 2002.6.24
	int		a_iCur_Mesh;
	for( a_iCur_Mesh = e_sInput_List.get_mesh_first(); a_iCur_Mesh != -1; a_iCur_Mesh = e_sInput_List.get_mesh_next() )
	{
		PrnMsg( stdout, "メッシュ[%d] データ取得\n", a_iCur_Mesh );

		e_iCurMeshCode	= a_iCur_Mesh;

		// メッシュ矩形の取り出し
		IGeometryPtr	a_ipMeshRect;
		a_ipMeshRect	= get_mesh_rect( a_iCur_Mesh );
		if( a_ipMeshRect == NULL ) {	// メッシュ矩形が無い場合
			PrnMsg( e_fErr_Fp, "%d\tのメッシュ矩形が見つかりませんでした\n", a_iCur_Mesh );
			return	( false );
		}
		
		IEnvelopePtr	a_ipRect;
		if( a_ipMeshRect->get_Envelope ( &a_ipRect ) ) {
			PrnMsg( e_fErr_Fp, "%d\tのメッシュ矩形の取得に失敗\n", a_iCur_Mesh );
			return	( false );
		}

		// 空間参照を取得する 2007.08.22
		ISpatialReferencePtr	a_ipSpRef;
		if( FAILED (a_ipRect->get_SpatialReference ( &a_ipSpRef )) ) {
			PrnMsg( e_fErr_Fp, "%d\t空間参照の取得に失敗\n", a_iCur_Mesh );
			return	( false );
		}

		a_ipNP_Area_Cursor	= e_mNP_AREA_WS.Search_Feature ( a_ipRect, esriSpatialRelIntersects, "", RET_FCURSOR );
		if ( a_ipNP_Area_Cursor == NULL ) {
			e_sPrnInfo.Prn_Info	( "駐車違反ポリゴンデータを取得できませんでした\n", stderr );
			return	( false );
		}
		PrnMsg( stdout, "メッシュコード<%d> 駐車違反ポリゴンデータ読み込み中\n", a_iCur_Mesh );
		if ( ! Set_NP_AREA_Data	( a_ipMeshRect, a_ipNP_Area_Cursor, e_mAll_Data, CITY_NOTE ) ) {
			return	( false );
		}

		// 出力ファイル名作成
		std::string	a_sOutFName;
		int	a_iMkLv	= 0;
		switch (eDigit)
		{
		case	2:	a_iMkLv	= 1; break;
		case	4:	a_iMkLv	= 1; break;
		case	6:	a_iMkLv	= 2; break;
		case	8:	a_iMkLv	= 3; break;
			break;
		}
		std::string	a_sOutPath;
		a_sOutFName	= make_mesh_fname ( a_iCur_Mesh, a_iMkLv, e_cpOutPath, "_NPA.shp", a_sOutPath );
		// データ出力パスを作成
		if( e_cUC.makedir ( (char*)a_sOutPath.c_str() ) == false ) {
			PrnMsg( e_fErr_Fp, "%d\tディレクトリ作成に失敗<%s>\n", a_iCur_Mesh, (char*)a_sOutPath.c_str() );
			return	( false );
		}

		if( ! NP_Area_to_Shape (	a_ipSpRef, e_mAll_Data, (char*)a_sOutFName.c_str() ) ) {
			return	( false );
		}

		e_mAll_Data.clear();
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
			PrnMsg( e_fErr_Fp, "%d\tIntersect失敗\n", e_iCurMeshCode);
			return	( NULL );
		}
	}else {
		if( FAILED( a_ipClip_Topo->Difference (c_ipClipPoly, &a_ipCliped_Shp)) ) {
			PrnMsg( e_fErr_Fp, "%d\tDifference失敗\n", e_iCurMeshCode);
			return	( NULL );
		}
	}

	VARIANT_BOOL	a_bResult;
	a_ipCliped_Shp->get_IsEmpty( &a_bResult );
	if(a_bResult == VARIANT_TRUE) {	// クリップ結果がNULLならメッシュ外
		*c_ipEmpty_F	= 1;	
	//	fprintf( e_fErr_Fp,"クリップ結果が空\n");
		return	( NULL );
	}

	// 空間参照を取得する 2007.08.22
	ISpatialReferencePtr	a_ipSpRef;
	if( FAILED (c_ipTgtGeo->get_SpatialReference ( &a_ipSpRef )) ) {
		PrnMsg( e_fErr_Fp, "%d\t空間参照の取得に失敗\n", e_iCurMeshCode );
		return	( NULL );
	}

	// シンプリファイをかける
	ITopologicalOperator2Ptr	a_ipTopo;

	a_ipTopo	= a_ipCliped_Shp;

	if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
		PrnMsg( e_fErr_Fp, "%d\tSimplifyに失敗\n", e_iCurMeshCode );
		return	( NULL );
	}
	IPolygonPtr	a_ipTmpPoly;
	a_ipTmpPoly = a_ipCliped_Shp;

	if(FAILED ( a_ipTmpPoly->SimplifyPreserveFromTo() ) ) {
		PrnMsg( e_fErr_Fp, "%d\tSimplifyPreserveFromToに失敗\n", e_iCurMeshCode );	// エラーメッセージ
		return	( NULL );
	}
	// 空間参照の付与
	if( FAILED (a_ipCliped_Shp->putref_SpatialReference ( a_ipSpRef )) ) {
		PrnMsg( e_fErr_Fp, "%d\t空間参照の付与に失敗\n", e_iCurMeshCode );
		return	( NULL );
	}

	return ( a_ipCliped_Shp );
}

/** ------------------------------------------------------------------------------
	駐車違反取締エリアデータをセットに落とす
	@return	成功 true
	@return	失敗 false
--------------------------------------------------------------------------------*/
bool		main_rootin::
Set_NP_AREA_Data ( IGeometryPtr&		c_ipMeshRect,		///< メッシュ矩形
				   IFeatureCursorPtr&	c_ipNoteCursor, 	///< 注記カーソル
				   ms_data_info&		c_mNP_Area_Data,	///< 駐車違反ポリゴンデータ
				   int					c_iData_Type	)	///< データタイプ
{
	int	a_iCount	= 0;
	// データをマルチセットのデータに落とす
	if( c_ipNoteCursor != NULL ) {
		IFeaturePtr	a_ipNote_Feat;
		// 検索されたポリゴンのループ
		ms_data_info	a_mTmp_NP_AREA_Data;
		for(;;)
		{
			data_info	a_dNP_AREA_Data;
			if( c_ipNoteCursor->NextFeature ( &a_ipNote_Feat ) == S_OK ) {

				Feature_All_Attrs	a_fNote_Attrs;
				if( ! a_fNote_Attrs.Init	( a_ipNote_Feat ) ) {
					PrnMsg( e_fErr_Fp, "%d\t駐車違反ポリゴンデータの１フィーチャの全属性の取得に失敗\n", e_iCurMeshCode );
					return	( false );
				}

				Feature_Attr		a_fPolyID, a_fAttrCode, a_fGroupCode;
				// オブジェクトID
				a_fPolyID			= a_fNote_Attrs.Get_Attr ( "OBJECTID" );	// 注記のOID
			//	a_fAttrCode			= a_fNote_Attrs.Get_Attr ( "AttrCode" );	// 属性分類コード914@～916@
			//	a_fGroupCode		= a_fNote_Attrs.Get_Attr ( "GroupCode" );	// 識別番号
				// 仕様変更の為変更	2009.05.18 		
				a_fAttrCode			= a_fNote_Attrs.Get_Attr ( "CODE" );		// 属性分類コード914@～916@
	
				std::string		a_sTmpAttrCode;

				a_sTmpAttrCode	= Str_to_SJIS ((CComBSTR *)(&a_fAttrCode.e_ccvAttr.bstrVal));

				a_dNP_AREA_Data.e_iOID			= a_fPolyID.e_ccvAttr.intVal;
				a_dNP_AREA_Data.e_iGroupCode	= a_fPolyID.e_ccvAttr.intVal;
			//	仕様変更の為変更	2009.05.18
			//	if( a_sTmpAttrCode == "914@" ) {	// 最重点
				if( a_sTmpAttrCode == "P_avaa" ) {
					a_dNP_AREA_Data.e_iAttrCode	= 1;
				}else
			//	仕様変更の為変更	2009.05.18
			//	if( a_sTmpAttrCode == "915@" ) {	// 重点
				if( a_sTmpAttrCode == "P_avab" ) {
					a_dNP_AREA_Data.e_iAttrCode	= 2;				
				}else {	// 最重点、重点以外は入れない
					continue;
				}
				
				// ここでクリップする　2007.10.11
				a_dNP_AREA_Data.e_ipGeo		= a_fNote_Attrs.Ret_Geo();			// ジオメトリ

				//---test
			//	if( a_dNP_AREA_Data.e_iOID == 1932 ) {
					ITopologicalOperator2Ptr	a_ipTopo(a_dNP_AREA_Data.e_ipGeo);
					if( FAILED ( a_ipTopo->put_IsKnownSimple (  VARIANT_FALSE ) ) ) {
						PrnMsg( e_fErr_Fp, "%d\tSimplifyに失敗\t%d\n", e_iCurMeshCode, a_dNP_AREA_Data.e_iOID );
						return	( false );
					}
					IPolygonPtr	a_iptmp(a_dNP_AREA_Data.e_ipGeo);
					if( FAILED(a_iptmp->SimplifyPreserveFromTo()) ){
						PrnMsg( e_fErr_Fp, "%d\tSimplifyPreserveFromToに失敗\t%d\n", e_iCurMeshCode, a_dNP_AREA_Data.e_iOID );
						return	( false );
					}
			//		Out_to_Shape (	a_dNP_AREA_Data.e_ipGeo, ".\\testdata" );		///< 出力シェイプファイル名
			//		return	( false );
			//	}
				//---test kokomade

				// メッシュでクリップ
				int				a_iEmpty_F	= 0;
				IGeometryPtr	a_ipCliped_Poly;
				a_ipCliped_Poly	= Mk_Clip_Data ( c_ipMeshRect, a_dNP_AREA_Data.e_ipGeo, 0, &a_iEmpty_F );
				if( a_iEmpty_F == 1 ) {
					PrnMsg( e_fErr_Fp, "%d\t空になってます\t%d\n", e_iCurMeshCode, a_dNP_AREA_Data.e_iOID);
					continue;
				}
				if( a_ipCliped_Poly == NULL ) {
					PrnMsg( e_fErr_Fp, "%d\tNULLになってます\t%d\n", e_iCurMeshCode, a_dNP_AREA_Data.e_iOID);
					return( false );
				}
				
				// クリップデータと差し替え
				a_dNP_AREA_Data.e_ipGeo	= a_ipCliped_Poly;	

				a_mTmp_NP_AREA_Data.insert( a_dNP_AREA_Data );
				a_iCount++;
			}else {
				break;
			}
		}
		
		// クリップ処理はここでやる
		ims_data_info	a_iCIndx1;
		int	a_iSkip_F	= 0;
		for( a_iCIndx1 = a_mTmp_NP_AREA_Data.begin(); a_iCIndx1 != a_mTmp_NP_AREA_Data.end(); a_iCIndx1++ )
		{
			data_info	a_dSrcDat;
			a_dSrcDat	= *a_iCIndx1;
			a_iSkip_F	= 0;

			ims_data_info	a_iCIndx2;
			for( a_iCIndx2 = a_iCIndx1; a_iCIndx2 != a_mTmp_NP_AREA_Data.end(); a_iCIndx2++ )
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
					PrnMsg( e_fErr_Fp, "%d\tDisjoint()関係の取得に失敗\n", e_iCurMeshCode);
					return	( false );
				}
				if( a_vRetBool == VARIANT_TRUE ) {
					continue;
				}

				// タッチもスキップ
				if( FAILED(a_ipRel->Touches ( a_dTgtDat.e_ipGeo, &a_vRetBool )) ){
					PrnMsg( e_fErr_Fp, "%d\tTouches()関係の取得に失敗\n", e_iCurMeshCode);
					return	( false );
				}
				if( a_vRetBool == VARIANT_TRUE ) {
					continue;
				}

				//-------------------------------------------------------------------------------------
				// Tgtの e_iAttrCode(属性分類コード (914@(最重点)=2 915@(重点)=1)) が自分より重点度が上の場合、
				// 重点度が同じ場合でも識別IDが後の場合はTgtでSrcをクリップ
				int	a_iClip_F	= 0;
				if( (a_dSrcDat.e_iAttrCode > a_dTgtDat.e_iAttrCode) ||
					((a_dSrcDat.e_iAttrCode == a_dTgtDat.e_iAttrCode) && (a_dSrcDat.e_iGroupCode < a_dTgtDat.e_iGroupCode)) )
				{
					int				a_iEmpty_F	= 0;
					IGeometryPtr	a_ipCliped_Poly;
					a_ipCliped_Poly	= Mk_Clip_Data ( a_dTgtDat.e_ipGeo, a_dSrcDat.e_ipGeo, 1, &a_iEmpty_F );
					if( a_iEmpty_F == 1 ) {
						PrnMsg( e_fErr_Fp, "%d\t空になってます\t%d\n", e_iCurMeshCode, a_dSrcDat.e_iOID);
						a_iSkip_F	= 1;
						break;
					}
					if( a_ipCliped_Poly == NULL ) {
						PrnMsg( e_fErr_Fp, "%d\tNULLになってます\t%d\n", e_iCurMeshCode, a_dSrcDat.e_iOID);
						return	( false );
					}
					// クリップしたデータと入れ替え
					a_dSrcDat.e_ipGeo	= a_ipCliped_Poly;
				}
			}
			// 空になる場合はスキップ
			if( a_iSkip_F == 1 ) {
				continue;
			}

			IPolygonPtr		a_ipTmpPolygon(a_dSrcDat.e_ipGeo);
			if( FAILED(a_ipTmpPolygon->SimplifyPreserveFromTo()) ) {
				PrnMsg( e_fErr_Fp, "%d\tSimplifyPreserveFromToに失敗\t%d\n", e_iCurMeshCode, a_dSrcDat.e_iOID );
				return	( false );
			}

			PointRotate		a_pRotate;
			IGeometryPtr	a_ipRotatedGeo;
			if( (a_ipRotatedGeo	= a_pRotate.ExecRotate ( a_dSrcDat.e_ipGeo )) == NULL ) {
				PrnMsg( e_fErr_Fp, "%d\t点列の回転に失敗\t%d\n", e_iCurMeshCode, a_dSrcDat.e_iOID );
				return	( false );
			}

			// マルチパートになっているポリゴンを分割する
			msi_get_parts_info	a_mMP_Poly;
			if( ! a_mMP_Poly.Init ( a_ipRotatedGeo, a_dSrcDat.e_iOID, "NP_Polygon", 59414130, e_fErr_Fp ) ) {
				PrnMsg( e_fErr_Fp, "%d\tマルチパートポリゴンの分割に失敗\t%d\n", e_iCurMeshCode, a_dSrcDat.e_iOID );
				return	( false );
			}

			long	a_lPart_Num	= 0;
			a_lPart_Num	= a_mMP_Poly.Get_Part_Num();
			int i = 0;
			for( i = 0; i < a_lPart_Num; i++ )
			{
				IGeometryPtr	a_ipCurGeo;
				if( (a_ipCurGeo	= a_mMP_Poly.Get_Geo_from_Part ( i+1 )) == NULL ) {
					PrnMsg( e_fErr_Fp, "%d\tシェイプの指定パート、サブパートから、ジオメトリ取得に失敗\t%d\t%d\n", e_iCurMeshCode, a_dSrcDat.e_iOID, i );
					return	( false );
				}
				data_info	a_dTmpDat;
				a_dTmpDat	= a_dSrcDat;
				a_dTmpDat.e_ipGeo	= a_ipCurGeo;
				c_mNP_Area_Data.insert ( a_dTmpDat );
			}
		}
	}
	return	( true );
}

/** ------------------------------------------------------------------------------
	駐車違反取締エリアデータをシェープファイルに変換
	@return	bool
--------------------------------------------------------------------------------*/
bool	main_rootin::
NP_Area_to_Shape (	ISpatialReferencePtr&	c_ipSpRef,			///< 空間参照情報
					ms_data_info&			c_mNP_Area_Data,	///< 駐車違反取締エリアデータ
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
		PrnMsg( e_fErr_Fp, "%d\tワークスペース（ディレクトリ）の生成に失敗しました\n", e_iCurMeshCode);
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
		PrnMsg( e_fErr_Fp, "%d\tワークスペース（ディレクトリ）のオープンに失敗しました\n", e_iCurMeshCode);
		return	( false );
	}
	// フィーチャクラスのオープン
	IFeatureWorkspacePtr	ipFWorkspace(a_ipSWSpace);
	IFeatureClassPtr		ipFClass;
	_bstr_t					a_bFCStr;
	a_bFCStr	= (char*)a_sShpName.c_str();
	ipFWorkspace->OpenFeatureClass(a_bFCStr, &ipFClass);
	if(ipFClass != NULL) {
		PrnMsg( e_fErr_Fp, "%d\tすでに同名のshapeファイルが存在しています\n", e_iCurMeshCode);
		a_ipSWSpace == NULL;
		return	( false );
	}

	IFieldsPtr				ipFields(CLSID_Fields);

	IGeometryDefPtr			ipGDef(CLSID_GeometryDef);
	IGeometryDefEditPtr		ipGDefEdit(ipGDef);

	// 格納するShapeのジオメトリタイプを設定
	if( FAILED(ipGDefEdit->put_GeometryType(esriGeometryPolygon)) ) {
		PrnMsg( e_fErr_Fp, "%d\tジオメトリタイプの設定に失敗\n", e_iCurMeshCode);
		return	( false );
	}
	// 空間参照を設定しないとprjファイルが出力されない
	if( FAILED (ipGDefEdit->putref_SpatialReference(c_ipSpRef)) ) {
		PrnMsg( e_fErr_Fp, "%d\t空間参照の付与に失敗\n", e_iCurMeshCode);
		return	( false );
	}

	// ジオメトリ(Shape)・フィールド追加
	//-------------------------------------------------------------
	IFieldsEditPtr	ipFieldsEdit(ipFields);
	IFieldPtr		ipField(CLSID_Field);

	IFieldEditPtr	ipFieldEdit1(ipField);
	if( FAILED(ipFieldEdit1->put_Name(T2BSTR(_T("Shape")))) ) {
		PrnMsg( e_fErr_Fp, "%d\tShapeの名前の設定に失敗\n", e_iCurMeshCode);
		return	( false );
	}
	if( FAILED(ipFieldEdit1->put_Type(esriFieldTypeGeometry)) ) {
		PrnMsg( e_fErr_Fp, "%d\tShapeのタイプの設定に失敗\n", e_iCurMeshCode);
		return	( false );
	}
	if( FAILED( ipFieldEdit1->put_IsNullable(VARIANT_FALSE)) ) {
		PrnMsg( e_fErr_Fp, "%d\tShapeのNULL非許容の設定に失敗\n", e_iCurMeshCode);
		return	( false );
	}
	if( FAILED(ipFieldEdit1->putref_GeometryDef(ipGDefEdit)) ) {
		PrnMsg( e_fErr_Fp, "%d\tShapeのジオメトリ定義の設定に失敗\n", e_iCurMeshCode);
		return	( false );
	}

	if(ipFieldsEdit->AddField(ipFieldEdit1) != S_OK) {
		PrnMsg( e_fErr_Fp, "%d\tShapeのフィールド追加に失敗\n", e_iCurMeshCode);
		return false;
	}

	IFieldPtr		ipField3(CLSID_Field);
	IFieldEditPtr	ipFieldEdit3(ipField3);
	if( FAILED(ipFieldEdit3->put_Name(T2BSTR(_T("NPINFO_C")))) ) {
		PrnMsg( e_fErr_Fp, "%d\tNPINFO_Cの名前の設定に失敗\n", e_iCurMeshCode);
		return	( false );
	}
	if( FAILED(ipFieldEdit3->put_Type(esriFieldTypeInteger)) ) {
		PrnMsg( e_fErr_Fp, "%d\tNPINFO_Cのタイプの設定に失敗\n", e_iCurMeshCode);
		return	( false );
	}
	if( FAILED(ipFieldEdit3->put_IsNullable(VARIANT_FALSE)) ) {
		PrnMsg( e_fErr_Fp, "%d\tNPINFO_CのNULL非許容の設定に失敗\n", e_iCurMeshCode);
		return	( false );
	}
	// [Arc10.3.1対応] Shapeフィールド長差異対応
	//   Arc10.1までフィールド長設定は実施しておらず、Integerフィールドはデフォルト長の9に設定されていた。
	//   Arc10.3.1から仕様変更により、Integerフィールドのデフォルト長が9→10に変更されたため、
	//   Integerフィールド長設定に有効なput_Precision()にてArc10.1までと同じ値になるフィールド長を指定して設定する。
	if (FAILED(ipFieldEdit3->put_Precision(9))) {
		PrnMsg(e_fErr_Fp, "%d\tNPINFO_Cのフィールド長の設定に失敗\n", e_iCurMeshCode);
		return	(false);
	}

	if(ipFieldsEdit->AddField(ipFieldEdit3) != S_OK) {
		PrnMsg( e_fErr_Fp, "%d\tNPINFO_Cのフィールド追加に失敗\n", e_iCurMeshCode);
		return	( false );
	}

	//-------------------------------------------------------------

	_bstr_t		a_bSName, a_bGeoFName;
	a_bSName	= (char*)a_sShpName.c_str();
	a_bGeoFName	= "Shape";

	if( FAILED(ipFWorkspace->CreateFeatureClass(a_bSName, ipFields,
				NULL, NULL, esriFTSimple, a_bGeoFName, CComBSTR(""), &ipFClass)) ) {
		PrnMsg( e_fErr_Fp, "%d\tフィーチャクラスの生成に失敗\n", e_iCurMeshCode );
		return	( false );
	}

	IFeatureCursorPtr	ipFeatureCursor;
	if( FAILED(ipFClass->Insert(VARIANT_TRUE, &ipFeatureCursor)) ) {
		PrnMsg( e_fErr_Fp, "%d\tフィーチャクラスのInsertカーソル生成に失敗\n", e_iCurMeshCode );
		return	( false );
	}

	// 属性フィールドのインデックスを取得
	long	a_lFldIndx[6] = {0,0,0,0,0,0};
	_bstr_t	a_bFldStr[6];
	a_bFldStr[0]	= "NPINFO_C";
	
	long	roop = 0;
	for( roop = 0; roop < 1; roop++ )
	{
		if (FAILED(ipFClass->FindField(a_bFldStr[roop], &a_lFldIndx[roop]))) {
			PrnMsg( e_fErr_Fp, "%d\t属性フィールドのインデックス取得に失敗\n", e_iCurMeshCode );
			return	( false );
		}
	}

	ims_data_info	a_iIndx;
	for( a_iIndx = c_mNP_Area_Data.begin(); a_iIndx != c_mNP_Area_Data.end(); a_iIndx++ )
	{
		IFeatureBufferPtr	ipFeatureBuffer;
		if( FAILED(ipFClass->CreateFeatureBuffer(&ipFeatureBuffer)) ) {
			PrnMsg( e_fErr_Fp, "%d\tフィーチャバッファ生成に失敗\n", e_iCurMeshCode );
			return	( false );
		}

		data_info	a_dCurDat;

		a_dCurDat	= *a_iIndx;

		// 属性付与
		ipFeatureBuffer->putref_Shape(a_dCurDat.e_ipGeo);
		if (FAILED(ipFeatureBuffer->put_Value(a_lFldIndx[0], CComVariant(a_dCurDat.e_iAttrCode))) ) {
			PrnMsg( e_fErr_Fp, "%d\t属性追加に失敗\n", e_iCurMeshCode);
			return	( false );
		}
	
		_variant_t va;
		if( FAILED(ipFeatureCursor->InsertFeature(ipFeatureBuffer, &va)) ) {
			PrnMsg( e_fErr_Fp, "%d\tフィーチャ追加に失敗\n", e_iCurMeshCode );
			return	( false );
		}
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
	e_eExeTime.Set_Start();

	if( eExe_Flag & ON_PGDB_MODE ){

		// パーソナルジオDB
		PrnMsg( stdout, "駐車違反取締エリアPGDB<%s>をオープン・・\n", e_cpPGDB_File ); 
		e_ipWorkspace_NP_AREA = DB_Open( e_cpPGDB_File );
		if( ! e_ipWorkspace_NP_AREA )	{
			PrnMsg( e_fErr_Fp, "パーソナルジオDBがオープンできませんでした\n"); 
			return	( false );
		}else {
			PrnMsg( stdout, "接続完了\n");
		}

	}else {

		// SDE　※未使用
		e_sPrnInfo.Prn_Info	( "駐車違反ポリゴンデータオープン・・\n", stdout );
		e_ipWorkspace_NP_AREA = DB_Open( e_cpInit_Str_NP_AREA );
		if( ! e_ipWorkspace_NP_AREA ) {
			e_sPrnInfo.Prn_Info	( "接続出来なかった\n", stderr );
			return	( false );
		} else {
			PrnMsg	( stdout, "接続完了\n" );
		}	

	}

	// メッシュ用のSDE
	e_sPrnInfo.Prn_Info	( "メッシュDBオープン・・\n", stdout );
	e_ipWorkspace_Mesh = DB_Open( e_cpInit_Str_MESH );
	if( ! e_ipWorkspace_Mesh ) {
		e_sPrnInfo.Prn_Info	( "接続出来なかった\n", stderr );
		return	( false );
	} else {
		PrnMsg	( stdout, "接続完了\n" );
	}	

	// DB接続にかかった時間の表示
	PrnMsg	( e_fErr_Fp,	"SiNDY-DB接続処理時間 = %0.2f(s)でした\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );

	// メッシュ読み込み
	if( !Read_Mesh_List() ) {
		e_sPrnInfo.Prn_Info	( "メッシュ読み込み失敗\n");
		return	( false );
	}

	// レイヤオープン
	if( !Open_Mesh_Layers() ) {
		e_sPrnInfo.Prn_Info	( "メッシュレイヤオープンに失敗しています\n", stderr );
		return	( false );
	}

	if( !Open_Layers ( ) ){	// 地図用ワークスペース
		e_sPrnInfo.Prn_Info	( "駐車違反取締エリアレイヤオープンに失敗しています\n", stderr );
		return	( false );
	}

	// データ読み込み
	if( !Read_NP_AREA_to_Shape ( ) ) {
		e_sPrnInfo.Prn_Info	( "データ処理に失敗しています\n", stderr );
		return	( false );
	}

	//　実行部終了
	// 終了時間の取得
	PrnMsg	( e_fErr_Fp,	"データチェック総時間 = %0.2f(s)でした\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );
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
	char*			a_cpList_File	= NULL;		//	リストファイル名
	if( eExe_Flag & ON_MESH || eExe_Flag & ON_MONO_MESH ) {		//	メッシュコードリスト指定の場合

		if( eExe_Flag & ON_MESH ) {
			a_cpList_File	= Ret_Mesh_File ( );
			if( a_cpList_File == NULL ) {
				PrnMsg	( stderr, "メッシュコードリストファイル名がとりだせませんでした[check_exe]\n");
				return	( false );
			}	
			if( e_sInput_List.read_mesh_list ( a_cpList_File ) != 0 ) {	// ここでリストを読み込む
				PrnMsg	( stderr, "メッシュコードリストファイル<%s>の読み込みに失敗[check_exe]\n", a_cpList_File );
				return	( false );
			}
		}else {
			if( e_sInput_List.read_mesh_list ( e_iMeshCode ) != 0 ) {	// ここでメッシュを読み込む
				PrnMsg( e_fErr_Fp, "メッシュ<%d>の設定に失敗[check_exe]\n", e_iMeshCode );
				return	( false );
			}
		}

	}
	return	( true );
}
