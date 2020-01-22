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
	\brief CAttributeDlgObj クラスの宣言
	\author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
	\date 2001-09-07
*/
#ifndef __ATTRIBUTEDLGOBJ_H_
#define __ATTRIBUTEDLGOBJ_H_

#include "resource.h"       // メイン シンボル
#include <atlhost.h>

#include <atlapp.h>                             // WTL（atlgdi.h、atlcrack.h）を使うため
#include <atlgdi.h>                             // CDC                 （WinMgrで使用）
#include <atlcrack.h>                           // メッセージ系
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
	\brief 属性表示・編集ダイアログ用 ActiveX コンポーネント

	属性表示・編集に関するものは全てこのコンポーネント内で処理します。
	外部のツールから使用されることを前提に作られています。

	ちなみに、現在使用しているツールは IAttributeTool、IEditTool、ISketchTool です。
	
	\b Example:

	// 属性表示・編集ツールの場合
	IAttributeDlgObjPtr m_ipAttributeDlg;

	m_ipAttributeDlg->Init( hook, TRUE );	

	使用する前に必ず初期化してください。
	EditOperation を属性ダイアログに任せる場合は TRUE にしてください。
	そうでない場合は、FALSE にしてください。

	通常、属性表示・編集ツール以外は何らかの編集の最中に属性の編集が入るはずですので、
	FALSE にし EditOperation の開始・終了は呼び出し元のツールで行ってください。

	m_ipAttributeDlg->DoModeless();

	モードレスダイアログで属性表示・編集ダイアログを表示します。
	属性表示・編集ツール以外はなるべく SetFeature() + DoModal()を使用してください。
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
	 * コンストラクタ
	 */
	CAttributeDlgObj();
	/**
	 * デストラクタ
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

		// ボタンのビットマップ読み込み
		m_hAliasModeBmp  = ::LoadBitmap(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDB_BITMAP_ALIASMODE));
		m_hGridModeBmp   = ::LoadBitmap(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDB_BITMAP_GRIDMODE));
		m_hDBNameModeBmp = ::LoadBitmap(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDB_BITMAP_DBNAMEMODE));
		m_hCommonModeBmp = ::LoadBitmap(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDB_BITMAP_COMMONMODE));

		// 名称系ライブラリ初期化
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

	// 名称系用定義
	static const int NO_NAME_ID = -1; //!< 名称IDが未設定
	static const int NO_NAME_ID_FIELD = -2; //!< NAME_IDフィールドを持たないフィーチャクラス

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
// CAttributeDlgObj メッセージマップ
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
	 * @defgroup group1 IActiveViewEvents インターフェース
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
	 * @defgroup group2 IEditEvents インターフェース
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
	 * @defgroup group3 IAttributeDlgObj インターフェース
	 * @ingroup group0
	 * @{
	 */

	/**
	 * @brief メンバ変数を初期化する
	 *
	 * メンバ変数を初期化します。一番最初に必ず呼んでください。
	 *
	 * @param hook			[in]	IDispatch
	 */
	STDMETHOD(Init)(IDispatch* hook );

	/**
	 * @brief フィーチャをダイアログにセットする
	 *
	 * フィーチャをダイアログにセットします。複数表示させたい場合は、続けて SetItem を呼んでください。
	 *
	 * @param pRow	[in]	ダイアログに表示させたいフィーチャ 
	 * @param pSet	[in]	LQテーブルセット 
	 * @param pGeometry	[in]	LQのジオメトリ
	 * @param IsCreated	[in]	作成するのかどうか 
	 */
	STDMETHOD(SetItem)(_IRow* pRow, _ISet* pSet, IGeometry* pGeometry, VARIANT_BOOL IsCreated );

	/**
	 * @brief モードレスダイアログでセレクション内のフィーチャを表示する
	 * 
	 * 表示対象のフィーチャはセレクションに入っているフィーチャのみです。
	 */
	STDMETHOD(DoModeless)();

	/**
	 * @brief セットされたフィーチャをモーダルダイアログで表示する
	 *
	 * このインターフェースを呼ぶ前に SetItem でフィーチャをセットしておく必要があります。 
	 *
	 * @param IsChanged	[out]	属性変更があったかどうか 
	 */
	STDMETHOD(DoModal)(VARIANT_BOOL* IsChanged);

	/**
	 * @brief ダイアログを閉じるインターフェイス
	 *
	 * 呼び出し元からダイアログを閉じたいときに使用されます
	 */
	STDMETHOD(Close)();

	/**
	 * @brief 方面とレーンの属性をDisableにするかどうかを設定するインターフェイス
	 *
	 * Disableにしたい場合には、呼び出し元で1を設定してください。 
	 *
	 * @param lMode	[in]	DISABLEにするかどうか 
	 */
	STDMETHOD(SetMode)(long lMode);

	/**
	 * @brief 現在ダイアログで表示しているフィーチャを返すインターフェイス
	 *
	 * @param pRow	[out]	選択しているフィーチャ 
	 */
	STDMETHOD(GetActiveItem)(_IRow** pRow);

	/**
	 * @brief 現在のオペレーションモードを返すインターフェイス
	 *
	 * 実質このインターフェイスは使わないだろう 
	 *
	 * @param pVal	[out]	オペレーションモード 
	 */
	STDMETHOD(get_DoOperation)(/*[out, retval]*/ VARIANT_BOOL *pVal);

	/**
	 * @brief オペレーションモードを設定するインターフェイス
	 *
	 * 属性ダイアログ側で保存するか、呼び出しもとのツールで保存するかどうかの設定を行う 
	 * VARIANT_TRUEを設定すれば、属性ダイアログ側で、VARIANT_FALSEであれば呼び出し元でSTOREを行う
	 *
	 * @param newVal	[in]	 属性ダイアログ側で保存するのかどうか
	 */
	STDMETHOD(put_DoOperation)(/*[in]*/ VARIANT_BOOL newVal);

	/**
	 * @brief 現在の属性編集ルールを返すインターフェイス
	 *
	 * 実質このインターフェイスは使わないだろう 
	 *
	 * @param pVal	[out]	属性編集ルール 
	 */
	STDMETHOD(get_EditType)(/*[out, retval]*/  sindyeAttributeEditType *pVal);

	/**
	 *  @brief 属性編集ダイアログで、ルールに従ってダイアログのOKボタンを押せるようにするかどうかを設定する
	 *
	 * この関数は"必ず"SetItemを呼んだ後に呼ぶようにしてください。SetItemで初期化(sindyeApplyRule)してしまっています。
	 * 属性ダイアログはシングルトンのため、他のツールで呼ばれた設定が、そのまま次回起動時に適用されてしまうため
	 * 設定しなおさないといけないため。
	 *
	 * [helpstring("ルール適用")]			sindyeApplyRule				= 0,
	 * [helpstring("ルール不適用編集可")]	sindyeNotApplyRuleEnable	= 1,
	 * [helpstring("ルール不適用編集不可")]	sindyeNotApplyRuleDisable	= 2,
	 *
	 * @param newVal [in] 属性編集ルール 
	 */
	STDMETHOD(put_EditType)(/*[in]*/ sindyeAttributeEditType newVal);

	/**
	 * 使用しないでください。スケッチツールからも呼ばれなくなりました
	 */
	STDMETHOD(put_AutoMode)(VARIANT_BOOL newVal);
	/** @} */

