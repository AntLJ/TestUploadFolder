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
#include <vector>
#include <locale>
#include <stdexcept>
#include <cassert>

namespace uh
{
	/**
	 * @brief �}���`�o�C�g����������C�h������ɕϊ�����
	 * @node C++�W�����C�u����std::codecvt���g���Ă���
	 * @param str [in] �}���`�o�C�g������
	 * @param loc [in] ���P�[��
	 * @return ���C�h������
	 */
	inline std::wstring Str2WStr(const std::string& str, const std::locale& loc)
	{
		typedef std::codecvt<wchar_t, char, std::mbstate_t> CodeCvt;
		const CodeCvt& converter = std::use_facet<CodeCvt>(loc);
		std::mbstate_t state = {0};

		const char* src     = str.c_str();
		const char* src_end = str.c_str() + str.length();
		int max = str.length();
		int wlen = converter.length(state, src, src_end, max);
		// null�^�[�~�l�[�g�p��1���������o�b�t�@���m�ۂ���
		std::vector<wchar_t> wchar_buf(wlen + 1);
		wchar_t* dst       = &wchar_buf.at(0);
		wchar_t* dst_limit = &wchar_buf.at(wlen);
		const char* src_next = 0;
		wchar_t* dst_next = 0;
		std::codecvt_base::result result = converter.in(
			state,		// ��ԕێ��p�̕ϐ�
			src,		// �ϊ���������̐擪�A�h���X
			src_end,	// �ϊ���������̍Ō�̎��̃A�h���X
			src_next,	// �Ō�ɕϊ��ɐ��������ϊ���������̗v�f�̎����w���|�C���^
			dst,		// �ϊ��敶����̐擪�A�h���X
			dst_limit,	// �ϊ��敶����̍Ō�̎��̃A�h���X
			dst_next);	// �Ō�ɕϊ��ɐ��������ϊ��敶����̗v�f�̎����w���|�C���^
		if(result != std::codecvt_base::ok)
		{
			throw std::runtime_error("Fail Str2WStr()");
		}
		// �S�Ă̕�����ϊ��ł����Ƃ��� src_end == src_next �ɂȂ�
		assert(src_end == src_next);
		// null�^�[�~�l�[�g���Ă���Ȃ��̂ŁA�����ōs��
		*dst_next = L'\0';
		return std::wstring(dst);
	}

	/**
	 * @brief ���C�h��������}���`�o�C�g������ɕϊ�����
	 * @node C++�W�����C�u����std::codecvt���g���Ă���
	 * @param wstr [in] ���C�h������
	 * @param loc [in] ���P�[��
	 * @return �}���`�o�C�g������
	 */
	inline std::string WStr2Str(const std::wstring& wstr, const std::locale& loc)
	{
		typedef std::codecvt<wchar_t, char, std::mbstate_t> CodeCvt;
		const CodeCvt& converter = std::use_facet<CodeCvt>(loc);
		std::mbstate_t state = {0};

		const wchar_t* src = wstr.c_str();
		const wchar_t* src_end  = wstr.c_str() + wstr.length();
		static const int DST_BUF_SIZE = 8;
		// ��VC11�ɂ�DST_BUF_SIZE����̂Ƃ��A
		// ��(dst + DST_BUF_SIZE)�𒴂����A�h���X�ɃA�N�Z�X���Ă��܂���肪���邱�Ƃ��m�F�ł��Ă���B
		// �����l�Ȍ��ۂ��N����ƍ���̂ŁA�o�b�t�@�𑽂߂Ɋm�ۂ���B
		std::vector<char> dst_buf(DST_BUF_SIZE + 5, '\0');
		char* dst = &dst_buf.at(0);
		std::string ret_str;
		while(src != src_end)
		{
			const wchar_t* src_next = 0;
			char* dst_next = 0;
			std::codecvt_base::result result = converter.out(
				state,
				src, src_end, src_next,
				dst, dst + DST_BUF_SIZE, dst_next);
//			assert(dst_next <= dst + DST_BUF_SIZE);	// ��q�̖��̊m�F�p
			switch(result)
			{
			case std::codecvt_base::partial:
			case std::codecvt_base::ok:
				if (dst < dst_next)
				{
					// �����������w�肵��append����̂ŁA
					// ��null�^�[�~�l�[�g�̂��Ƃ͋C�ɂ���K�v���Ȃ�
					ret_str.append(dst, (size_t)(dst_next - dst));
				}
				else
				{
					throw std::runtime_error("Fail WStr2Str()");
				}
				break;
			default:
				throw std::runtime_error("Fail WStr2Str()");
			}
			src = src_next;
		}
		return ret_str;
	}
}
