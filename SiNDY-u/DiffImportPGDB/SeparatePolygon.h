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
#include "sdebase.h"

class CSeparatePolygon :
	public CSDEBase
{
public:
	CSeparatePolygon();
	~CSeparatePolygon(void);
	bool init(const CArguments& args, const IWorkspacePtr& ipWorkspace,
		const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport );
private:
	bool fnExecMesh(COMMAND cCommand);
	bool fnSeparate(long cIndex);
	bool fnCheckAndSeparate(IFeaturePtr ipFeature, long cIndex);
	bool fnCopyAttr(IFeatureBufferPtr ipBuffer, IFeaturePtr ipFeature, long cIndex);
};
