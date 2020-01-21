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

#pragma once

#include <boost/shared_ptr.hpp>
#include "TableType.h"

namespace sindy {
class CFieldMap;
class CTableNameString;
typedef boost::shared_ptr<CFieldMap> CSPFieldMap;
typedef boost::shared_ptr<CTableNameString> CSPTableNameString;

class CSchemaSupport
{
public:
	CSchemaSupport();
	virtual ~CSchemaSupport();

	CSchemaSupport( const CSchemaSupport& obj );
	CSchemaSupport( sindyTableType::ECode emType, CSPTableNameString spNames, CSPFieldMap spFields );

	const CSchemaSupport& operator=( const CSchemaSupport& obj );

	void SetFieldMap( CSPFieldMap cFields ) { m_spFields = cFields; }
	void SetNameString( CSPTableNameString cNames ) { m_spNames = cNames; }
	virtual CSPFieldMap GetFieldMap() const { return m_spFields; }
	virtual CSPTableNameString GetNameString() const { return m_spNames; }

	CSPFieldMap GetFieldMap( IFields* ipFields ) const;
	CSPTableNameString GetNameString( ITable* ipTable ) const;

	/**
	 * @brief <b>�e�[�u���^�C�v���擾����</b>\n
	 * @return sindyTableType::ECode
	 */
	sindyTableType::ECode GetTableType() const;
	void SetTableType( sindyTableType::ECode emTableType ) { m_emTableType = emTableType; }

	/**
	 * @brief <b>�e�[�u������Ԃ�</b>\n
	 * @see CTableNameString::GetTableName()
	 * @retval LPCTSTR
	 */
	LPCTSTR GetTableName() const;

	/**
	 * @brief <b>�e�[�u���I�[�i�[�����擾����</b>\n
	 * �Ԃ���镶����͕K���啶���ł��B
	 * @return �e�[�u���I�[�i�[��
	 */
	LPCTSTR GetOwnerName() const;

	/**
	 * @brief <b>�I�[�i�[���t���e�[�u�������擾����</b>\n
	 * �u�I�[�i�[���v.�u�e�[�u�����v�̌`���ŕԂ��܂��B
	 * �e�[�u���������łȂ��I�[�i�[�����K�v�ȏꍇ�͂�������g�p���Ă�������\n
	 * �Ԃ���镶����͕K���啶���ł��B
	 * @return �I�[�i�[���t���e�[�u����
	 */
	LPCTSTR GetOwnerTableName() const;

	/**
	 * @brief <b>OBJECTID�̃t�B�[���h�C���f�b�N�X�ԍ����擾����</b>\n
	 * @retval 0���� �G���[
	 * @retval 0�ȏ� �C���f�b�N�X�ԍ�
	 */
	long GetOIDFieldIndex() const;

	/**
	 * @brief �t�B�[���h������C���f�b�N�X�ԍ����擾����
	 * @param lpcszFieldName [in] �t�B�[���h��
	 * @retval -1 �t�B�[���h�������݂��Ȃ�
	 * @retval -1�ȊO �t�B�[���h�C���f�b�N�X�ԍ�
	 */
	long FindField( LPCTSTR lpcszFieldName ) const;
protected:
	mutable sindyTableType::ECode	m_emTableType;	//!< �i�[����Ă���t�B�[�`���̃e�[�u�����ʎq
	mutable CSPTableNameString		m_spNames;		//!< �e�[�u�����L���b�V���p�ishared ptr�j
	mutable CSPFieldMap				m_spFields;		//!< �t�B�[���h�Ή��\�ishared ptr�j
};

typedef boost::shared_ptr<CSchemaSupport> CSPSchemaSupport;

} // sindy
