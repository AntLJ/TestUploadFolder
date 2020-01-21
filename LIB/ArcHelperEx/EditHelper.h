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

// EditHelper.h: CEditHelper クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITHELPER_H__6456CB1F_0C72_4A48_95B7_12F5D5AEE546__INCLUDED_)
#define AFX_EDITHELPER_H__6456CB1F_0C72_4A48_95B7_12F5D5AEE546__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _USE_SETLQATTRIBUTE

#include "MapHelper.h"
#include "ArcHelper.h"

//class CTargetItems;
class CErrObjContainer;

struct LQDEF {
	_IRowPtr obj;
	long oid;
	long linkid;
	long seq;
	bool last;
	long dir;	// 1:正方向 2:逆方向
	sindyeRoadQueueType kind;
	long infid;
};

struct ROADDEF {
	long oid;
	long rf;
	IGeometryPtr shape;
};

class CEditHelper : public CMapHelper, public CArcHelper
{
public:
	struct LQRFINFO
	{
		long oid;
		std::map<sindyeRoadQueueType,bool> LQRF;
	};

public:
	CEditHelper();
	virtual ~CEditHelper();

	struct LongPair { LONG lID1; LONG lID2; };

	CEditHelper( CArcHelper& cHelper ){ *this = cHelper; m_piMap = cHelper.GetMap(); };

	/**
	* @brief 代入演算子
	*
	* @note コピー元のCArcHelper から IMap オブジェクトを取得し、自身を初期化する。
	*
	* @param cArcHelper [in] コピー元のクラスオブジェクト
	*
	* @return 自身への参照
	*/
	CEditHelper& operator=( CArcHelper& cHelper ){ ((CArcHelper&)*this) = cHelper; m_piMap = cHelper.GetMap(); return *this; };

	/**
	* @brief 管理フィーチャをメッシュ境界と道路でスプリットする
	*
	* @warning 関数から戻った後、 編集を破棄する・しないに関わらずcTargetFeatures 及び cNewFeatures は必ず Store() する必要があります。
	*
	* @param cTartgetItems		[in,out]		スプリット対象を含む更新フィーチャ群
	* @param cNewFeatures		[out]			新規に作成されたフィーチャ群
	* @param bSplitAll			[in]			自動交差する場合は TRUE
	* @param pReferenceNodes	[out,optional]	取得したノード群
	* @param ipSplitPoints		[in,optional]   分割点群
	* 
	* @note ipSplitPointsを指定した場合、メッシュ境界を除いてそれ以外の場所で分割されなくなります。
	* 当然、bSplitAllのフラグも無意味なものになります（bug 4882）。
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL SplitRoadByRoadAndMesh( CTargetItems& cTargetFeatures, CTargetItems& cNewFeatures, BOOL bSplitAll, CErrObjContainer& errs, IPointPtr ipTakeOverPoint = NULL, CTargetItems* pReferenceNodes = NULL, IPointCollection* ipSplitPoints = NULL );
	BOOL SplitRoadByRoadAndMesh( CTargetItems& cTargetFeatures, CTargetItems& cNewFeatures, BOOL bSplitAll, IPointPtr ipTakeOverPoint = NULL, CTargetItems* pReferenceNodes = NULL, IPointCollection* ipSplitPoints = NULL );

	/**
	* @brief 管理フィーチャをジオメトリでスプリットする
	* 
	* @warning 関数から戻った後、編集を破棄する・しないに関わらず cTargetFeatures 及び cNewFeatures は必ず Store() する必要があります。
	*
	* @param cTartgetItems				[in,out]		スプリット対象を含む更新フィーチャ群
	* @param lpcszSplitLayers			[in]			スプリット対象レイヤ（「|」セパレート）
	* @param ipGeom						[in]			スプリットするためのジオメトリ
	* @param cNoSetModifyDataFeatures	[in,out]		編集履歴をつけたくないフィーチャ群
	* @param cNewFeatures				[out]			新規作成されたフィーチャ群
	* @param ipTakeOverPoint			[in,optional]	スプリット後に元のOIDを引き継ぐフィーチャを決めるためのポイント（このポイントに近いフィーチャが引き継ぐ）
	* @param iSplitMode					[in,optional]	スプリットモード（0=Cross 1=Touch 2=Both, デフォルト：2）
	* @param pReferenceNodes			[out,optional]	取得したノード群
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/ 
	BOOL SplitFeaturesByGeometry( CTargetItems& cTargetFeatures, LPCTSTR lpcszSplitLayers, IGeometryPtr ipGeom, CTargetItems& cNoSetModifyDataFeatures, CTargetItems& cNewFeatures, IPointPtr ipTakeOverPoint = NULL, INT iSplitMode = 2, CTargetItems* pReferenceNodes = NULL );

	/**
	* @brief ジオメトリをジオメトリでスプリットする
	*
	* @param ipBaseGeometry				[in]			スプリットするジオメトリ
	* @param ipSplitterGeometry			[in]			スプリットするためのジオメトリ
	* @param iSplitMode					[in,optional]	スプリットモード（0=Cross 1=Touch 2=Both, デフォルト：2）
	* @param bIsRoadFeature				[in,optional]	道路フィーチャかどうか（デフォルト：FALSE）
	* @param ipSplittedPoints			[in,optional]	bIsRoadFeature が TRUE の時にはこれにスプリットポイントが入る（道路用）
	* @param bTest						[in,optional]	テスト用。単純に切れるかどうか確認するだけのときはメッセージボックスを抑制するためにTRUEを設定すること（デフォルト:FALSE）
	*
	* @retval _ISetPtr	スプリット後の形状（IGeometryPtr）
	*/
	_ISetPtr SplitGeometryByGeometry( IGeometryPtr ipBaseGeometry, IGeometryPtr ipSplitterGeometry, INT iSplitMode = 2, BOOL bIsRoadFeature = FALSE, IGeometryPtr ipSplittedPoints = NULL, BOOL bTest = FALSE );

