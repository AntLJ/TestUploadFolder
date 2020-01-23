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

#if !defined(AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_)
#define AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/**
 * エラー大分類定義
 */
//なし

/**
 * エラーコード定義
 */
#define W_GAITOUSISETU_ADDRCODE_NOTSEIBI	1	// 11桁コード不一致(未整備)
#define W_GAITOUSISETU_ADDRCODE_SEIBI		2	// 11桁コード不一致(整備済)
#define W_GAITOUSISETU_OAZACODE				3	// 8桁コード不一致
#define W_GAITOUSISETU_CITYCODE				4	// 5桁コード不一致
#define N_HORYU_POINTSTAT					5	// 保留である
#define	E_NOSURVEY_OPERATOR					6	// 調査結果コード属性付与漏れ
#define E_UNKNOWN_POINTSTAT					7	// 不明の調査コード
#define E_DUP_OR_NOTON_ADMIN				8	// 行政界の境界上にあるまたは行政界上にない

#define W_TELNUM_FREECALL					9	// 電話番号がフリーダイヤル

#define E_ON_NGCITYSITE						10	// 位置不正(都市地図背景)
#define E_ON_NGBASESITE						11	// 位置不正(中縮背景)
#define E_DUP_OR_NOTON_CITYSITE				12	// 都市地図背景の境界上にあるのでチェックできません
#define E_DUP_OR_NOTON_BASESITE				13	// 中縮背景の境界上にあるのでチェックできません

#define E_PLURAL_OPERATOR					14	// ポイント郡で調査結果コードが違うポイントがある
#define E_POI_TABOO_IPCODE					15  // 使用不可POI付近に整備
#define E_SAME_SEQUENCE						16	// 同一シーケンス複数有り(Bug9295で追加）

/**
 * @class	CErrorHandle
 */
class CErrorHandle  
{
public:

	/**
	 * @brief	デフォルトコンストラクタ
	 */
	CErrorHandle()
	{
	}

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
		m_fout.open( static_cast<CT2CW>(lpcszFile), std::ios::out );
		return ( m_fout? true : false );
	}

	/**
	 * @brief	ヘッダー出力
	 * @param	lpszToolVer	[in]	ツールバージョン
	 */
	void printHeader( LPCTSTR lpszVer )
	{
		m_fout << "#" << lpszVer << std::endl;
		m_fout << "#SINDYSTDLOG" << std::endl;
		m_fout << "#対応状況\tレイヤ名\tオブジェクトID\tX\tY\tエラーレベル\tエラーコード\tエラーメッセージ\t"
			   << "住所名\t住所コード\t方書名称\t掲載名称\t調査結果\t情報ソース\t電話番号\t座標付与種別\tシーケンス\tTelTPとPOIとの距離(m)" << std::endl;
	}

	/**
	 * @brief	ログ出力
	 * @param	cTelTP			[in]	TelTPポイント
	 * @param	nErrorCode		[in]	エラーコード
	 * @param	lpstrBGDomain	[in]	背景種別ドメイン
	 */
	void printLog( const CTelTP& ob, int nErrorCode, const std::string* lpstrBGDomain = NULL );

	/**
	 * @brief	ログ出力
	 * @param	cTelTP			[in]	TelTPポイント
	 * @param	nErrorCode		[in]	エラーコード
	 * @param	dist			[in]	TelTPとPOIとの距離
	 */
	void printLog( const CTelTP& ob, int nErrorCode, const double dist);	// 距離追加版 (Bug 5701対応 07/11/30追加)

private:

	/**
	 * @brief	エラーコードからエラーメッセージ取得
	 * @param	nErrorCode	[in]	エラーコード
	 * @param	lpstrBGDomain	[in]	背景種別ドメイン
	 * @return	エラーメッセージ
	 */
	const std::string getErrorMsg( int nErrorCode, const std::string* lpstrBGDomain = NULL ) const;

private:

	std::ofstream	m_fout;					/// ログファイル出力ストリーム

	CString			m_strFCName;			/// フィーチャクラス名

	static const LPCTSTR	m_lpszErrMsg[];	/// エラーメッセージ一覧

};

#endif // !defined(AFX_ERRORHANDLE_H__7E3A5BEB_2D1E_4D3F_8155_F4CF2E7982EB__INCLUDED_)
