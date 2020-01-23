#pragma once
#include "ConvBase.h"
#include "Define.h"

/**
 * @brief 郵便番号ポリゴン変換クラス
 */
class CConvZipPol : public CConvBase
{
public:
	CConvZipPol(const CString& strOutDir, const IFeatureClassPtr ipZipAreaClass){
		m_strOutDir = strOutDir;
		m_ipZipAreaClass = ipZipAreaClass;
		m_bFeatureClass = true;
		m_strShapefileName = zip_pol::schema::zip_pol::kTableName;
	};
	~CConvZipPol(){
	};

public:

	/**
	 * @brief マージ後形状取得
	 * @return マージ後形状
	 */
	std::map<CString,IGeometryPtr>& getMergedZipGeom()
	{
		return m_mapMergedZipGeom;
	}
	void clearData(){
		m_mapZipGeom.clear();
		m_mapMergedZipGeom.clear();
		m_mapZipPriority.clear();
	}

private:
	/**
	 * @brief 特殊変換
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
	 * @brief 複数形状で構成される郵便番号のマージ処理
	 * @param mapZipGeom [in] 郵便番号とOID+形状のマップ
	 * @param mapMergedZipGeom [out] 郵便番号とマージされた形状のマップ
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool mergeZipGeom(std::map<CString,std::map<long,IGeometryPtr>>& mapZipGeom, std::map<CString,IGeometryPtr>& mapMergedZipGeom);

	/**
	 * @brief 郵便番号に対する優先度設定されたリスト取得
	 * @param mapZipPriority [out] 郵便番号と優先度のマップ
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool createZipCodePriority(std::map<CString,long>& mapZipPriority);

	/**
	 * @brief 優先度設定用のリスト取得
	 * @param mapZipPriority [out] 郵便番号と優先度のマップ
	 * @param mapOverlapZip [out] 重複している郵便番号のマップ（キーが優先度高、要素が優先度低の郵便番号）
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool getZipCodeList(std::map<CString,long>& mapZipPriority, std::map<CString,std::set<CString>>& mapOverlapZip);

	/**
	 * @brief 郵便番号に対する優先度の設定
	 * @param mapZipPriority [in,out] 郵便番号と優先度のマップ
	 * @param mapOverlapZip [in] 重複している郵便番号のマップ（キーが優先度高、要素が優先度低の郵便番号）
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool updatePriority(std::map<CString,long>& mapZipPriority, const std::map<CString,std::set<CString>>& mapOverlapZip);

	/**
	 * @brief 郵便番号に対する優先度の設定（再帰）
	 * @param mapZipPriority [in,out] 郵便番号と優先度のマップ
	 * @param mapOverlapZip [in] 重複している郵便番号のマップ（キーが優先度高、要素が優先度低の郵便番号）
	 * @param strZip [in] チェック対象の郵便番号
	 * @param setZip [in] strZipをキーとしたmapOverlapZipの要素
	 * @param setCheckedZip [in,out] 既にチェック済みの郵便番号（循環監視用）
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool setPriority(std::map<CString,long>& mapZipPriority, const std::map<CString,std::set<CString>>& mapOverlapZip, const CString &strZip, const std::set<CString>& setZip, std::set<CString> setCheckedZip);

private:
	IFeatureClassPtr m_ipZipAreaClass;				//!< 変換元の郵便番号ポリゴン
	std::map<CString,long> m_mapFieldIndexZipArea;	//!< 変換元（SDE,レプリカ）テーブルのフィールドインデックス

	std::map<CString,std::map<long,IGeometryPtr>> m_mapZipGeom;	//!< 郵便番号とOID＋形状のマップ
	std::map<CString,IGeometryPtr> m_mapMergedZipGeom;			//!< 郵便番号とマージされた形状のマップ
	std::map<CString,long> m_mapZipPriority;		//!< 郵便番号と優先度のマップ
};
