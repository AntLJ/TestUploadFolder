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
 * @class	MeshCodeList
 * @brief	���b�V���R�[�h���X�g
 */
class MeshCodeList
{
public:
	/**
	 * @brief  �R���X�g���N�^
	 */
	MeshCodeList(void){}

	 /**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~MeshCodeList(void){}
	
	/**
	 * @brief	���b�V�����X�g�ǂݍ���
	 * @param	[in] path     ���b�V�����X�g�p�X
	 * @retval	true:����  false:���s
	 */
	bool load( const CString& path);

	/**
	 * @brief	���b�V�����X�g�擾
	 * @return	���b�V�����X�g
	 */
	 const std::list<unsigned long>& getMeshCodeList() const;

private:
	std::list<unsigned long> m_listMeshCode; //!< ���b�V�����X�g
};
