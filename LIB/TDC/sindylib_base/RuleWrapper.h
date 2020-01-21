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

#include "ComWrapper.h"
#include <boost/shared_ptr.hpp>

namespace sindy {

//class CAutoValue;
class CRowBase;

typedef std::list<std::pair<CString,CString>> CAutoValue; //!< �����t�^����
class CRuleWrapper
{
public:
	CRuleWrapper(void){}
	virtual ~CRuleWrapper(void){}

	/**
	* @brief �����Ă���COM���N���A����
	*/
	void Clear()
	{
		m_ipRule = NULL;
		m_ipModify = NULL;
	}

	/**
	* @brief ���[���I�u�W�F�N�g�Ƀ��O�C����񂪃Z�b�g����Ă��邩�ǂ���
	* @retval true ���O�C���ς�
	* @retval false ���O�C�����Ă��Ȃ�
	*/
	bool IsLogined() const;

	/**
	* @brief �ҏW�\���ǂ������m�F����
	* @param cRow [in] row�i�쐬�ȊO��p�����݂���K�v������܂��j
	* @param emEditType [in] �ҏW�^�C�v
	* @param strMsg [out] �G���[���b�Z�[�W
	* @param check_modify [in,optional] ModifyCheck�����s���邩�ǂ����i�f�t�H���g�Ffalse�j
	* @param err2warn_modify [in,optional] ModifyCheck���x�������ɂ��邩�ǂ����i�f�t�H���g�Ffalse�j
	* @return �ҏW�s�^�C�v
	*/
	sindyeEditableType IsEditable( const CRowBase& cRow, sindyeEditType emEditType, CString& strMsg, bool check_modify = true, bool err2warn_modify = false  ) const;

	/**
	* @brief ���b�N�G���A���ǂ����m�F����
	* @param strUser [in] �e�[�u���I�[�i�[��
	* @param strTable [in] �e�[�u����
	* @param ipGeom [in] �m�F�Ώی`��
	* @retval true ���b�N�G���A
	* @retval false �񃍃b�N�G���A
	*/
	bool IsLockArea( const CString& strOwner, const CString& strTable, IGeometryPtr& ipGeom ) const;

	/**
	* @brief �ҏW�\�G���A���ǂ����m�F����
	* @param strTable [in] �e�[�u����
	* @param ipGeom [in] �m�F�Ώی`��
	* @retval true �ҏW�\�G���A
	* @retval false �ҏW�s�\�G���A
	*/
	bool IsEditableArea( const CString& strTable, IGeometryPtr& ipGeom ) const;

	/**
	* @brief ���ɕҏW���Ă��郆�[�U�����Ȃ����m�F����
	* @param cRow [in] row�ip�����݂���K�v������܂��j
	* @param strMsg [out] �G���[���b�Z�[�W
	* @retval true ���ɕҏW���Ă��郆�[�U������
	* @retval false �N���ҏW���Ă��Ȃ�
	*/
	bool IsModified( const CRowBase& cRow, CString& strMsg ) const;

	/**
	* @brief ��ƖړIID���擾����
	* @retval -1 ���O�C�����Ă��Ȃ�
	* @retval -1�ȊO ��ƖړIID
	*/
	long GetWorkPurposeID() const;

	/**
	* @brief ���O�C�������擾����
	* @retval ���O�C����
	*/
	CString GetOperatorName() const;
	
	/**
	* @brief �V�K�쐬���̎����t�^�������擾����
	* @note �쐬�\���C���ɐݒ肳�ꂽ�����쐬�����̒l���擾���܂�
	* @param strTable [in] �Ώۂ̃e�[�u����
	* @return �����t�^�����y�A���X�g�ifirst:�J������ second:�l�j
	*/
	const CAutoValue& GetCreatedAutoValue( const CString& strTable ) const;

	/**
	* @brief �����t�^�������擾����
	* @note �u��ƖړI�y��MXD�t�@�C���̕ҏW�v�ɐݒ肳�ꂽ���������t�^�̒l��
	* @return �����t�^�����y�A���X�g�ifirst:�J������ second:�l�j
	*/
	const CAutoValue& GetAutoValue() const;

	/**
	* @brief ���[���I�u�W�F�N�g���擾����
	*/
	ISiNDYRule4Ptr GetRule() const;

	/**
	* @brief ���f�B�t�@�C�`�F�b�N�I�u�W�F�N�g���擾����
	*/
	ISiNDYModifyCheckPtr GetModify() const;
protected:
	mutable ISiNDYRule4Ptr m_ipRule; //!< ���[���I�u�W�F�N�g
	mutable ISiNDYModifyCheckPtr m_ipModify; //!< ���f�B�t�@�C�`�F�b�N�I�u�W�F�N�g
	mutable std::map<CString,CAutoValue> m_mapAutoValue; //!< �����t�^�����i�쐬���̂݁j
	mutable CAutoValue m_cAutoValue; //!< �����t�^�����i�ҏW�S�ʗp�j
};

typedef boost::shared_ptr<CRuleWrapper> CSPRuleWrapper;

};
