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
#include "OutputLog.h"
#include <boost/filesystem.hpp>

namespace
{
	/**
	 * @brief �f�B���N�g���p�X���Ƀt�@�C���o�[�W�����Ɠ��t��t�^�����t�@�C���𐶐�
	 * @param fileName [in] ��������t�@�C����
	 * @param dirPath  [in] �f�B���N�g���p�X
	 */
inline CString createFilepath(
	const CString& fileName
	, const CString& dirPath
	, CVersion& version
	, SYSTEMTIME& time
	)
{
	boost::filesystem::path filepath(dirPath);
	filepath /= version.GetInternalName() + _T("_") + uh::str_util::ToString(&time, _T("%04d%02d%02d%02d%02d%02d")) + _T("_") + fileName;
	return filepath.c_str();
}

} // namespace

bool COutPut::Open( const CString& strDirName )
{
	SYSTEMTIME aTime;
	GetLocalTime(&aTime);
	CVersion cVer;
	CStringA runLogFile = createFilepath( _T("run.log"), strDirName, cVer, aTime);
	CStringA errLogFile = createFilepath( _T("err.log"), strDirName, cVer, aTime);
	m_ofs_run.open( runLogFile.GetString() );
	m_ofs_error.open( errLogFile.GetString() );

	if( ! m_ofs_run || ! m_ofs_error )
		return false;
	return true;
}

void COutPut::Close(){
	m_ofs_run.close();
	m_ofs_error.close();
}

void COutPut::OutputHeader()
{
	if( ! m_ofs_error )
		return;
	CString logStyle = _T("# FREESTYLELOG");
	m_ofs_error << CT2CA( logStyle ) << std::endl;
	std::vector<CString> fieldList = boost::assign::list_of
		( _T( "FLAG" ) )
		( _T( "LAYER" ) )
		( _T( "OBJECTID" ) )
		( _T( "���b�V���R�[�h" ) )
		( _T( "LON" ) )
		( _T( "LAT" ) )
		( _T( "�G���[���x��" ) )
		( _T( "�G���[�R�[�h" ) )
		( _T( "�G���[���b�Z�[�W" ) )
		( _T( "���1" ) )
		( _T( "���2" ) )
		( _T( "�ŏI�X�V��" ) )
		( _T( "�ŏI�X�V����" ) )
		( _T( "�ŏI�X�V�v���O����" ) )
		( _T( "�ŏI�v���O�����X�V����" ) );
	m_ofs_error << CT2CA( uh::str_util::join( fieldList, _T("\t") ) ) << std::endl;

	if( ! m_ofs_run )
		return;
	CVersion cver;
	OutputLog(err_level::info, err_code::StrProductName, cver.GetFileDescription());
	OutputLog(err_level::info, err_code::StrProductVer, cver.GetProductVersion());
}

void COutPut::SetModifyInfo( const long id, const CString& modOperator, const CString& modDate, const CString& modProgram, const CString& modProgramDate )
{
	m_id = id;
	m_modOperator = modOperator;
	m_modDate = modDate;
	m_modProgram = modProgram;
	m_modProgramDate = modProgramDate;
}

void COutPut::SetMeshCode( const CString meshcode )
{
	m_meshcode = meshcode;
}

void COutPut::OutputLog( const CString& layer, long id, err_level::ECode errLevel, long errCode,
	const CString& meshcode/* = _T("")*/, const CString& modOperator/* = _T("")*/, const CString& modDate/* = _T("")*/,
	const CString& modProgram/* = _T("")*/, const CString& modProgramDate/* = _T("")*/,
	const CString& info1/* = _T("")*/, const CString& info2/* = _T("")*/, double x/* = 0.0*/, double y/* = 0.0*/, bool isGeneral/* = true*/ )
{
	if( ! m_ofs_error )
		return;

	CString errMsg = isGeneral?GetErrMsg( errCode ):GetDownConvertErrMsg( errCode );
	CString str;
	if(x == 0.0 && y == 0.0){
		// X���W�AY���W���Ƃ���0�̏ꍇ�A���W�t�B�[���h�͋�ɂ���
		str.Format( _T("0\t%s\t%ld\t%s\t\t\t%s\t%ld\t%s\t%s\t%s\t%s\t%s\t%s\t%s"),
			layer, id, meshcode, GetErrLevelStr( errLevel ), errCode, errMsg,
			info1, info2, modOperator, modDate, modProgram, modProgramDate );
	}else{
		str.Format( _T("0\t%s\t%ld\t%s\t%lf\t%lf\t%s\t%ld\t%s\t%s\t%s\t%s\t%s\t%s\t%s"), 
			layer, id, meshcode, x, y, GetErrLevelStr( errLevel ), errCode, errMsg, 
			info1, info2, modOperator, modDate, modProgram, modProgramDate );
	}
	m_ofs_error << CT2CA( str ) << std::endl;
}