	/**
	 * @brief ポリラインをポイント群でスプリットする
	 *
	 * @param ipPolyline				[in]			スプリットするポリライン
	 * @param ipSplitterPoints			[in]			スプリットするためのポイント群
	 * @param ipSplittedPoints			[in,optional]	実際にスプリットされたポイントが格納される
	 *
	 * @retval _ISetPtr	スプリット後の形状（IGeometryPtr）
	 */
	_ISetPtr SplitPolylineByPoints( IPolylinePtr ipPolyline, IGeometryPtr ipSplitterPoints, IPointCollectionPtr ipSplittedPoints = NULL );

	/**
	 * @brief ポリゴンをポリラインでスプリットする
	 *
	 * @param ipPolygon					[in]			スプリットするポリゴン
	 * @param ipSplitterPolyline		[in]			スプリットするためのポリライン
	 * @param bTest						[in,optional]	テスト用。単純に切れるかどうか確認するだけのときはメッセージボックスを抑制するためにTRUEを設定すること（デフォルト:FALSE）
	 *
	 * @retval _ISetPtr
	 */
	_ISetPtr SplitPolygonByPolyline( IPolygonPtr ipPolygon, IPolylinePtr ipSplitterPolyline, BOOL bTest = FALSE );

#ifdef _USE_SETLQATTRIBUTE
	/**
	 * @brief	分割された道路に対応するように LQ の情報を修正する
	 *
	 * @param lFeatureID		[in]	元の道路リンク ID
	 * @param ipBeforeSplitGeom	[in]	元の道路リンクの移動後、スプリット前の形状
	 * @param cNewRoads			[in]	分割後の道路リンクフィーチャ
	 *
	 * @return 正常終了したら TRUE
	 */
	BOOL SetLQAttribute( LONG lFeatureID, IGeometryPtr ipBeforeSplitGeom, CTargetItems& cNewRoads, std::list<LQRFINFO>& listLQRFInfo, std::list<ROWDEF>& listLQRows );
#else
	/**
	 * @breif <b>分割された道路リンクのための新しいLQを作成する</b>\n
	 *　一つのリンク列に対して一度に複数回分割が行われる場合を考慮して、
	 * シーケンス、最終フラグ、方向コードは一切整合を取りません。
	 * 整合合わせは呼び出し元で行う必要があります。
	 * 整合合わせの方法に関しては、SplitFeaturesByGeometryの最後の方を参考に
	 * してください。
	 * cRoad					[in]	IDを引き継いだ道路リンク
	 * cNewRoads				[in]	新しく作成された道路リンク
	 * listLQRows				[out]	新しく作成されたLQのリスト
	 * @retval TRUE 正常終了
	 * @retval FALSE 失敗
	 */
	BOOL CEditHelper::CreateLQForSplittedRoad( CTargetItem& cRoad, CTargetItems& cNewRoads, std::list<ROWDEF>& listLQRows );
#endif // ifdef _USE_SETLQATTRIBUTE

