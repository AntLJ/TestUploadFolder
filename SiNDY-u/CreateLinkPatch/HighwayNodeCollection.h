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
 *	@file HighwayNodeCollection.h
 *	@brief ハイウェイノード集合クラス 定義部
 *	@author	F.Adachi
 *	@date	2005/10/21		新規作成
 *	$ID$
 */

#ifndef ___HIGHWAYNODECOLLECTION___
#define ___HIGHWAYNODECOLLECTION___

#include <string>
#include <map>
#include <set>
#include <boost/utility.hpp>

/// <b>ハイウェイノードクラス</b>
struct HighwayNode
{
	long	m_ObjectID;
	long	m_RoadCode;
	long	m_TollClass;	//< 料金所種別コード
};


/// <b>IPointの比較基準を定義するファンクタ</b>
struct point_less : public std::binary_function<IPoint*, IPoint*, bool>
{
	bool operator()( IPoint* lhs, IPoint* rhs ) const;
};


/// <b>ハイウェイノード集合クラス</b>
class HighwayNodeCollection : boost::noncopyable
{
public:
	HighwayNodeCollection(void);
	~HighwayNodeCollection();

	void read( IWorkspace* cpWorkspace );
	void disp( std::ostream& crStream ) const;
	void getHighwayNode( IPoint* cpPoint, std::set<const HighwayNode*>* cpHighwayNodeArray ) const;

protected:
	void assign( IPoint* cpPoint, IFeature* cpFeature );

private:
	typedef	std::multimap<IPointPtr, HighwayNode, point_less> Data;
	Data m_Data;	//< 座標とハイウェイノードの集合

	static const std::string m_FeatureClassName;
};

#endif
