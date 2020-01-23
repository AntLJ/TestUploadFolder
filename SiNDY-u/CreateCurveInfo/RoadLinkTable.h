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
 * @note
 * RoadLinkTable.h/cppはLQEditToolからのコピーを一部修正したもの。
 * これらはsindylib_baseの一部としていずれライブラリ化される予定だが、
 * その前にLQEditToolで作られたものを再利用したかったので、暫定措置としてコピーを使用。
 * TODO: 後にライブラリ化されたらそちらに置き換える。
 */
#pragma once

#include "TableContainer.h"

namespace sindy {

/**
 * @brief ライブラリを初期化する
 *
 * RowClassFactory、TableClassFactoryを置き換えます
 */
void init_sindynlib();

/**
 * @brief ライブラリの後始末をする
 *
 * グローバルにおいてあるSiRiUSモジュールの後始末をします
 * アプリケーションやDLLが終了する前に必ず呼ぶ必要があります。
 */
void uninit_sindynlib();

/**
 * @brief リンク列タイプ
 */
namespace linkqueue_type {
	enum ECode {
		kNone         =    0, //!< なし
		kLRTurn       =    1, //!< 右左折禁止
		kGuide        =    2, //!< 強制誘導
		kDirguide     =    4, //!< 方面案内
		kVICS         =    8, //!< VICS
		kLane         =   16, //!< レーン情報
		kByway        =   32, //!< 抜け道
		kRoute        =   64, //!< アクセス／乗り継ぎ道
		kUTurn        =  128, //!< Uターン可能箇所
		kInternavi    =  256, //!< インターナビVICS
		kIPCVICS      =  512, //!< IPC VICS
		kIntersection = 1024, //!< 進入方向別交差点
		kAll          = 2047, //!< 全て
	};
} // linkqueue_type

#define CAST_ROW boost::static_pointer_cast<CRowContainer>

/**
 * @brief 道路ネットワーク用のモデルを作成する
 *
 * modelに道路ネットワーク用のモデル（関連）を定義します。
 * @note 今のところ、定義されているのは道路とリンク列のみです。
 *
 * @param model [out] CModel
 */
void GetSindynModel( CModel& model );

/** 
 * @biref linkqueue_type から LQテーブル名のリストに変換する
 */
std::list<LPCTSTR> linkqueuetype2lqtablename( linkqueue_type::ECode emCode );

/**
 * @brief LQテーブル名からINFテーブル名に変換する
 */
LPCTSTR lq2inf( LPCTSTR lpcszLQ );

/**
 * @brief INFテーブル名からLQテーブル名に変換する
 */
LPCTSTR inf2lq( LPCTSTR lpcszInf );

/**
 * @brief 道路リンク管理クラス
 */
class CRoadLinkTable : public CTableContainer
{
public:
	explicit CRoadLinkTable() : CTableContainer() {}
	virtual ~CRoadLinkTable(){}
	CRoadLinkTable( const CRoadLinkTable& obj ) : CTableContainer( obj ){}
	CRoadLinkTable( ITable* lp, const CTableFinder& cFinder ) : CTableContainer( lp, cFinder ){}
	CRoadLinkTable( IFeatureClass* lp, const CTableFinder& cFinder ) : CTableContainer( lp, cFinder ){}
	CRoadLinkTable( sindyTableType::ECode emTableType, ITable *lp, const CTableFinder& cFinder ) : CTableContainer( emTableType, lp, cFinder ){}
	CRoadLinkTable( sindyTableType::ECode emTableType, IFeatureClass *lp, const CTableFinder& cFinder ) : CTableContainer( emTableType, lp, cFinder ){}
	CRoadLinkTable& operator =( ITable* lp ){ return static_cast<CRoadLinkTable&>(CTableContainer::operator=(lp)); }
	CRoadLinkTable& operator =( IFeatureClass* lp ){ return static_cast<CRoadLinkTable&>(CTableContainer::operator=(lp)); }
	CRoadLinkTable& operator =( const CTableBase& obj ){ return static_cast<CRoadLinkTable&>(CTableContainer::operator=(obj)); }
	CRoadLinkTable& operator =( const CRoadLinkTable& obj ){
		CTableContainer::operator=(obj);
		return *this;
	}
	operator ITable*() const { return (ITablePtr)p; }

	/**
	 * @brief sort用プレディケート（LQのシーケンス順にソート）
	 */
	class LessLQSequence
	{
	public:
		bool operator()( CSPContainerBase& c1, CSPContainerBase& c2 ) const;
	};

	/**
	 * @brief 接続されているかどうか
	 */
	bool IsConnected() const { return (ITable*)*this; }

	/**
	 * @brief 道路リンクのOBJECTIDで関連オブジェクトを検索する
	 *
	 * @warning 今のところ、関連オブジェクトとして引いてくるのはリンク列のみです。
	 * as_start,start_node_idは今のところ未実装です。セットしても何の効果もありません。
	 *
	 * @param listOID [in] 道路リンクのID
	 * @param detach [in,optional] Rowオブジェクトのインスタンスを切り離す場合はtrue（デフォルト：false）
	 * @param unique [in,optional] 追加検索の場合はtrue（デフォルト：false）
	 * @param emType [in,optional] 検索対象リンク列（デフォルト：linkqueue_type::kAll）
	 * @param as_start [in,optional] oidを流入リンクIDとするものに限定する（デフォルト：false）
	 * @param start_node_id [in,optional] as_start = true の場合に流入の始点となるノードID
	 */
	void Select( const std::list<long>&  listOID, bool detach = false, bool unique = false, linkqueue_type::ECode emType = linkqueue_type::kAll, bool as_start = false, long start_node_id = 0 );

	/**
	 * @brief 足りていないLQの検索
	 * @note
	 * 道路リンクの子供としてLQが、さらにその子供としてINFが登録されている前提。
	 * ROAD_LINK
	 *         |- LQ_*   <- 道路に関連するLQのみ
	 *                |- INF_*
	 * 
	 * ここまでで、道路に関連するLQと、それに関連するINFまでは検索済みなので、
	 * INFに関連するLQを全て取得する。
	 * ROAD_LINK ...
	 * INF_*
	 *     |- LQ_*
	 *     |- LQ_*  INFに関連するLQを全て取得する
	 *        ...
	 * @param detach [in,optional] Rowオブジェクトのインスタンスを切り離す場合はtrue（デフォルト：false）
	 * @param unique [in,optional] 追加検索の場合はtrue（デフォルト：false）
	 * @param emType [in,optional] 検索対象リンク列（デフォルト：linkqueue_type::kAll）
	 */
	void SelectLQs( bool detach/* = false*/, bool unique/* = false*/, linkqueue_type::ECode emType/* = linkqueue_type::kAll*/ );

	/**
	 * @brief LQ、INFの形状を補完する
	 *
	 * 既に検索済みのLQ、INFに対応する形状を道路リンクから取得します。
	 * 取得した形状は、それぞれのCRowContainerに対してGetShape()することで取得できます。
	 *
	 * @warning 注意点として、ROAD_LINKの子供として登録されているLQと、INFの子供として登録されている
	 * LQとは別物（IRowレベルでは一緒だが、CRowContainerレベルでは別物）であり、形状はINFの子供側に
	 * しか付与しません。
	 *
	 * @param emType [in] 検索対象リンク列（デフォルト：linkqueue_type::kAll）
	 */
	void SelectLQShape( linkqueue_type::ECode emType = linkqueue_type::kAll );
};

typedef boost::shared_ptr<CRoadLinkTable> CSPRoadLinkTable;

} // sindy
