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
#include "CsvLoader.h"
#include "Logger.h"
#include "globalfunc.h"
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_core/FieldMap.h>
#include <boost/filesystem.hpp>

const CString LAT_FIELD = _T("LAT");       // �o�x
const CString LON_FIELD = _T("LON");       // �ܓx

using namespace sindy;
using namespace sindy::schema;

FieldInfo::FieldInfo( long csvIdx, long destIdx, const CString& field, bool opt ):
	csvIndex(csvIdx),
	destIndex(destIdx),
	fieldName(field),
	forceOutput(opt),
	isChanged(false)
{
}

// �R���X�g���N�^
RecordInfo::RecordInfo():
	isCreate(false),
	oid(-1),
	hasShape(false),
	latlon()
{
}

// []�I�y���[�^
CComVariant& RecordInfo::operator [](const FieldInfo& fi)
{
	// �t�B�[���h���ŒT��
	auto& it = boost::find_if( m_fieldValues, [&fi](const fieldValue& fldVal){
		return fldVal.first.fieldName == fi.fieldName;
	});

	// ���Ɏ����Ă���΂����Ԃ�
	if( it != m_fieldValues.end() )
		return it->second;

	// �Ȃ���Βǉ�����
	m_fieldValues.push_back(std::make_pair(fi, CComVariant()));
	return m_fieldValues.back().second;
}

bool RecordInfo::setShape( const CString& fieldName, CComVariant& val )
{
	if( FAILED( val.ChangeType(VT_R8) ) )
		return false;

	if( fieldName.CompareNoCase( LON_FIELD ) == 0 )
		latlon.X = val.dblVal;
	else if( fieldName.CompareNoCase( LAT_FIELD ) == 0 )
		latlon.Y = val.dblVal;

	hasShape = true;

	return true;
}

void CsvLoader::init( const CString& path, const sindy::CFieldMap& destField, const CString& optFields )
{
	TryOpenCSV( path );

	// �uOBJECTID�v�t�B�[���h�͕K�{
	const auto& csvField = m_adoCur.fields();
	auto it = boost::find_if( csvField, [&](const CADOCursor::Field& field){
		return ( FieldInfo::isOidField(field.name) );
	});
	if( csvField.end() == it )
	{
		throw std::runtime_error( "���̓t�@�C����OBJECTID�t�B�[���h�����݂��܂���B" );
	}
	
	// �X�V�t�B�[���h���݃`�F�b�N
	CStringA noFields;
	bool hasLat(false), hasLon(false);
	for( const auto& field : csvField )
	{
		if( FieldInfo::isLatField( field.name ) )
		{
			hasLat = true;
			continue;
		}
		
		if( FieldInfo::isLonField( field.name ) )
		{
			hasLon = true;
			continue;
		}

		if( (destField.HasField( field.name )) < 0 )
			noFields.AppendFormat( "[%s] ", CT2A(field.name) );
	}
	if( !noFields.IsEmpty() )
	{
		std::string msg = "�X�V��̃e�[�u���ɑ��݂��Ȃ��t�B�[���h�ł��F " + noFields;
		throw std::runtime_error( msg );
	}

	// LAN,LOT�s�����`�F�b�N
	if( hasLat ^ hasLon )
	{
		throw std::runtime_error( "LON �܂��� LAT �t�B�[���h�����݂��܂���B" );
	}

	// �����o�̓t�B�[���h�����݃`�F�b�N
	auto optFieldLst = uh::str_util::split( optFields, _T(",") );
	for( const auto& optFld : optFieldLst )
	{
		auto exist = boost::find_if( csvField, [&optFld](const CADOCursor::Field& filed ){
			return (optFld.CompareNoCase( filed.name ) == 0); 
		});

		if( exist == csvField.end() )
			noFields.AppendFormat( "[%s] ", CT2A(optFld) );
	}
	if( !noFields.IsEmpty() )
	{
		std::string msg = "--force_output �I�v�V�����̃t�B�[���h���s���ł��F " + noFields;
		throw std::runtime_error( msg );
	}

	// �t�B�[���h�̏����m��
	long srcIdx = 0;
	for( auto itFld = csvField.begin(); itFld != csvField.end(); ++itFld, ++srcIdx )
	{
		m_fieldInfos.push_back( FieldInfo( 
			srcIdx,
			destField.HasField( itFld->name ),
			itFld->name,
			boost::find( optFieldLst, itFld->name ) != optFieldLst.end() )
		);
	}
}

// Load()�ŌĂ΂��񃁃��o�֐��̃v���g�^�C�v�錾
bool ValidateData( int lineNum, 
				   CComVariant& val, 
				   const FieldInfo& fi, 
				   const CFieldMap& fm, 
				   std::vector<CString>& errs );

