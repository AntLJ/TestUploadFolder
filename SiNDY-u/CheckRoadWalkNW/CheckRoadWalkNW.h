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
#include "DatabaseController.h"

#include "CheckRoadWalkLink.h"
#include "CheckRoadWalkNode.h"
#include "CheckRoadWalkPassageCondition.h"
#include "CheckRoadWalkOnewayCondition.h"
#include "CheckRoadCode.h"

#include <WinLib/VersionInfo.h>

#define OPTION_MAX 10

class	CCheckRoadWalkNW
{
private:
	
	std::ofstream			m_ErrorLog;				// ���O�o�͗p
	SYSTEMTIME				m_st;					// �J�n�E�I�����Ԏ擾�p

	bool					m_bIsError;				// �G���[�����݂������ǂ���
	std::string				m_inputDirPath;
	std::string				m_layer;				// ���̓��C����
	std::string				m_roadDBConnection;		// ���HDB�ڑ����
	std::string				m_walkDBConnection;		// ���s��DB�ڑ����
	std::string				m_meshDBConnection;		// �Q�ƃ��b�V��DB�ڑ����
	std::string				m_roadSchema;			// ���HDB�X�L�[�}
	std::string				m_walkSchema;			// ���s��DB�X�L�[�}
	std::string				m_meshSchema;			// �Q�ƃ��b�V��DB�X�L�[�}

	std::vector<std::string>		m_meshList;

public:

	CCheckRoadWalkNW():m_bIsError(false){};
	~CCheckRoadWalkNW(){};

	/**
	 * @brief	�g�p���@
	 */
	void printUsage();

	/*
	 * @brief	������
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool init(int argc, char* argv[]);
	/*
	 * @brief	�`�F�b�N���s
	 * @return	bool
	 */
	bool execute();

	/*
	 * @brief	�G���[�̑��ݗL��
	 * @return	bool
	 */
	bool isError();

private:
	/*
	 * @brief	�I�v�V�����`�F�b�N
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @return	bool
	 */
	bool	checkOptions(int argc, char* argv[]);

	/*
	 * @brief	�I�v�V�����`�F�b�N
	 * @param	argc	[in]	�����̐�
	 * @param	argv	[in]	����
	 * @param	pos	[in]	�`�F�b�N������������Ԗڂ̈�����
	 * @return	bool
	 */
	bool	checkOptionArgument(int argc, char* argv[], int pos);

	/**
	 * @brief	���X�g�t�@�C���ǂݍ���
	 * @param	lpszFile	[in]	�ǂݍ��݂������X�g��
	 * @param	VectorList	[out]	�Ǎ���Vector
	 * @return	bool
	 */
	bool	loadList( LPCTSTR lpszFile, std::vector<std::string> &VectorList );

	/**
	 * @brief	SHAPE���i�[����Ă���f�B���N�g���p�X�̍쐬
	 * @param	strMesh		[in]	���b�V���R�[�h
	 * @param	strPath		[in]	�t�@�C����_�p�X
	 * @return	string		[out]	SHAPE���i�[����Ă���f�B���N�g���p�X
	 */
	std::string	makeShapeDirName( std::string strMesh, std::string strPath);

	/**
	 * @brief	�V�F�[�v�t�@�C���`�F�b�N
	 * @param	strMesh			[in]	�`�F�b�N�Ώۃ��b�V��
	 * @param	strShapeDirPath	[in]	SHAPE�f�[�^�i�[�f�B���N�g��
	 * @param	dbCtrl	[in]	�Q��DB�Ǘ��N���X
	 * @return	bool
	 */	
	bool checkShape( std::string strMesh, std::string strShapeDirPath, CDatabaseController dbCtrl);

	/**
	 * @brief	RWL��RWN�̃m�[�h���݃`�F�b�N
	 * @param	ipRWLFeatureClass	[in]	�`�F�b�N����RWL�̃t�B�[�`���[�N���X
	 * @param	ipRWNFeatureClass	[in]	RWN�̃t�B�[�`���[�N���X
	 * @param	messageList	[out]	�G���[���b�Z�\�W���X�g
	 * @return	bool
	 */	
	bool checkExist(const IFeatureClassPtr ipRWLFeatureClass,const IFeatureClassPtr ipRWNFeatureClass,std::vector<CString>& messageList);

	/**
	 * @brief	RWN�̕ϊ��ΏۊO���R�[�h�����݂��Ȃ����Ƃ̃`�F�b�N
	 * @param	strMesh		[in]	���b�V���R�[�h
	 * @param	ipRWNFeatureClass	[in]	�`�F�b�N����RWN�̃t�B�[�`���[�N���X
	 * @param	RWNFiledIndex	[in]	RWL�̃t�B�[�`���[�N���X
	 * @param	dbCtrl	[in]	�Q��DB�Ǘ��N���X
	 * @param	messageList	[out]	�G���[���b�Z�\�W���X�g
	 * @return	bool
	 */
	bool checkNoTarget(const string& strMesh, const IFeatureClassPtr ipRWNFeatureClass, const FIELD_INDEX& RWNFiledIndex, CDatabaseController dbCtrl, std::vector<CString>& messageList);

	/**
	 * @brief	���s�҃����N�Ɂu�l�ԋ��p�����N�v��1�{�ȏ㑶�݂���m�[�h�̎擾
	 * @param	rwlFeatures	[in]	RWL�̃t�B�[�`���[
	 * @param	rwlFieldIndex	[in]	RWL�̃t�B�[���h�C���f�b�N�X
	 * @param	nodeList	[out]	�m�[�h���X�g
	 * @return	bool
	 */	
	bool getHumanCarLinkList(const MAP_FEATURE& rwlFeatures, const FIELD_INDEX& rwlFieldIndex, multiset<long>& nodeList);

	/**
	 * @brief	���O�o��
	 * @param	tag	[in]	���O���x���̃^�O
	 * @param	message1	[in]	���b�Z�[�W1
	 * @param	message2	[in]	���b�Z�[�W2
	 */	
	void printLog(std::string tag, std::string message1, std::string message2);

	/**
	 * @brief	���O�o��(List)
	 * @note	���b�Z�[�W�̃��X�g�P�P�Ƀ^�O�����ďo�͂���B
	 * @param	tag	[in]	���O���x���̃^�O
	 * @param	messageList	[in]	���b�Z�[�W�̃��X�g
	 */	
	void printLog(std::string tag, std::vector<CString> messageList);

};
