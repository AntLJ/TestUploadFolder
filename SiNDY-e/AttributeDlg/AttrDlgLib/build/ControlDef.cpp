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

// ControlDef.cpp: CControlDef �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../ControlDef.h"

#include <TDC/useful_headers/str_util.h>

//#include <atlhost.h>

namespace {
#ifdef SINDY_FOR_ENGLISH
	LPCTSTR ERROR_MORE_TAHN_32_DIGITS = _T("You can not enter more than 32 digits.");
	LPCTSTR ERROR_CAPTION             = _T("ERROR");
	LPCTSTR WARN_OVER_1024_BYTES      = _T("You can not enter more than 1024 bytes. Please note that the value will not be set as being entered now. ");
#else
	LPCTSTR ERROR_MORE_TAHN_32_DIGITS =	_T("32���ȏ���͂ł��܂���");
	LPCTSTR ERROR_CAPTION             = _T("�G���[");
	LPCTSTR WARN_OVER_1024_BYTES      = _T("1024�o�C�g�ȏ���͂ł��܂���B�����͂���Ă���ʂ�ɒl�̓Z�b�g����܂���B�����ӂ��������B");
#endif


	/**
	 * @brief �w��VARTYPE �������^���ۂ�
	 * @param type VARTYPE
	 * @retval true  �����^�ł���
	 * @retval false �����^�ł͂Ȃ�
	 */
	bool isIntegerType(VARTYPE type)
	{
		return
			type == VT_I2 || type == VT_I4  || type == VT_UI2 || type == VT_UI4 ||
			type == VT_I8 || type == VT_UI8 || type == VT_INT || type == VT_UINT
			;
	}
}

/**
 * �R���X�g���N�^
 */
CControlDef::CControlDef()
{
	m_ipRule.CreateInstance( CLSID_SiNDYRuleObj );
	Clear();
}
/**
 * �f�X�g���N�^
 */
CControlDef::~CControlDef()
{
	Clear();
}
void CControlDef::SetControlDef(IApplication* ipApp,    
								INT			nID, 
								HWND				hWnd,
								  CONTROLTYPE	type, 
								  INT			nGroupFrom,
								  INT			nGroupTo,
								  LPCTSTR		lpszFieldName/* = NULL*/, 
								  UINT			uiCheckType/* = NULL */,
								  LPCTSTR		lpszTableName/* = NULL */, 
								  LPCTSTR		lpszOuterKey/* = NULL */, 
								  INT			nFollowCtrlID/* = NULL */,
								  UINT			uiFollowBy/* = NULL */,
								  INT			nRelType)

{
	m_nID = nID;
	m_uiCheckType = uiCheckType;
	m_nGroupFrom = nGroupFrom;
	m_nGroupTo = nGroupTo;
	m_type = type;
	m_nFollowCtrlID = nFollowCtrlID;
	m_uiFollowBy = uiFollowBy;
	m_cArcHelper.Init( (IApplicationPtr)ipApp );

	m_hWnd = ::GetDlgItem( hWnd, m_nID );

	m_strTableName = lpszTableName;
	m_listFieldName.push_back( lpszFieldName );
}
void CControlDef::SetControlDef(IApplication* ipApp,  
								INT			nID, 
								HWND			hWnd,
								  CONTROLTYPE	type, 
								  INT			nGroupFrom,
								  INT			nGroupTo,
								  std::list<CString> arrayFieldNames, 
								  UINT			uiCheckType/* = NULL */,
								  LPCTSTR		lpszTableName/* = NULL */, 
								  LPCTSTR		lpszOuterKey/* = NULL */, 
								  INT			nFollowCtrlID/* = NULL */,
								  UINT			uiFollowBy/* = NULL */,
								  INT			nRelType/*,
								  IUnknownPtr	ipUnk*//* = NULL */)

{
	m_nID = nID;
	m_uiCheckType = uiCheckType;
	m_nGroupFrom = nGroupFrom;
	m_nGroupTo = nGroupTo;
	m_type = type;
	m_nFollowCtrlID = nFollowCtrlID;
	m_uiFollowBy = uiFollowBy;
	m_cArcHelper.Init( (IApplicationPtr)ipApp );

	m_hWnd = ::GetDlgItem( hWnd, m_nID );

	m_strTableName = lpszTableName;
	m_listFieldName = arrayFieldNames;
}
void CControlDef::SetFeatureDefList( std::list<FEATUREDEF>* pFeatureDefList )
{
	ATLASSERT( pFeatureDefList != NULL );
	ClearFeatureDef();

	m_pFeatureDefList = pFeatureDefList;
}
void CControlDef::SetCurrentFeatureDefIndex( LONG lIndex, LONG lTableIndex, LONG lRowIndex )
{
	/// �^����ꂽ�C���f�b�N�X���t�B�[�`���̐������Ȃ��Ȃ�߂�
	if( !m_pFeatureDefList || m_pFeatureDefList->size() <= (UINT)lIndex ) 
	{
		m_pFeatureDef = NULL;
		m_lTableIndex = m_lRowIndex = 0;

		return;
	}
	std::list<FEATUREDEF>::iterator it = m_pFeatureDefList->begin();
	advance( it, lIndex );

	m_pFeatureDef = &(*it);

	m_lTableIndex = lTableIndex;
	m_lRowIndex = lRowIndex;
}
CFieldDef* CControlDef::GetFieldDef( LPCTSTR lpcszFieldName )
{
	if( !lpcszFieldName ) {
		lpcszFieldName = *m_listFieldName.begin();

		/// ���̊֐��͑Ή�����t�B�[���h�̐�����̏ꍇ�Ɍ���Ȃ̂�
		if( m_listFieldName.size() > 1 ) {
			ATLASSERT(FALSE);
			return NULL;
		}
	}

	/// �t�B�[�`��������o�^����Ă��Ȃ��ꍇ�� NULL ��Ԃ�
	if( !m_pFeatureDef ) return NULL;

	/// �t�B�[�`���t�B�[���h
	// TYPE_FEATURESGRID �̎��̓X���[����悤�ɂ���iCTableEditDlg::SortByField() �����܂������Ȃ����߁j
	if( m_type == TYPE_FEATURESGRID || lstrcmpi( m_pFeatureDef->GetTableName(), m_strTableName ) == 0 )
	{
		for( std::list<CFieldDef>::iterator itField = m_pFeatureDef->begin(); itField != m_pFeatureDef->end(); itField++ )
		{
			if( 0 == itField->m_strFieldName.CompareNoCase( lpcszFieldName ) )
				return &(*itField);
		}
	}
	/// �O���e�[�u���t�B�[���h
	else
	{
		std::list<CRelationalTableDef>* pRelationalTableDef = m_pFeatureDef->GetRelationalTableDefList();
		for( std::list<CRelationalTableDef>::iterator it = pRelationalTableDef->begin(); it != pRelationalTableDef->end(); it++ ) 
		{
			if( lstrcmpi( it->GetTableName(), m_strTableName ) == 0 ) 
			{
				/// LINK_TIME_REG �ŌŒ�l�h���C�����擾���鎞�ɂǂ����Ă� ASSERT �ɂȂ�̂ŁA�����͊O��
				//ATLASSERT( it->size() == 1 );	//!< ���̊֐��͑Ή�����t�B�[���h�̐�����̏ꍇ�Ɍ���Ȃ̂�
				ATLTRACE(_T("CControlDef::GetFieldDef() : ���̊֐��͑Ή�����t�B�[���h����̏ꍇ�Ɍ��萳��ɓ��삵�܂� NUM=%d\n"), it->size() );
				for( std::list<CRowDef>::iterator itRow = it->begin(); itRow != it->end(); itRow++ )
				{
					for( std::list<CFieldDef>::iterator itField = itRow->begin(); itField != itRow->end(); itField++ )
					{
						if( 0 == itField->m_strFieldName.CompareNoCase( lpcszFieldName ) )
							return &(*itField);
					}
				}
			}
		}
	}

	return NULL;
}
void CControlDef::dump( LPCTSTR s )
{

}
/**
 * �Ή�����t�B�[���h�̃h���C�����擾����
 *
 *  �f�[�^�x�[�X�ɖ₢���킹����͈̂�x����i��x�ǂݍ��񂾂�����ŕۑ��j
 * 
 * @return ���s�����ꍇ�� NULL ���Ԃ�
 */
DOMAINMAP* CControlDef::GetDomain( LPCTSTR lpcszFieldName )
{
	CFieldDef* pFieldDef = GetFieldDef( lpcszFieldName );

	if( !pFieldDef ) {
		ATLTRACE(_T("CControlDef::GetDomain(): �֘A����t�B�[���h������܂��� ID=%d\n"), m_nID );
		return FALSE;
	}

	return pFieldDef->GetDomain();
}
void CControlDef::Clear()
{
	m_nID = -1;
	m_type = 0;
	m_uiCheckType = 0x00;
	m_nGroupFrom = -1;
	m_nGroupTo = -1;
	m_nFollowCtrlID = -1;
	m_uiFollowBy = 0x00;
	m_cArcHelper.Release();
	m_hWnd = NULL;


	m_bErrored = FALSE;
	ClearFeatureDef();

	m_bAliasOrField = FALSE;

	m_listFieldName.clear();
}

