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
 * @brief 両方のジオメトリの端点同士が接しているかどうかの判定
 * 
 * 双方の端点が両方とも接している場合（ループする場合）はfalseになります
 */
bool AheIsTouch_EndPoint( IGeometry* ipGeom1, IGeometry* ipGeom2 );

/**
 * @brief ポイントが第一引数の端点と接しているかどうかの判定
 *
 * 仮に第一引数のジオメトリがループしていてもエラーとはなりません
 */
bool AheIsTouch_EndPointAndPoint( IGeometry* ipGeom, IPoint* ipPoint );

/**
 * @brief 片方のジオメトリの中間点ともう片方のジオメトリの端点が接しているかどうかの判定
 * 
 * 中間点が比較されるのは第一引数、端点が比較されるのは第二引数となります。
 */
bool AheIsTouch_MidwayPointAndEndPoint( IGeometry* ipGeom1, IGeometry* ipGeom2 );

/**
 * @brief ipGeom1とipGeom2がループ無しで接続しているかどうかの判定
 */
bool AheIsTouch_EndPointNoRoop( IGeometry* ipGeom1, IGeometry* ipGeom2 );

double AheCalcDistance( IGeometry* ipGeom1, IGeometry* ipGeom2 );

IGeometryPtr ConstructUnion( const std::list<CAdapt<IGeometryPtr> >& listGeometry );

bool IsInvalidateGeometry(IGeometry* ipGeom);

/**
 * @class CToolPointer
 * @brief マウス操作イベントを実際に処理するクラスのベースクラス
 * 全てのマウス操作イベント処理クラスはこのクラスを継承する必要があります
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
	 * @brief 検索条件設定
	 * @bug   [Bug 10374]トレース可能なラインデータを、平行移動後（[MANAGEMENT_ID]is not Null)のオブジェクトに限定する
	 * @param lpcszWhere [in] 検索条件
	*/
	void setWhere(LPCTSTR lpcszWhere) { m_strWhere = lpcszWhere; }
private:
	void InitTolerance();
protected:
	CApplication m_cApp;
	CFeedback m_cFeedback;
	int m_nTolerance;
	bool m_bWithArea; //!< エリア表示を一緒に表示するかどうか [Bug 5851]
	CString m_strWhere; //!< 検索条件 [Bug 10374]
};

