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

#pragma once

// WinLib
#include "type_convert.h"
// SiNDYLib
#include "SketchFeedback.h"
// ArcHelperEx
#include "define.h"
#include "AheGlobals.h"
#include "AheDraw.h"
#include "GlobalFunctions.h"
#include "AheSymbolFunctions.h"
#include "ArcHelperEx.h"
// COMMON
#include "Feedback.h"
// ArcMapUIBase
#include "ToolBase.h"
#include "../ArcHelperEx/LinkSnapTrace.h"

enum mode_type {
	type_point,
	type_line_trace,
	type_real_line_trace,
	type_real_line_trace2,
	type_area,
	type_envelope,
	type_polyline,
	type_multi_polyline,
	type_polygon,
	type_realtime_line_trace,
	//type_sketch,
};

ISymbolPtr AheCreateArrowLineSymbol( COLORREF rgb );
bool AheEquals( IGeometry* ipGeom1, IGeometry* ipGeom2 );

/**
 * @brief �����̃W�I���g���̒[�_���m���ڂ��Ă��邩�ǂ����̔���
 * 
 * �o���̒[�_�������Ƃ��ڂ��Ă���ꍇ�i���[�v����ꍇ�j��false�ɂȂ�܂�
 */
bool AheIsTouch_EndPoint( IGeometry* ipGeom1, IGeometry* ipGeom2 );

/**
 * @brief �|�C���g���������̒[�_�Ɛڂ��Ă��邩�ǂ����̔���
 *
 * ���ɑ������̃W�I���g�������[�v���Ă��Ă��G���[�Ƃ͂Ȃ�܂���
 */
bool AheIsTouch_EndPointAndPoint( IGeometry* ipGeom, IPoint* ipPoint );

/**
 * @brief �Е��̃W�I���g���̒��ԓ_�Ƃ����Е��̃W�I���g���̒[�_���ڂ��Ă��邩�ǂ����̔���
 * 
 * ���ԓ_����r�����̂͑������A�[�_����r�����̂͑������ƂȂ�܂��B
 */
bool AheIsTouch_MidwayPointAndEndPoint( IGeometry* ipGeom1, IGeometry* ipGeom2 );

/**
 * @brief ipGeom1��ipGeom2�����[�v�����Őڑ����Ă��邩�ǂ����̔���
 */
bool AheIsTouch_EndPointNoRoop( IGeometry* ipGeom1, IGeometry* ipGeom2 );

double AheCalcDistance( IGeometry* ipGeom1, IGeometry* ipGeom2 );

IGeometryPtr ConstructUnion( const std::list<CAdapt<IGeometryPtr> >& listGeometry );

bool IsInvalidateGeometry(IGeometry* ipGeom);

/**
 * @class CToolPointer
 * @brief �}�E�X����C�x���g�����ۂɏ�������N���X�̃x�[�X�N���X
 * �S�Ẵ}�E�X����C�x���g�����N���X�͂��̃N���X���p������K�v������܂�
 */
class CToolPointer
{
public:
	CToolPointer();
	CToolPointer( IApplication* ipApp, bool bWithArea=false );
	virtual ~CToolPointer();
	virtual HRESULT OnMouseDown(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled);
	virtual HRESULT OnMouseMove(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled);
	virtual HRESULT OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled);
	virtual HRESULT OnDblClick(bool& /*bHandled*/){	return S_OK; }
	virtual HRESULT OnKeyDown(LONG keyCode, LONG Shift, bool& bHandled);
	virtual HRESULT OnKeyUp(LONG /*keyCode*/, LONG /*Shift*/, bool& /*bHandled*/){ return S_OK; }
	virtual HRESULT AfterDraw(IDisplay * Display, esriViewDrawPhase phase, bool& bHandled);
	virtual HRESULT OnContextMenu(LONG X, LONG Y, VARIANT_BOOL* handled);
	virtual void clear();
	virtual void init();
	IGeometryPtr GetSearchArea( IPoint* ipPoint, ISpatialReference* ipSpRef = NULL );
	virtual IGeometryPtr GetShape() const = 0;
	/**
	 * @brief ���������ݒ�
	 * @bug   [Bug 10374]�g���[�X�\�ȃ��C���f�[�^���A���s�ړ���i[MANAGEMENT_ID]is not Null)�̃I�u�W�F�N�g�Ɍ��肷��
	 * @param lpcszWhere [in] ��������
	*/
	void setWhere(LPCTSTR lpcszWhere) { m_strWhere = lpcszWhere; }
private:
	void InitTolerance();
protected:
	CApplication m_cApp;
	CFeedback m_cFeedback;
	int m_nTolerance;
	bool m_bWithArea; //!< �G���A�\�����ꏏ�ɕ\�����邩�ǂ��� [Bug 5851]
	CString m_strWhere; //!< �������� [Bug 10374]
};

/**
 * @class CToolPoint
 * @brief �|�C���g�I���ɕK�v�ȋ@�\�݂̂��T�|�[�g����}�E�X����C�x���g�����N���X
 * @note ���ۂɂ�CToolPointer�̎��̃N���X�ƂȂ�܂�
 */
class CToolPoint : public CToolPointer
{
public:
	CToolPoint(){}
	virtual ~CToolPoint(){}
	CToolPoint( IApplication* ipApp ) : CToolPointer(ipApp){}
	IGeometryPtr GetShape() const { return NULL; }
};

/**
 * @class CToolLineTrace
 * @brief ���C���n���g���[�X�I������@�\���T�|�[�g����}�E�X����C�x���g�����N���X
 * 
 * �t�B�[�`���P�ʂł̃g���[�X�ɂȂ�܂�
 * @���̋@�\�� GuideEditTool�Ŏg�p����Ă��܂�
 */
class CToolLineTrace : public CToolPointer
{
public:
	CToolLineTrace() : m_bUseMin(false), m_bShowContextMenu(true), m_bClosedPermission(false), m_bTouchMidwayPoint(true), m_color(RGB(0,255,255)), m_bClickDir(false), m_bVirtualFinish(false), m_bFinished(false) {}
	CToolLineTrace( IApplication* ipApp, bool bWithArea ) : m_bShowContextMenu(true), m_bClosedPermission(false), m_bTouchMidwayPoint(true), m_color(RGB(0,255,255)), m_bVirtualFinish(false), m_bFinished(false), CToolPointer(ipApp, bWithArea){}
	virtual ~CToolLineTrace(){ init(); }

