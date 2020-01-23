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

class CLogManager
{
public:
	CLogManager(void);
	~CLogManager(void);

	/**
	 * @brief 初期設定クラス 2014/5/27現在ログオープンするだけ
	 * @param logPath [in] ログファイルのパス
	 * @return 初期設定の成否
	 */
	bool Init( const CString& logPath );

	/**
	 * @brief ヘッダ出力 (抽出ログ)
	 */
	void WriteExtractLogHeader();

	/**
	 * @brief ヘッダ出力 (チェックログ)
	 */
	void WriteCheckLogHeader();

	/**
	 * @brief ログに文字列出力する関数
	 * @param [in] message 文字列
	 */
	void WriteInfo( LPCSTR message, ... );
	
	/**
	 * @brief ログ出力関数 (抽出ログ)
	 * @param [in] layer    レイヤ名
	 * @param [in] oid      OBJECTID
	 * @param [in] meshcode メッシュコード
	 * @param [in] errLevel エラーレベル
	 * @param [in] message  メッセージ
	 * @param [in] linkInfo リンク列情報
	 */
	void WriteExtractLog( const CStringA& layer, long oid, long meshcode, const CStringA& errLevel, const CStringA& message, const CStringA& linkInfo );

	/**
	 * @brief ログ出力関数 (チェックログ)
	 * @param [in] layer    レイヤ名
	 * @param [in] oid      OBJECTID
	 * @param [in] errCode  エラーコード
	 * @param [in] lon      経度
	 * @param [in] lat      緯度
	 * @param [in] meshcode 2次メッシュコード
	 * @param [in] linkID   流入リンクID
	 * @param [in] nodeID   流出ノードID
	 */
	void WriteCheckLog( const CStringA& layer, long oid, long errorCode, double lon, double lat, long meshcode, long linkID, long nodeID );

private:
	/**
	 * @brief ログファイルオープン
	 * @param [in] logPath ログパス
	 * @return ログファイルオープンの成否
	 */
	bool Open( const CString& logPath );

	/**
	 * @brief ログファイルクローズ
	 */
	void Close();
	
	/**
	 * @brief 現在時刻(システム時刻)を文字列で返す(YYYY/MM/DD hh:mm:ss)
	 */
	CStringA GetTime();

private:
	std::ofstream m_ofs; //!< ログファイルストリーム
};

