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

// SindyBaseAdmin.cpp: SindyBaseAdmin �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "SindyBaseAdmin.h"


SindyBaseAdmin::SindyBaseAdmin(SindyDBConnection *cDBConnection) :
SindyPolygonClass(cDBConnection, sindy::layer::BASE_ADMIN)
{

}

SindyBaseAdmin::~SindyBaseAdmin()
{

}
