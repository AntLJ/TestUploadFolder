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

// dllmain.h : モジュール クラスの宣言

class CcrdcnvLibModule : public CAtlDllModuleT< CcrdcnvLibModule >
{
public :
	DECLARE_LIBID(LIBID_crdcnvLibLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_CRDCNVLIB, "{9F321FB5-86DD-4F7D-862A-993B2113D7AA}")
};

extern class CcrdcnvLibModule _AtlModule;
