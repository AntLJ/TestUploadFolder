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
* @file TargetFeature.h
*
* @brief フィーチャクラスの定義ファイル
*/
#if	!defined( __TARGETFEATURE_H__)
#define	__TARGETFEATURE_H__	//!< 重複インクルード防止

#include "define.h"
#include "AheGlobals.h"
#include "AheLayerFunctions.h"
#include "AheGeometryOp.h"
#include "TargetFeature.h"
#include <algorithm>

class CErrObjContainer;

#if	defined( _UNICODE)
/**
* @var FIELDMAP
*
* @note wstring はUNICODE対応版
*
* @brief データセットフィールド名とインデックス対比コンポーネント
*/
typedef std::map< std::wstring, LONG>	FIELDMAP;
#else
/**
* @var FIELDMAP
*
* @note string はASCII対応版
*
* @brief データセットフィールド名とインデックス対比コンポーネント
*/
typedef std::map< std::string, LONG>	FIELDMAP;
#endif

/**
* @class CTargetItem
*
* @brief 操作対象管理クラス
*
* @note 選択したフィーチャ等をリスト管理する際の、管理要素
* 
*/
class CTargetItem
{
public:
	/**
	* @brief コンストラクタ
	*/
	CTargetItem( void);

	/**
	* @brief コンストラクタ
	*
	* @param ipFeature [in] 管理対象フィーチャ
	*/
///	CTargetItem( IFeaturePtr ipFeature, LONG lNewObjectID = -1 );
	CTargetItem( IFeaturePtr ipFeature );

	/**
	* @brief デストラクタ
	*/
	~CTargetItem( void);

	/**
	* @brief 管理データ設定
	*
	* @note インストラクタで初期化したものを置き換える。
	*
	* @param ipFeature		[in]			管理対象フィーチャ
///	* @param lNewObjectID	[in,optional]	CreateFeatureBuffer で新規作成されたフィーチャ用
	*/
///	BOOL Attach( IFeaturePtr ipFeature, LONG lNewObjectID = -1 );
	BOOL Attach( IFeaturePtr ipFeature );

	/**
	* @brief コピーコンストラクタ
	*
	* @param cTargetItem [in] コピー元インスタンス
	*
	* @return 自分自身のインスタンス
	*/
	CTargetItem& operator=( const CTargetItem& cTargetItem );

	/**
	* @brief 等号比較演算子
	*
	* @note 比較は、フィーチャのOIDとレイヤのテーブル名で比較される
	*
	* @param cTargetItem [in] 比較対象インスタンス
	*
	* @retval TURE 等しい
	* @retval FALSE 等しくない
	*/
	bool operator==( const CTargetItem& cTargetItem ) const;

	/**
	* @brief 不等号比較演算子
	*
	* @note operator==演算の結果を反転して返している
	* @note 本当は、std::rel_ops::operator!=を利用してもらいたい気もする。
	*
	* @param cTargetItem [in] 比較対象インスタンス
	*
	* @retval TURE 等しくない
	* @retval FALSE 等い
	*/
	bool operator!=( const CTargetItem& cTargetItem ) const;

	/**
	* @brief 大小比較
	*
	* @note OIDを大小比較する。OIDが同一の場合には、レイヤ名で比較。
	* @note 本来であれば、レイヤ名称を優先させるべきであるが、重複性が高いことと文字列であることから比較コストが高いためOIDを利用している。
	* @note このメソッドはlist::sort用に準備した。
	* @note ちなみに、>,>=,<=は、それぞれstd::rel_ops::operator>/std::rel_ops::operator>=/std::rel_ops::operator<=を利用して対応のこと。
	*
	* @retval TRUE 小さい
	* @retval FALSE 大きい
	*/
	bool operator<( const CTargetItem& cTargetItem ) const;

	/**
	* @brief 管理フィーチャの取得
	*
	* @return IFeaturePtr
	*/
	IFeaturePtr GetFeature() const;

	/**
	* @brief 管理フィーチャのジオメトリ取得
	*
	* @note この関数は、IFeature::get_Shapeで実装されている。
	* @note そのため、形状を変更する場合には十分に注意する必要がある。
	* @note 形状変更なく、速度重視にはこちらを推薦。
	*
	* @return IGeometryPtr
	*/
	IGeometryPtr GetShape() const;