	HRESULT OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled);
	HRESULT AfterDraw(IDisplay * Display, esriViewDrawPhase phase, bool& bHandled);
	HRESULT OnKeyDown(LONG keyCode, LONG Shift, bool& bHandled);
	HRESULT OnDblClick(bool& bHandled);
	HRESULT OnContextMenu(LONG X, LONG Y, VARIANT_BOOL* handled);
	void DrawHilight( IDisplay* Display );
	void ClearHilight();
	size_t GetCount() const { return m_listHilightGeometry.size(); }
	void SetClass( IFeatureClass* ipClass ){ m_ipClass = ipClass; }
	void clear();
	void init();
	IGeometryPtr GetShape() const {	return ((m_ipFinishGeometry) ? m_ipFinishGeometry: m_ipHilightGeometry); }
	IArrayPtr GetFeaturesorShapes(long nMode, bool bFinish=true);
	bool IsShowContextMenu() const { return m_bShowContextMenu; }
	void SetClosedPermission(bool bOK) { m_bClosedPermission = bOK; }
	void SetTouchMidwayPoint(bool bOK) { m_bTouchMidwayPoint = bOK; }
	void SetClickDirection(bool bClick) { m_bClickDir = bClick; }
	void SetUseMinDistance(bool bOn) { m_bUseMin = bOn; }
	void SetColor(COLORREF color) { m_color = color; }
	void undo();
	IFeaturePtr GetLastFeature()
	{
		if ( ! m_listHilightFeatures.empty() )
			return m_listHilightFeatures[m_listHilightFeatures.size()-1].back().m_T;
		return NULL;
	}
	void SetFinish();
	void SetVirtualFinishMode(bool bVirtual) { m_bVirtualFinish = bVirtual; }
	void ClearFinishFlg()
	{
		m_listFinishGeometry.clear();
		m_listFinishFeatures.clear();
		m_ipFinishGeometry = NULL;
		m_bFinished = false;
	}
protected:
	IFeatureClassPtr m_ipClass; //!< �g���[�X�Ώۂ̃t�B�[�`���N���X
	std::list<CAdapt<IGeometryPtr> > m_listHilightGeometry; //!< �n�C���C�g������W�I���g�����X�g
	std::map<long,std::list<CAdapt<IFeaturePtr>>> m_listHilightFeatures;	//!< �n�C���C�g�t�B�[�`�����X�g
	IGeometryPtr m_ipHilightGeometry;

	std::list<CAdapt<IGeometryPtr> > m_listFinishGeometry; //!< �m��W�I���g�����X�g
	std::list<CAdapt<IFeaturePtr>> m_listFinishFeatures;	//!< �m��t�B�[�`�����X�g
	IGeometryPtr m_ipFinishGeometry;
	ISymbolPtr m_ipHilightSymbol; //!< �n�C���C�g���̃V���{��
private:
	bool m_bShowContextMenu; //!< �R���e�L�X�g���j���[���o�����ǂ���
	bool m_bClosedPermission; //!< �`��̏z�������邩�ǂ���
	bool m_bTouchMidwayPoint; //!< �`��̕���������邩�ǂ���
	COLORREF m_color; //!< �n�C���C�g�̐F
	bool m_bClickDir; //!< �N���b�N�����|�C���g�ɋ߂��ق��ɕ����������邩�ǂ���
	bool m_bVirtualFinish; //!< ���I�����[�h���g�����ǂ���
	bool m_bFinished; //!< ���I�����[�h���Ɏg���I�����[�h
	bool m_bUseMin; //!< �I���t�B�[�`���̒��o�ɑI���ʒu�Ɉ�ԋ߂��t�B�[�`����D�悷�邩�ǂ���
};

/**
 * @class CToolLineTrace
 * @brief ���C���n���g���[�X�I������@�\���T�|�[�g����}�E�X����C�x���g�����N���X
 * 
 * �t�B�[�`���P�ʂł̃g���[�X�ɂȂ�܂�
 * @���̋@�\�� GuideEditTool�Ŏg�p����Ă��܂�
 */
class CToolRealTimeLineTrace : public CToolPointer
{
public:
	CToolRealTimeLineTrace() : m_bShowContextMenu(true), m_bClosedPermission(false), m_bTouchMidwayPoint(true), m_color(RGB(0,255,255)), m_bClickDir(false), m_bVirtualFinish(false), m_bFinished(false) {}
	CToolRealTimeLineTrace( IApplication* ipApp, bool bWithArea ) : m_bShowContextMenu(true), m_bClosedPermission(false), m_bTouchMidwayPoint(true), m_color(RGB(0,255,255)), m_bVirtualFinish(false), m_bFinished(false), CToolPointer(ipApp, bWithArea){}
	virtual ~CToolRealTimeLineTrace(){ init(); }

	HRESULT OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled);
	HRESULT OnMouseMove(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled);
	HRESULT AfterDraw(IDisplay * Display, esriViewDrawPhase phase, bool& bHandled);
	HRESULT OnKeyDown(LONG keyCode, LONG Shift, bool& bHandled);
	HRESULT OnDblClick(bool& bHandled);
	HRESULT OnContextMenu(LONG X, LONG Y, VARIANT_BOOL* handled);
	void DrawHilight( IDisplay* Display );
	void ClearHilight(bool bRedraw=true);
	size_t GetCount() const { return m_listHilightGeometry.size(); }
	void SetClass( IFeatureClass* ipClass ){ m_ipClass = ipClass; }
	void clear();
	void init();
	IGeometryPtr GetShape() const {	return ((m_ipFinishGeometry) ? m_ipFinishGeometry: m_ipHilightGeometry); }
	IArrayPtr GetFeaturesorShapes(long nMode, bool bFinish=true);
	bool IsShowContextMenu() const { return m_bShowContextMenu; }
	void SetClosedPermission(bool bOK) { m_bClosedPermission = bOK; }
	void SetTouchMidwayPoint(bool bOK) { m_bTouchMidwayPoint = bOK; }
	void SetClickDirection(bool bClick) { m_bClickDir = bClick; }
	void SetUseMinDistance(bool /*bOn*/) {}
	void SetColor(COLORREF color) { m_color = color; }
	void undo();
	IFeaturePtr GetLastFeature();
	void SetFinish();
	void SetVirtualFinishMode(bool bVirtual) { m_bVirtualFinish = bVirtual; }
	void ClearFinishFlg()
	{
		m_ipFeatureArray = NULL;
		m_ipFinishGeometry = NULL;
		m_bFinished = false;
	}
protected:
	long GetFeatureCount();
	void AddHilight(IFeature* ipFeature, bool bAdd, IPoint* ipPoint=NULL);
	IArrayPtr getArray() const;
	void setAhead(bool bAhead);
	bool isTracing() const;
	bool startTrace(IPoint* ipPoint);
	bool back_public();
	bool trace(IPoint* ipPoint);
	void stopTrace();

protected:
	IFeatureClassPtr m_ipClass; //!< �g���[�X�Ώۂ̃t�B�[�`���N���X
	std::list<CAdapt<IGeometryPtr> > m_listHilightGeometry; //!< �n�C���C�g������W�I���g�����X�g
	IGeometryPtr m_ipHilightGeometry;

	IArrayPtr m_ipFeatureArray;	//!< �m��t�B�[�`�����X�g
	IGeometryPtr m_ipFinishGeometry;
	ISymbolPtr m_ipHilightSymbol; //!< �n�C���C�g���̃V���{��
private:
	bool m_bShowContextMenu; //!< �R���e�L�X�g���j���[���o�����ǂ���
	bool m_bClosedPermission; //!< �`��̏z�������邩�ǂ���
	bool m_bTouchMidwayPoint; //!< �`��̕���������邩�ǂ���
	COLORREF m_color; //!< �n�C���C�g�̐F
	bool m_bClickDir; //!< �N���b�N�����|�C���g�ɋ߂��ق��ɕ����������邩�ǂ���
	boost::shared_ptr<LinkSnapTrace> m_pLinkSnapTrace;
	bool m_bVirtualFinish; //!< ���I�����[�h���g�����ǂ���
	bool m_bFinished; //!< ���I�����[�h���Ɏg���I�����[�h
};

