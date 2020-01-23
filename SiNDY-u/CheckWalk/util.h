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

#pragma once

#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <TDC/sindylib_core/FieldMap.h>
#include "FieldLists.h"
namespace util
{

inline void GetDomainMap( const CString& fieldName, const sindy::CFieldMap& fieldMap, std::map<long, CString>& classMap )
{
	std::map<CString, long> domainMap = fieldMap.GetDomain( fieldName );
	for( auto& domain : domainMap )
		classMap[domain.second] = domain.first;
};

/**
	* @brief	現在時刻取得用
	* @retval	CString  「YYYY/MM/DD hh:mm:ss」 
	*/
inline CString GetCurTime(){
	SYSTEMTIME st;
	::GetLocalTime( &st );
	CString strTime;
	strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
	return strTime;
	};

/// 3点の内積を求める
inline double innerproduct( double x1, double y1, double x2, double y2, double x3, double y3 )
{
	return ( x1 - x2 ) * ( x3 - x2 ) + ( y1 - y2 ) * ( y3 - y2 );
};

/// 2点の長さを求める
inline double length( double x1, double y1, double x2, double y2 )
{
	double dx = x2 - x1;
	double dy = y2 - y1;

	return sqrt( dx*dx + dy*dy );
};

/**
 * @brief 3点からなる線分の角度を求める
 *
 * @param x1	[in]	DOUBLE
 * @param y1	[in]	DOUBLE
 * @param x2	[in]	DOUBLE
 * @param y2	[in]	DOUBLE
 * @param x3	[in]	DOUBLE
 * @param y3	[in]	DOUBLE
 *
 * @retval 角度（-180～180度）
 */
inline double angle( double x1, double y1, double x2, double y2, double x3, double y3 ){
	DOUBLE dInnerProduct = util::innerproduct( x1, y1, x2, y2, x3, y3 );
	DOUBLE d = util::length( x1, y1, x2, y2 ) * util::length( x2, y2, x3, y3 );

	return ( acos( dInnerProduct / d ) * (double)180.0 ) / (double)M_PI;
};


/**
 * @brief R-Treeを生成する
 * @param geoClassMap [in]	対象フィーチャクラス
 * @param rTree	[out] 結果のR-Tree(冒頭の[r]は参照の意味ではないです)
 */
void createRTree(
	const std::map<long, GeoClass>& geoMap,
	BGRTree& rTree
	);

/**
 * @brief ベースメッシュ形状を取得
 * @param meshs [in] 取得するベースメッシュのメッシュコードリスト
 * @param table [in] ベースメッシュを保持するテーブル
 * @return ベースメッシュコードとメッシュ形状の対応表
 */
std::map<CString, IGeometryPtr> getBaseMesh(const std::set<std::string>& meshCodes, ITablePtr table);

/**
 * @brief	Polylineを構成点に分解
 * @param	ipLineGeometry	[in]	PolylineのGeometry
 * @param	seqPoint	[out]	構成点のリスト
 * @retval	true	成功
 * @retval	false	失敗
 */
bool getLine2Points(
	const IGeometryPtr& ipLineGeometry,
	std::vector<IPointPtr>& seqPoint
	);

/**
 * @brief	R-Treeから、与えた箱の範囲に引っかかるID群を取得
 * @param	box	[in]	取得したい範囲(エンベロープ)
 * @param	rTree [in]	取得してくる対象のR-Tree
 * @retval	set<long>	対象範囲のID群
 */
std::set<long> getIntersectsIDs( const BGBox& box, const BGRTree& rTree );

/**
 * @brief フィーチャマップから特定の属性値をもつフィーチャのID群を取得
 * @param featureMap   [in] フィーチャマップ
 * @param fieldName    [in] 属性名
 * @param fieldValues  [in] 取得する属性値リスト
 * @param meshGeometry [in] メッシュ形状
 * @return フィーチャのID群
 */
std::set<long> getTargetIDs(
	const FeaturesPack& feature
	, LPCTSTR fieldName
	, const std::set<long> fileldValues
	, const IGeometryPtr& meshGeometry );

/**
 * @brief フィーチャマップからメッシュに含まれるフィーチャのID群を取得
 * @param featureMap   [in] フィーチャマップ
 * @param meshGeometry [in] メッシュ形状
 * return フィーチャのID群
 */
std::set<long> getTargetIDs(
	const std::map<long, GeoClass>& featureMap
	, const IGeometryPtr& meshGeometry );

/**
 * @brief フィーチャマップの内、引数に渡したIDをもつフィーチャの形状をUnion
 * @param featureMap [in] フィーチャマップ
 * @param ids        [in] UnionするID群
 * @return Unionされた結果
 */
IGeometryPtr getUnionGeometry(
	const std::map<long, GeoClass>& featureMap
	, const std::set<long>& ids );

/**
 * @brief フィーチャマップのフィーチャの形状をUnion
 * @param featureMap [in] フィーチャマップ
 * @return Unionされた結果
 */
IGeometryPtr getUnionGeometry(const std::map<long, const GeoClass*>& featureMap);

/**
 * @brief ポイント群をUnion
 * @param pointList [in] ポイント群
 * @return Unionされた結果
 */
IGeometryPtr getUnionGeometry(const std::vector<IPointPtr>& pointList);

/**
 * @brief  交点を取得する
 * @note   交点が存在しない場合は空を返却
 * @param  baseGeometry  [in]  リンク1
 * @param  testGeometry  [in]  リンク2
 * @return 交点の座標群
 */
std::vector<IPointPtr> getCrossPoint(IGeometryPtr baseGeometry , IGeometryPtr testGeometry);

/**
 * @brief ID群から指定形状に完全内包されるものを除く
 * @detail 指定形状の枠線上のフィーチャは完全内包されていないものと
 * 判断する
 * @wanirng featureIdsの要素をOBJECTIDにもつフィーチャが
 * featureMap内に存在しない場合、out_of_range例外が発行される
 * @param featureIds [in] ID群
 * @param geometry   [in] 指定形状
 * @param featureMap [in] フィーチャマップ(idsの形状参照用)
 * @return ID群から指定形状に完全内包されるものを除いたID群
 */
std::set<long> excludeContainLink(
	const std::set<long>& featureIds
	, const IGeometryPtr& geometry
	, const std::map<long, GeoClass>& featureMap);

/**
 * @brief 繋がっているリンク毎にOBJECTID群を分ける
 * @detail
 * ・戻り値は
 * <繋がっているリンクの内一番小さいOBJECTID,
 * キーのOBJECTIDから到達可能なリンクのOBJECTID群><br>
 * キーのOBJECTIDから到達可能なリンクのOBJECTID群には、キーのOBJECTIDも含まれる。<br>
 * 例) リンク 1,2,3が、リンク 4,5,6,7が繋がっているとき、
 * <1, {1,2,3}>,<4,{5,6,7}><br>
 * ・リンク同士が繋がっているかどうかは、FROM_NODE_ID/TO_NODE_IDの値が
 * 同地かどうかで判定している
 * @param linIds  [in] 繋がっているか判定するリンク群
 * @param linkMap [in] フィーチャマップ(linkIdsの形状参照用)
 * @return <リンクID, リンクIDから到達可能なリンクIDのset>
 */
std::map<long, std::set<long>> toEveryPartLink(
	const std::set<long>& linkIds
	, const FeaturesPack& link );

/**
 * @brief	周辺の歩行者ノードに同名ポイントを持つノードが存在するかチェック
 * @param	checkObj	[in]	チェック対象オブジェクト
 * @param	checkObjMap	[in]	チェック対象オブジェクトのMap
 * @param	walkNodeMap	[in]	チェック参照用歩行者ノードMap
 * @param	walkNodeRTree	[in]	チェック参照用歩行者ノードRTree
 * @param	nameField	[in]	名称フィールド
 * @param	yomiField	[in]	ヨミフィールド
 * @param	idField	[in]	IDフィールド
 * @param   refer_id_threshold [in] エラーとする処理の閾値
 * @param	checkWalkNodeClass	[in]	チェック対象のノード種別（デフォルト:-1）
 * @retval	true	存在する
 * @retval	false	存在しない
 */
bool existAroundWalkNodeSameName(
	const GeoClass& checkObj,
	const FeaturesPack& checkFeatures,
	const FeaturesPack& walkNode,
	const BGRTree& walkNodeRTree,
	const CString& nameField,
	const CString& yomiField,
	const CString& idField,
	double refer_id_threshold,
	const long checkWalkNodeClass = -1
	);

/**
 * @brief 最終更新日時・プログラム更新日時の内、新しい日付を返す
 * @detail 一方がNULLならNULLでない方を返す。
 * 両方がNULLならVT_NULLを返す
 * @param modifyDate        [in] 最終更新日時
 * @param programModifyDate [in] プログラム更新日時
 * @return DATE型のCComVariant
 * @retval NULL型のCComVariant modifyDate,programModifyDateともNULL
 */
CComVariant selectModifyTime(
	const CComVariant& modifyDate
	, const CComVariant& programModifyDate);

/**
 * @brief 指定日時以降に編集されているか否か
 * @param modifyFilter [in] 編集履歴フィルタ
 * @param modifyDate   [in] チェック対象の最終更新日時
 * @param programModifyDate [in] チェック対象のプログラム更新日時
 * @retval true  指定日時以降に編集されている
 * @retval false 指定日時以降には編集されていないか、
 * 最終更新日時・プログラム最終更新日時がともにNULLである
 */
bool checkModifyTime(
	const CComVariant& modifyFilter
	, const CComVariant& modifyDate
	, const CComVariant& programModifyDate);

/**
 * @brief ポイントがリンク上にあるかどうかを判定
 * @note
 * ポイントとリンクの最近傍距離がDBの座標精度より小さいなら、リンク上にあると判定する。
 * リリースデータ作成時に使用するSplitLinkWLNで、
 * リンク上のノードの判定する処理があり、これをチェッカー用に修正した。
 * ※SplitLinkWLNでは、ノードを座標精度分大きくしたBufferを作成し、
 *  それがリンクと接しているかどうかを判定している。
 * こちらの方法でも最近傍距離かどうかの判定はできるが、本関数の実装の方が早い)
 * これを整備側で修正しないと、上述のリリースツールが異常終了するので、チェッカーで検出する。
 * rangeには、DBの座標許容値がくることを想定している。
 * @param [in] link  リンク
 * @param [in] point ポイント
 * @param [in] range リンク上にポイントが存在すると判定する許容値
 * @retval true  ポイントがリンク上に存在する
 * @retval false ポイントがリンク上に存在しない
*/
bool isPointOnLink(IGeometryPtr link, IGeometryPtr point, double range);

/**
 * @brief チェック対象のリンクの端点が、overlapLink上に存在するか否かを判定
 * @detail 端点とリンクがrange以内であるならtrueとする
 * @param [in] checkTargetLink チェック対象のリンク
 * @param [in] overlapLink	   チェック対象のリンクを含むリンク
 * @param [in] range           リンク上にポイントが存在すると判定する許容値
 * @retval true  リンクの端点が両方ともoverlapLink上に存在する
 * @retval false どちらか一方のリンクの端点がoverlapLink上に存在しない
 */
bool isLinkEdgeOnLink(IGeometryPtr checkTargetLink, IGeometryPtr overlapLink, double range);

/**
 * @brief ラインの構成点の内、あるラインに重なっている部分に含まれる構成点群を取得
 * @detail 構成点の順番はcheckdLineの始点→終点方向で格納
 * @param [in] checkedLine 構成点群を取得するリンク
 * @param [in] overlapLine 構成点取得用ライン
 * @param [in] range       リンク上にポイントが存在すると判定する許容値
 * @return 重なっている部分の構成点
 */
std::vector<IPointPtr> getOverlapConfigurationPoints(IPolylinePtr checkedLine, IPolylinePtr overlapLine, double range);

/**
 * @brief	指定された2つのGeoClass(Point)間の距離を求める
 * @param	geo1	[in]	PointのGeoClass1
 * @param	geo2	[in]	PointのGeoClass2
 * @retval	距離(m)
 */
double GetDistGeoClasses(
	const GeoClass& geo1,
	const GeoClass& geo2
	);

/**
 * @brief	指定された2つのGeometry(Point)間の距離を求める
 * @param	geo1	[in]	PointのGeometry1
 * @param	geo2	[in]	PointのGeometry2
 * @retval	距離(m)
 */
double GetDistGeometries(
	const IGeometryPtr& geo1,
	const IGeometryPtr& geo2
	);

/**
 * @brief IDセットから対象のOIDを除いたID群のオブジェクトリストを取得する
 * @param oID    [in] 対象のOID
 * @param oIDs   [in] IDセット
 * @param geoMap [in] オブジェクトの格納先
 * @return オブジェクトリスト
 */
std::list<GeoClass> GetObjects( 
	long oID,
	const std::set<long>& oIDs,
	const std::map<long, GeoClass>& geoMap
	);

/**
 * @brief IDセットから対象のOIDを除いたID群のオブジェクトマップを取得する
 * @param oID    [in] 対象のOID
 * @param oIDs   [in] IDセット
 * @param geoMap [in] オブジェクトの格納先
 * @return オブジェクトマップ <OID, オブジェクトへのポインタ>
 */
std::map<long, const GeoClass*> GetObjectMap(
	long oID,
	const std::set<long>& oIDs,
	const std::map<long, GeoClass>& geoMap
	);


/**
 * @brief 指定のリンクに関して、指定のノードIDに紐付かない方の端点に紐付くノードIDを取得
 * @note 始点にOID:1,終点にOID:2のノードが紐付いている場合、
 * ノードIDとして"1"を引数に与えると、2を返す
 * @param linkId  [in] リンクID
 * @param linkMap [in] リンクの属性値を格納したマップ
 * @param nodeId  [in] ノードID
 * @retrun 指定のノードIDとは逆の端点に紐付くノードのノードID
 * @retval 0 引数に下記のいずれかのエラーがある<br>
 * ・linkIdがlinkMapの中に存在しない<br>
 * ・nodeIdがlinkIdで指定したリンクに紐付いていない<br>
 */
long getOtherEdgeNode( long linkId, const FeaturesPack& linkMap, long nodeId );

/**
 * @brief 指定ノードがメッシュ境界上ノードかどうか判定
 * @param nodeId  [in] ノードID
 * @param nodeMap [in] ノードの属性値を格納したマップ
 */
bool isMeshEdgeNode( long nodeId, const FeaturesPack& nodeMap, bool isWalk );

/**
 * @brief 二次メッシュ境界の向こう側のリンクに接続するリンク群を取得
 * @detail 同時に、ノードIDも2次メッシュ境界の向こう側のノードIDに更新
 * @param linkId  [in] リンクID
 * @param linkMap [in] リンクの属性値を格納したマップ
 * @param nodeId  [in/out] ノードID
 * @param nodeMap [in] ノードの属性値を格納したマップ
 * @param nodeLinkIDs [in] ノードIDに対応するリンクIDの対応表
 */
std::set<long> getMeshOverLink(
	long linkId, const FeaturesPack& link
	, long& nodeId, const FeaturesPack& node
	, const NODEIDMAP& nodeLinkIDs
	, bool isWalk );

/**
 * @brief nodeIdを端点としたときの隣接するリンクを取得
 * @detail nodeIdが図郭上ノードの場合、
 * 2次メッシュ境界の向こう側のリンクに接続しているリンク群を取得
 * 同時に、端点ノードIDも2次メッシュ境界の向こう側のノードIDに更新
 * @param nodeId       [in/out] 端点ノードID
 * @param adjacentLink [in] 端点ノードに接続するリンク群
 * (隣接を取得したいリンクのリンクIDが除かれていること)
 * @param linkMap      [in] リンクの属性値を格納したマップ
 * @param nodeMap      [in] ノードの属性値を格納したマップ
 * @param nodeLinkIDs  [in] ノードIDに対応するリンクIDの対応表
 */
std::set<long> getAdjacentLink(
	long& nodeId
	, const std::map<long, const GeoClass*>& adjacentLink
	, const FeaturesPack& link
	, const FeaturesPack& node
	, const NODEIDMAP& nodeLinkIDs
	, bool isWalk
	);

/**
 * @brief 指定ノードがリンクIDの始点に接続しているか否か
 * @param linkId  [in] 指定ノードとの接続を確認するリンクのリンクID
 * @param linkMap [in] リンクIDに対する属性・形状の対応表
 * @param nodeId  [in] 指定ノード
 * @retval true  始点である
 * @retval false 始点ではない
 */
bool isFromNode( long linkId, const FeaturesPack& linkMap, long nodeId );


/**
 * @brief	フィーチャーが指定されたメッシュコード群のどのメッシュに含まれているか（枠上含む）取得する。
 * @param	geoClass	[in]	対象のフィーチャ
 * @param	cityMeshMap	[in]	メッシュコードMap
 * @return	（該当するメッシュがあった場合）メッシュコード
 * @return	（該当するメッシュがなかった場合）0
 */
long GetExistMeshcode(
	const GeoClass& geoClass,
	const std::map<long, std::pair<IGeometryPtr,IGeometryPtr>>& cityMeshMap
	);

/**
 * @brief	指定ノードに紐付くリンクは指定したリンクのみであるか（歩行者用）
 * @note	紐付くリンクが指定リンクのみでも、指定ノードが端点ノードの場合、判定結果はfalseにする
 * @param	nodeMapIteId	[in] ノードの属性値を格納したマップのイテレータ
 * @param	linkId			[in] リンクID
 * @param	nodeLinkIDs		[in] ノードに紐付くリンク群の情報（map）
 * @param	nodeClassIndex	[in] ノード属性インデックス
 * @retval  true 紐付くリンクは指定リンクのみ
 * @retval	false 指定リンク以外のリンクも紐付くか、何も紐付かない
 */
bool isOnlyTargetWalkLink(
	const std::map<long, GeoClass>::iterator& nodeMapIt,
	long linkId,
	const NODEIDMAP& nodeLinkIDs,
	long nodeClassIndex );

/**
* @brief	指定ノードに紐付くリンクは指定したリンクのみであるか（道路用）
* @note	紐付くリンクが指定リンクのみでも、指定ノードが端点ノードの場合、判定結果はfalseにする
* @param	nodeMapIteId	[in] ノードの属性値を格納したマップのイテレータ
* @param	linkId			[in] リンクID
* @param	nodeLinkIDs		[in] ノードに紐付くリンク群の情報（map）
* @param	nodeClassIndex	[in] ノード属性インデックス
* @retval	true 紐付くリンクは指定リンクのみ
* @retval	false 指定リンク以外のリンクも紐付くか、何も紐付かない
*/
bool isOnlyTargetRoadLink(
	const std::map<long, GeoClass>::iterator& nodeMapIt,
	long linkId,
	const NODEIDMAP& nodeLinkIDs,
	long nodeClassIndex );

/**
* @brief	指定ジオメトリに関係するリンクフィーチャをノード情報と共に取得
* @note		featuresのm_ipTableとm_FieldMapは予め設定されている必要がある
* @param	ipGeometry	[in] 基準ジオメトリ
* @param	fieldList	[in] 取得するフィールドリスト
* @param	strWhere	[in] 属性的検索条件
* @param	features	[in/out] フィーチャ群クラス
* @param	nodeNodeIDs [out] ノードIDに対応するノードIDの対応表（from/to to/from）
* @param	nodeLinkIDs [out] ノードIDに対応するリンクIDの対応表
* @retval	true 成功
* @retval	false 失敗
*/
bool GetLinkFeaturesWithNodeIdInGeometry(
	const IGeometryPtr& ipGeometry,
	const FIELD_LIST& fieldList,
	const CString& strWhere,
	FeaturesPack& features,
	NODEIDMAP& nodeNodeIDs,
	NODEIDMAP& nodeLinkIDs );

/**
* @brief	指定ジオメトリに関係するリンクフィーチャをノード情報と共に取得
* @note		featuresのm_ipTableとm_FieldMapは予め設定されている必要がある
* @param	ipGeometry	[in] 基準ジオメトリ
* @param	fieldList	[in] 取得するフィールドリスト
* @param	strWhere	[in] 属性的検索条件
* @param	features	[in/out] フィーチャ群クラス
* @param	nodeLinkIDs [out] ノードIDに対応するリンクIDの対応表
* @retval	true 成功
* @retval	false 失敗
*/
bool GetLinkFeaturesInGeometry(
	const IGeometryPtr& ipGeometry,
	const FIELD_LIST& fieldList,
	const CString& strWhere,
	FeaturesPack& features,
	NODEIDMAP& nodeLinkIDs );

/**
* @brief	指定ジオメトリに関係するフィーチャを取得
* @note		featuresのm_ipTableとm_FieldMapは予め設定されている必要がある
* @param	ipGeometry	[in] 基準ジオメトリ
* @param	fieldList	[in] 取得するフィールドリスト
* @param	strWhere	[in] 属性的検索条件
* @param	features	[in/out] フィーチャ群クラス
* @retval	true 成功
* @retval	false 失敗
*/
bool GetFeaturesInGeometry(
	const IGeometryPtr& ipGeometry,
	const FIELD_LIST& fieldList,
	const CString& strWhere,
	FeaturesPack& features );

/**
* @brief 指定IDをキャッシュから検索し、なければテーブル全体から検索する
* @note 「searchEdgeNode」前後を汎用化したもの
* @param features [in] 検索するフィーチャ群
* @param id [in] 検索するオブジェクトのOBJECTID
* @return 検索されたノードの情報を格納したGeoClass
* @retval GeoClassのm_ObjectIDが-1 検索に失敗
*/
GeoClass SearchObj( const FeaturesPack& features, long id );


/**
* @brief 道路リンクは歩行者リンクより完全一致ないの件数を取得する
* @param [in] checkedLinkIds 道路リンク群
* @param [in] walkLinkIds 歩行者リンク群
* @param [in] roadLink 道路リンクのフィーチャ群クラス
* @param [in] walkLink 歩行者リンクのフィーチャ群クラス
* @return 道路リンクは歩行者リンクより完全一致ないの件数
*/
long getDifferenceRoadLinkAndWalkLinkCount( std::set<long>checkedLinkIds, std::vector<long>walkLinkIds, const FeaturesPack&roadLink, const FeaturesPack&walkLink);

/**
* @brief チェック対象に交差なるポリゴンを検索
* @param [in] geoObject         チェック対象の情報
* @param [in] pack              群クラス
* @param [in] rtree             RTree
* @param [in] crossObjects      交差なるポリゴンのリスト
* @param [in] range             広げる範囲(単位:m)
* @return  交差なるポリゴンの数
*/
long findOverlapGeometry(
	const GeoClass& geoObject,
	const FeaturesPack& pack,
	const BGRTree& rtree,
	std::vector<GeoClass>& crossObjects,
	double range = 0
	);
} // util
