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

// AttributeToolObj.h : CAttributeToolObj �̐錾

#ifndef __ATTRIBUTETOOLOBJ_H_
#define __ATTRIBUTETOOLOBJ_H_

#include "resource.h"       // ���C�� �V���{��
#include "stdafx.h"
#include "ArcHelperEx/Feedback.h"
#include "ArcHelperEx/SketchFeedback.h"
#include <WinLib/AdviseAdapter.h>
#include "ArcMapUIBase/ToolKeys.h"
#include "../../LIB/ArcMapUIBase/ToolKeys.h"

/////////////////////////////////////////////////////////////////////////////
// CAttributeToolObj
class ATL_NO_VTABLE CAttributeToolObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAttributeToolObj, &CLSID_AttributeToolObj>,
	public IAttributeToolObj,
	public _ICommand,
	public ITool,
	public IActiveViewEvents,
	public IDocumentEvents,
	public ICommandSubType,
	public CToolKeys
{
public:
	CAttributeToolObj()
	{
		// �}�E�X�|�C���^�ǂݍ���
		m_cursor		= static_cast<HICON>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDC_CURSOR_IDENTIFY), IMAGE_CURSOR, 32, 32, LR_DEFAULTSIZE|LR_SHARED));
		m_curMove		= static_cast<HICON>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDC_CURSOR_IDENTIFYMOVE), IMAGE_CURSOR, 32, 32, LR_DEFAULTSIZE|LR_SHARED));
		m_curFreeSel	= static_cast<HICON>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDC_CURSOR_FREESELECT), IMAGE_CURSOR, 32, 32, LR_DEFAULTSIZE|LR_SHARED));
		m_LQcursor		= static_cast<HICON>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDC_CURSOR_LQ), IMAGE_CURSOR, 32, 32, LR_DEFAULTSIZE|LR_SHARED));
		m_LQcurMove		= static_cast<HICON>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDC_CURSOR_LQMOVE), IMAGE_CURSOR, 32, 32, LR_DEFAULTSIZE|LR_SHARED));
		m_LQcurFreeSel	= static_cast<HICON>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDC_CURSOR_LQFREESELECT), IMAGE_CURSOR, 32, 32, LR_DEFAULTSIZE|LR_SHARED));
		m_pCurPointer = &m_cursor;

		// �I�u�W�F�N�g�r�b�g�}�b�v�ǂݍ���
		m_bitmap	= static_cast<HBITMAP>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_IDENTIFY), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR));
		m_LQbitmap	= static_cast<HBITMAP>(::LoadImage(_Module.m_hInst, MAKEINTRESOURCE(IDB_BITMAP_LQ), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR));

		// ������
		m_bIsMouseDown = false;
		m_bIsRightDown = false;
		m_ipMouseDownPoint = NULL;
		m_ipFeedbackEnv = NULL;
		m_bDrawingSelectArea = false;
		m_lMouseDownX = 0;
		m_lMouseDownY = 0;
		m_bFreeSelectMode = false;
		m_bShowContextMenu = false;
		m_lShift = 0;
		m_lSearchItemNum = 0;

		// �v���p�e�B������
		m_bPerfectSelect = TRUE;
		m_bIsSelectedLastSequence = FALSE;
	}
	// �f�X�g���N�^
	~CAttributeToolObj()
	{
		// �}�E�X�|�C���^�y�уr�b�g�}�b�v�J��
		// �J�[�\���͊J�����Ă͂����Ȃ��B�V�X�e�����ł���Ȃ��Ȃ����珟��ɊJ�����Ă����̂�
		DeleteObject(m_bitmap);
		DeleteObject(m_LQbitmap);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_ATTRIBUTETOOLOBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolObj COM�}�b�v��`
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_COM_MAP(CAttributeToolObj)
	COM_INTERFACE_ENTRY(IAttributeToolObj)
	COM_INTERFACE_ENTRY(_ICommand)
	COM_INTERFACE_ENTRY(ITool)
	COM_INTERFACE_ENTRY(IToolKeys)
	COM_INTERFACE_ENTRY(IActiveViewEvents)
	COM_INTERFACE_ENTRY(IDocumentEvents)
	COM_INTERFACE_ENTRY(ICommandSubType)
	COM_INTERFACE_ENTRY(IToolKeys)
END_COM_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolObj �v���p�e�B�}�b�v��`
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_PROP_MAP(CAttributeToolObj)
	PROP_ENTRY("SelectLastSequence", 1, CLSID_AttributeToolPropLQ)
	PROP_ENTRY("PerfectSelect", 2, CLSID_AttributeToolPropLQ)
