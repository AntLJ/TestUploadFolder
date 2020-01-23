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

/**
 * @file Row.cpp
 * @brief CRow�N���X�����t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include "Rule.h"
#include "Row.h"
#include "Table.h"
#include "sindycomutil.h"
#include "TableTypeUtil.h"
#include "GlobalRules.h"
#include "GlobalFunctions.h"
#include "ErrorObject.h"
#include "meshutil.h"
#include "GeometryRule.h"
#include "CacheAttribute.h"
#include "AheNetwork.h"
#include "RuleModel.h"
#include "AheLayerFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace winutil;
using namespace sindy::schema::category;
namespace sindy {
using namespace errorcode;

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR EXIST_WHEN_MIGRATING         = _T("�ڍs������");
	LPCTSTR ATTRIBUTE_TOUNSUPPORTED_TYPE = _T("%s.%s : �T�|�[�g����Ă��Ȃ��^�ɑ΂��Ď��������t�^���悤�Ƃ��܂����B");
	LPCTSTR NONFEATURE_HAVE_SHAPE_CACHE  = _T("HRESULT[%d:%X] �t�B�[�`���ł͂Ȃ����̂��`��L���b�V���������Ă��܂�");
	LPCTSTR FAILED_TO_ADD_HISTORY        = _T("����t�^�Ɏ��s���܂���");
	LPCTSTR CREATE_CACHE                 = _T("�L���b�V�����쐬���܂�\n");
	LPCTSTR THERE_MAY_BE_NULL_SHAPE      = _T("NULL Shape �̉\��������܂�");
	LPCTSTR NOT_NULL_FIELD               = _T("[%s:NULL]:NOT NULL�t�B�[���h");
	LPCTSTR BYTES_OVER                   = _T("[%s:%s]:%d�o�C�g�I�[�o�[");
	LPCTSTR ALLOWABLE_NUM_OF_DIGITS      = _T("[%s:%s]:���e�����F%d");
	LPCTSTR ALLOWABLE_RANGE              = _T("[%s:%s]:���e�͈́F%s�`%s");
	LPCTSTR FIELDS_NOT_INITIALIZE        = _T("CRow���L���b�V������Ă���̂�m_spFields������������Ă��܂���B");
	LPCTSTR NAMESTRING_NOT_INITIALIZE    = _T("CRow���L���b�V������Ă���̂�m_spNameString������������Ă��܂���B");
#else
	LPCTSTR EXIST_WHEN_MIGRATING         = _T("Exist when migrating");
	LPCTSTR ATTRIBUTE_TOUNSUPPORTED_TYPE = _T("%s.%s: An attempt was made to automatically add attributes to an unsupported type.");
	LPCTSTR NONFEATURE_HAVE_SHAPE_CACHE  = _T("HRESULT[%d:%X] Non-feature is having the shape cache");
	LPCTSTR FAILED_TO_ADD_HISTORY        = _T("Failed to add history.");
	LPCTSTR CREATE_CACHE                 = _T("Create cache.\n");
	LPCTSTR THERE_MAY_BE_NULL_SHAPE      = _T("There may be a NULL shape");
	LPCTSTR NOT_NULL_FIELD               = _T("[%s:NULL]: NOT NULL field");
	LPCTSTR BYTES_OVER                   = _T("[%s:%s]: %d bytes over");
	LPCTSTR ALLOWABLE_NUM_OF_DIGITS      = _T("[%s:%s]: Allowable number of digits: %d");
	LPCTSTR ALLOWABLE_RANGE              = _T("[%s:%s]: Allowable range: %s to %s");
	LPCTSTR FIELDS_NOT_INITIALIZE        = _T("CRow is having cache, but m_spFields is not initialized.");
	LPCTSTR NAMESTRING_NOT_INITIALIZE    = _T("CRow is having cache, but m_spNameString is not initialized.");
#endif // SINDY_FOR_ENGLISH
}

// ���Ԍv���p
double g_dTotalCacheTime = 0.0;	//!< CRow::CreateCache�ɂ����鎞�Ԃ̑��v

CRow::CRow() : 
  m_emTableType(sindyTableType::unknown),
  m_pCache(NULL),
  m_pGeom(NULL),
  m_lOID(-1),
  m_bCreated(false),
  m_bDeleted(false)

{
	m_spNameString.reset((CTableNameString*)NULL);
	m_spFields.reset((CFieldMap*)NULL);
}

CRow::CRow( const CRow& obj ) :
	m_emTableType(obj.m_emTableType),
	m_spNameString(obj.m_spNameString),
	m_spFields(obj.m_spFields),
	m_lOID(obj.m_lOID),
	m_bCreated(obj.m_bCreated),
	m_bDeleted(obj.m_bDeleted)
{
	m_pCache = ( obj.m_pCache ) ? new CCacheAttribute(*obj.m_pCache) : NULL;
	m_pGeom = ( obj.m_pGeom ) ? new CGeometry(*obj.m_pGeom) : NULL;
	AtlComPtrAssign( (IUnknown**)&p, (_IRow*)obj ); // Release���ĂԂƃL���b�V����m_lOID�������������̂�NG
}

CRow::CRow( _IRow* lp, sindyTableType::ECode emTableType/* = sindyTableType::unknown*/, bool bCreated/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ ) :
	m_emTableType(emTableType),
	m_pCache(NULL),
	m_pGeom(NULL),
	m_lOID(-1),
	m_bCreated(bCreated),
	m_bDeleted(false),
	m_spFields(spFields),
	m_spNameString(spNameString)
{
	SetObject( lp, emTableType, bCreated, spFields, spNameString ); \
}

CRow& CRow::operator=( const CRow& obj )
{
	AtlComPtrAssign( (IUnknown**)Release(), (_IRow*)obj ); // Release���ĂԂƃL���b�V����m_lOID�������������̂Ő�ɂ��

	m_emTableType = obj.m_emTableType;
	m_spNameString = obj.m_spNameString;
	m_spFields = obj.m_spFields;
	if( m_pCache )
		delete m_pCache;
	m_pCache = ( obj.m_pCache ) ? new CCacheAttribute(*obj.m_pCache) : NULL;
	if( m_pGeom )
		delete m_pGeom;
	m_pGeom = ( obj.m_pGeom ) ? new CGeometry(*obj.m_pGeom) : NULL;
	m_lOID = obj.m_lOID;
	m_bCreated = obj.m_bCreated;
	m_bDeleted = obj.m_bDeleted;

	return *this;
}

CRow& CRow::operator=( _IRow* lp )
{
	m_emTableType = sindyTableType::unknown;
	m_spNameString.reset((CTableNameString*)NULL);
	m_spFields.reset((CFieldMap*)NULL);
	if( m_pCache )
		delete m_pCache;
	m_pCache = NULL;
	if( m_pGeom )
		delete m_pGeom;
	m_pGeom = NULL;
	m_lOID = -1;
	m_bCreated = false;
	m_bDeleted = false;
	AtlComPtrAssign( (IUnknown**)Release(), lp );

	return *this;
}

bool CRow::operator <( const CRow& obj )
{
	if( p && obj.p )
		return ( GetOID() < obj.GetOID() && *GetNameString() < *obj.GetNameString() );
	return false;
}

// �t�B�[�`���E���R�[�h���Z�b�g����
void CRow::SetObject( _IRow* lp, sindyTableType::ECode emTableType/* = sindyTableType::unknown*/, bool bCreated/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNames/* = CSPTableNameString()*/ )
{
	// �����̂��͎̂̂Ă�
	DeleteCache();
	// �����o�ϐ�������
	Init();

	// attach
	AtlComPtrAssign((IUnknown**)&p, lp);
	m_emTableType = emTableType;
	m_bCreated = bCreated;	// �쐬�t���O
	m_spFields = spFields;
	m_spNameString = spNames;
}

// �_�~�[CRow�����̉�������
void CRow::Materialization( _IRow* lp )
{
	_ASSERTE( ! p );        // ���̂�������̂ɑ΂��ď������悤�Ƃ��Ă���
	_ASSERTE( m_bCreated ); // �V�K�쐬�t���O���Ȃ����̂ɑ΂��ď������悤�Ƃ��Ă���

	AtlComPtrAssign((IUnknown**)&p, lp);
}

errorcode::sindyErrCode CRow::Materialization( ITable* ipTable )
{
	errorcode::sindyErrCode emErr = errorcode::sindyErr_NoErr; // �߂�l

	if ( !m_bDeleted ) {
		if( ! p && ipTable )
		{
			// ���̉�
			HRESULT hr = S_OK;
			if( SUCCEEDED( hr = ipTable->CreateRow( &p ) ) )
			{
			}
			else
				emErr = sindyErr_AOCreateRowFailed;
		}
		else
			emErr = sindyErr_AOCreateRowFailed;
	}
	else
		emErr = sindyErr_AOCreateRowFailed;

	return emErr;
}

errorcode::sindyErrCode CRow::Materialization( ITable* ipTable, ITable* ipSubTable )
{
	errorcode::sindyErrCode emErr = errorcode::sindyErr_NoErr; // �߂�l

	if ( errorcode::sindyErr_NoErr == (emErr = Materialization( ipTable )) ) {
		for ( CContainer::rows_iterator it = begin(CTable(ipSubTable).GetTableType()), itEnd = end(CTable(ipSubTable).GetTableType()); it != itEnd; ++it )
			emErr = (*it)->Materialization( ipSubTable );
	}

	return emErr;
}

// �t�B�[�`���̃R�s�[���쐬����
CRow CRow::Clone() const
{
	CRow cRow;	// �Ԃ�l

	LOGASSERTE_IF( p, sindyErr_ClassMemberMissing )
	{
		// �ҏW�J�n����Ă��邩�ǂ���
		IWorkspaceEditPtr ipWork( GetWorkspace() );
		VARIANT_BOOL vbIsBeingEdited = VARIANT_FALSE;
		ipWork->IsBeingEdited( &vbIsBeingEdited );
		LOGASSERTE_IF( vbIsBeingEdited, sindyErr_EditNotStarted )
		{
			// �쐬
			ITablePtr ipTable( GetTable() );
			LOGASSERTE_IF( SUCCEEDED( ipTable->CreateRow( cRow.Release() ) ), sindyErr_RowCreateFailed )
			{
				cRow.m_bCreated = true;	// �쐬�t���O�𗧂ĂĂ����i�����������ׁ݂̈j

				// �e�[�u�����L���b�V���A�t�B�[���h���Ή��\�R�s�[
				cRow.m_emTableType = m_emTableType;
				cRow.m_spFields = m_spFields;
				cRow.m_spNameString = m_spNameString;
				// �����E�`����R�s�[
				cRow.m_pCache = new CCacheAttribute( ( m_pCache ) ? *m_pCache : (_IRow*)*this );
				cRow.m_pGeom = new CGeometry( GetShapeCopy() );
			}
		}
	}
	return cRow;
}

// �t�B�[�`���̕ύX�����ɖ߂�
void CRow::Reset( bool bRecursive/* = false*/ )
{
	// CContainer
	if( bRecursive )
		CContainer::Reset( bRecursive );

	// �폜�t���O���N���A����Ȃ�����
	// �Ep���폜�ς݂̏ꍇ
	// �E�폜�t���O�������Ă���Ap�����݂��Ȃ�
	//
	// ��L�ȊO�͍ŏI�I�ɍ쐬�E�폜�t���O������������
	// 
	// DummyRow�ɂ��Ȃ���΂Ȃ�Ȃ�����
	// �E�폜�t���O�������Ă��炸�Ap�����݂��Ȃ�

	// p���폜�ς݂��ǂ�����OJBECTID������āA�G���[���o�邩�ǂ����Ŋm�F
	bool bAlreadyDeleted = false;
	if( p )
	{
		CComVariant vaValue;
		if( FAILED( p->get_Value( 1, &vaValue ) ) )
			bAlreadyDeleted = true;
	}

	// �폜�t���O���N���A����Ȃ������̏ꍇ�͂��̂܂܂ɂ���
	// �������AbAlreadyDeleted�̂ݗ����Ă���\�����l������
	// m_bDeleted�ɔ��f���Ă���
	if( bAlreadyDeleted || ( m_bDeleted && ( ! p ) ) )
	{
		m_bDeleted = true;
		return;
	}

	DeleteCache(); // �L���b�V���폜

	// �쐬�t���O�������Ă���Ap�����݂���ꍇ��
	// CreateDummyRow�ł���Ă��鑮���Z�b�g���s��
	// p�����݂��Ȃ��ꍇ�͑��݂��Ȃ��������Ƃɂ��Ȃ����
	// �Ȃ�Ȃ��̂ŁA�폜�t���O�𗧂ĂĂ���
	if( m_bCreated )
	{
		if( p )
		{
			// CTable::CreateDummyRow������
			// �������s��
			CreateCache(); // �L���b�V���쐬
			SetDefaultValue(); // �f�t�H���g�l�Z�b�g
			SetCreateAutoValue(); // �����t�^�����Z�b�g
			m_bCreated = true;
		}
		else
		{
			m_bDeleted = true;
			return;
		}
	}
	else
		m_bCreated = false;

	m_bDeleted = false;
}

