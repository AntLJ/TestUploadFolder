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

#include "stdafx.h"
#include "Container.h"
#include "Feature.h"
#include "ErrorObject.h"
#include "GlobalFunctions.h"

using namespace sindy;
using namespace sindy::errorcode;

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR DUPLICATE_OF_OID = _T("OID:%d�Əd�����Ă��܂�");
#else
	LPCTSTR DUPLICATE_OF_OID = _T("Duplicate of OID: %d");
#endif // SINDY_FOR_ENGLISH
}

CContainer::CContainer(void)
{
}

CContainer::~CContainer(void)
{
}

errorcode::sindyErrCode CContainer::Materialization( ITable* ipTable )
{
	errorcode::sindyErrCode emErr = errorcode::sindyErr_NoErr; // �߂�l

	for( iterator it = begin(); it != end(); ++it )
	{
		// �֘A���R�[�h�����̉�
		for( rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			emErr = (*it2)->Materialization( ipTable );
	}

	// find�p�C���f�b�N�X���X�V
	UpdateIndex();

	return emErr;
}

CContainer& CContainer::operator =( const CContainer& obj )
{
	m_mapRowsContainer = obj.m_mapRowsContainer;
#ifdef _USE_FIND_INDEX
	m_mapRowsContainerOIDIndex = obj.m_mapRowsContainerOIDIndex;
#endif //_USE_FIND_INDEX
	return *this;
}

void CContainer::push_back( const CSPRow& cRow, bool bSecureUnique/* = true*/ )
{
	if( ! ( bSecureUnique && find( *cRow ) ) )
	{
		sindyTableType::ECode emType = cRow->GetTableType();
		m_mapRowsContainer[emType].push_back( cRow );
#ifdef _USE_FIND_INDEX
		long lOID = cRow->GetOID();
		if( 0 < lOID )
			m_mapRowsContainerOIDIndex[emType][lOID] = m_mapRowsContainer[emType].size() - 1;
#endif //_USE_FIND_INDEX
	}
}

void CContainer::push_front( const CSPRow& cRow, bool bSecureUnique/* = true*/ )
{
	if( ! ( bSecureUnique && find( *cRow ) ) )
	{
		sindyTableType::ECode emType = cRow->GetTableType();
		m_mapRowsContainer[emType].push_front( cRow );
#ifdef _USE_FIND_INDEX
		long lOID = cRow->GetOID();
		if( 0 < lOID )
		{
			// �����̂��͈̂�����炷�K�v������̂ŁA���̃��\�b�h��push_back���Ⴞ�߂�
			// �ꍇ�Ɍ��肷�邱�ƁI�I
			for( std::map<long,long>::iterator it = m_mapRowsContainerOIDIndex[emType].begin(); it != m_mapRowsContainerOIDIndex[emType].end(); ++it )
				++it->second;
			m_mapRowsContainerOIDIndex[emType][lOID] = 0;
		}
#endif //_USE_FIND_INDEX
	}
}

void CContainer::clear()
{
	m_mapRowsContainer.clear();
#ifdef _USE_FIND_INDEX
	m_mapRowsContainerOIDIndex.clear();
#endif //_USE_FIND_INDEX
}

bool CContainer::QueryDoughnutInteriorContainFeatures(sindyTableType::ECode emTableType, const CGeometry& cGeom, CContainer& cContains) const
{
	for ( const_rows_iterator it2 = begin(emTableType), it2End = end(emTableType); it2 != it2End; ++it2 ) {
		bool bHasContain = false;

		// Interior
		CGeometry::const_part_iterator itPart = (*it2)->GetShape()->part_begin(); ++itPart;

		for ( ; itPart != (*it2)->GetShape()->part_end(); ++itPart ) {
			CGeometry cInteriorGeom = CGeometry::ToPolygon( (*itPart) );

			if ( cInteriorGeom.Contains( cGeom ) || cInteriorGeom.Equals( cGeom ) ) {
				bHasContain = true;
				break;
			}
		}

		if ( bHasContain ) {
			cContains.push_back( (*it2) );
			break;
		}
	}

	return !cContains.empty();
}

// �R���e�i���I�u�W�F�N�g�����̏����Œ��o����
CContainer CContainer::Query( sindyCompareOperator emOperator, sindyTableType::ECode emTableType ) const
{
	CContainer cContainer;
	Query( emOperator, emTableType, cContainer );
	return cContainer;
}
CContainer CContainer::Query( sindyCompareOperator emOperator, const CGeometry& cGeom, sindyeSpatialRel emSpRel ) const
{
	CContainer cContainer;
	Query( emOperator, cGeom, emSpRel, cContainer );
	return cContainer;
}
CContainer CContainer::Query( sindyCompareOperator emOperator, long lFieldIndex, const VARIANT& vaValue ) const
{
	CContainer cContainer;
	Query( emOperator, lFieldIndex, vaValue, cContainer );
	return cContainer;
}
CContainer CContainer::Query( sindyCompareOperator emOperator, LPCTSTR lpcszFieldName, const VARIANT& vaValue ) const
{
	CContainer cContainer;
	Query( emOperator, lpcszFieldName, vaValue, cContainer );
	return cContainer;
}
CContainer CContainer::Query( sindyCompareOperator emOperator, LPCTSTR lpcszTableName ) const
{
	CContainer cContainer;
	Query( emOperator, lpcszTableName, cContainer );
	return cContainer;
}
void CContainer::Query( sindyCompareOperator emOperator, sindyTableType::ECode emTableType, CContainer& cContainer ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			sindyTableType::ECode emTargetTableType = (*it2)->GetTableType();
			switch( emOperator )
			{
				case sindyCompOpeEqual:	           if( emTargetTableType == emTableType ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanSmall:	       if( emTargetTableType < emTableType )  cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanBig:          if( emTargetTableType > emTableType )  cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanSmallOrEqual: if( emTargetTableType <= emTableType ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanBigOrEqual:   if( emTargetTableType >= emTableType ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeNotEqual:         if( emTargetTableType != emTableType ) cContainer.push_back( *it2 ); break;
				default: _ASSERTE( false ); break;
			}
			(*it2)->Query( emOperator, emTableType, cContainer );
		}
	}
}
void CContainer::Query( sindyCompareOperator emOperator, const CGeometry& cGeom, sindyeSpatialRel emSpRel, CContainer& cContainer ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			if( ! (IGeometry*)cGeom )
				cContainer.push_back(*it2);
			else {
				CGeometry* pGeom = (*it2)->GetShape();
				if( ! pGeom )
					continue;

				if( emSpRel & sindyeSpatialRelTouch ) {
					bool bRet = cGeom.Touches( (IGeometry*)(*(*it2)->GetShape()) );
					if( ( sindyCompOpeEqual == emOperator && bRet ) ||
						( sindyCompOpeNotEqual == emOperator && ( ! bRet ) ) )
						cContainer.push_back( *it2 );
				}
				if( emSpRel & sindyeSpatialRelOverlap ) {
					bool bRet = cGeom.Overlaps( (IGeometry*)(*(*it2)->GetShape()) );
					if( ( sindyCompOpeEqual == emOperator && bRet ) ||
						( sindyCompOpeNotEqual == emOperator && ( ! bRet ) ) )
						cContainer.push_back( *it2 );
				}
				if( emSpRel & sindyeSpatialRelCross ) {
					bool bRet = cGeom.Crosses( (IGeometry*)(*(*it2)->GetShape()) );
					if( ( sindyCompOpeEqual == emOperator && bRet ) ||
						( sindyCompOpeNotEqual == emOperator && ( ! bRet ) ) )
						cContainer.push_back( *it2 );
				}
				if( emSpRel & sindyeSpatialRelWithin ) {
					bool bRet = cGeom.Within( (IGeometry*)(*(*it2)->GetShape()) );
					if( ( sindyCompOpeEqual == emOperator && bRet ) ||
						( sindyCompOpeNotEqual == emOperator && ( ! bRet ) ) )
						cContainer.push_back( *it2 );
				}
				if( emSpRel & sindyeSpatialRelContain ) {
					bool bRet = cGeom.Contains( (IGeometry*)(*(*it2)->GetShape()) );
					if( ( sindyCompOpeEqual == emOperator && bRet ) ||
						( sindyCompOpeNotEqual == emOperator && ( ! bRet ) ) )
						cContainer.push_back( *it2 );
				}
				if( emSpRel & sindyeSpatialRelDisjoint ) {
					bool bRet = cGeom.Disjoint( (IGeometry*)(*(*it2)->GetShape()) );
					if( ( sindyCompOpeEqual == emOperator && bRet ) ||
						( sindyCompOpeNotEqual == emOperator && ( ! bRet ) ) )
						cContainer.push_back( *it2 );
				}
				if( emSpRel & sindyeSpatialRelEqual ) {
					bool bRet = cGeom.Equals( (IGeometry*)(*(*it2)->GetShape()) );
					if( ( sindyCompOpeEqual == emOperator && bRet ) ||
						( sindyCompOpeNotEqual == emOperator && ( ! bRet ) ) )
						cContainer.push_back( *it2 );
				}
			}
			(*it2)->Query( emOperator, cGeom, emSpRel, cContainer );
		}
	}
}
void CContainer::Query( sindyCompareOperator emOperator, long lFieldIndex, const VARIANT& vaValue, CContainer& cContainer ) const
{
	if( 0 > lFieldIndex )
	{
		_ASSERTE( 0 <= lFieldIndex );
		return;
	}

	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			const CFieldMap& cFields( *(*it2)->GetFieldMap() );
			if( (size_t)lFieldIndex < cFields.GetFieldCount() )
			{
				CComVariant vaTargetValue;
				(*it2)->GetValue( lFieldIndex, vaTargetValue );
				if( vaTargetValue.vt == vaValue.vt ||
					vaTargetValue.vt == VT_NULL || 
					vaValue.vt == VT_NULL )
				{
					switch( emOperator )
					{
						case sindyCompOpeEqual:            if( vaTargetValue == vaValue ) cContainer.push_back( *it2 ); break;
						case sindyCompOpeThanSmall:        if( vaTargetValue <  vaValue ) cContainer.push_back( *it2 ); break;
						case sindyCompOpeThanBig:          if( vaTargetValue >  vaValue ) cContainer.push_back( *it2 ); break;
						case sindyCompOpeThanSmallOrEqual: if( vaTargetValue < vaValue || vaTargetValue == vaValue ) cContainer.push_back( *it2 ); break;
						case sindyCompOpeThanBigOrEqual:   if( vaTargetValue > vaValue || vaTargetValue == vaValue ) cContainer.push_back( *it2 ); break;
						case sindyCompOpeNotEqual:         if( vaTargetValue != vaValue ) cContainer.push_back( *it2 ); break;
						default: _ASSERTE( false ); break;
					}
				}
			}
			(*it2)->Query( emOperator, lFieldIndex, vaValue, cContainer );
		}
	}
}
void CContainer::Query( sindyCompareOperator emOperator, LPCTSTR lpcszFieldName, const VARIANT& vaValue, CContainer& cContainer ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			CComVariant vaTargetValue;
			(*it2)->GetValueByFieldName( lpcszFieldName, vaTargetValue );
			if( vaTargetValue.vt == vaValue.vt ||
				vaTargetValue.vt == VT_NULL || 
				vaValue.vt == VT_NULL )
			{
				switch( emOperator )
				{
					case sindyCompOpeEqual:            if( vaTargetValue == vaValue ) cContainer.push_back( *it2 ); break;
					case sindyCompOpeThanSmall:        if( vaTargetValue <  vaValue ) cContainer.push_back( *it2 ); break;
					case sindyCompOpeThanBig:          if( vaTargetValue >  vaValue ) cContainer.push_back( *it2 ); break;
					case sindyCompOpeThanSmallOrEqual: if( vaTargetValue < vaValue || vaTargetValue == vaValue ) cContainer.push_back( *it2 ); break;
					case sindyCompOpeThanBigOrEqual:   if( vaTargetValue > vaValue || vaTargetValue == vaValue ) cContainer.push_back( *it2 ); break;
					case sindyCompOpeNotEqual:         if( vaTargetValue != vaValue ) cContainer.push_back( *it2 ); break;
					default: _ASSERTE( false ); break;
				}
				(*it2)->Query( emOperator, lpcszFieldName, vaValue, cContainer );
			}
		}
	}
}
void CContainer::Query( sindyCompareOperator emOperator, LPCTSTR lpcszTableName, CContainer& cContainer ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			CString strFieldName( (*it2)->GetTableName() );
			switch( emOperator )
			{
				case sindyCompOpeEqual:            if( strFieldName == lpcszTableName ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanSmall:        if( strFieldName <  lpcszTableName ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanBig:          if( strFieldName >  lpcszTableName ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanSmallOrEqual: if( strFieldName <= lpcszTableName ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeThanBigOrEqual:   if( strFieldName >= lpcszTableName ) cContainer.push_back( *it2 ); break;
				case sindyCompOpeNotEqual:         if( strFieldName != lpcszTableName ) cContainer.push_back( *it2 ); break;
				default: _ASSERTE( false ); break;
			}
			(*it2)->Query( emOperator, lpcszTableName, cContainer );
		}
	}
}

// �L���b�V�����폜����
void CContainer::DeleteCache( bool bDeleteContainerCache/* = false*/ )
{
	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
		for( rows_iterator itRows = it->second.begin(), itRowsEnd = it->second.end(); itRows != itRowsEnd; ++itRows )
			itRows->get()->DeleteCache( bDeleteContainerCache );
	}
}

// �t�B�[�`���̕ύX�����ɖ߂�
void CContainer::Reset( bool bRecursive/* = false*/ )
{
	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
		for( rows_iterator itRows = it->second.begin(); itRows != it->second.end(); )
		{
			itRows->get()->Reset( bRecursive );
			// �܂��폜�t���O���c���Ă���ꍇ�̓R���e�i���珜��
			if( itRows->get()->IsDeleted() )
				itRows = it->second.erase( itRows );
			else
				++itRows;
		}
	}
}

