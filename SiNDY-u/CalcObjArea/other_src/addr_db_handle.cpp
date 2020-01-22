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

#include "Stdafx.h"
#include "addr_db_handle.h"
#include "addr_str_work.h"

/**
	住所DBへの接続
*/
bool	addr_db_handle::
Init_DB ( char*	c_cpConnectStr, 	///< DB接続文字列(<ユーザ名>/<パスワード>@<サービス名>)
		  FILE* c_fpErr_Fp	)
{
	if( c_cpConnectStr == NULL ) {
		return	( false );
	}

	// エラー出力初期化
	if( c_fpErr_Fp != NULL ) {
		m_fpErr_Fp	= c_fpErr_Fp;
	}

	// オラクルDB読み込みモード
	m_iMode	=	READ_ODB;

	/// クラスライブラリ初期化
	OStartup();	

	char*	a_sTmpStr	= NULL;
	a_sTmpStr	= new char[strlen(c_cpConnectStr)+1];
	strcpy	( a_sTmpStr, c_cpConnectStr );

	string	a_sUser_Name;	///< ユーザ名
	string	a_sPasswd;		///< 接続パスワード
	string	a_sService_Name;///< サービス名

	char*	a_cpCurPt	= NULL;
	a_cpCurPt	= strtok( a_sTmpStr, "/" );
	if( a_cpCurPt != NULL ) {	// ユーザ名
		a_sUser_Name	= a_cpCurPt;
	}
	a_cpCurPt	= strtok( NULL, "@" );
	if( a_cpCurPt != NULL ) {	// 接続パスワード
		a_sPasswd		= a_cpCurPt;
	}
	a_cpCurPt	= strtok( NULL, "" );
	if( a_cpCurPt != NULL ) {	// サービス名
		a_sService_Name	= a_cpCurPt;	
	}

	/// 接続の設定
	if( m_oAdm_DB.Open ( a_sService_Name.c_str(), a_sUser_Name.c_str(), a_sPasswd.c_str() ) != OSUCCESS ) {	// 接続失敗
		return	( false );
	}

	// DBでの作業が実行可能
	m_bExec_OK	= true;

	return	( true );
}

/**
	クラスの初期化
*/
bool	addr_db_handle::
Init_TextDB	( char*		c_cpDBPath,		///< テキストDBパス名
	 		  FILE*		c_fpErr_Fp	)
{
    bool	iRet	= false;

	m_iMode	=	READ_TDB;

	// エラー出力初期化
	if( c_fpErr_Fp != NULL ) {
		m_fpErr_Fp	= c_fpErr_Fp;
	}

    if(_access ( c_cpDBPath, 0 ) == 0){	// テキストDBパスが存在した場合
        m_sDBPath	= c_cpDBPath;
        iRet = true;
		// 作業が実行可能
		m_bExec_OK	= true;

    }else{
        fprintf	( m_fpErr_Fp, "AddressData may not exist. Check DataPath:%s\n", c_cpDBPath );
    }
    return ( iRet );
}

/**
	行政コードの正当性チェック
	@return RET_OK		正しいコード
	@return RET_HAISHI	廃止になったコード
	@return RET_000		000がつく
*/
int addr_db_handle::
IsValidCode11	(	const char	*Code,
					char		*AreaName	)
{
    int					iRet;
    const char			*p = Code;
    char				City[12];
    m_string::iterator	it;
    
	// 5桁コード取り出し
    strcpy(City, Code);
    City[5] = '\0';
	// 頭5桁で読み込み済みリストをサーチ
    if(m_Ready.find(City) == m_Ready.end()) {	
        LoadAddress(Code);			// テキストファイルからデータをロードする
		m_Ready.insert(City);		// 読み込み済みリストへ追加
    }
    
	// 11桁コードでデータをサーチ
    if((it = m_Map.find(Code)) == m_Map.end()){
        string Comp = Code;
        Comp += '1';
        if((it = m_Map.find(Comp)) == m_Map.end()){
            Comp = Code;
            Comp += '2';
            if((it = m_Map.find(Comp)) == m_Map.end()){
                AreaName[0] = '\0';
                iRet = 0;  //Not Found
            }else{
                iRet = RET_HAISHI;  //Not Use
            }
        }else{
            iRet = RET_000; //Attn
        }
    }else{	/// サーチしたデータが見つかった
        iRet = RET_OK;
    }
    if(iRet){
        FormatStr(AreaName, ((*it).second).data());
    }
    return iRet;
}

