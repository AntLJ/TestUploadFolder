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
#include <TDC/useful_headers/flush_scope_guard.h>

//	2003.07.04	main_rootin.hから定義を分離

string	main_rootin::
Str_to_SJIS		(	CComBSTR&	c_CStr	)		// 文字列(Unicode)
{
	string		ret_str;

	if (*c_CStr != 0) {
		_bstr_t a_bTmpStr(c_CStr);
		ret_str = a_bTmpStr;
	}
	return	(ret_str);
}

/** ------------------------------------------------------------------------------
	SiNDYDBへの接続オープン
	@return	true	成功
	@return	false	失敗
--------------------------------------------------------------------------------*/
bool	main_rootin::
SiNDY_DB_Open	(IWorkspacePtr&			c_ipWorkspace_Addr	)
{
	// 住所DB
	if( !DB_Open	( c_ipWorkspace_Addr,
					  e_pTool_Init.get_UserName_Address(), e_pTool_Init.get_Password_Address(),
					  e_pTool_Init.get_Instance_Address(), e_pTool_Init.get_Server_Address(),
					  e_pTool_Init.get_Version_Address() )) {
		fprintf	( stderr, "接続出来なかった\n");
		return	( false );
	}else {
		fprintf	( stdout, "接続完了\n" );
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
			char*					c_cpInstance,		///< インスタンス
			char*					c_cpServer,			///< サーバ名
			char*					c_cpVersion	)		///< バージョン
{
	USES_CONVERSION;	// OLE2T 等の一部のマクロを使用する場合には必ず必要

	

	CString connectStr;
	connectStr.Format(_T("%s/%s/%s/%s/%s"),	c_cpUserName, c_cpPassword,	c_cpVersion, c_cpInstance, c_cpServer );
	printf(_T(	"接続プロパティ：<%s>\n"),	connectStr);
	
	c_ipWorkspace = sindy::create_workspace(connectStr);

	// DBへ接続
	printf(_T("接続中..."));
	if( !c_ipWorkspace ) {
		return	( false );
	}else {
		return	( true );
	}
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
			fprintf	( stderr, "エラーログファイル<%s>がオープンできない[main_rootin]\n", e_cpLog_File );
			return	( false );
		}
	}
	return	( true );
}

/** ---------------------------------------------------------------------------
//	エラーログのヘッダを出力する
//	@return	無し
-----------------------------------------------------------------------------*/
void	main_rootin::
Output_Log_Header	( void )
{
	fprintf	( e_fErr_Fp,		"#SINDYSTDLOG\n" );
	fprintf	( e_fErr_Fp,		"#ツールバージョン          : <%s>\n", e_sVersion.c_str() );
	if( eExe_Flag & ON_PGDB_MODE ) {
		fprintf	( e_fErr_Fp,	"#接続情報(PGDB)            : <%s>\n",				e_cpPGDB_File );
	}else {
		fprintf	( e_fErr_Fp,	"#接続情報(DB_ADDRESS)      : <%s/%s/%s/%s/%s>\n",	e_pTool_Init.get_UserName_Address(), e_pTool_Init.get_Password_Address(),
																					e_pTool_Init.get_Version_Address(),	e_pTool_Init.get_Instance_Address(),
																					e_pTool_Init.get_Server_Address() );
	}
	fprintf	( e_fErr_Fp,		"#行政界ポリゴンレイヤ      : <%s>\n", e_sCheck_Layer );
	fprintf	( e_fErr_Fp,		"#号ポイントレイヤ          : <%s>\n", e_sGou_Layer );
}

#define	OPTION_MAX	8
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
//	int		digit		= 0;	// メッシュの桁数

	//-----------------------------------------------------------------------
	// ツールバージョン表示 2008.11.12
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

//	if(argc < 3) {
//		return( false );
//	}

//	e_cpInit_File		= argv[1];	///< 初期化ファイル(住所DB接続)も含む
//	e_sCheck_Layer		= argv[2];	///< チェックレイヤ（注記）
//	e_sGou_Layer		= argv[3];	///< 号ポイントレイヤ

	// オプションがなにも無いとき
