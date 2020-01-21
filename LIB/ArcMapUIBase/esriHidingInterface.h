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

#ifndef _ESRIHIDINGINTERFACE_H_
#define _ESRIHIDINGINTERFACE_H_

enum esriCommandFlags
{
    esriCmdFlagNormal = 0,
    esriCmdFlagContextBar = 1,
    esriCmdFlagLocked = 2,
    esriCmdFlagUIControl = 4,
    esriCmdFlagInnerTool = 8,
    esriCmdFlagInnerToolCtrl = 16,
    esriCmdFlagInnerMultiItem = 32,
    esriCmdFlagUICommandBar = 64,
    esriCmdFlagShortcutMenu = 128,
    esriCmdFlagSplitButton = 256,
    esriCmdFlagInvalid = 512
};

enum esriToolbarChangeCodes
{
    esriToolbarChangeNone = 0,
    esriToolbarUpdate = 1,
    esriToolbarAddCommand = 2,
    esriToolbarRemoveCommand = 4,
    esriToolbarModifyCommand = 16,
    esriToolbarRefresh = 32
};

struct __declspec(uuid("fab1df18-4e4d-11d2-aaaf-00c04fa375f1"))
IChangeTableEntry : IUnknown
{
    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall get_CommandItem (
        struct ICommandItem * * ppCommandItem ) = 0;
    virtual HRESULT __stdcall get_ID (
        struct IUID * * ppID ) = 0;
    virtual HRESULT __stdcall get_ChangeAction (
        enum esriToolbarChangeCodes * pAction ) = 0;
    virtual HRESULT __stdcall get_Index (
        long * pIndex ) = 0;
    virtual HRESULT __stdcall get_templContext (
        long * ctx ) = 0;
};

struct __declspec(uuid("db0eae70-4e48-11d2-aaaf-00c04fa375f1"))
IChangeTable : IUnknown
{
    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall get_BarID (
        GUID * pClsid ) = 0;
    virtual HRESULT __stdcall get_ChangeTableEntry (
        long Index,
        struct IChangeTableEntry * * ppEntry ) = 0;
    virtual HRESULT __stdcall get_Count (
        long * pSize ) = 0;
    virtual HRESULT __stdcall Reset ( ) = 0;
    virtual HRESULT __stdcall GetDocContext (
        long * Context ) = 0;
};

struct __declspec(uuid("85e9bbd7-441d-11d2-94d4-080009eebecb"))
ICommandBarsAdmin : IUnknown
{
    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall Create (
        struct IApplication * app ) = 0;
    virtual HRESULT __stdcall Add (
        struct ICommandBar * bar ) = 0;
    virtual HRESULT __stdcall GetCount (
        long * Count ) = 0;
    virtual HRESULT __stdcall GetItem (
        long Index,
        struct ICommandBar * * bar ) = 0;
    virtual HRESULT __stdcall Remove (
        long Index ) = 0;
};

