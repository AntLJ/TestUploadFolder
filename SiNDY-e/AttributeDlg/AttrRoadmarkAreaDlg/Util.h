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

namespace util
{
	/**
	* @brief  �|�����C�����Z�O�����g�P�ʂŕ��������|�����C���Q�ŕԂ�
	* @param  [in]  polyline ��������|�����C��
	* @return ������̃|�����C���Q
	*/
	std::vector<IPolylinePtr> splitPolyline(IPolylinePtr polyline);

	/**
	* @brief  2�̃|�����C���Ԃ̍ŋߖT�_���擾����
	* @param[in]   polygon    �ŋߖT�����߂�Ώۂ̃|���S��
	* @param[in]   polyline   �ŋߖT�����߂�Ώۂ̃|�����C��
	* @param[out]  nearest_point1  �|���S�����̍ŋߖT�_
	* @param[out]  nearest_point2  �|�����C�����̍ŋߖT�_
	* @retval true   �擾����
	* @retval false  �擾���s
	*/
	bool getNearestPoint(IPolygonPtr polygon, IPolylinePtr polyline, IPointPtr& nearest_point1, IPointPtr& nearest_point2);

	/**
	 * @brief  2�̃|�����C���Ԃ̍ŋߖT�_���擾����
	 * @param[in]   polyline1   �ŋߖT�����߂�Ώۂ̃|�����C��
	 * @param[in]   polyline2   �ŋߖT�����߂�Ώۂ̃|�����C��
	 * @param[out]  nearest_point1	polyline1���̍ŋߖT�_
	 * @param[out]  nearest_point2  polyline2���̍ŋߖT�_
	 * @retval true   �擾����
	 * @retval false  �擾���s
	*/
	bool getNearestPointBetweenPolyline(IPolylinePtr polyline1, IPolylinePtr polyline2, IPointPtr& nearest_point1, IPointPtr& nearest_point2);

	/**
	 * @brief  �C�ӂ̃|�C���g����ɁA���̃|�����C����̈�ԋ߂��|�C���g�A�y�у|�C���g�܂ł̋������擾����
	 * @param[in]  ipBasePoint       ����̃|�C���g
	 * @param[in]  ipTargetPolyline  ��ԋ߂��|�C���g����������鑤�̃|�����C��
	 * @param[out] ipNearestPoint    ��ԋ߂��|�C���g
	 * @param[out] distance          ��ԋ߂��|�C���g�܂ł̋���
	 * @retval true   �擾����
	 * @retval false  �擾���s
	 */
	bool getNearestPointAndDistance(IPointPtr ipBasePoint, IPolylinePtr ipTargetPolyline, IPointPtr& ipNearestPoint, double& distance);
}

