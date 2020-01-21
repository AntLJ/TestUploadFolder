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
 * @file LicenseFunctions.cpp
 * @brief ƒ‰ƒCƒZƒ“ƒXŒnŠÖ”À‘•ƒtƒ@ƒCƒ‹
 * @author ’n}®”õ–{•”’n}‚c‚a§ì•”ƒVƒXƒeƒ€ŠJ”­ƒOƒ‹[ƒv ŒÃì‹MG
 * @version $Id$
 */
#include "stdafx.h"
#include "AheLicenseFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool CheckLicense( esriLicenseProductCode code )
{
	IAoInitializePtr ipInit;
	if( NULL == ipInit )
		ipInit.CreateInstance( CLSID_AoInitialize );

	if( NULL != ipInit )
	{
		esriLicenseProductCode emCode = (esriLicenseProductCode)0;
		ipInit->InitializedProduct( &emCode );
		if( emCode >= code )
			return true;
		else
			return false;
	}
	return false;
}