struct __declspec(uuid("bc480932-cb1e-11d1-88ea-00805f1525f4"))
ICommandItemAdmin : IUnknown
{
    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall Create (
        struct ICommandItem * pParent,
        GUID * pClassID,
        IDispatch * pHook,
        long SubType ) = 0;
    virtual HRESULT __stdcall SetIndex (
        long Index ) = 0;
    virtual HRESULT __stdcall SetID (
        long itemID ) = 0;
    virtual HRESULT __stdcall SetType (
        enum esriCommandTypes Type ) = 0;
    virtual HRESULT __stdcall SetAppAdmin (
        struct IApplicationAdmin * pContainer ) = 0;
    virtual HRESULT __stdcall get_InnerID (
        GUID * pClassID ) = 0;
    virtual HRESULT __stdcall SetInnerID (
        GUID * pClassID ) = 0;
    virtual HRESULT __stdcall SetBitmap (
        unsigned int hBitmap,
        VARIANT_BOOL bTakeOwnership ) = 0;
    virtual HRESULT __stdcall get_InfoFlags (
        enum esriCommandFlags * Flags ) = 0;
    virtual HRESULT __stdcall AttachToControl (
        struct ICommandItem * pParent,
        enum esriCommandTypes Type,
        long templateIndex,
        long UIctrlID,
        IDispatch * pHook ) = 0;
    virtual HRESULT __stdcall GetMultiItem (
        struct IMultiItem * * MultiItem ) = 0;
    virtual HRESULT __stdcall ApplyChangeTable (
        struct IChangeTable * pChangeTable ) = 0;
    virtual HRESULT __stdcall ConstructMySubItems (
        IUnknown * pUnk ) = 0;
    virtual HRESULT __stdcall SetTemporary (
        VARIANT_BOOL bTemp ) = 0;
    virtual HRESULT __stdcall IsTemporary (
        VARIANT_BOOL * bTemp ) = 0;
    virtual HRESULT __stdcall UpdateStyle (
        enum esriCommandStyles Style ) = 0;
    virtual HRESULT __stdcall SetTemplateContext (
        long templContext ) = 0;
    virtual HRESULT __stdcall GetTemplateContext (
        long * templContext ) = 0;
    virtual HRESULT __stdcall SetCustomizationID (
        long ID ) = 0;
    virtual HRESULT __stdcall GetCustomizationID (
        long * ID ) = 0;
    virtual HRESULT __stdcall GetUIControlInfo (
        long * templIndex,
        long * ctrlID ) = 0;
    virtual HRESULT __stdcall SetConstructMode (
        VARIANT_BOOL Mode ) = 0;
};

struct __declspec(uuid("e7346a10-d622-11d1-be51-080009a874fa"))
ICommandBarAdmin : IUnknown
{
    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall Reset ( ) = 0;
    virtual HRESULT __stdcall Add (
        struct ICommandItem * Item ) = 0;
    virtual HRESULT __stdcall Remove (
        long Index,
        VARIANT_BOOL bPersistDelete,
        VARIANT_BOOL bResetParent ) = 0;
    virtual HRESULT __stdcall Insert (
        long Index,
        struct ICommandItem * Item ) = 0;
    virtual HRESULT __stdcall GetCustContextCommand (
        struct ICommandItem * * Item ) = 0;
    virtual HRESULT __stdcall SetCustContextCommand (
        struct ICommandItem * Item ) = 0;
    virtual HRESULT __stdcall UpdateVisibility (
        VARIANT_BOOL bVisible ) = 0;
    virtual HRESULT __stdcall UpdateCaption (
        BSTR barCaption ) = 0;
    virtual HRESULT __stdcall Update ( ) = 0;
    virtual HRESULT __stdcall GetNextValidCustomizationID (
        long * ID ) = 0;
    virtual HRESULT __stdcall StoreAllChanges ( ) = 0;
    virtual HRESULT __stdcall SetDynamicSize (
        long X,
        long Y ) = 0;
    virtual HRESULT __stdcall GetDynamicSize (
        long * X,
        long * Y ) = 0;
    virtual HRESULT __stdcall SetVisibility (
        VARIANT_BOOL bVisible ) = 0;
    virtual HRESULT __stdcall GetVisibility (
        VARIANT_BOOL * pVisible ) = 0;
};

struct __declspec(uuid("5aaf7113-3888-11d3-9f57-00c04f6bc61a"))
IHelpTool : IUnknown
{
    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall ShowHelp (
        struct _ICommand * pCommand ) = 0;
    virtual HRESULT __stdcall ShowMultiItemHelp (
        struct _ICommand * pCommand,
        long Index ) = 0;
};

