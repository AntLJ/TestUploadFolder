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

/*! \file FieldDef.h
	\brief CFieldDef �N���X�̃C���^�[�t�F�[�X
	\author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
	\date 2002-03-05
*/

#if !defined(AFX_FIELDDEF_H__6B343E74_CE5C_4B23_B417_E5BC6DBA1D53__INCLUDED_)
#define AFX_FIELDDEF_H__6B343E74_CE5C_4B23_B417_E5BC6DBA1D53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CArcHelper;


struct CODEVALUE_DOMAIN {
public:
	long idx;					//!< �R�[�h�l���R���{�{�b�N�X�֊i�[�����Ƃ��̃C���f�b�N�X
	CComVariant code;			//!< �R�[�h�l
	CString code_discription;	//!< �R�[�h�l�̐���(�R���{�{�b�N�X�ւ̕W���p)

	CODEVALUE_DOMAIN() : idx(-1) {}
	void setIdx(long index) { idx = index; }

	bool operator<(const CODEVALUE_DOMAIN& domain) const {
		return idx < domain.idx;
	}
	bool operator==(const CODEVALUE_DOMAIN& domain) const {
		return idx == domain.idx;
	}
};

namespace virtual_field{
	extern const _TCHAR kShapeZ[];
}

/**
 * \brief  �Œ�l�h���C�����
 */
typedef std::set<CODEVALUE_DOMAIN> DOMAINMAP;

/**
 * \brief �����W�h���C���i�[�p�\����
 */
struct RANGEDOMAIN {
	DOUBLE dMax;		//!< �����W�h���C���̍ő�l
	DOUBLE dMin;		//!< �����W�h���C���̍ŏ��l
	BOOL   bExist;		//!< ���ݔ���t���O
	BOOL   bInited;		//!< ����������t���O
};

/*! \class CFieldDef
 	\brief IField �̃��b�p�[�N���X

	���̃N���X�� IField �ƈ�Έ�̊֌W�ɂȂ�܂��B
	�������A�g�p���₷���悤�ɂ������̃����o�ϐ���ǉ����Ă���܂��B
	�܂��A�Ⴆ�ΌŒ�l�h���C���Ȃǃf�[�^�x�[�X�ɕ��ׂ�������Ǝv������̂ɂ��Ă͈�x
	�ǂݍ��񂾂�j�������܂ŃN���X���ŏ���ێ����܂��B

	\b Example:

	CFieldDef fieldDef;

	CFieldDef�I�u�W�F�N�g�𐶐����܂��B
	
	fieldDef.SetFieldDef( NODE_TABLE_NAME, _T("KANJI_NAME") );	
	
	�e�[�u�����ƃt�B�[���h�������ꂼ��m_lpszTableName��m_lpszFieldName�ɐݒ肳��܂��B

	fieldDef.SetRowBuffer( 3, ipField, ipRowBuffer );		
	
	�t�B�[���h�̃C���f�b�N�X�ԍ��AIField�C���^�[�t�F�[�X�I�u�W�F�N�g�AIFeature�i����IRow�j�C���^�[�t�F�[�X�I�u�W�F�N�g��n���܂��B
	\note 3�Ԗڂ̈�����IRowBuffer�C���^�[�t�F�[�X�I�u�W�F�N�g�ɂȂ��Ă��܂����A
	\note IFeature��IRow��IRowBuffer�C���^�[�t�F�[�X�������Ă���̂ł��̂܂ܓn���Ă����v�ł��B
*/
class CFieldDef
{
public:
	CFieldDef();
	virtual ~CFieldDef();

public:
	void SetArcHelper( IApplication* ipApp ){m_cArcHelper.Init( (IApplicationPtr)ipApp );};
	void SetFieldDef( LPCTSTR lpszTableName, LPCTSTR lpszFieldName );
	CFieldDef* GetFieldDef( LPCTSTR lpszTableName, LPCTSTR lpszFieldName );
	void SetRowBuffer( LONG lFieldIndex, IFieldPtr ipField, IRowBufferPtr ipRowBuffer );
	BOOL Store();
	void Reset();
	void Clear();
	DOMAINMAP* GetDomain();					//!< �Œ�l�h���C����Ԃ�
	RANGEDOMAIN* GetRangeDomain();			//!< �����W�h���C����Ԃ�
	CComVariant GetCodeDomainValue(long idx);	//!< �R���{�{�b�N�X�̒l��Ԃ�
	VARTYPE GetCodedDomainType();			//!< ������^�̃R�[�h�l�h���C���̌^��Ԃ�
	BOOL IsEditable();						//!< ���̃t�B�[���h���ҏW�\���ǂ���
	BOOL CheckValue( const CComVariant& vaValue, HWND hWnd );
	BOOL SetNewValue( CComVariant vaNewValue, HWND hWnd );
	void dump();
	CString value2string( BOOL bIsInitValue = FALSE );
public:
	CArcHelper		m_cArcHelper;			//!< CArcHelper �N���X�̃|�C���^
	CString			m_strFieldName;		//!< �Ή�����t�B�[���h��
	CString			m_strFieldAliasName;	//!< �Ή�����t�B�[���h�̃G�C���A�X��
	CString			m_strTableName;		//!< �Ή�����e�[�u�����i�֘A�e�[�u���̏ꍇ�̂݁j
	BOOL			m_bIsUnset;				//!< �l���Z�b�g���ꂽ���ǂ����̃t���O�i�t�B�[���h�����݂��邩�ǂ����j
	BOOL			m_bIsNullable;
	BOOL			m_bIsInited;
	BOOL			m_bChanged;
	CComVariant		m_vaValue;				//!< ���݂̒l�i�ύX�����ꍇ�͂��̒l���ꏏ�ɕύX�����j
	CComVariant		m_vaInitValue;			//!< ������Ԃ̒l�i�ύX����y�ђl�����ɖ߂��ꍇ�Ɏg�p�j
	LONG			m_lFieldIndex;			//!< �t�B�[���h�̃C���f�b�N�X�ԍ��iCRowDef::Delate()�ACRowDef::Create() �Ŏg�p����̂Ńp�u���b�N�j
	IFieldPtr		m_ipField;				//!< CAttrDlg::CreateLinkTimeRegRow() �ŃZ�b�g����K�v������̂Ńp�u���b�N
private:
	BOOL			m_bIsNotDomain;			//!< �h���C�������邩�ǂ����̃t���O�i�����ς݊��Ȃ��ꍇ��TRUE�A����ȊO��FALSE�j
	BOOL			m_bIsUnsetOld;			//!< m_bIsUnset �̕ۑ��p�i�V�K�t�B�[���h�ǉ��̏ꍇ�� m_bIsUnset != m_bIsUnsetOld�j
	INT				m_nRelType;				//!< = 2 �̎��� Node_Queue ���g�p����̂� Store ���ɓ��ʏ����K�v
	DOMAINMAP		m_mapDomain;			//!< �R�[�h�l�h���C�����
	RANGEDOMAIN		m_strctRangeDomain;		//!< �����W�h���C��
	IRowBufferPtr	m_ipRowBuffer;			//!< �Ή����� IRowBuffer
};

#endif // !defined(AFX_FIELDDEF_H__6B343E74_CE5C_4B23_B417_E5BC6DBA1D53__INCLUDED_)
