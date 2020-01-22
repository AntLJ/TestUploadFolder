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

/**
 * @brief 調査対象ポイントレコード
 */
class SurveyPointRec : public std::vector< CString >
{
public:
	/**
	 * @breif コンストラクタ
	 */
	SurveyPointRec();

	/**
	 * @brief デストラクタ
	 */
	~SurveyPointRec();

	/**
	 * @brief 比較演算子
	 */
	bool operator < ( const SurveyPointRec& cRec ) const
	{
		if( e_objectID == cRec.e_objectID )
			return ( e_srcType < cRec.e_srcType );
		else
			return ( e_objectID < cRec.e_objectID );
	}

	bool setGeometry( const IFeaturePtr& ipFeature );

public:
	long	e_objectID;		//!< オブジェクトID
	long	e_srcType;		//!< 出典コード
	long	e_bldOID;		//!< 建物ポリゴンOID

	IPointPtr	e_ipPoint;		//!< 当該調査対象ポイントジオメトリ
};

/**
 * @brief 調査対象ポイントリスト管理クラス
 */
class SurveyPointManager : std::set< SurveyPointRec, std::less< SurveyPointRec > >
{
public:
	/**
	 * @brief 調査対象ポイントリストのフィールドインデックス
	 */
	enum EFieldIndex
	{
		kObjectID = 0,	//!< オブジェクトID
		kName,			//!< 名称
		kOrgName1,		//!< 加工前名称1
		kOrgName2,		//!< 加工前名称2
		kKana,			//!< カナ
		kAddr,			//!< 住所
		kOrgAddr,		//!< 整形前住所
		kAddrCode,		//!< 住所コード
		kAddrLon,		//!< 住所経度
		kAddrLat,		//!< 住所緯度
		kRepType,		//!< 地番代表点複合コード
		kAddrLv,		//!< 住所レベルコード
		kPinPoint,		//!< ピンポイントフラグ
		kMultiSameBldg,	//!< 同一家形内複数住所存在フラグ
		kSameAddr,		//!< 同一iPC住所複数存在フラグ
		kSameAddrCount,	//!< 同一iPC住所数
		kAddrVer,		//!< 住所解析元XYDB名
		kSrcType,		//!< 出典コード
		kSrcName,		//!< 出典元
		kSrcDate,		//!< 出典資料入手時期
		kSrcID,			//!< 出典資料ID
		kBldg1Div1,		//!< 用途区分1
		kBldg1Div2,		//!< 用途区分2
		kBldg2Div1,		//!< 建物棟用途区分1
		kBldg2Div2,		//!< 建物棟用途区分2
		kHeight,		//!< 建物高さ
		kFloors,		//!< 地上階数
		kBaseFloors,	//!< 地下階数
		kBldgArea,		//!< 建物面積
		kAllArea,		//!< 延べ床面積
		kSrcStartDate,	//!< 素材使用開始年月日
		kSrcUpdateDate,	//!< 素材更新年月日
		kUpdateDate,	//!< 更新年月日
		kAdoption,		//!< 採用コード
	};

	/**
	 * @brief コンストラクタ
	 */
	SurveyPointManager(
		const IFeatureClassPtr& ipCityMesh,
		const IFeatureClassPtr& ipCityAdmin,
		const std::map<CString, long>& mapCityMeshIndexes,
		const std::map<CString, long>& mapCityAdminIndexes,
		const std::map<CString, long>& mapBldgSrcPtIndexes,
		const std::map<CString, long>& mapTownpageIndexes,
		const CString& sourceName,
		const CString& sourceDate,
		const ADODB::_ConnectionPtr& conAddrDB
		);

	/**
	 * @brief デストラクタ
	 */
	~SurveyPointManager();

	/**
	 * @brief 出力先ファイルオープン
	 * @param fileName [in] 出力先ファイル名
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool open( const CString& fileName );

	/**
	 * @brief 出力先ファイルクローズ
	 */
	void close();

	/**
	 * @brief 調査対象ポイントレコードの追加（素材ポイント由来）
	 * @param srcOID [in] 素材ポイントOID
	 * @param bldOID [in] 建物ポリゴンOID
	 * @param ipFeature [in] 対象素材ポイントフィーチャ
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool addFromBldgNameSrc( long srcOID, long bldOID, const IFeaturePtr& ipFeature );

	/**
	 * @brief 調査対象ポイントレコードの追加（タウンページ由来）
	 * @param twnOID [in] タウンページポイントOID
	 * @param bldOID [in] 建物ポリゴンOID
	 * @param ipFeature [in] 対象タウンページポイントフィーチャ
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool addFromTownpage( long twnOID, long bldOID, const IFeaturePtr& ipFeature );

	/**
	 * @brief ファイルへの出力
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool output();

private:
	/**
	 * @brief メッシュコードの取得
	 * @param ipPoint [in] 対象ポイント
	 * @return メッシュコード
	 */
	CString getMeshCode( const IPointPtr& ipPoint );

	/**
	 * @brief 住所文字列の取得
	 * @param ipPoint [in] 対象ポイント
	 * @return 住所文字列（11桁住所コードレベル）
	 */
	CString getAddrName( const IPointPtr& ipPoint );

	/**
	 * @brief タウンページポイントから20桁住所コードを取得
	 * @param ipFeature [in] 対象タウンページポイントフィーチャ
	 * @param strAddrCode [out] 20桁住所コード
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool getAddrCodeFromTownpage( const IFeaturePtr& ipFeature, CString& strAddrCode );

private:
	static const long SRC_TYPE_TOWN = sindy::schema::sj::buildingname_common::source_type::kTownPage;	//!< タウンページの出典コード（調査対象ポイントリスト向け）

	std::ofstream m_ofsOutput;				//!< 出力先ファイルストリーム

	std::map< long, CString >		m_mapFields;			//!< フィールドリスト

	const IFeatureClassPtr&			m_ipCityMesh;			//!< citymeshフィーチャクラス
	const IFeatureClassPtr&			m_ipCityAdmin;			//!< city_adminフィーチャクラス
	const std::map<CString, long>&	m_mapCityMeshIndexes;	//!< citymeshテーブルフィールドインデックス情報
	const std::map<CString, long>&	m_mapCityAdminIndexes;	//!< city_adminテーブルフィールドインデックス情報
	const std::map<CString, long>&	m_mapBldgSrcPtIndexes;	//!< buildingname_src_pointテーブルフィールドインデックス情報
	const std::map<CString, long>&	m_mapTownpageIndexes;	//!< townpageテーブルフィールドインデックス情報

	CString	m_sourceName;		//!< TOWNPAGE由来調査対象ポイントのSOURCENAMEフィールドに出力する値
	CString	m_sourceDate;		//!< TOWNPAGE由来調査対象ポイントのSOURCEDATEフィールドに出力する値

	const ADODB::_ConnectionPtr&	m_conAddrDB;			//!< 住所DB接続オブジェクト
};