void COutPut::OutputRegLog( err_level::ECode errLevel, long errCode,
	const CString& info1/* = _T("")*/, const CString& info2/* = _T("")*/, double x/* = 0.0*/, double y/* = 0.0*/, bool isGeneral/* = true*/ )
{
	if ( !m_ofs_error )
		return;

	CString str, info;
	if( errCode == err_code::ExistFlag ) info = info2;
	else if( errCode == err_code::PositionNearError) info = info1;

	CString message = isGeneral? GetErrMsg(errCode) : GetDownConvertErrMsg(errCode, info);

	if ( x == 0.0 && y == 0.0 ) {
		// X���W�AY���W���Ƃ���0�̏ꍇ�A���W�t�B�[���h�͋�ɂ���
		str.Format( _T( "0\t%s\t%ld\t%s\t\t\t%s\t%ld\t%s\t%s\t%s\t%s\t%s\t%s\t%s" ),
			m_layer, m_id, m_meshcode, GetErrLevelStr( errLevel ), errCode, message,
			info1, info2, m_modOperator, m_modDate, m_modProgram, m_modProgramDate );
	}
	else {
		str.Format( _T( "0\t%s\t%ld\t%s\t%lf\t%lf\t%s\t%ld\t%s\t%s\t%s\t%s\t%s\t%s\t%s" ),
			m_layer, m_id, m_meshcode, x, y, GetErrLevelStr( errLevel ), errCode, message,
			info1, info2, m_modOperator, m_modDate, m_modProgram, m_modProgramDate );
	}
	m_ofs_error << CT2CA( str ) << std::endl;
}

void COutPut::OutputLog( const CString& layer, const CString& meshcode, double x, double y,
	err_level::ECode errLevel, long errCode )
{
	if( ! m_ofs_error )
		return;
	CString str;
	// OBJECTID���Ȃ����߁A��ɂ���
	str.Format( _T("0\t%s\t\t%s\t%0.13lf\t%0.13lf\t%s\t%ld\t%s\t%s\t%s\t%s\t%s\t%s\t%s"),
			layer, meshcode, x, y, GetErrLevelStr( errLevel ), errCode, GetErrMsg( errCode ),
			_T(""), _T(""), _T(""), _T(""), _T(""), _T("") );
	m_ofs_error << CT2CA( str ) << std::endl;
}

void COutPut::OutputLog( err_level::ECode errLevel, long errCode, const CString& msg )
{
	if( ! m_ofs_run )
		return;
	CString str;
	str.Format( _T("# %s\t%s\t%s"), GetErrLevelStr( errLevel ), GetErrMsg( errCode ), msg );
	m_ofs_run << CT2CA( str ) << std::endl;
}


void COutPut::OutputStdErr( err_level::ECode errLevel, long errCode, const CString& msg )
{
	std::cerr << "#" << CT2CA( GetErrLevelStr( errLevel ) ) << "\t" 
		<< CT2CA( GetErrMsg( errCode ) )<< "\t" << CT2CA( msg ) << std::endl;
}


CString COutPut::GetErrLevelStr( err_level::ECode errLevel )
{
	switch (errLevel){
	case err_level::fatal:	return _T("FATAL");
	case err_level::error:	return _T("ERROR");
	case err_level::warning: return _T("WARNING");
	case err_level::info:	return _T("INFO");
	case err_level::notice: return _T("NOTICE");
	default:	return _T("");
	}
}

CString COutPut::GetDownConvertErrMsg( long errCode, const CString& info )
{
	switch( errCode ){
		using namespace err_code;
		case NotConnectNW:		return _T("���HNW�ɂȂ����Ă��Ȃ�");
		case NgWalkLinkClass1:	return _T("���s�҃����N��ʃR�[�h����`�O");
		case NotConRoadWalkLink:		return _T("�ڑ������N�E���W�J���R�l�N�V���������HNW�ɐڑ����Ă��Ȃ�");
		case MatchRoadLink:		return _T("���W�J���R�l�N�V�����ł͂Ȃ����s�҃����N�����H�����N�ɏd�Ȃ��Ă���");
		case NgHNWalkClass:		return _T("���s�҃����N��ʂ�HEIGHT_NODE�����ΏۊO");
		case ExistFlag: 
		case PositionNearError: return GetErrEnumMsg(errCode, info);

		default:	return _T("");
	}
}

