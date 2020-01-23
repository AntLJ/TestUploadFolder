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
 * @file	Adrp.h
 */

// テーブル名定義
const CString C_ADRP_TABLE_001 = sindy::schema::pref_point::kTableName;
const CString C_ADRP_TABLE_002 = sindy::schema::city_point::kTableName;
const CString C_ADRP_TABLE_003 = sindy::schema::oaza_point::kTableName;
const CString C_ADRP_TABLE_004 = sindy::schema::aza_point::kTableName;
const CString C_ADRP_TABLE_005 = sindy::schema::main_no_point::kTableName;
const CString C_ADRP_TABLE_006 = sindy::schema::sub_no_point::kTableName;

// 検索時のソートフィールド定義
const CString C_ADRP_SORT_001 = sindy::schema::pref_point::kPrefCode;
const CString C_ADRP_SORT_002 = CString(sindy::schema::city_point::kPrefCode) + _T(",") + CString(sindy::schema::city_point::kCityCode);
const CString C_ADRP_SORT_003 = CString(sindy::schema::oaza_point::kPrefCode) + _T(",") + CString(sindy::schema::oaza_point::kCityCode) + _T(",") + CString(sindy::schema::oaza_point::kAddrCode1);
const CString C_ADRP_SORT_004 = CString(sindy::schema::aza_point::kPrefCode) + _T(",") + CString(sindy::schema::aza_point::kCityCode) + _T(",") + CString(sindy::schema::aza_point::kAddrCode1) + _T(",") + CString(sindy::schema::aza_point::kAddrCode2);
const CString C_ADRP_SORT_005 = _T("");	// 005と006はコンテナに入れてソートするため検索時のソートは不要
const CString C_ADRP_SORT_006 = _T("");	// 005と006はコンテナに入れてソートするため検索時のソートは不要

// 出力ファイル名定義
const CString C_ADRP_FILE_001 = _T("ADRP001.tsv");
const CString C_ADRP_FILE_002 = _T("ADRP002.tsv");
const CString C_ADRP_FILE_003 = _T("ADRP003.tsv");
const CString C_ADRP_FILE_004 = _T("ADRP004.tsv");
const CString C_ADRP_FILE_005 = _T("ADRP005.tsv");
const CString C_ADRP_FILE_006 = _T("ADRP006.tsv");

/**
 * @class	CAdrpBase
 * @brief	ADRP変換ベース変換クラス
 */
