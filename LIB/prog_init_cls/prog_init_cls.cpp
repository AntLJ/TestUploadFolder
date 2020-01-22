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

//
// 2002.06.05
// ツールの初期化及び設定に関しての処理を行う。
#include "stdafx.h"
#include "prog_init_cls.h"

/**
	メッシュリストの読み込み
	※指定したメッシュを１つ１つ読み込んでいく
	@return 成功　0　失敗　-1
*/
int		mk_input_list::
read_mesh_list	(	int		c_iMesh	)	///	メッシュコード
{
	crd_cnv			crd;

	int	cur_digit	= 0;
	cur_digit	= crd.GetMeshDigit( c_iMesh );
	if( cur_digit == -1 ) {
		return	( -1 );
	}
	
	// リストに追加
	mesh_set.insert	( c_iMesh );
	mesh_digit		= cur_digit;	// 桁数の獲得
	return	( 0 );
}

/**
	メッシュリストの読み込み
	内部的にメッシュコードの桁数が違うと失敗扱いとなる
	@return 成功　0　失敗　-1
*/
int		mk_input_list::
read_mesh_list	(	char	*c_cFile	)	// 読み込みファイル名
{
	const	int	bmax	= 128;
	FILE		*p_RFp;		
	char		p_RBuf[bmax];
	int			a_StrLen = 0;	

	p_RFp	= fopen(c_cFile, "r");
	if(p_RFp == NULL) {
		fprintf	(stderr, "メッシュリストファイル <%s> が開けません!!\n", c_cFile);
		return	(-1);
	}

	crd_cnv			crd;
	int	pre_digit	= 0;
	int	first_flag	= 0;
	int	fail_flag	= 0;	// 動作がこけたかの判定に使用
	while(fgets(p_RBuf, bmax, p_RFp) != NULL)
	{
		int			cur_digit	= 0;
		a_StrLen	= strlen(p_RBuf);
		// 最後の"\n"を取り除く
		if(p_RBuf[a_StrLen - 1] == '\n') {
			p_RBuf[a_StrLen - 1] = '\0';
		}

		// コメント文を飛ばす
		if(p_RBuf[0] == '#' || p_RBuf[0] == '\0') {
			continue;
		}

		int			meshcode	= 0;
		meshcode	=  atoi(p_RBuf);

		cur_digit	= crd.GetMeshDigit(meshcode);
		if(first_flag == 0) {
			pre_digit	= cur_digit; 
			first_flag	= 1;
		}
		if(pre_digit != cur_digit) {
			fprintf		(stderr, "読み込んだメッシュリストの桁数に食い違うものがあります<%s>\n", p_RBuf);
			fail_flag	= 1;
			break;
		}
		// リストに追加
		mesh_set.insert	(meshcode);
	}

	fclose(p_RFp);

	if(fail_flag == 1) {
		return	(-1);
	}
	mesh_digit	= pre_digit;	// 桁数の獲得
	return(0);
}

/** 2007.06.28 追加
	文字列レコードの追加
*/
void	mk_input_list::
put_str_record	(	char	*c_cStr	)
{
	std::string	put_str( c_cStr );

	// リストに追加
	data_set.insert	( put_str );
}

/**
	データリストの読み込み
	読み込み文字列の長さが違ってもよい仕様にしている
	@return 成功　0　失敗　-1
*/
int		mk_input_list::
read_data_list	(	char	*c_cFile	)	// 読み込みファイル名
{
	const	int	bmax		= 512;
	FILE		*p_RFp;		
	char		p_RBuf[bmax];
	int			a_StrLen	= 0;	

	p_RFp	= fopen ( c_cFile, "r" );
	if(p_RFp == NULL) {
		fprintf	( stderr, "リストファイル <%s> が開けません!!\n", c_cFile );
		return	( -1 );
	}

	crd_cnv			crd;
	while ( fgets ( p_RBuf, bmax, p_RFp ) != NULL )
	{
		a_StrLen	= strlen(p_RBuf);
		// 最後の"\n"を取り除く
		if(p_RBuf[a_StrLen - 1] == '\n') {
			p_RBuf[a_StrLen - 1] = '\0';
		}

		// コメント文を飛ばす
		if( p_RBuf[0] == '#' || p_RBuf[0] == '\0' ) {
			continue;
		}

		// メモリを確保する
	//	char*	put_str	= NULL;
	//	put_str	= new char[a_StrLen+1];
	//	if( put_str == NULL ) {
	//		fprintf	( stderr, "<%s>の読み込みデータリストを作成する際に、メモリ確保に失敗しました。\n", c_cFile );
	//		return	( -1 );
	//	}

		// データをコピー
		//strcpy	( put_str, p_RBuf );
		//put_str	= p_RBuf;

		// リストに追加
		put_str_record ( p_RBuf );
	}

	fclose	( p_RFp );

	return	( 0 );
}

