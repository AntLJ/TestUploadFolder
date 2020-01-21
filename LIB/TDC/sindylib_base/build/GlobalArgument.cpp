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

/**
 * @file �O���[�o���ϐ����̃t�@�C��
 * @brief DLL���ŃO���[�o���Ɏg�p����ϐ��̎��̂��`���܂��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#include "stdafx.h"
#include "GlobalRules.h"
#include "GlobalFiles.h"
#include "GeometryRule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
	// ���[���p
	CLogicRule	g_cLogicRule;									// �_���`�F�b�N�p���[���Ǘ��N���X
	CSPRuleWrapper g_cRule;										// SiNDY�ҏW���[���Ǘ��N���X

	bool g_bCheckUpdatedByArcObjects = true;					// CRow::Store()���ɃL���b�V���쐬�O���l�����ĕύX���m�F���邩�ǂ����iSiNDY-e�ł͎g�p�֎~�I�I�j
	bool g_bSetHistory = true;									// CRow::Store()���ɗ������Z�b�g���邩�ǂ���
#ifdef _DEBUG
	bool g_bTraceUpdate = true;									// CRow::Store()���ɕۑ��󋵂̃g���[�X���o�����ǂ���
#endif // ifdef _DEBUG
	bool g_bOutputTraceUpdate = false;							// CRow::Store()���̕ۑ��󋵃g���[�X��TRACEMESSAGE�ŏo�͂��邩�ǂ���
	bool g_bCreateAllStoreInfo = true;							// CRow::Store()���ɑS�ẴG���[�I�u�W�F�N�g�𐶐����邩
	bool g_bStoreContainer = true;								// CRow::Store()����CContainer�̒���Store���邩�ǂ���
	bool g_bCalcStoreTime = false;								// CRow::Store()����IRow::Store()���Ԃ��v�����邩�ǂ���
	DWORD g_dwTotalStoreTime = 0;								// CRow::Store()����IRow::Store()���Ԃ̐ώZ
	long g_lTotalStoreCount = 0;								// CRow::Store()����IRow::Store()�񐔂̐ώZ
	double g_dTotalCacheTime = 0.0;								// CRow::CreateCache�ɂ����鎞�Ԃ̑��v

	bool g_bBufferedSearch = true;								// CAnnotations::Select�n�őI��R�ꂪ�Ȃ��悤�Ɍ����`����o�b�t�@�����O���Č������邩�ǂ���
	bool g_bDisableCheckToNoSubstance = false;					// ���̂������Ȃ����̂��`�F�b�N�Ώۂ��珜�O����
} // sindy