CString COutPut::GetErrMsg( long errCode )
{
	switch( errCode ){
		using namespace err_code;

		// 2**:ERROR
		case FailedGeometryCulc: return _T("�`�F�b�N�ߒ��Ō`��v�Z�Ɏ��s");

		// 9**:FATAL
		case OpenLogFail:		return _T("Open LogFile Fail");
		case OpenMeshFileFail:	return _T("Open meshlist Fail");
		case OpenWsFail:		return _T("Open Workspace Fail");
		case OpenFcFail:		return _T("Open Feature Class Fail");
		case OpenTblFail:		return _T("Open Table Fail");
		case GetNineMeshFail:	return _T("�א�9���b�V���|���S���擾���s");
		case GetRoadFail:		return _T("���H�n�I�u�W�F�N�g�擾���s");
		case GetWalkFail:		return _T("���s�Ҍn�I�u�W�F�N�g�擾���s");
		case GetStationFail:	return _T("�w�o������|�C���g�擾���s");
		case GetMeshBoundFail:	return _T("���b�V���̘g���擾���s");
		case GetWalkRelFail:	return _T("���s�Ҋ֘A�e�[�u���̎擾���s");
		case GetPoiPointFail:	return _T("POI�|�C���g�擾���s");
		case NumErrInMeshList:  return _T("���b�V�����X�g�ɐ����ȊO������");
		case DigitErrInMeshList:return _T("���b�V���R�[�h�̌���������Ă���");
		case DataTypeError:     return _T("�I�v�V����data_type�Ɉُ�l(1�A2�ȊO�̒l)");
		case DataTypeTargetError:     return _T("�I�v�V����data_type�Ɛݒ�t�@�C����[target]�I�v�V������v���Ȃ�");
		case walkTypeError:     return _T("�f�[�^�x�[�X����WALKTYPE_C�Ǝw��I�v�V����data_type�l�������Ă��Ȃ�");
		case GetScrambleAreaFail:	  return _T("���b�V���̃X�N�����u���G���A�擾���s");
		case GetPedxingFail:	return _T("���b�V���̉��f�|�C���g�擾���s");
		
		// 8**:INFO
		case StrWalkDB:			return _T("Walk DB");
		case StrRoadDB:			return _T("Road DB");
		case StrStationDB:		return _T("Station DB");
		case StrMeshDB:			return _T("Mesh DB");
		case StrLogDir:			return _T("Log Dir");
		case StrMeshList:		return _T("Mesh List");
		case StrDistThreshold:	return _T("Dist Threshold");
		case StrAngleThreshold:	return _T("Angle Threshold");
		case StrNeiThreshold:	return _T("Neighbor Threshold");
		case StrRefThreshold:	return _T("ReferID Threshold");
		case StrStartTime:		return _T("Start Time");
		case StrEndTime:		return _T("End Time");
		case StrStart:			return _T("Start");
		case StrEnd:			return _T("End");
		case StrException:		return _T("Exception");
		case StrCRSThreshold:	return _T("Cross Threshold");
		case StrBuddyDistThreshold:		return _T("Buddy Dist Threshold");
		case StrBuddyAngleThreshold:	return _T("Buddy Angle Threshold");
		case StrCheckType:		return _T("Check Type");
		case StrChecking:		return _T("Checking");
		case StrProductName:	return _T("Product Name");
		case StrProductVer:		return _T("Product Version");
		case StrHeightNodeINI:	return _T("HeightNode INI");
		case MaxLinkDistance:	return _T("Max LinkRelation Distance");
		case StrPoiDB:			return _T("POI DB");
		case StrUnderDB:		return _T("UnderGroundArea DB");
		case StrDayFilter:      return _T("day");
		case StrTimeFilter:     return _T("time");

		// 1**:Walk Node ERROR
		case NgWalkNodeClass:	return _T("���s�҃m�[�h��ʃR�[�h����`�O");
		case NgWalkNodeGeom:	return _T("���s�҃m�[�h�`�󂪕s��");
		case BorderCornerNode:	return _T("�}�s�̋��Ƀm�[�h������");
		case NgBorderNodeAttr:	return _T("2�����b�V�����E����ɂ���̂ɐ}�s��m�[�h�ɂȂ��Ă��Ȃ�");
		case NgBorderNodeGeom:	return _T("�}�s��m�[�h��2�����b�V�����E����ɖ���");
		case NoConnect:			return _T("���s�҃����N�ɐڑ����Ă��Ȃ�");
		case NgOnRoadNode1:		return _T("���H�m�[�h��ɂ���̂ɓ��HNW�ڑ��m�[�h�ɂȂ��Ă��Ȃ�");
		case NgOnRoadNode2:		return _T("���HNW�ڑ��m�[�h�����H�m�[�h��ɖ���");
		case NgTerminalNode1:	return _T("�[�_�Ȃ̂ɒ[�_�m�[�h�ɂȂ��Ă��Ȃ�");
		case NgTerminalNode2:	return _T("�ڑ������N�������Ȃ̂ɁA�[�_�m�[�h");
		case TouchNgLink:		return _T("�m�[�h���֌W�Ȃ������N�ɐڐG���Ă���");
		case TouchNgNode:		return _T("�m�[�h���ʂ̃m�[�h�ɐڐG���Ă���");
		case BorderRoadNodeID:	return _T("�}�s��m�[�h�ɓ��H�m�[�h�Ή�ID���t�^����Ă���");
		case NgTerminalLink:	return _T("�[�_�m�[�h�ɐڑ����Ă͂����Ȃ����s�҃����N���");
		case NearOtherName:		return _T("�ߖT�ɕʂ̌����_���̂����m�[�h�����݂���");
		case NotFindRoadNode:	return _T("���H�m�[�h�Ή�ID�ɑΉ����铹�H�m�[�h���Ȃ�");
		case NotFindStation:	return _T("�w�o�����|�C���gID�ɑΉ�����w�o����POI���Ȃ�");
		case NgStationNode:		return _T("�w�o�����m�[�h�Ȃ̂ɉw�o�����|�C���gID���t�^����Ă��Ȃ�");
		case NoNameRoadNode:	return _T("���H�m�[�h�Ή�ID�ɑΉ����铹�H�m�[�h�Ɍ����_���̂��Ȃ�");
		case TooFarRoadNode:	return _T("���H�m�[�h�Ή�ID�ɑΉ����铹�H�m�[�h���߂��ɂȂ�");
		case TooFarStation:		return _T("�w�o�����|�C���gID�ɑΉ�����w�o����POI���߂��ɂȂ�");
		case NotConnectNW:		return _T("���HNW�ڑ��m�[�h�����݂��Ȃ�");
		case NotConnectCross:	return _T("���H�m�[�h�Ή�ID�����̂ɉ��f���������N�ɐڑ����Ă��Ȃ�");
		case NgOnRoadLink:		return _T("���s�҃m�[�h�����H�����N�ɏ���Ă���");
		case ConnectCarOnly:	return _T("�ڑ����铹�H�����N�����s�֎~�̃����N�̂�");
		case NgStationGateID:	return _T("�w�o�����|�C���gID���t�^����Ă���̂ɉw�o�����m�[�h�ł͂Ȃ�");
		case NgUnderGateNode:	return _T("�n���X�o�����m�[�h�Ȃ̂ɒn���X�o�����|�C���gID���t�^����Ă��Ȃ�");
		case NotFindUnderGate:	return _T("�n���X�o�����|�C���gID�ɑΉ�����n���X�o����POI���Ȃ�");
		case TooFarUnderGate:	return _T("�n���X�o�����|�C���gID�ɑΉ�����n���X�o����POI���߂��ɂȂ�");
		case NgUnderGateID:		return _T("�n���X�o�����|�C���gID���t�^����Ă���̂ɒn���X�o�����m�[�h�ł͂Ȃ�");
		case NgTicketGate1:		return _T( "���D�m�[�h�Ȃ̂ɒ[�_�ɂȂ��Ă���" );
		case NgTicketGate2:		return _T( "���D�m�[�h�ɉ��D�������N���ڑ����Ă��Ȃ�" );
		case NgTicketGate3:		return _T( "���D�m�[�h���瓹�H�ڑ��m�[�h�܂Őڑ�����Ă��Ȃ�" );
		case NgOnRoadLinkException:		return _T("���H�����N��ɂ���̂ɓ��H�����N�ڑ��m�[�h�ɂȂ��Ă��Ȃ�");
		case NgNotOnRoadLink:		    return _T("���H�����N�ڑ��m�[�h�����H�����N��ɖ���");
		case NgOnRoadLinkCross:	return _T("���H�����N�ڑ��m�[�h�����H�����N�����ӏ��ɂ���");
		case NgOnWalkLinkCross:	return _T("�X�N�����u�������_�Ȃ̂ɃX�N�����u���G���A�|���S��������");
		case ConnectWalkLink:	return _T("���W�J���R�l�N�V�����������ڑ����Ă���");
		case NgConnectWalkLinkAngle:	return _T("���W�J���R�l�N�V�������s�p�ɐڑ����Ă���");

		// 3**:Walk Link ERROR
		case NgWalkLinkClass1:	return _T("���s�Ҏ�ʂ���`�O");
		case NgWalkLinkClass2:	return _T("���s�Ҏ�ʃR�[�h�ɐ����ΏۊO�̒l���t�^����Ă���");
		case NgFloorMoveClass:	return _T("�K�w�ړ���ʃR�[�h����`�O");
		case NgOnewayCode1:		return _T("����ʍs��ʃR�[�h����`�O");
		case NgNoPassCode:		return _T("���s�Ғʍs�֎~�R�[�h����`�O");
		case NgWalkCodeC:		return _T("���s�ҘH���R�[�h����`�O");
		case NgFlagField:		return _T("�t���O�t�B�[���h�Ȃ̂ɁA[0]��[1]�ȊO�̒l");
		case NgFromID:			return _T("�n�_�m�[�hID���s��");
		case NgToID:			return _T("�I�_�m�[�hID���s��");
		case NgWalkLinkGeom:	return _T("���s�҃����N�`�󂪕s��");
		case CrossMesh:			return _T("2�����b�V�������f���Ă���");
		case NoBorderNode:		return _T("���b�V���ׂ��̃����N�ɐ}�s�m�[�h�����݂��Ȃ�");
		case NotFoundFromNode:	return _T("FromNodeID�̃m�[�h��������Ȃ�");
		case NotFoundToNode:	return _T("ToNodeID�̃m�[�h��������Ȃ�");
		case NotSameBorderAttr:	return _T("���b�V�����E�̗אڂő������قȂ�");
		case NgBorderLink:		return _T("���b�V�����E�ŗאڃ����N���s��");
		case NgBorderOneway:	return _T("����ʍs��2�����b�V�����E�œr�؂�Ă���");
		case UnMatchStartFrom:	return _T("�����N�̎n�_��From�m�[�h�̍��W����v���Ȃ�");
		case UnMatchEndTo:		return _T("�����N�̏I�_��To�m�[�h�̍��W����v���Ȃ�");
		case UnderThresholdSeg: return _T("�����Z�O�����g�����݂���");
		case UnderThresholdAg:	return _T("�}���ȃ^�[�������݂���");
		case DupPoint:			return _T("�\���_���d�����Ă���");
		case LoopLinkSingleFT:	return _T("�����N�̎n�I�_��������");
		case LoopLinkDupFT:		return _T("����From/To���������N����������");
		case NgFloorMoveAndF:	return _T("�K�w�ړ���ʂ��t�^����Ă��Ȃ�");
		case ExistFloorMove:	return _T("�K�w�ړ���ʂ��t�^����Ă���");
		case NotFoundWalkCode:	return _T("�Ή�������s�ҘH���R�[�h�f�[�^���Ȃ�");
		case DiffWalkCode:		return _T("���s�ҘH���R�[�h���O��̃����N�ňقȂ�");
		case NotOnewayCode:		return _T("����ʍs��ʂ��t�^����Ă��Ȃ�");
		case UnMatchRoadLink:	return _T("�l�ԋ��p�����N�����H�����N�Ɋ܂܂�Ă��Ȃ�");
		case NotConOpenAreaF:	return _T("�L�ꂪ���Ă��Ȃ�");
		case NotConRoadWalkLink:	return _T("�ڑ������N�����H�����N�A�܂��͐l�ԋ��p�����N�ɐڑ����Ă��Ȃ�");
		case ExistFlag:			return _T("�t���O�����ΏۊO�̕��s�Ҏ�ʂɑ΂��t���O����������Ă���");
		case NgRoadSurfaceClass:	return _T("�H�ʑf�ރR�[�h����`�O");
		case ThreeMoreCross:	return _T("2�ȏ�̃����N�Ƃ̌����ӏ����߂�");
		case NotConRoadNwNode:	return _T("�l�ԋ��p�����N�Q�����HNW�ڑ��m�[�h�ɐڑ����Ă��Ȃ�");
		case NgHumanCarAndF:	return _T("�l�ԋ��p�����N�ɋK������������Ă���");
		case NgNotNoPassInfo:	return _T("�ʍs�֎~���ԋK���֘A�t���O��ON�����A�ʍs�֎~�K����񂪑��݂��Ȃ�");
		case NgBorderNoPassage: return _T("�ʍs�֎~��2�����b�V�����E�ŕς���Ă���");
		case NgLinkRelationF:	return _T("�����N�R�Â��e�[�u���t���O��ON�����A���s�ҁE���H�����N�R�t���e�[�u����񂪂Ȃ�");
		case NgHCWalkCode:		return _T("�l�ԋ��p�����N�ɕ��s�ҘH���R�[�h����������Ă���");
		case NgNoPassageRF:		return _T("�ʍs�֎~��ʃR�[�h�Ɗ֘A�t���O�̐��������Ƃ�Ă��Ȃ�");
		case NgOnewayRF:		return _T("�����ʃR�[�h�Ɗ֘A�t���O�̐��������Ƃ�Ă��Ȃ�");
		case NgNotOnewayInfo:	return _T("����ʍs���ԋK���֘A�t���O��ON�����A����ʍs�K����񂪑��݂��Ȃ�");
		case BothRoadNW:		return _T("From�m�[�h,To�m�[�h���ɓ��HNW�ڑ��m�[�h");
		case NgRoadLinkWalkable:	return _T("�l�ԋ��p�����N�ƈ�v���铹�H�����N�����s�֎~�ȊO");
		case WithInRoadNode:	return _T("���s�҃����N�ɓ��H�m�[�h���܂܂�Ă���");
		case DiffWalkClass:		return _T("���s�Ҏ�ʂ��O��̃����N�ƈقȂ�");
		case DiffFloorLevel:	return _T("�K�w���x�����O��̃����N�ňقȂ�");
		case DiffFloorMoveCode:	return _T("���s�����N�̊K�w�ړ���ʂ̕������قȂ�");
		case SameEscOnway:		return _T("���s�����N�̃G�X�J���[�^�̈���ʍs����������");
		case NgMovingWalkArcade:	return _T("�G�X�J���[�^/�I�[�g�E�H�[�N�t���O��ON�����A�����t���t���O��OFF");
		case MatchRoadLink:		return _T("�l�ԋ��p�����N�ł͂Ȃ��̂ɓ��H�����N�Ƀ}�b�`���Ă���");
		case NgUnderConnect:    return _T("�G���x�[�^�[���~���K�w���Ȃ��̂ɒn���Ɍq�����Ă���");
		case PointOnOtherLink:  return _T("�\���_���[�_�����̃����N�ɏ���Ă���");
		case NgCrossRoadNode:  return _T( "���f�����̕Б��ɂ������H�m�[�hID���t�^����Ă��Ȃ�" );
		case NgInternalTicketGate:return _T( "���D�������N�Ɖ��D�O�����N�Ԃ̃m�[�h�����D�m�[�h�ɂȂ��Ă��Ȃ�" );
		case NgBorthCrossRoadNode:return _T("���f�����̗��[�m�[�h������ł��Ȃ�");
		case NgWalkLinkNotOnScrambleArea: return _T("�X�N�����u���t���O���t�^����Ă���̂ɃX�N�����u���G���A���ɖ���");
		case NgWalkLinkOnScrambleArea:    return _T("�X�N�����u���G���A���Ȃ̂ɃX�N�����u���t���O���t�^����Ă��Ȃ�");
		case NgWalkLinkWheelChair:        return _T("�Ԉ֎q�ʍs�R�[�h���s���Ȓl");
		case NgWalkLinkPlatformNumber:    return _T("�̂�Δԍ����s���Ȓl");
		case MicroLogicalConnection:      return _T("�������W�J���R�l�N�V����");
		case NgCrossWalkLink:   return _T("���W�J���R�l�N�V�����ƌ������Ă���");

		// 4**:Walk Link WARN
		case ExistOnewayCode:	return _T("����ʍs��ʂ��t�^����Ă���");
		case CrossMultiLane:	return _T("�݂Ȃ����f�������������Ă��铹�H�����N��2�Ԑ��ȏ�");

		// 5**:Road Link ERROR
		case OutWalkable:        return _T("���s�\��ʃR�[�h����`�O");
		case NgWalkable:         return _T("���H�����N�Ȃ̂ɕ��s�\��ʃR�[�h�����s�Ґ�p�ɂȂ��Ă���");
		case NotWalkSandwiched:   return _T("�l�ԋ��p�̒��ŕ��s�֎~�������Ă���");
		case NgCarOnly:          return _T("�����Ԑ�p���Ȃ̂ɕ��s�\��ʃR�[�h�������Ȃ��i���s�֎~�j�ɂȂ��Ă��Ȃ�");
		case NgPedestrianZone:   return _T("���s�ғV���t���O����`�O");
		case NgPedestrianAndF:   return _T("���s�ғV���t���O��ON�����A�ʍs�֎~�K����񂪑��݂��Ȃ�");
		case NgPedZoneCarOnly:   return _T("���s�ғV���t���O��ON�������H�����N��ʂ������Ԑ�p��");
		case HumanCarSandwiched: return _T("���s�֎~�̒��Ől�ԋ��p�������Ă���");
		case NgConnectedSideWalk:return _T("�������Ȃ����Ă��Ȃ�");
		case NgWalkableClass:    return _T("���s�\��ʃR�[�h���s���Ȓl");
		case NgOverMeshWalkableClass:    return _T("���b�V�����E�ŕ��s�\��ʂ��ς���Ă���");

		// 6**:Walk Code List ERROR
		case NgWalkCode:		return _T("���s�ҘH���R�[�h���s���Ȓl");
		case NgWalkCodeKanji1:	return _T("���s�ҘH���������s��");
		case NgWalkCodeKanji2:	return _T("���s�ҘH���������f�t�H���g�l");
		case NotExistWalkCodeWL:	return _T("���s�ҘH���R�[�h�����s�҃����N��1�������݂��Ȃ�");
		case NgWalkCodeYomi:	return _T("���s�ҘH���ǂ݂��s��");

		// 7**:Walk No Passage ERROR
		case NotExistNoPassWL:	return _T("�ʍs�֎~�K�������e�[�u���Ŏw�肵�����s�҃����N�����݂��Ȃ�");
		case OverLapNoPassTime:	return _T("�ʍs�֎~�̋K�����Ԃ��d�Ȃ��Ă���");
		case NgNoPassageTime:	return _T("�ʍs�֎~�̋K���������s���Ȓl");
		case SameNoPassageTime:	return _T("�ʍs�֎~�K���������������̂��������݂��Ă���");
		case NgNoPassFullYear:	return _T("�ʍs�֎~�K�������̒ʔN��1/1�`12/31�ɂȂ��Ă��Ȃ�");
		case NgNoPassLinkID:	return _T("�����NID���s���Ȓl");

		// 10**:Height Node ERROR
		case NoExistLink:		return _T("�����NID�Ɏw�肵�������N�����݂��Ȃ�");
		case NgHeight:			return _T("���΍������͈͊O");
		case NgLinkTable1:		return _T("�����N�e�[�u������`�O");
		case NgLinkTable2:		return _T("�����N�e�[�u���������ΏۊO");
		case NgBothRoadLink:	return _T("���H�����N���m�̑��΍���");
		case NgSelhCross:		return _T("���Ȍ����ӏ��ɕt�^����Ă���");
		case NgHeightEqual:		return _T("���΍���������");
		case NgHNWalkClass:		return _T("���s�Ҏ�ʂ����΍����t�^�ΏۊO");
		case NgHeightWalk:		return _T("���s�Ҏ�ʂɑ΂��鑊�΍������͈͊O");
		case NgHeightRoad:		return _T("�����N�������ɑ΂��鑊�΍������͈͊O");
		case NoExistPointOnLink:	return _T("HEIGHT_NODE�������N��ɂȂ�");
		case NearNoramlNode:	return _T("HEIGHT_NODE�t�߂Ƀm�[�h�����݂���");
		case NearHeightNode:	return _T("HEIGHT_NODE�t�߂ɕʂ�HEIGT_NODE�����݂���");
		case NearSegment:		return _T("HEIGHT_NODE�t�߂Ƀ����NID1�܂��̓����NID2�̍\���_�����݂���");
		case LargeDifference:	return _T("���΍����̍����傫��");
		case NonExistHeightNode: return _T("HEIGHT_NODE������ׂ��ӏ��ɑ��݂��Ȃ�");
		case NgHeightNodeGeometry:  return _T("HEIGHT_NODE�̌`�󂪕s��");
		case NgHeightNodeLinkID:  return _T("�����NID���s���Ȓl");

		// 11**:Walk Oneway ERROR
		case NotExistOnewayWL:	return _T("����ʍs�K�������e�[�u���Ŏw�肵�����s�҃����N�����݂��Ȃ�");
		case OverLapOnewayTime: return _T("����ʍs���m�A�܂��͈���ʍs�ƒʍs�֎~�̋K�����Ԃ��d�Ȃ��Ă���");
		case NgOnewayTime:		return _T("����ʍs�̋K���������s���Ȓl");
		case SameOnewayTime:	return _T("����ʍs�܂��͒ʍs�֎~�őS�������K�����Ԃ��������݂��Ă���");
		case NgOnewayFullYear:	return _T("�K�������̒ʔN��1/1�`12/31�ɂȂ��Ă��Ȃ�");
		case NgOnewayLinkID:	return _T("�����NID���s���Ȓl");
		case NgOnewayLinkDir:	return _T("�����N�����R�[�h���s���Ȓl");

		// 12**:LinkRealation ERROR
		case NotExistWalkLink:	return _T("������NID�����s�҃����N�ɑ��݂��Ȃ�");
		case NotExistRoadLink:	return _T("�֘A�����NID�����H�����N�ɑ��݂��Ȃ�");
		case NgLRWalkClass:		return _T("������N�̕��s�Ҏ�ʂ��ΏۊO");
		case NgLRRoadClass:		return _T("�֘A�����N�̓��H�\����ʂ��ΏۊO");
		case NgLRWalkFlag:		return _T("�R�t�ΏۊO�ɂȂ�t���O��ON�ɂȂ��Ă���");
		case NgLinkDist:		return _T("�����N���m������Ă���");
		case NgLinkAngle:		return _T("�����N���m�̊p�x���傫��");
		case NgConnectRelLinks:	return _T("�֘A�����N�Q���ڑ����Ă��Ȃ�");
		case NgBuddyRelation:	return _T("�����[�X�ϊ��ŕR�t���Ȃ�Buddy�֌W");
		case NgLRRoadLinkWalkable: return _T("Buddy�����̂ɕ��s�֎~�ȊO");
		case NgDuplicatedRecord: return _T("�����N�����[�V�����ɏd�����R�[�h������");

		// 13**:StationGatePoint ERROR
		case NotExistSameNameSGP:	return _T("�t�߂ɓ���̉w�o�������̂����w�o�����m�[�h�����݂��Ȃ�");

		// 14**:PoiPointOrg ERROR
		case NotExistSameNamePOI:	return _T("�t�߂ɓ���̒n���X�o�������̂����n���X�o�����m�[�h�����݂��Ȃ�");
		case NotAlivePOI        :   return _T("�m�[�h�ɕR�t���n���X�o����POI�̍폜�R�[�h���u�����v�ł͂Ȃ�");

		// 15**:RoadNode ERROR
		case NotExistSameNameRND:	return _T("�t�߂ɓ���̌����_���������s�҃m�[�h�����݂��Ȃ�");

		// 16**:WalkLink-UnderGround ERROR
		case PartOfUnderLinkIncludedArea: return _T("�n���ʘH�̈ꕔ���n���X����͂ݏo�Ă���");
		case UndergroundAreaNoneOverLinks: return _T("�n���ʘH�����S�ɒn���X����͂ݏo�Ă���");
		case UndergroundAreaNotExist: return _T("�n���ʘH�����N�Q�ɏd�Ȃ�n���X���Ȃ�");

		// 17**:Pedxing ERROR
		case NgPedxingGeometry:       return _T("���f�|�C���g�̌`�󂪕s��");
		case NgPedxingOnScrambleArea: return _T("���f�|�C���g���X�N�����u���G���A���ɂ���");
		case NgPedxingOnRoadLink:     return _T("���f�|�C���g�����H�����N��ɖ���");
		case NgPedxingCrossRoadLink:  return _T("���f�|�C���g�����H�����N�����ӏ��ɂ���");
		case NgPedxingCrossWalkLink:  return _T("���f�|�C���g�����s�҃����N�����ӏ��ɂ���");
		case NgPedxingWithWalkLink:   return _T("���f�|�C���g�Ɋ֌W�Ȃ����s�҃����N�����݂���");
		case NgPedxingClass:          return _T("���f�^�C�v�R�[�h���s���Ȓl");
		case CrossRoadLink:           return _T("�Ӗ��Ȃ�PEDXING");
		// 18**:ScrambleArea ERROR
		case NgScrambleAreaGeometry:       return _T("�X�N�����u���G���A�|���S���̌`�󂪕s��");
		case NgScrambleAreaCrossWalkLink:  return _T("�X�N�����u���G���A���ɃX�N�����u���t���O���t�^���ꂽ���f������1������");
		case NgScrambleAreaClass:          return _T("�X�N�����u����ʃR�[�h���s���Ȓl");

		default:	return _T("");
	}
}

