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

#ifndef ARCTL_SPECIF_FIELD_H_
#define ARCTL_SPECIF_FIELD_H_

namespace arctl {

namespace specif {

template <LPCTSTR FieldName>
class field_t : public IFieldPtr
{
public:
	field_t(IFields* ipFields) { operator=(ipFields); }

	const IField* get() const { return GetInterfacePtr(); }

	field_t& operator=(IFields* ipFields)
	{
		_ASSERT(ipFields);

		long nIndex;
		atl2::valid(ipFields->FindField(CComBSTR(FieldName), &nIndex));
		atl2::valid(ipFields->get_Field(nIndex, operator&()));

		return *this;
	}
};

} // namespace specif
 
} // namespace arctl

#endif // ARCTL_SPECIF_FIELD_H_
