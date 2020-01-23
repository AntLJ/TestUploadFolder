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
#include <RowBase.h>

/// ���[�h�^�C�v
enum S2SMode : int
{
	s2sModeConvert, //!< �R���o�[�g���[�h
	s2sModeMerge, //!< �}�[�W���[�h
	s2sModeCrdcnv, //!< ���n�n�ϊ����[�h
	s2sModeClip, //!< �N���b�s���O���[�h
};

/**
 * @brief �R�}���h���C�������N���X
 */
class Arguments
{
public:
	// �V�X�e���ϐ�(�c�[�����Ŏg�p������ʂȕϐ�)�̎��ʎq���`
	enum SYSTEM_VALUE
	{
		MESHCODE,
	};

	/**
	 * @brief �R���X�g���N�^
	 */
	Arguments()
	{
		m_thread = 1;
		m_meshLayerName = _T("CITYMESH");
		m_dissolveMulti = true;
		m_output_ondemand_search = false;
	}

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
	 * @param  cOptName  [in]  �I�v�V������
	 * @param  optValue [in]  �I�v�V�����l
	 * @param  bFile    [in]  �t�@�C���ł��邩�i�f�t�H���gfalse�j
	 * @retval true:    ����OK
	 * @retval false:   ����NG
	 */
	bool CheckParam( const char* const cOptName, const uh::tstring& optValue, bool bFile = false );

	/**
	 * @brief  ���̓f�[�^��PGDB����FGDB��?
	 * @note   DB�ڑ���񂪃t�@�C���܂��̓t�H���_�ł��邩�Ŕ��f
	 * @param  str  [in]  DB�ڑ����
	 * @retval true/DB�ڑ����PGDB��FGDB
	 */
	bool isPGDBorFGDB(const uh::tstring& str);

	/**
	 * @brief  �f�B���N�g���p�X����PG(FG)DB�̃t�@�C���p�X�������ꗗ�ŕԋp
	 * @param  directoryPath  [in]  �f�B���N�g���p�X
	 * @retval PG(FG)DB�̃t�@�C���p�X
	 */
	std::vector<uh::tstring> getPGFGDBPath(const uh::tstring& directoryPath);

	/**
	 * @brief  PG(FG)DB�̃t�@�C���p�X���烁�b�V���R�[�h�������ꗗ�ŕԋp
	 * @param  vPaths  [in]  PG(FG)DB�̃t�@�C���p�X
	 * @retval ���b�V���R�[�h�ꗗ
	 */
	std::vector<uh::tstring> getMeshCodesFromPaths(const std::vector<uh::tstring>& vPaths);

	/**
	* @brief �C���v�b�g/�A�E�g�v�b�g���̃I�[�i�[���⊮�e�[�u������Ԃ�
	* @param name [in] �⊮�������e�[�u����
	* @return �u�I�[�i�[��.�e�[�u�����v��Ԃ�\n�I�[�i�[���w�肪������΁u�e�[�u�����v�̂ݕԂ�
	*/
	uh::tstring getInputTableName( const uh::tstring& name ) const;
	uh::tstring getOutputTableName( const uh::tstring& name ) const;

	/**
	 * @brief �I�v�V�����Ŏw�肳�ꂽ�l���擾����
	 */
	CComVariant GetValue( LPCTSTR name ) const;

	/**
	 * @brief ���ʎq�ɑΉ�����V�X�e���ϐ����擾
	 * @param[in] identifier  ���ʎq
	 * @param[in] nullValue  �ϐ�����`����Ă��Ȃ��ꍇ�ɕԂ��l(�f�t�H���g�F�󕶎���)
	 * @retval nullValue�ȊO ���ʎq�ɑΉ�����V�X�e���ϐ�
	 * @retval nullValue     ���ʎq�ɑΉ�����V�X�e���ϐ��͒�`����Ă��Ȃ�
	 */
	uh::tstring GetSystemValue(
		SYSTEM_VALUE identifier, const uh::tstring& nullValue = uh::tstring() ) const;

public:
	uh::tstring  m_input;					//!< [�K�{]���͏��
	uh::tstring  m_output;					//!< [�K�{]�o�͏��
	uh::tstring  m_input_owner;				//!< [�C��]���͑��I�[�i�[��
	uh::tstring  m_output_owner;			//!< [�C��]�o�͑��I�[�i�[��
	uh::tstring  m_mesh_list_str;			//!< [�C��]���b�V��/���X�g/�t�@�C��
	uh::tstring  m_meshDB;					//!< [�K�{]���b�V��DB
	uh::tstring  m_meshTableOwner;			//!< [�C��]���b�V��DB�I�[�i�[
	uh::tstring  m_meshLayerName;			//!< [�K�{]���b�V�������̒P��
	std::map<S2SMode,uh::tstring>  m_mapDefFile; //!< [�K�{]��`�t�@�C���p�X
	uh::tstring  m_runLog;					//!< [�K�{]���s���O�t�@�C���p�X
	uh::tstring  m_errLog;					//!< [�K�{]�G���[���O�t�@�C���p�X
	long m_thread;                          //!< [�C��]�X���b�h��
	std::vector<uh::tstring> m_values;      //!< [�C��]��`���Ŏg�p����l�̎w��(������)
	std::vector<uh::tstring> m_meshes;		//!< ���b�V���R�[�h���X�g
	std::vector<uh::tstring> m_paths;		//!< PG(FG)DB�̃t�@�C���p�X�Q
	bool m_dissolveMulti;                   //!< [�C��]�}���`�p�[�g���������邩�ǂ����i�f�t�H���g�Ftrue�j
	std::map<uh::tstring,CComVariant> m_mapValue; //!< GetValue�ŕԂ��l
	std::map<SYSTEM_VALUE,uh::tstring> m_systemValue; //!< �c�[�����œ��ʂȈӖ������ϐ�
	bool m_output_ondemand_search;          //!< [�C��]�A�E�g�v�b�g���̍X�V�Ώۃt�B�[�`���͓s�x��������i�f�t�H���g�Ffalse�j
};
