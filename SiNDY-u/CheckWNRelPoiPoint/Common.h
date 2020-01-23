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

#include "stdafx.h"

#define LOG_ERROR	"ERROR"

#define MWN_LAYER	_T("MWN")
#define SQL_NODECLS _T("wkndcls_c = ")

typedef std::map<long, long> MAP_REL_TBL;			// �֘A�e�[�u���}�b�v�i�L�[�FNODE_ID�A�v�f�FPOI_ID�j
typedef std::map<long, WKSPoint> MAP_POI_DATA;		// POI�f�[�^�}�b�v�i�L�[�F�Ǘ��R�[�h�A�v�f�F�o�x�E�ܓx�j

namespace nsCommon
{
	/**
	 * @brief	���X�g�ǂݍ���(vector)
	 * @param	file		[in]	�t�@�C���p�X
	 * @param	vecList		[out]	�R���e�i
	 * @param	sErrMsg		[out]	�G���[���b�Z�[�W
	 * @return						��������
	 */
	bool loadList2vec(const std::string& file, std::vector<std::string>& vecList, std::string& sErrMsg);

	/**
	 * @brief	���X�g�ǂݍ���(set)
	 * @param	file		[in]	�t�@�C���p�X
	 * @param	setList		[out]	�R���e�i
	 * @param	sErrMsg		[out]	�G���[���b�Z�[�W
	 * @return						��������
	 */
	bool loadList2set(const std::string& file, std::set<std::string>& setList, std::string& sErrMsg);

	/**
	 * @breif	�t�B�[���h�C���f�b�N�X�̎擾
	 * @param	ipTable		[in]	�擾���e�[�u��
	 * @param	csField		[in]	�t�B�[���h��
	 * @param	lIndex		[out]	�C���f�b�N�X
	 * @param	sErrMsg		[out]	�G���[���b�Z�[�W
	 * @return						��������
	 */
	bool getFieldIndex(const ITablePtr ipTable,
					   const CString& csField,
					   long &lIndex,
					   std::string& sErrMsg);

	/**
	 * @breif	���C�����擾
	 * @param	sFilePath	[in]	��̓t�@�C���i�t���p�X�j
	 * @return						���C����
	 */
	std::string getLayerName(const std::string& sFilePath);

	/**
	 * @breif	�t�@�C���������p�X�擾
	 * @param	sFilePath	[in]	��̓t�@�C���i�t���p�X�j
	 * @return						�t�@�C���������p�X
	 */
	std::string getPath(const std::string& sFilePath);
};

