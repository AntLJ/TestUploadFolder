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


/**
 * @brief �R�}���h���C�������N���X
 */
class Arguments
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
//	Arguments() : m_meshMode(false), m_queryMode(false) {};
	Arguments(){};

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  �R�}���h���C�������̊m�F
	 * @param  optName  [in]  �I�v�V������
	 * @param  optValue [in]  �I�v�V�����l
	 * @param  bFile    [in]  �t�@�C���ł��邩�i�f�t�H���gfalse�j
	 * @retval true    ����OK
	 * @retval false   ����NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue, bool bFile = false );

	/**
	 * @brief  ���s���[�h�m�F
	 * @param  optMode1
	 */
//	bool SetMode( const boost::program_options::variables_map& vm, const char* const meshMode, const char* const queryMode );

public:
	uh::tstring  m_strRailNwDB;        //!< �S��NW�ڑ���
	uh::tstring  m_strRailPoiDB;       //!< �S��POI�ڑ���
	uh::tstring  m_strMeshDB;          //!< ���b�V��DB�ڑ���
	uh::tstring  m_strStationDB;       //!< ���k�w�ɐڑ���
	uh::tstring  m_strLog;             //!< ���O�t�@�C��
//	uh::tstring  m_strMeshLog;         //!< ���b�V�����[�h���O�t�@�C��
//	uh::tstring  m_strQueryLog;        //!< �N�G�����[�h���O�t�@�C��
	uh::tstring  m_strMeshList;        //!< ���b�V�����X�g
	uh::tstring  m_dist_threshold;     //!< ��������臒l(m)
	uh::tstring  m_neighbor_threshold; //!< �ߖT����臒l(m)
//	bool         m_meshMode;           //!< ���b�V�����[�h
//	bool         m_queryMode;          //!< �N�G�����[�h

};