/* ����Ȃ��H*/
void CControlDef::Reset()
{
	// �G���[�t���O�͂����ŃN���A
	m_bErrored = FALSE;
}
BOOL CControlDef::GetOldValue( CComVariant& vaValue )
{
	CFieldDef* pFieldDef = GetFieldDef();

	if( !pFieldDef ) {
		ATLTRACE(_T("CControlDef::GetOldValue(): �֘A����t�B�[���h������܂��� ID=%d\n"), m_nID );
		return FALSE;
	}

	vaValue = pFieldDef->m_vaValue;

	return TRUE;
}
void CControlDef::GetStringValue( CString& str )
{
	CFieldDef* pFieldDef = GetFieldDef();

	ATLASSERT( pFieldDef != NULL );
	if( !pFieldDef ) return ;
	
	str = pFieldDef->value2string();
}
/**
 * �^����ꂽLONG�l�� m_vaValue �ɃZ�b�g
 *  ���̍ہA�����l����ύX����Ă���� m_bChanged = TRUE �ɁA�ύX����Ă��Ȃ���� FALSE �ɃZ�b�g����
 *  ��ԍŏ��̃t�B�[���h�������Ȃ��̂ŁA�����̃t�B�[���h���������̂ɂ͓K�p�ł��Ȃ�
 * 
 * @param l				[in]	�Z�b�g���� LONG �l
 *
 */
void CControlDef::SetValue( CComVariant vaValue )
{
	CFieldDef* pFieldDef = GetFieldDef();

	if( !pFieldDef ) return ;

	// [Bug4731]�l�̃`�F�b�N�͂��邯��ǂ��A�l���͈͊O�������肵���Ƃ��Ă��A���݂̒l��ێ����Ă����B
	// �͈͊O�̒l�������Ƃ��Ă��A����͍ŏI�I�Ƀ`�F�b�N����m��ł��Ȃ����߁A���Ȃ��B
	pFieldDef->CheckValue( vaValue, GetParent( m_hWnd ) );
	pFieldDef->m_vaValue = vaValue;

	/// ���߂ăZ�b�g���ꂽ�ꍇ
	/// �������Ȃ��� CFieldDef::Store() ���ɂ͂������
	if( pFieldDef->m_bIsUnset )
		pFieldDef->m_bIsUnset = FALSE;

	// [Bug 5794] [Bug5775]�̏C���ɂ�葮���l�̕ύX�����ɖ߂����ꍇ�̏�����������Ă��܂����̂ł�����m_bChanged��߂���悤�ɂ���
	if( (pFieldDef->m_vaValue).vt == VT_BSTR )
	{
		// [bug 5775] VT_BSTR �� VARIANT ���I�y���[�^�Ŕ�r����Ɓu�\�v�Ɓu�[�v����ʂ���Ȃ��悤��
		if( pFieldDef->m_vaInitValue.vt == VT_BSTR && 0 == wcscmp((pFieldDef->m_vaValue).bstrVal, (pFieldDef->m_vaInitValue).bstrVal))
			pFieldDef->m_bChanged = FALSE;
		// DATE�^�͕�����Ƃ��ď������邽�߁A��r����DATE�^�ɖ߂�
		else if( pFieldDef->m_vaInitValue.vt == VT_DATE )
		{
			CComVariant vaDateValue = vaValue;
			vaDateValue.ChangeType( VT_DATE );

			if( vaDateValue.date == (pFieldDef->m_vaInitValue).date )
				pFieldDef->m_bChanged = FALSE;
			else
				pFieldDef->m_bChanged = TRUE;
		}
		else
			pFieldDef->m_bChanged = TRUE;
	}
	else
	{
		if( pFieldDef->m_vaValue != pFieldDef->m_vaInitValue )
			pFieldDef->m_bChanged = TRUE;
		else
			pFieldDef->m_bChanged = FALSE;
	}
	ATLTRACE( pFieldDef->m_bChanged ? _T("CControlDef::SetValue() : �l���ύX����܂��� %s\n") : _T("CControlDef::SetValue() : �l��������Ԃɖ߂�܂��� %s\n"), pFieldDef->m_strFieldName );


	// �G�f�B�b�g�R���g���[���̏ꍇ�� _ttol �ŕϊ����Ă���̂ōĕ\�������ق����ǂ�
	// �󔒂͑S�� <null> �ƕ\���������̂ŁA�X�g�����O�^�C�v���ǉ�
//	if( m_type == TYPE_EDIT || m_type == TYPE_EDITSTRING ) SetEdit();
}
/**
 * �R���g���[������l���擾���A�ύX����Ă���� m_vaValue �ɒl���Z�b�g����
 *
 *  �ύX�t���O�� SetLongValue() �ŃZ�b�g����
 *
 * @return m_vaValue �̒l���ύX���ꂽ�� TRUE�A�ύX����Ȃ������� FALSE
 */
BOOL CControlDef::SetValueFromControl()
{
	CComVariant vaCtrlVal;
	if( !GetControlValue( vaCtrlVal ) ) return FALSE;

	CComVariant vaOldVal;
	GetOldValue( vaOldVal );


	// [bug 5775] VT_BSTR �� VARIANT ���I�y���[�^�Ŕ�r����Ɓu�\�v�Ɓu�[�v����ʂ���Ȃ��悤��
	if( vaCtrlVal.vt == VT_BSTR && vaOldVal.vt == VT_BSTR && (0 != wcscmp( vaCtrlVal.bstrVal, vaOldVal.bstrVal )) )
	{
		SetValue( vaCtrlVal );
		return TRUE;
	}

	// DATE�^�͕�����Ƃ��ď������邽�߁A��r����DATE�^�ɖ߂�
	else if( vaCtrlVal.vt == VT_BSTR && vaOldVal.vt == VT_DATE )
	{
		CComVariant vaDateVal = vaCtrlVal;
		vaDateVal.ChangeType( VT_DATE );
		if( vaDateVal != vaOldVal )
		{
			SetValue( vaCtrlVal );
			return TRUE;
		}
	}

	/// �ύX����Ă���
	else if( vaCtrlVal != vaOldVal ) {
		SetValue( vaCtrlVal );
		return TRUE;
	}
	else {
		// NULL �̏ꍇ�́A<null> �Ƌ󔒂̏ꍇ������̂ŁA�K�� <null> �\���ɂ��Ȃ���
		if( vaCtrlVal.vt == VT_NULL )
			SetValue( vaCtrlVal );
	}

	return FALSE;
}
/**
 * ���݃R���g���[���ɕ\������Ă���E���邢�͑I������Ă���l���擾
 *
 * LONG�l����
 * �G�f�B�b�g�R���g���[���̏ꍇ�͖������LONG�l�ɒ������߁A���l�����������Ȃ�ꍇ����
 *
 * @param def			[in]	�R���g���[��
 * @param lVal			[out]	�擾���ꂽ�l
 * 
 * @return �l���擾�ł���� TRUE�A�o���Ȃ���� FALSE
 */
BOOL CControlDef::GetControlValue( CComVariant& vaVal )
{
	TCHAR strVal[2000];
	INT nByte = -1;
	_ASSERTE( vaVal.vt == VT_NULL || vaVal.vt == VT_EMPTY );
	vaVal.Clear();

	switch( m_type ) {
	case TYPE_COMBO:
	{
		int nCurSel = (int)::SendMessage(m_hWnd, CB_GETCURSEL, 0, 0L);
		if (0 <= nCurSel)
		{
			DWORD dwTmp = ::SendMessage(m_hWnd, CB_GETITEMDATA, nCurSel, 0L);
			// �R���{�{�b�N�X�Ŋ��� 0xFFFFFFF ���Z�b�g����Ă����� NULL �Ƃ���B @see SetComboBox()
			if (dwTmp == 0xFFFFFFF)
			{
				vaVal.vt = VT_NULL;
			}
			else
			{
				// ���OK�{�^���������ꏉ������ɂ����ɔ��ŗ��邱�Ƃ�����B
				// ���̂��߁A������FieldDef �����Ȃ��ꍇ�͉��������I������B
				CFieldDef* fieldDef = GetFieldDef();
				if (!fieldDef)
					return FALSE;
				// �����^�̃h���C���l�Ȃ�΁A�R���{�{�b�N�X�ɃR�[�h�l���i�[����Ă���̂ł��̂܂܎擾����
				// ����ȊO�̃h���C���^�́A�R���{�{�b�N�X�ɃC���f�b�N�X���i�[����Ă���̂ŁAGetCodeDomainValue() �Ŏ��o��
				// �h���C���łȂ��l���i�[����Ă���ꍇ��VT_NULL�ƂȂ�̂ŁA��������̂܂܎擾����
				VARTYPE type = fieldDef->GetCodedDomainType();
				if(isIntegerType(type) || type == VT_NULL )
					vaVal = (long)dwTmp;
				else
					vaVal = GetFieldDef()->GetCodeDomainValue(dwTmp);
			}
		}
		break;
	}
	case TYPE_EDIT:
	case TYPE_EDITFLOAT:
	case TYPE_EDITDOUBLE:
		// [Bug4843]���l���͂�32���ȏ�͂Ȃ��Ə���ɂ��Ă��܂�
		if( ::GetWindowTextLength( m_hWnd ) > 32 ) {
			AheMessageBox( NULL, ERROR_MORE_TAHN_32_DIGITS, ERROR_CAPTION, MB_OK|MB_TASKMODAL );
			return FALSE;
		}
		::GetWindowText( m_hWnd, strVal, 32 );	//!< 32�o�C�g�ȏ���͂��邱�Ƃ͖����Ɖ���
		// <null> �̏ꍇ���l��
		if( lstrcmpi( strVal, NULL_VALUE ) == 0 || lstrcmpi( strVal, _T("") ) == 0 )
			vaVal.vt = VT_NULL;
		else
		{
			if( m_type == TYPE_EDIT )
			{
				vaVal.vt = VT_I4;
				vaVal.lVal = _ttol( strVal );
			}
			// [Bug 5333]���s��NW(WALK_LINK)��FLOOR_LEVEL��FLOAT�̒l���Ƃ邽�߁A���ʑΉ����Ȃ��Ă͂����Ȃ�
			else if( m_type == TYPE_EDITFLOAT )
			{
				vaVal.vt = VT_R4;
				vaVal.fltVal = _ttof(strVal);
			}
			else if( m_type == TYPE_EDITDOUBLE )
			{
				vaVal.vt = VT_R8;
				vaVal.dblVal = _ttof(strVal);
			}
		}
		break;
	case TYPE_CHECKBOX:
		vaVal = (long)::SendMessage(m_hWnd, BM_GETCHECK, 0, 0L);
		break;
	case TYPE_EDITSTRING:
		// [Bug 5418]����������1024�o�C�g�܂�OK�ɂȂ����B��	// [Bug 4891]������������512�o�C�g�܂ł�OK�Ƃ̂���
		// GetWindowTextLength���ƁA�������ł̃`�F�b�N�ɂȂ��Ă��܂��̂ŁAANSI�ɕϊ����o�C�g���̃`�F�b�N�ɂ���
		// 1024�{�I�[�R�[�h�{1�o�C�g�擾���ă`�F�b�N�A::GetWindowText�̑�3������UNICODE���ł͕������ƂȂ��Ă��܂�
		// �̂ł��܂�Ӗ��͂Ȃ������A�A
		::GetWindowText( m_hWnd, strVal, 1026 );	 
		nByte = ::WideCharToMultiByte(CP_ACP, 0, T2CW((LPCTSTR)(strVal)), -1, NULL, 0, NULL, NULL) - 1;
		if( nByte > 1024 )
			AheMessageBox( NULL, WARN_OVER_1024_BYTES, ERROR_CAPTION, MB_OK|MB_TASKMODAL );
		// <null> �̏ꍇ���l��
		if( lstrcmpi( strVal, NULL_VALUE ) == 0 || lstrcmpi( strVal, _T("") ) == 0 )
			vaVal.vt = VT_NULL;
		else
			vaVal = (LPCTSTR)strVal;
		break;
	default:
		return FALSE;
		break;
	}

	return TRUE;
}
/**
 * �i�[����Ă���l���G���[���ǂ����̔���
 *
 * @return �G���[�Ȃ� TRUE�A�����łȂ���� FALSE
 */