// �폜����
void CContainer::Delete( LPCTSTR lpcszTableName/* = NULL*/, bool bRecursive/* = false*/ )
{
	if( lpcszTableName )
	{
		bool AllScan = false; // �e�[�u������S�ăX�L�������邩�ǂ���

		sindyTableType::ECode emType = AheTableName2TableType( lpcszTableName );
		if( sindyTableType::other == emType || 
			sindyTableType::unknown == emType )
			AllScan = true;

		if( AllScan )
		{
			// �e�[�u�������r���Ȃ���폜
			for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
			{
				bool bCanDeleteContainer = false; // �R���e�i�P�ʂŏ����Ă��ǂ����ǂ���
				// �e�[�u���^�C�v���������Ă���ꍇ�͂����Ŕ��肷�邱�Ƃŏ����ł��X�s�[�h�҂�
				if( sindyTableType::other != it->first && 
					0 == _tcsicmp( AheTableType2TableName(it->first), lpcszTableName ) )
					bCanDeleteContainer = true;

				for( rows_iterator itRows = it->second.begin(), itRowsEnd = it->second.end(); itRows != itRowsEnd; ++itRows )
				{
					if( bCanDeleteContainer ||                                         // �R���e�i�P�ʂŏ����Ă��ǂ��ꍇ
						0 == _tcsicmp( (*itRows)->GetTableName(), lpcszTableName ) )   // �e�[�u��������v����ꍇ
						itRows->get()->Delete( bRecursive );
				}
			}
		}
		else {
			// ����̃e�[�u���^�C�v�̂ݍ폜
			iterator it = find( emType );
			if( it != end() )
			{
				for( rows_iterator itRows = it->second.begin(), itRowsEnd = it->second.end(); itRows != itRowsEnd; ++itRows )
					(*itRows)->Delete( bRecursive );
			}
		}

	}
	else {
		// �S�č폜
		for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		{
			for( rows_iterator itRows = it->second.begin(), itRowsEnd = it->second.end(); itRows != itRowsEnd; ++itRows )
				itRows->get()->Delete( bRecursive );
		}
	}
}


