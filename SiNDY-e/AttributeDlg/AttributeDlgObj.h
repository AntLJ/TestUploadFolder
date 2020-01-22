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

/*! \file AttributeDlgObj.h
	\brief CAttributeDlgObj �N���X�̐錾
	\author INCREMENTP CORP. R&D�Z�p�Z���^�[��S�J���O���[�v �Ð�M�G
	\date 2001-09-07
*/
#ifndef __ATTRIBUTEDLGOBJ_H_
#define __ATTRIBUTEDLGOBJ_H_

#include "resource.h"       // ���C�� �V���{��
#include <atlhost.h>

#include <atlapp.h>                             // WTL�iatlgdi.h�Aatlcrack.h�j���g������
#include <atlgdi.h>                             // CDC                 �iWinMgr�Ŏg�p�j
#include <atlcrack.h>                           // ���b�Z�[�W�n
#include "WinMgr.h"

#include "FeatureTree.h"
#include "VerifyLog.h"
#include "ExportDlg2.h"

#include "TableFinder.h"
#include "name_init.h"

namespace sindy{
	class COfficialNameTable;
	class COfficialNameRow;
	class CAlternativeNameTable;
	class CAlternativeNameRow;
	class CTranslationTable;
	class CTranslationRow;
	class CTableFinder;
	typedef boost::shared_ptr<COfficialNameTable> CSPOfficialNameTable;
	typedef boost::shared_ptr<COfficialNameRow> CSPOfficialNameRow;
	typedef boost::shared_ptr<CAlternativeNameTable> CSPAlternativeNameTable;
	typedef boost::shared_ptr<CAlternativeNameRow> CSPAlternativeNameRow;
	typedef boost::shared_ptr<CTranslationTable> CSPTranslationTable;
	typedef boost::shared_ptr<CTranslationRow> CSPTranslationRow;
}

