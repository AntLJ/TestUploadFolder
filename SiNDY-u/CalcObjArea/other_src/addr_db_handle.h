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

//--------------------------------------------
// 住所DB取り扱い用クラス・・
// eleven.cpp等をベースに作成
//--------------------------------------------
#include <io.h>
#include <stdio.h>
#include <string>
#include <map>
#include <set>
#include <Oracl.h>	// オラクル用

#ifndef	_ADDR_DB_HANDLE_H_
#define	_ADDR_DB_HANDLE_H_

//using namespace stlport;
using namespace std;

// モード設定用定義
#define	READ_ODB	0x01	///< オラクルDB接続
#define	READ_TDB	0x02	///< テキストDB接続

typedef map<string, string, less<string> >	m_string;
typedef map<string, int, less<string> >		m_string2;
typedef set<string, less<string> >			l_string;

struct	addr_dat
{
	string	e_sCode;	///< 住所コード
//	string	e_sName;	///< 住所名

	string	e_sKen_Kanji;
	string	e_sShi_Kanji;
	string	e_sOAza_Kanji;
	string	e_sAza_Kanji;

	addr_dat() {
	}
	~addr_dat() {
	}
	bool	operator<(const addr_dat c_aDat) const {
		bool	a_bRet	= false;
		if( e_sCode != c_aDat.e_sCode ) {
			if( e_sCode < c_aDat.e_sCode ) {
				a_bRet	= true;
			}
		}
		return	( a_bRet );
	}
};

typedef set<string, less<string> >							s_string;
typedef set<string, less<string> >::iterator				is_string;

typedef	set<addr_dat, less<addr_dat> >						s_addr_dat;
typedef	set<addr_dat, less<addr_dat> >::iterator			is_addr_dat;
typedef	map<string, addr_dat, less<string> >				m_addr_dat;
typedef	map<string, addr_dat, less<string> >::iterator		im_addr_dat;

/**
	住所DBの取り扱い用クラス
*/
class	addr_db_handle
{
	string		m_sDBPath;		///< テキストDBのパス
    ODatabase	m_oAdm_DB;		///< 住所DBへのアクセス
	// TEXT_DBで使用
	m_string	m_Map;			///< 読み込んだテキストDBの格納領域
	m_string2	m_City;			///< 読み込んだテキストDBの格納領域(5桁用)
    l_string	m_Ready;		///< 読み込み済み5桁名称のリスト
	// DBで使用
	s_addr_dat	m_sDatas;		///< 住所データの格納領域
	s_addr_dat	m_sOldDatas;	///< 新コードと旧名称のデータセット
	is_addr_dat	m_iOldIndx;		///< 旧データのインデックス
	int			m_iMode;		///< DB読み込みモード
	bool		m_bExec_OK;		///< 実行可能かどうか 2004.09.06

	m_addr_dat	m_sRevData;		///< 地名→5桁用の情報

	FILE*		m_fpErr_Fp;		///< エラー出力用ファイルポインタ 2009.01.26

	/**
		テキストDBの読み込み
	*/
	bool		LoadAddress_TextDB(
					const char*	c_cpCode
				);
	
	/**
		オラクル住所DBの読み込み
	*/
	bool		LoadAddress_DB(
					const char*	c_cpCode
				);
	/**
		オラクル住所DBの読み込み
	*/
	bool		LoadOldAddress_DB(
					const char*	c_cpCode
				);

	/**
		区に対応する市の5桁コードの情報を取得する
	*/
	bool		GetShiKuInfo(
					ODatabase&	c_oAdm_DB,	///< 住所DB[IN]
					string&		c_sKen,		///< 都道府県名[IN]
					string&		c_sShi,		///< 市区町村名[IN]
					addr_dat&	c_aDat		///< 地名データ[OUT]
				);
					

public:
	/**
		テキスト住所DBのロード
	*/
    bool		LoadAddress	(
					const char*	c_cpCode	///<	住所コード
				);
	/**
	*/
    void		FormatStr	(
					char*		Addr,
					const char*	Src
				);

	/**
		実行可能かどうかを返す
		@return	実行可能	true
		@return	実行不能	false
	*/
	bool		Exec_OK	( void ) {
					return	( m_bExec_OK );
				};

public:
	enum ret_value	{
		RET_OK		= 1,	///	正しいコード
		RET_HAISHI,			///	廃止になったコード
		RET_000				///	000がつく
	};
	// コンストラクタ
	addr_db_handle	( ) {
		m_Map.clear		();
		m_City.clear	();
		m_Ready.clear	();
		m_iMode			= 0;	///< どのモードでもない
		m_bExec_OK		= false;
		m_fpErr_Fp		= stderr;
	}
	// デストラクタ
	~addr_db_handle	( ) {
		m_Map.clear		();
		m_City.clear	();
		m_Ready.clear	();
	}

	// 初期化(DB)
	bool	Init_DB		(
				char*	c_cpConnectStr,				///< DB初期化文字列
				FILE*	c_fpErr_Fp=NULL
			);

	// 初期化(TEXTDB)
	bool	Init_TextDB (
				char*	c_cpDBPath,					///< テキストDBパス名
				FILE*	c_fpErr_Fp=NULL
			);

	/**
		行政コードの正当性チェック
		@return RET_OK		正しいコード
		@return RET_HAISHI	廃止になったコード
		@return RET_000		000がつく
	*/
    int		IsValidCode11(
				const char	*Code,					///< 11桁コード
				char		*AreaName
			);

	/**
		行政コードの正当性チェック（5桁用）
		@return RET_OK		正しいコード
		@return RET_HAISHI	廃止になったコード
		@return RET_000		000がつく
	*/
	int		IsValidCode5 (
				const char	*Code			///< 5桁コード
			);

	/**
		OracleDBから直で取ってきた文字列を取得
	*/
	bool 	GetOraDBStr	(
				const char	*Code,		///< 11桁コード[IN]
				addr_dat&	c_aDat		///< 地名データ[OUT]
			);

	/**
		区名から市の5桁等の情報を取得する
	*/
	bool 	GetCitybyWard(
				addr_dat&	c_aInputDat,///< 入力情報[IN]
				addr_dat&	c_aRetDat	///< 返却情報[IN]
			);

	/**
		データ検索
	*/
	addr_dat	Find_Addr_Data	(
					char*	c_cpKey		///< サーチキー（5桁コード）
				);

	/**
		旧名リストの最初を返す
	*/
	bool		Find_Old_Name_First	( 
					char*		c_cpKey,	///<サーチキー（5桁コード）
					addr_dat&	c_aDat		///< 旧名データ[OUT]
				);

	/**
		旧名リストの次を返す
	*/
	bool		Find_Old_Name_Next	(
					char*		c_cpKey,	///<サーチキー（5桁コード）
					addr_dat&	c_aDat		///<旧名データ[OUT]
				);
};

#endif	// _ADDR_DB_HANDLE_H_
