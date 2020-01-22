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

// ErrorHandle.h: CErrorHandle クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_)
#define AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WinLib/VersionInfo.h>
#include <io.h>

/**
 * エラー大分類定義
 */
#define E_MISMATCH_ADDRCODE11_NOTSEIBI	100	// 行政界が未整備で11桁住所コード異なる
#define E_MISMATCH_ADDRCODE11_SEIBI		200	// 行政界が整備済で11桁住所コード異なる
#define E_MISMATCH_GAIKU				300	// 行政界が整備済で街区が異なる
#define E_MATCH_GAIKU					400	// 行政界が整備済で街区が一致

/**
 * エラーコード定義
 */
#define	N_GOU_CSPOINT_NOTSEIBI			1	// CSポイントで取得(未整備)
#define W_GOU_ADDRCODE_SEIBI			2	// 11桁コード不一致(整備済)
#define E_GOU_GAIKU_SEIBI				3	// 街区符号が異なる(整備済)
#define W_GOU_OAZACODE_SEIBI			25	// 8桁コード不一致（GOU整備） ... Ver 1.1.0.3 より追加
#define E_EXGOU_NOTSEIBI				4	// GOU拡張整備なのに未整備(整備済)
#define W_EXGOU_ADDRCODE_SEIBI			5	// 11桁コード不一致(整備済)
#define E_EXGOU_GAIKU_SEIBI				24	// 街区符号（親地番）一致（整備済み） ... Ver 1.0.2.2 より追加
#define W_EXGOU_OAZACODE_SEIBI			26	// 8桁コード不一致（GOU拡張整備） ... Ver 1.1.0.3 より追加
#define	E_TPG							6	// 調査結果コード選択誤り(TPG整備)
#define	N_REF_ADDRCODE_NOTSEIBI			7	// 11桁コード不一致(未整備)
#define	W_REF_ADDRCODE_SEIBI			8	// 11桁コード不一致(整備済)
#define	N_REF_GAIKU_NOTSEIBI			9	// 街区符号が異なる(整備済)
#define	N_NUM_ADDRCODE_NOTSEIBI			10	// 11桁コード不一致(未整備)
#define	W_NUM_ADDRCODE_SEIBI			11	// 11桁コード不一致(整備済)
#define	N_NUM_GAIKU_NOTSEIBI			12	// 街区符号が異なる(整備済)
#define	W_ADMIN_ADDRCODE_NOTSEIBI		13	// 行政界不一致なのにデータと一致(未整備)
#define	W_ADMIN_ADDRCODE_SEIBI			14	// 行政界不一致なのにデータと一致(整備済)
#define	E_NOSURVEY_OPERATOR				15	// 調査結果コード属性付与漏れ
#define W_MISMATCH_CITYCODE				16	// 5桁コード不一致
#define E_ON_EXGAIKUFUGO				17	// 街区符号拡張の上にポイント
#define E_UNKNOWN_POINTSTAT				18	// 不明の調査コード
#define E_DUP_OR_NOTON_ADMIN			19	// 行政界の境界上にあるまたは行政界上にない
#define E_ON_NGCITYSITE					20	// 位置不正(都市地図背景)
#define E_ON_NGBASESITE					21	// 位置不正(中縮背景)
#define E_DUP_OR_NOTON_CITYSITE			22	// 都市地図背景の境界上にあるまたは行政界上にない
#define E_DUP_OR_NOTON_BASESITE			23	// 中縮背景の境界上にあるまたは行政界上にない

#define W_ALPS_CITYCODE					27	// 5桁コード不一致[アルプス] ... Ver 1.1.0.3 より追加
#define W_ALPS_OAZACODE					28	// 8桁コード不一致[アルプス] ... Ver 1.1.0.3 より追加
#define N_ALPS_ADDRCODE_SEIBI			29	// 11桁コード不一致（整備済）[アルプス] ... Ver 1.1.0.3 より追加
#define N_ALPS_ADDRCODE_NOTSEIBI		30	// 11桁コード不一致（未整備）[アルプス] ... Ver 1.1.0.3 より追加
#define N_ALPS_GAIKU_SEIBI				31	// 街区符号が異なる（整備済）[アルプス] ... Ver 1.1.0.3 より追加
#define E_ALPS_ON_NGCITYSITE			32	// 位置不正（都市地図背景）[アルプス] ... Ver 1.1.0.3 より追加
#define E_ALPS_ON_NGBASESITE			33	// 位置不正（中縮背景）[アルプス] ... Ver 1.1.0.3 より追加
#define E_ALPS_DUP_OR_NOTON_CITYSITE	34	// 都市地図背景の境界上にあるのでチェックできません[アルプス] ... Ver 1.1.0.3 より追加
#define E_ALPS_DUP_OR_NOTON_BASESITE	35	// 中縮背景の境界上にあるのでチェックできません[アルプス] ... Ver 1.1.0.3 より追加

