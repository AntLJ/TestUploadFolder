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
 * @file RoadNetwork.h
 * @brief <b>CRoadNetwork�N���X��`�t�@�C��</b>\n
 * @author 
 * @vertion $Id$
 */
#ifndef _ROADNETWORK_H_
#define _ROADNETWORK_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RelRoad.h"
#include "RelNode.h"
#include "LinkQueue.h"
//#include "FeatureClass.h"
#include "FeatureContainer.h"
#include "util.h"
#include "ListLong.h"

namespace sindy {

// 2�����b�V����`�Ō������邩�ǂ����̃t���O
// 
// 2�����b�V���Ńl�b�g���[�N����������ꍇ�͌�����`��
// �Ŋ֘A�t�B�[�`�����������邽�߁A�������ȗ�������܂��B
// �o�b�`��������2�����b�V���P�ʂŏ���������ۂɂ͂���
// �t���O��ON�ɂ��邱�ƂŌ����̍ۂɂ��Ȃ荂��������܂��B
// 
// �����l�� false �ł��B
extern bool g_bNetworkSearchBy2ndMesh;

/**
 * @class CRoadNetwork
 * @brief <b>�l�b�g���[�N�Ɋւ��鑍���������s���N���X</b>\n
 * ���̃N���X�ł́A�l�b�g���[�N�����N�A�l�b�g���[�N�m�[�h�A�y�ѓ���֌W�ɂ�����̂̊֘A�t���A�����A�������ʂ�
 * �֘A�t���ƊǗ����s���܂��B
 */