	/**
	* @brief 管理フィーチャのジオメトリ取得
	*
	* @note この関数は、IFeature::get_ShapeCopyで実装されていることに注意
	* @note 形状が変更される様な場合には、こちらを利用のこと。
	*
	* @return IGeometryPtr
	*/
	IGeometryPtr GetShapeCopy() const;

	/**
	* @brief 形状の設定
	*
	* @note 形状をフィーチャに設定
	*
	* @param piGeometry [in] 設定する形状
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL SetShape( IGeometryPtr piGeometry );

	/**
	 * @brief OBJECTID を持っているかどうか
	 *
	 * @retval TRUE 持っている
	 * @retval FALSE 持っていない
	 */
	BOOL HasOID() const;

	/**
	 * @brief OBJECTID の取得
	 *
	 * @return OBJECTID（存在しなければ -1）
	 */
	LONG GetOID() const;

	/**
	 * @brief IWorkspacePtr の取得
	 *
	 * @return IWorkspacePtr（取得しなければ NULL）
	 */
	IWorkspacePtr GetWorkspace() const;

	/**
	 * @brief フィーチャジオメトリの FromPoint を取得する
	 *
	 * @note フィーチャのジオメトリがポリラインの場合に限る
	 *
	 * @return IPoint （ポリラインではない場合は NULL）
	 */
	IPointPtr GetFromPoint();

	/**
	 * @brief フィーチャジオメトリの ToPoint を取得する
	 *
	 * @note フィーチャのジオメトリがポリラインの場合に限る
	 *
	 * @return IPoint （ポリラインではない場合は NULL）
	 */
	IPointPtr GetToPoint();

	/**
	* @brief 管理フィーチャのジオメトリタイプを取得
	*
	* @return ジオメトリタイプ
	*/
	esriGeometryType GetGeometryType() const;

	/**
	 * @brief フィーチャのフィーチャクラス名称取得
	 *
	 * @note AheGetFeatureClassName() をそのまま使用
	 *
	 * @return CString
	 */
	CString GetFeatureClassName() const;

	/**
	 * @brief フィーチャのオーナ名取得
	 *
	 * @note AheGetFeatureClassOwnerName() をそのまま使用
	 *
	 * @return CString
	 */
	CString GetFeatureClassOwnerName() const;
	
	/**
	 * @brief フィーチャのオーナ名.クラス名取得
	 *
	 * @return CString
	 */
	CString GetOwnerDotClassName() const;

	/**
	 * @brief バージョン名取得
	 *
	 * @return CString
	 */
	CString GetVersionName() const;

	/**
	 * @brief フィーチャのフィーチャクラス取得
	 *
	 * @note AheGetFeatureClass() をそのまま使用
	 *
	 * @return IFeatureClassPtr
	 */
	IFeatureClassPtr GetFeatureClass() const;

	/**
	* @brief 更新情報の設定
	*
	* @note operator/purpose/modifydateに情報を設定する。
	* @note operatorとpurposeは、SiNDYのプロパティ設定ファイルから取得。
	* @note modifydateはパラメータのデータを利用。パラメータがNULLの場合、現時点でのシステムのローカルタイム取得。
	*
	* @param emUpdateType [in] 更新タイプ情報
	* @param lpModifyTime [in] 更新情報に設定する更新時刻
	*
	* @see AheSetModifyData
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL SetModifyData( sindyUpdateType emUpdateType, LPSYSTEMTIME lpModifyTime = NULL);

	/**
	* @brief 管理対象フィーチャをバッファサイズを指定してポリゴン化
	*
	* @param dbBufferSize [in] バッファ化するサイズ(map unit値
	* @param ppiPolygon [in] バッファ化した形状を受け取る
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL CreatePolygon( double dbBufferSize, IPolygon** ppiPolygon);

	/**
	* @brief 管理対象フィーチャを指定した点でヒットテスト
	*
	* @param piTestPoint [in] テストを実施するポイント
	* @param dbTolerance [in] ヒットテスト判定の閾値
	* @param piPoint [out] ヒットしたポイント
	* @param pdbHitDist [out] 指定したポイントから実際にヒットしたポイントまでの距離
	* @param plPartIndex [out] ヒットした頂点のポイント列へのインデックス
	* @param plVertIndex [out] ヒットした辺へのインデックス
	*
	* @retval esriGeometryPartNone 指定したエリア内でヒットしない
	* @retval esriGeometryPartVertex 頂点にヒットした
	* @retval esriGeometryPartBoundary 辺上にヒットした
	*/
	esriGeometryHitPartType HitTest( IPointPtr piTestPoint, double dbTolerance, IPoint** piPoint, double* pdbHitDist = NULL, LONG* plPartIndex = NULL, LONG* plVertIndex = NULL, esriGeometryHitPartType testType = (esriGeometryHitPartType)(esriGeometryPartVertex|esriGeometryPartBoundary) );