class CToolRealLineTrace : public CToolPointer
{
public:
	CToolRealLineTrace() : m_bShowContextMenu(true) {}
	CToolRealLineTrace( IApplication* ipApp ) : m_bShowContextMenu(true), CToolPointer(ipApp)
	{
	}
	virtual ~CToolRealLineTrace()
	{
		init();
	}
	HRESULT OnMouseMove(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = false;
		CToolPointer::OnMouseMove( Button, Shift, ipPoint, bHandled );

		if( IsStartTracing() )// �g���[�X���`�󂪊m�肵�Ă���ꍇ�̓g���[�X����
		{
			CreateGraph( ipPoint );
			m_cApp.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
			m_ipSegmentGraphCursor->get_CurrentTrace( &m_ipTracePolyline );
			bHandled = true;
		}
		return S_OK;
	}
	HRESULT OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = false;
		CToolPointer::OnMouseUp( Button, Shift, ipPoint, bHandled );

		switch( Button )
		{
			case 2:    // �E�N���b�N
			{
				if( !m_ipSegmentGraphCursor )
					m_bShowContextMenu = true;
				else
					m_bShowContextMenu = false;

				RefreshItems();
				m_cApp.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

				break;
			}
			case 1:    // ���N���b�N
			{
				// �g���[�X���[�h�p----------
				ILayerPtr ipCurrentLayer;
				// TOC�I�����C�����擾
				if( AheGetSelectedLayer( m_cApp.GetDocument(), &ipCurrentLayer, true ) )
				{
					IFeatureClassPtr ipFC = AheGetFeatureClass( ipCurrentLayer );
					esriGeometryType eType;
					eType = esriGeometryNull;
					if( ipFC )
						ipFC->get_ShapeType( &eType );
					// ���C���n�̃��C���̂ݑΉ�
					// �g���[�X���t�B�[�`��������A�g���[�X�`�󂪂Ȃ��ꍇ�̓g���[�X�����J�n
					if( eType == esriGeometryPolyline && IsStartTracing() && !m_ipSegmentGraphCursor )
					{
						m_bShowContextMenu = false;
						CreateGraph( ipPoint );
					}
					// ���łɃg���[�X�`�󂪂���ꍇ�͌`����擾
					// ��GetShape()��m_ipTracePolyline���擾�ł���̂Ō�͌Ăяo�����ɂ��C���B
					else if( eType == esriGeometryPolyline && m_ipSegmentGraphCursor )
					{
						m_ipSegmentGraphCursor->get_CurrentTrace( &m_ipTracePolyline );
						m_ipSegmentGraphCursor = NULL;
						bHandled = true;
						// ��U�\���_��ʂ�߂��Ă���ēx���̍\���_�Ŋm�肷��ƁA�ŏI�_���d������ibug 7289�j
						// ���傤���Ȃ��̂ŁA�d���\���_����菜��
						AheRemoveAdjoiningPoint(m_ipTracePolyline);
					}
				}
				//---------------------------
				break;
			}
			default:
				break;
		}
		return S_OK;
	}
	HRESULT AfterDraw(IDisplay * Display, esriViewDrawPhase phase, bool& bHandled)
	{
		bHandled = false;
		if( Display && esriViewForeground == phase && m_ipTracePolyline )
			AheDrawPolyline( m_cApp.GetFocusMapScreenDisplay(), m_ipTracePolyline, RGB( 255, 100, 0 ), 5, esriSLSSolid, FALSE, TRUE );

		return CToolPointer::AfterDraw(Display, phase, bHandled );
	}
	HRESULT OnKeyUp(LONG keyCode, LONG Shift, bool& bHandled)
	{
		bHandled = false;

		switch ( keyCode )
		{
			case VK_ESCAPE: // ESC
				clear();	// �t�B�[�h�o�b�N��~

				bHandled = true;
				break;
		}

		if ( !bHandled )
			CToolPointer::OnKeyUp(keyCode, Shift, bHandled);

		return S_OK;
	}
	HRESULT OnContextMenu(LONG /*X*/, LONG /*Y*/, VARIANT_BOOL* handled)
	{
		*handled = ( m_bShowContextMenu ) ? VARIANT_FALSE : VARIANT_TRUE;
		return S_OK;
	}
	// �g���[�X������
	void clear()
	{
		CToolPointer::clear();
		RefreshItems();
		m_bShowContextMenu = true;	// �R���e�L�X�g���j���[�t���OON
	}
	// �g���[�X������
	void RefreshItems()
	{
		m_ipSegmentGraphCursor = NULL;
		m_ipTracePolyline = NULL;
		m_listBaseGeom.clear();
	}
	// �g���[�X���C���`��쐬����
	void CreateGraph( IPoint* ipPoint )
	{
		if( !m_ipSegmentGraphCursor )
		{
			// �g���[�X�J�n
			m_ipTracePolyline = NULL;
			IGeometryCollectionPtr ipTraceGeomBag( CLSID_GeometryBag );

			// [Bug 7819]���C���g���[�X���A���ʒu�Ɍ`��y�[�X�g����ƃ��C���������
			// ISegmentGraph�ɂ����ތ`��ɋ�ԎQ�Ɨ^���Ă��Ȃ��Ƃ����
			if( ! m_listBaseGeom.empty() )
			{
				ISpatialReferencePtr ipSpRef;
				m_listBaseGeom.begin()->m_T->get_SpatialReference( &ipSpRef );
				((IGeometryPtr)ipTraceGeomBag)->putref_SpatialReference( ipSpRef );
			}
			for( std::list<CAdapt<IGeometryPtr>>::const_iterator it = m_listBaseGeom.begin(); it != m_listBaseGeom.end(); ++it )
				ipTraceGeomBag->AddGeometry( it->m_T );
			ISegmentGraphPtr ipSegmentGraph( CLSID_SegmentGraph );

			ipSegmentGraph->Load( (IEnumGeometryPtr)ipTraceGeomBag, VARIANT_FALSE, VARIANT_TRUE );

			// �J�n�ʒu���X�i�b�v�̑ΏۂƂ���
			ipSegmentGraph->GetCursor( ipPoint, &m_ipSegmentGraphCursor );
		}
		else
		{
			VARIANT_BOOL vaCanMove = VARIANT_FALSE;
			m_ipSegmentGraphCursor->MoveTo( ipPoint, &vaCanMove );
		}
	}
	void init()
	{
		CToolPointer::init();
		RefreshItems();
	}
	/**
	* @brief �g���[�X���`���ǉ�����
	* �g���[�X�`��̃��Z�b�g�ł͂Ȃ��ǉ��ɂȂ�܂��B
	* �O�̌`��������ɂ� ClearBaseGeometry ���Ăт܂��B
	* �����̌`����Z�b�g�ł��܂����A�}�[�W��Ɉ�M�����ɂȂ�Ȃ��ꍇ��
	* �}�E�X�|�C���^�Ɉ�ԋ߂����̂������I�ɑI������܂��B
	* @param ipBaseFeature [in] �g���[�X���t�B�[�`��
	*/
	void AddBaseGeometry( IFeature* ipBaseFeature )
	{
		if( ! ipBaseFeature )
			return;

		IGeometryPtr ipGeom;
		ipBaseFeature->get_ShapeCopy( &ipGeom );
		m_listBaseGeom.push_back( ipGeom );
	}
	/**
	* @brief �g���[�X���`���ǉ�����
	* @see AddBaseGeometry( IFeature* ipBaseFeature )
	* @param ipGeom [in] �g���[�X���`��
	*/
	void AddBaseGeometry( IGeometryPtr& ipGeom )
	{
		if( ! ipGeom )
			return;

		m_listBaseGeom.push_back( ipGeom );
	}
	/**
	* @brief �g���[�X���`����N���A����
	*/
	void ClearBaseGeometry(){ GetBaseGeometries().clear(); }
	/**
	* @brief �g���[�X���W�I���g���擾
	*/
	std::list<CAdapt<IGeometryPtr>>& GetBaseGeometries()
	{
		return m_listBaseGeom;
	}
	/**
	* @brief �g���[�X���J�n����Ă��邩�ǂ���
	*/
	bool IsStartTracing() const { return ! m_listBaseGeom.empty(); }
	/// �g���[�X�m��`��擾
	IGeometryPtr GetShape() const
	{
		return m_ipTracePolyline;
	}
	bool IsShowContextMenu() const { return m_bShowContextMenu; }
