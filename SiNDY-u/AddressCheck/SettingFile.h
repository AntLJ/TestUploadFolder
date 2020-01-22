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
 * @class	SettingFile
 * @brief	�ݒ�t�@�C��
 */
class SettingFile
{
public:
	/**
	 * @brief  �R���X�g���N�^
	 */
	SettingFile(void){}

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~SettingFile(void){}
	
	/**
	 * @brief	�ݒ�t�@�C���ǂݍ���
	 * @param	[in] path     �ݒ�t�@�C���p�X
	 * @param	[in] layer    ���C�����X�g
	 * @retval	true:����  false:���s
	 */
	bool load(const CString& path, const std::vector<CString>& layer);

	/**
	 * @brief	���O�t�@�C�����̎擾
	 * @param	[in] layer      ���݂̃��C��
	 * @return	���O�t�@�C����
	 */
	const CString& getLayerLogName( const CString& layer ) const;

	/**
	 * @brief	�ݒ�A�C�e���̎擾
	 * @param	[in] layer        ���݂̃��C��
	 * @param	[in] itemName     �ݒ�A�C�e����
	 * @return	�ݒ�A�C�e��������
	 */
	CString getLayerSettingItem(const CString& layer, const CString& itemName)const;

private:
	std::map<CString,CString> m_layerLogName; //!< ���C�� * ���O�t�@�C�������X�g
	CString m_settingFilePath;                //!< �ݒ�t�@�C���p�[�X
};
