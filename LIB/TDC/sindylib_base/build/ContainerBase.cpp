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
#include <boost/foreach.hpp>
#include "ContainerBase.h"
#include "RowBase.h"
#include "GeometryBase.h"
#include "ErrorObject2.h"
#include "GlobalFunctions.h"
#include "SchemaSupport.h"

#define foreach BOOST_FOREACH
using namespace sindy;
using namespace sindy::errorcode;

#ifndef SINDY_FOR_ENGLISH
const static _TCHAR DUPLI_OID[] = _T("OID:%d�Əd�����Ă��܂�");
#else
const static _TCHAR DUPLI_OID[] = _T("Duplicate of OID:%d");
#endif	// SINDY_FOR_ENGLISH

bool CContainerBase::Equal::operator()( const CSPContainerBase& c ) const
{
/*typeof(
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(&*c);
	const CRowBase* pRow2 = dynamic_cast<const CRowBase*>(m_pContainer);
	if( pRow && pRow2 )
		return ( *pRow == *pRow2 );
	else if( pRow || pRow2 )
		return false;
*/
	return (void*)m_pContainer == (void*)&c; // �R���e�i���m�Ȃ璆�g���r���Ă����傤���Ȃ��̂Ń|�C���^��r��
}

/*bool CContainerBase::EqualTableType::operator()( const CSPContainerBase& c ) const
{
	sindyTableType::ECode emType = m_emType;
	sindyTableType::ECode emType2 = c->_GetTableType();

	// �ǂ��炩��unknown�̏ꍇ��false��Ԃ�
	if( sindyTableType::unknown == emType ||
		sindyTableType::unknown == emType2 )
		return false;

	return emType == emType2;
}

bool CContainerBase::EqualTableTypeSPRowBase::operator()( const CSPContainerBase& c ) const
{
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(&*c);
	if( pRow )
		return ( m_emType == pRow->GetTableType() );
	return false;
}

bool CContainerBase::EqualTableType_and_OIDSPRowBase::operator()( const CSPContainerBase& c ) const
{
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(&*c);
	if( pRow )
		return ( m_emType == pRow->GetTableType() && m_lOID == pRow->GetOID() );
	return false;
}

bool CContainerBase::EqualTableType_and_ValueSPRowBase::operator()( const CSPContainerBase& c ) const
{
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(&*c);
	if( pRow )
	{
		CComVariant vaValue;
		pRow->GetValue(m_lpcszFieldName,vaValue);
		return ( m_emType == pRow->GetTableType() && vaValue == *m_value );
	}
	return false;
}

bool CContainerBase::EqualRowBaseSPRowBase::operator()( const CSPContainerBase& c ) const
{
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(&*c);
	if( pRow )
		return ( *m_pRow == *pRow );
	return false;
}*/

CContainerBase::CContainerBase(void)
{
}

CContainerBase::~CContainerBase(void)
{
}

CContainerBase& CContainerBase::operator =( const CContainerBase& obj )
{
	GetContainer() = obj.GetContainer();
	return *this;
}

void CContainerBase::push_back( const CSPContainerBase& cCon, bool bSecureUnique/* = true*/ )
{
	if( ( ! bSecureUnique ) || end() == find( cCon ) )
		GetContainer().push_back( cCon );
}

void CContainerBase::push_front( const CSPContainerBase& cCon, bool bSecureUnique/* = true*/ )
{
	if( ( ! bSecureUnique ) || end() == find( cCon ) )
		GetContainer().push_front( cCon );
}

CContainerBase::iterator CContainerBase::insert( CContainerBase::iterator it, const CSPContainerBase& cContainer, bool bSecureUnique/* = true*/ )
{
	iterator ret = ( bSecureUnique ? find( cContainer ) : end() );
	if( ret == end() )
		ret = GetContainer().insert( it, cContainer );
	return ret;
}

/*CContainerBase::iterator CContainerBase::find( _IRow* ipRow )
{
	return find( CRowBase( ipRow ) );
}

CContainerBase::const_iterator CContainerBase::find( _IRow* ipRow ) const
{
	return find( CRowBase( ipRow ) );
}*/

void CContainerBase::clear()
{
	GetContainer().clear();
}

