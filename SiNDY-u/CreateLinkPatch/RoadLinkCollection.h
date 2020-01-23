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
 *	@file RoadLinkCollection.h
 *	@brief ���H�����N�W���N���X ��`��
 *	@author	F.Adachi
 *	@date	2005/10/21		�V�K�쐬
 *	$ID$
 */

#ifndef ___ROADLINKCOLLECTION___
#define ___ROADLINKCOLLECTION___

#include <string>
#include <map>
#include <boost/utility.hpp>

/// <b>���H�����N�N���X</b>
struct RoadLink
{
	long			m_RoadCode;
	long			m_RoadClass;
	long			m_NaviClass;
	long			m_UpDownClass;
	long			m_MainLinkClass;
	long			m_FromNodeID;
	long			m_ToNodeID;
	IGeometryPtr	m_pShape;

	RoadLink(void);
	~RoadLink();
};


/// <b>���H�����N�W���N���X</b>
class RoadLinkCollection : boost::noncopyable
{
public:
	typedef std::map<long, RoadLink> Data;

	RoadLinkCollection(void);
	~RoadLinkCollection();

	void read( IWorkspace* cpWorkspace, std::string cWhereClauseStr, std::string cSubFieldStr );
	void disp( std::ostream& crStream ) const;

	const Data* getRoadLinkCollection(void) const { return &m_Data; }
	RoadLink	getRoadLink( long cObjectID ) const;
	int			getMeshCode( long cObjectID ) const;

protected:
	void assign( IFeature* cpFeature );

private:
	Data m_Data;	//< ���H�����N�W��

	static const std::string m_FeatureClassName;
public:
	typedef Data::const_iterator RoadLinkIter;
};

#endif