//	if( argc == 3 ) {
//		return	( true );
//	}

	// オプションだから、今のところ使用しない
	for(i = 1; i < argc;)
	{
		if(strcmp(argv[i], "-l") == 0) { // ログファイル設定（設定が無い場合は、エラーはstderrへ垂れ流しになる）
			if(argc < i+2) return( false );
			e_cpLog_File	= argv[i+1];	///< メッシュコードリストファイル
			a_optcount[0]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-code") == 0) {
			if(argc < i+2) return( false );
			a_exeflag	|= ON_MONO_ADCODE;
			e_cpAddrCode	= argv[i+1];	
			a_optcount[1]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-t") == 0) {
			if( argc < i+2) return( false );
			e_cpCode_List_Txt	= argv[i+1];
			a_exeflag	|= ON_CODE_LIST_TXT;
			a_optcount[2]++;
			i+= 2;
		}else
		if(strcmp(argv[i], "-v") == 0) {
			//if(argc < i+1) return( false );
			print_version();
			a_optcount[3]++;
			i+=1;
			return	( false );	// 仮にこうする？
		}else
		if(strcmp(argv[i], "-pgdb") == 0) {
			if(argc < i+2) return( false );
			e_cpPGDB_File	= argv[i+1];
			a_exeflag		|= ON_PGDB_MODE;
			WCARD1			= "\?";
			a_optcount[4]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-i") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_INIT_FILE;
			e_cpInit_File	= argv[i+1];	
			a_optcount[5]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-cadm") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_CITY_ADMIN;
			e_sCheck_Layer	= argv[i+1];	
			a_optcount[6]++;
			i+=2;
		}else
		if(strcmp(argv[i], "-gou") == 0) {
			if(argc < i+2) return( false );
			a_exeflag		|= ON_GOU_POINT;
			e_sGou_Layer	= argv[i+1];	
			a_optcount[7]++;
			i+=2;
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

	// 2008.11.28 環境変数に対応
	int	a_iInit_Ret	= 0;
	if( a_exeflag & ON_INIT_FILE ) {
		a_iInit_Ret	= e_pTool_Init.read_init_file ( Ret_Init_File() );
	}else {
		a_iInit_Ret	= e_pTool_Init.read_init_env ( );
	}
	if( a_iInit_Ret != 0 ) {
		fprintf ( stderr, "初期化ファイル<%s>のオープンに失敗しました\n", Ret_Init_File() );
		return	( false );
	}

	// オプション指定が無い場合は、環境変数から取得してみる
	if( !(a_exeflag & ON_CITY_ADMIN) ) {
		e_sCheck_Layer	= e_pTool_Init.get_City_Admin();
	}
	if( !(a_exeflag & ON_GOU_POINT) ) {
		e_sGou_Layer	= e_pTool_Init.get_Gou_Point();
	}

	eExe_Flag	= a_exeflag;
//	return		( a_exeflag );
	return		( true );

}

/** ------------------------------------------------------------------------------
// 使用法表示
--------------------------------------------------------------------------------*/
void		main_rootin::
print_usage	(char **argv)
{
	fprintf(	stdout,
				"=== 【住所変換モードチェックツール】(%s) ===\n"
				"<usage>\n"
				"%s [オプション...]\n"
				"\t<オプション>\n"
				"\t      '-i'              : 設定ファイル名\n"
				"\t      '-cadm'           : 行政界レイヤ名(ex. SiNDY2008B.City_Admin)\n"
				"\t      '-gou'            : 号ポイントレイヤ名(ex. SiNDY2008B.Gou_Point)\n"
				"\t      '-l'              : ログファイル名を指定\n"
				"\t      '-code'           : 住所コードを指定して実行（8桁、11桁のみ対応）（ex 20209001101)\n"
				"\t      '-t'              : 出力11桁コードテキストファイル（ex .\\tmp\\変換モード11桁.txt）\n"
				"\t                        　※指定しない場合は標準出力に出力する\n"
				"\t      '-pgdb'           : 指定PGDBのデータに対してチェックを実行\n"
				"\t      '-v'              : バージョン情報表示\n"
				"\t[環境変数]\n"
				"\t  <共通>\n"
				"\t      DB_ADDRESS        : <ユーザ名>/<パスワード>/<バージョン>/<インスタンス>/<サーバ>\n"
				"\t      FC_CITY_ADMIN     : 都市地図行政界FC\n"
				"\t      FC_GOU_POINT      : 号ポイントFC\n"
				"\t  ※上記の環境変数での設定は'-i'オプションでも同様に設定できます\n"
			//	"\t      '-m'           : メッシュコードリストファイルを指定\n"
			//	"\t      '-M'           : メッシュコードを指定（1次、2次、8桁メッシュ）\n"
			//	"\t      '-w'           : Where句を指定（例：\"OBJECTID = 1192\" ）\n"
			//	"\t<補足>\n"
			//	"\t      ・'-m','-M','-w'は同時指定できません\n"
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
				"=== 【住所変換モードチェックツール】(%s) ===\n"
				"\t<バージョン情報>\n"
				"\t2004/05/20        1.00               ・ 公開バージョン\n"
				"\t2005/05/11        1.10               ・ 8桁一致する住所において住所変換モードのフラグが\n"
				"\t                                        立っているものがあるにもかかわらずフラグが立って\n"
				"\t                                        いない11桁住所がある場合をエラーとしてチェックす\n"
				"\t                                        るように修正。\n"
				"\t2006/08/01        1.20               ・Arc9.1に対応\n"
				"\t2008/12/03        2.0.0.1            ・ VC8への移行が未対応だったので対応\n"
				"\t                                     ・ ツールバージョン情報追加\n"
				"\t                                     ・ ログヘッダ情報追加\n"
				"\t                                     ・ PGDBに対応('-pgdb'オプション)\n"
				"\t                                     ・ 環境変数での設定に対応('-g','-ca'追加)\n"
				"\t                                     ・ [Bug 6543]への対応。(住所変換モードを「大字も生成」\n"
				"\t                                        にしていない行政界もエラー対象となる)\n"
				"\t                                     ・ ログに号タイプの情報を追加\n"
				"\t2008/12/08        2.0.0.2            ・ [Bug 6543]への再修正(9～11桁目が'000'をチェック対象に)\n"
				"\t2008/12/16        2.0.0.3            ・ [Bug 6543]への再修正(「街区符号」「拡張街区符号」を表示)\n"
				"\t2010/09/17        2.1.0.4            ・ [bug 8229]への対応(加除コード移行に対応して欲しい)\n"
				"\t                                        ※エラーメッセージもADDRCODE2への変更にあわせて変更\n"
				"\t2010/10/29        2.1.1.5            ・ [bug 8338]への対応(Addrconvmode_chkを加除仕様にして欲しい)\n"

				

//				"\t2004/03/25        1.00               : 実運用バージョン\n"
				, e_sVersion.c_str());
}

/** ------------------------------------------------------------------------------
	Where()句を取得する
--------------------------------------------------------------------------------*/
string	main_rootin::
Get_Where_String(	char*	c_cpLayerName )		// レイヤ名
{
	string	a_sRet_Str	= "";
	if( c_cpLayerName == NULL ) {
		a_sRet_Str	= "";
		return	( a_sRet_Str );
	}

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

	//------------------------------------------------------
	// 他の属性値でのチェック	2003.07.29
	//------------------------------------------------------
	// レイヤ名で接続WSを判定
	string		a_sLayerName	= a_cpLayer;
	if( a_sLayerName == "City_Admin"	)	///< 都市地図行政界
	{
		a_sRet_Str	= "AREACLASS_C <> 2";
	}else {	// 行政界以外
		a_sRet_Str	= "";
	}

	if( a_cpLayerTmp != NULL ) {
		delete [] a_cpLayerTmp;
	}
	return ( a_sRet_Str );
}

/** ------------------------------------------------------------------------------
	レイヤオープン
--------------------------------------------------------------------------------*/
bool	main_rootin::
Open_Layers		( 	IWorkspacePtr	ipWorkspace_Addr )	// 住所用ワークスペース
			//		IWorkspacePtr	ipWorkspace_Map )	// 地図用ワークスペース
{
	char*	a_cpCity_Admin_Layer	= NULL;	// 都市地図行政界レイヤ名
	char*	a_cpGou_Point_Layer		= NULL;	// 号ポイントレイヤ名

	a_cpCity_Admin_Layer	= e_sCheck_Layer;
	a_cpGou_Point_Layer		= e_sGou_Layer;

	if( !e_mCity_Admin_WS.Init ( ipWorkspace_Addr, a_cpCity_Admin_Layer ) ) {
		fprintf	( stderr, "都市地図行政界レイヤ<%s>の読み込みに失敗\n", a_cpCity_Admin_Layer );
		return	( false );
	}
	if( !e_mGou_Point_WS.Init ( ipWorkspace_Addr, a_cpGou_Point_Layer ) ) {
		fprintf	( stderr, "号ポイントレイヤ<%s>の読み込みに失敗\n", a_cpGou_Point_Layer );
		return	( false );	
	}
	return	( true );
}

/**
	行政コードリストををセットに落とす
	@return	データ数
*/
int		main_rootin::
Set_AdminCode_Data (	IFeatureCursorPtr&	c_ipAdminCursor )	///< 行政界カーソル
{
	int	a_iCount	= 0;
	// データをマルチセットのデータに落とす
	if( c_ipAdminCursor != NULL ) {
		IFeaturePtr	a_ipNote_Feat;
		// 検索された注記のループ
		for(;;)
		{
			data_info	a_dAdmin_Data;
			if( c_ipAdminCursor->NextFeature ( &a_ipNote_Feat ) == S_OK ) {

				Feature_All_Attrs	a_fNote_Attrs;
				a_fNote_Attrs.Init	( a_ipNote_Feat );
				// オブジェクトID
				Feature_Attr		a_fObjID;
				a_fObjID			= a_fNote_Attrs.Get_Attr ( "OBJECTID" );
				// 市区町村コード
				Feature_Attr		a_fCityCode;
				a_fCityCode			= a_fNote_Attrs.Get_Attr ( "CITYCODE" );
				// 住所コード
				Feature_Attr		a_fAddrCode;
				a_fAddrCode			= a_fNote_Attrs.Get_Attr ( "ADDRCODE" );
				// 住所コード2
				Feature_Attr		a_fAddrCode2;
				a_fAddrCode2		= a_fNote_Attrs.Get_Attr ( "ADDRCODE2" );
				// 変換タイプ
			//	Feature_Attr		a_fConvCode;
			//	a_fConvCode			= a_fNote_Attrs.Get_Attr ( "CONVTYPE_C" );

				a_dAdmin_Data.e_iOID		= a_fObjID.e_ccvAttr.intVal;
				a_dAdmin_Data.e_sCityCode	= Str_to_SJIS ((CComBSTR&)a_fCityCode.e_ccvAttr.bstrVal);
				a_dAdmin_Data.e_sAddrCode	= Str_to_SJIS ((CComBSTR&)a_fAddrCode.e_ccvAttr.bstrVal);
				a_dAdmin_Data.e_sAddrCode2	= Str_to_SJIS ((CComBSTR&)a_fAddrCode2.e_ccvAttr.bstrVal);
			//	a_dAdmin_Data.e_iConvMode	= a_fConvCode.e_ccvAttr.intVal;

				e_mAdmin_Datas.insert( a_dAdmin_Data );		///< 変換モード確認
				e_mAdmin_Datas_Uniq.insert( a_dAdmin_Data );///< 11桁ユニーク

				string	a_sCode_8;	// 8桁コード
				a_sCode_8	= a_dAdmin_Data.e_sCityCode;
				char	a_cpCode_3[4];
				memset	( a_cpCode_3, '\0', 4 );
				strncpy ( a_cpCode_3, &a_dAdmin_Data.e_sAddrCode[0], 3 );
				a_sCode_8	+= a_cpCode_3;
				e_sCode_8.insert ( a_sCode_8 );

				a_iCount++;
			}else {
				break;
			}
		}
	}
	return	( a_iCount );
}

/** ------------------------------------------------------------------------------
	11桁コードリストの出力
--------------------------------------------------------------------------------*/
void	main_rootin::
Print_Code_List	( void )
{
	fprintf	( stdout, "11桁コードリスト作成中・・・\n" );
	FILE*	a_fpOutput	= NULL;
	if( eExe_Flag & ON_CODE_LIST_TXT ) {
		a_fpOutput	= fopen ( e_cpCode_List_Txt, "w" );
		if( a_fpOutput == NULL ) {
			fprintf	(	stderr, "コードリスト出力ファイル<%s>がオープンできません！標準出力に出力します\n",
						e_cpCode_List_Txt );
			a_fpOutput	= stdout;
		}
	}else {
		a_fpOutput	= stdout;
	}

	is_data_info	a_iIndx;
	for( a_iIndx = e_mAdmin_Datas_Uniq.begin(); a_iIndx != e_mAdmin_Datas_Uniq.end(); a_iIndx++ )
	{
		data_info	a_dCurDat	= *a_iIndx;
		fprintf	( a_fpOutput, "%s%s\n", &a_dCurDat.e_sCityCode[0], &a_dCurDat.e_sAddrCode[0] );  
	}

	if( a_fpOutput != stderr ) {
		if( e_cpCode_List_Txt == NULL ) {
			fprintf	( stdout, "11桁コードをstdoutに出力しました\n" );
		}else {
			fprintf	( stdout, "11桁コードをファイル<%s>に出力しました\n", e_cpCode_List_Txt );
		}
		fclose	( a_fpOutput );
	}
}

/** ------------------------------------------------------------------------------
	変換モードコードエラーのチェック
--------------------------------------------------------------------------------*/
void	main_rootin::
Chk_ConvMode	( void )
{
	char	a_cpMsg[256];	// メッセージ用

	e_sPrnInfo.Set_Layer_Name	( e_sCheck_Layer );

	ims_data_info	a_iIndx;
	for( a_iIndx = e_mAdmin_Datas.begin(); a_iIndx != e_mAdmin_Datas.end(); a_iIndx++ )
	{
		data_info	a_dDat	= *a_iIndx;

		e_sPrnInfo.Set_OID ( a_dDat.e_iOID );

		int		a_iCode_9_11	= 0;

		a_iCode_9_11	= atoi ( &a_dDat.e_sAddrCode[3] );	// 2004.05.14 CityCodeになってたので修正
		
		int		a_iEStat	= 0;

		// [bug 8338] ③対応のため追加
		if( a_dDat.e_sAddrCode2 != "000000" &&
			a_dDat.e_sAddrCode == a_dDat.e_sAddrCode2 )
		{
			sprintf	( a_cpMsg, "ADDRCODEとADDRCODE2が同じコード\t%s%s", a_dDat.e_sCityCode.c_str(), a_dDat.e_sAddrCode.c_str() );
			e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_ERROR );
			e_sPrnInfo.Set_Err_Msg		( a_cpMsg );
			e_sPrnInfo.Prn_SiNDY_Err	( 0, NULL, stderr );
		}

		// [bug 8338] ①の対応のため、コメントアウト
//	//	if( a_dDat.e_iConvMode == 1 &&
//		if( a_dDat.e_sAddrCode2 != "000000" &&
//	//		!(101 <= a_iCode_9_11 && a_iCode_9_11 <= 850) ) {
//			!(101 <= a_iCode_9_11 && a_iCode_9_11 <= 899) ) {
//	//		sprintf	( a_cpMsg, "９～１１桁のコードがおかしい\t%s%s\t変換モード%d",
//	//				  &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0], a_dDat.e_iConvMode );
//			sprintf	( a_cpMsg, "９～１１桁のコードがおかしい\t%s%s\tADDRCODE2\t<%s>",
//					  &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0], a_dDat.e_sAddrCode2.c_str() );
//			e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_ERROR );
//			e_sPrnInfo.Set_Err_Msg		( a_cpMsg );
//			e_sPrnInfo.Prn_SiNDY_Err	( 0, NULL, stderr );
//
//		}
		// 変換モード2は無いのでコメントアウト 2010.09.15
	//	else
	//	if( a_dDat.e_iConvMode == 2 ) {
	//		sprintf	( a_cpMsg, "変換モードが%d\t%s%s", a_dDat.e_iConvMode, &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0] );
	//		e_sPrnInfo.Set_Err_Level	( smp_sindy_err_info::EC_WARNING);
	//		e_sPrnInfo.Set_Err_Msg		( a_cpMsg );
	//		e_sPrnInfo.Prn_SiNDY_Err	( 0, NULL, stderr );
	//	}
	}

	// エラーレベルを戻す
	e_sPrnInfo.Set_Err_Level ( smp_sindy_err_info::EC_ERROR );
}