protected:
	ISymbolPtr m_ipHilightSymbol; //!< �n�C���C�g���̃V���{��
	ISegmentGraphCursorPtr	m_ipSegmentGraphCursor;
	IPolylinePtr			m_ipTracePolyline;
	std::list<CAdapt<IGeometryPtr>> m_listBaseGeom;//!< �g���[�X���W�I���g��
private:
	bool m_bShowContextMenu; //!< �R���e�L�X�g���j���[���o�����ǂ���
};

// GS�Č��Œǉ������`��g���[�X�N���X
class CToolRealLineTrace2 : public CToolPointer
{
public:
	CToolRealLineTrace2() : m_bShowContextMenu(true) {}
	CToolRealLineTrace2( IApplication* ipApp ) : m_bShowContextMenu(true), CToolPointer(ipApp), m_bVertexFeedback( false )
	{
	}

	virtual ~CToolRealLineTrace2()
	{
		init();
	}

	HRESULT OnMouseMove(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = false;
		CToolPointer::OnMouseMove( Button, Shift, ipPoint, bHandled );

		if( IsStartTracing() )// �g���[�X���`�󂪊m�肵�Ă���ꍇ�̓g���[�X����
		{
			if( m_bVertexFeedback )
			{
				if( m_ipDisplayFeedback )
					m_ipDisplayFeedback->MoveTo( ipPoint );
			}
			else
			{
				if( m_ipSegmentGraphCursor )
				{
					VARIANT_BOOL vaCanMove = VARIANT_FALSE;
					m_ipSegmentGraphCursor->MoveTo( ipPoint, &vaCanMove );
					m_cApp.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
					m_ipSegmentGraphCursor->get_CurrentTrace( &m_ipTracePolyline );
				}
			}
			bHandled = true;
		}
		return S_OK;
	}

	HRESULT OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = false;
		CToolPointer::OnMouseUp( Button, Shift, ipPoint, bHandled );

