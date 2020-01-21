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

#ifndef __SINDY_CONNECTION_INFO_STRING_I_H__
#define __SINDY_CONNECTION_INFO_STRING_I_H__

#include "exportimport.h"

namespace sindy {

class SINDYLIB_API ConnectionInfoStringI
{
public:
// ç\íz/è¡ñ≈
	ConnectionInfoStringI();
	virtual ~ConnectionInfoStringI();

// ê›íË
	void setConnectionInfoString(IWorkspace* ipWorkspace);
	bool setConnectionInfoString(IPropertySet* ipPropertySet);

// èàóù
	IWorkspacePtr connect();

// ê√ìIèàóù
	static IWorkspacePtr connectWorkspace(LPCTSTR lpszConnectionInfo);

protected:
// èÉêàâºëzä÷êî
	virtual void setString(LPCTSTR lpszConnectionInfo) = 0;
	virtual LPCTSTR getString() const = 0;
};

} // namespace sindy;

#endif // __SINDY_CONNECTION_INFO_STRING_I_H__