	/**
	 * @brief <b>シーケンスの整合性を修復する</b>\n
	 * SetLQAttributeがバグバグなので、この関数でINF毎のシーケンスを
	 * 修復します。
	 * @param listLQ			[in]	修復対象LQのリスト
	 * @param mapRoad			[in]	関連する道路リンクのリスト（キー：OBJECTID）
	 * @retval TRUE 修復成功
	 * @retval FALSE 修復失敗
	 */
	static BOOL FixLQSequence( std::list<LQDEF>& listLQ, std::map<long, ROADDEF>& mapRoad );

	/**
	 * @brief 分割されたNWフィーチャの関連テーブルを削除
	 * @note  CRuleRelationTableで関連フラグフィールドが空でないものについては、関連フラグが立っていた場合のみ削除する
	 *
	 * @param ipFeature				[in]	削除元のNWフィーチャ
	 *
	 * @retval TRUE  関連テーブルの削除に成功
	 * @retval FALSE 関連テーブルの削除に失敗
	 */
	BOOL DeleteRelationTables( IFeaturePtr ipFeature ) const;

	/**
	 * @brief 分割されたNWフィーチャの関連テーブルの内容をコピー
	 * @note  CRuleRelationTableで関連フラグフィールドが空でないものについては、関連フラグが立っていた場合のみコピーする
	 *
	 * @param ipFeature				[in]	コピー元のNWリンク
	 * @param cSplittedRoadFeatures	[in]	分割されたNWリンク
	 *
	 * @retval TRUE  関連テーブルのコピーに成功
	 * @retval FALSE 関連テーブルのコピーに失敗
	 */
	BOOL CopyRelationAttribute( IFeaturePtr ipFeature, CTargetItems& cSplittedRoadFeatures );

	/**
	 * @brief 交差あるいは接触しているポイントを取得する
	 *
	 * @param cDstItems			[in]			比較先フィーチャ群
	 * @param cSrcItems			[in]			比較元フィーチャ群
	 * @param iMode				[in,optional]	判定モード（0=Cross 1=Touch 2=Both, デフォルト：2）
	 * @param pNoRelItems		[out,optional]	比較先フィーチャ群の中で交点・接点が見つからなかったものが入る
	 *
	 * @return IPointCollectionPtr
	 */
	IPointCollectionPtr GetCrossOrTouchPoints( CTargetItems& cDstItems, CTargetItems& cSrcItems, INT iMode = 2, CTargetItems* pNoRelItems = NULL );

	/**
	* @brief 切り抜きや XOR を行ったことで、マルチポリゴン化したオブジェクトを分割する
	*
	* @param ILayerPtr			[in]	対象のレイヤ
	* @param CTargetItems&		[in]	元のマルチポリゴン
	* @param CTargetItems&		[in]	分割後のポリゴンコレクション
	*
	* @retval TRUE		処理は成功
	* @retval FALSE		処理中にエラーが発生
	**/
	BOOL SplitMultiPolygonToSimplePolygons(ILayerPtr pILayer, CTargetItems& cSource, CTargetItems& cDest);
	
