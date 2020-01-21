/**
 * @file AheNetwork.h
 * 
 * @brief SiNDYデータモデルにおけるネットワークモデルに関するグローバル関数など
 * 定義するファイルです。\n
 * 現在ネットワークモデルと呼ばれるものは以下の通り：
 * @li 道路ネットワーク
 * @li 歩行者ネットワーク（2007年度より開発開始）
 * @li 地下駐車場ネットワーク（現在は道路ネットワークの一部）
 * 
 * @author コンテンツ本部地図DB制作部システム開発グループ 古川貴宏
 * @date 2007/04/20
 * @copy Copyright(c) 2007 INCREMENT P CORP., All Rights Reserved.
 */
#pragma once

class CTargetItem;
template <class T> class CTargetItemsT;
typedef CTargetItemsT<CTargetItem> CTargetItems;

/// ネットワークモデルタイプ
enum sindyeNetworkModel
{
	sindyeNotNetwork,        //!< NWではない
	sindyeNetworkRoad,       //!< 道路NW
	sindyeNetworkWalk,       //!< 歩行者NW
	sindyeNetworkSubway,     //!< 地下鉄路線NW
	sindyeNetworkRailway,    //!< 鉄道NW
	sindyeNetworkLane, 		 //!< 車線NW (ADAM)
	sindyeNetworkUpDown, 	 //!< 上下線NW (ADAM)
	sindyeNetworkCompart, 	 //!< 区画線NW (ADAM)
	sindyeNetworkBorder 	 //!< 道路境界線NW (ADAM)
};

/// ネットワークモデルのラインフィーチャクラスかどうかを判定する
bool AheIsNetworkLink( LPCTSTR lpcszTableName );
bool AheIsNetworkLink( ILayer* ipLayer );
bool AheIsNetworkLink( IFeatureClass* ipFeatureClass );
bool AheIsNetworkLink( IFeature* ipFeature );

/// ADAMネットワークモデルのラインフィーチャクラスかどうかを判定する
bool AheIsAdamNetworkLink(LPCTSTR lpcszTableName);
bool AheIsAdamNetworkLink(ILayer* ipLayer);
bool AheIsAdamNetworkLink(IFeatureClass* ipFeatureClass);
bool AheIsAdamNetworkLink(IFeature* ipFeature);

/// ネットワークモデルのポイントフィーチャクラスかどうかを判定する
bool AheIsNetworkNode( LPCTSTR lpcszTableName );
bool AheIsNetworkNode( ILayer* ipLayer );
bool AheIsNetworkNode( IFeatureClass* ipFeatureClass );
bool AheIsNetworkNode( IFeature* ipFeature );

/// ADAMネットワークモデルのポイントフィーチャクラスかどうかを判定する
bool AheIsAdamNetworkNode(LPCTSTR lpcszTableName);
bool AheIsAdamNetworkNode(ILayer* ipLayer);
bool AheIsAdamNetworkNode(IFeatureClass* ipFeatureClass);
bool AheIsAdamNetworkNode(IFeature* ipFeature);

/// ネットワークモデルかどうか判定する
bool AheIsNetwork( LPCTSTR lpcszTableName );
bool AheIsNetwork( ILayer* ipLayer );
bool AheIsNetwork( IFeatureClass* ipFeatureClass );
bool AheIsNetwork( IFeature* ipFeature );

/// ADAMネットワークモデルかどうか判定する
bool AheIsAdamNetwork(LPCTSTR lpcszTableName);
bool AheIsAdamNetwork(ILayer* ipLayer);
bool AheIsAdamNetwork(IFeatureClass* ipFeatureClass);
bool AheIsAdamNetwork(IFeature* ipFeature);

/// ネットワークモデルタイプ判定
sindyeNetworkModel AheGetNetWorkType( LPCTSTR lpcszTableName );
sindyeNetworkModel AheGetNetWorkType( ILayer* ipLayer );
sindyeNetworkModel AheGetNetWorkType( IFeatureClass* ipFeatureClass );
sindyeNetworkModel AheGetNetWorkType( IFeature* ipFeature );

