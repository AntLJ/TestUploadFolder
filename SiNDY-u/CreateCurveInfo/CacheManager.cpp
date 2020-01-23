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

#include "StdAfx.h"
#include "CacheManager.h"
#include "RowContainer.h"
#include "CurveInfoUtility.h"

namespace curve_info
{
	CacheManager::CacheManager(
		IFeatureClassPtr cpRoadLinkClass,
		IFeatureClassPtr cpRoadNodeClass,
		IFeatureClassPtr cpBaseMeshClass
	) :
		m_pRoadLinkClass( cpRoadLinkClass ),
		m_pRoadNodeClass( cpRoadNodeClass ),
		m_pBaseMeshClass( cpBaseMeshClass )
	{
		// sindylib_base���g�p���邽�߂̏���
		init_sindynlib();
		defineSindynModel();
	}

	void CacheManager::defineSindynModel()
	{
		using namespace sindy::schema;

		// �����N���g�b�v�Ƃ������f��
		m_Model.Add( road_link::kTableName, kObjectID, NULL, NULL );
		// �����N��̂����A�K�v�Ȃ͉̂E���܋֎~����
		m_Model.Add( lq_turnreg::kTableName,	link_queue::kLinkID,	road_link::kTableName,	kObjectID			);
		m_Model.Add( inf_turnreg::kTableName,	kObjectID,				lq_turnreg::kTableName,	link_queue::kInfID	);

		// ���L�̗��R�ɂ��A�m�[�h�͏�L���f���Ɋ܂߂Ȃ��B
		//		�����N�ɂ̓m�[�h�Ƃ̊֘A�t��������L�[��From_Node_ID��To_Node_ID�̂Q��ނ��邪�A
		//		�����sindylib_base�̃��f���̎d�l�̓L�[���������鎖�ɑΉ����Ă��Ȃ��B
		//		�܂��A����̗p�r�ł͊֘A�t���Ă��Ȃ��Ă����ɍ���Ȃ��B
		//		����āA�m�[�h�̓����N�Ƃ͖��֌W�̃e�[�u���Ƃ��Ĉ������̂Ƃ��A���f����ł͒�`���Ȃ��B
	}

	MeshCodes CacheManager::cachedMeshCodes() const
	{
		MeshCodes aMeshCodes;
		BOOST_FOREACH (const Cache::value_type& rElement, m_Cache)
		{
			aMeshCodes.insert( rElement.first );
		}
		return aMeshCodes;
	}

	bool CacheManager::isCached( long cMeshCode ) const
	{
		return m_Cache.find( cMeshCode ) != m_Cache.end();
	}

	/**
	 * @brief sindylib_base��CSPContainerBase�̃\�[�g����`����v���f�B�P�[�g
	 * @note	sindylib_base���瓾���郊���N�Q�A�m�[�h�Q��ID���ɕ��ׂ邽�߂ɕK�v�B
	 */
	struct lessCSPContainerBase
	{
		bool operator() ( const CSPContainerBase& lhs, const CSPContainerBase& rhs ) const
		{
			using namespace utility::sindylib_base;
			return irow( lhs )->OID < irow( rhs )->OID;
		}
	};

	const CSPTableFinder CacheManager::cache( long cMeshCode, CString cQueryWhereClause )
	{
		using namespace sindy;
		using namespace utility::sindylib_base;

		if (! isCached( cMeshCode ))
		{
			// ���b�V���̃t�B�[�`�����擾
			IFeaturePtr ipMesh( getMeshFeature( m_pBaseMeshClass, cMeshCode ) );
			// ���b�V���̃t�B�[�`����������Ȃ���Ύ��s
			if (! ipMesh)
			{
				throw std::runtime_error("���b�V���̃t�B�[�`���擾�Ɏ��s");
			}

			// �e�[�u���t�@�C���_���쐬
			CSPTableFinder pTableFinder( new CTableFinder );
			m_Cache[ cMeshCode ] = pTableFinder;
			pTableFinder->InitCollection( AheGetTable( m_pRoadLinkClass), m_Model );

			// ���b�V�����̑S�����N���擾
			IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
			ipQueryFilter = AheGetSpatialFilter( ipMesh->GetShapeCopy(), m_pRoadLinkClass, esriSpatialRelContains );

			// �R�}���h���C�������ŗ^����ꂽ�N�G����ǉ� (bug9617)
			if (! cQueryWhereClause.IsEmpty() )
			{
				ipQueryFilter->PutWhereClause( _bstr_t( cQueryWhereClause ) );
			}

			// ���b�V�����̃����N�ƁA���f����ł���Ɋ֘A�t���I�u�W�F�N�g�Q�i�����N��LQ��Inf�܂Łj���L���b�V��
			using namespace schema;
			CSPTableContainer pLinkTable = pTableFinder->FindTable( road_link::kTableName );		///< �����N�e�[�u��
			pLinkTable->Select(
				std::map< CString,IQueryFilterPtr >( 
					boost::assign::map_list_of(
						CString( road_link::kTableName ),
						ipQueryFilter
					)
				)
			);
			pLinkTable->sort( lessCSPContainerBase() );
			// ��L�œǂݍ���Inf_Turnreg�ɑ�����LQ�Q��ǂݍ���
			road_link_table( pLinkTable )->SelectLQs( false, false, linkqueue_type::kLRTurn );

			// ���b�V�����̃m�[�h���L���b�V��
			CSPTableContainer pNodeTable = pTableFinder->FindTable( road_node::kTableName );		///< �m�[�h�e�[�u��
			pNodeTable->Select(
				std::map< CString,IQueryFilterPtr >( 
					boost::assign::map_list_of(
						CString( road_node::kTableName ),
						AheGetSpatialFilter( ipMesh->GetShapeCopy(), m_pRoadNodeClass, esriSpatialRelIntersects )
					)
				)
			);
			pNodeTable->sort( lessCSPContainerBase() );
		}

		return m_Cache[ cMeshCode ];
	}

	void CacheManager::clear( long cMeshCode )
	{
		m_Cache.erase( cMeshCode );
	}

	void CacheManager::clearExcept( MeshCodes cMeshCodes )
	{
		// �L���b�V����������郁�b�V���̃��b�V���R�[�h�Q���쐬
		MeshCodes aClearMeshCodes;
		BOOST_FOREACH (Cache::value_type& rElement, m_Cache)
		{
			long aMeshCode = rElement.first;

			// ���O���b�V�����X�g�ɍڂ��Ă��郁�b�V���͏��������Ȃ�
			if (cMeshCodes.find( aMeshCode ) == cMeshCodes.end())
			{
				aClearMeshCodes.insert( aMeshCode );
			}
		}

		// ���
		BOOST_FOREACH (long aMeshCode, aClearMeshCodes)
		{
			clear( aMeshCode );
		}
	}

	long CacheManager::existingMeshCode( long cLinkID ) const
	{
		using namespace sindy::schema;

		BOOST_FOREACH (const Cache::value_type& rElement, m_Cache)
		{
			const CSPTableFinder pTableFinder = rElement.second;
			const CSPTableContainer pLinkTable = pTableFinder->FindTable( road_link::kTableName );
			CContainerBase::iterator it = pLinkTable->find_by_key( kObjectID, cLinkID );
			if (it != pLinkTable->end())
			{
				return rElement.first;
			}
		}
		return 0;
	}

} // namespace curve_info