/**
	行政コードの正当性チェック（5桁用）
	@return RET_OK		正しいコード
	@return RET_HAISHI	廃止になったコード
	@return RET_000		000がつく
*/
int addr_db_handle::
IsValidCode5	( const char	*Code )
{
    int					iRet;
    const char			*p = Code;
    char				City[12];
    m_string2::iterator	it;
    
	// 5桁コード取り出し
    strcpy(City, Code);
    City[5] = '\0';
	// 頭5桁で読み込み済みリストをサーチ
    if(m_Ready.find(City) == m_Ready.end()){
        m_Ready.insert(City);		// 読み込み済みリストへ追加
		if(!LoadAddress(Code)) {	// テキストファイルからデータをロードする
            return	0;
		}
    }
    
	// 11桁コードでデータをサーチ
    if((it = m_City.find(City)) != m_City.end()){
		iRet	= (*it).second;
	}else {
		iRet	= 0;
	}
    return iRet;
}

/**
	OracleDBから直で取ってきた文字列を取得
*/
bool addr_db_handle::
GetOraDBStr	(	const char	*c_cpCode,
				addr_dat&	c_aDat	)
{
    bool				iRet;
    const char			*p = c_cpCode;
    char				City[12];
    m_string::iterator	it;

	memset	( City, '\0', 12 );
    
	// 5桁コード取り出し
    strcpy(City, c_cpCode);
    City[5] = '\0';
	// 頭5桁で読み込み済みリストをサーチ
    if(m_Ready.find(City) == m_Ready.end()) {	
        LoadAddress(c_cpCode);			// テキストファイルからデータをロードする
		m_Ready.insert(City);			// 読み込み済みリストへ追加
    }
    
	// 11桁コードでデータをサーチ
	is_addr_dat		a_iIndx;
	addr_dat		a_aSKey;
	a_aSKey.e_sCode	= c_cpCode;
//	a_aSKey.e_sName	= "";
	a_iIndx	= m_sDatas.find ( a_aSKey );
	if( a_iIndx != m_sDatas.end() ) {
		c_aDat	= *a_iIndx;
		iRet			= true;
	}else {
		iRet			= false;
	}
    return iRet;
}

/**
	区名から市の5桁等の情報を取得する
*/
bool addr_db_handle::
GetCitybyWard(	addr_dat&	c_aInputDat,	///< 入力情報[IN]
				addr_dat&	c_aRetDat	)	///< 返却情報[IN]
{
	// 特別区対応用 "xx市　oo区"の場合は、xx市の5桁を取得する
	long	a_lPos	= 0;
	a_lPos	= c_aInputDat.e_sShi_Kanji.find ( "市　" );
	if( a_lPos != c_aInputDat.e_sShi_Kanji.npos ) {	// 全角スペースで検索
		long	a_lLen	= 0;
		a_lLen	= c_aInputDat.e_sShi_Kanji.length();
		// 最後の1文字を取り出し比較
		if(a_lLen - 2 > 0) {
			if( c_aInputDat.e_sShi_Kanji.substr(a_lLen-2,2) == "区") {
				// "市"で文字列を区切る
				string	a_sShi;
				a_sShi	= c_aInputDat.e_sShi_Kanji.substr ( 0, a_lPos+2 );
				
				string	a_sKey;
				a_sKey	= c_aInputDat.e_sKen_Kanji;
				a_sKey	+=a_sShi;
				im_addr_dat	a_iIndx;
				a_iIndx	= m_sRevData.find(a_sKey);
				if( a_iIndx != m_sRevData.end()){
					c_aRetDat	= (*a_iIndx).second;
					return	( true );
				}
			}
		}
	}
	c_aRetDat	= c_aInputDat;
	return	( false );
}

