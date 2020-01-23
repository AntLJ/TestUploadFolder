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
 * @brief <b>CRoadNetworkクラス定義ファイル</b>\n
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

// 2次メッシュ矩形で検索するかどうかのフラグ
// 
// 2次メッシュでネットワークを検索する場合は検索矩形内
// で関連フィーチャも完結するため、処理が簡略化されます。
// バッチ処理等で2次メッシュ単位で処理をする際にはこの
// フラグをONにすることで検索の際にかなり高速化されます。
// 
// 初期値は false です。
extern bool g_bNetworkSearchBy2ndMesh;

/**
 * @class CRoadNetwork
 * @brief <b>ネットワークに関する総合処理を行うクラス</b>\n
 * このクラスでは、ネットワークリンク、ネットワークノード、及び内包関係にあるものの関連付け、検索、検索結果の
 * 関連付けと管理を行います。
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

	//@{ @name CFeaturesContainer インターフェース
	/**
	 * @brief <b>形状で検索する</b>\n
	 * 道路リンク、道路ノード、リンク内属性、傾斜を検索します。検索結果は自動的に
	 * 関連付けられリスト内に保持されます。\n
	 * 内部でバッファリングなどはしないので、ポイントで検索する場合は
	 * バッファリングした形状を渡してください。\n
	 * @param ipGeom		[in]			検索形状
	 * @param bRecycling	[in,optional]	trueにすると、道路リンク、道路ノード、リンク内属性、傾斜のキャッシュを作成して切り離し
	 * @param bCreateCache	[in,optional]	キャッシュを作成する（bRecyclingがtrueの場合はこのフラグに関係なく必ず作成される）
	 * @param pErrs			[out,optional]	エラー格納コンテナ（オブジェクト同士の関連付けに失敗したもののみ入る）
	 * @param bGetRelated	[in,optional]	関連する道路を取得するかどうか
	 *
	 * @return sindyErrCode	エラーがある場合はcErrsのGetDefaultErrorCodeの結果が返る
	 */
	errorcode::sindyErrCode Select( IGeometry* ipGeom, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true );
	errorcode::sindyErrCode _Select( IGeometry* ipGeom, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true, bool bGetRelated = true );
	errorcode::sindyErrCode _Select( IQueryFilter* ipFilter, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* pErrs = NULL, bool bInit = true, bool bGetRelated = true );

	/**
	 * @brief <b>形状で検索する</b>\n
	 * 道路リンク、道路ノード、リンク内属性、傾斜を検索します。検索結果は自動的に
	 * 関連付けられリスト内に保持されます。\n
	 * 内部でバッファリングなどはしないので、ポイントで検索する場合は
	 * バッファリングした形状を渡してください。\n
	 * @param ipGeom		[in]			検索形状
	 * @param bRecycling	[in,optional]	trueにすると、道路リンク、道路ノード、リンク内属性、傾斜のキャッシュを作成して切り離し
	 * @param bCreateCache	[in,optional]	キャッシュを作成する（bRecyclingがtrueの場合はこのフラグに関係なく必ず作成される）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SelectByObject( _IRow* ipRow, bool bRecycling = false, bool bCreateCache = false, bool bInit = true );
	errorcode::sindyErrCode _SelectByObject( _IRow* ipRow, bool bRecycling = false, bool bCreateCache = false, bool bInit = true, bool bGetRelated = true );

	/**
	 * @brief <b>指定した道路OIDに関連するリンク列に関連する道路を検索する</b>\n
	 * リンク列に関連する道路のみ検索します。
	 * @param listRoadIDs	[in]			道路OID群
	 * @param bRecycling	[in,optional]	trueにすると、道路リンク、道路ノード、リンク内属性、傾斜のキャッシュを作成して切り離し
	 * @param bCreateCache	[in,optional]	キャッシュを作成する（bRecyclingがtrueの場合はこのフラグに関係なく必ず作成される）
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SelectByRoadFromLinkqs(const CListLong& listRoadIDs, bool bRecycling = false, bool bCreateCache = false, CErrorObjects* cErrs = NULL);

	/**
	 * @brief <b>指定した道路OIDに関連するリンク列に関連する道路を検索する</b>\n
	 * 2次メッシュ上ノードに接続している道路がある時は、反対側の道路も検索対象に追加する
	 * @param listRoadIDs		[in]		初期道路OID群
	 * @param listSearchRoads	[in/out]	検索道路OID群（2次メッシュ上ノードに接続する道路を含む）
	 * @return sindyErrCode
	 */
	void _SelectByRoadFromLinkqs(const CListLong& listRoadIDs, CListLong& listSearchRoads );

	/**
	 * @brief <b>与えられた形状との交差部分で分割処理を行う</b>\n
	 * @param pSource		[in]	分割対象
	 * @param cGeom			[in]	分割形状
	 * @param pSource		[in]	分割形状と一致するフィーチャを削除するかどうか
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects );
	errorcode::sindyErrCode _Split( CSPRow pSource, const CGeometry& cGeom, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>与えられたフィーチャのマージを行う</b>\n
	 * 指定フィーチャが格納されていること。
	 * @param pSource		[in]	マージ元
	 * @param pDest			[in]	マージ先
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Merge( CSPRow pSource, CSPRow pDest, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>与えられた形状の範囲にある点の削除を行う</b>\n
	 * @param pRow			[in]	削除対象
	 * @param cGeom			[in]	削除形状
	 * @param cErrObjects	[out]	エラーオブジェクト格納コンテナ
	 * @param bEraseOnly	[in/optional]	自分自身のみにするかどうか（pRowが内包関係の時にしか使わない）
	 *                                      削除ツールのセグメント削除で始点・終点どちらかが含まれている時に、
	 *                                      OIDを保持するために構成点構成点削除をするようにしている関係上
	 *                                      pRowで指定している以外の内包関係は処理しないようにするためだけに使用
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Erase( CSPRow pRow, const CGeometry& cGeom, CErrorObjects& cErrObjects, bool bEraseOnly=false );

	/**
	 * @brief <b>フィーチャの削除を行う</b>\n
	 * 指定フィーチャが格納されていること。
	 * @param pRow			[in]	削除対象
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Delete( CSPRow pRow, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>フィーチャの削除を行う</b>\n
	 * 指定フィーチャが格納されていること。
	 * @param pRow				[in]	削除対象
	 * @param listexclusions	[in]	除外オブジェクト
	 * @param bDelEdgeNode		[in]	2次メッシュ上ノードに接続している道路を削除するかどうか（通常は必ずtrueのこと、falseはErase()から呼ばれたときのみ）
	 * @param cErrObjects		[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode _Delete( CSPRow pRow, CContainer& listexclusions, bool bDelEdgeNode, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>中身を交換する</b>\n
	 * Split後にOIDを保持したいものを入れ替えたい時とかに使用すること。
	 * 作成/削除フラグは交換しないようにしてあります。
	 * @param pSource			[in]	ペースト対象
	 * @param pDest				[in]	コピー対象
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode swap( CSPRow pSource, CSPRow pDest );

	/**
	 * @brief <b>LQの削除を行う</b>\n
	 * 指定フィーチャが格納されていること。
	 * @param pRow				[in]	削除対象（道路）
	 * @param cErrObjects		[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode _DeleteLQ( CSPRow pRow, CErrorObjects& cErrObjects );

	/**
	 * @brief <b>リンク列の削除を行う</b>\n
	 * 指定フィーチャが格納されていること。
	 * @param cDelLinkq			[in]	削除対象
	 * @param cErrObjects		[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode DeleteLinkq( CLinkQueue& cDelLinkq, CErrorObjects& cErrObjects );

	/**
	 * 道路ののLQRFの再設定する
	 *
	 * @param emLinkqType		[in]	リンク列種別タイプ
	 * @param cErrObjects		[out]	エラーオブジェクト格納用コンテナ
	 *
	 * @retval true LQRFの再設定成功
	 * @retval false 失敗
	 */
	errorcode::sindyErrCode ResetRoadLQRF(sindyTableType::ECode emLinkqType, CErrorObjects& cErrObjects);

	/**
	 * @brief <b>指定したリンク列のLQの削除を行う</b>\n
	 * 指定フィーチャが格納されていること。
	 * @param pLinkq			[in]	削除対象（INF）
	 * @param cLQ				[in]	削除対象（LQ）
	 * @param cErrObjects		[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode DeleteLQ( CLinkQueue* pLinkq, const link_queue::CLQ& cLQ, CErrorObjects& cErrObjects );
	/**
	 * @brief <b>リンク列の実体化を行う</b>\n
	 */
	void MaterializationLinkq();

	/**
	 * ノードの種別を再設定する
	 *
	 * @param cRow				[in]			ノードフィーチャ
	 * @param bIsChanged		[out,optional]	変更されたかどうか
	 *
	 * @retval true ノード種別再設定成功
	 * @retval false 種別が決定できなかった
	 */
	bool ResetNodeAttribute( CSPRow cRow, bool* bIsChanged = NULL );

	/**
	 * 正しいと思われる道路ノード種別を返す
	 *
	 * @note cFeatureItems には、ipNodeFeature が接続するすべの道路リンクが含まれていることが前提です。
	 * @note この前提条件を満たしていない場合、返り値の道路ノード種別が正しく設定できない場合があります。
	 *
	 * @param cNode				[in]			道路ノード種別変更前のノード
	 * @param pbHasNodeAttr		[out]			道路ノードに接続する道路リンクが2本以下の場合にノードの交差点名称及び信号機
	 * @return sindyeNodeClass
	 */
	schema::road_node::node_class::ECode GetModifyRoadNodeKind( const CSPRow& cRow, bool* pbHasNodeAttr ) const;

	/**
	 * 正しいと思われる歩行者ノード種別を返す
	 *
	 * @note cFeatureItems には、ipNodeFeature が接続するすべの歩行者リンクが含まれていることが前提です。
	 * @note この前提条件を満たしていない場合、返り値の歩行者ノード種別が正しく設定できない場合があります。
	 *
	 * @param cRow			[in]			歩行者ノード種別変更前のノード
	 *
	 * @return sindyeWalkNodeClass
	 */
	schema::walk_node::node_class::ECode GetModifyWalkNodeKind( const CSPRow& cRow ) const;
	
	/**
	 * @brief 正しいと思われる地下鉄ノード種別を返す
	 * @param cRow [in] 地下鉄ノード種別変更前のノード
	 * @return スキーマライブラリに定義された地下鉄ノード種別
	 */
	schema::sj::subway_node::node_class::ECode GetModifySubwayNodeKind( const CSPRow& cRow ) const;

	/**
	 * @brief 正しいと思われる鉄道ノード種別を返す
	 * @param cRow [in] 鉄道ノード種別変更前のノード
	 * @return 鉄道ノード種別
	 */
	sindy::schema::sj::railway_node::node_class::ECode GetModifyRailwayNodeKind( const CSPRow& cRow ) const;

	/**
	 * @brief <b>編集内容を保存する</b>\n
	 * この関数では整合性のチェックを行いません。\n
	 * 必ずCheck関数を読んだ後にこの関数を呼んでください。\n
	 */
	errorcode::sindyErrCode Store( CErrorObjects& cErrObjects );

	/**
	 * @brief <b>整合性をチェックする</b>\n
	 * フィーチャを編集した後に必ずチェックを行ってください。
	 * @param cRule			[in]	論理チェック用ルール
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrObjects ) const;

	/**
	 * @brief <b>コンフリクト回避のための既編集チェックを行う</b>\n
	 * @param cErrObjects	[out]	エラーオブジェクト格納用コンテナ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckModify( CErrorObjects& cErrObjects ) const;

	/**
	 * @brief <b>キャッシュされているかどうかをチェックする</b>\n
	 * キャッシュされていないものはエラーログに出力されます。
	 * @retval true		全てキャッシュされている
	 * @retval false	一つでもキャッシュされていないものがある
	 */
	bool IsCached() const;

	/**
	 * @brief <b>登録されているフィーチャがあればそのポインタを返す</b>\n
	 * 今のところ属性ダイアログで使用中
	 * @return CRow*	登録されているフィーチャのポインタ
	 */
	const CRow* FindObject( _IRow* ipRow ) const;

	//@}

	//@{ @name リンク列タイプイテレータ
	inf_iterator inf_begin() const { return LQTypeIterator(); }
	inf_iterator inf_end() const { return LQTypeIterator(false,true); }
	//@}

	//@{ @name リンク列コンテナ操作関数
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
	 * @brief <b>メンバをクリアする</b>\n
	 */
	void clear();