void CsvLoader::Load( const CString& path, const CString& optFields, const sindy::CTableContainer& table )
{
	boost::timer t;
	RUNLOGGER.Info( uh::str_util::format(
		_T("���̓t�@�C���ǂݍ���... : %s"), path), true);

	auto fm = *table.GetFieldMap();
	init( path, fm, optFields );

	// �S���R�[�h���������Ȃ���擾
	long lineNum = 1;     // �s���J�E���^�B�w�b�_���l������1����
	bool hasErr = false;
	for( auto it = m_adoCur.begin(); it != m_adoCur.end(); ++it, ++lineNum )
	{
		// �t�B�[���h���Ƃ̒l���擾����
		RecordInfo ri;
		for( const auto& fi : GetFieldInfos() )
		{
			CComVariant& csvVal = (*it)[fi.csvIndex];   // CSV�Ŏw�肳�ꂽ�l
			
			// �t�B�[���h�l�`�F�b�N
			std::vector<CString> errs;
			if( !ValidateData( lineNum, csvVal, fi, fm, errs ) )
			{
				RUNLOGGER.Log( uh::str_util::join(errs, _T("\n")) + _T("\n"), false );
				hasErr = true;
				continue;
			}
			
			if( fi.isOidField() )
			{
				if( uh::variant_util::isNullOrEmpty(csvVal) )
					ri.isCreate = true;
				else
					ri.oid = csvVal.lVal;
			}
			else if( fi.isLatLonField() )
			{
				ri.setShape( fi.fieldName, csvVal );
			}

			// �t�B�[���h�̒l���擾
			ri[fi] = csvVal;
		}

		// �X�V�ΏۂȂ�Ώۃ��R�[�h�̑��݂��m�F���Ă���
		if( !ri.isCreate &&
			!table.GetRow( ri.oid ) )
		{
			RUNLOGGER.Log( uh::str_util::format( 
				_T("Line:%ld\t\t\t�X�V�Ώۂ̃I�u�W�F�N�g�����݂��܂���iOID:%ld�j\n"), lineNum, ri.oid ),
				false );

			hasErr = true;
			continue;
		}

		// �X�V�Ώ�
		Add( lineNum, ri );
	}

	if( hasErr )
	{
		throw std::runtime_error( "���̓f�[�^�ɃG���[������܂��B" );
	}

	RUNLOGGER.Info( 
		uh::str_util::format( _T("����: %ld records. ( %lf[s] )\n"), m_records.size(), t.elapsed() ), true);
}

// �ǉ�
void CsvLoader::Add( long lineNum, const RecordInfo& record )
{
	m_records.push_back(
		std::make_pair( lineNum, record )
	);
}

// OID�Ō���
const RecordInfo& CsvLoader::find_by_oid( long oid ) const
{
	auto it = boost::find_if( m_records, [&oid]( const CsvRecord& record ){
		return (record.second.oid == oid);
	});

	if( it == m_records.end() )
	{
		throw std::runtime_error( uh::str_util::formatA(
			_T("�y�����G���[�zCSV�̃L���b�V�����ɑ��݂��Ȃ����R�[�h�ł��iOID:%ld�j"), oid ) );
	}
	return it->second;
}

// ���R�[�h�ǂݍ���
void CsvLoader::TryOpenCSV( const CString& path )
{
	boost::filesystem::path fullPath = path.GetString();
	
	if( !m_ado.connect( fullPath.c_str() ) )
	{
		std::string msg = "���̓t�@�C���I�[�v���G���[�F " + CStringA(path);
		throw std::runtime_error( msg );
	}
	
	// �����Ńt���p�X�w�肷��Ɠ��{��t�H���_���܂ރp�X���ƊJ���Ȃ��Ȃ�̂Ńt�@�C���������B
	// �Ȃ灪�ł͂��̃t�H���_�܂łł����́H�Ǝv���������������킯�ł��Ȃ��炵���B�B
	CStringA query = "SELECT * FROM " + CStringA(fullPath.filename().c_str());
	m_adoCur = m_ado.search( query );
	if( S_OK != m_adoCur.err() )
	{
		std::string msg = "���̓t�@�C���ǂݍ��݃G���[�F " + CStringA(m_adoCur.GetErrorMessage());
		throw std::runtime_error( msg );
	}

}

// �X�V�Ώۂ�OID�擾
std::vector< long > CsvLoader::GetUpdateIDs() const
{
	std::vector< long > ids;
	for( const auto& row : m_records )
	{
		const RecordInfo& ri = row.second;

		if( ri.isCreate )
			continue;

		ids.push_back( ri.oid );
	}

	return ids;
}


