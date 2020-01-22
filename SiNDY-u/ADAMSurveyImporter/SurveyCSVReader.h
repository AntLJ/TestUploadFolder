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

#include <boost/tokenizer.hpp>

#include "DataAccessor.h"
#include "SurveyCSV.h"

namespace adam_v2
{
/**
 * @brief	�[���f�[�^�ǂݍ��݃N���X
 */
class SurveyCSVReader : public DataAccessor
{
public:
	SurveyCSVReader(void);
	~SurveyCSVReader(void);

	/**
	 * @brief	�f�[�^�ǂݍ���
	 * @param[in]	fileName	CSV�t�@�C����
	 * @retval	true:	����
	 * @retval	false:	���s
	 */
	bool Read(const uh::tstring& fileName);
	
	/**
	 * @brief	�f�[�^��������
	 * @note	���g�p
	 * @param[in]	tableName	ADAM�e�[�u����
	 * @retval	true:	����
	 * @retval	false:	���s
	 */
	bool Write(const uh::tstring& tableName)
	{
		return false;
	}

	/**
	 * @brief	�f�[�^�N���A
	 */
	void Clear();

	/**
	 * @brief	�ǂݍ��ݍς݃f�[�^�̎擾
	 * @note	�����G���[�̓��͂�SKIP����Ă���
	 * @return	�ǂݍ��ݍς݃f�[�^
	 */
	SurveyCSVPtr GetSurveyCSVPtr();

	/**
	 * @brief	�폜���R�[�h�X�L�b�v�t���O�̐ݒ�
	 * @param[in]   isSkip �X�L�b�v���邩�ۂ�
	 */
	void SetSkipDeleteRecord( bool isSkip ) { _skipDeleteRecord = isSkip; };

	/**
	 * @brief	�o�[�W�����`�F�b�N
	 * @retval	true:	����
	 * @retval	false:	���s
	 */
	bool CheckCsvVersion();

private:
	/**
	 * @brief	�f�[�^�ǂݍ���
	 * @param[in]	dirPath		�f�B���N�g���p�X
	 * @param[in]	fileName	�t�@�C����
	 * @param[in]	func		�֐��|�C���^
	 * @retval	true:	����
	 * @retval	false:	���s
	 */
	bool ReadCSV(
		const uh::tstring& dirPath,
		const uh::tstring& fileName,
		bool (SurveyCSVReader::*func)(const boost::tokenizer<boost::char_separator<char>>&)
		);

	/**
	 * @brief	�g�[�N�����̎擾
	 * @param[in]	tokens	�f���~�^�ŕ��������v�f
	 * @return	�g�[�N����
	 */
	long getTokenCount(const boost::tokenizer<boost::char_separator<char>>& token);

	/**
	 * @brief	�W�I���g���̐������`�F�b�N
	 * @note	�`�F�b�NNG�̃I�u�W�F�N�g�͍폜����
	 * @retval	true:	�ُ�Ȃ�
	 * @retval	false:	�ُ킠��
	 */
	bool CheckGeometryIntegrity();

	/**
	 * @brief	OID���я��`�F�b�N
	 * @retval	true:	�ُ�Ȃ�
	 * @retval	false:	�ُ킠��
	 */
	bool CheckSortByOID();

	/**
	 * @brief	�폜���R�[�h������
	 *
	 * @param[in]	fileName	�t�@�C����
	 * @param[in]	tokens		���R�[�h
	 * @param[in]	index		�X�V�R�[�h�̃C���f�b�N�X
	 * @retval		true		�폜���R�[�h�ł���
	 * @retval		false		�폜���R�[�h�łȂ�
	 */
	bool IsDeleteRecord( const uh::tstring& fileName, const boost::tokenizer<boost::char_separator<char>>& tokens, long index );

	/**
	 * @brief	�X�V�R�[�h�̃C���f�b�N�X�擾
	 *
	 * @param[in]	fileName	�t�@�C����
	 * @return	�C���f�b�N�X
	 */
	long GetIndexUpdateC( const uh::tstring& fileName );

	/**
	 * @brief	�t�@�C�����Ƃ̓ǂݍ��ݏ���
	 *
	 * @param[in]	tokens	�f���~�^�ŕ��������v�f
	 * @retval	true:	����
	 * @retval	false:	���s
	 */
	bool ReadLaneLink				(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadLanePoint				(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRelLaneLink			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadCompartLink			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadCompartPoint			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRelCompartLink			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadBorderLink				(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadBorderPoint			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRelBorderLink			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadSignalPoint			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRelSignalPoint			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRoadSignPoint			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRelRoadSignPoint		(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadCrossWalkLine			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRelCrossWalkLine		(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadVirtualCompartLink		(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadVirtualCompartPoint	(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRelVirtualCompartLink	(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadCoveringArea			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadSpatialRef				(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadVersion				(const boost::tokenizer<boost::char_separator<char>>& tokens);
private:
	SurveyCSVPtr	_csv;				//!< �f�[�^����
	bool			_skipDeleteRecord;	//!< �X�V�R�[�h�u�폜�v�̃��R�[�h���X�L�b�v���邩
	bool _hasError;						//!< �G���[�L���t���O(�C���|�[�g�����̃`�F�b�N�p)
};
typedef std::shared_ptr<SurveyCSVReader> SurveyCSVReaderPtr;

}; //adam_v2
