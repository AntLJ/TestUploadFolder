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


class CCheckRelPointPolygon
{
public:

	/**
	 * @brief ポイントとポリゴンの関係をチェックする
	 * @param args	[in]	コマンドライン引数
	 * @retval true	処理成功
	 * @retval false	処理失敗
	 */
	bool Check(const Arguments& args);

	/**
	 * @brief 初期化
	 * @param args	[in]	コマンドライン引数
	 * @retval true	成功
	 * @retval false	失敗
	 */
	bool Init(const Arguments& args);

	/**
	 * @brief ワークスペース取得
	 * @param ipPointWork	[out]	ポイントの接続先
	 * @param ipPolyWork	[out]	ポリゴンの接続先
	 * @retval true 取得成功
	 * @retval false 取得失敗
	 */
	bool GetWorkspace(IWorkspacePtr& ipPointWork, IWorkspacePtr& ipPolyWork);

	/**
	 * @brief フィーチャクラス取得
	 * @param ipPointWork	[in]	ポイントの接続先
	 * @param ipPolyWork	[in]	ポリゴンの接続先
	 * @param ipPointFC	[out]	ポイントのフィーチャクラス
	 * @param ipPolyFC	[out]	ポリゴンのフィーチャクラス
	 * @retval true 取得成功
	 * @retval false 取得失敗
	 */
	bool GetFeatureClass(IWorkspacePtr ipPointWork, IWorkspacePtr ipPolyWork, IFeatureClassPtr& ipPointFC, IFeatureClassPtr& ipPolyFC);

	/**
	 * @brief 指定されたフィーチャが指定されたフィーチャクラスに重なっているかをチェックする
	 * @param ipFeature	[in]	対象のオブジェクト
	 * @param ipFeatureClass	[in]	接触しているか調べるフィーチャ
	 * @param strErrMsg	[in]	エラーメッセージ
	 * @param bIsMyself	[in]	対象のオブジェクトと接触判定を見るフィーチャが同じフィーチャかどうか
	 */
	void CheckIntersects(IFeaturePtr ipFeature, IFeatureClassPtr ipFeatureClass, const CString& strErrMsg, bool bIsMyself = false);

	/**
	 * @brief 開始時のログを書きこむ
	 * @param args	[in]	コマンドライン引数
	 */
	void WriteStartLog(const Arguments& args);

	/**
	 * @brief エラーログを書きこむ
	 * @param strObject	[in]	オブジェクト名
	 * @param OID	[in]	オブジェクトID
	 * @param strErrMsg	[in]	エラーメッセージ
	 */
	void WriteErrLog(const CString& strObject, long OID, const CString& strErrMsg);

	/**
	 * @brief 終了時のログを書きこむ
	 * @param success	[in]	処理成功時はtrue、処理失敗時はfalse
	 */
	void WriteEndLog(bool success = true);

private:
	uh::tstring m_PointConnect;  //!< ポイントの接続先
	uh::tstring m_PolyConnect;   //!< ポリゴンの接続先
	uh::tstring m_Point;         //!< 設定ファイルのPOINTで指定されたフィーチャ名
	uh::tstring m_Poly;          //!< 設定ファイルのPOLYGONで指定されたフィーチャ名
	std::ofstream m_RunLog;      //!< 実行ログ
	std::ofstream m_ErrLog;      //!< エラーログ
	CString m_PointSubF;        //!< ポイントのSubFields
	CString m_PointWhere;       //!< ポイントのWHERE句
	CString m_PolySubF;         //!< ポリゴンのSubFields
	CString m_PolyWhere;        //!< ポリゴンのWHERE句
};
