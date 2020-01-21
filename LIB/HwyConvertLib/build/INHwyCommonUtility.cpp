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

#include "INHwyCommonUtility.h"
#include "INHwySchema.h"

long GetFacilClass(_IRow* cpRow, SINDY2INHWYINFO* cpHwyInfo)
{
	IFeaturePtr ipFacilInfoPoint;
	std::string aErrorMessage = "施設種別コードの取得に失敗";
	if (FAILED(cpHwyInfo->GetFacilPoint(cpRow, &ipFacilInfoPoint)))
		throw std::runtime_error(aErrorMessage);
	if (ipFacilInfoPoint == NULL)
		throw std::runtime_error(aErrorMessage);

	long aSindyFacilClass;
	try
	{
		aSindyFacilClass = ipFacilInfoPoint->GetValue( ipFacilInfoPoint->Fields->_FindField(_bstr_t(sindy::facil_info_point::FACILCLASS_C.c_str())) );
	}
	catch (...)
	{
		throw std::runtime_error(aErrorMessage);
	}

	return Sindy2INCode(in::schema::hwy_common_field::facil_class::tFacilClass, aSindyFacilClass);
}

bool needFacilNameChange( Customer cCustomer )
{
	// TF向けのみ変換を行わない
	switch (cCustomer)
	{
		case TF:
			return false;
		case PEC:
		case PEC_NO_SPECIFIC_TOLL:
		case BT_KIWIC:
		case BE:
		case MF:
		case SHP05:
		case OTHERS:
			break;
		default:
			ATLASSERT(0);
	}

	return true;
}
