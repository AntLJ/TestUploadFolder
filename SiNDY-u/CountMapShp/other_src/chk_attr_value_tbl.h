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

#include "util_class.h"
#include <string>
#include <map>

#ifndef	_CHK_ATTR_VALUE_TBL_H_
#define	_CHK_ATTR_VALUE_TBL_H_

using	namespace	std;

#define	DISP_TOP	0x8
#define	DISP_MID	0x4
#define	DISP_BASE	0x2
#define	DISP_CITY	0x1

// 表示無し		= 0x00
// 文字列あり	= 0x01
// 記号あり		= 0x02
// 指示点化あり	= 0x04
#define	NO_DISP		0x00
#define	EX_STR		0x01
#define	EX_MARK		0x02
#define	EX_POINT	0x04

struct	attr_record
{
	int		e_iMeshDigit;	///< 該当するメッシュの桁数
	string	e_sLayerName;	///< レイヤ名(実際は
	string	e_sFieldName;	///< フィールド名
	string	e_sDomainType;	///< ドメインタイプ
	string	e_sDomainName;	///< 属性名称
	string	e_sAttrName;	///< 属性名称（ドメインタイプがrangeの場合は持たない）
	long	e_lValue;		///< 属性値
	long	e_lMin;			///< 最小値
	long	e_lMax;			///< 最大値


	attr_record() {
		e_iMeshDigit= 0;
		e_lValue	= 0;
		e_lMin		= 0;
		e_lMax		= 0;
	}

	/**
		比較演算子定義
		@return	bool
	*/
	bool	operator<(const attr_record c_aDat ) const {
		bool	a_bRet	= false;
		if( e_iMeshDigit != c_aDat.e_iMeshDigit ) {
			if( e_iMeshDigit < c_aDat.e_iMeshDigit ) {
				a_bRet	= true;
			}			
		}else
		if( e_sLayerName != c_aDat.e_sLayerName ) {	///< レイヤ名
			if( e_sLayerName < c_aDat.e_sLayerName ) {
				a_bRet	= true;
			}
		}else
		if( e_sFieldName != c_aDat.e_sFieldName ) {	///< フィールド名
			if( e_sFieldName < c_aDat.e_sFieldName ) {
				a_bRet	= true;
			}
		}else
		if( e_sDomainType != c_aDat.e_sDomainType ) {	///< ドメインタイプ
			if( e_sDomainType < c_aDat.e_sDomainType ) {
				a_bRet	= true;
			}
		}else
		if( e_lValue != c_aDat.e_lValue ) {		///< 属性値
			if( e_lValue < c_aDat.e_lValue ) {
				a_bRet	= true;
			}
		}
		return ( a_bRet );
	}

};

typedef	map<int, attr_record, less<int> >					m_attr_record;
typedef	map<int, attr_record, less<int> >::iterator			im_attr_record;
typedef	set<attr_record, less<attr_record> >				s_attr_record;
typedef	set<attr_record, less<attr_record> >::iterator		is_attr_record;

/**
	スケール別表示ルール
*/
struct	scale_disp_rule
{
	long	e_lDispRule[4];
	
	scale_disp_rule() {
		int i = 0;
		for( i = 0; i < 4; i++ )
		{
			e_lDispRule[i]	= 0;
		}
	}
	~scale_disp_rule() {
	}

	void	operator=( scale_disp_rule c_aDat ) {
		int i = 0;
		for(i = 0; i < 4; i++ )
		{
			e_lDispRule[i]	= c_aDat.e_lDispRule[i];
		}
	}

};

/**
	注記表示ルールの扱い
*/
struct	note_disp_rule
{
	long			e_lValue;			///< 属性値
	string			e_sName;			///< 属性名
	int				e_iDispType;		///< 表示タイプ（線状等）
	int				e_iDispFlag;		///< 表示フラグ
	scale_disp_rule	e_lTopDispRule;		///< トップマップ表示ルール
	scale_disp_rule	e_lMidDispRule;		///< ミドルマップ表示ルール
	scale_disp_rule	e_lBaseDispRule;	///< ベースマップ表示ツール
	scale_disp_rule	e_lCityDispRule;	///< 都市地図表示ルール

	note_disp_rule() {
		e_iDispFlag		= 0;
		e_iDispType		= 0;
		e_sName			= "";
		e_lValue		= 0;
	}
	~note_disp_rule() {
	}

	void	operator=( note_disp_rule c_aDat ) {
		e_lValue		= c_aDat.e_lValue;			///< 属性値
		e_sName			= c_aDat.e_sName;			///< 属性名
		e_iDispType		= c_aDat.e_iDispType;		///< 表示タイプ（線状等）
		e_iDispFlag		= c_aDat.e_iDispFlag;		///< 表示フラグ
		e_lTopDispRule	= c_aDat.e_lTopDispRule;	///< トップマップ表示ルール
		e_lMidDispRule	= c_aDat.e_lMidDispRule;	///< ミドルマップ表示ルール
		e_lBaseDispRule	= c_aDat.e_lBaseDispRule;	///< ベースマップ表示ツール
		e_lCityDispRule	= c_aDat.e_lCityDispRule;	///< 都市地図表示ルール		
	}

	/**
		比較演算子定義
		@return	bool
	*/
	bool	operator<(const note_disp_rule c_aDat ) const {
		bool	a_bRet	= false;
		if( e_lValue != c_aDat.e_lValue ) {			///< 属性値
			if( e_lValue < c_aDat.e_lValue ) {
				a_bRet	= true;
			}			
		}
		return ( a_bRet );
	}
};

typedef	multiset<note_disp_rule, less<note_disp_rule> >				ms_note_disp_rule;
typedef	multiset<note_disp_rule, less<note_disp_rule> >::iterator	ims_note_disp_rule;

class	chk_attr_value_tbl
{

private:
	s_attr_record		e_sAttr_Value_Tbl;	// 属性値のテーブル
	ms_note_disp_rule	e_sNote_Disp_Rule;	// 注記の表示ルール
	FILE*		e_fpLog;
	util_class	e_uCu;

public:
	chk_attr_value_tbl () {
		e_fpLog	= stderr;
	}
	~chk_attr_value_tbl () {
	}

	/**
		ログファイルの設定
	*/
	bool	Log_Init	(
		FILE*	c_fpLogFp
	);

	/**
		対応テーブルを読み込む
	*/
	bool	read_table	(
		char*	c_cpRFName
	);

	/**
		対応テーブルを読み込む
	*/
	bool	read_note_disp_rule	(
		char*	c_cpRFName
	);

	/**
		データのサーチを行う
	*/
	bool	find_data	(
		int				c_iMeshDigit,	///< データが該当するメッシュ桁数[IN]
		string&			c_sLayerName,	///< レイヤ名[IN]
		string&			c_sFieldName,	///< フィールド名称[IN]
		int				c_iValue,		///< 属性値
		attr_record*	c_apAttRec		///< 属性レコード[OUT]
	);

	/**
		該当する注記表示ルールを取り出す
	*/
	bool	find_rel	(
		int				c_iMeshDigit,	///< データが該当するメッシュ桁数[IN]
		int				c_iValue,		///< 属性値[IN]
		note_disp_rule*	c_apAttRec		///< 属性レコード[OUT]
	);

};

#endif
