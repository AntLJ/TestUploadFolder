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

#ifndef IDOCUMENTEVENTSIMPL_H__
#define IDOCUMENTEVENTSIMPL_H__

template<class T>
class ATL_NO_VTABLE CIDocumentEventsImpl : public IDocumentEvents, public CAdviseAdapterHelper<T, IDocument, &IID_IDocumentEvents>
{
public:
	STDMETHOD(ActiveViewChanged)(){ return S_OK; }
	STDMETHOD(BeforeCloseDocument)(VARIANT_BOOL * /*abortClose*/){ return S_OK; }
	STDMETHOD(CloseDocument)(){ return S_OK; }
	STDMETHOD(MapsChanged)(){ return S_OK; }
	STDMETHOD(NewDocument)(){ return S_OK; }
	STDMETHOD(OnContextMenu)(LONG /*X*/, LONG /*Y*/, VARIANT_BOOL * /*handled*/){ return S_OK; }
	STDMETHOD(OpenDocument)(){ return S_OK; }
};

#endif // ifndef IDOCUMENTEVENTSIMPL_H__
