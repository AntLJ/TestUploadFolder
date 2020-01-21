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
 * @file RowBase.cpp
 * @brief CRowBase�N���X�����t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#include "stdafx.h"
#include <boost/foreach.hpp>
#include "type_convert.h"
#include "RowBase.h"
#include "GeometryBase.h"
#include "sindycomutil.h"
#include "TableTypeUtil.h"
#include "GlobalRules.h"
#include "GlobalFunctions.h"
#include "NameString.h"
#include "ErrorObject2.h"
#include "meshutil.h"
#include "GeometryRule.h"
#include "CacheAttribute.h"
#include "winutil.h"
#include "AheNetwork.h"
#include "RuleModel.h"
#include "AheLayerFunctions.h"
#include "sindylib_base_global.h"
#include "TimeTrace.h"
#include "RuleWrapper.h"
#include "GlobalRules.h"
#include <sindy/schema/category.h>

using namespace sindy::schema::category;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define foreach BOOST_FOREACH

#ifndef SINDY_FOR_ENGLISH
const static _TCHAR EXISTING_AND_SET_FLAG[]            = _T("���̂�����폜�t���O�y�э쐬�t���O�������Ă���");
const static _TCHAR NONFEATURE_IS_HAVING_SHAPE_CACHE[] = _T("HRESULT[%d:%X] �t�B�[�`���ł͂Ȃ����̂��`��L���b�V���������Ă��܂�");
const static _TCHAR FAIL_ADD_HISTRY[]                  = _T("����t�^�Ɏ��s���܂���");
const static _TCHAR SHAPE_CHANGE[]                     = _T("�`��ύX");
const static _TCHAR ATTR_MODIFY[]                      = _T("�����ύX");
const static _TCHAR NEW_CREATE[]                       = _T("�V�K�쐬");
const static _TCHAR DELETE_STR[]                       = _T("�폜");
const static _TCHAR EXISTED_SINDY_INTRODUCTION[]       = _T("�ڍs�������H");
const static _TCHAR FAIL_MOVE_FEATURE[]                = _T("�t�B�[�`���ړ��Ɏ��s");
const static _TCHAR NULL_CANNOT_SET[]                  = _T("NULL��ݒ肷�邱�Ƃ͂ł��܂���[%s]");
const static _TCHAR OVER_CHARNUM_LIMIT[]               = _T("�������������z���Ă��܂�[%s:%s...]:%d�o�C�g�I�[�o�[");
const static _TCHAR OVER_CHARNUM_LIMIT2[]              = _T("�������������z���Ă��܂�[%s:%s]:%d�o�C�g�I�[�o�[");
const static _TCHAR OVER_DIGITNUM_LIMIT[]              = _T("���e�������I�[�o�[���Ă��܂�[%s:%s]:���e�����F%d");
const static _TCHAR OUT_OF_RANGEDOMAIN[]               = _T("�����W�h���C���͈͊O�ł�[%s:%s]:���e�͈́F%s�`%s");
const static _TCHAR NON_EXIST_DOMAINCODE[]             = _T("�R�[�h�l�h���C���ɑ��݂��܂���[%s:%s]");
const static _TCHAR AUTO_ATTR_ASSIGNMENT_NOT_SUPPORT[] = _T("%s.%s : �T�|�[�g����Ă��Ȃ��^�ɑ΂��Ď��������t�^���悤�Ƃ��܂����B");
const static _TCHAR DELETE_INFO[]                      = _T("[�폜] TABLE[%s] : OID[%d]\n");
const static _TCHAR SAVE_INFO[]                        = _T("[�ۑ�] TABLE[%s] : OID[%d] : �ۑ������i%s�j\n");
const static _TCHAR FIELDS_IS_NOT_INITIALIZED[]        = _T("CRowBase���L���b�V������Ă���̂�m_spFields������������Ă��܂���B");
const static _TCHAR NAMESTR_IS_NOT_INITIALIZED[]       = _T("CRowBase���L���b�V������Ă���̂�m_spNameString������������Ă��܂���B");
const static _TCHAR FAIL_PUTREF_SHAPE[]                = _T("IFeature::putref_Shape()�Ɏ��s");
const static _TCHAR FAIL_GET_SHAPECOPY[]               = _T("IFeature::get_ShapeCopy() : ���s���܂���");
const static _TCHAR MAY_BE_NULL_SHAPE[]                = _T("NULL Shape �̉\��������܂�");
const static _TCHAR TIME_CREATE_CACHE_ATTR[]           = _T("CreateCache:����");
const static _TCHAR TIME_CREATE_CACHE_SHAPE[]          = _T("CreateCache:�`��");
const static _TCHAR TIME_CREATE_CACHE_RELEASE[]        = _T("CreateCache:�؂藣��");
#else
const static _TCHAR EXISTING_AND_SET_FLAG[]            = _T("It is existing, and create flag and delete flag is set");
const static _TCHAR NONFEATURE_IS_HAVING_SHAPE_CACHE[] = _T("HRESULT[%d:%X] Non-feature is having the shape cache");
const static _TCHAR FAIL_ADD_HISTRY[]                  = _T("Failed to add history");
const static _TCHAR SHAPE_CHANGE[]                     = _T("Shape change");
const static _TCHAR ATTR_MODIFY[]                      = _T("Attribute modification");
const static _TCHAR NEW_CREATE[]                       = _T("Create new");
const static _TCHAR DELETE_STR[]                       = _T("Delete");
const static _TCHAR EXISTED_SINDY_INTRODUCTION[]       = _T("Existed at the time of SiNDY introduction? ");
const static _TCHAR FAIL_MOVE_FEATURE[]                = _T("Failed to move feature");
const static _TCHAR NULL_CANNOT_SET[]                  = _T("Null cannot be set [%s]");
const static _TCHAR OVER_CHARNUM_LIMIT[]               = _T("Over the character number limit [%s:%s...]:%d bytes excess ");
const static _TCHAR OVER_CHARNUM_LIMIT2[]              = _T("Over the character number limit [%s:%s]:%d bytes excess ");
const static _TCHAR OVER_DIGITNUM_LIMIT[]              = _T("Number of digit is over the allowable number of digit s[%s:%s]:allowable number of digits:%d");
const static _TCHAR OUT_OF_RANGEDOMAIN[]               = _T("Out of range of the range domain[%s:%s]:allowable range:%s-%s");
const static _TCHAR NON_EXIST_DOMAINCODE[]             = _T("Does not exist in the code value domain[%s:%s]");
const static _TCHAR AUTO_ATTR_ASSIGNMENT_NOT_SUPPORT[] = _T("%s.%s : Automatic attribute assignment tried to adjust the model which is not supported.");
const static _TCHAR DELETE_INFO[]                      = _T("[Delete] TABLE[%s] : OID[%d]\n");
const static _TCHAR SAVE_INFO[]                        = _T("[Save] TABLE[%s] : OID[%d] : Save finished�i%s�j\n");
const static _TCHAR FIELDS_IS_NOT_INITIALIZED[]        = _T("CRowBase is having cache, but m_spFields is not initialized.");
const static _TCHAR NAMESTR_IS_NOT_INITIALIZED[]       = _T("CRowBase is having cache, but m_spNameString is not initialized.");
const static _TCHAR FAIL_PUTREF_SHAPE[]                = _T("IFeature::putref_Shape() failed.");
const static _TCHAR FAIL_GET_SHAPECOPY[]               = _T("IFeature::get_ShapeCopy() : failed");
const static _TCHAR MAY_BE_NULL_SHAPE[]                = _T("It may be NULL Shape.");
const static _TCHAR TIME_CREATE_CACHE_ATTR[]           = _T("CreateCache:Attribute");
const static _TCHAR TIME_CREATE_CACHE_SHAPE[]          = _T("CreateCache:Shape");
const static _TCHAR TIME_CREATE_CACHE_RELEASE[]        = _T("CreateCache:Release");
#endif	// SINDY_FOR_ENGLISH

using namespace winutil;
namespace sindy {
using namespace errorcode;

CRowBase::CRowBase() : 
  CSchemaSupport(),
  m_pCache(NULL),
  m_pGeom(NULL),
  m_lOID(-1),
  m_bCreated(false),
  m_bDeleted(false)

{
}

