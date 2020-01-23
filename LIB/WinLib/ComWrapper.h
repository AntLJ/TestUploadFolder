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

/**
 * @file ComWrapper.h
 * @brief CComWrapperクラス定義ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _COMWRAPPER_H_
#define _COMWRAPPER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @class CComWrapper
 * @brief <b>COMオブジェクトのラッパークラス</b>\n
 * _com_ptr_t<>やCComPtrではラップクラスのポインタや参照を
 * 返すことができない（&や*のオペレータが定義されている）ので
 * その場合はこのクラスを基底クラスにします。
 **/
template <class T>
class CComWrapper
{
public:
	typedef T _PtrClass;
	explicit CComWrapper()
	{
		p=NULL;
	}
/*	CComWrapper(T* lp)
	{
		if ((p = lp) != NULL)
			p->AddRef();
	}
	CComWrapper(const CComPtr<T>& lp)
	{
		if ((p = lp.p) != NULL)
			p->AddRef();
	}*/
	virtual ~CComWrapper()
	{
		if (p)
			p->Release();
	}
/*	operator T*() const
	{
		return (T*)p;
	}*/
/*	T& operator*() const
	{
		ATLASSERT(p!=NULL);
		return *p;
	}
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&()
	{
		ATLASSERT(p==NULL);
		return &p;
	}*/
/*	_NoAddRefReleaseOnCComPtr<T>* operator->() const
	{
		ATLASSERT(p!=NULL);
		return (_NoAddRefReleaseOnCComPtr<T>*)p;
	}
	T* operator=(T* lp)
	{
		return (T*)AtlComPtrAssign((IUnknown**)&p, lp);
	}
	T* operator=(const CComPtr<T>& lp)
	{
		return (T*)AtlComPtrAssign((IUnknown**)&p, lp.p);
	}*/
	/**
	 * @brief ポインタ渡し（&hogeなど）で渡すときは、必ずReleaseを使用すること！！
	 */
/*	T* _p() const {
		return (T*)p;
	}*/
	bool operator!() const
	{
		return (p == NULL);
	}
	bool operator<(T* pT) const
	{
		return p < pT;
	}
	bool operator==(T* pT) const
	{
		return p == pT;
	}
	bool operator==(int null) const
	{
		return (int)p == null;
	}
	bool operator!=(T* pT) const
	{
		return p != pT;
	}
	bool operator!=(int null) const
	{
		return (int)p != null;
	}
	// Compare two objects for equivalence
	bool IsEqualObject(IUnknown* pOther)
	{
		if (p == NULL && pOther == NULL)
			return true; // They are both NULL objects

		if (p == NULL || pOther == NULL)
			return false; // One is NULL the other is not

		CComPtr<IUnknown> punk1;
		CComPtr<IUnknown> punk2;
		p->QueryInterface(IID_IUnknown, (void**)&punk1);
		pOther->QueryInterface(IID_IUnknown, (void**)&punk2);
		return punk1 == punk2;
	}
/*	void Attach(T* p2)
	{
		if (p)
			p->Release();
		p = p2;
	}
	T* Detach()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}
	HRESULT CopyTo(T** ppT)
	{
		ATLASSERT(ppT != NULL);
		if (ppT == NULL)
			return E_POINTER;
		*ppT = p;
		if (p)
			p->AddRef();
		return S_OK;
	}
	HRESULT SetSite(IUnknown* punkParent)
	{
		return AtlSetChildSite(p, punkParent);
	}*/
/*	HRESULT Advise(IUnknown* pUnk, const IID& iid, LPDWORD pdw)
	{
		return AtlAdvise(p, pUnk, iid, pdw);
	}*/
	HRESULT CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		ATLASSERT(p == NULL);
		return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
	}
/*	HRESULT CoCreateInstance(LPCOLESTR szProgID, LPUNKNOWN pUnkOuter = NULL, DWORD dwClsContext = CLSCTX_ALL)
	{
		CLSID clsid;
		HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
		ATLASSERT(p == NULL);
		if (SUCCEEDED(hr))
			hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
		return hr;
	}*/
	template <class Q>
	HRESULT QueryInterface(Q** pp) const
	{
		ATLASSERT(pp != NULL && *pp == NULL);
		return p->QueryInterface(__uuidof(Q), (void**)pp);
	}
//protected:
	/**
	 * @brief <b>インターフェースポインタをリリースする</b>\n
	 * インターフェースポインタをリリースします。
	 * 通常のラッパークラスと違うのは、インターフェースポインタのポインタ
	 * を返すところです。
	 * 例えば、引数としてT**をとるようなCOMのメソッドに自分自身を渡す場合、
	 * 通常だとRelease()して自分自身をNULLにしてから、そのポインタを渡す
	 * 必要があります。しかし、それだとRelease()し忘れてインターフェースが
	 * リークすることがあります。そういう場合は、このメソッドを使用して
	 * リリースを同時に行ってください。
	 * <h3>例：自分がCComWrapper<IWorkspace>の時、IWorkspaceFactory::Open()で自分自身にオープンしたワークスペースをアタッチしたい時</h3>
	 * @code
	 * IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
	 * ipWorkspaceFactory->OpenFromFile( _T("C:\\hoge.mdb"), 0, Release() );
	 * @endcode
	 * @return インターフェースポインタのポインタ
	 */
	virtual T** Release()
	{
		IUnknown* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
		return &p;
	}

	CComWrapper(const CComWrapper& r)
	{
		if((p = r.p) != 0) {
			p->AddRef();
		}
	}

#ifdef DEBUG
	void TraceRefCount() const
	{
		if( p )
		{
			ATLTRACE(_T("refcount : %d\n"), p->AddRef() );
			p->Release();
		}
	}
#endif // DEBUG
protected:
	T* p;
};

//template <class T> bool operator==( const int& n, const CComWrapper<T>& obj ){ return (obj==(T*)n); }
//template <class T> bool operator!=( const int& n, const CComWrapper<T>& obj ){ return (obj!=(T*)n); }

#endif // !defined(AFX_COMWRAPPER_H__1DE61A98_171D_4E3D_8FA3_1640914BD1C8__INCLUDED_)
