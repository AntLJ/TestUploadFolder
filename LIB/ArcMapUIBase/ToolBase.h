/*
 Copyright 1995-2004 ESRI

 All rights reserved under the copyright laws of the United States.

 You may freely redistribute and use this sample code, with or without modification.

 Disclaimer: THE SAMPLE CODE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED 
 WARRANTIES, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ESRI OR 
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 INTERRUPTION) SUSTAINED BY YOU OR A THIRD PARTY, HOWEVER CAUSED AND ON ANY 
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT ARISING IN ANY 
 WAY OUT OF THE USE OF THIS SAMPLE CODE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 SUCH DAMAGE.

 For additional information contact: Environmental Systems Research Institute, Inc.

 Attn: Contracts Dept.

 380 New York Street

 Redlands, California, U.S.A. 92373 

 Email: contracts@esri.com
*/
// MapToolBase.h: interface for the CToolBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBASE_H__D59C4843_8456_43FF_9E3A_AA7E5EA60A85__INCLUDED_)
#define AFX_TOOLBASE_H__D59C4843_8456_43FF_9E3A_AA7E5EA60A85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommandBase.h"
#include "ZooMoveController.h"
#include <boost/shared_ptr.hpp>
#include "ToolKeys.h"

class _U_CURSORorID
{
public:
	_U_CURSORorID() : m_hCur(NULL)
	{ }
	_U_CURSORorID(HCURSOR hCur) : m_hCur(hCur)
	{ }
	_U_CURSORorID(UINT nID) : m_hCur((HCURSOR)LoadImage(_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(nID), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE))
	{ }
	operator HCURSOR() { return m_hCur; }
	HCURSOR m_hCur;
};

enum uiMouseEventStatus
{
	uiMouseEventNothing = 0,
	uiMouseLButtonDown  = 1, //!< ���{�^����������Ă���
	uiMouseRButtonDown  = 2, //!< �E�{�^����������Ă���
	uiMouseMButtonDown  = 4, //!< ���{�^����������Ă���
	uiMouseDoubleClick  = 8, //!< �_�u���N���b�N
};

// 
// Provides basic implementation of a tool that requires a focus map 
// This inherits ICommand from CMapCommandBase
// Inherit this into your tool implementation
//
// Default behaviour
//   Cursor - returns normal cursor or mouse down cursor
//   Deactivate - returns true;
//   MouseDown, MouseUp, MouseMove - implementation is required.
//   other functions return S_OK
//
// Use the mouse helper functions in your implementations of OnMouseDown, OnMouseUp
// MouseDown will try and set the focus map (in PageLayout) if successful:
//   sets MouseCapure - mouse move goes to window even if pointer is outside the window)
//   Switches cursor
//   Sets IsMouseDown state to true.
// MouseUp will reset the state from MouseDown
//
// Constructor takes resource ids
//
class CToolBase : public CCommandBase,
                  public ITool,
                  public CToolKeys
{
public:
	CToolBase(UINT bitmapID = 0U,
            _U_STRINGorID caption = 0U,
			_U_STRINGorID name = 0U,
            _U_STRINGorID category = 0U,
            _U_STRINGorID tooltip = 0U,
            _U_STRINGorID message = 0U,
            _U_STRINGorID helpfile = 0U,
            UINT helpID = 0U,
            _U_CURSORorID cursor = 0U,
            _U_CURSORorID cursorMove = 0U,
			bool bDeleteCursor = true);

	virtual ~CToolBase();

	// ICommand
	STDMETHOD(OnClick)();
	// ITool
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
protected:
	//
	// Helper functions
	//
	HRESULT SetFocusMap(LONG X, LONG Y);              // Use this to switch the focus map on a pagelayout 
	bool MouseDown(LONG Button, LONG X, LONG Y);      // Sets focus map (if possible) and Starts mouse capture
	void MouseUp(LONG Button);                        // Finishes mouse capture
	bool IsMouseDown() { return ( GetMouseStatus() ) ? true : false; }
	void SetMouseStatus( LONG Button, bool bIsDown ); /// �}�E�X�̉��������󋵂��Z�b�g����
	uiMouseEventStatus GetMouseStatus() const { return m_emMouseStatus; }

public:
	/// �I�[�o�[�h���b�O�p�̃��b�Z�[�W���[�v���J�n����
	void MessageLoop( long Button );
protected:
	HCURSOR            m_hCursor;
	HCURSOR            m_hCursorMove;
	uiMouseEventStatus m_emMouseStatus; //!< �}�E�X�{�^������������
	bool               m_bDeleteCursor;
	boost::shared_ptr<ZooMoveController>  m_spZooMove;   //!< �g��k���E�X�N���[���R���g���[��
	POINT m_cMouseDownPoint; //!< �}�E�X�����f�o�C�X���W

	// �e�탂�[�h�ؑ�
	bool m_bEnableMButtonScroll; //!< ���{�^���h���b�O�ŃX�N���[���i�f�t�H���g�Ftrue�j
	bool m_bEnableOverDrag; //!< �n�}��ʂ��z���ăh���b�O�������邩�ǂ����i�f�t�H���g�Ftrue�j
};

#endif 
