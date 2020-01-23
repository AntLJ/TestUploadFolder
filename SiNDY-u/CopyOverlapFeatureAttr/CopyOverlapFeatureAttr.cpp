#include "stdafx.h"
#include "CopyOverlapFeatureAttr.h"
#include "FeatureObjects.h"
#include "Message.h"

// boost
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/find_if.hpp>

#include <sindy/workspace/name.h>	//!< create_workspace
#include <sindy/libschema.h>
#include <winlib/versioninfo.h>

#include <TDC/useful_headers/str_util.h>
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/useful_headers/scope_guard.h>

#include <ArcHelperEx/AheglobalsMisc.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <ArcHelperEx/AheGeometryOp.h>
#include <ArcHelperEx/AheGeometryConvert.h>

#include <fstream>
#include <locale>
#include <codecvt>

static crd_cnv g_cnv;

const int CCopyOverlapFeatureAttr::max_retry_num = 5;          //!< �ڑ����g���C��
const double CCopyOverlapFeatureAttr::max_retry_time = 3.0;    //!< �ڑ����g���C����(�b��)
const TCHAR* childver_name_format = _T("ETC_COFA_%04d%02d%02d%02d%02d%02d");   //!< �q�o�[�W�������̃t�H�[�}�b�g

// �ݒ�t�@�C���̍��ږ�
namespace settings
{
	const wchar_t kSource[]              = L"source";
	const wchar_t kSourceTable[]         = L"sourcetable";
	const wchar_t kSourceField[]         = L"sourcefield";
	const wchar_t kSourceWhere[]         = L"sourcewhere";

	const wchar_t kTarget[]              = L"target";
	const wchar_t kTargetTable[]         = L"targettable";
	const wchar_t kTargetField[]         = L"targetfield";
	const wchar_t kTargetWhere[]         = L"targetwhere";

	const wchar_t kMesh[]                = L"mesh";
	const wchar_t kMeshTable[]           = L"meshtable";
	const wchar_t kMeshListPath[]        = L"meshlist";

	const wchar_t kExeMode[]             = L"exemode";
	const wchar_t kAttrListPath[]        = L"attrlist";
	const wchar_t kDatumFlag[]           = L"datumflag";
	const wchar_t kFieldFlag[]           = L"fieldflag";

	const wchar_t kOverlay[]             = L"overlay";
	const wchar_t kMagnification[]       = L"magnification";
	const wchar_t kReduction[]           = L"reduction";
	const wchar_t kBuffer[]              = L"buffer";
}

// ���s���[�h
namespace exemode
{
	const CString skipMode               = _T("skip");
	const CString matchMode              = _T("match");
}

// R-Tree����A�^�������͈̔͂Ɉ���������ID�Q���擾
std::set<long> CCopyOverlapFeatureAttr::getIntersectsIDs( const BGBox& box, const BGRTree& rTree )
{
	std::set<long> retIDs;
	std::vector<std::pair<BGBox, unsigned int>> results;
	bgi::query( rTree, bgi::intersects( box ), std::back_inserter(results) );

	for( const auto& result : results )
		retIDs.insert( static_cast<long>( result.second ) );

	return retIDs;
}

// R-Tree�𐶐�����
void CCopyOverlapFeatureAttr::createRTree( const std::map<long, GeoClass>& geoMap, BGRTree& rTree )
{
	for(const auto& geo : geoMap){
		long oID = geo.first;
		BGBox box = geo.second.GetBox();
		rTree.insert( std::make_pair( box, static_cast<unsigned int>(oID) ) );
	}
}

// ���΃p�X���΃p�X�ɕϊ�
CString CCopyOverlapFeatureAttr::convertRelativePath( const CString& strPath )
{	
	CString strFullPath;
	TCHAR* pszFilePart = nullptr;

	GetFullPathName( strPath, MAX_PATH, strFullPath.GetBuffer( MAX_PATH ), &pszFilePart );
	strFullPath.ReleaseBuffer();

	return strFullPath;
}

// �ݒ���̎擾
bool CCopyOverlapFeatureAttr::getSettingInfo(const std::vector<std::wstring>& info)
{
	// info[0]��Key�Ainfo[1]��Value�ASize��2�ȊO�̏ꍇ�A���ݒ�ƌ��Ȃ��i����l�j
	if( info.size() != 2 )
		return true;

	std::wstring strKey   = info[0];
	std::wstring strValue = info[1];

	std::transform(strKey.begin(), strKey.end(), strKey.begin(), std::towlower);

	// ��r���̐ڑ���
	if( strKey.compare( settings::kSource ) == 0 )
		m_setting.strSource = strValue.c_str();

	// ��r���̃e�[�u���ƃt�B�[���h��
	else if( strKey.compare( settings::kSourceField ) == 0 )
	{
		// �ݒ�̓��e��"."�ŕ���(�Ⴆ�΁ABUILDING_STEP.FLOORS)
		std::vector<std::wstring> values;
		boost::split( values, strValue, boost::is_any_of(L".") );

		// values[0]���e�[�u�����Avalues[1]���t�B�[���h���ASize��2�ȊO�̏ꍇ�A�ُ�l
		if( values.size() != 2 )
			return false;

		m_setting.strSourceTable = values[0].c_str();
		m_setting.strSourceField = values[1].c_str();
	}

	// ��r�����w�肷��WHERE����
	else if( strKey.compare( settings::kSourceWhere ) == 0 )
		m_setting.strSourceWhere = strValue.c_str();

	// ��r��̐ڑ���
	else if( strKey.compare( settings::kTarget ) == 0 )
		m_setting.strTarget = strValue.c_str();

	// ��r��̃e�[�u���ƃt�B�[���h��
	else if( strKey.compare( settings::kTargetField ) == 0 )
	{
		// �ݒ�̓��e��"."�ŕ���(�Ⴆ�΁ABUILDING_STEP.FLOORS)
		std::vector<std::wstring> values;
		boost::split( values, strValue, boost::is_any_of(L".") );

		// values[0]���e�[�u�����Avalues[1]���t�B�[���h���ASize��2�ȊO�̏ꍇ�A�ُ�l
		if( values.size() != 2 )
			return false;

		m_setting.strTargetTable = values[0].c_str();
		m_setting.strTargetField = values[1].c_str();
	}

	// ��r����w�肷��WHERE����
	else if( strKey.compare( settings::kTargetWhere ) == 0 )
		m_setting.strTargetWhere = strValue.c_str();

	// �����P��(���b�V��)�̐ڑ���
	else if( strKey.compare( settings::kMesh ) == 0 )
		m_setting.strMesh = strValue.c_str();

	// �����P��(���b�V��)�̃e�[�u����
	else if( strKey.compare( settings::kMeshTable ) == 0 )
		m_setting.strMeshTable = strValue.c_str();

	// �����͈́i���b�V�����X�g�j�̃t�@�C���p�X
	else if( strKey.compare( settings::kMeshListPath ) == 0 )
	{
		CString tmpStrValue = strValue.c_str();
		if( PathIsRelative(tmpStrValue) )
			m_setting.strMeshListPath = convertRelativePath( strValue.c_str() );
		else
			m_setting.strMeshListPath = strValue.c_str();
	}

	// ���s���[�h
	else if( strKey.compare( settings::kExeMode ) == 0 )
		m_setting.strExeMode = strValue.c_str();

	// �����ϊ��\�̃t�@�C���p�X
	else if( strKey.compare( settings::kAttrListPath ) == 0 )
	{
		CString tmpStrValue = strValue.c_str();
		if( PathIsRelative(tmpStrValue) )
			m_setting.strAttrListPath = convertRelativePath( strValue.c_str() );
		else
			m_setting.strAttrListPath = strValue.c_str();
	}

	// ���n�n�����ϊ��t���O
	else if( strKey.compare( settings::kDatumFlag ) == 0 )
	{
		std::transform(strValue.begin(), strValue.end(), strValue.begin(), std::towlower);

		if( strValue.compare( L"true" ) == 0 )
			m_setting.bDatumFlag = true;
		else if( strValue.compare( L"false" ) == 0 || strValue.empty() )
			m_setting.bDatumFlag = false;
		else
			return false;
	}

	// �t�B�[���h�^�����ϊ��t���O
	else if( strKey.compare( settings::kFieldFlag ) == 0 )
	{
		std::transform(strValue.begin(), strValue.end(), strValue.begin(), std::towlower);

		if( strValue.compare( L"true" ) == 0 )
			m_setting.bFieldFlag = true;
		else if( strValue.compare( L"false" ) == 0 || strValue.empty() )
			m_setting.bFieldFlag = false;
		else
			return false;
	}

	// �d������臒l
	else if( strKey.compare( settings::kOverlay ) == 0 )
	{
		// ���ݒ�̏ꍇ�AcheckSettings()�Ń`�F�b�N
		if( !strValue.empty() )
			m_setting.dOverlay = std::stod( strValue );
	}

	// �g�嗦��臒l
	else if( strKey.compare( settings::kMagnification ) == 0 )
	{
		// ���ݒ�̏ꍇ�AcheckSettings()�Ń`�F�b�N
		if( !strValue.empty() )
			m_setting.dMagnification = std::stod( strValue );
	}

	// �k������臒l
	else if( strKey.compare( settings::kReduction ) == 0 )
	{
		// ���ݒ�̏ꍇ�AcheckSettings()�Ń`�F�b�N
		if( !strValue.empty() )
			m_setting.dReduction = std::stod( strValue );
	}

	// �o�b�t�@�[�̕��i���[�g���P�ʁj
	else if( strKey.compare( settings::kBuffer ) == 0 )
	{
		// ���ݒ�̏ꍇ�AcheckSettings()�Ń`�F�b�N
		if( !strValue.empty() )
			m_setting.dBuffer = std::stod( strValue );
	}

	return true;
}