private:
	/**
	 * @brief フィーチャをメンバ変数（m_cFeatureItems）に追加する
	 *
	 * @param ipFeature	[in]	ダイアログに表示したいフィーチャ
	 */
	void AddFeatures( IFeaturePtr ipFeature );

	/**
	 * @brief m_cFeatureItems に格納したフィーチャをツリーに表示する
	 *
	 * @note この関数が呼ばれる前に AddFeatures() によってフィーチャがメンバ変数に格納されている必要があります
	 *
	 * @param cFeatureSet		[in]	CTargetItems
	 *
	 * @retval 正常終了したら TRUE
	 */
	BOOL SetFeatures( CTargetItems cFeatureSet );

	/**
	 * @brief フィーチャをメンバ変数（m_pFeatureArray）に追加する
	 *
	 * @param ipFeature	[in]	ダイアログに表示したいフィーチャ
	 */
	void AddLQs( _IRowPtr ipInfoRow, _ISetPtr ipLQRows, IGeometryPtr ipLQGeom, BOOL bIsCreated );

	/**
	 * @brief m_ipFeatureSet に格納したフィーチャをツリーに表示する
	 *
	 * @note この関数が呼ばれる前に AddLQs() によってフィーチャがメンバ変数に格納されている必要があります
	 *
	 * @param listRowDef		[in]	list<ROWDEF>
	 *
	 * @retval 正常終了したら TRUE
	 */
	BOOL SetLQs( std::list<LQROWDEF>& listRowDef );

	/**
	 * @brief ツリーで選んでいるフィーチャの子ダイアログを作成し、ダイアログに実際に表示する関数
	 *
	 * @retval 正常終了したら TRUE
	 */
	BOOL SetSelectedFeaturesToDlg();

	/**
	 * @brief 子ダイアログのリサイズを行う
	 */
	void ResizeClientDlg();

	/**
	 * @brief フィーチャの保存を行う
	 *
	 * @retval 正常終了したら TRUE
	 */
	BOOL StoreFeature();

	/**
	 * @brief ノード種別の再設定を行う
	 *
	 * @param ipNodeFeature [in] ノード種別を再設定するフィーチャ
	 *
	 * @retval 正常終了したら TRUE
	 */
	bool StoreRoadNord( IFeaturePtr ipNodeFeature );

	/**
	 * @brief リンク列の保存を行う
	 *
	 * @retval 正常終了したら TRUE
	 */
	BOOL LQStoreFeature();

	/**
	 * @brief ダイアログを閉じる
	 *
	 * @retval 正常終了したら TRUE
	 */
	BOOL CloseDlg( BOOL bStore );

	/**
	 * @brief OKボタンの設定
	 *
	 * @note 引数のbEditedはあくまで変更がかかった場合にTRUEを設定してOKボタンを押せるようにする
	 * というものだが、それ以外の条件（変更がかかってなくても新規作成の時には押せなくてはいけないなど）
	 * によって、押せたり押せなかったりする
	 *
	 * @param bEdited [in] TRUEならOKボタンをENABLEにする
	 */
	void SetButton( BOOL bEdited );

	/**
	 * @brief ダイアログに表示しているフィーチャの中に変更があったかどうかを調べる
	 *
	 * @retval 変更があったら TRUE
	 */
	BOOL Changed();

	/**
	 * @brief イベントに接続する
	 *
	 * @note ActiveViewEventsとEditEventsに接続、切断をする関数
	 *
	 * @param bConnect [in] 接続する場合にはTRUE（デフォルト）、切断するときにはFALSEを設定
	 *
	 * @retval 正常終了したら TRUE
	 */
	BOOL ConnectToEvents( BOOL bConnect = TRUE );

	/**
	 * @brief 保存の際にかけるエラーチェック
	 *
	 * @retval エラーがなければ TRUE
	 */
	BOOL ErrorCheck();

	/**
	 * @brief チェック後のデータ更新処理
	 * @note  各属性ダイアログクラス側でデータを更新する必要があるときはここで行う<br>
	 *        更新処理はCExportDlg2を継承している属性ダイアログのみ行われる
	 * @retval true  更新成功
	 * @retval false 更新失敗
	 */
	bool Update();

	/**
	 * @brief 子ダイアログを取得する
	 *
	 * @param strTableName [in] テーブル名
	 *
	 * @retval 子ダイアログ
	 */
	CExportDlg* SetChildDlg(CString strTableName);

	/**
	 * @brief レジストリに属性ダイアログ情報を書き込む
	 */
	void WriteProperty();

	/**
	 * @brief レジストリから属性ダイアログ情報を読み取る
	 */
	void LoadProperty();

	/**
	 * @brief モディファイチェック
	 */
	void FinalModifyCheck();

	/**
	 * @brief ツールチップの作成と変更
	 *
	 * @param nID [in] 関連するコントロールのID
	 */
	void InitToolTip(const int nID);

	/**
	 * @briefフィーチャの属性値にDB的なエラー（SiNDYRule的なエラーは見ない）がないかどうか調べる
	 *
	 * @param pFeatureDef [in] チェックしたいフィーチャ
	 *
	 * @retval エラーがなければ TRUE
	 */
	BOOL CheckValue(CFeatureDef *pFeatureDef);

	/**
	 * @brief リンク列の属性値にDB的なエラー（SiNDYRule的なエラーは見ない）がないかどうか調べる
	 *
	 * @param pLQRowDef [in] チェックしたいリンク列
	 *
	 * @retval エラーがなければ TRUE
	 */
	BOOL LQCheckValue(CLQRowDef *pLQRowDef);

	/**
	 * @brief フィールドの値チェック
	 *
	 * @param ipField [in] チェックするフィールド
	 * @param vaValue [in] チェックする値
	 * @param lOID [in] エラーの時メッセージを出すためのオブジェクトID
	 * @param bstrName [in] エラーの時メッセージを出すためのテーブル名
	 * @param bIsPGDB [in] PGDBかどうか
	 *
	 * @retval エラーがなければ TRUE
	 */
	bool CheckField( IFieldPtr ipField, CComVariant vaValue, long lOID, CComBSTR bstrName, bool bIsPGDB );

	/**
	 * @brief テーブルファインダー取得関数
	 *
	 * @param ipFeature [in] 名称表示しようとしているフィーチャ
	 *
	 * @retval bool テーブルファインダー取得できればtrue
	 */
	bool InitCollection( IFeaturePtr ipFeature );

	/**
	 * @brief フィーチャから名称IDを取得する関数
	 *
	 * @param ipFeature [in] 名称IDを取得したいフィーチャ
	 *
	 * @retval long 名称ID
	 *              NAME_IDフィールドを持たない場合はNO_NAME_ID_FIELD(-2)
	 *              NAME_IDフィールドはあるが、名称IDがまだない場合NO_NAME_ID(-1)
	 */
	long GetNameID( IFeaturePtr ipFeature );

	/**
	 * @brief 属性ダイアログにOfficialNameとAlternativeNameを表示する関数
	 *
	 * @param ipFeature [in] 名称を表示したいフィーチャ
	 *
	 * @retval bool 名称を表示できたらtrue
	 *              表示するフィーチャのワークスペースにOFFICIAL_NAMEテーブルがない場合などはfalse
	 */
	bool DisplayObjectName( IFeaturePtr ipFeature );

	/**
	* @brief フォントを設定する
	* @note レジストリで指定したフォントとサイズを設定します
	* デフォルトでは MS UI ゴシック 14pt が設定されます
	* @param wnd [in] 対象コントロールのウィンドウ
	*/
	void SetLocalFont( CWindow& wnd );


