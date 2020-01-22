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

// LQRowDef.h: CLQRowDef クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LQROWDEF_H__918B7FC0_F359_4A5B_B6B5_94768D3CDD1A__INCLUDED_)
#define AFX_LQROWDEF_H__918B7FC0_F359_4A5B_B6B5_94768D3CDD1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FeatureDef.h"

class CLQRowDef : public CFeatureDef  
{
public:
	CLQRowDef();
	virtual ~CLQRowDef();

	inline void SetLQRow( _IRowPtr ipLQRow, IGeometryPtr ipGeom, sindyeRoadQueueType type, LPCTSTR lpcszDispName, BOOL bDefault ){ SetFeature( ipLQRow ); m_ipGeom = ipGeom; m_type = type; m_strDispName = lpcszDispName; m_bDefault = bDefault; };
	inline IGeometryPtr GetShape(){ return m_ipGeom; };
	inline sindyeRoadQueueType GetType(){ return m_type; };
	inline LPCTSTR GetDispName(){ return m_strDispName; };
	inline BOOL GetDefault(){ return m_bDefault; };
	inline void SetDefault( BOOL bDefault ){ m_bDefault = bDefault; };
private:
	IGeometryPtr m_ipGeom;
	sindyeRoadQueueType m_type;
	CString m_strDispName;
	BOOL m_bDefault;
};

#endif // !defined(AFX_LQROWDEF_H__918B7FC0_F359_4A5B_B6B5_94768D3CDD1A__INCLUDED_)
