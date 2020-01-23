#pragma once
#include "ConvBase.h"
#include "ConvRelZipAddr.h"
#include "Define.h"

/**
 * @brief 市区町村代表ポイント変換用クラス
 */
class CConvRep : public CConvBase
{
public:
	CConvRep(const CString& strOutDir, const ITablePtr& ipRelAddr, const CString& strAddrDB, const CADOBase& cADOBase){
		m_strOutDir = strOutDir;
		m_ipRelAddr = ipRelAddr;
		m_strAddrDB = strAddrDB;
		m_cADOBase = &cADOBase;
		m_bFeatureClass = true;
		m_strShapefileName = zip_pol::schema::zip_rep::kTableName;
	}
	~CConvRep(){
		m_mapFieldIndexRelAddr.clear();
		m_mapZipRep.clear();
	}
public:

	void clearData(){
		m_mapZipRep.clear();
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
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool outputData() override;

private:
	CString m_strAddrDB;			//!< 住所DB接続情報
	const CADOBase* m_cADOBase;		//!< 住所DBコネクション
	ITablePtr m_ipRelAddr;			//!< 郵便番号に紐付く住所コード（変換元テーブル）
	std::map<CString,long> m_mapFieldIndexRelAddr;	//!< 変換元（SDE,レプリカ）テーブルのフィールドインデックス

	std::map<CString,std::map<std::pair<CString,CString>,IPointPtr>> m_mapZipRep;	//!< 郵便番号と市区町村代表ポイントのマップ
};
