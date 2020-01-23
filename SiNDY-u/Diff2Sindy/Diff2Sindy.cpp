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
#include "Diff2Sindy.h"
#include "PathFinder.h"
#include "ListProcessor.h"
#include "PGFGDBImportProcessor.h"
#include "PGFGDBExportProcessor.h"
#include "PGFGDBConvertProcessor.h"
#include "PGFGDBMergeProcessor.h"
#include "MeshClipProcessor.h"
#include "LotClipProcessor.h"
#include "PPLProcessor.h"
#include "PathUtility.h"

#include <arctl/coinitializer.h>
#include <directory_util.h>
#include <TDC/sindylib_core/Workspace.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <future>

using namespace sindy;

namespace sindy {
	extern bool g_bCheckUpdatedByArcObjects;
#ifdef _DEBUG
	extern bool g_bTraceUpdate;
#endif // ifdef _DEBUG
	extern bool g_bOutputTraceUpdate;
	extern bool g_bCreateAllStoreInfo;
}

/**
 * @brief taregt �� num �ɕ�������
 *
 * target�̑�����num�ɂ݂��Ȃ��ꍇ�́A�P���ɕ�������܂�
 * @param target [in] �����Ώ�
 * @param num [in] ������
 * @return �������ꂽ���̂��i�[���ꂽ�R���e�i
 */
template <typename T>
std::vector<std::vector<T>> split( const std::vector<T>& target, size_t num )
{
	std::vector<std::vector<T>> ret;

	if( 1 > num || target.empty() )
		return ret;

	size_t target_count = target.size();
	// ��������������菭�Ȃ��Ȃ�A�������𑍐��ɍ��킹��
	if( num > target_count )
		num = target_count;
	size_t split_count = target_count / num;
	std::vector<T>::const_iterator start = target.begin();
	size_t remainder_count = target_count;
	for( size_t i = 0; i < num; ++i )
	{
		if( 0 == remainder_count )
			break;

		size_t copy_count = split_count;
		if( split_count > remainder_count )
			copy_count = remainder_count;

		ret.push_back( std::vector<T>() );
		std::copy( start, start + copy_count, std::back_inserter( *ret.rbegin() ) );
		start += copy_count;
	}
	return ret;
}

// ������
bool CDiff2Sindy::init()
{
	g_bCheckUpdatedByArcObjects = false; // sindylib�ł̍X�V�̗L���̓L���b�V���쐬�O���l�����Ȃ�
#ifdef _DEBUG
	g_bTraceUpdate = false; // ���R�[�h�X�V���̃f�o�b�O�g���[�X�͍s��Ȃ�
#endif // ifdef _DEBUG
	g_bOutputTraceUpdate = false; // ���R�[�h�X�V���̕W���o�͂ւ̃g���[�X�͍s��Ȃ�
	g_bCreateAllStoreInfo = false; // �G���[�I�u�W�F�N�g�̓G���[��������̂̂�

	// ADAM�̃J���g�l�C���Ή��ŕK�v�Ȃ̂Œǉ�
	// ����ł͍���ꍇ�̓I�v�V�����Ŏw��ł���悤�ɂ���K�v������
	g_bStoreShapeJudge_PointZ_Exact = true; // Store()���̌`��ύX����ŁAZ�l�̋��e�l�𔻒肵�Ȃ�

	return true;
}

