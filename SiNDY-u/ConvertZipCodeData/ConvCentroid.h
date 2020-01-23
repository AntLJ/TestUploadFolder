#pragma once
#include "ConvBase.h"
#include "Define.h"

/**
 * @brief 重心ポイント変換用クラス
 */
class CConvCentroid : public CConvBase
{
public:
	CConvCentroid(const CString& strOutDir){
		m_strOutDir = strOutDir;
		m_bFeatureClass = true;
		m_strShapefileName = zip_pol::schema::zip_centroid::kTableName;
	}
	~CConvCentroid(){
		m_mapMergedZipGeom.clear();
		m_mapZipCentorid.clear();
	}

public:

	/**
	 * @brief マージ後形状設定
	 */
	void setMergedZipGeom(const std::map<CString,IGeometryPtr>& mapMergedZipGeom)
	{
		m_mapMergedZipGeom = mapMergedZipGeom;
	}
	void clearData(){
		m_mapMergedZipGeom.clear();
		m_mapZipCentorid.clear();
	}

private:
	/**
	 * @brief 特殊変換（仮想関数）
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool convSpecial() override;

	/** 
	 * @brief フィールドインデックス作成
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool createFieldIndex() override;

	/** 
	 * @brief 出力先Shapefileのフィールド設定
	 * @param ipFields [out] フィールド群
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool createFields(IFieldsPtr& ipFields) override;

	/**
	 * @brief 変換元テーブルからデータ読み込み
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool loadData() override;

	/**
	 * @brief Shapefileへデータ出力
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool outputData() override;

private:
	/**
	 * @brief 重心ポイント作成
	 * @param mapZipGeom [out] 読み込んだデータを郵便番号とOID+形状でマップしたもの
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool createCentroid(const std::map<CString,IGeometryPtr>& mapMergedZipGeom);

private:
	std::map<CString,IGeometryPtr> m_mapMergedZipGeom;	//!< 郵便番号とマージされた形状のマップ
	std::map<CString,IPointPtr> m_mapZipCentorid;		//!< 郵便番号と重心ポイントのマップ
};
