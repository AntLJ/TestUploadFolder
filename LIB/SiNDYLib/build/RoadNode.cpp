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

// Node.cpp: CNode �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RoadNode.h"
#include "meshutil.h"
#include "ErrorObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace road_node {
using namespace errorcode;

// ���������`�F�b�N����
errorcode::sindyErrCode CRoadNode::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrs ) const
{
	// ��{�`�F�b�N
	CFeature::CheckLogic( cRule, cErrs );

	CErrorInfos cErrInfos;
	// �m�[�h�̈ʒu�̑Ó����`�F�b�N�i�񎟃��b�V�������Ƃ̐����j
	int ix = 0, iy = 0;
	CGeometry::const_point_iterator it = GetShape()->point_begin();
	LL2MESH( it.get_x(), it.get_y(), 2, ix, iy );

	// �m�[�h��4���ɂȂ����ǂ���
	if( (0 == ix || 1000000 == ix) && (0 == iy || 1000000 == iy) )
		cErrInfos.push_back( CErrorInfo( sindyErr_NodeExist4MeshEdge, sindyErrLevel_ERR ) );

	// ���H�A���s�҂̃m�[�h��ʖ��i���������ǁA�A�j
	std::map<CString, CString> nodeClassNames = boost::assign::map_list_of
		(CString(schema::road_node::kTableName), CString(schema::road_node::kNodeClass))
		(CString(schema::walk_node::kTableName), CString(schema::walk_node::kNodeClass));

	// �e�m�[�h��2�����b�V����m�[�h���
	std::map< CString, std::set<long> > onMeshNodeClass = boost::assign::map_list_of
		(CString(schema::road_node::kTableName), boost::assign::list_of          // ���H
			((long)schema::road_node::node_class::kEdge)
			((long)schema::road_node::node_class::kPecEdge))

		(CString(schema::walk_node::kTableName), boost::assign::list_of          // ���s��
			((long)schema::walk_node::node_class::kOutline));

	// �m�[�h���
	long nodeClass = GetLongValueByFieldName(nodeClassNames[GetTableName()], -1);

	// �e��̃m�[�h��ʂŃ��b�V����`�F�b�N�ibug 12213�j
	if (onMeshNodeClass[GetTableName()].count(nodeClass) > 0)
	{
		// ���b�V��������ɂ��邩�ǂ���
		if ((0 != ix && 1000000 != ix) && (0 != iy && 1000000 != iy))
			cErrInfos.push_back(CErrorInfo(sindyErr_NodeOfMeshEdgeNotExistMeshEdge, sindyErrLevel_ERR));
	}
	else
	{
		// ���b�V��������ɂȂ����ǂ���
		if ((0 == ix || 1000000 == ix) || (0 == iy || 1000000 == iy))
			cErrInfos.push_back(CErrorInfo(sindyErr_NodeOfMeshEdgeNotExistMeshEdge, sindyErrLevel_ERR));
	}

	if( sindyErr_NoErr != cErrInfos.GetDefaultErrCode() )
	{
		CErrorObject cErr(*this); // TODO: cErrs.push_back( *this, cErrorInfos ) �Ƃ��ł����ق����ǂ�
		cErr.push_back( cErrInfos );
		cErrs.push_back( cErr );
	}
	return cErrInfos.GetDefaultErrCode();
}

// ��������ʂ��ݒ肳��Ă��邩
errorcode::sindyErrCode CRoadNode::CheckNodeClass() const
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l
/*	/// �m�[�h�ڂ��铹�H�����N���擾
	int nCount = 0;//GetTouchedRoadLink();
	
	node_class::ECode eNode = GetNodeClass_C();

	if ( 1 == nCount && eNode != node_class::kTerminal ) {
		/// ���H����{�����Ȃ��̂ɒ[�_�m�[�h����Ȃ�
		return errorcode::sindyErr_InvalidNodeClass;
	}

	if ( 3 >= nCount && eNode != node_class::kCross ) {
		/// �O�{�ȏ�̓��H���ڂ��Ă�̂Ɍ����_����Ȃ�
		return errorcode::sindyErr_InvalidNodeClass;
	}

	return errorcode::sindyErr_NoErr;*/

	emErr = sindyErr_NoImplimentation;
	return emErr;
}

bool CRoadNode::IsExistAttrNode( bool bCheckAll/* = true*/ ) const
{
	// �����_�t���O���t���Ă��邩�ǂ���
	if ( GetSignalCode() == 1 )
		 return true;

	// �����_���̂����邩�ǂ���
	TCHAR lpcszNameKanji[2000];

	if ( GetNameKanji( 2000, lpcszNameKanji ) && lstrcmpi( _T(""), lpcszNameKanji ) != 0 )
		return true;

	// �����_���Ȗ��̂����邩�ǂ���
	TCHAR lpcszNameKana[2000];

	if ( GetNameYomi( 2000, lpcszNameKana ) && lstrcmpi( _T(""), lpcszNameKana ) != 0 )
		return true;

	if ( bCheckAll ) {
		// �����R�[�h���t���Ă��邩�ǂ���
		if ( GetHeightCode() != 0 )
			return true;

		// �����f�[�^�������Ă��邩�ǂ���
		if ( GetHeight() != 0 )
			return true;
	}

	return false;
}

bool CRoadNode::IsBorderNode() const
{
	bool bRet = false;
	long nNodeClass = GetLongValueByFieldName( schema::road_node::kNodeClass, -1 );

	if ( nNodeClass == schema::road_node::node_class::kEdge || 
		nNodeClass == schema::road_node::node_class::kPecEdge || 
		nNodeClass == schema::walk_node::node_class::kOutline )
		bRet = true;

	return bRet;
}

} // road_node

} // sindy
