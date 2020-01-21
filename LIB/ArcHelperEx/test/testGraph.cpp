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
 * @brief �O���t�E�p�X���쐬����N���X�̃e�X�g�N���X
 * <h3>�`�F�b�N���e�F<h3>
 * @li �O���t�𐳂�����邱�Ƃ��ł��邩
 * @li �p�X�𐳂�����邱�Ƃ��ł��邩
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
		                                  //        �@
		listItem.push_back(CItem(0,1,0)); //     0�^  �_1
		listItem.push_back(CItem(1,2,1)); //    �^      �_
		listItem.push_back(CItem(0,4,2)); // 0 ��-2-�C-3-�A
		listItem.push_back(CItem(4,2,3)); //    �_      �^
		listItem.push_back(CItem(0,3,4)); //    4 �_  �^ 5
		listItem.push_back(CItem(3,2,5)); //        �B

		g.create_graph( listItem );
	}
	virtual ~CTestGraph(){}

	/// �O���t�����������ǂ����̃e�X�g
	void testGraph_CreateGraph()
	{
		for( graph<CItems,CItem>::const_iterator it = g.begin(); it != g.end(); ++it )
		{
			switch( (*it)->__edge_native.GetOID() )
			{
				case 0:
				{
					// 0�ׂ̗�1,2,4
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
					// 2�ׂ̗�0,3,4
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
					// 4�ׂ̗�0,2,5
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
	/// �p�X�����������ǂ����̃e�X�g
	void testGraph_CreatePath()
	{
		// �n�_�w��A�I�_���w��
		std::list<path<CItem> > listPath;
		std::list<long> listFrom, listTo;
		listFrom.push_back(0);
		listTo.push_back(2);
		g.create_path( listFrom, std::list<long>(), listPath );
		g.trace_path( listPath );
		ATLTRACE(_T("\n\n"));
		// �쐬���ꂽ�p�X��6
		//- 0 - 1 - 5 - 4 - 2 - 3
		//- 0 - 1 - 3 - 2 - 4 - 5
		//- 2 - 3 - 5 - 4 - 0 - 1
		//- 2 - 3 - 1 - 0 - 4 - 5
		//- 4 - 5 - 3 - 2 - 0 - 1
		//- 4 - 5 - 1 - 0 - 2 - 3
		CPPUNIT_ASSERT_EQUAL( (size_t)6, listPath.size() );
	}
	/// EndPoint�w�肪�����ƌ������ǂ����̃e�X�g
	void testGraph_CreatePath_WithEndPoint()
	{
		// �n�I�_�w��
		std::list<path<CItem> > listPath;
		std::list<long> listFrom, listEnd;
		listFrom.push_back(0); // �n�_�̓m�[�h0
		listEnd.push_back(2);  // �I�_�̓m�[�h2
		g.create_path( listFrom, listEnd, listPath );
		g.trace_path( listPath );
		ATLTRACE(_T("\n\n"));
		// �쐬���ꂽ�p�X��3
		//- 0 - 1
		//- 2 - 3
		//- 4 - 5
		CPPUNIT_ASSERT_EQUAL( (size_t)3, listPath.size() );
	}
	/// EndPoint����������ꍇ�̃e�X�g
	void testGraph_CreatePath_WithMultiEndPoint()
	{
		CItems cItems;
		cItems.push_back( CItem( 0, 1, 0 ) ); //      0     1
		cItems.push_back( CItem( 1, 2, 1 ) ); // 0��----�@----�A
		std::list<long> listStart, listEnd;   //     ��    ��
		listStart.push_back( 0 );
		listStart.push_back( 1 );
		listEnd.push_back( 1 );
		listEnd.push_back( 2 );

		std::list<path<CItem> > listPath;
		graph<CItems,CItem> g( cItems );
		g.create_path( listStart, listEnd, listPath );
		g.trace_path( listPath );
		ATLTRACE(_T("\n\n"));
		// �쐬���ꂽ�p�X��4
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
		cItems.push_back( CItem( 1, 2, 1 ) ); // 0��----�@----�A
		std::list<long> listStart, listEnd;   //     ��    ��
		listStart.push_back( 0 );
		listStart.push_back( 1 );
		listEnd.push_back( 1 );
		listEnd.push_back( 2 );

		std::list<path<CItem> > listPath;
		graph<CItems,CItem> g( cItems );
		g.create_path( listStart, listEnd, listPath );
		g.trace_path( listPath );
		ATLTRACE(_T("\n\n"));
		// �쐬���ꂽ�p�X��4
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
	// �L���O���t�e�X�g
	void testGraph_DirectedGraph2()
	{
		CItems items;
		std::list<path<directed_edge<CItem> > > listPath;
		// from_node_id, to_node_id, edge_id, �L�� or ����
		items.push_back( CItem( 0, 2, 100, false ) ); //         7
		items.push_back( CItem( 2, 1, 101, true  ) ); //         |106
		items.push_back( CItem( 3, 2, 102, true  ) ); // 4��104��5��105��6
		items.push_back( CItem( 2, 5, 103, false ) ); //         |103
		items.push_back( CItem( 4, 5, 104, true  ) ); // 1��101��2��102��3
		items.push_back( CItem( 5, 6, 105, true  ) ); //         |100
		items.push_back( CItem( 5, 7, 106, false ) ); //         0

		graph<CItems,directed_edge<CItem> > g( items );

		std::list<long> listStart;
		listStart.push_back( 0 );
		g.create_path( listStart, std::list<long>(), listPath );
		g.trace_path( listPath );
		ATLTRACE(_T("\n\n"));
		// �쐬���ꂽ�p�X��3
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