	/**
	* @brief Ring が IPolygonPtr2 の内部にある場合は、Interior として追加する
	*
	* @param IPolygon2Ptr				[in/out]	チェック対象のポリゴン
	* @param IRingPtr					[in/out]	チェックする Ring
	* @param IGeometryCollectionPtr		[out]		追加した Ring のジオメトリ
	**/
	void AddInterior(IPolygon2Ptr, IRingPtr, IGeometryCollectionPtr);

	/**
	* @brief ポリゴンの重なりをチェックする（重なっていた場合は切り抜く）
	*
	* @param cTargetItems [in]	チェックする対象
	* @param cModifyItems [out]	切り抜かれたオブジェクト
	* @param bAskOverlap  [in]	重なりの回避を尋ねるかどうか
	*
	* @retval TRUE	処理は成功
	* @retval FALSE エラーが発生
	**/
	BOOL CutOutPolygonByPolygon( CTargetItems& cTargetItems, CTargetItems& cModifyItems, bool bAskOverlap=true );

	/**
	* @brief クリックされた点を含む内接ポリゴンを作成する
	*
	* @param ILayerPtr		[in]	作成対象のレイヤ
	* @param IPointPtr		[in]	クリックされた点
	* @param IGeometry**	[out]	作成されたジオメトリ
	*
	* @note	参照カウントを変更する必要なし
	*
	* @retval > 0		内接ポリゴンあり（作成）
	* @retval = 0		内接ポリゴンなし
	* @retval < 0		エラー発生
	*/
	int CreateInteriorPolygon(ILayer*, IPoint*, IGeometry**);

	// GetCrossOrTouchPointsと同等だが無駄な処理を省いた高速版
	IPointCollectionPtr GetCrossOrTouchPoints2( CTargetItems& cDstItems, CTargetItems& cSrcItems, INT iMode = 2, CTargetItems* pNoRelItems = NULL );

	// GetCrossOrTouchPointsと同等だが無駄な処理を省いた高速版
	IGeometryPtr GetCrossOrTouchPoints2( IGeometryPtr ipBaseGeometry, IGeometryPtr ipTestGeometry, INT iMode = 2 );

	// AhePointIsOnGeometryと同等だが処理は高速なはず
	BOOL PointIsOnGeometry( IGeometryPtr ipBaseGeometry, IPointPtr ipTestPoint, ISpatialReferencePtr ipSpRef = NULL, BOOL bExeptVertex = FALSE );

private:
	/**
	 * @brief 対象フィーチャ群をスプリットした結果を得る
	 * @note  構造のチェックなどは行っていない
	 * @param lpcszSplitLayers         [in]			 スプリット対象レイヤ（「|」セパレート）
	 * @param ipGeom                   [in]           スプリットするためのジオメトリ
	 * @param cNoSetModifyDataFeatures [in]           編集履歴をつけたくないフィーチャ群
	 * @param cModifyNodeKindFeatures  [in,out]       ノード種別を再設定したもので、且つ cNewFeatures にも cTargetFeatures にも存在しないもの
	 * @param ipTakeOverPoint          [in,optional]  スプリット後に元のOIDを引き継ぐフィーチャを決めるためのポイント（このポイントに近いフィーチャが引き継ぐ）
	 * @param iSplitMode               [in,optional]  スプリットモード（0=Cross 1=Touch 2=Both, デフォルト：2）
	 * @param cTakeOveredFeatures      [in,out]       分割された結果元のIDを引き継いだフィーチャ群
	 * @param pReferenceNodes          [out,optional] 取得したノード群
	 * @param cNewFeatures             [in,out]       新規作成されたフィーチャ群
	 * @param cTargetFeatures          [in,out]       スプリット対象を含む更新フィーチャ群
	 * @pram  listLQRows               [out]          スプリットの際に検索したLQレコード
	 * @retval TRUE  処理に成功
	 * @retval FALSE 処理に失敗
	 */
	BOOL GetSplitResult(
		LPCTSTR lpcszSplitLayers,
		const IGeometryPtr& ipGeom,
		CTargetItems& cNoSetModifyDataFeatures,
		CTargetItems& cModifyNodeKindFeatures,
		const IPointPtr& ipTakeOverPoint,
		INT iSplitMode,
		CTargetItems& cTakeOveredFeatures,
		CTargetItems* pReferenceNodes,
		CTargetItems& cNewFeatures,
		CTargetItems& cTargetFeatures,
		std::list<ROWDEF>& listLQRows);