/*// �R���e�i���I�u�W�F�N�g�����̏����Œ��o����
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
}*/
CSPContainerBase CContainerBase::Query( sindyCompareOperator2 emOperator, LPCTSTR lpcszTableName, bool bSecureUnique/* = true*/ ) const
{
	CSPContainerBase cContainer( new CContainerBase() );
	Query( emOperator, lpcszTableName, *cContainer, bSecureUnique );
	return cContainer;
}
/*void CContainer::Query( sindyCompareOperator emOperator, sindyTableType::ECode emTableType, CContainer& cContainer ) const
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
}*/
CContainerBase& CContainerBase::Query( sindyCompareOperator2 emOperator, LPCTSTR lpcszTableName, CContainerBase& cContainer, bool bSecureUnique/* = true*/ ) const
{
	// dynamic_cast���g���̂Œx��
	// CRowContainer��CTableContainer��CContainerBase���p������N���X��
	// �����������̂��g�p�����K�v������
	// �����ł����Ď������Ă���̂͏������z�֐��ɂ������Ȃ����߂���
	for( const_iterator it = begin(); it != end(); ++it )
	{
		CSPSchemaSupport cSchema = boost::dynamic_pointer_cast<CSchemaSupport>(*it); // �x���Ȃ�
		if( cSchema )
		{
			CString strFieldName( cSchema->GetTableName() );
			switch( emOperator )
			{
			case sindyCompOpeEqual2:            if( strFieldName == lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeThanSmall2:        if( strFieldName <  lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeThanBig2:          if( strFieldName >  lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeThanSmallOrEqual2: if( strFieldName <= lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeThanBigOrEqual2:   if( strFieldName >= lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeNotEqual2:         if( strFieldName != lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			default: _ASSERTE( false ); break;
			}
		}
		(*it)->Query( emOperator, lpcszTableName, cContainer, bSecureUnique );
	}
	return cContainer;
}

// �L���b�V�����폜����
void CContainerBase::DeleteCache()
{
	CRowBase* pRow = dynamic_cast<CRowBase*>(this);
	if( pRow )
		pRow->DeleteCache();

	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		(*it)->DeleteCache();
}

// �t�B�[�`���̕ύX�����ɖ߂�
void CContainerBase::Reset()
{
	foreach( CSPContainerBase& con, GetContainer() )
		con->Reset();
}

// �폜����
void CContainerBase::Delete( /*LPCTSTR lpcszTableName*//* = NULL*//*, bool bRecursive*//* = false*/ )
{
#if 0
	// �S�폜���ǂ����̔���
	bool bAllDelete = ( lpcszTableName ) ? true : false;
	if( ! bAllDelete )
	{
		LPCTSTR lpcszThisTableName = _GetTableName();
		if( lpcszThisTableName && 0 == lstrcmpi( lpcszThisTableName, lpcszTableName ) )
			bAllDelete = true;
	}

	if( bAllDelete )
	{
		CRowBase* pRowBase = dynamic_cast<CRowBase*>(this);
		if( pRowBase )
			pRowBase->Delete();

		for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		{
			bool bDelete = ( bAllDelete ) ? true : false;
			(*it)->Delete( bAllDelete ? NULL : lpcszTableName );
		}
	}
#else
	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		(*it)->Delete();
#endif // if 0
}

void CContainerBase::UnDelete()
{
	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		(*it)->UnDelete();
}

// �^����ꂽ�`��͈̔͂ŕ��s�ړ��������s��
sindyErrCode CContainerBase::Move( const CGeometryBase& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMove, CErrorObjectsBase& cErrs )
{
	// CRowBase���̏���
	CRowBase* pRow = dynamic_cast<CRowBase*>(this);
	if( pRow )
		pRow->Move( cGeom, pairMove, emMove, cErrs );

	// �R���e�i���X�g�̏���
	for( iterator it = begin(); it != end(); ++it )
		(*it)->Move( cGeom, pairMove, emMove, cErrs );

	return cErrs.GetDefaultErrCode();
}
// �ҏW���e��ۑ�����
sindyErrCode CContainerBase::Store( CErrorObjectsBase& cErrs )
{
	// CRowBase���̏���
	CRowBase* pRow = dynamic_cast<CRowBase*>(this);
	if( pRow )
		pRow->Store( cErrs );

	// �R���e�i���X�g�̏���
	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		(*it)->Store( cErrs );

	return cErrs.GetDefaultErrCode();
}

// ���������`�F�b�N����
sindyErrCode CContainerBase::CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrObjects ) const
{
	// TODO : �`�F�b�J�[�Ƃ��Ďg�����߂ɂ́A�`�F�b�N����̈��CFeatures::CheckLogic()����n����悤�ɂ��Ȃ��Ƃ��߁B

	// CRowBase���̏���
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow )
		pRow->CheckLogic( cRule, cErrObjects );

	// �R���e�i���X�g�̏���
	for( const_iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
		const CRowBase* pRow = dynamic_cast<const CRowBase*>(&**it);
		if( pRow )
		{
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			// �폜����Ă��Ȃ����̂̂݃`�F�b�N
			if ( ! pRow->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted )
			{
				// �P�̃`�F�b�N
				pRow->CheckLogic( cRule, cErrObjects );

				// �����`�F�b�N
				CGeometryBase cGeom( (IGeometry*)*pRow );
				if( (IGeometry*)cGeom )
				{
					IGeometryPtr ipEditArea;
					CSPErrorObjectBase cErrObj( new CErrorObjectBase( *pRow ) ); // �G���[���i�[�p

					// �|���S�����ύX���������I�u�W�F�N�g��1000�_�ȏ�\���_�������̂̂ݕύX�̈�擾
					if ( cGeom.GetGeometryType() == esriGeometryPolygon && emUpdateType != schema::ipc_table::update_type::kDefault && cGeom.GetPointCount() >= 1000 ) {
						// �ύX�̈�擾
						ipEditArea = pRow->GetShapeCopy((ShapeMode)(enumAll|enumDiff));
					}

					// �ύX���������I�u�W�F�N�g�̂�
					if ( emUpdateType != schema::ipc_table::update_type::kDefault )
					{
						// �d�������C���͔�΂�
						if( AheIsOverlapableLayer( pRow->GetTableName() ) )
							continue;

						// �`��d���`�F�b�N�ibug 4963�j
						const_iterator it2 = it;
						++it2;
						for( ; it2 != itEnd; ++it2 )
						{
							const CRowBase* pRow2 = dynamic_cast<const CRowBase*>(&**it2);
							if( pRow2 )
							{
								emUpdateType = schema::ipc_table::update_type::kDefault;

								// �폜����Ă��Ȃ����̂ƃ`�F�b�N
								if ( ! pRow2->Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted )
								{
									const CGeometryBase* pGeom2 = pRow2->GetShape();
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
												if( cGeom.Overlaps( (IGeometry*)(*pGeom2) ) || 
													cGeom.Contains( (IGeometry*)(*pGeom2) ) ||
													cGeom.Within( (IGeometry*)(*pGeom2) ) ||
													cGeom.Equals( (IGeometry*)(*pGeom2) ) )
												{
													cErrObj->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryOverlapped, sindyErrLevel_ERR, (IGeometry*)(*pRow->GetShape()), -1, -1, DUPLI_OID, pRow->GetOID() ) ) );
												}
											}
										}
									}
								}
							}
						}
					}
					if( sindyErr_NoErr != cErrObj->GetDefaultErrCode() )
						cErrObjects.push_back( cErrObj );
				}
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// �R���t���N�g����̂��߂̊��ҏW�`�F�b�N���s��
sindyErrCode CContainerBase::CheckModify( CErrorObjectsBase& cErrObjects ) const
{
	// CRowBase���̏���
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow )
		pRow->CheckModify( cErrObjects );

	// �R���e�i���X�g�̏���
	for( const_iterator it = begin(); it != end(); ++it )
		(*it)->CheckModify( cErrObjects );

	return cErrObjects.GetDefaultErrCode();
}

// �����ɕύX�����������ǂ������`�F�b�N����
bool CContainerBase::Changed( schema::ipc_table::update_type::ECode* emUpdateType/* = NULL*/ ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		if( (*it)->Changed( emUpdateType ) )
			return true;
	}
	return false;
}