/*! \class CAttributeDlgObj
	\brief �����\���E�ҏW�_�C�A���O�p ActiveX �R���|�[�l���g

	�����\���E�ҏW�Ɋւ�����̂͑S�Ă��̃R���|�[�l���g���ŏ������܂��B
	�O���̃c�[������g�p����邱�Ƃ�O��ɍ���Ă��܂��B

	���Ȃ݂ɁA���ݎg�p���Ă���c�[���� IAttributeTool�AIEditTool�AISketchTool �ł��B
	
	\b Example:

	// �����\���E�ҏW�c�[���̏ꍇ
	IAttributeDlgObjPtr m_ipAttributeDlg;

	m_ipAttributeDlg->Init( hook, TRUE );	

	�g�p����O�ɕK�����������Ă��������B
	EditOperation �𑮐��_�C�A���O�ɔC����ꍇ�� TRUE �ɂ��Ă��������B
	�����łȂ��ꍇ�́AFALSE �ɂ��Ă��������B

	�ʏ�A�����\���E�ҏW�c�[���ȊO�͉��炩�̕ҏW�̍Œ��ɑ����̕ҏW������͂��ł��̂ŁA
	FALSE �ɂ� EditOperation �̊J�n�E�I���͌Ăяo�����̃c�[���ōs���Ă��������B

	m_ipAttributeDlg->DoModeless();

	���[�h���X�_�C�A���O�ő����\���E�ҏW�_�C�A���O��\�����܂��B
	�����\���E�ҏW�c�[���ȊO�͂Ȃ�ׂ� SetFeature() + DoModal()���g�p���Ă��������B
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

class ATL_NO_VTABLE CAttributeDlgObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAttributeDlgObj, &CLSID_AttributeDlgObj>,
	public IDispatchImpl<IAttributeDialog, &IID_IAttributeDialog, &LIBID_ATTRIBUTEDLGLib>,
	public CAxDialogImpl<CAttributeDlgObj>,
	public IActiveViewEvents,
	public IEditEvents
{
public:
	/**
	 * �R���X�g���N�^
	 */
	CAttributeDlgObj();
	/**
	 * �f�X�g���N�^
	 */
	~CAttributeDlgObj(){}

	HRESULT FinalConstruct()
	{
		m_hCurrentWnd = NULL;
		m_cExportDlg = NULL;
		m_dwViewCookie = 0;
		m_dwEditCookie = 0;
		m_bDoOperation = false;
		m_bIsModal = false;
		m_bIsEnding = false;
		m_lFeatureDefIndex = 0;
		m_bIsDBNameMode = false;
		m_bIsGridMode = false;
		m_bIsPushButton = false;
		m_lRoadTabNumber = 0;
		m_lAccessTabNumber = 0;
		m_lHwyNodeTabNumber = 0;
		m_lDRMATabNumber = 0;
		m_lFirstOID = 0;
		m_bIsFirst = false;
		m_bIsCancel = false;
		m_bIsChanged = false;
		m_bClosedDialog = false;
		m_bIsSetFeature = false;
		m_sAttributeEditType = sindyeApplyRule;
		m_bIsGridEditMode = false;
		m_bIsLineHilight = false;
		m_bIsVertexHilight = false;
		m_lRefreshBuf = 5;
		m_strInstallPath = _T("");
		m_bIsLQAttribute = false;
		m_lMode = 0;
		m_hMessageWND = NULL;
		m_hMessageWNDEnglish = NULL;

		// �{�^���̃r�b�g�}�b�v�ǂݍ���
		m_hAliasModeBmp  = ::LoadBitmap(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDB_BITMAP_ALIASMODE));
		m_hGridModeBmp   = ::LoadBitmap(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDB_BITMAP_GRIDMODE));
		m_hDBNameModeBmp = ::LoadBitmap(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDB_BITMAP_DBNAMEMODE));
		m_hCommonModeBmp = ::LoadBitmap(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDB_BITMAP_COMMONMODE));

		// ���̌n���C�u����������
		init_namelib();

		return CComObjectRootEx<CComSingleThreadModel>::FinalConstruct();
	};
	void FinalRelease()
	{
		::DeleteObject( m_hAliasModeBmp );
		::DeleteObject( m_hGridModeBmp );
		::DeleteObject( m_hDBNameModeBmp );
		::DeleteObject( m_hCommonModeBmp );

		CComObjectRootEx<CComSingleThreadModel>::FinalRelease();
	};

	enum { IDD = IDD_ATTRBASEDLG };

	struct LQROWDEF {
		_IRowPtr			obj;
		IGeometryPtr	shp;
		CString			name;	
		LONG			flg;
		BOOL			deflt;
		std::list<ROWDEF>	lqlist;
	};

	// ���̌n�p��`
	static const int NO_NAME_ID = -1; //!< ����ID�����ݒ�
	static const int NO_NAME_ID_FIELD = -2; //!< NAME_ID�t�B�[���h�������Ȃ��t�B�[�`���N���X

/**
 * @defgroup group0 CAttributeDlgObj
 */
DECLARE_REGISTRY_RESOURCEID(IDR_ATTRIBUTEDLGOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

DECLARE_CLASSFACTORY_MYSINGLETON(CAttributeDlgObj)

BEGIN_COM_MAP(CAttributeDlgObj)
	COM_INTERFACE_ENTRY(IAttributeDialog)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IActiveViewEvents)
	COM_INTERFACE_ENTRY(IEditEvents)
END_COM_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttributeDlgObj ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttributeDlgObj)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_TREESELECTCHANGE, OnChange)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_FEATURECHANGED, OnChangedFeature)
	MESSAGE_HANDLER(WM_CHANGECURRENTFEATURE, OnSetCurrentFeature)
	MESSAGE_HANDLER(WM_CLOSE, OnClose)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDC_BUTTON_CLOSE, OnCloseClicked )
	COMMAND_HANDLER(IDC_BUTTON_ALIASMODE, BN_CLICKED, OnClickedAlias_or_Field)
	COMMAND_HANDLER(IDC_BUTTON_GRIDMODE, BN_CLICKED, OnClickedCommon_Or_Grid)
	COMMAND_HANDLER(IDC_BUTTON_DELETEWARN, BN_CLICKED, OnClickedDeleteWarn)
	COMMAND_HANDLER(IDC_BUTTON_CONFIRMATION, BN_CLICKED, OnClickedConfirmation)
	MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