		switch( Button )
		{
			case 1:    // ���N���b�N
			{
				ILayerPtr ipCurrentLayer;
				// TOC�I�����C�����擾
				if( AheGetSelectedLayer( m_cApp.GetDocument(), &ipCurrentLayer, true ) )
				{
					IFeatureClassPtr ipFC = AheGetFeatureClass( ipCurrentLayer );
					esriGeometryType eType = esriGeometryNull;
					if( ipFC )
						ipFC->get_ShapeType( &eType );
					// ���C���n�̃��C���̂ݑΉ�
					if( eType != esriGeometryPolyline )
						break;

					// �g���[�X���t�B�[�`��������A�g���[�X�`�󂪂Ȃ��ꍇ�̓g���[�X�����J�n
					if( IsStartTracing() )
					{
						// �X�i�b�v���Ă���
						m_cApp.Snap( ipPoint );

						// �����܂łɊm�肵���`��m�ۂ��Ă���
						if( m_ipSegmentGraphCursor )
						{
							m_ipSegmentGraphCursor->get_CurrentTrace( &m_ipTracePolyline );
							m_listFixedTraceGeom.push_back( m_ipTracePolyline );
						}

						if( m_ipDisplayFeedback )
						{
							IPolylinePtr ipPolyline( CLSID_Polyline );
							ISpatialReferencePtr ipSpRef;
							m_listBaseGeom.begin()->m_T->get_SpatialReference( &ipSpRef );
							((IGeometryPtr)ipPolyline)->putref_SpatialReference( ipSpRef );

							// �i�[���Ă���Ō�̌`���ToPoint���擾����
							IPolylinePtr ipLastPolyline = m_listFixedTraceGeom.rbegin()->m_T;
							IPointPtr ipEndPoint;
							ipLastPolyline->get_ToPoint( &ipEndPoint );
							ipPolyline->put_FromPoint( ipEndPoint );
							ipPolyline->put_ToPoint( ipPoint );
							m_listFixedTraceGeom.push_back( (IPolylinePtr)ipPolyline );
						}

						m_bShowContextMenu = false;
						if( m_bVertexFeedback )
							VertexFeedback( ipPoint );
						else
							CreateGraph( ipPoint );
					}
				}
				break;
			}
			case 2:    // �E�N���b�N
			{
				if( !m_ipSegmentGraphCursor )
					m_bShowContextMenu = true;
				else
					m_bShowContextMenu = false;

				RefreshItems();
				m_cApp.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

				break;
			}
			default:
				break;
		}
		return S_OK;
	}

	HRESULT AfterDraw(IDisplay * Display, esriViewDrawPhase phase, bool& bHandled)
	{
		bHandled = false;
		if( Display && esriViewForeground == phase )
		{
			// �����݂̃g���[�X���Ă���`���`��
			if( m_ipTracePolyline )
				AheDrawPolyline( m_cApp.GetFocusMapScreenDisplay(), m_ipTracePolyline, RGB( 255, 100, 0 ), 5, esriSLSSolid, FALSE, TRUE );

			// ����܂łɊm�肵�Ă���`��̕`��
#if (1400 <= _MSC_VER) && (_MSC_VER < 1700)
			std::list< CAdapt< IPolylinePtr > >::const_iterator itBegin = m_listFixedTraceGeom.begin();
			std::list< CAdapt< IPolylinePtr > >::const_iterator itEnd = m_listFixedTraceGeom.end();
			for( std::list< CAdapt< IPolylinePtr > >::const_iterator it = itBegin; it != itEnd; ++it )
			{
				AheDrawPolyline( m_cApp.GetFocusMapScreenDisplay(), (*it).m_T, RGB( 255, 100, 0 ), 5, esriSLSSolid, FALSE, TRUE );
			}
#else
			for( const auto &line : m_listFixedTraceGeom )
			{
				AheDrawPolyline( m_cApp.GetFocusMapScreenDisplay(), line, RGB( 255, 100, 0 ), 5, esriSLSSolid, FALSE, TRUE );
			}
#endif
		}

		return CToolPointer::AfterDraw(Display, phase, bHandled );
	}

	HRESULT OnKeyUp(LONG keyCode, LONG Shift, bool& bHandled)
	{
		bHandled = false;

		switch ( keyCode )
		{
			case VK_ESCAPE: // ESC
				clear();	// �t�B�[�h�o�b�N��~
				m_cApp.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
				bHandled = true;
				break;
		}

		if ( !bHandled )
			CToolPointer::OnKeyUp(keyCode, Shift, bHandled);

		return S_OK;
	}

	HRESULT OnContextMenu(LONG /*X*/, LONG /*Y*/, VARIANT_BOOL* handled)
	{
		*handled = ( m_bShowContextMenu ) ? VARIANT_FALSE : VARIANT_TRUE;
		return S_OK;
	}

	// �g���[�X������
	void clear()
	{
		CToolPointer::clear();
		RefreshItems();
		m_bShowContextMenu = true;	// �R���e�L�X�g���j���[�t���OON
	}

	// �g���[�X������
	void RefreshItems()
	{
		m_bVertexFeedback = false;
		m_ipDisplayFeedback = NULL;
		m_ipSegmentGraphCursor = NULL;
		m_ipTracePolyline = NULL;
		m_listBaseGeom.clear();
		m_ipCurrentTracingFeature = NULL;
		m_listFixedTraceGeom.clear();
	}

	// �g���[�X���C���`��쐬����
	void CreateGraph( IPoint* ipPoint )
	{
		// SegmentGraph�̃g���[�X�̏ꍇ�́ADisplayFeedback���������Ă���
		m_ipDisplayFeedback = NULL;
		// �g���[�X�J�n
		m_ipTracePolyline = NULL;
		IGeometryCollectionPtr ipTraceGeomBag( CLSID_GeometryBag );

		// [Bug 7819]���C���g���[�X���A���ʒu�Ɍ`��y�[�X�g����ƃ��C���������
		// ISegmentGraph�ɂ����ތ`��ɋ�ԎQ�Ɨ^���Ă��Ȃ��Ƃ����
		if( ! m_listBaseGeom.empty() )
		{
			ISpatialReferencePtr ipSpRef;
			m_listBaseGeom.begin()->m_T->get_SpatialReference( &ipSpRef );
			((IGeometryPtr)ipTraceGeomBag)->putref_SpatialReference( ipSpRef );
		}
		ipTraceGeomBag->AddGeometry( m_listBaseGeom.rbegin()->m_T );
		ISegmentGraphPtr ipSegmentGraph( CLSID_SegmentGraph );

		ipSegmentGraph->Load( (IEnumGeometryPtr)ipTraceGeomBag, VARIANT_FALSE, VARIANT_TRUE );

		// �J�n�ʒu���X�i�b�v�̑ΏۂƂ���
		ipSegmentGraph->GetCursor( ipPoint, &m_ipSegmentGraphCursor );
	}

	// �\���_�t�B�[�h�o�b�N����
	void VertexFeedback( IPointPtr ipPoint )
	{
		// VertexFeedback���ۂɂ́ASegmentGraph�̂ق��͏�����
		m_ipSegmentGraphCursor = NULL;
		m_ipDisplayFeedback.CreateInstance( CLSID_GroupFeedback );
		// �O���[�v�t�B�[�h�o�b�N�̏����i����ipSet�ɃZ�b�g���ꂽ���̂��t�B�[�h�o�b�N�����j
		_ISetPtr ipSet;
		m_ipDisplayFeedback->QueryInterface( &ipSet );
		IVertexFeedbackPtr ipVertexFeedback( CLSID_VertexFeedback );
		IPointPtr ipEndPoint;
		if( m_ipTracePolyline )
		{
			m_ipTracePolyline->get_ToPoint( &ipEndPoint );
			m_ipTracePolyline = NULL;
		}
		ILinePtr ipLine( CLSID_Line );
		ipLine->put_FromPoint( ipEndPoint );
		ipLine->put_ToPoint( ipPoint );

		IPolylinePtr ipPolyline( CLSID_Polyline );
		ipPolyline->put_FromPoint( ipEndPoint );
		ipPolyline->put_ToPoint( ipPoint );
		m_listFixedTraceGeom.push_back( ipPolyline );

		ipVertexFeedback->AddSegment( (_ISegment*)(_ISegmentPtr)(ipLine), VARIANT_TRUE );
		ipSet->Add( ipVertexFeedback );
		m_ipDisplayFeedback->putref_Display( m_cApp.GetFocusMapScreenDisplay() );
	}

	void init()
	{
		CToolPointer::init();
		RefreshItems();
	}

	/**
	* @brief �g���[�X���`���ǉ�����
	* �g���[�X�`��̃��Z�b�g�ł͂Ȃ��ǉ��ɂȂ�܂��B
	* �O�̌`��������ɂ� ClearBaseGeometry ���Ăт܂��B
	* �����̌`����Z�b�g�ł��܂����A�}�[�W��Ɉ�M�����ɂȂ�Ȃ��ꍇ��
	* �}�E�X�|�C���^�Ɉ�ԋ߂����̂������I�ɑI������܂��B
	* @param ipBaseFeature [in] �g���[�X���t�B�[�`��
	*/
	void AddBaseGeometry( IFeaturePtr ipBaseFeature )
	{
		m_ipCurrentTracingFeature = ipBaseFeature;
		if( ! ipBaseFeature )
			return;

		IGeometryPtr ipGeom;
		ipBaseFeature->get_ShapeCopy( &ipGeom );
		m_listBaseGeom.push_back( ipGeom );
	}

	/**
	* @brief �g���[�X���`����N���A����
	*/
	void ClearBaseGeometry(){ GetBaseGeometries().clear(); }

	/**
	* @brief �g���[�X���W�I���g���擾
	*/
	std::list<CAdapt<IGeometryPtr>>& GetBaseGeometries()
	{
		return m_listBaseGeom;
	}

	IFeaturePtr& GetCurrentTracingFeature()
	{
		return m_ipCurrentTracingFeature;
	}

	/**
	* @brief �g���[�X���J�n����Ă��邩�ǂ���
	*/
	bool IsStartTracing() const { return ! m_listBaseGeom.empty(); }

	/// �g���[�X�m��`��擾
	IGeometryPtr GetShape() const
	{
		if( m_listBaseGeom.empty() )
			return NULL;
		IGeometryPtr ipRetGeom( CLSID_Polyline );
		ISpatialReferencePtr ipSpRef;
		m_listBaseGeom.begin()->m_T->get_SpatialReference( &ipSpRef );
		((IGeometryPtr)ipRetGeom)->putref_SpatialReference( ipSpRef );

		IPointCollectionPtr ipPointCol = ipRetGeom;

#if (1400 <= _MSC_VER) && (_MSC_VER < 1700)
		std::list< CAdapt< IPolylinePtr > >::const_iterator itBegin = m_listFixedTraceGeom.begin();
		std::list< CAdapt< IPolylinePtr > >::const_iterator itEnd = m_listFixedTraceGeom.end();
		for( std::list< CAdapt< IPolylinePtr > >::const_iterator it = itBegin; it != itEnd; ++it )
		{
			long lPointCount = 0;
			IPointCollectionPtr ipPointTempCol = (IPolylinePtr)(*it).m_T;
			ipPointCol->AddPointCollection( ipPointTempCol );
		}
#else
		for( const auto &line : m_listFixedTraceGeom )
		{
			long lPointCount = 0;
			IPointCollectionPtr ipPointTempCol = (IPolylinePtr)line;
			ipPointCol->AddPointCollection( ipPointTempCol );
		}
#endif
		AheRemoveAdjoiningPoint((IPolylinePtr)ipPointCol);

		return ipPointCol;
	}
	void SetVertexTraceMode( bool bVertexMode ){ m_bVertexFeedback = bVertexMode;};
	bool IsShowContextMenu() const { return m_bShowContextMenu; }