/** ------------------------------------------------------------------------------
	11桁が一致する行政界の中で、変換モードコードが違うものをチェックする
--------------------------------------------------------------------------------*/
void	main_rootin::
Chk_SameCode11Admin ( void )
{
	char	a_cpMsg[256];	// メッセージ用

	e_sPrnInfo.Set_Layer_Name	( e_sCheck_Layer );

	is_data_info	a_iIndx;
	
	fprintf ( stdout, "11桁が一致する行政界をチェックします\n" );

	for( a_iIndx = e_mAdmin_Datas_Uniq.begin(); a_iIndx != e_mAdmin_Datas_Uniq.end(); a_iIndx++ )
	{
		data_info	a_dDat	= *a_iIndx;

		fprintf	( stdout, "行政コード<%s%s>変換モードチェック中\n", &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0] );

		// 住所データの中で、変換モードが1と2のものを取ってくる
		char	a_cpWhereStr[128];
//		sprintf	( a_cpWhereStr, "CITYCODE = '%s' AND ADDRCODE = '%s' AND CONVTYPE_C <> %d",
//				  &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0], a_dDat.e_iConvMode );
		sprintf	( a_cpWhereStr, "CITYCODE = '%s' AND ADDRCODE = '%s' AND ADDRCODE2 = '000000'",
				  &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0] );
		string	a_sWhere_Str= a_cpWhereStr; 
		IFeatureCursorPtr	a_ipAdminCursor;

		a_ipAdminCursor	= e_mCity_Admin_WS.Search_Where ( a_sWhere_Str, RET_FCURSOR );
		if ( a_ipAdminCursor == NULL ) {
			e_sPrnInfo.Prn_Info	( "指定住所コードの行政界が取得できませんでした\n", stderr );
			continue;
		}
		int	i = 0;
		IFeaturePtr		a_ipCity_Admin_Feat;
		for (;;i++)
		{
			if( a_ipAdminCursor->NextFeature ( &a_ipCity_Admin_Feat ) == S_OK ) {
				// フィーチャの属性を取り出す
				Feature_All_Attrs	a_fAdmin_Attrs;
				a_fAdmin_Attrs.Init( a_ipCity_Admin_Feat );
			
				// オブジェクトID
				Feature_Attr	a_fObjID;
				a_fObjID		= a_fAdmin_Attrs.Get_Attr ( "OBJECTID" );

			//	Feature_Attr	a_fConvCode;
			//	a_fConvCode		= a_fAdmin_Attrs.Get_Attr ( "CONVTYPE_C" );	// 変換タイプ

				Feature_Attr	a_fAddrCode2;
				a_fAddrCode2	= a_fAdmin_Attrs.Get_Attr ( "ADDRCODE2" );

				string	a_sAddrCode2;
				a_sAddrCode2	= Str_to_SJIS ((CComBSTR&)a_fAddrCode2.e_ccvAttr.bstrVal);

				e_sPrnInfo.Set_OID ( a_fObjID.e_ccvAttr.intVal );

			//	if( a_dDat.e_iConvMode != a_fConvCode.e_ccvAttr.intVal ) {
				if( a_sAddrCode2 == "000000" ) {
				//	sprintf	( a_cpMsg, "同一11桁コードなのに変換モードが違う\tモード%d\t%s%s",
				//			  a_fConvCode.e_ccvAttr.intVal, &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0] );
					sprintf	( a_cpMsg, "同一11桁コードなのに変換モードが違う\t%s%s", &a_dDat.e_sCityCode[0], &a_dDat.e_sAddrCode[0] );
					e_sPrnInfo.Set_Err_Level	( smp_sindy_err_info::EC_ERROR);
					e_sPrnInfo.Set_Err_Msg		( a_cpMsg );
					e_sPrnInfo.Prn_SiNDY_Err	( 0, NULL, stderr );
				}
			}else {
				break;
			}
		}
	}
}

