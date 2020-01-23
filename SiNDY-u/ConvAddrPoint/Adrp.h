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

// �e�[�u������`
const CString C_ADRP_TABLE_001 = sindy::schema::pref_point::kTableName;
const CString C_ADRP_TABLE_002 = sindy::schema::city_point::kTableName;
const CString C_ADRP_TABLE_003 = sindy::schema::oaza_point::kTableName;
const CString C_ADRP_TABLE_004 = sindy::schema::aza_point::kTableName;
const CString C_ADRP_TABLE_005 = sindy::schema::main_no_point::kTableName;
const CString C_ADRP_TABLE_006 = sindy::schema::sub_no_point::kTableName;

// �������̃\�[�g�t�B�[���h��`
const CString C_ADRP_SORT_001 = sindy::schema::pref_point::kPrefCode;
const CString C_ADRP_SORT_002 = CString(sindy::schema::city_point::kPrefCode) + _T(",") + CString(sindy::schema::city_point::kCityCode);
const CString C_ADRP_SORT_003 = CString(sindy::schema::oaza_point::kPrefCode) + _T(",") + CString(sindy::schema::oaza_point::kCityCode) + _T(",") + CString(sindy::schema::oaza_point::kAddrCode1);
const CString C_ADRP_SORT_004 = CString(sindy::schema::aza_point::kPrefCode) + _T(",") + CString(sindy::schema::aza_point::kCityCode) + _T(",") + CString(sindy::schema::aza_point::kAddrCode1) + _T(",") + CString(sindy::schema::aza_point::kAddrCode2);
const CString C_ADRP_SORT_005 = _T("");	// 005��006�̓R���e�i�ɓ���ă\�[�g���邽�ߌ������̃\�[�g�͕s�v
const CString C_ADRP_SORT_006 = _T("");	// 005��006�̓R���e�i�ɓ���ă\�[�g���邽�ߌ������̃\�[�g�͕s�v

// �o�̓t�@�C������`
const CString C_ADRP_FILE_001 = _T("ADRP001.tsv");
const CString C_ADRP_FILE_002 = _T("ADRP002.tsv");
const CString C_ADRP_FILE_003 = _T("ADRP003.tsv");
const CString C_ADRP_FILE_004 = _T("ADRP004.tsv");
const CString C_ADRP_FILE_005 = _T("ADRP005.tsv");
const CString C_ADRP_FILE_006 = _T("ADRP006.tsv");

/**
 * @class	CAdrpBase
 * @brief	ADRP�ϊ��x�[�X�ϊ��N���X
 */