END_PROP_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolObj �J�e�S���}�b�v��`
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_CATEGORY_MAP(CAttributeToolObj)
	IMPLEMENTED_CATEGORY(__uuidof(CATID_MxCommands))
END_CATEGORY_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// IActiveViewEvents �C���^�[�t�F�[�X��`
//
/////////////////////////////////////////////////////////////////////////////
public:
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
	
/////////////////////////////////////////////////////////////////////////////
//
// ICommand�C���^�[�t�F�[�X��`
//
/////////////////////////////////////////////////////////////////////////////
public:
	STDMETHOD(get_Enabled)(VARIANT_BOOL * Enabled);
	STDMETHOD(get_Checked)(VARIANT_BOOL * Checked);
	STDMETHOD(get_Name)(BSTR * Name);
	STDMETHOD(get_Caption)(BSTR * Caption);
	STDMETHOD(get_Tooltip)(BSTR * Tooltip);
	STDMETHOD(get_Message)(BSTR * Message);
	STDMETHOD(get_HelpFile)(BSTR * HelpFile);
	STDMETHOD(get_HelpContextID)(LONG * helpID);
	STDMETHOD(get_Bitmap)(OLE_HANDLE * Bitmap);
	STDMETHOD(get_Category)(BSTR * categoryName);
	STDMETHOD(OnCreate)(IDispatch * hook);
	STDMETHOD(OnClick)();
/////////////////////////////////////////////////////////////////////////////
//
// ICommandSubType�C���^�[�t�F�[�X��`
//
/////////////////////////////////////////////////////////////////////////////
public:
	STDMETHOD(SetSubType)(LONG SubType);
	STDMETHOD(GetCount)(LONG * Count);
/////////////////////////////////////////////////////////////////////////////
//
// ITool�C���^�[�t�F�[�X��`
//
/////////////////////////////////////////////////////////////////////////////
public:
	STDMETHOD(get_Cursor)(OLE_HANDLE * Cursor);
	STDMETHOD(OnMouseDown)(LONG Button, LONG Shift, LONG X, LONG Y);
	STDMETHOD(OnMouseMove)(LONG Button, LONG Shift, LONG X, LONG Y);
	STDMETHOD(OnMouseUp)(LONG Button, LONG Shift, LONG X, LONG Y);
	STDMETHOD(OnDblClick)();
	STDMETHOD(OnKeyDown)(LONG keyCode, LONG Shift);
	STDMETHOD(OnKeyUp)(LONG keyCode, LONG Shift);
	STDMETHOD(OnContextMenu)(LONG X, LONG Y, VARIANT_BOOL * handled);
	STDMETHOD(Refresh)(OLE_HANDLE hDC);
	STDMETHOD(Deactivate)(VARIANT_BOOL * complete);
/////////////////////////////////////////////////////////////////////////////
//
// IDocumentEvents�C���^�[�t�F�[�X��`
//
/////////////////////////////////////////////////////////////////////////////
public:
	STDMETHOD(ActiveViewChanged)();
	STDMETHOD(MapsChanged)();
	STDMETHOD(NewDocument)();
	STDMETHOD(OpenDocument)();
	STDMETHOD(BeforeCloseDocument)(VARIANT_BOOL * abortClose);
	STDMETHOD(CloseDocument)();
/////////////////////////////////////////////////////////////////////////////
//
// IAttributeToolObj �C���^�[�t�F�[�X��`
//
/////////////////////////////////////////////////////////////////////////////
public:
	STDMETHOD(get_SelectLastSequence)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_SelectLastSequence)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_PerfectSelect)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_PerfectSelect)(/*[in]*/ VARIANT_BOOL newVal);

