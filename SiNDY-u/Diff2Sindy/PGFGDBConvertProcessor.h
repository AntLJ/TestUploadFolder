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
#include <Workspace.h>
#include "ConvertingDefinitionManager.h"
#include "Processor.h"
#include "DBReplicater.h"
#include "Editor.h"

namespace sindy {
	class CTableContainer;
	typedef boost::shared_ptr<CTableContainer> CSPTableContainer;
}
/// �t�B�[�`���̓�d�����h�~�N���X
class RowMutex
{
public:
	RowMutex() : m_bEnable(false){}
	/**
	* @brief ���삳���邩�ǂ���
	* @param enabled [in] ���삳����ꍇ��true
	*/
	void enable( bool enabled ){ m_bEnable = enabled; }
	/**
	* @brief �Ώۃt�B�[�`����o�^����
	* @param table [in] �e�[�u����
	* @param oid [in] OBJECTID
	* @param isSrc [in] �\�[�X����ID�̏ꍇ�ɂ�true
	* retval true �o�^�ł����i�̂ŏ�������OK�j
	* retval false �o�^�ł��Ȃ������i���ɏ����ς݂Ȃ̂ŏ����s�j
	*/
	bool regist( const uh::tstring& table, long oid, bool isSrc ) const;
private:
	bool m_bEnable; //!< ���삳���邩�ǂ����i�f�t�H���g�Ffalse�j
};

// first:�e�[�u�����ASRC�̃t�B�[���h���̃y�A�Asecond:(first:SRC���t�B�[���h�̒l�Asecond:DST�����R�[�h)
// DST�����t�B�[���h�̒l�݂̂Ƃ��Ă��Ȃ��̂́A���̉��O�ł�OBJECTID���擾�ł��Ȃ�����
typedef std::map<std::pair<uh::tstring, uh::tstring>, std::map<CComVariant, sindy::CSPRowBase>> RelationValues;

/**
 * @brief ����PGFGDB�R���o�[�g�N���X
 */
template <typename T>
class PGFGDBConvertProcessor : public Processor<T>
{
public:

	PGFGDBConvertProcessor(const uh::tstring& in, const uh::tstring& out, const ConvertingDefinitionManager& manager, const Arguments& args) : 
		Processor<T>(), 
		m_strInput(in), 
		m_manager(manager),
		m_args(args)
	{
		m_input.Workspace().m_bTrace = false;
		m_output.Workspace().m_bTrace = false;
		if( !isDirectory(in) )
		{
			m_input.Workspace().Connect(in.c_str());
			// 1��1�̃R���o�[�g�����̏ꍇ�̓��b�V�����E�t�B�[�`���̏�����
			// �d�����Ȃ��悤�ɂ���
			m_limitter.enable(true);
		}
		m_output.Workspace().Connect(out.c_str());
	}

	virtual ~PGFGDBConvertProcessor(){}

	/**
	 * @brief  �R���o�[�g�������s
	 * @param  src  [in]  ���b�V���R�[�h
	 * @retval true:��������
	 */
	bool proc(const uh::tstring& src) override;

	/// ���[�v�����O�������s
	bool preprocess() override;

	/// ���[�v�����㏈�����s
	bool postprocess() override;

	/**
	 * @brief  �ꊇ�R���o�[�g�������s
	 * @param  srcs  [in] ���b�V���R�[�h
	 * @retval true:��������
	 */
	bool bulkproc(const std::vector<uh::tstring>& srcs) override;

private:
	/**
	* @brief �t�H���_�w��̏ꍇ�̃��b�V���P�ʂ�DB�p�X���擾����
	* @param mesh [in] ���b�V���R�[�h
	* @return PGDB�t�@�C���p�X/FGDB�t�H���_�p�X
	*/
	uh::tstring GetWorkspaceString( const uh::tstring& mesh ) const;

	/**
	 * @brief �R���o�[�g�������C������
	 * @param hasError [out] �G���[���o���ꍇ�́Atrue���Ԃ�
	 * @param mesh [in] �����Ώۃ��b�V�� �w�肪�Ȃ��ꍇ�́Abulkexe��TRUE�̃e�[�u����S���ꊇ����
	 */
	void ConvertTables(bool& hasError, const uh::tstring& mesh = _T(""));

	/**
	 * @brief �e�[�u���P�̂̃R���o�[�g�������s���B���̉��͂��̊֐��ł͍s���Ȃ��B
	 * @param table [in] �X�V�Ώۃe�[�u����
	 * @param dstT  [in, out] �X�V�Ώۃe�[�u��
	 * @param mesh  [in] �����Ώۃ��b�V�� �w�肪�Ȃ��ꍇ�́Abulkexe��TRUE�̃e�[�u����S���ꊇ����
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool ConvertTable(const std::pair<uh::tstring,IndexList>& table, sindy::CSPTableContainer& dstT, const uh::tstring& mesh);

private:
	uh::tstring m_strInput; //!< ���͐�w�蕶����
	CEditor   m_input, m_output;		//!< ���o�͐��DB
	DBReplicater m_dbReplicater;		//!< ���R�[�h�R�s�[�N���X
	ConvertingDefinitionManager m_manager; //!< �R���o�[�g�Ώے�`�N���X
	Arguments m_args; //!< �����N���X
	RowMutex m_limitter; //!< �t�B�[�`����d�����h�~�p
	std::map<uh::tstring, std::map<uh::tstring, uh::tstring>> m_relationFields; //!< first:�e�[�u�����Asecond:�X�V���ɃL���b�V�����Ă����O���L�[�t�B�[���h��(SRC, DST)
	RelationValues m_relationValues;           //!< �X�V��̊O���L�[�̒l���i�[���Ă���}�b�v
};