 CRowBase::CRowBase( const CRowBase& obj ) :
	CSchemaSupport(obj),
	m_lOID(obj.m_lOID),
	m_bCreated(obj.m_bCreated),
	m_bDeleted(obj.m_bDeleted)
{
	m_pCache = ( obj.m_pCache ) ? new CCacheAttribute(*obj.m_pCache) : NULL;
	m_pGeom = ( obj.m_pGeom ) ? new CGeometryBase(*obj.m_pGeom) : NULL;
	AtlComPtrAssign( (IUnknown**)&p, (_IRow*)obj ); // Release���ĂԂƃL���b�V����m_lOID�������������̂�NG
}

CRowBase::CRowBase( _IRow* lp, sindyTableType::ECode emTableType/* = sindyTableType::unknown*/, bool bCreated/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ ) :
	CSchemaSupport( emTableType, spNameString, spFields ),
	m_pCache(NULL),
	m_pGeom(NULL),
	m_lOID(-1),
	m_bCreated(bCreated),
	m_bDeleted(false)
{
	SetObject( lp, emTableType, bCreated, spFields, spNameString );
}

CRowBase& CRowBase::operator=( const CRowBase& obj )
{
	operator=( obj.p );

	// �X�L�[�}�����R�s�[
	CSchemaSupport::operator=( obj );
	// �L���b�V���������R�s�[
	m_pCache = ( obj.m_pCache ) ? new CCacheAttribute(*obj.m_pCache) : NULL;
	m_pGeom = ( obj.m_pGeom ) ? new CGeometryBase(*obj.m_pGeom) : NULL;
	m_lOID = obj.m_lOID;
	m_bCreated = obj.m_bCreated;
	m_bDeleted = obj.m_bDeleted;

	return *this;
}

CRowBase& CRowBase::operator=( _IRow* lp )
{
	AtlComPtrAssign( (IUnknown**)Release(), lp );

	return *this;
}

CRowBase::operator IGeometry*() const
{
	if( m_pGeom )
		return (IGeometry*)(*m_pGeom);
	else {
		IFeaturePtr ipFeat( p );
		if( ipFeat )
		{
			IGeometryPtr ipGeom;
			ipFeat->get_Shape( &ipGeom );
			return ipGeom;
		}
	}
	return (IGeometry*)NULL;
}

bool CRowBase::operator <( const CRowBase& obj )
{
	if( p && obj.p )
		return ( GetOID() < obj.GetOID() && *GetNameString() < *obj.GetNameString() );
	return false;
}

// �t�B�[�`���E���R�[�h���Z�b�g����
void CRowBase::SetObject( _IRow* lp, sindyTableType::ECode emTableType/* = sindyTableType::unknown*/, bool bCreated/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNames/* = CSPTableNameString()*/ )
{
	// �����̂��͎̂̂Ă�
	DeleteCache();
	// �����o�ϐ�������
	Init();

	// attach
	AtlComPtrAssign((IUnknown**)&p, lp);
	m_emTableType = emTableType;
	m_bCreated = bCreated;	// �쐬�t���O
	if( spFields )
		SetFieldMap( spFields );
	if( spNames )
		SetNameString( spNames );
}

// �_�~�[CRowBase�����̉�������
void CRowBase::Materialization( _IRow* lp )
{
	_ASSERTE( ! p );        // ���̂�������̂ɑ΂��ď������悤�Ƃ��Ă���
	_ASSERTE( m_bCreated ); // �V�K�쐬�t���O���Ȃ����̂ɑ΂��ď������悤�Ƃ��Ă���

	AtlComPtrAssign((IUnknown**)&p, lp);
}

// �t�B�[�`���̃R�s�[���쐬����
CSPRowBase CRowBase::Clone() const
{
	CSPRowBase cSPRowBase( new CRowBase() );	// �Ԃ�l
	CRowBase& cRowBase( *cSPRowBase );

	if( p )
	{
		// �ҏW�J�n����Ă��邩�ǂ���
		IWorkspaceEditPtr ipWork( GetWorkspace() );
		VARIANT_BOOL vbIsBeingEdited = VARIANT_FALSE;
		ipWork->IsBeingEdited( &vbIsBeingEdited );
		LOGASSERTE_IF( vbIsBeingEdited, sindyErr_EditNotStarted )
		{
			// �쐬
			ITablePtr ipTable( GetTable() );
			LOGASSERTE_IF( SUCCEEDED( ipTable->CreateRow( cRowBase.Release() ) ), sindyErr_RowCreateFailed )
			{
				cRowBase.m_bCreated = true;	// �쐬�t���O�𗧂ĂĂ����i�����������ׁ݂̈j

				// �e�[�u�����L���b�V���A�t�B�[���h���Ή��\�R�s�[
				(CSchemaSupport&)cRowBase = (const CSchemaSupport&)*this;
				// �����E�`����R�s�[
				cRowBase.m_pCache = new CCacheAttribute( ( m_pCache ) ? *m_pCache : (_IRow*)*this );
				cRowBase.m_pGeom = new CGeometryBase( GetShapeCopy() );
			}
		}
	}
	return cSPRowBase;
}

// �t�B�[�`���̕ύX�����ɖ߂�
void CRowBase::Reset()
{
	// �폜�t���O���N���A����Ȃ�����
	// �Ep���폜�ς݂̏ꍇ
	// �E�폜�t���O�������Ă���Ap�����݂��Ȃ�
	//
	// ��L�ȊO�͍ŏI�I�ɍ쐬�E�폜�t���O������������
	// 
	// DummyRowBase�ɂ��Ȃ���΂Ȃ�Ȃ�����
	// �E�폜�t���O�������Ă��炸�Ap�����݂��Ȃ�

	// p���폜�ς݂��ǂ�����OJBECTID������āA�G���[���o�邩�ǂ����Ŋm�F
	bool bAlreadyDeleted = false;
	if( p )
	{
		CComVariant vaValue;
		if( FAILED( Fix_BSTR_get_Value( p, 1, &vaValue ) ) )
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
	// CreateDummyRowBase�ł���Ă��鑮���Z�b�g���s��
	// p�����݂��Ȃ��ꍇ�͑��݂��Ȃ��������Ƃɂ��Ȃ����
	// �Ȃ�Ȃ��̂ŁA�폜�t���O�𗧂ĂĂ���
	if( m_bCreated )
	{
		if( p )
		{
			// CTable::CreateDummyRowBase������
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

IFieldsPtr CRowBase::GetFields() const
{
	IFieldsPtr ipFields;
	if( p ) {
		INVOKEMETHOD( get_Fields( &ipFields ) );
	}
	return ipFields;
}

bool CRowBase::HasOID() const
{
	// m_lOID��-1�̏ꍇ�͖��擾�Ȃ̂ŁA���ۂɒ��ׂ�
	if( -1 == m_lOID )
	{
		VARIANT_BOOL vbHasOID = VARIANT_FALSE;
		if( p ) {
			INVOKEMETHOD( get_HasOID( &vbHasOID ) );
		}
		return VB2bool(vbHasOID);
	}
	else
		return ( m_lOID < 0 ) ? false : true;
}

long CRowBase::GetOID() const
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

void CRowBase::SetDefaultValue()
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
			}
		}
	}
}

// �V�K�쐬���̎����t�^������t�^����
void CRowBase::SetCreateAutoValue()
{
	// ���O�C�����Ă���ꍇ�ɂ͐V�K�쐬���̎����t�^����
	if( g_cRule && g_cRule->IsLogined() )
	{
		const CAutoValue& cValue( g_cRule->GetCreatedAutoValue(GetTableName()) );
		CSPFieldMap cFields( GetFieldMap() );
		for( CAutoValue::const_iterator it = cValue.begin(); it != cValue.end(); ++it )
		{
			long lFieldIndex = cFields->FindField( it->first );
			if( 0 < lFieldIndex )
			{
				switch( cFields->GetFieldType( lFieldIndex ) )
				{
				case esriFieldTypeInteger:
				case esriFieldTypeSmallInteger: SetValue( lFieldIndex, _ttol(it->second) ); break;
				case esriFieldTypeString: SetValue( lFieldIndex, it->second ); break;
				default:
					_ERRORLOG( sindyErr_AlgorithmFailed, AUTO_ATTR_ASSIGNMENT_NOT_SUPPORT, GetTableName(), it->first );
					break;
				}
			}
		}
	}
}

sindyErrCode CRowBase::Store( CErrorObjectsBase& cErrs )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	if ( ! p )	// ���̂��Ȃ����̂̓X���[
		return cErrs.GetDefaultErrCode();

