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

// SketchFeedback.h: CSketchFeedback クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKETCHFEEDBACK_H__E4192E70_27CD_428E_A00E_784E5A9B9F4E__INCLUDED_)
#define AFX_SKETCHFEEDBACK_H__E4192E70_27CD_428E_A00E_784E5A9B9F4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum sindyeSketchMode {
	sindyeSketch_Point,
	sindyeSketch_Envelope,
	sindyeSketch_Polyline,
	sindyeSketch_Polygon,
};

class CSketchFeedback  
{
public:
	CSketchFeedback();
	virtual ~CSketchFeedback();

	void SetDisplay( IScreenDisplay* ipDisp ){ m_ipDisp = ipDisp; }

	bool Start( IPoint* ipStart, sindyeSketchMode emMode )
	{
		switch( emMode )
		{
			case sindyeSketch_Point:
				if( !StartPointFeedback( ipStart ) ) return false; break;
			case sindyeSketch_Envelope:
				if( !StartEnvelopeFeedback( ipStart ) ) return false; break;
			case sindyeSketch_Polyline:
				if( !StartLineFeedback( ipStart ) ) return false; break;
			case sindyeSketch_Polygon:
				if( !StartPolygonFeedback( ipStart ) ) return false; break;
			default: return false; break;
		}
		return true;
	}
	BOOL MoveTo( IPointPtr ipMovePoint );
	BOOL AddPoint( IPointPtr ipAddPoint );
	BOOL RemovePoint( IPointPtr ipMovePoint );

	/**
	 * @brief フィードバックを再開する
	 *
	 * @param ipFeedbackGeom	[in]	Stopしたときの形状
	 *
	 * @retval TRUE 再開成功
	 * @retval FALSE 再開失敗
	 */
	BOOL RestartFeedback( IGeometryPtr ipFeedbackGeom );
	IGeometryPtr Stop();
	void Refresh( OLE_HANDLE hDC );
	BOOL IsStartingFeedback();
	LONG GetPointCount();
	IPointPtr GetPoint( long lNum );
private:
	bool StartPointFeedback( IPoint* ipStart );
	bool StartEnvelopeFeedback( IPoint* ipStart );
	bool StartLineFeedback( IPoint* ipStart );
	bool StartPolygonFeedback( IPoint* ipStart );
	bool StartVertexFeedback( IPoint* ipStart );
	ISymbolPtr GetSketchSymbol();
	ISymbolPtr GetSnapSymbol();
	ISymbolPtr GetSketchVertexSymbol();
//	IEditPropertiesPtr GetEditProp();
private:
#pragma warning(push)
#pragma warning(disable:4251)
//	CArcHelper										m_cArcHelper;
//	IEditPropertiesPtr								m_ipEditProp;

	IDisplayFeedbackPtr								m_ipFeedback;
	std::list< CAdapt< IMovePointFeedbackPtr > >	m_listVertexFeedback;
//	CFeedback										m_cSnapPointFeedback;

	ILineSymbolPtr									m_ipSketchSymbol;
	IMarkerSymbolPtr								m_ipVertexSymbol;
//	IMarkerSymbolPtr								m_ipSnapSymbol;

	IPointCollectionPtr                             m_ipPolygonGeom;      // ポリゴン形状保存用
protected:
	IScreenDisplayPtr								m_ipDisp;
#pragma warning(pop)
};

#endif // !defined(AFX_SKETCHFEEDBACK_H__E4192E70_27CD_428E_A00E_784E5A9B9F4E__INCLUDED_)
