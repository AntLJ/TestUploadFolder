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

#include "StdAfx.h"
#include "Arguments.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/text_util.h>
#include <boost/filesystem.hpp>
#include <sindy/workspace.h>
#include <sindy/schema/reference.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

Arguments::Arguments():
	m_meshLayerName(sindy::schema::citymesh::kTableName),
	m_outputType(UNKOWN)
{}

bool Arguments::parse(int argc, _TCHAR* argv[])
{
	const char * const k_input					= "input";
	const char * const k_output					= "output";
	const char * const k_mesh                   = "mesh";
	const char * const k_mesh_db                = "mesh_db";
	const char * const k_mesh_layer_name        = "mesh_layer_name";
	const char * const k_config                 = "config";
	const char * const k_format                 = "format";
	const char * const k_runlog					= "runlog";
	const char * const k_errlog					= "errlog";

	po::options_description desc("How to use");

	try{
		desc.add_options()
			(k_input,  tvalue<tstring>(&m_input),     AheLoadStringA(IDS_OPTION_REQUIRED_TAG)+AheLoadStringA(IDS_OPTION_INPUT))
			(k_output, tvalue<tstring>(&m_output),    AheLoadStringA(IDS_OPTION_REQUIRED_TAG)+AheLoadStringA(IDS_OPTION_OUTPUT))
			(k_mesh,   tvalue<tstring>(&m_meshList),  AheLoadStringA(IDS_OPTION_OPTIONAL_TAG)+AheLoadStringA(IDS_OPTION_MESH))
			(k_mesh_db,tvalue<tstring>(&m_meshDB),    AheLoadStringA(IDS_OPTION_OPTIONAL_TAG)+AheLoadStringA(IDS_OPTION_MESHDB))
			(k_mesh_layer_name, tvalue<tstring>(&m_meshLayerName), AheLoadStringA(IDS_OPTION_OPTIONAL_TAG)+AheLoadStringA(IDS_OPTION_MESH_LAYERNAME))
			(k_config, tvalue<tstring>(&m_config),    AheLoadStringA(IDS_OPTION_OPTIONAL_TAG)+AheLoadStringA(IDS_OPTION_CONFIG))
			(k_format, tvalue<tstring>(&m_dbFormat),  AheLoadStringA(IDS_OPTION_OPTIONAL_TAG)+AheLoadStringA(IDS_OPTION_DB_FORMAT))
			(k_runlog, tvalue<tstring>(&m_runLog),    AheLoadStringA(IDS_OPTION_REQUIRED_TAG)+AheLoadStringA(IDS_OPTION_RUNLOG))
			(k_errlog, tvalue<tstring>(&m_errLog),    AheLoadStringA(IDS_OPTION_REQUIRED_TAG)+AheLoadStringA(IDS_OPTION_ERRLOG))
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		good &= checkParam( k_input,  m_input );
		good &= checkParam( k_output, m_output );
		good &= checkParam( k_runlog, m_runLog );
		good &= checkParam( k_errlog, m_errLog );

		if( ! good )
		{
			cerr << AheLoadStringA(IDS_REQUIRED_PARAMETER).GetString() << endl;
			cerr << desc << endl;
			return false;
		}
		// formatの指定があるなら小文字にしておく
		std::transform( m_dbFormat.begin(), m_dbFormat.end(), m_dbFormat.begin(), ::tolower );

		// ファイルであるべきものがファイルになっているかチェック
		if( !m_config.empty() )
			good &= checkFile( k_config, m_config );
		if( !m_meshList.empty() )
			good &= checkFile( k_mesh, m_meshList );

		if( util::isDirectory(m_output) )
		{
			m_outputType = FOLDER;
			// outputがフォルダならmeshリストの指定は必須
			if( m_meshList.empty() )
			{
				good = false;
				cerr << AheLoadStringA(IDS_NECESSARY_MESHLIST_TO_FOLDER).GetString() << endl;
			}
			// formatの指定がないならmdbが指定されたものとして扱う
			if( m_dbFormat.empty() )
			{
				m_dbFormat = _T("mdb");
			}
			// 不正な指定はエラー
			else if( !( util::eq( m_dbFormat, _T("mdb") ) || util::eq( m_dbFormat, _T("gdb") ) ) )
			{
				good = false;
				cerr << AheLoadStringA(IDS_NONE_SUPPORT_FORMAT).GetString() << endl;
			}
		}
		else if( util::isFile(m_output) )
		{
			m_outputType = LOCAL;
			// outputが存在しているファイルなら、拡張子判定
			good &= checkFormat(m_output);
		}
		else
		{
			// フォルダ以外が指定されている場合、試しに接続
			IWorkspacePtr tempWorkspace = sindy::create_workspace(m_output.c_str());
			// 接続できない = 存在しないファイルと思われるのでファイル用の判定
			// (存在しないフォルダを指定している場合もこの判定になる)
			if(!tempWorkspace)
			{
				m_outputType = LOCAL;
				good &= checkFormat(m_output);
			}
			else
			{
				m_outputType = SDE;
				// ディレクトリでない&ファイルでない&接続できた場合、それはSDEのはず
				esriWorkspaceType type = esriFileSystemWorkspace;
				tempWorkspace->get_Type(&type);
				if(type!=esriRemoteDatabaseWorkspace)
				{
					assert(false);
					good = false;
					cerr << AheLoadStringA(IDS_UNKOWN_DB).GetString() << endl;
				}
			}
		}
		switch(m_outputType)
		{
		case SDE:
			// SDEの場合、メッシュ出力機能は使用できない
			if( !m_meshDB.empty() )
			{
				good = false;
				cerr << AheLoadStringA(IDS_CANT_APPOINT_MESHDB).GetString() << endl;
			}
		case LOCAL:
			// ファイル・SDEが指定されている場合、formatは指定禁止
			if( !m_dbFormat.empty() )
			{
				good = false;
				cerr << AheLoadStringA(IDS_CANT_APPOINT_FORMAT).GetString() << endl;
			}
			break;
		default:
			break;
		}

		if(m_meshList.empty())
		{
			// mesh_dbが指定されているのにmeshがないのは不正
			// (出力先がSDEの場合はmeshを指定できないので、このエラー文は不要)
			if( !m_meshDB.empty() && m_outputType!=SDE )
			{
				good = false;
				cerr << AheLoadStringA(IDS_NECESSARY_MESHLIST_TO_CREATE).GetString() << endl;
			}
		}
		else
		{
			// mesh_dbの指定がなく、outputがフォルダでない場合、meshがあるのは不正
			if( m_meshDB.empty() && m_outputType!=FOLDER )
			{
				good = false;
				cerr << AheLoadStringA(IDS_MESHLIST_IS_UNNECESSARY).GetString() << endl;
			}
		}
		if( ! good )
		{
			cerr << desc << endl;
			return false;
		}
	}
	catch(const std::exception& e){
		cerr << e.what() << endl;
		cerr << desc << endl;
		return false;
	}
	return true;
}

bool Arguments::checkParam( const char* const optName, const tstring& optValue, bool bFile /* = false */ ) const
{
	if( optValue.empty() ){	
		cerr << optName << AheLoadStringA(IDS_NONE_NECESSARY_PARAMETER).GetString() << endl;
		return false;
	}
	if( !bFile ) return true;
	return checkFile( optName, optValue );
}
bool Arguments::checkFile( const char* const optName, const uh::tstring& optValue ) const
{
	if( ! isFile( optValue.c_str() ))
	{
		cerr << toStr( optValue ) << AheLoadStringA(IDS_IS_NOT_FILE).GetString() << endl;
		return false;
	}
	return true;
}

bool Arguments::checkFormat( const uh::tstring& path ) const
{
	boost::filesystem::path bPath(path.c_str());
	if( !( util::eq( bPath.extension().c_str(), _T(".gdb") ) || util::eq( bPath.extension().c_str(), _T(".mdb") ) ) )
	{
		cerr << AheLoadStringA(IDS_NONE_SUPPORT_FILE_FORMAT).GetString() << endl;
		return false;
	}
	return true;
}
