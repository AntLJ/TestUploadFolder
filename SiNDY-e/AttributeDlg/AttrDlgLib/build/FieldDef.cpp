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

/*! \file FieldDef.cpp
	\brief CFieldDef �N���X�̃C���v�������e�[�V����
	\author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
	\date 2002-03-05
*/

#include "stdafx.h"
#include "../FieldDef.h"

namespace {
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR  ERROR_ENTER_NUMBER  = _T("Please enter a number");
	LPCTSTR  ERROR_INPUT_VALUE   = _T("Input value error");
	LPCTSTR  ERROR_OUT_OF_LIMITS = _T("The value is out of limits: %d Please enter a value between %g - %g ");
#else
	LPCTSTR  ERROR_ENTER_NUMBER  = _T("���l����͂��Ă�������");
	LPCTSTR  ERROR_INPUT_VALUE   = _T("���͒l�G���[");
	LPCTSTR  ERROR_OUT_OF_LIMITS = _T("�����͈͊O�̒l�ł� : %d   %g - %g �̊Ԃœ��͂��Ă�������");
#endif

	/**
	 * @brief Z�l�t���̃|�C���g���ǂ����𔻒肷��
	 * @param [in] ipFeatureClass �t�B�[�`���N���X
	 * @retval true �t�B�[�`���N���X��Z�l�t���̃|�C���g
	 * @retval false �t�B�[�`���N���X��Z�l�t���̃|�C���g�ł͂Ȃ�
	 */
	bool IsPointWithZ(IGeometryPtr ipGeometry)
	{
		// �W�I���g�����L������Z�l�����łȂ����false��Ԃ�
		if(!(ipGeometry && AheGeoHasZ(ipGeometry)))
			return false;

		// �t�B�[�`���N���X�̃W�I���g���^�C�v���擾����BPOINT�^���ǂ����𔻒f��Ƃ���
		esriGeometryType type = esriGeometryNull;
		ipGeometry->get_GeometryType(&type);
		
		return type == esriGeometryPoint;
	}
}

namespace virtual_field {
	const _TCHAR kShapeZ[] = _T("SHAPE.Z");
}

/**
	�R���X�g���N�^�E�f�X�g���N�^
*/
/**
 * �R���X�g���N�^
 */
CFieldDef::CFieldDef()
{
	Clear();
}
/**
 * �f�X�g���N�^
 */
CFieldDef::~CFieldDef()
{
	/**
	 * m_mapDomain����ɂ��܂�
	 */
//	m_mapDomain.empty();
	m_mapDomain.clear();
}
/**
 * �����o�ϐ���S�ď�����Ԃɖ߂�
 */
void CFieldDef::Clear()
{
	m_ipRowBuffer= NULL;
	m_ipField = NULL;
	m_mapDomain.clear();

	m_vaInitValue = -2L;
	m_vaValue = -2L;

	m_lFieldIndex = -1;
	m_bChanged = FALSE;
	m_bIsInited = FALSE;
	m_bIsNullable = FALSE;
	m_bIsUnset = m_bIsUnsetOld = TRUE;
	m_bIsNotDomain = FALSE;

	// �����W�h���C���p�̍\���̂̏�����
	m_strctRangeDomain.dMax = 0.0;
	m_strctRangeDomain.dMin = 0.0;
	m_strctRangeDomain.bExist = TRUE;
	m_strctRangeDomain.bInited = FALSE;
}
/**
 * �����o�ϐ����t�B�[���h�l�̕ύX��������O�ɖ߂�
 */
void CFieldDef::Reset()
{
	m_bChanged = FALSE;
	m_vaValue = m_vaInitValue;
	m_bIsUnset = m_bIsUnsetOld;	//!< �Z�b�g���ꂽ���ǂ����̃t���O���߂��K�v����iStore�̊֌W�j
}
/**
 * �t�B�[���h�l�̕ύX���f�[�^�x�[�X�ɏ�������
 *
 * @return �l���X�V���ꂽ�� TRUE �A�X�V����Ȃ������� FALSE ��Ԃ�
 *
 * �t�B�[���h�l�ɕύX���������ꍇ�im_bChanged �� TRUE �̏ꍇ�j�� IRowBuffer �� m_vaValue ��K�p���܂��B
 * \note �����̏������݂͂����ł͍s���܂���BCFeatureDef::Store() ���Q�Ƃ��Ă��������B 
 */
