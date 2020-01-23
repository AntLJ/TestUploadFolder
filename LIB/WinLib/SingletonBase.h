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

/********** Start of singleton code **********/ 
template <class Base>
class  CMyComObject: public CComObject<Base>
{
public:
  // クラス ファクトリに、COM オブジェクトが削除されたことを知らせる変数。
  CMyComObject** m_ppObj;

  STDMETHOD_(ULONG, Release)()
  {
     ULONG l = InternalRelease();
     if (l == 0)
     {
        // NULL を設定して、クラス ファクトリに作成しなおすことを伝えます。
        *m_ppObj = NULL;
        delete this;
     }
     return l;
  }

  static HRESULT WINAPI CreateInstance(CMyComObject<Base>** pp)
  {
     ATLASSERT(pp != NULL);
     HRESULT hRes = E_OUTOFMEMORY;
     CMyComObject<Base>* p = NULL;
     ATLTRY(p = new CMyComObject<Base>())
     if (p != NULL)
     {
        p->SetVoid(NULL);
        p->InternalFinalConstructAddRef();
        hRes = p->FinalConstruct();
        p->InternalFinalConstructRelease();
        if (hRes != S_OK)
        {
           delete p;
           p = NULL;
        }
     }
     *pp = p;
     // クラス ファクトリで、ポインタ m_ppObj -> CComObject<> を初期化します。
     p->m_ppObj = pp;
     return hRes;
  }
};

template <class T>
class CMyComClassFactorySingleton : public CComClassFactory
{
public:
  CMyComObject<T>* m_pObj;   // COM オブジェクト。
  CComAutoCriticalSection m_sec;  // クラス ファクトリは CComObjectCached に格納されるため、クリティカル セクションが必要になります。すべてのクライアントは、このクラス ファクトリを示す同一の低水準ポインタを取得します。このクラス ファクトリ中のインスタンス データは、必ず同期しなければいけません。
  // IClassFactory
  STDMETHOD(CreateInstance)(LPUNKNOWN pUnkOuter, REFIID riid,
                            void** ppvObj)
  {
     if (NULL == ppvObj)
        return E_POINTER;
     *ppvObj = NULL;
     // シングルトンでのアグリゲーションはサポートされていません。
     ATLASSERT(pUnkOuter == NULL);
     if (pUnkOuter != NULL)
        return CLASS_E_NOAGGREGATION;
     HRESULT hRes = E_FAIL;
     // クリティカル セクションをロックします。
     m_sec.Lock();
     // COM オブジェクトを生成します。
     if (NULL == m_pObj)
     {
        hRes = CMyComObject<T>::CreateInstance(&m_pObj);
        if (SUCCEEDED(hRes))
        {
           // マーシャリングされたインターフェイスを開放し、ストリームを開放します。
           if (m_pStream)
           {
              AtlFreeMarshalStream(m_pStream);
              m_pStream = NULL;
           }
           // ストリームに対して、インターフェイスをマーシャリングします。
           // MSHLFLAGS_TABLEWEAK を使わなければいけないので、AtlMarshalPtrInProc は使えません。
           hRes = CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);
           if (SUCCEEDED(hRes))
           {
			  IUnknown* ipUnk;
			  m_pObj->QueryInterface( IID_IUnknown, (void**)&ipUnk );

              hRes = CoMarshalInterface(m_pStream, IID_IUnknown,
                     ipUnk, MSHCTX_INPROC, NULL, MSHLFLAGS_TABLEWEAK);
              if (FAILED(hRes))
              {
                 m_pStream->Release();
                 m_pStream = NULL;
              }
           }
        }
        if (FAILED(hRes))
        {
           // クリティカル セクションをアンロックします。
           m_sec.Unlock();
           return hRes;
        }
     }
     CComPtr<IUnknown> spUnk;
     // ストリームからインターフェイスをアンマーシャリングします。
     hRes = AtlUnmarshalPtr(m_pStream, IID_IUnknown, &spUnk);
     // unlock critical section
     m_sec.Unlock();
     if (FAILED (hRes))
        return hRes;
     // アンマーシャリングしたインターフェイスで QI を呼び出します。
     hRes = spUnk->QueryInterface(riid, ppvObj);
        return hRes;
  }
  LPSTREAM m_pStream; // マーシャリングされたインターフェイスを格納するのに使用する IStream* です。

  void FinalRelease()
  {
     // マーシャリングされたインターフェイスを開放し、ストリームを開放します。
     if (m_pStream)
        AtlFreeMarshalStream (m_pStream);
  }

  CMyComClassFactorySingleton() : m_pObj(NULL), m_pStream(NULL)
  {
  }
};

#define DECLARE_CLASSFACTORY_MYSINGLETON(obj) DECLARE_CLASSFACTORY_EX(CMyComClassFactorySingleton<obj>)
/********** End of singleton code **********/ 