	/**
	* @brief 指定ジオメトリを指定した点でヒットテスト
	*
	* @param piGeometry [in] ヒットテスト対象のジオメトリ
	* @param piTestPoint [in] テストを実施するポイント
	* @param dbTolerance [in] ヒットテスト判定の閾値
	* @param piPoint [out] ヒットしたポイント
	* @param pdbHitDist [out] 指定したポイントから実際にヒットしたポイントまでの距離
	* @param plPartIndex [out] ヒットした頂点のポイント列へのインデックス
	* @param plVertIndex [out] ヒットした辺へのインデックス
	*
	* @retval esriGeometryPartNone 指定したエリア内でヒットしない
	* @retval esriGeometryPartVertex 頂点にヒットした
	* @retval esriGeometryPartBoundary 辺上にヒットした
	*/
	esriGeometryHitPartType HitTest( IGeometryPtr piGeometry, IPointPtr piTestPoint, double dbTolerance, IPoint** piPoint, double* pdbHitDist = NULL, LONG* plPartIndex = NULL, LONG* plVertIndex = NULL, esriGeometryHitPartType testType = (esriGeometryHitPartType)(esriGeometryPartVertex|esriGeometryPartBoundary) );

	/**
	* @brief 管理対象フィーチャを囲むエンベロープの取得
	*
	* @note 現在管理対象になっているフィーチャを包含するエンベロープを返す。
	*
	* @param ppiEnv [out] 管理フィーチャを包含するエンベロープの取得
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetEnvelope( IEnvelope** ppiEnv);

	/**
	* @brief 管理対象フィーチャのフィールド名とそのインデックス管理マップを作成する。
	*
	* @param cNameMap [out] フィールド名称とカラムインデックスの対応マップコンテナ
	* @param cAliasMap [out] フィールドエイリアス名称とカラムインデックスの対応マップコンテナ
	*
	* @return フィールド数
	*/
	LONG GetFieldMap( FIELDMAP& cNameMap, FIELDMAP& cAliasMap);

	/**
	 * @brief PECリンクかどうか
	 *
	 * @note ROADCLASS_C=27 or 28 の場合が PEC リンクに該当します
	 */
	BOOL IsPECLink();

	/**
	* @brief 指定のジオメトリとの交差検証
	*
	* @see AheIsCross()
	*
	* @param piGeom [in] 比較対象ジオメトリインスタンス
	*
	* @retval TRUE 交差
	* @retval FALSE 非交差
	*/
	BOOL IsCrosses( IGeometryPtr piGeom);
	/**
	* @brief 指定のジオメトリとの交差検証
	*
	* @see AheIsCrosses()
	*
	* @param cTargetItem [in] 比較対象管理クラス
	*
	* @retval TRUE 交差
	* @retval FALSE 非交差
	*/
	BOOL IsCrosses( CTargetItem& cTargetItem);

	/**
	* @brief 指定のジオメトリとの内包検証
	*
	* @see AheIsWithin()
	*
	* @param piGeom [in] 比較対象ジオメトリインスタンス
	*
	* @retval TRUE 内包
	* @retval FALSE 非内包
	*/
	BOOL IsWithin( IGeometryPtr piGeom);

	/**
	* @brief 指定のジオメトリとの内包検証
	*
	* @see AheIsWithin2()
	*
	* @param piGeom [in] 比較対象ジオメトリインスタンス
	*
	* @retval TRUE 内包
	* @retval FALSE 非内包
	*/
	BOOL IsWithin2( IGeometryPtr piGeom);

	/**
	* @brief 指定のジオメトリとの内包検証
	*
	* @see AheIsWithin()
	*
	* @param piGeom [in] 比較対象管理クラス
	*
	* @retval TRUE 内包
	* @retval FALSE 非内包
	*/
	BOOL IsWithin( CTargetItem& cTargetItem);

