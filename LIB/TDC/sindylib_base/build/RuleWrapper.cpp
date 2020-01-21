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
#include "RuleWrapper.h"
#include "type_convert.h"
#include "RowBase.h"
#include "GeometryBase.h"
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

using namespace sindy;

extern "C" const GUID __declspec(selectany) CLSID_SiNDYRuleObj =             {0x642db0f6,0x97cb,0x488d,{0xad,0x99,0xbb,0xd9,0xab,0xda,0xc2,0x47}};
extern "C" const GUID __declspec(selectany) CLSID_SiNDYModifyCheckObj =      {0x67dc3e12,0x061b,0x4ca8,{0xa8,0x29,0x40,0xc6,0x8d,0x7d,0x17,0x7a}};

#ifndef SINDY_FOR_ENGLISH
const static _TCHAR CANNOT_CREATE_LAYER[] = _T("�쐬�s�\�ȃ��C���ł��F%s");
const static _TCHAR THIS_IS_LOCKMESH[]    = _T("���b�V�����b�N�G���A�ł��F%s");
const static _TCHAR NOT_EDITABLE_AREA[]   = _T("�ҏW�\�G���A�ł͂���܂���F%s");
#else
const static _TCHAR CANNOT_CREATE_LAYER[] = _T("Layer that cannot be created: %s");
const static _TCHAR THIS_IS_LOCKMESH[]    = _T("This area is locked.�F%s");
const static _TCHAR NOT_EDITABLE_AREA[]   = _T("This is not an area that can be edited.�F%s");
#endif	// SINDY_FOR_ENGLISH


// ���[���I�u�W�F�N�g�Ƀ��O�C����񂪃Z�b�g����Ă��邩�ǂ���
bool CRuleWrapper::IsLogined() const
{
	VARIANT_BOOL is_logined = VARIANT_FALSE;
	ISiNDYRule4Ptr ipRule( GetRule() );
	if( ipRule )
		ipRule->IsLogined( &is_logined );

	return VB2bool(is_logined);
}

// ���ɕҏW���Ă��郆�[�U�����Ȃ����m�F����
bool CRuleWrapper::IsModified( const CRowBase& cRow, CString& strMsg ) const
{
	ISiNDYModifyCheckPtr ipMod( GetModify() );
	if( ! ipMod )
		return false;

	VARIANT_BOOL vbIsModified = VARIANT_FALSE;
	CComBSTR bstrModMsg;
	ipMod->IsModified( (_IRow*)cRow, &vbIsModified, &bstrModMsg );
	if( vbIsModified )
	{
		strMsg = bstrModMsg;
		return true;
	}
	return false;
}

