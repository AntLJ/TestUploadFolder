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

/// �G���[���x��
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
	CString m_LineNo;	// �G���[�p�i�s�ԍ��j
	CString m_Seq;		// �G���[�p�i�V�[�P���X�j
	CString m_Name;		// �G���[�p�i���́j
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
	 * @brief	���W�����`�F�b�N(Bug8763)
	 * @note	1/256 �� �x���b�`�F�b�N
	 * @param	strField	[in]	�t�B�[���h(�o�x�E�ܓx����p)
	 * @param	strData		[in]	�f�[�^
	 * @return	bool
	 */
	bool fnCoordCheck( const CString& strField, const CString& strData );

	/**
	 * @brief	�G���[���O�̏o��(Bug8767)
	 * @param	cErrLv		[in]	�G���[���x��
	 * @param	cMsg		[in]	�G���[���b�Z�[�W
	 * @param	cAdd		[in]	�ǉ����
	 * @return	�Ȃ�
	 */
	void fnOutputErr(EErrLv cErrLv, const CString& cMsg, const CString& cAdd = _T(""));
};
