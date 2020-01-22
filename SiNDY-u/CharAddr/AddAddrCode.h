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

#include <oracl.h>

#define CON_STR_SIZE		100		///< �ڑ�������̃T�C�Y

#define ALL_KANJI_MAX_LEN	93		///< �������́i�����j�̍ő咷
#define KEN_KANJI_MAX_LEN	9		///< �������́i�s���{���j�̍ő咷
#define SHI_KANJI_MAX_LEN	25		///< �������́i�s�撬���j�̍ő咷
#define OAZA_KANJI_MAX_LEN	37		///< �������́i�厚�j�̍ő咷
#define AZA_KANJI_MAX_LEN	25		///< �������́i���E���ځj�̍ő咷

#define ALL_KANA_MAX_LEN	185		///< �S�p�J�i���́i�����j�̍ő咷
#define KEN_KANA_MAX_LEN	17		///< �S�p�J�i���́i�s���{���j�̍ő咷
#define SHI_KANA_MAX_LEN	49		///< �S�p�J�i���́i�s�撬���j�̍ő咷
#define OAZA_KANA_MAX_LEN	73		///< �S�p�J�i���́i�厚�j�̍ő咷
#define AZA_KANA_MAX_LEN	49		///< �S�p�J�i���́i���E���ځj�̍ő咷

void CheckLog( const CString& strLogPath );

bool Connect( const CString& strConnect );

/// �ڑ�����������[�U���^�p�X���[�h�^�T�[�r�X���ɕ���
bool fnAnalyzeConStr( const CString& strConnect, char *cUser, char *cPass, char *cService);

/// �Z���R�[�h�i�Q�E�T�E�W�E11���j���������̂ɕϊ�
/**
 * @param cCode [in] �Z���R�[�h�i2/5/8/11���j
 * @param cKanji [out] �Z���R�[�h�ɑΉ����銿�����́i�����C�S�p�X�y�[�X�����ρj
 * @retval true ����
 * @retval false ���s
 */
bool CodeToKanji(const char *cCode, char *cKanji);

/// �Z���R�[�h���K�w���Ƃɕ���
/**
 * @param cCode [in] �Z���R�[�h�i5/8/11/16/20/22���j
 * @param cKen [out] �s���{���R�[�h
 * @param cShi [out] �s�撬���R�[�h
 * @param cOaza [out] �厚�R�[�h
 * @param cAza [out] ���E���ڃR�[�h
 * @param cChiban [out] �n�ԃR�[�h
 * @param cBango [out] �ԍ��R�[�h
 * @param cKatagaki [out] �����R�[�h
 * @retval true ����
 * @retval false ���s
 */
bool PartitionCode( const char *cCode, char *cKen, char *cShi, char *cOaza = NULL, char *cAza = NULL, char *cChiban	= NULL, char *cBango = NULL, char *cKatagaki = NULL );

