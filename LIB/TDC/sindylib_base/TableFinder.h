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
#include <boost/utility.hpp>
#include "Model.h"

namespace sindy {

class CTableContainer;
typedef boost::shared_ptr<CTableContainer> CSPTableContainer;

class CTableFinder/* : boost::noncopyable*/
{
public:
	CTableFinder(void);
	CTableFinder( const CModel& cModel );
	virtual ~CTableFinder(void);

	CSPTableContainer FindTable( LPCTSTR lpcszTableName ) const;
	void InitCollection( ITable* ipTable, const CModel& model );
	const CModel& GetModel() const { return m_cModel; }

	/**
	 * @brief テーブルファインダに明示的にテーブルをセットする
	 *
	 * @warning 他のテーブルファインダとで共用したい場合にのみ
	 * 使用してください。通常は使用しないでください。
	 */
	void insert( LPCTSTR lpcszTableName, CSPTableContainer& con ) {
		m_mapCollection[lpcszTableName] = con;
	}

	typedef std::map<CString,CSPTableContainer>::iterator iterator;
	typedef std::map<CString,CSPTableContainer>::const_iterator const_iterator;
	iterator begin(){ return m_mapCollection.begin(); }
	iterator end(){ return m_mapCollection.end(); }
	const_iterator begin() const { return m_mapCollection.begin(); }
	const_iterator end() const { return m_mapCollection.end(); }
	bool empty() const { return m_mapCollection.empty(); }
	void ClearCollection();
protected:
	ITablePtr GetExistTable() const;
private:
	IWorkspacePtr m_ipWorkspace;
	mutable std::map<CString,CSPTableContainer> m_mapCollection;
	CModel m_cModel;
};

} // sindy
