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

#include <string>

namespace uh
{
	/**
	 * @brief TCHAR�Ή�std::string
	 */
	typedef std::basic_string<TCHAR> tstring;

	/**
	 * @brief ���C�h�������std::string�ɕϊ�����
	 * @note	TCHAR*�ɂ�����CT2A()�̂悤�ɁA
	 *			uh::tstring�ɑ΂��ď�Ƀ}���`�o�C�g�������Ԃ����Ƃ�ړI�Ƃ���֐��ł���B
	 * @param	str [in]	���C�h������
	 * @return	std::string������
	 */
	inline std::string toStr(const std::wstring& wstr)
	{
		return std::string(CW2A(wstr.c_str()));
	}

	/**
	 * @brief �}���`�o�C�g�������std::string�ɂ���
	 * @note	TCHAR*�ɂ�����CT2A()�̂悤�ɁA
	 *			uh::tstring�ɑ΂��ď�Ƀ}���`�o�C�g�������Ԃ����Ƃ�ړI�Ƃ���֐��ł���B
	 * @param	str [in]	�}���`�o�C�g������
	 * @return	std::string������
	 */
	inline std::string toStr(const std::string& str)
	{
		return str;
	}

	/**
	 * @brief ���C�h�������std::wstring�ɂ���
	 * @note	TCHAR*�ɂ�����CT2W()�̂悤�ɁA
	 *			uh::tstring�ɑ΂��ď�Ƀ��C�h�������Ԃ����Ƃ�ړI�Ƃ���֐��ł���B
	 * @param	wstr [in]	���C�h������
	 * @return	std::wstring������
	 */
	inline std::wstring toWStr(const std::wstring& wstr)
	{
		return wstr;
	}

	/**
	 * @brief �}���`�o�C�g�������std::wstring�ɕϊ�����
	 * @note	TCHAR*�ɂ�����CT2W()�̂悤�ɁA
	 *			uh::tstring�ɑ΂��ď�Ƀ��C�h�������Ԃ����Ƃ�ړI�Ƃ���֐��ł���B
	 * @param	str [in]	�}���`�o�C�g������
	 * @return	std::wstring������
	 */
	inline std::wstring toWStr(const std::string& str)
	{
		return std::wstring(CA2W(str.c_str()));
	}

	/**
	 * @brief ���C�h�������tstring�ɕϊ�����
	 * @note	TCHAR*�ɂ�����CW2T()�̂悤�ɁA
	 *			���tstring�������Ԃ����Ƃ�ړI�Ƃ���֐��ł���B
	 * @param	wstr [in]	���C�h������
	 * @return	tstring������
	 */
	inline tstring toTStr(const std::wstring& wstr)
	{
		return tstring(CW2T(wstr.c_str()));
	}

	/**
	 * @brief �}���`�o�C�g�������tstring�ɕϊ�����
	 * @note	TCHAR*�ɂ�����CA2T()�̂悤�ɁA
	 *			���tstring�������Ԃ����Ƃ�ړI�Ƃ���֐��ł���B
	 * @param	str [in]	�}���`�o�C�g������
	 * @return	tstring������
	 */
	inline tstring toTStr(const std::string& str)
	{
		return tstring(CA2T(str.c_str()));
	}
}
