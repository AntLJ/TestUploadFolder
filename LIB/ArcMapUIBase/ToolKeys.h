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

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CToolKeys : public IToolKeys
{
public:
	CToolKeys(){};
	virtual ~CToolKeys(){};

	// IToolKeys
	STDMETHOD(OverrideKey)(long keyCode, long /*shift*/, VARIANT_BOOL* override)
	{
		if ( ! override )
			return E_POINTER;

		*override = VARIANT_FALSE;

		switch ( keyCode )
		{
			case 'Z':
			case 'X':
			case 'C':
			case 'B':
			case VK_ADD:
			case VK_SUBTRACT:
				*override = VARIANT_TRUE;
				break;
		}

		return S_OK;
	}
};
