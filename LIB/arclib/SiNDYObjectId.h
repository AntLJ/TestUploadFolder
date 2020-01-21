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

#ifndef __SINDY_OBJECTID_H__
#define __SINDY_OBJECTID_H__

#include "SiNDYAttrCommunity.h"
#include "SiNDYFieldIndex.h"

namespace sindy {

/**
 * @brief OID�����o�����߂����̃N���X
 *
 * SINDY_FIELDVALUE_CLASS�}�N���̈����Ƃ��Ďg����B�Ɨǂ��B
 */
template <LPCTSTR TName>
class OIDT
{
public:
// ��`
	/**
	 * @note typdef�Œ�`����ƌp�����ɕs����N����̂ł����Čp�����g�p�B
	 */
	class FieldIndex : public sindy::FieldIndex<TName>
	{
	public:
		template <typename T>
		FieldIndex(T const& rArg) : sindy::FieldIndex<TName>(rArg) { }
	};

// �\�z/����
	OIDT() : m_nObjectId(0) {}
	OIDT(_IRow* ipRow) { ipRow->get_OID(&m_nObjectId); }
	OIDT(IFeature* ipFeature) { ipFeature->get_OID(&m_nObjectId); }

// �ݒ�/�擾
	long getValue() const { return m_nObjectId; }

// ���Z�q�̃I�[�o�[���[�h
	operator long () const { return getValue(); }
	OIDT& operator=(_IRow* ipRow) { ipRow->get_OID(&m_nObjectId); return *this; }
	OIDT& operator=(IFeature* ipFeature) { ipFeature->get_OID(&m_nObjectId); return *this; }

	bool operator<(const OIDT& rObjectId) { return m_nObjectId < rObjectId.m_nObjectId; }

private:
// �ϐ�
	long m_nObjectId;
};

class ObjectId : public OIDT<kObjectId>
{
public:
	ObjectId() {}
	ObjectId(_IRow* ipRow) : OIDT<kObjectId>(ipRow) {}
	ObjectId(IFeature* ipFeature) : OIDT<kObjectId>(ipFeature) {}
};

class OID : public OIDT<kOID>
{
public:
	OID() {}
	OID(_IRow* ipRow) : OIDT<kOID>(ipRow) {}
	OID(IFeature* ipFeature) : OIDT<kOID>(ipFeature) {}
};

class FID : public OIDT<kFID>
{
public:
	FID() {}
	FID(_IRow* ipRow) : OIDT<kFID>(ipRow) {}
	FID(IFeature* ipFeature) : OIDT<kFID>(ipFeature) {}
};

}; // namespace sindy

#endif // __SINDY_OBJECTID_H__
