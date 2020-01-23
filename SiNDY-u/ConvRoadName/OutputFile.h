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

#include <codecvt>

class OutputFile
{
public:
	OutputFile(LPCTSTR outputDir)
	 :m_inited(false) 
	{
		m_roadNamePath.Format(_T("%s\\%s.txt"), outputDir, ROAD_NAME);
		m_relRoadNameLinkPath.Format(_T("%s\\%s.txt"), outputDir, REL_ROAD_NAME_LINK);
		m_spltRoadNamePath.Format(_T("%s\\%s.txt"), outputDir, SPLIT_ROAD_NAME);
	}
	~OutputFile() {}

public:
	/**
	 * @brief 初期化
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool init();

	/**
	 * @brief ファイル出力
	 * @param rdNameRecs	[in] 道路名称データレコード変換結果
	 * @param rlNameLnkRecs [in] 道路リンクに紐づく道路名称レコード変換結果
	 * @param spltRdNameRecs [in] 道路名称分割データレコード変換結果
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool write(const std::set<RoadNameRecord>& rdNameRecs, const std::set<RelNameLinkRecord>& rlNameLnkRecs, const std::set<SpltNameRecord>& spltRdNameRecs);

private:
	/**
	 * @brief ファイル削除
	 * @note ファイルが存在する場合のみ削除
	 * @param filePath [in]
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool deleteFile(LPCTSTR filePath);

	/**
	 * @brief 道路名称ファイル出力
	 * @param rdNameRecs [in] 道路名称データレコード
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool writeRoadName(const std::set<RoadNameRecord>& rdNameRecs);

	/**
	 * @brief 道路リンクに紐づく道路出力
	 * @param rlNameLnkRecs [in] 道路リンクに紐づく道路レコード
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool writeRelNameLink(const std::set<RelNameLinkRecord>& rlNameLnkRecs);

	/**
	 * @brief 道路名称分割データ出力
	 * @param spltRdNameRecs [in] 道路名称分割データレコード
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool writeSplitRoadName(const std::set<SpltNameRecord>& spltRdNameRecs);

	/**
	 * @brief UTF-16→UTF-8変換
	 * @param utf16Str [in] UTF16文字列
	 * @return UTF8文字列
	 */
	inline CStringA UTF16toUTF8(const CStringW& utf16Str)
	{
		// ワイド文字列とバイト文字列を相互変換するクラスを初期化
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;

		// UTF-16→UTF-8変換
		std::string utf8Str = conv.to_bytes(utf16Str);

		return utf8Str.c_str();
	}

private:
	bool m_inited;

	CString m_roadNamePath;
	CString m_relRoadNameLinkPath;
	CString m_spltRoadNamePath;
	

};

