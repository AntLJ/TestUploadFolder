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
 *	@brief �n�C�E�F�C�m�[�h�W���N���X ��`��
 *	@author	F.Adachi
 *	@date	2005/10/21		�V�K�쐬
 *	$ID$
 */

#ifndef ___HIGHWAYNODECOLLECTION___
#define ___HIGHWAYNODECOLLECTION___

#include <string>
#include <map>
#include <set>
#include <boost/utility.hpp>

/// <b>�n�C�E�F�C�m�[�h�N���X</b>
struct HighwayNode
{
	long	m_ObjectID;
	long	m_RoadCode;
	long	m_TollClass;	//< ��������ʃR�[�h
};


/// <b>IPoint�̔�r����`����t�@���N�^</b>
struct point_less : public std::binary_function<IPoint*, IPoint*, bool>
{
	bool operator()( IPoint* lhs, IPoint* rhs ) const;
};


/// <b>�n�C�E�F�C�m�[�h�W���N���X</b>
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
	Data m_Data;	//< ���W�ƃn�C�E�F�C�m�[�h�̏W��

	static const std::string m_FeatureClassName;
};

#endif
