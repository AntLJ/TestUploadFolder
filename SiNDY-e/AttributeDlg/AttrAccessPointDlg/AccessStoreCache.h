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

#include <sindy/schema.h>
#include <boost/operators.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

typedef std::basic_string<TCHAR> tstring;

namespace access_store_cache
{
	/**
	 * @brief �t�B�[���h
	 * @note	�uAccess_Store�e�[�u���̂P�t�B�[���h�v�y�сuAccess_Store���O���b�h�̂P�Z���v�ɓ�����f�[�^�B
	 *			�O���b�h�Ɋi�[�����l�͑S�Đ����l�܂���bool�l�Ȃ̂ŁAlong�^�Ŏ����B
	 */
	struct CASField
		: private boost::operators<CASField>
	{
		CASField() : m_OrgValue( 0 ), m_NewValue( 0 ) {}
		CASField( long cOrgValue ) : m_OrgValue( cOrgValue ), m_NewValue( cOrgValue ) {}

		long m_OrgValue;		///< ���i�ǂݍ��񂾎��j�̒l
		long m_NewValue;		///< �X�V��̒l�i�����l��m_OrgValue�Ɠ����j

		/**
		 * @brief �召��r���Z�q
		 * @note	�X�V��̒l�������r��Ƃ���B
		 * @param[in]	crField	��r�Ώۃt�B�[���h
		 * @return	��r����
		 */
		bool operator< ( const CASField& crField ) const
		{
			return m_NewValue < crField.m_NewValue;
		}

		/**
		 * @brief ������r���Z�q
		 * @note	�X�V��̒l�������r��Ƃ���B
		 * @param[in]	crField	��r�Ώۃt�B�[���h
		 * @return	��r����
		 */
		bool operator== ( const CASField& crField ) const
		{
			return m_NewValue == crField.m_NewValue;
		}

		/**
		 * @brief ���̒l�ƍX�V��̒l���قȂ邩�ۂ��𔻒肷��
		 * @return	���茋��
		 */
		bool isUpdated() const
		{
			return m_OrgValue != m_NewValue;
		}
	};
	typedef tstring FieldName;
	typedef std::map<FieldName, CASField> CASFields;

	/**
	 * @brief �s
	 * @note	�uAccess_Store�e�[�u���̂P�s�v�y�сuAccess_Store���O���b�h�̂P�s�v�ɓ�����f�[�^�B
	 */
	struct CASRow
	{
		CASRow() : m_IsAdded(false), m_IsDeleted(false) {}

		CASFields m_Fields;		///< �t�B�[���h�Q
		bool m_IsAdded;										///< �ǉ����ꂽ�s�ł��邩�ۂ�
		bool m_IsDeleted;									///< �폜���ꂽ�s�ł��邩�ۂ�
		_IRowPtr m_pRow;									///< ���̎擾��_IRowPtr�i�ǉ����ꂽ�s�Ȃ�NULL�j

		/**
		 * @brief �w�肳�ꂽ�t�B�[���h�������t�B�[���h�ւ̎Q�Ƃ�Ԃ�
		 * @note	���݂��Ȃ��t�B�[���h�����w�肳�ꂽ�ꍇ��std::runtime_error��O�𑗏o����B
		 *			��{�I�ɂ́A�g�����ɊԈႢ���Ȃ���΋N����Ȃ��͂��B
		 *			�f�[�^���f���̃t�B�[���h�����ς������A�t�B�[���h���폜���ꂽ��A�L���b�V���ɐ���Ɋi�[�ł��Ă��Ȃ��ꍇ�ɋN����B
		 * @param[in]	cFieldName	�t�B�[���h��
		 * @return	�t�B�[���h
		 */
		const CASField& validField( tstring cFieldName ) const
		{
			CASFields::const_iterator itField = m_Fields.find( cFieldName );
			if (itField == m_Fields.end())
			{
				throw std::runtime_error("CASRow::validValue()���s");
			}
			return itField->second;
		}
	};
	typedef long ObjectID;
	typedef std::map<ObjectID, CASRow> CASRows;

	typedef long MasterCode;	///< �`�F�[���X�}�X�^�[�R�[�h
	typedef long SundayFlag;	///< ���j���t���O
	typedef boost::tuple<MasterCode, SundayFlag, ObjectID> CSortedASRowsKey;	///< �O���b�h���ł̃\�[�g��ƂȂ�l�̏W��
	typedef std::map<CSortedASRowsKey, CASRow> CSortedASRows;					///< �O���b�h�p�Ƀ\�[�g���ꂽCASRow�̏W��

