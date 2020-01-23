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
 * @brief ���[�v���� �����N���X
 * @note  ListProcessor���́A���̃T�u�N���X�ɖ{�N���X�����������N���X��n��
 * �@�@�@ ���[�v���񂷑z��
 */
template <typename T>
class Processor
{
public:
	Processor<T>(){};
	virtual ~Processor<T>(){};

	/**
	 * @brief  �����������\�b�h
	 * @param  src  [in]	���[�v�����œn�����l
	 * @retval
	 */
	virtual bool proc(const T& src){return true;}

	/// @brief ���[�v�����O�������\�b�h
	virtual bool preprocess(){ return true; }

	/// @brief ���[�v����
	virtual bool postprocess(){ return true; }

	/** 
	 * @brief  �ꊇ�����������\�b�h
	 * @param  srcs [in]  �����ΏیQ
	 * @retval 
	 */
	virtual bool bulkproc(const std::vector<T>& srcs){return true;}

private:

};