/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolObj �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	/**
	 * @brief �t�B�[�`���I���p��OnMouseUp�֐�\n
	 * ���̒��Ŕ͈͑I������������t�B�[�`���������o���AAttributeDlg�ɓn���āA�_�C�A���O���o���܂�
	 *
	 * @param Button	[in]	LONG �E�N���b�N(1)or���N���b�N(2)
	 * @param Shift		[in]	LONG Shift�����������ǂ���
	 * @param X			[in]	LONG �N���b�N����X���W
	 * @param Y			[in]	LONG �N���b�N����Y���W
	 */
	void OnAttrMouseUp(LONG Button, LONG Shift, LONG X, LONG Y);

	/**
	 * @brief �����N��I���p��OnMouseUp�֐�\n
	 * ���̒��Ŕ͈͑I�����������烊���N��������o���AAttributeDlg�ɓn���āA�_�C�A���O���o���܂�
	 * �����N��͊�{�I�ɁA�Z�`�I�����A���͈̔͂ɓ����Ă��铹�H�����N��T���āA���̓��H�����N�ɂ��Ă���
	 * �����N�񂪑I������܂��B
	 *
	 * @param Button	[in]	LONG �E�N���b�N(1)or���N���b�N(2)
	 * @param Shift		[in]	LONG Shift�����������ǂ���
	 * @param X			[in]	LONG �N���b�N����X���W
	 * @param Y			[in]	LONG �N���b�N����Y���W
	 */
	void OnLQMouseUp(LONG Button, LONG Shift, LONG X, LONG Y);

	/** 
	 * @brief ���������ׂ����C���̈ꗗ���擾����\n
	 * �����N�񑮐��ҏW�c�[���Ƌ��ʂ䂦�ɓ��H�����N���C���݂̂������Ă���ꍇ�Ƃŏꍇ�킯������Ă��܂�
	 *
	 * @note ���������ׂ����C���Ƃ́A�t�B�[�`�����C���ł���ATOC�Ń��C�����I������Ă��Ȃ���΁A
	 * �\������Ă��邷�ׂẴ��C���ƂȂ�A���C�����I������Ă���΁A���̃��C���A�܂��O���[�v���C�����I�������
	 * ����΁A���̃O���[�v���C���ȉ��̕\������Ă��郌�C�����ׂĂƂȂ�܂�
	 *
	 * @param bIsRoadLinkOnly	[in]	�����N��I�����t�B�[�`���I����
	 *
	 * @return ���������ׂ����C���̃��X�g
	 */
	std::list< CAdapt<ILayerPtr> > GetSearchLayer(bool bIsRoadLinkOnly);

	/** 
	 * @brief �I�����C���̈ꗗ���擾����\n
	 * �����N�񑮐��ҏW�c�[���Ƌ��ʂ䂦�ɓ��H�����N���C���݂̂������Ă���ꍇ�Ƃŏꍇ�킯������Ă��܂�
	 *
	 * @note GetSearchLayer����̂݌Ă΂��֐��ł���A�I�����Ă��郌�C���A�܂��̓O���[�v���C�����烌�C��
	 * �̃��X�g��Ԃ��֐��ł��B
	 *
	 * @param ipSelectedLayer	[in]	�I�����Ă��郌�C��
	 * @param bIsRoadLinkOnly	[in]	�����N��I�����t�B�[�`���I����
	 *
	 * @return �I�����Ă��郌�C���̃��X�g
	 */
	std::list< CAdapt<ILayerPtr> > _GetSearchLayer(ILayerPtr ipSelectedLayer, bool bIsRoadLinkOnly);

	/** 
	 * @brief ���C�����\������Ă��邩�ǂ���
	 *  
	 * @param ipLayer	[in]	���C��
	 *
	 * @return �\������Ă���Ȃ� true
	 */
	bool IsVisibleLayer( ILayerPtr ipLayer );

	/** 
	 * @brief �O���[�v���C���ȉ��ɂ���\������Ă��郌�C����Ԃ�
	 *  
	 * @param ipComositeLayer	[in]	�O���[�v���C��
	 *
	 * @return ���C���̃��X�g
	 */
	std::list< CAdapt< ILayerPtr > > GetAllLayerUnderGroup(ICompositeLayerPtr ipCompositeLayer);

	/** 
	 * @brief ���C�����������đ����_�C�A���O�ɃZ�b�g����
	 * 
	 * @param ipGeom	[in]	�����`��
	 * @param ipLayer	[in]	���C��
	 *
	 * @retval true �����_�C�A���O�Ƀt�B�[�`���Z�b�g����
	 * @retval false �����_�C�A���O�Ƀt�B�[�`���Z�b�g���Ȃ�����
	 */
	bool SelectByShapeFromLayer( IGeometryPtr ipGeom, ILayerPtr ipLayer );

	/** 
	 * @brief ���H�����N�ɂ��Ă��郊���N�����������\n
	 *  
	 * @param ipRoadClass	[in]	�������铹�H�����N�N���X
	 * @param ipEnv			[in]	�����͈͌`��
	 * @param type			[in]	�����N��̃^�C�v
	 * @param bLastLink		[in]	�I�[�̃����N�݂̂������Ă��邩�ǂ���
	 *
	 * @return �I���ł��������N�񂪂���Ȃ� true
	 */
	bool SelectTargetLQs( IFeatureClassPtr ipRoadClass, IGeometryPtr ipGeom, sindyeRoadQueueType type, bool bLastLink  );

	/** 
	 * @brief AttributeDlg���쐬�E�Ԃ��֐�\n
	 * �����_�C�A���O���쐬����Ă��Ȃ�������쐬����
	 *  
	 * @return �����_�C�A���O
	 */
	IAttributeDialogPtr GetAttributeDlg();

	/** 
	 * @brief �����_�C�A���O�ɑI�������t�B�[�`����n��\n
	 *
	 * @param ipFeatureCursor ���������t�B�[�`���J�[�\�� 
	 *
	 * @return ���������t�B�[�`���ɂȂɂ��t�B�[�`���������Ă�����true
	 */
	bool AddFeatureToDialog( IFeatureCursorPtr ipFeatureCursor );

	/** 
	 * @brief �����ҏW�̃v���p�e�B�̒l���ύX���ꂽ��A���W�X�g���ɓo�^����\n
	 *
	 * @param bIsPerfectSelect			���ʁA���[���̏ꍇ�Ɏ擾�����N�Ɋւ��邷�ׂẴ����N��擾���邩�ǂ��� 
	 * @param bIsSelectedLastSequence	���ʁA���[���̏ꍇ�ɋ�`�I���������H�����N�𗬓������N�Ƃ��郊���N���I�����邩�ǂ���
	 */
	void WriteProperty( BOOL bIsPerfectSelect, BOOL bIsSelectedLastSequence );

	/** 
	 * @brief �N�����Ƀ��W�X�g���ɓo�^����Ă���v���p�e�B�̓��e��ǂݎ��
	 */
	void LoadProperty();

	/**
	 * @brief <b>�����N�񂪕\������Ă��ĕҏW�ł��郂�[�h�擾</b>\n
	 * �ҏWOK�̃��[�h�͕\������Ă��鎖���O��ɂȂ��Ă���B
	 * [Bug 6042] �V���{���ύX�r���[�c�[���̃����N��ҏW�{�^���ɑΉ����ė~����
	 * @return �ҏWOK�ȃ��[�h
	 */
	long GetEditLinkQueueMode(IFeatureRenderer* ipRednerer);

	/**
	 * @brief �����N��e�[�u���擾
	 * @param ipTable [in] ITable(���H)
	 * @param lpcszName [in] �e�[�u����
	 * @param lpcszExName [in] ��O�������s���e�[�u����(INF_IPCVICS/LQ_IPCVICS)
	 * @return ITable �����N��e�[�u��
	*/
	ITablePtr GetTable(const ITablePtr& ipTable, LPCTSTR lpcszName, LPCTSTR lpcszExName);

	/**
	 * @brief TOC�ɂ���w�肵���e�[�u��������ITable���擾
	 * @param lpcszName [in] �e�[�u����
	 * @param lpcszOwner [in/optional] �I�[�i��
	 * @return ���݂��鎞��ITable���Ԃ�B���݂��Ȃ�����NULL
	*/
	ITablePtr GetSameTable(LPCTSTR lpcszName, LPCTSTR lpcszOwner=nullptr);

	/**
	 * @brief �I�[�i�����擾
	 * @param ipTable [in] �e�[�u��
	 * @return �I�[�i���i��̎��̓p�X����Ԃ��j
	*/
	CString GetOwnerName(const ITablePtr& ipTable);

	/**
	 * �w��̃G���A���ɑ��݂��郍�[�h�L���[��INFO���R�[�h���擾
	 * ArcHelperEx::AheSelectInfoRowByShape�Ƃقړ����iLQRF�����Ȃ��Łj
	 * 
	 * @param ipRoadClass			[in]			���H�����N�N���X
	 * @param ipInfoTable			[in]			�������郍�[�h�L���[�� INFO �e�[�u��
	 * @param ipLQTable				[in]			�������郍�[�h�L���[�� LQ �e�[�u��
	 * @param ipGeom				[in]			�I��͈́i���H�����N��I���j
	 * @param lDrawMode				[in]			�����N��̎�ށiIRoadRenderer �Ŏg�p����`�惂�[�h���g�p���邱�Ɓj
	 * @param listInfoRows			[out]			�擾�ł��� INFO �̃��R�[�h
	 * @param listRoads				[out]			�擾�ł������H�����N
	 * @param lSequence				[in,optional]	LQ_* ����������ۂɎw�肷��V�[�P���X�ԍ��i�f�t�H���g�F-1�j
	 * @param bLastLink				[in,optional]	���o�����N���܂߂邩�ǂ���
	 *
	 * @retval �擾�ł���INFO�̃��R�[�h��
	 */
	long SelectInfoRowByShape( const IFeatureClassPtr& ipRoadClass, const ITablePtr& ipInfoTable, const ITablePtr& ipLQTable, const IGeometryPtr& ipGeom, long lDrawMode, std::list<ROWDEF>& listInfoRows, std::list<ROWDEF>& listRoads, long lSequence = -1, bool bLastLink = false );