private:
#pragma warning(push)
#pragma warning(disable:4251)
//	std::map<long,CRelRoad>										m_mapRelRoad;	//!< 道路リンク関連クラスリスト（キー：OBJECTID）
//	std::map<long,CRelNode>										m_mapRelNode;	//!< ノード関連クラスリスト（キー：OBJECTID）
	std::map<sindyTableType::ECode, std::list<CLinkQueue> >	m_mapLinkQueue;	//!< リンク列リスト（キー：OBJECTID）
//	TRowsContainer m_mapRowsContainer; //!< 保持するオブジェクトコンテナ（キー：sindyTableType::ECode,OBJECTID）
#pragma warning(pop)
	/**
	* @brief 2次メッシュ区画上ノード判定
	* @note 地下鉄路線対応の際にGetModifyWalkNodeKind()から分離
	*       AheGetModifySubwayNodeKind()と共通で使用する。
	* @param cRow　　　　　　　　[in] 　　ノード種別変更前のノード
	* @param listConnectRoads　[in]    ノードに接続するリンク
	* @param hasErr            [out]   処理が正常に終了したか否か 
	* @return                          2次メッシュ区画上ノードか否か
	*/
	bool OnMeshEdgeNode( const CSPRow& cRow, std::list<road_link::CRoadLink*>& listConnectRoads, bool& hasErr ) const;
	
	/**
	* @brief 道路ノード上判定
	* @param geom [in] 対象の形状（ポイントまたはマルチポイント）
	* @param onlyMeshBorder [in] trueなら2次メッシュ境界ノード限定
	* @return geomが2道路ノード上にあるか否か
	*/
	bool isOnNodePoint( const CGeometry& geom, bool onlyMeshBorder = false );

	/**
	* @brief 2次メッシュで接する内包関係の相手を取得する
	* @param row [in] 対象のフィーチャ
	* @param useOrgShape [in] rowの元形状（変更前）を使用する場合はtrue
	* @return 対象のフィーチャと２次メッシュ境界で接する同種の内包関係フィーチャ
	*         ２次メッシュに接していない場合はNULL
	*/
	CSPRow getContainOppositeMesh( const CSPRow& row, bool useOrgShape = false );
	
	/**
	* @brief 内包関係ものから構成点を削除する
	* @note 親となる道路リンクや、そこに含まれる他の内包関係との共有構成点も削除する
	* @param containRel　　[in,out] 　　対象の内包関係（リンク内属性、傾斜）
	* @param delVertex　   [in]        削除する構成点形状（ポイント、またはマルチポイント）
	* @param cErrObjects   [out]       エラー情報
	*/
	void deleteContainVertex( CSPRow& containRel, const CGeometry& delVertex, CErrorObjects& cErrObjects );
	
	/**
	* @brief 内包関係ものを削除する
	* @param containLine　　[in,out] 　　対象の内包関係（リンク内属性、傾斜）
	*/
	void deleteContainRelation( CSPRow& containLine );
	
	/**
	* @brief 内包関係ものに関連する共有構成点を削除する
	* @note 親となる道路リンクや、そこに含まれる他の内包関係との共有構成点を削除する
	* @param containRel　[in,out] 　　対象の内包関係（リンク内属性、傾斜）
	* @param vertexs 　　[in] 　　    削除する構成点の形状
	*/
	void deleteSharedVertex( CSPRow& containRel, const CGeometry& vertices );
	
	/**
	* @brief 内包関係ものの親となる道路リンクを取得する
	* @param containRel　　[in] 　　対象の内包関係（リンク内属性、傾斜）
	*/
	CSPRow getParentLink( const CSPRow& containRel );
};

} // sindy

#endif // _ROADNETWORK_H_
