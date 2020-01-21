#pragma once
#include "TDC/RepPointCalculator/Define.h"

/**
 * @brief �W�I���g����\�_�����߂�
 * @param [in] ipGeometry �W�I���g��
 * @param [in] mode ��\�_�v�Z���[�h
 * @retval ��\�_�|�C���g(IPointPtr)
 */
IPointPtr AheGetRepPoint(const IGeometryPtr& ipGeometry, rpc::CalcMode mode);

/**
 * @brief �W�I���g����\�_�����߂�
 * @param [in] ipGeometry �W�I���g��
 * @param [in] mode ��\�_�v�Z���[�h
 * @retval ��\�_�|�C���g(WKSPoint)
 */
WKSPoint AheGetRepWKSPoint(const IGeometryPtr& ipGeometry, rpc::CalcMode mode);