struct __declspec(uuid("523974d6-e8e3-11d3-a67e-0008c7df97b9"))
IApplicationAdmin : IUnknown
{
    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall GetIconFromFaceID (
        long FaceID,
        unsigned int * Bitmap ) = 0;
    virtual HRESULT __stdcall GetIconCount (
        long * iconCount ) = 0;
    virtual HRESULT __stdcall GetCursorFromCursorID (
        long cursorID,
        unsigned int * pCursor ) = 0;
    virtual HRESULT __stdcall GetCursorCount (
        long * cursorCount ) = 0;
    virtual HRESULT __stdcall FireMacro (
        struct ICommandItem * pItem ) = 0;
    virtual HRESULT __stdcall OnBarChange (
        GUID * changedBar,
        struct ICommandItem * pCommandItem,
        enum esriToolbarChangeCodes notifyCode,
        long Index,
        enum esriCmdBarType Type,
        VARIANT_BOOL resetbar ) = 0;
    virtual HRESULT __stdcall DockBar (
        GUID * bar,
        enum esriDockFlags dockFlags,
        struct ICommandBar * pRefBar ) = 0;
    virtual HRESULT __stdcall ChangeTableUpdateAdd (
        GUID * changedBar,
        struct ICommandItem * pCommandItem,
        long Index,
        enum esriCmdBarType Type ) = 0;
    virtual HRESULT __stdcall ChangeTableUpdateRemove (
        GUID * changedBar,
        struct ICommandItem * pCommandItem,
        long Index,
        enum esriCmdBarType Type ) = 0;
    virtual HRESULT __stdcall ChangeTableUpdateModify (
        GUID * changedBar,
        struct ICommandItem * pCommandItem,
        long Index,
        enum esriCmdBarType Type ) = 0;
    virtual HRESULT __stdcall ChangeTableUpdateToolbarSize (
        GUID * changedBar,
        struct ICommandBarAdmin * pBar ) = 0;
    virtual HRESULT __stdcall ChangeTableUpdateToolbarVisibility (
        GUID * changedBar,
        VARIANT_BOOL bVis ) = 0;
    virtual HRESULT __stdcall OnCommandBarCreate (
        GUID * BarID,
        struct ICommandBar * pParent,
        struct ICommandBar * * ppBar ) = 0;
    virtual HRESULT __stdcall CreateDocCommandBar (
        long docIndex,
        enum esriCmdBarType __MIDL_0018,
        struct ICommandBar * pParent,
        BSTR Name,
        struct ICommandBar * * ppBar ) = 0;
    virtual HRESULT __stdcall GetAppHook (
        IDispatch * * pHook ) = 0;
    virtual HRESULT __stdcall IsIDEActive (
        VARIANT_BOOL * pHook ) = 0;
    virtual HRESULT __stdcall TrackPopup (
        struct ICommandBar * cmdBar,
        long X,
        long Y,
        struct ICommandItem * * choiceIndex ) = 0;
    virtual HRESULT __stdcall EditVBAObject (
        struct ICommandItem * Object ) = 0;
    virtual HRESULT __stdcall IsVBALoaded (
        VARIANT_BOOL * bVBALoaded ) = 0;
    virtual HRESULT __stdcall ShowAboutDialog ( ) = 0;
    virtual HRESULT __stdcall Unlock ( ) = 0;
    virtual HRESULT __stdcall FindChangeTable (
        GUID * CLSID,
        struct IChangeTable * * ppTable ) = 0;
    virtual HRESULT __stdcall FindDocSpecificChangeTable (
        GUID * CLSID,
        struct IChangeTable * * ppTable ) = 0;
    virtual HRESULT __stdcall GetCAT (
        enum esriCATtype Type,
        GUID * cat ) = 0;
    virtual HRESULT __stdcall UpdateAccelerators ( ) = 0;
    virtual HRESULT __stdcall DoIdleProcessing ( ) = 0;
    virtual HRESULT __stdcall SetDocContext (
        long templateIndex ) = 0;
    virtual HRESULT __stdcall GetDocContext (
        long * pTemplateIndex ) = 0;
    virtual HRESULT __stdcall GetCurrentCustFilter (
        struct ICustomizationFilter * * pFilter ) = 0;
    virtual HRESULT __stdcall SetTemporaryMode (
        VARIANT_BOOL bMode ) = 0;
    virtual HRESULT __stdcall GetTemporaryMode (
        VARIANT_BOOL * pMode ) = 0;
    virtual HRESULT __stdcall AddChangeTable (
        GUID * BarID,
        enum esriCommandTypes Type ) = 0;
    virtual HRESULT __stdcall StoreCommandItemChanges (
        struct ICommandItem * pItem ) = 0;
    virtual HRESULT __stdcall ResetCommandItemChanges (
        struct ICommandItem * pItem ) = 0;
    virtual HRESULT __stdcall GetDocument (
        long Index,
        struct IDocumentAdmin * * docAdmin ) = 0;
    virtual HRESULT __stdcall SetDocModified (
        VARIANT_BOOL Modified ) = 0;
    virtual HRESULT __stdcall IsLocked (
        VARIANT_BOOL * bLocked ) = 0;
    virtual HRESULT __stdcall GetPassword (
        BSTR * Password ) = 0;
    virtual HRESULT __stdcall SetTempResetMode (
        VARIANT_BOOL Mode ) = 0;
    virtual HRESULT __stdcall SetResetParentMode (
        VARIANT_BOOL Mode ) = 0;
    virtual HRESULT __stdcall SetHelpModeObject (
        struct IHelpTool * pHelpCommand ) = 0;
    virtual HRESULT __stdcall GetHelpModeObject (
        struct IHelpTool * * ppHelpCommand ) = 0;
    virtual HRESULT __stdcall SetHwnd (
        unsigned int hWnd ) = 0;
    virtual HRESULT __stdcall GetDefaultTemplatePath (
        BSTR * defaultTemplatePath ) = 0;
    virtual HRESULT __stdcall CreateDockableWindow (
        struct IDockableWindowDef * pDef,
        struct IDockableWindowDefInfo * pInfo,
        unsigned int * me ) = 0;
    virtual HRESULT __stdcall ShowDockableWindow (
        VARIANT_BOOL Show,
        unsigned int dockWnd ) = 0;
    virtual HRESULT __stdcall GetDockableWndHorzSize (
        unsigned int m_hwnd,
        long * X,
        long * Y ) = 0;
    virtual HRESULT __stdcall GetDockableWndVertSize (
        unsigned int m_hwnd,
        long * X,
        long * Y ) = 0;
    virtual HRESULT __stdcall GetDockableWndFloatSize (
        unsigned int m_hwnd,
        long * X,
        long * Y ) = 0;
    virtual HRESULT __stdcall SetDockableWndHorzSize (
        unsigned int m_hwnd,
        long X,
        long Y ) = 0;
    virtual HRESULT __stdcall SetDockableWndVertSize (
        unsigned int m_hwnd,
        long X,
        long Y ) = 0;
    virtual HRESULT __stdcall SetDockableWndFloatSize (
        unsigned int m_hwnd,
        long X,
        long Y ) = 0;
    virtual HRESULT __stdcall DockDockableWindow (
        unsigned int hWnd,
        enum esriDockFlags dockFlags ) = 0;
    virtual HRESULT __stdcall GetCommandCache (
        struct ICommandCache * * pCmdCache ) = 0;
    virtual HRESULT __stdcall SetLargeIcons (
        VARIANT_BOOL bLargeIcons ) = 0;
    virtual HRESULT __stdcall GetLargeIcons (
        VARIANT_BOOL * bLargeIcons ) = 0;
    virtual HRESULT __stdcall RedrawToolbars ( ) = 0;
    virtual HRESULT __stdcall GetUserTemplatePath (
        BSTR * defaultTemplatePath ) = 0;
    virtual HRESULT __stdcall SetToolTipsEnabled (
        VARIANT_BOOL bToolTips ) = 0;
    virtual HRESULT __stdcall GetToolTipsEnabled (
        VARIANT_BOOL * bToolTips ) = 0;
    virtual HRESULT __stdcall CreateAccelerators ( ) = 0;
    virtual HRESULT __stdcall GetActiveView (
        unsigned int * hActiveView ) = 0;
    virtual HRESULT __stdcall DockControlBar (
        unsigned int hWnd,
        struct tagRECT * pRect ) = 0;
    virtual HRESULT __stdcall GetHwndFromControlBarID (
        GUID * controlBarID,
        unsigned int * hWnd ) = 0;
};