// �K�{���ځi������j�̃`�F�b�N
bool CCopyOverlapFeatureAttr::checkStringSetting(const std::wstring& strKey, const CString& strValue)
{
	CString message;
	if( strValue.IsEmpty() )
	{
		message = uh::str_util::format( message::getMsg(message::iRequiredSetting), strKey.c_str() );
		writeRunLog(message, false);
		return false;
	}

	return true;
}

// �K�{���ځi�����^�j�̃`�F�b�N
bool CCopyOverlapFeatureAttr::checkNumericSetting(const std::wstring& strKey, double value)
{
	CString message;

	// �����l�̂܂܂��ƁA���ݒ�ƌ��Ȃ�
	if( value == -1.0 )
	{
		message = uh::str_util::format( message::getMsg(message::iRequiredSetting), strKey.c_str() );
		writeRunLog(message, false);
		return false;
	}

	// TODO: 臒l�̒萔���͗v����

	// �d�����̐ݒ�l��0�`100%�ȊO�̏ꍇ�A�G���[
	if( strKey.compare( settings::kOverlay ) == 0 )
	{
		if( value < 0 || value > 100 )
		{
			message = uh::str_util::format( message::getMsg(message::iAbnormalOverlay), value );
			writeRunLog(message, false);
			return false;
		}
	}

	// �g�嗦�̐ݒ�l��100%�����̏ꍇ�A�G���[
	else if( strKey.compare( settings::kMagnification ) == 0 )
	{
		if( value < 100 )
		{
			message = uh::str_util::format( message::getMsg(message::iAbnormalMagnification), value );
			writeRunLog(message, false);
			return false;
		}
	}

	// �k�����̐ݒ�l��0�`100%�ȊO�̏ꍇ�A�G���[
	else if( strKey.compare( settings::kMagnification ) == 0 )
	{
		if( value < 0 || value > 100 )
		{
			message = uh::str_util::format( message::getMsg( message::iAbnormalReduction ), value );
			writeRunLog( message, false );
			return false;
		}
	}

	// �o�b�t�@�[�̐ݒ�l��0�����̏ꍇ�A�G���[
	else if( strKey.compare( settings::kBuffer ) == 0 )
	{
		if( value < 0 )
		{
			message = uh::str_util::format( message::getMsg(message::iAbnormalBuffer), value );
			writeRunLog(message, false);
			return false;
		}
	}

	return true;
}

// �t�@�C���̃`�F�b�N
bool CCopyOverlapFeatureAttr::checkFileSetting(const std::wifstream& ifs, const CString& strFilePath)
{
	CString message;
	if( ifs.fail() )
	{
		message = uh::str_util::format( message::getMsg(message::iCannotOpen), strFilePath );
		writeRunLog(message, false);
		return false;
	}

	if( ifs.eof() )
	{
		message = uh::str_util::format( message::getMsg(message::iEmptyFile), strFilePath );
		writeRunLog(message, false);
		return false;
	}

	return true;
}

// �ݒ���e�̃`�F�b�N
bool CCopyOverlapFeatureAttr::checkSettings()
{
	bool result = true;

	// �K�{���ځi������^�j�̃`�F�b�N
	result &= checkStringSetting( settings::kSource,         m_setting.strSource );
	result &= checkStringSetting( settings::kSourceTable,    m_setting.strSourceTable );
	result &= checkStringSetting( settings::kSourceField,    m_setting.strSourceField );

	result &= checkStringSetting( settings::kTarget,         m_setting.strTarget );
	result &= checkStringSetting( settings::kTargetTable,    m_setting.strTargetTable );
	result &= checkStringSetting( settings::kTargetField,    m_setting.strTargetField );

	result &= checkStringSetting( settings::kMesh,           m_setting.strMesh );
	result &= checkStringSetting( settings::kMeshTable,      m_setting.strMeshTable );
	result &= checkStringSetting( settings::kMeshListPath,   m_setting.strMeshListPath );

	result &= checkStringSetting( settings::kExeMode,        m_setting.strExeMode );

	// �K�{���ځi�����^�j�̃`�F�b�N
	result &= checkNumericSetting( settings::kOverlay,       m_setting.dOverlay );
	result &= checkNumericSetting( settings::kMagnification, m_setting.dMagnification );
	result &= checkNumericSetting( settings::kReduction,     m_setting.dReduction );
	result &= checkNumericSetting( settings::kBuffer,        m_setting.dBuffer );

	return result;
}

// �ݒ�t�@�C���̓ǂݍ���
bool CCopyOverlapFeatureAttr::readSettingFile(const CString& strFilePath)
{
	std::wifstream  ifs( strFilePath );
	if( !checkFileSetting(ifs, strFilePath) )
		return false;

	// UTF-8�œǂݍ���
	std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
	ifs.imbue( utf8_locale );

	std::wstring strLine;
	while( std::getline( ifs, strLine ) )
	{
		// ��s�A�R�����g�s���O
		if( strLine.empty() ) continue;
		if(	strLine.front() == L'#' ) continue;

		// �ݒ�̓��e��"=:"�ŕ���(Where���"="�Ƌ�ʂ��邽�߁A�Ⴆ�΁Asourcewhere=:FLOORS = 0)
		std::vector<std::wstring> info;
		boost::split_regex( info, strLine, boost::wregex(L"=:") );
		if( !getSettingInfo(info) ) return false;
	}

	// �ݒ���e�̃`�F�b�N
	if( !checkSettings() )
		return false;

	return true;
}

// ���b�V�����X�g�̓ǂݍ���
bool CCopyOverlapFeatureAttr::readMeshList(const CString& strFilePath)
{
	std::wifstream ifs( strFilePath );
	if( !checkFileSetting(ifs, strFilePath) )
		return false;

	// UTF-8�œǂݍ���
	std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
	ifs.imbue( utf8_locale );

	std::wstring strLine;
	while( std::getline( ifs, strLine ) )
	{
		// ��s�A�R�����g�s���O
		if( strLine.empty() ) continue;
		if(	strLine.front() == L'#' ) continue;

		m_setMeshList.insert( std::stoul(strLine) );
	}

	return true;
}

// �����ϊ��\�̓ǂݍ���
bool CCopyOverlapFeatureAttr::readAttributeList(const CString& strFilePath)
{
	std::wifstream ifs( strFilePath );
	if( !checkFileSetting(ifs, strFilePath) )
		return false;

	// UTF-8�œǂݍ���
	std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
	ifs.imbue( utf8_locale );

	std::wstring strLine;
	while( std::getline( ifs, strLine ) )
	{
		// ��s�A�R�����g�s���O
		if( strLine.empty() ) continue;
		if(	strLine.front() == L'#' ) continue;

		// ��r���Ɣ�r��̑����l���^�u�ŕ���
		std::vector<std::wstring> info;
		boost::split( info, strLine, boost::is_any_of(L"\t") );

		// info[0]����r���Ainfo[1]����r��ASize��2�ȊO�̏ꍇ�A�ُ�l
		if( info.size() != 2 )
			return false;

		m_mapAttrList.insert( std::pair<CString, CString>(info[0].c_str(), info[1].c_str()) );
	}

	return true;
}