sindyTableType::ECode CRow::GetTableType() const
{
	if( sindyTableType::unknown == m_emTableType )
		m_emTableType = AheTableName2TableType( GetNameString()->GetTableName() );

	return m_emTableType;
}

void CRow::Delete( bool bRecursive/* = false*/ )
{
	m_bDeleted = true;	// �폜�ς݃t���O�t�^

	if( bRecursive )
	{
		// ����[���̂��͍̂폜����
		for( CRuleRelationshipContain::const_value_iterator itContain = g_cRuleRelationshipContain.begin(GetTableName()),
			itContainEnd = g_cRuleRelationshipContain.end(GetTableName());
			itContain != itContainEnd; ++itContain )
			CContainer::Delete( itContain->first, bRecursive );
		// �e�[�u���^�C�v��other�̂��͖̂ⓚ���p�ō폜�iSiNDY-i�ł͑S��other�œ����Ă���̂Łc�j
		for( rows_iterator itRows = begin(sindyTableType::other), itRowsEnd = end(sindyTableType::other); itRows != itRowsEnd; ++itRows )
			(*itRows)->Delete( bRecursive );
	}
}

IFieldsPtr CRow::GetFields() const
{
	IFieldsPtr ipFields;
	INVOKEMETHOD( get_Fields( &ipFields ) );
	return ipFields;
}

bool CRow::HasOID() const
{
	// m_lOID��-1�̏ꍇ�͖��擾�Ȃ̂ŁA���ۂɒ��ׂ�
	if( -1 == m_lOID )
	{
		VARIANT_BOOL vbHasOID = VARIANT_FALSE;
		INVOKEMETHOD( get_HasOID( &vbHasOID ) );
		return VB2bool(vbHasOID);
	}
	else
		return ( m_lOID < 0 ) ? false : true;
}

long CRow::GetOID() const
{
	// ���Ɏ擾�����`�Ղ�����ꍇ�̓L���b�V�����g�p
	if( p )
	{
		if( -1 == m_lOID )
		{
			if( ! HasOID() )
				m_lOID = -2; // OBJECTID�������e�[�u�����T�|�[�g
			else {
				INVOKEMETHOD( get_OID( const_cast<long*>(&m_lOID) ) );
			}
		}
	}
	return m_lOID;
}

void CRow::SetDefaultValue()
{
	CSPFieldMap cspFields( GetFieldMap() );
	if( cspFields )
	{
		CFieldMap& cFields( *cspFields.get() );
		long lFieldCount = cFields.GetFieldCount();
		for( long i = 0; i < lFieldCount; ++i )
		{
			if( cFields.IsEditable( i ) )
			{
				const VARIANT& va( cFields.GetDefaultValue(i) );
				if( ! ( VT_NULL == va.vt && ( ! cFields.IsNullable( i ) ) ) ) // NOT NULL�Ȃ̂Ƀf�t�H���g�l��NULL�̏ꍇ�̓G���[�ɂȂ�̂ŉ������Ȃ�
					SetValue( i, cFields.GetDefaultValue( i ) );
				else
				{
					// �I�y���[�^���ƍ�ƖړIID��NULL�̂܂܂��ƁA�V�K�쐬����CheckLogic()��NOT NULL�`�F�b�N�ŃG���[�ɂȂ�̂ŉ���B
					CComVariant vaValue;

					GetValue( i, vaValue );

					if ( VT_NULL == vaValue.vt )
					{
						if ( i == cFields.HasField( schema::ipc_table::kOperator ) )
							SetValue( i, CComVariant(CComBSTR(EXIST_WHEN_MIGRATING)) );
						else if ( i == cFields.HasField( schema::ipc_table::kPurpose ) )
							SetValue( i, CComVariant(0) );
					}
				}
			}
		}
	}
}

// �V�K�쐬���̎����t�^������t�^����
void CRow::SetCreateAutoValue()
{
	// ���O�C�����Ă���ꍇ�ɂ͐V�K�쐬���̎����t�^����
	if( g_cRule.IsLogined() )
	{
		const creatablelayers::CCreatableLayers* pCreLayers( g_cRule.GetCreatableLayersTable().Find( g_cRule.GetWorkPurposeID(), g_cRule.GetLayerNamesTable().Find( GetTableName() ) ) );
		if( pCreLayers )
		{
			const CAutoValue& cValue( pCreLayers->GetAutoValue() );
			CSPFieldMap cFields( GetFieldMap() );
			for( CAutoValue::const_iterator it = cValue.begin(); it != cValue.end(); ++it )
			{
				long lFieldIndex = cFields->FindField( it->first );
				if( 0 < lFieldIndex )
				{
					switch( cFields->GetFieldType( lFieldIndex ) )
					{
						case esriFieldTypeInteger:
						case esriFieldTypeSmallInteger: SetLongValue( lFieldIndex, _ttol(it->second) ); break;
						case esriFieldTypeString: SetStringValue( lFieldIndex, it->second ); break;
						default:
							_ERRORLOG( sindyErr_AlgorithmFailed, ATTRIBUTE_TOUNSUPPORTED_TYPE, GetTableName(), it->first );

							break;
					}
				}
			}
		}
	}
}