	CSPErrorObjectBase pErr;
	// �G���[�I�u�W�F�N�g�Ɏ������g��o�^
	if( g_bCreateAllStoreInfo ) // �C���|�[�g���ɂ͑S�Ẵ��R�[�h�����G���[�I�u�W�F�N�g�ɓo�^����K�v�͂Ȃ��i���Ȃ莞�Ԃ�������c�j
		pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );

	// ���̂�����̂ɍ쐬�t���O�y�э폜�t���O�������Ă��邱�Ƃ͂��肦�Ȃ�
	if( IsCreated() && IsDeleted() )
	{
		if( ! pErr )
			pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
		pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AlgorithmFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("%s"), EXISTING_AND_SET_FLAG ) ) );
		return cErrs.GetDefaultErrCode();
	}

	schema::ipc_table::update_type::ECode emUpdate = schema::ipc_table::update_type::kDefault;

	// �폜�̏ꍇ
	HRESULT hr = S_OK;
	if( IsDeleted() )
	{
		GetOID(); // �폜�����Ƃł��Ȃ��Ȃ�̂�m_lOID�ɃL���b�V�����Ă����im_lOID��DeleteCache�ł͏����Ȃ��j

		// �L���b�V���폜
		DeleteCache();
		// �폜
		if( SUCCEEDED( hr = p->Delete() ) )
		{
#ifdef _DEBUG
			if( g_bOutputTraceUpdate || g_bTraceUpdate )
#else
			if( g_bOutputTraceUpdate )
#endif // ifdef _DEBUG
			{
				if( g_bOutputTraceUpdate )
					TRACEMESSAGE( DELETE_INFO, GetTableName(), m_lOID );
#ifdef _DEBUG
				else
					ATLTRACE( _T("[�폜] TABLE[%s] : OID[%d]\n"), GetTableName(), m_lOID );
#endif // ifdef _DEBUG
			}
		}
		else {
			if( ! pErr )
				pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
			pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AODeleteFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X] MSG[%s]"), hr, hr, GETCOMERRORSTRING() ) ) );
		}
		return cErrs.GetDefaultErrCode();
	}

	bool bAttrUpdated = false; // UPDATE_C�X�V�p
	// �폜�ȊO�̏ꍇ
	if( m_pCache || m_pGeom )
	{
		// ����
		// �L���b�V��������ꍇ�̓L���b�V�����ɔ��f������
		if( m_pCache )
		{
			// �L���b�V�����f
			const CFieldMap& cFields( *GetFieldMap() );
			long i = -1;
			foreach( const CFieldMap::_FIELD& field, std::make_pair( cFields.begin(), cFields.end() ) )
			{
				++i;
				// �ҏW�ł��Ȃ��t�B�[���h�͔�΂�
				if( field.editable && esriFieldTypeGeometry != field.type )
				{
					VARIANT& va = *(*m_pCache)[i];
					// EMPTY�̏ꍇ�͕ҏW�s�̃t�B�[���h���A���̓N�G���t�B���^�ŏȂ��ꂽ����
					if( va.vt != VT_EMPTY )
					{
						// �ύX������ꍇ�̂�put_Value����i�����ύX�̂݁j
						bool update = ( m_bCreated )  ? true : false; // �V�K�쐬�̎��͕K���Z�b�g����̂Ŕ�r���Ȃ�
						if( ! update )
						{
							_variant_t vaValue;
							if( SUCCEEDED( hr = Fix_BSTR_get_Value( p, i, &vaValue ) ) )
							{
								if( ! IsEqualSDEValue( field.type, va, vaValue ) )
									update = true;
							}
							else {
								if( ! pErr )
									pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
								pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AOGetValueFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) ) );
							}
						}
						if( update )
						{
							if( SUCCEEDED( hr = p->put_Value( i, va ) ) )
							{
								emUpdate = schema::ipc_table::update_type::kAttrUpdated;
								bAttrUpdated = true; // �����X�V�͌`��X�V�ŏ㏑������邽�߁AUPDATE_C�𐳂����X�V���邽�߂ɂ͕ێ�����K�v����
							}
							else {
								CComVariant vaErr( va );
								vaErr.ChangeType(VT_BSTR);
								if( VT_NULL == vaErr.vt )
									vaErr = _T("NULL");
								if( ! pErr )
									pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
								pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AOPutValueFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X] FIELD[%s] VALUE[%s] MSG[%s]"), hr, hr, cFields.GetName(i), (LPCTSTR)CString(vaErr.bstrVal), GETCOMERRORSTRING() ) ) );
								Trace( true, false );
							}
						}
					}
				}
			}
		}
		// �`��
		if( m_pGeom )
		{
			IFeaturePtr ipFeat(p);
			if( ipFeat )
			{
				// �ύX������ꍇ�̂�putref_Shape����i�����ύX�̂݁j
				bool update = ( m_bCreated )  ? true : false; // �V�K�쐬�̎��͕K���Z�b�g����̂Ŕ�r���Ȃ�
				if( ! update )
				{
					IGeometryPtr ipGeom;
					if( SUCCEEDED( hr = ipFeat->get_Shape( &ipGeom ) ) )
					{
						// ���胋�[�`���� IRelationalOperator::Equals���g��
						if( g_bStoreShapeJudge_Native )
						{
							IRelationalOperatorPtr ipRel( ipGeom );
							if( ipRel )
							{
								VARIANT_BOOL vaEqual = VARIANT_FALSE;
								ipRel->Equals( (IGeometry*)*m_pGeom, &vaEqual );
								if( ! vaEqual )
									update = true;
							}
						}
						// ���胋�[�`���ɍ\���_��r�iCGeometry::EqualsVertex�j���g��
						else if( g_bStoreShapeJudge_Exact )
						{
							if( ! m_pGeom->EqualsVertex( ipGeom ) )
								update = true;
						}
						// ���胋�[�`���� CGeoemtry::Equals�iIClone::IsEqual�j���g���{��
						else {
							if( ! m_pGeom->Equals( ipGeom ) )
								update = true;
						}
					}
					else {
						if( ! pErr )
							pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
						pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_GeometryNotFound, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) ) );
					}
				}
				if( update )
				{
					if( SUCCEEDED( hr = ipFeat->putref_Shape( (IGeometry*)*m_pGeom ) ) )
						emUpdate = schema::ipc_table::update_type::kShapeUpdated;
					else {
						if( ! pErr )
							pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
						pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AOPutValueFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X]"), hr, hr ) ) );
						AheTraceGeometry( (IGeometry*)*m_pGeom, _T("putref_Shape���s") );
					}
				}
			}
			else {
				if( ! pErr )
					pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
				pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AlgorithmFailed, sindyErrLevel_ERR, NULL, 0, -1, NONFEATURE_IS_HAVING_SHAPE_CACHE, hr, hr ) ) );
			}
		}
	}

	if( m_bCreated ) emUpdate = schema::ipc_table::update_type::kCreated;

	// �L���b�V���폜
	DeleteCache();

	// �V�K�쐬/�폜�ȊO�̓L���b�V���쐬�O�̕ύX�m�F�i�L���b�V�����������ꍇ�͓�x��ԂɂȂ邯�ǂ��傤���Ȃ��c�j
	// ���\���Ԃ������鏈���̂悤�Ȃ̂ŁA�O���t���O�Ő���
	if( g_bCheckUpdatedByArcObjects ) // �L���b�V���쐬�O�̕ύX���`�F�b�N���邩�ǂ���
	{
		if( ! ( schema::ipc_table::update_type::kCreated == emUpdate ||
			schema::ipc_table::update_type::kDeleted == emUpdate ) )
			Changed( &emUpdate );
	}

	if( sindyErr_NoErr == emErr )
	{
		if( schema::ipc_table::update_type::kDefault != emUpdate )
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
					if( sindyErr_NoErr == ( emErr = SetHistory( emUpdate, bAttrUpdated ) ) )
						bStore = true;
					else 
					{
						if( ! pErr )
							pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
						pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( emErr, sindyErrLevel_ERR, NULL, 0, -1, FAIL_ADD_HISTRY ) ) );
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
					if( g_bTraceUpdate || g_bOutputTraceUpdate )
#else
					if( g_bOutputTraceUpdate )
#endif // ifdef _DEBUG
					{
						CString cStr;
						switch( emUpdate )
						{
						case schema::ipc_table::update_type::kShapeUpdated: cStr = SHAPE_CHANGE; break;
						case schema::ipc_table::update_type::kAttrUpdated:  cStr = ATTR_MODIFY; break;
						case schema::ipc_table::update_type::kCreated:      cStr = NEW_CREATE; break;
						case schema::ipc_table::update_type::kDeleted:      cStr = DELETE_STR; _ASSERTE( schema::ipc_table::update_type::kDeleted != emUpdate ); break; // �폜�͂������Ȃ��͂�
						default: cStr = EXISTED_SINDY_INTRODUCTION; break;
						}
						if( g_bOutputTraceUpdate )
							TRACEMESSAGE( SAVE_INFO, GetTableName(), GetOID(), cStr );
#ifdef _DEBUG
						else
							ATLTRACE( _T("[�ۑ�] TABLE[%s] : OID[%d] : �ۑ������i%s�j\n"), GetTableName(), GetOID(), cStr );
#endif // ifdef _DEBUG
					}
				}
				else {
					if( ! pErr )
						pErr = cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this ) ) );
					pErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AOStoreFailed, sindyErrLevel_ERR, NULL, 0, -1, _T("HRESULT[%d:%X] MSG[%s]"), hr, hr, GETCOMERRORSTRING() ) ) );
					Trace();
					// �t�B�[���h�̌����ӂ���`�F�b�N���Ă���
					CRowBase::CheckLogic( CLogicRule(), cErrs );
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
			ATLTRACE( _T("%s : TABLE[%s] : OID[%d] : ���ۑ��i�ύX�����j\n"), __func__, GetTableName(), GetOID() );
#endif // ifdef _DEBUG
	}

	// �Ō��Cerate�t���O��false�ɂ��Ă���
	m_bCreated = false;

	return cErrs.GetDefaultErrCode();
}

// �R���t���N�g����̂��߂̊��ҏW�`�F�b�N���s��
sindyErrCode CRowBase::CheckModify( CErrorObjectsBase& cErrs ) const
{
	if ( (_IRow*)p ) {	// ���̂�������̂̂�
		if ( Changed() ) {

			// �������g
			CString strMsg;
			if( g_cRule && g_cRule->IsModified( *this, strMsg ) )
			{
				CSPErrorObjectBase cErr( new CErrorObjectBase( *this ) );
				cErr->push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AlreadyModified, sindyErrLevel_ERR, NULL, -1, -1, _T("%s"), strMsg ) ) );
				cErrs.push_back( cErr );
			}
		}
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

sindyErrCode CRowBase::SetHistory( schema::ipc_table::update_type::ECode emCode, bool bAttrUpdated/* = false */ )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	const CFieldMap& cFields( *GetFieldMap() );
	long lUpdateTypeIndex =cFields.HasField( schema::ipc_table::kUpdateType );
	if( 0 < lUpdateTypeIndex ) SetValue( lUpdateTypeIndex, emCode );					// �X�V�^�C�v

	if( g_cRule && g_cRule->IsLogined() )
	{
		long lOperatorIndex = cFields.HasField( schema::ipc_table::kOperator );
		long lModifyDateIndex = cFields.HasField( schema::ipc_table::kModifyDate );
		long lPurposeIndex = cFields.HasField( schema::ipc_table::kPurpose );
		if( 0 < lOperatorIndex ) SetValue( lOperatorIndex, g_cRule->GetOperatorName() );	// �I�y���[�^��
		if( 0 < lModifyDateIndex ) SetValue( lModifyDateIndex, GetTime() );				// �ŏI�X�V����
		if( 0 < lPurposeIndex ) SetValue( lPurposeIndex, g_cRule->GetWorkPurposeID() );		// ��ƖړIID
	}
	else {
		long lModifyProgNameIndex = cFields.HasField( schema::ipc_table::kModifyProgName );
		long lProgModifyDateIndex = cFields.HasField( schema::ipc_table::kProgModifyDate );
		if( 0 < lModifyProgNameIndex )
		{
			TCHAR szBuffer[_MAX_PATH] = {};
			::GetModuleFileName(NULL, szBuffer, _MAX_PATH);
			LPCTSTR pos = ::_tcsrchr(szBuffer, _T('\\'));
			SetValue( lModifyProgNameIndex,  pos ? pos + 1 : szBuffer );	// �X�V�v���O������
		}
		if( 0 < lProgModifyDateIndex ) SetValue( lProgModifyDateIndex, GetTime() );				// �o�b�`�����ł̍ŏI�X�V����
	}
	// SiNDY-c�X�V�^�C�v
	// PGDB�ҏW���[�h�ɂ�炸����΍X�V�ibug 5777,10744�j
	long lUpdateCIndex = cFields.HasField( sindyc_table::kUpdateC );
	if( 0 < lUpdateCIndex )
	{
		SetValue( lUpdateCIndex, update_type2update_code( emCode ) | GetValue( lUpdateCIndex, 0L ) ); // �X�V�^�C�v�iPGDB�ҏW���[�h�p�j�ibug 5201�j
		// �`��X�V�̏ꍇ�ɂ́A�����X�V�ł�����ꍇ����
		// �Ȃ̂ŁA�`��X�V�̏ꍇ�̂�bAttrUpdated�t���O�����Ēǉ��Ή�����
		if( schema::ipc_table::update_type::kShapeUpdated == emCode && bAttrUpdated )
			SetValue( lUpdateCIndex, 2L | GetValue( lUpdateCIndex, 0L ) );
	}

	// �����t�^����
	emErr = SetAutoValue();

	return emErr;
}

