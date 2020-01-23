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
#include <TDC/sindylib_core/Workspace.h>
#include "Processor.h"
#include "DBReplicater.h"
#include "TableNamesSearcher.h"
#include "Notifier.h"

using namespace sindy;

/**
 * @brief ����PGFGDB�C���|�[�g�N���X
 */
template <typename T>
class PGFGDBImportProcessor : public Processor<T>
{
public:

	PGFGDBImportProcessor(const CWorkspace& workspace, const std::vector<uh::tstring>& paths) : Processor<T>(), m_workspace(workspace), m_paths(paths){};

	virtual ~PGFGDBImportProcessor(){};

	/**
	 * @brief  �C���|�[�g�������s
	 * @param  src  [in]  ���b�V���R�[�h
	 * @retval true:��������
	 */
	virtual bool proc(const T& src);

	/**
	 * @brief  �����Ώۂ̃��[�N�X�y�[�X��ԋp
	 * @retval ���[�N�X�y�[�X
	 */
	const CWorkspace& getWorkspace(){ return m_outputWorkspace; }

private:

	CWorkspace   m_workspace;			//!< �o�͐��DB
	DBReplicater m_dbReplicater;		//!< ���R�[�h�R�s�[�N���X
	std::vector<uh::tstring> m_paths;	//!< PGDB,FGDB�̃p�X�Q

	/**
	 * @brief  ���R�[�h�̃R�s�[�����{����
	 * @param  srcWorkspace  [in]		�R�s�[�����[�N�X�y�[�X
	 * @param  destWorkspace [in/out]   �R�s�[�惏�[�N�X�y�[�X
	 * @retval true:��������
	 */
	bool copyTableData(IWorkspacePtr srcWorkspace, IWorkspacePtr destWorkspace);
};

template <typename T>
bool PGFGDBImportProcessor<T>::proc(const T& src)
{
	uh::tstring mesh(src);

	// ���b�V���R�[�h�ɑΉ�����PGFGDB�t�@�C���p�X���擾
	auto it = std::find_if( m_paths.begin(), m_paths.end(), [mesh](const uh::tstring& str) { return str.find(mesh) != uh::tstring::npos; } );
	
	// ���b�V�����X�g�ɑΉ�����t�@�C����������Ȃ������ꍇ
	if( it == m_paths.end() )
	{
		std::wstringstream ss;
		ss << _T("���b�V�����X�g�ɑΉ�����t�@�C����������܂��� ���b�V���R�[�h : ") <<  mesh;
		std::cerr << uh::toStr(ss.str()) << std::endl;
		notifier::versatile(ss.str(), std::wcout);
		notifier::versatile(ss.str(), std::wcerr);
		return true;
	}

	uh::tstring filePath(*it);

	// ���O
	{
		std::wstringstream ss;
		ss << _T("�C���|�[�g�J�n") <<  _T(" file:") <<  filePath;
		notifier::versatile(ss.str(), std::wcout);
	}

	CWorkspace pInputWorkspace(filePath.c_str());

	// ���R�[�h�̃R�s�[�����{
	if(!copyTableData((IWorkspace*)pInputWorkspace, (IWorkspace*)m_workspace))
		return false;

	// ���O
	{
		std::wstringstream ss;
		ss << _T("�C���|�[�g�I��") <<  _T(" file:") <<  filePath;
		notifier::versatile(ss.str(), std::wcout);
	}

	return true;
}

template <typename T>
bool PGFGDBImportProcessor<T>::copyTableData(IWorkspacePtr srcWorkspace, IWorkspacePtr destWorkspace)
{
	// �f�[�^�Z�b�g�擾
	IEnumDatasetNamePtr ipEnumDataset;
	srcWorkspace->get_DatasetNames(esriDTAny,&ipEnumDataset);
	if ( !ipEnumDataset )
		return false;

	// �R�s�[�Ώۂ̃e�[�u�����擾
	std::vector<uh::tstring> vTableNames;
	TableNamesSearcher searcher;
	searcher.getWorkspaceTableNames(ipEnumDataset, vTableNames);

	// ��t�B���^�[�A�󃊃X�g���쐬
	IQueryFilterPtr	ipFilter(CLSID_QueryFilter);
	std::list<uh::tstring> listColumns;

	// �e�[�u�����ƂɃR�s�[��������
	for( const auto& tableName : vTableNames)
	{
		// ���O
		{
			std::wstringstream ss;
			ss << _T("�C���|�[�g�J�n") <<  _T(" table:") <<  tableName;
			notifier::versatile(ss.str(), std::wcout);
		}

		// �e�[�u���̓��e���R�s�[
		if( !m_dbReplicater.InsertTable(ipFilter, (IWorkspace*)srcWorkspace, tableName, destWorkspace, tableName, listColumns, true) )
		{
			std::wstringstream ss;
			ss << _T("�C���|�[�g���s") <<  _T(" table:") <<  tableName;
			notifier::critical::versatile( ss.str() );
		}

		// ���O
		{
			std::wstringstream ss;
			ss << _T("�C���|�[�g�I��") <<  _T(" table:") <<  tableName;
			notifier::versatile(ss.str(), std::wcout);
		}
	}

	return true;
}
