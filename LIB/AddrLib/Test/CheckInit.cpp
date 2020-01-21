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

#include "stdafx.h"
#include "CheckInit.h"

#include <arctl/coinitializer.h>
#include <AddrLib/Initializer.h>

std::mutex CheckInit::s_mutex;

CheckInit::CheckInit()
{
	const arctl::coinitializer coInitializer;

	// �Z���}�X�^��Release���[�U(���̎��_�ōŐV�̃��W���[�Z���}�X�^)����
	// �擾����̂ŁA������Z�����̂��ς�邱�Ƃ��l������B
	// �e�X�g�Ɏ��s�����ꍇ�́A���̕ӂ���`�F�b�N���Ă݂邱�ƁB
	addr::Init_AreaCodeHelper();
	addr::Init_CodeConverter(verificationDatDir + _T("CodeConvTable.mdb"));
	addr::Init_YomiConverter(verificationDatDir + _T("YomiTable.mdb"));
}

CheckInit& CheckInit::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_mutex);

	static CheckInit instance;
	return instance;
};