	/**
	* @brief 指定のジオメトリとの接触検証
	*
	* @see AheIsTouch()
	*
	* @param piGeom [in] 比較対象ジオメトリインスタンス
	*
	* @retval TRUE 接触
	* @retval FALSE 非接触
	*/
	BOOL IsTouches( IGeometryPtr piGeom);
	/**
	* @brief 指定のジオメトリとの接触検証
	*
	* @see AheIsTouch()
	*
	* @param piGeom [in] 比較対象管理クラス
	*
	* @retval TRUE 接触
	* @retval FALSE 非接触
	*/
	BOOL IsTouches( CTargetItem& cTargetItem);

	/**
	* @brief 指定のジオメトリとの非接触検証
	*
	* @see AheIsDisjoint()
	*
	* @param piGeom [in] 比較対象ジオメトリインスタンス
	*
	* @retval TRUE 非接触
	* @retval FALSE 接触
	*/
	BOOL IsDisjoint( IGeometryPtr piGeom);
	/**
	* @brief 指定のジオメトリとの非接触検証
	*
	* @see AheIsDisjoint()
	*
	* @param piGeom [in] 比較対象管理クラス
	*
	* @retval TRUE 非接触
	* @retval FALSE 接触
	*/
	BOOL IsDisjoint( CTargetItem& cTargetItem);

	/**
	* @brief 指定のジオメトリとの合同検証
	*
	* @see AheIsEqual()
	*
	* @param piGeom [in] 比較対象ジオメトリインスタンス
	*
	* @retval TRUE 合同
	* @retval FALSE 非合同
	*/
	BOOL IsEquals( IGeometryPtr piGeom);
	/**
	* @brief 指定のジオメトリとの合同検証
	*
	* @see AheIsEqual()
	*
	* @param piGeom [in] 比較対象管理クラス
	*
	* @retval TRUE 合同
	* @retval FALSE 非合同
	*/
	BOOL IsEquals( CTargetItem& cTargetItem);

	/**
	* @brief 指定のジオメトリとの部分的重複検証
	*
	* @see AheIsOverlap()
	*
	* @param piGeom [in] 比較対象ジオメトリインスタンス
	*
	* @retval TRUE 重複
	* @retval FALSE 非重複
	*/
	BOOL IsOverlaps( IGeometryPtr piGeom);
	/**
	* @brief 指定のジオメトリとの部分的重複検証
	*
	* @see AheIsOverlap()
	*
	* @param piGeom [in] 比較対象管理クラス
	*
	* @retval TRUE 重複
	* @retval FALSE 非重複
	*/
	BOOL IsOverlaps( CTargetItem& cTargetItem);

	/**
	* @brief 指定のジオメトリとの包含検証
	*
	* @see AheIsContain()
	*
	* @param piGeom [in] 比較対象ジオメトリインスタンス
	*
	* @retval TRUE 包含
	* @retval FALSE 非包含
	*/
	BOOL IsContains( IGeometryPtr piGeom);
	/**
	* @brief 指定のジオメトリとの包含検証
	*
	* @see AheIsContain2()
	*
	* @param piGeom [in] 比較対象ジオメトリインスタンス
	*
	* @retval TRUE 包含
	* @retval FALSE 非包含
	*/
	BOOL IsContains2( IGeometryPtr piGeom);
	/**
	* @brief 指定のジオメトリとの包含検証
	*
	* @see AheIsContain()
	*
	* @param piGeom [in] 比較対象管理クラス
	*
	* @retval TRUE 包含
	* @retval FALSE 非包含
	*/
	BOOL IsContains( CTargetItem& cTargetItem);

	/**
	* @brief ２次メッシュノード検証
	*
	* @retval TRUE ２次メッシュノード
	* @retval FALSE 非２次メッシュノード
	*/
	BOOL IsBorderNode( void);

	/**
	* @brief ２次メッシュノード検証
	*
	* @retval TRUE ２次メッシュノード
	* @retval FALSE 非２次メッシュノード
	*/
	BOOL IsBorderPECNode( void);

	/**
	* @brief フィーチャの削除
	*
	* @retval TRUE 正常終了
	* @retval FALSE 不正終了
	*/
	BOOL Delete();