// �ҏW�\���ǂ������m�F����
sindyeEditableType CRuleWrapper::IsEditable( const CRowBase& cRow, sindyeEditType emEditType, CString& strMsg, bool check_modify/* = true*/, bool err2warn_modify/* = false*/  ) const
{
	sindyeEditableType emRetType = sindyeEditableNoDefine;
	CComBSTR bstrMsg;

	// ���f�B�t�@�C�`�F�b�N
	if( check_modify )
	{
		if( IsModified( cRow, strMsg ) )
			return err2warn_modify ? sindyeEditableWarnning : sindyeEditableNoEditable;
	}

	// �ҏW���[���m�F
	if( sindyeEditableNoDefine == emRetType )
	{
		ISiNDYRule4Ptr ipRule( GetRule() );
		if( ipRule && IsLogined() )
		{
			IGeometryPtr ipGeom( cRow.GetShapeCopy() );
			// �V�K���̂̏ꍇ�́A�o���Ń`�F�b�N����ibug 9745�j
			if( cRow.IsCreated() )
			{
				emRetType = sindyeEditableEditable;

				// �쐬�\���ǂ���
				VARIANT_BOOL vbIsCreatable = VARIANT_FALSE;
				ipRule->IsCreatableLayer( (CComBSTR)cRow.GetTableName(), &vbIsCreatable );
				if( ! vbIsCreatable )
				{
					emRetType = sindyeEditableNoEditable;
					strMsg.Format( CANNOT_CREATE_LAYER, cRow.GetTableName() );
				}

				// ���b�N�G���A���ǂ���
				if( sindyeEditableEditable == emRetType && IsLockArea( cRow.GetOwnerName(), cRow.GetTableName(), ipGeom ) )
				{
					emRetType = sindyeEditableNoEditable;
					strMsg.Format( THIS_IS_LOCKMESH, cRow.GetTableName() );
				}

				// �ҏW�G���A���ǂ���
				if( sindyeEditableEditable == emRetType && !IsEditableArea( cRow.GetTableName(), ipGeom ) )
				{
					emRetType = sindyeEditableNoEditable;
					strMsg.Format( NOT_EDITABLE_AREA, cRow.GetTableName() );
				}
			}
			else
			{
				// CRowBase::GetShape()��m_pGeom��NULL�������ꍇ��(IGeometry*)�ɃL���X�g�ł��Ȃ��̂�GetShapeCopy()���g���ibug 9750�j
				ipRule->IsEditableFeature( (_IRow*)cRow, ipGeom, emEditType, &emRetType, &bstrMsg );
				strMsg = bstrMsg;
			}
		}
	}

	return emRetType;
}

// ���b�N�G���A���ǂ����m�F����
bool CRuleWrapper::IsLockArea( const CString& strOwner, const CString& strTable, IGeometryPtr& ipGeom ) const
{
	ISiNDYRule4Ptr ipRule( GetRule() );
	if( ! ipRule )
		return false;

	VARIANT_BOOL vbIsLockArea = VARIANT_FALSE;
	ipRule->IsLockArea( CComBSTR(strOwner), CComBSTR(strTable), ipGeom, &vbIsLockArea );

	return VB2bool(vbIsLockArea);
}

// �ҏW�\�G���A���ǂ����m�F����
bool CRuleWrapper::IsEditableArea( const CString& strTable, IGeometryPtr& ipGeom ) const
{
	ISiNDYRule4Ptr ipRule( GetRule() );
	if( ! ipRule )
		return true;

	VARIANT_BOOL vbIsEditArea = VARIANT_TRUE;
	ipRule->IsEditArea( CComBSTR(strTable), ipGeom, &vbIsEditArea );

	return VB2bool(vbIsEditArea);
}

// ��ƖړIID���擾����
long CRuleWrapper::GetWorkPurposeID() const
{
	long ret = -1; // �߂�l
	CComBSTR purpose;
	ISiNDYRule4Ptr ipRule( GetRule() );
	if( ipRule && IsLogined() )
		ipRule->GetOperatingPurpose( &ret, &purpose );

	return ret;
}

// ���O�C�������擾����
CString CRuleWrapper::GetOperatorName() const
{
	CComBSTR bstrName;
	ISiNDYRule4Ptr ipRule( GetRule() );
	if( ipRule && IsLogined() )
		ipRule->get_OperatorName( &bstrName );

	return CString(bstrName);
}