BOOL CFieldDef::Store()
{
	IUnknownPtr ipUnk = NULL;
	BOOL bStored = FALSE;

	if( !m_bChanged )
		return bStored;

	/// �g���ĂȂ����A���R�[�h���Ȃ��ꍇ�͔�΂�
	if( m_bIsUnset && !m_ipRowBuffer )
		return bStored;

	// �t�B�[���h�C���f�b�N�X��0�ȏ�Ȃ�A�l���Z�b�g
	if(m_ipField)
	{
		m_ipRowBuffer->put_Value( m_lFieldIndex, m_vaValue );
	}
	else if(m_strFieldName.CompareNoCase(virtual_field::kShapeZ) == 0)
	{
		// �C���f�b�N�X�����̒l�Ńt�B�[���h����Z�l�̉��z�t�B�[���h���w���Ă����ꍇ��Z�l���Z�b�g
		IGeometryPtr ipGeometry;
		IFeaturePtr(m_ipRowBuffer)->get_Shape(&ipGeometry);
		if(!IsPointWithZ(ipGeometry))
			return bStored;

		IPointPtr(ipGeometry)->put_Z(m_vaValue.dblVal);
		IFeaturePtr(m_ipRowBuffer)->putref_Shape(ipGeometry);
	}
				
	ATLTRACE(_T("CFieldDef::Store() : �l���X�V���܂��F%s::%s\n"), m_strTableName, m_strFieldName );
	bStored = TRUE;	//!< �X�V����

	m_bChanged = FALSE;	//!< �X�V�t���O��߂�

	return bStored;
}
/**
 * �e�[�u�����ƃt�B�[���h�����Z�b�g����
 *
 * @param lpszTableName	[in]	�e�[�u����
 * @param lpszFieldName	[in]	�t�B�[���h��
 *
 * \note �����o�ϐ��� m_lpszTableName �� m_lpszFieldName �ɃR�s�[���邾���ł�
 */
void CFieldDef::SetFieldDef( LPCTSTR lpszTableName,	LPCTSTR lpszFieldName )
{
	m_strFieldName = lpszFieldName;
	m_strTableName = lpszTableName;
}
/**
 * CFieldDef �����ۂɎg�p�ł���悤�ɐݒ肷��
 *
 * @param lFieldIndex	[in]	2�Ԗڂ̈����� IField �C���^�[�t�F�[�X�I�u�W�F�N�g�̃C���f�b�N�X�ԍ�
 * @param ipField		[in]	�Ή����� IField �C���^�[�t�F�[�X�I�u�W�F�N�g
 * @param ipRowBuffer	[in]	�Ή����� IRowBuffer �C���^�[�t�F�[�X�I�u�W�F�N�g�i���ۂɒl������肷��̂͂������j
 *
 * \note ���̊֐��ŃZ�b�g���Ȃ��Ǝ��ۂɎg�p�ł���悤�ɂ͂Ȃ�Ȃ��i�O���e�[�u�����R�[�h�̐V�K�쐬�̏ꍇ�������j
 */