/**
 * @class CToolPoint
 * @brief ポイント選択に必要な機能のみをサポートするマウス操作イベント処理クラス
 * @note 実際にはCToolPointerの実体クラスとなります
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
 * @brief ライン系をトレース選択する機能をサポートするマウス操作イベント処理クラス
 * 
 * フィーチャ単位でのトレースになります
 * @この機能は GuideEditToolで使用されています
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
	IFeatureClassPtr m_ipClass; //!< トレース対象のフィーチャクラス
	std::list<CAdapt<IGeometryPtr> > m_listHilightGeometry; //!< ハイライトさせるジオメトリリスト
	std::map<long,std::list<CAdapt<IFeaturePtr>>> m_listHilightFeatures;	//!< ハイライトフィーチャリスト
	IGeometryPtr m_ipHilightGeometry;

	std::list<CAdapt<IGeometryPtr> > m_listFinishGeometry; //!< 確定ジオメトリリスト
	std::list<CAdapt<IFeaturePtr>> m_listFinishFeatures;	//!< 確定フィーチャリスト
	IGeometryPtr m_ipFinishGeometry;
	ISymbolPtr m_ipHilightSymbol; //!< ハイライト時のシンボル
private:
	bool m_bShowContextMenu; //!< コンテキストメニューを出すかどうか
	bool m_bClosedPermission; //!< 形状の循環を許可するかどうか
	bool m_bTouchMidwayPoint; //!< 形状の分岐を許可するかどうか
	COLORREF m_color; //!< ハイライトの色
	bool m_bClickDir; //!< クリックしたポイントに近いほうに方向を向けるかどうか
	bool m_bVirtualFinish; //!< 仮終了モードを使うかどうか
	bool m_bFinished; //!< 仮終了モード時に使う終了モード
	bool m_bUseMin; //!< 選択フィーチャの抽出に選択位置に一番近いフィーチャを優先するかどうか
};

/**
 * @class CToolLineTrace
 * @brief ライン系をトレース選択する機能をサポートするマウス操作イベント処理クラス
 * 
 * フィーチャ単位でのトレースになります
 * @この機能は GuideEditToolで使用されています
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
	IFeatureClassPtr m_ipClass; //!< トレース対象のフィーチャクラス
	std::list<CAdapt<IGeometryPtr> > m_listHilightGeometry; //!< ハイライトさせるジオメトリリスト
	IGeometryPtr m_ipHilightGeometry;

	IArrayPtr m_ipFeatureArray;	//!< 確定フィーチャリスト
	IGeometryPtr m_ipFinishGeometry;
	ISymbolPtr m_ipHilightSymbol; //!< ハイライト時のシンボル
private:
	bool m_bShowContextMenu; //!< コンテキストメニューを出すかどうか
	bool m_bClosedPermission; //!< 形状の循環を許可するかどうか
	bool m_bTouchMidwayPoint; //!< 形状の分岐を許可するかどうか
	COLORREF m_color; //!< ハイライトの色
	bool m_bClickDir; //!< クリックしたポイントに近いほうに方向を向けるかどうか
	boost::shared_ptr<LinkSnapTrace> m_pLinkSnapTrace;
	bool m_bVirtualFinish; //!< 仮終了モードを使うかどうか
	bool m_bFinished; //!< 仮終了モード時に使う終了モード
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

		if( IsStartTracing() )// トレース元形状が確定している場合はトレース処理
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
			case 2:    // 右クリック
			{
				if( !m_ipSegmentGraphCursor )
					m_bShowContextMenu = true;
				else
					m_bShowContextMenu = false;

				RefreshItems();
				m_cApp.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );

				break;
			}
			case 1:    // 左クリック
			{
				// トレースモード用----------
				ILayerPtr ipCurrentLayer;
				// TOC選択レイヤを取得
				if( AheGetSelectedLayer( m_cApp.GetDocument(), &ipCurrentLayer, true ) )
				{
					IFeatureClassPtr ipFC = AheGetFeatureClass( ipCurrentLayer );
					esriGeometryType eType;
					eType = esriGeometryNull;
					if( ipFC )
						ipFC->get_ShapeType( &eType );
					// ライン系のレイヤのみ対応
					// トレース元フィーチャがあり、トレース形状がない場合はトレース処理開始
					if( eType == esriGeometryPolyline && IsStartTracing() && !m_ipSegmentGraphCursor )
					{
						m_bShowContextMenu = false;
						CreateGraph( ipPoint );
					}
					// すでにトレース形状がある場合は形状を取得
					// →GetShape()でm_ipTracePolylineを取得できるので後は呼び出し側にお任せ。
					else if( eType == esriGeometryPolyline && m_ipSegmentGraphCursor )
					{
						m_ipSegmentGraphCursor->get_CurrentTrace( &m_ipTracePolyline );
						m_ipSegmentGraphCursor = NULL;
						bHandled = true;
						// 一旦構成点を通り過ぎてから再度その構成点で確定すると、最終点が重複する（bug 7289）
						// しょうがないので、重複構成点を取り除く
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
				clear();	// フィードバック停止

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
	// トレース初期化
	void clear()
	{
		CToolPointer::clear();
		RefreshItems();
		m_bShowContextMenu = true;	// コンテキストメニューフラグON
	}
	// トレース初期化
	void RefreshItems()
	{
		m_ipSegmentGraphCursor = NULL;
		m_ipTracePolyline = NULL;
		m_listBaseGeom.clear();
	}
	// トレースライン形状作成処理
	void CreateGraph( IPoint* ipPoint )
	{
		if( !m_ipSegmentGraphCursor )
		{
			// トレース開始
			m_ipTracePolyline = NULL;
			IGeometryCollectionPtr ipTraceGeomBag( CLSID_GeometryBag );

			// [Bug 7819]ライントレースし、同位置に形状ペーストするとラインがずれる
			// ISegmentGraphにつっこむ形状に空間参照与えてやらないとずれる
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

			// 開始位置もスナップの対象とする
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
	* @brief トレース元形状を追加する
	* トレース形状のリセットではなく追加になります。
	* 前の形状を消すには ClearBaseGeometry を呼びます。
	* 複数の形状をセットできますが、マージ後に一筆書きにならない場合は
	* マウスポインタに一番近いものが自動的に選択されます。
	* @param ipBaseFeature [in] トレース元フィーチャ
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
	* @brief トレース元形状を追加する
	* @see AddBaseGeometry( IFeature* ipBaseFeature )
	* @param ipGeom [in] トレース元形状
	*/
	void AddBaseGeometry( IGeometryPtr& ipGeom )
	{
		if( ! ipGeom )
			return;

		m_listBaseGeom.push_back( ipGeom );
	}
	/**
	* @brief トレース元形状をクリアする
	*/
	void ClearBaseGeometry(){ GetBaseGeometries().clear(); }
	/**
	* @brief トレース元ジオメトリ取得
	*/
	std::list<CAdapt<IGeometryPtr>>& GetBaseGeometries()
	{
		return m_listBaseGeom;
	}
	/**
	* @brief トレースが開始されているかどうか
	*/
	bool IsStartTracing() const { return ! m_listBaseGeom.empty(); }
	/// トレース確定形状取得
	IGeometryPtr GetShape() const
	{
		return m_ipTracePolyline;
	}
	bool IsShowContextMenu() const { return m_bShowContextMenu; }