	/**
	* @brief 形状チェック
	*
	* @note チェック結果の文字列は GetErrMessage() で取得できます
	* @see GetErrMessage
	*
	* @param check				[out]			チェック結果
	* @param bCheckRoad			[in,optional]	道路リンクとノードの接続性をチェックするかどうか（デフォルト：TRUE）
	* @param bIsCheckAngle		[in,optional]	セグメント接続角度をチェックするかどうか（デフォルト：TRUE）
	* @param ipRule				[in,optional]	ルールオブジェクト（デフォルト：NULL）
	*
	* @retval TRUE 正常終了
	* @retval FALSE 不正終了
	*/
	BOOL IsSafeGeometry( CErrObjContainer& errs, BOOL bCheckRoad = TRUE, BOOL bIsCheckAngle = TRUE, ISiNDYRule3Ptr ipRule = NULL );

	/** 
	 * @brief ポイントが From ポイントであるかどうかの判定
	 *
	 * @see AheIsFromPoint()
	 *
	 * @param ipPointGeom	[in]			ポイントジオメトリ
	 * @param ipSpRef		[in,optional]	ISpatialRefenrece
	 *
	 * @retval TRUE From ポイントである
	 * @retval FALSE From ポイントではない、又はラインフィーチャではない
	 */
	BOOL IsFromPoint( IGeometryPtr ipPointGeom, ISpatialReferencePtr ipSpRef = NULL );

	/** 
	 * @brief ポイントが To ポイントであるかどうかの判定
	 *
	 * @see AheIsToPoint()
	 *
	 * @param ipPointGeom	[in]			ポイントジオメトリ
	 * @param ipSpRef		[in,optional]	ISpatialRefenrece
	 *
	 * @retval TRUE To ポイントである
	 * @retval FALSE To ポイントではない、又はラインフィーチャではない
	 */
	BOOL IsToPoint( IGeometryPtr ipPointGeom, ISpatialReferencePtr ipSpRef = NULL );

	/**
	 * @brief ポイントが端点であるかどうかの判定
	 *
	 * @see AheIsEndPoint()
	 *
	 * @param ipPointGeom	[in]			ポイントジオメトリ
	 * @param ipSpRef		[in,optional]	ISpatialRefenrece
	 *
	 * @retval TRUE 端点である
	 * @retval FALSE端点ではない、又はラインフィーチャではない
	 */
	BOOL IsEndPoint( IGeometryPtr ipPointGeom, ISpatialReferencePtr ipSpRef = NULL );
	
	/**
	* @brief 編集完了
	*
	* @retval TRUE 正常終了
	* @retval FALSE 不正終了
	*/
	BOOL Store();

	/**
	* @brief 地理参照を調整
	*
	* @note 管理対象のレイヤの地理参照をフィーチャに設定
	*
	* @param piSpatialReference [in] 設定する地理参照指定しない場合は、管理対象のレイヤから取得して設定
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL SnapToSpatialReference( ISpatialReferencePtr piSpatialReference = NULL);

	/**
	* @brief 指定分のサイズ拡張
	*
	* @note TopologicalOperator::Buffer相当の処理
	* @note ppiGeometryにNULL指定した場合、ジオメトリは管理対象フィーチャに適用される。Storeは行わない。
	*
	* @param dbDistance [in] 拡張サイズ（MapUNIT
	* @param ppiGeometry [out] 拡張結果形状
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL Buffer( double dbDistance, IGeometry** ppiGeometry = NULL);

	/**
	* @brief 指定形状との差分取得
	*
	* @note TopologicalOperator::Difference相当の処理
	* @note ppiGeometryにNULL指定した場合、ジオメトリは管理対象フィーチャに適用される。Storeは行わない。
	*
	* @param piGeometrySrc [in] 比較形状
	* @param ppiGeometry [out] 拡張結果形状
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL Difference( IGeometryPtr piGeometrySrc, IGeometry** ppiGeometry = NULL);
	/**
	* @brief 指定形状との差分取得
	*
	* @note TopologicalOperator::Difference相当の処理
	* @note ppiGeometryにNULL指定した場合、ジオメトリは管理対象フィーチャに適用される。Storeは行わない。
	*
	* @param cTargetItem [in] 比較形状
	* @param ppiGeometry [out] 拡張結果形状
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL Difference( CTargetItem& cTargetItem, IGeometry** ppiGeometry = NULL);

	/**
	* @brief 指定形状との共通形状抽出
	* 
	* @note TopologicalOperator::Difference相当の処理
	* @note ppiGeometryにNULL指定した場合、ジオメトリは管理対象フィーチャに適用される。Storeは行わない。
	*
	* @param piGeometrySrc [in] 比較形状
	* @param resultDimension [in]
	* @param ppiGeometry [out] 拡張結果形状
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL Intersect( IGeometryPtr piGeometrySrc, esriGeometryDimension resultDimension, IGeometry** ppiGeometry = NULL);
	/**
	* @brief 指定形状との共通形状抽出
	* 
	* @note TopologicalOperator::Difference相当の処理
	* @note ppiGeometryにNULL指定した場合、ジオメトリは管理対象フィーチャに適用される。Storeは行わない。
	*
	* @param cTargetItem [in] 比較形状
	* @param resultDimension [in]
	* @param ppiGeometry [out] 拡張結果形状
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL Intersect( CTargetItem& cTargetItem, esriGeometryDimension resultDimension, IGeometry** ppiGeometry = NULL);

	/**
	* @brief 指定形状との総合形状抽出
	* 
	* @note TopologicalOperator::Union相当の処理
	* @note ppiGeometryにNULL指定した場合、ジオメトリは管理対象フィーチャに適用される。Storeは行わない。
	*
	* @param piGeometrySrc [in] 比較形状
	* @param ppiGeometry [out] 拡張結果形状
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL Union( IGeometryPtr piGeometrySrc, IGeometry** ppiGeometry = NULL);
	/**
	* @brief 指定形状との総合形状抽出
	* 
	* @note TopologicalOperator::Union相当の処理
	* @note ppiGeometryにNULL指定した場合、ジオメトリは管理対象フィーチャに適用される。Storeは行わない。
	*
	* @param cTargetItem [in] 比較形状
	* @param ppiGeometry [out] 拡張結果形状
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL Union( CTargetItem& cTargetItem, IGeometry** ppiGeometry = NULL);

	/**
	 * @brief エラーメッセージの取得
	 *
	 * @retval CString エラーメッセージ
	 */
	CString GetErrMessage();

