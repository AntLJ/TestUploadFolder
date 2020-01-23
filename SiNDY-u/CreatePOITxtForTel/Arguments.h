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

class CArguments
{
public:
	CArguments(void);
	~CArguments(void);

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse( int argc, _TCHAR* argv[] );

public:
	uh::tstring m_poiDB;     //!< POI�f�[�^SDE�ڑ���
	uh::tstring m_addrDB;    //!< �Z���f�[�^SDE�ڑ���
	uh::tstring m_mapDB;     //!< �n�}�f�[�^SDE�ڑ���
	uh::tstring m_poiPoint;  //!< POI_POINT
	uh::tstring m_poiSubPoint; //!< POI_SUB_POINT
	uh::tstring m_cityAdmin; //!< CITY_ADMIN
	uh::tstring m_building;  //!< BUILDING
	uh::tstring m_contents;  //!< POI_CONTENTS_MASTER
	uh::tstring m_listPath;  //!< �o�͑Ώێ�ʕ\���X�g
	uh::tstring m_outFile;   //!< POI_POINT�o�̓t�@�C����
	uh::tstring m_outFileSub;   //!< POI_SUB_POINT�o�̓t�@�C����
	uh::tstring m_idnCode;   //!< �����Ώ�IDNCODE
};