// �L���b�V���𔽉f������
void CRow::UpdateCache( schema::ipc_table::update_type::ECode& emUpdate, CErrorObjects& cErrs, CErrorObject* pErr )
{
	if( ! p )
		return;

	HRESULT hr = S_OK;

	// ����
	// �L���b�V��������ꍇ�̓L���b�V�����ɔ��f������
	if( m_pCache )
	{
		// �L���b�V�����f
		CFieldMap& cFields( *GetFieldMap() );
		long lFieldCount = m_pCache->GetCount();
		for( long i = 0; i < lFieldCount; ++i )
		{
			// �ҏW�ł��Ȃ��t�B�[���h�͔�΂�
			if( cFields.IsEditable( i ) )
			{
				VARIANT& va = *(*m_pCache)[i];
				// EMPTY�̏ꍇ�͕ҏW�s�̃t�B�[���h���A���̓N�G���t�B���^�ŏȂ��ꂽ����
				if( va.vt != VT_EMPTY )
				{
					// �ύX������ꍇ�̂�Store����
					_variant_t vaValue;
					if( SUCCEEDED( hr = p->get_Value( i, &vaValue ) ) )
					{
						if( vaValue != va )
						{
							if( SUCCEEDED( hr = p->put_Value( i, va ) ) )
								emUpdate = schema::ipc_table::update_type::kAttrUpdated;
							else {
								CComVariant vaErr( va );
								vaErr.ChangeType(VT_BSTR);
								if( VT_NULL == vaErr.vt )
									vaErr = _T("NULL");
								if( ! pErr )
									pErr = &cErrs.push_back( CErrorObject( *this ) );
								pErr->push_back( CErrorInfo( sindyErr_AOPutValueFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X] FIELD[%s] VALUE[%s]"), hr, hr, cFields.GetName(i), (LPCTSTR)CString(vaErr.bstrVal) ) );
								Trace( true, false );
							}
						}
					}
					else {
						if( ! pErr )
							pErr = &cErrs.push_back( CErrorObject( *this ) );
						pErr->push_back( CErrorInfo( sindyErr_AOGetValueFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) );
					}
				}
			}
		}
	}
	// �`��
	if( m_pGeom )
	{
		IFeaturePtr ipFeat;
		if( SUCCEEDED( hr = p->QueryInterface( IID_IFeature, (void**)&ipFeat ) ) )
		{
			IGeometryPtr ipGeom;
			if( SUCCEEDED( hr = ipFeat->get_Shape( &ipGeom ) ) )
			{
				if( ! m_pGeom->EqualsVertex( ipGeom ) )
				{
					if( SUCCEEDED( hr = ipFeat->putref_Shape( (IGeometry*)*m_pGeom ) ) )
						emUpdate = schema::ipc_table::update_type::kShapeUpdated;
					else {
						if( ! pErr )
							pErr = &cErrs.push_back( CErrorObject( *this ) );
						pErr->push_back( CErrorInfo( sindyErr_AOPutValueFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) );
						AheTraceGeometry( (IGeometry*)*m_pGeom, _T("putref_Shape���s") );
					}
				}
			}
			else {
				if( ! pErr )
					pErr = &cErrs.push_back( CErrorObject( *this ) );
				pErr->push_back( CErrorInfo( sindyErr_GeometryNotFound, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) );
			}
		}
		else {
			if( ! pErr )
				pErr = &cErrs.push_back( CErrorObject( *this ) );
			pErr->push_back( CErrorInfo( sindyErr_AlgorithmFailed, sindyErrLevel_ERR, NULL, 0, -1, NONFEATURE_HAVE_SHAPE_CACHE, hr, hr ) );
		}
	}
}

sindyErrCode CRow::Store( CErrorObjects& cErrs )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	if ( ! p /*&& m_bDeleted*/ )	// ���̂��Ȃ��č폜�������̂̓X���[�B
		return cErrs.GetDefaultErrCode();

	CErrorObject* pErr = NULL;
	if( g_bCreateAllStoreInfo ) // �C���|�[�g���ɂ͑S�Ẵ��R�[�h�����G���[�I�u�W�F�N�g�ɓo�^����K�v�͂Ȃ��i���Ȃ莞�Ԃ�������c�j
	{
		// �G���[�I�u�W�F�N�g�Ɏ������g��o�^
		if ( ! p ) {	// ���̂��Ȃ��ꍇ
			CErrorObject cErrObj;

			cErrObj.SetErrorObject( boost::tuple<CString, long, CGeometry>(GetTableName(), -1, NULL) ); 
			pErr = &cErrs.push_back( cErrObj );
		} else
			pErr = &cErrs.push_back( CErrorObject( *this ) );
	}

	// �֘A�I�u�W�F�N�g�̕ۑ�
	if( g_bStoreContainer )
		CContainer::Store( cErrs );

	LOGASSERTEERR_IF( p, sindyErr_ClassMemberMissing )
	{
		// �l�b�g���[�N�����N���폜����Ȃ���
		if ( AheIsNetworkLink( GetTableName() ) && ! m_bDeleted ) {
			bool bHasDeleted = false;

			// �֘A�I�u�W�F�N�g�ɍ폜�������̂����邩�`�F�b�N
			for( CContainer::const_iterator it = begin(); it != end(); ++it )
			{
				for( CContainer::const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
				{
					schema::ipc_table::update_type::ECode emUpdate = schema::ipc_table::update_type::kDefault;

					if ( (*it2)->Changed( &emUpdate ) && emUpdate == schema::ipc_table::update_type::kDeleted ) {
						bHasDeleted = true;
						break;
					}
				}

				if ( bHasDeleted )
					break;
			}

			// �폜�������̂�����΁A������t�^(�N���[���t���O��t���邽��)
			if ( bHasDeleted )
				SetHistory( schema::ipc_table::update_type::kAttrUpdated );
		}

		schema::ipc_table::update_type::ECode emUpdate = schema::ipc_table::update_type::kDefault;

		// �폜�̏ꍇ
		HRESULT hr = S_OK;
		if( m_bDeleted )
		{
#ifdef _DEBUG
			long lOID = GetOID(); // �폜�����Ƃł��Ȃ��Ȃ�̂�m_lOID�ɃL���b�V�����Ă����im_lOID��DeleteCache�ł͏����Ȃ��j
#else
			GetOID(); // �폜�����Ƃł��Ȃ��Ȃ�̂�m_lOID�ɃL���b�V�����Ă����im_lOID��DeleteCache�ł͏����Ȃ��j
#endif // ifdef _DEBUG
			// �폜
			// �ۑ�
			// �V�K�쐬�������̂�Store()�����ɁADelete����Ƃ��߂ۂȂ̂�Store()���Ƃ�
			// �������A���̏ꍇ�͐�ɃL���b�V���𔽉f�����Ă����Ȃ���NULL�`��t�B�[�`���쐬
			// ���[�_�[�ɔ�������̂ŁA�L���b�V���𔽉f�ibug 9451�j
			if ( m_bCreated )
			{
				UpdateCache( emUpdate, cErrs, pErr ); // ��ɃL���b�V�����f�����Ă���
				if( SUCCEEDED( hr = p->Store() ), sindyErr_AOStoreFailed )
				{
#ifdef _DEBUG
					CString cStr;
					switch( emUpdate )
					{
						case schema::ipc_table::update_type::kShapeUpdated: cStr = _T("�`��ύX"); break;
						case schema::ipc_table::update_type::kAttrUpdated:  cStr = _T("�����ύX"); break;
						case schema::ipc_table::update_type::kCreated:      cStr = _T("�V�K�쐬"); break;
						case schema::ipc_table::update_type::kDeleted:      cStr = _T("�폜"); break;
						default: cStr = _T("�ڍs�������H"); break;
					}
					//TRACEMESSAGE( _T("%s : TABLE[%s] : OID[%d] : �ۑ������i%s�j\n"), CA2CT(__func__), GetTableName(), GetOID(), cStr );
					ATLTRACE( _T("%s : TABLE[%s] : OID[%d] : �ۑ������i%s�j\n"), __func__, GetTableName(), GetOID(), cStr );
#endif // ifdef _DEBUG
				}
				else {
					if( ! pErr )
						pErr = &cErrs.push_back( CErrorObject( *this ) );
					pErr->push_back( CErrorInfo( sindyErr_AOStoreFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) );
				}
			}

			// �L���b�V���폜
			DeleteCache();

			// [FYI] IsKnownSimple��FALSE�̎��́A�����I��Simplify�������ۂ��i����\���_�������Ȃ�݂����j���ƂŒ��ׂ邱�ƁiStore������ēxGetFeature�Ŏ擾�������Ȃ���DB���ɂ���`��ɂȂ�Ȃ���j
			if( SUCCEEDED( hr = p->Delete() ) )
			{
#ifdef _DEBUG
				ATLTRACE( _T("%s : TABLE[%s] : OID[%d] : �폜�����i%s�j\n"), __func__, GetTableName(), m_lOID, _T("�폜") );
#endif // ifdef _DEBUG
			}
			else {
				if( ! pErr )
					pErr = &cErrs.push_back( CErrorObject( *this ) );
				pErr->push_back( CErrorInfo( sindyErr_AODeleteFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) );
			}
		}
		// �폜�ȊO�̏ꍇ
		else if( m_pCache || m_pGeom )
			UpdateCache( emUpdate, cErrs, pErr );

		if( m_bCreated ) emUpdate = schema::ipc_table::update_type::kCreated;
		if( m_bDeleted ) emUpdate = schema::ipc_table::update_type::kDeleted;	// �폜��D��

		// �L���b�V���폜
		DeleteCache();

		// �V�K�쐬/�폜�ȊO�̓L���b�V���쐬�O�̕ύX�m�F�i�L���b�V�����������ꍇ�͓�x��ԂɂȂ邯�ǂ��傤���Ȃ��c�j
		if( ! ( schema::ipc_table::update_type::kCreated == emUpdate ||
			    schema::ipc_table::update_type::kDeleted == emUpdate ) )
		{
			// ���\���Ԃ������鏈���̂悤�Ȃ̂ŁA�O���t���O�Ő���
			if( g_bCheckUpdatedByArcObjects ) // �L���b�V���쐬�O�̕ύX���`�F�b�N���邩�ǂ���
				Changed( &emUpdate );
			else
				emUpdate = schema::ipc_table::update_type::kAttrUpdated; // �����I��Store������i�����ɂ͑����ҏW�����͂���Ȃ��̂ŁASiNDY-e�n�ł͍��������p�O���t���O�͎g�p�֎~�I�I�j
		}

		if( sindyErr_NoErr == emErr )
		{
			if( schema::ipc_table::update_type::kDefault != emUpdate )
			{
				// �폜�͏��O���Ȃ��Ƃ܂����B
				if( schema::ipc_table::update_type::kDeleted != emUpdate )
				{
					bool bStore = false;

					// �����t�B�[���h���Ȃ�SiNDY�I�u�W�F�N�g�����݂���iSiNDY-i�Ƃ��j
					// �����ɂƂ��Ă͖��ʂȏ����ɂ����Ȃ�Ȃ��̂ŁA�O���t���O�Ő���
					// �ł���悤�ɂ���
					if( g_bSetHistory )
					{
						// LQ�����O���Ȃ��Ƃ܂����B
						if ( !AheIsLQTableType( GetTableType() ) ) {
							// ������t�^
							if( sindyErr_NoErr == ( emErr = SetHistory( emUpdate ) ) )
								bStore = true;
							else 
							{
								if( ! pErr )
									pErr = &cErrs.push_back( CErrorObject( *this ) );
								pErr->push_back( CErrorInfo( emErr, sindyErrLevel_ERR, NULL, 0, -1, FAILED_TO_ADD_HISTORY ) );
								bStore = false;
							}
						}
						else
							bStore = true;
					}
					else
						bStore = true;

					if ( bStore ) {
						// �ۑ�
						// [FYI] IsKnownSimple��FALSE�̎��́A�����I��Simplify�������ۂ��i����\���_�������Ȃ�݂����j���ƂŒ��ׂ邱�ƁiStore������ēxGetFeature�Ŏ擾�������Ȃ���DB���ɂ���`��ɂȂ�Ȃ���j
						static DWORD dwStoreStart = 0;
						if( g_bCalcStoreTime )
							dwStoreStart = GetTickCount();
						if( SUCCEEDED( hr = p->Store() ) )
						{
#ifdef _DEBUG
							if( g_bTraceUpdate )
							{
								CString cStr;
								switch( emUpdate )
								{
									case schema::ipc_table::update_type::kShapeUpdated: cStr = _T("�`��ύX"); break;
									case schema::ipc_table::update_type::kAttrUpdated:  cStr = _T("�����ύX"); break;
									case schema::ipc_table::update_type::kCreated:      cStr = _T("�V�K�쐬"); break;
									case schema::ipc_table::update_type::kDeleted:      cStr = _T("�폜"); break;
									default: cStr = _T("�ڍs�������H"); break;
								}
								//TRACEMESSAGE( _T("%s : TABLE[%s] : OID[%d] : �ۑ������i%s�j\n"), CA2CT(__func__), GetTableName(), GetOID(), cStr );
								ATLTRACE( _T("%s : TABLE[%s] : OID[%d] : �ۑ������i%s�j\n"), __func__, GetTableName(), GetOID(), cStr );
							}
#endif // ifdef _DEBUG
						}
						else {
							if( ! pErr )
								pErr = &cErrs.push_back( CErrorObject( *this ) );
							pErr->push_back( CErrorInfo( sindyErr_AOStoreFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X] %s"), hr, hr, GETCOMERRORSTRING() ) );
							Trace();
						}
						if( g_bCalcStoreTime )
						{
							g_dwTotalStoreTime += ( GetTickCount() - dwStoreStart );
							++g_lTotalStoreCount;
						}
					}
				}
#ifdef _DEBUG
				else if( g_bTraceUpdate )
					ATLTRACE( _T("%s : TABLE[%s] : OID[%d] : �폜\n"), __func__, GetTableName(), GetOID() );
#endif // ifdef _DEBUG
			}
#ifdef _DEBUG
			else if( g_bTraceUpdate )
				ATLTRACE( _T("%s : TABLE[%s] : OID[%d] : ���ۑ��i�ύX�����j\n"), __func__, GetTableName(), GetOID() );
#endif // ifdef _DEBUG
		}
	} 

	// �Ō��Cerate�t���O��false�ɂ��Ă���
	m_bCreated = false;

	return cErrs.GetDefaultErrCode();
}

// �R���t���N�g����̂��߂̊��ҏW�`�F�b�N���s��
sindyErrCode CRow::CheckModify( CErrorObjects& cErrs ) const
{
	if ( (_IRow*)p ) {	// ���̂�������̂̂�
		if ( Changed() ) {
			// �������g
			TCHAR strMsg[2048];
			if( g_cRule.IsModified( *this, 2048, strMsg ) )
			{
				CErrorObject cErr( *this );
				cErr.push_back( CErrorInfo( sindyErr_AlreadyModified, sindyErrLevel_ERR, NULL, -1, -1, _T("%s"), strMsg ) );
				cErrs.push_back( cErr );
			}
		}
		// �֘A�I�u�W�F�N�g
		CContainer::CheckModify( cErrs );
	}

	return cErrs.GetDefaultErrCode();
}

// SiNDY-c�p�X�V�^�C�v�ϊ��֐��ibug 5201�j
long update_type2update_code( schema::ipc_table::update_type::ECode emCode )
{
	using namespace schema::ipc_table::update_type;

	long lRet = -1; // �Ԃ�l
	switch( emCode )
	{
		case kDefault:		lRet = 0; break; // �ڍs������ -> �C���Ȃ�/������
		case kCreated:		lRet = 3; break; // �V�K�쐬   -> �V�K����/�`��A�������C��
		case kShapeUpdated:	lRet = 1; break; // �`��ύX   -> �`����C��
		case kAttrUpdated:	lRet = 2; break; // �����ύX   -> �������C��
		case kDeleted:       // �폜
		case kiPSImported:   // ��C�C���|�[�g
		case kImported:      // �C���|�[�g
		default:			lRet = -1; break; // �Ή�������̂��Ȃ����A�܂��������c
	}
	return lRet;
}

sindyErrCode CRow::SetHistory( schema::ipc_table::update_type::ECode emCode )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	LOGASSERTEERR_IF( p, sindyErr_ClassMemberMissing )
	{
		const CFieldMap& cFields( *GetFieldMap() );
		long lUpdateTypeIndex =cFields.HasField( schema::ipc_table::kUpdateType );
		if( 0 < lUpdateTypeIndex ) SetLongValue( lUpdateTypeIndex, emCode );					// �X�V�^�C�v
		if( g_cRule.IsLogined() )
		{
			long lOperatorIndex = cFields.HasField( schema::ipc_table::kOperator );
			long lModifyDateIndex = cFields.HasField( schema::ipc_table::kModifyDate );
			long lPurposeIndex = cFields.HasField( schema::ipc_table::kPurpose );
			if( 0 < lOperatorIndex ) SetStringValue( lOperatorIndex, g_cRule.GetOperatorName() );	// �I�y���[�^��
			if( 0 < lModifyDateIndex ) SetStringValue( lModifyDateIndex, GetTime() );				// �ŏI�X�V����
			if( 0 < lPurposeIndex ) SetLongValue( lPurposeIndex, g_cRule.GetWorkPurposeID() );		// ��ƖړIID
			// �����t�^����
			const CAutoValue& cAutoValue( g_cRule.GetAutoValue() );
			for( CAutoValue::const_iterator it = cAutoValue.begin(); it != cAutoValue.end(); ++it )
			{
				long lFieldIndex = cFields.HasField( it->first );
				if( 0 < lFieldIndex ) 
				{
					switch( cFields.GetFieldType(lFieldIndex) )
					{
					case esriFieldTypeInteger:
					case esriFieldTypeSmallInteger: SetLongValue( lFieldIndex, _ttol(it->second) ); break;
					case esriFieldTypeString: SetStringValue( lFieldIndex, it->second ); break;
					default:
						_ERRORLOG( emErr = sindyErr_AlgorithmFailed, ATTRIBUTE_TOUNSUPPORTED_TYPE, GetTableName(), it->first );
						break;
					}
				}
			}
			// PGDB�ҏW���[�h�̏ꍇ�̍X�V�^�C�v
//			if( AheIsPGDBEditMode() ) // ���[�h�ɂ�炸����΍X�V�ibug 5777�j
			long lUpdateCIndex = cFields.HasField( sindyc_table::kUpdateC );
			if( 0 < lUpdateCIndex )
				SetLongValue( lUpdateCIndex, update_type2update_code( emCode ) | GetLongValue( lUpdateCIndex, 0 ) ); // �X�V�^�C�v�iPGDB�ҏW���[�h�p�j�ibug 5201�j
		}
		else {
			long lModifyProgNameIndex = cFields.HasField( schema::ipc_table::kModifyProgName );
			long lProgModifyDateIndex = cFields.HasField( schema::ipc_table::kProgModifyDate );
			if( 0 < lModifyProgNameIndex )
			{
				TCHAR szBuffer[_MAX_PATH];
				::GetModuleFileName(NULL, szBuffer, _MAX_PATH);
				LPCTSTR pos = ::_tcsrchr(szBuffer, _T('\\'));
				SetStringValue( lModifyProgNameIndex,  pos ? pos + 1 : szBuffer );	// �X�V�v���O������
			}
			if( 0 < lProgModifyDateIndex ) SetStringValue( lProgModifyDateIndex, GetTime() );				// �o�b�`�����ł̍ŏI�X�V����
		}
	}

	return emErr;
}

ITablePtr CRow::GetTable() const
{
	ITablePtr ipTable;
	INVOKEMETHOD( get_Table( &ipTable ) );
	return ipTable;
}

IWorkspacePtr CRow::GetWorkspace() const
{
	IWorkspacePtr ipRet;	// �Ԃ�l

	IDatasetPtr ipDataset( GetTable() );
	LOGASSERTE_IF( NULL != ipDataset, sindyErr_COMFunctionFailed )
	{
		LOGASSERTE_IF( SUCCEEDED( ipDataset->get_Workspace( &ipRet ) ), sindyErr_AOGetWorkspaceFailed );
	}

	return ipRet;
}

sindyErrCode CRow::GetValue( long lIndex, VARIANT& vaValue ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	if( lIndex >= 0 )
	{
		// �L���b�V������
		if( m_pCache )
		{
			if( FAILED( VariantCopy( &vaValue, (*m_pCache)[lIndex] ) ) )
				emErr = sindyErr_MemoryAllocationFailed;	// �K�؂ȕԂ�l�ł͂Ȃ�����
		}
		// �L���b�V���Ȃ�
		else {
			LOGASSERTE_IF( p, sindyErr_ClassMemberMissing )
			{
				HRESULT hr;	// [bug 3182] ���܂ɂ����ŃG���[���o��̂ŁA�����̂��߂ɏڍ׃��O���o��
				SUCCEEDED_IF( hr = p->get_Value( lIndex, &vaValue ), emErr = sindyErr_AOGetValueFailed, _T("HRESULT[%ld(0x%X)] TABLE[%s] OID[%ld] lIndex[%ld]"), hr, hr, GetTableName(), GetOID(), lIndex )
				{
				}
			}
//			INVOKEMETHOD( get_Value( lIndex, &vaValue ) );
//			emErr = (FAILED(hr)) ? sindyErr_AOGetValueFailed : sindyErr_NoErr;
		}
	}
	else
		emErr = sindyErr_ArgLimitOver;	// �C���f�b�N�X��0�ȏ�

	return emErr;
}

sindyErrCode CRow::SetValue( long lIndex, const VARIANT& vaValue )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	if( lIndex >= 0 )
	{
		// �L���b�V������
		if( m_pCache )
		{
			if( FAILED( VariantCopy( (*m_pCache)[lIndex], (VARIANTARG*)&vaValue ) ) )	// const�Ŏ󂯎���Ă���Ȃ��̂Ŗ������L���X�g
				emErr = sindyErr_MemoryAllocationFailed;	// �K�؂ȕԂ�l�ł͂Ȃ�����
		}
		// �L���b�V���Ȃ�
		else {
			INVOKEMETHOD( put_Value( lIndex, vaValue ) );
			emErr = (FAILED(hr)) ? sindyErr_AOPutValueFailed : sindyErr_NoErr;
		}
	}
	else
		emErr = sindyErr_ArgLimitOver;	// �C���f�b�N�X��0�ȏ�

	return emErr;
}

