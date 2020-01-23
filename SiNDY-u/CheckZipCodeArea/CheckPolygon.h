#pragma once

#include "CheckBase.h"

class CheckPolygon
	: public CheckBase
{
public:
	CheckPolygon();
	virtual ~CheckPolygon();

	/**
	 * @brief 初期化
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool Init();

	/**
	 * @brief チェック実行
	 * @retval true エラーなし
	 * @retval false エラーあり
	 */
	bool Check();

private:
	/**
	 * @brief 郵便番号が同じポリゴンが隣接していないかをチェック
	 * @param meshGeometry [in] メッシュジオメトリ
	 * @retval true エラーなし
	 * @retval false エラーあり
	 */
	bool CheckPolygonDuplication(const IGeometryPtr& meshGeometry);

private:
	std::list<long> m_meshes; //!< 処理対象メッシュコード格納
	IFeatureClassPtr m_ipZipCodeArea; //!< 郵便番号ポリゴンフィーチャクラス
	ISpatialReferencePtr m_ipSpRef; //!< 郵便番号ポリゴン空間参照(メッシュ形状作成用)
	sindy::CFieldMap m_zipCodeAreaFieldMap; //!< 郵便番号ポリゴンフィールドマップ
};

