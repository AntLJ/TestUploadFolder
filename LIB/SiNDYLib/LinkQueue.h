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

// LinkQueue.h: CLinkQueue クラスのインターフェイス
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

	//@{ @name LQコンテナ操作関数
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
	 * @brief <b>LQをシーケンス順にソートする</b>\n
	 * シーケンス番号のチェックは行いませんので、チェックする場合は
	 * CheckLQSequenceを呼ぶ必要があります。
	 */
	void sort() { m_listLQ.sort(); }

	/**
	 * @brief <b>メンバをクリアする</b>\n
	 */
	void clear();

	/**
	 * @brief <b>LQ数</b>\n
	 */
	long size() const { return m_listLQ.size(); }

	/**
	 * @brief <b>LQをセットする</b>\n
	 * LQをセットし終わったら必ずsortを呼んで
	 * LQをシーケンス順に並べ替える必要があります。\n\n
	 * LQを追加する際、INFのIDと整合をチェックします。
	 * @param emType	[in]	追加するLQのタイプ
	 * @param lp		[in]	追加するLQ
	 * @retval sindyErr_NoErr	追加成功
	 * @retval 上記以外			追加失敗
	 */
	errorcode::sindyErrCode push_back( sindyTableType::ECode emType, _IRow* lp, CSPFieldMap& cFields, CSPTableNameString& cNames );

	/**
	 * @brief <b>指定した道路のOIDを持つLQがあるか</b>\n
	 * @param cRow	[in]	道路
	 */
	bool HasRoad(const CRow& cRow) const;

	/**
	 * @brief <b>リンク列を削除する</b>\n
	 * @param cErrObjects		[in]	エラーオブジェクト
	 */
	errorcode::sindyErrCode Delete( CErrorObjects& cErrObjects );

	/**
	 * @brief <b>LQを削除する</b>\n
	 * @param cRow				[in]	削除するLQに関連する道路
	 * @param cNewLinkq			[in/out]	削除した結果、分割された場合にLQが格納される
	 * @param cErrObjects		[in/out]	エラーオブジェクト
	 */
	errorcode::sindyErrCode DeleteLQ( const CRow& cRow, CLinkQueue& cNewLinkq, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>中身を交換する</b>\n
	 * @param cLinkq			[in]	交換するリンク列
	 * @param bIsSwapLQ			[in]	LQ群も交換するかどうか
	 * @param bIsCopyCache		[in]	キャッシュをコピーするかどうか
	 * @param bIsCopyRow		[in]	オブジェクトをコピーするかどうか
	 * @param bIsCopyFlag		[in]	作成削除フラグをコピーするかどうか
	 * @param bIsCopyContainer	[in]	コンテナをコピーするかどうか
	 */
	void swap( CLinkQueue& cLinkq, bool bIsSwapLQ, bool bIsCopyCache=true, bool bIsCopyRow=true, bool bIsCopyFlag=true, bool bIsCopyContainer=true );

	/**
	 * @brief <b>リンク列をマージする</b>\n
	 * @param cSource			[in]	マージ元道路
	 * @param cDest				[in]	マージ先道路
	 * @param bIsReverse		[in]	マージ先と先の向きが違うかどうか
	 * @param emTableType		[in]	道路のテーブルタイプ
	 * @param cRoadnetwork		[in]	道路群
	 * @param cErrObjects		[in]	エラーオブジェクト
	 */
	errorcode::sindyErrCode Merge( const CRow& cSource, const CRow& cDest, bool bIsReverse, sindyTableType::ECode emTableType, const CContainer& cRoads, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>リンク列の形状を取得</b>\n
	 * @param bUseEditUpdate	[in]	編集状態を考慮するか
	 * @param emTableType		[in]	道路のテーブルタイプ
	 * @param cRoadnetwork		[in]	道路群
	 */
	IGeometryPtr CreateLQGeometry(bool bUseEditUpdate, sindyTableType::ECode emTableType, const CContainer& cRoadnetwork);

	/**
	 * @brief <b>リンク列を分割する</b>\n
	 * @param cOrgRoadRow		[in]	オリジナルの道路
	 * @param emTableType		[in]	道路のテーブルタイプ
	 * @param cNewRoads			[in]	オリジナルの道路から分割によって作成された道路群
	 * @param cErrObjects		[in]	エラーオブジェクトLQ
	 */
	errorcode::sindyErrCode Split( const CRow& cOrgRoadRow, sindyTableType::ECode emTableType, const CContainer& cNewRoads, CErrorObjects& cErrObjects );

	//@}
	//@{ @name チェック関数
	/**
	 * @brief <b>リンク列をチェックする</b>\n
	 * このチェック関数では以下の項目をチェックします。
	 * @li INF単体チェック
	 * @li LQ単体チェック
	 * @li INFとLQでIDの整合が取れているかどうか
	 * @li LQのシーケンス番号及びその並びが適切かどうか
	 * 道路リンクとの整合性まではチェックしません。
	 * @retval sindyErr_NoErr	エラー無し
	 * @retval 上記以外			エラー
	 */
	errorcode::sindyErrCode Check() const;

	/**
	 * @brief <b>LQのシーケンスをチェックする</b>\n
	 * この関数を呼ぶ前にsortする必要があります。
	 * @retval sindyErr_NoErr	エラー無し
	 * @retval 上記以外			エラー
	 */
	errorcode::sindyErrCode CheckLQSequence() const;
	//@}

	/**
	 * @brief <b>編集内容を保存する</b>\n
	 * この関数では整合性のチェックを行いません。\n
	 * 必ずCheck関数を読んだ後にこの関数を呼んでください。
	 */
	errorcode::sindyErrCode Store( CErrorObjects& cErrObjects );

	/**
	 * @brief <b>既に編集されていないかどうか確認する</b>\nj
	 * 変更があったものに対してのみチェックを行います。\n
	 * キャッシュを作成していないと変更のチェックに引っかからないので注意してください。\n
	 * @param cErrObjects	[in,out]	エラーオブジェクトが入る又は、中身をチェックする
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckModify( CErrorObjects& cErrObjects ) const;

	/**
	 * @brief <b>整合性をチェックする</b>\n
	 * フィーチャを編集した後に必ずチェックを行ってください。
	 * @param cRule			[in]	論理チェック用ルール
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const;

	/**
	 * @brief <b>属性に変更があったかどうかをチェックする</b>\n
	 * キャッシュしている場合に限り有効です。キャッシュしていなければ、常に
	 * falseが返ります。
	 * 仮想関数なのは、リンク列でCLQに変更があった時にCInfoを更新しなければいけないから
	 * @param emUpdateType	[out,optional]	変更タイプが入る（デフォルト：NULL）
	 * @retval true 変更あり
	 * @retval false 変更なし（キャッシュしていない場合は常にfalse）
	 */
	bool Changed( schema::ipc_table::update_type::ECode* emUpdateType = NULL ) const;

private:
#pragma warning(push)
#pragma warning(disable:4251)
	std::list<link_queue::CLQ>			m_listLQ;	//!< LQのリスト\nシーケンス順に並んでいるものとする
#pragma warning(pop)
};

} // sindy

#endif // !defined(_LINKQUEUE_H_)
