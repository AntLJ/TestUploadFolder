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
#include "RoadLinkTable.h"
#include <boost/shared_ptr.hpp>

namespace curve_info
{
	using namespace adlib;
	using namespace sindy;

	typedef boost::shared_ptr< CTableFinder > CSPTableFinder;

	/**
	 * @brief DB����ǂݍ��񂾃f�[�^�̃L���b�V�����Ǘ�����N���X
	 */
	class CacheManager
	{
	private:
		typedef std::map< adlib::MeshCode, CSPTableFinder > Cache;
		Cache m_Cache;			///< ���b�V���P�ʂ̃L���b�V��

		IFeatureClassPtr		m_pRoadLinkClass;		///< ���H�����N�t�B�[�`���N���X
		IFeatureClassPtr		m_pRoadNodeClass;		///< ���H�m�[�h�t�B�[�`���N���X
		IFeatureClassPtr		m_pBaseMeshClass;		///< �Q�����b�V���t�B�[�`���N���X

		CModel m_Model;			///< sindylib_base�̃��f����`

	public:
		/// �R���X�g���N�^
		CacheManager(
			IFeatureClassPtr cpRoadLinkClass,
			IFeatureClassPtr cpRoadNodeClass,
			IFeatureClassPtr cpBaseMeshClass
		);

		/// �f�X�g���N�^
		~CacheManager() {}

		/// sindylib_base�Ŏg�p���郂�f�����`����
		void defineSindynModel();

		/// �Q�����b�V���t�B�[�`���N���X��Ԃ�
		const IFeatureClassPtr meshClass() const { return m_pBaseMeshClass; }

		/// �L���b�V�������݂��郁�b�V���̃��b�V���R�[�h�Q��Ԃ�
		MeshCodes cachedMeshCodes() const;

		/**
		 * @brief �w�肳�ꂽ���b�V���̃L���b�V�������ɑ��݂��邩�ۂ��𔻒肷��
		 * @param[in]	cMeshCode	�Ώۃ��b�V��
		 * @return	���茋��
		 */
		bool isCached( long cMeshCode ) const;

		/**
		 * @brief �w�肳�ꂽ���b�V���̃L���b�V����Ԃ�
		 * @note	�L���b�V�������݂��Ȃ���΍쐬����Bstd::map::operator[]�Ɠ��l�̍l�����B
		 * @note	TODO: �ł���΁AcQueryWhereClause�̎w��ɂ����ɏd�����������s���ꂽ�ꍇ�A
		 *			�������f���ăT�[�o�ɒv���I�ȕ��ׂ�^���Ȃ��悤�ɂ���d�g�݂������������ǂ��B�^�C���A�E�g��݂���Ƃ��H
		 * @@param[in]	cMeshCode	�Ώۃ��b�V��
		 * @@param[in]	cQueryWhereClause	�Ώۃ����N�w��N�G�� (bug9617)
		 * @return	�L���b�V�����ꂽ�f�[�^���擾���邽�߂̃e�[�u���t�@�C���_
		 */
		const CSPTableFinder cache( long cMeshCode, CString cQueryWhereClause );

		/**
		 * @brief �w�肳�ꂽ���b�V���̃L���b�V�����������
		 * @param[in]	cMeshCode	�Ώۃ��b�V��
		 */
		void clear( long cMeshCode );

		/**
		 * @brief �w�肳�ꂽ���b�V���Q�ȊO�̃L���b�V����S�ĉ������
		 * @param[in]	cMeshCode	�Ώۃ��b�V��
		 */
		void clearExcept( MeshCodes cMeshCodes );

		/**
		 * @brief �ǂݍ��ݍς݃��b�V���̒�����A�w�肳�ꂽ�����N�����݂��郁�b�V����T���A���̃��b�V���R�[�h��Ԃ�
		 * @param[in]	cLinkID	�Ώۃ����NID
		 * @return	���b�V���R�[�h�i������Ȃ������ꍇ��0�j
		 */
		long existingMeshCode( long cLinkID ) const;
	};
	typedef std::tr1::shared_ptr< CacheManager > CacheManagerPtr;

} // namespace curve_info

