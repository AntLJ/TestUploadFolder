#pragma once
#include "ConvBase.h"
#include "Define.h"

/**
 * @brief 郵便番号と住所紐付けテーブルのレコード用クラス
 */
class CRelZipAddr {
public:
	bool operator==(const CRelZipAddr& cRelZipAddr) const {
		return strPrefCode == cRelZipAddr.strPrefCode &&
			strCityCode == cRelZipAddr.strCityCode &&
			strOazaCode == cRelZipAddr.strOazaCode &&
			strAzaCode == cRelZipAddr.strAzaCode &&
			strChibanCode == cRelZipAddr.strChibanCode &&
			strJukyoCode == cRelZipAddr.strJukyoCode;
	}
	bool operator<(const CRelZipAddr& cRelZipAddr) const {
		return strPrefCode != cRelZipAddr.strPrefCode ? strPrefCode < cRelZipAddr.strPrefCode :
			strCityCode != cRelZipAddr.strCityCode ? strCityCode < cRelZipAddr.strCityCode :
			strOazaCode != cRelZipAddr.strOazaCode ? strOazaCode < cRelZipAddr.strOazaCode :
			strAzaCode != cRelZipAddr.strAzaCode ? strAzaCode < cRelZipAddr.strAzaCode :
			strChibanCode != cRelZipAddr.strChibanCode ? strChibanCode < cRelZipAddr.strChibanCode :
			strJukyoCode < cRelZipAddr.strJukyoCode;
	}
public:
	CString strPrefCode;
	CString strCityCode;
	CString strOazaCode;
	CString strAzaCode;
	CString strChibanCode;
	CString strJukyoCode;
};

/**
 * @brief 郵便番号と住所紐付けテーブルの変換クラス
 */
class CConvRelZipAddr : public CConvBase
{
public:
	CConvRelZipAddr(const CString& strOutDir, const ITablePtr& ipRelAddr, const CString& strAddr11Dir){
		m_strOutDir = strOutDir;
		m_ipRelAddr = ipRelAddr;
		m_strAddr11Dir = strAddr11Dir;
		m_bFeatureClass = false;
		m_strShapefileName = zip_pol::schema::rel_zip_addr::kTableName;
	}
	~CConvRelZipAddr(){
	}

public:
	
	std::map<CString,std::set<CRelZipAddr>>& getZipAddr()
	{
		return m_mapZipAddr;
	}
	void clearData(){
		m_mapZipAddr.clear();
		m_setAddr11Acc2.clear();
	}

private:
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
	 * @param mapZipAddr [in] データインポート用にに加工したデータ
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool outputData() override;

private:
	/**
	 * @brief 11桁住所データ（addr??.txt）から精度2の11桁住所コード取得
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool loadAddr11Acc2();

private:
	ITablePtr m_ipRelAddr;				//!< 郵便番号に紐付く住所コード（変換元テーブル）
	CString m_strAddr11Dir;				//!< 11桁住所データ格納ディレクトリ
	std::set<CString> m_setAddr11Acc2;	//!< 精度2の11桁住所コード（ADR003相当）
	std::map<CString,long> m_mapFieldIndexRelAddr;	//!< 変換元（SDE,レプリカ）テーブルのフィールドインデックス

	std::map<CString,std::set<CRelZipAddr>> m_mapZipAddr;	//!< 郵便番号と関連する20桁コードのマップ
};