//�t�B�[���h�Ή��\�̎Q�Ƃ�Ԃ�
CSPFieldMap CRow::GetFieldMap() const
{
	if( ! m_spFields )
	{
		// �L���b�V������Ă��Ȃ��Ȃ烁���o�������ŏ���������
		// ���������ƁACRow����CTableNameString�̎��̂������ƂɂȂ�̂�
		// �{���͍D�܂����Ȃ��B�������A���֐����d�����A�����ō쐬�ł���悤��
		// ���Ă����B�������A�f�o�b�O�g���[�X�͏o���ĊJ���҂ɂ͂킩��悤��
		// ���Ă������ƁB
		LOGASSERTEM_IF( ! IsCached(), sindyErr_ClassMemberMissing, FIELDS_NOT_INITIALIZE )
		{
			IFieldsPtr ipFields;
			LOGASSERTE_IF( NULL != ( ipFields = GetFields() ), sindyErr_COMFunctionFailed )
			{
				m_spFields.reset( new CFieldMap( ipFields ) );

				ATLTRACE(_T("%s : CFieldMap cache created.\n"), __func__ );
			}
		}
	}

	return m_spFields;
}

// �e�[�u�����Ȃǂ��Ǘ����郁���o�̎Q�Ƃ�Ԃ�
CSPTableNameString CRow::GetNameString() const
{
	if( ! m_spNameString )
	{
		// �L���b�V������Ă��Ȃ��Ȃ烁���o�������ŏ���������
		// ���������ƁACRow����CTableNameString�̎��̂������ƂɂȂ�̂�
		// �{���͍D�܂����Ȃ��B�������A���֐����d�����A�����ō쐬�ł���悤��
		// ���Ă����B�������A�f�o�b�O�g���[�X�͏o���ĊJ���҂ɂ͂킩��悤��
		// ���Ă������ƁB
		LOGASSERTEM_IF( ! IsCached(), sindyErr_ClassMemberMissing, NAMESTRING_NOT_INITIALIZE )
		{
			ITablePtr ipTable;
			LOGASSERTE_IF( NULL != ( ipTable = GetTable() ), sindyErr_COMFunctionFailed )
			{
				m_spNameString.reset( new CTableNameString( ipTable ) );
#ifdef _DEBUG
				if( m_spNameString.get()->IsSDE() )
				{
					ATLTRACE(_T("%s : CTableNameString cache created : %s.%s[%s]%s@%s:%s\n"), 
						__func__,
						m_spNameString.get()->GetOwnerName(),
						m_spNameString.get()->GetTableName(),
						m_spNameString.get()->GetVersionName(),
						m_spNameString.get()->GetUserName(),
						m_spNameString.get()->GetServerName(),
						m_spNameString.get()->GetInstanceName()
					);
				}
				else {
					ATLTRACE(_T("%s : CTableNameString cache created : %s\\\\%s\n"),
						__func__,
						m_spNameString.get()->GetTableName(),
						m_spNameString.get()->GetPathName()
					);
				}
#endif // ifdef _DEBUG
			}
		}
	}

	return m_spNameString;
}

// �e�[�u������Ԃ�
LPCTSTR CRow::GetTableName() const
{
	return GetNameString()->GetTableName();
}
/*
// �e�[�u������Ԃ�
LPCTSTR CRow::GetTableName( LPTSTR lpszName, int nLen ) const
{
	// �����`�F�b�N
	if( nLen > 0 && lpszName != NULL )
	{
		// ������
		lpszName[0] = _T('\0');

		CTable cTable( GetTableType(), GetTable() );
		LPCTSTR lpcszName = cTable.GetTableName();
		int nNameLen = lstrlen( lpcszName );
		
		if( nLen > nNameLen )
			lstrcpyn( lpszName, lpcszName, nLen );
		else
			ERRORLOG(sindyErr_BufferIsNotEnough);	// �o�b�t�@�̈悪����Ȃ�
	}
	else
		ERRORLOG(sindyErr_ArgLimitOver);

	return lpszName;
};
LPCTSTR CRow::GetTableName( _IRow* ipRow, LPTSTR lpszName, int nLen )
{
	CRow pRow;
	pRow.SetObject( ipRow );

	return pRow.GetTableName( lpszName, nLen );
}

// �I�[�i�[����Ԃ�
LPCTSTR CRow::GetOwnerName( LPTSTR lpszName, int nLen ) const
{
	// �����`�F�b�N
	if( nLen > 0 && lpszName != NULL )
	{
		// ������
		lpszName[0] = _T('\0');

		CTable cTable( GetTableType(), GetTable() );
		LPCTSTR lpcszName = cTable.GetOwnerName();
		int nNameLen = lstrlen( lpcszName );
		
		if( nLen > nNameLen )
			lstrcpyn( lpszName, lpcszName, nLen );
		else
			ERRORLOG(sindyErr_BufferIsNotEnough);	// �o�b�t�@�̈悪����Ȃ�
	}
	else
		ERRORLOG(sindyErr_ArgLimitOver);

	return lpszName;
}
LPCTSTR CRow::GetOwnerName( _IRow* ipRow, LPTSTR lpszName, int nLen )
{
	CRow pRow;
	pRow.SetObject( ipRow );

	return pRow.GetOwnerName( lpszName, nLen );
}


LPCTSTR CRow::GetServerName( LPTSTR lpszName, int nLen ) const
{
	USES_CONVERSION;
	// �����`�F�b�N
	if( nLen > 0 && lpszName != NULL )
	{
		// ������
		lpszName[0] = _T('\0');

		CTable cTable( GetTableType(), GetTable() );
		IWorkspacePtr ipWorkspace;
		ipWorkspace = cTable.GetWorkspace();
		if( ipWorkspace != NULL )
		{
			IPropertySetPtr ipPropertySet;
			ipWorkspace->get_ConnectionProperties( &ipPropertySet );
			if( ipPropertySet != NULL )
			{
				CComVariant vaValue;
				ipPropertySet->GetProperty( CComBSTR(_T("SERVER")),&vaValue );
				LPCTSTR lpcszName = OLE2CT( vaValue.bstrVal );
				int nNameLen = lstrlen( lpcszName );
				
				if( nLen > nNameLen )
					lstrcpyn( lpszName, lpcszName, nLen );
				else
					ERRORLOG(sindyErr_BufferIsNotEnough);	// �o�b�t�@�̈悪����Ȃ�
			}
		}
	}
	else
		ERRORLOG(sindyErr_ArgLimitOver);

	return lpszName;
}
LPCTSTR CRow::GetServerName( _IRow* ipRow, LPTSTR lpszName, int nLen ) 
{
	CRow pRow;
	pRow.SetObject( ipRow );

	return pRow.GetServerName( lpszName, nLen );
}


LPCTSTR CRow::GetVersionName( LPTSTR lpszName, int nLen ) const
{
	USES_CONVERSION;
	// �����`�F�b�N
	if( nLen > 0 && lpszName != NULL )
	{
		// ������
		lpszName[0] = _T('\0');

		CTable cTable( GetTableType(), GetTable() );
		IWorkspacePtr ipWorkspace;
		ipWorkspace = cTable.GetWorkspace();
		if( ipWorkspace != NULL )
		{
			IPropertySetPtr ipPropertySet;
			ipWorkspace->get_ConnectionProperties( &ipPropertySet );
			if( ipPropertySet != NULL )
			{
				CComVariant vaValue;
				ipPropertySet->GetProperty( CComBSTR(_T("VERSION")),&vaValue );
				LPCTSTR lpcszName = OLE2CT( vaValue.bstrVal );
				int nNameLen = lstrlen( lpcszName );
				
				if( nLen > nNameLen )
					lstrcpyn( lpszName, lpcszName, nLen );
				else
					ERRORLOG(sindyErr_BufferIsNotEnough);	// �o�b�t�@�̈悪����Ȃ�
			}
		}
	}
	else
		ERRORLOG(sindyErr_ArgLimitOver);

	return lpszName;
}
LPCTSTR CRow::GetVersionName( _IRow* ipRow, LPTSTR lpszName, int nLen ) 
{
	CRow pRow;
	pRow.SetObject( ipRow );

	return pRow.GetVersionName( lpszName, nLen );
}


void CRow::GetInfo( _IRow* ipRow, LPTSTR lpszLayerName, LPTSTR lpszOwnerName, LPTSTR lpszServerName, LPTSTR lpszVersionName)
{
	ITablePtr ipTable;
	IDatasetPtr ipDataset;
	if( ipRow != NULL )
	{
		// ������
		lpszLayerName[0] = _T('\0');
		lpszOwnerName[0] = _T('\0');
		lpszServerName[0] = _T('\0');
		lpszVersionName[0] = _T('\0');

		CRow::GetTableName( ipRow, lpszLayerName, 64 );
		CRow::GetOwnerName( ipRow, lpszOwnerName, 64 );
		CRow::GetServerName( ipRow, lpszServerName, 64 );
		CRow::GetVersionName( ipRow, lpszVersionName, 64 );
	}
}
*/
bool CRow::IsEqualObject(_IRow* ipRow ) const
{
	if( p )
	{
		// �܂��́AOID�݂̂Ŕ�r����i���x�D��j
		if( AheGetOID( ipRow ) == GetOID() )
		{
			ITablePtr ipTable;
			ipRow->get_Table( &ipTable );
			CTableNameString cNames( ipTable );
			return ( cNames == *GetNameString() );
		}
	}
	return false;
}

