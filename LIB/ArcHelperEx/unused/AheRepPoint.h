#pragma once
#include "TDC/RepPointCalculator/Define.h"

/**
 * @brief ジオメトリ代表点を求める
 * @param [in] ipGeometry ジオメトリ
 * @param [in] mode 代表点計算モード
 * @retval 代表点ポイント(IPointPtr)
 */
IPointPtr AheGetRepPoint(const IGeometryPtr& ipGeometry, rpc::CalcMode mode);

/**
 * @brief ジオメトリ代表点を求める
 * @param [in] ipGeometry ジオメトリ
 * @param [in] mode 代表点計算モード
 * @retval 代表点ポイント(WKSPoint)
 */
WKSPoint AheGetRepWKSPoint(const IGeometryPtr& ipGeometry, rpc::CalcMode mode);
