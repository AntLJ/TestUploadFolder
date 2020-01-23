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

#include <map>
#include "ADLib.h"

namespace adlib
{
	namespace row_array
	{
		// �I�u�W�F�N�gID�ɂ�錟���p�R���e�i�̌^�̒�`
		typedef std::map<OID, _IRowPtr> OIDTable;

		/**
		 * @brief �I�u�W�F�N�gID�Ō����\��Row�̏W��
		 * @note	�o�T�FSiNDYQToMPQ
		 */
		class RowArray
		{
		public:
			RowArray(void) {}
			virtual ~RowArray(void) {}

			/**
			 * @brief �^����ꂽ�t�B�[�`���J�[�\������I�u�W�F�N�g�Q��ǂݍ���
			 * @param[in]	crRowCursor	�Ώۃt�B�[�`���J�[�\��
			 */
			void load( _ICursor& crCursor );

			/**
			 * @brief �I�u�W�F�N�gID�ɂ�錟��
			 * @param[in]	cOID	�������郊���N�̃I�u�W�F�N�gID
			 * @return	�����N�̃t�B�[�`���i������Ȃ��ꍇ��NULL��Ԃ��j
			 */
			_IRowPtr searchByOID( long cOID ) const;

			/**
			 * @brief �S�R���e�i�̑S�v�f���폜
			 * @note	�h���N���X�ŌʂɃR���e�i�����Ȃ�I�[�o�[���C�h����K�v����
			 */
			virtual void clear(void);

			/**
			 * @brief OIDTable�ւ̎Q�Ƃ�ǂݎ���p�œn��
			 */
			const OIDTable& getOIDTable(void) const { return m_OIDTable; }

			/**
			 * @brief �ǂݍ��݂���������Ă��邩�ۂ��𔻒肷��
			 * @return	���茋��
			 */
			bool isLoaded() const { return m_OIDTable.size() != 0; }

		private:
			OIDTable m_OIDTable;					//!< �I�u�W�F�N�gID�ɂ�錟���p�R���e�i
		};
	} // namespace row_array
} // namespace adlib