void addr_db_handle::
FormatStr (	char		*Addr,
			const char	*Src )
{
    const char *p = Src;
    int i;
    for(i = 0; *p != '\t';i++){
        Addr[i] = *p;
        p++;
    }
    Addr[i] = ' ';
    
    for(p++, i++; *p != '\t';i++){
        Addr[i] = *p;
        p++;
    }
    Addr[i] = ' ';

    for(p++, i++; *p != '\t';i++){
        Addr[i] = *p;
        p++;
    }
    Addr[i] = ' ';

    for(p++, i++; *p != '\n';i++){
        Addr[i] = *p;
        p++;
    }
    Addr[i] = '\0';
}

/**
	住所データの読み込み
*/
bool	addr_db_handle::
LoadAddress	( const char *c_cpCode )		///< 行政コード
{
	if( m_iMode	== READ_ODB ) {
		if( !LoadAddress_DB ( c_cpCode ) ) {
			return	( false );
		}
	//	if( !LoadOldAddress_DB( c_cpCode ) ) {
	//		return	( false );
	//	}
	}else
	if( m_iMode == READ_TDB ) {
		if( !LoadAddress_TextDB ( c_cpCode ) ) {
			return	( false );
		}
	}
	return	( true );
}

/**
	テキストDBの読み込み
*/
bool addr_db_handle::
LoadAddress_TextDB( const char *c_cpCode )	///< 行政コード
{
    static	char Path[256];
    static	char Buf[128];
    FILE	*fp;
    bool	iRet = true;
    string	ElvCode;
	string	FiveCode;	// 5桁コード用
    string	str;
 
    pair<int, string> data;
	int	Status	= 2;	// 5桁コード用(Not Use)
    sprintf(Path, "%s/%.2s/%.5s.txt", &m_sDBPath[0], c_cpCode, c_cpCode );
    if((fp = fopen(Path, "rt")) != NULL){
        while(fgets(Buf, 128, fp) != NULL){
            Buf[11] = '\0';
            ElvCode = Buf;
            if(Buf[12] != '0'){
                ElvCode += Buf[12];
            }else {
				Status	= 1;	// 5桁コード用
			}
            str = &(Buf[14]);
            m_Map.insert(m_string::value_type(ElvCode, str));
        }
        fclose(fp);
		char	Tmp[8];
		strncpy	(Tmp, c_cpCode, 5);
		Tmp[5] = '\0';
		FiveCode = Tmp;
		m_City.insert(m_string2::value_type(FiveCode, Status));
    }else{
        iRet = false;
    }
    return iRet;
}