class CAdrpBase abstract : public CAddr2TsvBase
{
public:
	CAdrpBase(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrpBase(void) {};

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
	CFileEUC	m_cFile;
};

/**
 * @class	CAdrp001
 * @brief	ADRP001変換クラス
 */
class CAdrp001 : public CAdrpBase
{
public:
	CAdrp001(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrp001(void) {};
	bool init(void);

	/**
	* @brief	件数表用コード名称対応リスト取得
	* @return	件数表用コード名称対応リスト
	*/
	std::map<CString, CString> getCodeNameList() const { return m_codeNameList; }

private:
	bool convRecord(const _IRowPtr& ipRow);

protected:
	long m_idxPrefCode;
	long m_idxName;
	long m_idxYomi;
	std::map<CString, CString> m_codeNameList;	// 件数表用コード名称対応リスト
};

/**
 * @class	CAdrp002
 * @brief	ADRP002変換クラス
 */
class CAdrp002 : public CAdrpBase
{
public:
	CAdrp002(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrp002(void) {};
	bool init(void);

	/**
	* @brief	5桁コードリスト取得
	* @return	5桁コードリスト
	*/
	std::set<std::pair<CString, CString>> getCodeList() const { return m_codeList; }

	/**
	* @brief	件数表用コード名称対応リスト取得
	* @return	件数表用コード名称対応リスト
	*/
	std::map<std::pair<CString, CString>, CString> getCodeNameList() const { return m_codeNameList; }

private:
	bool convRecord(const _IRowPtr& ipRow);

private:
	long m_idxPrefCode;
	long m_idxCityCode;
	long m_idxName;
	long m_idxYomi;
	long m_idxAdminAtt;
	std::set<std::pair<CString, CString>> m_codeList;	// 5桁コードリスト
	std::map<std::pair<CString, CString>, CString> m_codeNameList;	// 件数表用コード名称対応リスト
};

/**
* @class	CAdrp003
* @brief	ADRP003変換クラス
*/
class CAdrp003 : public CAdrpBase
{
public:
	CAdrp003(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrp003(void) {};
	bool init(void);

	/**
	* @brief	件数表用リスト取得
	* @return	件数表用リスト
	*/
	std::map<std::pair<CString, CString>, long> getCodeCountList() const { return m_codeCountList; }

private:
	bool convRecord(const _IRowPtr& ipRow);

protected:
	long m_idxPrefCode;
	long m_idxCityCode;
	long m_idxAddrCode1;
	long m_idxName;
	long m_idxYomi;
	long m_idxPrefix;
	long m_idxSuffix;
	long m_idxNameCode;
	long m_idxAddrDisp;
	std::map<std::pair<CString, CString>, long> m_codeCountList;	// 件数表用リスト
};

/**
* @class	CAdrp004
* @brief	ADRP004変換クラス
*/
class CAdrp004 : public CAdrpBase
{
public:
	CAdrp004(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrp004(void) {};
	bool init(void);

	/**
	* @brief	件数表用リスト取得
	* @return	件数表用リスト
	*/
	std::map<std::pair<CString, CString>, long> getCodeCountList() const { return m_codeCountList; }

private:
	bool convRecord(const _IRowPtr& ipRow);

protected:
	long m_idxPrefCode;
	long m_idxCityCode;
	long m_idxAddrCode1;
	long m_idxAddrCode2;
	long m_idxName;
	long m_idxYomi;
	long m_idxPrefix;
	long m_idxSuffix;
	long m_idxNameCode;
	long m_idxAddrDisp;
	std::map<std::pair<CString, CString>, long> m_codeCountList;	// 件数表用リスト
};


/**
 * @class	CAdrpListBase
 * @brief	ADRP変換(5桁コードリスト使用)ベース変換クラス
 */
class CAdrpListBase abstract : public CAddr2TsvBase
{
public:
	CAdrpListBase(const CParam& cParam, const IWorkspacePtr& ipWS, const std::set<std::pair<CString, CString>>& codeList);
	~CAdrpListBase(void) {};

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
	* @brief	出力
	* @note		純粋仮想関数（継承先で実装が必要）
	*/
	virtual void output(void) = 0;

	/**
	* @brief	各種設定
	*/
	void setOutDirName(const CString& strOutDirName) { m_strOutDirName = strOutDirName; };
	void setOutFileName(const CString& strOutFileName) { m_strOutFileName = strOutFileName; };
	void setWhereFormat(const CString& strWhereFormat) { m_strWhereFormat = strWhereFormat; };

protected:
	CString				m_strOutDirName;
	CString				m_strOutFileName;
	CString				m_strWhereFormat;
	CFileEUC			m_cFile;
	const std::set<std::pair<CString, CString>>& m_codeList;	// 5桁コードリスト
};

/**
* @class	CAdrpData005
* @brief	ADRP005データ
*/
struct CAdrpData005
{
	CString strPrefCode;
	CString strCityCode;
	CString strAddrCode1;
	CString strAddrCode2;
	CString strChibanCode;
	CString strBldOID;		// BLD_OIDは、NULLの可能性があるため文字列で保持
	long    lBldOID;		// BLD_OIDは、ソート時は数値で比較するため数値も保持
	int     mesh;
	int     x;
	int     y;
	CString strName;
	CString strYomi;
	CString	strStatus;
	CString strPntType;
	CString strAddrDisp;
	CString strRef;
	CString strBranch;
	CString strMulti;
	CString strGnrtType;

public:
	CAdrpData005() : lBldOID(0xFFFFFFFF), mesh(0), x(0), y(0) {};

	// 演算子オーバーロードで比較関数を定義
	bool operator<(const CAdrpData005& right) const {
		if (strPrefCode != right.strPrefCode)
			return strPrefCode < right.strPrefCode;
		if (strCityCode != right.strCityCode)
			return strCityCode < right.strCityCode;
		if (strAddrCode1 != right.strAddrCode1)
			return strAddrCode1 < right.strAddrCode1;
		if (strAddrCode2 != right.strAddrCode2)
			return strAddrCode2 < right.strAddrCode2;
		if (strChibanCode != right.strChibanCode)
			return strChibanCode < right.strChibanCode;
		if (lBldOID != right.lBldOID)	// 比較は数値で比較
			return lBldOID < right.lBldOID;
		if (mesh != right.mesh)
			return mesh < right.mesh;
		if (x != right.x)
			return x < right.x;
		return y < right.y;
	}
};

/**
* @class	CAdrpData006
* @brief	ADRP006データ
*/
struct CAdrpData006
{
	CString strPrefCode;
	CString strCityCode;
	CString strAddrCode1;
	CString strAddrCode2;
	CString strChibanCode;
	CString strJukyoCode;
	CString strBldOID;		// BLD_OIDは、NULLの可能性があるため文字列で保持
	long    lBldOID;		// BLD_OIDは、ソート時は数値で比較するため数値も保持
	int     mesh;
	int     x;
	int     y;
	CString strName;
	CString strYomi;
	CString strRef;
	CString strBranch;
	CString strMulti;
	CString strGnrtType;

public:
	CAdrpData006() : lBldOID(0xFFFFFFFF), mesh(0), x(0), y(0) {};