/**
	8桁が一致する行政界の中で、変換モードが違うものをチェックする
*/
void	main_rootin::
Chk_SameCode8Admin_Mode ( void )
{
	char	a_cpMsg[256];	// メッセージ用

	e_sPrnInfo.Set_Layer_Name	( e_sCheck_Layer );

	
	fprintf ( stdout, "8桁が一致する行政界をチェックします\n" );
	is_string	a_iIndx;
	for( a_iIndx = e_sCode_8.begin(); a_iIndx != e_sCode_8.end(); a_iIndx++ )
	{
		string	a_sDat	= *a_iIndx;

		fprintf	( stdout, "行政コード<%s>変換モードチェック中\n", &a_sDat[0]);

		char		a_cpCode_5[6], a_cpCode_3[4];
		memset	( a_cpCode_5, '\0', 6 );
		memset	( a_cpCode_3, '\0', 4 );

		strncpy	( a_cpCode_5, &a_sDat[0], 5 );
		strncpy	( a_cpCode_3, &a_sDat[5], 3 );

		// 住所データの中で、変換モードが1と2のものを取ってくる
		char	a_cpWhereStr[128];
		sprintf	( a_cpWhereStr, "CITYCODE = '%s' AND ADDRCODE LIKE '%s%s%s%s'", a_cpCode_5, a_cpCode_3, WCARD1, WCARD1, WCARD1 );
		string	a_sWhere_Str= a_cpWhereStr; 
		IFeatureCursorPtr	a_ipAdminCursor;

		a_ipAdminCursor	= e_mCity_Admin_WS.Search_Where ( a_sWhere_Str, RET_FCURSOR );
		if ( a_ipAdminCursor == NULL ) {
			e_sPrnInfo.Prn_Info	( "指定住所コードの行政界が取得できませんでした\n", stderr );
			continue;
		}

		int	i = 0;
		IFeaturePtr		a_ipCity_Admin_Feat;
		for (;;i++)
		{
			if( a_ipAdminCursor->NextFeature ( &a_ipCity_Admin_Feat ) == S_OK ) {
				// フィーチャの属性を取り出す
				Feature_All_Attrs	a_fAdmin_Attrs;
				a_fAdmin_Attrs.Init( a_ipCity_Admin_Feat );
			
				// オブジェクトID
				Feature_Attr	a_fObjID;
				a_fObjID		= a_fAdmin_Attrs.Get_Attr ( "OBJECTID" );

			//	Feature_Attr	a_fConvCode;
			//	a_fConvCode		= a_fAdmin_Attrs.Get_Attr ( "CONVTYPE_C" );	// 変換タイプ

				// 市区町村コード
				Feature_Attr	a_fCityCode;
				a_fCityCode		= a_fAdmin_Attrs.Get_Attr ( "CITYCODE" );
				// 住所コード
				Feature_Attr	a_fAddrCode;
				a_fAddrCode		= a_fAdmin_Attrs.Get_Attr ( "ADDRCODE" );

				Feature_Attr	a_fAddrCode2;
				a_fAddrCode2	= a_fAdmin_Attrs.Get_Attr ( "ADDRCODE2" );

				string	a_sCityCode, a_sAddrCode, a_sAddrCode2;
				a_sCityCode	= Str_to_SJIS ((CComBSTR&)a_fCityCode.e_ccvAttr.bstrVal);
				a_sAddrCode	= Str_to_SJIS ((CComBSTR&)a_fAddrCode.e_ccvAttr.bstrVal);
				a_sAddrCode2= Str_to_SJIS ((CComBSTR&)a_fAddrCode2.e_ccvAttr.bstrVal);

				e_sPrnInfo.Set_OID ( a_fObjID.e_ccvAttr.intVal );

				//下３桁の値を取り出す
				int		a_iCode_9_11	= 0;
				a_iCode_9_11	= atoi ( &a_sAddrCode[3] );

				// 住所変換モードが1じゃなくて下３桁が'101'～'850'の場合
			//	if( a_fConvCode.e_ccvAttr.intVal != 1 &&
				if( (a_sAddrCode2 == "000000" ) &&
				//	(101 <= a_iCode_9_11 && a_iCode_9_11 <= 850) ) {
					(101 <= a_iCode_9_11 && a_iCode_9_11 <= 899) ) {
				//	sprintf	( a_cpMsg, "8桁一致する住所において住所変換モードのフラグが立っていない\tモード%d\t%s%s",
				//		a_fConvCode.e_ccvAttr.intVal, &a_sCityCode[0], &a_sAddrCode[0] );
					sprintf	( a_cpMsg, "8桁一致する住所においてADDRCODE2に値が入っていない\t%s%s", &a_sCityCode[0], &a_sAddrCode[0] );
					e_sPrnInfo.Set_Err_Level	( smp_sindy_err_info::EC_ERROR);
					e_sPrnInfo.Set_Err_Msg		( a_cpMsg );
					e_sPrnInfo.Prn_SiNDY_Err	( 0, NULL, stderr );
				}
			}else {
				break;
			}
		}
	}
}

