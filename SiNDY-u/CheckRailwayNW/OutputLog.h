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

#pragma once

#include "stdafx.h"

// �G���[���x���񋓌^
namespace err_level {
	enum ECode {
		fatal,
		error,
		warning,
		info
	};
}; // err_level

// �G���[�R�[�h�񋓌^
namespace err_code {
	enum ECode {

		// 9**:FATAL
		OpenLogFail       = 901,
		OpenMeshFileFail  = 902,
		OpenWsFail        = 903,
		OpenFcFail        = 904,
		OpenTblFail       = 905,
		GetNineMeshFail   = 906,
		GetRailFail       = 907,
		GetStaPoiFail     = 908,
		GetBaseStaFail    = 909,
		GetMeshBoundFail  = 910,
		GetOIDAllFail     = 911,
//		GetRoadCodeList   = 911,

		// 8**:INFO
		StrRailNWDB       = 801,
		StrRailPoiDB      = 802,
		StrStationDB      = 803,
		StrMeshDB         = 804,
		StrLogFile        = 805,
		StrMeshList       = 806,
		StrDistThreshold  = 807,
//		StrAngleThreshold = 808,
		StrNeiThreshold   = 809,
		StrStartTime      = 810,
		StrEndTime        = 811,
		StrStart          = 812,
		StrQueryStart     = 813,

		// 1**:Rail Node ERROR
		NgRailNodeClass   = 101,
		NgRailNodeGeom    = 102,
		BorderCornerNode  = 103,
		NgBorderNodeAttr  = 104,
		NgBorderNodeGeom  = 105,
		NoConnect         = 106,
		NgTerminalNode    = 107,
		NgBranchNode      = 108,
		NgAlterNode       = 109,
		TouchNgLink       = 110,
		TouchNgNode       = 111,
		NoTouchBaseSta    = 112,
		NgRelJoinLine1    = 113,
		NgRelJoinLine2    = 114,
		NgRelJoinLine3    = 115,
		TooFarStaPoint    = 116,


		// 3**:Rail Link ERROR
		NgRailLinkClass   = 301,
		NgDispScaleClass  = 302,
		NgFlagField       = 303,
		NgFromID          = 304,
		NgToID            = 305,
		NgRailLinkGeom    = 306,
		CrossMesh         = 307,
		NoBorderNode      = 308,
		NotFoundFromNode  = 309,
		NotFoundToNode    = 310,
		NotSameBorderAttr = 311,
		NgBorderLink      = 312,
		UnMatchStartFrom  = 313,
		UnMatchEndTo      = 314,
		UnderThresholdSeg = 315,
		DupPoint          = 316,
		LoopLinkSingleFT  = 317,
		LoopLinkDupFT     = 318,
		NgRelRailwayLine1 = 319,
		NgRelRailwayLine2 = 320,

		// 5**:Query ERROR
		NonRelRailLink    = 501,
		NonRelRailNode    = 502,
		NotOnlySeqLine    = 503,
		DupStationNum     = 504,
		NoNumberStaNum    = 505,
		NgSeqStaNum       = 506,
		NonJoinLinkInfo   = 507,
		NonRailwayLine    = 508,



	};
}; // err_code

/**
 * @brief   �o�̓N���X
 * @note    �W���o�� / ���O�o��
 */
class COutPut
{
public:

	/**
	 * @brief   �R���X�g���N�^
	 */
	COutPut(){};

	/**
	 * @brief   �f�X�g���N�^
	 */
	~COutPut(){};

	/**
	 * @brief   �I�[�v��
	 * @param   strFileName [in]  ���O�t�@�C����
	 * @retval  true  �I�[�v������
	 * @retval  false �I�[�v�����s
	 */
	bool Open( const CString& strFileName ){
		m_ofs.open( strFileName );
		if( ! m_ofs )
			return false;
		return true;
	};

	/**
	 * @brief   �N���[�Y
	 */
	void Close(){
		m_ofs.close();
	};