/**
	区に対応する市の5桁コードの情報を取得する
*/
bool	addr_db_handle::
GetShiKuInfo(	ODatabase&	c_oAdm_DB,	///< 住所DB[IN]
				string&		c_sKen,		///< 都道府県名[IN]
				string&		c_sShi,		///< 市区町村名[IN]
				addr_dat&	c_aDat	)	///< 地名データ[OUT]
{
	//--------------------------------------------------------------------
	///	SQL文
	char	addsql[128];
	memset( addsql , '\0' , 128 );
	
	string	a_sKuSql;

	a_sKuSql	+= "SELECT "
					"KEN_CODE,"
					"SHI_CODE,"
					"SHI_KANJI "
				"from "
					"AREACODE_MASTER "
				"where ";

	sprintf( addsql ,	"KEN_KANJI='%s' AND SHI_KANJI='%s' AND HAISHI_DATE='000000'", c_sKen.c_str(), c_sShi.c_str() );	// 2009.05.22 修正

	a_sKuSql += addsql;

	/// SQL文でデータ取り出す
	ODynaset odyn_for_Ku( m_oAdm_DB , (char*)a_sKuSql.c_str() );
	char	a_ken_code_tmp[3];
	char	a_shi_code_tmp[4];

	char	a_ken_str[9];
	char	a_shi_str[25];

	addr_str_work	a_aASWork;

	addr_dat		a_iTmpAddr;	// 特別区の対応用
	bool			a_iRet	= true;
	while (!odyn_for_Ku.IsEOF()){
		memset	( a_ken_code_tmp, '\0' , 3 );
		memset	( a_shi_code_tmp, '\0' , 4 );

		memset	( a_shi_str, '\0' , 25 );

		if( odyn_for_Ku.GetFieldValue	( "KEN_CODE", a_ken_code_tmp, 3 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "住所DBフィールドのデータの取得に失敗\n");
			a_iRet	= false;
			break;
		}
		if( odyn_for_Ku.GetFieldValue	( "SHI_CODE", a_shi_code_tmp, 4 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "住所DBフィールドのデータの取得に失敗\n");
			a_iRet	= false;
			break;
		}
		if( odyn_for_Ku.GetFieldValue	( "SHI_KANJI", a_shi_str, 25 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "住所DBフィールドのデータの取得に失敗\n");
			a_iRet	= false;
			break;
		}

		addr_dat			a_iAdd;
		a_iAdd.e_sCode		= a_ken_code_tmp;
		a_iAdd.e_sCode		+=a_shi_code_tmp;
		a_iAdd.e_sKen_Kanji	= c_sKen;
		a_iAdd.e_sShi_Kanji	= c_sShi;

		c_aDat	= a_iAdd;
		break;	// 一回だけで充分なので抜ける
	//	odyn_for_Ku.MoveNext();
	}
	return	( a_iRet );
	//--------------------------------------------------------------------
}