// �����t�^������t�^����
sindyErrCode CRowBase::SetAutoValue()
{
	if( ! ( g_cRule && g_cRule->IsLogined() ) )
		return sindyErr_NoErr;

	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	const CFieldMap& cFields( *GetFieldMap() );
	const CAutoValue& cAutoValue( g_cRule->GetAutoValue() );
	for( CAutoValue::const_iterator it = cAutoValue.begin(); it != cAutoValue.end(); ++it )
	{
		long lFieldIndex = cFields.HasField( it->first );
		if( 0 < lFieldIndex ) 
		{
			switch( cFields.GetFieldType(lFieldIndex) )
			{
			case esriFieldTypeInteger:
			case esriFieldTypeSmallInteger: SetValue( lFieldIndex, _ttol(it->second) ); break;
			case esriFieldTypeString: SetValue( lFieldIndex, it->second ); break;
			default:
				_ERRORLOG( emErr = sindyErr_AlgorithmFailed, AUTO_ATTR_ASSIGNMENT_NOT_SUPPORT, GetTableName(), it->first );
				break;
			}
		}
	}

	return emErr;
}

ITablePtr CRowBase::GetTable() const
{
	ITablePtr ipTable;
	if( p ) {
		INVOKEMETHOD( get_Table( &ipTable ) );
	}
	return ipTable;
}

IWorkspacePtr CRowBase::GetWorkspace() const
{
	IWorkspacePtr ipRet;	// �Ԃ�l

	IDatasetPtr ipDataset( GetTable() );
	if( ipDataset )
	{
		LOGASSERTE_IF( SUCCEEDED( ipDataset->get_Workspace( &ipRet ) ), sindyErr_AOGetWorkspaceFailed );
	}

	return ipRet;
}

sindyErrCode CRowBase::GetValue( long lIndex, VARIANT& vaValue ) const
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
			if( p )
			{
				HRESULT hr;	// [bug 3182] ���܂ɂ����ŃG���[���o��̂ŁA�����̂��߂ɏڍ׃��O���o��
				SUCCEEDED_IF( hr = Fix_BSTR_get_Value( p, lIndex, &vaValue ), emErr = sindyErr_AOGetValueFailed, _T("HRESULT[%ld(0x%X)] TABLE[%s] OID[%ld] lIndex[%ld]"), hr, hr, GetTableName(), GetOID(), lIndex )
				{
				}
			}
			else
				emErr = sindyErr_ClassMemberMissing;
		}
	}
	else
		emErr = sindyErr_ArgLimitOver;	// �C���f�b�N�X��0�ȏ�

	return emErr;
}

sindyErrCode CRowBase::SetValue( long lIndex, const VARIANT& vaValue )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	if( lIndex >= 0 )
	{
		// �L���b�V������
		if( m_pCache )
		{
			if( VT_BSTR == vaValue.vt && 0 == wcslen( vaValue.bstrVal ) )
			{
				CComVariant va;
				va.vt = VT_NULL;
				if( FAILED( VariantCopy( (*m_pCache)[lIndex], (VARIANTARG*)&va ) ) )	// const�Ŏ󂯎���Ă���Ȃ��̂Ŗ������L���X�g
					emErr = sindyErr_MemoryAllocationFailed;	// �K�؂ȕԂ�l�ł͂Ȃ�����
			}
			else {
				if( FAILED( VariantCopy( (*m_pCache)[lIndex], (VARIANTARG*)&vaValue ) ) )	// const�Ŏ󂯎���Ă���Ȃ��̂Ŗ������L���X�g
					emErr = sindyErr_MemoryAllocationFailed;	// �K�؂ȕԂ�l�ł͂Ȃ�����
			}
		}
		// �L���b�V���Ȃ�
		else {
			if( p ) {
				INVOKEMETHOD( put_Value( lIndex, vaValue ) );
				emErr = (FAILED(hr)) ? sindyErr_AOPutValueFailed : sindyErr_NoErr;
			}
			else
				emErr = sindyErr_ClassMemberMissing;
		}
	}
	else
		emErr = sindyErr_ArgLimitOver;	// �C���f�b�N�X��0�ȏ�

	return emErr;
}

// �l��NULL���ǂ����m�F����
bool CRowBase::IsNull( long lIndex ) const
{
	CComVariant va;
	GetValue( lIndex, va );
	return ( VT_NULL == va.vt || VT_EMPTY == va.vt );
}

//�t�B�[���h�Ή��\�̎Q�Ƃ�Ԃ�
CSPFieldMap CRowBase::GetFieldMap() const
{
	if( p && ( ! CSchemaSupport::GetFieldMap() ) )
	{
		// �L���b�V������Ă��Ȃ��Ȃ烁���o�������ŏ���������
		// ���������ƁACRowBase����CTableNameString�̎��̂������ƂɂȂ�̂�
		// �{���͍D�܂����Ȃ��B�������A���֐����d�����A�����ō쐬�ł���悤��
		// ���Ă����B�������A�f�o�b�O�g���[�X�͏o���ĊJ���҂ɂ͂킩��悤��
		// ���Ă������ƁB
		LOGASSERTEM_IF( ! IsCached(), sindyErr_ClassMemberMissing, FIELDS_IS_NOT_INITIALIZED )
		{
			IFieldsPtr ipFields;
			LOGASSERTE_IF( NULL != ( ipFields = GetFields() ), sindyErr_COMFunctionFailed )
			{
				ATLTRACE(_T("%s : CFieldMap cache created.\n"), __func__ );
				return CSchemaSupport::GetFieldMap( ipFields );
			}
		}
	}

	return CSchemaSupport::GetFieldMap();
}

// �e�[�u�����Ȃǂ��Ǘ����郁���o�̎Q�Ƃ�Ԃ�
CSPTableNameString CRowBase::GetNameString() const
{
	if( p && ( ! CSchemaSupport::GetNameString() ) )
	{
		// �L���b�V������Ă��Ȃ��Ȃ烁���o�������ŏ���������
		// ���������ƁACRowBase����CTableNameString�̎��̂������ƂɂȂ�̂�
		// �{���͍D�܂����Ȃ��B�������A���֐����d�����A�����ō쐬�ł���悤��
		// ���Ă����B�������A�f�o�b�O�g���[�X�͏o���ĊJ���҂ɂ͂킩��悤��
		// ���Ă������ƁB
		LOGASSERTEM_IF( ! IsCached(), sindyErr_ClassMemberMissing, NAMESTR_IS_NOT_INITIALIZED )
		{
			ITablePtr ipTable;
			LOGASSERTE_IF( NULL != ( ipTable = GetTable() ), sindyErr_COMFunctionFailed )
			{
				return CSchemaSupport::GetNameString( ipTable );
			}
		}
	}

	return CSchemaSupport::GetNameString();
}

bool CRowBase::IsEqualObject(_IRow* ipRowBase ) const
{
	if( p )
	{
		// �܂��́AOID�݂̂Ŕ�r����i���x�D��j
		if( AheGetOID( ipRowBase ) == GetOID() )
		{
			ITablePtr ipTable;
			ipRowBase->get_Table( &ipTable );
			CTableNameString cNames( ipTable );
			return ( cNames == *GetNameString() );
		}
	}
	return false;
}

bool CRowBase::IsEqualObject( const CRowBase& cRowBase ) const
{
	if( cRowBase.p && p ) // DummyRowBase�̏ꍇ��p�����݂��Ȃ�
		return ( GetOID() == cRowBase.GetOID() && *GetNameString() == *cRowBase.GetNameString() );
	return false;
}

// �L���b�V�����쐬����
sindyErrCode CRowBase::CreateCache( bool bDetach/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CRowBase::CreateCache()"));

	double dStart = GetTickCount();	// ���Ԍv���p

	if( spFields )
		SetFieldMap( spFields );
	if( spNameString )
		SetNameString( spNameString );

	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	g_trace.Start(TIME_CREATE_CACHE_ATTR);
	// �����̃L���b�V�����Ȃ��ꍇ�ɂ̂ݍ쐬
	// �����̃L���b�V��
	if( ! m_pCache )
	{
		// OBJECTID�����͂����Ŏ擾
		if( p )
			m_lOID = GetOID();
		// �Ή��\�쐬�ŃG���[��������΃L���b�V�����쐬����
		if( GetFieldMap() )
		{
			m_pCache = new CCacheAttribute();
			if( m_pCache )
				emErr = m_pCache->CreateCache( p, *GetFieldMap() );
			else
				emErr = sindyErr_MemoryAllocationFailed;

		}
		else {
			emErr = sindyErr_FieldMapNotCreated;	// �t�B�[���h�Ή��\���Z�b�g����Ă��Ȃ�
			ERRORLOG(emErr);
			SASSERT( GetFieldMap() );
		}
	}
	g_trace.End(TIME_CREATE_CACHE_ATTR);
	g_trace.Start(TIME_CREATE_CACHE_SHAPE);
	// �`��̃L���b�V��
	if( ! m_pGeom )
	{
		if( 0 < m_spFields->GetShapeIndex() ) // p��NULL�̏ꍇ����ibug 5421�j���A�G���[�Ƃ͂��Ȃ��iCTable::CreateDummyRowBase�̂��߁j
		{
			IGeometryPtr ipGeom;
			IFeaturePtr ipFeat( p );
			if( ipFeat )
			{
				LOGASSERTEM_IF( SUCCEEDED(ipFeat->get_ShapeCopy( &ipGeom )), sindyErr_GeometryNotFound, _T("%s"), FAIL_GET_SHAPECOPY )
				{
				}
			}
			m_pGeom = new CGeometryBase( ipGeom );
		}
	}
	g_trace.End(TIME_CREATE_CACHE_SHAPE);

	g_trace.Start(TIME_CREATE_CACHE_RELEASE);
	// �؂藣��
	if( bDetach )
	{
		if( p )
		{
			p->Release();
			p = NULL;
		}
	}

	// �L���b�V�����Ԃ��v�Z
	g_dTotalCacheTime += (double)GetTickCount()-dStart;
	g_trace.End(TIME_CREATE_CACHE_RELEASE);

	return emErr;
}

