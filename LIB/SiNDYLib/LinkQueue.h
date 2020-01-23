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

// LinkQueue.h: CLinkQueue �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(_LINKQUEUE_H_)
#define _LINKQUEUE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Info.h"
#include "LQ.h"
#include "ErrorObject.h"

namespace sindy {

class LQTypeIterator
{
public:
	LQTypeIterator( bool _r = false, bool _end = false )
	{
		__reverce = _r;
		if( _end )
			__inf = sindyTableType::unknown;
		else
			__inf = ( __reverce ) ? sindyTableType::inf_intersection : sindyTableType::inf_byway;
	}
	const LQTypeIterator& operator++() {
		switch( __inf )
		{
			case sindyTableType::inf_byway:		__inf = ( __reverce ) ? sindyTableType::unknown			: sindyTableType::inf_dirguide; break;
			case sindyTableType::inf_dirguide:	__inf = ( __reverce ) ? sindyTableType::inf_byway		: sindyTableType::inf_guide; break;
			case sindyTableType::inf_guide:		__inf = ( __reverce ) ? sindyTableType::inf_dirguide	: sindyTableType::inf_internavi; break;
			case sindyTableType::inf_internavi:	__inf = ( __reverce ) ? sindyTableType::inf_guide		: sindyTableType::inf_lane; break;
			case sindyTableType::inf_lane:		__inf = ( __reverce ) ? sindyTableType::inf_internavi	: sindyTableType::inf_route; break;
			case sindyTableType::inf_route:		__inf = ( __reverce ) ? sindyTableType::inf_lane		: sindyTableType::inf_turnreg; break;
			case sindyTableType::inf_turnreg:	__inf = ( __reverce ) ? sindyTableType::inf_route		: sindyTableType::inf_uturn; break;
			case sindyTableType::inf_uturn:		__inf = ( __reverce ) ? sindyTableType::inf_turnreg		: sindyTableType::inf_vics; break;
			case sindyTableType::inf_vics:		__inf = ( __reverce ) ? sindyTableType::inf_uturn		: sindyTableType::inf_ipcvics; break;
			case sindyTableType::inf_ipcvics:	__inf = ( __reverce ) ? sindyTableType::inf_vics		: sindyTableType::inf_intersection; break;
			case sindyTableType::inf_intersection:	__inf = ( __reverce ) ? sindyTableType::inf_ipcvics		: sindyTableType::unknown; break;
			default: __inf = sindyTableType::unknown; break;
		}
		return *this;
	}
	const LQTypeIterator& operator--() {
		__reverce = ! __reverce;
		operator++();
		__reverce = ! __reverce;
	}
	const sindyTableType::ECode& operator*() const { return __inf; }
	LPCTSTR lqrf() const {
		switch( __inf )
		{
			case sindyTableType::inf_byway:		return schema::road_link::kBywayLQRF; break;
			case sindyTableType::inf_dirguide:	return schema::road_link::kDirGuideLQRF; break;
			case sindyTableType::inf_guide:		return schema::road_link::kGuideLQRF; break;
			case sindyTableType::inf_internavi:	return schema::road_link::kInternaviLQRF; break;
			case sindyTableType::inf_lane:		return schema::road_link::kLaneLQRF; break;
			case sindyTableType::inf_route:		return schema::road_link::kRouteLQRF; break;
			case sindyTableType::inf_turnreg:	return schema::road_link::kTurnRegLQRF; break;
			case sindyTableType::inf_uturn:		return schema::road_link::kUturnLQRF; break;
			case sindyTableType::inf_vics:		return schema::road_link::kVICSLQRF; break;
			case sindyTableType::inf_ipcvics:	return schema::road_link::kIpcVicsLQRF; break;
			case sindyTableType::inf_intersection:	return schema::road_link::kIntersectionLQRF; break;
			default:							return NULL; break;
		}
	}
	sindyTableType::ECode lq() const {
		switch( __inf )
		{
			case sindyTableType::inf_byway:		return sindyTableType::lq_byway; break;
			case sindyTableType::inf_dirguide:	return sindyTableType::lq_dirguide; break;
			case sindyTableType::inf_guide:		return sindyTableType::lq_guide; break;
			case sindyTableType::inf_internavi:	return sindyTableType::lq_internavi; break;
			case sindyTableType::inf_lane:		return sindyTableType::lq_lane; break;
			case sindyTableType::inf_route:		return sindyTableType::lq_route; break;
			case sindyTableType::inf_turnreg:	return sindyTableType::lq_turnreg; break;
			case sindyTableType::inf_uturn:		return sindyTableType::lq_uturn; break;
			case sindyTableType::inf_vics:		return sindyTableType::lq_vics; break;
			case sindyTableType::inf_ipcvics:	return sindyTableType::lq_ipcvics; break;
			case sindyTableType::inf_intersection:	return sindyTableType::lq_intersection; break;
			default:							return sindyTableType::unknown; break;
		}
	}
	bool operator !=( const LQTypeIterator& __sc ) const { return ( ! ( **this == *__sc)  ); }
	bool operator ==( const LQTypeIterator& __sc ) const { return ( **this == *__sc ); }
private:
	sindyTableType::ECode __inf;
	bool __reverce;
};
typedef LQTypeIterator inf_iterator;

class SINDYLIB_API CLinkQueue : public link_queue::CInfo
{
public:
	explicit CLinkQueue(){}
	virtual ~CLinkQueue()
	{
		clear();
	}