struct __declspec(uuid("f2dcdccb-186e-11d3-9f40-00c04f6bddd9"))
ISdeWorkspaceImpl : IUnknown
{
    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall get_ConnectionHandle (
        long * Connection ) = 0;
    virtual HRESULT __stdcall ConnectedAsDBA (
        VARIANT_BOOL * ConnectedAsDBA ) = 0;
    virtual HRESULT __stdcall GetSdeReleaseInfo (
        long * major,
        long * minor,
        long * bugfix,
        long * composite ) = 0;
    virtual HRESULT __stdcall get_ConnectionWorkspace (
        struct IWorkspace * * innerWorkspace ) = 0;
};

struct __declspec(uuid("2e31d995-e9d4-11d1-aa82-00c04fa37585"))
IVersionImpl : IUnknown
{
    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall get_StateID (
        long * sid ) = 0;
    virtual HRESULT __stdcall IsDescendantOf (
        struct IVersion * ancestorVersion,
        VARIANT_BOOL * IsDescendantOf ) = 0;
    virtual HRESULT __stdcall IsAncestorOf (
        struct IVersion * descendantVersion,
        VARIANT_BOOL * IsAncestorOf ) = 0;
    virtual HRESULT __stdcall put_IsDeleted (
        VARIANT_BOOL _arg1 ) = 0;
    virtual HRESULT __stdcall GetModifiedClasses (
        struct IVersion * otherVersion,
        struct IEnumBSTR * * enumClasses ) = 0;
    virtual HRESULT __stdcall CreateChildState (
        long * newSID ) = 0;
    virtual HRESULT __stdcall CreateVersion (
        BSTR Name,
        long sid,
        struct IVersion * * newVersion ) = 0;
    virtual HRESULT __stdcall AlterState (
        long sid ) = 0;
    virtual HRESULT __stdcall IsStateDirectDescendant (
        long sid,
        long ancestorSID,
        VARIANT_BOOL * isDirectDescendant ) = 0;
    virtual HRESULT __stdcall CreateAnonymousVersion (
        long sid,
        struct IVersion * * newVersion ) = 0;
    virtual HRESULT __stdcall SwitchVersionState (
        long sid ) = 0;
    virtual HRESULT __stdcall BuildCompressedStateTree (
        long * baseSID,
        long * Count,
        long * * childIDs,
        long * * parentIDs,
        struct ILog * LogFile ) = 0;
    virtual HRESULT __stdcall BroadcastDatabaseStateChanged ( ) = 0;
    virtual HRESULT __stdcall BroadcastDatabaseStateChangedToAllNamedVersions ( ) = 0;
};