std::vector< CsvLoader::CsvRecords > CsvLoader::GetNewRecordsList() const
{
	RUNLOGGER.Info( _T("�V�K�쐬���X�g�擾... "), true );
	long count = 0;
	boost::timer t;

	std::vector< CsvRecords > recList;
	CsvRecords records;
	for( const auto& row : m_records )
	{
		const RecordInfo& ri = row.second;

		if( !ri.isCreate )
			continue;

		records.push_back( ri );
		if( records.size() >= 10000 )
		{
			recList.push_back( records );
			records.clear();

			std::cerr << count << "records. " << t.elapsed() << "[s]" <<std::endl;
			t.restart();
		}

		++count;
	}
	
	recList.push_back( records );
	RUNLOGGER.Info( _T("OK.\n"), true );
	return recList;
}

//////////////////////
// �񃁃��o�֐��̎���
//////////////////////


bool ValidateData( int lineNum, 
				   CComVariant& val,             // ChangeType()�Ō^���ς��\��������
				   const FieldInfo& fi, 
				   const CFieldMap& fm, 
				   std::vector<CString>& errs )
{
	using namespace uh::variant_util;

	// �l���w�肳��Ă��Ȃ���΃m�[�`�F�b�N
	if( isNullOrEmpty(val) )
		return true;

	// �G���[���b�Z�[�W�����o���p
	auto setErrMsg = [&]( const CString& msg ){
		using namespace uh::str_util;
		// <�s��> <�t�B�[���h��> <�l> <���e>
		CString strLine = uh::str_util::format( _T("Line:%ld"), lineNum );
		val.ChangeType( VT_BSTR );
		std::list<CString> err = boost::assign::list_of
			(strLine)(fi.fieldName)(val)(msg);

		errs.push_back( uh::str_util::join( err, _T("\t") ) );
	};

	// LAT,LON�t�B�[���h��SHAPE�ɓǂݑւ�
	CFieldMap::_FIELD fieldAttr = {};
	if( fi.isLatLonField() )
	{
		if( fm.GetShapeIndex() < 0 )
		{
			setErrMsg( _T("�Ώۃe�[�u���Ɍ`��t�B�[���h�����݂��܂���") );
			return false;
		}
		fieldAttr = fm.GetFieldAttr( fm.GetShapeIndex() );
	}
	else
	{
		fieldAttr = fm.GetFieldAttr( fi.destIndex );
	}


	// �^�`�F�b�N
	{
		static const std::map< esriFieldType, VARTYPE > typeChecker = boost::assign::map_list_of
			(esriFieldTypeOID, VT_I4)
			(esriFieldTypeInteger, VT_I4)
			(esriFieldTypeDouble, VT_R8)
			(esriFieldTypeString, VT_BSTR)
			(esriFieldTypeDate, VT_DATE)
			(esriFieldTypeGeometry, VT_R8);  // Geometry�̓|�C���g�iLAT,LON�j�̂݃T�|�[�g

		auto it = typeChecker.find( fieldAttr.type );
		if( it == typeChecker.end() )
		{
			setErrMsg( _T("���T�|�[�g�̃f�[�^�^�C�v�ł�") );
		}

		// TODO: ADO��CSV�������ꍇ�A�ŏ��̐��s����t�B�[���h�̃f�[�^�^����������A
		//       ����Ȃ����̂̓t�F�b�`�̎��_��VT_NULL�ɂ����̂ŁA���̃`�F�b�N�͂��܂�Ӗ��Ȃ�����
		if( FAILED( val.ChangeType(it->second) ) )
		{
			setErrMsg( uh::str_util::format( 
				_T("�s���ȃf�[�^�^�ł��i%s�j"), toTypeString(val) ) );
		}
	}
	
	// �^���Ƃ̏ڍ׃`�F�b�N
	switch( fieldAttr.type )
	{
	case esriFieldTypeDouble:
	case esriFieldTypeInteger:
		{
			// �S����
			long precision = CStringW(uh::str_util::ToString(val)).GetLength();
			if( fieldAttr.precision < precision )
			{
				setErrMsg( uh::str_util::format( 
					_T("�������I�[�o�[���Ă��܂��B[%ld]�i�����F%ld�j"), precision, fieldAttr.precision ) );
			}
		}
		break;
	case esriFieldTypeString:
		{
			// ����
			long length = CStringW(uh::str_util::ToString(val)).GetLength();
			if( fieldAttr.length < length )
			{
				setErrMsg( uh::str_util::format( 
					_T("�����񐔂��I�[�o�[���Ă��܂��B[%ld]�i�����F%ld�j"), length, fieldAttr.length ) );
			}
		}
		break;
	default:
		break;
	}

	// NULL���`�F�b�N
	if( !fieldAttr.nullable && 
		uh::str_util::ToString(val) == NULL_VALUE )
	{
		setErrMsg( _T("���Y�t�B�[���h��null�s���ł��B") );
	}

	// �R�[�h�l�`�F�b�N
	if( !fieldAttr.rdomain.empty() && 
		fieldAttr.rdomain.find(val.lVal) == fieldAttr.rdomain.end() )
	{
		setErrMsg( _T("���݂��Ȃ��R�[�h�l�ł��B") );
	}

	return errs.empty();
}
