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
#include "OutputLog.h"

// TODO: ���̂΂炵�Ċ���ꂻ���Ȃ���

class POICheckFunction
{
public:
	POICheckFunction(COutPut& out, double refer_id_threshold):
		m_output(out)
		, m_refer_id_threshold(refer_id_threshold)
	{}

	/**
	 * @brief	POI�|�C���g�̃`�F�b�N
	 * @param	meshcode	[in]	�`�F�b�N�Ώۃ��b�V���R�[�h
	 * @param	ipMeshGeo	[in]	�`�F�b�N�Ώۂ̃W�I���g��
	 * @param	poiPoint   	[in]	�`�F�b�N�Ώ�POI�|�C���g�Q�N���X
	 * @param	walkNode   	[in]	�`�F�b�N�Q�Ɨp���s�҃m�[�h�Q�N���X
	 * @param	walkNodeRTree	[in]	�`�F�b�N�Q�Ɨp���s�҃m�[�hRTree
	 */
	void CheckPoiPoint( 
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo, 
		const FeaturesPack& poiPoint,
		const FeaturesPack& walkNode,
		const BGRTree& walkNodeRTree
		);

protected:
	COutPut& m_output; //!< ���O�o�͗p�C���X�^���X�ւ̎Q��
	double m_refer_id_threshold; //!< �֘AID����臒l(m)
};
