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

#include "common.h"
#include "PgDataBaseEx.h"
#include "IllCooker.h"

#include <sindy/schema/illust.h>

/**
 *	@class  IllConverter
 *	@brief	�C���X�gEGG�ϊ��N���X
 */
class IllConverter 
{
public:
	/**
	 *	@brief	�R���X�g���N�^
	 *	@param	orgDataBase [in] �ϊ����C���X�g�����NPostgreSQLDB�n���h�����O�N���X
	 *	@param	tgtDataBase [in] �ϊ���PostgreSQLDB�n���h�����O�N���X
	 *	@param	orgSchema [in] �ϊ����C���X�g�����N�e�[�u���i�[�X�L�[�}
	 *	@param	tgtSchema [in] �ϊ���e�[�u���i�[�X�L�[�}
	 */
	IllConverter(std::shared_ptr<PgDataBaseEx> orgDataBase,
				 std::shared_ptr<PgDataBaseEx> tgtDataBase,
				 LPCTSTR orgIlSchema, 
				 LPCTSTR tgtSchema);
	/**
	 *	@brief	�f�X�g���N�^
	 */
	~IllConverter(void);

	/**
	 *	@brief �C���X�gEGG�ϊ��֐�
	 *	@param	illSetInfo	[in] �C���X�g�������ݒ�t�@�C�����
	 *	@param	inputListInfo [in] �C���X�g�t�@�C�����Ή��\���
	 *	@return true ����I��
	 *	@return false �ُ�I��
	 */
	bool Convert(const std::vector<IllSetRecord>& illSetInfo, const std::map<ClassCode, std::map<OID, IllFileNames>>& inputListInfo);

private:
	/**
	 *	@brief	�o�̓e�[�u���쐬
	 *	@return true ����
	 *	@return false ���s
	 */
	bool CreateIllTables();
	/**
	 *	@brief	�e�[�u���폜�֐�
	 *	@param	dataBase [in] �Ώ�DB�n���h�����O�N���X
	 *	@param	tableName [in] �폜�Ώۃe�[�u��
	 *	@return true ����
	 *	@return false ���s
	 */
	bool DeleteTable(std::shared_ptr<PgDataBaseEx> dataBase, LPCTSTR tableName);
	/**
	 *	@brief	�ϊ��ΏۃC���X�g�����N���R�[�h�Q�擾�֐�
	 *	@param	illClass	[in] �ΏۃC���X�g�N���X
	 *	@param	dstCode		[in] �Ώێd������R�[�h
	 *	@param	rows		[out] ���R�[�h�Q
	 *	@return true ����
	 *	@return false ���s
	 */
	bool GetIllLinkRows(LPCTSTR illClass, LPCTSTR dstCode, ns::RowsPtr& rows);
	/**
	 *	@brief	���R�[�h��񁨃e�L�X�g�֐�
	 *	@note	���R�[�h����PostgreSQL�̃R�s�[���p�̕�����ɕϊ�����B
	 *	@param	infIllInfos [in] inf_illust���R�[�h���
	 *	@param	lqIllInfos [in] lq_illust���R�[�h���
	 *	@param	infCopyValues [out] inf_illust�R�s�[���p������
	 *	@param	lqCopyValues [out] lq_illust�R�s�[���p������
	 */
	void Info2Str(const std::map<OID, InfIllInfo>& infIllInfos,
				  const std::map<LqIllKey, LqIllInfo>& lqIllInfos,
				  std::string& infCopyValues,
				  std::string& lqCopyValues);
	/**
	 *	@brief	���R�[�h�C���|�[�g����
	 *	@param	tableName [in] �Ώۃe�[�u����
	 *	@param	fileds [in] �Ώۃt�B�[���h��(,��؂�)
	 *	@param	imporValues [in] �o�^�l
	 *	@return true ����
	 *	@return false ���s
	 */
	bool importValues(LPCSTR tableName, LPCSTR fields, const std::string& importValues);

private:
	
	std::shared_ptr<PgDataBaseEx> m_orgDataBase;		//!< PostgreSQLDB�n���h�����O�N���X (input �C���X�g�����N�p)
	std::shared_ptr<PgDataBaseEx> m_tgtDataBase;		//!< PostgreSQLDB�n���h�����O�N���X (output�p)

	IllCooker m_illCooker;								//!< �C���X�g�����N���R�[�h�ϊ��N���X

	CString m_illustLink;								//!< �ϊ����C���X�g�����N�e�[�u����(�X�L�[�}+�e�[�u����)
	CString m_roadLink;									//!< �ϊ������[�h�����N�e�[�u����
	CString m_infIllustLink;							//!< �ϊ���C���X�g���e�[�u����(�X�L�[�}+�e�[�u����)
	CString m_lqIllustLink;								//!< �ϊ���C���X�g�����N��e�[�u����(�X�L�[�}+�e�[�u����)
	CString m_infFields;								//!< �ϊ���C���X�g���t�B�[���h������(�C���|�[�g���Ɏg�p)
	CString m_lqFields;									//!< �ϊ���C���X�g�����N��t�B�[���h������(�C���|�[�g���Ɏg�p)

};