#define E_SAME_ADDRCODE					36	// 同一20桁複数有り ... Ver 2.1.2.4 より追加（Bug9296で追加）

/**
 * @class	CErrorHandle
 */
class CErrorHandle  
{
public:

	/**
	 * @brief	デフォルトコンストラクタ
	 */
	CErrorHandle() : m_bIsAddMode(false){}
	virtual ~CErrorHandle(){};

	/**
	 * @brief	フィーチャクラス名セット
	 * @param	lpszFCName	[in]	フィーチャクラス名
	 */
	void setFCName( LPCTSTR lpszFCName ){ m_strFCName = lpszFCName; }

	/**
	 * @brief	ファイルオープン
	 * @param	lpcszFile	[in]	ファイル名
	 * @return	bool
	 */
	bool open( LPCTSTR lpcszFile )
	{
		// 先にファイル存在チェック
		m_bIsAddMode = (_access(lpcszFile, 0) != -1)? true : false;
		int nOpenMode = m_bIsAddMode? std::ios::out|std::ios::app : std::ios::out;
		m_fout.open( static_cast<CT2CW>(lpcszFile), nOpenMode );
		return ( m_fout? true : false );
	}

	/**
	 * @brief	ヘッダー出力
	 */
	void printHeader()
	{
		// 追記モードでなければヘッダー出力
		if( !m_bIsAddMode )
		{
			CVersion cVer;
			CString strMsg;
			strMsg.Format( _T("#SINDYSTDLOG\n")
						   _T("#%s FILEVERSION %s PRODUCTVERSION %s\n")
						   _T("#対応状況\tレイヤ名\tオブジェクトID\tX\tY\tエラーレベル\tエラーコード\tエラーメッセージ\t")
						   _T("住所名\t住所コード\t方書名称\t掲載名称\t調査結果\t情報ソース\t住所検索ポイント化フラグ\t同一20桁保持OID"),
						   cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
			m_fout << strMsg<< std::endl;
		}
	}

	/**
	 * @brief	ログ出力
	 * @param	cExTP			[in]	ExTPポイント
	 * @param	nErrorCode		[in]	エラーコード
	 * @param	strBGDomain	[in]	背景種別ドメイン
	 */
	void printLog( const CExTP& ob, int nErrorCode, const std::string& strBGDomain = "" );

	/**
	 * @brief	ログ出力
	 * @param	cExTP			[in]	ExTPポイント
	 * @param	nErrorCode		[in]	エラーコード
	 * @param	lOID			[in]	同一20桁コード保持相手のオブジェクトID
	 */
	void printLog( const CExTP& ob, int nErrorCode, const long lOID );		// 同一20桁コード保持相手のオブジェクトID追加版（Bug 9296対応）

private:

	/**
	 * @brief	エラー大分類から、エラーコードを取得
	 * @param	nErrorState	[in]	エラー大分類
	 * @return	エラーコード
	 */
	int getErrorCode( int nErrorState, POINTSTAT_C ePointStat );

	/**
	 * @brief	エラーコードからエラーメッセージ取得
	 * @param	nErrorCode	[in]	エラーコード
	 * @param	strBGDomain	[in]	背景種別ドメイン
	 * @return	エラーメッセージ
	 */
	const std::string getErrorMsg( int nErrorCode, const std::string& strBGDomain = "" ) const;

private:

	bool			m_bIsAddMode;			//!< 追記モード？

	std::ofstream	m_fout;					//!< ログファイル出力ストリーム

	CString			m_strFCName;			//!< フィーチャクラス名

	static const LPCTSTR	m_lpszErrMsg[];	//!< エラーメッセージ一覧

};

#endif // !defined(AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_)
