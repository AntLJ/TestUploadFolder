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

// LinksCache.h: LinksCache �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Link.h"

namespace sindy
{
typedef std::map<long, Link> LinkMap;

/**
 * @brief �����N�ق��L���b�V���N���X
 */
class LinksCache  
{
public:
// �\�z/����
	LinksCache();
	virtual ~LinksCache();

	void setLinkClass(const IFeatureClassPtr& ipClass);

// ����
	bool init_cache();
	bool add_cache(long nOID, const _IRowPtr& ipRow);
	void clearCache();

// ����
	const Link* findLink(const IPointPtr& ipPoint, double dTolerance) const;
	int findLinkId(const IPointPtr& ipPoint, double dTolerance) const;

// ����
	bool cacheAreaIsContain(const IGeometryPtr& ipGeometry);

// �v���p�e�B
	LinkMap m_cLinkMap; ///< �����N�L���b�V��

	IEnvelopePtr m_ipEnvelope; ///< �L���b�V����`

private:
	IFeatureClassPtr m_ipLinkClass;

	IFeatureClassPtr m_ipCachedLinkClass; ///< �L���b�V���������H�����N�t�B�[�`���N���X
};

}