	/**
	* @brief スプリット対象レイヤに所属するフィーチャ群をそれぞれスプリット形状で分割する
	* @note  構成点の追加位置を算出
	* @note  関連テーブルのコピーを作成
	* @param lpcszSplitLayers         [in]			 スプリット対象レイヤ（「|」セパレート）
	* @param strOthersName            [in]           NWモデル以外のフィーチャを表すモデル名
	* @param ipGeom                   [in]           スプリット形状
	* @param cNoSetModifyDataFeatures [in]           編集履歴をつけたくないフィーチャ群
	* @param ipTakeOverPoint          [in,optional]  スプリット後に元のOIDを引き継ぐフィーチャを決めるためのポイント（このポイントに近いフィーチャが引き継ぐ）
	* @param iSplitMode               [in,optional]  スプリットモード（0=Cross 1=Touch 2=Both, デフォルト：2）
	* @param listLQRFInfo             [out]          付与するLQRF群
	* @pram  listLQRows               [out]          スプリットの際に検索したLQレコード
	* @param cTakeOveredFeatures      [in,out]       分割された結果元のIDを引き継いだフィーチャ群
	* @param cNewFeatures             [in,out]       新規作成されたフィーチャ群
	* @param cTargetFeatures          [in,out]       スプリット対象を含む更新フィーチャ群
	* @param createNodePoints         [in]           NWモデルごとの、構成点（ノード）を作るべきポイントの集合
	* @retval TRUE  処理に成功
	* @retval FALSE 処理に失敗
	*/
	BOOL SplitFeatures(
		LPCTSTR lpcszSplitLayers,
		LPCTSTR strOthersName,
		const IGeometryPtr& ipGeom,
		const CTargetItems& cNoSetModifyDataFeatures,
		const IPointPtr& ipTakeOverPoint,
		INT iSplitMode,
		std::list<LQRFINFO>& listLQRFInfo,
		std::list<ROWDEF>& listLQRows,
		CTargetItems& cTakeOveredFeatures,
		CTargetItems& cNewFeatures,
		const CTargetItems& cTargetFeatures,
		std::map<const CString, IPointCollectionPtr>& createNodePoints);

