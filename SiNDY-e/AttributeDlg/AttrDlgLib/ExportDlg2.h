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

//#include "ExportDlg.h"
class CExportDlg;

/**
 * @brief �g���ł̑����_�C�A���O�C���^�[�t�F�[�X
 * @note  �����_�C�A���O���Ŋ֘A�e�[�u�����̍X�V�������链�Ȃ��ꍇ��<br>
 *        �����_�C�A���O�N���X�ł�������p������
 */
class CExportDlg2 : public CExportDlg
{
public:
	virtual bool Update() = 0;
};

