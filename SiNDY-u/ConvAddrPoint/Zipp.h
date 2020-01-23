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
* @file	Zipp.h
*/

// テーブル名定義
const CString C_ZIPP_TABLE_001 = sindy::schema::zip_point::kTableName;

// 検索時のソートフィールド定義
const CString C_ZIPP_SORT_001 = CString(sindy::schema::zip_point::kZipCode) + _T(",") + 
								CString(sindy::schema::zip_point::kPrefCode) + _T(",") +
								CString(sindy::schema::zip_point::kCityCode) + _T(",") +
								CString(sindy::schema::zip_point::kAddrCode1) + _T(",") +
								CString(sindy::schema::zip_point::kAddrCode2);

// 出力ファイル名定義
const CString C_ZIPP_FILE_001 = _T("ZIPP001.tsv");


//-----------
// CZipp001
//-----------
class CZipp001 : public CAdrpBase
{
public:
	CZipp001(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CZipp001() {};
	bool init(void);

	/**
	* @brief	件数表用リスト取得
	* @return	件数表用リスト
	*/
	std::map<long, std::vector<long>> getCodeCountList() const { return m_codeCountList; }

private:
	bool convRecord(const _IRowPtr& ipRow);

protected:
	long m_idxZipCode;
	long m_idxZipAtt;
	long m_idxName;
	long m_idxYomi;
	long m_idxPrefCode;
	long m_idxCityCode;
	long m_idxAddrCode1;
	long m_idxAddrCode2;
	long m_idxRep;
	std::map<long, std::vector<long>> m_codeCountList;	// 件数表用リスト
};