// ���s���O�t�@�C���ɃI�v�V�����������o�͂���
void CCopyOverlapFeatureAttr::writeSettings()
{
	CString message;
	message = uh::str_util::format( message::getMsg(message::iWriteSettingInfo) );
	writeRunLog( message, false, true );

	// Source
	message = uh::str_util::format( message::getMsg(message::iWriteSource), m_setting.strSource );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteSourceTable), m_setting.strSourceTable );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteSourceField), m_setting.strSourceField );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteSourceWhere), m_setting.strSourceWhere );
	writeRunLog( message, false, true );

	// Target
	message = uh::str_util::format( message::getMsg(message::iWriteTarget), m_setting.strTarget );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteTargetTable), m_setting.strTargetTable );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteTargetField), m_setting.strTargetField );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteTargetWhere), m_setting.strTargetWhere );
	writeRunLog( message, false, true );

	// Mesh
	message = uh::str_util::format( message::getMsg(message::iWriteMesh), m_setting.strMesh );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteMeshTable), m_setting.strMeshTable );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteMeshListPath), m_setting.strMeshListPath );
	writeRunLog( message, false, true );

	// ���̏��
	message = uh::str_util::format( message::getMsg(message::iWriteExeMode), m_setting.strExeMode );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteAttrListPath), m_setting.strAttrListPath );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteDatumFlag), m_setting.bDatumFlag );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteFieldFlag), m_setting.bFieldFlag );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteOverlay), m_setting.dOverlay );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteMagnification), m_setting.dMagnification );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg( message::iWriteReducation ), m_setting.dReduction );
	writeRunLog( message, false, true );
	message = uh::str_util::format( message::getMsg(message::iWriteBuffer), m_setting.dBuffer );
	writeRunLog( message, false, true );
}

bool CCopyOverlapFeatureAttr::createChildVer( const IWorkspacePtr& ipWorkspace )
{
	// �q�o�[�W�������쐬
	SYSTEMTIME systemTime;
	::GetLocalTime( &systemTime );
	CString strChildVer;
	strChildVer.Format( childver_name_format, systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond );

	// �q�o�[�W�����쐬
	IVersionPtr ipCurVersion( ipWorkspace );
	IVersionPtr ipChildVersion;
	ipCurVersion->CreateVersion( _bstr_t( strChildVer ), &ipChildVersion );

	CString message;
	if( !ipChildVersion )
	{
		message.Format( _T( "%s : %s" ), message::getMsg( message::iErrorCreateVersion ), strChildVer );
		writeRunLog(message, false);
		return false;
	}

	// �쐬�����q�o�[�W�����ɃR�����g�ǉ�
	ipChildVersion->put_Description( _bstr_t( message::getMsg(message::iChildVerComment) ) );

	m_ipWorkspaceEdit = ( IWorkspaceEditPtr )ipChildVersion;
	if( !m_ipWorkspaceEdit )
	{
		message = uh::str_util::format( message::getMsg(message::iErrorEditWorkspace) );
		writeRunLog(message, false);
		return false;
	}
	else
	{
		message.Format( _T( "%s : %s" ), message::getMsg( message::iSuccessCreateVersion ), strChildVer );
		writeRunLog(message, false);
	}
	return true;
}

// ���[�N�X�y�[�X�I�[�v��
bool CCopyOverlapFeatureAttr::openWorkspace( const CString& name, IWorkspacePtr& ipWorkspace )
{
	// DB�ɐڑ�(�ڑ����g���C����3�b5��)
	ipWorkspace = sindy::create_workspace( name, true, max_retry_num, max_retry_time );
	if( !ipWorkspace )
		return false;

	return true;
}

// DB�̃I�[�v������
bool CCopyOverlapFeatureAttr::openDB()
{
	CString message;

	// ��r���̃��[�N�X�y�[�X�I�[�v��
	if( !openWorkspace(m_setting.strSource, m_ipSourceWorkspace) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotConnectDB), m_setting.strSource );
		writeRunLog(message, false);
		return false;
	}

	// ��r��̃��[�N�X�y�[�X�I�[�v��
	if( !openWorkspace(m_setting.strTarget, m_ipTargetWorkspace) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotConnectDB), m_setting.strTarget );
		writeRunLog(message, false);
		return false;
	}

	// ��r�悪SDE�̏ꍇ�͎q�o�[�W�������쐬����
	sindy::CWorkspace cWork(m_ipTargetWorkspace);
	if ( cWork.GetNameString()->IsSDE() )
	{
		if( !createChildVer(m_ipTargetWorkspace) )
			return false;
	}
	else
		m_ipWorkspaceEdit = m_ipTargetWorkspace;

	// ���b�V���̃��[�N�X�y�[�X�I�[�v��
	if( !openWorkspace(m_setting.strMesh, m_ipMeshWorkspace) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotConnectDB), m_setting.strMesh );
		writeRunLog(message, false);
		return false;
	}
	return true;
}

// �t�B�[�`���N���X���擾
bool CCopyOverlapFeatureAttr::openFeatureClass(const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass)
{
	IFeatureWorkspacePtr ipFeatureWork(ipWorkspace);

	if(FAILED(ipFeatureWork->OpenFeatureClass(CComBSTR(name), &ipFeatureClass)))
		return false;

	return true;
}

// ���̓t�B�[�`���N���X���擾
bool CCopyOverlapFeatureAttr::openFC()
{
	CString message;

	// ��r���̃t�B�[�`���N���X���擾
	if( !openFeatureClass( m_setting.strSourceTable, m_ipSourceWorkspace, m_ipSourceFC ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotOpenFC), m_setting.strSourceTable );
		writeRunLog(message, false);
		return false;
	}

	// ��r��̃t�B�[�`���N���X���擾�i�ҏW�p���[�N�X�y�[�X���g�p�j
	if( !openFeatureClass( m_setting.strTargetTable, m_ipWorkspaceEdit, m_ipTargetFC ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotOpenFC), m_setting.strTargetTable );
		writeRunLog(message, false);
		return false;
	}

	// ���b�V���̃t�B�[�`���N���X���擾
	if( !openFeatureClass( m_setting.strMeshTable, m_ipMeshWorkspace, m_ipMeshFC ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotOpenFC), m_setting.strMeshTable );
		writeRunLog(message, false);
		return false;
	}
	return true;
}

// �e�t�B�[���h��Index���擾
bool CCopyOverlapFeatureAttr::getFieldsIndex()
{
	CString message;
	using namespace sindy::schema;

	// �t�B�[���hMeshCode��Index
	m_nMeshCodeIndex = -1;
	if( FAILED( m_ipMeshFC->FindField(CComBSTR(category::mesh_code_table::kMeshCode), &m_nMeshCodeIndex) ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotGetIndex), category::mesh_code_table::kMeshCode );
		writeRunLog(message, false);
		return false;
	}

	// ��r���t�B�[���h��Index
	m_nSourceFieldIndex = -1;
	if( FAILED( m_ipSourceFC->FindField(CComBSTR(m_setting.strSourceField), &m_nSourceFieldIndex) ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotGetIndex), m_setting.strSourceField );
		writeRunLog(message, false);
		return false;
	}

	// ��r��t�B�[���h��Index
	m_nTargetFieldIndex = -1;
	if( FAILED( m_ipTargetFC->FindField(CComBSTR(m_setting.strTargetField), &m_nTargetFieldIndex) ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotGetIndex), m_setting.strTargetField );
		writeRunLog(message, false);
		return false;
	}

	// Shape�̃t�B�[���h����10�����ȉ��łȂ���΂Ȃ�Ȃ��Ƃ�������������̂ŁA
	// ���L3�t�B�[���h�iUpdateType�AModifyProgName�AProgModifyDate�j�����݂��Ȃ��\��������

	// �ŏI�X�V���e��Index
	m_nUpdateTypeIndex = -1;
	if( FAILED( m_ipTargetFC->FindField(CComBSTR(global::common_table::kUpdateType), &m_nUpdateTypeIndex) ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotGetIndex), global::common_table::kUpdateType );
		writeRunLog(message, false);
	}

	// �ŏI�X�V�v���O��������Index
	m_nModifyProgNameIndex = -1;
	if( FAILED( m_ipTargetFC->FindField(CComBSTR(global::common_table::kModifyProgName), &m_nModifyProgNameIndex) ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotGetIndex), global::common_table::kModifyProgName );
		writeRunLog(message, false);
	}

	// �ŏI�v���O�����X�V������Index
	m_nProgModifyDateIndex = -1;
	if( FAILED( m_ipTargetFC->FindField(CComBSTR(global::common_table::kProgModifyDate), &m_nProgModifyDateIndex) ) )
	{
		message = uh::str_util::format( message::getMsg(message::iNotGetIndex), global::common_table::kProgModifyDate );
		writeRunLog(message, false);
	}

	return true;
}