// �L���b�V�����폜����
void CRowBase::DeleteCache(/* bool bDeleteContainerCache*//* = false*/ )
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
	// m_lOID��DeleteCache�ł͍폜���Ȃ��i�d�l�I�I�j
}

// �L���b�V������Ă��邩�ǂ������`�F�b�N����
bool CRowBase::IsCached() const
{
	bool bRet = false;	// �Ԃ�l

	// �������L���b�V������Ă��邩�ǂ���
	if( m_pCache )
		bRet = true;

	// �`�󂪃L���b�V������Ă��邩�ǂ���
	if( ! m_pGeom )
	{
		if( bRet )
			bRet = false;
	}

	return bRet;
}

// �R�s�[����
sindyErrCode CRowBase::CopyTo( CRowBase& dest, bool bIsCopyCache/*=true*/, bool bIsCopyRowBase/*=true*/, bool bIsCopyFlag/*=true*/, bool bIsCopyHistory/* = true*/, bool bIsCopySchema/*=true*/ ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	if ( bIsCopyRowBase ) {
		AtlComPtrAssign( (IUnknown**)dest.Release(), (_IRow*)*this );	// IRow(QI)
		// OID���R�s�[����
		dest.m_lOID = m_lOID;
	}

	if ( bIsCopyCache ) {
		// �������R�s�[����i�����̃L���b�V����dest.Release()�ō폜�����̂ł���Ȃ��j
		if( m_pCache )
		{
			static std::list<CString> g_listHistory;
			std::map<CString,CComVariant> mapHistory;
			if ( dest.m_pCache )
			{
				// �����R�s�[���Ȃ��ꍇ�͌��̗������o�b�N�A�b�v
				if( ! bIsCopyHistory )
				{
					if( g_listHistory.empty() )
					{
						g_listHistory.push_back( schema::ipc_table::kOperator );
						g_listHistory.push_back( schema::ipc_table::kModifyDate );
						g_listHistory.push_back( schema::ipc_table::kPurpose );
						g_listHistory.push_back( schema::ipc_table::kUpdateType );
						g_listHistory.push_back( schema::ipc_table::kProgModifyDate );
						g_listHistory.push_back( schema::ipc_table::kModifyProgName );
						g_listHistory.push_back( schema::ipc_table::kUserClaim ); // ������܂߂�ƁA��������Ȃ��� ipc_table ����
					}
					const CFieldMap& fields = *dest.GetFieldMap();
					foreach( const CString& name, g_listHistory )
					{
						long index = fields.HasField(name);
						if( 0 <= index )
							mapHistory[name] = dest.GetValue(index);
					}
				}
				delete dest.m_pCache;
			}

			dest.m_pCache = new CCacheAttribute( *m_pCache );
			if( ! dest.m_pCache ) 
				emErr = sindyErr_MemoryAllocationFailed;
			else {
				// �����R�s�[���Ȃ��ꍇ�͌��̗�����߂�
				if( ! bIsCopyHistory )
				{
					const CFieldMap& fields = *GetFieldMap(); // ���̌�X�L�[�}���ύX�����\�������邽�߃\�[�X����CFieldMap���g�p����
					foreach( const CString& name, g_listHistory )
					{
						long index = fields.HasField(name);
						if( 0 <= index )
							dest.SetValue(index, mapHistory[name]);
					}
				}
			}
		}
		// �`����R�s�[����
		if( m_pGeom )
		{
			if ( dest.m_pGeom )
				delete dest.m_pGeom;

			dest.m_pGeom = new CGeometryBase( *m_pGeom );
			if( ! dest.m_pGeom )
				emErr = sindyErr_MemoryAllocationFailed;
		}

		if( emErr != sindyErr_NoErr )
		{
			ERRORLOG(emErr);
			SASSERT( emErr != sindyErr_NoErr );
		}
		// �X�L�[�}�֌W���R�s�[����
		if( bIsCopySchema )
			(CSchemaSupport&)dest = (const CSchemaSupport&)*this;
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

void CRowBase::swap( CRowBase& src, bool bIsCopyCache/*=true*/, bool bIsCopyRowBase/*=true*/, bool bIsCopyFlag/*=true*/ )
{
	CRowBase cSwapRowBase;

	CopyTo( cSwapRowBase, bIsCopyCache, bIsCopyRowBase, bIsCopyFlag );
	src.CopyTo( *this, bIsCopyCache, bIsCopyRowBase, bIsCopyFlag );
	cSwapRowBase.CopyTo( src, bIsCopyCache, bIsCopyRowBase, bIsCopyFlag );
}

// �^����ꂽ�`��͈̔͂ŕ��s�ړ��������s��
sindyErrCode CRowBase::Move( const CGeometryBase& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode, CErrorObjectsBase& cErrs )
{
	sindyErrCode emErr = sindyErr_NoErr;

	CGeometryBase* pGeom( GetShape() );
	if( pGeom && sindyErr_NoErr != ( emErr = pGeom->Move( cGeom, pairMove, emMode ) ) )
		cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( *this, CSPErrorInfoBase( new CErrorInfoBase( emErr, sindyErrLevel_ERR, NULL, -1, -1, _T("%s"), FAIL_MOVE_FEATURE ) ) ) ) );

	return cErrs.GetDefaultErrCode();
}

// �����ɕύX�����������ǂ������`�F�b�N����
bool CRowBase::Changed( schema::ipc_table::update_type::ECode* emUpdateType/* = NULL*/ ) const
{
	// ��Ɍ`��Ŕ�r����
	bool bChanged = ShapeChanged(); // �Ԃ�l

	if( emUpdateType ) 
		*emUpdateType = bChanged ? schema::ipc_table::update_type::kShapeUpdated : schema::ipc_table::update_type::kDefault;	// ����������

	// �`�󂪕ύX����Ă���Ȃ瑮���̕ύX�`�F�b�N�͂���Ȃ�
	if( bChanged )
		return true;

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
			const CFieldMap& fields( *GetFieldMap() );

			// �L���b�V��������ꍇ
			if( m_pCache )
			{
				CComVariant vaCache, vaValue;
				long i = -1;
				foreach( const CFieldMap::_FIELD& field, std::make_pair( fields.begin(), fields.end() ) )
				{
					++i;
					if( field.editable )
					{
						// Shape�t�B�[���h�̓L���b�V�������Ȃ����A��r���ł��Ȃ��̂Ŕ�΂�
						if( esriFieldTypeGeometry == field.type )
							continue;

						vaCache.Clear();
						vaValue.Clear();

						GetValue( i, vaCache );
						Fix_BSTR_get_Value( p, i, &vaValue );
						if( ! IsEqualSDEValue( field.type, vaCache, vaValue ) )
						{
							bChanged = true;
							if( emUpdateType )
								*emUpdateType = schema::ipc_table::update_type::kAttrUpdated;
							break;
						}
					}
				}
			}
			// �L���b�V���������ꍇ
			else {
				// �����̃`�F�b�N
				IRowChangesPtr ipRowBaseChanges( p );
				if( NULL != ipRowBaseChanges )
				{
					long lFieldCount = fields.GetFieldCount();
					VARIANT_BOOL vbChanged = VARIANT_FALSE;
					for( long i = 0; i < lFieldCount; ++i )
					{
						ipRowBaseChanges->get_ValueChanged( i, &vbChanged );
						if( vbChanged )
						{
							bChanged = true;
							if( emUpdateType )
								*emUpdateType = schema::ipc_table::update_type::kAttrUpdated;
							break;	// ��ł�����Ȃ�����`�F�b�N�I���
						}
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
bool CRowBase::Changed( LPCTSTR lpcszFieldName ) const
{
	if( ! p )
		return IsCreated() ? true : false;

	bool bChanged = false; // �Ԃ�l

	// �L���b�V��������ꍇ�ɂ̓L���b�V���Ŕ�r
	if( m_pCache )
	{
		long lIndex = GetFieldMap()->FindField(lpcszFieldName);
		if( GetFieldMap()->IsShapeField( lIndex ) )
			return ShapeChanged();

		LOGASSERTE_IF( lIndex >= 0, sindyErr_AOGetFieldIndexFailed )
		{
			CComVariant vaCache, vaValue;
			GetValue( lIndex, vaCache );
			Fix_BSTR_get_Value( p, lIndex, &vaValue );

			// VT_DATE�̔�r�͍s���Ȃ����߁AVT_BSTR�ɕϊ����Ĕ�r����
			if( vaCache.vt == VT_DATE )
				vaCache.ChangeType(VT_BSTR);
			if( vaValue.vt == VT_DATE )
				vaValue.ChangeType(VT_BSTR);

			if( vaCache != vaValue )
				bChanged = true;
		}
	}
	// �L���b�V�����Ȃ��ꍇ�ɂ̓L���b�V������炸�ɔ�r
	else {
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
			if( esriFieldTypeGeometry == esriType )
				return ShapeChanged();
			else {
				IRowChangesPtr ipRowBaseChanges( p );
				CComVariant vaOrgVal, vaCurrentVal;
				ipRowBaseChanges->get_OriginalValue( lIndex, &vaOrgVal );
				Fix_BSTR_get_Value( p, lIndex, &vaCurrentVal );
				if( vaOrgVal != vaCurrentVal )
					bChanged = true;
			}
		}
	}
	return bChanged;
}

// �`��ɕύX�����邩�ǂ����`�F�b�N����
bool CRowBase::ShapeChanged() const
{
	// �쐬�A�폜�͕ύX���ꂽ�ƌ��Ȃ�
	if( IsDeleted() || IsCreated() )
		return true;

	// ���̂��������͕̂ύX���ꂽ�ƌ��Ȃ�
	if( ! p )
		return true;

	// �L���b�V��������ꍇ
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
					if( g_bShapeChanged_UseRelationalOperator )
					{
						IRelationalOperatorPtr ipRelOpe( ipOrg );
						VARIANT_BOOL vbEqual = VARIANT_FALSE;
						ipRelOpe->Equals( (IGeometry*)*m_pGeom, &vbEqual );
						return ! VB2bool(vbEqual);
					}
					else {
						if( ! m_pGeom->Equals( ipOrg ) )
							return true;
					}
				}
			}
		}
	}
	// �L���b�V���������ꍇ
	else {
		IFeatureChangesPtr ipFeatureChanges( (IFeaturePtr)p );
		if( ipFeatureChanges )
		{
			VARIANT_BOOL vbChanged = VARIANT_FALSE;
			ipFeatureChanges->get_ShapeChanged( &vbChanged );
			if( vbChanged )
				return true;
		}
	}
	return false;
}

