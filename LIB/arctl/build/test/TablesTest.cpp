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

/**
 * @file TablesTest.cpp
 * @brief for @ref unit_test
 */

#include "stdafx.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <mockpp/ExpectationList.h>
#include <boost/mpl/list.hpp>
#include <arctl/tables.h>

namespace {

/// @class Tables1
ARCTL_TABLES_CLASS(Tables1, (T(kTable1))(F(kTable3)));

/// @class Tables2
ARCTL_TABLES_CLASS(Tables2, (T(kTable2)));

/// @class Tables
ARCTL_TABLES_CLASS(Tables, ((Tables1))((Tables2)));

typedef mockpp::ExpectationList<std::string> ExpectStringList;
typedef mockpp::ExpectationList<ITable*> ExpectTableList;

struct TestFunction
{
	TestFunction(ExpectStringList* pStringList, ExpectTableList* pTableList) :
	m_pStringList(pStringList),
	m_pTableList(pTableList)
	{
	}

	template <typename T>
	void operator()(const T& rT)
	{
		m_pStringList->addActual(std::string(CT2CA(rT.name())));
		m_pTableList->addActual(ITablePtr(rT.get()));
	}

	ExpectStringList* m_pStringList;
	ExpectTableList* m_pTableList;
};

} // anonymous namespace

/**
 * @brief arctl::tables のテストクラス
 */
class TablesTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(TablesTest);
	CPPUNIT_TEST(test1);
	CPPUNIT_TEST(test2);
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp()
	{
		m_pNameList = new ExpectStringList("NameList");
		m_pTableList = new ExpectTableList("TableList");
	}

	virtual void tearDown()
	{
		delete m_pNameList;
		delete m_pTableList;
	}

	void test1()
	{
		Tables1 aTables1;

		ITablePtr ipTable1(__uuidof(FeatureLayer));

		aTables1.get<kTable1>() = ipTable1;

		m_pNameList->addExpected(std::string(CT2CA(kTable1)));
		m_pTableList->addExpected(ipTable1);

		m_pNameList->addExpected(std::string(CT2CA(kTable3)));
		m_pTableList->addExpected(ITablePtr());

		rns::mpl::for_each(aTables1, TestFunction(m_pNameList, m_pTableList));
		m_pNameList->verify();
		m_pTableList->verify();
	}

	void test2()
	{
		Tables aTables;

		ITablePtr ipTable1(__uuidof(FeatureLayer));
		ITablePtr ipTable2(__uuidof(FeatureLayer));

		aTables.get<kTable1>() = ipTable1;
		aTables.get<kTable2>() = ipTable2;

		m_pNameList->addExpected(std::string(CT2CA(kTable1)));
		m_pTableList->addExpected(ipTable1);

		m_pNameList->addExpected(std::string(CT2CA(kTable3)));
		m_pTableList->addExpected(ITablePtr());

		m_pNameList->addExpected(std::string(CT2CA(kTable2)));
		m_pTableList->addExpected(ipTable2);

		rns::mpl::for_each(aTables, TestFunction(m_pNameList, m_pTableList));
		m_pNameList->verify();
		m_pTableList->verify();
	}

private:
	ExpectStringList* m_pNameList;
	ExpectTableList* m_pTableList;
};

CPPUNIT_TEST_SUITE_REGISTRATION(TablesTest);
