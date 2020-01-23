#pragma once

#include "Arguments.h"
#include "LogWriter.h"
#include "DatabaseUtility.h"

#include <ArcHelperEx/AheMeshUtil.h>

/**
 * @brief   メインクラス
 */
class CCopyOverlapFeatureAttr : public LogWriter
{
public:
	/**
	 * @brief 設定情報 
	 */
	struct SettingInfo
	{
		CString   strSource;          //!< 比較元の接続先
		CString   strSourceTable;     //!< 比較元のテーブル名
		CString   strSourceField;     //!< 比較元のフィールド名
		CString   strSourceWhere;     //!< 比較元のWHERE条件

		CString   strTarget;          //!< 比較先の接続先
		CString   strTargetTable;     //!< 比較先のテーブル名
		CString   strTargetField;     //!< 比較先のフィールド名
		CString   strTargetWhere;     //!< 比較先のWHERE条件

		CString   strMesh;            //!< 処理単位(メッシュ)の接続先
		CString   strMeshTable;       //!< 処理単位(メッシュ)のテーブル名
		CString   strMeshListPath;    //!< 処理範囲（メッシュリスト）のファイルパス

		CString   strExeMode;         //!< 実行モード
		CString   strAttrListPath;    //!< 属性変換表のファイルパス
		bool      bDatumFlag;         //!< 測地系強制変換フラグ
		bool      bFieldFlag;         //!< フィールド型強制変換フラグ

		double    dOverlay;           //!< 重畳率の閾値
		double    dMagnification;     //!< 拡大率の閾値
		double    dReduction;         //!< 縮小率の閾値
		double    dBuffer;            //!< バッファーの幅（メートル単位）

		SettingInfo(): bDatumFlag(false), bFieldFlag(false), dOverlay(-1.0), dMagnification(-1.0), dReduction(-1.0), dBuffer(-1.0) {}
		virtual ~SettingInfo(){}
	};

public:

	/**
	 * @brief   コンストラクタ
	 */
	CCopyOverlapFeatureAttr(): m_nMeshCodeIndex(-1), m_nSourceFieldIndex(-1), m_nTargetFieldIndex(-1),
		m_nUpdateTypeIndex(-1), m_nModifyProgNameIndex(-1), m_nProgModifyDateIndex(-1) {}

	/**
	 * @brief   デストラクタ
	 */
	virtual ~CCopyOverlapFeatureAttr(){};

