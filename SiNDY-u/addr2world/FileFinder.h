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
#include <atlbase.h>
#include <atlstr.h>
#include <vector>
#include <set>

class FileFinder
{
public:
	FileFinder(void);
	virtual ~FileFinder(void);

	/**
	 * @brief <b>検索の実行</b>\n
	 * @param path [in] 検索を開始するパス
	 * @param pattern [in] ファイル名のパターン
	 * @param subDirectories [in] trueの場合はサブディレクトリも検索する
	 * @return 正常終了時はtrue, エラー発生時はfalse
	 */
	virtual bool Search(LPCTSTR path, LPCTSTR pattern, bool subDirectories = true);

	/**
	 * @brief <b>リストのクリア</b>\n
	 */
	virtual void Clear();

	/**
	 * @brief <b>検出されたファイル数の取得</b>\n
	 * @return 検出されたファイル数
	 */
	virtual int GetCount() const;

	/**
	 * @brief <b>ファイル名の取得</b>\n
	 * @return ファイル名
	 */
	virtual ATL::CString GetFilePath(int index) const;
	virtual ATL::CString operator[](int index) const { return GetFilePath(index); }

	// 除外するディレクトリの設定
	// 事前に設定しなければならない
	// ディレクトリ名称のみでOK
//	virtual void PutExcludeDirectory(LPCTSTR lpDir);
//	virtual void ClearExcludeDirectory();

	void SetDisplayStatus(bool disp = true) { m_DisplayStatus = disp; }

protected:
	/**
	 * @brief <b>フォルダの検索</b>\n
	 * @param path [in] 検索を開始するパス
	 */
	virtual bool SearchDirectories(LPCTSTR path, bool subDirectories, std::set<ATL::CString> &directories);

private:
	std::vector<ATL::CString> m_FileList; //!< 検出されたファイルリスト
	std::set<ATL::CString> m_ExDir; // 除外フォルダ

	bool m_DisplayStatus;

	friend class FileFinderTest;
};

