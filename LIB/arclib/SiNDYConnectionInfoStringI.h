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
// �\�z/����
	ConnectionInfoStringI();
	virtual ~ConnectionInfoStringI();

// �ݒ�
	void setConnectionInfoString(IWorkspace* ipWorkspace);
	bool setConnectionInfoString(IPropertySet* ipPropertySet);

// ����
	IWorkspacePtr connect();

// �ÓI����
	static IWorkspacePtr connectWorkspace(LPCTSTR lpszConnectionInfo);

protected:
// �������z�֐�
	virtual void setString(LPCTSTR lpszConnectionInfo) = 0;
	virtual LPCTSTR getString() const = 0;
};

} // namespace sindy;

#endif // __SINDY_CONNECTION_INFO_STRING_I_H__