bool addr_db_handle::
LoadAddress_DB( const char *c_cpCode )	///< 行政コード
{

	// 既読データの確認
	is_addr_dat		a_iIndx;
	addr_dat		a_aSKey;	// サーチキー

	a_aSKey.e_sCode	= c_cpCode;
	a_iIndx	= m_sDatas.find ( a_aSKey );
	if( a_iIndx != m_sDatas.end() ) {	// 既に読み込んだデータ
		return	( true );
	}

	// 文字コードの桁数チェック
	size_t	a_sCodeLen	= 0;
	a_sCodeLen	= strlen (c_cpCode);
	if( a_sCodeLen != 5 && a_sCodeLen != 11 ) {
		return	( false );
	}

	// 赤坂さんのソースを参考に
	char	AdrFive[6];
	char	KenCode[3];
	char	ShiCode[4];
//	char	OazaCode[4];

	memcpy( AdrFive  , c_cpCode , 5 );		AdrFive[5] = '\0';
	memcpy( KenCode  , c_cpCode , 2 );		KenCode[2] = '\0';
	memcpy( ShiCode  , c_cpCode+2 , 3 );	ShiCode[3] = '\0';
//	memcpy( OazaCode , c_cpCode+5 , 3 );	OazaCode[3]= '\0';

	///	SQL文
	char	tmpsql[1148];
	memset( tmpsql , '\0' , 1148 );
	
	string	a_sSql;

	a_sSql	+= "SELECT "
					"KEN_CODE,"
					"SHI_CODE,"
					"OAZA_CODE,"
					"AZA_CODE,"
					"KEN_KANJI,"
					"SHI_KANJI,"
					"OAZA_KANJI,"
					"AZA_KANJI "
				"from "
					"AREACODE_MASTER "
				"where ";

	sprintf( tmpsql ,	"KEN_CODE='%s' and SHI_CODE='%s'", KenCode, ShiCode );

	a_sSql += tmpsql;

	/// SQL文でデータ取り出す
	addr_dat		a_iTmpAddr;	// 特別区の対応用
	
	ODynaset odyn( m_oAdm_DB , (char*)a_sSql.c_str() );
	char	a_ken_code_tmp[3];
	char	a_shi_code_tmp[4];
	char	a_oaza_code_tmp[4];
	char	a_aza_code_tmp[4];

	char	a_ken_str[9];
	char	a_shi_str[25];
	char	a_oaza_str[37];
	char	a_aza_str[25];
	char	a_cpCode11[12];
//	char	a_cpChimei[62];
	string	addr_str;

	addr_str_work	a_aASWork;

	int	a_iCount	= 0;
	while (!odyn.IsEOF()){
		memset	( a_ken_code_tmp, '\0' , 3 );
		memset	( a_shi_code_tmp, '\0' , 4 );
		memset	( a_oaza_code_tmp, '\0' , 4 );
		memset	( a_aza_code_tmp, '\0' , 4 );

		memset	( a_ken_str, '\0' , 9 );
		memset	( a_shi_str, '\0' , 25 );
		memset	( a_oaza_str, '\0' , 37 );
		memset	( a_aza_str, '\0' , 25 );
		memset	( a_cpCode11, '\0' , 12 );
	//	memset	( a_cpChimei, '\0' , 62 );

		if( odyn.GetFieldValue	( "KEN_CODE", a_ken_code_tmp, 3 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "住所DBフィールドのデータの取得に失敗\n");
			break;
		}
		if( odyn.GetFieldValue	( "SHI_CODE", a_shi_code_tmp, 4 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "住所DBフィールドのデータの取得に失敗\n");
			break;
		}
		if( odyn.GetFieldValue	( "OAZA_CODE", a_oaza_code_tmp, 4 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "住所DBフィールドのデータの取得に失敗\n");
			break;
		}
		if( odyn.GetFieldValue	( "AZA_CODE", a_aza_code_tmp, 4 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "住所DBフィールドのデータの取得に失敗\n");
			break;
		}
		if( odyn.GetFieldValue	( "KEN_KANJI", a_ken_str, 9 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "住所DBフィールドのデータの取得に失敗\n");
			break;
		}
		if( odyn.GetFieldValue	( "SHI_KANJI", a_shi_str, 25 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "住所DBフィールドのデータの取得に失敗\n");
			break;
		}
		if( odyn.GetFieldValue	( "OAZA_KANJI", a_oaza_str, 37 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "住所DBフィールドのデータの取得に失敗\n");
			break;
		}
		if( odyn.GetFieldValue	( "AZA_KANJI", a_aza_str, 25 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "住所DBフィールドのデータの取得に失敗\n");
			break;
		}

	//	sprintf	( tmpstr4, "%s%s%s", tmpstr1, tmpstr2, tmpstr3 );
		sprintf	( a_cpCode11, "%s%s%s%s", a_ken_code_tmp, a_shi_code_tmp, a_oaza_code_tmp, a_aza_code_tmp );
	//	sprintf	( a_cpChimei, "%s%s", a_oaza_str, a_aza_str );
	
	//	addr_str	= tmpstr4;
	
	//	m_Map.insert(m_string::value_type(addr_str, addr_str));
		addr_dat			a_iAdd;
		a_iAdd.e_sCode		= a_cpCode11;
	//	a_iAdd.e_sName		= a_cpChimei;
		a_iAdd.e_sKen_Kanji	= a_ken_str;
		a_iAdd.e_sShi_Kanji	= a_shi_str;
		a_iAdd.e_sOAza_Kanji= a_oaza_str;
		a_iAdd.e_sAza_Kanji	= a_aza_str;

		// ここで文字列を妥当な形式に変換して格納
	//	char*	a_cpCutStr	= NULL;
	//	a_aASWork.Get_Sikutyouson_Name ( tmpstr4, tmpstr3, &a_cpCutStr );// 市区町村名だけ抜く
	//	a_iAdd.e_sName	= a_cpCutStr;

	//	printf("newcode=%s\tname=%s\n", a_cpCode11, a_cpChimei );			
	
		m_sDatas.insert( a_iAdd );
		// 5桁コード検索用のレコードも作る
		if( a_iCount == 0 ) {
			a_iAdd.e_sCode	= AdrFive;
			a_iAdd.e_sOAza_Kanji= "";
			a_iAdd.e_sAza_Kanji	= "";
			m_sDatas.insert( a_iAdd );
			a_iTmpAddr		= a_iAdd;	// 特別区対応の判定用
		}
		odyn.MoveNext();
		a_iCount++;
	}
	odyn.Close();	// クローズしておかないとGetShiKuInfo内でおかしくなる。
	// ここに来てデータがない場合は失敗
	if( a_iCount == 0 ) {
		return	( false );
	}
	// 特別区対応用 "xx市　oo区"の場合は、xx市の5桁を取得する
	long	a_lPos	= 0;
	a_lPos	= a_iTmpAddr.e_sShi_Kanji.find ( "市　" );
	if( a_lPos != a_iTmpAddr.e_sShi_Kanji.npos ) {	// 全角スペースで検索
		long	a_lLen	= 0;
		a_lLen	= a_iTmpAddr.e_sShi_Kanji.length();
		// 最後の1文字を取り出し比較
		if(a_lLen - 2 > 0) {
			if( a_iTmpAddr.e_sShi_Kanji.substr(a_lLen-2,2) == "区") {
				// "市"で文字列を区切る
				string	a_sShi;
				a_sShi	= a_iTmpAddr.e_sShi_Kanji.substr ( 0, a_lPos+2 );
				
				addr_dat	a_iTmpDat;
				if( GetShiKuInfo( m_oAdm_DB, a_iTmpAddr.e_sKen_Kanji, a_sShi, a_iTmpDat ) ) {
					string		a_sKeyStr;
					a_sKeyStr	= a_iTmpDat.e_sKen_Kanji;	// 都道府県名
					a_sKeyStr	+=a_iTmpDat.e_sShi_Kanji;	// 市区町村名
					m_sRevData.insert( m_addr_dat::value_type( a_sKeyStr, a_iTmpDat ) );
				}
			}
		}
	}
	return	( true );
}

