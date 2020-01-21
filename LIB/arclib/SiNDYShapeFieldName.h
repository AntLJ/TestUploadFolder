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

#ifndef __SINDY_FIELDVALUE_STRING_H__
#define __SINDY_FIELDVALUE_STRING_H__

#include "SiNDYImplicitCast.h"

namespace sindy {

// CShapeFieldNameBSTR 定義
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief フィーチャクラスの形状フィールド名をCComBSTR型で取得するクラス
 */
class CShapeFieldNameBSTR_ : public CComBSTR
{
public:
	CShapeFieldNameBSTR_(IFeatureClass* ipFeatureClass);
	CShapeFieldNameBSTR_(IFeatureClassPtr ipFeatureClass);

private:
	void setShapeFieldName(IFeatureClass* ipFeatureClass);
};

typedef InterfacePtr_<CShapeFieldNameBSTR_> CShapeFieldNameBSTR;

// CShapeFieldNameBSTR 実装
/////////////////////////////////////////////////////////////////////////////

inline CShapeFieldNameBSTR_::CShapeFieldNameBSTR_(IFeatureClass* ipFeatureClass)
{
	setShapeFieldName(ipFeatureClass);
}

inline CShapeFieldNameBSTR_::CShapeFieldNameBSTR_(IFeatureClassPtr ipFeatureClass)
{
	setShapeFieldName(static_cast<IFeatureClass*>(ipFeatureClass));
}

inline void CShapeFieldNameBSTR_::setShapeFieldName(IFeatureClass* ipFeatureClass)
{
	if(ipFeatureClass) {
		ipFeatureClass->get_ShapeFieldName(CComBSTR::operator&());
	}
}

}; // namespace sindy

#endif // __SINDY_FIELDVALUE_STRING_H__