/**
	初期化ファイルの読み込み
	内部的にメッシュコードの桁数が違うと失敗扱いとなる
	@return 成功　0　失敗　-1
*/
int		prog_init::
read_init_file	(	char	*c_cFile,	// 読み込みファイル名
 					int		c_iMode	)	// 読み込みモード 0=SDE用 1=SiNDY-C用
{
	const	int	bmax	= 256;
	FILE		*p_RFp;		
	char		p_RBuf[bmax];
	int			a_StrLen = 0;

	p_RFp	= fopen(c_cFile, "r");
	if(p_RFp == NULL) {
		fprintf	(stderr, "プログラム初期化ファイル <%s> が開けません!!\n", c_cFile);
		return	(-1);
	}

	crd_cnv		crd;
	int			first_flag	= 0;
	int			fail_flag	= 0;		// 動作がこけたかの判定に使用
	char*		cut_str		= "\t ";	// タブとスペースでカット
	while(fgets(p_RBuf, bmax, p_RFp) != NULL)
	{
		int			cur_digit	= 0;
		a_StrLen	= strlen(p_RBuf);
		// 最後の"\n"を取り除く
		if(p_RBuf[a_StrLen - 1] == '\n') {
			p_RBuf[a_StrLen - 1] = '\0';
		}

		// コメント文を飛ばす
		if(p_RBuf[0] == '#' || p_RBuf[0] == '\0') {
			continue;
		}

		// タブできる
		char*	field[3]	= {NULL, NULL, NULL};

		field[0]	= strtok(p_RBuf, cut_str);
		field[1]	= strtok(NULL, cut_str);
		field[2]	= strtok(NULL, cut_str);
		
		if(field[0] == NULL) {
			fprintf	(stderr, "初期化ファイル<%s>のフォーマットが違います\n", c_cFile);
			return	(-1);
		}

		std::string		fstr[2];
		fstr[0]	= field[0];

		// 大文字化 2007.10.26 追加
		//-------------------------------------------------------------
		int	c = 0;
		while(fstr[0][c] != '\0' ){
			fstr[0][c]	= (char)toupper(fstr[0][c]);
			c++;
		}
		//-------------------------------------------------------------

		fstr[1]	= field[1];
		if(DB_MAP == fstr[0]){	///< 地図系DB接続
			if( c_iMode == 0 ) {	// 2007.03.01
				if(!get_Connect_Info	( field[1],	&e_sUserName_Map, &e_sPassword_Map, &e_sVersion_Map, &e_sInstance_Map, &e_sServer_Map )) {
					fprintf	( stderr, "接続情報の指定が間違っています\n");
					return	( -1 );
				}
			}else {
				e_sDataFile_Path_Map	= field[1];	// 2007.03.01
			}
		}else if(DB_ADDRESS == fstr[0]){	///< 住所系DB接続
			if( c_iMode == 0 ) {	// 2007.03.01
				if(!get_Connect_Info	( field[1],	&e_sUserName_Address, &e_sPassword_Address, &e_sVersion_Address, &e_sInstance_Address, &e_sServer_Address )) {
					fprintf	( stderr, "接続情報の指定が間違っています\n");
					return	( -1 );
				}
			}else {
				e_sDataFile_Path_Address	= field[1];	// 2007.03.01
			}
		}else if(DB_BASE == fstr[0]){	///< 中縮尺DB接続
			if( c_iMode == 0 ) {	// 2007.03.01
				if(!get_Connect_Info	( field[1],	&e_sUserName_Base, &e_sPassword_Base, &e_sVersion_Base, &e_sInstance_Base, &e_sServer_Base )) {
					fprintf	( stderr, "接続情報の指定が間違っています\n");
					return	( -1 );
				}
			}else {
				e_sDataFile_Path_Base	= field[1];	// 2007.03.01			
			}
		}else if(DB_ROAD == fstr[0]){	///< 道路系DB接続 [bug 7066]への対応 2009.07.02
			if( c_iMode == 0 ) {
				if(!get_Connect_Info	( field[1],	&e_sUserName_Road, &e_sPassword_Road, &e_sVersion_Road, &e_sInstance_Road, &e_sServer_Road )) {
					fprintf	( stderr, "接続情報の指定が間違っています\n");
					return	( -1 );
				}
			}else {
				e_sDataFile_Path_Road	= field[1];		
			}
		}else if(DB_ANNO == fstr[0]){	///< 注記系DB接続 cityとanno,poiの分離対応 2014/05/07
			if( c_iMode == 0 ) {
				if(!get_Connect_Info	( field[1],	&e_sUserName_Anno, &e_sPassword_Anno, &e_sVersion_Anno, &e_sInstance_Anno, &e_sServer_Anno )) {
					fprintf	( stderr, "接続情報の指定が間違っています\n");
					return	( -1 );
				}
			}else {
				e_sDataFile_Path_Anno	= field[1];		
			}
		}else if(DB_ANY == fstr[0]){	///< 汎用DB接続
			if( c_iMode == 0 ) {	// 2007.03.01
				if(!get_Connect_Info	( field[1],	&e_sUserName, &e_sPassword, &e_sVersion, &e_sInstance, &e_sServer )) {
					fprintf	( stderr, "接続情報の指定が間違っています\n");
					return	( -1 );			
				}
			}else {
				e_sDataFile_Path		= field[1];	// 2007.03.01			
			}
		}else
		if(Add_FC(INI_UN) == fstr[0]) {	///< ユーザ名
			e_sUserName	= fstr[1];
		}else
		if(Add_FC(INI_PW) == fstr[0]) {	///<パスワード
			e_sPassword	= fstr[1];
		}else
		if(Add_FC(INI_IT) == fstr[0]) {	///< インスタンス
			e_sInstance	= fstr[1];
		}else
		if(Add_FC(INI_SV) == fstr[0]) {		///< サーバ名
			e_sServer	= fstr[1];
		}else
		if(Add_FC(INI_VER) == fstr[0]) {	///< バージョン
			e_sVersion	= fstr[1];
		}else
	//	if(Add_FC(INI_OPATH) == fstr[0]) {	///< データ出力パス名
	//		e_sOutput_Path	= fstr[1];
	//	}else
		if(Add_FC(INI_CMESH) == fstr[0]) {	///< 都市地図メッシュレイヤ
			e_sCity_Mesh	= fstr[1];
		}else
		if(Add_FC(INI_CNOTE) == fstr[0]) {	///< 都市地図注記ポイントレイヤ
			e_sCity_Note	= fstr[1];
		}else
		if(Add_FC(INI_CNLINE) == fstr[0]) {	///< 都市地図注記ラインレイヤ
			e_sCity_Note_Line	= fstr[1];
		}else
		if(Add_FC(INI_CHIBAN) == fstr[0]) {	///< 地番注記レイヤ
			e_sChiban			= fstr[1];
		}else
		if(Add_FC(INI_CLINE) == fstr[0]) {	///< 都市地図背景アークレイヤ
			e_sCity_Line	= fstr[1];
		}else
		if(Add_FC(INI_CRWAY) == fstr[0]) {	///< 都市地図鉄道レイヤ
			e_sCity_RailWay	= fstr[1];
		}else
		if(Add_FC(INI_CSITE) == fstr[0]) {	///< 都市地図街区レイヤ
			e_sCity_Site	= fstr[1];
		}else
		if(Add_FC(INI_CSTA) == fstr[0]) {	///< 都市地図駅舎レイヤ
			e_sCity_Station	= fstr[1];
		}else
		if(Add_FC(INI_BLD) == fstr[0]) {	///< 都市地図建物レイヤ
			e_sCity_Building	= fstr[1];
		}else
		if(Add_FC(INI_BLDL) == fstr[0]) {	///< 都市地図建物アークレイヤ
			e_sCity_Build_Line	= fstr[1];
		}else
		if(Add_FC(INI_STEP) == fstr[0]) {	///< 都市地図段差形状レイヤ
			e_sCity_Build_Step	= fstr[1];
		}else
		if(Add_FC(INI_ROOF) == fstr[0]) {	///< 都市地図建物屋上施設形状レイヤ
			e_sCity_Building_Roof	= fstr[1];
		}else
		if(Add_FC(INI_TUNNEL) == fstr[0]) {	///< 都市地図トンネル坑口形状レイヤ
			e_sCity_Tunnel	= fstr[1];
		}else
		if(Add_FC(INI_GOU) == fstr[0]) {	///< 号ポイント
			e_sGou_Point		= fstr[1];
		}else
		if(Add_FC(INI_ADM) == fstr[0]) {	///< 行政界レイヤ
			e_sAdmin			= fstr[1];
		}else
		if(Add_FC(INI_BADM) == fstr[0]) {	///< 中縮尺行政界レイヤ
			e_sBase_Admin		= fstr[1];
		}else
		if(Add_FC(INI_CADM) == fstr[0]) {	///< 都市地図行政界レイヤ
			e_sCity_Admin		= fstr[1];
		}else
		if(Add_FC(INI_BMESH) == fstr[0]) {	///< 中縮尺メッシュレイヤ
			e_sBase_Mesh		= fstr[1];
		}else
		if(Add_FC(INI_BNL1) == fstr[0]) {	///< 中縮尺注記ラインレイヤ１
			e_sBase_Note_Line1	= fstr[1];
		}else
		if(Add_FC(INI_BNL2) == fstr[0]) {	///< 中縮尺注記ラインレイヤ２
			e_sBase_Note_Line2	= fstr[1];
		}else
		if(Add_FC(INI_BNL3) == fstr[0]) {	///< 中縮尺注記ラインレイヤ３
			e_sBase_Note_Line3	= fstr[1];
		}else
		if(Add_FC(INI_BNL4) == fstr[0]) {	///< 中縮尺注記ラインレイヤ４
			e_sBase_Note_Line4	= fstr[1];
		}else
		if(Add_FC(INI_BNOTE) == fstr[0]) {	///< 中縮尺注記
			e_sBase_Note	= fstr[1];
		}else
		if(Add_FC(INI_BCNT) == fstr[0]) {	///< 中縮尺段彩レイヤ
			e_sBase_Contour	= fstr[1];
		}else
		if(Add_FC(INI_BLINE) == fstr[0]) {	///< 中縮尺背景アークレイヤ
			e_sBase_Line	= fstr[1];
		}else
		if(Add_FC(INI_BRWAY) == fstr[0]) {	///< 中縮尺鉄道レイヤ
			e_sBase_Railway	= fstr[1];
		}else
		if(Add_FC(INI_BSITE) == fstr[0]) {	///< 中縮尺背景、施設レイヤ
			e_sBase_Site	= fstr[1];
		}else
		if(Add_FC(INI_BSTA) == fstr[0]) {	///< 中縮尺駅舎レイヤ
			e_sBase_Station	= fstr[1];
		}else
		if(Add_FC(INI_MMESH) == fstr[0]) {	///< ミドルメッシュレイヤ
			e_sMiddle_Mesh		= fstr[1];
		}else
		if(Add_FC(INI_MNL1) == fstr[0]) {	///< ミドル注記ラインレイヤ１
			e_sMiddle_Note_Line1	= fstr[1];
		}else
		if(Add_FC(INI_MNL2) == fstr[0]) {	///< ミドル注記ラインレイヤ２
			e_sMiddle_Note_Line2	= fstr[1];
		}else
		if(Add_FC(INI_MNL3) == fstr[0]) {	///< ミドル注記ラインレイヤ３
			e_sMiddle_Note_Line3	= fstr[1];
		}else
		if(Add_FC(INI_MNL4) == fstr[0]) {	///< ミドル注記ラインレイヤ４
			e_sMiddle_Note_Line4	= fstr[1];
		}else
		if(Add_FC(INI_MNOTE) == fstr[0]) {	///< ミドル注記
			e_sMiddle_Note	= fstr[1];
		}else
		if(Add_FC(INI_MCNT) == fstr[0]) {	///< ミドル段彩レイヤ
			e_sMiddle_Contour	= fstr[1];
		}else
		if(Add_FC(INI_MLINE) == fstr[0]) {	///< ミドル背景アークレイヤ
			e_sMiddle_Line	= fstr[1];
		}else
		if(Add_FC(INI_MSITE) == fstr[0]) {	///< ミドル背景、施設レイヤ
			e_sMiddle_Site	= fstr[1];
		}else
		if(Add_FC(INI_TMESH) == fstr[0]) {	///< トップメッシュレイヤ
			e_sTop_Mesh		= fstr[1];
		}else
		// トップマップ注記用 2004.10.26 追加
		if(Add_FC(INI_TNL1) == fstr[0]) {	///< トップ注記ラインレイヤ１
			e_sTop_Note_Line1	= fstr[1];
		}else
		if(Add_FC(INI_TNL2) == fstr[0]) {	///< トップ注記ラインレイヤ２
			e_sTop_Note_Line2	= fstr[1];
		}else
		if(Add_FC(INI_TNL3) == fstr[0]) {	///< トップ注記ラインレイヤ３
			e_sTop_Note_Line3	= fstr[1];
		}else
		if(Add_FC(INI_TNL4) == fstr[0]) {	///< トップ注記ラインレイヤ４
			e_sTop_Note_Line4	= fstr[1];
		}else
		if(Add_FC(INI_TNOTE) == fstr[0]) {	///< トップ注記
			e_sTop_Note	= fstr[1];
		}else
		// トップマップ用 2003.01.05 追加
		if(Add_FC(INI_TCNT) == fstr[0]) {	///< トップ段彩レイヤ
			e_sTop_Contour	= fstr[1];
		}else
		if(Add_FC(INI_TLINE) == fstr[0]) {	///< トップ背景アークレイヤ
			e_sTop_Line		= fstr[1];
		}else
		if(Add_FC(INI_TSITE) == fstr[0]) {	///< トップ背景、施設レイヤ
			e_sTop_Site		= fstr[1];
		}else
		//　トップマップ用ここまで
		if(Add_FC(INI_RLINK) == fstr[0]) {	///< 道路リンクレイヤ
			e_sRoad_Link	= fstr[1];		
		}else
		if(Add_FC(INI_PAREA) == fstr[0]) {	///< PAREA代表点レイヤ
			e_sParea		= fstr[1];		
		}else
		if(Add_FC(INI_BRP) == fstr[0]) {	///< 中縮尺Addrポイントレイヤ
			e_sBase_Rep_Point= fstr[1];		
		}else
		if(Add_FC(INI_BACK_ATTR) == fstr[0]) {	///< 背景種別用テーブル
			e_sBackground_Class	= fstr[1];
		}else
		if(Add_FC(INI_ANNO_ATTR) == fstr[0]) {	///< 注記種別用テーブル
			e_sAnnotation_Class	= fstr[1];
		}else
		if(Add_FC(INI_POI) == fstr[0]) {
			e_sPOI_Point = fstr[1];
		}
	}

	fclose(p_RFp);

	if(fail_flag == 1) {
		return	(-1);
	}
	return(0);
}

