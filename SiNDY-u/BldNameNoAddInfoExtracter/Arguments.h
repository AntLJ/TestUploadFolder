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

namespace option{
	extern const std::string kBldNameDb;     //!< �����r�����̃e�[�u���̂���DB�̐ڑ�����
	extern const std::string kAddrDb;        //!< �Z��DB�iareacode_master�̂���DB�j
	extern const std::string kAdminDb;       //!< CITY_ADMIN�̂���DB�̐ڑ�������
	extern const std::string kPoiDb;         //!< POI_POINT_ORG�̂���DB�̐ڑ�������
	extern const std::string kBaseAdminList; //!< �s���E�R�[�h�i5���j���X�g
	extern const std::string kRunLog;        //!< ���s���O�t�@�C���̃p�X
	extern const std::string kErrLog;        //!< �G���[���O�t�@�C���̃p�X
	extern const std::string kResult;        //!< ���s���ʃ��X�g�̃p�X
} // namespace arg

  /**
  * @brief �������N���X
  * �P�̃I�v�V�����̏����Ǘ�����
  */
class CArgInfo
{
private:
	CArgInfo();
public:
	/**
	* @brief �R���X�g���N�^
	* @note �����I�v�V�����̓o�^���s��
	* @param arg_ [in] �I�v�V������ 
	* @param desc_ [in] �I�v�V�����̐���
	* @param required_ [in] �K�{�Ȃ�true����
	* @param flag_ [in] �t���O�I�v�V�����Ȃ�true����
	* @param default_ [in] �f�t�H���g�l
	*/
	CArgInfo( const std::string& arg_, const std::string& desc_, bool required_ = false, bool flag_ = false, const std::string& default_ = "" );

	std::string arg;      //!< �I�v�V������
	std::string desc;     //!< ����
	bool required;        //!< �K�{��
	bool flag;            //!< �t���O�I�v�V������
	std::string def_val;  //!< �f�t�H���g�l
};

/**
* @brief �����Ǘ��N���X
* �I�v�V�����S�̂̏����Ǘ�����
*/
class CArgInfos
{
public:
	void add(const CArgInfo& arg);
	bool parse(int argc, TCHAR* argv[]);
	CString getValue( const std::string& arg ) const;
	bool checkArg() const;
	bool hasOption( const std::string& arg ) const;

	std::vector<CArgInfo>::const_iterator begin() const { return m_argInfos.cbegin(); }
	std::vector<CArgInfo>::const_iterator end() const { return m_argInfos.cend(); }

	std::vector<CArgInfo> m_argInfos;
	boost::program_options::variables_map m_args;
};

/**
 * @brief �R�}���h���C�������N���X
 */
class Arguments
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	Arguments(){}

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	* @brief �����Ŏw�肳�ꂽ�l���擾����
	* @param arg [in] ������
	* @return �����Ŏw�肳�ꂽ�l
	*/
	CString getValue( const std::string& arg ) const { return m_args.getValue( arg ); }

	/**
	* @brief �����Ŏw�肳��Ă��邩
	* @note �t���O�����̊m�F�Ɏg�p����
	* @param arg [in] ������
	* @return �����Ŏw�肳��Ă����true
	*/
	bool hasOption( const std::string& arg ) const { return m_args.hasOption( arg ); };

	/**
	* @brief �w�肳��Ă���������擾����
	* @note ���O��W���o�͂ւ̕\���p
	* @return �������A�l
	*/
	CString description() const;

public:
	CArgInfos m_args; //!< �����Q
};