// �L���b�V��������ꍇ�ɂ��̃L���b�V�����R�s�[�����Ɏ擾����
const VARIANT* CRowBase::GetCacheValue( long index ) const { return ( m_pCache ? (*m_pCache)[index] : (const VARIANT*)NULL ); }
VARIANT* CRowBase::GetCacheValue( long index ){ return ( m_pCache ? (*m_pCache)[index] : (VARIANT*)NULL ); }
const VARIANT* CRowBase::GetValuePointer( long index, CComVariant& va ) const
{
	const VARIANT* ret = GetCacheValue(index);
	if( ! ret ) {
		if( sindyErr_NoErr != GetValue( index, va ) )
			SASSERT(false); // �l�擾�Ɏ��s
		else
			ret = &va;
	}
	return ret;
}
VARIANT* CRowBase::GetValuePointer( long index, CComVariant& va )
{
	VARIANT* ret = GetCacheValue(index);
	if( ! ret ) {
		if( sindyErr_NoErr != GetValue( index, va ) )
			SASSERT(false); // �l�擾�Ɏ��s
		else
			ret = &va;
	}
	return ret;
}

// CComVariant��Ԃ�GetValue
CComVariant CRowBase::GetValue( long lIndex ) const 
{
	CComVariant va;
	return *GetValuePointer( lIndex, va );
}


// �C���f�b�N�X�ԍ���bool�l���擾����
bool CRowBase::GetValue( long index, bool bDefaultValue ) const
{
	CComVariant va;
	const VARIANT* pVal = GetValuePointer( index, va );

	if( ! pVal )
		return bDefaultValue;

	switch( pVal->vt )
	{
	case VT_I4: bDefaultValue = ( pVal->lVal == 0 )? false : true;	break;
	case VT_NULL: break;
	default: SASSERT( pVal->vt == VT_I4 || pVal->vt == VT_NULL ); break;
	}

	return bDefaultValue;
}

// �C���f�b�N�X�ԍ���bool�l���Z�b�g����
sindyErrCode CRowBase::SetValue( long index, bool bNewValue )
{
	return SetValue( index, ( bNewValue ? 1L : 0L ) );
}

// �C���f�b�N�X�ԍ���long�l���擾����
long CRowBase::GetValue( long index, long lDefaultValue ) const
{
	CComVariant va;
	const VARIANT* pVal = GetValuePointer( index, va );

	if( ! pVal )
		return lDefaultValue;

	switch( pVal->vt )
	{
	case VT_I2: return pVal->iVal; break;
	case VT_I4: return pVal->lVal; break;
	case VT_NULL: return lDefaultValue; break;
	default: SASSERT( pVal->vt == VT_I4 || pVal->vt == VT_NULL ); break;
	}

	return lDefaultValue;
}

// �C���f�b�N�X�ԍ���long�l���Z�b�g����
sindyErrCode CRowBase::SetValue( long index, long lNewValue )
{
	VARIANT* pVal = GetCacheValue( index );
	if( pVal )
	{
		VariantClear( pVal );
		pVal->vt = VT_I4;
		pVal->lVal = lNewValue;

		return sindyErr_NoErr;
	}
	else
		return SetValue( index, CComVariant(lNewValue) );
}

sindyErrCode CRowBase::SetValue( long index, LPCTSTR lpcszNewValue )
{
	VARIANT* pVal = GetCacheValue( index );
	if( pVal )
	{
		VariantClear( pVal );
		pVal->vt = VT_BSTR;
		pVal->bstrVal = ::SysAllocString(lpcszNewValue);

		return sindyErr_NoErr;
	}
	else
		return SetValue( index, CComVariant(lpcszNewValue) );
}

// �C���f�b�N�X�ԍ���double�l���擾����
double CRowBase::GetValue( long index, const double& dDefaultValue ) const
{
	CComVariant va;
	const VARIANT* pVal = GetValuePointer( index, va );

	if( ! pVal )
		return dDefaultValue;

	switch( pVal->vt )
	{
	case VT_R4: return pVal->fltVal; break;
	case VT_R8: return pVal->dblVal; break;
	case VT_NULL: break;
	default: SASSERT( pVal->vt == VT_R8 || pVal->vt == VT_NULL ); break;
	}

	return dDefaultValue;
}

// �C���f�b�N�X�ԍ���double�l���Z�b�g����
sindyErrCode CRowBase::SetValue( long index, const double& dNewValue )
{
	VARIANT* pVal = GetCacheValue( index );
	if( pVal )
	{
		VariantClear( pVal );
		pVal->vt = VT_R8;
		pVal->dblVal = dNewValue;

		return sindyErr_NoErr;
	}
	else
		return SetValue( index, CComVariant(dNewValue) );
}

CString CRowBase::GetValue( long index, LPCTSTR lpcszDefaultValue ) const
{
	CComVariant va;
	const VARIANT* pVal = GetValuePointer( index, va );

	if( ! pVal )
		return lpcszDefaultValue;

	switch( pVal->vt )
	{
	case VT_NULL:
		return _T("");
		// fall through
	case VT_BSTR:
		return pVal->bstrVal;
		// fall through
	default:
		{
			CComVariant vaCpy;
			vaCpy.ChangeType( VT_BSTR, pVal );
			return vaCpy.bstrVal;
			// fall through
		}
	}
}

// �w�肳�ꂽ�t�B�[���h���f�t�H���g�l���ǂ������`�F�b�N����
bool CRowBase::IsDefaultValue( LPCTSTR lpcszFieldName ) const
{
	const CFieldMap& cFields( *GetFieldMap() );
	long lIndex = cFields.FindField( lpcszFieldName );
	if( 0 > lIndex )
	{
		_ASSERTE( 0 <= lIndex );
		return false;
	}
	CComVariant vaValue, vaDefault = cFields.GetDefaultValue(lIndex);
	GetValue( lIndex, vaValue );

	// �{���̌^�ɖ߂��ă`�F�b�N
	VARENUM emType = VT_ERROR;
	switch( cFields.GetFieldType(lIndex) )
	{
	case esriFieldTypeSmallInteger:
	case esriFieldTypeInteger:
		emType = VT_I4;
		break;
	case esriFieldTypeSingle:
	case esriFieldTypeDouble:
		emType = VT_R8;
		break;
	case esriFieldTypeString:
		emType = VT_BSTR;
		// �󕶎�->NULL�ɂ���
		if( vaValue.vt == VT_BSTR && 0 == wcslen(vaValue.bstrVal) )
		{
			vaValue.Clear();
			vaValue.vt = VT_NULL;
		}
		break;
	case esriFieldTypeDate:
		emType = VT_DATE;
		break;
	default:
		break;
	}
	if( vaValue.vt != vaDefault.vt )
	{
		vaValue.ChangeType(emType);
		vaDefault.ChangeType(emType);
	}

	return ( vaValue == vaDefault );
}

// �ύX�O�̒l���擾����
CComVariant CRowBase::GetOriginalValue( LPCTSTR lpcszFieldName ) const
{
	return GetOriginalValue( GetFieldMap()->FindField( lpcszFieldName ) );
}

CComVariant CRowBase::GetOriginalValue( long lFieldIndex ) const
{
	CComVariant vaRet; // �߂�l
	const CFieldMap& cFields( *GetFieldMap() );

	// �`�F�b�N
	if( ! ( 0 <= lFieldIndex && (ULONG)lFieldIndex < cFields.GetFieldCount() ) )
	{
		_ASSERTE( 0 <= lFieldIndex && (ULONG)lFieldIndex < cFields.GetFieldCount() ); // �t�B�[���h�C���f�b�N�X�͈͊O
		return vaRet;
	}
	if( ! p )
	{
		_ASSERTE( p ); // ���̂�������΂Ȃ�Ȃ�
		return vaRet;
	}

	Fix_BSTR_get_Value( p, lFieldIndex, &vaRet );

	return vaRet;
}

CString CRowBase::GetDomainName( LPCTSTR lpszFiedlName ) const
{
	long index = this->FindField(lpszFiedlName);
	std::map<long, CString> domainMap = this->GetFieldMap()->GetFieldAttr(index).rdomain;
	CString value = this->GetValue(index, _T(""));
	// �R�[�h�l�h���C���łȂ���Βl�����̂܂ܕԂ�
	// �h���C����`�O�̒l�ł���΂��̂܂ܕԂ�
	if(domainMap.empty() || domainMap.find(_ttol(value)) == domainMap.end())
		return value;

	return domainMap[_ttol(value)];
}