struct __declspec(uuid("a6c50711-42c0-11d2-94d4-080009eebecb"))
IDocumentAdmin : IUnknown
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall SetID (
        /*[in]*/ struct IUID * ID ) = 0;
      virtual HRESULT __stdcall SetType (
        /*[in]*/ enum esriDocumentType docType ) = 0;
      virtual HRESULT __stdcall AddControl (
        /*[in]*/ GUID * CLSID,
        /*[out,retval]*/ long * controlID ) = 0;
      virtual HRESULT __stdcall GetControl (
        /*[in]*/ long controlID,
        /*[out,retval]*/ IUnknown * * pCtrl ) = 0;
      virtual HRESULT __stdcall GetControlName (
        /*[in]*/ long controlID,
        /*[out,retval]*/ BSTR * ctrlName ) = 0;
      virtual HRESULT __stdcall RenameControl (
        /*[in]*/ long controlID,
        /*[in]*/ BSTR newName ) = 0;
      virtual HRESULT __stdcall RemoveControl (
        /*[in]*/ long controlID ) = 0;
      virtual HRESULT __stdcall EditControl (
        /*[in]*/ long controlID ) = 0;
      virtual HRESULT __stdcall GetTemplateIndex (
        /*[out,retval]*/ long * docContext ) = 0;
      virtual HRESULT __stdcall SaveDocument ( ) = 0;
      virtual HRESULT __stdcall SetModified (
        /*[in]*/ VARIANT_BOOL dirty ) = 0;
      virtual HRESULT __stdcall SetVBAChanged ( ) = 0;
      virtual HRESULT __stdcall GetVBAProjectName (
        /*[out,retval]*/ BSTR * projectName ) = 0;
      virtual HRESULT __stdcall RemoveChangeTable (
        /*[in]*/ GUID * CLSID ) = 0;
};

