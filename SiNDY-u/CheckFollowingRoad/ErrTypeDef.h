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

namespace err_type
{

/**
 * @brief	�G���[�̎��
 */
enum error_type
{
	kChangeFollow = 101, //!< ���Ȃ蓹�H���ҏW�O��ŕω�
	kChangeLink = 102, //!< �ڑ����Ă��铹�H�����N���ҏW�O��ŕω�
	kIncreaseLink = 103, //!< ���H�����N���ҏW�O��ő���
	kDecreaseLink = 104, //!< ���H�����N���ҏW�O��Ō���
	kBaseUnspecified = 105, //!< ��r���o�[�W�����œ��Ȃ蓹�H�����s��
	kCompUnspecified = 106, //!< ��r��o�[�W�����œ��Ȃ蓹�H�����s��
	kDiffAngle = 199 //!< [Debug]���Ȃ蓹�H�̊p�x���ҏW�O���20���ȏ�ω�
};

} // err_type