/**
	プログラム初期化環境変数の読み込み
	環境変数から情報を取り出す
	@return 成功　0　失敗　-1
*/
int		prog_init::
read_init_env	( 	int		c_iMode	)		// 読み込みモード 0=SDE用 1=SiNDY-C用
{

	char*		a_cpEnvStr	= NULL;		// 環境変数のポインタ
	int			cur_digit	= 0;
	
	if( (a_cpEnvStr = getenv(DB_MAP)) != NULL ) {	///< 地図系DB接続
		if( c_iMode == 0 ) {	// 2007.03.01
			if(!get_Connect_Info	( a_cpEnvStr,	&e_sUserName_Map, &e_sPassword_Map, &e_sVersion_Map, &e_sInstance_Map, &e_sServer_Map )) {
				fprintf	( stderr, "接続情報の指定が間違っています\n");
				return	( -1 );
			}
		}else {
			e_sDataFile_Path_Map	= a_cpEnvStr;	// 2007.03.01
		}
	}
	if( (a_cpEnvStr = getenv(DB_BASE)) != NULL ) {	///< 中縮尺系DB接続
		if( c_iMode == 0 ) {	// 2007.03.01
			if(!get_Connect_Info	( a_cpEnvStr,	&e_sUserName_Base, &e_sPassword_Base, &e_sVersion_Base, &e_sInstance_Base, &e_sServer_Base )) {
				fprintf	( stderr, "接続情報の指定が間違っています\n");
				return	( -1 );
			}
		}else {
			e_sDataFile_Path_Base	= a_cpEnvStr;	// 2007.03.01
		}
	}
	if( (a_cpEnvStr = getenv(DB_ROAD)) != NULL ) {	///< 道路系DB接続 [bug 7066]対応 2009.07.07
		if( c_iMode == 0 ) {	// 2007.03.01
			if(!get_Connect_Info	( a_cpEnvStr,	&e_sUserName_Road, &e_sPassword_Road, &e_sVersion_Road, &e_sInstance_Road, &e_sServer_Road )) {
				fprintf	( stderr, "接続情報の指定が間違っています\n");
				return	( -1 );
			}
		}else {
			e_sDataFile_Path_Road	= a_cpEnvStr;	// 2007.03.01
		}
	}
	if( (a_cpEnvStr = getenv(DB_ANNO)) != NULL ) {	///< 注記系DB接続 cityとanno,poiの分離対応 2014/05/07
		if( c_iMode == 0 ) {
			if(!get_Connect_Info	( a_cpEnvStr,	&e_sUserName_Anno, &e_sPassword_Anno, &e_sVersion_Anno, &e_sInstance_Anno, &e_sServer_Anno )) {
				fprintf	( stderr, "接続情報の指定が間違っています\n");
				return	( -1 );
			}
		}else {
			e_sDataFile_Path_Anno	= a_cpEnvStr;
		}
	}
	if( (a_cpEnvStr = getenv(DB_ADDRESS)) != NULL ) {	///< 住所系DB接続
		if( c_iMode == 0 ) {	// 2007.03.01
			if(!get_Connect_Info	( a_cpEnvStr,	&e_sUserName_Address, &e_sPassword_Address, &e_sVersion_Address, &e_sInstance_Address, &e_sServer_Address )) {
				fprintf	( stderr, "接続情報の指定が間違っています\n");
				return	( -1 );
			}
		}else {
			e_sDataFile_Path_Address	= a_cpEnvStr;	// 2007.03.01
		}
	}
	if( (a_cpEnvStr = getenv(DB_ANY)) != NULL ) {	///< 汎用DB接続
		if( c_iMode == 0 ) {	// 2007.03.01
			if(!get_Connect_Info	( a_cpEnvStr,	&e_sUserName, &e_sPassword, &e_sVersion, &e_sInstance, &e_sServer )) {
				fprintf	( stderr, "接続情報の指定が間違っています\n");
				return	( -1 );			
			}
		}else {
			e_sDataFile_Path		= a_cpEnvStr;	// 2007.03.01
		}
	}
//	if( (a_cpEnvStr = getenv( Add_FC(INI_UN))) != NULL ) {	///< ユーザ名
//		e_sUserName	= a_cpEnvStr;
//	}
//	if( (a_cpEnvStr = getenv( Add_FC(INI_PW))) != NULL ) {	///<パスワード
//		e_sPassword	= a_cpEnvStr;
//	}
//	if( (a_cpEnvStr = getenv( Add_FC(INI_IT))) != NULL ) {	///<インスタンス
//		e_sInstance	= a_cpEnvStr;
//	}
//	if( (a_cpEnvStr = getenv( Add_FC(INI_SV ))) != NULL ) {	///<サーバ名
//		e_sServer	= a_cpEnvStr;
//	}
//	if( (a_cpEnvStr = getenv( Add_FC(INI_VER))) != NULL ) {	///<バージョン
//		e_sVersion	= a_cpEnvStr;
//	}
//	if( (a_cpEnvStr = getenv( Add_FC(INI_OPATH))) != NULL ) {///<データ出力パス名
//		e_sOutput_Path	= a_cpEnvStr;
//	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CMESH))) != NULL ) {	///<都市地図メッシュレイヤ
		e_sCity_Mesh	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CNOTE))) != NULL ) {	///<都市地図注記ポイントレイヤ
		e_sCity_Note	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CNLINE))) != NULL ) {	///<都市地図注記ラインレイヤ
		e_sCity_Note_Line	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CHIBAN))) != NULL ) {	///<地番注記レイヤ
		e_sChiban	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CLINE))) != NULL ) {	///<都市地図背景アークレイヤ
		e_sCity_Line	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CRWAY))) != NULL ) {	///<都市地図鉄道レイヤ
		e_sCity_RailWay	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CSITE))) != NULL ) {	///<都市地図街区レイヤ
		e_sCity_Site	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CSTA))) != NULL ) {	///<都市地図駅舎レイヤ
		e_sCity_Station	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BLD))) != NULL ) {	///<都市地図建物レイヤ
		e_sCity_Building	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BLDL))) != NULL ) {	///<都市地図建物アークレイヤ
		e_sCity_Build_Line	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_STEP))) != NULL ) {	///<都市地図段差形状レイヤ
		e_sCity_Build_Step	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_ROOF))) != NULL ) {	///<都市地図建物屋上施設形状レイヤ
		e_sCity_Building_Roof	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_TUNNEL))) != NULL ) {	///<都市地図トンネル坑口形状レイヤ
		e_sCity_Tunnel	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_GOU))) != NULL ) {	///<号ポイントレイヤ 2003.7.18
		e_sGou_Point		= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_ADM))) != NULL ) {	///<行政界レイヤ
		e_sAdmin	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BADM))) != NULL ) {	///<中縮尺行政界レイヤ
		e_sBase_Admin	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_CADM))) != NULL ) {	///<都市地図行政界レイヤ
		e_sCity_Admin	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BMESH))) != NULL ) {///<中縮尺メッシュレイヤ
		e_sBase_Mesh	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BNL1))) != NULL ) {///<中縮尺注記ラインレイヤ１
		e_sBase_Note_Line1	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BNL2))) != NULL ) {///<中縮尺注記ラインレイヤ２
		e_sBase_Note_Line2	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BNL3))) != NULL ) {///<中縮尺注記ラインレイヤ３
		e_sBase_Note_Line3	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BNL4))) != NULL ) {///<中縮尺注記ラインレイヤ４
		e_sBase_Note_Line4	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BNL4))) != NULL ) {///<中縮尺注記ラインレイヤ４
		e_sBase_Note_Line4	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BNOTE))) != NULL ) {	///< 中縮尺注記
		e_sBase_Note	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BCNT))) != NULL ) {	///< 中縮尺段彩レイヤ
		e_sBase_Contour	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BLINE))) != NULL ) {	///< 中縮尺背景アークレイヤ
		e_sBase_Line	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BRWAY))) != NULL ) {	///< 中縮尺鉄道レイヤ
		e_sBase_Railway	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BSITE))) != NULL ) {	///< 中縮尺背景、施設レイヤ
		e_sBase_Site	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BSTA))) != NULL ) {	///< 中縮尺駅舎レイヤ
		e_sBase_Station	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MMESH))) != NULL ) {	///< ミドルメッシュレイヤ
		e_sMiddle_Mesh		= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MNL1))) != NULL ) {	///< ミドル注記ラインレイヤ１
		e_sMiddle_Note_Line1	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MNL2))) != NULL ) {	///< ミドル注記ラインレイヤ２
		e_sMiddle_Note_Line2	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MNL3))) != NULL ) {	///< ミドル注記ラインレイヤ３
		e_sMiddle_Note_Line3	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MNL4))) != NULL ) {	///< ミドル注記ラインレイヤ４
		e_sMiddle_Note_Line4	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MNOTE))) != NULL ) {	///< ミドル注記
		e_sMiddle_Note	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MCNT))) != NULL ) {		///< ミドル段彩レイヤ
		e_sMiddle_Contour	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MLINE))) != NULL ) {	///< ミドル背景アークレイヤ
		e_sMiddle_Line	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_MSITE))) != NULL ) {	///< ミドル背景、施設レイヤ
		e_sMiddle_Site	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_TMESH))) != NULL ) {	///< ミドルメッシュレイヤ
		e_sTop_Mesh		= a_cpEnvStr;
	}
	// トップマップ用 2003.01.05追加
	if( (a_cpEnvStr = getenv( Add_FC(INI_TCNT))) != NULL ) {		///< トップ段彩レイヤ
		e_sTop_Contour	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_TLINE))) != NULL ) {	///< トップ背景アークレイヤ
		e_sTop_Line		= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_TSITE))) != NULL ) {	///< トップ背景、施設レイヤ
		e_sTop_Site		= a_cpEnvStr;
	}
	//　トップマップ用ここまで
	if( (a_cpEnvStr = getenv( Add_FC(INI_RLINK))) != NULL ) {	///< 道路リンクレイヤ
		e_sRoad_Link	= a_cpEnvStr;		
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_PAREA))) != NULL ) {	///< PAREA代表点レイヤ
		e_sParea		= a_cpEnvStr;		
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BRP))) != NULL ) {		///< 中縮尺Addrポイントレイヤ
		e_sBase_Rep_Point= a_cpEnvStr;		
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_BACK_ATTR))) != NULL ) {	///< 背景種別用テーブル
		e_sBackground_Class	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_ANNO_ATTR))) != NULL ) {	///< 注記種別用テーブル
		e_sAnnotation_Class	= a_cpEnvStr;
	}
	if( (a_cpEnvStr = getenv( Add_FC(INI_POI))) != NULL ) {	///< POI_POINTテーブル
		e_sPOI_Point	= a_cpEnvStr;
	}
	return(0);
}