protected:
	ISymbolPtr m_ipHilightSymbol; //!< �n�C���C�g���̃V���{��
	bool m_bVertexFeedback; //!< �\���_�t�B�[�h�o�b�N���Ă��邩�ǂ���
	ISegmentGraphCursorPtr	m_ipSegmentGraphCursor;  //!< �t�B�[�h�o�b�N�p�̃Z�O�����g�J�[�\��
	IDisplayFeedbackPtr		m_ipDisplayFeedback;	//!< �t�B�[�h�o�b�N
	IPolylinePtr			m_ipTracePolyline;     //!< �g���[�X���Ă��郉�C���`��
	std::list<CAdapt<IGeometryPtr>> m_listBaseGeom;//!< �g���[�X���W�I���g��
	IFeaturePtr m_ipCurrentTracingFeature;//!< �g���[�X���W�I���g��
	std::list< CAdapt< IPolylinePtr > > m_listFixedTraceGeom;//!< �g���[�X�`��
private:
	bool m_bShowContextMenu; //!< �R���e�L�X�g���j���[���o�����ǂ���
};

class CToolArea : public CToolPointer
{
public:
	CToolArea() : m_bShowContextMenu(true){}
	CToolArea( IApplication* ipApp ) : CToolPointer(ipApp,true), m_bShowContextMenu(true){}
	CToolArea( IApplication* ipApp, bool bWithArea ) : CToolPointer(ipApp, bWithArea), m_bShowContextMenu(true){}
	virtual ~CToolArea()
	{
		init();
	}

	HRESULT OnMouseDown(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		if ( Button == 1 ) {		// ���N���b�N
			m_ipStartPoint = ipPoint;	// �J�n�|�C���g�ێ�
			m_bShowContextMenu = false;	// �R���e�L�X�g���j���[�t���OOFF
		} else if ( Button == 2 ) {
			clear();	// �t�B�[�h�o�b�N��~
			m_ipStartPoint = NULL;	// �t�B�[�h�o�b�N�`�󏉊���
		}

		return CToolPointer::OnMouseDown(Button, Shift, ipPoint, bHandled);
	}

	HRESULT OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		if ( Button == 1 ) {	// ���N���b�N
			if ( !m_bShowContextMenu ) {
				clear();	// �t�B�[�h�o�b�N��~
				m_ipStartPoint = ipPoint;	// �J�n�|�C���g�ێ�
			}
		} else if ( Button == 2 ) {	// �E�N���b�N
			m_ipStartPoint = NULL;
		}

		return CToolPointer::OnMouseUp(Button, Shift, ipPoint, bHandled);
	}

	HRESULT OnContextMenu(LONG /*X*/, LONG /*Y*/, VARIANT_BOOL* handled)
	{
		*handled = ( m_bShowContextMenu ) ? VARIANT_FALSE : VARIANT_TRUE;
		return S_OK;
	}

	void clear()
	{
		CToolPointer::clear();

		m_ipStartPoint = NULL;		// �J�n�|�C���g������
		m_bShowContextMenu = true;	// �R���e�L�X�g���j���[�t���OON
	}

	void init()
	{
		CToolPointer::init();
		// �n�C���C�g�N���A
		clear();
	}

	/*!
		\fn		GetShape() const
		\brief	�t�B�[�h�o�b�N�m��`��擾
		\retval IGeometryPtr
	*/
	IGeometryPtr GetShape() const { return m_ipStartPoint; };
	bool IsShowContextMenu() const { return m_bShowContextMenu; }
protected:
	IPointPtr	m_ipStartPoint;		//!< �t�B�[�h�o�b�N�J�n�_

private:
	bool		m_bShowContextMenu;	//!< �R���e�L�X�g���j���[���o�����ǂ���
};

class CToolEnvelope : public CToolPointer, public CSketchFeedback
{
public:
	CToolEnvelope() : m_bShowContextMenu(true) {}
	CToolEnvelope( IApplication* ipApp, bool bWithArea ) : CToolPointer(ipApp, bWithArea), m_bShowContextMenu(true)
	{
		CSketchFeedback::SetDisplay( (IScreenDisplayPtr)m_cApp.GetFocusMapScreenDisplay() );
	}
	virtual ~CToolEnvelope()
	{
		init();
	}

	HRESULT OnMouseDown(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		if ( Button == 1 ) {		// ���N���b�N
			m_ipFeddbackGeom = NULL;	// �t�B�[�h�o�b�N�`�󏉊���
			m_ipStartPoint = ipPoint;	// �J�n�|�C���g�ێ�
			m_bShowContextMenu = false;	// �R���e�L�X�g���j���[�t���OOFF
		} else if ( Button == 2 ) {
			clear();	// �t�B�[�h�o�b�N��~
			m_ipFeddbackGeom = NULL;	// �t�B�[�h�o�b�N�`�󏉊���
		}

		return CToolPointer::OnMouseDown(Button, Shift, ipPoint, bHandled);
	}

	HRESULT OnMouseMove(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = false;

		if ( m_ipStartPoint ) {	// �J�n�|�C���g�ێ��H
			CToolPointer::clear();	// �|�C���g�t�B�[�h�o�b�N��~

			if ( CSketchFeedback::IsStartingFeedback() )	// �t�B�[�h�o�b�N�J�n��ԁH
				CSketchFeedback::MoveTo( ipPoint );	// �t�B�[�h�o�b�N�ړ�
			else
				CSketchFeedback::Start( m_ipStartPoint, sindyeSketch_Envelope );	// �t�B�[�h�o�b�N�J�n

			bHandled = true;
		}

		if ( !bHandled )
			CToolPointer::OnMouseMove(Button, Shift, ipPoint, bHandled);

		return S_OK;
	}

	HRESULT OnMouseUp(LONG Button, LONG /*Shift*/, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = true;

		if ( Button == 1 ) {	// ���N���b�N
			m_ipFeddbackGeom = CSketchFeedback::Stop();	// �t�B�[�h�o�b�N��~

			// ��`�����������Ȃ����`�F�b�N
			IScreenDisplayPtr ipDisp = m_cApp.GetFocusMapScreenDisplay();

			if ( ipDisp ) {
				IDisplayTransformationPtr ipDispTrans;

				ipDisp->get_DisplayTransformation( &ipDispTrans );

				if ( ipDispTrans ) {
					// MouseDown�_
					long fx=0, fy=0;

					if ( m_ipStartPoint ) {
						ipDispTrans->FromMapPoint( m_ipStartPoint, &fx, &fy );

						// MouseUp�_
						long lx=0, ly=0;

						ipDispTrans->FromMapPoint( ipPoint, &lx, &ly );

						if ( abs( fx - lx ) + abs( fy - ly ) <= 2 )
							m_ipFeddbackGeom = ipPoint;	// MouseUp�|�C���g���i�[
					}
				}
			}

			clear();	// �t�B�[�h�o�b�N��~
		} else if ( Button == 2 ) {	// �E�N���b�N
			clear();	// �t�B�[�h�o�b�N��~
			m_ipFeddbackGeom = NULL;	// �t�B�[�h�o�b�N�`�󏉊���
		}

		return S_OK;
	}