/** ------------------------------------------------------------------------------
// 行政界のリストを取得する（ジオメトリ含む）
--------------------------------------------------------------------------------*/
void	main_rootin::
Set_Admin_List	( IFeatureCursorPtr&	c_ipAdminCursor,
				  v_admin_info&			c_vAdmLists )
{
	IFeaturePtr		a_ipCity_Admin_Feat;

	int	i = 0;
	for (;;i++)
	{
		if( c_ipAdminCursor->NextFeature ( &a_ipCity_Admin_Feat ) == S_OK ) {

			// フィーチャの属性を取り出す
			Feature_All_Attrs	a_fAdm_Attrs;
			a_fAdm_Attrs.Init( a_ipCity_Admin_Feat );

			// オブジェクトID
			Feature_Attr		a_fAdminObjID;
			a_fAdminObjID		= a_fAdm_Attrs.Get_Attr ( "OBJECTID" );
			// 市区町村コード
			Feature_Attr		a_fCityCode;
			a_fCityCode			= a_fAdm_Attrs.Get_Attr ( "CITYCODE" );
			// 住所コード
			Feature_Attr		a_fAddrCode;
			a_fAddrCode			= a_fAdm_Attrs.Get_Attr ( "ADDRCODE" );
			// 住所コード2
			Feature_Attr		a_fAddrCode2;
			a_fAddrCode2		= a_fAdm_Attrs.Get_Attr ( "ADDRCODE2" );
			// 街区符号
			Feature_Attr		a_fGaiku;
			a_fGaiku			= a_fAdm_Attrs.Get_Attr	( "GAIKUFUGO");		// 2008.12.16 [Bug 6543]対応
			// 拡張街区符号
			Feature_Attr		a_fExtGaiku;
			a_fExtGaiku			= a_fAdm_Attrs.Get_Attr	( "EXTGAIKUFUGO");	// 2008.12.16 [Bug 6543]対応

			string	a_sCityCode, a_sAddrCode, a_sAddrCode2;
			a_sCityCode	= Str_to_SJIS ((CComBSTR&)a_fCityCode.e_ccvAttr.bstrVal);
			a_sAddrCode	= Str_to_SJIS ((CComBSTR&)a_fAddrCode.e_ccvAttr.bstrVal);
			a_sAddrCode2= Str_to_SJIS ((CComBSTR&)a_fAddrCode2.e_ccvAttr.bstrVal);

			IGeometryPtr		a_ipAdminGeo;

			if( FAILED(a_ipCity_Admin_Feat->get_Shape ( &a_ipAdminGeo )) ) {	// フィーチャから
				fprintf	(stderr, "シェイプの取得に失敗\n");
				continue;
			}

			admin_info		a_aDat;
			
			a_aDat.e_iOID	= a_fAdminObjID.e_ccvAttr.intVal;
			a_aDat.e_ipGeom	= a_ipAdminGeo;
			a_aDat.e_sCode1	= a_sCityCode + a_sAddrCode;
			a_aDat.e_sCode2	= a_sCityCode + a_sAddrCode2;

			//------------------------------------------------------------------------
			// 2008.12.16 [Bug 6543]対応
			if( a_fGaiku.Is_Null() ) {
				a_aDat.e_lGaiku	= -1;
			}else {
				a_aDat.e_lGaiku	= a_fGaiku.e_ccvAttr.intVal;
			}
			if( a_fExtGaiku.Is_Null() ) {
				a_aDat.e_sExtGaiku	= "";
			}else {
				a_aDat.e_sExtGaiku	= Str_to_SJIS ((CComBSTR&)a_fExtGaiku.e_ccvAttr.bstrVal);
			}
			//------------------------------------------------------------------------

			c_vAdmLists.push_back ( a_aDat );
		
		}else {
			break;
		}
	}
}