struct __declspec(uuid("bb98fdb8-f561-11d3-a67f-0008c7df97b9"))
IDockableWindowDefInfo : IUnknown
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall GetDockableWindowDef (
        /*[out]*/ struct IDockableWindowDef * * def ) = 0;
      virtual HRESULT __stdcall GetCtrlID (
        /*[out]*/ long * ID ) = 0;
      virtual HRESULT __stdcall GetID (
        /*[out]*/ struct IUID * * ID ) = 0;
      virtual HRESULT __stdcall GetCaption (
        /*[out]*/ BSTR * Caption ) = 0;
      virtual HRESULT __stdcall GetHorzSize (
        /*[out]*/ long * X,
        /*[out]*/ long * Y ) = 0;
      virtual HRESULT __stdcall GetVertSize (
        /*[out]*/ long * X,
        /*[out]*/ long * Y ) = 0;
      virtual HRESULT __stdcall GetFloatSize (
        /*[out]*/ long * X,
        /*[out]*/ long * Y ) = 0;
      virtual HRESULT __stdcall GetVisibility (
        /*[out]*/ VARIANT_BOOL * bVis ) = 0;
      virtual HRESULT __stdcall SetDockableWindowDef (
        /*[in]*/ struct IDockableWindowDef * def ) = 0;
      virtual HRESULT __stdcall SetCtrlID (
        /*[in]*/ long ID ) = 0;
      virtual HRESULT __stdcall SetID (
        /*[in]*/ struct IUID * ID ) = 0;
      virtual HRESULT __stdcall SetCaption (
        /*[in]*/ BSTR Caption ) = 0;
      virtual HRESULT __stdcall SetHorzSize (
        /*[in]*/ long X,
        /*[in]*/ long Y ) = 0;
      virtual HRESULT __stdcall SetVertSize (
        /*[in]*/ long X,
        /*[in]*/ long Y ) = 0;
      virtual HRESULT __stdcall SetFloatSize (
        /*[in]*/ long X,
        /*[in]*/ long Y ) = 0;
      virtual HRESULT __stdcall SetVisibility (
        /*[in]*/ VARIANT_BOOL bVis ) = 0;
      virtual HRESULT __stdcall Clear ( ) = 0;
      virtual HRESULT __stdcall GetDockParent (
        unsigned int * hParent ) = 0;
      virtual HRESULT __stdcall SetDockParent (
        unsigned int hParent ) = 0;
};

struct __declspec(uuid("53b28aca-e706-11d3-a67d-0008c7df97b9"))
IDockableWindowAdmin : IUnknown
{
    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall Create (
        /*[in]*/ IDispatch * pHook,
        /*[in]*/ struct IDockableWindowDef * pDef,
        /*[in]*/ struct IDockableWindowDefInfo * pInfo ) = 0;
      virtual HRESULT __stdcall GetHWND (
        /*[out]*/ unsigned int * hWnd ) = 0;
      virtual HRESULT __stdcall GetHorzSize (
        /*[out]*/ long * X,
        /*[out]*/ long * Y ) = 0;
      virtual HRESULT __stdcall GetVertSize (
        /*[out]*/ long * X,
        /*[out]*/ long * Y ) = 0;
      virtual HRESULT __stdcall GetFloatSize (
        /*[out]*/ long * X,
        /*[out]*/ long * Y ) = 0;
      virtual HRESULT __stdcall SetHorzSize (
        /*[in]*/ long X,
        /*[in]*/ long Y ) = 0;
      virtual HRESULT __stdcall SetVertSize (
        /*[in]*/ long X,
        /*[in]*/ long Y ) = 0;
      virtual HRESULT __stdcall SetFloatSize (
        /*[in]*/ long X,
        /*[in]*/ long Y ) = 0;
};

