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

#ifndef DND_H_
#define DND_H_

template <typename T>
class ATL_NO_VTABLE IDropSourceImpl : public IDropSource
{
public:
	IDropSourceImpl()
	{
	}

	virtual ~IDropSourceImpl()
	{
	}

public:
	STDMETHOD(QueryContinueDrag)(BOOL fEscapePressed, DWORD grfKeyState)
	{
		// ESCが押された場合やマウスのボタンが両方押されたときは中止
		if(fEscapePressed || (MK_LBUTTON | MK_RBUTTON) == (grfKeyState & (MK_LBUTTON | MK_RBUTTON))) {
			return DRAGDROP_S_CANCEL;
		}

		// マウスボタンが離されたときはドロップ
		if((grfKeyState & (MK_LBUTTON | MK_RBUTTON)) == 0) {
			return DRAGDROP_S_DROP;
		}

		return S_OK;
	}

	STDMETHOD(GiveFeedback)(DWORD dwEffect)
	{
		return DRAGDROP_S_USEDEFAULTCURSORS;
	}
};

class ATL_NO_VTABLE CDropSource :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDropSourceImpl<IDropSource>
{
public:
BEGIN_COM_MAP(CDropSource)
    COM_INTERFACE_ENTRY(IDropSource)
END_COM_MAP()

	static IDropSourcePtr create()
	{
		typedef CComObject<CDropSource> DropSource;

		DropSource* pSrc;
		DropSource::CreateInstance(&pSrc);
		return pSrc;
	}
};

class ATL_NO_VTABLE CSimpleDataObject :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDataObject
{
public:
	CSimpleDataObject();
	virtual ~CSimpleDataObject();

BEGIN_COM_MAP(CSimpleDataObject)
    COM_INTERFACE_ENTRY(IDataObject)
END_COM_MAP()

// IDataObject
	STDMETHOD(GetData)(FORMATETC *pFormatetc, STGMEDIUM *pmedium);
	STDMETHOD(GetDataHere)(FORMATETC *pFormatetc, STGMEDIUM *pmedium);
	STDMETHOD(QueryGetData)(FORMATETC *pFormatetc);
	STDMETHOD(GetCanonicalFormatEtc)(FORMATETC *pFormatetcIn, FORMATETC *pFormatetcInOut);
	STDMETHOD(SetData)(FORMATETC *pFormatetc, STGMEDIUM *pmedium, BOOL fRelease);
	STDMETHOD(EnumFormatEtc)(DWORD dwDirection, IEnumFORMATETC **ppenumFormatetc);
	STDMETHOD(DAdvise)(FORMATETC *pFormatetc, DWORD advf, IAdviseSink *pAdvSink, DWORD *pdwConnection);
	STDMETHOD(DUnadvise)(DWORD dwConnection);
	STDMETHOD(EnumDAdvise)(IEnumSTATDATA **ppenumAdvise);

	static IDataObjectPtr create(LPCSTR src);

protected:
	FORMATETC fmt;
	STGMEDIUM medium;
};

namespace detail {

inline bool duplicate_STGMEDIUM(STGMEDIUM *pdest, const FORMATETC* pFormatetc, const STGMEDIUM *pmedium)
{
	HANDLE hVoid;

	switch (pmedium->tymed) {
	case TYMED_HGLOBAL:
		hVoid = OleDuplicateData(pmedium->hGlobal, pFormatetc->cfFormat, (UINT)NULL);
		pdest->hGlobal = static_cast<HGLOBAL>(hVoid);
		break;
	case TYMED_GDI:
		hVoid = OleDuplicateData(pmedium->hBitmap, pFormatetc->cfFormat, (UINT)NULL);
		pdest->hBitmap = static_cast<HBITMAP>(hVoid);
		break;
	case TYMED_MFPICT:
		hVoid = OleDuplicateData(pmedium->hMetaFilePict, pFormatetc->cfFormat, (UINT)NULL);
		pdest->hMetaFilePict = static_cast<HMETAFILEPICT>(hVoid);
		break;
	case TYMED_ENHMF:
		hVoid = OleDuplicateData(pmedium->hEnhMetaFile, pFormatetc->cfFormat, (UINT)NULL);
		pdest->hEnhMetaFile = static_cast<HENHMETAFILE>(hVoid);
		break;
	case TYMED_FILE:
		hVoid = OleDuplicateData(pmedium->lpszFileName, pFormatetc->cfFormat, (UINT)NULL);
		pdest->lpszFileName = static_cast<LPOLESTR>(hVoid);
		break;
	case TYMED_NULL:
	case TYMED_ISTREAM:
	case TYMED_ISTORAGE:
	default:
		hVoid = NULL;
		break;
	}

	if(hVoid == NULL)
		return false;

	pdest->tymed = pmedium->tymed;
	pdest->pUnkForRelease = pmedium->pUnkForRelease;

	if(pmedium->pUnkForRelease != NULL) {
		pmedium->pUnkForRelease->AddRef();
	}

	return	true;
}

} // namespace detail