bool addr_db_handle::
LoadOldAddress_DB( const char *c_cpCode )	///< 現行政コード
{

	// 既読データの確認
	is_addr_dat		a_iIndx;
	addr_dat		a_aSKey;	// サーチキー

	a_aSKey.e_sCode	= c_cpCode;
	a_iIndx	= m_sOldDatas.find ( a_aSKey );
	if( a_iIndx != m_sOldDatas.end() ) {
	}

	// 赤坂さんのソースを参考に
	char	AdrFive[6];
	char	KenCode[3];
	char	ShiCode[4];

	memcpy( AdrFive , c_cpCode , 5 );	AdrFive[5] = '\0';
	memcpy( KenCode , c_cpCode , 2 );	KenCode[2] = '\0';
	memcpy( ShiCode , c_cpCode+2 , 3 );	ShiCode[3] = '\0';

	///	SQL文
	char	sql[1148];
	memset( sql , '\0' , 1148 );

	// 旧コードを取ってくる
	sprintf( sql ,	"SELECT "
						"DISTINCT "
						"KEN_CODE,"
						"SHI_CODE, "
						"SHI_KANJI "
					"from "
						"AREACODE_MASTER "
					"where "
						"(NEW_KEN_CODE='%s' and NEW_SHI_CODE='%s') and "
						"not(KEN_CODE='%s' and SHI_CODE='%s') ",
						KenCode, ShiCode, KenCode, ShiCode);
					//	"and OAZA_CODE=%d) and AZA_CODE=%d)", );

	/// SQL文でデータ取り出す
	ODynaset odyn( m_oAdm_DB , sql );
	char	tmpstr1[10];
	char	tmpstr2[10];
	char	tmpstr3[25];
	char	tmpstr4[45];
	string	addr_str;

	addr_str_work	a_aASWork;

	int	a_iCount	= 0;
	while (!odyn.IsEOF()){
		memset				( tmpstr1, '\0' , 10 );
		memset				( tmpstr2, '\0' , 10 );
		memset				( tmpstr3, '\0' , 25 );
		memset				( tmpstr4, '\0' , 45 );
	
		if( odyn.GetFieldValue	( "KEN_CODE", tmpstr1, 3 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "住所DBフィールドのデータの取得に失敗\n");
			break;
		}
		if( odyn.GetFieldValue	( "SHI_CODE", tmpstr2, 4 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "住所DBフィールドのデータの取得に失敗\n");
			break;
		}
		if( odyn.GetFieldValue	( "SHI_KANJI", tmpstr3, 25 ) != OSUCCESS ) {
			fprintf	( m_fpErr_Fp, "住所DBフィールドのデータの取得に失敗\n");
			break;
		}
		sprintf	( tmpstr4, "%s%s", tmpstr1, tmpstr2 );
	
		addr_dat		a_iAdd;
	//	a_iAdd.e_sCode	= tmpstr4;
		a_iAdd.e_sCode	= c_cpCode;	// 現コードを入れる

		// ここで文字列を妥当な形式に変換して格納
		char*	a_cpCutStr	= NULL;
		a_aASWork.Get_Sikutyouson_Name ( tmpstr4, tmpstr3, &a_cpCutStr );// 市区町村名だけ抜く
//		a_iAdd.e_sName	= a_cpCutStr;
printf("oldcode=%s\tname=%s\n", c_cpCode, a_cpCutStr );		
		m_sOldDatas.insert( a_iAdd );		
		odyn.MoveNext();
		a_iCount++;
	}
	return	( true );
}