// �^����ꂽ�`��Ƃ̌��������ŕ����������s��
/*errorcode::sindyErrCode CContainer::Split( const CGeometry& cGeom, const CSplitTarget& cSplitTarget, CErrorObjects& cErrObjects )
{
	for( iterator it = begin(); it != end(); ++it )
	{
		CErrorObject cErrObj( *it ); // �G���[���i�[�p

		if( sindyErr_NoErr != it->Split( cGeom, cSplitTarget, cErrObj.push_back( CErrorInfos() ) ) )
			cErrObjects.push_back( cErrObj );
	}

	return cErrObjects.GetDefaultErrCode();
}*/

// �^����ꂽ�`��͈̔͂ŕ��s�ړ��������s��
sindyErrCode CContainer::Move( const CGeometry& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMove, CErrorObjects& cErrObjects )
{
	for( iterator it = begin(); it != end(); ++it )
	{
		for( rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			CFeature* pFeat = NULL;
			if( NULL != ( pFeat = dynamic_cast<CFeature*>( it2->get() ) ) )
				pFeat->Move( cGeom, pairMove, emMove, cErrObjects );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// �ҏW���e��ۑ�����
sindyErrCode CContainer::Store( CErrorObjects& cErrObjects )
{
	for( iterator it = begin(); it != end(); ++it )
	{
		for( rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			// �ύX�����������ǂ�����CRow::Store()���ōs�����߁A�����ł�
			// �P����CRow::Store()���ĂԂ����ł悢
			// �֘A�I�u�W�F�N�g�̕ۑ���CRow::Store()�ōs��
			(*it2)->Store( cErrObjects );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// ���������`�F�b�N����
sindyErrCode CContainer::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const
{
	// TODO : �`�F�b�J�[�Ƃ��Ďg�����߂ɂ́A�`�F�b�N����̈��CFeatures::CheckLogic()����n����悤�ɂ��Ȃ��Ƃ��߁B

	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			// �P�̃`�F�b�N
			//
			// �ύX����Ȃ��Ɋւ�炸���Ȃ��Ɠ����R���e�i�܂Ń`�F�b�N�������Ȃ�
			// ���A�`��ɂ�炸�Ƀ`�F�b�N���K�v�ibug 6410�j
			(*it2)->CheckLogic(  /*g_cRule.CreateLogicRule( *it, */cRule/* )*/, cErrObjects );

			// �����`�F�b�N
			//
			// �ύX������A���폜����Ă��Ȃ����̂̂݃`�F�b�N
			// TODO�F�`�F�b�J�[�Ƃ��Ďg�p����ɂ�CLogicRule�Ƀt���O����������K�v����i���̂܂܂ł͔��ɂ܂����j
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;
			(*it2)->Changed( &emUpdateType );
			if( emUpdateType == schema::ipc_table::update_type::kDeleted ||
				emUpdateType == schema::ipc_table::update_type::kDefault )
				continue;

			// �����`�F�b�N
			CGeometry* pGeom = (*it2)->GetShape();
			if( pGeom )
			{
				IGeometryPtr ipEditArea;
				CErrorObject cErrObj( *it2->get() ); // �G���[���i�[�p

				// �|���S�����ύX���������I�u�W�F�N�g��1000�_�ȏ�\���_�������̂̂ݕύX�̈�擾
				if ( pGeom->GetGeometryType() == esriGeometryPolygon && emUpdateType != schema::ipc_table::update_type::kDefault && pGeom->GetPointCount() >= 1000 ) {
					// �ύX�̈�擾
					ipEditArea = (*it2)->GetShapeCopy(true, true, false, true);
				}

				// �ύX���������I�u�W�F�N�g�̂�
				if ( emUpdateType != schema::ipc_table::update_type::kDefault )
				{
					// �d�������C�����������΂�
					if( AheIsOverlapableLayer( AheTableType2TableName(it->first) ) )
						continue;

					// �`��d���`�F�b�N�ibug 4963�j
					for( const_rows_iterator it3 = it->second.begin(); it3 != it->second.end(); ++it3 )
					{
						if ( it2 != it3 ) {
							emUpdateType = schema::ipc_table::update_type::kDefault;

							// �폜����Ă��Ȃ����̂ƃ`�F�b�N
							if ( ! (*it3)->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted )
							{
								CGeometry* pGeom2 = (*it3)->GetShape();
								if( pGeom2 )
								{
									if ( pGeom2->GetGeometryType() == esriGeometryPolygon ) {
										bool bCheck2 = true;

										if ( ipEditArea )
										{
											// �ύX�̈�ɂ������Ă���I�u�W�F�N�g�̂ݏd���`�F�b�N
											if ( ! pGeom2->Overlaps( ipEditArea ) )
												bCheck2 = false;
										}

										if ( bCheck2 )
										{
											// [Bug 5804]��܂���Ă�����́A���Ă�����̗����Ń`�F�b�N���Ȃ��Ƃ����Ȃ�
											if( pGeom->Overlaps( (IGeometry*)(*pGeom2) ) || 
												pGeom->Contains( (IGeometry*)(*pGeom2) ) ||
												pGeom->Within( (IGeometry*)(*pGeom2) ) ||
												pGeom->Equals( (IGeometry*)(*pGeom2) ) )
											{
												cErrObj.push_back( CErrorInfo( sindyErr_GeometryOverlapped, sindyErrLevel_ERR, (IGeometry*)(*(*it3)->GetShape()), -1, -1, DUPLICATE_OF_OID, (*it3)->GetOID() ) );
											}
										}
									}
								}
							}
						}
					}
				}
				if( sindyErr_NoErr != cErrObj.GetDefaultErrCode() )
					cErrObjects.push_back( cErrObj );
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// �R���t���N�g����̂��߂̊��ҏW�`�F�b�N���s��
sindyErrCode CContainer::CheckModify( CErrorObjects& cErrObjects ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			(*it2)->CheckModify( cErrObjects );
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// �L���b�V������Ă��邩�ǂ������`�F�b�N����
bool CContainer::IsCached() const
{
	bool bRet = false;	// �Ԃ�l

	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			bool bCached = (*it2)->IsCached();
			if( ! bRet )
				bRet = bCached;
		}
	}

	return bRet;
}

CSPRow CContainer::find( _IRow* ipRow ) const
{
	return find( CRow(ipRow) );
}

CSPRow CContainer::find( const CRow& cRow ) const
{
	for( const_iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
		for( const_rows_iterator itRows = it->second.begin(), itRowsEnd = it->second.end(); itRows != itRowsEnd; ++itRows )
		{
			if( cRow == **itRows )
				return *itRows;

			CSPRow cRet( (*itRows)->find(cRow) );
			if( cRet.get() )
				return cRet;
		}
	}
	return CSPRow();
}

CContainer::rows_iterator CContainer::find( sindyTableType::ECode emType, long lOID )
{
	rows_iterator itEnd = end(emType);
#ifndef _USE_FIND_INDEX
	for( rows_iterator it = begin(emType); it != itEnd; ++it )
	{
		if( (*it)->GetOID() == lOID )
			return it;
	}
#else
	std::map<long,long>::const_iterator it = m_mapRowsContainerOIDIndex[emType].find(lOID);
	if( it != m_mapRowsContainerOIDIndex[emType].end() )
	{
		rows_iterator itRows = m_mapRowsContainer[emType].begin();
		std::advance( itRows, it->second );
		return itRows;
	}
#endif  //_USE_FIND_INDEX
	return itEnd;
}

CContainer::const_rows_iterator CContainer::find( sindyTableType::ECode emType, long lOID ) const
{
	const_rows_iterator itEnd = end(emType);
#ifndef _USE_FIND_INDEX
	for( const_rows_iterator it = begin(emType); it != itEnd; ++it )
	{
		if( (*it)->GetOID() == lOID )
			return it;
	}
#else
	TRowsContainerOIDIndex::const_iterator it = m_mapRowsContainerOIDIndex.find(emType);
	if( it != m_mapRowsContainerOIDIndex.end() )
	{
		std::map<long,long>::const_iterator itIndex = it->second.find(lOID);
		if( itIndex != it->second.end() )
		{
			const_rows_iterator itRows = m_mapRowsContainer.find(emType)->second.begin();
			std::advance( itRows, itIndex->second );
			return itRows;
		}
	}
#endif  //_USE_FIND_INDEX
	return itEnd;
}

CContainer::rows_iterator CContainer::find( sindyTableType::ECode emType, LPCTSTR lpcszFieldName, const VARIANT& vaValue )
{
	rows_iterator itEnd = end(emType);
	for( rows_iterator it = begin(emType); it != itEnd; ++it )
	{
		CComVariant vaTestValue;
		(*it)->GetValueByFieldName(lpcszFieldName, vaTestValue);
		if( vaTestValue == vaValue )
			return it;
	}
	return itEnd;
}
CContainer::const_rows_iterator CContainer::find( sindyTableType::ECode emType, LPCTSTR lpcszFieldName, const VARIANT& vaValue ) const
{
	const_rows_iterator itEnd = end(emType);
	for( const_rows_iterator it = begin(emType); it != itEnd; ++it )
	{
		CComVariant vaTestValue;
		(*it)->GetValueByFieldName(lpcszFieldName, vaTestValue);
		if( vaTestValue == vaValue ) 
			return it;
	}
	return itEnd;
}

void CContainer::erase( const CSPRow& cRow )
{
	sindyTableType::ECode emType = cRow->GetTableType();
	const_rows_iterator it = find( emType, cRow->GetOID() );

	if ( it != end(emType) )
		m_mapRowsContainer[emType].erase( std::remove( begin(emType), end(emType), cRow ), end(emType) );
	UpdateIndex();
}

CContainer::rows_iterator CContainer::erase( sindyTableType::ECode emType, rows_iterator position )
{
	return m_mapRowsContainer[emType].erase( position );
}

CContainer::rows_iterator CContainer::erase( sindyTableType::ECode emType, rows_iterator first, rows_iterator last )
{
	return m_mapRowsContainer[emType].erase( first, last );
}

// ���݊i�[����Ă���t�B�[�`���̑��݋�`��Ԃ�
IEnvelopePtr CContainer::GetEnvelope( bool bOriginal/* = true*/, bool bChanged/* = true*/, bool bFeatureChangedOriginal/* = true*/ ) const
{
	IEnvelopePtr ipRet;
	for( const_iterator it = begin(); it != end(); it++ )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			IEnvelopePtr ipEnv( (*it2)->GetEnvelope( bOriginal, bChanged, bFeatureChangedOriginal ) );
			if( ipRet )
			{
				if( ipEnv )
				{
					LOGASSERTE_IF( SUCCEEDED( ipRet->Union( ipEnv ) ), sindyErr_GeometryFunctionFailed )
					{
					}
				}
			}
			else
				ipRet = ipEnv;
		}
	}
	return ipRet;
}

// ���݊i�[����Ă���t�B�[�`���̌`����}�[�W�������̂�Ԃ�
IGeometryPtr CContainer::GetShape( bool bLatest/* = true*/, bool bOriginal/* = false*/, bool bFeatureChangedOriginal/* = false*/, long lParam/* = -1*/ ) const
{
	// TODO: ��K�w�����Ή����Ă��Ȃ�
	// �����Ƃ��Ȃ�A�W�I���g���^�C�v���ƂɌ`����W
	// -> �W�I���g���^�C�v���ƂɃ}�[�W
	// -> GeometryBag�Ɋi�[
	// �Ƃ�����ق����������ǂ��H
	IGeometryPtr ipRet;

	IGeometryCollectionPtr ipPointsCol, ipPolylinesCol, ipPolygonsCol;
	for( const_iterator itType = begin(); itType != end(); ++itType )
	{
		for( const_rows_iterator it = itType->second.begin(); it != itType->second.end(); ++it )
		{
			IGeometryPtr ipGeom = (*it)->GetShapeCopy( bLatest, bOriginal, bFeatureChangedOriginal, false, lParam );
			if( ipGeom != NULL )
			{
				esriGeometryType emType = esriGeometryAny;
				ipGeom->get_GeometryType( &emType );
				switch( emType )
				{
					case esriGeometryBag:
					{
						IEnumGeometryPtr ipEnumGeom( ipGeom );
						ipEnumGeom->Reset();
						IGeometryPtr ipGeomPart;
						while( S_OK == ipEnumGeom->Next( &ipGeomPart ) )
						{
							esriGeometryType emTypePart = esriGeometryAny;
							ipGeomPart->get_GeometryType( &emTypePart );
							switch( emTypePart )
							{
								case esriGeometryMultipoint:
								case esriGeometryPoint:
									if( ipPointsCol == NULL )
										ipPointsCol.CreateInstance( CLSID_GeometryBag );
									ipPointsCol->AddGeometry( ipGeomPart );
									break;
								case esriGeometryPolyline:
									if( ipPolylinesCol == NULL )
										ipPolylinesCol.CreateInstance( CLSID_GeometryBag );
									ipPolylinesCol->AddGeometry( ipGeomPart );
									break;
								case esriGeometryPolygon:
									if( ipPolygonsCol == NULL )
										ipPolygonsCol.CreateInstance( CLSID_GeometryBag );
									ipPolygonsCol->AddGeometry( ipGeomPart );
									break;
								default:
									_ASSERTE(false);
									break;
							}
						}
						break;
					}
					case esriGeometryMultipoint:
					case esriGeometryPoint:
						if( ipPointsCol == NULL )
							ipPointsCol.CreateInstance( CLSID_GeometryBag );
						ipPointsCol->AddGeometry( ipGeom );
						break;
					case esriGeometryPolyline:
						if( ipPolylinesCol == NULL )
							ipPolylinesCol.CreateInstance( CLSID_GeometryBag );
						ipPolylinesCol->AddGeometry( ipGeom );
						break;
					case esriGeometryPolygon:
						if( ipPolygonsCol == NULL )
							ipPolygonsCol.CreateInstance( CLSID_GeometryBag );
						ipPolygonsCol->AddGeometry( ipGeom );
						break;
					default:
						_ASSERTE(false);
						break;
				}
			}
		}
	}
	// ���ꂼ���Union����
	ITopologicalOperatorPtr ipPointsTopo, ipPolylinesTopo, ipPolygonsTopo;
	if( ipPointsCol != NULL )
	{
		ipPointsTopo.CreateInstance( CLSID_Multipoint );
		ipPointsTopo->ConstructUnion( (IEnumGeometryPtr)ipPointsCol );
	}
	if( ipPolylinesCol != NULL )
	{
		ipPolylinesTopo.CreateInstance( CLSID_Polyline );
		ipPolylinesTopo->ConstructUnion( (IEnumGeometryPtr)ipPolylinesCol );
	}
	if( ipPolygonsCol != NULL )
	{
		ipPolygonsTopo.CreateInstance( CLSID_Polygon );
		ipPolygonsTopo->ConstructUnion( (IEnumGeometryPtr)ipPolygonsCol );
	}
	// �ǂꂩ������Ȃ���΂����Ԃ�l�Ƃ���
	bool bPoints    = ( ipPointsTopo != NULL && ipPolylinesTopo == NULL && ipPolygonsTopo == NULL );
	bool bPolylines = ( ipPointsTopo == NULL && ipPolylinesTopo != NULL && ipPolygonsTopo == NULL );
	bool bPolygons  = ( ipPointsTopo == NULL && ipPolylinesTopo == NULL && ipPolygonsTopo != NULL );
	if( bPoints || bPolylines || bPolygons )
	{
		if( bPoints )
			ipRet = ipPointsTopo;
		else if( bPolylines )
			ipRet = ipPolylinesTopo;
		else if( bPolygons )
			ipRet = ipPolygonsTopo;
	}
	else if( ipPointsTopo != NULL || ipPolylinesTopo != NULL || ipPolygonsTopo != NULL )
	{
		IGeometryCollectionPtr ipGeomCol( CLSID_GeometryBag );
		if( ipPointsTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPointsTopo );
		if( ipPolylinesTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPolylinesTopo );
		if( ipPolygonsTopo != NULL )
			ipGeomCol->AddGeometry( (IGeometryPtr)ipPolygonsTopo );
		ipRet = ipGeomCol;
	}
	return ipRet;
}

sindyErrCode CContainer::CreateCache( bool bDetatch/* = false*/ )
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			sindyErrCode emErrTmp = (*it2)->CreateCache( bDetatch );
			if( sindyErr_NoErr != emErrTmp )
				return emErrTmp;
		}
	}
	return errorcode::sindyErr_NoErr;
}

// �L���b�V���ȊO�̃p�����[�^���X�V����
sindyErrCode CContainer::UpdateParameter( bool bForce/* = false*/ )
{
	for( iterator it = begin(); it != end(); ++it )
	{
		for( rows_iterator itRow = it->second.begin(); itRow != it->second.end(); ++itRow )
		{
			errorcode::sindyErrCode emErr = (*itRow)->UpdateParameter( bForce );
			if( errorcode::sindyErr_NoErr != emErr )
				return emErr;
		}
	}
	return errorcode::sindyErr_NoErr;
}
void CContainer::DeleteParameter()
{
	for( iterator it = begin(); it != end(); ++it )
	{
		for( rows_iterator itRow = it->second.begin(); itRow != it->second.end(); ++itRow )
			(*itRow)->DeleteParameter();
	}
}

void CContainer::Trace( bool bAttribute/* = true*/, bool bGeometry/* = true*/ ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			(*it2)->Trace( bAttribute, bGeometry );
	}
}

IGeometryPtr CContainer::GetEditArea() const
{
	IGeometryCollectionPtr ipGeomBag;

	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			// �`��ύX�ƐV�K�쐬�̕ύX���������I�u�W�F�N�g
			if ( (*it2)->Changed( &emUpdateType ) && 
				( emUpdateType == schema::ipc_table::update_type::kShapeUpdated || emUpdateType == schema::ipc_table::update_type::kCreated ) ) {
				CGeometry* pGeom = (*it2)->GetShape();

				if( pGeom )
				{
					IGeometryPtr ipEditArea = (*it2)->GetShapeCopy(true, true, false, true);

					if ( ipEditArea )
					{
						if ( ! ipGeomBag ) {
							ipGeomBag.CreateInstance( CLSID_GeometryBag );
							((IGeometryPtr)ipGeomBag)->putref_SpatialReference( (*it2)->GetShape()->GetSpatialReference() );
						}

						ipGeomBag->AddGeometry( (IGeometryPtr)AheCreateClone( ipEditArea ) );
					}
				}
			}
		}
	}

	return ipGeomBag;
}

// find�p��OID�C���f�b�N�X���č\�z����
void CContainer::UpdateIndex()
{
#ifdef _USE_FIND_INDEX
	m_mapRowsContainerOIDIndex.clear();
	for( const_iterator it = begin(); it != end(); ++it )
	{
		std::map<long,long>& mapOIDIndex( m_mapRowsContainerOIDIndex[it->first] );
		long i = 0;
		for( const_rows_iterator itRows = it->second.begin(); itRows != it->second.end(); ++itRows )
			mapOIDIndex[itRows->get()->GetOID()] = i++;
	}
#endif //_USE_FIND_INDEX
}
