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
 * @file dmodifier.h
 * @brief for field.h
 */
#ifndef ARCTL_FIELD_DYNAMIC_MODIFIER_H_
#define ARCTL_FIELD_DYNAMIC_MODIFIER_H_

#include <map>
#include <boost/shared_ptr.hpp>

namespace arctl {

namespace field {

// ��`

/**
 * @brief ���I�X�V�N���X�B
 */
class dmodifier
{
public:
// ��`
	typedef std::map<long, _variant_t> NewValueMap;
	typedef NewValueMap::value_type value_type;

//@{ @name �\�z/����
	dmodifier();
	dmodifier(long nIndexId, const VARIANT& rVariant);
//@}

//@{ @name �ݒ�/�擾
	void insert(long nIndexId, const VARIANT& rVariant);
//@}

//@{ @name ���Z
	bool operator()(IRowBuffer* ipRow) const;
//@}
private:
// �ϐ�
	boost::shared_ptr<NewValueMap> m_pNewValueMap;
};

// ����

inline dmodifier::dmodifier() :
m_pNewValueMap(new NewValueMap)
{
}

inline dmodifier::dmodifier(long nIndexId, const VARIANT& rVariant) :
m_pNewValueMap(new NewValueMap)
{
	m_pNewValueMap->insert(value_type(nIndexId, rVariant));
}

inline void dmodifier::insert(long nIndexId, const VARIANT& rVariant)
{
	(*m_pNewValueMap)[nIndexId] = rVariant;
}

inline bool dmodifier::operator()(IRowBuffer* ipRow) const
{
	bool bModified = false;

	for(NewValueMap::const_iterator it = m_pNewValueMap->begin(); it != m_pNewValueMap->end(); ++it) {
		_variant_t va;
		ipRow->get_Value(it->first, &va);
		if(va != it->second) {
			bModified |= (ipRow->put_Value(it->first, it->second) == S_OK);
		}
	}

	return bModified;
}

} // namespace field

} // namespace arctl

#endif // ARCTL_FIELD_DYNAMIC_MODIFIER_H_