	/**
	* @brief ネットワークリンクであるフィーチャのみを対象としてスプリット処理
	* @note  具体的にはRuleModelにてNWモデルとして定義されているフィーチャのみを対象とする
	* @note  ノードを生成する処理やリンク列の分割処理を行う
	* @param ipGeom                   [in]           スプリットするためのジオメトリ
	* @param cNewFeatures             [in,out]       新規作成されたフィーチャ群
	* @param cTargetFeatures          [in,out]       スプリット対象を含む更新フィーチャ群
	* @param cNoSetModifyDataFeatures [in,out]       編集履歴をつけたくないフィーチャ群
	* @param cModifyNodeKindFeatures  [in,out]       ノード種別を再設定したもので、且つ cNewFeatures にも cTargetFeatures にも存在しないもの
	* @param listLQRFInfo             [in]           付与するLQRF群
	* @param createNodePoints         [in]           NWモデルごとの、構成点（ノード）を作るべきポイントの集合
	* @param cTakeOveredFeatures      [in]           分割された結果元のIDを引き継いだフィーチャ群
	* @param pReferenceNodes          [out,optional] 取得したノード群
	* @retval TRUE  処理に成功したか、cTargetFeaturesがNWモデルでない
	* @retval FALSE 処理に失敗
	*/
	BOOL SplitNetworkLink(
		const IGeometryPtr& ipGeom,
		CTargetItems& cNewFeatures,
		CTargetItems& cTargetFeatures,
		CTargetItems& cNoSetModifyDataFeatures,
		CTargetItems& cModifyNodeKindFeatures,
		const std::list<LQRFINFO>& listLQRFInfo,
		const std::map<const CString, IPointCollectionPtr>& createNodePoints,
		CTargetItems& cTakeOveredFeatures,
		CTargetItems* pReferenceNodes) const;

	/**
	* @brief ネットワークリンクに内包されるフィーチャ群の構成点や関連IDに適切なIDを設定
	* @note  道路リンクに対するリンク内属性及び傾斜など。
	* @param cNewFeatures            [in,out] 新規作成されたフィーチャ群
	* @param cTargetFeatures         [in,out] スプリット対象を含む更新フィーチャ群
	* @param cTakeOveredFeatures     [in]     分割された結果元のIDを引き継いだフィーチャ群
	* @param cModifyNodeKindFeatures [in]     ノード種別を再設定したもので、且つ cNewFeatures にも cTargetFeatures にも存在しないもの
	* @retval TRUE  関連IDフィールドの設定に成功
	* @retval FALSE 関連IDフィールドの設定に失敗
	*/
	BOOL ResetCommonFeatures(
		CTargetItems& cNewFeatures,
		CTargetItems& cTargetFeatures,
		const CTargetItems& cNoSetModifyDataFeatures,
		const CTargetItems& cTakeOveredFeatures);
	/**
	* @brief 全てのリンク内属性及び傾斜の EndPoint を接するネットワークリンクに追加する
	* @param cNewFeatures            [in]     新規作成されたフィーチャ群
	* @param cSegAttrAndGradFeatures [in,out] 新規作成・更新されたフィーチャからリンク内属性・傾斜だけ抜き出したもの
	* @param cAllRoadFeatures        [in]     新規作成・更新されたフィーチャからネットワークリンクだけ抜き出したもの
	* @param ipRoadSpRef             [in]     分割対象となったネットワークリンクフィーチャクラスの空間参照
	* @retval TRUE  EndPointの追加に成功
	* @retval FALSE EndPointの追加に失敗
	*/
	BOOL AddEndPointToNetworkLink(
		const CTargetItems& cNewFeatures,
		CTargetItems& cSegAttrAndGradFeatures,
		const CTargetItems& cAllRoadFeatures,
		const ISpatialReferencePtr& ipRoadSpRef) const;

	/**
	* @brief 全てのリンク内属性及び傾斜の構成点再構築及び関連リンクIDの再設定
	* @param cNewFeatures             [in,out] 新規作成されたフィーチャ群
	* @param cTargetFeatures          [in,out] スプリット対象を含む更新フィーチャ群
	* @param cNoSetModifyDataFeatures [in]     編集履歴をつけたくないフィーチャ群
	* @param cTakeOveredFeatures      [in]     分割された結果元のIDを引き継いだフィーチャ群
	* @param cSegAttrAndGradFeatures  [in,out] 新規作成・更新されたフィーチャからリンク内属性・傾斜だけ抜き出したもの
	* @param cAllRoadFeatures         [in]     新規作成・更新されたフィーチャからネットワークリンクだけ抜き出したもの
	* @param ipRoadClass              [in]     分割対象となったネットワーク路リンクフィーチャクラス
	* @param ipRoadSpRef              [in]     分割対象となったネットワークリンクフィーチャクラスの空間参照
	* @retval TRUE  関連ネットワークリンクIDの再設定に成功
	* @retval FALSE 関連ネットワークリンクIDの再設定に失敗
	*/
	BOOL ReconstructCommonVertexAndResetRelField(
		CTargetItems& cNewFeatures,
		CTargetItems& cTargetFeatures,
		const CTargetItems& cNoSetModifyDataFeatures,
		const CTargetItems& cTakeOveredFeatures,
		CTargetItems& cSegAttrAndGradFeatures,
		const CTargetItems& cAllRoadFeatures,
		const IFeatureClassPtr& ipRoadClass,
		const ISpatialReferencePtr& ipRoadSpRef);