	// 演算子オーバーロードで比較関数を定義
	bool operator<(const CAdrpData006& right) const {
		if (strPrefCode != right.strPrefCode)
			return strPrefCode < right.strPrefCode;
		if (strCityCode != right.strCityCode)
			return strCityCode < right.strCityCode;
		if (strAddrCode1 != right.strAddrCode1)
			return strAddrCode1 < right.strAddrCode1;
		if (strAddrCode2 != right.strAddrCode2)
			return strAddrCode2 < right.strAddrCode2;
		if (strChibanCode != right.strChibanCode)
			return strChibanCode < right.strChibanCode;
		if (strJukyoCode != right.strJukyoCode)
			return strJukyoCode < right.strJukyoCode;
		if (lBldOID != right.lBldOID)	// 比較は数値で比較
			return lBldOID < right.lBldOID;
		if (mesh != right.mesh)
			return mesh < right.mesh;
		if (x != right.x)
			return x < right.x;
		return y < right.y;
	}
};

/**
 * @class	CAdrp005
 * @brief	ADRP005変換クラス
 */
class CAdrp005 : public CAdrpListBase
{
public:
	CAdrp005(const CParam& cParam, const IWorkspacePtr& ipWS, const std::set<std::pair<CString, CString>>& codeList);
	~CAdrp005(void) {};
	bool init(void);

	/**
	* @brief	件数表用リスト取得
	* @return	件数表用リスト
	*/
	std::map<std::pair<CString, CString>, std::vector<long>> getCodeCountList() const { return m_codeCountList; }

private:
	bool convRecord(const _IRowPtr& ipRow);
	void output(void);

private:
	long m_idxPrefCode;
	long m_idxCityCode;
	long m_idxAddrCode1;
	long m_idxAddrCode2;
	long m_idxChibanCode;
	long m_idxBldOID;
	long m_idxName;
	long m_idxYomi;
	long m_idxStatus;
	long m_idxPntType;
	long m_idxAddrDisp;
	long m_idxRef;
	long m_idxBranch;
	long m_idxMulti;
	long m_idxGnrtType;
	std::vector<CAdrpData005> m_outData;	// ADRP出力用コンテナ（ソートする必要があるため一旦コンテナに保持）
	std::map<std::pair<CString, CString>, std::vector<long>> m_codeCountList;	// 件数表用リスト
};

/**
* @class	CAdrp006
* @brief	ADRP006変換クラス
*/
class CAdrp006 : public CAdrpListBase
{
public:
	CAdrp006(const CParam& cParam, const IWorkspacePtr& ipWS, const std::set<std::pair<CString, CString>>& codeList);
	~CAdrp006(void) {};
	bool init(void);

	/**
	* @brief	件数表用リスト取得
	* @return	件数表用リスト
	*/
	std::map<std::pair<CString, CString>, std::vector<long>> getCodeCountList() const { return m_codeCountList; }

private:
	bool convRecord(const _IRowPtr& ipRow);
	void output(void);

private:
	long m_idxPrefCode;
	long m_idxCityCode;
	long m_idxAddrCode1;
	long m_idxAddrCode2;
	long m_idxChibanCode;
	long m_idxJukyoCode;
	long m_idxBldOID;
	long m_idxName;
	long m_idxYomi;
	long m_idxRef;
	long m_idxBranch;
	long m_idxMulti;
	long m_idxGnrtType;
	std::vector<CAdrpData006> m_outData;	// ADRP出力用コンテナ（ソートする必要があるため一旦コンテナに保持）
	std::map<std::pair<CString, CString>, std::vector<long>> m_codeCountList;	// 件数表用リスト
};