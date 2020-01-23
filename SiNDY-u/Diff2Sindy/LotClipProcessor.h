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

#include "MeshClipProcessor.h"

/**
 * @brief �N���b�s���O���s�N���X
 */
template <typename T>
class LotClipProcessor : public MeshClipProcessor<T>
{
public:

	/// �R���X�g���N�^
	LotClipProcessor(const sindy::CWorkspace& workspace, const ClippingDefinitionManager& m_ClipDef, const std::vector<uh::tstring>& meshs);

	/// �f�X�g���N�^
	virtual ~LotClipProcessor(){};

	/// �N���b�v���������{����
	bool doClip( sindy::CTableContainer& con, IEnvelopePtr mesh ) override;
private:
	/**
	* @brief ���b�g�|���S��������������
	* @param spref [in] ��ԎQ��
	*/
	void initLotPolygon( ISpatialReferencePtr spref );

	IGeometryPtr m_ipLotPolygon; //!< ���b�g�|���S��
	std::vector<uh::tstring> m_vecMesh; //!< ���b�V�����X�g
};