	/**
	 * @brief   �w�b�_�o��
	 */
	void OutputHeader()
	{
		if( ! m_ofs )
			return;
		CString str = _T("#SINDYSTDLOG");
		m_ofs << CT2CA( str ) << std::endl;
	};

	/**
	 * @brief   ���O�o��
	 * @note    SINDYSTDLOG�`��
	 * @param   layer    [in]  ���C����
	 * @param   id       [in]  ObjectID
	 * @param   errLevel [in]  �G���[���x��
	 * @param   errCode  [in]  �G���[�R�[�h
	 * @param   msg      [in]  �t���[���b�Z�[�W
	 */
	//void OutputLog( const CString& layer, long id, err_level::ECode errLevel, long errCode )
	//{
	//	if( ! m_ofs )
	//		return;
	//	CString str;
	//	str.Format( _T("0\t%s\t%ld\t\t\t%s\t%ld\t%s\t\t\t\t"), layer, id, GetErrLevelStr( errLevel ), errCode, GetErrMsg( errCode ) );
	//	m_ofs << CT2CA( str ) << std::endl;
	//};
	//void OutputLog( const CString& layer, long id, err_level::ECode errLevel, long errCode, const CString& info )
	//{
	//	if( ! m_ofs )
	//		return;
	//	CString str;
	//	str.Format( _T("0\t%s\t%ld\t\t\t%s\t%ld\t%s\t%s\t\t\t"), layer, id, GetErrLevelStr( errLevel ), errCode, GetErrMsg( errCode ), info );
	//	m_ofs << CT2CA( str ) << std::endl;
	//};
	//void OutputLog( const CString& layer, long id, err_level::ECode errLevel, long errCode, const CString& info1, const CString& info2 )
	//{
	//	if( ! m_ofs )
	//		return;
	//	CString str;
	//	str.Format( _T("0\t%s\t%ld\t\t\t%s\t%ld\t%s\t%s\t%s\t\t"), layer, id, GetErrLevelStr( errLevel ), errCode, GetErrMsg( errCode ), info1, info2 );
	//	m_ofs << CT2CA( str ) << std::endl;
	//};
	void OutputLog( const CString& layer, long id, err_level::ECode errLevel, long errCode, const CString& modOperator = _T(""), const CString& modDate = _T(""), const CString& info1 = _T(""), const CString& info2 = _T("") )
	{
		if( ! m_ofs )
			return;
		CString str;
		str.Format( _T("0\t%s\t%ld\t\t\t%s\t%ld\t%s\t%s\t%s\t%s\t%s"), layer, id, GetErrLevelStr( errLevel ), errCode, GetErrMsg( errCode ), info1, info2, modOperator, modDate );
		m_ofs << CT2CA( str ) << std::endl;
	};

	/**
	 * @brief   ���O�o��
	 * @note    �R�����g�`��
	 * @param   errLevel [in]  �G���[���x��
	 * @param   errCode  [in]  �G���[�R�[�h
	 * @param   msg      [in]  �t���[���b�Z�[�W
	 */
	void OutputLog( err_level::ECode errLevel, long errCode, const CString& msg )
	{
		if( ! m_ofs )
			return;
		CString str;
		str.Format( _T("# %s\t%s\t%s"), GetErrLevelStr( errLevel ), GetErrMsg( errCode ), msg );
		m_ofs << CT2CA( str ) << std::endl;
	};

	/**
	 * @brief   �W���o��
	 * @param   errLevel [in]  �G���[���x��
	 * @param   errCode  [in]  �G���[�R�[�h
	 * @param   msg      [in]  �t���[���b�Z�[�W
	 */
	void OutputStdErr( err_level::ECode errLevel, long errCode, const CString& msg )
	{
		std::cerr << "#" << CT2CA( GetErrLevelStr( errLevel ) ) << "\t" 
			<< CT2CA( GetErrMsg( errCode ) )<< "\t" << CT2CA( msg ) << std::endl;
	};

private:

