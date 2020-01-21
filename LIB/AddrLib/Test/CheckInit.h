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

#include "stdafx.h"

#include <SiNDYLib/Workspace.h>
#include <SiNDYLib/FeatureClass.h>

#include <AddrLib/AddrCodeList.h>
#include <AddrLib/AreaCodeHelper.h>
#include <AddrLib/Building.h>
#include <AddrLib/CityAdmin.h>
#include <AddrLib/CSAddrPoint.h>
#include <AddrLib/GouPoint.h>
#include <AddrLib/GouPoints.h>
#include <AddrLib/JusyoHelper.h>
#include <AddrLib/KLinkPoint.h>
#include <AddrLib/ZipcodeArea.h>

#include <boost/noncopyable.hpp>
#include <mutex>

namespace
{
	const CString verificationDatDir = _T("\\\\win\\tdc\\ced-ref\\verification\\LIB\\AddrLib\\UnitTest\\dat\\");
}

class CheckInit : private boost::noncopyable
{
private:
	CheckInit();
	virtual ~CheckInit(){};

public:
	static CheckInit& GetInstance();
	static std::mutex s_mutex;
};