void CFieldDef::SetRowBuffer( LONG lFieldIndex, IFieldPtr ipField, IRowBufferPtr ipRowBuffer )
{
	m_lFieldIndex = lFieldIndex;
	m_ipField = ipField;

	/// ipSet �� NULL �Ȃ�t�B�[���h�����݂��Ă��Ȃ��̂Ńt���O�𗧂Ă�
	if( !ipRowBuffer ) {
		m_bIsUnset = m_bIsUnsetOld = TRUE;

		return;
	} else {
		m_ipRowBuffer = ipRowBuffer;
		m_bIsUnset = m_bIsUnsetOld = FALSE;
	}

	// �t�B�[���h�̎��̂�����Ȃ�A�l���擾���ď�����
	if(m_ipField)
	{
		ipRowBuffer->get_Value( m_lFieldIndex, &m_vaInitValue );
		m_vaValue = m_vaInitValue;
	}
	else if(m_strFieldName.CompareNoCase(virtual_field::kShapeZ)==0)
	{
		// �t�B�[���h�̎��̂��Ȃ��A���O��Z�l�̉��z�t�B�[���h���w���Ă����ꍇ��Z�l���擾���Ēl��ݒ�
		IGeometryPtr ipGeometry;
		IFeaturePtr(ipRowBuffer)->get_Shape(&ipGeometry);
		double zVal = 0;
		if(IsPointWithZ(ipGeometry) && S_OK == IPointPtr(ipGeometry)->get_Z(&zVal))
		{
			m_vaValue.vt = VT_R8;
			m_vaValue.dblVal = zVal;
		}
		else
		{
			m_vaValue.vt = VT_NULL;
		}
		m_bIsInited = TRUE;
		return;
	}
			
	/// NULL OK �ȃt�B�[���h���ǂ���
	VARIANT_BOOL vaIsNullable = VARIANT_FALSE;
	m_ipField->get_IsNullable( &vaIsNullable );

	m_bIsNullable = ( vaIsNullable == VARIANT_TRUE ) ? TRUE : FALSE;	//!< NULL OK ���ǂ����̃t���O�𗧂Ă�

	// �t�B�[���h�̃G�C���A�X�����擾
	CComBSTR bstrAliasName;
	m_ipField->get_AliasName( &bstrAliasName );
	m_strFieldAliasName = bstrAliasName;

	// DOMAINMAP����ō��ƍ쐬���ƊJ�����̃X���b�h���قȂ邽�߂ɗ�����̂�
	// �����ō�����Ⴄ
	GetDomain();

	m_bIsInited = TRUE;	//!< �����������t���O
}
/**
 * �Ή�����t�B�[���h�̃h���C�����擾����
 *
 *  �f�[�^�x�[�X�ɖ₢���킹����͈̂�x����i��x�ǂݍ��񂾂�����ŕۑ��j
 * 
 * @return ���������ꍇ�� DOMAINMAP ��������A���s�����ꍇ�A�܂��͈�x�����������ʃh���C�������݂��Ȃ������ꍇ�� NULL ���Ԃ�
 */
DOMAINMAP* CFieldDef::GetDomain()
{
	// �t�B�[���h�����݂��Ȃ��ꍇ��NULL(Z�l�̉��z�t�B�[���h��������Ȃ��̂�) 2017/2/17 ta_suga
	if (!m_ipField)
		return NULL;

	IDomainPtr ipDomain = NULL;
	ICodedValueDomainPtr ipCodedValueDomain = NULL;
	BSTR bstrName;
	CComVariant vaValue;
	LONG lCount = 0;

	USES_CONVERSION;
	
	/// �ȑO�Ɋ��ɒ����ς݊��h���C�������݂��Ȃ������ꍇ
	if( m_bIsNotDomain ) 
		return NULL;

	else if( m_mapDomain.size() == 0  ) 
	{
		m_ipField->get_Domain( &ipDomain );
		if( !ipDomain )
			return NULL;
		ipCodedValueDomain = ipDomain;	//!< �Œ�l�h���C��
		if( !ipCodedValueDomain )
			return NULL;

		ipCodedValueDomain->get_CodeCount( &lCount );
		for( LONG i = 0; i < lCount; i++ ) {
			ipCodedValueDomain->get_Name( i, &bstrName );
			CString strName = bstrName;
			::SysFreeString( bstrName );
			ipCodedValueDomain->get_Value( i, &vaValue );

			CODEVALUE_DOMAIN domain;
			domain.idx = i;
			domain.code = vaValue;
			domain.code_discription = strName;
			m_mapDomain.insert(domain);
		}
	}

	return &m_mapDomain;
}
/**
 * �Ή�����t�B�[���h�̃����W�h���C�����擾����
 *
 *  �f�[�^�x�[�X�ɖ₢���킹����͈̂�x����i��x�ǂݍ��񂾂�����ŕۑ��j
 * 
 * @return ���������ꍇ�� DOMAINMAP ��������A���s�����ꍇ�A�܂��͈�x�����������ʃh���C�������݂��Ȃ������ꍇ�� NULL ���Ԃ�
 */
