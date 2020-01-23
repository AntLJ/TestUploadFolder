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
#include "Log.h"
#include <utf8/usjis.h>

class CPoiFile
{
public:
	CPoiFile(void);
	~CPoiFile(void);

	/**
	 * @brief	POI�t�@�C�����X�g�ǂݍ���
	 * @param	sPoiListPath	[in]	POI���X�g�t�@�C���p�X
	 * @param	setContCode		[in]	�ΏۃR���e���c�R�[�h
	 * @param	vecPoiData		[out]	POI�f�[�^�R���e�i
	 * @return							��������
	 */
	bool readPoiList(const std::string& sPoiListPath,
					 std::set<std::string>& setContCode,
					 MAP_POI_DATA& vecPoiData);

protected:
	/**
	 * @brief	POI�f�[�^�擾
	 * @param	file			[in]	�t�@�C���p�X
	 * @param	setContCode		[in]	�ΏۃR���e���c�R�[�h
	 * @param	vecPoiData		[out]	POI�f�[�^�R���e�i
	 * @return							��������
	 */
	bool getPoiData(const std::string& file,
					std::set<std::string>& setContCode,
					MAP_POI_DATA& mapPoiData);
};