	CLinkQueue& operator=( const CLinkQueue& obj )
	{
		CopyCLinkQueue( *this, obj );
		return *this;
	}

	friend errorcode::sindyErrCode CopyCLinkQueue( CLinkQueue& dest, const CLinkQueue& src );

	explicit CLinkQueue( _IRow* lp )
	{
		*this = lp;
	}
	CLinkQueue& operator=( _IRow* lp )
	{
		clear();
		SetObject( lp );
		return *this;
	}

	CLinkQueue( sindyTableType::ECode emType, _IRow* lp )
	{
		SetObject( lp, emType );
	}

	CLinkQueue( sindyTableType::ECode emType, _IRow* lp, const CSPFieldMap& cFields, const CSPTableNameString& cNames )
	{
		SetObject( lp, emType, false, cFields, cNames );
	}

	errorcode::sindyErrCode Materialization(ITable* ipInfTable, ITable* ipLqTable);

	//@{ @name LQ�R���e�i����֐�
	typedef std::list<link_queue::CLQ>::iterator iterator;
	typedef std::list<link_queue::CLQ>::const_iterator const_iterator;
	typedef std::list<link_queue::CLQ>::reverse_iterator reverse_iterator;
	typedef std::list<link_queue::CLQ>::const_reverse_iterator const_reverse_iterator;
	iterator begin() { return m_listLQ.begin(); }
	iterator end() { return m_listLQ.end(); }
	const_iterator begin() const { return m_listLQ.begin(); }
	const_iterator end() const { return m_listLQ.end(); }
	reverse_iterator rbegin() { return m_listLQ.rbegin(); }
	reverse_iterator rend() { return m_listLQ.rend(); }
	const_reverse_iterator rbegin() const { return m_listLQ.rbegin(); }
	const_reverse_iterator rend() const { return m_listLQ.rend(); }
	iterator remove_lq( const link_queue::CLQ& cLQ )
	{
		for( iterator it = begin(); it != end(); ++it ) {
			if ( cLQ == (*it) )
				return m_listLQ.erase( it );
		}

		return end();
	}
	void push_back_lq( const link_queue::CLQ& cLQ ) { m_listLQ.push_back( cLQ ); }
	bool empty_lq() { return m_listLQ.empty(); }
	/**
	 * @brief <b>LQ���V�[�P���X���Ƀ\�[�g����</b>\n
	 * �V�[�P���X�ԍ��̃`�F�b�N�͍s���܂���̂ŁA�`�F�b�N����ꍇ��
	 * CheckLQSequence���ĂԕK�v������܂��B
	 */
	void sort() { m_listLQ.sort(); }