/**
	データ検索
*/
addr_dat	addr_db_handle::
Find_Addr_Data ( char*	c_cpKey	)	///< サーチキー（5桁コード）
{
	addr_dat	a_aRet;		// 返却値
	addr_dat	a_aSKey;	// サーチキー

	a_aSKey.e_sCode	= c_cpKey;

	is_addr_dat	a_iIndx;
	a_iIndx	= m_sDatas.find ( a_aSKey );
	if( a_iIndx != m_sDatas.end() ) {
		a_aRet	= *a_iIndx;
	}
	return	( a_aRet );
}

/**
	旧名リストの最初を返す
*/
bool	addr_db_handle::
Find_Old_Name_First	( char*		c_cpKey,	///<サーチキー（5桁コード）
					  addr_dat& c_aDat )	///< 旧名データ[OUT]
{
	addr_dat	a_aSKey;	// サーチキー

	a_aSKey.e_sCode	= c_cpKey;

	m_iOldIndx	= m_sOldDatas.find ( a_aSKey );
	if( m_iOldIndx != m_sOldDatas.end() ) {
		c_aDat	= *m_iOldIndx;
	}else {
		return	( false );
	}
	return	( true );
}

/**
	旧名リストの次を返す
*/
bool	addr_db_handle::
Find_Old_Name_Next	( char*		c_cpKey,	///<サーチキー（5桁コード）
					  addr_dat&	c_aDat )	///<旧名データ[OUT]
{
	m_iOldIndx++;
	if( m_iOldIndx != m_sOldDatas.end() ) {
		c_aDat	= *m_iOldIndx;
	}else {
		return	( false );
	}
	return	( true );
}

