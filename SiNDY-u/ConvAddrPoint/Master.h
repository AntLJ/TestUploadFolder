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
* @file	Master.h
*/

// テーブル名定義
const CString C_MST_TABLE_ADM = sindy::schema::admin_att_master::kTableName;
const CString C_MST_TABLE_PFX = sindy::schema::prefix_master::kTableName;
const CString C_MST_TABLE_SFX = sindy::schema::suffix_master::kTableName;

// 検索時のソートフィールド定義
const CString C_MST_SORT_ADM = CString(sindy::schema::admin_att_master::kAdminAtt);
const CString C_MST_SORT_PFX = CString(sindy::schema::prefix_master::kPrefix);
const CString C_MST_SORT_SFX = CString(sindy::schema::suffix_master::kSuffix);

// コード値ドメイン名定義
const CString C_MST_CODEDOMAIN_NMA = _T("公称通称種別コード");
const CString C_MST_CODEDOMAIN_RAA = _T("住居表示区域コード");
const CString C_MST_CODEDOMAIN_CBA = _T("街区・地番識別コード");
const CString C_MST_CODEDOMAIN_GMA = _T("生成データ種別コード");
const CString C_MST_CODEDOMAIN_CST = _T("地番状態コード");
const CString C_MST_CODEDOMAIN_ZPA = _T("郵便番号種別コード");

// 出力ファイル名定義
const CString C_MST_FILE_ADM = _T("MSTADM.tsv");
const CString C_MST_FILE_PFX = _T("MSTPFX.tsv");
const CString C_MST_FILE_SFX = _T("MSTSFX.tsv");
const CString C_MST_FILE_NMA = _T("MSTNMA.tsv");
const CString C_MST_FILE_RAA = _T("MSTRAA.tsv");
const CString C_MST_FILE_CBA = _T("MSTCBA.tsv");
const CString C_MST_FILE_GMA = _T("MSTGMA.tsv");
const CString C_MST_FILE_CST = _T("MSTCST2.tsv");
const CString C_MST_FILE_ZPA = _T("MSTZPA2.tsv");

//-----------
// CMstAdm
//-----------
class CMstAdm : public CAdrpBase
{
public:
	CMstAdm(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CMstAdm() {};
	bool init(void);

private:
	bool convRecord(const _IRowPtr& ipRow);

protected:
	long m_idxAdminatt;
	long m_idxName;
};

//-----------
// CMstPfx
//-----------
class CMstPfx : public CAdrpBase
{
public:
	CMstPfx(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CMstPfx() {};
	bool init(void);

private:
	bool convRecord(const _IRowPtr& ipRow);

protected:
	long m_idxPrefix;
	long m_idxName;
	long m_idxYomi;
};

//-----------
// CMstSfx
//-----------
class CMstSfx : public CAdrpBase
{
public:
	CMstSfx(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CMstSfx() {};
	bool init(void);

private:
	bool convRecord(const _IRowPtr& ipRow);

protected:
	long m_idxSuffix;
	long m_idxName;
	long m_idxYomi;
};

//----------------
// CCodeDomain2Tsv
//----------------
class CCodeDomain2Tsv
{
public:
	CCodeDomain2Tsv(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CCodeDomain2Tsv() {};

	/**
	* @brief	処理実行
	* @param	strCodeDomainName	[in]	コード値ドメイン名
	* @param	strOutFileName		[in]	出力ファイル名
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool execute(const CString& strCodeDomainName, const CString& strOutFileName);

private:
	/**
	* @brief	処理実行
	* @param	strCodeDomainName	[in]	コード値ドメイン名
	* @param	mapDomain			[out]	コード値とドメイン名リスト
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool get(const CString& strCodeDomainName, std::map<long, CString>& mapDomain);

	/**
	* @brief	各種設定
	*/
	void setWorkspace(const IWorkspacePtr& ipWS) { m_ipWorkspace = ipWS; };
	void setOutDirName(const CString& strOutDirName) { m_strOutDirName = strOutDirName; };

protected:
	IWorkspacePtr	m_ipWorkspace;
	CString			m_strOutDirName;
};