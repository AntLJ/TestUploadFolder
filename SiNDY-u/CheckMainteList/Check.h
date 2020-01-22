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
using namespace Ipc::PoiMaintenance::Utility::Check;
typedef std::list<CString> HEADER;
typedef std::vector<CString> RECORD;
typedef std::list<RECORD> DATA;
typedef std::multimap<CString, RECORD> COMP_LIST;
typedef std::set<CString> ID_SET;
typedef std::set<CString> MIKAKU_SET;
typedef std::map<CString, int> CHECK_TYPE_MAP;
typedef std::map<CString, int> FIELD_INDEX_MAP;

struct DELETE_INFO {
	long m_DeleteCode;
	CString m_ClosedCode;
	CString m_ClosedMemo;
};

/// エラーレベル
enum EErrLv {
	kWarning,
	kError,
};

class CCheck
{
public:
	CCheck(void);
	~CCheck(void);
	bool init(LPCTSTR cOldDir, LPCTSTR cNewDir, LPCTSTR cLogFile, int cMode, const CString &cMMaster);
	bool execCheck(LPCTSTR cFileName);
private:
	CString m_OldDir;
	CString m_NewDir;
	std::ifstream m_OldFile;
	std::ifstream m_NewFile;
	bool fnHeadComp(void);
	HEADER m_Head;
	DATA m_OrgData;
	COMP_LIST m_NewData;
	ID_SET m_IDSet;
	MIKAKU_SET m_Mikaku;
	CHECK_TYPE_MAP m_CTypeMap;
	FIELD_INDEX_MAP m_FieldMap;
	bool fnAnalyzeHeadrer(const std::string cData, HEADER& cHead);
	long m_LineNoIndex;
	long m_SeqIndex;
	long m_FuncSeqIndex;
	long m_NameIndexOrg;
	long m_NameIndexNew;
	bool fnDataComp(void);
	bool fnLoadOldData(std::string cData);
	bool fnLoadNewData(std::string cData);
	bool fnCompare(void);
	bool fnCompareRecord(const RECORD& cOrgData, const RECORD& cNewData);
	bool fnFieldCheck(LPCTSTR aField, CString cData, const RECORD& cNewData);
	bool fnInfoFileCheck(CString cData);
	FILE* m_pFile;
	CString fnRegulate(const CString& cField, const CString& cData);
	CString fnCutSpace(CString cData);
	CString fnZen2Han(CString cData);
	CString fnHan2Zen(CString cData);
	CString fnCutMultiSpace(CString cData);
	CString fnCutDoubleQuotation(CString cData, LPCTSTR cField);
	CString m_LineNo;	// エラー用（行番号）
	CString m_Seq;		// エラー用（シーケンス）
	CString m_Name;		// エラー用（名称）
	bool fnNewDataCheck(void);
	CString fnConvYomiSP(CString cData);
	bool fnNotNullCheck(CString cField);
//	long fnSJIS2EUCSize(LPCSTR pszSrc, long cchSrc);
	bool fnExistFieldCheck(void);
	bool m_HaveAddr;
	bool m_ParkMode;
	int m_Mode;
	bool fnCompareRecordGS(const RECORD & cOrgData, const RECORD & cNewData);
	bool fnIsWeekData(const CString& cData);
	bool fnIsNotOrig(const CString& cData);
	bool fnLoadMikakuMaster(LPCTSTR cMMaster);
	bool fnSetDeleteInfo(DELETE_INFO& cDelInfo, CString cField, const CString cData);
	CString fnCompRegulate(const CString& cField, CString cData);

	/**
	 * @brief	座標書式チェック(Bug8763)
	 * @note	1/256 と 度分秒チェック
	 * @param	strField	[in]	フィールド(経度・緯度判定用)
	 * @param	strData		[in]	データ
	 * @return	bool
	 */
	bool fnCoordCheck( const CString& strField, const CString& strData );

	/**
	 * @brief	エラーログの出力(Bug8767)
	 * @param	cErrLv		[in]	エラーレベル
	 * @param	cMsg		[in]	エラーメッセージ
	 * @param	cAdd		[in]	追加情報
	 * @return	なし
	 */
	void fnOutputErr(EErrLv cErrLv, const CString& cMsg, const CString& cAdd = _T(""));
};
