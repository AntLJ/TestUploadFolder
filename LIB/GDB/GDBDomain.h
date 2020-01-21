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
 *	GDBDomain.h
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		êVãKçÏê¨
 */

#ifndef	___GDB_DOMAIN_H___
#define	___GDB_DOMAIN_H___

#include <string>

extern HRESULT GDBGetCodedValueDomain(IFeaturePtr ipFeature, const std::string &cColumn, ICodedValueDomain **ipCodedValueDomain);
extern HRESULT GDBGetCodedValueDomain(_IRowPtr ipRow, const std::string &cColumn, ICodedValueDomain **ipCodedValueDomain);

extern HRESULT GDBGetDomainName(ICodedValueDomainPtr ipCodedValueDomain, VARIANT vaVal, std::string *cName);
extern HRESULT GDBGetDomainName(_IRowPtr ipRow, const std::string &cColumn, std::string *cName);
extern HRESULT GDBGetDomainName(IFeaturePtr ipFeature, const std::string &cColumn, std::string *cName);

#endif	//___GDB_DOMAIN_H___
