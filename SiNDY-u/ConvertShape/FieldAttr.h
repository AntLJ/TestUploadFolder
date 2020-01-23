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

#include "global.h"

/**
 * @brief �ݒ�t�@�C����͗p�N���X
 */
class CFieldAttr {
public:
	CFieldAttr(){}
	~CFieldAttr(){}
public:
	bool Init(const CString& strFileAttrPath, const std::map<CString,esriFieldType>& mapOrgFieldType);

	std::map<CString,esriFieldType> GetOrgFieldType(){ return m_mapOrgFieldType; }
	std::list<PAIR_COL> GetListFieldName(){ return m_listFieldName; }
	std::map<CString,CString> GetOrgDstFieldName(){ return m_mapOrgDstFieldName; }
	std::map<CString,CString> GetDstOrgFieldName(){ return m_mapDstOrgFieldName; }
	std::map<ORG_COLNAME,std::set<CString>> GetColValue(){ return m_mapFieldValue; }
	std::map<ORG_COLNAME,TRANS_VALUE> GetTransValue(){ return m_mapTransValue; }
	std::map<DEST_COLNAME,CComVariant> GetDefautlValue(){ return m_mapDefaultValue; }
	std::map<DEST_COLNAME,std::map<CString,CString>> GetFieldInfo(){ return m_mapFieldInfo; }
	std::map<DEST_COLNAME,std::map<CString,CString>> GetAttrValue(){ return m_mapAttrValue; }

private:
	/**
	 * @brief �ݒ�t�@�C�����
	 * @param strFileAttrPath [in] �ݒ�t�@�C���p�X
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool AnalyzeFieldAttrFile(const CString& strFileAttrPath);

	/**
	 * @brief �J�����̈Ӗ��擾
	 * @param strFieldColumn [in] �J����������
	 * return field_attr_mark::ECode
	 */
	field_attr_mark::ECode GetFieldMarkType(const CString& strFieldColumn);