	HRESULT OnKeyUp(LONG keyCode, LONG Shift, bool& bHandled)
	{
		bHandled = false;

		switch ( keyCode ) {
			case VK_ESCAPE: // ESC
				clear();	// �t�B�[�h�o�b�N��~

				bHandled = true;
				break;
		}

		if ( !bHandled )
			CToolPointer::OnKeyUp(keyCode, Shift, bHandled);

		return S_OK;
	}

	HRESULT OnContextMenu(LONG /*X*/, LONG /*Y*/, VARIANT_BOOL* handled)
	{
		*handled = ( m_bShowContextMenu ) ? VARIANT_FALSE : VARIANT_TRUE;
		return S_OK;
	}

	HRESULT AfterDraw(IDisplay * Display, esriViewDrawPhase phase, bool& bHandled)
	{
		bHandled = false;

		if ( Display && esriViewForeground == phase ) {
			OLE_HANDLE hDC = 0;

			if ( SUCCEEDED ( Display->get_hDC( &hDC ) ) )
				CSketchFeedback::Refresh( hDC );

			bHandled = true;
		}
		CToolPointer::AfterDraw(Display, phase, bHandled);

		return S_OK;
	}

	void clear()
	{
		CToolPointer::clear();
		if ( CSketchFeedback::IsStartingFeedback() )	// �t�B�[�h�o�b�N�J�n��ԁH
			CSketchFeedback::Stop();	// �t�B�[�h�o�b�N��~

		m_ipStartPoint = NULL;		// �J�n�|�C���g������
		m_bShowContextMenu = true;	// �R���e�L�X�g���j���[�t���OON
	}

	void init()
	{
		CToolPointer::init();
		// �n�C���C�g�N���A
		clear();
	}

	/*!
		\fn		GetShape() const
		\brief	�t�B�[�h�o�b�N�m��`��擾
		\retval IGeometryPtr
	*/
	IGeometryPtr GetShape() const { return m_ipFeddbackGeom; };
	bool IsShowContextMenu() const { return m_bShowContextMenu; }
protected:
	IPointPtr		m_ipStartPoint;		//!< �t�B�[�h�o�b�N�J�n�_
	IGeometryPtr	m_ipFeddbackGeom;	//!< �t�B�[�h�o�b�N�`��

private:
	bool			m_bShowContextMenu;	//!< �R���e�L�X�g���j���[���o�����ǂ���
};

class CToolPolyline : public CToolPointer, public CSketchFeedback
{
public:
	CToolPolyline() : m_bShowContextMenu(true){}
	CToolPolyline( IApplication* ipApp, bool bWithArea=false ) : CToolPointer(ipApp,bWithArea), m_bShowContextMenu(true)
	{
		CSketchFeedback::SetDisplay( (IScreenDisplayPtr)m_cApp.GetFocusMapScreenDisplay() );
	}
	virtual ~CToolPolyline()
	{
		init();
	}

	bool Restart( IGeometry* ipFeedbackGeom )
	{
		m_bShowContextMenu = false;
		return BOOL2bool(CSketchFeedback::RestartFeedback( ipFeedbackGeom ));
	}

	HRESULT OnMouseDown(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = true;

		if ( Button == 2 ) {
			if ( !CSketchFeedback::IsStartingFeedback() )	// �t�B�[�h�o�b�N��~��ԁH
				m_ipFeddbackGeom = NULL;	// �t�B�[�h�o�b�N�`�󏉊���
		}

		return CToolPointer::OnMouseDown(Button, Shift, ipPoint, bHandled);
	}

	HRESULT OnMouseMove(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = true;

		// �I��͈͂̕\��
		CToolPointer::OnMouseMove(Button,Shift,ipPoint,bHandled);

		// �X�P�b�`�t�B�[�h�o�b�N
		if ( CSketchFeedback::IsStartingFeedback() )	// �t�B�[�h�o�b�N�J�n��ԁH
			CSketchFeedback::MoveTo( ipPoint );	// �t�B�[�h�o�b�N�ړ�

		return CToolPointer::OnMouseMove(Button,Shift,ipPoint,bHandled);
	}

	HRESULT OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = true;

		if ( Button == 1 ) {	// ���N���b�N
			if ( IsStartingFeedback() )	// �t�B�[�h�o�b�N�J�n��ԁH
				CSketchFeedback::AddPoint( ipPoint );	// �t�B�[�h�o�b�N�|�C���g�ǉ�
			else {
				m_ipFeddbackGeom = NULL;	// �t�B�[�h�o�b�N�`�󏉊���
				m_bShowContextMenu = false;	// �R���e�L�X�g���j���[�t���OOFF
				CSketchFeedback::Start( ipPoint, sindyeSketch_Polyline );	// �t�B�[�h�o�b�N�J�n
			}
		} else if ( Button == 2 ) {	// �E�N���b�N
			if ( CSketchFeedback::IsStartingFeedback() ) {	// �t�B�[�h�o�b�N�J�n��ԁH
				if ( !CSketchFeedback::RemovePoint( ipPoint ) )	// �t�B�[�h�o�b�N�߂������H
					CSketchFeedback::Stop();	// �t�B�[�h�o�b�N��~

			} else
				m_bShowContextMenu = true;	// �R���e�L�X�g���j���[�t���OON
		}

		return CToolPointer::OnMouseUp(Button,Shift,ipPoint,bHandled);
	}

	HRESULT OnDblClick(bool& bHandled)
	{
		bHandled = true;

		if ( CSketchFeedback::IsStartingFeedback() ) {	// �t�B�[�h�o�b�N�J�n��ԁH
			m_bShowContextMenu = true;	// �R���e�L�X�g���j���[�t���OON

			m_ipFeddbackGeom = CSketchFeedback::Stop();	// �t�B�[�h�o�b�N��~
		}

		return CToolPointer::OnDblClick(bHandled);
	}

	HRESULT OnKeyUp(LONG keyCode, LONG Shift, bool& bHandled)
	{
		bHandled = false;

		switch ( keyCode ) {
			case VK_ESCAPE: // ESC
				clear();	// �t�B�[�h�o�b�N��~

				bHandled = true;
				break;
		}

		if ( !bHandled )
			CToolPointer::OnKeyUp(keyCode, Shift, bHandled);

		return S_OK;
	}

	HRESULT OnContextMenu(LONG /*X*/, LONG /*Y*/, VARIANT_BOOL* handled)
	{
		*handled = ( m_bShowContextMenu ) ? VARIANT_FALSE : VARIANT_TRUE;
		return S_OK;
	}

	HRESULT AfterDraw(IDisplay * Display, esriViewDrawPhase phase, bool& bHandled)
	{
		bHandled = true;

		if ( Display && esriViewForeground == phase ) {
			OLE_HANDLE hDC = 0;

			if ( SUCCEEDED ( Display->get_hDC( &hDC ) ) )
				CSketchFeedback::Refresh( hDC );

			bHandled = true;
		}

		return CToolPointer::AfterDraw(Display, phase, bHandled );
	}

	void clear()
	{
		CToolPointer::clear();
		if ( CSketchFeedback::IsStartingFeedback() )	// �t�B�[�h�o�b�N�J�n��ԁH
			CSketchFeedback::Stop();	// �t�B�[�h�o�b�N��~

		m_bShowContextMenu = true;	// �R���e�L�X�g���j���[�t���OON
	}

	void init()
	{
		CToolPointer::init();
		// �n�C���C�g�N���A
		clear();
	}

	/*!
		\fn		GetShape() const
		\brief	�t�B�[�h�o�b�N�m��`��擾
		\retval IGeometryPtr
	*/
	IGeometryPtr GetShape() const { return m_ipFeddbackGeom; };
	bool IsShowContextMenu() const { return m_bShowContextMenu; }
