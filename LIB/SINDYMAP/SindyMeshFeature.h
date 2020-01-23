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

// SindyMeshFeature.h: SindyMeshFeature クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYMESHFEATURE_H__1BF51652_FEE8_403A_98D0_D46B20C03B70__INCLUDED_)
#define AFX_SINDYMESHFEATURE_H__1BF51652_FEE8_403A_98D0_D46B20C03B70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyFeatureClass.h"

class SindyMeshFeature : public SindyFeatureClass  
{
public:
	SindyMeshFeature(SindyDBConnection *cDBConnection, const std::string &cName, const std::string &cMeshFieldName);
	virtual ~SindyMeshFeature();

	virtual HRESULT Load();
	virtual HRESULT SearchByMeshCode(long cMeshCode, IFeature **ipFeature);

protected:
	GDBOBJLIST mMeshList;

private:
	std::string mMeshFieldName;
};

#endif // !defined(AFX_SINDYMESHFEATURE_H__1BF51652_FEE8_403A_98D0_D46B20C03B70__INCLUDED_)