bool CRow::IsEqualObject( const CRow& cRow ) const
{
	if( cRow.p && p ) // DummyRow�̏ꍇ��p�����݂��Ȃ�
		return ( GetOID() == cRow.GetOID() && *GetNameString() == *cRow.GetNameString() );
	return false;
}

// �L���b�V�����쐬����
sindyErrCode CRow::CreateCache( bool bDetatch/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ )
{
	double dStart = GetTickCount();	// ���Ԍv���p

	if( spNameString )
		m_spNameString = spNameString;
	if( spFields )
		m_spFields     = spFields;

	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	// �����̃L���b�V�����Ȃ��ꍇ�ɂ̂ݍ쐬
	// �����̃L���b�V��
	if( ! m_pCache )
	{
		// OBJECTID�����͂����Ŏ擾
		if( p )
			m_lOID = GetOID();

		// �t�B�[���h�Ή��\��������΍쐬
		if( ! m_spFields )
		{
			if( p )
				m_spFields.reset( new CFieldMap( GetFields() ) );
			else
				emErr = sindyErr_ClassMemberMissing;
		}

		// �e�[�u���Ή��\��������΍쐬
		if( ! m_spNameString )
		{
			if( p )
				m_spNameString.reset( new CTableNameString( GetTable() ) );
			else
				emErr = sindyErr_ClassMemberMissing;
		}

		// �Ή��\�쐬�ŃG���[��������΃L���b�V�����쐬����
		if( m_spFields )
		{
			m_pCache = new CCacheAttribute();
			if( m_pCache )
				emErr = m_pCache->CreateCache( p, *m_spFields );
			else
				emErr = sindyErr_MemoryAllocationFailed;

		}
		else {
			emErr = sindyErr_FieldMapNotCreated;	// �t�B�[���h�Ή��\���Z�b�g����Ă��Ȃ�
			ERRORLOG(emErr);
			SASSERT( m_spFields );
		}
	}
	// �`��̃L���b�V��
	if( ! m_pGeom )
	{
		if( p ) // p��NULL�̏ꍇ����ibug 5421�j���A�G���[�Ƃ͂��Ȃ��iCTable::CreateDummyRow�̂��߁j
		{
			// �t�B�[�`���̂Ƃ��݂̂̓���Ƃ���̂ŁAQUERYINVOKEMETHOD�͎g�p���Ȃ��i�G���[���o�邽�߁j
			IFeaturePtr ipFeature;
			if( SUCCEEDED( p->QueryInterface( IID_IFeature, (void**)&ipFeature ) ) )
			{
				IGeometryPtr ipGeom;
				if( SUCCEEDED( ipFeature->get_Shape( &ipGeom ) ) )
				{
					// �t�B�[�`���쐬���ɌĂ΂ꂽ�ꍇ��
					// NULL Shape�͂��肤�邪�Am_pGeom�͍���Ă���
					// �K�v������
					m_pGeom = new CGeometry( ipGeom );
					m_pGeom->CreateCache( bDetatch );
				}
				else {
					emErr = sindyErr_GeometryNotFound;
					ERRORLOG(emErr);
					SASSERT( false );
				}
			}
		}
	}

	// �؂藣��
	if( bDetatch )
	{
		if( p )
		{
			p->Release();
			p = NULL;
		}
	}

/*#ifdef _DEBUG
	if( m_pGeom )
		CGeometry::Trace( *m_pGeom, _T("hoge") );
#endif // ifdef _DEBUG*/

	// �L���b�V�����Ԃ��v�Z
	g_dTotalCacheTime += (double)GetTickCount()-dStart;

	// �֘A�I�u�W�F�N�g�̃L���b�V�����쐬
	CContainer::CreateCache( bDetatch );

	return emErr;
}

// �L���b�V�����폜����
void CRow::DeleteCache( bool bDeleteContainerCache/* = false*/ )
{
	// �����̃L���b�V�����폜
	if( m_pCache )
	{
		delete m_pCache;
		m_pCache = NULL;
	}
	// �`��̃L���b�V�����폜
	if( m_pGeom )
	{
		delete m_pGeom;
		m_pGeom = NULL;
	}
	m_lOID = -1;

	// �R���e�i�̃L���b�V�����폜
	if( bDeleteContainerCache )
		CContainer::DeleteCache( bDeleteContainerCache );
}

// �L���b�V������Ă��邩�ǂ������`�F�b�N����
bool CRow::IsCached() const
{
	bool bRet = false;	// �Ԃ�l

	// �������L���b�V������Ă��邩�ǂ���
	if( m_pCache )
		bRet = true;

	// �`�󂪃L���b�V������Ă��邩�ǂ���
	if( ! ( m_pGeom && m_pGeom->IsCached() ) )
	{
		if( bRet )
			bRet = false;
	}

	return bRet;
}

// �R�s�[����
sindyErrCode CRow::CopyTo( CRow& dest, bool bIsCopyCache/*=true*/, bool bIsCopyRow/*=true*/, bool bIsCopyFlag/*=true*/, bool bIsCopyContainer/*=true*/ ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	if ( bIsCopyRow ) {
		AtlComPtrAssign( (IUnknown**)dest.Release(), (_IRow*)*this );	// IRow(QI)
		// OID���R�s�[����
		dest.m_lOID = m_lOID;
	}

	if ( bIsCopyCache ) {
		// �������R�s�[����i�����̃L���b�V����dest.Release()�ō폜�����̂ł���Ȃ��j
		if( m_pCache )
		{
			if ( dest.m_pCache )
				delete dest.m_pCache;

			dest.m_pCache = new CCacheAttribute( *m_pCache );
			if( ! dest.m_pCache ) 
				emErr = sindyErr_MemoryAllocationFailed;
		}
		// �`����R�s�[����
		if( m_pGeom )
		{
			if ( dest.m_pGeom )
				delete dest.m_pGeom;

			dest.m_pGeom = new CGeometry( *m_pGeom );
			if( ! dest.m_pGeom )
				emErr = sindyErr_MemoryAllocationFailed;
		}

		if( emErr != sindyErr_NoErr )
		{
			ERRORLOG(emErr);
			SASSERT( emErr != sindyErr_NoErr );
		}
		// �t�B�[���h�Ή��\���R�s�[����
		dest.m_spFields = m_spFields;

		// �e�[�u�������Ǘ��I�u�W�F�N�g���R�s�[����
		dest.m_spNameString = m_spNameString;

		// �e�[�u���^�C�v���R�s�[����
		dest.m_emTableType = m_emTableType;

		if ( bIsCopyContainer )
			((CContainer&)dest) = (CContainer&)(*this);
	}

	if ( bIsCopyFlag ) {
		dest.m_bCreated = m_bCreated;
		dest.m_bDeleted = m_bDeleted;
	}

	// src���_�~�[����dest�̃I�u�W�F�N�g�����݂���Ƃ��́Adest�̍쐬�t���O��߂��B
	if ( ! (_IRow*)(*this) && (_IRow*)dest && dest.m_bCreated )
		dest.m_bCreated = false;

	return emErr;
}

void CRow::swap( CRow& src, bool bIsCopyCache/*=true*/, bool bIsCopyRow/*=true*/, bool bIsCopyFlag/*=true*/, bool bIsCopyContainer/*=true*/ )
{
	CRow cSwapRow;

	CopyTo( cSwapRow, bIsCopyCache, bIsCopyRow, bIsCopyFlag, bIsCopyContainer );
	src.CopyTo( *this, bIsCopyCache, bIsCopyRow, bIsCopyFlag, bIsCopyContainer );
	cSwapRow.CopyTo( src, bIsCopyCache, bIsCopyRow, bIsCopyFlag, bIsCopyContainer );
}

// IRow*��V�������̂ɑ}���ւ���
sindyErrCode CRow::ReplaceNewRow( const CTable* pTargetTable/* = NULL*/, bool bRecursive/* = false*/ )
{
	if( !( p || pTargetTable ) )
		return sindyErr_ClassMemberMissing;

	CTable cTable( *pTargetTable );
	if( ! cTable )
		cTable.SetObject( GetTable(), GetFieldMap(), GetNameString() );

	_IRowPtr ipRow;
	if( FAILED( ((ITable*)cTable)->CreateRow( &ipRow ) ) )
		return sindyErr_AOCreateRowFailed;

	AtlComPtrAssign( (IUnknown**)&p, (IUnknown*)ipRow );
	m_lOID = AheGetOID( ipRow ); // m_lOID�����͍X�V���Ă���

	m_bCreated = true; // �쐬�t���O�𗧂Ă�

	if( pTargetTable )
	{
		// �e�[�u�����n����Ă���ꍇ�̓I�[�i�[����t�B�[���h���
		// �قȂ�ꍇ���l�����邽�߁A�����ŃN���A����K�v������
		// �ł��A���̌�t�B�[���h����e�[�u����񂪕K�v�ȏꍇ�ɂ�
		// �e�t�B�[�`���ōč쐬�������Ă��Ȃ���������Ȃ��c�B
		// ����ς�A�e�R���e�i�Ńe�[�u�������Ǘ����邩�ASiNDY-i��
		// �悤�Ƀe�[�u���t�@�C���_���p�����邩���Ȃ��Ƃ��߂���Ȃ��B�B�B
		// �ł���������ƕʃ��[�N�X�y�[�X���ꏏ�Ɉ����Ȃ����Ȃ��B�B�B
		// �������Ȃ��B�B�B

		if( m_pCache )
		{
			// �����ŃL���b�V����V�����t�B�[���h���Ԃɕ��בւ���K�v������
			const CFieldMap& cNewFields( *cTable.GetFieldMap() );
			const CFieldMap& cOldFields( *GetFieldMap() );
			CCacheAttribute* pNewCache = new CCacheAttribute();
			pNewCache->CreateCache( p, cNewFields );
			int nCount = cNewFields.GetFieldCount();
			for( int i = 0; i < nCount; ++i )
			{
				long lIndex = cOldFields.FindField( cNewFields.GetName(i) );
				if( 0 <= lIndex )
					VariantCopy( (*pNewCache)[i], (*m_pCache)[lIndex] ); // VariantCopy��ThreadSafe����Ȃ��iMSDN �w���v���j
			}
			// �����L���b�V���}���ւ�
			delete m_pCache;
			m_pCache = pNewCache;
		}
		m_spNameString = cTable.GetNameString();
		m_spFields = cTable.GetFieldMap();
	}

	if( ! bRecursive )
		return sindyErr_NoErr;

	// �ċA�I�ɏ�������ꍇ
	sindyErrCode emErr = sindyErr_NoErr; // �߂�l

	LPCTSTR lpcszTableName = GetTableName(); // ���̃I�u�W�F�N�g�̃e�[�u����
	for( iterator it = begin(); it != end(); ++it )
	{
		// �R���e�i���̃e�[�u�������擾���Ă���
		LPCTSTR lpcszRelationTableName = AheTableType2TableName( it->first );
		// ID�֘A��������̂��ǂ������ׂĂ���
		CString strOuterKeyFieldName; // �O���L�[�t�B�[���h��
		for( CRuleRelationTable::const_value_iterator itRule = g_cRuleRelationTable.begin(lpcszTableName);
			itRule != g_cRuleRelationTable.end(lpcszTableName); ++itRule )
		{
			if( 0 == itRule->get<1>().CompareNoCase( lpcszRelationTableName ) )
			{
				strOuterKeyFieldName = itRule->get<2>();
				break;
			}
		}
		// ipTargetTable���w�肳��Ă���ꍇ�͓����e�[�u�����������̂�n��
		ITablePtr ipContainerTargetTable;
		if( pTargetTable )
		{
			ipContainerTargetTable = cTable.OpenSameOwnerTable( lpcszRelationTableName );
			if( ! ipContainerTargetTable )
			{
				emErr = sindyErr_AOGetFeatureClassFailed; // �e�[�u�����I�[�v���ł��Ȃ��ꍇ�͌p�������Ȃ�
				break;
			}
		}
		for( rows_iterator itRow = it->second.begin(); itRow != it->second.end(); ++itRow )
		{
			CTable cContainerTable( ipContainerTargetTable );
			if( sindyErr_NoErr != ( emErr = (*itRow)->ReplaceNewRow( &cContainerTable, bRecursive ) ) ) // �}���ւ�
				break;
			if( sindyErr_NoErr != ( emErr = (*itRow)->SetLongValueByFieldName(strOuterKeyFieldName, m_lOID) ) ) // �O���L�[���Z�b�g
				break;
		}
		if( sindyErr_NoErr != emErr )
			break;
	}
	return emErr;
}