protected:
	IGeometryPtr	m_ipFeddbackGeom;	//!< �t�B�[�h�o�b�N�`��

	bool			m_bShowContextMenu;	//!< �R���e�L�X�g���j���[���o�����ǂ���
};

class CToolPolygon : public CToolPointer, public CSketchFeedback
{
public:
	CToolPolygon() : m_bShowContextMenu(true){}
	CToolPolygon( IApplication* ipApp, bool bWithArea=false ) : CToolPointer(ipApp,bWithArea), m_bShowContextMenu(true)
	{
		CSketchFeedback::SetDisplay( (IScreenDisplayPtr)m_cApp.GetFocusMapScreenDisplay() );
	}
	virtual ~CToolPolygon()
	{
		init();
	}

	bool Restart( IGeometry* ipFeedbackGeom )
	{
		m_bShowContextMenu = false;
		return BOOL2bool(CSketchFeedback::RestartFeedback( ipFeedbackGeom ));
	}
	HRESULT OnMouseDown(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = true;

		if ( Button == 2 ) {
			if ( !CSketchFeedback::IsStartingFeedback() )	// �t�B�[�h�o�b�N��~��ԁH
				m_ipFeddbackGeom = NULL;	// �t�B�[�h�o�b�N�`�󏉊���
		}

		return CToolPointer::OnMouseDown(Button, Shift, ipPoint, bHandled);
	}

	HRESULT OnMouseMove(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = true;

		if ( CSketchFeedback::IsStartingFeedback() ) // �t�B�[�h�o�b�N�J�n��ԁH
			CSketchFeedback::MoveTo( ipPoint );	// �t�B�[�h�o�b�N�ړ�

		return CToolPointer::OnMouseMove(Button,Shift,ipPoint,bHandled);
	}

	HRESULT OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = true;

		if ( Button == 1 ) {	// ���N���b�N
			if ( IsStartingFeedback() )	// �t�B�[�h�o�b�N�J�n��ԁH
				CSketchFeedback::AddPoint( ipPoint );	// �t�B�[�h�o�b�N�|�C���g�ǉ�
			else {
				m_ipFeddbackGeom = NULL;	// �t�B�[�h�o�b�N�`�󏉊���
				m_bShowContextMenu = false;	// �R���e�L�X�g���j���[�t���OOFF
				CSketchFeedback::Start( ipPoint, sindyeSketch_Polygon );	// �t�B�[�h�o�b�N�J�n
			}
		} else if ( Button == 2 ) {	// �E�N���b�N
			if ( CSketchFeedback::IsStartingFeedback() ) {	// �t�B�[�h�o�b�N�J�n��ԁH
				if ( !CSketchFeedback::RemovePoint( ipPoint ) )	// �t�B�[�h�o�b�N�߂������H
					CSketchFeedback::Stop();	// �t�B�[�h�o�b�N��~
			} else
				m_bShowContextMenu = true;	// �R���e�L�X�g���j���[�t���OON
		}

		return CToolPointer::OnMouseUp(Button,Shift,ipPoint,bHandled);
	}

	HRESULT OnDblClick(bool& bHandled)
	{
		bHandled = true;

		if ( CSketchFeedback::IsStartingFeedback() ) {	// �t�B�[�h�o�b�N�J�n��ԁH
			m_bShowContextMenu = true;	// �R���e�L�X�g���j���[�t���OON

			m_ipFeddbackGeom = CSketchFeedback::Stop();	// �t�B�[�h�o�b�N��~
		}

		return CToolPointer::OnDblClick(bHandled);
	}

	HRESULT OnKeyUp(LONG keyCode, LONG Shift, bool& bHandled)
	{
		bHandled = false;

		switch ( keyCode ) {
			case VK_ESCAPE: // ESC
				clear();	// �t�B�[�h�o�b�N��~

				bHandled = true;
				break;
		}

		if ( !bHandled )
			CToolPointer::OnKeyUp(keyCode, Shift, bHandled);

		return S_OK;
	}

	HRESULT OnContextMenu(LONG /*X*/, LONG /*Y*/, VARIANT_BOOL* handled)
	{
		*handled = ( m_bShowContextMenu ) ? VARIANT_FALSE : VARIANT_TRUE;
		return S_OK;
	}

	HRESULT AfterDraw(IDisplay * Display, esriViewDrawPhase phase, bool& bHandled)
	{
		bHandled = true;

		if ( Display && esriViewForeground == phase ) {
			OLE_HANDLE hDC = 0;

			if ( SUCCEEDED ( Display->get_hDC( &hDC ) ) )
				CSketchFeedback::Refresh( hDC );

			bHandled = true;
		}

		return CToolPointer::AfterDraw(Display,phase,bHandled);
	}

	void clear()
	{
		CToolPointer::clear();
		if ( CSketchFeedback::IsStartingFeedback() )	// �t�B�[�h�o�b�N�J�n��ԁH
			CSketchFeedback::Stop();	// �t�B�[�h�o�b�N��~

		m_bShowContextMenu = true;	// �R���e�L�X�g���j���[�t���OON
	}

	void init()
	{
		CToolPointer::init();
		// �n�C���C�g�N���A
		clear();
	}
	/*!
		\fn		GetPointCount()
		\brief	�t�B�[�h�o�b�N�|�C���g���擾
		\retval	long
	*/
	LONG GetPointCounts()
	{
		LONG lRetCount = 0;
		if( CSketchFeedback::IsStartingFeedback() )
			lRetCount = CSketchFeedback::GetPointCount();
		return lRetCount;
	}
	IPointPtr GetPoints( long lNum )
	{
		IPointPtr ipPoint;
		if( CSketchFeedback::IsStartingFeedback() )
			ipPoint = CSketchFeedback::GetPoint( lNum );
		return ipPoint;
	}
	/*!
		\fn		GetShape() const
		\brief	�t�B�[�h�o�b�N�m��`��擾
		\retval IGeometryPtr
	*/
	IGeometryPtr GetShape() const { return m_ipFeddbackGeom; };
	bool IsShowContextMenu() const { return m_bShowContextMenu; }
protected:
	IGeometryPtr	m_ipFeddbackGeom;	//!< �t�B�[�h�o�b�N�`��

private:
	bool			m_bShowContextMenu;	//!< �R���e�L�X�g���j���[���o�����ǂ���
};
