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

#include <TDC/sindylib_base/IndexedContainer.h>

/// �����Ώۏ����Ǘ��p�N���X
class Condition
{
public:
	/// �R���X�g���N�^�i����j
	Condition(){}
	
	/// �ǉ�
	void add( const CString& key, const CComVariant& val );
	/// �L�[�Q�擾
	const sindy::KeyList keys() const { return keys_; } 
	/// �l�Q�擾
	const sindy::ValueList values() const { return values_; }
	/// �\���p
	CString getQuery( IWorkspacePtr work ) const ;
private:
	/// �����`�F�b�N
	void check( const CString& key, const CComVariant& val );

	sindy::KeyList keys_;      //!< �L�[�Q
	sindy::ValueList values_;  //!< �l�Q
};

/// �����Ώۏ����Q�Ǘ��p�N���X
class ConditionMgr
{
public:
	ConditionMgr(){}
	virtual ~ConditionMgr(void){}

	void init( const CString& filePath );

	std::vector<Condition>::const_iterator begin() const { return conditions.cbegin(); }
	std::vector<Condition>::const_iterator end() const { return conditions.cend(); }

private:
	void add( const Condition& con ){ conditions.push_back(con); }

	std::vector<Condition> conditions;
};