	/**
	 * @brief �ݒ���J�������
	 * @note ���̊֐����o�R���Ċۂߍ��݁A�����w��A�t�B�[���h���A�����l�A�����w��{�i�[�l�w��֐���
	 * @param strOrgFieldName [in] �ϊ����t�B�[���h��
	 * @param eOrgFieldMark [in] �ϊ����t�B�[���h�I�v�V�������
	 * @param strDstFieldName [in] �ϊ���t�B�[���h��
	 * @param strColumnInfo [in] �w��J�������
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool SetOptionInfo(const CString& strOrgFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strDstFieldName, const CString& strColumnInfo);

	/**
	 * @brief �ۂߍ��ݏ����
	 * @param strOrgFieldName [in] �ϊ����t�B�[���h��
	 * @param eOrgFieldMark [in] �ϊ����t�B�[���h�I�v�V�������
	 * @param strColumnInfo [in] �w��J�������
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool SetOptionRound(const CString& strOrgFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo);

	/**
	 * @brief �����w������
	 * @param strOrgFieldName [in] �ϊ����t�B�[���h��
	 * @param eOrgFieldMark [in] �ϊ����t�B�[���h�I�v�V�������
	 * @param strColumnInfo [in] �w��J�������
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool SetOptionDesignation(const CString& strOrgFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo);

	/**
	 * @brief �t�B�[���h�����
	 * @param strOrgFieldName [in] �ϊ����t�B�[���h��
	 * @param strDstFieldName [in] �ϊ���t�B�[���h��
	 * @param eOrgFieldMark [in] �ϊ����t�B�[���h�I�v�V�������
	 * @param strColumnInfo [in] �w��J�������
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool SetOptionFieldInfo(const CString& strOrgFieldName, const CString& strDstFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo);

	/**
	 * @brief �����l�����
	 * @param strDstFieldName [in] �ϊ���t�B�[���h��
	 * @param eOrgFieldMark [in] �ϊ����t�B�[���h�I�v�V�������
	 * @param strColumnInfo [in] �w��J�������
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool SetOptionDefaultValue(const CString& strDstFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo);

	/**
	 * @brief �����w��{�i�[�l�w������
	 * @param strDstFieldName [in] �ϊ���t�B�[���h��
	 * @param eOrgFieldMark [in] �ϊ����t�B�[���h�I�v�V�������
	 * @param strColumnInfo [in] �w��J�������
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool SetOptionAttr(const CString& strDstFieldName, field_attr_mark::ECode eOrgFieldMark, const CString& strColumnInfo);

	/**
	 * @brief �ۂߍ��ݎ��̕�������
	 * @param strOrgFieldName [in] �ϊ����t�B�[���h��
	 * @param strColumnInfo [in] �w��J�������
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool GetStrRoundValue(const CString& strOrgFieldName, const CString& strColumnInfo);

	/**
	 * @brief �V���O���N�H�[�g��������
	 * @param strOrgFieldName  [in] �ϊ����t�B�[���h��
	 * @param strColumnInfoAll [in] �w��J�������
	 * @param strColumnInfo    [in] �w��J�������
	 * @param str1stValue      [in] �I�v�V���������w��A�Ȃ���΋󕶎��w��i�u/�v�u=�v�u:�v�Ȃǁj
	 * @param strFindValue     [in] ����������i�ۂߍ��ݎw��̏ꍇ�u:'test'('hoge' 'boge' 'muge')�v�ϊ����'test'���擾���邽�߂ɂ́u(�v���w��A�ϊ�����'hoge'��'boge'��'muge'���擾����ɂ́u �v�i���p�X�y�[�X�j���w�肷��j
	 * @param bFind            [in] ���������񂪌�����Ȃ��ꍇ�G���[�Ƃ��邩�ǂ����itrue=������Ȃ��ꍇ�̓X���[, false=������Ȃ�������G���[�j
	 * @param bEscape          [in] �擾�����l�̃V���O���N�H�[�g�u'�v�̈����itrue=�G�X�P�[�v����iDB�Ɋi�[�ADB�Ɣ�r�p�j, false=�G�X�P�[�v���Ȃ��iSQL��Where��p�j�j
	 * @param strSplitedValue  [out] strColumnInfo��strFindValue�ŕ��������l�istrColumnInfo�u'abc'/'fgb/bac'�v��strFindValue�u/�v�ŕ������AstrSplitedValue�u'abc'�v���擾����j
	 * @param strLeftValue     [out] ���������c��̒l�istrSplitedValue�ȊO�̒l�u'fgb/bac'�v���擾�j
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool GetSplitedValue(const CString& strOrgFieldName, const CString& strColumnInfoAll, const CString& strColumnInfo, const CString& str1stValue, const CString& strFindValue, bool bFind, bool bEscape, CString& strSplitedValue, CString& strLeftValue);

	/**
	 * @brief �ۂߍ��ݎ��̐��l��������
	 * @param strOrgFieldName [in] �ϊ����t�B�[���h��
	 * @param strColumnInfo [in] �w��J����������
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool GetNumRoundValue(const CString& strOrgFieldName, const CString& strColumnInfo);

	/**
	 * @brief ������擾
	 * @param lStart [in] �J�n�ʒu
	 * @param lEnd [in] �I���ʒu
	 * @param vecRound [in] �u'�v������̕�����R���e�i
	 * @param bEscSQuart [in] �������́u'�v�̐ݒ���@�itrue=�G�X�P�[�v����iDB�Ɋi�[�ADB�Ɣ�r�p�j, false=�G�X�P�[�v���Ȃ��iSQL��Where��p�j�j
	 * @return CString ������̕�����
	 */
	CString GetStrValue(long lStart, long lEnd, const std::vector<CString>& vecRound, bool bEscSQuart);

	/**
	 * @brief �I�v�V���������폜�i�擪�����폜�j
	 * @param strColumnInfo [in] �w��J����������
	 * @return CString �I�v�V���������폜��̕�����
	 */
	CString GetColumnValue(const CString& strColumnInfo);