	/**
	 * @brief <b>�����o���N���A����</b>\n
	 */
	void clear();

	/**
	 * @brief <b>LQ��</b>\n
	 */
	long size() const { return m_listLQ.size(); }

	/**
	 * @brief <b>LQ���Z�b�g����</b>\n
	 * LQ���Z�b�g���I�������K��sort���Ă��
	 * LQ���V�[�P���X���ɕ��בւ���K�v������܂��B\n\n
	 * LQ��ǉ�����ہAINF��ID�Ɛ������`�F�b�N���܂��B
	 * @param emType	[in]	�ǉ�����LQ�̃^�C�v
	 * @param lp		[in]	�ǉ�����LQ
	 * @retval sindyErr_NoErr	�ǉ�����
	 * @retval ��L�ȊO			�ǉ����s
	 */
	errorcode::sindyErrCode push_back( sindyTableType::ECode emType, _IRow* lp, CSPFieldMap& cFields, CSPTableNameString& cNames );

	/**
	 * @brief <b>�w�肵�����H��OID������LQ�����邩</b>\n
	 * @param cRow	[in]	���H
	 */
	bool HasRoad(const CRow& cRow) const;

	/**
	 * @brief <b>�����N����폜����</b>\n
	 * @param cErrObjects		[in]	�G���[�I�u�W�F�N�g
	 */
	errorcode::sindyErrCode Delete( CErrorObjects& cErrObjects );

