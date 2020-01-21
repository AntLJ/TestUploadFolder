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

#ifndef SINDYEDITOR_IEDITEVENTSIMPL_H__
#define SINDYEDITOR_IEDITEVENTSIMPL_H__

template<class T>
class ATL_NO_VTABLE CIEditEventsImpl : public IEditEvents, public CAdviseAdapterHelper<T, IEditor, &IID_IEditEvents>
{
public:
	STDMETHOD(OnSelectionChanged)(){ return S_OK; }
	STDMETHOD(OnCurrentLayerChanged)(){ return S_OK; }
	STDMETHOD(OnCurrentTaskChanged)(){ return S_OK; }
	STDMETHOD(OnSketchModified)(){ return S_OK; }
	STDMETHOD(OnSketchFinished)(){ return S_OK; }
	STDMETHOD(AfterDrawSketch)(IDisplay * pDpy){ return S_OK; }
	STDMETHOD(OnStartEditing)(){ return S_OK; }
	STDMETHOD(OnStopEditing)(VARIANT_BOOL Save){ return S_OK; }
	STDMETHOD(OnConflictsDetected)(){ return S_OK; }
	STDMETHOD(OnUndo)(){ return S_OK; }
	STDMETHOD(OnRedo)(){ return S_OK; }
	STDMETHOD(OnCreateFeature)(IObject * obj){ return S_OK; }
	STDMETHOD(OnChangeFeature)(IObject * obj){ return S_OK; }
	STDMETHOD(OnDeleteFeature)(IObject * obj){ return S_OK; }
};

#endif // ifndef SINDYEDITOR_IEDITEVENTSIMPL_H__
