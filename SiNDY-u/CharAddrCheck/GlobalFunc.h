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

/**
* @brief	�t�B�[�`���N���X����Where����w�肵��OID�Ǝw��t�B�[���h�̒l���擾
* @param	strWhere[in]				Where��
* @param	strField[in]				�w��t�B�[���h
* @param	ipFeatureClass[in]			�w��t�B�[�`���N���X
* @param	lType[in]					�����^�C�v�i0:���|�C���g, ����ȊO:�s���E�j
* @retval	OID���L�[�ɂ����w��t�B�[���h�̒l�̃}�b�v
*/
std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> GetItems( LPCTSTR lpcszWhere, LPCTSTR lpcszField, IFeatureClass* ipFeatureClass, long lType );

/**
* @brief	�ǂݕϊ���̃`�F�b�N���X�g����Where����w�肵��OID�Ǝw��t�B�[���h�̒l���擾
* @param	strWhere[in]				Where��
* @param	strField[in]				�w��t�B�[���h
* @param	ipFeatureClass[in]			�w��t�B�[�`���N���X
* @param	lpcszPath[in]				�ǂݕϊ���̃`�F�b�N���X�g
* @param	lType[in]					�����^�C�v�i0:���|�C���g, ����ȊO:�s���E�j
* @retval	OID���L�[�ɂ����w��t�B�[���h�̒l�̃}�b�v
*/
std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> GetItems2( LPCTSTR lpcszWhere, LPCTSTR lpcszField, IFeatureClass* ipFeatureClass, LPCTSTR lpcszListPath, long lType );

bool CodeToKanji( LPCTSTR lpcszCode, char *lpszKanji, const ODatabase& eOraDB );
bool PartitionCode( LPCTSTR lpcszCode, LPTSTR lpszKen, LPTSTR lpszShi, LPTSTR lpszOaza, LPTSTR lpszAza );

/**
* @brief	�t�@�C�����ݒ�i�����t�@�C�����������悤�Ɂ@001�`999�܂Łj
* @param	lpcszOutPut[in]				�o�̓f�B���N�g��
* @param	lpcszFile[in]				�t�@�C�����i�g���q��txt�ŌŒ�j
* @param	lCount[in]					�t�@�C���ԍ�
* @retval	�o�̓t�@�C�����i�t���p�X�j
*/
CString CreateLogFile( LPCTSTR lpcszOutput, LPCTSTR lpcszFile, long lCount );