	/**
	 * @brief Access_Store�e�[�u���̃L���b�V��
	 * @note	�O���b�h�ɑ΂���ύX��������DB�ɔ��f�����Ȃ��Ă悢�悤�ɂ��邽�߂̃f�[�^�B
	 *			�O���b�h��Œl���ύX���ꂽ��܂��L���b�V����ύX���A�u�ۑ��v�̎w������������܂Ƃ߂�DB�ɔ��f����B
	 */
	class CASCache
	{
	public:
		CASCache(void);
		~CASCache(void);

		/**
		 * @brief �S�f�[�^�폜
		 */
		void clear()
		{
			m_Rows.clear();
			m_SortedRows.clear();
		}

		/**
		 * @brief �s�f�[�^��ǉ�����
		 * @note	�u�ǉ��v�{�^���������ꂽ���p�B�A�N�Z�X�|�C���gID�ȊO�̑S�t�B�[���h�������l�̍s��ǉ�����B
		 * @param[in]	cAccessPointID	�s�f�[�^���֘A�t���A�N�Z�X�|�C���g��ID
		 */
		void addRow( long cAccessPointID );

		/**
		 * @brief IRowPtr����擾���������i�[�����s�f�[�^��ǉ�����
		 * @param[in]	�ǉ�����s�f�[�^�̌��ɂȂ�Row�I�u�W�F�N�g
		 */
		void addRow( _IRow* cpRow );

		/**
		 * @brief �w�肳�ꂽ�I�u�W�F�N�gID�����s�̎w�肳�ꂽ�t�B�[���h���X�V����
		 * @note	�Ώۍs�܂��̓t�B�[���h�����݂��Ȃ���Ή������Ȃ��B
		 * @param[in]	cObjectID	�ΏۃI�u�W�F�N�gID
		 * @param[in]	cFieldName	�Ώۃt�B�[���h��
		 * @param[in]	cValue		�t�B�[���h�Ɋi�[����l
		 */
		void updateField( long cObjectID, tstring cFieldName, long cValue );

		/**
		 * @brief �w�肳�ꂽ�I�u�W�F�N�gID�����s�̍폜�t���O�𔽓]������
		 * @note	�w�肳�ꂽ�I�u�W�F�N�gID�����s�����݂��Ȃ���Ή������Ȃ��B
		 * @param[in]	cObjectID	�ΏۃI�u�W�F�N�gID
		 */
		void changeDeleteFlag( long cObjectID );

		/**
		 * @brief 
		 */
		const CASRows& rows() const { return m_Rows; }

		/**
		 * @brief �\�[�g�ςݍs�Q�����߂ĕԂ�
		 * @note	m_Rows�̍s�Q���O���b�h�p�ɕ��ёւ��ĕԂ��B�\�[�g�ςݍs�Q�̗L���Ɋւ�炸�\�[�g���s���B
		 * @return	�\�[�g�ςݍs�Q
		 * @retval	NULL	���s�iCASField::validField()���s���j
		 */
		const CSortedASRows* sortRows();

		/**
		 * @brief �\�[�g�ςݍs�Q��Ԃ�
		 * @note	�i�[����Ă���\�[�g�ςݍs�Q�ւ̎Q�Ƃ�Ԃ��B�\�[�g�ςݍs�Q���쐬�ς݂��ǂ����̓`�F�b�N���Ȃ��B
		 * @return	�\�[�g�ςݍs�Q
		 */
		const CSortedASRows& sortedRows();

	private:
		/**
		 * @brief �w�肳�ꂽ�I�u�W�F�N�gID�����s��Ԃ�
		 * @param[in]	cObjectID	�ΏۃI�u�W�F�N�gID
		 * @return	�s�i������Ȃ����NULL�j
		 */
		CASRow* getRow( long cObjectID );

		/**
		 * @brief �w�肳�ꂽ�t�B�[���h�������t�B�[���h��Ԃ�
		 * @param[in]	cObjectID	�ΏۃI�u�W�F�N�gID
		 * @param[in]	cFieldName	�Ώۃt�B�[���h��
		 * @return	�t�B�[���h�i������Ȃ����NULL�j
		 */
		CASField* getField( long cObjectID, tstring cFieldName );

		/**
		 * @brief ���I�u�W�F�N�gID�𔭍s���ĕԂ�
		 * @note	���I�u�W�F�N�gID�Ƃ́A�u�ǉ��v�{�^���ō쐬����ADB�ɖ����f�̍s�ɗ^������I�u�W�F�N�gID�B
		 *			DB����擾�����s�Ɣ��Ȃ��悤�ɂ��邽�߁A���̒l�Ƃ���B
		 *			���I�u�W�F�N�gID�����s�����ɂ���΂��̓��ŏ��̂���-1��Ԃ��A�Ȃ����-1��Ԃ��B
		 */
		long getTempObjectID() const;

		CASRows m_Rows;		///< �s�Q
		CSortedASRows m_SortedRows;	///< �O���b�h�p�Ƀ\�[�g���ꂽ�s�Q
	};

} // namespace access_store_cache