BOOL CControlDef::Errored()
{
	return m_bErrored;
}
/**
 * �R���g���[���Ɋ֘A����t�B�[���h�ɕύX�����邩�ǂ����̔���
 *
 * @return �ύX������� TRUE�A�ύX���Ȃ���� FALSE
 */
BOOL CControlDef::Changed()
{
	FEATUREDEF* pFeatureDef = NULL;
	BOOL bRet = FALSE;

	// �R���g���[����TYPE_FEATURESGRID�̏ꍇ�A�S�Ẵt�B�[�`���̃t�B�[���h�𒲂ׂ�K�v������
	if( m_type == TYPE_FEATURESGRID || m_type == TYPE_FEATUREGRID) {
		if( !m_pFeatureDefList )
			return FALSE;

		for( std::list<FEATUREDEF>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
		{
			bRet = _Changed( &(*it) );
			
			// ��ł��ύX����������I��
			if( bRet )
				break;
		}
	} 
	// ����ȊO�̃^�C�v�̏ꍇ
	else {
		if( !m_pFeatureDef ) return FALSE;
	
		pFeatureDef = m_pFeatureDef;
		bRet = _Changed( pFeatureDef );
	}

	return bRet;
}
/**
 * Changed() ����Ă΂������֐�
 *
 * @param pFeatureDef �t�B�[�`����`�N���X
 *
 * @return �ύX������� TRUE�A�Ȃ���� FALSE
 */
BOOL CControlDef::_Changed( FEATUREDEF* pFeatureDef )
{
	ATLASSERT( pFeatureDef != FALSE );

	CFieldDef* pFieldDef = NULL;
	BOOL bRet = FALSE;

	// �t�B�[�`���t�B�[���h
	if( lstrcmpi( pFeatureDef->GetTableName(), m_strTableName ) == 0 ||
		m_type == TYPE_FEATUREGRID )	// TYPE_FEATUREGRID �̏ꍇ�̓t�B�[���h�����e�[�u�������o�^����Ȃ�
	{
		for( std::list<CFieldDef>::iterator itField = pFeatureDef->begin(); itField != pFeatureDef->end(); ++itField )
		{
			// TYPE_FEATUREGRID �̏ꍇ�̓t�B�[���h�����o�^����Ȃ��̂� m_listFieldName �ŉ񂷂̂� NG
			if( m_type == TYPE_FEATUREGRID )
			{
				if( itField->m_bChanged )
				{
					bRet = TRUE;
					pFieldDef = &(*itField);
					break;
				}
			} 
			// TYPE_FEATUREGRID �ȊO
			else {
				for( std::list<CString>::iterator itFieldName = m_listFieldName.begin(); itFieldName != m_listFieldName.end(); ++itFieldName )
				{
					if( 0 == itField->m_strFieldName.CompareNoCase( *itFieldName ) )
					{
						if( itField->m_bChanged )
						{
							bRet = TRUE;
							pFieldDef = &(*itField);
							break;
						}
					}
				}
			}
			if( bRet )
				break;
		}
	}
	// �O���e�[�u���t�B�[���h
	else
	{
		std::list<CRelationalTableDef>* pRelationalTableDef = pFeatureDef->GetRelationalTableDefList();
		for( std::list<CRelationalTableDef>::iterator it = pRelationalTableDef->begin(); it != pRelationalTableDef->end(); it++ ) 
		{
			if( 0 == m_strTableName.CompareNoCase( it->GetTableName() ) ) 
			{
				// LINK_TIME_REG �ŌŒ�l�h���C�����擾���鎞�ɂǂ����Ă� ASSERT �ɂȂ�̂ŁA�����͊O��
				///ATLASSERT( it->size() == 1 );	// ���̊֐��͑Ή�����t�B�[���h�̐�����̏ꍇ�Ɍ���Ȃ̂�
				ATLTRACE(_T("CControlDef::Changed() : ���̊֐��͑Ή�����t�B�[���h����̏ꍇ�Ɍ��萳��ɓ��삵�܂� NUM=%d\n"), it->size() );
				for( std::list<CRowDef>::iterator itRow = it->begin(); itRow != it->end(); ++itRow )
				{
					// �O���e�[�u���̃��R�[�h�ɐV�K�쐬�t���O���͍폜�t���O���t���Ă�����ύX����Ƃ���
					if( itRow->GetDeleteFlag() || itRow->GetCreateFlag() )
					{
						// �f�o�b�O
						ATLTRACE(_T("CControlDef::Changed()�F�O���e�[�u���̃��R�[�h�ɐV�K�쐬�t���O���͍폜�t���O���t���Ă��܂�\n"));

						return TRUE;
					}

					for( std::list<CFieldDef>::iterator itField = itRow->begin(); itField != itRow->end(); itField++ )
					{
						for( std::list<CString>::iterator itFieldName = m_listFieldName.begin(); itFieldName != m_listFieldName.end(); itFieldName++ )
						{
							if( 0 == itField->m_strFieldName.CompareNoCase( *itFieldName ) )
							{
								if( itField->m_bChanged )
								{
									bRet = TRUE;
									pFieldDef = &(*itField);
									break;
								}
							}
						}
						if( bRet )
							break;
					}
					if( bRet )
						break;
				}
			}
			if( bRet )
				break;
		}
	}
	// �f�o�b�O
	if( bRet )
	{
		ATLTRACE(_T("CControlDef::Changed()�F�l���ύX����Ă��܂��F%s value=%s and init value=%s\n"), 
			pFieldDef->m_strFieldName, pFieldDef->value2string(), pFieldDef->value2string(TRUE) );
	}

	return bRet;
}
/**
 * ���̃R���g���[���� DISP_CLASS �������R���g���[�����ǂ����̔���
 *
 * @return DispClass �������R���g���[���Ȃ� TRUE�ADispClass �ł͖����܂��͕������R�[�h�A�����t�B�[���h�̏ꍇ�� FALSE
 */
BOOL CControlDef::IsDispClass()
{
	if( m_listFieldName.size() != 1 ) return FALSE;

	if( lstrcmpi( *m_listFieldName.begin(), _T("ROADCLASS_C") ) == 0 ) return TRUE;

	return FALSE;
}
BOOL CControlDef::IsUnset()
{
	CFieldDef* pFieldDef = GetFieldDef();

	if( !pFieldDef ) {
		ATLTRACE(_T("CControlDef::IsUnset(): �֘A����t�B�[���h������܂��� ID=%d\n"), m_nID );
		return TRUE;
	}

	return pFieldDef->m_bIsUnset;
}
LPCTSTR CControlDef::GetTableName()
{	
	return m_strTableName;
}
/**
 * ���̃R���g���[���������t�B�[���h�����擾
 *  �����t�B�[���h�������R���g���[���̏ꍇ�͎g�p���Ȃ����Ɓi�������R�[�h�̏ꍇ��OK�j
 *
 * @return �����t�B�[���h�̏ꍇ�� NULL ��Ԃ�
 */
LPCTSTR CControlDef::GetFieldName()
{	
	if( m_listFieldName.size() != 1 ) return NULL;

	return *m_listFieldName.begin();

}

/**
 * �R���g���[���̓��͏�Ԃ�ύX����
 *
 * @param bEnable	[in]	Enable �ɂ���ꍇ�� TRUE�ADisable �ɂ���ꍇ�� FALSE
 * @param bForce	[in]	�����I�ɐ��䂷�邩�ǂ����i�ҏW�J�n�A�I���̃n���h�����O���݂̂Ɏg�p�j
 * @param bEditable	[in]	�����I�ɕҏW�E�s��ݒ肷��i�ҏW�J�n�A�I���̃n���h�����O���݂̂Ɏg�p�j
 */
void CControlDef::ChangeEnable( BOOL bEnable, BOOL bForce, BOOL bEditable )
{
	if ( m_type == TYPE_NONE )
		return;

	if( (m_type == TYPE_OTHER) || (m_type == TYPE_FEATUREGRID) || (m_type ==  TYPE_TIMEREG) || (m_type ==  TYPE_OWTIMEREG) ||
		(m_type == TYPE_OUTERROWS) || (m_type ==TYPE_FEATURESGRID) /*|| (m_type == TYPE_WALKONEWAY) || (m_type == TYPE_WALKNOPASSAGE)*/)
	{
		bEnable = TRUE;
	}
	else {

		// �ҏW�s�̎��ɂ͐�ΕҏW�����Ȃ�
		if( m_pFeatureDef->m_sindyeEditableType == sindyeEditableNoEditable )
			bEnable = FALSE;

		// �ҏW�x���̎��ɂ́A�ҏW�x�������{�^�������������ǂ����Ń`�F�b�N
		if( m_pFeatureDef->m_sindyeEditableType == sindyeEditableWarnning )
		{
			if( m_pFeatureDef->GetWarnDelete() )
				bEnable = TRUE;
			else 
				bEnable = FALSE;
		}

		// �����I�ɕҏW�s�̎��͐�΂ɕҏW�����Ȃ����[�j���O�Ƃ��֌W�Ȃ�
		if( bForce && !bEditable ) 
			bEnable = FALSE;
	}


	DWORD dwRemove, dwAdd;

	if( !(m_type == TYPE_EDIT || m_type == TYPE_EDITFLOAT || m_type == TYPE_EDITDOUBLE || m_type == TYPE_EDITSTRING )) {
		if( bEnable ) {
				dwAdd = NULL;
				dwRemove = WS_DISABLED;
		} else {
			dwAdd = WS_DISABLED;
			dwRemove = NULL;
		}
		DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if(dwStyle == dwNewStyle)
			return;
		::SetWindowLong(m_hWnd, GWL_STYLE, dwNewStyle);
	}
	/// �G�f�B�b�g�R���g���[���̏ꍇ����
	if( m_type == TYPE_EDIT || m_type == TYPE_EDITFLOAT || m_type == TYPE_EDITDOUBLE || m_type == TYPE_EDITSTRING ) {
		if( bEnable )
			::SendMessage( m_hWnd, EM_SETREADONLY, FALSE, 0L );
		else
			::SendMessage( m_hWnd, EM_SETREADONLY, TRUE, 0L );
	}

	return ;
}
/**
 * �R���{�{�b�N�X�̃��X�g�̍쐬���s��
 *
 * DISPCLASS_C �̒l�Ń`�F�b�N���A���X�g�̓��e�����肵�܂��B
 *
 * @param lDispClass	[in]	DISPCLASS_C �̒l
 */
void CControlDef::SetComboBox( LONG lDispClass )
{
	//IFieldsPtr           ipFields           = NULL;
	//IFieldPtr            ipField            = NULL;
	//IDomainPtr           ipDomain           = NULL; 
	//ICodedValueDomainPtr ipCodedValueDomain = NULL;


	USES_CONVERSION;

	// �G���[�t���O�͂����ň�U FALSE �ɂ��Ă����Ȃ��ƁA�t�B�[�`����؂�ւ�������L���ɂȂ��Ă��܂�
	m_bErrored = FALSE;

	DOMAINMAP* mapDomain = GetDomain();

	// �R���{�{�b�N�X���̃A�C�e����S�ăN���A
	CComboBox cmb( m_hWnd );
	cmb.ResetContent();

	// CFieldDef::m_vaValue �ɓo�^����Ă���l���擾
	CComVariant vaOldValue;
	GetOldValue( vaOldValue );

	// �l����h���C��
	if( mapDomain )
	{
		for( auto& domain : *mapDomain )
		{
			CString strDomainName	= domain.code_discription;
			cmb.InsertString( domain.idx, strDomainName );

			if(isIntegerType(domain.code.vt))
				cmb.SetItemData( domain.idx, domain.code.lVal );
			else {
				// �����^�̃h���C���ȊO�̂Ƃ��͎b��I�ɃC���f�b�N�X�̒l�����Ă���
				cmb.SetItemData(domain.idx, domain.idx);
			}

			// ���݂̒l�Ɠ����������������炻���\��
			if( vaOldValue == domain.code ) {
				cmb.SetCurSel( domain.idx );
			}
		}
	}

	// Nullable �ȃt�B�[���h�̏ꍇ�̓��X�g�ɒǉ�
	if( IsNullable() ) 
	{
		BOOL bNullAdd = TRUE;
		// DISPOTHER �ŕK�{�ɂȂ��Ă�����̂� NULL �̓R���{�{�b�N�X�ɓ���Ȃ�
		if( m_uiCheckType & CHECK_DISPOTHER )
		{
			sindyeDispOtherRelation nDispOtherRel;
			m_ipRule->GetDispOtherRule( lDispClass, CComBSTR(GetFieldName()), &nDispOtherRel );
			if( ( nDispOtherRel == sindyeDispOtherOption ) || ( nDispOtherRel == sindyeDispOtherExceptNoRes ) )
				bNullAdd = FALSE;
		}

		if( bNullAdd )
		{
			long count = cmb.GetCount();
			cmb.InsertString(count, NULL_VALUE );
			cmb.SetItemData(count, 0xFFFFFFF );	// <null> �̒l�� 0xFFFFFFF �ɂ��Ă���
			if( vaOldValue.vt == VT_NULL ) {
				cmb.SetCurSel(count + 1 );
			}
		}
	}
}
void CControlDef::SetEdit()
{
	CString strVal;

	CEdit edit( m_hWnd );
	INT nTextLen = edit.GetWindowTextLength() + 1;
	LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
	edit.GetWindowText( lpszText, nTextLen );

	/// m_bIsUnset �� TRUE �̏ꍇ�͋󔒂��Z�b�g
	if( IsUnset() != TRUE ) {
		if( m_type == TYPE_EDITSTRING ) {
			GetStringValue( strVal );
		} else {
			CComVariant vaValue;
			GetOldValue( vaValue );
			// VT_NULL �̏ꍇ�͉�������Ȃ�
			if( vaValue.vt != VT_NULL )
			{
				// [Bug 5333]���s��NM��FLOOR_LEVEL�ŁA����VT_R4���o��
				// ���͑��i�q�_�C�A���O���j�ŏ����_1���܂ł������͂ł��Ȃ��悤�ɂ��Ă��܂��Ă���
				if( vaValue.vt == VT_R4 )
					strVal.Format( _T("%lf"), vaValue.fltVal ); 
				else
				if( vaValue.vt == VT_R8 )
					strVal.Format( _T("%lf"), vaValue.dblVal ); 
				else
				{
					ATLASSERT( vaValue.vt == VT_I4 );
					strVal.Format(_T("%d"), vaValue.lVal );	//!< �ʓ|�Ȃ̂�LONG����
				}
			}
		}
	}

	// �l���ς���Ă�����e�L�X�g������������
	if( strVal.Compare( lpszText ) != 0 )
		::SetWindowText( m_hWnd, strVal );

}
void CControlDef::SetCheckBox()
{
	CComVariant vaValue;
	GetOldValue( vaValue );
	//[Bug 5531]�Ђ܂��t���O�Ȃǂ̐̂̃f�[�^�ł̓t�B�[���h�̂Ȃ��l�̏ꍇ�ɂ̓`�F�b�N���͂����A�ҏW�s�ɂ��Ă���
	if( vaValue.vt == VT_EMPTY )
	{
		::SendMessage( m_hWnd, BM_SETCHECK, 0, 0L);
		::EnableWindow( m_hWnd, FALSE);
	}
	else
		::SendMessage( m_hWnd, BM_SETCHECK, vaValue.lVal, 0L);
}
void CControlDef::SetFeaturesGrid( BOOL bMulti )
{
	/// �O���b�h�擾
	IVSFlexGridPtr spGrid;
	CAxWindow wndGrid( m_hWnd );
	wndGrid.QueryControl(&spGrid);

	if( !spGrid )
	{
		ATLASSERT( spGrid != NULL );
		return;
	}

	if( bMulti ) {
		LONG lCols = 1;
		if( m_pFeatureDefList->size() > 0 )
		{
			lCols = m_listFieldName.size() + 1;
		}
		spGrid->put_Cols( lCols );
		spGrid->put_Rows( 1 );
		spGrid->put_FixedCols( 1 );
		spGrid->put_FixedRows( 1 );
	} else {
		/// �������i1�s2��B��s�ڂ� FixedRow�j�Ȃ�ł����ł��Ȃ���Ȃ��́H�킯�킩��ˁ[
		spGrid->put_Cols( 2 );
		spGrid->put_Rows( 1 );
		spGrid->put_FixedCols( 1 );
		spGrid->put_FixedRows( 1 );
	}
	LONG lCount = 0;
	
	/// ����t�B�[�`�����Z�b�g����Ă��Ȃ��Ȃ�߂�
	if( !m_pFeatureDefList ) return;

	for( std::list<FEATUREDEF>::iterator i = m_pFeatureDefList->begin(); i != m_pFeatureDefList->end(); i++, lCount++ ) 
	{
		CString strItem, strItemTmp;
		std::list<LONG> listChanged;
		std::list<LONG> listUnEditable;

		LONG lFieldCount = 0;
		for( std::list<CString>::iterator itFieldNameList = m_listFieldName.begin(); itFieldNameList != m_listFieldName.end(); itFieldNameList++, lFieldCount++ )
		{
			std::list<CFieldDef>::iterator j;
			for( j = i->begin(); j != i->end(); j++ ) 
			{
				/// �����t�B�[���h�������̂�T��
				if( (*itFieldNameList).CompareNoCase( j->m_strFieldName ) == 0 ) 
					break;
			}

			// �֌W�Ȃ��O���b�h�̏ꍇ�͂����Ȃ�
			if( j == i->end() )
			{
				return;
			}

			// bMulti == TRUE �̂Ƃ��̂݃t�B�[���h�̃G�C���A�X������s�ڂɓ����
			if( lCount == 0 && bMulti )
			{
				CComBSTR bstrName;
				j->m_ipField->get_AliasName( &bstrName );

				spGrid->put_TextMatrix( 0, lFieldCount + 1, bstrName );
			}

			CString str;
			// NULL �������ꍇ�̏���			
			if( j->m_vaValue.vt == VT_NULL )
			{
				str = NULL_VALUE;
			} 
			// NULL ����Ȃ��ꍇ�̏���
			else 
			{
				DOMAINMAP* domain = j->GetDomain();
				if( !domain ) {
					va2str( j->m_vaValue, str );
				} else {
					for( DOMAINMAP::iterator l = domain->begin(); l != domain->end(); l++ ) {
						if( l->code == j->m_vaValue ) {
							str = l->code_discription;
							break;
						}
					}
				}
			}
			strItemTmp = strItem;
			if( strItemTmp.Compare( _T("") ) == 0 ) {
				strItem.Format(_T("%d\t%s"), lCount, str );
			} else {
				strItem.Format(_T("%s\t%s"), strItemTmp, str );
			}

			// �ҏW����Ă���t�B�[���h�̓C���f�b�N�X�ԍ��ێ�
			if( j->m_bChanged )
				listChanged.push_back( lFieldCount );
			// �ҏW�s�̃t�B�[���h�̓C���f�b�N�X�ԍ��ێ�
			if( !j->IsEditable() )
				listUnEditable.push_back( lFieldCount );
		}
		spGrid->AddItem( _bstr_t( strItem ) );

		std::list<LONG>::iterator n;
		// �F��ς���t�B�[���h�s��ǉ�������ŕύX�i�ҏW����Ă���t�B�[���h�j
		for( n = listChanged.begin(); n != listChanged.end(); n++ )
			SetGridColorToRed( spGrid, lCount + 1, (*n) + 1 );
		// �F��ς���t�B�[���h�s��ǉ�������ŕύX�i�ҏW�s�̃t�B�[���h�j
		for( n = listUnEditable.begin(); n != listUnEditable.end(); n++ )
			SetGridColorToUnEdit( spGrid, lCount + 1, (*n) + 1 );
		// Delete�t���O�������Ă�����O���[�\��
		if( i->GetDeleteFlag() )
		{
			for( ULONG lSel = 1; lSel <= m_listFieldName.size(); lSel++ )
				SetGridColor( spGrid, lCount + 1, lSel, 100, 100, 100 );
		}
	}
	// �ʒu����
	if( lCount > 0 )
		spGrid->AutoSize( 0, CComVariant( (LONG)m_listFieldName.size() ) );
}
void CControlDef::SetOuterRowsGrid()
{
	/// �O���b�h�擾
	IVSFlexGridPtr spGrid;
	CAxWindow wndGrid( m_hWnd );
	wndGrid.QueryControl(&spGrid);

	ATLASSERT( spGrid != NULL );

	/// �O���b�h������
	spGrid->put_Cols( m_listFieldName.size() );
	spGrid->put_Rows( 1 );
	spGrid->put_FixedCols( 1 );
	spGrid->put_FixedRows( 1 );

	LONG lCount = 0;
	
	/// ����t�B�[�`�����Z�b�g����Ă��Ȃ��Ȃ�߂�
	if( !m_pFeatureDef ) return;

	CRelationalTableDef* pRelTable = m_pFeatureDef->GetRelationalTableDef( m_strTableName );

	if( !pRelTable ) return;

	for( CRelationalTableDef::iterator i = pRelTable->begin(); i != pRelTable->end(); i++, lCount++ ) {
		CString strItem, strItemTmp;
		std::list<LONG> listChanged;
		LONG lFieldCount = 0;
		for( std::list<CString>::iterator itFieldNameList = m_listFieldName.begin(); itFieldNameList != m_listFieldName.end(); itFieldNameList++, lFieldCount++ )
		{
			std::list<CFieldDef>::iterator j;
			for( j = i->begin(); j != i->end(); j++ ) 
			{
				/// �����t�B�[���h�������̂�T��
				if( 0 == (*itFieldNameList).CompareNoCase( j->m_strFieldName ) )
					break;
			}

			/// ��΂���͂��B�Ȃ���΃t�B�[���h���̓o�^���Ԉ���Ă���
			ATLASSERT( j != i->end() );

			CString str;
			// NULL �������ꍇ�̏���			
			if( j->m_vaValue.vt == VT_NULL )
			{
				str = NULL_VALUE;
			} 
			// NULL ����Ȃ��ꍇ�̏���
			else 
			{
				DOMAINMAP* domain = j->GetDomain();
				if( ! domain ) {
					va2str( j->m_vaValue, str );
				} else {
					for( DOMAINMAP::iterator l = domain->begin(); l != domain->end(); l++ ) {
						if( l->code == j->m_vaValue ) {
							str = l->code_discription;
							break;
						}
					}
				}
			}
			strItemTmp = strItem;
			if( strItemTmp.Compare( _T("") ) == 0 ) {
				strItem.Format(_T("%d\t%s"), lCount, str );
			} else {
				strItem.Format(_T("%s\t%s"), strItemTmp, str );
			}

			/// �ҏW����Ă��邩�ǂ����`�F�b�N���āA�F��ς���
			if( j->m_bChanged ) {
				listChanged.push_back( lFieldCount );
				
			}
		}
		spGrid->AddItem( _bstr_t(strItem) );
		/// �F��ς���t�B�[���h���s��ǉ�������ŕύX
		for( std::list<LONG>::iterator n = listChanged.begin(); n != listChanged.end(); n++ )
			SetGridColorToRed( spGrid, lCount + 1, (*n) + 1 );
	}
}
void CControlDef::SetFeatureGrid( BOOL bMulti )
{
	/// �O���b�h�擾
	IVSFlexGridPtr spGrid;
	CAxWindow wndGrid( m_hWnd );
	wndGrid.QueryControl(&spGrid);
	LONG lFieldCount = 0;

	// �O���b�h�̍s�����擾
	if( m_lTableIndex < 0 && m_lRowIndex < 0 )
	{
		// �t�B�[�`���̑����̏ꍇ
		lFieldCount = m_pFeatureDef->size();
	} else {
		// �O���e�[�u���̑����̏ꍇ
		std::list<CRelationalTableDef>* pRelTableDefList = m_pFeatureDef->GetRelationalTableDefList();
		std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin();
		lFieldCount = itRelTableDefList->size();
	}

	// �X�N���[���o�[�̈ʒu��ۑ����邽��
	VARIANT_BOOL	bVisible = VARIANT_FALSE;
	BOOL	bBorderFlag = FALSE;
	LONG		lTopRow=0;
	for(LONG i=1; i<lFieldCount+1; i++)
	{
		if( !bBorderFlag )
		{
			spGrid->get_RowIsVisible(i,&bVisible);
			if( bVisible == VARIANT_FALSE)
			{
				bBorderFlag = TRUE;
				continue;
			}
			break;
		}
		else
		{
			spGrid->get_RowIsVisible(i,&bVisible);
			if( bVisible == VARIANT_TRUE)
			{
				lTopRow = i;
				break;
			}
		}
	}
	
	ATLASSERT( spGrid != NULL );

	if( bMulti ) {
		spGrid->put_Cols( 5 );
		spGrid->put_Rows( 1 );
		spGrid->put_FixedCols( 1 );
		spGrid->put_FixedRows( 1 );
	} else {
		spGrid->put_Cols( 2 );
		spGrid->put_Rows( 1 );
		spGrid->put_FixedCols( 1 );
		spGrid->put_FixedRows( 1 );
#ifndef SINDY_FOR_ENGLISH
		spGrid->put_TextMatrix( 0, 0, CComBSTR(_T("��������")) );
		spGrid->put_TextMatrix( 0, 1, CComBSTR(_T("�����l")) );
#else
		spGrid->put_TextMatrix( 0, 0, CComBSTR(_T("Field Name")) );
		spGrid->put_TextMatrix( 0, 1, CComBSTR(_T("Value")) );
#endif	// SINDY_FOR_ENGLISH
		spGrid->PutColWidth( 0, 2000 );
	}

	if( !m_pFeatureDef ) return;

	lFieldCount = 0;
	if( m_lTableIndex < 0 && m_lRowIndex < 0 )
	{
		// �t�B�[�`���̑����̏ꍇ
		for( std::list<CFieldDef>::iterator j = m_pFeatureDef->begin(); j != m_pFeatureDef->end(); j++, lFieldCount++ ) 
		{
			_SetFeatureGrid( &(*j), spGrid, lFieldCount );
		}
	} else {
		// �O���e�[�u���̑����̏ꍇ
		std::list<CRelationalTableDef>* pRelTableDefList = m_pFeatureDef->GetRelationalTableDefList();
		std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin();
		advance( itRelTableDefList, m_lTableIndex );
		std::list<CRowDef>::iterator it = itRelTableDefList->begin();
		advance( it, m_lRowIndex );
		for( std::list<CFieldDef>::iterator itFieldDef = it->begin(); itFieldDef != it->end(); itFieldDef++, lFieldCount++ )
		{
			_SetFeatureGrid( &(*itFieldDef), spGrid, lFieldCount );
		}
	}

	spGrid->put_TopRow(lTopRow);
	// �O���b�h�\���͑����ҏW�ł��Ȃ��悤�ɂ���

	CString strRegOpenKey;
	strRegOpenKey.Format( _T("%s%s%sAttributeTool"), SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );

	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	BOOL bIsGridEditMode = FALSE;
	if ( hOpenKey )
	{
		DWORD dwResult = 0;
		if( EmxGetRegDword( _T("GridEditMode"), &dwResult, hOpenKey ) )
			bIsGridEditMode = (BOOL)dwResult;

		RegCloseKey( hOpenKey );
	}

	if( !bIsGridEditMode )
		spGrid->put_Editable(flexEDNone);

}
void CControlDef::_SetFeatureGrid( CFieldDef* pFieldDef, IVSFlexGridPtr spGrid, LONG lFieldCount )
{
	CString str, strItem;

	// NULL �������ꍇ�̏���			
	if( pFieldDef->m_vaValue.vt == VT_NULL )
	{
		str = NULL_VALUE;
	} 
	// NULL ����Ȃ��ꍇ�̏���
	else 
	{
		DOMAINMAP* domain = pFieldDef->GetDomain();
		if( ! domain )
		{
			// ��ƖړI���̂݃��[���f�[�^�x�[�X����ǂݍ���
			if( 0 == pFieldDef->m_strFieldName.CompareNoCase( PURPOSE_C_FIELD ) )
			{
				CComBSTR bstrName;
				m_ipRule->GetOperatingPurposeFromID( pFieldDef->m_vaValue.lVal, &bstrName );
				str = bstrName;

				// ���Ȃ��������Ă����ƖړI0�ɂ����āA0���o�͂��邽�߂ɕK�v
				if( str.IsEmpty() )
					str.Format(_T("%d"), pFieldDef->m_vaValue.lVal );
			}
			else
				va2str( pFieldDef->m_vaValue, str );
		} 
		else {
			for( DOMAINMAP::iterator l = domain->begin(); l != domain->end(); l++ )
			{
				if( l->code == pFieldDef->m_vaValue )
				{
					str = l->code_discription;
					break;
				}
			}
		}
	}

	if( ! m_bAliasOrField )
		strItem.Format(_T("%s\t%s"), pFieldDef->m_strFieldAliasName, str );
	else
		strItem.Format(_T("%s\t%s"), pFieldDef->m_strFieldName, str );

	spGrid->AddItem( _bstr_t(strItem) );

	/// �ҏW����Ă��邩�ǂ����`�F�b�N���āA�F��ς���
	if( pFieldDef->m_bChanged ) {
		SetGridColorToRed( spGrid, lFieldCount + 1, 1 );
				
	}
	/// �ҏW�s�̃t�B�[���h�Ȃ�F������
	if( !pFieldDef->IsEditable() ) {
		SetGridColorToUnEdit( spGrid, lFieldCount + 1, 1 );
	}
}
void CControlDef::SetOtherControl()
{

}
/**
 * �f�[�^�x�[�X���� NULL OK �ȃt�B�[���h���ǂ����̃`�F�b�N
 *
 * @return Nullable �Ȃ� TRUE
 */
BOOL CControlDef::IsNullable( LPCTSTR lpcszFieldName )
{
	CFieldDef* pFieldDef = GetFieldDef( lpcszFieldName );

	if( !pFieldDef ) {
		ATLTRACE(_T("CControlDef::IsNullable(): �֘A����t�B�[���h������܂��� ID=%d\n"), m_nID );
		return FALSE;
	}

	return pFieldDef->m_bIsNullable;
}
/**
 * 
 * @return �ҏW�ł���ꍇ�� TRUE �A�ҏW�ł��Ȃ��Ȃ� FALSE
 */
BOOL CControlDef::Click( LONG l1, LONG l2, BOOL bIsGridEditMode )
{
	/// �O���b�h�擾
	IVSFlexGridPtr spGrid;
	CAxWindow wndGrid( m_hWnd );
	wndGrid.QueryControl(&spGrid);

	ATLASSERT( spGrid != NULL );

	LONG lFieldNum, lFeatNum, lCount;
	DOMAINMAP* domain;
	DOMAINMAP::iterator itDomain;
	std::list<FEATUREDEF>::iterator itFeatureDefList;
	std::list<CFieldDef>::iterator itFieldDefList;
	CString strList;
	
	switch( m_type ) {
	case TYPE_FEATUREGRID:
		lFieldNum = l1 - 1;
		// ���ݑI������Ă���t�B�[���h�܂ŃV�t�g
		ATLASSERT( m_pFeatureDef != NULL );
		// �O���e�[�u���������ꍇ
		if( m_lTableIndex < 0 && m_lRowIndex < 0 )
		{
			itFieldDefList = m_pFeatureDef->begin();
			advance( itFieldDefList, lFieldNum );
		} else {
			std::list<CRelationalTableDef>* pRelTableDefList = m_pFeatureDef->GetRelationalTableDefList();
			std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin();
			advance( itRelTableDefList, m_lTableIndex );
			std::list<CRowDef>::iterator it = itRelTableDefList->begin();
			advance( it, m_lRowIndex );
			itFieldDefList = it->begin();
			advance( itFieldDefList, lFieldNum );
		}
		
		break;
	case TYPE_FEATURESGRID:
		lFieldNum = l2 - 1;
		lFeatNum = l1 - 1;
		// ���ݑI������Ă��郌�R�[�h�܂ŃV�t�g
		itFeatureDefList = m_pFeatureDefList->begin();
		advance( itFeatureDefList, lFeatNum );
		// ���ݑI������Ă���t�B�[���h�܂ŃV�t�g
		std::list<CString>::iterator itFieldName = m_listFieldName.begin();
		advance( itFieldName, lFieldNum );
		for( itFieldDefList = itFeatureDefList->begin(); itFieldDefList != itFeatureDefList->end(); ++itFieldDefList )
		{
			if( 0 == itFieldDefList->m_strFieldName.CompareNoCase( *itFieldName ) ) 
				break;
		}
		break;
	}

	/// �ҏW�ł��Ȃ��t�B�[���h�Ȃ�A��
	if( !bIsGridEditMode )
	{
		if( !itFieldDefList->IsEditable() )
			return FALSE;
	}
	else
	{
		VARIANT_BOOL vaEditable = VARIANT_FALSE;
		itFieldDefList->m_ipField->get_Editable( &vaEditable );
		if( vaEditable == VARIANT_FALSE )
			return FALSE;
	}

	/// �h���C���擾
	domain = itFieldDefList->GetDomain();

	/// �h���C���̂Ȃ��ꍇ
	if( !domain ) 
	{
		CComBSTR bstrCel;

		USES_CONVERSION;

		spGrid->get_Text( &bstrCel );
		if( lstrcmpi( OLE2T(bstrCel), NULL_VALUE ) == 0 )
		{
			CComBSTR bstrText(_T(""));
			spGrid->put_TextMatrix( l1, l2, bstrText.Detach() );
		}

	}
	/// �h���C���̂���ꍇ
	else 
	{
		for( lCount = 0, itDomain = domain->begin(); itDomain != domain->end(); itDomain++, lCount++ ) {
			if( strList.Compare( _T("") ) != 0 ) {
				strList += _T("|");
			}
			strList += itDomain->code_discription;
		}
		/// Nullable �ȃt�B�[���h�̏ꍇ�̓��X�g�ɒǉ�
		if( itFieldDefList->m_bIsNullable ) {
			if( strList.Compare( _T("") ) != 0 ) {
				strList += _T("|");
			}
			strList += NULL_VALUE;
		}
		spGrid->put_ComboList( CComBSTR(strList) );
	}
	return TRUE;
}
BOOL CControlDef::IsClickCelEditable( LONG l1, LONG l2 )
{
	/// �O���b�h�擾
	IVSFlexGridPtr spGrid;
	CAxWindow wndGrid( m_hWnd );
	wndGrid.QueryControl(&spGrid);

	ATLASSERT( spGrid != NULL );

	LONG lFieldNum, lFeatNum;
	std::list<FEATUREDEF>::iterator itFeatureDefList;
	std::list<CFieldDef>::iterator itFieldDefList;
	CString strList;
	
	switch( m_type ) {
	case TYPE_FEATUREGRID:
		lFieldNum = l1 - 1;
		// ���ݑI������Ă���t�B�[���h�܂ŃV�t�g
		ATLASSERT( m_pFeatureDef != NULL );
		// �O���e�[�u���������ꍇ
		if( m_lTableIndex < 0 && m_lRowIndex < 0 )
		{
			itFieldDefList = m_pFeatureDef->begin();
			advance( itFieldDefList, lFieldNum );
		} else {
			std::list<CRelationalTableDef>* pRelTableDefList = m_pFeatureDef->GetRelationalTableDefList();
			std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin();
			advance( itRelTableDefList, m_lTableIndex );
			std::list<CRowDef>::iterator it = itRelTableDefList->begin();
			advance( it, m_lRowIndex );
			itFieldDefList = it->begin();
			advance( itFieldDefList, lFieldNum );
		}
		
		break;
	case TYPE_FEATURESGRID:
		lFieldNum = l2 - 1;
		lFeatNum = l1 - 1;
		// ���ݑI������Ă��郌�R�[�h�܂ŃV�t�g
		itFeatureDefList = m_pFeatureDefList->begin();
		advance( itFeatureDefList, lFeatNum );
		// ���ݑI������Ă���t�B�[���h�܂ŃV�t�g
		std::list<CString>::iterator itFieldName = m_listFieldName.begin();
		advance( itFieldName, lFieldNum );
		for( itFieldDefList = itFeatureDefList->begin(); itFieldDefList != itFeatureDefList->end(); ++itFieldDefList )
		{
			if( 0 == itFieldDefList->m_strFieldName.CompareNoCase( *itFieldName ) ) 
				break;
		}
		break;
	}

	VARIANT_BOOL vaEditable = VARIANT_FALSE;
	itFieldDefList->m_ipField->get_Editable( &vaEditable );

	CComBSTR bstrFieldName;
	itFieldDefList->m_ipField->get_Name( &bstrFieldName );
	CString strFieldName( bstrFieldName );
	INT iPos = strFieldName.ReverseFind('.');
	if( iPos > 0 ) strFieldName = strFieldName.Right( strFieldName.GetLength() - iPos - 1 );
	strFieldName.MakeUpper();	// �S�đ啶���ɒu��

	// �`��͕ύX�\�ƂȂ��Ă���̂ŁAget_Editable�ł͔��f�ł��Ȃ�����
	if(      lstrcmp( strFieldName, _T("FID")            ) == 0 ) vaEditable = VARIANT_FALSE; // FID
	else if( lstrcmp( strFieldName, _T("SHAPE")          ) == 0 ) vaEditable = VARIANT_FALSE; // Shape
	else if( lstrcmp( strFieldName, _T("AREA")           ) == 0 ) vaEditable = VARIANT_FALSE; // AREA
	else if( lstrcmp( strFieldName, _T("SHAPE_LENGTH")   ) == 0 ) vaEditable = VARIANT_FALSE; // Shape_length

	return ( vaEditable ) ? TRUE : FALSE;
}

void CControlDef::Edited( LONG l1, LONG l2 )
{
	/// �O���b�h�擾
	IVSFlexGridPtr spGrid;
	CAxWindow wndGrid( m_hWnd );
	wndGrid.QueryControl(&spGrid);

	ATLASSERT( spGrid != NULL );

	LONG lFieldNum, lFeatNum;
	DOMAINMAP* domain;
	DOMAINMAP::iterator itDomain;
	std::list<FEATUREDEF>::iterator itFeatureDefList;
	std::list<CFieldDef>::iterator itFieldDefList;
	CString strList;
	LPCTSTR lpszFieldName = NULL;
	CRelationalTableDef::iterator itRelTable;

	switch( m_type ) {
	case TYPE_FEATUREGRID:
		lFieldNum = l1 - 1;
		// ���ݑI������Ă���t�B�[���h�܂ŃV�t�g
		ATLASSERT( m_pFeatureDef != NULL );
		// �O���e�[�u���������ꍇ
		if( m_lTableIndex < 0 && m_lRowIndex < 0 )
		{
			itFieldDefList = m_pFeatureDef->begin();
			advance( itFieldDefList, lFieldNum );
		} else {
			std::list<CRelationalTableDef>* pRelTableDefList = m_pFeatureDef->GetRelationalTableDefList();
			std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin();
			advance( itRelTableDefList, m_lTableIndex );
			std::list<CRowDef>::iterator it = itRelTableDefList->begin();
			advance( it, m_lRowIndex );
			itFieldDefList = it->begin();
			advance( itFieldDefList, lFieldNum );
		}
		break;
	case TYPE_FEATURESGRID:
		lFieldNum = l2 - 1;
		lFeatNum = l1 - 1;
		// ���ݑI������Ă��郌�R�[�h�܂ŃV�t�g
		itFeatureDefList = m_pFeatureDefList->begin();
		advance( itFeatureDefList, lFeatNum );
		// ���ݑI������Ă���t�B�[���h�܂ŃV�t�g
		std::list<CString>::iterator itFieldName = m_listFieldName.begin();
		advance( itFieldName, lFieldNum );
		for( itFieldDefList = itFeatureDefList->begin(); itFieldDefList != itFeatureDefList->end(); ++itFieldDefList )
		{
			if( 0 == itFieldDefList->m_strFieldName.CompareNoCase( *itFieldName ) )
				break;
		}
		break;
	}


	// �h���C���擾
	domain = itFieldDefList->GetDomain();

	USES_CONVERSION;

	// �I�����ꂽ�e�L�X�g����l���擾
	CComBSTR bstrText;
	spGrid->get_TextMatrix( l1, l2, &bstrText );
	CComVariant vaValue;
	LONG lVarType = -1;
	itFieldDefList->m_ipField->get_VarType( &lVarType );

	vaValue.vt = VT_EMPTY;	// ������
	ATLASSERT( itFieldDefList->m_vaValue.vt != VT_EMPTY );	// �ق�Ƃ͂����Ƃ��邯��

	// �l�ϊ�
	if( domain )
	{
		CString strText( bstrText );
		if( 0 == strText.CompareNoCase( NULL_VALUE ) )
			vaValue.vt = VT_NULL;
		else {
			for( itDomain = domain->begin(); itDomain != domain->end(); itDomain++ )
			{
				if( 0 == itDomain->code_discription.Compare( strText ) )
				{
					vaValue = itDomain->code;
					break;
				}
			}
			ATLASSERT( vaValue.vt != VT_EMPTY );
		}
	} else {
		CString strText( bstrText );
		if( strText.IsEmpty() )
			vaValue.vt = VT_NULL;
		else {
			vaValue = bstrText;
			vaValue.ChangeType( (USHORT)lVarType );
		}

		if( !itFieldDefList->CheckValue( vaValue, GetParent( m_hWnd ) ) )
		{
			// �\�������ɖ߂�
			CComVariant vaCelText;
			if( itFieldDefList->m_vaValue.vt == VT_NULL )
				spGrid->put_Text( CComBSTR(NULL_VALUE) );
			else
			{
				vaCelText.ChangeType( VT_BSTR, &(itFieldDefList->m_vaValue) );
				ATLASSERT( vaCelText.vt == VT_BSTR );
				spGrid->put_Text( vaCelText.bstrVal );
			}

			return;
		}

		USES_CONVERSION;

		CComVariant vaCelText;
		switch( lVarType )
		{
			case VT_BSTR:
				if( vaValue.vt == VT_NULL || lstrcmpi( OLE2T(vaValue.bstrVal), _T("") ) == 0 || lstrcmpi( OLE2T(vaValue.bstrVal), NULL_VALUE ) == 0 ) {
					spGrid->put_Text( _bstr_t(NULL_VALUE) );
					vaValue.ChangeType( VT_NULL );
				}
				break;
			case VT_I4:
			case VT_I2:
			case VT_R4:
			case VT_R8:
			{
				CString strText = OLE2T(bstrText);
				if( strText.CompareNoCase( NULL_VALUE ) == 0 ||	strText.CompareNoCase( _T("") ) == 0 ) {
					spGrid->put_Text( _bstr_t(NULL_VALUE) );
					vaValue.vt = VT_NULL;
				} else {
					vaCelText.ChangeType( VT_BSTR, &vaValue );
					spGrid->put_Text( vaCelText.bstrVal );
				}
				break;
			}
			default:
				break;
		}
	}

	/// m_vaValue �ɃZ�b�g
	itFieldDefList->m_vaValue = vaValue;
	itFieldDefList->m_bIsUnset = FALSE;		//!< �Z�b�g�t���O�i���ꂪ�Ȃ���Store�ł��Ȃ��j
	/// m_vaInitValue �Ɣ�ׂĕύX����Ă�����F��ύX
	if( itFieldDefList->m_vaValue != itFieldDefList->m_vaInitValue ) {
		SetGridColorToRed( spGrid, l1, l2 );
		/// m_bChanged �t���O�́H
		itFieldDefList->m_bChanged = TRUE;
	}
	/// ���ɖ߂����ꍇ���l������
	else {
		SetGridColorToDefault( spGrid, l1, l2 );
		itFieldDefList->m_bChanged = FALSE;
	}
}
/**
 * LinkTimeReg �̓��e���O���b�h�ɔ��f������
 *
 * @param nID				[in]	�I������Ă��郊���N�� ObjectID
 */
void CControlDef::SetLinkTimeRegGrid()
{
	LONG lArrayCount = 0;
	IVSFlexGridPtr			spGrid = NULL;

	// �O���b�h�R���g���[���擾
	CAxWindow wndGrid( m_hWnd );
	wndGrid.QueryControl(&spGrid);

	bool bIsWalkLink = false;
	CString strTblName = m_pFeatureDef->GetTableName();
	if( strTblName.CompareNoCase( _T("WALK_LINK") ) == 0 )
		bIsWalkLink = true;

	// �P�s�ڂ̓��\�[�X���Ńv���p�e�B�ݒ�i | (Index) | �K������ | �j�� | �j
	long lCols = 3;
	if( m_type == TYPE_OWTIMEREG )	// ��ʃO���b�h�������琳�t�J�������ǉ�
		lCols++;
	else if( bIsWalkLink )			// ���s�ҒʋփO���b�h��������K���ΏۃJ�������ǉ�
		lCols++;

	spGrid->put_Cols( lCols );

	// ��ʃO���b�h�������琳�t�̏ꍇ�ɕ����J������\��
	if( m_type == TYPE_OWTIMEREG )
	{
		CFieldDef* pFieldDef = m_pFeatureDef->GetFieldDef( m_pFeatureDef->GetTableName(), _T("ONEWAY_C") );
		if( pFieldDef )
		{
			LONG lOneway_c = pFieldDef->m_vaValue.lVal;
			if (lOneway_c == 5){		
				spGrid->PutColWidth(1, 900);
			}else{
				spGrid->PutColWidth(1,   0);
			}
		}
	}

	spGrid->put_Rows( 1 );

	if( !m_pFeatureDef )
	{
		ATLASSERT( m_pFeatureDef != NULL );
		return ;
	}

	CRelationalTableDef* pRelTableDef = m_pFeatureDef->GetRelationalTableDef( m_strTableName );
	
	/// LINK_TIME_REG �̃e�[�u�����o�^����Ă��Ȃ���Ζ߂�
	if( !pRelTableDef )
		return ;

	/// �������R�[�h��\������΂�����
	LONG lRowCount = 0;
	lRowCount = pRelTableDef->size();

	for( LONG i = 0; i < lRowCount; i++ ) {

		CString strDomainName1, strDomainName2, strNopassTarget;
		LONG lSMonth, lSDay, lEMonth, lEDay, lSHour, lSMin, lEHour, lEMin;
		BOOL bChanged1 = FALSE, bChanged2 = FALSE;

		CRelationalTableDef::iterator itRelTable = pRelTableDef->begin();

		advance( itRelTable, i );	//!< �\�����邽�߂̃��R�[�h�܂ŃV�t�g

		for( std::list<CFieldDef>::iterator itListFieldDef = itRelTable->begin(); itListFieldDef != itRelTable->end(); itListFieldDef++ )
		{
			// �e�t�B�[���h���擾���A���ԋK�����擾
			if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("START_MONTH") ) )
				lSMonth = itListFieldDef->m_vaValue.lVal;
			else if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("START_DAY") ) )
				lSDay   = itListFieldDef->m_vaValue.lVal;
			else if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("END_MONTH") ) )
				lEMonth   = itListFieldDef->m_vaValue.lVal;
			else if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("END_DAY") ) )
				lEDay   = itListFieldDef->m_vaValue.lVal;
			else if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("START_HOUR") ) )
				lSHour   = itListFieldDef->m_vaValue.lVal;
			else if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("START_MIN") ) )
				lSMin   = itListFieldDef->m_vaValue.lVal;
			else if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("END_HOUR") ) )
				lEHour   = itListFieldDef->m_vaValue.lVal;
			else if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("END_MIN") ) )
				lEMin   = itListFieldDef->m_vaValue.lVal;

			/// LINKDIR_C
			if ( (m_type == TYPE_OWTIMEREG) )
			{
				if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("LINKDIR_C") ) )
				{
					DOMAINMAP* domain = itListFieldDef->GetDomain();
					for( DOMAINMAP::iterator itDomain = domain->begin(); itDomain != domain->end(); ++itDomain )
					{
						if( itListFieldDef->m_vaValue.vt == VT_NULL )
						{
							strDomainName1 = NULL_VALUE;	// XXXXX TBD �b��I��null
							if( itListFieldDef->m_vaValue != itListFieldDef->m_vaInitValue ) 
								bChanged1 = TRUE;	//!< �ύX�t���O�Z�b�g
							break;
						} 
						else if( itDomain->code == itListFieldDef->m_vaValue )
						{
							strDomainName1 = itDomain->code_discription;
							if( itListFieldDef->m_vaValue != itListFieldDef->m_vaInitValue ) 
								bChanged1 = TRUE;	//!< �ύX�t���O�Z�b�g
							break;
						}				
					}
				}
			}

			/// DAYOFWEEK_CODE
			if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("DAYOFWEEK_C") ) )
			{
				DOMAINMAP* domain = itListFieldDef->GetDomain();
				for( DOMAINMAP::iterator itDomain = domain->begin(); itDomain != domain->end(); ++itDomain )
				{
					if( itListFieldDef->m_vaValue.vt == VT_NULL )
					{
						strDomainName2 = NULL_VALUE;
						if( itListFieldDef->m_vaValue != itListFieldDef->m_vaInitValue ) 
							bChanged2 = TRUE;	//!< �ύX�t���O�Z�b�g
						break;
					} 
					else if( itDomain->code == itListFieldDef->m_vaValue )
					{
						strDomainName2 = itDomain->code_discription;
						if( itListFieldDef->m_vaValue != itListFieldDef->m_vaInitValue )
							bChanged2 = TRUE;	//!< �ύX�t���O�Z�b�g
						break;
					}
				}
			}

			// NOPASS_TARGET�i���s�җp�̋K���Ώۂ𔻕ʂ��邽�߂̃h���C�� [bug 5472]�j
			if( 0 == itListFieldDef->m_strFieldName.CompareNoCase( _T("NOPASS_TARGET") ) )
			{
				itListFieldDef->dump();
				DOMAINMAP* domain = itListFieldDef->GetDomain();
				for( DOMAINMAP::iterator itDomain = domain->begin(); itDomain != domain->end(); ++itDomain )
				{
					if( itListFieldDef->m_vaValue.vt == VT_NULL )
					{
						strNopassTarget = NULL_VALUE;
						break;
					} 
					else if( itDomain->code == itListFieldDef->m_vaValue )
					{
						strNopassTarget = itDomain->code_discription;
						break;
					}
				}
			}
		}
		CString strPeriod = GetPeriodString(lSMonth, lSDay, lEMonth, lEDay, lSHour, lSMin, lEHour, lEMin);


		CString strGrid;
		// ��ʁi���H�E���s�ҁj�\���p���R�[�h
		if( m_type == TYPE_OWTIMEREG ){
			strGrid.Format(_T("%d\t%s\t%s\t%s"), i, strDomainName1, strPeriod, strDomainName2 );
		// ���s�Ғʋ֕\���p
		}else if( bIsWalkLink ){
			strGrid.Format(_T("%d\t%s\t%s\t%s"), i, strPeriod, strDomainName2, strNopassTarget );
		// ���̑��i���H�ʋցj
		}else{
			strGrid.Format(_T("%d\t%s\t%s"), i, strPeriod, strDomainName2 );
		}

		spGrid->AddItem( _bstr_t(strGrid) );
	}

	// ���` �� �ҏW�s��
	spGrid->AutoSize( 0, spGrid->Cols-1 );
	spGrid->put_Editable( flexEDNone );
}

