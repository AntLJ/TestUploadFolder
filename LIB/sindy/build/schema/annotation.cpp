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
#include <sindy/schema/annotation.h>

namespace sindy {

namespace schema {

namespace annotation {
	const _TCHAR kNameString1[]		= _T("NAME_STRING1");		//!< ���L������Q
	const _TCHAR kNameString2[]		= _T("NAME_STRING2");		//!< ���L������Q
	const _TCHAR kNameString3[]		= _T("NAME_STRING3");		//!< ���L������R
	const _TCHAR kStringNum1[]		= _T("STRING_NUM1");		//!< ���L�P������
	const _TCHAR kStringNum2[]		= _T("STRING_NUM2");		//!< ���L�Q������
	const _TCHAR kStringNum3[]		= _T("STRING_NUM3");		//!< ���L�R������
	const _TCHAR kAnnoCode[]		= _T("ANNOCLASS_C");		//!< ���L���
	const _TCHAR kNameYomi[]		= _T("NAME_YOMI");			//!< ���L�ǂ�
	const _TCHAR kDispType[]		= _T("DISPTYPE_C");			//!< �\���^�C�v�i�s�s�n�}�p�j
	const _TCHAR kSC1DispType[]		= _T("SC1DISPTYPE_C");		//!< �\���^�C�v�i�X�P�[��1�j
	const _TCHAR kSC2DispType[]		= _T("SC2DISPTYPE_C");		//!< �\���^�C�v�i�X�P�[��2�j
	const _TCHAR kSC3DispType[]		= _T("SC3DISPTYPE_C");		//!< �\���^�C�v�i�X�P�[��3�j
	const _TCHAR kSC4DispType[]		= _T("SC4DISPTYPE_C");		//!< �\���^�C�v�i�X�P�[��4�j
	const _TCHAR kContentsCode[]	= _T("CONTENTS_CODE");		//!< �R���e���c�R�[�h
	const _TCHAR kContentsSeq[]		= _T("CONTENTS_SEQ");		//!< �R���e���c���V�[�P���X
	const _TCHAR kAnnoDisp[]		= _T("ANNO_DISP_C");		//!< ���L�\���R�[�h
} // annotation

namespace annotation_rel_poi {
	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< �R���e���c�R�[�h
	const _TCHAR kContentsSeq[]  = _T("CONTENTS_SEQ"); //!< �R���e���c���V�[�P���X
	const _TCHAR kAnnoDispCode[] = _T("ANNO_DISP_C"); //!< ���L�\���R�[�h
} // annotation_rel_poi

namespace disp_line {
	const _TCHAR kFontSize[]		= _T("FONTSIZE");			//!< �t�H���g�T�C�Y
	const _TCHAR kAnnoID[]			= _T("ANNO_ID");			//!< �Ή����L����ID
	const _TCHAR kTargetStr[]		= _T("TARGET_STR_C");		//!< �Ώە�����R�[�h
} // disp_line

const _TCHAR city_annotation::kTableName[]		= _T("CITY_ANNOTATION");	//!< �e�[�u����
const _TCHAR d1_annotation::kTableName[]		= _T("D1_ANNOTATION");		//!< �e�[�u����
const _TCHAR base_annotation::kTableName[]		= _T("BASE_ANNOTATION");	//!< �e�[�u����
const _TCHAR middle_annotation::kTableName[]	= _T("MIDDLE_ANNOTATION");	//!< �e�[�u����
const _TCHAR top_annotation::kTableName[]		= _T("TOP_ANNOTATION");		//!< �e�[�u����
const _TCHAR top_annotation_jpn::kTableName[]		= _T("TOP_ANNOTATION_JPN");	//!< �e�[�u����
const _TCHAR city_disp_line::kTableName[]		= _T("CITY_DISP_LINE");
const _TCHAR c_sc1disp_line::kTableName[]		= _T("C_SC1DISP_LINE");
const _TCHAR b_sc1disp_line::kTableName[]		= _T("B_SC1DISP_LINE");
const _TCHAR b_sc2disp_line::kTableName[]		= _T("B_SC2DISP_LINE");
const _TCHAR b_sc3disp_line::kTableName[]		= _T("B_SC3DISP_LINE");
const _TCHAR b_sc4disp_line::kTableName[]		= _T("B_SC4DISP_LINE");
const _TCHAR m_sc1disp_line::kTableName[]		= _T("M_SC1DISP_LINE");
const _TCHAR m_sc2disp_line::kTableName[]		= _T("M_SC2DISP_LINE");
const _TCHAR m_sc3disp_line::kTableName[]		= _T("M_SC3DISP_LINE");
const _TCHAR m_sc4disp_line::kTableName[]		= _T("M_SC4DISP_LINE");
const _TCHAR t_sc1disp_line::kTableName[]		= _T("T_SC1DISP_LINE");
const _TCHAR t_sc2disp_line::kTableName[]		= _T("T_SC2DISP_LINE");
const _TCHAR t_sc3disp_line::kTableName[]		= _T("T_SC3DISP_LINE");
const _TCHAR t_sc4disp_line::kTableName[]		= _T("T_SC4DISP_LINE");
const _TCHAR t_sc1disp_line_jpn::kTableName[]		= _T("T_SC1DISP_LINE_JPN");
const _TCHAR t_sc2disp_line_jpn::kTableName[]		= _T("T_SC2DISP_LINE_JPN");
const _TCHAR t_sc3disp_line_jpn::kTableName[]		= _T("T_SC3DISP_LINE_JPN");
const _TCHAR t_sc4disp_line_jpn::kTableName[]		= _T("T_SC4DISP_LINE_JPN");

} // schema

} // sindy
