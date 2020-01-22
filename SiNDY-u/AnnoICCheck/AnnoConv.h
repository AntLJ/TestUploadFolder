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

// AnnoConv.h: AnnoConv クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANNOCONV_H__538D72B8_D38F_49CA_A0D6_B7AA9EAC5711__INCLUDED_)
#define AFX_ANNOCONV_H__538D72B8_D38F_49CA_A0D6_B7AA9EAC5711__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef std::map<long, CString, std::less<long> > ANNO_TABLE;

class AnnoConv  
{
public:
	LPCTSTR getAttrName(LONG cSindyAttr);
	bool init(IFeatureClassPtr  ipAnno);
	AnnoConv();
	virtual ~AnnoConv();
protected:
	ANNO_TABLE m_Table;
};

#endif // !defined(AFX_ANNOCONV_H__538D72B8_D38F_49CA_A0D6_B7AA9EAC5711__INCLUDED_)