/** ------------------------------------------------------------------------------
	号ポイントのリストを作成する
--------------------------------------------------------------------------------*/
int		main_rootin::
Set_Gou_Datas	( v_admin_info&			c_vAdmLists, 		///< 行政界
				  ms_gou_info&			c_mGou_Infos )		///< 取り出した号データ
{

	IFeaturePtr		a_ipCity_Admin_Feat;

	// 号番号がNULLのものは除く
	char	a_cpWhereStr[128];
	sprintf	( a_cpWhereStr,	"GOU_NO IS NOT NULL" );
	string	a_sWhere_Str= a_cpWhereStr; 

	iv_admin_info	a_iIndx;
	for( a_iIndx = c_vAdmLists.begin(); a_iIndx != c_vAdmLists.end(); a_iIndx++ )
	{
		admin_info	a_aAdmDat	= *a_iIndx;
	
			
		IFeatureCursorPtr	a_ipGouCursor;

		a_ipGouCursor	= e_mGou_Point_WS.Search_Feature ( a_aAdmDat.e_ipGeom, esriSpatialRelContains, a_sWhere_Str, RET_FCURSOR );
		if ( a_ipGouCursor == NULL ) {
			e_sPrnInfo.Prn_Info	( "指定住所コードの行政界が取得できませんでした\n", stderr );
			continue;
		}

		IFeaturePtr		a_ipGou_Feat;
		int	j = 0;
		for (;;j++)
		{
			if( a_ipGouCursor->NextFeature ( &a_ipGou_Feat ) == S_OK ) {
				
				// フィーチャの属性を取り出す
				Feature_All_Attrs	a_fGou_Attrs;
				a_fGou_Attrs.Init( a_ipGou_Feat );
			
				// オブジェクトID
				Feature_Attr	a_fObjID;
				a_fObjID		= a_fGou_Attrs.Get_Attr ( "OBJECTID" );
				// 号データ
				Feature_Attr	a_fGou_NO;
				a_fGou_NO		= a_fGou_Attrs.Get_Attr ( "GOU_NO" );	// 変換タイプ
				// 号タイプ 2008.12.03
				Feature_Attr	a_fGouType;
				a_fGouType		= a_fGou_Attrs.Get_Attr ( "GOUTYPE");
			
				e_sPrnInfo.Set_OID ( a_fObjID.e_ccvAttr.intVal );
				
				gou_info			a_gDat;
				a_gDat.e_iOID		= a_fObjID.e_ccvAttr.intVal;
				a_gDat.e_sCode11_1	= a_aAdmDat.e_sCode1;
				a_gDat.e_sCode11_2	= a_aAdmDat.e_sCode2;
				a_gDat.e_sChiban	= "";
				a_gDat.e_sChiban	= Str_to_SJIS ((CComBSTR&)a_fGou_NO.e_ccvAttr.bstrVal);
				a_gDat.e_iGouType	= a_fGouType.e_ccvAttr.intVal;
				//------------------------------------------------------------------------
				// 2008.12.16 [Bug 6543]対応
				a_gDat.e_lGaiku		= a_aAdmDat.e_lGaiku;
				a_gDat.e_sExtGaiku	= a_aAdmDat.e_sExtGaiku;					
				//------------------------------------------------------------------------
				c_mGou_Infos.insert ( a_gDat );
			}else {
				break;
			}
		}
	}
	return	( 0 );
}

