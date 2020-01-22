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

// AttributeToolObj.cpp : CAttributeToolObj �̃C���v�������e�[�V����
#include "stdafx.h"
#include <algorithm>
#include "AttributeTool.h"
#include "AttributeToolObj.h"
#include <WinLib/WaitCursor.h>
#include "Map.h"

/////////////////////////////////////////////////////////////////////////////
//
// IActiveViewEvents �C���^�[�t�F�[�X
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeToolObj::ContentsChanged()
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::ContentsCleared()
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::ItemAdded(VARIANT Item)
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::ItemDeleted(VARIANT Item)
{
  return S_OK;	
}
STDMETHODIMP CAttributeToolObj::ItemReordered(VARIANT Item, LONG toIndex)
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::SelectionChanged()
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::ViewRefreshed(IActiveView * View, esriViewDrawPhase phase, VARIANT data, IEnvelope * Envelope)
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::AfterDraw(IDisplay * Display, esriViewDrawPhase phase)
{
	switch (phase)
	{
		case esriViewNone:
			break;
		case esriViewBackground:
			break;
		case esriViewGeography:
			break;
		case esriViewGeoSelection:
			break;
		case esriViewGraphics:
			break;
		case esriViewGraphicSelection:
			break;
		case esriViewForeground:
			if( m_bDrawingSelectArea )
			{
				OLE_HANDLE hDC = 0;
				if( Display )
					Display->get_hDC( &hDC );
				m_cFeedback.ResetSearchArea( hDC );
			}
			break;
		default:
			break;
	}

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::AfterItemDraw(SHORT Index, IDisplay * Display, esriDrawPhase phase)
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::FocusMapChanged()
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::SpatialReferenceChanged()
{
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
// ICommand�C���^�[�t�F�[�X
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeToolObj::get_Enabled(VARIANT_BOOL * Enabled)
{
	if (Enabled == NULL)
		return E_POINTER;

	// �����\���ҏW�c�[���́A���Enable
	*Enabled = VARIANT_TRUE;
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_Checked(VARIANT_BOOL * Checked)
{
	if (Checked == NULL)
		return E_POINTER;
		
	*Checked = VARIANT_FALSE;	// Tool is not checked
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_Name(BSTR * Name)
{
	if (Name == NULL)
		return E_POINTER;
		
	if( m_lSubType == 1 )
		*Name = ::SysAllocString(L"SiNDYe_AttributeTool");
	else if( m_lSubType == 2 )
		*Name = ::SysAllocString(L"SiNDYe_LQAttributeTool");

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_Caption(BSTR * Caption)
{
	if (Caption == NULL)
		return E_POINTER;
	
	CComBSTR bstrName, bstrLQName;
	bstrName.LoadString( IDS_NAME );
	bstrLQName.LoadString( IDS_LQNAME );
	if( m_lSubType == 1 )
		*Caption = ::SysAllocString( bstrName );
	else if( m_lSubType == 2 )
		*Caption = ::SysAllocString( bstrLQName );

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_Tooltip(BSTR * Tooltip)
{
	if (Tooltip == NULL)
		return E_POINTER;
		
	CComBSTR bstrToolTip, bstrLQToolTip;
	bstrToolTip.LoadString( IDS_TOOLTIP );
	bstrLQToolTip.LoadString( IDS_LQTOOLTIP );
	if( m_lSubType == 1 )
		*Tooltip = ::SysAllocString(bstrToolTip);
	else if( m_lSubType == 2 )
		*Tooltip = ::SysAllocString(bstrLQToolTip);

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_Message(BSTR * Message)
{
	if( !Message )
		return E_POINTER;

	CComBSTR bstrMessage, bstrLQMessage;
	bstrMessage.LoadString( IDS_MESSAGE );
	bstrLQMessage.LoadString( IDS_LQMESSAGE );
	if( m_lSubType == 1 )
		*Message = ::SysAllocString(bstrMessage);
	else if( m_lSubType == 2 )
		*Message = ::SysAllocString(bstrLQMessage);

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_HelpFile(BSTR * HelpFile)
{
	if( !HelpFile )
		return E_POINTER;

	(*HelpFile) = ::SysAllocString(_HELPFILENAME);
		
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_HelpContextID(LONG * helpID)
{
	if( !helpID )
		return E_POINTER;
		
	if( m_lSubType == 1 )
		(*helpID) = _HELPID_ATTRIBUTETOOL;
	else if( m_lSubType == 2 )
		(*helpID) = _HELPID_LQATTRIBUTETOOL;

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_Bitmap(OLE_HANDLE * Bitmap)
{
	if( !Bitmap )
		return E_POINTER;
		
	if( m_lSubType == 1 )
		*Bitmap = (OLE_HANDLE) m_bitmap;
	else if( m_lSubType == 2 )
		*Bitmap = (OLE_HANDLE) m_LQbitmap;

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_Category(BSTR * categoryName)
{
	if( !categoryName )
		return E_POINTER;
		
	*categoryName = ::SysAllocString(L"SiNDY-e Tool");
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnCreate(IDispatch * hook)
{
	// ArcMap����ł��g�p�ł���悤��
	IDispatchPtr ipDisp( hook );
	m_cArcHelper.Init( ipDisp );
	LoadProperty();
	return S_OK;
} 
STDMETHODIMP CAttributeToolObj::OnClick()
{
	// DocumentEvents, ActiveViewEvents�ɐڑ�
	m_cDocumentEvents = m_cArcHelper.GetDocument();
	m_cDocumentEvents.Advise( GetUnknown() );
	m_cActiveViewEvents = m_cArcHelper.GetActiveView();
	m_cActiveViewEvents.Advise( GetUnknown() );

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
// ICommandSubType�C���^�[�t�F�[�X
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeToolObj::SetSubType(LONG SubType)
{
	m_lSubType = SubType;
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::GetCount(LONG * Count)
{
	*Count = 2;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
// ITool�C���^�[�t�F�[�X
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeToolObj::get_Cursor(OLE_HANDLE * Cursor)
{
	if( !Cursor )
		return E_POINTER;

	*Cursor = (OLE_HANDLE)(*m_pCurPointer);

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnMouseDown(LONG Button, LONG Shift, LONG X, LONG Y)
{
	// �N���b�N�|�C���g�擾
	m_lMouseDownX = X;
	m_lMouseDownY = Y;

	// �I��͈͂̃t�B�[�h�o�b�N���Ȃ�t�B�[�h�o�b�N���Ƃ߂�
	if( m_bDrawingSelectArea )
	{
		// �I����`�̕`���~
		m_cFeedback.StopDrawSearchArea();
		m_bDrawingSelectArea = false;
	}

	if (Button == 1) {	
		// �N���b�N���ꂽ�_�����݂̍��W�n�Ŏ擾
		AheQueryMapPoint( m_cArcHelper.GetScreenDisplay(), X, Y, &m_ipMouseDownPoint );	
		m_bIsMouseDown = true;
	}
	else if( Button == 2 )
	{
		// ���N���b�N��������Ă��Ȃ��Ƃ������A�E�N���b�N���I���ɂ���
		if( !m_bIsMouseDown )
			m_bIsRightDown = true;
		// �E�N���b�N�ŋ�`�I���L�����Z�������Ƃ��ɂ́A���{�^���������Ă��Ă��A�����Ă��Ȃ���Ԃɂ��Ă���
		m_bIsMouseDown = false;
		if( m_ipFeedbackEnv )
		{
			IEnvelopePtr ipSearchExtent;
			m_ipFeedbackEnv->Stop(&ipSearchExtent);
			m_ipFeedbackEnv = NULL;
		}
	}

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnMouseMove(LONG Button, LONG Shift, LONG X, LONG Y)
{
	IPointPtr ipPoint;
	AheQueryMapPoint( m_cArcHelper.GetScreenDisplay(), X, Y, &ipPoint);

	if( m_bFreeSelectMode )
	{
		if( m_lSubType == 1 )
			m_pCurPointer = &m_curFreeSel;
		else if( m_lSubType == 2 )
			m_pCurPointer = &m_LQcurFreeSel;
	}

	if( m_bIsMouseDown )
	{
		/// �}�E�X�|�C���^�ύX�i��`�I���j
		if( !m_bFreeSelectMode )
		{
			if( m_lSubType == 1 )
				m_pCurPointer = &m_curMove;
			else if( m_lSubType == 2 )
				m_pCurPointer = &m_LQcurMove;
		}

		/// �I����`�`��
		if( !m_bFreeSelectMode )
		{
			if( Button == 1 || Button == 3)
			{
				if (m_ipFeedbackEnv == NULL)
				{
					m_ipFeedbackEnv.CreateInstance(CLSID_NewEnvelopeFeedback);
					m_ipFeedbackEnv->putref_Display(m_cArcHelper.GetScreenDisplay());
					m_ipFeedbackEnv->Start(m_ipMouseDownPoint);
				}
				m_ipFeedbackEnv->MoveTo(ipPoint);
			}
		}
	} 
	else
	{
		// �}�E�X�|�C���^
		if( !m_bFreeSelectMode )
		{
			if( m_lSubType == 1 )
				m_pCurPointer = &m_cursor;
			else if( m_lSubType == 2 )
				m_pCurPointer = &m_LQcursor;
		}

		// �I��̈��`�`��
		if( !m_bFreeSelectMode )
		{
			if( m_bDrawingSelectArea )
			{
				// �`��
				if( !m_cFeedback.DrawSearchArea( ipPoint ) )
					m_cFeedback.StartDrawSearchArea( m_cArcHelper.GetScreenDisplay(), ipPoint, AheGetTolerance() );
			}
			else
			{
				m_cFeedback.StartDrawSearchArea( m_cArcHelper.GetScreenDisplay(), ipPoint, AheGetTolerance() );
				m_bDrawingSelectArea = true;
			}
		}
		else
			m_cSketchFeedback.MoveTo( ipPoint );
	}

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnMouseUp(LONG Button, LONG Shift, LONG X, LONG Y)
{
	// �R���e�L�X�g���j���[���o�����ǂ���
	// OnMouseUp�̍Ō�ł��Ǝ��R�I�����[�h�̂Ƃ��ɍŌ��1�_���������Ƃ���
	// IsStartingFeedback()��false�ɂȂ��Ă���A�R���e�L�X�g���j���[���o�Ă��܂����߁A�擪�Ŕ��肵�Ă���
	// �܂��A���N���b�N�������Ă��Ȃ��āA�E�N���b�N�������Ă���Ƃ������ɂ���i�܁[�A������O�Ȃ̂����A���ꂪ�d�v�j
	if( Button == 2 && m_cSketchFeedback.IsStartingFeedback() == FALSE && !m_bIsMouseDown && m_bIsRightDown)
		m_bShowContextMenu = true;
	else
		m_bShowContextMenu = false;

	if( m_bIsRightDown && Button == 2)
		m_bIsRightDown = false;

	// �O�̂��߂Ƀ}�E�X��������Ă��邩�ǂ��������ƃ`�F�b�N�i�_�u���N���b�N�̂Ƃ��Ȃǂ��l�����āj
	IPointPtr ipPoint;
	AheQueryMapPoint( m_cArcHelper.GetScreenDisplay(), X, Y, &ipPoint);
	if( m_bIsMouseDown && Button == 1)
	{
		m_bIsMouseDown = false;
		if( !m_bFreeSelectMode )
		{
			// �I���A�C�e���̌��������Ń��Z�b�g
			m_lSearchItemNum = 0;
			if( m_lSubType == 1 )
			{
				m_pCurPointer = &m_cursor;
				OnAttrMouseUp(Button, Shift, X, Y );
			}
			else if( m_lSubType == 2 )
			{	
				m_pCurPointer = &m_LQcursor;
				OnLQMouseUp(Button, Shift, X, Y );
			}
		}
		else
		{
			// �|���S���I��1�_�ڂ̓t�B�[�h�o�b�N�J�n����
			if( !m_cSketchFeedback.IsStartingFeedback() )
			{
				m_cSketchFeedback.SetDisplay( m_cArcHelper.GetScreenDisplay() );
				m_cSketchFeedback.Start( ipPoint, sindyeSketch_Polygon );
			}
			// �G���A�|�C���g�ǉ��i2�_�ڈȍ~�j
			else
				m_cSketchFeedback.AddPoint( ipPoint );
		}
	}
	// �E�N���b�N�œ_�폜�BRemovePoint�ɓn���_�͏����_�ł͂Ȃ��āA������_��n���B�����ƁA�Ō�̓_�������Ă����
	if( Button == 2 && m_bFreeSelectMode == true )
		m_cSketchFeedback.RemovePoint( ipPoint );

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnDblClick()
{
	if( m_bFreeSelectMode )
	{
		// �t�B�[�h�o�b�N����`����擾
		// �E�_�u���N���b�N�ł��ꉞ���Ȃ��ɂ��Ă���
		m_ipSelectGeom = m_cSketchFeedback.Stop();
		if( m_ipSelectGeom )
		{
			((IPolygonPtr)m_ipSelectGeom)->SimplifyPreserveFromTo();
			LONG lRingCount;
			((IPolygonPtr)m_ipSelectGeom)->get_ExteriorRingCount( &lRingCount );
			
			// �s���`��
			CComBSTR bstrSelectInvalid, bstrToolName;
			if( m_lSubType == 1 )
				bstrToolName.LoadString( IDS_NAME );
			else
				bstrToolName.LoadString( IDS_LQNAME );
			bstrSelectInvalid.LoadString( IDS_SELECTINVALID );
			if( lRingCount > 1 )
				AheMessageBox( CString( bstrSelectInvalid ), CString( bstrToolName ), MB_OK, WARNLEVEL_LOW );
			else if( lRingCount == 1 )
			{
				// �I���A�C�e���̌��������Ń��Z�b�g
				m_lSearchItemNum = 0;
				if( m_lSubType == 1 )
				{
					m_pCurPointer = &m_cursor;
					OnAttrMouseUp(0, m_lShift, 0, 0 );
				}
				else if( m_lSubType == 2 )
				{	
					m_pCurPointer = &m_LQcursor;
					OnLQMouseUp(0, m_lShift, 0, 0 );
				}
			}
		}
		// �n���I������珉�������Ă���
		m_ipSelectGeom = NULL;
	}
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnKeyDown(LONG keyCode, LONG Shift)
{
	BOOL blHandled = false;
	IEnvelopePtr ipSearchExtent;
	switch( keyCode )
	{
	case VK_SHIFT:
		m_lShift = 1;
		break;
	case VK_SPACE:
		// �t���O�ƃ}�E�X�J�[�\���ύX
		if( m_bFreeSelectMode )
		{
			m_bFreeSelectMode = false;
			if( m_lSubType == 1 )
				m_pCurPointer = &m_cursor;
			else if( m_lSubType == 2 )
				m_pCurPointer = &m_LQcursor;
		}
		else
		{
			m_bFreeSelectMode = true;
			if( m_lSubType == 1 )
				m_pCurPointer = &m_curFreeSel;
			else if( m_lSubType == 2 )
				m_pCurPointer = &m_LQcurFreeSel;
		}
		SetCursor( *m_pCurPointer );
		// �I��͈͂̃t�B�[�h�o�b�N���Ȃ�t�B�[�h�o�b�N���Ƃ߂�
		if( m_bDrawingSelectArea )
		{
			// �I����`�̕`���~
			m_cFeedback.StopDrawSearchArea();
			m_bDrawingSelectArea = false;
		}
		// ������break���Ȃ��A���������s���Ă���
	case VK_ESCAPE:
		// �t�B�[�h�o�b�N�r���Ȃ�X�y�[�X�L�[������Ă��A�t�B�[�h�o�b�N�Ƃ߂�
		// �I����`�̕`���~
		if( m_ipFeedbackEnv )
		{
			m_ipFeedbackEnv->Stop(&ipSearchExtent);
			m_ipFeedbackEnv = NULL;
		}
		if( m_cSketchFeedback.IsStartingFeedback() )
			m_cSketchFeedback.Stop();
		// ������
		m_bIsMouseDown = false;
		m_bIsRightDown = false;
		m_ipSelectGeom = NULL;
		break;
	default:
		AheZooMoveKeyDownProc( m_cArcHelper.GetMap(), keyCode, Shift, &blHandled);
		break;
	}

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnKeyUp(LONG keyCode, LONG Shift)
{
	if( keyCode == VK_SHIFT )
		m_lShift = 0;

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnContextMenu(LONG X, LONG Y, VARIANT_BOOL * handled)
{
	if (handled == NULL)
		return E_POINTER;

	if ( m_bShowContextMenu )
		(*handled) = VARIANT_FALSE;
	else
		(*handled) = VARIANT_TRUE;

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::Refresh(OLE_HANDLE hDC)
{
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::Deactivate(VARIANT_BOOL * complete)
{
	if( !complete )
		return E_POINTER;

	// �c�[���̐ؑւ̍ۂɃ_�C�A���O���J���Ă�����A�_�C�A���O����Ȃ��Ă͂����Ȃ�
	if( m_ipAttributeDlg != NULL )
		m_ipAttributeDlg->Close();

	// IActiveViewEvents�ڑ�����
	m_cActiveViewEvents.Unadvise();
	// IDocumentEvents�ڑ�����
	m_cDocumentEvents.Unadvise();

	// �t�B�[�h�o�b�N�͕K���~�߂邱��
	if( m_bDrawingSelectArea ) {
		// �I����`�̕`���~
		m_cFeedback.StopDrawSearchArea();
		m_bDrawingSelectArea = false;
	}
	
	*complete = VARIANT_TRUE;
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
// IDocumentEvents�C���^�[�t�F�[�X
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeToolObj::ActiveViewChanged()
{
	// �f�[�^�r���[�̃��C�A�E�g�r���[�Ȃǂ̃r���[�̐؂�ւ����������ꍇ�ɁAActiveViewEvents��ݒ肵�Ȃ���
	// IActiveViewEvents�ڑ�����
	m_cActiveViewEvents.Unadvise();
	// IActiveViewEvents�ڑ����Ȃ���
	m_cActiveViewEvents = m_cArcHelper.GetActiveView();
	m_cActiveViewEvents.Advise( GetUnknown() );

	// �t�B�[�h�o�b�N���Ă�����~�߂�
	m_cFeedback.StopDrawSearchArea();

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::MapsChanged()
{
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::NewDocument()
{
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OpenDocument()
{
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::BeforeCloseDocument(VARIANT_BOOL * abortClose)
{
	if (abortClose == NULL)
		return S_OK;
		
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::CloseDocument()
{
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolObj�����o�֐�
//
/////////////////////////////////////////////////////////////////////////////
// �t�B�[�`���I���p��OnMouseUp�֐�
void CAttributeToolObj::OnAttrMouseUp(LONG Button, LONG Shift, LONG X, LONG Y)
{
	IGeometryPtr ipGeom;
	if( !m_bFreeSelectMode )
	{
		IPointPtr ipPoint;
		AheQueryMapPoint( m_cArcHelper.GetScreenDisplay(), X, Y, &ipPoint);

		// ��`�I��
		if( m_ipFeedbackEnv ) 
		{
			IEnvelopePtr ipSearchExtent;
			m_ipFeedbackEnv->Stop(&ipSearchExtent);
			m_ipFeedbackEnv = NULL;
			ipGeom = ipSearchExtent;
		}

		// MouseDown,MouseUp�̍��W�̍������������iX,Y�̍��̍��v�l��2�ȉ��j�ɂ́A��`�I���ł͂Ȃ�
		// �N���b�N�������Ƃɂ��Ă����B[Bug4319]���܂ɑI���ł��Ȃ��Ƃ�������ւ̑Ή�
		if( abs( m_lMouseDownX - X ) + abs( m_lMouseDownY - Y ) <= 2 )
		{
			UINT unTorelance = AheGetTolerance();
			// Arc8.2�̃o�O�ɑΉ����邽��[Bug2458]�A1.5�{���炢�͈̔͂Ō�������
			// Arc9�ł��A�܂��ɂ͂Ȃ��������S�ɂ͏C������Ă��Ȃ����߁A���̂܂܂ɂ��Ă���
			unTorelance = unTorelance + ( unTorelance / 2 );
			// �I���G���A�Ƃ��Đݒ肳��Ă���pixel�l���A���݂̊g�僂�[�h�ŕϊ�����
			DOUBLE dSearchRadius = AheConvertPixelsToMapUnits( m_cArcHelper.GetScreenDisplay(), unTorelance );
			// �I���t�B�[�`���i�[�R���|�[�l�b�g�̏�����
			ITopologicalOperatorPtr ipTopo( ipPoint );
			// ���͂��ꂽ�|�C���g��I���G���A���a�Ńo�b�t�@���A�I���G���A���쐬
			ipTopo->Buffer( dSearchRadius, &ipGeom );
		}
	}
	else
		ipGeom = m_ipSelectGeom;

	// [Bug 4872]�I�𒼌�Ɍł܂�Ƃ����o�O�������ɋN��������̂Ȃ̂��ǂ����킩��Ȃ����A���炩�̌����ŁA
	// �I���`�󂪂Ƃ��Ă���Ȃ��ꍇ�ɁA���̐�ɐi�ނƌł܂肻���Ȃ��߁A�����ŃG���[�`�F�b�N���Ă����B
	VARIANT_BOOL vaIsEmpty = VARIANT_TRUE;
	if( ipGeom )
		ipGeom->get_IsEmpty( &vaIsEmpty );
	if( !ipGeom || vaIsEmpty )
		return;

	// �t�B�[�`���I�𒆂ɂ̓J�[�\���������v�ɂ��Ă����i����if���������玩���I�ɁA�O�̃J�[�\���ɖ߂�j
	CWaitCursor cWaitCursor;

	// �����Ώۂ̃��C�����X�g���擾����
	std::list< CAdapt< ILayerPtr > > listLayers = GetSearchLayer(false);
	// 1�����C���Ȃ�������I��
	if( listLayers.size() < 1 ) 
		return;

	// �擾���Ă������C���Ńt�B�[�`������
	bool bSelected = false;
	for( std::list< CAdapt< ILayerPtr > >::iterator it = listLayers.begin(); it != listLayers.end(); ++it )
	{
		bool bTempSelected = SelectByShapeFromLayer( ipGeom, (*it).m_T );
		if( bTempSelected && !bSelected )
			bSelected = true;
		// [Bug 5343]�Ή��ŁA�I�������t�B�[�`����1000�𒴂�����A�_�C�A���O���Ă��܂�
		if( m_lSearchItemNum > 1000 )
		{
			CComBSTR bstrOverMessage, bstrToolName;
			bstrToolName.LoadString( IDS_NAME );
			bstrOverMessage.LoadString( IDS_SELECTITEMOVER );
			AheMessageBox( CString( bstrOverMessage ), bstrToolName, MB_OK, WARNLEVEL_LOW );
			GetAttributeDlg()->Close();
			return;
		}
	}

	if( bSelected )
	{
		// �����\��
		GetAttributeDlg()->put_DoOperation(VARIANT_TRUE);
		GetAttributeDlg()->DoModeless();
	}
}

//�����N��I���p��OnMouseUp�֐�
void CAttributeToolObj::OnLQMouseUp(LONG Button, LONG Shift, LONG X, LONG Y)
{
	IGeometryPtr ipGeom;
	if( !m_bFreeSelectMode )
	{
		// �N���b�N�ł̓����N��͏E���Ȃ��d�l�i�����̂��ȁj
		if( m_ipFeedbackEnv ) 
		{
			IEnvelopePtr ipSearchExtent;
			m_ipFeedbackEnv->Stop(&ipSearchExtent);
			m_ipFeedbackEnv = NULL;
			ipGeom = ipSearchExtent;
		}
		// �N���b�N�����ꍇ�ɂ͂��̂܂܏I��
		if( !ipGeom )
			return;
	}
	else
		ipGeom = m_ipSelectGeom;

	// [Bug 4872]�I�𒼌�Ɍł܂�Ƃ����o�O�������ɋN��������̂Ȃ̂��ǂ����킩��Ȃ����A���炩�̌����ŁA
	// �I���`�󂪂Ƃ��Ă���Ȃ��ꍇ�ɁA���̐�ɐi�ނƌł܂肻���Ȃ��߁A�����ŃG���[�`�F�b�N���Ă����B
	VARIANT_BOOL vaIsEmpty = VARIANT_TRUE;
	if( ipGeom )
		ipGeom->get_IsEmpty( &vaIsEmpty );
	if( !ipGeom || vaIsEmpty )
	{
		CComBSTR bstrErrorSearchGeom, bstrToolName;
		bstrErrorSearchGeom.LoadString( IDS_ERRORSEARCHGEOM );
		bstrToolName.LoadString( IDS_LQNAME );
		AheMessageBox( bstrErrorSearchGeom, bstrToolName, MB_OK, WARNLEVEL_LOW );
		return;
	}

	// �t�B�[�`���I�𒆂ɂ̓J�[�\���������v�ɂ��Ă����i����if���������玩���I�ɁA�O�̃J�[�\���ɖ߂�j
	CWaitCursor cWaitCursor;

	// �����Ώۂ̃��C�����X�g���擾����
	std::list< CAdapt< ILayerPtr > > listLayers = GetSearchLayer(true);
	// 1�����H�����N�Ȃ�������I��
	if( listLayers.size() < 1 ) 
		return;

	// �擾�������ׂĂ̓��H�����N�������������N�񌩂��Ă���
	bool bFind = false, bIsSelected = false;
	for( std::list< CAdapt< ILayerPtr > >::iterator it = listLayers.begin(); it != listLayers.end(); ++it )
	{
		IFeatureRendererPtr ipRend;
		((IGeoFeatureLayerPtr)it->m_T)->get_Renderer( &ipRend );

		// [Bug 6042] �V���{���ύX�r���[�c�[���̃����N��ҏW�{�^���ɑΉ����ė~����
		long lMode = GetEditLinkQueueMode( ipRend );

		// �����N��̎�ނ��ƂɃ��[�v���܂킷
		for( long i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; i++ )
		{
			// �����N�񂪕\������Ă��邩�ǂ����`�F�b�N
			bool bIsHit = false;
			if( m_cLQInfo.GetLoadQueueDrawMode( (sindyeRoadQueueType)i ) & lMode )
			{
				bFind = true;
				// �I�������͈͂̃W�I���g���̒��Ƀ����N�񂪂��邩�ǂ������ׂ�
				bool bIsShift = false;
				if( Shift == 1 )
					bIsShift = true;
				bIsHit = SelectTargetLQs( AheGetFeatureClass( it->m_T ), ipGeom, (sindyeRoadQueueType)i, bIsShift );
			}
			if( bIsHit )
				bIsSelected = true;
		}
	}
	
	// �_�C�A���O�\��
	if( bFind )
	{
		GetAttributeDlg()->SetMode( Shift );
		GetAttributeDlg()->put_DoOperation(VARIANT_TRUE);
		GetAttributeDlg()->DoModeless();
	}
	else
	{
		CComBSTR bstrDisplayLinkQueue, bstrToolName;
		bstrDisplayLinkQueue.LoadString( IDS_DISPLAYLINKQUEUE );
		bstrToolName.LoadString( IDS_LQNAME );
		AheMessageBox( bstrDisplayLinkQueue, bstrToolName, MB_OK, WARNLEVEL_HIGH );
		return;
	}

	if( !bIsSelected )
	{
		CComBSTR bstrNotSelectLinkQueue, bstrToolName;
		bstrNotSelectLinkQueue.LoadString( IDS_NOTSELECTEDLINKQUEUE );
		bstrToolName.LoadString( IDS_LQNAME );
		AheMessageBox( bstrNotSelectLinkQueue, bstrToolName, MB_OK, WARNLEVEL_LOW );
		return;
	}
}

// ���������ׂ����C���̈ꗗ���擾����
std::list< CAdapt<ILayerPtr> > CAttributeToolObj::GetSearchLayer(bool bIsRoadLinkOnly)
{
	std::list< CAdapt< ILayerPtr > > listLayers;

	IMxDocumentPtr ipMxDoc = m_cArcHelper.GetDocument();
	IContentsViewPtr ipContentsView;
	ipMxDoc->get_ContentsView( 0, &ipContentsView );

	// �I���A�C�e�����擾
	std::list< CAdapt<ILayerPtr> > listSelectedLayer;
	CComVariant vaSelectedItem;
	ipContentsView->get_SelectedItem( &vaSelectedItem );
	// [Bug 5424]TOC�Ŕ�\�����C����I�����Ă���ƁA�����Ă���قȂ郌�C����I�����Ă��܂��o�O�őI�����Ă��邯�ǁA
	// _GetSearchLayer�ł͔�\�����C���͖������đI��0�Ƃ��Ă��܂����߁A�I�����Ă��邩�ǂ������t���O�Ŕ��f���Ȃ����Ⴂ���Ȃ�
	bool bSelectItem = false;// ���C���I�����Ă��邩�ǂ���
	if( VT_UNKNOWN == vaSelectedItem.vt)
	{
		// �������C���I�����Ă���ꍇ
		_ISetPtr ipSet( vaSelectedItem );
		if( ipSet )
		{
			bSelectItem = true;
			IUnknownPtr ipUnk;
			ipSet->Reset();
			while( ipSet->Next( &ipUnk ) == S_OK )
			{
				// �O���[�v���C���I�����Ă���ꍇ�ɂ́A���̒��̃��C�����擾
				std::list< CAdapt<ILayerPtr> > listTempLayer = _GetSearchLayer( ipUnk, bIsRoadLinkOnly );
				for( std::list< CAdapt<ILayerPtr> >::iterator it = listTempLayer.begin(); it != listTempLayer.end(); ++it )
					listSelectedLayer.push_back( *it );
			}
		}
		// �ЂƂ������C����I�����Ă���ꍇ
		else if( ((ILayerPtr)vaSelectedItem) )
		{
			bSelectItem = true;
			listSelectedLayer = _GetSearchLayer( (ILayerPtr)vaSelectedItem, bIsRoadLinkOnly );
		}
	}
	
	// [Bug 5407]�����ҏW�_�C�A���O�̕\����TOC�̃��C�����ɂ��Ăق����ɑΉ�
	// ipContentsView->get_SelectedItem�Ŏ���Ă����A�C�e����TOC�̏ォ�珇�ԂƂ͌���Ȃ����߁A
	// ��x��̏����Ŋm�ۂ��Ă����Ă���A�S���[�v���܂킷
	// �W�I�t�B�[�`�����C���������擾����
	IMapPtr ipMap = m_cArcHelper.GetMap();
	IUIDPtr ipUID( CLSID_UID );
	ipUID->put_Value( CComVariant(L"{E156D7E5-22AF-11D3-9F99-00C04F6BC78E}") ); // IGeoFeatureLayer
	// ���ׂĂ̌����Ă���t�B�[�`�����C���Ō���������
	IEnumLayerPtr ipEnumLayer;
	// IMap::get_Layers�͏ォ�珇�ԂɎ����Ă��Ă���Ă���悤���B
	if( SUCCEEDED( ipMap->get_Layers( ipUID, VARIANT_TRUE, &ipEnumLayer ) ) )
	{
		ipEnumLayer->Reset();
		ILayerPtr ipLayer;
		while( S_OK == ipEnumLayer->Next( &ipLayer ) )
		{
			if( IsVisibleLayer( ipLayer ) )
			{
				// �����N��I���̏ꍇ�ɂ͓��H�����N���������X�g�ɂ����
				if( bIsRoadLinkOnly )
				{
					if( AheGetFeatureClassName( ipLayer ).CompareNoCase( ROAD_TABLE_NAME ) == 0 )
					{
						// ���C���I�����Ă��Ȃ�������A���̂܂܃��X�g��
						if( !bSelectItem )
							listLayers.push_back( ipLayer );
						else
						{
							// �I�����Ă��郌�C���Ń��[�v�܂킷
							bool bIsFind = false;
							for( std::list< CAdapt<ILayerPtr> >::iterator it = listSelectedLayer.begin(); it != listSelectedLayer.end(); ++it )
							{
								if( it->m_T == ipLayer )
								{
									bIsFind = true;
									break;
								}
							}
							if( bIsFind )
								listLayers.push_back( ipLayer );
						}
					}
				}
				else
				{
					// ���C���I�����Ă��Ȃ�������A���̂܂܃��X�g��
					if( !bSelectItem )
						listLayers.push_back( ipLayer );
					else
					{
						// �I�����Ă��郌�C���Ń��[�v�܂킷
						bool bIsFind = false;
						for( std::list< CAdapt<ILayerPtr> >::iterator it = listSelectedLayer.begin(); it != listSelectedLayer.end(); ++it )
						{
							if( it->m_T == ipLayer )
							{
								bIsFind = true;
								break;
							}
						}
						if( bIsFind )
							listLayers.push_back( ipLayer );
					}
				}
			}
		}
	}
	return listLayers;
}

// �I�����Ă��郌�C�����猟���p���C���̃��X�g���쐬����
std::list< CAdapt<ILayerPtr> > CAttributeToolObj::_GetSearchLayer(ILayerPtr ipSelectedLayer, bool bIsRoadLinkOnly)
{
	std::list< CAdapt< ILayerPtr > > listLayers;
	// �I������Ă��郌�C�����O���[�v���C���������łȂ����ŏꍇ�킯
	ICompositeLayerPtr ipCompositeLayer( ipSelectedLayer );
	if( !ipCompositeLayer )
	{
		IGeoFeatureLayerPtr ipLayer = ipSelectedLayer;
		if( ipLayer )
		{
			if( IsVisibleLayer( ipLayer ) )
			{
				if( bIsRoadLinkOnly )
				{
					// �I���������C�������H�����N�A���A���C�������Ă��Ȃ��Ƃ����Ȃ�
					if( AheGetFeatureClassName( ipLayer ).CompareNoCase( ROAD_TABLE_NAME ) == 0 )
						listLayers.push_back( (ILayerPtr)ipLayer );
				}
				else
					listLayers.push_back( (ILayerPtr)ipLayer );
			}
		}
	}
	// �I���������C�����O���[�v���C���̏ꍇ
	else
	{
		if( IsVisibleLayer( ipCompositeLayer ) )
		{
			// �O���[�v���C���ȉ��̌����Ă��郌�C�����擾
			std::list< CAdapt< ILayerPtr > > listGroupLayer = GetAllLayerUnderGroup( ipCompositeLayer );
			// �����N��I���̏ꍇ�ɂ͓��H�����N������Ԃ�
			if( bIsRoadLinkOnly )
			{
				for( std::list< CAdapt< ILayerPtr > >::iterator itLayer = listGroupLayer.begin(); itLayer != listGroupLayer.end(); ++itLayer )
				{
					if( AheGetFeatureClassName( (*itLayer).m_T ).CompareNoCase( ROAD_TABLE_NAME ) == 0 )
						listLayers.push_back( (*itLayer).m_T );
				}
			}
			else
				listLayers = listGroupLayer;
		}
	}
	return listLayers;
}

// ���C���������Ă��邩�ǂ������ׂ�
bool CAttributeToolObj::IsVisibleLayer( ILayerPtr ipLayer )
{
	bool bRet = false;
	IMapLayersPtr ipMapLayers( m_cArcHelper.GetMap() );
	VARIANT_BOOL vbVisible = VARIANT_FALSE;
	ipMapLayers->IsLayerVisible( ipLayer, &vbVisible );
	if( vbVisible )
		bRet = true;
	return bRet;
}

// �O���[�v���C���ɓ����Ă��錩���Ă���t�B�[�`�����C�����擾����
std::list< CAdapt<ILayerPtr> > CAttributeToolObj::GetAllLayerUnderGroup(ICompositeLayerPtr ipCompositeLayer)
{
	std::list< CAdapt<ILayerPtr> > listLayer;
	if( !ipCompositeLayer )
		return listLayer;

	long lCompositeLayerNum = 0;
	ipCompositeLayer->get_Count( &lCompositeLayerNum );
	for(long i = 0; i < lCompositeLayerNum; i++)
	{
		ILayerPtr	ipLayer;
		ipCompositeLayer->get_Layer( i, &ipLayer );
		ICompositeLayerPtr ipTempCompositeLayer( ipLayer );
		if( ipTempCompositeLayer )
		{
			// �ċA�Ŏ������g�Ăяo��
			std::list< CAdapt<ILayerPtr> > listTempLayer = GetAllLayerUnderGroup( ipTempCompositeLayer );
			std::copy(listTempLayer.begin(), listTempLayer.end(), std::back_inserter(listLayer));
		}
		else
		{
			// �t�B�[�`�����C���݂̂�����
			IFeatureLayerPtr ipFeatureLayer;
			if( ipLayer->QueryInterface( &ipFeatureLayer ) == S_OK )
			{
				// �����Ă���t�B�[�`�������ɂ��Ă���
				if( IsVisibleLayer( ipFeatureLayer ) )
					listLayer.push_back( (ILayerPtr)ipFeatureLayer );
			}
		}
	}
	return listLayer;
}

// ���C�����������đ����_�C�A���O�ɃZ�b�g����
bool CAttributeToolObj::SelectByShapeFromLayer( IGeometryPtr ipGeom, ILayerPtr ipLayer )
{
	bool bSelected = false;
	// ��ԎQ�Ƃ��킹��
	IGeoDatasetPtr ipGeoDataset = ipLayer;
	ISpatialReferencePtr ipSpRef;
	if( ipGeoDataset )
		ipGeoDataset->get_SpatialReference( &ipSpRef );
	if( ipSpRef != NULL )
	{
		ipGeom->putref_SpatialReference( ipSpRef );
		ipGeom->SnapToSpatialReference();
	}

	// TOC �Ń��C�����I������Ă���ꍇ�͂��̃��C�����݂̂Ō���
	IFeatureCursorPtr ipCursor = AheSelectByShapeFromLayer( ipGeom, ipLayer, esriSpatialRelIntersects );
	if( AddFeatureToDialog(ipCursor) )
		bSelected = true;

	return bSelected;
}

// ���H�����N�ɂ��Ă��郊���N�����������
bool CAttributeToolObj::SelectTargetLQs( IFeatureClassPtr ipRoadClass, IGeometryPtr ipGeom, sindyeRoadQueueType type, bool bLastLink )
{
	// �n���ꂽ���C�����烏�[�N�X�y�[�X�擾
	IWorkspacePtr ipWorkspace;
	IDatasetPtr ipDataset( ipRoadClass );
	if( ipDataset )
		ipDataset->get_Workspace( &ipWorkspace );
	if( !ipWorkspace )
		return false;

	CString strOwnerName = AheGetFeatureClassOwnerName( ipDataset );
	CString strInfoTableName,strLQTableName;
	if( !strOwnerName.IsEmpty() )
	{
		strOwnerName += _T(".");
		strInfoTableName = strOwnerName;
		strLQTableName = strOwnerName;
	}

	CLoadQueueInfo cLQInfo;
	strInfoTableName += cLQInfo.GetLoadQueueInfo( type, sindyeLoadQueueInfoInfoTableName );
	strLQTableName += cLQInfo.GetLoadQueueInfo( type, sindyeLoadQueueInfoLQTableName );

	ITablePtr ipInfoTable = GetTable(ipRoadClass, strInfoTableName, sindy::schema::inf_ipcvics::kTableName );
	ITablePtr ipLQTable = GetTable(ipRoadClass, strLQTableName, sindy::schema::lq_ipcvics::kTableName );

	long lDrawMode = -1;
	// �i�[�p���X�g
	std::list<ROWDEF> listInfoRows, listLQRows, listLQRoads, listRoads;
	// ���H
	CString strRoadRelFieldName = cLQInfo.GetLoadQueueInfo( type, sindyeLoadQueueInfoLQRFRoadFieldName );
	if( ( type == sindyeRoadQueueLane || type == sindyeRoadQueueDirguide || type == sindyeRoadQueueTurnreg ) &&
		( m_bPerfectSelect == TRUE || m_bIsSelectedLastSequence == TRUE ) )
	{
		if( m_bPerfectSelect && !bLastLink )
		{
			std::list<ROWDEF> listInfoRowsTmp, listLQRowsTmp, listLQRoadsTmp, listRoadsTmp;

			SelectInfoRowByShape( ipRoadClass, ipInfoTable, ipLQTable, ipGeom, lDrawMode, listInfoRowsTmp, listRoads );
			AheSelectLQRowByInfoRows( listInfoRowsTmp, ipLQTable, listLQRowsTmp );
			// SEQUENCE=1 �������� LQ �̓��H�����N ID ���擾����
			for( std::list<ROWDEF>::iterator it = listLQRowsTmp.begin(); it != listLQRowsTmp.end(); it++ )
			{
				CComVariant vaSequence = AheGetAttribute( it->obj, SEQUENCE_FIELD );
				if( vaSequence.vt == VT_I4 && vaSequence.lVal == 1 )
				{
					CComVariant vaRoadID = AheGetAttribute( it->obj, LINK_ID_FIELD );
					if( vaRoadID.vt == VT_I4 )
					{
						ROWDEF def;
							
						def.flg = lDrawMode;
						def.oid = vaRoadID.lVal;
						def.sequence = 1;

						listRoadsTmp.push_back( def );
					}
					else
						ATLASSERT( vaRoadID.vt == VT_I4 );
				}
			}
			AheSelectInfoRowByRoadIDs( listRoadsTmp, ipInfoTable, ipLQTable, lDrawMode, listInfoRows );
		}
		// ���o�����N�݂̂̃����N���I������ꍇ
		else if( m_bIsSelectedLastSequence || bLastLink )
			SelectInfoRowByShape( ipRoadClass, ipInfoTable, ipLQTable, ipGeom, lDrawMode, listInfoRows, listRoads, 1, bLastLink );
	}
	else
		SelectInfoRowByShape( ipRoadClass, ipInfoTable, ipLQTable, ipGeom, lDrawMode, listInfoRows, listRoads );

	AheSelectLQRowByInfoRows( listInfoRows, ipLQTable, listLQRows );
	AheSelectLQRoad( ipRoadClass, listLQRows, listRoads, listLQRoads );
	
	// AheGetLQRoadGeometry()���ĂԑO��listLQRows���V�[�P���X�Ń\�[�g���Ă����K�v�L��I�I
	listLQRows.sort();

	// �_�C�A���O�Ƀ����N��ƃW�I���g�����Z�b�g
	bool bIsHit = false;
	for( std::list<ROWDEF>::iterator it = listInfoRows.begin(); it != listInfoRows.end(); it++ )
	{
		BOOL bFixFlow = FALSE;
		IGeometryPtr ipRoadGeom = AheGetLQRoadGeometry( it->oid, listLQRows, listLQRoads, &bFixFlow );
		if( !bFixFlow )
		{
			ATLASSERT( bFixFlow == TRUE );
			ATLTRACE(_T("CLQAttributeToolObj::SelectTargetLQs() : �����N��̕���������ł��܂���ł����iOBJECTID = %d�j\n"), it->oid );
		}
		// �֘A���� LQ ���擾���ăZ�b�g����
		_ISetPtr ipLQRows( CLSID_Set );
		for( std::list<ROWDEF>::iterator itLQs = listLQRows.begin(); itLQs != listLQRows.end(); itLQs++ )
		{
			CComVariant vaValue = AheGetAttribute( itLQs->obj, INF_ID_FIELD );
			if( vaValue.vt == VT_I4 && vaValue.lVal == it->oid )
				ipLQRows->Add( itLQs->obj );
		}
		GetAttributeDlg()->SetItem( it->obj, ipLQRows, ipRoadGeom, VARIANT_FALSE );
		bIsHit = true;
		m_lSearchItemNum++;
	}

	return bIsHit;
}

// �����_�C�A���O���쐬����Ă��Ȃ�������쐬����
IAttributeDialogPtr CAttributeToolObj::GetAttributeDlg()
{
	if( !m_ipAttributeDlg )
	{
		// �����\���_�C�A���O�쐬
		m_ipAttributeDlg.CreateInstance( CLSID_AttributeDlgObj );
		m_ipAttributeDlg->Init( m_cArcHelper.GetApp() );
	}

	return m_ipAttributeDlg;
}

// �����_�C�A���O�ɑI�������t�B�[�`����n��
bool CAttributeToolObj::AddFeatureToDialog( IFeatureCursorPtr ipFeatureCursor )
{
	bool bSelected = false;
	if( ipFeatureCursor ) 
	{
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK && ipFeature )
		{
			// [Bug4768]�����Ă���t�B�[�`���̂ݒǉ�����
			if( !AheIsVisibleFeature( m_cArcHelper.GetMap(), ipFeature ) )
				continue;

			// �����_�C�A���O�Ƀt�B�[�`����n��
			// ��4�����̒l��LQ�_�C�A���O�Ŏg�����߁A�����_�C�A���O�̏ꍇ��TRUE�ł�FALSE�ł��Ȃ�ł��悢
			GetAttributeDlg()->SetItem( ipFeature, NULL, NULL, VARIANT_FALSE );
			bSelected = true;
			m_lSearchItemNum++;
		}
	}

	return bSelected;
}

// �����ҏW�̃v���p�e�B�̒l���ύX���ꂽ��A���W�X�g���ɓo�^����\n
void CAttributeToolObj::WriteProperty( BOOL bIsPerfectSelect, BOOL bIsSelectedLastSequence )
{
	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );

	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey ) {
		// ���ʁA���[���̏ꍇ�Ɋ����Ɏ擾���邩�ǂ���
		if( bIsPerfectSelect == TRUE )
			EmxSetRegDword( _T("IsPerfectSelect"), (DWORD)m_bPerfectSelect, hOpenKey );
		// ���ʁA���[���̏ꍇ�ɋ�`�I���������H�����N�𗬓������N�Ƃ��郊���N���I�����邩�ǂ���
		if( bIsSelectedLastSequence == TRUE )
			EmxSetRegDword( _T("IsSelectedLastSequence"), (DWORD)m_bIsSelectedLastSequence, hOpenKey );

		RegCloseKey( hOpenKey );
	}
}

// �N�����Ƀ��W�X�g���ɓo�^����Ă���v���p�e�B�̓��e��ǂݎ��
void CAttributeToolObj::LoadProperty()
{
	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );

	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey ) {
		DWORD dwResult;
		// ���ʁA���[���̏ꍇ�Ɋ����Ɏ擾���邩�ǂ���
		if( EmxGetRegDword( _T("IsPerfectSelect"), &dwResult, hOpenKey ) == TRUE )
			m_bPerfectSelect = (BOOL)dwResult;
		// ���ʁA���[���̏ꍇ�ɋ�`�I���������H�����N�𗬓������N�Ƃ��郊���N���I�����邩�ǂ���
		if( EmxGetRegDword( _T("IsSelectedLastSequence"), &dwResult, hOpenKey ) == TRUE )
			m_bIsSelectedLastSequence = (BOOL)dwResult;

		RegCloseKey( hOpenKey );
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// IAttributeToolObj �C���^�[�t�F�[�X��`
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeToolObj::get_PerfectSelect(VARIANT_BOOL *pVal)
{
	if( !pVal )
		return E_POINTER;

	*pVal = ( m_bPerfectSelect == TRUE ? VARIANT_TRUE : VARIANT_FALSE );

	return S_OK;
}

STDMETHODIMP CAttributeToolObj::put_PerfectSelect(VARIANT_BOOL newVal)
{
	m_bPerfectSelect = ( newVal == VARIANT_TRUE ? TRUE : FALSE );
	WriteProperty( TRUE, FALSE );

	return S_OK;
}

STDMETHODIMP CAttributeToolObj::get_SelectLastSequence(VARIANT_BOOL *pVal)
{
	if( !pVal )
		return E_POINTER;

	*pVal = ( m_bIsSelectedLastSequence == TRUE ? VARIANT_TRUE : VARIANT_FALSE );

	return S_OK;
}

STDMETHODIMP CAttributeToolObj::put_SelectLastSequence(VARIANT_BOOL newVal)
{
	m_bIsSelectedLastSequence = ( newVal == VARIANT_TRUE ? TRUE : FALSE );
	WriteProperty( FALSE, TRUE );

	return S_OK;
}

// �ҏW�\�ȃ����N��̃��[�h�擾
// [Bug 6042] �V���{���ύX�r���[�c�[���̃����N��ҏW�{�^���ɑΉ����ė~����
long CAttributeToolObj::GetEditLinkQueueMode(IFeatureRenderer* ipRednerer)
{
	IRoadRendererPtr ipRoadR( ipRednerer );

	long lDrawMode = 0;

	if ( ipRoadR )
		ipRoadR->get_EditMode( &lDrawMode );	// �ҏW���[�h�擾

	// �����N��Ɋ֌W�Ȃ����[�h�͏��O�B
	lDrawMode = lDrawMode & ~(_MULTISYMBOL_ENDPOINT | _MULTISYMBOL_VERTEX | _MULTISYMBOL_ONEWAY | _MULTISYMBOL_NOPASSAGE | _MULTISYMBOL_ANNOTATION | _MULTISYMBOL_S1ANNOPOINT | _MULTISYMBOL_S2ANNOPOINT | _MULTISYMBOL_S3ANNOPOINT | _MULTISYMBOL_S4ANNOPOINT);

	if ( ipRoadR ) {
		if ( lDrawMode <= 0 ) {	// �ҏWOK�ȃ��[�h���Ȃ����Ă��Ƃ͑I�����Ă��Ȃ�����\�����[�h���擾
			ipRoadR->get_DrawMode( &lDrawMode );	// �\�����[�h�擾

			// �����N��Ɋ֌W�Ȃ����[�h�͏��O�B
			lDrawMode = lDrawMode & ~(_MULTISYMBOL_ENDPOINT | _MULTISYMBOL_VERTEX | _MULTISYMBOL_ONEWAY | _MULTISYMBOL_NOPASSAGE | _MULTISYMBOL_ANNOTATION | _MULTISYMBOL_S1ANNOPOINT | _MULTISYMBOL_S2ANNOPOINT | _MULTISYMBOL_S3ANNOPOINT | _MULTISYMBOL_S4ANNOPOINT);
		}
	}

	return lDrawMode;
}

// �����N��e�[�u���擾
ITablePtr CAttributeToolObj::GetTable(const ITablePtr& ipTable, LPCTSTR lpcszName, LPCTSTR lpcszExName)
{
	CString strTable( lpcszName );

	// �I�[�i���t���̃e�[�u�����̎��̓I�[�i�������
	if( _tcschr( lpcszName, _T('.') ) )
	{
		int nPeriodIndex = strTable.Find( _T('.'), 0 );
		strTable = strTable.Right( strTable.GetLength() - nPeriodIndex - 1 );
	}

	// [Bug 6429]ronly�Őڑ����Ă��铹�H�̃����N�񂪕\������Ȃ�
	// ���H�Ɠ����I�[�i�[�̃e�[�u�����I�[�v������
	ITablePtr ipRet = AheOpenSameOwnerTable( ipTable, strTable );

	if ( ! ipRet )
	{
		// [Bug 10196]�ʂ̃T�[�o�ɂ���iPC_VICS��\���ł���悤�ɂ���
		// TOC�ɓ��H�Ɠ����f�[�^�Z�b�g��INF_***���ǂݍ��܂�Ă��邩�m�F
		// �����IPCVICS���݂̂ɂ���
		if ( 0 == strTable.CompareNoCase( lpcszExName ) )
			ipRet = GetSameTable(strTable);
	}

	return ipRet;
}

// TOC�ɂ���w�肵���e�[�u��������ITable���擾
ITablePtr CAttributeToolObj::GetSameTable(LPCTSTR lpcszName, LPCTSTR lpcszOwner)
{
	// TOC�ɓǂݍ��܂�Ă���LQ_IPCVICS��ǂݍ���
	CMap cMap( m_cArcHelper.GetFocusMap() );
	ITableCollectionPtr ipTables( (IMap*)cMap );

	if ( ! ipTables )
		return nullptr;

	CString strOwner( lpcszOwner );
	strOwner.MakeLower();

	long nCount = 0;
	ipTables->get_TableCount( &nCount );

	for ( long i = 0; i < nCount; ++i )
	{
		ITablePtr ipTable;
		ipTables->get_Table( i, &ipTable );

		if ( ! ipTable )
			continue;

		CComBSTR bstrName;
		AheGetLayerTableName( (IMap*)cMap, ipTable, &bstrName );

		CString strName( bstrName );
		if ( 0 == strName.CompareNoCase( lpcszName ) )
		{
			if ( lpcszOwner && _tcslen(lpcszOwner) > 0 )
			{
				if ( GetOwnerName(ipTable).CompareNoCase( strOwner ) == 0 )
					return ipTable;
			}
			else
				return ipTable;
		}
	}

	return nullptr;
}

// �I�[�i�����擾
CString CAttributeToolObj::GetOwnerName(const ITablePtr& ipTable)
{
	CString strName( AheGetFeatureClassOwnerName(ipTable) );

	if ( strName.IsEmpty() )
	{
		// ��̎��̓p�X���擾
		IWorkspacePtr ipWork( AheGetWorkspace( ipTable ) );
		if ( ! ipWork )
			return strName;

		CComBSTR bstrName;
		ipWork->get_PathName( &bstrName );
		strName = bstrName;
	}

	return strName;
}

long CAttributeToolObj::SelectInfoRowByShape( const IFeatureClassPtr& ipRoadClass, const ITablePtr& ipInfoTable, const ITablePtr& ipLQTable, const IGeometryPtr& ipGeom, long lDrawMode, std::list<ROWDEF>& listInfoRows, std::list<ROWDEF>& listRoads, long lSequence/* = -1*/, bool bLastLink )
{
	if( ! ipRoadClass || ! ipInfoTable || ! ipLQTable || ! ipGeom )
		return 0;

	// ���H�����N�t�B�[�`����I��
	IFeatureCursorPtr ipFeatureCursor = AheSelectByShape( ipGeom, ipRoadClass, esriSpatialRelIntersects );
	if( ! ipFeatureCursor )
		return 0;

	// LQ �e�[�u�����������邽�߂̃N�G���t�B���^�쐬
	IFeaturePtr ipFeature;
	long lCount = 0;

	while( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK && ipFeature )
	{
		long lOID = -1;
		ipFeature->get_OID( &lOID );

		IGeometryPtr ipGeom;
		ipFeature->get_ShapeCopy( &ipGeom );

		ROWDEF rowDef;
		rowDef.oid = lOID;
		rowDef.shp = ipGeom;
		rowDef.obj = ipFeature;
		rowDef.flg = lDrawMode;

		if( lSequence != -1 )
			rowDef.sequence = lSequence;

		listRoads.push_back( rowDef );
	}

	return AheSelectInfoRowByRoadIDs( listRoads, ipInfoTable, ipLQTable, lDrawMode, listInfoRows, bLastLink );
}
