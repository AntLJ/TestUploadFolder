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

#include "ToolPointer.h"

class CToolMultiPolyline : public CToolPolyline
{
public:
   /**
	* @brief  コンストラクタ
	*/
	CToolMultiPolyline(){}
	CToolMultiPolyline( IApplication* ipApp, bool bWithArea=false ) : CToolPolyline(ipApp,bWithArea) {}

   /**
	* @brief  デストラクタ
	*/
	virtual ~CToolMultiPolyline() { init(); }


	HRESULT OnMouseDown(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled);
	HRESULT OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled);
	HRESULT OnDblClick(bool& bHandled);
	HRESULT OnKeyUp(LONG keyCode, LONG Shift, bool& bHandled);

   /**
	* @brief  フィードバックのリスタート
	* @param[in] ipFeedbackGeom リスタートするフィードバック形状
	* @retval true   リスタート成功
	* @retval false  リスタート失敗
	*/
	bool Restart( IGeometry* ipFeedbackGeom );

   /**
	* @brief  フィードバック形状をマルチパートのポリラインで返す
	* @return フィードバック形状
	*/
	IGeometryPtr GetShape() const;

   /**
	* @brief  初期化
	*/
	void init();

   /**
	* @brief  フィードバック形状初期化
	*/
	void clear();

   /**
	* @brief  フィードバック形状をシングルパートのジオメトリ群で返す
	* @return フィードバック形状
	*/
	std::vector<CAdapt<IGeometryPtr>> GetShapeAsSingleParts() const { return m_FeedbackGeomList; };
	 

private:
	typedef std::vector<CAdapt<IGeometryPtr>>::const_iterator CIter;

	std::vector<CAdapt<IGeometryPtr>> m_FeedbackGeomList;	//!< フィードバック形状群

};


