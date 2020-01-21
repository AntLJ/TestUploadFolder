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
 * @file SiNDYField.h
 * @brief �����t�B�[���h�擾�E�ێ��N���X��`
 * @author �n�}DB���암���H����O���[�v �e�n�����Y
 * $Id$
 */
#ifndef _SINDY_FIELD_H_
#define _SINDY_FIELD_H_

#include "SiNDYFieldIndex.h"

namespace sindy {

//////////////////////////////////////////////////////////////////////////////////////////
//
// Field
//
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief �����t�B�[���h�ƃC���f�b�N�X���擾�E�i�[����N���X
 *
 * ����N���X�̃R���X�g���N�^�͊��p�����A�C���f�b�N�X�����N���X�Őݒ肷��B
 */
template <LPCTSTR _FieldName>
class Field : public FieldIndex<_FieldName>
{
public:
// �\�z
	/// IFields�ɂ��\�z
	Field(IFields* ipFields)
	{
		init(ipFields);
	}

	/// IFields�X�}�[�g�|�C���^�ɂ��\�z
	Field(const IFieldsPtr& ipFields)
	{
		init(ipFields);
	}

// �擾
	operator IField* () const
	{
		return m_ipField;
	}

private:
// �\�z
	/**
	 * @brief �����t�B�[���h�A����т��̃C���f�b�N�X��ݒ肷��
	 *
	 * @param ipFields [in] �����t�B�[���h�ꗗ
	 */
	void init(IFields* ipFields)
	{
		ipFields->FindField(CComBSTR(_FieldName), &m_nFieldIndex);
		ipFields->get_Field(m_nFieldIndex, &m_ipField);
	}

// �ϐ�
	IFieldPtr m_ipField; ///< �����t�B�[���h
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// �֐�
//
//////////////////////////////////////////////////////////////////////////////////////////

// field_cast

template <LPCTSTR _Name>
inline const Field<_Name>& field_cast(const Field<_Name>& rField)
{
	return rField;
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// SINDY_FIELD_CLASS
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "SiNDYInheritMacro.h"


#define SINDY_FIELD_CLASS(c, n, tuple) SINDY_MULTIPLE_INHERITANCE_I(c, n, _LIST_##n tuple, _SINDY_FIELD_INHERIT, _SINDY_FIELD_CONSTRUCT)

#define _SINDY_FIELD_INHERIT(name) public sindy::Field<name>
#define _SINDY_FIELD_CONSTRUCT(name) sindy::Field<name>(_Arg1)

} // namespace sindy

#endif // _SINDY_FIELD_H_
