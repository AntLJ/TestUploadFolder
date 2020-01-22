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
#include "Arguments.h"
#include "RunLogger.h"
#include "ErrLogger.h"
#include "TDC/sindylib_core/Workspace.h"

/**
 * @brief ���s�N���X
 */
class CCheckFollowingRoad
{
public:

	// ���H�m�[�h�̓��Ȃ胊���N����Ǘ�
	// (���H�����N��OID����1, ���H�����N��OID����2, 2�̃����N�������p�x)
	typedef std::map<std::pair<long, long>, double> FollowingRoad;

	CCheckFollowingRoad( const Arguments& args, CRunLogger& runLog, CErrLogger& errLog, const std::vector<long>& meshes )
		: m_Args( args ), m_RunLogger( runLog ), m_ErrLogger( errLog ), m_Meshes( meshes ) {};

	/**
	* @brief ���������s
	* @note ���b�V���P�ʂŃ��[�v���񂵁A�`�F�b�N�������s��
	* @retval true	���ׂẴ`�F�b�N�����Ő���I������
	* @retval false	1��ł��`�F�b�N�����ňُ�I������
	*/
	bool execute();

private:

	/**
	* @brief �`�F�b�N�����O�̏���
	* @note DB�ڑ��A�e�[�u���̗L���̊m�F�A�e�[�u���t�@�C���_�[�̃��f���ݒ���s��
	* @retval true	DB�ւ̐ڑ������������Ώۃe�[�u�������ׂđ��݂��Ă���
	* @retval false	DB�ւ̐ڑ��Ɏ��s�A�܂��͏����Ώۃe�[�u�������݂��Ȃ�
	*/
	bool preProcessing();

	/**
	* @brief �`�F�b�N�����s
	* @note �w�肳�ꂽ���b�V�����̃t�B�[�`���ɑ΂��ă`�F�b�N����
	* @param mesh[in] �`�F�b�N�͈͂̃��b�V���R�[�h
	* @retval true	�`�F�b�N�����ɐ���
	* @retval false	�`�F�b�N�����Ɏ��s
	*/
	bool doProcessing( const long mesh );

	/**
	* @brief �w�肳�ꂽDB�ɐڑ�����
	* @note �ڑ��Ɏ��s�����ۂɂ́A���s���O�ɏo�͂���
	* @param dbName[in] DB��
	* @param ws[out] �w�肵��DB�̃��[�N�X�y�[�X
	* @retval true	�ڑ��ɐ���
	* @retval false	�ڑ��Ɏ��s
	*/
	bool connectDB( const uh::tstring& dbName, sindy::CWorkspace& ws );

	/**
	* @brief �w�肵���e�[�u�����擾����
	* @note �擾�Ɏ��s�����ۂɂ́A���s���O�ɏo�͂���
	* @param ws[in] DB�̃��[�N�X�y�[�X
	* @param tableName[in] �e�[�u����
	* @param table[out] �w�肵���e�[�u���ւ̃|�C���^
	* @retval true	�擾�ɐ���
	* @retval false	�擾�Ɏ��s
	*/
	bool openTargetTable( const sindy::CWorkspace& ws, const uh::tstring& tableName, ITablePtr& table );
	
	/**
	* @brief �w�肵�����H�m�[�h�̓��Ȃ胊���N����擾
	* @param nodeID[in] ���Ȃ胊���N����擾���������H�m�[�h��OID
	* @param linkT[in] ���H�����N�̃e�[�u���R���e�i
	* @param followingRoad[out] ���Ȃ胊���N��
	* @retval true	�擾�ɐ���
	* @retval false	�擾�Ɏ��s
	*/
	bool getFollowingRoad( const long nodeOID, const sindy::CTableContainer& linkT, FollowingRoad& followingRoad );

	/**
	* @brief fromPoint����toPoint�܂ł̊p�x[��]�����߂�
	* @param fromPoint[in] �n�_
	* @param toPoint[in] �I�_
	* @return fromPoint����toPoint�܂ł̊p�x[��]
	*/
	double getAngle( const IPointPtr& fromPoint, const IPointPtr& toPoint );

	/**
	* @brief �o�[�W�����Ԃœ��H�m�[�h�ɐڑ����铹�H�����N�ɕω����Ȃ����`�F�b�N
	* @param baseLinkT[in] ��r���o�[�W�����̓��H�����N�̃e�[�u���R���e�i
	* @param compLinkT[in] ��r��o�[�W�����̓��H�����N�̃e�[�u���R���e�i
	* @retval true	�ω�����
	* @retval false	�ω��Ȃ�
	*/
	bool hasChangedConnectedLink( const sindy::CTableContainer& baseLinkT, const sindy::CTableContainer& compLinkT );

	/**
	* @brief ���H�m�[�h�܂��͂���ɐڑ����铹�H�����N���A�w����ȍ~�ɍX�V���ꂽ���`�F�b�N
	* @note �ŏI�X�V���ƍŏI�v���O�����X�V�������`�F�b�N�Ώ�
	* @note �ŏI�X�V�����w�肳��Ă��邩�ǂ����������Ŕ��肷��
	* @param nodeRow[in] ���H�m�[�h��Row�R���e�i(���H�����N���R�t���Ă���)
	* @retval true	�X�V����Ă��� or �ŏI�X�V�����w�肳��Ă��Ȃ�
	* @retval false	�X�V����Ă��Ȃ�
	*/
	bool isModifiedSinceDesignedDate( const sindy::CRowContainer& nodeRow );

	/**
	* @brief ��r���o�[�W�����̓��H�m�[�h�Ɠ��Ȃ胊���N��̑Ή��}�b�v���擾
	* @param nodeT[in] ���H�m�[�h�̃e�[�u���R���e�i
	* @param mapFollowingRoad[out] ���H�m�[�h�Ɠ��Ȃ胊���N��̑Ή��}�b�v
	* @retval true	�擾�ɐ���
	* @retval false	�擾�Ɏ��s
	*/
	bool getMapFollowingRoad( const sindy::CTableContainer& nodeT, std::map<long, FollowingRoad>& mapFollowingRoad );

	/**
	* @brief ��r���o�[�W�����Ɣ�r��o�[�W�����̓��Ȃ胊���N����r
	* @param mesh[in] �`�F�b�N�͈͂̃��b�V���R�[�h
	* @param baseNodeT[in] ��r���o�[�W�����̓��H�m�[�h�̃e�[�u���R���e�i
	* @param compNodeT[in] ��r��o�[�W�����̓��H�m�[�h�̃e�[�u���R���e�i
	* @retval true	��r�����ɐ���
	* @retval false	��r�����Ɏ��s
	*/
	bool chkDiffFollowingRoad( const long mesh, sindy::CTableContainer& baseNodeT, sindy::CTableContainer& compNodeT );
	
private:
	const Arguments& m_Args; //!< �R�}���h���C�������Ǘ��N���X
	CRunLogger& m_RunLogger; //!< ���s���O�Ǘ��N���X
	CErrLogger& m_ErrLogger; //!< �G���[���O�Ǘ��N���X
	const std::vector<long>& m_Meshes;	//!< �����ΏۂƂȂ郁�b�V���R�[�h�̈ꗗ

	sindy::CTableFinder m_BaseFinder; //!< ��r���o�[�W�����̃e�[�u���t�@�C���_�[
	sindy::CTableFinder m_CompFinder; //!< ��r��o�[�W�����̃e�[�u���t�@�C���_�[
};

