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

#ifndef SINDY_ADVISEADAPTERIMPL_H__
#define SINDY_ADVISEADAPTERIMPL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*#ifndef _ADVISEADAPTER_H_
#error AdviseAdapterImpl.h requires AdviseAdapter.h to be included first
#endif // ifndef _ADVISEADAPTER_H_
*/
#include "AdviseAdapter.h"

template<class T, class Interface, const IID* pdiid>
class CAdviseAdapterHelper
{
protected:
	HRESULT Advise( Interface* ipObj )
	{
		T* pT = static_cast<T*>(this);
		m_cAdapter = ipObj;
		return m_cAdapter.Advise( pT->GetUnknown() );
	}
	HRESULT Unadvise()
	{
		HRESULT hr = m_cAdapter.Unadvise();
		m_cAdapter = NULL;
		return hr;
	}
protected:
	CAdviseAdapter<Interface, pdiid> m_cAdapter;
};

#endif // ifndef SINDY_ADVISEADAPTERIMPL_H__
