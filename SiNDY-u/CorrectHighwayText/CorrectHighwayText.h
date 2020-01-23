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
#include "Logger.h"
#include <TDC/sindylib_core/Workspace.h>
namespace sindy {
	class CTableFinder;
	class CTableContainer;
	typedef boost::shared_ptr<CTableContainer> CSPTableContainer;
}

/**
 * @brief   ���C���N���X
 */
class CorrectHighwayText
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CorrectHighwayText( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~CorrectHighwayText(){}

	/**
	 * @brief   ���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute();

private:

	/**
	 * @brief   ������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool init();

	/**
	* @brief   �ҏW�J�n
	* @note    �ҏW���s���̂�HIGHWAY_TEXT�Ō����������Ȃ��̂ŁAStartEditOperation()���֐����ōs��
	* @retval  true  ����
	* @retval  false ���s
	*/
	bool startEdit();

	/**
	* @brief   �ҏW�I��
	* @retval  true  ����
	* @retval  false ���s
	*/
	bool stopEdit(bool bSave);

	/**
	 * @brief  �ҏW�Z�b�V������j������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool abortEditOperation();

	/**
	 * @brief  HIGHWAY_TEXT�̐������킹����
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool correctHwyText();

	/**
	 * @brief   �e�[�u���t�@�C���_������
	 * @param   table  [in] �e�[�u����
	 * @param   finder [out] �e�[�u���t�@�C���_
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool initTableFinder(const CString& table, sindy::CTableFinder& finder);

	/**
	 * @brief   �s����POINT_ID�̒l���C������
	 * @param   spHwyTextT [in] HIGHWAY_TEXT�e�[�u��
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool correctIllegalPointID(sindy::CSPTableContainer& spHwyTextT);

	/**
	* @brief   �s����POINT_ID�Ɠ����l��POINT_ID�����݂��邩
	* @note    ���݂���ꍇ�AcorrectRelPointIDFields()�Ō��X�̐���ȃf�[�^���X�V����Ă��܂��̂Ń`�F�b�N����B
	* @param   spHwyTextT [in] HIGHWAY_TEXT�e�[�u��
	* @param   pointIDs [in] �X�V�OPOINT_ID�Q
	* @retval  true  ���݂���
	* @retval  false ���݂��Ȃ�
	*/
	bool existBeforeEditPointID(sindy::CSPTableContainer& spHwyTextT, const std::vector<long>& pointIDs);

	/**
	 * @brief   �s����POINT_ID�Ɠ����l�������Ă���֘A�t�B�[���h�Q���C������
	 * @param   spHwyTextT [in] HIGHWAY_TEXT�e�[�u��
	 * @param   pointIDs [in] �s����POINT_ID�Q
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool correctRelPointIDFields(sindy::CSPTableContainer& spHwyTextT, const std::vector<long>& pointIDs);

	/**
	 * @brief   �ҏW�����f�[�^���X�g�A
	 * @param   spHwyTextT [in] HIGHWAY_TEXT�e�[�u��
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool store(sindy::CSPTableContainer& spHwyTextT);

private:

	Arguments m_args;            //!< ����
	sindy::CWorkspace m_tgtWork; //!< �����Ώېڑ���
	std::map<long, long> m_illegalPointIDs; //!< first�F�C���Ώ�POINT_ID, second�F�X�V��POINT_ID�iOBJECTID�j

};

