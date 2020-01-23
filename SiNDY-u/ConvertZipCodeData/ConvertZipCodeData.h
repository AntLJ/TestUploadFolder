#pragma once
#include "Arguments.h"

/**
 * @brief 郵便番号データの変換元クラス
 */
class CConvertZipCodeData
{
public:
	CConvertZipCodeData(){}
	~CConvertZipCodeData(){
	}

public:
	/**
	 * @brief 初期化
	 * @param args [in] コマンドライン引数
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool init(const Arguments& args);

	/**
	 * @brief 処理実行
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool run();

private:
	CString m_strOutDir;				//!< 出力先ディレクトリ
	IFeatureClassPtr m_ipZipAreaClass;	//!< 変換元の郵便番号ポリゴンクラス
	ITablePtr m_ipRelAddr;				//!< 変換元の郵便番号と住所紐付けテーブル
	CString m_strAddrDB;				//!< 住所DB接続情報
	CString m_strAddr11Dir;				//!< 11住所データ格納先
	CADOBase m_cADOBase;				//!< 住所DBコネクション
};