// �����ɕύX�����������ǂ������`�F�b�N����

bool CRow::Changed( schema::ipc_table::update_type::ECode* emUpdateType/* = NULL*/ ) const
{
	bool bChanged = false; // �Ԃ�l

	if( emUpdateType ) *emUpdateType = schema::ipc_table::update_type::kDefault;	// ����������

	// �����̃`�F�b�N
	if( p )
	{
		if ( m_bDeleted ) {
			bChanged = true;
			if( emUpdateType )
				*emUpdateType = schema::ipc_table::update_type::kDeleted;
		}
		else if( m_bCreated ) {
			bChanged = true;
			if( emUpdateType )
				*emUpdateType = schema::ipc_table::update_type::kCreated;
		} else {
			long lFieldCount = GetFieldMap()->GetFieldCount();

			// �L���b�V��������ꍇ
			if( m_pCache || m_pGeom )
			{
				if( m_pCache )
				{
					CComVariant vaCache, vaValue;

					for( long i = 0; i < lFieldCount; ++i )
					{
						if( GetFieldMap()->IsEditable( i ) )
						{
							// Shape�t�B�[���h�̓L���b�V�������Ȃ����A��r���ł��Ȃ��̂Ŕ�΂�
							if( GetFieldMap()->IsShapeField( i ) )
								continue;

							vaCache.Clear();
							vaValue.Clear();

							GetValue( i, vaCache );
							INVOKEMETHOD( get_Value(i, &vaValue) );

							// VT_DATE�̔�r�͍s���Ȃ����߁AVT_BSTR�ɕϊ����Ĕ�r����
							if( vaCache.vt == VT_DATE )
								vaCache.ChangeType(VT_BSTR);
							if( vaValue.vt == VT_DATE )
								vaValue.ChangeType(VT_BSTR);

							if( vaCache != vaValue )
							{
								bChanged = true;
								if( emUpdateType )
									*emUpdateType = schema::ipc_table::update_type::kAttrUpdated;
								break;
							}
						}
					}
				}
				// �`��̃`�F�b�N
				if( m_pGeom )
				{
					IFeaturePtr ipFeat;
					if( SUCCEEDED( p->QueryInterface( IID_IFeature, (void**)&ipFeat ) ) )
					{
						IGeometryPtr ipOrg;
						LOGASSERTE_IF( SUCCEEDED( ipFeat->get_Shape( &ipOrg ) ), sindyErr_GeometryNotFound )
						{
							// NULL������ꍇ������̂Ń`�F�b�N
							if( (IGeometry*)*m_pGeom )
							{
								if( ! m_pGeom->EqualsVertex( ipOrg ) )
								{
									bChanged = true;
									if( emUpdateType )
										*emUpdateType = schema::ipc_table::update_type::kShapeUpdated;
								}
							}
						}
					}
				}
			}
			// �L���b�V���������ꍇ
			else {
				// �����̃`�F�b�N
				IRowChangesPtr ipRowChanges( p );
				if( NULL != ipRowChanges )
				{
					VARIANT_BOOL vbChanged = VARIANT_FALSE;
					for( long i = 0; i < lFieldCount; ++i )
					{
						ipRowChanges->get_ValueChanged( i, &vbChanged );
						if( vbChanged )
						{
							bChanged = true;
							if( emUpdateType )
								*emUpdateType = schema::ipc_table::update_type::kAttrUpdated;
							break;	// ��ł�����Ȃ�����`�F�b�N�I���
						}
					}
				}
				IFeatureChangesPtr ipFeatureChanges( (IFeaturePtr)p );
				if( NULL != ipFeatureChanges )
				{
					VARIANT_BOOL vbChanged = VARIANT_FALSE;
					ipFeatureChanges->get_ShapeChanged( &vbChanged );
					if( vbChanged )
					{
						bChanged = true;
						if( emUpdateType )
							*emUpdateType = schema::ipc_table::update_type::kShapeUpdated;
					}
				}
			}
		}
	}
	else if( m_bDeleted )
	{
		bChanged = true;
		if( emUpdateType )
			*emUpdateType = schema::ipc_table::update_type::kDeleted;
	}
	else if( m_bCreated )
	{
		bChanged = true;
		if( emUpdateType )
			*emUpdateType = schema::ipc_table::update_type::kCreated;
	}

	return bChanged;
}

// �����ɕύX�����������ǂ������`�F�b�N����
bool CRow::Changed( LPCTSTR lpcszFieldName ) const
{
	bool bChanged = false; // �Ԃ�l

	// �L���b�V��������ꍇ�ɂ̓L���b�V���Ŕ�r
	if( m_pCache )
	{
		long lIndex = GetFieldMap()->FindField(lpcszFieldName);
		if( GetFieldMap()->IsShapeField( lIndex ) )
			return bChanged;

		LOGASSERTE_IF( lIndex >= 0, sindyErr_AOGetFieldIndexFailed )
		{
			CComVariant vaCache, vaValue;
			GetValue( lIndex, vaCache );
			INVOKEMETHOD( get_Value( lIndex, &vaValue ) );

			// VT_DATE�̔�r�͍s���Ȃ����߁AVT_BSTR�ɕϊ����Ĕ�r����
			if( vaCache.vt == VT_DATE )
				vaCache.ChangeType(VT_BSTR);
			if( vaValue.vt == VT_DATE )
				vaValue.ChangeType(VT_BSTR);

			if( vaCache != vaValue )
				bChanged = true;
		}
	}
	else if( p != NULL )
	{
		IFieldsPtr ipFields;
		p->get_Fields( &ipFields );
		long lIndex = -1;
		ipFields->FindField( _bstr_t(lpcszFieldName), &lIndex );
		if( lIndex > 0 )
		{
			IFieldPtr ipField;
			ipFields->get_Field( lIndex, &ipField );
			esriFieldType esriType;
			ipField->get_Type( &esriType );
			// �`���r�͂ł��Ȃ��̂Ŗ���
			if( esriType != esriFieldTypeGeometry )
			{
				IRowChangesPtr ipRowChanges( p );
				CComVariant vaOrgVal, vaCurrentVal;
				ipRowChanges->get_OriginalValue( lIndex, &vaOrgVal );
				p->get_Value( lIndex, &vaCurrentVal );
				if( vaOrgVal != vaCurrentVal )
					bChanged = true;
			}
		}
	}
	return bChanged;
}

// �t�B�[���h���Ńt�B�[���h�l���擾����
sindyErrCode CRow::GetValueByFieldName( LPCTSTR lpcszFieldName, VARIANT& vaValue ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	if( lpcszFieldName != NULL )
		emErr = GetValue( GetFieldMap()->FindField(lpcszFieldName), vaValue );	// �l�擾
	else {
		emErr = sindyErr_ArgIsNull;	// �t�B�[���h����NULL
		ERRORLOG(emErr);
	}

	return emErr;
}

// �t�B�[���h���Ńt�B�[���h�l���Z�b�g����
sindyErrCode CRow::SetValueByFieldName( LPCTSTR lpcszFieldName, const VARIANT& vaValue )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	if( lpcszFieldName != NULL )
		emErr = SetValue( GetFieldMap()->FindField(lpcszFieldName), vaValue );
	else {
		emErr = sindyErr_ArgIsNull;
		ERRORLOG(emErr);
	}

	return emErr;
}

// �C���f�b�N�X�ԍ���bool�l���擾����
bool CRow::GetBoolValue( long index, bool bDefaultValue ) const
{
	VARIANT vaValue;
	VariantInit(&vaValue);

	// �����̃`�F�b�N��GetValue�ł��邩��ǂ�
	if( sindyErr_NoErr == GetValue( index, vaValue ) )
	{
		switch( vaValue.vt )
		{
			case VT_I4: bDefaultValue = ( vaValue.lVal == 0 )? false : true;	break;
			case VT_NULL: break;
			default: SASSERT( vaValue.vt == VT_I4 || vaValue.vt == VT_NULL ); break;
		}
	}
	else
		SASSERT( false );	// �l�擾�Ɏ��s����

	return bDefaultValue;
}

// �C���f�b�N�X�ԍ���bool�l���Z�b�g����
sindyErrCode CRow::SetBoolValue( long index, bool bNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_I4;
	vaValue.lVal = ( bNewValue ) ? 1 : 0;

	return SetValue( index, vaValue );
}

// �C���f�b�N�X�ԍ���long�l���擾����
long CRow::GetLongValue( long index, long lDefaultValue ) const
{
	VARIANT vaValue;
	VariantInit(&vaValue);

	// �����̃`�F�b�N��GetValue�ł��邩��ǂ�
	if( sindyErr_NoErr == GetValue( index, vaValue ) )
	{
		switch( vaValue.vt )
		{
			case VT_I4: break;
			case VT_NULL: vaValue.lVal = lDefaultValue; break;
			default: SASSERT( vaValue.vt == VT_I4 || vaValue.vt == VT_NULL ); vaValue.lVal = lDefaultValue; break;
		}
	}
	else
		SASSERT( false );	// �l�擾�Ɏ��s����

	return vaValue.lVal;
}

// �C���f�b�N�X�ԍ���long�l���Z�b�g����
sindyErrCode CRow::SetLongValue( long index, long lNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_I4;
	vaValue.lVal = lNewValue;

	return SetValue( index, vaValue );
}

// �C���f�b�N�X�ԍ���double�l���擾����
double CRow::GetDoubleValue( long index, const double& dDefaultValue ) const
{
	VARIANT vaValue;
	VariantInit(&vaValue);

	// �����̃`�F�b�N��GetValue�ł��邩��ǂ�
	if( sindyErr_NoErr == GetValue( index, vaValue ) )
	{
		switch( vaValue.vt )
		{
			case VT_I8: break;
			case VT_NULL: vaValue.dblVal = dDefaultValue; break;
			default: SASSERT( vaValue.vt == VT_I8 || vaValue.vt == VT_NULL ); vaValue.dblVal = dDefaultValue; break;
		}
	}
	else
		SASSERT( false );	// �l�擾�Ɏ��s����

	return vaValue.dblVal;
}

// �C���f�b�N�X�ԍ���double�l���Z�b�g����
sindyErrCode CRow::SetDoubleValue( long index, const double& dNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_I8;
	vaValue.dblVal = dNewValue;

	return SetValue( index, vaValue );
}

// �C���f�b�N�X�ԍ���String�l���擾����
LPCTSTR CRow::GetStringValue( long index, int nLen, LPTSTR lpszValue ) const
{
	LPCTSTR pRet = 0;	// �Ԃ�l

	LOGASSERTE_IF( lpszValue != NULL, sindyErr_ArgIsNull )
	{
		// ������
		lpszValue[0] = _T('\0');

		VARIANT vaValue;
		VariantInit(&vaValue);

		// �����̃`�F�b�N��SetValueByFieldName�ł��邩��ǂ�
		sindyErrCode emErr = sindyErr_NoErr;
		LOGASSERT_IF( sindyErr_NoErr == ( emErr = GetValue( index, vaValue ) ), emErr )
		{
			USES_CONVERSION;

			switch( vaValue.vt )
			{
				case VT_BSTR: lstrcpyn( lpszValue, OLE2CT(vaValue.bstrVal), nLen ); pRet = lpszValue; break;
				case VT_NULL: break;
				default: LOGASSERTE( vaValue.vt == VT_BSTR || vaValue.vt == VT_NULL, sindyErr_AOUnsupportValueType ); break;
			}
		}

		VariantClear(&vaValue);
	}

	return pRet;
}

// �C���f�b�N�X�ԍ��ŕ�����l���Z�b�g����
sindyErrCode CRow::SetStringValue( long index, LPCTSTR lpcszNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_BSTR;
	vaValue.bstrVal = T2BSTR(lpcszNewValue);

	sindyErrCode emErr = SetValue( index, vaValue );
	VariantClear(&vaValue);

	return emErr;
}