	/**
	 * @brief エラーメッセージの削除
	 */
	void RemoveErrMessage();

	/**
	* @brief 指定形状との差分取得
	*
	* @note TopologicalOperator::SymmetricDifference相当の処理
	* @note ppiGeometryにNULL指定した場合、ジオメトリは管理対象フィーチャに適用される。Storeは行わない。
	*
	* @param piGeometrySrc [in] 比較形状
	* @param ppiGeometry [out] 拡張結果形状
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL SymmetricDifference( IGeometryPtr piGeometrySrc, IGeometry** ppiGeometry);

	/**
	* @brief 指定形状との差分取得
	*
	* @note TopologicalOperator::SymmetricDifference相当の処理
	* @note ppiGeometryにNULL指定した場合、ジオメトリは管理対象フィーチャに適用される。Storeは行わない。
	*
	* @param ppiGeometry [out] 拡張結果形状
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL SymmetricDifference( IGeometry** ppiGeometry);

	/**
	* @brief ポイントコレクションの取得
	*
	* @param piGeometrySrc [in] 取得形状
	*
	* @retval IPointCollection のインスタンス
	*/
	IPointCollectionPtr Collection( IGeometryPtr piGeometrySrc );

	/**
	* @brief ポイントコレクションの取得
	*
	* @retval IPointCollection のインスタンス
	*/
	IPointCollectionPtr GetPointCollection();

	/**
	* @brief ポリラインの取得
	*
	* @param piGeometrySrc [in] 取得形状
	*
	* @retval IPolyline のインスタンス
	*/
	IPolylinePtr GetPolyline( IGeometryPtr piGeometrySrc );

	/**
	* @brief ポリラインの取得
	*
	* @param piGeometrySrc [in] 取得形状
	* @param ppiPointCollection [out] 結果ポリライン
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetPolyline( IGeometryPtr piGeometrySrc, IPolyline** ppiPolyline);

	/**
	* @brief ポリラインの取得
	*
	* @param ppiPointCollection [out] 結果ポリライン
	*
	* @retval TRUE 正常終了
	* @retval FALSE 異常終了
	*/
	BOOL GetPolyline( IPolyline** ppiPolyline);

	/**
	* @brief ポリラインの取得
	*
	* @retval IPolyline のインスタンス
	*/
	IPolylinePtr GetPolyline();

	/**
	* @brief 構成点数の取得
	*
	* @retval 構成点数
	*/
	LONG GetVertexCount();