/** ------------------------------------------------------------------------------
	セットを見て号の重複をチェックする
--------------------------------------------------------------------------------*/
void	main_rootin::
Chk_Gou_Dupli	( ms_gou_info& c_mGou_Infos )
{

	char	a_cpMsg[256];	// メッセージ用

	e_sPrnInfo.Set_Layer_Name	( e_sGou_Layer );

	ims_gou_info	a_iInfo;
	// 号文字列のセットを用意する
	set<string,less<string> >	a_sGouSet;
	for( a_iInfo = c_mGou_Infos.begin(); a_iInfo != c_mGou_Infos.end(); a_iInfo++ )
	{
		gou_info	a_gDat		= *a_iInfo;
		a_sGouSet.insert ( a_gDat.e_sChiban );
//		printf("%s\t%s\t%d\n", &a_gDat.e_sChiban[0], &a_gDat.e_sCode11[0], a_gDat.e_iOID );
	}

	e_sPrnInfo.Set_Err_Level	( smp_sindy_err_info::EC_ERROR );

	// 号文字列をキーにしてチェックする
	set<string,less<string> >::iterator		a_iGIndx;
	for( a_iGIndx = a_sGouSet.begin(); a_iGIndx != a_sGouSet.end(); a_iGIndx++ )
	{
		string		a_sChiban	= *a_iGIndx;
		gou_info	a_gSKey;
		// サーチキー
		a_gSKey.e_sChiban	= a_sChiban;	// 号文字列
		a_gSKey.e_sCode11_1	= "00000000000";
		a_gSKey.e_sCode11_2	= "00000000000";

		ims_gou_info	a_iBIndx, a_iEIndx;	// 開始・終了インデックス
		a_iBIndx	= c_mGou_Infos.lower_bound( a_gSKey );
		a_gSKey.e_sCode11_1	= "99999999999";
		a_gSKey.e_sCode11_2	= "99999999999";
		a_iEIndx	= c_mGou_Infos.upper_bound( a_gSKey );

		ims_gou_info	a_iGIIndx;
		int			roop = 0;
		gou_info	a_sCompare;
		bool		a_bErrStat	= false;
		for( a_iGIIndx = a_iBIndx; a_iGIIndx != a_iEIndx; a_iGIIndx++, roop++ )
		{
			gou_info	a_gGouInfo	= *a_iGIIndx;
			if( roop == 0 )	{
				a_sCompare	= a_gGouInfo;
			}else {
				// 11桁コードが違う場合、エラーステートにしてループを抜ける
				// [bug 8338]②対応で修正
				if( a_sCompare.e_sCode11_1 != a_gGouInfo.e_sCode11_1 &&
					(a_sCompare.e_sCode11_2 == a_gGouInfo.e_sCode11_1 ||
					((a_sCompare.e_sCode11_2 == a_gGouInfo.e_sCode11_2) && a_sCompare.e_sCode11_2 != "000000")) ) {
					a_bErrStat	= true;
					break;
				}
			}
		}
		// エラーがあった場合
		if( a_bErrStat == true ) {
			for( a_iGIIndx = a_iBIndx; a_iGIIndx != a_iEIndx; a_iGIIndx++ )
			{
				gou_info	a_gCurDat	= *a_iGIIndx;

				e_sPrnInfo.Set_OID ( a_gCurDat.e_iOID );

				//------------------------------------------------------------------------
				// 2008.12.16 [Bug 6543]対応 街区符号、拡張街区符号の出力用				
				char		a_cpGaikuStr[64];
				if( a_gCurDat.e_lGaiku >= 0 ) {
					sprintf	( a_cpGaikuStr, "%d", a_gCurDat.e_lGaiku );
				}else {
					// どちらも空の場合は、拡張街区符号の""がa_cpGaikuStrに入る
					sprintf	( a_cpGaikuStr, "%s", a_gCurDat.e_sExtGaiku.c_str() );
				}
				//------------------------------------------------------------------------

				sprintf	( a_cpMsg, "8桁一致で11桁が違う行政界で号が重複\t%s\t%d\t%s\t%s",
						  a_gCurDat.e_sChiban.c_str(), a_gCurDat.e_iGouType, a_gCurDat.e_sCode11_1.c_str(), a_cpGaikuStr );
				e_sPrnInfo.Set_Err_Msg		( a_cpMsg );
				e_sPrnInfo.Prn_SiNDY_Err	( 0, NULL, stderr );
			}
		}
	}
}

/** ------------------------------------------------------------------------------
	8桁が一致する行政界の中で、別ポリゴンに同じ号ポイントがあるかのチェック
--------------------------------------------------------------------------------*/
void	main_rootin::
Chk_SameCode8Admin ( void )
{
//	char	a_cpMsg[256];	// メッセージ用

	e_sPrnInfo.Set_Layer_Name	( e_sCheck_Layer );

	is_string	a_iIndx;
	for( a_iIndx = e_sCode_8.begin(); a_iIndx != e_sCode_8.end(); a_iIndx++ )
	{
		string	a_sDat	= *a_iIndx;

		char		a_cpCode_5[6], a_cpCode_3[4];
		memset	( a_cpCode_5, '\0', 6 );
		memset	( a_cpCode_3, '\0', 4 );

		strncpy	( a_cpCode_5, &a_sDat[0], 5 );
		strncpy	( a_cpCode_3, &a_sDat[5], 3 );

		fprintf	( stdout, "行政コード<%s>号ポイントチェック中\n", &a_sDat[0]);

		// 8桁が一致する行政界を取ってくる
		char	a_cpWhereStr[128];
		// 2008.11.12 [Bug 6543]への対応。(住所変換モードを「大字も生成」にしていない行政界もエラー対象となる) 
	//	sprintf	( a_cpWhereStr, "CITYCODE = '%s' AND ADDRCODE LIKE '%s%s%s%s' AND CONVTYPE_C <> 0", a_cpCode_5, a_cpCode_3, WCARD1, WCARD1, WCARD1 );
	//	sprintf	( a_cpWhereStr, "(CITYCODE = '%s' AND ADDRCODE LIKE '%s%s%s%s' AND CONVTYPE_C <> 0) OR "
		sprintf	( a_cpWhereStr, "(CITYCODE = '%s' AND ADDRCODE LIKE '%s%s%s%s' AND ADDRCODE2 <> '000000') OR "			 
								"(CITYCODE = '%s' AND ADDRCODE = '%s000')", a_cpCode_5, a_cpCode_3, WCARD1, WCARD1, WCARD1, a_cpCode_5, a_cpCode_3 );
		string	a_sWhere_Str= a_cpWhereStr;
		IFeatureCursorPtr	a_ipAdminCursor;

		a_ipAdminCursor	= e_mCity_Admin_WS.Search_Where ( a_sWhere_Str, RET_FCURSOR );
		if ( a_ipAdminCursor == NULL ) {
			e_sPrnInfo.Prn_Info	( "指定住所コードの行政界が取得できませんでした\n", stderr );
			continue;
		}

		// ここから一塊のチェック ----------------------------------------------
		v_admin_info	a_vAdmLists;

		// 行政界のリストを取得する（ジオメトリ含む）
		Set_Admin_List	( a_ipAdminCursor, a_vAdmLists ); 

		ms_gou_info		a_mGou_Infos;

		// 号データを取ってくる
		if( Set_Gou_Datas ( a_vAdmLists, a_mGou_Infos ) == 0 ) {
		}

		// 実チェック部
		Chk_Gou_Dupli	( a_mGou_Infos );
		// ここまで ------------------------------------------------------------
	}
}