// �t�B�[���h����bool�l���擾����
bool CRow::GetBoolValueByFieldName( LPCTSTR lpcszFieldName, bool bDefaultValue ) const
{
	VARIANT vaValue;
	VariantInit(&vaValue);

	// �����̃`�F�b�N��SetValueByFieldName�ł��邩��ǂ�
	if( sindyErr_NoErr == GetValueByFieldName( lpcszFieldName, vaValue ) )
	{
		switch( vaValue.vt )
		{
			case VT_I4: bDefaultValue = ( vaValue.lVal == 0 ) ? false : true; break;
			case VT_NULL: break;
			default: SASSERT( vaValue.vt == VT_I4 || vaValue.vt == VT_NULL ); break;
		}
	}
	else
		SASSERT( false );	// �l�擾�Ɏ��s����

	return bDefaultValue;
}

// �t�B�[���h����bool�l���Z�b�g����
sindyErrCode CRow::SetBoolValueByFieldName( LPCTSTR lpcszFieldName, bool bNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_I4;
	vaValue.lVal = (bNewValue) ? 1 : 0;

	return SetValueByFieldName( lpcszFieldName, vaValue );
}

// �t�B�[���h����long�l���擾����
long CRow::GetLongValueByFieldName( LPCTSTR lpcszFieldName, long lDefaultValue ) const
{
	VARIANT vaValue;
	VariantInit(&vaValue);

	// �����̃`�F�b�N��SetValueByFieldName�ł��邩��ǂ�
	if( sindyErr_NoErr == GetValueByFieldName( lpcszFieldName, vaValue ) )
	{
		switch( vaValue.vt )
		{
			case VT_I4: break;
			case VT_I2: break;
			case VT_NULL: vaValue.lVal = lDefaultValue; break;
			default: //SASSERT( vaValue.vt == VT_I4 || vaValue.vt == VT_NULL );
				vaValue.lVal = lDefaultValue; break;
		}
	}
	else
		SASSERT( false );	// �l�擾�Ɏ��s����

	return vaValue.lVal;
}

// �t�B�[���h����long�l���Z�b�g����
errorcode::sindyErrCode CRow::SetLongValueByFieldName( LPCTSTR lpcszFieldName, long lNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_I4;
	vaValue.lVal = lNewValue;

	return SetValueByFieldName( lpcszFieldName, vaValue );
}

// �t�B�[���h����double�l���擾����
double CRow::GetDoubleValueByFieldName( LPCTSTR lpcszFieldName, const double& dDefaultValue ) const
{
	VARIANT vaValue;
	VariantInit(&vaValue);

	// �����̃`�F�b�N��SetValueByFieldName�ł��邩��ǂ�
	if( sindyErr_NoErr == GetValueByFieldName( lpcszFieldName, vaValue ) )
	{
		switch( vaValue.vt )
		{
			case VT_R8:
			case VT_DATE: break;
			case VT_NULL: vaValue.dblVal = dDefaultValue; break;
			default: SASSERT( vaValue.vt == VT_R8 || vaValue.vt == VT_DATE || vaValue.vt == VT_NULL ); vaValue.dblVal = dDefaultValue; break;
		}
	}
	else
		SASSERT( false );	// �l�擾�Ɏ��s����

	return vaValue.dblVal;
}

// �t�B�[���h����double�l���Z�b�g����\n
errorcode::sindyErrCode CRow::SetDoubleValueByFieldName( LPCTSTR lpcszFieldName, const double& dNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_R8;
	vaValue.dblVal = dNewValue;

	return SetValueByFieldName( lpcszFieldName, vaValue );
}

// �t�B�[���h����String�l���擾����
LPCTSTR CRow::GetStringValueByFieldName( LPCTSTR lpcszFieldName, int nLen, LPTSTR lpszValue ) const
{
	LPCTSTR pRet = NULL;	// �Ԃ�l

	LOGASSERTE_IF( lpszValue != NULL, sindyErr_ArgIsNull )
	{
		// ������
		lpszValue[0] = _T('\0');

		VARIANT vaValue;
		VariantInit(&vaValue);

		// �����̃`�F�b�N��SetValueByFieldName�ł��邩��ǂ�
		sindyErrCode emErr = sindyErr_NoErr;
		LOGASSERT_IF( sindyErr_NoErr == ( emErr = GetValueByFieldName( lpcszFieldName, vaValue ) ), emErr )
		{
			USES_CONVERSION;
			switch( vaValue.vt )
			{
				case VT_BSTR: lstrcpyn( lpszValue, OLE2CT(vaValue.bstrVal), nLen ); pRet = lpszValue; break;
				case VT_NULL: break;
				default: LOGASSERTE( vaValue.vt == VT_BSTR || vaValue.vt == VT_NULL, sindyErr_AOUnsupportValueType ); break;
			}
		}
		VariantClear(&vaValue);
	}

	return pRet;
}

// �t�B�[���h���ŕ�����l���Z�b�g����
errorcode::sindyErrCode CRow::SetStringValueByFieldName( LPCTSTR lpcszFieldName, LPCTSTR lpcszNewValue )
{
	VARIANT vaValue;
	VariantInit(&vaValue);
	vaValue.vt = VT_BSTR;
	vaValue.bstrVal = T2BSTR(lpcszNewValue);

	sindyErrCode emErr = SetValueByFieldName( lpcszFieldName, vaValue );
	VariantClear(&vaValue);
	return emErr;
}

// �t�B�[���h����String�l�̃T�C�Y���擾����
int CRow::GetStringValueLengthByFieldName( LPCTSTR lpcszFieldName ) const
{
	int nSize = -1;	// �Ԃ�l

	VARIANT vaValue;
	VariantInit(&vaValue);

	sindyErrCode emErr = sindyErr_NoErr;
	LOGASSERT_IF( sindyErr_NoErr == ( emErr = GetValueByFieldName( lpcszFieldName, vaValue ) ), emErr )
	{
		USES_CONVERSION;

		switch( vaValue.vt )
		{
			case VT_BSTR: nSize = lstrlen( OLE2T(vaValue.bstrVal) ); break;
			case VT_NULL: nSize = 0; break;
			default: SASSERT( vaValue.vt == VT_BSTR || vaValue.vt == VT_NULL ); break;
		}
	}
	VariantClear(&vaValue);

	return nSize;
}

// �t�B�[���h����String�l�̕��������擾����
int CRow::GetStringValueNumByFieldName( LPCTSTR lpcszFieldName ) const
{
	int nSize = -1;	// �Ԃ�l

	VARIANT vaValue;
	VariantInit(&vaValue);

	sindyErrCode emErr = sindyErr_NoErr;
	LOGASSERT_IF( sindyErr_NoErr == ( emErr = GetValueByFieldName( lpcszFieldName, vaValue ) ), emErr )
	{
		switch( vaValue.vt )
		{
			case VT_BSTR: nSize = SysStringLen( vaValue.bstrVal ); break;
			case VT_NULL: nSize = 0; break;
			default: SASSERT( vaValue.vt == VT_BSTR || vaValue.vt == VT_NULL ); break;
		}
	}
	VariantClear(&vaValue);

	return nSize;
}

// �֘A�I�u�W�F�N�g���X�g�Ɋi�[����
/*void CRow::push_back( const CSPRow& cRow )
{
	LOGASSERTE_IF( *cRow, sindyErr_ArgIsNull )
		m_listRelRows[cRow->GetTableType()].push_back( cRow );
}*/

CGeometry* CRow::GetShape()
{
	// �`�󂪃L���b�V������Ă��邩�ǂ���
	if( ! ( m_pGeom && m_pGeom->IsCached() ) )
	{
		// �`��������Ă���I�u�W�F�N�g���ǂ����m�F
		if( p && (IFeaturePtr)p )
		{
			// �g���[�X�o�����ƁI�I
			TRACEMESSAGE(CREATE_CACHE);
			CreateCache();
		}
	}

	return m_pGeom;
}

const CGeometry* CRow::GetShape() const
{
	// �`�󂪃L���b�V������Ă��邩�ǂ���
	if( ! ( m_pGeom && m_pGeom->IsCached() ) )
	{
		// �`��������Ă���I�u�W�F�N�g���ǂ����m�F
		if( p && (IFeaturePtr)p )
		{
			// �g���[�X�o�����ƁI�I
			TRACEMESSAGE(CREATE_CACHE);
			const_cast<CRow*>(this)->CreateCache();	// �L���b�V����邾���Ȃ̂ŁAconst �O��
		}
	}

	return m_pGeom;
}

sindyErrCode CRow::SetShape( IGeometry* ipGeom )
{
	if( ! ipGeom )
		return sindyErr_ArgIsNull;

	if( ! m_pGeom )
		m_pGeom = new CGeometry( (IGeometryPtr)AheCreateClone(ipGeom) );
	else
		(*m_pGeom) = ipGeom;

	return sindyErr_NoErr;
}

// ���݊i�[����Ă���t�B�[�`���̌`��̃R�s�[��Ԃ�
IGeometryPtr CRow::GetShapeCopy( bool bLatest/* = true*/, bool bOriginal/* = false*/, bool bFeatureChangedOriginal/* = false*/, bool bDiff/* = false*/, long/* lParam = -1*/  ) const
{
	IGeometryPtr ipGeom;	// �Ԃ�l

	if( bLatest )
	{
		if( m_pGeom )
			ipGeom = AheCreateClone( (IGeometry*)(*this->GetShape()) );
		else {
			// �L���b�V�����Ă��Ȃ��ꍇ�̓L���b�V�������Ȃ��悤�ɂ����Ƃ���
			IFeaturePtr ipFeature;
			if( SUCCEEDED( QueryInterface( &ipFeature ) ) )
			{
				SUCCEEDED_IF( ipFeature->get_ShapeCopy( &ipGeom ), sindyErr_GeometryNotFound, THERE_MAY_BE_NULL_SHAPE )
				{
				}
			}
		}
	}
	if( p )
	{
		IFeaturePtr ipFeature;
		if( SUCCEEDED( QueryInterface( &ipFeature ) ) )
		{
			if( bOriginal || ( ipGeom == NULL && IsCached() ) )
			{
				IGeometryPtr ipGeomTmp;
				SUCCEEDED_IF( ipFeature->get_ShapeCopy( &ipGeomTmp ), sindyErr_GeometryNotFound, THERE_MAY_BE_NULL_SHAPE )
				{
					// [Bug 6065] ipGeom��NULL�ibLatest��false��bOriginal��true�̎��Ɂj��ipGeomTmp��NULL�̎��ɃG���[���O���o�͂����̂ŉ���B
					if ( ipGeom ) {
						if ( ! bDiff )
							ipGeom = CGeometry::Union( ipGeom, ipGeomTmp );
						else
							ipGeom = CGeometry::SymmetricDifference( ipGeom, ipGeomTmp );
					} else
						ipGeom = ipGeomTmp;
				}
			}
			if( bFeatureChangedOriginal )
			{
				IGeometryPtr ipGeomTmp;
				IFeatureChangesPtr ipChanges( ipFeature );
				LOGASSERTE_IF( SUCCEEDED( ipChanges->get_OriginalShape( &ipGeomTmp ) ), sindyErr_GeometryFunctionFailed )
				{
					// [Bug 6065] ipGeom��NULL�ibLatest��false���Ɂj��ipGeomTmp��NULL�̎��ɃG���[���O���o�͂����̂ŉ���B
					if ( ipGeom ) {
						if ( ! bDiff )
							ipGeom = CGeometry::Union( ipGeom, ipGeomTmp );
						else
							ipGeom = CGeometry::SymmetricDifference( ipGeom, ipGeomTmp );
					} else
						ipGeom = AheCreateClone( ipGeomTmp );
				}
			}
		}
	}
	return ipGeom;
}