// ������
bool CCopyOverlapFeatureAttr::init(const Arguments& args)
{
	CString message;

	// �ݒ�t�@�C���̓ǂݍ���
	if( !readSettingFile(args.m_setting.c_str()) )
	{
		message = uh::str_util::format( message::getMsg(message::iReadFailed), args.m_setting.c_str() );
		writeRunLog(message, false);
		return false;
	}

	// ���b�V�����X�g�̓ǂݍ���
	if( !readMeshList(m_setting.strMeshListPath) )
	{
		message = uh::str_util::format( message::getMsg(message::iReadFailed), m_setting.strMeshListPath );
		writeRunLog(message, false);
		return false;
	}

	// �����ϊ��\�̓ǂݍ��݁i�K�{�ł͂Ȃ��j
	if( !m_setting.strAttrListPath.IsEmpty() )
	{
		if( !readAttributeList(m_setting.strAttrListPath) )
		{
			message = uh::str_util::format( message::getMsg(message::iReadFailed), m_setting.strAttrListPath );
		writeRunLog(message, false);
			return false;
		}
	}

	// ���O�t�@�C�����J��(Close��LogWriter�̃f�X�g���N�^�ŌĂяo���̂ł��̊֐����ł͌Ă΂Ȃ�)
	if( !openLogFiles(args.m_runlog.c_str(), args.m_errlog.c_str(), args) )
		return false;

	// �ݒ���̏o��
	writeSettings();

	return true;
}

// ���b�V�����X�g���A���b�V���ƃo�b�t�@�[�̃|���S�����擾
bool CCopyOverlapFeatureAttr::getMeshPolygons(std::map<unsigned long, std::pair<IGeometryPtr, IGeometryPtr>>& mapMeshGeo)
{
	CString message;
	using namespace sindy::schema;

	CString strSubFields;
	strSubFields.Format( _T("%s,%s"), category::mesh_code_table::kMeshCode, feature::kShape );

	IFeatureCursorPtr ipFeatureCursor;
	if( FAILED(m_ipMeshFC->Search(AheInitQueryFilter( nullptr, strSubFields, nullptr ), VARIANT_FALSE, &ipFeatureCursor)) )
	{
		message = uh::str_util::format( message::getMsg(message::iErrorMeshSearch), m_setting.strAttrListPath );
		writeRunLog(message, false);
		return false;
	}

	IFeaturePtr ipFeature;
	while ( S_OK == (ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature )
	{
		CComVariant vaMeshCode = 0;
		IGeometryPtr ipMeshGeom;
		ipFeature->get_Value(m_nMeshCodeIndex, &vaMeshCode);
		ipFeature->get_Shape(&ipMeshGeom);

		// TERTIARYMESH�̃��b�V���R�[�h��10���ŁAlVal�Ŏ擾�ł��Ȃ����߁AdblVal�Ŏ擾����
		unsigned long ulMeshCode = 0;
		if( 0 == m_setting.strMeshTable.CompareNoCase( global::citymesh::kTableName ) )
			ulMeshCode = vaMeshCode.dblVal;
		else
			ulMeshCode = vaMeshCode.lVal;

		if( m_setMeshList.find(ulMeshCode) != m_setMeshList.end() )
		{
			// ���b�V�����Ƀ��[�g������ܓx�o�x�̋����ɕϊ�
			double bufferSize = 0.0;
			IUnitConverterPtr ipUnitConv( CLSID_UnitConverter );
			ipUnitConv->ConvertUnits( m_setting.dBuffer, esriMeters, esriDecimalDegrees, &bufferSize );

			// �w�苗����Buffer���쐬
			IGeometryPtr ipBuffer;
			if( FAILED( ((ITopologicalOperatorPtr)ipMeshGeom)->Buffer( bufferSize, &ipBuffer ) ))
				continue;;

			mapMeshGeo[ulMeshCode] = std::make_pair( ipMeshGeom, ipBuffer );
			m_setMeshList.erase(ulMeshCode);
		}
	}

	// ���݂��Ȃ����b�V���R�[�h���w�肳�ꂽ���̃`�F�b�N
	if( m_setMeshList.size() > 0 )
	{
		for( const auto& mesh : m_setMeshList )
		{
			message = uh::str_util::format( message::getMsg(message::iNotExistMeshCode), mesh );
			writeRunLog(message, false);
		}
	}

	return true;
}

// ��r���̃W�I���g�����擾
bool CCopyOverlapFeatureAttr::getSourceObjectInMesh( const IGeometryPtr& ipMeshGeo, std::map<long, GeoClass>& mapSource)
{
	CString message;
	using namespace sindy::schema;

	IFeatureCursorPtr ipCursor;
	if( FAILED( m_ipSourceFC->Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), NULL, m_setting.strSourceWhere ), VARIANT_FALSE, &ipCursor ) ) || !ipCursor )
	{
		message = uh::str_util::format( message::getMsg(message::iErrorSourceSearch) );
		writeRunLog(message, false);
		return false;
	}


	IFeaturePtr ipFeature;
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature )
	{
		GeoClass SourceGeoClass;
		SourceGeoClass.oid         = ipFeature->GetOID();
		SourceGeoClass.geometry    = ipFeature->GetShapeCopy();
		SourceGeoClass.attr.first  = m_setting.strSourceField;
		SourceGeoClass.attr.second = uh::str_util::ToString( ipFeature->GetValue( m_nSourceFieldIndex ) );

		// ���n�n�����ϊ��t���O��True�A�����n�n����v���Ă��Ȃ��ꍇ�A��r��̑��n�n��^�Ƃ��čX�V����
		if( m_setting.bDatumFlag && !isSameSpRef() )
			SourceGeoClass.geometry->putref_SpatialReference(m_ipTargetSpRef);

		mapSource.insert( std::make_pair(SourceGeoClass.oid, SourceGeoClass) );
	}

	return true;
}

