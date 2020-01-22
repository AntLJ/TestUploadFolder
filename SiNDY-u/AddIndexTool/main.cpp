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
#include "Arguments.h"
#include <arctl/coinitializer.h>
#include <atl2/exception.h>
#include <TDC/useful_headers/japanese_locale.h>

#include "ParseFunction.h"
#include "AddIndex.h"

using namespace std;

enum RESULT:int
{
	SUCCESS   =0, //!< ����I��
	ABEND     =1, //!< �G���[�L��ُ�I��
	EXCEPTION =2, //!< ��O�L��ُ�I��
};

namespace
{
/**
 * @brief �R�}���h���C�������̏�ԂƊJ�n�������o��
 * @param args [in] �R�}���h���C������
 */
inline void writeStart(const Arguments& args)
{
	RUNLOGGER << _T("# ") << AheLoadString(IDS_OPTION_INPUT).GetString() << _T(" : ")
		<< args.m_db << _T("\n");
	RUNLOGGER << _T("# ") << AheLoadString(IDS_OPTION_CONFIG).GetString() << _T(" : ")
		<< args.m_config << _T("\n");
	RUNLOGGER << _T("# ") << AheLoadString(IDS_OPTION_RUNLOG).GetString() << _T(" : ")
		<< args.m_runLog << _T("\n");
	RUNLOGGER << _T("# ") << AheLoadString(IDS_OPTION_ERRLOG).GetString() << _T(" : ")
		<< args.m_errLog << _T("\n");

	// �J�n�������o��
	RUNLOGGER.writeTime(AheLoadString(IDS_LOG_START_TIME).GetString());
}

} // namespace

int _tmain(int argc, _TCHAR* argv[])
{
	// ���P�[���֘A�̕s��h�~
	uh::setJapaneseCharacterTypeFacet();

	// ���s���ʂ��i�[
	RESULT ret = RESULT::SUCCESS;
	try
	{
		const arctl::coinitializer aCoInitializer;
		try
		{
			// �R�}���h���C����������
			Arguments args;
			if(! args.parse(argc, argv))
			{
				cerr << AheLoadStringA(IDS_PARAMETER_HAVE_PROBLEM).GetString() << endl;
				return RESULT::ABEND;
			}
			// �G���[���O�E���s���O���J��
			if( !ERRLOGGER.Initialize( args.m_errLog ) )
			{
				cerr << AheLoadStringA(IDS_FAILED_OPEN_LOG).GetString()
					<< " : FILE[" << uh::toStr(args.m_errLog) << "]" << std::endl;
				return RESULT::ABEND;
			}
			if( !RUNLOGGER.Initialize( args.m_runLog ) )
			{
				cerr << AheLoadStringA(IDS_FAILED_OPEN_LOG).GetString()
					<< " : FILE[" << uh::toStr(args.m_runLog) << "]" << std::endl;
				return RESULT::ABEND;
			}

			// �����J�n���Ԃ����O�ɏo��
			writeStart( args );

			index_def::INDEXINFOMAP indexInfo;
			if( !parser::xmlToIndexDef(args.m_config, indexInfo) )
				return RESULT::ABEND;

			// �ȉ�����t�H���_/�t�@�C��/���C���̐����J�n
			CAddIndex addIndex( args.m_db, indexInfo );
			if( !addIndex.execute() )
				return RESULT::ABEND;
		}
		catch(const _com_error& e)
		{
			throw std::runtime_error(uh::toStr(atl2::stringize(e)));
		}
	}
	catch(const std::exception& e)
	{
		wcerr << uh::toWStr(e.what()) << endl;
		RUNLOGGER.fatal(uh::toTStr(e.what()).c_str());
		ret = RESULT::EXCEPTION;
	}
	return ret;
}