RANGEDOMAIN* CFieldDef::GetRangeDomain()
{
	// �t�B�[���h�����݂��Ȃ��ꍇ��NULL(Z�l�̉��z�t�B�[���h��������Ȃ��̂�) 2017/2/17 ta_suga
	if(!m_ipField)
		return NULL;

	IDomainPtr ipDomain = NULL;
	IRangeDomainPtr ipRangeDomain = NULL;
	CComVariant vaValue;
	LONG lCount = 0;

	USES_CONVERSION;
	
	/// �ȑO�Ɋ��ɒ����ς݂̏ꍇ
	if( m_strctRangeDomain.bInited ) 
	{
		if( m_strctRangeDomain.bExist )	
			return &m_strctRangeDomain;	// �����W�h���C�������݂��Ă���Ȃ烁���o��Ԃ�
		else 
			return NULL;			// �����W�h���C�������݂��Ă��Ȃ��Ȃ� NULL ��Ԃ�
	}	

	// 2�x���������Ȃ�
	m_strctRangeDomain.bInited = TRUE;

	m_ipField->get_Domain( &ipDomain );

	if( !ipDomain ) 
	{
		m_strctRangeDomain.bExist = FALSE;
		return NULL;
	}
	
	ipRangeDomain = ipDomain;	// �����W�h���C��
	if( !ipRangeDomain )
	{
		m_strctRangeDomain.bExist = FALSE;
		return NULL;
	}

	// �ő�l�̎擾
	ipRangeDomain->get_MaxValue( &vaValue );
	vaValue.ChangeType( VT_R8 );	// �ⓚ���p�� DOUBLE �ɕϊ��i���v�H�j
	m_strctRangeDomain.dMax = vaValue.dblVal;
	// �ŏ��l�̎擾
	ipRangeDomain->get_MinValue( &vaValue );
	vaValue.ChangeType( VT_R8 );
	m_strctRangeDomain.dMin = vaValue.dblVal;

	m_strctRangeDomain.bExist = TRUE;	// �����W�h���C�����������ꍇ�̓t���O�𗧂Ă�

	return &m_strctRangeDomain;
}

CComVariant CFieldDef::GetCodeDomainValue(long idx)
{
	auto iter = find_if(m_mapDomain.begin(), m_mapDomain.end(), 
		[&idx](const CODEVALUE_DOMAIN& domain) 
	{
		return domain.idx == idx;
	});

	if (iter == m_mapDomain.end())
		return CComVariant();

	return iter->code;
}

VARTYPE CFieldDef::GetCodedDomainType()
{
	return m_mapDomain.empty() ? VT_NULL : m_mapDomain.begin()->code.vt;
}

/**
 * �ҏW�ł���t�B�[���h���ǂ����̔�����s��
 *
 * @return �ҏW�ł���t�B�[���h�Ȃ� TRUE�A�ҏW�ł��Ȃ��Ȃ� FALSE ��Ԃ�
 *
 * \note ���̊֐��ł͖{���ҏW�ł��Ȃ��t�B�[���h�̂ق��ɁASiNDY-e �I�Ƀ��[�U�ɕύX���ė~�����Ȃ��t�B�[���h���܂�ł��܂��B
 * \note ����������邱�Ƃ��\�z�����̂ŁA���̏ꍇ�͓K�X�ǉ�����K�v������܂��B
 */
BOOL CFieldDef::IsEditable()
{
	return AheIsEditableField( m_ipField );
}
/**
 * �f�o�b�O���[�h���A�����o�ϐ����A�E�g�v�b�g�E�B���h�E�Ƀ_���v����
 */