// �d�������擾
double CCopyOverlapFeatureAttr::getOverlay( const IGeometryPtr& ipSourceGeo, const IGeometryPtr& ipTargetGeo)
{
	// �d�����́A��r���Ɣ�r��̃t�B�[�`���d���������A��r��t�B�[�`���`��̉�%���߂邩��\���������B
	// 0%�`100%�̒l�敪�ƂȂ�B
	double overlay = -1.0;

	esriGeometryType sourceType = esriGeometryNull;
	ipSourceGeo->get_GeometryType( &sourceType );
	esriGeometryType targetType = esriGeometryNull;
	ipTargetGeo->get_GeometryType( &targetType );

	if( (sourceType != esriGeometryPoint && sourceType != esriGeometryPolyline && sourceType != esriGeometryPolygon)
		|| (targetType != esriGeometryPoint && targetType != esriGeometryPolyline && targetType != esriGeometryPolygon) )
	{
		// �z��O�̃W�I���g��
		CString message = uh::str_util::format( message::getMsg(message::iErrUnexpectedGeometryType) );
		writeRunLog(message, false);
		overlay = -1.0;
		return overlay;
	}

	// ��r��     ��r��     �d����						
	// �|���S��   �|���S��   �d�������̖ʐ� / �|���S���i��r��j�̖ʐ� * 100 [%]						
	// �|���S��   �|�����C�� �d�������̒��� / �|�����C���̑S�� * 100 [%]						
	// �|���S��   �|�C���g   �|���S�����Ƀ|�C���g������΁i�ڂ���ꍇ���܂ށj100%�A����ȊO��0%						
	// �|�����C�� �|���S��   �d�������̒��� / �|�����C���̑S�� * 100 [%]						
	// �|�����C�� �|�����C�� �d�������̒��� / �|�����C���i��r��j�̑S�� * 100 [%]						
	// �|�����C�� �|�C���g   �|�����C����Ƀ|�C���g�������100%�A����ȊO��0%						
	// �|�C���g   �|���S��   �|���S�����Ƀ|�C���g������΁i�ڂ���ꍇ���܂ށj100%�A����ȊO��0%						
	// �|�C���g   �|�����C�� �|�����C����Ƀ|�C���g�������100%�A����ȊO��0%						
	// �|�C���g   �|�C���g   �|�C���g�ƃ|�C���g����v���Ă����100%�A����ȊO��0%						

	IGeometryPtr ipIntersectGeo;

	// ��r�悪�|�C���g�̏ꍇ�A��r����Within�Ŕ��肷��
	switch (targetType)
	{
	case esriGeometryPoint:
		{
			// �|�C���g���|���S���̕ӏ�ɂ������ꍇ�A���̓|�C���g���|�����C���̒[�_�ɂ������ꍇ�AAheIsWithin2()��False��Ԃ����߁A
			// AheIsDisjoint����ŏd�����𔻒肷��
			if( AheIsDisjoint(ipTargetGeo, ipSourceGeo) )
				overlay = 0.0;
			else
				overlay = 100.0;
			break;
		}
	case esriGeometryPolyline:
		{
			if( sourceType == esriGeometryPoint )
			{
				// �|�C���g���|�����C���̒[�_�ɂ������ꍇ�AAheIsWithin2()��False��Ԃ����߁A
				// AheIsDisjoint����ŏd�����𔻒肷��
				if( AheIsDisjoint(ipTargetGeo, ipSourceGeo) )
					overlay = 0.0;
				else
					overlay = 100.0;
			}
			else
			{
				ipIntersectGeo = AheIntersect(ipSourceGeo, ipTargetGeo, esriGeometry1Dimension);
				if( !ipIntersectGeo )
				{
					overlay = 0.0;
					break;
				}
				ipIntersectGeo->putref_SpatialReference(m_ipTargetSpRef);

				// ��v�Ȃ�100%�ɂ���
				// Shape�ȂǍ��W���x�̖����ꍇ�ɂ́A������ʐόv�Z�ō��W���x�̂���ꍇ�ɔ�ׂĔ����ȍ������܂��
				// �����A���̎��ł��W�I���g�����͈̂�v���邽�߁A100%�Ƃ݂Ȃ�
				if( AheIsEqual( ipTargetGeo, ipIntersectGeo ) )
				{
					overlay = 100.0;
					break;
				}

				double intersectLength = 0.0, baseLength = 0.0;
				IPolylinePtr(ipIntersectGeo)->get_Length(&intersectLength);
				IPolylinePtr(ipTargetGeo)->get_Length(&baseLength);

				overlay = (intersectLength / baseLength) * 100;
			}
			break;
		}
	case esriGeometryPolygon:
		{
			// ��r�悪�|���S���̏ꍇ�A�������Ă��镔���̎����͔�r���Ɠ���
			if( sourceType == esriGeometryPoint )
			{
				// �|�C���g���|���S���̕ӏ�ɂ������ꍇ�AAheIsWithin2()��False��Ԃ����߁A
				// AheIsDisjoint����ŏd�����𔻒肷��
				if( AheIsDisjoint(ipSourceGeo, ipTargetGeo) )
					overlay = 0.0;
				else
					overlay = 100.0;
			}
			else if( sourceType == esriGeometryPolyline )
			{
				ipIntersectGeo = AheIntersect(ipSourceGeo, ipTargetGeo, esriGeometry1Dimension);
				if( !ipIntersectGeo )
				{
					overlay = 0.0;
					break;
				}
				ipIntersectGeo->putref_SpatialReference(m_ipTargetSpRef);

				// ��v�Ȃ�100%�ɂ���
				if( AheIsEqual( ipTargetGeo, ipIntersectGeo ) )
				{
					overlay = 100.0;
					break;
				}

				double intersectLength = 0.0, baseLength = 0.0;
				IPolylinePtr(ipIntersectGeo)->get_Length(&intersectLength);
				IPolylinePtr(ipSourceGeo)->get_Length(&baseLength);

				overlay = (intersectLength / baseLength) * 100;
			}
			else
			{
				ipIntersectGeo = AheIntersect(ipSourceGeo, ipTargetGeo, esriGeometry2Dimension);
				if( !ipIntersectGeo )
				{
					overlay = 0.0;
					break;
				}
				ipIntersectGeo->putref_SpatialReference(m_ipTargetSpRef);

				// ��v�Ȃ�100%�ɂ���
				if( AheIsEqual( ipTargetGeo, ipIntersectGeo ) )
				{
					overlay = 100.0;
					break;
				}

				double intersectArea = 0.0, baseArea = 0.0;
				IAreaPtr(ipIntersectGeo)->get_Area(&intersectArea);
				IAreaPtr(ipTargetGeo)->get_Area(&baseArea);

				overlay = (intersectArea / baseArea) * 100;
			}
			break;
		}
	default:
		{
			overlay = -1.0;
			break;
		}
	}

	return overlay;
}