/**
 * @brief ネットワークモデルに関連するかどうか判定する
 * @note ネットワークモデル自身も含まれます
 */
bool AheIsRelateWithNetwork( LPCTSTR lpcszTableName );
bool AheIsRelateWithNetwork( ILayer* ipLayer );
bool AheIsRelateWithNetwork( IFeatureClass* ipFeatureClass );
bool AheIsRelateWithNetwork( IFeature* ipFeature );

/**
 * @brief ネットワークモデルでの関連テーブル名一覧を取得する
 * @note AheGetNetworkSplitTarget + AheGetNetworkNodeFeatureClassName
 * @param lpcszTableName [in] ネットワークモデルに関連するテーブル名（何でも良い）
 * @return テーブル名一覧（縦棒区切り）
 */
CString AheGetNetworkTarget( LPCTSTR lpcszTableName );

/**
 * @brief ネットワークモデルでのスプリット処理に必要なライン系テーブル名一覧を取得する
 * @note CEditHelper::SplitRoadByRoadAndMeshでどうしても必要だったので作成
 * @param lpcszTableName [in] ネットワークモデルに関連するテーブル名（何でも良い）
 * @param bContainNW     [in] 一覧にネットワークリンクそのものを含むか
 * trueなら ネットワークリンクそのものを含んだ一覧を返す(例：|ROAD_LINK|SEGMENT_ATTR|GRAD|)
 * falseなら ネットワークリンクを含まない一覧を返す(例：|SEGMENT_ATTR|GRAD|) [デフォルト:true]
 * @return テーブル名一覧（縦棒区切り）
 */
CString AheGetNetworkSplitTarget( LPCTSTR lpcszTableName, bool bContainNW = true );

/**
 * @brief ネットワークリンクフィーチャクラス名の一覧を取得する
 * @note CTargetFeatures::GetNetworkLinkFeaturesでどうしても必要だったので作成
 */
CString AheGetNetworkLinks();

/**
 * @brief ネットワークノードフィーチャクラス名の一覧を取得する
 * @note CTargetFeatures::GetNetworkNodeFeaturesでどうしても必要だったので作成
 */
CString AheGetNetworkNodes();

/// ネットワークリンクフィーチャクラス名を返す
CString AheGetNetworkLinkFeatureClassName( LPCTSTR lpcszTableName );

/// ネットワークノードフィーチャクラス名を返す
CString AheGetNetworkNodeFeatureClassName( LPCTSTR lpcszTableName );

/**
 * @brief ネットワークリンクフィーチャクラスを返す
 * 渡されたフィーチャに関連するネットワークモデルのリンクフィーチャクラスを返します。
 */
IFeatureClassPtr AheGetNetworkLinkFeatureClass( IFeature* ipFeature );

/**
 * @brief ネットワークノードフィーチャクラスを返す
 * 渡されたフィーチャに関連するネットワークモデルのノードフィーチャクラスを返します。
 */
IFeatureClassPtr AheGetNetworkNodeFeatureClass( IFeature* ipFeature );

/**
 * @brief 渡されたテーブル名に付随する関連フラグフィールド名のリストを取得する
 * @param lpcszTableName [in] 関連フラグが付いているテーブル名（e.x. ROAD_LINK）
 * @return 関連フラグフィールド名のリスト（e.x. ROAD_LINK なら ONWAY_RF と NOPASSAGE_RF）
 */
std::list<CString> AheGetRelationFlagFieldName( LPCTSTR lpcszTableName );

/**
 * @biref 関連フラグフィールド名と対応する外部テーブルの外部キーフィールド名を取得する
 * @param lpcszTableName    [in] 関連フラグが付いているテーブル名（e.x. ROAD_LINK）
 * @param lpcszRelFieldname [in] 関連フラグフィールド名（e.x. ONWAY_RF）
 * @return 外部テーブルの外部キーフィールド名（e.x. LINK_ID）
 */