// �V�K�쐬���̎����t�^�������擾����
const CAutoValue& CRuleWrapper::GetCreatedAutoValue( const CString& strTable ) const
{
	// �L���b�V��������Ȃ�L���b�V����T��
	if( ! m_mapAutoValue.empty() )
		return m_mapAutoValue[strTable];

	// �L���b�V�����Ȃ��Ȃ�A�L���b�V�����쐬
	static CAutoValue dummy;
	ISiNDYRule4Ptr ipRule( GetRule() );
	if( ! ( ipRule && IsLogined() ) )
		return dummy; // ���O�C������܂ł́A�R���e�i�͋�ɂ��Ă���

	m_mapAutoValue[_T("dummy")] = CAutoValue(); // ��d�������h�~

	IEnumAutoValueLayerPtr ipEnum;
	ipRule->get_EnumAutoValueLayer( &ipEnum );
	if( ! ipEnum )
		return dummy;

	IEnumAutoValuePtr ipAutoValues;
	ipEnum->Reset();
	while( S_OK == ipEnum->Next( &ipAutoValues ) && ipAutoValues )
	{
		CComBSTR bstrName;
		ipAutoValues->get_Name( &bstrName );
		CAutoValue& listAutoValue = m_mapAutoValue[CString(bstrName)];
		ipAutoValues->Reset();
		BSTR bstrCol = NULL, bstrVal = NULL;
		while( S_OK == ipAutoValues->Next( &bstrCol, &bstrVal ) )
		{
			listAutoValue.push_back( std::make_pair(CString(bstrCol), CString(bstrVal)) );
			::SysFreeString(bstrCol); // CComBSTR�̎g���񂵂̓��[�N�̂��ƂȂ̂ŁA��BSTR���g�p����
			::SysFreeString(bstrVal);
		}
	}
	return m_mapAutoValue[strTable];
}

// ���������t�^�̒l���擾����ibug 9759�j
const CAutoValue& CRuleWrapper::GetAutoValue() const
{
	// �L���b�V��������Ȃ�L���b�V����T��
	if( ! m_cAutoValue.empty() )
		return m_cAutoValue;

	// �L���b�V�����Ȃ��Ȃ�A�L���b�V�����쐬
	static CAutoValue dummy;
	ISiNDYRule4Ptr ipRule( GetRule() );
	if( ! ( ipRule && IsLogined() ) )
		return dummy; // ���O�C������܂ł́A�R���e�i�͋�ɂ��Ă���

	m_cAutoValue.push_back( std::make_pair( _T(""), _T("") ) ); // ��d�������h�~

	// ���������t�^���擾
	CComBSTR bstrAutoAttrGiving;
	ipRule->GetAutoAttrGiving( &bstrAutoAttrGiving );

	// tstring�ɋ󕶎���˂����ނƗ�����ibug 9755�j
	if( !bstrAutoAttrGiving )
		return m_cAutoValue;

	// field_name=value|�c �t�H�[�}�b�g
	uh::tstring av( bstrAutoAttrGiving );
	std::list<uh::tstring> key_vals;
	get_separated_values( av, uh::tstring(_T("|")), key_vals );
	BOOST_FOREACH( const uh::tstring& key_val, key_vals )
	{
		std::list<uh::tstring> vals;
		get_separated_values( key_val, uh::tstring(_T("=")), vals );
		if( 2 != vals.size() )
			continue;

		std::list<uh::tstring>::const_iterator it1 = vals.begin();
		std::list<uh::tstring>::const_iterator it2 = vals.begin(); ++it2;
		m_cAutoValue.push_back( std::make_pair( it1->c_str(), it2->c_str() ) );
	}

	// �l������΃_�~�[�������Ă���
	if( 1 < m_cAutoValue.size() )
		m_cAutoValue.pop_front();

	return m_cAutoValue;
}

// ���[���I�u�W�F�N�g���擾����
ISiNDYRule4Ptr CRuleWrapper::GetRule() const
{
	if( ! m_ipRule )
		m_ipRule.CreateInstance( CLSID_SiNDYRuleObj ); // �V���O���g���Ȃ̂ŁA�����ō���č��x���Ȃ�

	return m_ipRule;
}

// ���f�B�t�@�C�`�F�b�N�I�u�W�F�N�g���擾����
ISiNDYModifyCheckPtr CRuleWrapper::GetModify() const
{
	if( ! m_ipModify )
		m_ipModify.CreateInstance( CLSID_SiNDYModifyCheckObj ); // �V���O���g���Ȃ̂ŁA�����ō���č��x���Ȃ�

	return m_ipModify;
}
