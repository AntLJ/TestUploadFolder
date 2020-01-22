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
 * @brief �t�B�[���h�^�C�v
 */
namespace FIELDTYPE {
	enum ECode {
		UNKNOWN_TYPE = 0,	//!< ���ݒ�
		SHAPE,				//!< �`��
		INTEGER,			//!< ����
		DOUBLE,				//!< ����
		STRING				//!< ������
	};
}

/**
 * @brief �����͈͗p�N���X
 * @note �Ή����Ă���̂͐��l�^�̂�<br>
 *       �\���C���[�W�͉��L<br>
 *           symbol1 : >  <br>
 *           value1  : 10 <br>
 *         ��L�𐔎��ɂ���Ɓ�[���肵�����ϐ�] > 10 <br>
 *           symbol2 : <=  <br>
 *           value2  : 31 <br>
 *         ��L�𐔎��ɂ���Ɓ�[���肵�����ϐ�] <= 31  <br>
 *         ��L2�����킹���
 *           10 < [���肵�����ϐ�] <= 31 <br>
 *         �ƂȂ�B <br>
 *         �͈͂ƂȂ�Ȃ��g�ݍ��킹�i [���肵�����ϐ�] < 10, [���肵�����ϐ�] >= 31 �j<br>
 *         �ƂȂ�P�[�X�͔F�߂Ȃ��B
 */
class CRange {
public:
	CRange(){}
	~CRange(){}
	
public:

	bool operator<(const CRange& cRange) const {
		return _ttof(this->m_strValue1) != _ttof(cRange.m_strValue1) ? _ttof(this->m_strValue1) < _ttof(cRange.m_strValue1) : 
			_ttof(this->m_strValue2) != _ttof(cRange.m_strValue2) ? _ttof(this->m_strValue2) < _ttof(cRange.m_strValue2) : 
			this->m_strSymbol1 != cRange.m_strSymbol1 ? this->m_strSymbol1 < cRange.m_strSymbol1 : this->m_strSymbol2 < cRange.m_strSymbol2;
	}
	
	bool operator>(const CRange& cRange) const {
		return _ttof(this->m_strValue1) != _ttof(cRange.m_strValue1) ? _ttof(this->m_strValue1) > _ttof(cRange.m_strValue1) : 
			_ttof(this->m_strValue2) != _ttof(cRange.m_strValue2) ? _ttof(this->m_strValue2) > _ttof(cRange.m_strValue2) : 
			this->m_strSymbol1 != cRange.m_strSymbol1 ? this->m_strSymbol1 > cRange.m_strSymbol1 : this->m_strSymbol2 > cRange.m_strSymbol2;
	}

public:
	bool init(const CString& groupName, const CString& column1, const CString& type, const CString& strSymbol1, const CString& strValue1, const CString& strSymbol2, const CString& strValue2);

public:
	CString m_strSymbol1;
	CString m_strValue1;
	CString m_strSymbol2;
	CString m_strValue2;

};

/**
 * @brief �ϊ��t�B�[���h�Ή��\�N���X
 */
class CFieldDefInfo{
public:
	CFieldDefInfo() : m_srcFieldIndex(-1), m_canNull(false), m_Length(-1), m_Number(-1), m_Decimal(-1), m_bNullValue(false), m_bFixedValue(false) {}
	~CFieldDefInfo(){}

public:
	/**
	 * @brief ������
	 * @param groupName [in] �O���[�v��*
	 * @param line [in] �ϊ��t�B�[���h�Ή��\��1���R�[�h
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool init(const CString& groupName, const CString& line);

private:
	/**
	 * @brief �ϊ������i�[
	 * @param groupName [in] �O���[�v��
	 * @param column1 [in] ������i1��ځj
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool setSrcInfo(const CString& groupName, const CString& column1);

	/**
	 * @brief �ϊ�����i�[
	 * @param groupName [in] �O���[�v��
	 * @param column2 [in] ������i2��ځj
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool setDstInfo(const CString& groupName, const CString& column2);

	/**
	 * @brief �t�B�[���h���i�[
	 * @param groupName [in] �O���[�v��
	 * @param column1 [in] �t�B�[���h�Ή��\�̃J����������i1��ځj�i�G���[�o�͕����p�j
	 * @param column3 [in] �t�B�[���h�Ή��\�̃J����������i3��ځj
	 * @param column4 [in] �t�B�[���h�Ή��\�̃J����������i4��ځj
	 * @param column5 [in] �t�B�[���h�Ή��\�̃J����������i5��ځj
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool setFieldInfo(const CString& groupName, const CString& column1, const CString& column3, const CString& column4, const CString& column5);

	/**
	 * @brief �ۂߍ��ݒl�擾
	 * @param groupName [in] �O���[�v���i�G���[�o�͕����p�j
	 * @param column1 [in] �t�B�[���h�Ή��\�̃J����������i1��ځj�i�G���[�o�͕����p�j
	 * @param cToken [in] �ϊ��t�B�[���h�Ή��\�̃^�u�����g�[�N��
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool setRoundValue(const CString& groupName, const CString& column1, CStringTokenizer& cToken);

	/**
	 * @brief �����l�擾
	 * @param groupName [in] �O���[�v���i�G���[�o�͕����p�j
	 * @param column1 [in] �t�B�[���h�Ή��\�̃J����������i1��ځj�i�G���[�o�͕����p�j
	 * @param type [in] �G���[�l���L���l���i�G���[�o�͕����p�j
	 * @param cToken [in] �ϊ��t�B�[���h�Ή��\�̃^�u�����g�[�N��
	 * @param setCheckValue [out] �����l�Q
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool setConditionalValue(const CString& groupName, const CString& column1, const CString& type, CStringTokenizer& cToken, std::set<CString>& setCheckValue);

	/**
	 * @brief �����͈͎擾
	 * @param groupName [in] �O���[�v���i�G���[�o�͕����p�j
	 * @param column1 [in] �t�B�[���h�Ή��\�̃J����������i1��ځj�i�G���[�o�͕����p�j
	 * @param type [in] �G���[�l���L���l���i�G���[�o�͕����p�j
	 * @param cToken [in] �ϊ��t�B�[���h�Ή��\�̃^�u�����g�[�N��
	 * @param setConditinalRange [out] �����͈͌Q
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool setConditionalRange(const CString& groupName, const CString& column1, const CString& type, CStringTokenizer& cToken, std::set<CRange>& setConditinalRange);

	/**
	 * @brief Null�l�擾
	 * @param groupName [in] �O���[�v���i�G���[�o�͕����p�j
	 * @param column1 [in] �t�B�[���h�Ή��\�̃J����������i1��ځj�i�G���[�o�͕����p�j
	 * @param type [in] �G���[�l���L���l���i�G���[�o�͕����p�j
	 * @param cToken [in] �ϊ��t�B�[���h�Ή��\�̃^�u�����g�[�N��
	 * @param strNullValue [out] Null�l
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool setNullValue(const CString& groupName, const CString& column1, const CString& type, CStringTokenizer& cToken, CString& strNullValue);

	/**
	 * @brief �Œ�l�l�擾
	 * @param groupName [in] �O���[�v���i�G���[�o�͕����p�j
	 * @param column1 [in] �t�B�[���h�Ή��\�̃J����������i1��ځj�i�G���[�o�͕����p�j
	 * @param type [in] �G���[�l���L���l���i�G���[�o�͕����p�j
	 * @param cToken [in] �ϊ��t�B�[���h�Ή��\�̃^�u�����g�[�N��
	 * @param strFixedValue [out] �Œ�l
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool setFixedValue(const CString& groupName, const CString& column1, const CString& type, CStringTokenizer& cToken, CString& strFixedValue);

	/**
	 * @brief �����͈̓`�F�b�N
	 * @param groupName [in] �O���[�v���i�G���[�o�͕����p�j
	 * @param column1 [in] �t�B�[���h�Ή��\�̃J����������i1��ځj�i�G���[�o�͕����p�j
	 * @param type [in] �G���[�l���L���l���i�G���[�o�͕����p�j
	 * @param setConditionalRange [out] �����͈͒l�Q
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool checkConditionalRange(const CString& groupName, const CString& column1, const CString& type, const std::set<CRange>& setConditionalRange);

public:
	CString m_srcTableName;	//!< �ϊ����e�[�u����
	CString m_srcFieldName;	//!< �ϊ����t�B�[���h��
	long m_srcFieldIndex;	//!< �ϊ����t�B�[���h�̃t�B�[���h�C���f�b�N�X
	CString m_dstFieldName;	//!< �ϊ���t�B�[���h��
	FIELDTYPE::ECode m_FieldType;	//!< �t�B�[���h�^�C�v
	bool m_canNull;			//!< NULL����
	long m_Length;			//!< STRING�̂�
	long m_Number;			//!< INTEGER,DOUBLE�i���������̌����j
	long m_Decimal;			//!< DOUBLE�̂݁i���������̌����j

	bool m_bNullValue;		//!< NULL�l�ݒ肳��Ă��邩�ǂ���
	CString m_NullValue;	//!< NULL�l
	bool m_bFixedValue;		//!< �Œ�l�ݒ肳��Ă��邩�ǂ���
	CString m_FixedValue;	//!< �Œ�l

	std::map<CString,CString> m_mapRoundValue;	//!< �ۂߍ��ݗp
	std::set<CString> m_setErrValue;			//!< �G���[�l�p
	std::set<CRange> m_setErrRange;				//!< �G���[�͈͗p
	std::set<CString> m_setValidValue;			//!< �L���l�p
	std::set<CRange> m_setValidRange;			//!< �L���͈͗p
};
