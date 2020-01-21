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

#include <boost/shared_ptr.hpp>

namespace sindy {
class CRowContainer;
class CTableContainer;
class CTableFinder;
typedef boost::shared_ptr<CRowContainer> CSPRowContainer;
typedef boost::shared_ptr<CTableContainer> CSPTableContainer;
}

sindy::CSPRowContainer _RowClassFactory( const sindy::CTableContainer& cTable, _IRowPtr& ipRow );
sindy::CSPTableContainer _TableClassFactory( const sindy::CTableFinder& cFinder, ITablePtr& ipTable );
sindy::CSPTableContainer _FastTableClassFactory( const sindy::CTableFinder& cFinder, LPCTSTR lpcszTableName );

typedef sindy::CSPRowContainer (*RowClassFactoryFunc)( const sindy::CTableContainer& cTable, _IRowPtr& ipRow );
typedef sindy::CSPTableContainer (*TableClassFactoryFunc)( const sindy::CTableFinder& cFinder, ITablePtr& ipTable );
typedef sindy::CSPTableContainer (*FastTableClassFactoryFunc)( const sindy::CTableFinder& cFinder, LPCTSTR lpcszTableName );

extern RowClassFactoryFunc RowClassFactory;
extern TableClassFactoryFunc TableClassFactory;
extern FastTableClassFactoryFunc FastTableClassFactory;
