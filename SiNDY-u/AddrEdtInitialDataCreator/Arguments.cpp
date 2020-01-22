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
#include "Utility.h"
#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");

	try{
		desc.add_options()
			(arg::k_imp_type,     tvalue<tstring>(),  "[必須]インポートタイプ")
			(arg::k_edt_addr_db,  tvalue<tstring>(),  "[必須]SiNDY（住所・編集）接続先")
			(arg::k_run_log,      tvalue<tstring>(),  "[必須]実行ログ")
			(arg::k_err_log,      tvalue<tstring>(),  "[必須]エラーログ")
			(arg::k_old_addr_db,  tvalue<tstring>(),  "[任意]SiNDY（旧住所・編集）接続先")
			(arg::k_gs_addr_db,   tvalue<tstring>(),  "[任意]GS納品データ(住所）接続先")
			(arg::k_citylist,     tvalue<tstring>(),  "[任意]市区町村コードリスト")
			(arg::k_extcode_list, tvalue<tstring>(),  "[任意]拡張コードリスト")
			;

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// 必須パラメータのチェック
		bool good = true;
		CString impType;
		good &= CheckImpTypeParam( vm, arg::k_imp_type, impType );
		good &= CheckParam( vm, arg::k_edt_addr_db, m_edtAddrDb );
		good &= CheckParam( vm, arg::k_run_log, m_runLog );
		good &= CheckParam( vm, arg::k_err_log, m_errLog );

		// 実行タイプ別の必須パラメータのチェック
		// imp_typeオプションのチェックでエラーの場合は実施しない
		switch(m_impType)
		{
		case Utility::ExecuteType::PlaceNameRep :
			good &= CheckParam( vm, arg::k_old_addr_db, m_oldAddrDb );
			break;
		case Utility::ExecuteType::AddrPoly :
		case Utility::ExecuteType::AddrPoint :
		case Utility::ExecuteType::AddrAll :
			good &= CheckParam( vm, arg::k_gs_addr_db, m_gsAddrDb );
			good &= CheckParam( vm, arg::k_citylist, m_cityList, true );
			good &= CheckParam( vm, arg::k_extcode_list, m_extcodeList, true );
			break;
		default:
			break;
		}

		if( ! good ){
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

bool Arguments::CheckImpTypeParam(
		const po::variables_map& vm,
		const char* const optName,
		CString& optValue
)
{
	// imp_typeオプションのチェック
	if( !CheckParam(vm, optName, optValue) )
	{
		return false;
	}

	m_impTypeStr = optValue;
	
	// imp_typeオプション値のチェック
	if( optValue.CompareNoCase( CString(arg_imp_type::k_placename_rep) ) == 0 )
	{
		m_impType = Utility::ExecuteType::PlaceNameRep;
	}
	else if( optValue.CompareNoCase( CString(arg_imp_type::k_addr_poly) ) == 0 )
	{
		m_impType = Utility::ExecuteType::AddrPoly;
	}
	else if( optValue.CompareNoCase( CString(arg_imp_type::k_addr_point) ) == 0 )
	{
		m_impType = Utility::ExecuteType::AddrPoint;
	}
	else if( optValue.CompareNoCase( CString(arg_imp_type::k_addr_all) ) == 0 )
	{
		m_impType = Utility::ExecuteType::AddrAll;
	}
	else
	{
		m_impType = Utility::ExecuteType::None;
		cerr << uh::toStr(optName) << "オプションに規定外のオプション値が指定されています(" << (CT2A)optValue << ")" << endl;
		return false;
	}

	return true;
}

bool Arguments::CheckParam( 
		const po::variables_map& vm,
		const char* const optName,
		CString& optValue,
		bool bFile /* = false */
)
{
	// オプションが指定されているかのチェック
	if( vm.count(optName) == 0 )
	{
		cerr << uh::toStr(optName) << "オプションは必須です" << endl;
		return false;
	}

	// オプション値が指定されているかのチェック
	optValue = vm[optName].as<uh::tstring>().c_str();
	if( optValue.IsEmpty() )
	{	
		cerr << uh::toStr(optName) << "オプションは必須です" << endl;
		return false;
	}

	if( bFile )
	{
		if( !PathFileExists( optValue ))
		{
			cerr << (CT2A)optValue << "が存在しません" << endl;
			return false;
		}
	}
	return true;
}
