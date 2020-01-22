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
#include <TDC/useful_headers/tstring.h>

/**
 * @brief	�����i�[�p�R���e�i
 */
struct Arguments
{
	uh::tstring in_list;         //!< �������ʃt�@�C�����X�g
	uh::tstring import_log;      //!< �C���|�[�g���O�t�@�C��
	uh::tstring auto_rpoint_log; //!< ���[�g�T���p�|�C���g���O�t�@�C��
	uh::tstring db_check_log;    //!< DB�`�F�b�N���O�t�@�C��
	uh::tstring export_log;      //!< �G�N�X�|�[�g���O�t�@�C��
	uh::tstring bus_db;          //!< �o�X�C���|�[�g�pDB
	uh::tstring road_db;         //!< ���HNW�pDB
	uh::tstring company_list;    //!< �o�͎��ƎҖ����X�g
	uh::tstring export_dir;      //!< �o�͐�t�H���_

	bool        bImport;          //!< �C���|�[�g���[�h
	bool        bAutoRPoint;     //!< ���[�g�T���p�|�C���g���[�h
	bool        bDbCheck;        //!< DB�`�F�b�N���[�h
	bool        bExport;          //!< �G�N�X�|�[�g���[�h
	bool        bAllCompany;     //!< �S���Ǝ҃G�N�X�|�[�g

	Arguments(){
		bImport     = false;
		bAutoRPoint = false;
		bDbCheck    = false;
		bExport     = false;
		bAllCompany = false;
	}
};
