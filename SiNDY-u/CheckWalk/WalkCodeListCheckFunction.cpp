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
#include "WalkCodeListCheckFunction.h"
#include "value.h"

using namespace sindy::schema;
using namespace uh::str_util;

void WalkCodeListCheckFunction::checkWalkCodeList(
	const std::set<long>& walkCodeSet,
	const RowsPack& walkCode
	)
{
	m_output.SetLayer( walk_code_list::kTableName );
	m_output.OutputStdErr( err_level::info, err_code::StrChecking, walk_code_list::kTableName );

	long walkCodeIndex = walkCode.m_FieldMap.FindField( walk_code_list::kWalkCode );
	long nameKanjiIndex = walkCode.m_FieldMap.FindField( walk_code_list::kNameKanji );
	long nameYomiIndex = walkCode.m_FieldMap.FindField( walk_code_list::kNameYomi );

	long modOperatorIndex = walkCode.m_FieldMap.FindField( walk_code_list::kOperator );
	long modDateIndex = walkCode.m_FieldMap.FindField( walk_code_list::kModifyDate );
	for ( const auto& walkCodeRow : walkCode.m_Rows )
	{
		auto walkCode = walkCodeRow.second->GetValue( walkCodeIndex ).lVal;
		auto nameKanji = ToString( walkCodeRow.second->GetValue( nameKanjiIndex ) );
		auto nameYomi = ToString( walkCodeRow.second->GetValue( nameYomiIndex ) );

		m_output.SetModifyInfo( walkCodeRow.second->GetOID(),
			ToString( walkCodeRow.second->GetValue( modOperatorIndex ) ),
			ToString( walkCodeRow.second->GetValue( modDateIndex ) ), _T( "" ), _T( "" ) );

		if( ! judge_value::IsDefineWalkCode( walkCodeRow.first ) )    // ���G���[ ���s�ҘH��ID����`�O
			m_output.OutputRegLog( err_level::error, err_code::NgWalkCode, ToString( walkCode ) );

		if( nameKanji.IsEmpty() ) // ���G���[ ���s�ҘH�������̕����񒷂��O
			m_output.OutputRegLog( err_level::error, err_code::NgWalkCodeKanji1, ToString( walkCode ) );

		if( nameYomi.IsEmpty() ) // ���G���[ ���s�ҘH���ǂ݂̕����񒷂��O
			m_output.OutputRegLog( err_level::error, err_code::NgWalkCodeYomi, ToString( walkCode ) );

		if( 0 == nameKanji.CompareNoCase( _T( "-New-" ) ) )    // ���G���[ ���s�ҘH���������f�t�H���g�l
			m_output.OutputRegLog( err_level::error, err_code::NgWalkCodeKanji2, ToString( walkCode ) );

		if( 0 == walkCodeSet.count( walkCode ) ) // ���G���[ ���s�ҘH���R�[�h�����s�҃����N��1�������݂��Ȃ�
			m_output.OutputRegLog( err_level::error, err_code::NotExistWalkCodeWL, ToString( walkCode ) );
	}
}

void WalkCodeListCheckFunction::checkRelease(
	const RowsPack& walkCode
	)
{
	m_output.SetLayer( walk_code_list::kTableName );
	m_output.OutputStdErr( err_level::info, err_code::StrChecking, walk_code_list::kTableName );

	long walkCodeIndex = walkCode.m_FieldMap.FindField( walk_code_list::kWalkCode );
	long nameKanjiIndex = walkCode.m_FieldMap.FindField( walk_code_list::kNameKanji );
	long nameYomiIndex = walkCode.m_FieldMap.FindField( walk_code_list::kNameYomi );

	long modOperatorIndex = walkCode.m_FieldMap.FindField( walk_code_list::kOperator );
	long modDateIndex = walkCode.m_FieldMap.FindField( walk_code_list::kModifyDate );
	for ( const auto& walkCodeRow : walkCode.m_Rows )
	{

		long walkCode = walkCodeRow.second->GetValue( walkCodeIndex ).lVal;
		auto nameKanji = ToString(walkCodeRow.second->GetValue( nameKanjiIndex ) );
		auto nameYomi = ToString(walkCodeRow.second->GetValue( nameYomiIndex ) );

		m_output.SetModifyInfo( walkCodeRow.second->GetOID(),
			ToString( walkCodeRow.second->GetValue( modOperatorIndex ) ),
			ToString( walkCodeRow.second->GetValue( modDateIndex ) ), _T( "" ), _T( "" ) );
		if( nameKanji.IsEmpty() ) // ���G���[ ���s�ҘH�������̕����񒷂��O
			m_output.OutputRegLog( err_level::error, err_code::NgWalkCodeKanji1, ToString( walkCode ) );

		if( nameYomi.IsEmpty() ) // ���G���[ ���s�ҘH���ǂ݂̕����񒷂��O
			m_output.OutputRegLog( err_level::error, err_code::NgWalkCodeYomi, ToString( walkCode ) );
	}
}