/** ------------------------------------------------------------------------------
	行政界のループ
--------------------------------------------------------------------------------*/
void	main_rootin::
Admin_Check_Roops ( IFeatureCursorPtr&	c_ipAdminCursor	) 	///< 行政界カーソル
{
//	char	a_cpMsg[256];	// メッセージ用

	// 行政界のループ
	int	i=0;
	IFeaturePtr		a_ipCity_Admin_Feat;
	int				a_iCityName_Count	= 0;
	int				a_iCityOffice_Count	= 0;

	// 11桁コード等の情報をセットにする
	Set_AdminCode_Data ( c_ipAdminCursor );
	// 11桁コードリストの出力
	Print_Code_List ( );
	// 変換モードコードエラーのチェック
	Chk_ConvMode ( );
	// 11桁が一致する行政界の中で、変換モードコードが違うものをチェックする
	Chk_SameCode11Admin ( );
	// 8桁が一致する行政界の中で、変換モードコードが違うものをチェックする
	Chk_SameCode8Admin_Mode ( );
	// 8桁が一致する行政界の中で、号ポイントが同じものが他の行政界にあるかどうかのチェック
	Chk_SameCode8Admin ( );
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
//	HRESULT				hr;

	uh::flush_scope_guard fsg;

	// 開始時間の設定
	e_eExeTime.Set_Start();

	// COM を使用できるようにするための初期化
//	::CoInitialize(0);

	// SiNDYの初期化設定
//	prog_init		a_pTool_Init;
//	if( a_pTool_Init.read_init_file ( Ret_Init_File() ) != 0 ) {
//		fprintf( stderr, "初期化ファイル<%s>のオープンに失敗しました\n", Ret_Init_File());
//		return ( false );
//	}
	// COM を使用できるようにするための初期化
//	::CoInitialize(0);
	USES_CONVERSION;	// OLE2T 等の一部のマクロを使用する場合には必ず必要

	IWorkspacePtr			ipWorkspace_Addr;
	if( eExe_Flag & ON_PGDB_MODE ) {
		// パーソナルジオDB用です
		IWorkspaceFactoryPtr	ipWorkspaceFactory_Addr	( CLSID_AccessWorkspaceFactory );
		IPropertySetPtr			ipPropertySet_Addr		( CLSID_PropertySet );					// 接続プロパティー格納用
		_bstr_t					a_bsTmpFName;
		a_bsTmpFName			= e_cpPGDB_File;
		// カレント直下にオープンしてみる
		ipWorkspaceFactory_Addr->OpenFromFile( a_bsTmpFName, 0, &ipWorkspace_Addr );
		if( ipWorkspace_Addr == NULL )	{
			fprintf ( stderr, "パーソナルジオDBがオープンできませんでした\n"); 
	//		End	();
		}else {
			printf(_T("接続完了"));
		}
		printf(_T("\n"));
	}else {
		// 行政界用接続

		if( !SiNDY_DB_Open	( ipWorkspace_Addr )) {
			fprintf	( stderr, "接続出来なかった\n");
			return	( false );
		}else {
			fprintf	( stdout, "接続完了\n" );
		}
	}

	// DB接続にかかった時間の表示
	fprintf	( stdout,	"SiNDY-DB接続処理時間 = %0.2f(s)でした\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );

	// エラーログファイルのオープン
	if( !Open_Err_Log () ) {
		return	( false );
	}else {
		if( !e_sPrnInfo.Init(e_fErr_Fp) ) {
			return	( false );
		}
	}

	// ログのヘッダ情報を出力
	Output_Log_Header( );

	// レイヤオープン
	if( !Open_Layers ( ipWorkspace_Addr ) ){	// 地図用ワークスペース
		e_sPrnInfo.Prn_Info	( "レイヤオープンに失敗しています", stderr );
		return	( false );
	}


	char	a_cpWhereStr[128];

	// ここで引っ掛けたい住所コードをWhere句にする　2004/05/14
	if( eExe_Flag & ON_MONO_ADCODE ) {
		char	a_cpStr_5[6];
		char	a_cpStr_6[7];
		memset	( a_cpStr_5, '\0', 6 );
		memset	( a_cpStr_6, '\0', 7 );
		int	a_iLeng	= 0;
		a_iLeng	= strlen ( e_cpAddrCode );
		if( a_iLeng != 8 && a_iLeng != 11 ) {
			e_sPrnInfo.Prn_Info	( "指定住所コードの桁数は対応外です\n", stderr );
			return	( false );
		}
		strncpy	( a_cpStr_5, e_cpAddrCode, 5 );
		if( a_iLeng == 8 ) {
			strncpy	( a_cpStr_6, e_cpAddrCode+5, 3 );
		//	sprintf	( a_cpWhereStr, "CONVTYPE_C <> 0 AND CITYCODE = '%s' AND ADDRCODE LIKE '%s%s%s%s'", a_cpStr_5, a_cpStr_6, WCARD1, WCARD1, WCARD1 );
			sprintf	( a_cpWhereStr, "ADDRCODE2 <> '000000' AND CITYCODE = '%s' AND ADDRCODE LIKE '%s%s%s%s'", a_cpStr_5, a_cpStr_6, WCARD1, WCARD1, WCARD1 );
		}else {
			strncpy	( a_cpStr_6, e_cpAddrCode+5, 6 );
		//	sprintf	( a_cpWhereStr, "CONVTYPE_C <> 0 AND CITYCODE = '%s' AND ADDRCODE = '%s'", a_cpStr_5, a_cpStr_6 );
			sprintf	( a_cpWhereStr, "ADDRCODE2 <> '000000' AND CITYCODE = '%s' AND ADDRCODE = '%s'", a_cpStr_5, a_cpStr_6 );
		}
	}else {
	//	sprintf	( a_cpWhereStr, "CONVTYPE_C <> 0" );
		sprintf	( a_cpWhereStr, "ADDRCODE2 <> '000000'" );
	}

	// 住所データの中で、変換モードが1と2のものを取ってくる
	string	a_sWhere_Str= a_cpWhereStr; 
	IFeatureCursorPtr	a_ipAdminCursor;
	a_ipAdminCursor	= e_mCity_Admin_WS.Search_Where ( a_sWhere_Str, RET_FCURSOR|RET_COUNT );
	if ( a_ipAdminCursor == NULL ) {
		e_sPrnInfo.Prn_Info	( "指定住所コードの行政界が取得できませんでした\n", stderr );
		return	( false );
	}
	Admin_Check_Roops ( a_ipAdminCursor );
//	printf(" 該当行政界数 = %d\n", e_mCity_Admin_WS.Get_Data_Count() );

	//　実行部終了
	// 終了時間の取得
	fprintf	( stdout,	"データチェック総時間 = %0.2f(s)でした\n",
						(double)((e_eExeTime.Get_From_Start()) / 1000.0) );
	return	( true );
}
