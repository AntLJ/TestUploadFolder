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

#include <atlstr.h>
#include <FieldMap.h>
#include <list>
#include "CommonData.h"

/**
 * @brief ����e�[�u���Ƃ���e�[�u�����֘A�t���邽�߂����ɑ��݂���e�[�u���𑀍삷�邽�߂̃N���X
 * @warning  �f�[�^�̒ǉ��E�폜�̂ǂ���ɂ����Ă��A�g�p����t�B�[���h�������ꍇ�ɂ��������Ȃ�
 */

class CRelationTableManager
{
public:
	/**
	* @brief �폜����f�[�^�̃��X�g�ƂȂ�f�[�^�^
	* @note  �Ⴆ�΁A����t�B�[���h�̒l��0,1,2�̂��̂��폜�������Ƃ����ꍇ�A
	* @note  ���̃��X�g�̗v�f�͓����珇��[0][1][2]�ƂȂ�
	*/
	typedef std::list<CString> DELETELIST;

	/**
	* @brief �폜����f�[�^�̑g�ݍ��킹�ƂȂ郊�X�g���i�[����f�[�^�^
	* @note  �Ⴆ�΁A�t�B�[���h1�̒l��"Hoge",�t�B�[���h2�̒l��0�̂��̂�
	* @note  �Ⴆ�΁A�t�B�[���h1�̒l��"Pugya",�t�B�[���h2�̒l��1�̂��̂�
	* @note  �폜�������ꍇ�A���̃��X�g�̗v�f�͓����珇��[["Hoge"][0]][["Pugya"][1]]�ƂȂ�
	*/
	typedef std::list<std::list<CString>> DELETELISTS;

	/**
	 * @brief ����������
	 * @warning  ���̃C���X�^���X���g�p����O�ɁA�K����x�����Ăяo������
	 * @param[in] strTable  �֘A�e�[�u����
	 * @param[in] strKey    ��L�[�ƂȂ�t�B�[���h��
	 * @param[in] subFields �l�̎擾�E�}���E�폜�̍ۂɎg�p����t�B�[���h���̃��X�g
	 * @param[in] ipRow     ���̈����Ɠ����I�[�i�[�̃e�[�u�����J��
	 */
	void Initialize( const CString& strTable, const CString& strKey, const FIELDNAMES& subFields, _IRowPtr& ipRow );

	/*
	* @brief �֘A�e�[�u���Ƀf�[�^��ǉ�
	* @param[in] lOID    �I�𒆂̃t�B�[�`���[��OID
	* @param[in] lList   �ǉ�����f�[�^�̃��X�g(1�v�f=1�s 1�v�f�̒��Ɋe�J�����̃f�[�^�������Ă���)
	* @retval true  �f�[�^�̒ǉ��ɐ���
	* @retval false �f�[�^�̒ǉ��Ɏ��s
	*/
	bool AddData( long lOID, const AOI_LINE& lList ) const;

	/*
	* @brief �֘A�e�[�u���Ƀf�[�^��ǉ�
	* @param[in] lOID    �I�𒆂̃t�B�[�`���[��OID
	* @param[in] lList   �ǉ�����f�[�^�̃��X�g(1�v�f=1�s)
	* @retval true  �f�[�^�̒ǉ��ɐ���
	* @retval false �f�[�^�̒ǉ��Ɏ��s
	*/
	bool AddData( long lOID, const AOI_MATRIX& lList ) const;

	/*
	* @brief �֘A�e�[�u������f�[�^���폜
	* @param[in] lOID           �I�𒆂̃t�B�[�`���[��OID
	* @param[in] lList          �폜����f�[�^�̃��X�g(1�v�f=1�s)
	* @retval true  �f�[�^�̍폜�ɐ���
	* @retval false �f�[�^�̍폜�Ɏ��s
	*/
	bool DeleteData( long lOID, const DELETELIST& lList ) const;

	/*
	* @brief �֘A�e�[�u������f�[�^���폜
	* @param[in] lOID    �I�𒆂̃t�B�[�`���[��OID
	* @param[in] lList   �폜����f�[�^�̃��X�g(1�v�f=1�s 1�v�f�̒��ɃJ����)
	* @retval true  �f�[�^�̍폜�ɐ���
	* @retval false �f�[�^�̍폜�Ɏ��s
	*/
	bool DeleteData( long lOID, const DELETELISTS& lList ) const;

	/*
	* @brief �֘A�e�[�u������L�[�t�B�[���h��OID����v����s�̈�v�f���擾
	* @param[in] �I�𒆂̃t�B�[�`���[��OID
	* @param[in] �擾����v�f��
	* @return retList �擾�����f�[�^�̃��X�g
	*/
	AOI_COLUMN GetRelationData( long lOID, const CString& strField ) const;

	/*
	* @brief �֘A�e�[�u������L�[�t�B�[���h��OID����v����s�̓���̗v�f���擾
	* @param[in] �I�𒆂̃t�B�[�`���[��OID
	* @param[in] �擾����v�f�����i�[�������X�g
	* @return retList �擾�����f�[�^�̃��X�g(1�v�f=1�s 1�v�f���̊e�v�f���e�J�����̒l)
	*/
	AOI_MATRIX GetRelationData( long lOID, const FIELDNAMES& strFields ) const;

private:
	/*
	* @brief IRowBufferPtr�ɒl������
	* @param[in] ipRowBuffer �l������o�b�t�@
	* @param[in] columns     �����l
	* @retval true  ����I��
	* @retval false �����ꂩ�̒l�����邱�ƂɎ��s
	*/
	bool putValues( IRowBufferPtr& ipRowBuffer, const AOI_LINE& columns ) const;

	/*
	* @brief DB��̃f�[�^���폜����ۂ̏������𐶐�
	* @param[in] lOID  "m_KeyFieldName=%ld"�Ɏg�p
	* @param[in] lList �폜����f�[�^�̊e�J�����̒l���i�[�������X�g(1�v�f=1�s 1�v�f���̊e�v�f���J�����̒l)
	* @return  strWhereClause �폜����s�̏�����
	*/
	CString makeDeleteWhereClause(long lOID, const DELETELISTS& lList) const;

	ITablePtr         m_ipTable;      //!< �֘A�e�[�u��
	sindy::CFieldMap  m_RelTableFM;   //!< �֘A�e�[�u���̃t�B�[���h�}�b�v
	CString           m_KeyFieldName; //!< �֘A�e�[�u��������������ۂ̃L�[�Ƃ���t�B�[���h
	FIELDNAMES        m_subFields;    //!< �f�[�^���擾�E�폜����ۂ̔���Ɏg�p����t�B�[���h
};