private:
	CArcHelper					m_cArcHelper;		//!< ArcObjects ヘルパークラス
	ISiNDYRulePtr				m_ipRule;			//!< ISiNDYRuleクラス
	DWORD						m_dwViewCookie;		//!< IActiveViewEvents接続用Cookie
	DWORD						m_dwEditCookie;		//!< IEditEvents接続用Cookie
	BOOL						m_bDoOperation;		//!< StartEditOperation, StopEditOperation をするかどうか

	CWinMgr						m_winMgr;			//!< ウィンドウマネージャコントロール
	CFeatureTree				m_cTreeCtrl;		//!< ツリーコントロール
	CSizerBar					m_wndSizerBar;		//!< サイズバーコントロール
	BOOL						m_bIsModal;			//!< モーダルフラグ(TRUE:モーダル, FALSE:モードレス)
	BOOL						m_bIsEnding;		//!< 終了処理フラグ(TRUE:終了処理中, FALSE:その他)

	CTargetItems				m_cFeatureItems;	//!< フィーチャ登録用（登録が済んだら削除される）
	std::list<CFeatureDef>		m_listFeatureDef;	//!< 登録されたフィーチャ格納用クラスのリスト
	std::list<LQROWDEF>			m_listLQ;			//!< フィーチャ登録用（登録が済んだら削除される）
	std::list<CLQRowDef>		m_listLQRowDef;		//!< 登録されたフィーチャ格納用クラスのリスト
	CLoadQueueInfo				m_cLQInfo;			//!< リンク列情報クラス

	IEnvelopePtr				m_ipSelectFeatEnv;	//!< 現在選択中のフィーチャの範囲

	CContainedWindow			m_cTree, m_cBtn1, m_cBtn2, m_cBtnClose, m_cBtnSave,m_cBtnDelWarn,m_cBtnConfirmation; //!< コントロールのハンドル
	
	LONG						m_lFeatureDefIndex;	//!< FeatureTreeでどれが選択されているかのインデックス
	BOOL						m_bIsDBNameMode;	//!< 属性をエイリアスかDBのフィールド名かのフラグ
	BOOL						m_bIsGridMode;		//!< グリッドかコモンかのフラグ
	BOOL						m_bIsPushButton;	//!< グリッドかコモン変更ボタンをクリックしたかどうかのフラグ
	HBITMAP						m_hAliasModeBmp;	//!< エイリアス表示切替用ボタン画像
	HBITMAP						m_hGridModeBmp;		//!< グリッド表示切替用ボタン画像
	HBITMAP						m_hDBNameModeBmp;	//!< DBフィールド名表示切替用ボタン画像
	HBITMAP						m_hCommonModeBmp;	//!< コモン表示切替用ボタン画像
	LONG						m_lRoadTabNumber;	//!< 道路リンクのタブ記憶用数
	LONG						m_lAccessTabNumber;	//!< アクセスポイントのタブ記憶用数
	LONG						m_lHwyNodeTabNumber;	//!< ハイウェイノードのタブ記憶用数
	LONG						m_lDRMATabNumber;	//!< DRMAのタブ記憶用数
	LONG						m_lFirstOID;		//!< 最初にダイアログに入ってきたオブジェクトID
	BOOL						m_bIsFirst;			//!< 2次メッシュを跨いでいる場合などに選択したものをダイアログにだすためのフラグ

	BOOL						m_bIsCancel;		//!< キャンセル操作フラグ(TRUE:キャンセル, FALSE:その他) ※更新が有ったかでは無い点に注意
	BOOL						m_bIsChanged;		//!< フィーチャに変更があるかどうか

	BOOL						m_bClosedDialog;	//!< ダイアログ閉じるフラグ(TRUE:閉じている,FALSE:開いている)
	CRect						m_cDlgRect;			//!< ダイアログの矩形
	
	BOOL						m_bIsSetFeature;	//!< 新たにセットフィーチャされたかどうかのフラグ（TRUE:セットされた,FALSE:セットされ終わった）
	sindyeAttributeEditType		m_sAttributeEditType;	//!< 属性ダイアログの編集タイプ
	CVerifyLog					m_cVerifyLog;		//!< 内容確認ログオブジェクト
	BOOL						m_bIsGridEditMode;	//!< グリッド表示かどうかのフラグ
	bool						m_bIsLineHilight;	//!< ラインをハイライトさせるかどうかのフラグ
	bool						m_bIsVertexHilight;	//!< 構成点をハイライトさせるかどうかのフラグ
	unsigned long				m_lRefreshBuf;		//!< フィーチャの再描画範囲（ [bug 5163] ）
	HWND						m_hCurrentWnd;		//!< 子ダイアログのウィンドウハンドル
	std::map<CString, CExportDlg*>	m_mapDlg;			//!< 子ダイアログのダイアログクラスのマップ
	std::map<CString, HMODULE >		m_mapModule;		//!< 子ダイアログのDLLモジュールのマップ
	CExportDlg*					m_cExportDlg;		//!< 現在表示されている子ダイアログクラス
	CString						m_strInstallPath;	//!< SiNDY-eインストールルートのパス
	BOOL						m_bIsLQAttribute;	//!< LQかどうかのフラグ
	long						m_lMode;			//!< 方面とレーンの属性をDisableにするかどうかのフラグ
	HWND						m_hMessageWND;		//!< ツールチップのハンドル
	HWND						m_hMessageWNDEnglish;	//!< [bug11183]ツールチップのハンドル（タイ語名称の表示に使う）
	std::list<CAdapt<_IRowPtr> > m_lstUpdate;		//!< 変更されたと思うもの（ModifyCheck で使う）
	sindy::CTableFinder			m_cFinder;          //!< 名称表示用テーブルファインダー
	CFont						m_OfficialNameFont;	//!< [bug 11183]名称表示用フォント

};

OBJECT_ENTRY_AUTO(__uuidof(AttributeDlgObj), CAttributeDlgObj)

#endif //__ATTRIBUTEDLGOBJ_H_
