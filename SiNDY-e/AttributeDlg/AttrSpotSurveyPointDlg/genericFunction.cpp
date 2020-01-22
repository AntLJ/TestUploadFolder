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

#include "stdafx.h"
#include "genericFunction.h"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <TDC/useful_headers/str_util.h>

#include <ArcHelperEx/AheLayerFunctions.h>

namespace generic
{

IDockableWindowPtr getDockableWindow(const CLSID& clsid, IApplication* ipApp)
{
	IDockableWindowManagerPtr ipDocWndMng = (IApplicationPtr)ipApp;
	if( !ipDocWndMng ) return nullptr;

	IUIDPtr ipUID(CLSID_UID);
	ipUID->put_Value( CComVariant( uh::str_util::ToString(clsid)) );

	IDockableWindowPtr ipDockableWindow;
	ipDocWndMng->GetDockableWindow(ipUID, reinterpret_cast<IDockableWindow**>(&ipDockableWindow));

	return ipDockableWindow;
}

} // generic