// ���s
bool CDiff2Sindy::execute()
{
	// ������
	if( !init() )
		return false;

	// ���O�\��
	outputExecInfo();

	// �R���o�[�g�������s���i�X���b�h���s�Ή��j
	if( !m_args.m_mapDefFile[s2sModeConvert].empty() )
	{
#ifndef S2S_NOUSETHREAD
		bool ret = true;
		// ���b�V�����X�g���X���b�h�����ɕ�����
		auto listMeshes = split( m_args.m_meshes, m_args.m_thread );
		//  �X���b�h���s
		std::list<std::future<bool>> threads;
		for( const auto& meshes : listMeshes )
			threads.push_back( std::async( std::launch::async, [this, &meshes]{ return convertDB(meshes); } ) );
		// �I���m�F
		for( auto& th : threads )
		{
			if( !th.get() )
				ret = false;
//			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		if( ! ret )
			return false;
#else
		if( !convertDB(m_args.m_meshes) )
			return false;
#endif // ifndef S2S_NOUSETHREAD
	}

	// �אڃ}�[�W�������s���i�X���b�h���s��Ή��j
	if( !m_args.m_mapDefFile[s2sModeMerge].empty() )
	{
		if( !mergeDB(m_args.m_meshes) )
			return false;
	}

	// �N���b�s���O�������s���i�X���b�h���s��Ή��j
	if( !m_args.m_mapDefFile[s2sModeClip].empty() )
	{
		if( !clipDB(m_args.m_meshes) )
			return false;
	}

	return true;
}

// �����̃��b�V���ɋ�؂�ꂽ�e�[�u���A�t�B�[�`������ɂ���
bool CDiff2Sindy::convertDB( const std::vector<uh::tstring>& meshes )
{
#ifndef S2S_NOUSETHREAD
	const arctl::coinitializer aCoInitializer;
	{
		IArcGISLocalePtr ipLocal(CLSID_ArcGISLocale);
		ipLocal->SetThreadLocale();
#endif // ifndef S2S_NOUSETHREAD
		ConvertingDefinitionManager mgr;
		mgr.parse(uh::toTStr(m_args.m_mapDefFile[s2sModeConvert])); // �R���o�[�g���{��`�N���X���쐬

		if( !mgr )
			return true; // ��`���Ȃ��Ȃ�I��
		// 1 ���b�V�� �̏����N���X���쐬
		std::shared_ptr<Processor<uh::tstring>> processor;

		// output,input���t�H���_���ǂ����ŏ����N���X�𕪂���
		if( isDirectory( m_args.m_output ) && !isDirectory( m_args.m_input ) )
			processor = std::shared_ptr<PGFGDBExportProcessor<uh::tstring>>(new PGFGDBExportProcessor<uh::tstring>( CWorkspace( m_args.m_input.c_str() ), m_args.m_output )); // TODO: ������
		else
			processor = std::shared_ptr<PGFGDBConvertProcessor<uh::tstring>>(
			new PGFGDBConvertProcessor<uh::tstring>( m_args.m_input, m_args.m_output, mgr, m_args));

		// ���[�v�����N���X���쐬����
		ListProcessor<uh::tstring> listProcessor( meshes, processor );

		// ���[�v���O����
		if(!listProcessor.doPreprocessing())
			return false;

		// ���[�v�������s
		if(!listProcessor.doLoopProcessing())
			return false;

		// �ꊇ�������s
		if(!listProcessor.doBulkProcessing())
			return false;

		// ���[�v���㏈��
		if(!listProcessor.doPostprocessing())
			return false;
#ifndef S2S_NOUSETHREAD
	}
#endif // ifndef S2S_NOUSETHREAD
	return true;
}

// �אڃ}�[�W���s��
bool CDiff2Sindy::mergeDB( const std::vector<uh::tstring>& meshes )
{
	MergingDefinitionManager mgr;
	mgr.parse(uh::toTStr(m_args.m_mapDefFile[s2sModeMerge])); // �}�[�W���{��`�N���X���쐬

	if( !mgr )
		return true; // ��`���Ȃ��Ȃ�I��

	CWorkspace outPutWorkspace(m_args.m_output.c_str());

	// 1���b�V�����Ƃ̃}�[�W�����N���X���쐬
	std::shared_ptr<Processor<uh::tstring>> processor;
	processor = std::shared_ptr<PGFGDBMergeProcessor<uh::tstring>>(new PGFGDBMergeProcessor<uh::tstring>( outPutWorkspace,
																											m_args.m_meshDB,
																											m_args.m_meshLayerName,
																											m_args.m_meshTableOwner,
																											mgr,
																											m_args));

	// ���[�v�����N���X���쐬����
	ListProcessor<uh::tstring> listProcessor( meshes, processor );

	// ���[�v���O����
	if(!listProcessor.doPreprocessing())
		return false;

	// ���[�v�������s
	if (!listProcessor.doLoopProcessing())
		return false;

	// ���[�v���㏈��
	if(!listProcessor.doPostprocessing())
		return false;

	return true;
}

// �N���b�s���O���s��
bool CDiff2Sindy::clipDB( const std::vector<uh::tstring>& meshes )
{
	ClippingDefinitionManager mgr;
	mgr.parse(uh::toTStr(m_args.m_mapDefFile[s2sModeClip])); // �}�[�W���{��`�N���X���쐬

	if( !mgr )
		return true; // ��`���Ȃ��Ȃ�I��

	CWorkspace outPutWorkspace(m_args.m_output.c_str());

	// 1���b�V�����Ƃ̃N���b�s���O�����N���X���쐬
	std::shared_ptr<Processor<uh::tstring>> processor;
	switch( mgr.getClipType() )
	{
	case clipTypeLot:
		processor = std::make_shared<LotClipProcessor<uh::tstring>>( outPutWorkspace, mgr, meshes );
		break;
	case clipTypeMesh:
	default:
		processor = std::make_shared<MeshClipProcessor<uh::tstring>>( outPutWorkspace, mgr );
		break;
	}

	// ���[�v�����N���X���쐬����
	ListProcessor<uh::tstring> listProcessor( meshes, processor );

	// ���[�v���O����
	if(!listProcessor.doPreprocessing())
		return false;

	// ���[�v�������s
	if (!listProcessor.doLoopProcessing())
		return false;

	// ���[�v���㏈��
	if(!listProcessor.doPostprocessing())
		return false;

	return true;

}

// �����J�n���O��\��
void CDiff2Sindy::outputExecInfo() const
{
	using namespace std;

	wcout << L"#SINDYSTDLOG" << endl;
	wcout << L"# " << uh::toWStr( m_InternalName.GetString() )
		  << L" F:" << uh::toWStr( m_FileVersion.GetString() )
		  << L" P:" << uh::toWStr( m_ProductVersion.GetString() ) << endl;
	wcout << L"# �C���v�b�gDB��� : " << m_args.m_input << endl;
	wcout << L"# �A�E�g�v�b�gDB��� : " << m_args.m_output << endl;
	wcout << L"# �C���v�b�g���I�[�i�[�� : " << m_args.m_input_owner << endl;
	wcout << L"# �A�E�g�v�b�g���I�[�i�[�� : " << m_args.m_output_owner << endl;
	wcout << L"# ���b�V��/���X�g/���b�V���t�@�C�� : " << m_args.m_mesh_list_str << endl;
	wcout << L"# ���b�V��DB : " << m_args.m_meshDB << endl;
	wcout << L"# ���b�V��DB�I�[�i�[�� : " << m_args.m_meshTableOwner << endl;
	wcout << L"# ���b�V�����b�V��DB�Q�ƃ��C���� : " << m_args.m_meshLayerName << endl;
	wcout << L"# �}�[�W��`�t�@�C���p�X : " << m_args.m_mapDefFile[s2sModeMerge] << endl;
	wcout << L"# �R���o�[�g��`�t�@�C���p�X : " << m_args.m_mapDefFile[s2sModeConvert] << endl;
//	wcout << L"# ���n�n�ϊ���`�t�@�C���p�X : " << m_args.m_mapDefFile[s2sModeCrdcnv] << endl;
	wcout << L"# �N���b�s���O��`�t�@�C���p�X : " << m_args.m_mapDefFile[s2sModeClip] << endl;
	wcout << L"# ���s���O�t�@�C���p�X : " << m_args.m_runLog << endl;
	wcout << L"# �G���[���O�t�@�C���p�X : " << m_args.m_errLog << endl;
	wcout << L"# ���s�X���b�h�� : " << m_args.m_thread << endl;
	for( const auto& val : m_args.m_values )
		wcout << L"# �I�v�V����VALUE : " << uh::toWStr( val ) << endl;
	wcout << L"# �}���`�p�[�g���� : " << ( m_args.m_dissolveMulti ? L"TRUE" : L"FALSE" ) << endl;
	wcout << L"# �J�n���� : " << boost::lexical_cast< wstring >( boost::posix_time::second_clock::local_time() ) << endl;

	wcerr << L"#SINDYSTDLOG" << endl;
}