	/** 
	 * @brief   �G���[���x���̕�����擾
	 * @param   errLevel [in]  �G���[���x��
	 * @retval  CString  �G���[���x��������
	 */
	CString GetErrLevelStr( err_level::ECode errLevel )
	{
		switch (errLevel){
			using namespace err_level;
			case fatal:   return _T("FATAL");
			case error:   return _T("ERROR");
			case warning: return _T("WARNING");
			case info:    return _T("INFO");
			default:      return _T("");
		}
	};

	/**
	 * @brief   �G���[�R�[�h����G���[���b�Z�[�W�擾
	 * @param   errCode [in]  �G���[�R�[�h
	 * @retval  CString �G���[���b�Z�[�W
	 */
	CString GetErrMsg( long errCode )
	{
		switch( errCode ){
			using namespace err_code;

			// 9**:FATAL
			case OpenLogFail:      return _T("Open LogFile Fail");
			case OpenMeshFileFail: return _T("Open meshlist Fail");
			case OpenWsFail:       return _T("Open Workspace Fail");
			case OpenFcFail:       return _T("Open Feature Class Fail");
			case OpenTblFail:      return _T("Open Table Fail");
			case GetNineMeshFail:  return _T("�א�9���b�V���|���S���擾���s");
			case GetRailFail:      return _T("�S��NW�n�I�u�W�F�N�g�擾���s");
			case GetStaPoiFail:    return _T("�w�|�C���g�擾���s");
			case GetBaseStaFail:   return _T("���k�w�Ƀ|���S���擾���s");
			case GetMeshBoundFail: return _T("���b�V���̘g���擾���s");
			case GetOIDAllFail:    return _T("�S������ObjectID�擾���s");
			//case GetRoadCodeList:  return _T("RoadCodeList�̎擾���s");

			// 8**:INFO
			case StrRailNWDB:       return _T("Rail NW DB");
			case StrRailPoiDB:      return _T("Rail POI DB");
			case StrStationDB:      return _T("Station DB");
			case StrMeshDB:         return _T("Mesh DB");
			case StrLogFile:        return _T("Log File");
			case StrMeshList:       return _T("Mesh List");
			case StrDistThreshold:  return _T("Dist Threshold");
			case StrNeiThreshold:   return _T("Neighbor Threshold");
			case StrStartTime:      return _T("Start Time");
			case StrEndTime:        return _T("End Time");
			case StrStart:          return _T("Start");
			case StrQueryStart:     return _T("QueryChk Start");

				// 1**:Rail Node ERROR
			case NgRailNodeClass:   return _T("�m�[�h��ʃR�[�h����`�O");
			case NgRailNodeGeom:    return _T("�`�󂪕s��");
			case BorderCornerNode:  return _T("�}�s�̋��Ƀm�[�h������");
			case NgBorderNodeAttr:  return _T("2�����b�V�����E����ɂ���̂ɐ}�s��m�[�h�ɂȂ��Ă��Ȃ�");
			case NgBorderNodeGeom:  return _T("�}�s��m�[�h��2�����b�V�����E����ɖ���");
			case NoConnect:         return _T("�S�������N�ɐڑ����Ă��Ȃ�");
			case NgTerminalNode:    return _T("�[�_�Ȃ̂ɒ[�_�m�[�h�ɂȂ��Ă��Ȃ�");
			case NgBranchNode:      return _T("����Ȃ̂ɕ���m�[�h�ɂȂ��Ă��Ȃ�");
			case NgAlterNode:       return _T("�ڑ����郊���N�̑������قȂ�̂ɑ����ω��_�m�[�h�ɂȂ��Ă��Ȃ�");
			case TouchNgLink:       return _T("�m�[�h���֌W�Ȃ������N�ɐڐG���Ă���");
			case TouchNgNode:       return _T("�m�[�h���ʂ̃m�[�h�ɐڐG���Ă���");
			case NoTouchBaseSta:    return _T("���k�w�Ƀ|���S����ɑ��݂��Ȃ��wPOI�m�[�h�����݂���");
			case NgRelJoinLine1:    return _T("�wPOI�m�[�h�Ȃ̂ɘH�����X�g�̒l��null");
			case NgRelJoinLine2:    return _T("�wPOI�m�[�h�ȊO�̃m�[�h�ŘH�����X�g�̒l��null�ȊO");
			case NgRelJoinLine3:    return _T("�ڑ����Ă��郊���N�ɑ��݂��Ȃ��H��ID���m�[�h�ɑ��݂��Ă���");
			case TooFarStaPoint:    return _T("�wPOI�m�[�h�̋ߖT�ɑΉ�����wPOI�����݂��Ȃ�");


			// 3**:Rail Link ERROR
			case NgRailLinkClass:   return _T("�S����ʃR�[�h����`�O");
			case NgDispScaleClass:  return _T("�X�P�[���\���R�[�h����`�O");
			case NgFlagField:       return _T("�t���O�t�B�[���h�Ȃ̂ɁA[0]��[1]�ȊO�̒l");
			case NgFromID:          return _T("�n�_�m�[�hID���s��");
			case NgToID:            return _T("�I�_�m�[�hID���s��");
			case NgRailLinkGeom:    return _T("�S�������N�`�󂪕s��");
			case CrossMesh:         return _T("2�����b�V�������f���Ă���");
			case NoBorderNode:      return _T("���b�V���ׂ��̃����N�ɐ}�s�m�[�h�����݂��Ȃ�");
			case NotFoundFromNode:  return _T("FromNodeID�̃m�[�h��������Ȃ�");
			case NotFoundToNode:    return _T("ToNodeID�̃m�[�h��������Ȃ�");
			case NotSameBorderAttr: return _T("2�����b�V�����E�̗אڂŃ����N�̑������قȂ�");
			case NgBorderLink:      return _T("���b�V�����E�ŗאڃ����N���s��");
			case UnMatchStartFrom:  return _T("�����N�̎n�_��From�m�[�h�̍��W����v���Ȃ�");
			case UnMatchEndTo:      return _T("�����N�̏I�_��To�m�[�h�̍��W����v���Ȃ�");
			case UnderThresholdSeg: return _T("�����Z�O�����g�����݂���");
			case DupPoint:          return _T("�\���_���d�����Ă���");
			case LoopLinkSingleFT:  return _T("�����N�̎n�I�_��������");
			case LoopLinkDupFT:     return _T("From/To�̑g�ݍ��킹�����������N���������݂���");
			case NgRelRailwayLine1: return _T("�S����ʃR�[�h��0(�S��POI��ʂŕ���)�ŘH�����X�g�̒l��null");
			case NgRelRailwayLine2: return _T("�S����ʃR�[�h��1or2(�ݕ����E�S��������)�ŘH�����X�g�̒l��null�ȊO");

			// 5**:Query ERROR
			case NonRelRailLink:    return _T("�R�Â��S�������N�����݂��Ȃ�");
			case NonRelRailNode:    return _T("�R�Â��S���m�[�h�����݂��Ȃ�");
			case NotOnlySeqLine:    return _T("�����H�����̂ŘA�����Ă��郊���N�Q���������݂��Ă���");
			case DupStationNum:     return _T("�w�ԍ����d�����Ă���");
			case NoNumberStaNum:    return _T("�w�ԍ��ɐ��������݂��Ȃ�");
			case NgSeqStaNum:       return _T("�w�ԍ��̕��я����s��");
			case NonJoinLinkInfo:   return _T("�R�t��������H�������݂��Ȃ�");
			case NonRailwayLine:    return _T("�R�t���S���H�������݂��Ȃ�");

			default:                return _T("");
		}
	};

private:

	std::ofstream m_ofs; //!< ���O�o�̓X�g���[��
};
