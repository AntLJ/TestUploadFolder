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

#pragma once

#include "ADOBase.h"

/**
 * @brief 住所サーバ接続管理クラス
 *
 * 殆どはproteusのプロシージャの移植部分です。
 * ただし、5桁レベルまではキャッシュするため、処理データ数
 * が多ければ多いほどプロシージャよりも高速に動作します。
 *
 **/
class CAddrServerBase
{
public:
	CAddrServerBase(void);
	virtual ~CAddrServerBase(void);

	/// 接続する
	bool Connect( LPCTSTR lpcszConn );
	/// オーナーセット
	void SetOwner( LPCTSTR lpcszOwner ){ m_strOwner = lpcszOwner; }
	LPCTSTR GetOwner() const { return m_strOwner; }
	/// プロシージャを使用するかどうか
	void SetUseProcedure( bool bUse ){ m_bUseProcedure = bUse; }
	bool GetUseProcedure() const { return m_bUseProcedure; }

	CADOBase& ADO() { return m_cConn; }
	/**
	* @brief 住所文字列から座標を取得するファンクション(SS-PJで使用)
	* @param lpcszAddr  住所文字列
	* @retval 結果文字列(各属性がカンマ区切りで返される)
	*         各属性は下記の通り
	*          (1) 解析対象住所文字列
	*          (2) 解析精度(どのレベルまで解析できたか)
	*          (3)～(8)  都道府県コード～住居番号コード
	*          (9)～(14) 都道府県名称～住居番号名称
	*          (15) メッシュコード
	*          (16) メッシュX
	*          (17) メッシュY
	*          (18) 座標精度
	*/
	CString addrtocode( LPCTSTR lpcszAddr ) const;

	/// 住所コード取得関数
	CString addr2code( const CString& strAddr ) const;
	CString _addr2code( const CString& strAddr ) const;

	/// 住所コード取得関数２(市区町村コード指定版)
	CString addr2code( const CString& strCityCode, const CString& strAddr ) const;

private:
	/**
	 * @brief 住所コードから住所名取得
	 * @param lpcszAddrCode	[in]		住所コード
	 * @param listName		[in out]	住所名格納用
	 */
	bool GetAddrName( LPCTSTR lpcszAddrCode, std::vector<CString>& listName ) const;

	/**
	 * @brief 住所コードから座標取得
	 * @param lpcszAddrCode	[in]	住所コード
	 * @param seido			[in]	文字列精度
	 * @param mesh			[out]	メッシュコード
	 * @param x				[out]	100万正規化X座標
	 * @param y				[out]	100万正規化Y座標
	 * @param seido_ap		[out]	座標精度
	 */
	bool GetAddrPos( LPCTSTR lpcszAddrCode, long seido, int& mesh, int& x, int& y, long& seido_ap ) const;

	/// 住所文字列を正規化する（揺らぎの吸収）
	static CString CommonExpr( CString& strAdrs );

	/// 検索文字列が先頭から存在した場合にその文字列をカットする
	static CString ReplaceHead( const CString& src, const CString& rep );

	/// 「大字」「字」「小字」の除去
	static CString RemoveAzaHead( const CString& src );

	/// 漢数字→アラビア数字変換関数（「一」～「九十九」→ 「１」～「９９」）
	static CString Kanji2Num( const CString& src );

	/**
	 * @brief	漢数字→アラビア数字変換関数（「一」～「九十九」→ 「１」～「９９」）
	 * @note	何でもかんでも変換せず、以下の条件に合致するもだけ変換
	 *			(1) ○条 ○地割 ○丁 ○線 ○－
	 * @param	SrcStr	[in]	対象データ
	 * @return	変換データ
	 */
	static CString Kanji2NumEx( const CString& src );

	/**
	 * @brief	都道府県コード取得関数
	 * 
	 * strAddrに含まれる都道府県名を前方一致で検索し、名称とコードをセットで返します。
	 * strAddrは都道府県名以外を含んでいてもいいです。
	 *
	 * @param	strAddr		[in,out]		解析文字列（解析できた分は削除される）
	 * @param	doCache		[in,optional]	キャッシュをするかどうか（デフォルト：true）
	 * @retval bool
	 */
	bool GetKenCode( CString& strAddr, CString& strRetKenCode, bool doCache = true ) const;
	/**
	 * @brief	市区町村コード取得関数
	 * 
	 * strAddrに含まれる市区町村名を前方一致で検索し、名称とコードをセットで返します。
	 * strAddrは市区町村名以外を含んでいてもいいです。
	 * なお、戻り値の市区町村名は正規化された文字列であることに注意してください。
	 *
	 * @param	strAddr		[in,out]		解析文字列（解析できた分は削除される）
	 * @param	strKenCode	[in]			都道府県コード
	 * @param	doCache		[in,optional]	キャッシュをするかどうか（デフォルト：true）
	 * @retval bool
	 */
	bool GetShiCode( CString& strAddr, const CString& strKenCode, CString& strRetShiCode, bool doCache = true ) const;

	/**
	* @brief	大字・字コード取得関数
	* @param	strAddr		[in out]	解析文字列
	* @param	strKenCode	[in]		都道府県コード
	* @param	strShiCode	[in]		市区町村コード
	* @param	strOazaCode	[out]		解析結果大字・通称コード
	* @param	strAzaCode	[out]		解析結果字・丁目コード
	*/
	bool GetOazaAzaCode( CString& strAddr, const CString& strKenCode, const CString& strShiCode, CString& strRetOazaCode, CString& strRetAzaCode ) const;

	/**
	 * @brief	地番コード取得関数
	 * @param	AdrsStr		[in out]	解析文字列
	 * @param	RetF		[in]		大字解析? 字解析?
	 * @param	KenCode		[in]		都道府県コード
	 * @param	ShiCode		[in]		市区町村コード
	 * @param	OazaCode	[in]		大字・通称コード
	 * @param	AzaCode		[in out]	字・丁目コード
	 * @param	r_ChibanCode[out]		解析結果地番コード
	 */
	bool GetChibanCode( CString& strAddr, int nRetF, const CString& strKenCode, const CString& strShiCode, const CString& strOazaCode, CString& strAzaCode, CString& strRetChibanCode ) const;

	/**
	 * @brief	住居番号コード取得関数
	 * @param	AdrsStr		[in out]	解析文字列
	 * @param	KenCode		[in]		都道府県コード
	 * @param	ShiCode		[in]		市区町村コード
	 * @param	OazaCode	[in]		大字・通称コード
	 * @param	AzaCode		[in]		字・丁目コード
	 * @param	ChibanCode	[in]		地番コード
	 * @param	r_BangoCode	[in]		解析結果住居番号コード
	 */
	bool GetBangoCode( CString& strAddr, const CString& strKenCode, const CString& strShiCode, const CString& strOazaCode, const CString& strAzaCode, const CString& strChibanCode, CString& strRetBangoCode ) const;

protected:
	CADOBase m_cConn;
	CString m_strOwner;
	bool m_bUseProcedure; //!< プロシージャを使用するかどうか
	bool m_bIsOracle; //!< Oracleかどうか（falseならPostgreSQLという判断）
};