CString AheGetRelationOuterKeyFieldName( LPCTSTR lpcszTableName, LPCTSTR lpcszRelFieldName );

/** 関連フラグフィールド名と対応する外部テーブル名を取得する
 * @param lpcszTableName    [in] 関連フラグが付いているテーブル名（e.x. ROAD_LINK）
 * @param lpcszRelFieldname [in] 関連フラグフィールド名（e.x. ONWAY_RF）
 * @return 外部テーブル名（e.x. TIME_ONEWAY）
 */
CString AheGetRelationTableName( LPCTSTR lpcszTableName, LPCTSTR lpcszRelFieldName );

/**
 * @brief フィーチャに関連する外部テーブルをオープンする
 * @param ipFeature         [in] ベースフィーチャ
 * @param lpcszRelFieldName [in] 関連フィールド名
 * @return 関連テーブル
 */
ITablePtr AheOpenRelationTable( IFeature* ipFeature, LPCTSTR lpcszRelFieldName );

/**
 * @brief ネットワークノードに接続されているネットワークリンクを取得
 *
 * AheGetConnectedRoad()と処理は同じ
 */
LONG AheGetConnectedNetworkLink( IFeaturePtr ipFeature, CTargetItems& cTargetItems );

/**
 * @brief ネットワークリンクが接続しているノードフィーチャを取得
 *
 * AheGetConnectedNode()と処理は同じ
 */
LONG AheGetConnectedNetworkNode( IFeaturePtr ipFeature, CTargetItems& cTargetItems );

/**
 * @brief 選択したネットワークリンクの反対側のリンクを返し、反対側のノードが2次メッシュ区画辺上ノードかどうか
 *
 * AheGetConnectedRoadAndNode()と処理は同じ
 */
bool AheGetConnectedNetworkLinkAndNode( IFeaturePtr ipSelectRoad, IFeaturePtr ipSelectNode, IFeaturePtr &ipConnectedNetworkLink, IFeaturePtr &ipOppNode );

/**
 * @brief 2次メッシュを跨いでいるネットワークリンクを取得する
 *
 * AheIsRoadCrossMesh()と処理は同じで、ネットワークリンクで判断できる
 */
bool AheIsNetworkLinkCrossMesh( IMapPtr ipMap, IFeaturePtr ipFeature, CTargetItems &cNetworkLinkCrossMesh );

/**
 * @brief Containルール関係にあるもののうち関連させるべきものを取得する
 *
 * 道路リンクとリンク内属性のような関係の場合に、未確定なリンク内属性形状から
 * 関連する道路リンクを取得する場合に使用します。
 * その際、関連IDも未確定なものと判断し、使用しません。
 * 
 * @param ipWithinFeature [in] 「含まれる」側のフィーチャ
 * @param cContainFeature [in] 「含む」側のフィーチャ群
 * @return 関連させるべきと判断された「含む」側フィーチャ群
 */
CTargetItems AheGetRelationshipContainFeature( IFeaturePtr ipWithinFeature, const CTargetItems& cContainFeatures );

/**
 * @brief 歩行者NWの汎用モデルかダウンコンバートモデルかを判定する関数
 *
 * @note この関数は歩行者NWまたは歩行者ノードの子ダイアログでのみ利用すること
 *       ダウンコンバート版は2年で完了する見込みのため、その後1年ほど保持した後は
 *       データも削除することを推奨し、本関数も削除するのが望ましい。
 *
 * @param ipFeature [in] 歩行者NWまたは歩行者ノード
 *
 * @return kDownConvert ダウンコンバート版
 * @return kGeneral		汎用版
 */
sindy::schema::walk_type::type_class::ECode AheIsDownconvert( IFeaturePtr ipFeature );