class SINDYLIB_API CRoadNetwork : 
	public CFeatureContainer
{
public:
	explicit CRoadNetwork(){}
	virtual ~CRoadNetwork(){ /*clear();*/ }

	explicit CRoadNetwork( IFeatureClass* lp )
	{
		SetObject( lp );
	}

	CRoadNetwork& operator=( IFeatureClass* lp )
	{
		SetObject( lp );
		clear();
		return *this;
	}

	sindyTableType::ECode GetCurrentTableType() const {return m_emTableType;}

	//@{ @name CFeaturesContainer �C���^�[�t�F�[�X
	/**
	 * @brief <b>�`��Ō�������</b>\n
	 * ���H�����N�A���H�m�[�h�A�����N�������A�X�΂��������܂��B�������ʂ͎����I��
	 * �֘A�t����ꃊ�X�g���ɕێ�����܂��B\n
	 * �����Ńo�b�t�@�����O�Ȃǂ͂��Ȃ��̂ŁA�|�C���g�Ō�������ꍇ��
	 * �o�b�t�@�����O�����`���n���Ă��������B\n
	 * @param ipGeom		[in]			�����`��
	 * @param bRecycling	[in,optional]	true�ɂ���ƁA���H�����N�A���H�m�[�h�A�����N�������A�X�΂̃L���b�V�����쐬���Đ؂藣��
	 * @param bCreateCache	[in,optional]	�L���b�V�����쐬����ibRecycling��true�̏ꍇ�͂��̃t���O�Ɋ֌W�Ȃ��K���쐬�����j
	 * @param pErrs			[out,optional]	�G���[�i�[�R���e�i�i�I�u�W�F�N�g���m�̊֘A�t���Ɏ��s�������̂̂ݓ���j
	 * @param bGetRelated	[in,optional]	�֘A���铹�H���擾���邩�ǂ���
	 *
	 * @return sindyErrCode	�G���[������ꍇ��cErrs��GetDefaultErrorCode�̌��ʂ��Ԃ�
	 */
	errorcode::sindyErrCode Select( IGeometry* ipGeom, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true );
	errorcode::sindyErrCode _Select( IGeometry* ipGeom, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true, bool bGetRelated = true );
	errorcode::sindyErrCode _Select( IQueryFilter* ipFilter, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true, bool bGetRelated = true );

	/**
	 * @brief <b>�`��Ō�������</b>\n
	 * ���H�����N�A���H�m�[�h�A�����N�������A�X�΂��������܂��B�������ʂ͎����I��
	 * �֘A�t����ꃊ�X�g���ɕێ�����܂��B\n
	 * �����Ńo�b�t�@�����O�Ȃǂ͂��Ȃ��̂ŁA�|�C���g�Ō�������ꍇ��
	 * �o�b�t�@�����O�����`���n���Ă��������B\n
	 * @param ipGeom		[in]			�����`��
	 * @param bRecycling	[in,optional]	true�ɂ���ƁA���H�����N�A���H�m�[�h�A�����N�������A�X�΂̃L���b�V�����쐬���Đ؂藣��
	 * @param bCreateCache	[in,optional]	�L���b�V�����쐬����ibRecycling��true�̏ꍇ�͂��̃t���O�Ɋ֌W�Ȃ��K���쐬�����j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SelectByObject( _IRow* ipRow, bool bRecycling = false, bool bCreateCache = false, bool bInit = true );
	errorcode::sindyErrCode _SelectByObject( _IRow* ipRow, bool bRecycling = false, bool bCreateCache = false, bool bInit = true, bool bGetRelated = true );

	/**
	 * @brief <b>�w�肵�����HOID�Ɋ֘A���郊���N��Ɋ֘A���铹�H����������</b>\n
	 * �����N��Ɋ֘A���铹�H�̂݌������܂��B
	 * @param listRoadIDs	[in]			���HOID�Q
	 * @param bRecycling	[in,optional]	true�ɂ���ƁA���H�����N�A���H�m�[�h�A�����N�������A�X�΂̃L���b�V�����쐬���Đ؂藣��
	 * @param bCreateCache	[in,optional]	�L���b�V�����쐬����ibRecycling��true�̏ꍇ�͂��̃t���O�Ɋ֌W�Ȃ��K���쐬�����j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SelectByRoadFromLinkqs(const CListLong& listRoadIDs, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* cErrs = NULL);

	/**
	 * @brief <b>�w�肵�����HOID�Ɋ֘A���郊���N��Ɋ֘A���铹�H����������</b>\n
	 * 2�����b�V����m�[�h�ɐڑ����Ă��铹�H�����鎞�́A���Α��̓��H�������Ώۂɒǉ�����
	 * @param listRoadIDs		[in]		�������HOID�Q
	 * @param listSearchRoads	[in/out]	�������HOID�Q�i2�����b�V����m�[�h�ɐڑ����铹�H���܂ށj
	 * @return sindyErrCode
	 */
	void _SelectByRoadFromLinkqs(const CListLong& listRoadIDs, CListLong& listSearchRoads );

	/**
	 * @brief <b>�^����ꂽ�`��Ƃ̌��������ŕ����������s��</b>\n
	 * @param pSource		[in]	�����Ώ�
	 * @param cGeom			[in]	�����`��
	 * @param pSource		[in]	�����`��ƈ�v����t�B�[�`�����폜���邩�ǂ���
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects );
	errorcode::sindyErrCode _Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>�^����ꂽ�t�B�[�`���̃}�[�W���s��</b>\n
	 * �w��t�B�[�`�����i�[����Ă��邱�ƁB
	 * @param pSource		[in]	�}�[�W��
	 * @param pDest			[in]	�}�[�W��
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Merge( CSPRow pSource, CSPRow pDest, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>�^����ꂽ�`��͈̔͂ɂ���_�̍폜���s��</b>\n
	 * @param pRow			[in]	�폜�Ώ�
	 * @param cGeom			[in]	�폜�`��
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�R���e�i
	 * @param bEraseOnly	[in/optional]	�������g�݂̂ɂ��邩�ǂ����ipRow������֌W�̎��ɂ����g��Ȃ��j
	 *                                      �폜�c�[���̃Z�O�����g�폜�Ŏn�_�E�I�_�ǂ��炩���܂܂�Ă��鎞�ɁA
	 *                                      OID��ێ����邽�߂ɍ\���_�\���_�폜������悤�ɂ��Ă���֌W��
	 *                                      pRow�Ŏw�肵�Ă���ȊO�̓���֌W�͏������Ȃ��悤�ɂ��邽�߂����Ɏg�p
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Erase( CSPRow pRow, const CGeometry& cGeom, CErrorObjects& cErrObjects, bool bEraseOnly=false );

	/**
	 * @brief <b>�t�B�[�`���̍폜���s��</b>\n
	 * �w��t�B�[�`�����i�[����Ă��邱�ƁB
	 * @param pRow			[in]	�폜�Ώ�
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Delete( CSPRow pRow, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>�t�B�[�`���̍폜���s��</b>\n
	 * �w��t�B�[�`�����i�[����Ă��邱�ƁB
	 * @param pRow				[in]	�폜�Ώ�
	 * @param listexclusions	[in]	���O�I�u�W�F�N�g
	 * @param bDelEdgeNode		[in]	2�����b�V����m�[�h�ɐڑ����Ă��铹�H���폜���邩�ǂ����i�ʏ�͕K��true�̂��ƁAfalse��Erase()����Ă΂ꂽ�Ƃ��̂݁j
	 * @param cErrObjects		[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode _Delete( CSPRow pRow, CContainer& listexclusions, bool bDelEdgeNode, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>���g����������</b>\n
	 * Split���OID��ێ����������̂����ւ��������Ƃ��Ɏg�p���邱�ƁB
	 * �쐬/�폜�t���O�͌������Ȃ��悤�ɂ��Ă���܂��B
	 * @param pSource			[in]	�y�[�X�g�Ώ�
	 * @param pDest				[in]	�R�s�[�Ώ�
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode swap( CSPRow pSource, CSPRow pDest );

	/**
	 * @brief <b>LQ�̍폜���s��</b>\n
	 * �w��t�B�[�`�����i�[����Ă��邱�ƁB
	 * @param pRow				[in]	�폜�Ώہi���H�j
	 * @param cErrObjects		[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode _DeleteLQ( CSPRow pRow, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>�����N��̍폜���s��</b>\n
	 * �w��t�B�[�`�����i�[����Ă��邱�ƁB
	 * @param cDelLinkq			[in]	�폜�Ώ�
	 * @param cErrObjects		[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode DeleteLinkq( CLinkQueue& cDelLinkq, CErrorObjects& cErrObjects );

	/**
	 * ���H�̂�LQRF�̍Đݒ肷��
	 *
	 * @param emLinkqType		[in]	�����N���ʃ^�C�v
	 * @param cErrObjects		[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 *
	 * @retval true LQRF�̍Đݒ萬��
	 * @retval false ���s
	 */
	errorcode::sindyErrCode ResetRoadLQRF(sindyTableType::ECode emLinkqType, CErrorObjects& cErrObjects);

	/**
	 * @brief <b>�w�肵�������N���LQ�̍폜���s��</b>\n
	 * �w��t�B�[�`�����i�[����Ă��邱�ƁB
	 * @param pLinkq			[in]	�폜�ΏہiINF�j
	 * @param cLQ				[in]	�폜�ΏہiLQ�j
	 * @param cErrObjects		[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode DeleteLQ( CLinkQueue* pLinkq, const link_queue::CLQ& cLQ, CErrorObjects& cErrObjects );
	/**
	 * @brief <b>�����N��̎��̉����s��</b>\n
	 */
	void MaterializationLinkq();

	/**
	 * �m�[�h�̎�ʂ��Đݒ肷��
	 *
	 * @param cRow				[in]			�m�[�h�t�B�[�`��
	 * @param bIsChanged		[out,optional]	�ύX���ꂽ���ǂ���
	 *
	 * @retval true �m�[�h��ʍĐݒ萬��
	 * @retval false ��ʂ�����ł��Ȃ�����
	 */
	bool ResetNodeAttribute( CSPRow cRow, bool* bIsChanged = NULL );

	/**
	 * �������Ǝv���铹�H�m�[�h��ʂ�Ԃ�
	 *
	 * @note cFeatureItems �ɂ́AipNodeFeature ���ڑ����邷�ׂ̓��H�����N���܂܂�Ă��邱�Ƃ��O��ł��B
	 * @note ���̑O������𖞂����Ă��Ȃ��ꍇ�A�Ԃ�l�̓��H�m�[�h��ʂ��������ݒ�ł��Ȃ��ꍇ������܂��B
	 *
	 * @param cNode				[in]			���H�m�[�h��ʕύX�O�̃m�[�h
	 * @param pbHasNodeAttr		[out]			���H�m�[�h�ɐڑ����铹�H�����N��2�{�ȉ��̏ꍇ�Ƀm�[�h�̌����_���̋y�ѐM���@
	 * @return sindyeNodeClass
	 */
	schema::road_node::node_class::ECode GetModifyRoadNodeKind( const CSPRow& cRow, bool* pbHasNodeAttr ) const;

	/**
	 * �������Ǝv������s�҃m�[�h��ʂ�Ԃ�
	 *
	 * @note cFeatureItems �ɂ́AipNodeFeature ���ڑ����邷�ׂ̕��s�҃����N���܂܂�Ă��邱�Ƃ��O��ł��B
	 * @note ���̑O������𖞂����Ă��Ȃ��ꍇ�A�Ԃ�l�̕��s�҃m�[�h��ʂ��������ݒ�ł��Ȃ��ꍇ������܂��B
	 *
	 * @param cRow			[in]			���s�҃m�[�h��ʕύX�O�̃m�[�h
	 *
	 * @return sindyeWalkNodeClass
	 */
	schema::walk_node::node_class::ECode GetModifyWalkNodeKind( const CSPRow& cRow ) const;
	
	/**
	 * @brief �������Ǝv����n���S�m�[�h��ʂ�Ԃ�
	 * @param cRow [in] �n���S�m�[�h��ʕύX�O�̃m�[�h
	 * @return �X�L�[�}���C�u�����ɒ�`���ꂽ�n���S�m�[�h���
	 */
	schema::sj::subway_node::node_class::ECode GetModifySubwayNodeKind( const CSPRow& cRow ) const;

	/**
	 * @brief �������Ǝv����S���m�[�h��ʂ�Ԃ�
	 * @param cRow [in] �S���m�[�h��ʕύX�O�̃m�[�h
	 * @return �S���m�[�h���
	 */
	sindy::schema::sj::railway_node::node_class::ECode GetModifyRailwayNodeKind( const CSPRow& cRow ) const;

	/**
	 * @brief <b>�ҏW���e��ۑ�����</b>\n
	 * ���̊֐��ł͐������̃`�F�b�N���s���܂���B\n
	 * �K��Check�֐���ǂ񂾌�ɂ��̊֐����Ă�ł��������B\n
	 */
	errorcode::sindyErrCode Store( CErrorObjects& cErrObjects );

	/**
	 * @brief <b>���������`�F�b�N����</b>\n
	 * �t�B�[�`����ҏW������ɕK���`�F�b�N���s���Ă��������B
	 * @param cRule			[in]	�_���`�F�b�N�p���[��
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const;

	/**
	 * @brief <b>�R���t���N�g����̂��߂̊��ҏW�`�F�b�N���s��</b>\n
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckModify( CErrorObjects& cErrObjects ) const;

	/**
	 * @brief <b>�L���b�V������Ă��邩�ǂ������`�F�b�N����</b>\n
	 * �L���b�V������Ă��Ȃ����̂̓G���[���O�ɏo�͂���܂��B
	 * @retval true		�S�ăL���b�V������Ă���
	 * @retval false	��ł��L���b�V������Ă��Ȃ����̂�����
	 */
	bool IsCached() const;

	/**
	 * @brief <b>�o�^����Ă���t�B�[�`��������΂��̃|�C���^��Ԃ�</b>\n
	 * ���̂Ƃ��둮���_�C�A���O�Ŏg�p��
	 * @return CRow*	�o�^����Ă���t�B�[�`���̃|�C���^
	 */
	const CRow* FindObject( _IRow* ipRow ) const;

	//@}

	//@{ @name �����N��^�C�v�C�e���[�^
	inf_iterator inf_begin() const { return LQTypeIterator(); }
	inf_iterator inf_end() const { return LQTypeIterator(false,true); }
	//@}

	//@{ @name �����N��R���e�i����֐�
	typedef std::list<CLinkQueue>::iterator linkq_iterator;
	typedef std::list<CLinkQueue>::const_iterator const_linkq_iterator;
	typedef std::map<sindyTableType::ECode, std::list<CLinkQueue> >::iterator linkqs_iterator;
	typedef std::map<sindyTableType::ECode, std::list<CLinkQueue> >::const_iterator const_linkqs_iterator;
	linkqs_iterator linkqs_begin(){ return m_mapLinkQueue.begin(); }
	linkqs_iterator linkqs_end(){ return m_mapLinkQueue.end(); }
	const_linkqs_iterator linkqs_begin() const { return m_mapLinkQueue.begin(); }
	const_linkqs_iterator linkqs_end() const { return m_mapLinkQueue.end(); }
	linkq_iterator linkq_begin( sindyTableType::ECode type ) { return m_mapLinkQueue[type].begin(); }
	linkq_iterator linkq_end( sindyTableType::ECode type ) { return m_mapLinkQueue[type].end(); }
	const_linkq_iterator linkq_begin( sindyTableType::ECode type ) const { return m_mapLinkQueue.find(type)->second.begin(); }
	const_linkq_iterator linkq_end( sindyTableType::ECode type ) const { return m_mapLinkQueue.find(type)->second.end(); }
	linkq_iterator linkq_find( sindyTableType::ECode type, long oid )
	{
		linkq_iterator itEnd = linkq_end(type);
		for( linkq_iterator it = linkq_begin(type); it != itEnd; ++it )
		{
			if( it->GetOID() == oid )
				return it;
		}
		return itEnd;
	}
	const_linkq_iterator linkq_find( sindyTableType::ECode type, long oid ) const 
	{
		const_linkq_iterator itEnd = linkq_end(type);
		for( const_linkq_iterator it = linkq_begin(type); it != itEnd; ++it )
		{
			if( it->GetOID() == oid )
				return it;
		}
		return itEnd;
	}
	linkq_iterator linkq_erase(sindyTableType::ECode type, linkq_iterator it )
	{
		return m_mapLinkQueue[type].erase( it );
	}
	//@}
	/**
	 * @brief <b>�����o���N���A����</b>\n
	 */
	void clear();
private:
#pragma warning(push)
#pragma warning(disable:4251)
//	std::map<long,CRelRoad>										m_mapRelRoad;	//!< ���H�����N�֘A�N���X���X�g�i�L�[�FOBJECTID�j
//	std::map<long,CRelNode>										m_mapRelNode;	//!< �m�[�h�֘A�N���X���X�g�i�L�[�FOBJECTID�j
	std::map<sindyTableType::ECode, std::list<CLinkQueue> >	m_mapLinkQueue;	//!< �����N�񃊃X�g�i�L�[�FOBJECTID�j
//	TRowsContainer m_mapRowsContainer; //!< �ێ�����I�u�W�F�N�g�R���e�i�i�L�[�FsindyTableType::ECode,OBJECTID�j
#pragma warning(pop)
	/**
	* @brief 2�����b�V������m�[�h����
	* @note �n���S�H���Ή��̍ۂ�GetModifyWalkNodeKind()���番��
	*       AheGetModifySubwayNodeKind()�Ƌ��ʂŎg�p����B
	* @param cRow�@�@�@�@�@�@�@�@[in] �@�@�m�[�h��ʕύX�O�̃m�[�h
	* @param listConnectRoads�@[in]    �m�[�h�ɐڑ����郊���N
	* @param hasErr            [out]   ����������ɏI���������ۂ� 
	* @return                          2�����b�V������m�[�h���ۂ�
	*/
	bool OnMeshEdgeNode( const CSPRow& cRow, std::list<road_link::CRoadLink*>& listConnectRoads, bool& hasErr ) const;
	
	/**
	* @brief ���H�m�[�h�㔻��
	* @param geom [in] �Ώۂ̌`��i�|�C���g�܂��̓}���`�|�C���g�j
	* @param onlyMeshBorder [in] true�Ȃ�2�����b�V�����E�m�[�h����
	* @return geom��2���H�m�[�h��ɂ��邩�ۂ�
	*/
	bool isOnNodePoint( const CGeometry& geom, bool onlyMeshBorder = false );

	/**
	* @brief 2�����b�V���Őڂ������֌W�̑�����擾����
	* @param row [in] �Ώۂ̃t�B�[�`��
	* @param useOrgShape [in] row�̌��`��i�ύX�O�j���g�p����ꍇ��true
	* @return �Ώۂ̃t�B�[�`���ƂQ�����b�V�����E�Őڂ��铯��̓���֌W�t�B�[�`��
	*         �Q�����b�V���ɐڂ��Ă��Ȃ��ꍇ��NULL
	*/
	CSPRow getContainOppositeMesh( const CSPRow& row, bool useOrgShape = false );
	
	/**
	* @brief ����֌W���̂���\���_���폜����
	* @note �e�ƂȂ铹�H�����N��A�����Ɋ܂܂�鑼�̓���֌W�Ƃ̋��L�\���_���폜����
	* @param containRel�@�@[in,out] �@�@�Ώۂ̓���֌W�i�����N�������A�X�΁j
	* @param delVertex�@   [in]        �폜����\���_�`��i�|�C���g�A�܂��̓}���`�|�C���g�j
	* @param cErrObjects   [out]       �G���[���
	*/
	void deleteContainVertex( CSPRow& containRel, const CGeometry& delVertex, CErrorObjects& cErrObjects );
	
	/**
	* @brief ����֌W���̂��폜����
	* @param containLine�@�@[in,out] �@�@�Ώۂ̓���֌W�i�����N�������A�X�΁j
	*/
	void deleteContainRelation( CSPRow& containLine );
	
	/**
	* @brief ����֌W���̂Ɋ֘A���鋤�L�\���_���폜����
	* @note �e�ƂȂ铹�H�����N��A�����Ɋ܂܂�鑼�̓���֌W�Ƃ̋��L�\���_���폜����
	* @param containRel�@[in,out] �@�@�Ώۂ̓���֌W�i�����N�������A�X�΁j
	* @param vertexs �@�@[in] �@�@    �폜����\���_�̌`��
	*/
	void deleteSharedVertex( CSPRow& containRel, const CGeometry& vertices );
	
	/**
	* @brief ����֌W���̂̐e�ƂȂ铹�H�����N���擾����
	* @param containRel�@�@[in] �@�@�Ώۂ̓���֌W�i�����N�������A�X�΁j
	*/
	CSPRow getParentLink( const CSPRow& containRel );
};

} // sindy

#endif // _ROADNETWORK_H_