	/**
	 * @brief ��������
	 * @param strFieldName [in] �t�B�[���h��
	 * @param strColumnInfo [in] �w��J�������
	 * @param strRetValue [out] �ϊ��㕶����
	 * @param bEscSQuart [in] �������́u'�v�̐ݒ���@�itrue=�G�X�P�[�v����iDB�Ɋi�[�ADB�Ɣ�r�p�j, false=�G�X�P�[�v���Ȃ��iSQL��Where��p�j�j
	 * @param bOptionDel [in] �w��J�������̃I�v�V�����������폜���邩�ǂ����i�擪�����̍폜�Ɠ��`�j
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool GetSettingValue(const CString& strFieldName, const CString& strColumnInfo, CString& strRetValue, bool bEscSQuart, bool bOptionDel);

	/**
	 * @brief �t�B�[���h�^�C�v�iesriFieldType�j�擾
	 * @param strOrgFieldName [in] �ϊ����t�B�[���h��
	 * @param strDstFieldName [in] �ϊ���t�B�[���h��
	 * @param mapFieldInfo [in] �t�B�[���h���
	 * @param eFieldType [out] �t�B�[���h�^�C�v
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool GetFieldType(const CString& strOrgFieldName, const CString& strDstFieldName, const std::map<CString,CString>& mapFieldInfo, esriFieldType& eFieldType);

	/**
	 * @brief �V�K�쐬�t�B�[���h���ǂ����Őݒ荀�ڂɉߕs�����Ȃ����`�F�b�N
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool CheckInfoByNewField();

	/**
	 * @brief �t�B�[���h�ݒ���̊i�[�l�`�F�b�N
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool CheckFieldInfo();

	/**
	 * @brief �t�B�[���h�ݒ���ŐV�K�쐬�t�B�[���h���ǂ����Őݒ荀�ڂɉߕs�����Ȃ����`�F�b�N
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool CheckFieldTypeByNewField();

	/**
	 * @brief �t�B�[���h�ݒ���Ńt�B�[���h�^�C�v�ɂ��ݒ荀�ڂɉߕs�����Ȃ����`�F�b�N
	 * @note  Integer:�X�P�[���~�A���x���A�����~
	 * @note  String :�X�P�[���~�A���x�~�A������
	 * @note  Double :�X�P�[�����A���x���A�����~
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool CheckFieldInfoByFieldType();

	/**
	 * @brief �t�B�[���h�ݒ���̐ݒ�l���t�B�[���h�^�C�v�ɍ����Ă��邩�`�F�b�N
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool CheckFieldInfoValue();

	/**
	 * @brief �t�B�[���h�ݒ���̐ݒ�l���͈͓��Ɏ��܂��Ă��邩
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool CheckFieldInfoValue2();

	/**
	 * @brief �����l�̐ݒ�l���t�B�[���h�^�C�v�ɍ����Ă��邩�`�F�b�N
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool CheckDefaultValue();

	/**
	 * @brief �����w��̐ݒ�l���t�B�[���h�^�C�v�ɍ����Ă��邩�`�F�b�N
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool CheckFieldValue();

	/**
	 * @brief �ۂߍ��ݎw��̐ݒ�l���t�B�[���h�^�C�v�ɍ����Ă��邩�`�F�b�N
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool CheckTransValue();

	/**
	 * @brief �����؂�o���̐ݒ�l���t�B�[���h�^�C�v�ɍ����Ă��邩�`�F�b�N
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool CheckAttrValue();

	/**
	 * @brief �ݒ�l���t�B�[���h�^�C�v�ɍ����Ă��邩�`�F�b�N
	 * @param strFieldName [in] �w��t�B�[���h�̃t�B�[���h��
	 * @param strOption [in] ������
	 * @param strValue [in] �ݒ�l
	 * @param eFieldType [in] �w��t�B�[���h�̃t�B�[���h�^�C�v
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool CheckValueOfFieldType(const CString& strFieldName, const CString& strOption, const CString& strValue, esriFieldType eFieldType);

private:
	std::map<CString,esriFieldType> m_mapOrgFieldType;        //!< �ϊ����t�B�[���h�^�C�v�\
	std::list<PAIR_COL> m_listFieldName;                      //!< �ϊ����E��t�B�[���h�����X�g
	std::map<CString,CString> m_mapOrgDstFieldName;           //!< �ϊ����E��t�B�[���h���Ή��\
	std::map<CString,CString> m_mapDstOrgFieldName;           //!< �ϊ���E���t�B�[���h���Ή��\
	std::map<ORG_COLNAME,std::set<CString>> m_mapFieldValue;            //!< �����w��pmap
	std::map<ORG_COLNAME,TRANS_VALUE> m_mapTransValue;                  //!< �ۂߍ��ݗpmap
	std::map<DEST_COLNAME,CComVariant> m_mapDefaultValue;               //!< �V�K�쐬�t�B�[���h�̃f�t�H���g�l
	std::map<DEST_COLNAME,std::map<CString,CString>> m_mapFieldInfo;    //!< �t�B�[���h�݌v���
	std::map<DEST_COLNAME,std::map<CString,CString>> m_mapAttrValue;    //!< �����擾�p
};