inline CSimpleDataObject::CSimpleDataObject()
{
	medium.tymed = TYMED_NULL;
}

inline CSimpleDataObject::~CSimpleDataObject()
{
	if(medium.tymed != TYMED_NULL) {
		::ReleaseStgMedium(&medium);
	}
}

inline STDMETHODIMP CSimpleDataObject::GetData(FORMATETC* pFormatetc, STGMEDIUM* pmedium)
{
	if(pFormatetc == NULL || pmedium == NULL)
		return E_INVALIDARG;

	HRESULT hr = QueryGetData(pFormatetc);
	if(hr != S_OK)
		return hr;

	if(! detail::duplicate_STGMEDIUM(pmedium, pFormatetc, &medium))
		return E_OUTOFMEMORY;

	return S_OK;
}

inline STDMETHODIMP CSimpleDataObject::GetDataHere(FORMATETC*, STGMEDIUM*)
{
	return E_NOTIMPL;
}

inline STDMETHODIMP CSimpleDataObject::QueryGetData(FORMATETC* pFormatetc)
{
	if(pFormatetc == NULL)
		return E_INVALIDARG;

	if (! (DVASPECT_CONTENT & pFormatetc->dwAspect))
		return DV_E_DVASPECT;

	if(fmt.cfFormat == pFormatetc->cfFormat && (fmt.tymed & pFormatetc->tymed) != 0)
		return S_OK;

	return DV_E_FORMATETC;
}

inline STDMETHODIMP CSimpleDataObject::EnumFormatEtc(DWORD , IEnumFORMATETC**)
{
	return E_NOTIMPL;
}

inline STDMETHODIMP CSimpleDataObject::SetData(FORMATETC*, STGMEDIUM*, BOOL)
{
	return E_NOTIMPL;
}

inline STDMETHODIMP CSimpleDataObject::GetCanonicalFormatEtc(FORMATETC*, FORMATETC*)
{
	return E_NOTIMPL;
}

inline STDMETHODIMP CSimpleDataObject::DAdvise(FORMATETC*, DWORD, IAdviseSink*, DWORD*)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

inline STDMETHODIMP CSimpleDataObject::DUnadvise(DWORD)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

inline STDMETHODIMP CSimpleDataObject::EnumDAdvise(IEnumSTATDATA**)
{
	return OLE_E_ADVISENOTSUPPORTED;
}

inline IDataObjectPtr CSimpleDataObject::create(LPCSTR src)
{
	typedef CComObject<CSimpleDataObject> SimpleDataObject;

	SimpleDataObject* pDataObj;
	if(FAILED(SimpleDataObject::CreateInstance(&pDataObj)))
		return 0;

	const IDataObjectPtr ipRes(pDataObj);

	HGLOBAL hText = ::GlobalAlloc(GHND, ::strlen(src) + 1);
	if(hText == NULL)
		return 0;
	
	char* p = static_cast<char*>(::GlobalLock(hText));
	::strcpy(p, src);
	::GlobalUnlock(hText);

	pDataObj->fmt.cfFormat = CF_TEXT;
	pDataObj->fmt.dwAspect = DVASPECT_CONTENT;
	pDataObj->fmt.lindex = -1;
	pDataObj->fmt.ptd = NULL;
	pDataObj->fmt.tymed = TYMED_HGLOBAL;

	pDataObj->medium.hGlobal = hText;
	pDataObj->medium.tymed = TYMED_HGLOBAL;
	pDataObj->medium.pUnkForRelease = NULL;
	
	return ipRes;
}

#endif // DND_H_