/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolObj �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	CArcHelper				m_cArcHelper;		//!< ArcObjects �w���p�[�N���X
	IAttributeDialogPtr		m_ipAttributeDlg;	//!< �����\���E�ҏW�_�C�A���O
	CLoadQueueInfo			m_cLQInfo;			//!< �����N��̏��

	bool					m_bIsMouseDown;		//!< ���N���b�N����p
	bool					m_bIsRightDown;		//!< �E�N���b�N����p
	long					m_lMouseDownX, m_lMouseDownY; //!< MouseDown�������W
	bool					m_bFreeSelectMode;	//!< ���R�I�����[�h�t���O
	bool					m_bShowContextMenu;	//!< �E�N���b�N���j���[���o���ׂ����ǂ���
	long					m_lShift;			//!< �V�t�g�������Ă��邩�ǂ���

	IPointPtr				m_ipMouseDownPoint;	//!< �}�E�X���N���b�N�����n�_�̍��W
	INewEnvelopeFeedbackPtr	m_ipFeedbackEnv;	//!< �I����`�`��p
	bool					m_bDrawingSelectArea;	//!< �Z�`�I�𒆂��ǂ���
	CFeedback				m_cFeedback;		//!< �Z�`�I��p�̃t�B�[�h�o�b�N�N���X
	CSketchFeedback	m_cSketchFeedback;			//!< �̈�I��p�t�B�[�h�o�b�N
	IGeometryPtr			m_ipSelectGeom;		//!< ���R�I�����[�h�̌`��
	long					m_lSearchItemNum;	//!< �I�������A�C�e���̐�

	long					m_lSubType;			//!< �T�u�^�C�v

	// �J�[�\���֌W
	HCURSOR					m_cursor;			//!< �����\���ҏW�c�[���̃f�t�H���g�J�[�\��
	HCURSOR					m_LQcursor;			//!< �����N�񑮐��\���ҏW�c�[���̃f�t�H���g�J�[�\��
	HCURSOR					m_curMove;			//!< �����Ă���r���̃J�[�\��
	HCURSOR					m_LQcurMove;		//!< �����N�񑮐��\���ҏW�c�[���̓����Ă���r���̃J�[�\��
	HCURSOR					m_curFreeSel;		//!< ���R�I��p�r���̃J�[�\��
	HCURSOR					m_LQcurFreeSel;		//!< �����N�񑮐��\���ҏW�c�[���̎��R�I��r���̃J�[�\��
	HCURSOR*                m_pCurPointer;		//!< ���ݕ\�����̃J�[�\���|�C���^
	HBITMAP					m_bitmap;			//!< �����\���ҏW�c�[���{�^���̃r�b�g�}�b�v
	HBITMAP					m_LQbitmap;			//!< �����N�񑮐��\���ҏW�c�[���{�^���̃r�b�g�}�b�v

	// �C�x���g�ڑ�
	CAdviseAdapter<IDocument, &IID_IDocumentEvents>		m_cDocumentEvents;	//!< IDocumentEvents�ւ̐ڑ��N���X
	CAdviseAdapter<IActiveView, &IID_IActiveViewEvents>	m_cActiveViewEvents;	//!< IActiveViewEvents�ւ̐ڑ��N���X

	BOOL					m_bPerfectSelect;	//!< ���ʁA���[���̏ꍇ�Ɋ����Ɏ擾���邩�ǂ���
	BOOL					m_bIsSelectedLastSequence;	//!< ���ʁA���[���̏ꍇ�ɋ�`�I���������H�����N�𗬓������N�Ƃ��郊���N���I�����邩�ǂ���
};

#endif //__ATTRIBUTETOOLOBJ_H_