// ���������`�F�b�N����
sindyErrCode CRow::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrs ) const
{
	if( g_bDisableCheckToNoSubstance )
	{
		// ���̂��Ȃ����́i�V�K�쐬���ꂽ���̂������j��
		// �`�F�b�N�Ώۂ���O��
		if( ! ( p || m_bCreated ) )
			return sindyErr_NoErr;
	}

	// �������g�̃`�F�b�N
	// �`��
	schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

	if ( Changed( &emUpdateType ) )
	{
		if( cRule.GetUseEditableRule() ) // �ҏW���[�����g�p����ꍇ�݂̂Ɍ���
		{
			// �ҏW�s�`�F�b�N
			sindyeEditType emEditType = (sindyeEditType)-1;

			switch( emUpdateType )
			{
				case schema::ipc_table::update_type::kCreated: emEditType = sindyeEditCreate; break;
				case schema::ipc_table::update_type::kDeleted: emEditType = sindyeEditDelete; break;
				case schema::ipc_table::update_type::kShapeUpdated: emEditType = sindyeEditMove; break;
				case schema::ipc_table::update_type::kAttrUpdated: emEditType = sindyeEditAttribute; break;
				default: break;
			}

			if( -1 < (int)emEditType )
			{
				CErrorInfos infos;

				if( sindyErr_NoErr != CheckEditable( emEditType, infos ) )
				{
					CErrorObject cErr( *this );
					cErr.push_back( infos );
					cErrs.push_back( cErr );
				}
			}
		}

		// �ҏW�������́��폜����Ă��Ȃ����̂̂݃`�F�b�N
		if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
			const CGeometry* pGeom = GetShape();
			if( pGeom )
			{
				IEnvelopePtr ipChkEnv;

				// �|���S�����ύX���������I�u�W�F�N�g��1000�_�ȏ�\���_�������̂̂ݕύX�̈�擾
				if ( pGeom->GetGeometryType() == esriGeometryPolygon && emUpdateType != schema::ipc_table::update_type::kDefault ) {
					if ( pGeom->GetPointCount() > 1000 ) {
						// �ύX�̈�擾
						IGeometryPtr ipEdit = GetShapeCopy(true, true, false, true);

						if ( ipEdit )
							ipEdit->get_Envelope( &ipChkEnv );
					}
				}

				CBasicLogicRule cBasicRule;
				// �Z�O�����g�`�F�b�N
				CErrorInfos cErrInfos;
				if( sindyErr_NoErr != pGeom->CheckLogic( cRule.GetBasicRule( GetTableName(), GetNameString()->GetUserName(), cBasicRule ), cErrInfos, ipChkEnv ) )
				{
					CErrorObject cErr( *this );
					cErr.push_back( cErrInfos );
					cErrs.push_back( cErr );
				}
			}
			// ����
			bool bIsSDE = GetNameString()->IsSDE(); // ������t�B�[���h��PGDB��SDE�Ō^���Ⴄ���߁A�ǂ����Ȃ̂��𔻒f����
			const CFieldMap& cFields( *GetFieldMap() );
			int nIndex = 0;
			for( CFieldMap::const_iterator it = cFields.begin(), itEnd = cFields.end(); it != itEnd; ++it, ++nIndex )
			{
				if( ! it->editable )
					continue;

				CComVariant vaValue;
				GetValue( nIndex, vaValue );
				if( VT_EMPTY == vaValue.vt )
					continue; // �t�B�[���h���擾��������Ă���ꍇ

				// NOT NULL�`�F�b�N�ibug 6434�j
				if( ! it->nullable )
				{
					if( VT_NULL == vaValue.vt || 
						VT_BSTR == vaValue.vt && 0 >= ::SysStringLen(vaValue.bstrVal) )
						cErrs.push_back( CErrorObject( *this, CErrorInfo( sindyErr_SetNullValueToNotNullField, sindyErrLevel_ERR, pGeom ? (IGeometry*)*pGeom : NULL, nIndex, -1, NOT_NULL_FIELD, it->name ) ) );
				}

				switch( it->type )
				{
					case esriFieldTypeString:
					{
						// ������t�B�[���h���ӂ�̌��o
						if( VT_BSTR == vaValue.vt )
						{
							// �I�[�����͓���Ȃ�
							// [Bug 6129] City_Admin�̍\���_���폜�ł��Ȃ��B�iPGDB�̂݁j
							// PGDB�̎���it->length�͕������B
							size_t lByte = bIsSDE ? ( strlen(CW2A(vaValue.bstrVal)) ) : ( (wcslen( vaValue.bstrVal ) ) );
							if( VT_BSTR == vaValue.vt && lByte > it->length )
								cErrs.push_back( CErrorObject( *this, CErrorInfo( sindyErr_FieldLengthLimitOver, sindyErrLevel_ERR, pGeom ? (IGeometry*)*pGeom : NULL, nIndex, -1, BYTES_OVER, it->name, CString(vaValue.bstrVal), lByte - it->length ) ) );
						}
						break;
					}
					default:
						if( VT_NULL != vaValue.vt )
						{
							// �������ӂ�`�F�b�N
							if( 0 < it->precision )
							{
								// ���������̂݃`�F�b�N����
								long num = 0;
								{
									CComVariant va( vaValue );
									va.ChangeType( VT_I4 );
									if( va.vt != VT_I4 ) // vaValue=""�̏ꍇ�ɂ�ChangeType��VT_I4�ɂȂ�Ȃ�
										va = 0L;
									va.lVal = abs(va.lVal);
									while( 0 < va.lVal ){ va.lVal /= 10; ++num; }
								}
								if( it->precision - it->scale < num )
								{
									CComVariant vaValueStr( vaValue );
									vaValueStr.ChangeType( VT_BSTR );
									cErrs.push_back( CErrorObject( *this, CErrorInfo( sindyErr_FieldPrecisionLimitOver, sindyErrLevel_ERR, pGeom ? (IGeometry*)*pGeom : NULL, nIndex, -1, ALLOWABLE_NUM_OF_DIGITS, it->name, CString(vaValueStr.bstrVal), it->precision - it->scale ) ) );
								}
							}
							// �����W�`�F�b�N
							if( VT_EMPTY != it->range.first.vt || VT_EMPTY != it->range.second.vt )
							{
								CComVariant va( vaValue ), vaMin( it->range.first ), vaMax( it->range.second );
								va.ChangeType( VT_R8 ); vaMin.ChangeType( VT_R8 ); vaMax.ChangeType( VT_R8 );
								if( ! ( ( VT_EMPTY == it->range.first.vt  || vaMin.dblVal <= va.dblVal ) &&   // MIN
									    ( VT_EMPTY == it->range.second.vt || vaMax.dblVal >= va.dblVal ) ) )  // MAX
								{
									CComVariant vaValueStr( vaValue ), vaMin( it->range.first ), vaMax( it->range.second );
									vaValueStr.ChangeType( VT_BSTR ); vaMin.ChangeType( VT_BSTR ); vaMax.ChangeType( VT_BSTR );
									cErrs.push_back( CErrorObject( *this, CErrorInfo( sindyErr_FieldRangeLimitOver, sindyErrLevel_ERR, pGeom ? (IGeometry*)*pGeom : NULL, nIndex, -1, ALLOWABLE_RANGE, it->name, CString(vaValueStr.bstrVal), CString(vaMin.bstrVal), CString(vaMax.bstrVal) ) ) );
								}
							}
						}
						break;
				}
			}
		}
	}

	// �֘A�I�u�W�F�N�g�̃`�F�b�N
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_rows_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			(*it2)->CheckLogic( cRule, cErrs );
	}
	return cErrs.GetDefaultErrCode();
}

// �ҏW�\���ǂ������`�F�b�N����
sindyErrCode CRow::CheckEditable( sindyeEditType emType, CErrorInfos& cErrInfos ) const
{
	TCHAR strMsg[2048];

	sindyeEditableType emEditable = g_cRule.IsEditable( *this, emType, 2048, strMsg );
	if( sindyeEditableEditable != emEditable )
	{
		sindyErrCode eErrCode = sindyErr_RuleNotFound;
		switch( emType )
		{
		case sindyeEditCreate:		eErrCode = sindyErr_RuleNotAllowedCreate; break;
		case sindyeEditMove:		eErrCode = sindyErr_RuleNotAllowedMove; break;
		case sindyeEditDelete:		eErrCode = sindyErr_RuleNotAllowedDelete; break;
		case sindyeEditAttribute:	eErrCode = sindyErr_RuleNotAllowedEditAttribute; break;
		default:break;
		}
		cErrInfos.push_back( CErrorInfo( eErrCode, ( sindyeEditableWarnning == emEditable ) ? sindyErrLevel_WARN : sindyErrLevel_ERR, NULL, -1, -1, strMsg ) );
	}

	return cErrInfos.GetDefaultErrCode();
}

// �����o�ϐ�������������
void CRow::Init()
{
	m_emTableType = sindyTableType::unknown;
	m_pCache = NULL;
	m_pGeom  = NULL;
	m_lOID   = -1;
	m_bCreated = m_bDeleted = false;
	m_spNameString.reset( (CTableNameString*)NULL );
	m_spFields.reset( (CFieldMap*)NULL );
	CContainer::clear();
}

bool CRow::IsSameAttribute(const CRow& src, const CListString& cExclusions/*=CListString()*/) const
{
	// �ڑ��v���p�e�B�A�I�[�i���A�e�[�u������������
	if ( !(src.GetNameString()->IsSameSource( *GetNameString() ) &&
           (0 == _tcscmp(GetTableName(), src.GetTableName())) ) )
		return false;

	CListString cDefaultExclusions;

	cDefaultExclusions.push_back( cExclusions );
	// ���O�t�B�[���h
	cDefaultExclusions.push_back( schema::road_link::kSource );
	cDefaultExclusions.push_back( schema::road_link::kAdminCode );
	cDefaultExclusions.push_back( schema::inf_uturn::kSourceName );

	// �����e�[�u���ɏ������邩
	ITablePtr ipTable = GetTable();
	ITablePtr ipTableSrc = src.GetTable();
	bool bIsSameTable = ipTable && ipTableSrc && ( (ITable*)ipTable == (ITable*)ipTableSrc );

	long i = 0;
	for ( CFieldMap::iterator it = GetFieldMap()->begin(), itEnd = GetFieldMap()->end(); it != itEnd; ++it,++i ) 
	{
		if( std::find( cDefaultExclusions.begin(), cDefaultExclusions.end(), it->name ) != cDefaultExclusions.end() ) // ��r���O�t�B�[���h�H
			continue;

		const CFieldMap& fields = *GetFieldMap();
		if( !fields.IsSiNDYEditable( i ) ) // SiNDY�I�ɕҏW�\�ȃt�B�[���h�H
			continue;
		
		if( AheIsSindycField(fields.GetName( i )) ) // SiNDY-c�ŗL�̃t�B�[���h�H�ibug 9779�j
			continue;
		
		if( !bIsSameTable )
		{
			if( src.GetFieldMap()->HasField( fields.GetName( i ) ) == -1 ) // �����ɑ��݂���H
				continue;
		}

		CComVariant vaSource, vaDest;

		GetValueByFieldName( it->name, vaSource );
		src.GetValueByFieldName( it->name, vaDest );

		if ( vaSource != vaDest )
			return false;
	}

	return true;
}

void CRow::Trace( bool bAttribute/* = true*/, bool bGeometry/* = true*/ ) const
{
	CSPTableNameString sp = GetNameString();
	if( sp->GetPathName() )
		ATLTRACE(_T("CON[%s] TABLE[%s] OID[%d]\n"), sp->GetPathName(), sp->GetOwnerTableName(), GetOID() );
	else
		ATLTRACE(_T("CON[%s.%s@%s:%s] TABLE[%s] OID[%d]\n"), sp->GetUserName(), sp->GetVersionName(), sp->GetServerName(), sp->GetInstanceName(), sp->GetOwnerTableName(), GetOID() );

	if( bAttribute )
	{
		CFieldMap& cFields( *GetFieldMap() );
		long lFieldCount = cFields.GetFieldCount();
		for( long i = 0; i < lFieldCount; ++i )
		{
			switch( cFields.GetFieldType(i) )
			{
				case esriFieldTypeInteger:
				case esriFieldTypeSmallInteger:
				case esriFieldTypeSingle:
				case esriFieldTypeDouble:
				case esriFieldTypeString:
				case esriFieldTypeDate:
				{
					CComVariant vaValue;
					GetValue( i, vaValue );
					vaValue.ChangeType(VT_BSTR);
					ATLTRACE(_T("TABLE[%s] OID[%d] : %02d FIELD[%s] VALUE[%s]\n"), GetTableName(), GetOID(), i, cFields.GetName(i), ( VT_NULL == vaValue.vt ) ? _T("<NULL>") : CString(vaValue.bstrVal) );
				}
				default:
					break;
			}
		}
	}

	if( bGeometry )
	{
		if( (IFeaturePtr)p )
		{
			IGeometryPtr ipGeom = (IGeometry*)(*GetShape());
			CGeometry::Trace( ipGeom, _T("�`��") );
		}
	}
}

} // namespace sindy
