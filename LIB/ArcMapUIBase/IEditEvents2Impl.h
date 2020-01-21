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

#ifndef SINDYEDITOR_IEDITEVENTS2IMPL_H__
#define SINDYEDITOR_IEDITEVENTS2IMPL_H__

template<class T>
class ATL_NO_VTABLE CIEditEvents2Impl : public IEditEvents2, public CAdviseAdapterHelper<T, IEditor, &IID_IEditEvents2>
{
public:
	STDMETHOD(OnCurrentZChanged)(){ return S_OK; }
	STDMETHOD(OnVertexMoved)(IPoint * point){ return S_OK; }
	STDMETHOD(OnVertexAdded)(IPoint * point){ return S_OK; }
	STDMETHOD(OnVertexDeleted)(IPoint * point){ return S_OK; }
	STDMETHOD(BeforeStopEditing)(VARIANT_BOOL save){ return S_OK; }
	STDMETHOD(OnAbort)(){ return S_OK; }
	STDMETHOD(OnStartOperation)(){ return S_OK; }
	STDMETHOD(BeforeStopOperation)(){ return S_OK; }
	STDMETHOD(OnStopOperation)(){ return S_OK; }
	STDMETHOD(OnSaveEdits)(){ return S_OK; }
};

#endif // ifndef SINDYEDITOR_IEDITEVENTS2IMPL_H__