extern "C" const GUID __declspec(selectany) IID_ICommandItemAdmin =
    {0xbc480932,0xcb1e,0x11d1,{0x88,0xea,0x00,0x80,0x5f,0x15,0x25,0xf4}};
extern "C" const GUID __declspec(selectany) IID_ICommandBarsAdmin =
    {0x85e9bbd7,0x441d,0x11d2,{0x94,0xd4,0x08,0x00,0x09,0xee,0xbe,0xcb}};
extern "C" const GUID __declspec(selectany) IID_ICommandBarAdmin =
    {0xe7346a10,0xd622,0x11d1,{0xbe,0x51,0x08,0x00,0x09,0xa8,0x74,0xfa}};
extern "C" const GUID __declspec(selectany) IID_IHelpTool =
    {0x5aaf7113,0x3888,0x11d3,{0x9f,0x57,0x00,0xc0,0x4f,0x6b,0xc6,0x1a}};
extern "C" const GUID __declspec(selectany) IID_IApplicationAdmin =
    {0x523974d6,0xe8e3,0x11d3,{0xa6,0x7e,0x00,0x08,0xc7,0xdf,0x97,0xb9}};
extern "C" const GUID __declspec(selectany) IID_ISdeWorkspaceImpl =
    {0xf2dcdccb,0x186e,0x11d3,{0x9f,0x40,0x00,0xc0,0x4f,0x6b,0xdd,0xd9}};
extern "C" const GUID __declspec(selectany) IID_IVersionImpl =
    {0x2e31d995,0xe9d4,0x11d1,{0xaa,0x82,0x00,0xc0,0x4f,0xa3,0x75,0x85}};
extern "C" const GUID __declspec(selectany) IID_IDocumentAdmin =
    {0xa6c50711,0x42c0,0x11d2,{0x94,0xd4,0x08,0x00,0x09,0xee,0xbe,0xcb}};
extern "C" const GUID __declspec(selectany) IID_IDockableWindowDefInfo =
    {0xbb98fdb8,0xf561,0x11d3,{0xa6,0x7f,0x00,0x08,0xc7,0xdf,0x97,0xb9}};
extern "C" const GUID __declspec(selectany) IID_IDockableWindowAdmin =
    {0x53b28aca,0xe706,0x11d3,{0xa6,0x7d,0x00,0x08,0xc7,0xdf,0x97,0xb9}};

_COM_SMARTPTR_TYPEDEF(ICommandBarsAdmin, __uuidof(ICommandBarsAdmin));
_COM_SMARTPTR_TYPEDEF(ICommandItemAdmin, __uuidof(ICommandItemAdmin));
_COM_SMARTPTR_TYPEDEF(ICommandBarAdmin, __uuidof(ICommandBarAdmin));
_COM_SMARTPTR_TYPEDEF(IHelpTool, __uuidof(IHelpTool));
_COM_SMARTPTR_TYPEDEF(IApplicationAdmin, __uuidof(IApplicationAdmin));
_COM_SMARTPTR_TYPEDEF(ISdeWorkspaceImpl, __uuidof(ISdeWorkspaceImpl));
_COM_SMARTPTR_TYPEDEF(IVersionImpl, __uuidof(IVersionImpl));
_COM_SMARTPTR_TYPEDEF(IDocumentAdmin, __uuidof(IDocumentAdmin));
_COM_SMARTPTR_TYPEDEF(IDockableWindowDefInfo, __uuidof(IDockableWindowDefInfo));
_COM_SMARTPTR_TYPEDEF(IDockableWindowAdmin, __uuidof(IDockableWindowAdmin));

#endif // ifndef _ESRIHIDINGINTERFACE_H_