/**
 * ���ɂ��̕\���p�������Ԃ�
 *
 * ���݂̃����o�ϐ�������ɂ��\���p�̕�������쐬���A�Ԃ��܂�
 *
 * @return ���ɂ��̕\���p������
 */
CString CControlDef::GetDateString(LONG lSMonth, LONG lSDay, LONG lEMonth, LONG lEDay)
{
	CString strDate;

	if( lSMonth == 1 && lSDay == 1 && lEMonth == 12 && lEDay == 31 ) {
#ifndef SINDY_FOR_ENGLISH
		strDate = _T("�ʔN");	//!< �ʔN�̏ꍇ
#else
		strDate = _T("All year");	//!< �ʔN�̏ꍇ
#endif	// SINDY_FOR_ENGLISH
	} else if( lSMonth ==0 && lEMonth == 0 ){
#ifndef SINDY_FOR_ENGLISH
		strDate.Format( _T("����%s-%s"), l2str(lSDay), l2str(lEDay) ); // 0�� �� �����\��( bug[6354] )
#else
		strDate.Format( _T("Every month%s-%s"), l2str(lSDay), l2str(lEDay) ); // 0�� �� �����\��( bug[6354] )
#endif	// SINDY_FOR_ENGLISH
	}else{
		strDate.Format(_T("%s/%s-%s/%s"), l2str(lSMonth), l2str(lSDay), l2str(lEMonth), l2str(lEDay) );
	}

	return strDate;
}
/**
 * ���Ԃ̕\���p�������Ԃ�
 *
 * ���݂̃����o�ϐ����玞�ԕ\���p�̕�������쐬���A�Ԃ��܂�
 *
 * @return ���Ԃ̕\���p������
 */
