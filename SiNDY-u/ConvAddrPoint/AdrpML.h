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

/**
 * @file	ADrpML.h
 */

 // テーブル名定義
const CString C_ADRPML_TABLE_001 = sindy::schema::multi_language::pref_point::kTableName;
const CString C_ADRPML_TABLE_002 = sindy::schema::multi_language::city_point::kTableName;
const CString C_ADRPML_TABLE_003 = sindy::schema::multi_language::oaza_point::kTableName;
const CString C_ADRPML_TABLE_004 = sindy::schema::multi_language::aza_point::kTableName;
const CString C_ADRPML_TABLE_005 = sindy::schema::multi_language::main_no_point::kTableName;
const CString C_ADRPML_TABLE_006 = sindy::schema::multi_language::sub_no_point::kTableName;

// ソートフィールド定義
const CString C_ADRPML_SORT_001 = CString(sindy::schema::multi_language::pref_point::kPrefCode) + _T(",")
								+ CString(sindy::schema::multi_language::pref_point::kLangCode);
const CString C_ADRPML_SORT_002 = CString(sindy::schema::multi_language::city_point::kPrefCode) + _T(",")
								+ CString(sindy::schema::multi_language::city_point::kCityCode) + _T(",")
								+ CString(sindy::schema::multi_language::city_point::kLangCode);
const CString C_ADRPML_SORT_003 = CString(sindy::schema::multi_language::oaza_point::kPrefCode) + _T(",")
								+ CString(sindy::schema::multi_language::oaza_point::kCityCode) + _T(",")
								+ CString(sindy::schema::multi_language::oaza_point::kAddrCode1) + _T(",")
								+ CString(sindy::schema::multi_language::oaza_point::kLangCode);
const CString C_ADRPML_SORT_004 = CString(sindy::schema::multi_language::aza_point::kPrefCode) + _T(",")
								+ CString(sindy::schema::multi_language::aza_point::kCityCode) + _T(",")
								+ CString(sindy::schema::multi_language::aza_point::kAddrCode1) + _T(",")
								+ CString(sindy::schema::multi_language::aza_point::kAddrCode2) + _T(",")
								+ CString(sindy::schema::multi_language::aza_point::kLangCode);
const CString C_ADRPML_SORT_005 = CString(sindy::schema::multi_language::main_no_point::kPrefCode) + _T(",")
								+ CString(sindy::schema::multi_language::main_no_point::kCityCode) + _T(",")
								+ CString(sindy::schema::multi_language::main_no_point::kAddrCode1) + _T(",")
								+ CString(sindy::schema::multi_language::main_no_point::kAddrCode2) + _T(",")
								+ CString(sindy::schema::multi_language::main_no_point::kChibanCode) + _T(",")
								+ CString(sindy::schema::multi_language::main_no_point::kLangCode);
const CString C_ADRPML_SORT_006 = CString(sindy::schema::multi_language::sub_no_point::kPrefCode) + _T(",")
								+ CString(sindy::schema::multi_language::sub_no_point::kCityCode) + _T(",")
								+ CString(sindy::schema::multi_language::sub_no_point::kAddrCode1) + _T(",")
								+ CString(sindy::schema::multi_language::sub_no_point::kAddrCode2) + _T(",")
								+ CString(sindy::schema::multi_language::sub_no_point::kChibanCode) + _T(",")
								+ CString(sindy::schema::multi_language::sub_no_point::kJukyoCode) + _T(",")
								+ CString(sindy::schema::multi_language::sub_no_point::kLangCode);

// 出力ファイル名定義
const CString C_ADRPML_FILE_001 = _T("ADRP001ML.tsv");
const CString C_ADRPML_FILE_002 = _T("ADRP002ML.tsv");
const CString C_ADRPML_FILE_003 = _T("ADRP003ML.tsv");
const CString C_ADRPML_FILE_004 = _T("ADRP004ML.tsv");
const CString C_ADRPML_FILE_005 = _T("ADRP005ML.tsv");
const CString C_ADRPML_FILE_006 = _T("ADRP006ML.tsv");

/**
 * @class	CAdrpMLBase
 * @brief	ADRPML変換ベース変換クラス
 */
class CAdrpMLBase abstract : public CAddr2TsvBase
{
public:
	CAdrpMLBase(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrpMLBase(void) {};

	/**
	* @brief	初期化
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool init(void);

	/**
	* @brief	処理実行
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool execute(void);

protected:
	/**
	* @brief	各種設定
	*/
	void setOutDirName(const CString& strOutDirName) { m_strOutDirName = strOutDirName; };
	void setOutFileName(const CString& strOutFileName) { m_strOutFileName = strOutFileName; };

protected:
	CString		m_strOutDirName;
	CString		m_strOutFileName;
	CFileUTF8	m_cFile;
};

/**
 * @class	CAdrp001ML
 * @brief	ADRP001ML変換クラス
 */
class CAdrp001ML : public CAdrpMLBase
{
public:
	CAdrp001ML(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrp001ML(void);
	bool init(void);

private:
	bool convRecord(const _IRowPtr& ipRow);

protected:
	long m_idxPrefCode;
	long m_idxLangCode;
	long m_idxName;
};

/**
* @class	CAdrp002ML
* @brief	ADRP002ML変換クラス
*/
class CAdrp002ML : public CAdrpMLBase
{
public:
	CAdrp002ML(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrp002ML(void);
	bool init(void);

private:
	bool convRecord(const _IRowPtr& ipRow);

protected:
	long m_idxPrefCode;
	long m_idxCityCode;
	long m_idxLangCode;
	long m_idxName;
};

/**
* @class	CAdrp003ML
* @brief	ADRP003ML変換クラス
*/
class CAdrp003ML : public CAdrpMLBase
{
public:
	CAdrp003ML(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrp003ML(void);
	bool init(void);

private:
	bool convRecord(const _IRowPtr& ipRow);

protected:
	long m_idxPrefCode;
	long m_idxCityCode;
	long m_idxAddrCode1;
	long m_idxLangCode;
	long m_idxName;
};

/**
* @class	CAdrp004ML
* @brief	ADRP004ML変換クラス
*/
class CAdrp004ML : public CAdrpMLBase
{
public:
	CAdrp004ML(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrp004ML(void);
	bool init(void);

private:
	bool convRecord(const _IRowPtr& ipRow);

protected:
	long m_idxPrefCode;
	long m_idxCityCode;
	long m_idxAddrCode1;
	long m_idxAddrCode2;
	long m_idxLangCode;
	long m_idxName;
};

/**
* @class	CAdrp005ML
* @brief	ADRP005ML変換クラス
*/
class CAdrp005ML : public CAdrpMLBase
{
public:
	CAdrp005ML(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrp005ML(void);
	bool init(void);

private:
	bool convRecord(const _IRowPtr& ipRow);

protected:
	long m_idxPrefCode;
	long m_idxCityCode;
	long m_idxAddrCode1;
	long m_idxAddrCode2;
	long m_idxChibanCode;
	long m_idxLangCode;
	long m_idxName;
};

/**
* @class	CAdrp006ML
* @brief	ADRP006ML変換クラス
*/
class CAdrp006ML : public CAdrpMLBase
{
public:
	CAdrp006ML(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrp006ML(void);
	bool init(void);

private:
	bool convRecord(const _IRowPtr& ipRow);

protected:
	long m_idxPrefCode;
	long m_idxCityCode;
	long m_idxAddrCode1;
	long m_idxAddrCode2;
	long m_idxChibanCode;
	long m_idxJukyoCode;
	long m_idxLangCode;
	long m_idxName;
};