CGeometryBase* CRowBase::GetShape()
{
	return m_pGeom;
}

const CGeometryBase* CRowBase::GetShape() const
{
	return m_pGeom;
}

sindyErrCode CRowBase::SetShape( IGeometry* ipGeom )
{
	if( ! ipGeom )
		return sindyErr_ArgIsNull;

	if( m_pGeom )
		(*m_pGeom) = ipGeom;
	else {
		IFeaturePtr ipFeat( p );
		if( ipFeat )
		{
			LOGASSERTEM_IF( SUCCEEDED( ipFeat->putref_Shape( (IGeometryPtr)AheCreateClone(ipGeom) ) ), sindyErr_AOErr, _T("%s"), FAIL_PUTREF_SHAPE )
			{
			}
		}
		// INF��LQ���t�B�[�`������Ȃ����̂ł��`��͕K�v�Ȃ��Ƃ�����̂�
		else
			m_pGeom = new CGeometryBase( ipGeom );
	}

	return sindyErr_NoErr;
}

// ���݊i�[����Ă���t�B�[�`���̌`��̃R�s�[��Ԃ�
IGeometryPtr CRowBase::GetShapeCopy( ShapeMode emMode/* = enumLatest*/ ) const
{
	IGeometryPtr ipGeom;	// �Ԃ�l

	if( emMode & enumLatest )
	{
		if( m_pGeom )
			ipGeom = AheCreateClone( (IGeometry*)(*this->GetShape()) );
		else if( p ) {
			// �L���b�V�����Ă��Ȃ��ꍇ�̓L���b�V�������Ȃ��悤�ɂ����Ƃ���
			IFeaturePtr ipFeature;
			if( SUCCEEDED( QueryInterface( &ipFeature ) ) )
			{
				SUCCEEDED_IF( ipFeature->get_ShapeCopy( &ipGeom ), sindyErr_GeometryNotFound, MAY_BE_NULL_SHAPE )
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
			if( (emMode & enumOriginal) || ( ipGeom == NULL && IsCached() ) )
			{
				IGeometryPtr ipGeomTmp;
				SUCCEEDED_IF( ipFeature->get_ShapeCopy( &ipGeomTmp ), sindyErr_GeometryNotFound, MAY_BE_NULL_SHAPE )
				{
					// [Bug 6065] ipGeom��NULL�ibLatest��false��bOriginal��true�̎��Ɂj��ipGeomTmp��NULL�̎��ɃG���[���O���o�͂����̂ŉ���B
					if ( ipGeom ) {
						if ( ! (emMode & enumDiff) )
							ipGeom = CGeometryBase::Union( ipGeom, ipGeomTmp );
						else
							ipGeom = CGeometryBase::SymmetricDifference( ipGeom, ipGeomTmp );
					} else
						ipGeom = ipGeomTmp;
				}
			}
			if( emMode & enumFeatureChangedOriginal )
			{
				IGeometryPtr ipGeomTmp;
				IFeatureChangesPtr ipChanges( ipFeature );
				LOGASSERTE_IF( SUCCEEDED( ipChanges->get_OriginalShape( &ipGeomTmp ) ), sindyErr_GeometryFunctionFailed )
				{
					// [Bug 6065] ipGeom��NULL�ibLatest��false���Ɂj��ipGeomTmp��NULL�̎��ɃG���[���O���o�͂����̂ŉ���B
					if ( ipGeom ) {
						if ( ! (emMode & enumDiff) )
							ipGeom = CGeometryBase::Union( ipGeom, ipGeomTmp );
						else
							ipGeom = CGeometryBase::SymmetricDifference( ipGeom, ipGeomTmp );
					} else
						ipGeom = AheCreateClone( ipGeomTmp );
				}
			}
		}
	}
	return ipGeom;
}

// ���݊i�[����Ă���t�B�[�`���̑��݋�`��Ԃ�
IEnvelopePtr CRowBase::GetEnvelope( ShapeMode emMode/* = enumLatest*/ ) const
{
	IEnvelopePtr ipRet;

	// CRow���ł̕ύX�`���`
	if( (emMode & enumLatest) && m_pGeom )
		ipRet = m_pGeom->GetEnvelope();

	// �I���W�i��
	if( emMode & enumOriginal )
	{
		IFeaturePtr ipFeat( p );
		if( ipFeat )
		{
			IEnvelopePtr ipEnv;
			IGeometryPtr ipGeom;
			ipFeat->get_Shape( &ipGeom );
			if( ipGeom )
			{
				ipGeom->get_Envelope( &ipEnv );
				if( ipEnv )
				{
					if( ipRet )
					{
						LOGASSERTE_IF( SUCCEEDED( ipRet->Union( ipEnv ) ), sindyErr_GeometryFunctionFailed );
					}
					else
						ipRet = ipEnv;
				}
			}
		}
	}

	// IFeatureChanged::OriginalShape
	if( emMode & enumFeatureChangedOriginal )
	{
		IFeatureChangesPtr ipChanged( p );
		if( ipChanged )
		{
			VARIANT_BOOL vbChanged = VARIANT_FALSE;
			ipChanged->get_ShapeChanged( &vbChanged );
			if( vbChanged )
			{
				IEnvelopePtr ipEnv;
				IGeometryPtr ipGeom;
				ipChanged->get_OriginalShape( &ipGeom );
				if( ipGeom )
				{
					ipGeom->get_Envelope( &ipEnv );
					if( ipEnv )
					{
						if( ipRet )
						{
							LOGASSERTE_IF( SUCCEEDED( ipRet->Union( ipEnv ) ), sindyErr_GeometryFunctionFailed );
						}
						else
							ipRet = ipEnv;
					}
				}
			}
		}
	}

	return ipRet;
}

// ���������`�F�b�N����
sindyErrCode CRowBase::CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrs ) const
{
	CErrorInfosBase infos;
	CheckLogic( cRule, infos );
	// �G���[�����݂���Ȃ�ǉ�
	if( ! infos.empty() )
	{
		CSPErrorObjectBase cErr( new CErrorObjectBase( *this ) );
		cErr->push_back( infos );
		cErrs.push_back( cErr );
	} 
	return cErrs.GetDefaultErrCode();
}

void CRowBase::CheckLogic( const CLogicRule& cRule, CErrorInfosBase& infos ) const
{
	// CLogicRule�ɃT�|�[�g�ǉ������̂�
	if( g_bDisableCheckToNoSubstance )
	{
		_ASSERTE( ! g_bDisableCheckToNoSubstance );
		const_cast<CLogicRule&>(cRule).SetDisableCheckToNoSubstance( g_bDisableCheckToNoSubstance );
	}

	if( cRule.GetDisableCheckToNoSubstance() )
	{
		// ���̂��Ȃ����́i�V�K�쐬���ꂽ���̂������j��
		// �`�F�b�N�Ώۂ���O��
		if( ! ( p || m_bCreated ) )
			return;
	}

	// �������g�̃`�F�b�N
	bool bIsSDE = GetNameString()->IsSDE(); // ������t�B�[���h��PGDB��SDE�Ō^���Ⴄ���߁A�ǂ����Ȃ̂��𔻒f����
	const CFieldMap& cFields( *GetFieldMap() );

	bool bChangedAttr = false;  // �������ύX���ꂽ���ǂ���
	bool bChangedShape = false; // �`�󂪕ύX���ꂽ���ǂ���

	// �폜����Ă��Ȃ��ꍇ�Ɍ��葮���E�`��`�F�b�N���s��
	if( ! IsDeleted() )
	{
		// �����`�F�b�N
		int nIndex = 0;
		for( CFieldMap::const_iterator it = cFields.begin(), itEnd = cFields.end(); it != itEnd; ++it, ++nIndex )
		{
			if( ! it->editable )
				continue;

			CComVariant vaValue, vaOrg;
			GetValue( nIndex, vaValue );
			if( VT_EMPTY == vaValue.vt )
				continue; // �t�B�[���h���擾��������Ă���ꍇ

			bool bChangedField = ( IsCreated() || ( ! p ) ) ? true : false; // ���̎��_��Detach����Ă�����͕̂ύX�O�̒l�����Ȃ��̂ŐV�K�Ɠ��������ɂ���
			if( ! bChangedField )
			{
				 vaOrg = GetOriginalValue( nIndex );
				 bChangedField = !( vaValue == vaOrg );

				 if( bChangedField )
					 bChangedAttr = true;
			}
			CheckLogic_Field( cRule, infos, nIndex, *it, vaValue, vaOrg, bChangedField, bIsSDE );
		}

		// �`��`�F�b�N�iTODO: ChangedShape�֐��Ƃ��ċN�����ׂ��j
		IGeometryPtr ipOrgShape;
		IFeatureChangesPtr ipFeat( p );
		if( ipFeat || GetShape() ) // �t�B�[�`�����ǂ���
		{
			if( IsCreated() || ( ! p ) ) // // ���̎��_��Detach����Ă�����͕̂ύX�O�̒l�����Ȃ��̂ŐV�K�Ɠ��������ɂ���
				bChangedShape = true;
			else {
				if( ipFeat )
					ipFeat->get_OriginalShape( &ipOrgShape );

				// �`��L���b�V�����Ȃ��ꍇ
				if( ! GetShape() )
				{
					VARIANT_BOOL vbChanged = VARIANT_FALSE;
					if( ipFeat )
						ipFeat->get_ShapeChanged( &vbChanged );
					bChangedShape = VB2bool(vbChanged);
				}
				else {
					if( ipOrgShape )
						bChangedShape = ( ! m_pGeom->Equals( ipOrgShape ) );
				}
			}

			if( ( ! cRule.GetDisableCheckNoChange() ) || 
				( cRule.GetDisableCheckNoChange() && bChangedShape ) )
			{
				const CGeometryBase cGeom( (IGeometry*)*this );
				if( (IGeometry*)cGeom )
				{
					IEnvelopePtr ipChkEnv;

					// �|���S������1000�_�ȏ�\���_�������̂̂ݕύX�̈�擾
					if ( cGeom.GetGeometryType() == esriGeometryPolygon  ) {
						if ( cGeom.GetPointCount() > 1000 ) {
							// �ύX�̈�擾
							IGeometryPtr ipEdit = GetShapeCopy( (ShapeMode)(enumAll|enumDiff) );

							if ( ipEdit )
								ipEdit->get_Envelope( &ipChkEnv );
						}
					}

					// �Z�O�����g�`�F�b�N
					cGeom.CheckLogic( cRule.GetBasicRule( GetTableName(), GetNameString()->GetUserName(), CBasicLogicRule() ), infos, ipChkEnv );
				}

			}
		}
	}

	// �ύX�^�C�v�����
	schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;
	if( IsDeleted() )
		emUpdateType = schema::ipc_table::update_type::kDeleted;
	else if( IsCreated() )
		emUpdateType = schema::ipc_table::update_type::kCreated;
	else if( bChangedShape )
		emUpdateType = schema::ipc_table::update_type::kShapeUpdated;
	else if( bChangedAttr )
		emUpdateType = schema::ipc_table::update_type::kAttrUpdated;


	if( cRule.GetUseEditableRule() && // �ҏW���[�����g�p����ꍇ�݂̂Ɍ���
		schema::ipc_table::update_type::kDefault != emUpdateType )
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
			CheckEditable( emEditType, infos, cRule.GetDoCheckModify() );
	}
}