protected:
	ISymbolPtr m_ipHilightSymbol; //!< ハイライト時のシンボル
	ISegmentGraphCursorPtr	m_ipSegmentGraphCursor;
	IPolylinePtr			m_ipTracePolyline;
	std::list<CAdapt<IGeometryPtr>> m_listBaseGeom;//!< トレース元ジオメトリ
private:
	bool m_bShowContextMenu; //!< コンテキストメニューを出すかどうか
};

// GS案件で追加した形状トレースクラス
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

		if( IsStartTracing() )// トレース元形状が確定している場合はトレース処理
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
			case 1:    // 左クリック
			{
				ILayerPtr ipCurrentLayer;
				// TOC選択レイヤを取得
				if( AheGetSelectedLayer( m_cApp.GetDocument(), &ipCurrentLayer, true ) )
				{
					IFeatureClassPtr ipFC = AheGetFeatureClass( ipCurrentLayer );
					esriGeometryType eType = esriGeometryNull;
					if( ipFC )
						ipFC->get_ShapeType( &eType );
					// ライン系のレイヤのみ対応
					if( eType != esriGeometryPolyline )
						break;

					// トレース元フィーチャがあり、トレース形状がない場合はトレース処理開始
					if( IsStartTracing() )
					{
						// スナップしておく
						m_cApp.Snap( ipPoint );

						// ここまでに確定した形状確保しておく
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

							// 格納している最後の形状のToPointを取得する
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
			case 2:    // 右クリック
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
			// 今現在のトレースしている形状を描画
			if( m_ipTracePolyline )
				AheDrawPolyline( m_cApp.GetFocusMapScreenDisplay(), m_ipTracePolyline, RGB( 255, 100, 0 ), 5, esriSLSSolid, FALSE, TRUE );

			// これまでに確定している形状の描画
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
				clear();	// フィードバック停止
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

	// トレース初期化
	void clear()
	{
		CToolPointer::clear();
		RefreshItems();
		m_bShowContextMenu = true;	// コンテキストメニューフラグON
	}

	// トレース初期化
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

	// トレースライン形状作成処理
	void CreateGraph( IPoint* ipPoint )
	{
		// SegmentGraphのトレースの場合は、DisplayFeedback初期化しておく
		m_ipDisplayFeedback = NULL;
		// トレース開始
		m_ipTracePolyline = NULL;
		IGeometryCollectionPtr ipTraceGeomBag( CLSID_GeometryBag );

		// [Bug 7819]ライントレースし、同位置に形状ペーストするとラインがずれる
		// ISegmentGraphにつっこむ形状に空間参照与えてやらないとずれる
		if( ! m_listBaseGeom.empty() )
		{
			ISpatialReferencePtr ipSpRef;
			m_listBaseGeom.begin()->m_T->get_SpatialReference( &ipSpRef );
			((IGeometryPtr)ipTraceGeomBag)->putref_SpatialReference( ipSpRef );
		}
		ipTraceGeomBag->AddGeometry( m_listBaseGeom.rbegin()->m_T );
		ISegmentGraphPtr ipSegmentGraph( CLSID_SegmentGraph );

		ipSegmentGraph->Load( (IEnumGeometryPtr)ipTraceGeomBag, VARIANT_FALSE, VARIANT_TRUE );

		// 開始位置もスナップの対象とする
		ipSegmentGraph->GetCursor( ipPoint, &m_ipSegmentGraphCursor );
	}

	// 構成点フィードバック処理
	void VertexFeedback( IPointPtr ipPoint )
	{
		// VertexFeedbackやる際には、SegmentGraphのほうは初期化
		m_ipSegmentGraphCursor = NULL;
		m_ipDisplayFeedback.CreateInstance( CLSID_GroupFeedback );
		// グループフィードバックの準備（このipSetにセットされたものがフィードバックされる）
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
	* @brief トレース元形状を追加する
	* トレース形状のリセットではなく追加になります。
	* 前の形状を消すには ClearBaseGeometry を呼びます。
	* 複数の形状をセットできますが、マージ後に一筆書きにならない場合は
	* マウスポインタに一番近いものが自動的に選択されます。
	* @param ipBaseFeature [in] トレース元フィーチャ
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
	* @brief トレース元形状をクリアする
	*/
	void ClearBaseGeometry(){ GetBaseGeometries().clear(); }

	/**
	* @brief トレース元ジオメトリ取得
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
	* @brief トレースが開始されているかどうか
	*/
	bool IsStartTracing() const { return ! m_listBaseGeom.empty(); }

	/// トレース確定形状取得
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
	ISymbolPtr m_ipHilightSymbol; //!< ハイライト時のシンボル
	bool m_bVertexFeedback; //!< 構成点フィードバックしているかどうか
	ISegmentGraphCursorPtr	m_ipSegmentGraphCursor;  //!< フィードバック用のセグメントカーソル
	IDisplayFeedbackPtr		m_ipDisplayFeedback;	//!< フィードバック
	IPolylinePtr			m_ipTracePolyline;     //!< トレースしているライン形状
	std::list<CAdapt<IGeometryPtr>> m_listBaseGeom;//!< トレース元ジオメトリ
	IFeaturePtr m_ipCurrentTracingFeature;//!< トレース元ジオメトリ
	std::list< CAdapt< IPolylinePtr > > m_listFixedTraceGeom;//!< トレース形状
private:
	bool m_bShowContextMenu; //!< コンテキストメニューを出すかどうか
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
		if ( Button == 1 ) {		// 左クリック
			m_ipStartPoint = ipPoint;	// 開始ポイント保持
			m_bShowContextMenu = false;	// コンテキストメニューフラグOFF
		} else if ( Button == 2 ) {
			clear();	// フィードバック停止
			m_ipStartPoint = NULL;	// フィードバック形状初期化
		}

		return CToolPointer::OnMouseDown(Button, Shift, ipPoint, bHandled);
	}

	HRESULT OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		if ( Button == 1 ) {	// 左クリック
			if ( !m_bShowContextMenu ) {
				clear();	// フィードバック停止
				m_ipStartPoint = ipPoint;	// 開始ポイント保持
			}
		} else if ( Button == 2 ) {	// 右クリック
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

		m_ipStartPoint = NULL;		// 開始ポイント初期化
		m_bShowContextMenu = true;	// コンテキストメニューフラグON
	}

	void init()
	{
		CToolPointer::init();
		// ハイライトクリア
		clear();
	}

	/*!
		\fn		GetShape() const
		\brief	フィードバック確定形状取得
		\retval IGeometryPtr
	*/
	IGeometryPtr GetShape() const { return m_ipStartPoint; };
	bool IsShowContextMenu() const { return m_bShowContextMenu; }
protected:
	IPointPtr	m_ipStartPoint;		//!< フィードバック開始点

private:
	bool		m_bShowContextMenu;	//!< コンテキストメニューを出すかどうか
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
		if ( Button == 1 ) {		// 左クリック
			m_ipFeddbackGeom = NULL;	// フィードバック形状初期化
			m_ipStartPoint = ipPoint;	// 開始ポイント保持
			m_bShowContextMenu = false;	// コンテキストメニューフラグOFF
		} else if ( Button == 2 ) {
			clear();	// フィードバック停止
			m_ipFeddbackGeom = NULL;	// フィードバック形状初期化
		}

		return CToolPointer::OnMouseDown(Button, Shift, ipPoint, bHandled);
	}

	HRESULT OnMouseMove(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = false;

		if ( m_ipStartPoint ) {	// 開始ポイント保持？
			CToolPointer::clear();	// ポイントフィードバック停止

			if ( CSketchFeedback::IsStartingFeedback() )	// フィードバック開始状態？
				CSketchFeedback::MoveTo( ipPoint );	// フィードバック移動
			else
				CSketchFeedback::Start( m_ipStartPoint, sindyeSketch_Envelope );	// フィードバック開始

			bHandled = true;
		}

		if ( !bHandled )
			CToolPointer::OnMouseMove(Button, Shift, ipPoint, bHandled);

		return S_OK;
	}

	HRESULT OnMouseUp(LONG Button, LONG /*Shift*/, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = true;

		if ( Button == 1 ) {	// 左クリック
			m_ipFeddbackGeom = CSketchFeedback::Stop();	// フィードバック停止

			// 矩形が小さすぎないかチェック
			IScreenDisplayPtr ipDisp = m_cApp.GetFocusMapScreenDisplay();

			if ( ipDisp ) {
				IDisplayTransformationPtr ipDispTrans;

				ipDisp->get_DisplayTransformation( &ipDispTrans );

				if ( ipDispTrans ) {
					// MouseDown点
					long fx=0, fy=0;

					if ( m_ipStartPoint ) {
						ipDispTrans->FromMapPoint( m_ipStartPoint, &fx, &fy );

						// MouseUp点
						long lx=0, ly=0;

						ipDispTrans->FromMapPoint( ipPoint, &lx, &ly );

						if ( abs( fx - lx ) + abs( fy - ly ) <= 2 )
							m_ipFeddbackGeom = ipPoint;	// MouseUpポイントを格納
					}
				}
			}

			clear();	// フィードバック停止
		} else if ( Button == 2 ) {	// 右クリック
			clear();	// フィードバック停止
			m_ipFeddbackGeom = NULL;	// フィードバック形状初期化
		}

		return S_OK;
	}

	HRESULT OnKeyUp(LONG keyCode, LONG Shift, bool& bHandled)
	{
		bHandled = false;

		switch ( keyCode ) {
			case VK_ESCAPE: // ESC
				clear();	// フィードバック停止

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
		if ( CSketchFeedback::IsStartingFeedback() )	// フィードバック開始状態？
			CSketchFeedback::Stop();	// フィードバック停止

		m_ipStartPoint = NULL;		// 開始ポイント初期化
		m_bShowContextMenu = true;	// コンテキストメニューフラグON
	}

	void init()
	{
		CToolPointer::init();
		// ハイライトクリア
		clear();
	}

	/*!
		\fn		GetShape() const
		\brief	フィードバック確定形状取得
		\retval IGeometryPtr
	*/
	IGeometryPtr GetShape() const { return m_ipFeddbackGeom; };
	bool IsShowContextMenu() const { return m_bShowContextMenu; }
protected:
	IPointPtr		m_ipStartPoint;		//!< フィードバック開始点
	IGeometryPtr	m_ipFeddbackGeom;	//!< フィードバック形状

private:
	bool			m_bShowContextMenu;	//!< コンテキストメニューを出すかどうか
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
			if ( !CSketchFeedback::IsStartingFeedback() )	// フィードバック停止状態？
				m_ipFeddbackGeom = NULL;	// フィードバック形状初期化
		}

		return CToolPointer::OnMouseDown(Button, Shift, ipPoint, bHandled);
	}

	HRESULT OnMouseMove(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = true;

		// 選択範囲の表示
		CToolPointer::OnMouseMove(Button,Shift,ipPoint,bHandled);

		// スケッチフィードバック
		if ( CSketchFeedback::IsStartingFeedback() )	// フィードバック開始状態？
			CSketchFeedback::MoveTo( ipPoint );	// フィードバック移動

		return CToolPointer::OnMouseMove(Button,Shift,ipPoint,bHandled);
	}

	HRESULT OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = true;

		if ( Button == 1 ) {	// 左クリック
			if ( IsStartingFeedback() )	// フィードバック開始状態？
				CSketchFeedback::AddPoint( ipPoint );	// フィードバックポイント追加
			else {
				m_ipFeddbackGeom = NULL;	// フィードバック形状初期化
				m_bShowContextMenu = false;	// コンテキストメニューフラグOFF
				CSketchFeedback::Start( ipPoint, sindyeSketch_Polyline );	// フィードバック開始
			}
		} else if ( Button == 2 ) {	// 右クリック
			if ( CSketchFeedback::IsStartingFeedback() ) {	// フィードバック開始状態？
				if ( !CSketchFeedback::RemovePoint( ipPoint ) )	// フィードバック戻が成功？
					CSketchFeedback::Stop();	// フィードバック停止

			} else
				m_bShowContextMenu = true;	// コンテキストメニューフラグON
		}

		return CToolPointer::OnMouseUp(Button,Shift,ipPoint,bHandled);
	}

	HRESULT OnDblClick(bool& bHandled)
	{
		bHandled = true;

		if ( CSketchFeedback::IsStartingFeedback() ) {	// フィードバック開始状態？
			m_bShowContextMenu = true;	// コンテキストメニューフラグON

			m_ipFeddbackGeom = CSketchFeedback::Stop();	// フィードバック停止
		}

		return CToolPointer::OnDblClick(bHandled);
	}

	HRESULT OnKeyUp(LONG keyCode, LONG Shift, bool& bHandled)
	{
		bHandled = false;

		switch ( keyCode ) {
			case VK_ESCAPE: // ESC
				clear();	// フィードバック停止

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
		if ( CSketchFeedback::IsStartingFeedback() )	// フィードバック開始状態？
			CSketchFeedback::Stop();	// フィードバック停止

		m_bShowContextMenu = true;	// コンテキストメニューフラグON
	}

	void init()
	{
		CToolPointer::init();
		// ハイライトクリア
		clear();
	}

	/*!
		\fn		GetShape() const
		\brief	フィードバック確定形状取得
		\retval IGeometryPtr
	*/
	IGeometryPtr GetShape() const { return m_ipFeddbackGeom; };
	bool IsShowContextMenu() const { return m_bShowContextMenu; }
protected:
	IGeometryPtr	m_ipFeddbackGeom;	//!< フィードバック形状

	bool			m_bShowContextMenu;	//!< コンテキストメニューを出すかどうか
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
			if ( !CSketchFeedback::IsStartingFeedback() )	// フィードバック停止状態？
				m_ipFeddbackGeom = NULL;	// フィードバック形状初期化
		}

		return CToolPointer::OnMouseDown(Button, Shift, ipPoint, bHandled);
	}

	HRESULT OnMouseMove(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = true;

		if ( CSketchFeedback::IsStartingFeedback() ) // フィードバック開始状態？
			CSketchFeedback::MoveTo( ipPoint );	// フィードバック移動

		return CToolPointer::OnMouseMove(Button,Shift,ipPoint,bHandled);
	}

	HRESULT OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled)
	{
		bHandled = true;

		if ( Button == 1 ) {	// 左クリック
			if ( IsStartingFeedback() )	// フィードバック開始状態？
				CSketchFeedback::AddPoint( ipPoint );	// フィードバックポイント追加
			else {
				m_ipFeddbackGeom = NULL;	// フィードバック形状初期化
				m_bShowContextMenu = false;	// コンテキストメニューフラグOFF
				CSketchFeedback::Start( ipPoint, sindyeSketch_Polygon );	// フィードバック開始
			}
		} else if ( Button == 2 ) {	// 右クリック
			if ( CSketchFeedback::IsStartingFeedback() ) {	// フィードバック開始状態？
				if ( !CSketchFeedback::RemovePoint( ipPoint ) )	// フィードバック戻が成功？
					CSketchFeedback::Stop();	// フィードバック停止
			} else
				m_bShowContextMenu = true;	// コンテキストメニューフラグON
		}

		return CToolPointer::OnMouseUp(Button,Shift,ipPoint,bHandled);
	}

	HRESULT OnDblClick(bool& bHandled)
	{
		bHandled = true;

		if ( CSketchFeedback::IsStartingFeedback() ) {	// フィードバック開始状態？
			m_bShowContextMenu = true;	// コンテキストメニューフラグON

			m_ipFeddbackGeom = CSketchFeedback::Stop();	// フィードバック停止
		}

		return CToolPointer::OnDblClick(bHandled);
	}

	HRESULT OnKeyUp(LONG keyCode, LONG Shift, bool& bHandled)
	{
		bHandled = false;

		switch ( keyCode ) {
			case VK_ESCAPE: // ESC
				clear();	// フィードバック停止

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
		if ( CSketchFeedback::IsStartingFeedback() )	// フィードバック開始状態？
			CSketchFeedback::Stop();	// フィードバック停止

		m_bShowContextMenu = true;	// コンテキストメニューフラグON
	}

	void init()
	{
		CToolPointer::init();
		// ハイライトクリア
		clear();
	}
	/*!
		\fn		GetPointCount()
		\brief	フィードバックポイント数取得
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
		\brief	フィードバック確定形状取得
		\retval IGeometryPtr
	*/
	IGeometryPtr GetShape() const { return m_ipFeddbackGeom; };
	bool IsShowContextMenu() const { return m_bShowContextMenu; }
protected:
	IGeometryPtr	m_ipFeddbackGeom;	//!< フィードバック形状

private:
	bool			m_bShowContextMenu;	//!< コンテキストメニューを出すかどうか
};