/**
	名称についてのチェックを行う
*/
/*
bool repre_pt_chk::
NameChk(	const char*	c_cpName, 		///< 比較元名称文字列[IN]
			char*		c_cpCmpStr )	///< 比較に使用した文字列[out]
{
    const char *p1, *p2;
    char *p;
    char Buf[64];
    char Buf2[64];
	char a_cpConvName[64];
	memset	( a_cpConvName, '\0', 64 );
    
    p1 = strchr(c_cpName, ' ');
    p1++;
    p1 = strchr(p1, ' ');
    p1++;
    if(strcmp(&(e_cpCode[8]), "000") == 0){
        p2 = strchr(p1, ' ');
        strncpy(Buf, p1, p2-p1);
        Buf[p2-p1] = '\0';
        CutSpace(Buf);
        if(strstr(Buf, "大字") == Buf){
            strcpy(a_cpConvName, Buf+4);	// 大字から後をコピー
        }else if(strstr(Buf, "字") == Buf){
            strcpy(a_cpConvName, Buf+2);	// 字から後をコピー
        }else{
            strcpy(a_cpConvName, Buf);		// なければそのままコピー
        }
    }else if(e_cpCode[8] == '0'){
        p2 = strchr(p1, ' ');
        p2++;
        strncpy(Buf, p1, p2-p1-1);
        if((p = strstr(p2, "丁")) != NULL){		// 丁目の前までを取り出す
            *p = '\0';
        }
        strcpy(&(Buf[p2-p1-1]), p2);
        CutSpace(Buf);
        strcpy(a_cpConvName, Buf);
    }else{
        p2 = strchr(p1, ' ');
        if(*(p2+1) == '\0'){
            strncpy(Buf, p1, p2-p1);
            Buf[p2-p1] = '\0';
        }else{
            strcpy(Buf, p2+1);
        }
        CutSpace(Buf);
        if(strstr(Buf, "字") == Buf){
            strcpy(a_cpConvName, Buf+2);
        }else{
            strcpy(a_cpConvName, Buf);
        }
    }
	strcpy(c_cpCmpStr, a_cpConvName);	// 2003.10.30 追加
    if(strcmp(a_cpConvName, e_cpName) != 0){
		ChgKetsu(e_cpName, Buf2);	//　ケ、ツの処理
		if(strcmp(a_cpConvName, Buf2) == 0){
			return true;
		}
		//fprintf(stderr, "名称が違う,%s,%s,%s\n", Err->AddrCode, Err->Name, Err->Name2);
	}else {
		return	true;
	}
	// だめだった場合は文字列を返す
    return false;
}*/

/**
	'ケ'、'ヶ'、'ツ','ッ'の違いを統一する
*//*
void repre_pt_chk::
ChgKetsu(const char*	c_cpSrc,	///< 変換前の文字列
		char*			c_cpTgt)	///< 変換後の文字列
{
    int i;
    int len = strlen(c_cpSrc);

    for(i = 0; i < len; i += 2){
        if(memcmp(c_cpSrc+i, "ヶ", 2) == 0){
            memcpy(c_cpTgt+i, "ケ", 2);
        }else if(memcmp(c_cpSrc+i, "ッ", 2) == 0){
            memcpy(c_cpTgt+i, "ツ", 2);
        }else{
            memcpy(c_cpTgt+i, c_cpSrc+i, 2);
        }
    }
    *(c_cpTgt+i) = '\0';
}
*/
/**
	スペースのカット
*//*
void repre_pt_chk::
CutSpace(char *Buf)		///< 文字列
{
    char *p;

    if((p = strstr(Buf, "　大字")) != NULL){
        strcpy(p, p+6);
    }else if((p = strstr(Buf, "　字")) != NULL){
        strcpy(p, p+4);
    }
    while((p = strstr(Buf, "　")) != NULL){
        strcpy(p, p+2);
    }
}
*/
