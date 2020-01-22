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
	extern const std::string kTargetDb;      //!< �X�V�Ώۂ̃e�[�u���̂���DB�̐ڑ�����
	extern const std::string kTargetTable;   //!< �X�V�Ώۂ̃e�[�u����
	extern const std::string kBuildingDB;    //!< �Q�Ƃ���BUILDING�̂���DB�̐ڑ�����
	extern const std::string kAddrDB;        //!< �Q�Ƃ���Z��DB�ioracle�j
	extern const std::string kXyUser;        //!< �Z��DB��XY�t�^���[�U�ixyYYMM_all�j
	extern const std::string kReleaseUser;   //!< �Z��DB�̃����[�X��Ɨp���[�U�iyYYMM�j
	extern const std::string kTargetList;    //!< ���͗p�w��������X�g�t�@�C��
	extern const std::string kRunLog;        //!< ���s���O�t�@�C��
	extern const std::string kErrLog;        //!< �G���[���O�t�@�C��
} // namespace arg

class CArgInfo
{
private:
	CArgInfo();
public:
	CArgInfo( const std::string& arg_, const std::string& desc_, bool required_ = false, bool flag_ = false, const std::string& default_ = "" );
	std::string arg;      //!< �I�v�V������
	std::string desc;     //!< ����
	bool required;        //!< �K�{��
	bool flag;            //!< �t���O�I�v�V������
	std::string def_val;  //!< �f�t�H���g�l
};

class CArgInfos
{
public:
	void add(const CArgInfo& arg);
	bool parse(int argc, TCHAR* argv[]);
	CString getValue( const std::string& arg ) const;
	bool chekArg() const;
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
