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

#include <TDC/useful_headers/boost_program_options.h>
namespace po = boost::program_options;

/**
* @brief  �����i�[�p�R���e�i
*/
struct Arguments
{
	uh::tstring convMode;       //!< �I�v�V�����Fcity,addr,all
	uh::tstring transResult;    //!< �|�󌋉ʃt�H���_
	uh::tstring out;            //!< �ϊ����ʃt�H���_
	uh::tstring runLog;         //!< ���s���O�t�@�C��
	uh::tstring errLog;         //!< �G���[���O�t�@�C��
};

/**
 * @brief  �����^�C�v
 */
enum argumentType
{
	kConvMode       = 1,
	kTransResult    = 2,
	kOut            = 3,
	kRunLog         = 4,
	kErrLog         = 5,
};

/**
 * @class  Argument
 * @brief  �R�}���h���C�������̏���
 */
class Argument
{
public:
	/**
	* @brief  �R���X�g���N�^
	*/
	Argument(void) {};
	
	/**
	* @brief  �f�X�g���N�^
	*/
	virtual ~Argument(void) {};

	/**
	* @brief  �K�v�ȃI�v�V������񂪂��邩���`�F�b�N����
	* @param  [in] vm           ������map
	* @param  [in] option       �I�v�V��������
	* @param  [in] desctiption  �I�v�V�����̋L�q
	* @retval true              �K�v�ȃI�v�V��������
	* @retval false             �K�v�ȃI�v�V�����Ȃ�
	*/
	bool checkParameter(const po::variables_map& vm, LPCSTR option, LPCSTR desctiption) const;

	/**
	* @brief  �R�}���h���C�������̉��
	* @param  [in] argc         �R�}���h���C�������̐�
	* @param  [in] argv         �R�}���h���C�������̓��e
	* @param  [in] runLogger    ���s���O�t�@�C��
	* @param  [in] errLogger    �G���[���O�t�@�C��
	* @retval true              ����
	* @retval false             ���s
	*/
	bool setParameter(int argc, TCHAR** argv, FileController& runLogger, FileController& errLogger);

	/**
	* @brief  �����̎擾
	* @param  [in] type         �����^�C�v
	* @return �����̓��e
	*/
	const CStringW& getValue(argumentType type) const;

	CStringW   m_convMode;      //!< �I�v�V�����Fcity,addr,all
	CStringW   m_transResult;   //!< �|�󌋉ʃt�H���_
	CStringW   m_out;           //!< �ϊ����ʃt�H���_
	CStringW   m_runLog;        //!< ���s���O�t�@�C��
	CStringW   m_errLog;        //!< �G���[���O�t�@�C��
};