ALT_MSG_MAP(IDC_FEATTREE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_GRIDMODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_ALIASMODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_CLOSE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDOK)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DELETEWARN)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_CONFIRMATION)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnWinMgr(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
 	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCloseClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnChangedFeature(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedAlias_or_Field(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedCommon_Or_Grid(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnGetDlgCode(INT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedDeleteWarn(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedConfirmation(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSetCurrentFeature(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	/**
	 * @defgroup group1 IActiveViewEvents �C���^�[�t�F�[�X
	 * @ingroup group0
	 * @{
	 */
	STDMETHOD(ContentsChanged)();
	STDMETHOD(ContentsCleared)();
	STDMETHOD(ItemAdded)(VARIANT Item);
	STDMETHOD(ItemDeleted)(VARIANT Item);
	STDMETHOD(ItemReordered)(VARIANT Item, LONG toIndex);
	STDMETHOD(SelectionChanged)();
	STDMETHOD(ViewRefreshed)(IActiveView * View, esriViewDrawPhase phase, VARIANT data, IEnvelope * Envelope);
	STDMETHOD(AfterDraw)(IDisplay * Display, esriViewDrawPhase phase);
	STDMETHOD(AfterItemDraw)(SHORT Index, IDisplay * Display, esriDrawPhase phase);
	STDMETHOD(FocusMapChanged)();
	STDMETHOD(SpatialReferenceChanged)();
	/** @} */

	/**
	 * @defgroup group2 IEditEvents �C���^�[�t�F�[�X
	 * @ingroup group0
	 * @{
	 */
	STDMETHOD(OnSelectionChanged)();
	STDMETHOD(OnCurrentLayerChanged)();
	STDMETHOD(OnCurrentTaskChanged)();
	STDMETHOD(OnSketchModified)();
	STDMETHOD(OnSketchFinished)();
	STDMETHOD(AfterDrawSketch)(IDisplay * pDpy);
	STDMETHOD(OnStartEditing)();
	STDMETHOD(OnStopEditing)(VARIANT_BOOL Save);
	STDMETHOD(OnConflictsDetected)();
	STDMETHOD(OnUndo)();
	STDMETHOD(OnRedo)();
	STDMETHOD(OnCreateFeature)(IObject * obj);
	STDMETHOD(OnChangeFeature)(IObject * obj);
	STDMETHOD(OnDeleteFeature)(IObject * obj);
	/** @} */

	/**
	 * @defgroup group3 IAttributeDlgObj �C���^�[�t�F�[�X
	 * @ingroup group0
	 * @{
	 */

	/**
	 * @brief �����o�ϐ�������������
	 *
	 * �����o�ϐ������������܂��B��ԍŏ��ɕK���Ă�ł��������B
	 *
	 * @param hook			[in]	IDispatch
	 */
	STDMETHOD(Init)(IDispatch* hook );

	/**
	 * @brief �t�B�[�`�����_�C�A���O�ɃZ�b�g����
	 *
	 * �t�B�[�`�����_�C�A���O�ɃZ�b�g���܂��B�����\�����������ꍇ�́A������ SetItem ���Ă�ł��������B
	 *
	 * @param pRow	[in]	�_�C�A���O�ɕ\�����������t�B�[�`�� 
	 * @param pSet	[in]	LQ�e�[�u���Z�b�g 
	 * @param pGeometry	[in]	LQ�̃W�I���g��
	 * @param IsCreated	[in]	�쐬����̂��ǂ��� 
	 */
	STDMETHOD(SetItem)(_IRow* pRow, _ISet* pSet, IGeometry* pGeometry, VARIANT_BOOL IsCreated );

	/**
	 * @brief ���[�h���X�_�C�A���O�ŃZ���N�V�������̃t�B�[�`����\������
	 * 
	 * �\���Ώۂ̃t�B�[�`���̓Z���N�V�����ɓ����Ă���t�B�[�`���݂̂ł��B
	 */
	STDMETHOD(DoModeless)();

	/**
	 * @brief �Z�b�g���ꂽ�t�B�[�`�������[�_���_�C�A���O�ŕ\������
	 *
	 * ���̃C���^�[�t�F�[�X���ĂԑO�� SetItem �Ńt�B�[�`�����Z�b�g���Ă����K�v������܂��B 
	 *
	 * @param IsChanged	[out]	�����ύX�����������ǂ��� 
	 */
	STDMETHOD(DoModal)(VARIANT_BOOL* IsChanged);

	/**
	 * @brief �_�C�A���O�����C���^�[�t�F�C�X
	 *
	 * �Ăяo��������_�C�A���O��������Ƃ��Ɏg�p����܂�
	 */
	STDMETHOD(Close)();

	/**
	 * @brief ���ʂƃ��[���̑�����Disable�ɂ��邩�ǂ�����ݒ肷��C���^�[�t�F�C�X
	 *
	 * Disable�ɂ������ꍇ�ɂ́A�Ăяo������1��ݒ肵�Ă��������B 
	 *
	 * @param lMode	[in]	DISABLE�ɂ��邩�ǂ��� 
	 */
	STDMETHOD(SetMode)(long lMode);

	/**
	 * @brief ���݃_�C�A���O�ŕ\�����Ă���t�B�[�`����Ԃ��C���^�[�t�F�C�X
	 *
	 * @param pRow	[out]	�I�����Ă���t�B�[�`�� 
	 */
	STDMETHOD(GetActiveItem)(_IRow** pRow);

	/**
	 * @brief ���݂̃I�y���[�V�������[�h��Ԃ��C���^�[�t�F�C�X
	 *
	 * �������̃C���^�[�t�F�C�X�͎g��Ȃ����낤 
	 *
	 * @param pVal	[out]	�I�y���[�V�������[�h 
	 */
	STDMETHOD(get_DoOperation)(/*[out, retval]*/ VARIANT_BOOL *pVal);

	/**
	 * @brief �I�y���[�V�������[�h��ݒ肷��C���^�[�t�F�C�X
	 *
	 * �����_�C�A���O���ŕۑ����邩�A�Ăяo�����Ƃ̃c�[���ŕۑ����邩�ǂ����̐ݒ���s�� 
	 * VARIANT_TRUE��ݒ肷��΁A�����_�C�A���O���ŁAVARIANT_FALSE�ł���ΌĂяo������STORE���s��
	 *
	 * @param newVal	[in]	 �����_�C�A���O���ŕۑ�����̂��ǂ���
	 */
	STDMETHOD(put_DoOperation)(/*[in]*/ VARIANT_BOOL newVal);

	/**
	 * @brief ���݂̑����ҏW���[����Ԃ��C���^�[�t�F�C�X
	 *
	 * �������̃C���^�[�t�F�C�X�͎g��Ȃ����낤 
	 *
	 * @param pVal	[out]	�����ҏW���[�� 
	 */
	STDMETHOD(get_EditType)(/*[out, retval]*/  sindyeAttributeEditType *pVal);

	/**
	 *  @brief �����ҏW�_�C�A���O�ŁA���[���ɏ]���ă_�C�A���O��OK�{�^����������悤�ɂ��邩�ǂ�����ݒ肷��
	 *
	 * ���̊֐���"�K��"SetItem���Ă񂾌�ɌĂԂ悤�ɂ��Ă��������BSetItem�ŏ�����(sindyeApplyRule)���Ă��܂��Ă��܂��B
	 * �����_�C�A���O�̓V���O���g���̂��߁A���̃c�[���ŌĂ΂ꂽ�ݒ肪�A���̂܂܎���N�����ɓK�p����Ă��܂�����
	 * �ݒ肵�Ȃ����Ȃ��Ƃ����Ȃ����߁B
	 *
	 * [helpstring("���[���K�p")]			sindyeApplyRule				= 0,
	 * [helpstring("���[���s�K�p�ҏW��")]	sindyeNotApplyRuleEnable	= 1,
	 * [helpstring("���[���s�K�p�ҏW�s��")]	sindyeNotApplyRuleDisable	= 2,
	 *
	 * @param newVal [in] �����ҏW���[�� 
	 */
	STDMETHOD(put_EditType)(/*[in]*/ sindyeAttributeEditType newVal);

	/**
	 * �g�p���Ȃ��ł��������B�X�P�b�`�c�[��������Ă΂�Ȃ��Ȃ�܂���
	 */
	STDMETHOD(put_AutoMode)(VARIANT_BOOL newVal);
	/** @} */

private:
	/**
	 * @brief �t�B�[�`���������o�ϐ��im_cFeatureItems�j�ɒǉ�����
	 *
	 * @param ipFeature	[in]	�_�C�A���O�ɕ\���������t�B�[�`��
	 */
	void AddFeatures( IFeaturePtr ipFeature );

	/**
	 * @brief m_cFeatureItems �Ɋi�[�����t�B�[�`�����c���[�ɕ\������
	 *
	 * @note ���̊֐����Ă΂��O�� AddFeatures() �ɂ���ăt�B�[�`���������o�ϐ��Ɋi�[����Ă���K�v������܂�
	 *
	 * @param cFeatureSet		[in]	CTargetItems
	 *
	 * @retval ����I�������� TRUE
	 */
	BOOL SetFeatures( CTargetItems cFeatureSet );

	/**
	 * @brief �t�B�[�`���������o�ϐ��im_pFeatureArray�j�ɒǉ�����
	 *
	 * @param ipFeature	[in]	�_�C�A���O�ɕ\���������t�B�[�`��
	 */
	void AddLQs( _IRowPtr ipInfoRow, _ISetPtr ipLQRows, IGeometryPtr ipLQGeom, BOOL bIsCreated );

	/**
	 * @brief m_ipFeatureSet �Ɋi�[�����t�B�[�`�����c���[�ɕ\������
	 *
	 * @note ���̊֐����Ă΂��O�� AddLQs() �ɂ���ăt�B�[�`���������o�ϐ��Ɋi�[����Ă���K�v������܂�
	 *
	 * @param listRowDef		[in]	list<ROWDEF>
	 *
	 * @retval ����I�������� TRUE
	 */
	BOOL SetLQs( std::list<LQROWDEF>& listRowDef );

	/**
	 * @brief �c���[�őI��ł���t�B�[�`���̎q�_�C�A���O���쐬���A�_�C�A���O�Ɏ��ۂɕ\������֐�
	 *
	 * @retval ����I�������� TRUE
	 */
	BOOL SetSelectedFeaturesToDlg();

	/**
	 * @brief �q�_�C�A���O�̃��T�C�Y���s��
	 */
	void ResizeClientDlg();

	/**
	 * @brief �t�B�[�`���̕ۑ����s��
	 *
	 * @retval ����I�������� TRUE
	 */
	BOOL StoreFeature();

	/**
	 * @brief �m�[�h��ʂ̍Đݒ���s��
	 *
	 * @param ipNodeFeature [in] �m�[�h��ʂ��Đݒ肷��t�B�[�`��
	 *
	 * @retval ����I�������� TRUE
	 */
	bool StoreRoadNord( IFeaturePtr ipNodeFeature );

	/**
	 * @brief �����N��̕ۑ����s��
	 *
	 * @retval ����I�������� TRUE
	 */
	BOOL LQStoreFeature();

	/**
	 * @brief �_�C�A���O�����
	 *
	 * @retval ����I�������� TRUE
	 */
	BOOL CloseDlg( BOOL bStore );

	/**
	 * @brief OK�{�^���̐ݒ�
	 *
	 * @note ������bEdited�͂����܂ŕύX�����������ꍇ��TRUE��ݒ肵��OK�{�^����������悤�ɂ���
	 * �Ƃ������̂����A����ȊO�̏����i�ύX���������ĂȂ��Ă��V�K�쐬�̎��ɂ͉����Ȃ��Ă͂����Ȃ��Ȃǁj
	 * �ɂ���āA�������艟���Ȃ������肷��
	 *
	 * @param bEdited [in] TRUE�Ȃ�OK�{�^����ENABLE�ɂ���
	 */
	void SetButton( BOOL bEdited );

	/**
	 * @brief �_�C�A���O�ɕ\�����Ă���t�B�[�`���̒��ɕύX�����������ǂ����𒲂ׂ�
	 *
	 * @retval �ύX���������� TRUE
	 */
	BOOL Changed();

	/**
	 * @brief �C�x���g�ɐڑ�����
	 *
	 * @note ActiveViewEvents��EditEvents�ɐڑ��A�ؒf������֐�
	 *
	 * @param bConnect [in] �ڑ�����ꍇ�ɂ�TRUE�i�f�t�H���g�j�A�ؒf����Ƃ��ɂ�FALSE��ݒ�
	 *
	 * @retval ����I�������� TRUE
	 */
	BOOL ConnectToEvents( BOOL bConnect = TRUE );

	/**
	 * @brief �ۑ��̍ۂɂ�����G���[�`�F�b�N
	 *
	 * @retval �G���[���Ȃ���� TRUE
	 */
	BOOL ErrorCheck();

	/**
	 * @brief �`�F�b�N��̃f�[�^�X�V����
	 * @note  �e�����_�C�A���O�N���X���Ńf�[�^���X�V����K�v������Ƃ��͂����ōs��<br>
	 *        �X�V������CExportDlg2���p�����Ă��鑮���_�C�A���O�̂ݍs����
	 * @retval true  �X�V����
	 * @retval false �X�V���s
	 */
	bool Update();

	/**
	 * @brief �q�_�C�A���O���擾����
	 *
	 * @param strTableName [in] �e�[�u����
	 *
	 * @retval �q�_�C�A���O
	 */
	CExportDlg* SetChildDlg(CString strTableName);

	/**
	 * @brief ���W�X�g���ɑ����_�C�A���O������������
	 */
	void WriteProperty();

	/**
	 * @brief ���W�X�g�����瑮���_�C�A���O����ǂݎ��
	 */
	void LoadProperty();

	/**
	 * @brief ���f�B�t�@�C�`�F�b�N
	 */
	void FinalModifyCheck();

	/**
	 * @brief �c�[���`�b�v�̍쐬�ƕύX
	 *
	 * @param nID [in] �֘A����R���g���[����ID
	 */
	void InitToolTip(const int nID);

	/**
	 * @brief�t�B�[�`���̑����l��DB�I�ȃG���[�iSiNDYRule�I�ȃG���[�͌��Ȃ��j���Ȃ����ǂ������ׂ�
	 *
	 * @param pFeatureDef [in] �`�F�b�N�������t�B�[�`��
	 *
	 * @retval �G���[���Ȃ���� TRUE
	 */
	BOOL CheckValue(CFeatureDef *pFeatureDef);

	/**
	 * @brief �����N��̑����l��DB�I�ȃG���[�iSiNDYRule�I�ȃG���[�͌��Ȃ��j���Ȃ����ǂ������ׂ�
	 *
	 * @param pLQRowDef [in] �`�F�b�N�����������N��
	 *
	 * @retval �G���[���Ȃ���� TRUE
	 */
	BOOL LQCheckValue(CLQRowDef *pLQRowDef);

	/**
	 * @brief �t�B�[���h�̒l�`�F�b�N
	 *
	 * @param ipField [in] �`�F�b�N����t�B�[���h
	 * @param vaValue [in] �`�F�b�N����l
	 * @param lOID [in] �G���[�̎����b�Z�[�W���o�����߂̃I�u�W�F�N�gID
	 * @param bstrName [in] �G���[�̎����b�Z�[�W���o�����߂̃e�[�u����
	 * @param bIsPGDB [in] PGDB���ǂ���
	 *
	 * @retval �G���[���Ȃ���� TRUE
	 */
	bool CheckField( IFieldPtr ipField, CComVariant vaValue, long lOID, CComBSTR bstrName, bool bIsPGDB );

	/**
	 * @brief �e�[�u���t�@�C���_�[�擾�֐�
	 *
	 * @param ipFeature [in] ���̕\�����悤�Ƃ��Ă���t�B�[�`��
	 *
	 * @retval bool �e�[�u���t�@�C���_�[�擾�ł����true
	 */
	bool InitCollection( IFeaturePtr ipFeature );

	/**
	 * @brief �t�B�[�`�����疼��ID���擾����֐�
	 *
	 * @param ipFeature [in] ����ID���擾�������t�B�[�`��
	 *
	 * @retval long ����ID
	 *              NAME_ID�t�B�[���h�������Ȃ��ꍇ��NO_NAME_ID_FIELD(-2)
	 *              NAME_ID�t�B�[���h�͂��邪�A����ID���܂��Ȃ��ꍇNO_NAME_ID(-1)
	 */
	long GetNameID( IFeaturePtr ipFeature );

	/**
	 * @brief �����_�C�A���O��OfficialName��AlternativeName��\������֐�
	 *
	 * @param ipFeature [in] ���̂�\���������t�B�[�`��
	 *
	 * @retval bool ���̂�\���ł�����true
	 *              �\������t�B�[�`���̃��[�N�X�y�[�X��OFFICIAL_NAME�e�[�u�����Ȃ��ꍇ�Ȃǂ�false
	 */
	bool DisplayObjectName( IFeaturePtr ipFeature );

	/**
	* @brief �t�H���g��ݒ肷��
	* @note ���W�X�g���Ŏw�肵���t�H���g�ƃT�C�Y��ݒ肵�܂�
	* �f�t�H���g�ł� MS UI �S�V�b�N 14pt ���ݒ肳��܂�
	* @param wnd [in] �ΏۃR���g���[���̃E�B���h�E
	*/
	void SetLocalFont( CWindow& wnd );


private:
	CArcHelper					m_cArcHelper;		//!< ArcObjects �w���p�[�N���X
	ISiNDYRulePtr				m_ipRule;			//!< ISiNDYRule�N���X
	DWORD						m_dwViewCookie;		//!< IActiveViewEvents�ڑ��pCookie
	DWORD						m_dwEditCookie;		//!< IEditEvents�ڑ��pCookie
	BOOL						m_bDoOperation;		//!< StartEditOperation, StopEditOperation �����邩�ǂ���

	CWinMgr						m_winMgr;			//!< �E�B���h�E�}�l�[�W���R���g���[��
	CFeatureTree				m_cTreeCtrl;		//!< �c���[�R���g���[��
	CSizerBar					m_wndSizerBar;		//!< �T�C�Y�o�[�R���g���[��
	BOOL						m_bIsModal;			//!< ���[�_���t���O(TRUE:���[�_��, FALSE:���[�h���X)
	BOOL						m_bIsEnding;		//!< �I�������t���O(TRUE:�I��������, FALSE:���̑�)

	CTargetItems				m_cFeatureItems;	//!< �t�B�[�`���o�^�p�i�o�^���ς񂾂�폜�����j
	std::list<CFeatureDef>		m_listFeatureDef;	//!< �o�^���ꂽ�t�B�[�`���i�[�p�N���X�̃��X�g
	std::list<LQROWDEF>			m_listLQ;			//!< �t�B�[�`���o�^�p�i�o�^���ς񂾂�폜�����j
	std::list<CLQRowDef>		m_listLQRowDef;		//!< �o�^���ꂽ�t�B�[�`���i�[�p�N���X�̃��X�g
	CLoadQueueInfo				m_cLQInfo;			//!< �����N����N���X

	IEnvelopePtr				m_ipSelectFeatEnv;	//!< ���ݑI�𒆂̃t�B�[�`���͈̔�

	CContainedWindow			m_cTree, m_cBtn1, m_cBtn2, m_cBtnClose, m_cBtnSave,m_cBtnDelWarn,m_cBtnConfirmation; //!< �R���g���[���̃n���h��
	
	LONG						m_lFeatureDefIndex;	//!< FeatureTree�łǂꂪ�I������Ă��邩�̃C���f�b�N�X
	BOOL						m_bIsDBNameMode;	//!< �������G�C���A�X��DB�̃t�B�[���h�����̃t���O
	BOOL						m_bIsGridMode;		//!< �O���b�h���R�������̃t���O
	BOOL						m_bIsPushButton;	//!< �O���b�h���R�����ύX�{�^�����N���b�N�������ǂ����̃t���O
	HBITMAP						m_hAliasModeBmp;	//!< �G�C���A�X�\���ؑ֗p�{�^���摜
	HBITMAP						m_hGridModeBmp;		//!< �O���b�h�\���ؑ֗p�{�^���摜
	HBITMAP						m_hDBNameModeBmp;	//!< DB�t�B�[���h���\���ؑ֗p�{�^���摜
	HBITMAP						m_hCommonModeBmp;	//!< �R�����\���ؑ֗p�{�^���摜
	LONG						m_lRoadTabNumber;	//!< ���H�����N�̃^�u�L���p��
	LONG						m_lAccessTabNumber;	//!< �A�N�Z�X�|�C���g�̃^�u�L���p��
	LONG						m_lHwyNodeTabNumber;	//!< �n�C�E�F�C�m�[�h�̃^�u�L���p��
	LONG						m_lDRMATabNumber;	//!< DRMA�̃^�u�L���p��
	LONG						m_lFirstOID;		//!< �ŏ��Ƀ_�C�A���O�ɓ����Ă����I�u�W�F�N�gID
	BOOL						m_bIsFirst;			//!< 2�����b�V�����ׂ��ł���ꍇ�ȂǂɑI���������̂��_�C�A���O�ɂ������߂̃t���O

	BOOL						m_bIsCancel;		//!< �L�����Z������t���O(TRUE:�L�����Z��, FALSE:���̑�) ���X�V���L�������ł͖����_�ɒ���
	BOOL						m_bIsChanged;		//!< �t�B�[�`���ɕύX�����邩�ǂ���

	BOOL						m_bClosedDialog;	//!< �_�C�A���O����t���O(TRUE:���Ă���,FALSE:�J���Ă���)
	CRect						m_cDlgRect;			//!< �_�C�A���O�̋�`
	
	BOOL						m_bIsSetFeature;	//!< �V���ɃZ�b�g�t�B�[�`�����ꂽ���ǂ����̃t���O�iTRUE:�Z�b�g���ꂽ,FALSE:�Z�b�g����I������j
	sindyeAttributeEditType		m_sAttributeEditType;	//!< �����_�C�A���O�̕ҏW�^�C�v
	CVerifyLog					m_cVerifyLog;		//!< ���e�m�F���O�I�u�W�F�N�g
	BOOL						m_bIsGridEditMode;	//!< �O���b�h�\�����ǂ����̃t���O
	bool						m_bIsLineHilight;	//!< ���C�����n�C���C�g�����邩�ǂ����̃t���O
	bool						m_bIsVertexHilight;	//!< �\���_���n�C���C�g�����邩�ǂ����̃t���O
	unsigned long				m_lRefreshBuf;		//!< �t�B�[�`���̍ĕ`��͈́i [bug 5163] �j
	HWND						m_hCurrentWnd;		//!< �q�_�C�A���O�̃E�B���h�E�n���h��
	std::map<CString, CExportDlg*>	m_mapDlg;			//!< �q�_�C�A���O�̃_�C�A���O�N���X�̃}�b�v
	std::map<CString, HMODULE >		m_mapModule;		//!< �q�_�C�A���O��DLL���W���[���̃}�b�v
	CExportDlg*					m_cExportDlg;		//!< ���ݕ\������Ă���q�_�C�A���O�N���X
	CString						m_strInstallPath;	//!< SiNDY-e�C���X�g�[�����[�g�̃p�X
	BOOL						m_bIsLQAttribute;	//!< LQ���ǂ����̃t���O
	long						m_lMode;			//!< ���ʂƃ��[���̑�����Disable�ɂ��邩�ǂ����̃t���O
	HWND						m_hMessageWND;		//!< �c�[���`�b�v�̃n���h��
	HWND						m_hMessageWNDEnglish;	//!< [bug11183]�c�[���`�b�v�̃n���h���i�^�C�ꖼ�̂̕\���Ɏg���j
	std::list<CAdapt<_IRowPtr> > m_lstUpdate;		//!< �ύX���ꂽ�Ǝv�����́iModifyCheck �Ŏg���j
	sindy::CTableFinder			m_cFinder;          //!< ���̕\���p�e�[�u���t�@�C���_�[
	CFont						m_OfficialNameFont;	//!< [bug 11183]���̕\���p�t�H���g

};

OBJECT_ENTRY_AUTO(__uuidof(AttributeDlgObj), CAttributeDlgObj)

#endif //__ATTRIBUTEDLGOBJ_H_