CString CControlDef::GetTimeString(LONG lSHour, LONG lSMin, LONG lEHour, LONG lEMin)
{
	CString strTime;

	strTime.Format(_T("%s:%s-%s:%s"), l2str(lSHour), l2str(lSMin), l2str(lEHour), l2str(lEMin) );

	return strTime;
}

/**
 * ���ԋK����������쐬���A�Ԃ�
 *
 * �O���b�h�œ�����\������Z���͈�Ȃ̂ŁA���̊֐��ł܂Ƃ߂ĕ�������쐬���܂�
 *
 * @return �\���p����������
 */
CString CControlDef::GetPeriodString(LONG lSMonth, LONG lSDay, LONG lEMonth, LONG lEDay, LONG lSHour, LONG lSMin, LONG lEHour, LONG lEMin)
{
	CString strPeriod;

	strPeriod.Format(_T("%s  %s"), GetDateString(lSMonth, lSDay, lEMonth, lEDay), GetTimeString(lSHour, lSMin, lEHour, lEMin) );
	return strPeriod;
}
/**
 * �^����ꂽ���l�𕶎���ɕϊ����ĕԂ�
 *
 * �ϊ�����ہA�ꌅ�̐��l�Ɋւ��Ă͓񌅂ɂ��ĕԂ��܂�
 *
 * @return �ϊ���̕�����
 */
