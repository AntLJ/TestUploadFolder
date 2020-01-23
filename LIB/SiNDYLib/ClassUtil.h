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
 * @file ClassUtil.h
 * @brief <b>�N���X��`�p���[�e�B���e�B��`�t�@�C��</b>
 * ���̃t�@�C���ɂ́ACRow���p������N���X��`�̂��߂�
 * ���[�e�B���e�B����`����Ă��܂��B
 * ���ɁA�N���X�̃R���X�g���N�^�E�f�X�g���N�^��A�R�s�[�I�y���[�^�Ȃ�
 * ���L�̋L�q���������̂Ɋւ��Ē�`���ȒP�ɍςނ悤�ȃ}�N�����`����
 * ����܂��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */

/**
 * @brief <b>���R�[�h�x�[�X�N���X�p�̃R���X�g���N�^�}�N��</b>\n
 * ���R�[�h�̃��b�v�N���X�Ŏg�p���Ă��������B
 */
#define ROWCLASS_CONSTRUCTOR( class_name, table_type ) \
public: \
	class_name() \
	{ \
		Init(); \
		m_emTableType = sindyTableType::table_type; \
	} \
	virtual ~class_name(){ /*DeleteCache();*/ } \
	class_name( const class_name& obj ) \
	{ \
		Init(); \
		*this = obj; \
	} \
	class_name( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::table_type, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() ) \
	{ \
		Init(); \
		SetObject( lp, emTableType, bCreated, spFields, spNameString ); \
	} \
	class_name& operator=( const class_name& obj ) \
	{ \
		obj.CopyTo( *this ); \
		return *this; \
	} \
	class_name& operator=( _IRow* lp ) \
	{ \
		SetObject( lp, sindyTableType::table_type ); \
		return *this; \
	}
