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

#include <WinLib/VersionInfo.h>
#include <atltime.h>

#include "Logger.h"
//#include "globalfunc.h"
#include "CsvLoader.h"

#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>

// useful_headers
#include <TDC/useful_headers/str_util.h>

// boost
#include <boost/filesystem/path.hpp>

const CString LAYER_FIELD = _T("LAYER");   // ���C����
const CString UPDATE_TYPE = _T("�X�V���e");  // �V�Kor�X�V
const CString COLMUNS     = _T("#cols#");  // �w�b�_�̗񐔊o���Ƃ��p

const CString ORG_FIELD = _T("�i�O�j");   // ���C����
const CString MODIFY_FIELD = _T("�i��j");   // ���C����

// �V���O���g���I�u�W�F�N�g�̃X���b�h����p
std::mutex CRunLogger::s_mutex;
std::mutex CErrLogger::s_mutex;

// �t�@�C���I�[�v��
bool CLogger::Initialize( const uh::tstring& strDirPath )
{
	if( ! m_Log || m_Log.is_open() )
		return false;

	// �t�H���_�p�X�ƃt�@�C������A��
	//boost::filesystem::path filepath = boost::filesystem::path(uh::toStr( strDirPath.c_str()) + uh::toStr( getFileName() ));

	m_Log.open( strDirPath );
	if( ! m_Log )
		return false;

	writeHeader();

	return true;
}

// ���O�փ��b�Z�[�W�o��
void CLogger::Log( const uh::tstring& strMessage, bool std_out )
{
	m_Log << uh::toStr( strMessage );
	if( std_out )
		std::cout << uh::toStr( strMessage ) << std::endl;
}


// ���O�փ��b�Z�[�W�o��
void CLogger::Log( const TCHAR* format, ... )
{
	CString cstr;
	if( format )
	{
		va_list args;
		va_start( args, format );
		cstr.FormatV( format, args );
		va_end(args);
	}
	Log( uh::toTStr( cstr.GetString() ), false );
}


// << �I�y���[�^�̃I�[�o�[���[�h
CLogger& CLogger::operator<<( const uh::tstring& strMessage )
{
	m_Log << uh::toStr( strMessage );
	return *this;
}



