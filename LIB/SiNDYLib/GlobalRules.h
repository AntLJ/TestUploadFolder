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
 * @file �O���[�o�����[���ϐ���`�t�@�C��
 * @brief DLL���ŃO���[�o���Ɏg�p����ϐ����`���܂��B
 * �f�[�^�Z�O�����g�Œ�`���Ă���킯�ł͂Ȃ��̂ł�����
 * ��`���ꂽ�ϐ��̓��e�͑S�ăv���Z�X�ʂɂȂ�܂��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _GLOBALARGUMENT_H_
#define _GLOBALARGUMENT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"
#include "TableType.h"

namespace sindy {
	class CRule;
	class CLogicRule;
	// ���[���p
	extern SINDYLIB_API CRule			g_cRule;		//!< ���[���Ǘ��N���X
	extern SINDYLIB_API CLogicRule		g_cLogicRule;	//!< �_���`�F�b�N�p���[���Ǘ��N���X

	// CRow::Store����p
	extern SINDYLIB_API bool	g_bCheckUpdatedByArcObjects;	//!< CRow::Store()���ɃL���b�V���쐬�O���l�����ĕύX���m�F���邩�ǂ����iSiNDY-e�ł͎g�p�֎~�I�I�j
	extern SINDYLIB_API bool	g_bSetHistory;					//!< CRow::Store()���ɗ������Z�b�g���邩�ǂ���
#ifdef _DEBUG
	extern SINDYLIB_API bool	g_bTraceUpdate;					//!< CRow::Store()���ɕۑ��󋵂̃g���[�X���o�����ǂ���
#endif // ifdef _DEBUG
	extern SINDYLIB_API bool	g_bOutputTraceUpdate;			//!< CRow::Store()���̕ۑ��󋵃g���[�X��TRACEMESSAGE�ŏo�͂��邩�ǂ���
	extern SINDYLIB_API bool	g_bCreateAllStoreInfo;			//!< CRow::Store()���ɑS�ẴG���[�I�u�W�F�N�g�𐶐����邩
	extern SINDYLIB_API bool	g_bStoreContainer;				//!< CRow::Store()����CContainer�̒���Store���邩�ǂ���
	extern SINDYLIB_API bool	g_bCalcStoreTime;				//!< CRow::Store()����IRow::Store()���Ԃ��v�����邩�ǂ���
	extern SINDYLIB_API DWORD	g_dwTotalStoreTime;				//!< CRow::Store()����IRow::Store()���Ԃ̐ώZ
	extern SINDYLIB_API long	g_lTotalStoreCount;				//!< CRow::Store()����IRow::Store()�񐔂̐ώZ

	// CAnnotations�ł̌�������p
	extern SINDYLIB_API bool	g_bBufferedSearch;				//!< CAnnotations::Select�n�őI��R�ꂪ�Ȃ��悤�Ɍ����`����o�b�t�@�����O���Č������邩�ǂ���
	extern SINDYLIB_API sindyTableType::ECode g_emViewTableType;//! CAnnotations::_SelectByShape�ŕs�v�Ȃ��̂������ۂɕ\���X�P�[�������肷��ۂɎg�p����

	// �`�F�b�N�n����p
	extern SINDYLIB_API bool	g_bDisableCheckToNoSubstance;	//!< ���̂������Ȃ����̂��`�F�b�N�Ώۂ��珜�O����
} // sindy

#endif // _GLOBALARGUMENT_H_