void CFieldDef::dump()
{
#ifdef _DEBUG	
	CComVariant vaValue, vaInitValue;
	vaValue.ChangeType( VT_BSTR, &m_vaValue );
	vaInitValue.ChangeType( VT_BSTR, &m_vaInitValue );

	ATLTRACE(_T("TABLE:%s FIELD:%s UNSET:%s UNSETOLD:%s NULLABLE:%s INITED:%s CHANGED:%s VALUE:%s INIVALUE:%s HASDOMAIN:%s RELTYPE:%d INDEX:%d\n"),
		m_strTableName, m_strFieldName, m_bIsUnset ? _T("TRUE") : _T("FALSE"), m_bIsUnsetOld ? _T("TRUE") : _T("FALSE"),
		m_bIsNullable ? _T("TRUE") : _T("FALSE"), m_bIsInited ? _T("TRUE") : _T("FALSE"), m_bChanged ? _T("TRUE") : _T("FALSE"),
		COLE2CT(vaValue.bstrVal), COLE2CT(vaInitValue.bstrVal), m_bIsNotDomain ? _T("FALSE") : _T("TRUE"), m_nRelType, m_lFieldIndex );
#endif
}
/**
 * m_vaValue����m_vaInitValue�̒l�𕶎���ɕϊ�����
 *
 * @param bIsInitValue	[in]	TRUE �̏ꍇ�� m_vaInitValue�AFALSE �̏ꍇ�� m_vaValue ���ϊ��ΏۂɂȂ�
 *
 * @return �ϊ���̒l��������Ƃ��ċA��
 *
 * \note NULL�A���Ȃǂ̓���ȏꍇ���l�����Ă���ɂ������������Ԃ��܂��B
 */
CString CFieldDef::value2string( BOOL bIsInitValue )
{
	CComVariant vaValue(_T("")), vaTarget;
	CString strRet;

	if( bIsInitValue ) 
		vaTarget = m_vaInitValue;
	else 
		vaTarget = m_vaValue;

	switch( vaTarget.vt )
	{
		case VT_NULL:
			strRet = _T("");
			break;
		case VT_EMPTY:
			strRet = EMPTY_VALUE;
			break;
		case VT_UNKNOWN:
			strRet = GEOMETRY_VALUE;
			break;
		default:
			vaValue.ChangeType( VT_BSTR, &vaTarget );
			strRet.Format( _T("%s"), COLE2T(vaValue.bstrVal));
			break;
	}
	
	return strRet;
}
/**
 * @brief �l���K�p�\���ǂ����`�F�b�N����
 *
 * @note �����W�h���C���̂݃`�F�b�N���Ă��܂�
 *
 * @param vaValue		[in]	�`�F�b�N����l
 *
 * @retval TRUE	�K�p�\
 * @retval FALSE �K�p�s�\
 */
BOOL CFieldDef::CheckValue( const CComVariant& vaValue, HWND hWnd )
{
	HWND hArcWnd = NULL;

	// NULL OK ����Ȃ��̂� VT_NULL �Ȃ�G���[
	if( vaValue.vt == VT_NULL )
	{
		if( ! m_bIsNullable )
			return FALSE;
	}
	else {
		// �����W�h���C�����l��
		RANGEDOMAIN* pDomain = GetRangeDomain();
		if( pDomain && pDomain->bExist )
		{
			if( vaValue.vt != VT_I4 )
			{
				AheMessageBox( hWnd, ERROR_ENTER_NUMBER, ERROR_INPUT_VALUE, MB_OK|MB_ICONWARNING|MB_TASKMODAL, WARNLEVEL_HIGH );
				return FALSE;
			}
			else {
				if( pDomain->dMax < (DOUBLE)vaValue.lVal || pDomain->dMin > (DOUBLE)vaValue.lVal )
				{
					CString strMsg;
					strMsg.Format(ERROR_OUT_OF_LIMITS, vaValue.lVal, pDomain->dMin, pDomain->dMax );
					AheMessageBox( NULL, strMsg, ERROR_INPUT_VALUE, MB_OK|MB_ICONWARNING|MB_TASKMODAL );
		
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

BOOL CFieldDef::SetNewValue( CComVariant vaNewValue, HWND hWnd )
{
	if( ! CheckValue( vaNewValue, hWnd ) )
		return FALSE;
	else if( vaNewValue != m_vaValue )
	{
		m_vaValue = vaNewValue;

		// ���߂ăZ�b�g���ꂽ�ꍇ
		// �������Ȃ��� CFieldDef::Store() ���ɂ͂������
		if( m_bIsUnset ) 
			m_bIsUnset = FALSE;	

		if( m_vaValue == m_vaInitValue )
		{
			m_bChanged = FALSE;
			ATLTRACE(_T("CFieldDef::SetNewValue() : �l��������Ԃɖ߂�܂��� %s\n"), m_strFieldName );
		} else {
			m_bChanged = TRUE;
			ATLTRACE(_T("CFieldDef::SetNewValue() : �l���ύX����܂��� %s\n"), m_strFieldName );
		}
	}

	return TRUE;
}