// �L���b�V������Ă��邩�ǂ������`�F�b�N����
bool CContainerBase::IsCached() const
{
	// CRowBase���̏���
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow )
	{
		if( ! pRow->IsCached() )
			return false;
	}

	// �R���e�i���X�g�̏���
	for( const_iterator it = begin(); it != end(); ++it )
	{
		// ��ł��L���b�V������Ă��Ȃ��Ȃ�false
		// �Ƃ���B�������邱�ƂŁACreateCache���Ă�
		// �ׂ̃g���K�ƂȂ�
		if( ! (*it)->IsCached() )
			return false; 
	}

	return true;
}

CContainerBase::iterator CContainerBase::erase( iterator it )
{
	return GetContainer().erase( it );
}

CContainerBase::iterator CContainerBase::erase( iterator first, iterator last )
{
	return GetContainer().erase( first, last );
}

/*void CContainerBase::erase( const CSPRowBase& cRow )
{
	iterator it = find( *cRow );
	while( it != end() )
		it = erase( it );
}*/

// ���݊i�[����Ă���t�B�[�`���̑��݋�`��Ԃ�
IEnvelopePtr CContainerBase::GetEnvelope( ShapeMode emMode/* = enumLatest*/, bool bChangedOnly/* = false*/, long lParam/* = -1*/ ) const
{
	IEnvelopePtr ipRet;

	// CRowBase���̏���
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow )
	{
		if( ( ! bChangedOnly ) || pRow->Changed() )
			ipRet = pRow->GetEnvelope( emMode );
	}

	// �R���e�i���X�g�̏���
	for( const_iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
		IEnvelopePtr ipEnv( (*it)->GetEnvelope( emMode, bChangedOnly, lParam ) );
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
	return ipRet;
}

