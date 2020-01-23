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
  // �N���X �t�@�N�g���ɁACOM �I�u�W�F�N�g���폜���ꂽ���Ƃ�m�点��ϐ��B
  CMyComObject** m_ppObj;

  STDMETHOD_(ULONG, Release)()
  {
     ULONG l = InternalRelease();
     if (l == 0)
     {
        // NULL ��ݒ肵�āA�N���X �t�@�N�g���ɍ쐬���Ȃ������Ƃ�`���܂��B
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
     // �N���X �t�@�N�g���ŁA�|�C���^ m_ppObj -> CComObject<> �����������܂��B
     p->m_ppObj = pp;
     return hRes;
  }
};

template <class T>
class CMyComClassFactorySingleton : public CComClassFactory
{
public:
  CMyComObject<T>* m_pObj;   // COM �I�u�W�F�N�g�B
  CComAutoCriticalSection m_sec;  // �N���X �t�@�N�g���� CComObjectCached �Ɋi�[����邽�߁A�N���e�B�J�� �Z�N�V�������K�v�ɂȂ�܂��B���ׂẴN���C�A���g�́A���̃N���X �t�@�N�g������������̒ᐅ���|�C���^���擾���܂��B���̃N���X �t�@�N�g�����̃C���X�^���X �f�[�^�́A�K���������Ȃ���΂����܂���B
  // IClassFactory
  STDMETHOD(CreateInstance)(LPUNKNOWN pUnkOuter, REFIID riid,
                            void** ppvObj)
  {
     if (NULL == ppvObj)
        return E_POINTER;
     *ppvObj = NULL;
     // �V���O���g���ł̃A�O���Q�[�V�����̓T�|�[�g����Ă��܂���B
     ATLASSERT(pUnkOuter == NULL);
     if (pUnkOuter != NULL)
        return CLASS_E_NOAGGREGATION;
     HRESULT hRes = E_FAIL;
     // �N���e�B�J�� �Z�N�V���������b�N���܂��B
     m_sec.Lock();
     // COM �I�u�W�F�N�g�𐶐����܂��B
     if (NULL == m_pObj)
     {
        hRes = CMyComObject<T>::CreateInstance(&m_pObj);
        if (SUCCEEDED(hRes))
        {
           // �}�[�V�������O���ꂽ�C���^�[�t�F�C�X���J�����A�X�g���[�����J�����܂��B
           if (m_pStream)
           {
              AtlFreeMarshalStream(m_pStream);
              m_pStream = NULL;
           }
           // �X�g���[���ɑ΂��āA�C���^�[�t�F�C�X���}�[�V�������O���܂��B
           // MSHLFLAGS_TABLEWEAK ���g��Ȃ���΂����Ȃ��̂ŁAAtlMarshalPtrInProc �͎g���܂���B
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
           // �N���e�B�J�� �Z�N�V�������A�����b�N���܂��B
           m_sec.Unlock();
           return hRes;
        }
     }
     CComPtr<IUnknown> spUnk;
     // �X�g���[������C���^�[�t�F�C�X���A���}�[�V�������O���܂��B
     hRes = AtlUnmarshalPtr(m_pStream, IID_IUnknown, &spUnk);
     // unlock critical section
     m_sec.Unlock();
     if (FAILED (hRes))
        return hRes;
     // �A���}�[�V�������O�����C���^�[�t�F�C�X�� QI ���Ăяo���܂��B
     hRes = spUnk->QueryInterface(riid, ppvObj);
        return hRes;
  }
  LPSTREAM m_pStream; // �}�[�V�������O���ꂽ�C���^�[�t�F�C�X���i�[����̂Ɏg�p���� IStream* �ł��B

  void FinalRelease()
  {
     // �}�[�V�������O���ꂽ�C���^�[�t�F�C�X���J�����A�X�g���[�����J�����܂��B
     if (m_pStream)
        AtlFreeMarshalStream (m_pStream);
  }

  CMyComClassFactorySingleton() : m_pObj(NULL), m_pStream(NULL)
  {
  }
};

#define DECLARE_CLASSFACTORY_MYSINGLETON(obj) DECLARE_CLASSFACTORY_EX(CMyComClassFactorySingleton<obj>)
/********** End of singleton code **********/ 