class CAdrpBase abstract : public CAddr2TsvBase
{
public:
	CAdrpBase(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrpBase(void) {};

	/**
	* @brief	������
	* @retval	true	����
	* @retval	false	���s
	*/
	bool init(void);

	/**
	* @brief	�������s
	* @retval	true	����
	* @retval	false	���s
	*/
	bool execute(void);

protected:
	/**
	* @brief	�e��ݒ�
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
 * @brief	ADRP001�ϊ��N���X
 */
class CAdrp001 : public CAdrpBase
{
public:
	CAdrp001(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrp001(void) {};
	bool init(void);

	/**
	* @brief	�����\�p�R�[�h���̑Ή����X�g�擾
	* @return	�����\�p�R�[�h���̑Ή����X�g
	*/
	std::map<CString, CString> getCodeNameList() const { return m_codeNameList; }

private:
	bool convRecord(const _IRowPtr& ipRow);

protected:
	long m_idxPrefCode;
	long m_idxName;
	long m_idxYomi;
	std::map<CString, CString> m_codeNameList;	// �����\�p�R�[�h���̑Ή����X�g
};

/**
 * @class	CAdrp002
 * @brief	ADRP002�ϊ��N���X
 */
class CAdrp002 : public CAdrpBase
{
public:
	CAdrp002(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrp002(void) {};
	bool init(void);

	/**
	* @brief	5���R�[�h���X�g�擾
	* @return	5���R�[�h���X�g
	*/
	std::set<std::pair<CString, CString>> getCodeList() const { return m_codeList; }

	/**
	* @brief	�����\�p�R�[�h���̑Ή����X�g�擾
	* @return	�����\�p�R�[�h���̑Ή����X�g
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
	std::set<std::pair<CString, CString>> m_codeList;	// 5���R�[�h���X�g
	std::map<std::pair<CString, CString>, CString> m_codeNameList;	// �����\�p�R�[�h���̑Ή����X�g
};

/**
* @class	CAdrp003
* @brief	ADRP003�ϊ��N���X
*/
class CAdrp003 : public CAdrpBase
{
public:
	CAdrp003(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrp003(void) {};
	bool init(void);

	/**
	* @brief	�����\�p���X�g�擾
	* @return	�����\�p���X�g
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
	std::map<std::pair<CString, CString>, long> m_codeCountList;	// �����\�p���X�g
};

/**
* @class	CAdrp004
* @brief	ADRP004�ϊ��N���X
*/
class CAdrp004 : public CAdrpBase
{
public:
	CAdrp004(const CParam& cParam, const IWorkspacePtr& ipWS);
	~CAdrp004(void) {};
	bool init(void);

	/**
	* @brief	�����\�p���X�g�擾
	* @return	�����\�p���X�g
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
	std::map<std::pair<CString, CString>, long> m_codeCountList;	// �����\�p���X�g
};


/**
 * @class	CAdrpListBase
 * @brief	ADRP�ϊ�(5���R�[�h���X�g�g�p)�x�[�X�ϊ��N���X
 */
class CAdrpListBase abstract : public CAddr2TsvBase
{
public:
	CAdrpListBase(const CParam& cParam, const IWorkspacePtr& ipWS, const std::set<std::pair<CString, CString>>& codeList);
	~CAdrpListBase(void) {};

	/**
	* @brief	������
	* @retval	true	����
	* @retval	false	���s
	*/
	bool init(void);

	/**
	* @brief	�������s
	* @retval	true	����
	* @retval	false	���s
	*/
	bool execute(void);

protected:
	/**
	* @brief	�o��
	* @note		�������z�֐��i�p����Ŏ������K�v�j
	*/
	virtual void output(void) = 0;

	/**
	* @brief	�e��ݒ�
	*/
	void setOutDirName(const CString& strOutDirName) { m_strOutDirName = strOutDirName; };
	void setOutFileName(const CString& strOutFileName) { m_strOutFileName = strOutFileName; };
	void setWhereFormat(const CString& strWhereFormat) { m_strWhereFormat = strWhereFormat; };

protected:
	CString				m_strOutDirName;
	CString				m_strOutFileName;
	CString				m_strWhereFormat;
	CFileEUC			m_cFile;
	const std::set<std::pair<CString, CString>>& m_codeList;	// 5���R�[�h���X�g
};

/**
* @class	CAdrpData005
* @brief	ADRP005�f�[�^
*/
struct CAdrpData005
{
	CString strPrefCode;
	CString strCityCode;
	CString strAddrCode1;
	CString strAddrCode2;
	CString strChibanCode;
	CString strBldOID;		// BLD_OID�́ANULL�̉\�������邽�ߕ�����ŕێ�
	long    lBldOID;		// BLD_OID�́A�\�[�g���͐��l�Ŕ�r���邽�ߐ��l���ێ�
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

	// ���Z�q�I�[�o�[���[�h�Ŕ�r�֐����`
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
		if (lBldOID != right.lBldOID)	// ��r�͐��l�Ŕ�r
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
* @brief	ADRP006�f�[�^
*/
struct CAdrpData006
{
	CString strPrefCode;
	CString strCityCode;
	CString strAddrCode1;
	CString strAddrCode2;
	CString strChibanCode;
	CString strJukyoCode;
	CString strBldOID;		// BLD_OID�́ANULL�̉\�������邽�ߕ�����ŕێ�
	long    lBldOID;		// BLD_OID�́A�\�[�g���͐��l�Ŕ�r���邽�ߐ��l���ێ�
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

	// ���Z�q�I�[�o�[���[�h�Ŕ�r�֐����`
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
		if (lBldOID != right.lBldOID)	// ��r�͐��l�Ŕ�r
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
 * @brief	ADRP005�ϊ��N���X
 */
class CAdrp005 : public CAdrpListBase
{
public:
	CAdrp005(const CParam& cParam, const IWorkspacePtr& ipWS, const std::set<std::pair<CString, CString>>& codeList);
	~CAdrp005(void) {};
	bool init(void);

	/**
	* @brief	�����\�p���X�g�擾
	* @return	�����\�p���X�g
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
	std::vector<CAdrpData005> m_outData;	// ADRP�o�͗p�R���e�i�i�\�[�g����K�v�����邽�߈�U�R���e�i�ɕێ��j
	std::map<std::pair<CString, CString>, std::vector<long>> m_codeCountList;	// �����\�p���X�g
};

/**
* @class	CAdrp006
* @brief	ADRP006�ϊ��N���X
*/
class CAdrp006 : public CAdrpListBase
{
public:
	CAdrp006(const CParam& cParam, const IWorkspacePtr& ipWS, const std::set<std::pair<CString, CString>>& codeList);
	~CAdrp006(void) {};
	bool init(void);

	/**
	* @brief	�����\�p���X�g�擾
	* @return	�����\�p���X�g
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
	std::vector<CAdrpData006> m_outData;	// ADRP�o�͗p�R���e�i�i�\�[�g����K�v�����邽�߈�U�R���e�i�ɕێ��j
	std::map<std::pair<CString, CString>, std::vector<long>> m_codeCountList;	// �����\�p���X�g
};