	/**
	 * @brief <b>LQ���폜����</b>\n
	 * @param cRow				[in]	�폜����LQ�Ɋ֘A���铹�H
	 * @param cNewLinkq			[in/out]	�폜�������ʁA�������ꂽ�ꍇ��LQ���i�[�����
	 * @param cErrObjects		[in/out]	�G���[�I�u�W�F�N�g
	 */
	errorcode::sindyErrCode DeleteLQ( const CRow& cRow, CLinkQueue& cNewLinkq, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>���g����������</b>\n
	 * @param cLinkq			[in]	�������郊���N��
	 * @param bIsSwapLQ			[in]	LQ�Q���������邩�ǂ���
	 * @param bIsCopyCache		[in]	�L���b�V�����R�s�[���邩�ǂ���
	 * @param bIsCopyRow		[in]	�I�u�W�F�N�g���R�s�[���邩�ǂ���
	 * @param bIsCopyFlag		[in]	�쐬�폜�t���O���R�s�[���邩�ǂ���
	 * @param bIsCopyContainer	[in]	�R���e�i���R�s�[���邩�ǂ���
	 */
	void swap( CLinkQueue& cLinkq, bool bIsSwapLQ, bool bIsCopyCache=true, bool bIsCopyRow=true, bool bIsCopyFlag=true, bool bIsCopyContainer=true );

	/**
	 * @brief <b>�����N����}�[�W����</b>\n
	 * @param cSource			[in]	�}�[�W�����H
	 * @param cDest				[in]	�}�[�W�擹�H
	 * @param bIsReverse		[in]	�}�[�W��Ɛ�̌������Ⴄ���ǂ���
	 * @param emTableType		[in]	���H�̃e�[�u���^�C�v
	 * @param cRoadnetwork		[in]	���H�Q
	 * @param cErrObjects		[in]	�G���[�I�u�W�F�N�g
	 */
	errorcode::sindyErrCode Merge( const CRow& cSource, const CRow& cDest, bool bIsReverse, sindyTableType::ECode emTableType, const CContainer& cRoads, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>�����N��̌`����擾</b>\n
	 * @param bUseEditUpdate	[in]	�ҏW��Ԃ��l�����邩
	 * @param emTableType		[in]	���H�̃e�[�u���^�C�v
	 * @param cRoadnetwork		[in]	���H�Q
	 */
	IGeometryPtr CreateLQGeometry(bool bUseEditUpdate, sindyTableType::ECode emTableType, const CContainer& cRoadnetwork);

	/**
	 * @brief <b>�����N��𕪊�����</b>\n
	 * @param cOrgRoadRow		[in]	�I���W�i���̓��H
	 * @param emTableType		[in]	���H�̃e�[�u���^�C�v
	 * @param cNewRoads			[in]	�I���W�i���̓��H���番���ɂ���č쐬���ꂽ���H�Q
	 * @param cErrObjects		[in]	�G���[�I�u�W�F�N�gLQ
	 */
	errorcode::sindyErrCode Split( const CRow& cOrgRoadRow, sindyTableType::ECode emTableType, const CContainer& cNewRoads, CErrorObjects& cErrObjects );

	//@}
	//@{ @name �`�F�b�N�֐�
	/**
	 * @brief <b>�����N����`�F�b�N����</b>\n
	 * ���̃`�F�b�N�֐��ł͈ȉ��̍��ڂ��`�F�b�N���܂��B
	 * @li INF�P�̃`�F�b�N
	 * @li LQ�P�̃`�F�b�N
	 * @li INF��LQ��ID�̐��������Ă��邩�ǂ���
	 * @li LQ�̃V�[�P���X�ԍ��y�т��̕��т��K�؂��ǂ���
	 * ���H�����N�Ƃ̐������܂ł̓`�F�b�N���܂���B
	 * @retval sindyErr_NoErr	�G���[����
	 * @retval ��L�ȊO			�G���[
	 */
	errorcode::sindyErrCode Check() const;

	/**
	 * @brief <b>LQ�̃V�[�P���X���`�F�b�N����</b>\n
	 * ���̊֐����ĂԑO��sort����K�v������܂��B
	 * @retval sindyErr_NoErr	�G���[����
	 * @retval ��L�ȊO			�G���[
	 */
	errorcode::sindyErrCode CheckLQSequence() const;
	//@}

	/**
	 * @brief <b>�ҏW���e��ۑ�����</b>\n
	 * ���̊֐��ł͐������̃`�F�b�N���s���܂���B\n
	 * �K��Check�֐���ǂ񂾌�ɂ��̊֐����Ă�ł��������B
	 */
	errorcode::sindyErrCode Store( CErrorObjects& cErrObjects );

	/**
	 * @brief <b>���ɕҏW����Ă��Ȃ����ǂ����m�F����</b>\nj
	 * �ύX�����������̂ɑ΂��Ă̂݃`�F�b�N���s���܂��B\n
	 * �L���b�V�����쐬���Ă��Ȃ��ƕύX�̃`�F�b�N�Ɉ���������Ȃ��̂Œ��ӂ��Ă��������B\n
	 * @param cErrObjects	[in,out]	�G���[�I�u�W�F�N�g�����閔�́A���g���`�F�b�N����
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckModify( CErrorObjects& cErrObjects ) const;

	/**
	 * @brief <b>���������`�F�b�N����</b>\n
	 * �t�B�[�`����ҏW������ɕK���`�F�b�N���s���Ă��������B
	 * @param cRule			[in]	�_���`�F�b�N�p���[��
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const;

	/**
	 * @brief <b>�����ɕύX�����������ǂ������`�F�b�N����</b>\n
	 * �L���b�V�����Ă���ꍇ�Ɍ���L���ł��B�L���b�V�����Ă��Ȃ���΁A���
	 * false���Ԃ�܂��B
	 * ���z�֐��Ȃ̂́A�����N���CLQ�ɕύX������������CInfo���X�V���Ȃ���΂����Ȃ�����
	 * @param emUpdateType	[out,optional]	�ύX�^�C�v������i�f�t�H���g�FNULL�j
	 * @retval true �ύX����
	 * @retval false �ύX�Ȃ��i�L���b�V�����Ă��Ȃ��ꍇ�͏��false�j
	 */
	bool Changed( schema::ipc_table::update_type::ECode* emUpdateType = NULL ) const;

private:
#pragma warning(push)
#pragma warning(disable:4251)
	std::list<link_queue::CLQ>			m_listLQ;	//!< LQ�̃��X�g\n�V�[�P���X���ɕ���ł�����̂Ƃ���
#pragma warning(pop)
};

} // sindy

#endif // !defined(_LINKQUEUE_H_)