	/**
	* @brief フィーチャタイプの取得
	*
	* @retval sindyeFeatureKinds
	*/
	sindyeFeatureKinds GetFeatureKinds() const;

	/**
	 * フィーチャから属性を取得する
	 *
	 * @param ipFeature		[in]	フィーチャ（レコード）
	 * @param lpcszFieldName	[in]	取得するフィールド名
	 * @param vaRetValue	[out]	取得できた値
	 *
	 * @retval 
	 */
	void GetAttribute( IFeaturePtr ipFeature, LPCTSTR lpcszFieldName, CComVariant& vaRetValue ) const;

	/**
	 * フィーチャから属性を取得する
	 *
	 * @param ipFeature		[in]	フィーチャ（レコード）
	 * @param lpcszFieldName	[in]	取得するフィールド名
	 *
	 * @retval 取得できた値
	 */
	CComVariant GetAttribute( IFeaturePtr ipFeature, LPCTSTR lpcszFieldName ) const;
	
	/**
	 * フィーチャから属性を取得する
	 *
	 * @param lpcszFieldName	[in]	取得するフィールド名
	 * @param vaRetValue	[out]	取得できた値
	 *
	 * @retval 
	 */
	void GetAttribute( LPCTSTR lpcszFieldName, CComVariant& vaRetValue ) const;

	/**
	 * フィーチャから属性を取得する
	 *
	 * @param lpcszFieldName	[in]	取得するフィールド名
	 *
	 * @retval 取得できた値
	 */
	CComVariant GetAttribute( LPCTSTR lpcszFieldName ) const;

	/**
	 * フィーチャに属性をセットする
	 * 
	 * @note Store() はしません
	 *
	 * @param ipFeature			[in]	セットするフィーチャ（レコード）
	 * @param lpcszFieldName	[in]	セットするフィールド名
	 * @param vaValue			[in]	セットする値
	 *
	 * @retval TRUE セットできた
	 * @retval FALSE セットできなかった
	 */
	BOOL SetAttribute( IFeaturePtr ipFeature, LPCTSTR lpcszFieldName, CComVariant vaValue );
	
	/**
	 * フィーチャに属性をセットする
	 * 
	 * @note Store() はしません
	 *
	 * @param lpcszFieldName	[in]	セットするフィールド名
	 * @param vaValue			[in]	セットする値
	 *
	 * @retval TRUE セットできた
	 * @retval FALSE セットできなかった
	 */
	BOOL SetAttribute( LPCTSTR lpcszFieldName, CComVariant vaValue);

	/**
	* @brief フィーチャを XOR で描画する
	*
	* @param ipFeature			[in]			表示するフィーチャ
	* @param ipScreen			[in]			IDisplay のインスタンス
	* @param dbBufferSize		[in]			バッファするサイズ
	* @param bDrawVertex		[in]			頂点を描画するかどうか
	* @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing するかどうか（デフォルト TRUE）
	*/
	void DrawFeatureXOR( IFeaturePtr ipFeature, IScreenDisplayPtr ipScreen, double dbBufferSize, BOOL bDrawVertex, BOOL bDoStartStop = TRUE);
	
	/**
	* @brief フィーチャを XOR で描画する
	*
	* @param ipScreen			[in]			表示するフィーチャ
	* @param dbBufferSize		[in]			バッファするサイズ
	* @param bDrawVertex		[in]			頂点を描画するかどうか
	* @param bDoStartStop		[in,optional]	StartDrawing, StopDrawing するかどうか（デフォルト TRUE）
	*/
	void DrawFeatureXOR( IScreenDisplayPtr ipScreen, double dbBufferSize, BOOL bDrawVertex, BOOL bDoStartStop = TRUE);

	/**
	* @brief フィーチャの頂点にヒットした場合はそのインデックス番号を、ヒットしなかった場合は -1 を返す
	*
	* @note TestGeometryHit() の簡易版
	*
	* @param ipFeature		[in]			対象となる IFeature のインスタンス
	* @param ipPoint		[in]			テストする IPoint のインスタンス
	* @param dTorelance		[in]			距離
	*
	* @retval -1以外 ヒットしたインデックス番号
	* @retval -1 ヒットしなかった
	**/
	LONG HitTestVertexIndex( IFeaturePtr ipFeature, IPointPtr ipPoint, double dTorelance);
	
