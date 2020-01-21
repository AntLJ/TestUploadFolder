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

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include "item.h"
#include "graph.h"

/**
 * @class CTestGraph
 * @brief グラフ・パスを作成するクラスのテストクラス
 * <h3>チェック内容：<h3>
 * @li グラフを正しく作ることができるか
 * @li パスを正しく作ることができるか
 */
class CTestGraph : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(CTestGraph);
  CPPUNIT_TEST(testGraph_CreateGraph);
  CPPUNIT_TEST(testGraph_CreatePath);
  CPPUNIT_TEST(testGraph_CreatePath_WithEndPoint);
  CPPUNIT_TEST(testGraph_CreatePath_WithMultiEndPoint);
  CPPUNIT_TEST(testGraph_CreatePath_WithMultiEndPoint2);
  CPPUNIT_TEST_SUITE_END();
public:
	CTestGraph()
	{
		                                  //        ①
		listItem.push_back(CItem(0,1,0)); //     0／  ＼1
		listItem.push_back(CItem(1,2,1)); //    ／      ＼
		listItem.push_back(CItem(0,4,2)); // 0 ○-2-④-3-②
		listItem.push_back(CItem(4,2,3)); //    ＼      ／
		listItem.push_back(CItem(0,3,4)); //    4 ＼  ／ 5
		listItem.push_back(CItem(3,2,5)); //        ③

		g.create_graph( listItem );
	}
	virtual ~CTestGraph(){}

	/// グラフが正しいかどうかのテスト
	void testGraph_CreateGraph()
	{
		for( graph<CItems,CItem>::const_iterator it = g.begin(); it != g.end(); ++it )
		{
			switch( (*it)->__edge_native.GetOID() )
			{
				case 0:
				{
					// 0の隣は1,2,4
					std::list<long> lst;
					for( std::list<boost::shared_ptr<edge<CItem> > >::const_iterator it2 = (*it)->__neighbours.begin(); it2 != (*it)->__neighbours.end(); ++it2 )
						lst.push_back( (*it2)->__edge_native.GetOID() );
					CPPUNIT_ASSERT( std::find(lst.begin(),lst.end(),1) != lst.end() );
					CPPUNIT_ASSERT( std::find(lst.begin(),lst.end(),2) != lst.end() );
					CPPUNIT_ASSERT( std::find(lst.begin(),lst.end(),4) != lst.end() );
					CPPUNIT_ASSERT_EQUAL( size_t(3), lst.size() );
					break;
				}
				case 2:
				{
					// 2の隣は0,3,4
					std::list<long> lst;
					for( std::list<boost::shared_ptr<edge<CItem> > >::const_iterator it2 = (*it)->__neighbours.begin(); it2 != (*it)->__neighbours.end(); ++it2 )
						lst.push_back( (*it2)->__edge_native.GetOID() );
					CPPUNIT_ASSERT( std::find(lst.begin(),lst.end(),0) != lst.end() );
					CPPUNIT_ASSERT( std::find(lst.begin(),lst.end(),3) != lst.end() );
					CPPUNIT_ASSERT( std::find(lst.begin(),lst.end(),4) != lst.end() );
					CPPUNIT_ASSERT_EQUAL( size_t(3), lst.size() );
					break;
				}
				case 4:
				{
					// 4の隣は0,2,5
					std::list<long> lst;
					for( std::list<boost::shared_ptr<edge<CItem> > >::const_iterator it2 = (*it)->__neighbours.begin(); it2 != (*it)->__neighbours.end(); ++it2 )
						lst.push_back( (*it2)->__edge_native.GetOID() );
					CPPUNIT_ASSERT( std::find(lst.begin(),lst.end(),0) != lst.end() );
					CPPUNIT_ASSERT( std::find(lst.begin(),lst.end(),2) != lst.end() );
					CPPUNIT_ASSERT( std::find(lst.begin(),lst.end(),5) != lst.end() );
					CPPUNIT_ASSERT_EQUAL( size_t(3), lst.size() );
					break;
				}
				default:
					break;
			}
		}
	}
	/// パスが正しいかどうかのテスト
	void testGraph_CreatePath()
	{
		// 始点指定、終点未指定
		std::list<path<CItem> > listPath;
		std::list<long> listFrom, listTo;
		listFrom.push_back(0);
		listTo.push_back(2);
		g.create_path( listFrom, std::list<long>(), listPath );
		g.trace_path( listPath );
		ATLTRACE(_T("\n\n"));
		// 作成されたパスは6
		//- 0 - 1 - 5 - 4 - 2 - 3
		//- 0 - 1 - 3 - 2 - 4 - 5
		//- 2 - 3 - 5 - 4 - 0 - 1
		//- 2 - 3 - 1 - 0 - 4 - 5
		//- 4 - 5 - 3 - 2 - 0 - 1
		//- 4 - 5 - 1 - 0 - 2 - 3
		CPPUNIT_ASSERT_EQUAL( (size_t)6, listPath.size() );
	}
	/// EndPoint指定がちゃんと効くかどうかのテスト
	void testGraph_CreatePath_WithEndPoint()
	{
		// 始終点指定
		std::list<path<CItem> > listPath;
		std::list<long> listFrom, listEnd;
		listFrom.push_back(0); // 始点はノード0
		listEnd.push_back(2);  // 終点はノード2
		g.create_path( listFrom, listEnd, listPath );
		g.trace_path( listPath );
		ATLTRACE(_T("\n\n"));
		// 作成されたパスは3
		//- 0 - 1
		//- 2 - 3
		//- 4 - 5
		CPPUNIT_ASSERT_EQUAL( (size_t)3, listPath.size() );
	}
	/// EndPointが複数ある場合のテスト
	void testGraph_CreatePath_WithMultiEndPoint()
	{
		CItems cItems;
		cItems.push_back( CItem( 0, 1, 0 ) ); //      0     1
		cItems.push_back( CItem( 1, 2, 1 ) ); // 0○----①----②
		std::list<long> listStart, listEnd;   //     →    →
		listStart.push_back( 0 );
		listStart.push_back( 1 );
		listEnd.push_back( 1 );
		listEnd.push_back( 2 );

		std::list<path<CItem> > listPath;
		graph<CItems,CItem> g( cItems );
		g.create_path( listStart, listEnd, listPath );
		g.trace_path( listPath );
		ATLTRACE(_T("\n\n"));
		// 作成されたパスは4
		//[0]-0-[1]
		//[0]-0-1-[2]
		//[1]-1-[2]
		//[2]-1-[1]
		CPPUNIT_ASSERT_EQUAL( (size_t)4, listPath.size() );
	}
	void testGraph_CreatePath_WithMultiEndPoint2()
	{
		CItems cItems;
		cItems.push_back( CItem( 1, 0, 0 ) ); //      0     1
		cItems.push_back( CItem( 1, 2, 1 ) ); // 0○----①----②
		std::list<long> listStart, listEnd;   //     ←    →
		listStart.push_back( 0 );
		listStart.push_back( 1 );
		listEnd.push_back( 1 );
		listEnd.push_back( 2 );

		std::list<path<CItem> > listPath;
		graph<CItems,CItem> g( cItems );
		g.create_path( listStart, listEnd, listPath );
		g.trace_path( listPath );
		ATLTRACE(_T("\n\n"));
		// 作成されたパスは4
		//[0]-0-[1]
		//[0]-0-1-[2]
		//[1]-1-[2]
		//[2]-1-[1]
		CPPUNIT_ASSERT_EQUAL( (size_t)4, listPath.size() );
	}
	void testGraph_DirectedGraph1()
	{
		CItems items;
		std::list<path<directed_edge<CItem> > > listPath;
		                                        //            2
		items.push_back( CItem( 0, 1, 100 ) );  //            | 103
		items.push_back( CItem( 3, 1, 101 ) );  //            |
		items.push_back( CItem( 1, 4, 102 ) );  //  3 - 101 - 1 - 102 - 4
		items.push_back( CItem( 1, 2, 103 ) );  //            |
		                                        //            | 100
		                                        //            0

		graph<CItems,directed_edge<CItem> > g( items );

		std::list<long> listStart;
		listStart.push_back( 0 );
		g.create_path( listStart, std::list<long>(), listPath );
		g.trace_path( listPath );
	}
	// 有向グラフテスト
	void testGraph_DirectedGraph2()
	{
		CItems items;
		std::list<path<directed_edge<CItem> > > listPath;
		// from_node_id, to_node_id, edge_id, 有向 or 無向
		items.push_back( CItem( 0, 2, 100, false ) ); //         7
		items.push_back( CItem( 2, 1, 101, true  ) ); //         |106
		items.push_back( CItem( 3, 2, 102, true  ) ); // 4→104→5→105→6
		items.push_back( CItem( 2, 5, 103, false ) ); //         |103
		items.push_back( CItem( 4, 5, 104, true  ) ); // 1←101←2←102←3
		items.push_back( CItem( 5, 6, 105, true  ) ); //         |100
		items.push_back( CItem( 5, 7, 106, false ) ); //         0

		graph<CItems,directed_edge<CItem> > g( items );

		std::list<long> listStart;
		listStart.push_back( 0 );
		g.create_path( listStart, std::list<long>(), listPath );
		g.trace_path( listPath );
		ATLTRACE(_T("\n\n"));
		// 作成されたパスは3
		// [0]-100-103-106-[-1]
		// [0]-100-103-105-[-1]
		// [0]-100-101-[-1]
		CPPUNIT_ASSERT_EQUAL( (size_t)3, listPath.size() );
	}
private:
	CItems listFrom, listItem;
	graph<CItems, CItem> g;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CTestGraph);