// 臒l�ȏ�̔�r���Ƃ̏d�������擾
std::multimap<double, GeoClass> CCopyOverlapFeatureAttr::getAllOverlay(const IGeometryPtr& ipTargetGeo, const std::set<long>& sourceIDs, const std::map<long, GeoClass>& mapSource, ErrItem& errItem)
{
	// �d�����̓�����r�����������݂���\�������邽�߁Astd::multimap���g�p����
	std::multimap<double, GeoClass> mapSourceOverlay;

	for( const auto& findSourceID : sourceIDs )
	{
		auto findSourceItr = mapSource.find( findSourceID );
		if( findSourceItr == mapSource.end() )
			continue;

		double overlay = getOverlay( findSourceItr->second.geometry, ipTargetGeo );

		// 臒l�ȏ�͏����ΏۂƂ���
		// ���_���100%�𒴂��邱�Ƃ͂��蓾�Ȃ�
		if( m_setting.dOverlay <= overlay && overlay <= 100 )
			mapSourceOverlay.insert( std::make_pair(overlay, findSourceItr->second) );
		else
		{
			errItem.nSrcOID = findSourceID;
			errItem.strNote.Format( _T("�d����:%.15lf, 臒l:%.15lf�`100"), overlay, m_setting.dOverlay );

			setDetailErrorInfo( err_code::errFailureOverlay, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
		}
	}
	return mapSourceOverlay;
}

// ��r���Ƃ̊g�k�������Ė��������̂��擾
std::set<long> CCopyOverlapFeatureAttr::getAllOkScale(const IGeometryPtr& ipTargetGeo, const std::set<long>& sourceIDs, const std::map<long, GeoClass>& mapSource, ErrItem& errItem)
{
	std::set<long> setSourceIDs;

	for( const auto& findSourceID : sourceIDs )
	{
		auto findSourceItr = mapSource.find( findSourceID );
		if( findSourceItr == mapSource.end() )
			continue;

		// ��r��      ��r��      �g�k��	
		// �|���S��    �|���S��    ��r���̖ʐ� / ��r��̖ʐ� * 100 [%]
		// �|�����C��  �|�����C��  ��r���̑S�� / ��r��̑S�� * 100 [%]

		IGeometryPtr ipSourceGeo = findSourceItr->second.geometry;

		esriGeometryType sourceType = esriGeometryNull;
		ipSourceGeo->get_GeometryType( &sourceType );
		esriGeometryType targetType = esriGeometryNull;
		ipTargetGeo->get_GeometryType( &targetType );

		// ��r���Ɣ�r�悪�����Ƃ��|�����C���A�����͗����Ƃ��|���S���̏ꍇ�̂݁A�g�k�����v�Z����K�v������
		double scale = 0.0;
		if( sourceType == esriGeometryPolyline && targetType == esriGeometryPolyline )
		{
			// ��v�Ȃ�100%�ɂ���
			if( AheIsEqual( ipTargetGeo, ipSourceGeo ) )
			{
				scale = 100.0;
			}
			else
			{
				// ��r�悪����̂��߁A1.0�Ə���������
				double sourceLength = 0.0, targetLength = 1.0;
				IPolylinePtr( ipSourceGeo )->get_Length( &sourceLength );
				IPolylinePtr( ipTargetGeo )->get_Length( &targetLength );

				scale = ( sourceLength / targetLength ) * 100;
			}
		}
		else if( sourceType == esriGeometryPolygon && targetType == esriGeometryPolygon )
		{
			// ��v�Ȃ�100%�ɂ���
			if( AheIsEqual( ipTargetGeo, ipSourceGeo ) )
			{
				scale = 100.0;
			}
			else
			{
				// ��r�悪����̂��߁A1.0�Ə���������
				double sourceArea = 0.0, targetArea = 1.0;
				IAreaPtr( ipSourceGeo )->get_Area( &sourceArea );
				IAreaPtr( ipTargetGeo )->get_Area( &targetArea );

				scale = ( sourceArea / targetArea ) * 100;
			}
		}
		// ��L�ȊO�̏ꍇ�A�S��r���������ΏۂƂȂ邽�߁A�g�k����100%�Ɛݒ肷��
		else
			scale = 100.0;

		// �k�����ȏ�A�g�嗦�ȉ��̒l�敪�ƂȂ�
		if( m_setting.dReduction <= scale && scale <= m_setting.dMagnification )
			setSourceIDs.insert(findSourceID);
		else
		{
			errItem.nSrcOID = findSourceID;
			errItem.strNote.Format( _T("�g�k��:%.15lf, 臒l:%.15lf�`%.15lf"), scale, m_setting.dReduction, m_setting.dMagnification);

			setDetailErrorInfo( err_code::errFailureScale, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
		}

	}
	return setSourceIDs;
}

// �t�B�[���h�l���w��l�ɕϊ�
CComVariant CCopyOverlapFeatureAttr::convertFieldValue(const CString& strValue)
{
	CComVariant rtnValue(strValue);

	esriFieldType targetFieldType = m_ipTargetField->GetType();

	switch (targetFieldType)
	{
	case esriFieldTypeSmallInteger:
		rtnValue.ChangeType(VT_I4);
		break;
	case esriFieldTypeInteger:
		rtnValue.ChangeType(VT_I8);
		break;
	case esriFieldTypeSingle:
		rtnValue.ChangeType(VT_R4);
		break;
	case esriFieldTypeDouble:
		rtnValue.ChangeType(VT_R8);
		break;
	case esriFieldTypeString:
		rtnValue.ChangeType(VT_BSTR);
		break;
	default:
		{
			// �z��O�̃t�B�[���h�^
			CString message = uh::str_util::format( message::getMsg(message::iErrUnexpectedFieldType) );
			writeRunLog(message, false);
		}
		break;
	}

	return rtnValue;
}

// �o�͗p�̍��W�l���擾
bool CCopyOverlapFeatureAttr::getShapeCoordinate(const IGeometryPtr& shape, ErrItem& errItem)
{
	switch(shape->GetGeometryType())
	{
	case esriGeometryType::esriGeometryPoint:
		{
			IPointPtr point = shape;
			if( !point ) return false;

			errItem.dLon = point->GetX();
			errItem.dLat = point->GetY();
		}
		break;
	case esriGeometryType::esriGeometryPolyline:
		{
			IPointCollectionPtr pointColl( shape );
			if( !pointColl ) return false;

			long pointNum = pointColl->GetPointCount();
			if( !pointNum )	return false;

			IPointPtr point = pointColl->GetPoint(pointNum/2);
			if ( !point ) return false;

			errItem.dLon = point->GetX();
			errItem.dLat = point->GetY();
		}
		break;
	case esriGeometryType::esriGeometryPolygon:
		{
			IAreaPtr ipArea( shape );
			if ( !ipArea ) return false;

			IPointPtr point;
			ipArea->get_Centroid( &point );
			if ( !point ) return false;

			errItem.dLon = point->GetX();
			errItem.dLat = point->GetY();
		}
		break;
	default:
		return false;
	}
	return true;
}

// �G���[�o�͗p�̋��ʏ����擾
ErrItem CCopyOverlapFeatureAttr::getCommonErrorInfo(unsigned long meshCode, const GeoClass& target)
{
	ErrItem errItem;
	errItem.strLayer  = m_setting.strTargetTable;
	errItem.nOID      = target.oid;
	errItem.nMeshCode = meshCode;

	// �o�͗p�̍��W�����擾
	getShapeCoordinate( target.geometry, errItem );

	return errItem;
}

// �G���[�o�͗p�̏ڍ׏���ݒ�
void CCopyOverlapFeatureAttr::setDetailErrorInfo(err_code::ECode errCode, ErrItem& errItem)
{
	switch (errCode)
	{
	case err_code::errUpdateFailed:
	case err_code::errPutValueFailed:
		errItem.strErrLevel = getErrLevelMsg( err_level::levelError );
		break;
	case err_code::errOverBuffer:
	case err_code::errSrcNotExist:
	case err_code::errOverlaySrcNotExist:
	case err_code::errScaleSrcNotExist:
	case err_code::errBothSrcNotExist:
	case err_code::errMulitSrcSkipMode:
	case err_code::errMulitSrcAttrNoMatch:
	case err_code::errAttrNotInAttrList:
	case err_code::errNotSameField:
	case err_code::errFailureOverlay:
	case err_code::errFailureScale:
		errItem.strErrLevel = getErrLevelMsg( err_level::levelWarning );
		break;
	case err_code::errSuccessUpdate:
		errItem.strErrLevel = getErrLevelMsg( err_level::levelInfo );
		break;
	default:
		errItem.strErrLevel = getErrLevelMsg( err_level::levelInfo );
		break;
	}
	errItem.nErrCode = errCode;
	errItem.strErrMsg = getErrCodeMsg(errCode);
	return;
}

// �G���[�o�͗p�̏ڍ׏������Z�b�g
void CCopyOverlapFeatureAttr::resetDetailErrorInfo(ErrItem& errItem)
{
	errItem.strErrLevel   = _T("");
	errItem.nErrCode      = 0;
	errItem.strErrMsg     = _T("");
	errItem.nSrcOID       = -1;
	errItem.strNote       = _T("");
}

// �Ώۃt�B�[���h���X�V
bool CCopyOverlapFeatureAttr::putValues(const CString& strValue, IFeaturePtr& ipFeature, ErrItem& errItem)
{
	using namespace sindy::schema;

	// �Ώۃt�B�[���h���X�V����
	if( FAILED( ipFeature->put_Value( m_nTargetFieldIndex, convertFieldValue(strValue) ) ) )
	{
		errItem.strNote = m_setting.strTargetField;
		setDetailErrorInfo( err_code::errPutValueFailed, errItem );
		writeErrLog( errItem, false );
		resetDetailErrorInfo( errItem );
		return false;
	}

	// ���L�ŏI�X�V�n�̂R�t�B�[���h�ɑ΂��AShape�AFGDB�f�[�^�ɂ͑��݂��Ȃ��\�������邽�߁A���݂��Ă���t�B�[���h�����X�V����
	if( -1 != m_nUpdateTypeIndex )
	{
		if( FAILED( ipFeature->put_Value( m_nUpdateTypeIndex, CComVariant( global::common_table::gl_chg_code::kAttrModify ) ) ) )
		{
			errItem.strNote = global::common_table::kUpdateType;
			setDetailErrorInfo( err_code::errPutValueFailed, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			return false;
		}
	}

	if( -1 != m_nModifyProgNameIndex )
	{
		if( FAILED( ipFeature->put_Value( m_nModifyProgNameIndex, CComVariant( CVersion().GetInternalName() ) ) ) )
		{
			errItem.strNote = global::common_table::kModifyProgName;
			setDetailErrorInfo( err_code::errPutValueFailed, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			return false;
		}
	}

	if( -1 != m_nProgModifyDateIndex )
	{
		if( FAILED( ipFeature->put_Value( m_nProgModifyDateIndex, CComVariant( getNowTime().c_str() ) ) ) )
		{
			errItem.strNote = global::common_table::kProgModifyDate;
			setDetailErrorInfo( err_code::errPutValueFailed, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			return false;
		}
	}
	return true;
}

// ��r��̃t�B�[���h���X�V
bool CCopyOverlapFeatureAttr::updateTargetField(const std::pair<unsigned long, std::pair<IGeometryPtr, IGeometryPtr>>& meshGeoRec, const std::map<long, GeoClass>& mapSource, const BGRTree& sourceRTree)
{
	CString message;
	using namespace sindy::schema;

	// ���b�V�����̔�r�����������
	IFeatureCursorPtr ipUpdateCursor;
	if( FAILED( m_ipTargetFC->Update( AheInitQueryFilter( AheInitSpatialFilter( NULL, meshGeoRec.second.first ), NULL, m_setting.strTargetWhere ), VARIANT_FALSE, &ipUpdateCursor ) ) || !ipUpdateCursor )
	{
		message = uh::str_util::format( message::getMsg(message::iErrorTargetSearch) );
		writeRunLog(message, false);
		return false;
	}

	long nCount = 0; // ��r��̌����i�o�͗p�j
	IFeaturePtr ipFeature;
	while( S_OK == ipUpdateCursor->NextFeature( &ipFeature ) && ipFeature )
	{
		++nCount;

		GeoClass targetGeoClass;
		targetGeoClass.oid         = ipFeature->GetOID();

		// �����ς݂̏ꍇ�A�X�L�b�v�i���b�V�����ׂ��W�I���g���ɑ΂��A�d������������邽�߂Ɂj
		if( m_setProcessedOID.find(targetGeoClass.oid) != m_setProcessedOID.end() )
			continue;

		targetGeoClass.geometry    = ipFeature->GetShapeCopy();
		targetGeoClass.attr.first  = m_setting.strTargetField;
		targetGeoClass.attr.second = uh::str_util::ToString( ipFeature->GetValue( m_nTargetFieldIndex ) );

		ErrItem errItem = getCommonErrorInfo( meshGeoRec.first, targetGeoClass );

		// ��r��̃W�I���g������r���̌����͈́i�o�b�t�@�[�͈̔́j���͂ݏo�����ꍇ�A���O���o�͂��A�X�L�b�v����
		IRelationalOperatorPtr ipRel( targetGeoClass.geometry.m_T );
		VARIANT_BOOL isWithin = VARIANT_FALSE;
		ipRel->Within(meshGeoRec.second.second, &isWithin );
		if( isWithin != VARIANT_TRUE)
		{
			setDetailErrorInfo( err_code::errOverBuffer, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			continue;
		}

		// �����������r����ID�Q���擾
		//�i��r���̃|�����C������r��̃|���S���̕ӂ̏�ɂ������ꍇ�AgetIntersectsIDs��0��Ԃ����߁A�R��Ȃ��悤��GetBox��0.1m�g�傷��j
		std::set<long> sourceIDs = getIntersectsIDs( targetGeoClass.GetBox(0.1), sourceRTree );

		// �d�Ȃ��Ă����r�������݂��Ȃ�
		if( sourceIDs.empty() )
		{
			setDetailErrorInfo( err_code::errSrcNotExist, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			continue;
		}

		// ��r���Ƃ̏d�������擾�i�d�����̓�����r�����������݂���\�������邽�߁Astd::multimap���g�p����j
		std::multimap<double, GeoClass> mapSourceOverlay;  // Key:�d����; Value:��r���̃W�I���g���ƃt�B�[���h�l�����i�[����N���X
		mapSourceOverlay = getAllOverlay( targetGeoClass.geometry, sourceIDs, mapSource, errItem );

		// �d������臒l�𖞂�����r�������݂��Ȃ�
		if( mapSourceOverlay.empty() )
		{
			setDetailErrorInfo( err_code::errOverlaySrcNotExist, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			continue;
		}

		// �g�k���𖞂�����r����OID���擾
		std::set<long> setSourceScale;
		setSourceScale = getAllOkScale( targetGeoClass.geometry, sourceIDs, mapSource, errItem );

		// �g�嗦�Ək������臒l�𖞂�����r�������݂��Ȃ�
		if( setSourceScale.empty() )
		{
			setDetailErrorInfo( err_code::errScaleSrcNotExist, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			continue;
		}

		// �d�����Ɗg�嗦�𗼕��Ƃ���������r�����擾
		std::multimap<double, GeoClass>::iterator sourceItr;
		for( sourceItr = mapSourceOverlay.begin(); sourceItr != mapSourceOverlay.end(); )
		{
			if( setSourceScale.find(sourceItr->second.oid) == setSourceScale.end() )
				sourceItr = mapSourceOverlay.erase( sourceItr );
			else
				++sourceItr;
		}

		// �d�����Ɗg�嗦�𗼕��Ƃ���������r�������݂��Ȃ�
		if( mapSourceOverlay.empty() )
		{
			setDetailErrorInfo( err_code::errBothSrcNotExist, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
			continue;
		}

		// �d�����Ɗg�嗦�𖞂�����r���ɑ΂��A�ł��d�����������t�B�[�`���ōX�V����
		// �imapSourceOverlay���d�����̏����Ń\�[�g����邽�߁A�Ō���̏d�������ł������j
		std::multimap<double, GeoClass>::reverse_iterator rItr;
		rItr = mapSourceOverlay.rbegin();

		long maxOverlayOID = rItr->second.oid;
		double maxOverlay = rItr->first;
		CString maxAttrValue = rItr->second.attr.second;

		bool bUpdate = true; // �X�V�K�v�t���O

		// ��Ԗڂ����r����
		++rItr;
		for( ; rItr != mapSourceOverlay.rend(); ++rItr )
		{
			if( maxOverlay > rItr->first )
				break;

			// �ł��d�����������t�B�[�`����2�ȏ�A�����s���[�h���X�L�b�v���[�h�̏ꍇ�A�X�V���Ȃ�
			if( maxOverlay == rItr->first && 0 == m_setting.strExeMode.CompareNoCase(exemode::skipMode) )
			{
				errItem.nSrcOID = rItr->second.oid;
				errItem.strNote.Format( _T("�t�B�[�`���[1��OID:%d, �d����:%f, �t�B�[�`���[2��OID:%d, �d����:%f"),
					maxOverlayOID, maxOverlay, rItr->second.oid, rItr->first );

				setDetailErrorInfo( err_code::errMulitSrcSkipMode, errItem );
				writeErrLog( errItem, false );
				resetDetailErrorInfo( errItem );
				bUpdate = false;
				break;
			}

			// �ł��d�����������t�B�[�`����2�ȏ�A�������l����v���Ă��Ȃ��ꍇ�A�X�V���Ȃ�
			if( maxOverlay == rItr->first && maxAttrValue != rItr->second.attr.second )
			{
				errItem.nSrcOID = rItr->second.oid;
				errItem.strNote.Format( _T("�t�B�[�`���[1��OID:%d, �����l:%s, �t�B�[�`���[2��OID:%d, �����l:%s"),
					maxOverlayOID, maxAttrValue, rItr->second.oid, rItr->second.attr.second );

				setDetailErrorInfo( err_code::errMulitSrcAttrNoMatch, errItem );
				writeErrLog( errItem, false );
				resetDetailErrorInfo( errItem );
				bUpdate = false;
				break;
			}
		}

		// �X�V����K�v���Ȃ��ꍇ�A�X�L�b�v
		if( !bUpdate )
			continue;

		// �����ϊ��\���w�肳�ꂽ�ꍇ�A�����ϊ��\�ɂ��X�V����(key:��r���̑����l�Avalue:��r��̑����l)
		if( !m_mapAttrList.empty() )
		{
			std::map<CString, CString>::iterator attrItr;
			attrItr = m_mapAttrList.find( maxAttrValue );

			// �ϊ��\�ɒl���Ȃ��ꍇ�͍X�V�X�L�b�v
			if( attrItr == m_mapAttrList.end() )
			{
				setDetailErrorInfo( err_code::errAttrNotInAttrList, errItem );
				writeErrLog( errItem, false );
				resetDetailErrorInfo( errItem );
				continue;
			}
			else
				maxAttrValue = attrItr->second;
		}

		// �Ώۃt�B�[���h���X�V
		if( !updateField( maxOverlayOID, maxAttrValue, ipFeature, errItem ) )
			continue;

		if( FAILED(ipUpdateCursor->UpdateFeature(ipFeature)) )
		{
			// �X�V���s
			setDetailErrorInfo( err_code::errUpdateFailed, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
		}
		else
		{
			// ����X�V
			errItem.nSrcOID = maxOverlayOID;
			setDetailErrorInfo( err_code::errSuccessUpdate, errItem );
			writeErrLog( errItem, false );
			resetDetailErrorInfo( errItem );
		}

		// �d������������邽�߂ɁA�����ς݂̔�r����L�^����
		m_setProcessedOID.insert(targetGeoClass.oid);
	}

	// ��r�悪���݂��Ȃ�
	if( 0 == nCount )
	{
		message = uh::str_util::format( message::getMsg(message::iNotExistTarget) );
		writeRunLog(message, false);
	}
	return true;
}

// ���n�n�̃`�F�b�N
bool CCopyOverlapFeatureAttr::checkDatum()
{
	IGeoDatasetPtr(m_ipSourceFC)->get_SpatialReference(&m_ipSourceSpRef);
	IGeoDatasetPtr(m_ipTargetFC)->get_SpatialReference(&m_ipTargetSpRef);

	// ���n�n�����ϊ��t���O��False�A����r���Ɣ�r��̑��n�n����v���Ă��Ȃ��ꍇ�A�����𒆎~����
	if( !m_setting.bDatumFlag && !isSameSpRef() )
	{
		CString message = uh::str_util::format( message::getMsg( message::iNotSameSpRef ) );
		writeRunLog( message, false );
		return false;
	}
	return true;
}

// �t�B�[���h�^����v���Ă��邩���`�F�b�N
bool CCopyOverlapFeatureAttr::isSameField()
{
	if( !m_ipSourceField || !m_ipTargetField )
		return false;

	// �^�C�v�A���A�������`�F�b�N
	esriFieldType sourceFieldType = m_ipSourceField->GetType();
	esriFieldType targetFieldType = m_ipTargetField->GetType();

	if( sourceFieldType != targetFieldType )
		return false;

	if( m_ipSourceField->GetLength() != m_ipTargetField->GetLength() )
		return false;

	if( m_ipSourceField->GetPrecision() != m_ipTargetField->GetPrecision() )
		return false;

	if( m_ipSourceField->GetScale() != m_ipTargetField->GetScale() )
		return false;

	return true;
}

// ���n�n����v���Ă��邩���`�F�b�N
bool CCopyOverlapFeatureAttr::isSameSpRef()
{
	if( !m_ipSourceSpRef || !m_ipTargetSpRef )
		return false;

	long lSrcFcCode = m_ipSourceSpRef->GetFactoryCode();
	long lTrgFcCode = m_ipTargetSpRef->GetFactoryCode();

	return ( lSrcFcCode == lTrgFcCode );
}

// �t�B�[���h�̃^�C�v�������Ώۂł��邩���`�F�b�N
bool CCopyOverlapFeatureAttr::isProcessType( const esriFieldType& fieldType )
{
	// �����^�A�����^�A������̂ݏ����Ώۂł���
	switch (fieldType)
	{
	case esriFieldTypeSmallInteger:
	case esriFieldTypeInteger:
	case esriFieldTypeSingle:
	case esriFieldTypeDouble:
	case esriFieldTypeString:
		return true;
	default:
		return false;
	}
}

// �t�B�[���h�̃^�C�v�������Ώۂł��邩���`�F�b�N
bool CCopyOverlapFeatureAttr::isProcessType()
{
	if( !m_ipSourceField || !m_ipTargetField )
		return false;

	esriFieldType sourceFieldType = m_ipSourceField->GetType();
	esriFieldType targetFieldType = m_ipTargetField->GetType();

	if( !isProcessType(sourceFieldType) || !isProcessType(targetFieldType) )
		return false;

	return true;
}

// �t�B�[���h�^�̃`�F�b�N
bool CCopyOverlapFeatureAttr::checkFieldType()
{
	IFieldsPtr ipSourceFields = m_ipSourceFC->GetFields();
	IFieldsPtr ipTargetFields = m_ipTargetFC->GetFields();

	ipSourceFields->get_Field( m_nSourceFieldIndex, &m_ipSourceField );
	ipTargetFields->get_Field( m_nTargetFieldIndex, &m_ipTargetField );

	// ��r���Ɣ�r��̃t�B�[���h�^�������ΏۊO�̏ꍇ�A�����𒆎~����
	if( !isProcessType() )
	{
		CString message = uh::str_util::format( message::getMsg(message::iErrUnexpectedFieldType) );
		writeRunLog(message, false);
		return false;
	}

	// �t�B�[���h�^�����ϊ��t���O��False�A����r���Ɣ�r��̃t�B�[���h�^����v���Ă��Ȃ��ꍇ�A�����𒆎~����
	if( !m_setting.bFieldFlag )
	{
		if( !isSameField() )
		{
			CString message = uh::str_util::format( message::getMsg(message::iNotSameField) );
			writeRunLog(message, false);
			return false;
		}
	}

	return true;
}

// �Ώۃt�B�[���h���X�V
bool CCopyOverlapFeatureAttr::updateField( long maxOverlayOID, const CString& maxAttrValue, IFeaturePtr& ipFeature, ErrItem& errItem )
{
	// ��r��Ɣ�r���̃t�B�[���h�̌^����v���Ă��Ȃ��ꍇ�A�x�����o�͂���
	if( !isSameField() )
	{
		errItem.nSrcOID = maxOverlayOID;
		setDetailErrorInfo( err_code::errNotSameField, errItem );
		writeErrLog( errItem, false );
		resetDetailErrorInfo( errItem );
	}

	// ��r��Ɣ�r���̑��n�n����v���Ă��Ȃ��ꍇ�A�x�����o�͂���
	if( !isSameSpRef() )
	{
		errItem.nSrcOID = maxOverlayOID;
		setDetailErrorInfo( err_code::errNotSameSpRef, errItem );
		writeErrLog( errItem, false );
		resetDetailErrorInfo( errItem );
	}

	// �Ώۃt�B�[���h���X�V����
	if( !putValues(maxAttrValue, ipFeature, errItem) )
		return false;

	return true;
}

// ���s
bool CCopyOverlapFeatureAttr::execute(const Arguments& args)
{
	// ������
	if( !init(args) )
		return false;

	try
	{
		// DB�̃I�[�v��
		if( !openDB() )
			return false;

		// �t�B�[�`���N���X�̃I�[�v��
		if( !openFC() )
			return false;

		// �e�t�B�[���h��Index���擾
		if( !getFieldsIndex() )
			return false;

		// ���n�n�����ϊ��t���O�̃`�F�b�N
		if( !checkDatum() )
			return false;

		// �t�B�[���h�^�����ϊ��t���O�̃`�F�b�N
		if( !checkFieldType() )
			return false;

		// �����͈͂̃��b�V�����擾
		std::map<unsigned long, std::pair<IGeometryPtr, IGeometryPtr>> mapMeshGeo;  // Key:���b�V���R�[�h; Value:���b�V���`��,�o�b�t�@�[�`��
		if( !getMeshPolygons(mapMeshGeo) )
			return false;

		CString message;
		if( mapMeshGeo.empty() )
		{
			message = uh::str_util::format( message::getMsg(message::iEmptyMeshGeo) );
			writeRunLog(message, false);
			return false;
		}

		// �����̃X�R�[�v�𔲂����狭���I��abort
		// (����abort�́A�ҏW�J�n����Ă���Ƃ������L���ɂȂ�悤�Ȏ����ɂȂ��Ă���)
		uh::scope_guard scope( std::bind( DatabaseUtility::abortEdit, m_ipWorkspaceEdit, message ) );
		if( !DatabaseUtility::startEdit(m_ipWorkspaceEdit, message) )
		{
			writeRunLog(message, false);
			return false;
		}
		else
		{
			message = uh::str_util::format( message::getMsg(message::iSuccessStartEditing) );
			writeRunLog(message, false);
		}

		// ���b�V���P�ʂŏ�������
		for( const auto& meshGeoRec : mapMeshGeo )
		{
			// �����J�n�̃��O���o��
			message = uh::str_util::format( message::getMsg(message::iStartMesh), meshGeoRec.first );
			writeRunLog(message);

			// �o�b�t�@�[�͈͓��̔�r���W�I���g�����擾�i���b�V�����ׂ��W�I���g�������݂���\�����l�����邽�߁j
			std::map<long, GeoClass> mapSource;  // Key:��r��OID; Value:��r���̃W�I���g���ƃt�B�[���h�l�����i�[����N���X
			if( !getSourceObjectInMesh( meshGeoRec.second.second, mapSource) )
				continue;

			// ��r�������݂��Ȃ�
			if( mapSource.empty() )
			{
				message = uh::str_util::format( message::getMsg(message::iNotExistSource) );
				writeRunLog(message, false);
				continue;
			}

			// R-Tree���쐬����
			BGRTree sourceRTree;
			createRTree( mapSource, sourceRTree );

			// �X�V����
			if( !updateTargetField( meshGeoRec, mapSource, sourceRTree ) )
				continue;

		}

		// �ҏW�I��
		if( !DatabaseUtility::stopEdit(m_ipWorkspaceEdit, message) )
		{
			writeRunLog(message, false);
			return false;
		}
		else
		{
			message = uh::str_util::format( message::getMsg(message::iSuccessStopEditing) );
			writeRunLog(message, false);
		}
	}
	catch(const std::exception& e)
	{
		m_setMeshList.clear();
		m_mapAttrList.clear();
		m_setProcessedOID.clear();
		return false;
	}

	return true;
}