	/**
	* @brief フィーチャの頂点にヒットした場合はそのインデックス番号を、ヒットしなかった場合は -1 を返す
	*
	* @note TestGeometryHit() の簡易版
	*
	* @param ipPoint		[in]			テストする IPoint のインスタンス
	* @param dTorelance		[in]			距離
	*
	* @retval -1以外 ヒットしたインデックス番号
	* @retval -1 ヒットしなかった
	**/
	LONG HitTestVertexIndex( IPointPtr ipPoint, double dTorelance);

	/**
	 * フィーチャが編集可能かどうかチェックする
	 *
	 * @note ISiNDYRuleObj::IsEditableFeature() をラップして、TRUE か FALSE を返します。
	 *
	 * @param ipFature	[in]	チェック対象フィーチャ
	 * @param type		[in]	sindyeEditType
	 *
	 * @retval TRUE 編集可能
	 * @retval FALSE 編集不可能
	 */
	BOOL IsEditableItem( _IRowPtr ipFeature, sindyeEditType type) const;

	/**
	 * フィーチャが編集可能かどうかチェックする
	 *
	 * @note ISiNDYRuleObj::IsEditableFeature() をラップして、TRUE か FALSE を返します。
	 *
	 * @param ipFature	[in]	チェック対象フィーチャ
	 * @param type		[in]	sindyeEditType
	 *
	 * @retval TRUE 編集可能
	 * @retval FALSE 編集不可能
	 */
	BOOL IsEditableItem( sindyeEditType type) const;

	/**
	 * @brief フィーチャの構成点を取得する
	 *
	 * @param ipFeature			[in]			IFeature のインスタンス
	 * @param lIndex			[in]			取得する構成点のインデックス
	 *
	 * @return IPoint のインスタンス
	 */
	IPointPtr GetPoint( LONG lIndex );

	/**
	 * @brief フィーチャの構成点を削除する
	 *
	 * @param lIndex			[in]			取得する構成点のインデックス
	 *
	 * @return IPoint のインスタンス
	 */
	IPointCollectionPtr RemovePoint( LONG lIndex );

	/**
	 * @brief 属性をコピーする
	 *
	 * @param ipFeature			[in]			IFeature のインスタンス
	 * @param cNewItem			[in]			アイテム
	 *
	 * @return 
	 */
	BOOL CopyAttribute( IFeaturePtr ipFeature, const CTargetItem& cNewItem );

	/**
	 * @brief 属性をコピーする
	 *
	 * @param ipFeature			[in]			IFeature のインスタンス
	 * @param ipNewFeature		[in]			IFeature のインスタンス
	 *
	 * @return 
	 */
	BOOL CopyAttribute( IFeaturePtr ipFeature, IFeaturePtr ipNewFeature );

	/**
	 * @brief 属性をコピーする
	 *
	 * @param cNewItem			[in]			アイテム
	 *
	 * @return 
	 */
	//BOOL CopyAttribute( const CTargetItem& cNewItem );

	/**
	 * @brief 属性をコピーする
	 *
	 * @param ipNewFeature		[in]			IFeature のインスタンス
	 *
	 * @return 
	 */
	BOOL CopyAttribute( IFeaturePtr ipNewFeature );

	/**
	 * @brief 地理参照取得
	 *
	 * @return ISpatialReference のインスタンス
	 */
	ISpatialReferencePtr GetSpatialReference() const;

	/**
	 * @brief 強制的に Simplify() をする
	 *
	 * @see AheForceSimplify
	 *
	 * @retval TRUE 成功
	 * @retval FALSE 失敗
	 */
	BOOL Simplify();

	/**
 	 * @brief ジオメトリがZ値を持っているか
	 * @retval true  Z値持ちである
	 * @retval false Z値持ちではない
	 */
	bool HasZ();

protected:

	/**
	* @brief 構成点追加処理
	*
	* @note ジオメトリの任意の位置に構成点追加
	*
	* @retval TRUE 正常終了
	* @retval FALSE 不正終了
	*/
	BOOL AddNewPoint( IGeometryPtr piGeometry, LONG lIndex, IPointPtr piNewPoint);

protected:
	IFeaturePtr m_ipFeature;			//!< 管理対象フィーチャ
	CString m_strErrMessage;			//!< エラーメッセージ格納用
};

#include "TargetFeature.inl"

#endif	//__TARGETFEATURE_H__
