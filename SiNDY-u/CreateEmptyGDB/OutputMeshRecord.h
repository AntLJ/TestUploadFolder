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
#include "DataDef.h"
/**
 * @brief �Ώۂ�GDB�Ƀ��b�V�����o�͂���
 */
class COutputMeshRecord
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param targetPathList [in] �������GDB�p�X�ꗗ
	 * @param meshlist       [in] ���b�V�����X�g�̓Ǎ��݌���
	 * @param meshDB         [in] ���b�V��DB
	 * @param meshLayerName  [in] ���b�V�����C����
	 */
	COutputMeshRecord(
		const std::vector<uh::tstring>& targetPathList
		, const std::vector<uh::tstring>& meshlist
		, const uh::tstring& meshDB
		, const uh::tstring& meshLayerName
		, OUTPUT_TYPE outputType):
		m_targetPathList(targetPathList)
		, m_meshlist(meshlist)
		, m_meshDB(meshDB)
		, m_meshLayerName(meshLayerName)
		, m_outputType(outputType)
	{}

	/**
	 * @brief �Ώۂ�GDB�Ƀ��b�V�����o�͂���
	 * @note �쐬��Ƀ��b�V���R�[�h���������R�[�h�����݂���ꍇ�A�o�͂��Ȃ�
	 * @return �����̐���
	 */
	bool output();

protected:
	/**
	 * @brief �o�͐�Ƀ��b�V�����X�g�Ɏw�肳�ꂽ���b�V�����o��
	 * @param path            [in] �o�͐�p�X
	 * @param inputLayer      [in] ���b�V�������惌�C��
	 * @param inputFM         [in] inputLayer�̃t�B�[���h�}�b�v
	 * @param inputShapeIndex [in] inputLayer�̌`��t�B�[���h�̃C���f�b�N�X�ԍ�
	 * @return �����̐���
	 */
	bool outputAllTargetMesh(
		const uh::tstring& path, const IFeatureClassPtr& inputLayer
		, const std::map<uh::tstring,long>& inputFM, long inputShapeIndex);

	/**
	 * @brief �����ɍ��킹�ă��R�[�h���o��
	 * @param outputLayer      [in]  �o�͐�̃��C��
	 * @param outputStr        [in]  �o�͐�DB�̐ڑ�������
	 * @param strWhereList     [in]  ���R�[�h��������
	 * @param inputLayer       [in]  ���b�V�������惌�C��
	 * @param inputFM          [in]  inputLayer�̃t�B�[���h�}�b�v
	 * @param inputShapeIndex  [in]  inputLayer�̌`��t�B�[���h�̃C���f�b�N�X�ԍ�
	 * @param outputFM         [in]  outputLayer�̃t�B�[���h�}�b�v
	 * @param outputShapeIndex [in]  outputLayer�̌`��t�B�[���h�̃C���f�b�N�X�ԍ�
	 * @param createdMeshList  [out] outputLayer�ɐ������ꂽ���b�V���R�[�h�̊i�[��
	 * @return �����̐���
	 */
	bool outputRecord(
		IFeatureClassPtr& outputLayer
		, const uh::tstring& outputStr
		, const std::vector<CString>& strWhereList
		, const IFeatureClassPtr& inputLayer
		, const std::map<uh::tstring,long>& inputFM, long inputShapeIndex
		, const std::map<uh::tstring,long>& outputFM, long outputShapeIndex
		, std::vector<CString>& createdMeshList);

	/**
	 * @brief �o�͑ΏۂƂȂ郁�b�V���̃��b�V���R�[�h���X�g���쐬
	 * @param path          [in] �o�͐�p�X
	 * @param outputMeshList [in] outputLayer�ɑ��݂��郁�b�V���R�[�h�ꗗ
	 * @return �o�͑Ώۂ̃��b�V���R�[�h���X�g
	 */
	std::vector<CString> makeTargetMeshList(
		const uh::tstring& path
		, const std::vector<unsigned long>& outputMeshList);

	/**
	 * @brief ���b�V�����������Q�𐶐�
	 * @param targetMeshList [in] �o�͑Ώۂ̃��b�V���R�[�h���X�g
	 * @return �o�͑Ώۂ̃��b�V���R�[�h���X�g
	 */
	std::vector<CString> makeWhereList(const std::vector<CString>& targetMeshList);

protected:
	const std::vector<uh::tstring>& m_targetPathList; //!< �������GDB�p�X�ꗗ
	const std::vector<uh::tstring>& m_meshlist;       //!< ���b�V�����X�g�̓Ǎ��݌���
	const uh::tstring m_meshDB;                       //!< ���b�V��DB
	const uh::tstring m_meshLayerName;                //!< ���b�V�����C����
	OUTPUT_TYPE m_outputType;                         //!< �o�͐�^�C�v
};
