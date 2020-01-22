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

#include "LogMgr.h"
#include "common.h"
#include <boost/program_options.hpp>

/**
 * @brief 対角線群格納用マルチマップ
 * @note 長さをキーに格納し、最長の対角線を取得できるようにする
 */
typedef std::multimap< double, ILinePtr, std::greater< double > > DiagonalLenMap;

/**
 * @brief 実行モード
 */
enum ExecuteMode
{
	ReleaseTsv,    // 検索リリースデータ作成
	TranslateList  // 多言語翻訳リスト作成
};

/**
 * @brief 建物ビル名称検索TSV変換クラス
 */
class BldNameTSVConverter
{
public:
	/**
	 * @brief コンストラクタ
	 */
	BldNameTSVConverter();

	/**
	 * @brief デストラクタ
	 */
	~BldNameTSVConverter();

	/**
	 * @brief 初期化
	 * @param argc [in] 引数の数
	 * @param argv [in] 引数リスト
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool init( int argc, _TCHAR* argv[] );

	/**
	 * @brief 処理の実行
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool exec();

private:
	/**
	 * @brief 引数のチェック
	 * @param argc [in] 引数の数
	 * @param argv [in] 引数リスト
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool checkArg( int argc, _TCHAR* argv[] );

	/**
	 * @brief 引数パラメータの取得
	 * @param varMap [in] 引数マップ
	 * @param strName [in] 引数名
	 * @param strParam [out] 取得したパラメータ
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getArgParam( const boost::program_options::variables_map& varMap, const std::string& strName, CString& strParam );

	/**
	 * @brief 設定ファイルの読み込み
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool loadIniFile();

	/**
	 * @brief 住所中間テーブル格納DBへの接続
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool openAddrDB();

	/**
	 * @breif エラーメッセージの出力
	 * @param errLv [in] エラーレベル
	 * @param strLayer [in] レイヤ名
	 * @param lOID [in] オブジェクトID
	 * @param strMsg [in] エラーメッセージ
	 */
	void outputErrLog( LogMgr::EErrLevel errLv, const CString& strLayer, long lOID, const CString& strMsg );