// �G���[�R�[�h�ƃ��b�Z�[�W����G���[���b�Z�[�W�擾
CString COutPut::GetErrEnumMsg( long errCode, const CString& info)
{
	using namespace err_code;
	using namespace sindy::schema;
	using namespace sindy::schema::walk_link;

	switch( errCode ){

	case ExistFlag:
		if(info == kAutoWalkFlag)
			return _T("�I�[�g�E�H�[�N�t���O�����ΏۊO�̕��s�Ҏ�ʂɑ΂��t���O����������Ă���");
		else if(info == kStairFlag)
			return _T("�K�i�t���O�����ΏۊO�̕��s�Ҏ�ʂɑ΂��t���O����������Ă���");
		else if(info == kSlopeFlag)
			return _T("�X���[�v�t���O�����ΏۊO�̕��s�Ҏ�ʂɑ΂��t���O����������Ă���");
		else if(info == kEscalatorFlag)
			return _T("�G�X�J���[�^�t���O�����ΏۊO�̕��s�Ҏ�ʂɑ΂��t���O����������Ă���");
		else if(info == kArcadeFlag)
			return _T("�������t���O�����ΏۊO�̕��s�Ҏ�ʂɑ΂��t���O����������Ă���");
		else if(info == kOpenAreaFlag)
			return _T("�L��t���O�����ΏۊO�̕��s�Ҏ�ʂɑ΂��t���O����������Ă���");
		else if(info == kTunnelFlag)
			return _T("�g���l���t���O�����ΏۊO�̕��s�Ҏ�ʂɑ΂��t���O����������Ă���");
		else if(info == kScrambleFlag)
			return _T("�X�N�����u���t���O�����ΏۊO�̕��s�Ҏ�ʂɑ΂��t���O����������Ă���");
		else
			return _T("�t���O�����ΏۊO�̕��s�Ҏ�ʂɑ΂��t���O����������Ă���");
	case PositionNearError:
		if(info.Find(walk_node::kTableName) >= 0)
			return _T("�|�C���g���mWALK_NODE�̋������߂�");
		else if(info.Find(pedxing::kTableName) >= 0)
			return _T("�|�C���g���mPEDXING�̋������߂�");
		else if(info.Find( height_node::kTableName) >= 0)
			return _T("�|�C���g���mHEIGHT_NODE�̋������߂�");
	default:	return _T("");
	}
}
