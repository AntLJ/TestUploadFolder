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

#ifndef IACTIVEVIEWEVENTSIMPL_H__
#define IACTIVEVIEWEVENTSIMPL_H__

template<class T>
class ATL_NO_VTABLE CIActiveViewEventsImpl : public IActiveViewEvents, public CAdviseAdapterHelper<T, IActiveView, &IID_IActiveViewEvents>
{
public:
	STDMETHOD(ContentsChanged)(){ return S_OK; }
	STDMETHOD(ContentsCleared)(){ return S_OK; }
	STDMETHOD(ItemAdded)(VARIANT /*Item*/){ return S_OK; }
	STDMETHOD(ItemDeleted)(VARIANT /*Item*/){ return S_OK; }
	STDMETHOD(ItemReordered)(VARIANT /*Item*/, LONG /*toIndex*/){ return S_OK; }
	STDMETHOD(SelectionChanged)(){ return S_OK; }
	STDMETHOD(ViewRefreshed)(IActiveView * /*view*/, esriViewDrawPhase /*phase*/, VARIANT /*data*/, IEnvelope * /*envelope*/){ return S_OK; }
	STDMETHOD(AfterDraw)(IDisplay * /*Display*/, esriViewDrawPhase /*phase*/){ return S_OK; }
	STDMETHOD(AfterItemDraw)(SHORT /*Index*/, IDisplay * /*Display*/, esriDrawPhase /*phase*/){ return S_OK; }
	STDMETHOD(FocusMapChanged)(){ return S_OK; }
	STDMETHOD(SpatialReferenceChanged)(){ return S_OK; }
};

#endif // ifndef IACTIVEVIEWEVENTSIMPL_H__
