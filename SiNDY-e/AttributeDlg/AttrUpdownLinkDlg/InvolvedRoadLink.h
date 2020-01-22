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

#include "libschema.h"
#include "schema.h"

class InvolvedRoadLink
{
public:
	InvolvedRoadLink(void);
	InvolvedRoadLink(IFeaturePtr roadLink, _IRowPtr relRow );
	InvolvedRoadLink(IFeaturePtr roadLink);

	~InvolvedRoadLink(void){}

	long m_roadID;          //!< RoadLinkのOID
	IFeaturePtr m_roadLink; //!< RoadLink
	_IRowPtr	m_relRow;	//!< 道路リンク－上下線リンク関連レコード
	                        //   変更が合ったレコードか
	int m_directionC;		//!< 道路リンク－上下線リンク関連レコードの道路リンク方向コード
};