CString CControlDef::l2str( LONG l )
{
	CString str;

	if( l == 0 ) {
		str.Format(_T("00"));
	} else {
		str.Format(_T("%d"), l );
	}

	return str;
}

void CControlDef::SetGridColor( IVSFlexGridPtr spGrid, LONG lRow, LONG lCol, LONG r, LONG g, LONG b )
{ 
	spGrid->put_Cell( flexcpBackColor, _variant_t(lRow), _variant_t(lCol), _variant_t(lRow), _variant_t(lCol), _variant_t((long)RGB(r, g, b)) ); 
}
void CControlDef::SetGridColorToRed( IVSFlexGridPtr spGrid, LONG lRow, LONG lCol )
{ 
	SetGridColor( spGrid, lRow, lCol, 255, 168, 126 ); 
}
void CControlDef::SetGridColorToDefault( IVSFlexGridPtr spGrid, LONG lRow, LONG lCol )
{
	SetGridColor( spGrid, lRow, lCol, 0, 0, 0 );
}
void CControlDef::SetGridColorToUnEdit( IVSFlexGridPtr spGrid, LONG lRow, LONG lCol )
{
	SetGridColor( spGrid, lRow, lCol, 200, 250, 200 );
}

void CControlDef::va2str( VARIANT &va, CString& str )
{
	USES_CONVERSION;

	CComVariant vaStr(_T(""));

	switch ( va.vt ) {
		case VT_EMPTY:
			vaStr = EMPTY_VALUE;
			break;
		case VT_NULL:
			vaStr = NULL_VALUE;
			break;
		case VT_BOOL:
			if( va.boolVal == VARIANT_TRUE )
				vaStr = TRUE_VALUE;
			else if( va.boolVal == VARIANT_FALSE )
				vaStr = FALSE_VALUE;
			break;
			case VT_UNKNOWN:
				vaStr = GEOMETRY_VALUE;
			break;
		default:
			vaStr.ChangeType( VT_BSTR, &va );
			break;
	}
	
	str = OLE2T( (BSTR)vaStr.bstrVal );

}
