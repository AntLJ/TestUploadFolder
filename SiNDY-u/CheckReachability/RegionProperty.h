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

#include <string>

/**
 * @brief ���[�W�����I�u�W�F�N�g���ʏ��
 */
struct RegionProperty
{
	std::wstring m_strRoadLinkWhereClause; ///< ���H�����N����Where��B
	IAreaPtr m_ipInnerArea; ///< ���[�W�����͈́B
	IAreaPtr m_ipOuterArea; ///< �I�[�o�[���b�v�͈́B
};
