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
 * @brief ���[�e�B���e�B�֐��Q
 * @note  ����͕�����֌W�̂�
 */
namespace util
{
	/**
	 * @brief   �w�蕶���񂪐�����\���Ă��邩�ۂ��𔻒肷��
	 * @param	str  ���͕�����
	 * @retval	�����ł���
	 * @retval	�����ł͂Ȃ�
	 */
	bool isInteger(const std::string& str);
	bool isInteger(const CString& str);
	bool isInteger(const CComVariant& val);

	/**
	* @brief   �w�蕶���񂪕��������_��\���Ă��邩�ۂ��𔻒肷��
	* @param	str  ���͕�����
	* @retval	���������_�ł���
	* @retval	���������_�ł͂Ȃ�
	*/
	bool isFloat(const std::string& str);
	bool isFloat(const CString& str);
	bool isFloat(const CComVariant& val);

	/**
	 * @brief   VARIANT�^�̕ϐ��^�C�v�����l�ł��邩
	 * @param	val  ����Ώ�
	 * @retval true  ���l�ł���
	 * @retval false ���l�łȂ�
	 */
	bool isNumericType(const CComVariant& val);

	/**
	 * @brief   VARIANT�^�̕ϐ��^�C�v�������ł��邩
	 * @param	val  ����Ώ�
	 * @retval true  ���l�ł���
	 * @retval false ���l�łȂ�
	 */
	bool isIntegerType(const CComVariant& val);
}
