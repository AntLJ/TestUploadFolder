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

// KKDLoadProtocol.cpp: KKDLoadProtocol �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include <DRMADevPlant.h>
#include "KKDLoadProtocol.h"

//////////////////////////////////////////////////////////////////////
// �v���g�R���̃I�[�o�[���C�h
//////////////////////////////////////////////////////////////////////

bool KKDLoadProtocol::load(const char* cDirectoryName, int cMeshCode, const char* cSuffix)
{
	return DRMALoadProtocol::load(cDirectoryName, cMeshCode, cSuffix);
}