// �t�B�[���h�P�ʂ̃`�F�b�N������
void CRowBase::CheckLogic_Field( const CLogicRule& cRule, CErrorInfosBase& cErrs, long lIndex, const CFieldMap::_FIELD& cFieldAttr, const CComVariant& vaValue, const CComVariant& vaOrg, bool bChanged, bool bIsSDE ) const
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// �ȉ��̃`�F�b�N�͑S�ăf�[�^�x�[�X�ɓ�����邩�ǂ����̃`�F�b�N�Ȃ̂őS�ĕύX������Ƃ��݂̂ł悢
	// �ύX�̂���E�Ȃ��Ɋւ�炸�s���ꍇ�͂������ɓ���邱�ƁI�I�I
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	if( ! bChanged )
		return;

	// [CHECK] NOT NULL�`�F�b�N�ibug 6434�j
	if( ! cFieldAttr.nullable )
	{
		if( VT_NULL == vaValue.vt || 
			VT_BSTR == vaValue.vt && 0 >= ::SysStringLen(vaValue.bstrVal) )
			cErrs.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_SetNullValueToNotNullField, sindyErrLevel_ERR, GetShapeCopy(), lIndex, -1, NULL_CANNOT_SET, cFieldAttr.name ) ) );
	}

	switch( cFieldAttr.type )
	{
	case esriFieldTypeString:
		{
			// [CHECK] ������t�B�[���h���ӂ�̌��o
			if( VT_BSTR == vaValue.vt )
			{
				// �I�[�����͓���Ȃ�
				// [Bug 6129] City_Admin�̍\���_���폜�ł��Ȃ��B�iPGDB�̂݁j
				// PGDB�̎���it->length�͕������B
				size_t lByte = bIsSDE ? ( strlen(CW2A(vaValue.bstrVal)) ) : ( (wcslen( vaValue.bstrVal ) ) );
				if( VT_BSTR == vaValue.vt && lByte > cFieldAttr.length )
				{
					CString strValue(vaValue.bstrVal);
					if( 1000 < strValue.GetLength() )
						cErrs.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_FieldLengthLimitOver, sindyErrLevel_ERR, (IGeometry*)*this, lIndex, -1, OVER_CHARNUM_LIMIT, cFieldAttr.name, strValue.Mid(0,1000), lByte - cFieldAttr.length ) ) );
					else
						cErrs.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_FieldLengthLimitOver, sindyErrLevel_ERR, (IGeometry*)*this, lIndex, -1, OVER_CHARNUM_LIMIT2, cFieldAttr.name, strValue, lByte - cFieldAttr.length ) ) );
				}
			}
			break;
		}
	default:
		if( VT_NULL != vaValue.vt )
		{
			// [CHECK] �������ӂ�`�F�b�N
			if( 0 < cFieldAttr.precision )
			{
				// ���������̂݃`�F�b�N����
				long num = 0;
				{
					CComVariant va( vaValue );
					va.ChangeType( VT_I4 );
					va.lVal = abs(va.lVal);
					while( 0 < va.lVal ){ va.lVal /= 10; ++num; }
				}
				if( cFieldAttr.precision - cFieldAttr.scale < num )
				{
					CComVariant vaValueStr( vaValue );
					vaValueStr.ChangeType( VT_BSTR );
					cErrs.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_FieldPrecisionLimitOver, sindyErrLevel_ERR, (IGeometry*)*this, lIndex, -1, OVER_DIGITNUM_LIMIT, cFieldAttr.name, CString(vaValueStr.bstrVal), cFieldAttr.precision - cFieldAttr.scale ) ) );
				}
			}
			// [CHECK] �����W�`�F�b�N
			if( VT_EMPTY != cFieldAttr.range.first.vt || VT_EMPTY != cFieldAttr.range.second.vt )
			{
				CComVariant va( vaValue ), vaMin( cFieldAttr.range.first ), vaMax( cFieldAttr.range.second );
				va.ChangeType( VT_R8 ); vaMin.ChangeType( VT_R8 ); vaMax.ChangeType( VT_R8 );
				if( ! ( ( VT_EMPTY == cFieldAttr.range.first.vt  || vaMin.dblVal <= va.dblVal ) &&   // MIN
					( VT_EMPTY == cFieldAttr.range.second.vt || vaMax.dblVal >= va.dblVal ) ) )  // MAX
				{
					CComVariant vaValueStr( vaValue ), vaMin( cFieldAttr.range.first ), vaMax( cFieldAttr.range.second );
					vaValueStr.ChangeType( VT_BSTR ); vaMin.ChangeType( VT_BSTR ); vaMax.ChangeType( VT_BSTR );
					cErrs.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_FieldRangeLimitOver, sindyErrLevel_ERR, (IGeometry*)*this, lIndex, -1, OUT_OF_RANGEDOMAIN, cFieldAttr.name, CString(vaValueStr.bstrVal), CString(vaMin.bstrVal), CString(vaMax.bstrVal) ) ) );
				}
			}
			// [CHECK] �h���C���`�F�b�N
			if( ! cFieldAttr.rdomain.empty() )
			{
				if( cFieldAttr.rdomain.end() == cFieldAttr.rdomain.find(vaValue.lVal) )
				{
					CComVariant vaValueStr( vaValue );
					vaValueStr.ChangeType( VT_BSTR );
					cErrs.push_back( CSPErrorInfoBase( new CErrorInfoBase( sindyErr_FieldDomainLimitOver, sindyErrLevel_ERR, (IGeometry*)*this, lIndex, -1, NON_EXIST_DOMAINCODE, cFieldAttr.name, CString(vaValueStr.bstrVal) ) ) );
				}
			}
		}
		break;
	}
}

// �ҏW�\���ǂ������`�F�b�N����
sindyErrCode CRowBase::CheckEditable( sindyeEditType emType, CErrorInfosBase& cErrInfos, bool check_modify/* = true*/, bool err2warn_modify/* = false*/ ) const
{
	CString strMsg;
	sindyeEditableType emEditable = sindyeEditableEditable;
	if( g_cRule )
		emEditable = g_cRule->IsEditable( *this,  emType, strMsg, check_modify, err2warn_modify );

	// EditType�ɂ����ErrCode��ς���ibug 9606�j
	sindyErrCode errcode = sindyErr_NoErr;
	switch( emType )
	{
	case sindyeEditCreate: errcode = sindyErr_RuleNotAllowedCreate; break;
	case sindyeEditMove: errcode = sindyErr_RuleNotAllowedMove; break;
	case sindyeEditDelete: errcode = sindyErr_RuleNotAllowedDelete; break;
	case sindyeEditAttribute: errcode = sindyErr_RuleNotAllowedEditAttribute; break;
	default:
		break;
	}
	if( sindyeEditableEditable != emEditable )
		cErrInfos.push_back( CSPErrorInfoBase( new CErrorInfoBase( errcode, ( sindyeEditableWarnning == emEditable ) ? sindyErrLevel_WARN : sindyErrLevel_ERR, NULL, -1, -1, strMsg ) ) );

	return cErrInfos.GetDefaultErrCode();
}

// �����o�ϐ�������������
void CRowBase::Init()
{
	m_emTableType = sindyTableType::unknown;
	m_pCache = NULL;
	m_pGeom  = NULL;
	m_lOID   = -1;
	m_bCreated = m_bDeleted = false;
	SetNameString( CSPTableNameString() );
	SetFieldMap( CSPFieldMap() );
}

bool CRowBase::IsSameAttribute(const CRowBase& src, const CSimpleArray<CString>& cExclusions) const
{
	// �ڑ��v���p�e�B�A�I�[�i���A�e�[�u������������
	if ( !(src.GetNameString()->IsSameSource( *GetNameString() ) &&
           (0 == _tcscmp(GetTableName(), src.GetTableName())) ) )
		return false;

	std::list<CString> cDefaultExclusions;
	long count = cExclusions.GetSize();
	for( long i = 0; i < count; ++i )
		cDefaultExclusions.push_back( cExclusions[i] );

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

		GetValue( it->name, vaSource );
		src.GetValue( it->name, vaDest );

		if ( vaSource != vaDest )
			return false;
	}

	return true;
}

void CRowBase::Trace( bool bAttribute/* = true*/, bool bGeometry/* = true*/ ) const
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
			IGeometryPtr ipGeom( GetShapeCopy() );
			CGeometryBase::Trace( ipGeom, _T("�`��") );
		}
	}
}

} // namespace sindy
