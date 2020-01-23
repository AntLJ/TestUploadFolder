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

#include "stdafx.h"
#include "CreateEmptyGDB.h"

#include "Utility.h"

void CCreateEmptyGDB::setFormat(const uh::tstring& format)
{
	m_format = format;
}

void CCreateEmptyGDB::setOutput( const uh::tstring& path, OUTPUT_TYPE type)
{
	m_output = path;
	m_outputType = type;
}
void CCreateEmptyGDB::setMeshList( const std::vector<uh::tstring>& meshList )
{
	m_meshList = meshList;
}

bool CCreateEmptyGDB::create()
{
	switch(m_outputType)
	{
	case SDE:
		// SDE�ɂ͉������Ȃ��Ɣ��f���ĉ������Ȃ�
		m_targetPathList.push_back( m_output );
		return true;
		break;
	case LOCAL:
		{
			boost::filesystem::path outPath(m_output);
			return createFile(outPath.stem().c_str(), outPath.extension().c_str(), outPath.parent_path().c_str());
		}
		break;
	case FOLDER:
		return createLevelFolder( m_output );
		break;
	default:
		assert(false);
		return false;
		break;
	}
}

bool CCreateEmptyGDB::createFile(const uh::tstring& filename, const uh::tstring& ext, const uh::tstring& basePath)
{
	boost::filesystem::path bfPath(basePath);
	bfPath /= filename + ext;

	// ���łɃt�@�C���Ƃ��đ��݂���Ȃ牽�����Ȃ�
	if( util::isFile(bfPath.c_str()) )
	{
		m_targetPathList.push_back( bfPath.c_str() );
		return true;
	}

	IWorkspaceFactoryPtr workspaceFactory;
	if( util::eq( ext, _T(".mdb") ) )
		workspaceFactory = IWorkspaceFactoryPtr( CLSID_AccessWorkspaceFactory );
	else if( util::eq( ext, _T(".gdb") ) )
		workspaceFactory = IWorkspaceFactoryPtr( CLSID_FileGDBWorkspaceFactory );
	else
	{
		// �R�}���h���C��������͂őΉ��O�̊g���q�̓G���[�Ƃ��Ă���̂ł����ɂ͂��Ȃ�
		assert(false);
		return false;
	}
	IPropertySetPtr propertySet( CLSID_PropertySet );
	IWorkspaceNamePtr worksName;
	if( workspaceFactory->Create(
		CComBSTR(basePath.c_str()), CComBSTR(filename.c_str()),
		propertySet, 0, &worksName ) != S_OK)
	{
		RUNLOGGER.error(IDS_FAILED_CREATE_GDB, bfPath.c_str(), _T(""), _T(""));
		return false;
	}
	m_targetPathList.push_back( bfPath.c_str() );
	return true;
}

bool CCreateEmptyGDB::createLevelFolder(const uh::tstring& folderPath)
{
	if( m_meshList.empty() ) return true;
	boost::filesystem::path bPath(folderPath);
	unsigned long meshDigit = m_meshList.begin()->size();

	// ���b�V�����X�g����͂��āA�t�H���_�E�t�@�C�������p�̃c���[���쐬
	FolderTree root;
	for( const auto& meshcode : m_meshList )
	{
		switch(meshDigit)
		{
		case 10:
			{
				root.m_children.emplace_back();
				auto& child = root.m_children.back();
				child.m_folderName = meshcode.substr(0, 6);
				child.m_children.emplace_back();
				auto& grandchild = child.m_children.back();
				grandchild.m_folderName = meshcode.substr(0, 8);
				grandchild.m_fileName = meshcode;
			}
			break;
		case 8:
			{
				root.m_children.emplace_back();
				auto& child = root.m_children.back();
				child.m_folderName = meshcode.substr(0, 4);
				child.m_fileName = meshcode;
			}
			break;
		default:
			// ���b�V�����X�g�Ǎ��ݎ��ɐ������|���Ă���̂ł����ɂ͂��Ȃ�
			assert(false);
			return false;
			break;
		}
	}
	return createFilesFromTree(root, folderPath);
}

bool CCreateEmptyGDB::createFilesFromTree(const FolderTree& node, const boost::filesystem::path& rootPath)
{
	boost::filesystem::path bsPath(rootPath);
	bsPath /= node.m_folderName;

	// �t�H���_���̎w�肪����ꍇ�A�쐬
	if(!node.m_folderName.empty())
	{
		if( !boost::filesystem::exists( bsPath ) )
		{
			if( !boost::filesystem::create_directory( bsPath ) )
			{
				tstringstream ss;
				ss << _T("FOLDER[") << bsPath.c_str() << _T("]");
				RUNLOGGER.error(IDS_FAILED_CREATE_FOLDER, _T(""), _T(""), _T(""), ss.str());
				return false;
			}
		}
	}
	// �q������Ȃ�쐬
	bool bSuccess = true;
	for(const auto& child : node.m_children)
		bSuccess &= createFilesFromTree(child, bsPath);

	// �t�@�C���w�肪����Ȃ璼���ɍ쐬
	if(node.m_fileName)
	{
		bSuccess &= createFile( node.m_fileName.get(), _T(".")+m_format, bsPath.c_str() );
	}
	return bSuccess;
}
