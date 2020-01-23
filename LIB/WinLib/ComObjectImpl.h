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

#ifndef COMOBJECTIMPL_H_
#define COMOBJECTIMPL_H_

template <typename I>
class CComObjectImpl : public I
{
public:
	STDMETHOD_(ULONG, AddRef)()
	{
		return 1;
	}

	STDMETHOD_(ULONG, Release)()
	{
		return 1;
	}

	STDMETHOD(QueryInterface)(REFIID iid, void ** ppvObject)
	{
		return _InternalQueryInterface(iid, ppvObject);
	}

	template <class Q>
	HRESULT STDMETHODCALLTYPE QueryInterface(Q** pp)
	{
		return QueryInterface(__uuidof(Q), (void**)pp);
	}
};
#endif