// ���݊i�[����Ă���t�B�[�`���̌`����}�[�W�������̂�Ԃ�
IGeometryPtr CContainerBase::GetShapeCopy( ShapeMode emMode/* = enumLatest*/, bool bChangedOnly/* = false */, long lParam/* = -1*/ ) const
{
	_ISetPtr ipSet( CLSID_Set );

	GetShapeCopy( ipSet, emMode, bChangedOnly, lParam );
	return AheConstructUnion( ipSet );
}

void CContainerBase::GetShapeCopy( _ISetPtr& ipCol, ShapeMode emMode/* = enumLatest*/, bool bChangedOnly/* = false */, long lParam/* = -1*/ ) const
{
	// CRowBase���̏���
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow && ( (! bChangedOnly) || pRow->Changed() ) )
		ipCol->Add( pRow->GetShapeCopy( emMode ) );

	// �R���e�i���X�g�̏���
	for( const_iterator it = begin(), itEnd = end(); it != itEnd; ++it )
		(*it)->GetShapeCopy( ipCol, emMode, bChangedOnly, lParam );
}

sindyErrCode CContainerBase::CreateCache( bool bDetach/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ )
{
	sindyErrCode emErr = sindyErr_NoErr; // �߂�l

	foreach( CSPContainerBase& con, GetContainer() )
	{
		sindyErrCode emErrTmp = con->CreateCache( bDetach, spFields, spNameString );

		if( sindyErr_NoErr == emErrTmp )
			emErr = emErrTmp;
	}
	return errorcode::sindyErr_NoErr;
}

// �L���b�V���ȊO�̃p�����[�^���X�V����
sindyErrCode CContainerBase::UpdateParameter( bool bForce/* = false*/ )
{
	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
		errorcode::sindyErrCode emErr = (*it)->UpdateParameter( bForce );
		if( errorcode::sindyErr_NoErr != emErr )
			return emErr; // �r���ŏI���
	}
	return errorcode::sindyErr_NoErr;
}
void CContainerBase::DeleteParameter()
{
	for( iterator it = begin(), itEnd = end(); it != itEnd; ++it )
			(*it)->DeleteParameter();
}

void CContainerBase::Trace( bool bAttribute/* = true*/, bool bGeometry/* = true*/ ) const
{
#ifdef _DEBUG
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow )
	{
		ATLTRACE( _T("container: Table[%s] OID[%d]\n"), pRow->GetTableName(), pRow->GetOID() );
	}
#endif // ifdef _DEBUG

	for( const_iterator it = begin(), itEnd = end(); it != itEnd; ++it )
	{
#ifdef _DEBUG
		ATLTRACE(_T(" ") );
#endif // ifdef _DEBUG

		(*it)->Trace( bAttribute, bGeometry );
	}
}

/*// �e�[�u���^�C�v���擾����
sindyTableType::ECode CContainerBase::_GetTableType() const
{
	// CRowBase�̏ꍇ
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow )
		return pRow->GetTableType();

	// CTable�̏ꍇ
	const CTable* pTable = dynamic_cast<const CTable*>(this);
	if( pTable )
		return pTable->GetTableType();

	// �����̃R���e�i�̏ꍇ
	return sindyTableType::unknown;
}

// �e�[�u�������擾����
LPCTSTR CContainerBase::_GetTableName() const
{
	// CRowBase�̏ꍇ
	const CRowBase* pRow = dynamic_cast<const CRowBase*>(this);
	if( pRow )
		return pRow->GetTableName();

	// CTable�̏ꍇ
	const CTable* pTable = dynamic_cast<const CTable*>(this);
	if( pTable )
		return pTable->GetTableName();

	// �����̃R���e�i�̏ꍇ
	return NULL;
}*/