	/**
	* @brief 分割後のNWフィーチャとその関連フィーチャの対応がSiNDYのデータとして見たときに問題ないか確認
	* @note  cNewFeaturesは変更されないが、GetFeature関数はconstで呼び出せないため、constにしていない
	* @param cNewFeatures    [in]     新規作成されたフィーチャ群
	* @param cTargetFeatures [in,out] スプリット対象を含む更新フィーチャ群
	* @pram  listLQRows      [in]     スプリットの際に検索したLQレコード
	* @retval TRUE  構造に問題はなかった
	* @retval FALSE 構造に問題がある
	*/
	BOOL CheckNWStructure(CTargetItems& cNewFeatures, CTargetItems& cTargetFeatures,  const std::list<ROWDEF>& listLQRows) const;

	/**
	* @brief NWのノードとリンクがくっついているか確認
	* @note CheckNWStructure内でのみ使用される
	* @param cLastLinks [in] スプリット対象を含む更新されるNWフィーチャ群
	* @param cLastNodes [in] スプリット対象を含む更新されるNWノードフィーチャ群
	* @retval TRUE  リンク内属性・傾斜の分割も正常に行われた
	* @retval FALSE リンク内属性・傾斜の分割が正常に行われなかった
	*/
	BOOL CheckNWConnect(const CTargetItems& cLastLinks, const CTargetItems& cLastNodes) const;

	/**
	* @brief リンク内属性・傾斜の分割が正常に行われたか確認
	* @note CheckNWStructure内でのみ使用される
	* @param cLastRoads   [in] スプリット対象を含む更新される道路リンクフィーチャ群
	* @param cLastSegGrad [in] スプリット対象を含む更新されるリンク内属性・傾斜フィーチャ群
	* @retval TRUE  リンク内属性・傾斜の分割も正常に行われた
	* @retval FALSE リンク内属性・傾斜の分割が正常に行われなかった
	*/
	BOOL CheckCommonFeaturesConnect(const CTargetItems& cLastRoads, const CTargetItems& cLastSegGrad) const;

	/**
	* @brief リンク列の分割が正常に行われたか確認
	* @note CheckNWStructure内でのみ使用される
	* @note bug 3521対応時にフィーチャの属性変更も行うようになっているので、cLastRoadsはoutとなっている
	* @param cNewFeatures [in]     新規作成された道路リンクフィーチャ群
	* @param cLastRoads   [in,out] スプリット対象を含む更新される道路リンクフィーチャ群
	* @pram  listLQRows   [in]     スプリットの際に検索したLQレコード
	* @retval TRUE  リンク列の分割も正常に行われた
	* @retval FALSE リンク列の分割が正常に行われなかった
	*/
	BOOL CheckLQConnect(const CTargetItems& cNewFeatures, CTargetItems& cLastRoads, const std::list<ROWDEF>& listLQRows) const;

private:
/*	void SetDomain();
	void ResetDomain();*/
private:
	CLoadQueueInfo		m_cLoadQueueInfo;

//	DOUBLE m_dXMin, m_dXMax, m_dYMin, m_dYMax;
};

#endif // !defined(AFX_EDITHELPER_H__6456CB1F_0C72_4A48_95B7_12F5D5AEE546__INCLUDED_)