/**	2007.06.27 追加 from CrossRoadsNameChk から
	初期化文字列の読み込み
	@return 成功　0　失敗　-1
*/
bool	prog_init::
read_init_str	(	char	*c_cpConnect_Str	)	///< 接続文字列
{
	if( !get_Connect_Info ( c_cpConnect_Str,
							&e_sUserName, &e_sPassword, &e_sVersion,
							&e_sInstance, &e_sServer ) ) {
		return	( false );
	}
	return	( true );
}

/**
	接続情報の取得
	@return	成功	true
	@return	失敗	false
*/
bool	prog_init::
get_Connect_Info (	char*			c_cpConnect_Str,	///< 接続文字列
					std::string*	c_spUserName,		///< ユーザ名[out]
					std::string*	c_spPassword,		///< パスワード[out]
					std::string*	c_spVersion,		///< バージョン[out]
					std::string*	c_spInstance,		///< インスタンス[out]
					std::string*	c_spServer )		///< サーバ名[out]
{
	char*		cut_str		= "/";	// スラッシュでカット

	// タブできる
	char*	field[5]	= {NULL, NULL, NULL, NULL, NULL};

	field[0]	= strtok(c_cpConnect_Str, cut_str);	///< ユーザ名
	field[1]	= strtok(NULL, cut_str);			///< パスワード
	field[2]	= strtok(NULL, cut_str);			///< インスタンス
	field[3]	= strtok(NULL, cut_str);			///< サーバ名
	field[4]	= strtok(NULL, cut_str);			///< バージョン	

	int	i = 0;
	for(i = 0; i < 5; i++)
	{
		if(field[i] == NULL) {
			return	( false );
		}
	}

	*c_spUserName	= field[0];
	*c_spPassword	= field[1];
	*c_spVersion	= field[2];
	*c_spInstance	= field[3];
	*c_spServer		= field[4];

	return	( true );
}