	/**
	 * @brief   実行
	 * @param   args [in]  引数
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute(const Arguments& args);

private:
	static const int max_retry_num;     //!< 接続リトライ回数
	static const double max_retry_time; //!< 接続リトライ時間

	/**
	 * @brief   子バージョン生成
	 * @param   ipWorkspace [in] ワークスペース
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool createChildVer( const IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	ワークスペースオープン
	 * @param	name         [in]    接続名
	 * @param	ipWorkspace  [out]   ワークスペース
	 * @retval	true    成功
	 * @retval	false   失敗
	 */
	bool openWorkspace( const CString& name, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief   DBのオープン処理
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool openDB();

	/**
	 * @brief   フィーチャクラスを取得
	 * @param   name           [in]   フィーチャクラス名
	 * @param   ipWorkspace    [in]   ワークスペース
	 * @param   ipFeatureClass [out]  フィーチャクラス
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool openFeatureClass(const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass);

	/**
	 * @brief 入力フィーチャクラスを取得する
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool openFC();

	/**
	 * @brief 各フィールドのIndexを取得
	 * @retval true  取得成功
	 * @retval false 取得失敗
	 */
	bool getFieldsIndex();


	/**
	 * @brief 測地系のチェック
	 * @retval true  異常なし
	 * @retval false 異常あり
	 */
	bool checkDatum();

	/**
	 * @brief フィールド型のチェック
	 * @retval true  異常なし
	 * @retval false 異常あり
	 */
	bool checkFieldType();

	/**
	 * @brief 対象フィールドを更新
	 * @param   maxOverlayOID [in]  出力用の比較元OID
	 * @param   maxAttrValue  [in]  更新対象の属性値
	 * @param   ipFeature     [in]  更新対象フィーチャー
	 * @param   errItem       [in]  エラー情報
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool updateField(long maxOverlayOID, const CString& maxAttrValue, IFeaturePtr& ipFeature, ErrItem& errItem);

	/**
	 * @brief   初期化
	 * @param   args [in]  引数
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool init(const Arguments& args);

	/**
	 * @brief   設定ファイルの読み込み
	 * @param   strFilePath [in]  設定ファイルのパス
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool readSettingFile( const CString& strFilePath );

	/**
	 * @brief   メッシュリストの読み込み
	 * @param   strFilePath [in]  メッシュリストのパス
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool readMeshList( const CString& strFilePath );

	/**
	 * @brief   属性変換表の読み込み
	 * @param   strFilePath [in]  属性変換表のパス
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool readAttributeList( const CString& strFilePath );

	/**
	 * @brief   必須項目（文字列）のチェック
	 * @param   strKey   [in]  項目名
	 * @param   strValue [in]  項目値
	 * @retval  true  異常なし
	 * @retval  false 異常あり
	 */
	bool checkStringSetting( const std::wstring& strKey, const CString& strValue );

	/**
	 * @brief   必須項目（数字型）のチェック
	 * @param   strKey   [in]  項目名
	 * @param   value    [in]  項目値
	 * @retval  true  異常なし
	 * @retval  false 異常あり
	 */
	bool checkNumericSetting( const std::wstring& strKey, double value );

	/**
	 * @brief   ファイルのチェック
	 * @param   strKey      [in]  項目名
	 * @param   strFilePath [in]  項目値
	 * @retval  true  異常なし
	 * @retval  false 異常あり
	 */
	bool checkFileSetting( const std::wifstream& ifs, const CString& strFilePath );

	/**
	 * @brief   設定内容のチェック
	 * @retval  true  異常なし
	 * @retval  false 異常あり
	 */
	bool checkSettings();

	/**
	 * @brief   設定情報の取得
	 * @param   info [in]  設定情報
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool getSettingInfo(const std::vector<std::wstring>& info);

	/**
	 * @brief   相対パスを絶対パスに変換
	 * @param   strPath [in]  相対パス
	 * @retval  絶対パス
	 */
	CString convertRelativePath(const CString& strPath);

	/**
	 * @brief  実行ログファイルにオプション引数を出力する
	 */
	void writeSettings();

	/**
	 * @brief   メッシュリストより、メッシュとバッファーのポリゴンを取得
	 * @param   mapMeshGeo [out]  処理範囲のメッシュ
	 * @retval  true 取得成功
	 * @retval  false 取得失敗
	 */
	bool getMeshPolygons(std::map<unsigned long, std::pair<IGeometryPtr, IGeometryPtr>>& mapMeshGeo);

	/**
	 * @brief   比較元のジオメトリを取得
	 * @param   ipMeshGeo [in]   処理範囲のジオメトリ（バッファー使用）
	 * @param   mapSource [out]  比較元のジオメトリ
	 * @retval  true 取得成功
	 * @retval  false 取得失敗
	 */
	bool getSourceObjectInMesh(const IGeometryPtr& ipMeshGeo, std::map<long, GeoClass>& mapSource);

	/**
	 * @brief   比較先のフィールドを更新
	 * @param   meshGeoRec   [in]  処理範囲のジオメトリ
	 * @param   mapSource    [in]  比較元のジオメトリ
	 * @param   sourceRTree  [in]  比較元のRTree
	 * @retval  true 更新成功
	 * @retval  false 更新失敗
	 */
	bool updateTargetField(const std::pair<unsigned long, std::pair<IGeometryPtr, IGeometryPtr>>& meshGeoRec, const std::map<long, GeoClass>& mapSource, const BGRTree& sourceRTree);

	/**
	 * @brief   R-Treeを生成する
	 * @param   geoMap [in]  対象フィーチャクラス
	 * @param   rTree  [out] 結果のR-Tree(冒頭の[r]は参照の意味ではないです)
	 */
	void createRTree( const std::map<long, GeoClass>& geoMap, BGRTree& rTree );

	/**
	 * @brief   R-Treeから、与えた箱の範囲に引っかかるID群を取得
	 * @param   box   [in]  取得したい範囲(エンベロープ)
	 * @param   rTree [in]  取得してくる対象のR-Tree
	 * @retval  set<long>   対象範囲のID群
	 */
	std::set<long> getIntersectsIDs( const BGBox& box, const BGRTree& rTree );

	/**
	 * @brief   重畳率を取得
	 * @param   ipSourceGeo [in]  比較元のジオメトリ
	 * @param   ipTargetGeo [in]  比較先のジオメトリ
	 * @retval  double  重畳率
	 */
	double getOverlay( const IGeometryPtr& ipSourceGeo, const IGeometryPtr& ipTargetGeo );

	/**
	 * @brief   比較元との重畳率を取得
	 * @param   ipTargetGeo [in]  比較先のジオメトリ
	 * @param   sourceIDs   [in]  重なった比較元のID
	 * @param   mapSource   [in]  比較元のジオメトリ
	 * @param   errItem     [in]  エラー情報
	 * @retval  std::map<long, GeoClass> 重畳率と比較元を格納するコンテナ
	 */
	std::multimap<double, GeoClass> getAllOverlay( const IGeometryPtr& ipTargetGeo, const std::set<long>& sourceIDs, const std::map<long, GeoClass>& mapSource, ErrItem& errItem );

	/**
	 * @brief   比較元との拡縮率を見て、条件を満たす比較元を取得
	 * @param   ipTargetGeo [in]  比較先のジオメトリ
	 * @param   sourceIDs   [in]  重なった比較元のID
	 * @param   mapSource   [in]  比較元のジオメトリ
	 * @param   errItem     [in]  エラー情報
	 * @retval  std::map<long, GeoClass>  拡縮率を満たす比較元IDを格納するコンテナ
	 */
	std::set<long> getAllOkScale( const IGeometryPtr& ipTargetGeo, const std::set<long>& sourceIDs, const std::map<long, GeoClass>& mapSource, ErrItem& errItem );

	/**
	 * @brief   フィールド型が一致しているかをチェック
	 * @retval  true  フィールド型が一致している
	 * @retval  false フィールド型が一致していない
	 */
	bool isSameField();

	/**
	 * @brief   測地系が一致しているかをチェック
	 * @retval  true  測地系が一致している
	 * @retval  false 測地系が一致していない
	 */
	bool isSameSpRef();

	/**
	 * @brief   フィールドのタイプが処理対象であるかをチェック
	 * @param   fieldType [in]  チェック対象フィールドのタイプ
	 * @retval  true  フィールドのタイプが処理対象である
	 * @retval  false フィールドのタイプが処理対象ではない
	 */
	bool isProcessType( const esriFieldType& fieldType );

	/**
	 * @brief   フィールドのタイプが処理対象であるかをチェック
	 * @retval  true  フィールドのタイプが処理対象である
	 * @retval  false フィールドのタイプが処理対象ではない
	 */
	bool isProcessType();

	/**
	 * @brief   フィールド値を指定値に変換
	 * @param   strValue [in]  指定値
	 * @retval  CComVariant　フィールド値
	 */
	CComVariant convertFieldValue( const CString& strValue );

	/**
	 * @brief   エラー出力用の座標値を取得
	 * @param   shape   [in]   フィーチャ形状
	 * @param   errItem [out]  エラー情報
	 * @retval  true 取得成功
	 * @retval  false 取得失敗
	 */
	bool getShapeCoordinate( const IGeometryPtr& shape, ErrItem& errItem );

	/**
	 * @brief   エラー出力用の共通情報を取得
	 * @param   meshCode   [in]   メッシュコード
	 * @param   target     [in]   比較先の情報
	 * @retval  ErrItem 取得成功
	 */
	ErrItem getCommonErrorInfo( unsigned long meshCode, const GeoClass& target );

	/**
	 * @brief   エラー出力用の詳細情報を設定
	 * @param   errCode   [in]    エラーコード
	 * @param   errItem   [out]   エラー情報
	 */
	void setDetailErrorInfo( err_code::ECode errCode, ErrItem& errItem );

	/**
	 * @brief   エラー出力用の詳細情報をリセット
	 * @param   errItem   [out]   エラー情報
	 */
	void resetDetailErrorInfo( ErrItem& errItem );

	/**
	 * @brief   対象フィールドを更新
	 * @param   strValue  [in]   フィールド値
	 * @param   ipFeature [in]   更新対象フィーチャー
	 * @param   errItem   [in]   エラー情報
	 * @retval  true 更新成功
	 * @retval  false 更新失敗
	 */
	bool putValues(const CString& strValue, IFeaturePtr& ipFeature, ErrItem& errItem );


private:

	SettingInfo                 m_setting;                   //!< 設定情報

	std::set<unsigned long>     m_setMeshList;               //!< メッシュリスト
	std::map<CString, CString>  m_mapAttrList;               //!< 属性変換表(key:比較元の属性値、value:比較先の属性値)

	IWorkspacePtr               m_ipSourceWorkspace;         //!< 比較元ワークスペース
	IWorkspacePtr               m_ipTargetWorkspace;         //!< 比較先ワークスペース
	IWorkspacePtr               m_ipMeshWorkspace;           //!< メッシュワークスペース
	IWorkspaceEditPtr           m_ipWorkspaceEdit;           //!< 編集用ワークスペース（比較先）

	IFeatureClassPtr            m_ipSourceFC;                //!< 比較元フィーチャクラス
	IFeatureClassPtr            m_ipTargetFC;                //!< 比較先フィーチャクラス
	IFeatureClassPtr            m_ipMeshFC;                  //!< メッシュフィーチャクラス

	ISpatialReferencePtr        m_ipSourceSpRef;             //!< 比較元の空間参照
	ISpatialReferencePtr        m_ipTargetSpRef;             //!< 比較先の空間参照
	
	IFieldPtr                   m_ipSourceField;             //!< 比較元のフィールド
	IFieldPtr                   m_ipTargetField;             //!< 比較先のフィールド

	long                        m_nMeshCodeIndex;            //!< フィールドMeshCodeのIndex
	long                        m_nSourceFieldIndex;         //!< 比較元フィールドのIndex
	long                        m_nTargetFieldIndex;         //!< 比較先フィールドのIndex
	long                        m_nUpdateTypeIndex;          //!< フィールド最終更新内容ののIndex
	long                        m_nModifyProgNameIndex;      //!< フィールド最終更新プログラム名のIndex
	long                        m_nProgModifyDateIndex;      //!< フィールド最終プログラム更新日時のIndex
	
	std::set<long>              m_setProcessedOID;           //!< 処理済みの比較先OID
};

