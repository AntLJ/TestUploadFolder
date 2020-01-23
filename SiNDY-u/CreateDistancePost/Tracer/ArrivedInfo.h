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

// ArrivedInfo.h: ArrivedInfo �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Route.h"

namespace sindy
{
class ArrivedInfo  
{
public:
	typedef std::map<const LinkBranch*, Route> LinkKeyRouteMap;

// �\�z/����
	ArrivedInfo();
	virtual ~ArrivedInfo();

// ����
	void clear();

// �ݒ�
	Route* setRoute(const Route &rRoute);

// �擾����
	int countRoute() const;
	Route* bestRouteOfAll();
	const Route* bestRouteOfAll() const;
	Route* bestRouteOfReg();
	const Route* bestRouteOfReg() const;
	Route* bestRouteOfYetDepartured();

// ����
	bool isNotArrived() const;
	bool isBetterThanAll(const Route& rRoute) const;

private:
// �v���p�e�B
	Route m_cRoute; ///< �����܂ł̃��[�g
	LinkKeyRouteMap m_cUturnRouteMap; ///< �t�^�[���������ȃ��[�g
};

}