// �G���[���x���t���̃��b�Z�[�W�o��
void CRunLogger::Fatal( const uh::tstring& strMessage, bool std_err )
{
	Log( _T("%s FATAL -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
	if( std_err )
		std::cerr << uh::toStr( strMessage ) << std::endl;
	//throw std::runtime_error( uh::toStr( strMessage ) );
}

void CRunLogger::Error( const uh::tstring& strMessage )
{
	Log( _T("%s ERROR -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
}

void CRunLogger::Warn( const uh::tstring& strMessage )
{
	Log( _T("%s WARN  -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
}

void CRunLogger::Info( const uh::tstring& strMessage, bool std_out )
{
	Log( _T("%s INFO  -- : %s\n"), log_utility::GetDateTimeInfo().c_str(), strMessage.c_str() );
	if( std_out )
		std::cout << uh::toStr( strMessage ) << std::endl;
}

void CRunLogger::Debug( const uh::tstring& strMessage )
{
	Log( _T("%s DEBUG -- : %s\n"), log_utility::GetDateTimeInfo().c_str(),  strMessage.c_str() );
}


// ���s���O�t�@�C������Ԃ�
uh::tstring CRunLogger::getFileName()
{
	return log_utility::GetLogfileNamePrefix() + _T("run.log");
}


void CRunLogger::writeHeader()
{
	// �c�[����
	CVersion version;
	Log( _T("%s PRODUCTVERSION %s\n"), version.GetInternalName(), version.GetProductVersion() );
}

// ���ʏo��
void CRunLogger::WriteResult( const CsvLoader& csv )
{
	long all = csv.size();
	long create =std::count_if( csv.cbegin(), csv.cend(), []( const CsvLoader::CsvRecord& record ){
			return record.second.isCreate;
	});
	long update = all - create;

	CString result = 
		_T("\n")
		_T("####################\n")
		_T("# ��������\n")
		_T("####################\n")
		_T("���̓��R�[�h�� �F %ld\n")
		_T("--\n")
		_T(" �X�V���R�[�h���F %ld\n")
		_T(" �ǉ����R�[�h���F %ld\n");

	Log( uh::str_util::format( result, all, update, create ), true );
}

// �G���[���O�t�@�C������Ԃ�
uh::tstring CErrLogger::getFileName()
{
	return log_utility::GetLogfileNamePrefix() + _T("err.log");
}


void CErrLogger::writeHeader()
{
	Log( _T( "#FREESTYLELOG\n"), false );
}

std::map<CString, long> getColumnIndex( const CsvLoader& csv )
{
	using namespace sindy::schema;

	static std::map<CString, long> columnIdx;
	if( !columnIdx.empty() )
		return columnIdx;
	
	// �Œ��
	std::vector<CString> columns = boost::assign::list_of
		(LAYER_FIELD)             // LAYER
		(kObjectID)               // OBJECTID
		(UPDATE_TYPE);            // �X�V���e�i�V�Kor�X�V�j
	
	bool hasShapeField = false;
	for( const auto& fi : csv.GetFieldInfos() )
	{
		if( fi.isOidField() )
			continue;

		if( fi.isLatLonField() )
		{
			hasShapeField = true;
			continue;
		}

		columnIdx[ fi.fieldName ] = columns.size();

		columns.push_back( fi.fieldName + ORG_FIELD );
		columns.push_back( fi.fieldName + MODIFY_FIELD );
	}

	// �܌o�x�t�B�[���h���Ō�ɐς�
	if( hasShapeField )
	{
		columnIdx[ ipc_feature::kShape ] = columns.size();
		columns.push_back( LON_FIELD + ORG_FIELD );
		columns.push_back( LAT_FIELD + ORG_FIELD );
		columns.push_back( LON_FIELD + MODIFY_FIELD );
		columns.push_back( LAT_FIELD + MODIFY_FIELD );
	}

	// �񐔂��o���Ă���
	columnIdx[ COLMUNS ] = columns.size();

	// �w�b�_�s�����o��
	ERRLOGGER.Log( uh::str_util::join( columns, _T("\t") ) + _T("\n"), false );

	return columnIdx;
}

void CErrLogger::WriteLog( const CsvLoader& csv, 
	                       const sindy::CTableContainer& destT )
{
	using namespace sindy;
	using namespace sindy::schema;

	// �w�b�_���̃C���f�N�X���擾
	std::map<CString, long> columnIdx = getColumnIndex( csv );

	// �܌o�x�o�͗p
	auto toLatLon = [&columnIdx]( const CRowContainer& row, bool before, bool after, std::vector<CString>& columunVal)
	{
		long logIdx = columnIdx[ ipc_feature::kShape ];
		if( before )
		{
			IPointPtr orgP = row.CRowBase::GetShapeCopy(enumOriginal);
			if( !orgP )
			{
				// �O��NULL�`��͂��蓾��ibug 11640�j
				columunVal[ logIdx ]     = _T("<null>");
				columunVal[ logIdx + 1 ] = _T("<null>");
			}
			else
			{
				double org_x(0), org_y(0);
				orgP->QueryCoords( &org_x, &org_y );
				columunVal[ logIdx ]     = uh::str_util::ToString(org_x, _T("%lf"));   // LON(�O)
				columunVal[ logIdx + 1 ] = uh::str_util::ToString(org_y, _T("%lf"));   // LAT(�O)
			}
		}
		
		if( after )
		{
			IPointPtr newP = row.CRowBase::GetShapeCopy();
			if( !newP )
			{
                // NULL�`��ɂ���悤�ȋ@�\�͂Ȃ��A���Ƃ���NULL�`��̂��̂�M��Ȃ������ꍇ��
                // ���O�o�͑ΏۂɂȂ�Ȃ��i�����Ȃ��j�̂ł�����NULL�`��ƂȂ邱�Ƃ͂Ȃ��͂�
				throw std::runtime_error( uh::str_util::formatA( 
					_T("�y�����G���[�z�`��̎擾�Ɏ��s�iOID: %ld"), row.GetOID() ) );
			}
			double x(0), y(0);
			newP->QueryCoords( &x, &y );
			columunVal[ logIdx + 2 ]     = uh::str_util::ToString(x, _T("%lf"));   // LON(�O)
			columunVal[ logIdx + 3 ] = uh::str_util::ToString(y, _T("%lf"));       // LAT(�O)
		}
	};
	
	for( const auto& con : destT) 
	{
		const auto& row = *CAST_ROWC(con);

		// 1�s�������o��
		std::vector<CString> columunVal(columnIdx[ COLMUNS ]);
		columunVal[0] = destT.GetTableName();
		columunVal[1] = uh::str_util::ToString(row.GetOID());
		columunVal[2] = row.IsCreated() ? _T("�V�K") : _T("�X�V");

		// �V�K�Ȃ炷�ׂāi��j�ɏ����o��
		if( row.IsCreated() )
		{
			for( const auto& col : columnIdx )
			{
				if( col.first.Compare( COLMUNS ) == 0 )
					continue;

				// �`��Ȃ�
				const CString& fieldName = col.first;
				if( fieldName == ipc_feature::kShape )
				{
					toLatLon( row, false, true, columunVal );
					continue;
				}

				// �V�K�̏ꍇ�AOBJECTID��GetValue�ŏE���Ȃ��B�B
				CString val = uh::str_util::ToString(
					( FieldInfo::isOidField( fieldName ) ) ? row.GetOID() : row.GetValue( fieldName ) );
				
				// �i��j�Ȃ̂� +1
				long logIdx = col.second;
				columunVal[ logIdx + 1 ] = val;
			}
		}
		else
		{
			const RecordInfo& ri = csv.find_by_oid( row.GetOID() );
			for( const auto& fldVal : ri )
			{
				const FieldInfo& fi = fldVal.first;
				
				// �`��Ȃ�
				if( fi.isLatLonField() )
				{
					toLatLon( row, (ri.hasShape || fi.forceOutput), ri.hasShape, columunVal );
					continue;
				}

				// �ύX������΁i�O�j�i��j
				long logIdx = columnIdx[ fi.fieldName ];
				if( row.CRowBase::Changed( fi.fieldName ))
				{
					columunVal[ logIdx ] = uh::str_util::ToString( row.GetOriginalValue( fi.fieldName ) );
					columunVal[ logIdx + 1 ] = uh::str_util::ToString( row.GetValue( fi.fieldName ) );
					continue;
				}

				// �����o�̓I�v�V�����Ŏw�肳�ꂽ�t�B�[���h�Ȃ�ύX���Ȃ��Ă��i�O�j
				if( fi.forceOutput )
					columunVal[ logIdx ] = uh::str_util::ToString( row.GetOriginalValue( fi.fieldName ) );
			}
		}

		// 1�s�������o��
		ERRLOGGER.Log( uh::str_util::join( columunVal, _T("\t") ) + _T("\n"), false );
	}
}

namespace log_utility
{
	// �G���[���O�t�@�C�������擾
	static uh::tstring GetLogfileNamePrefix()
	{
		// �c�[�����擾
		CVersion version;
		CString strToolName = version.GetInternalName();
		// �g���q������΍폜
		if( strToolName.Find( _T('.') ) != -1 )
			strToolName = strToolName.Left( strToolName.ReverseFind(_T('.')) );;

		// ���݂̓�����"YYYYMMDDHHDDSS" �Ŏ擾
		CTime cTime = CTime::GetCurrentTime();
		const CString strTime = cTime.Format( _T("%Y%m%d%H%M%S") );

		return strToolName + _T("_") + strTime.GetString() + _T("_");
	}

	// �����̏��𕶎���ŕԂ�
	static uh::tstring GetDateTimeInfo()
	{
		CTime cTime = CTime::GetCurrentTime();
		const CString strTime = cTime.Format( _T("[%Y-%m-%d %H:%M:%S]") );

		return uh::toTStr( strTime.GetString() );
	}
}