	/**
	 * @brief 建物種別コードマスタの生成
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool createBldgClassMaster();

	/**
	 * @brief 検索リリースデータ（TSV）の生成
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool createBldgNameTSV();

	/**
	 * @brief 住所コードの取得
	 * @param lOID [in] 建物ビル名称ポイントOID
	 * @param lBldgOID [in] 建物ポリゴンOID
	 * @param ipFeature [in] 建物ビル名称ポイントフィーチャ
	 * @param strAddrCodeRet [out] 住所コード
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getAddrCode( long lOID, long lBldgOID, const IFeaturePtr& ipFeature, CString& strAddrCodeRet );

	/**
	 * @brief 住所コードの取得（住所中間テーブルより）
	 * @param lBldgOID [in] 建物ポリゴンOID
	 * @param strAddrCode [out] 住所コード（20桁）
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getAddrCodeFromIntermediate( long lBldgOID, CString& strAddrCode );

	/**
	 * @breif 建物ポリゴンOID→住所コード対応表の生成
	 * @note 当該建物ポリゴンOIDを含む 500,000 毎のOIDの範囲で、住所中間テーブルから対応表を作成する
	 * @param lBldgOID [in] 建物ポリゴンOID
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool createMapBldgOID2AddrCode( long lBldgOID );

	/**
	 * @brief 住所コードの取得（都市地図行政界ポリゴンより）
	 * @param lOID [in] 名称ポイントのOID（エラー出力用）
	 * @param ipGeometry [in] 名称ポイントのジオメトリ
	 * @param strAddrCode [out] 住所コード（16桁 or 11桁 or 5桁）
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getAddrCodeFromAdmin( long lOID, const IGeometryPtr& ipGeometry, CString& strAddrCode );

	/**
	 * @brief 建物ポリゴンの代表ポイントを取得する
	 * @param lOID [in] 名称ポイントのOID（エラー出力用）
	 * @param lBldgOID [in] 建物ポリゴンのOID
	 * @param ipFeature [in] 名称ポイントのフィーチャ
	 * @param ipPoint [out] 代表ポイント
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getBldgRepPoint( long lOID, long lBldgOID, const IFeaturePtr& ipFeature, IPointPtr& ipPoint );

	/**
	 * @brief 対象ポリゴンの代表ポイントを取得する
	 * @param ipPolygon [in] 対象ポリゴン
	 * @param ipPoint [out] 代表ポイント
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getRepPoint( const IPolygon4Ptr& ipPolygon, IPointPtr& ipPoint );

	/**
	 * @brief 対角線の生成とコンテナへの追加
	 * @param ipPolygon [in] 対象ポリゴン
	 * @param ipRing1 [in] 対象リング①
	 * @param lIndex1 [in] 対象構成点インデックス①
	 * @param ipRing2 [in] 対象リング②
	 * @param lIndex2 [in] 対象構成点インデックス②
	 * @param ipSpRef [in] 空間参照
	 * @param mapDiagonals [out] 対角線群格納コンテナ
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool createDiagonals( const IPolygon4Ptr& ipPolygon, const IRingPtr& ipRing1, long lIndex1, const IRingPtr& ipRing2, long lIndex2, const ISpatialReferencePtr& ipSpRef, DiagonalLenMap& mapDiagonals );

	/**
	 * @brief 建物ビル名称ポイントフィーチャカーソル取得
	 * @param ipFeatureCur [out] フィーチャカーソル（建物ビル名称ポイント）
	 * @param lCount       [out] 取得件数
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool searchBldNamePoint(IFeatureCursorPtr& ipFeatureCur, long& lCount);

	/**
	 * @brief 建物ビル名称ポイント属性値（実行モード共通属性）取得
	 * @param ipFeature [in]  フィーチャ（建物ビル名称ポイント）
	 * @param rec       [out] 建物ビル名称ポイントコンテナ
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getCommonAttrValue(const IFeaturePtr& ipFeature, BldNamePointRec& rec );

	/**
	 * @brief 建物ビル名称検索リリースデータ作成処理実行
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool execCreateReleaseTsv();

	/**
	 * @brief 多言語翻訳リスト作成処理実行
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool execCreateMultiTranslateList();

	/**
	 * @brief 多言語翻訳リスト作成
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool createMultiTranslateList();


private:
	ExecuteMode m_mode;         //!< 実行モード
	CString		m_strRunLog;	//!< 実行ログファイル名
	CString		m_strErrLog;	//!< エラーログファイル名
	CString		m_strIniFile;	//!< 設定ファイル名
	CString     m_strAddColumn; //!< 多言語翻訳リスト追加カラム設定ファイル
	CString     m_maxRecNum;    //!< ファイル内最大レコード件数
	std::vector<CString> m_addcolumnList; //!< 多言語翻訳リスト追加カラムリスト

	CString		m_strDBBase;	//!< ベースDB
	CString		m_strDBMap;		//!< 地図DB
	CString		m_strDBAddr;	//!< 住所DB
	CString		m_strDBAM;		//!< 住所中間テーブル格納DB
	CString		m_strGrpCode;	//!< データ分類コード
	CString		m_strOutDir;	//!< 出力ディレクトリ

	IFeatureClassPtr	m_ipFCBldgNamePoint;	//!< 建物ビル名称ポイントフィーチャクラス
	IFeatureClassPtr	m_ipFCBuilding;			//!< 建物ポリゴンフィーチャクラス
	IFeatureClassPtr	m_ipFCCityAdmin;		//!< 行政界ポリゴンフィーチャクラス

	FieldIndexes		m_mapIndexesBldgNamePoint;	//!< 建物ビル名称ポイントフィールドインデックスマップ
	FieldIndexes		m_mapIndexesCityAdmin;		//!< 行政界ポリゴンフィールドインデックスマップ

	ADODB::_ConnectionPtr	m_conAddrDB;	//!< 住所中間テーブル格納DBへの接続オブジェクト

	std::map< long, CString >	m_mapBldgOID2AddrCode;	//!< 建物ポリゴンOID→住所コード対応表
};
