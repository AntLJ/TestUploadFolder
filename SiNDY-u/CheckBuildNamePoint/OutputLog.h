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
#include <atlconv.h>
#include "lonlat.h"

// �G���[���x���񋓌^
namespace err_level {
	enum ECode {
		fatal,
		error,
		warning,
		info,
		ok,
		none,
	};
}; // err_level

// �G���[�R�[�h�񋓌^
namespace err_def {
	enum ECode {

		// OK
		Ok,
		oOKWordByBldcOKWord,
		oOkWordByMultiBldcOKWord,

		// FATAL
		OpenNgBlCFail,
		OpenNgWordFail,
		OpenBldcOKWordFail,
		OpenBldcNGWordFail,
		OpenReplaceFail,
		OpenJoinAnnoFail,
		OpenMeshFail,
		OpenExcludMarkFail,
		OpenLogFail,
		GetMeshFail,
		GetObjFail,

		// INFO
		StrBlNameDB,
		StrBuildingDB,
		StrAnnoDB,
		StrMeshDB,
		StrLogFile,
		StrMeshList,
		StrNgBuild,
		StrReplace,
		StrErrWord,
		StrWarnWord,
		StrBldcOKWord,
		StrBldcNGWord,
		StrChkFixclass,
		StrJoinAnno,
		StrExcludeMark,
		StrBufferRange,
		StrStartTime,
		StrEndTime,

		StrStart,
		StrEnd,
		StrQueryStart,
		StrQueryEnd,

		// Error/Warning
		eUnMatchBuildID,
		eDisjointBuild,
		eHoleUnMatchID,
		wHoleMatchID,
		eNgBlClass,
		eDupPointInOne,
		wSameNameInBuf,
		wLikeNameInBuf,
		eNgSpace,
		eNgParentheses,
		eLeftParentOnly,
		eRightParentOnly,
		ePipeExist,
		eTopZenHyphen,
		wNgHyphenWord,
		eTopHiraLowY,
		eNgHiraLowY,
		eTopKataLowY,
		eNgKataLowY,
		eTopHiraLowTsu,
		wNgHiraLowTsu,
		eNgHiraLowTsu,
		eTopKataLowTsu,
		wNgKataLowTsu,
		eNgKataLowTsu,
		eTopHiraLowAN,
		eTopKataLowAN,
		eOutRangeWord,
		eNoName,
		wNgWord,
		eNgWord,
		eLengthOver,
		wSameAnnoOwnNo,
		wLikeAnnoOwnNo,
		wSameAnnoOther,
		wLikeAnnoOther,
		wNotLikeAnnoOne,
		wSameAnnoOthers,
		wLikeAnnoOthers,
		wNotLikeAnnoDup,
		// ��������t�����l���
		eNamePipeInvalid,
		eYomiPipeInvalid,
		eDifferentName,
		eYomiKanaInvalid,
		eRequiredInfoInvalid,
		eInsufficientInfo,
		eNoBldClass2,
		eSameBldClass,
		eDifferentCount,
		eKaisuZero,
		// �������猚����ʃR�[�h�ʃ��[�h�`�F�b�N
		wNGNameByBldcOKWord,
		wConflictBldcWordChk,
		wConflictOKWordChk,
		eNGNameByBldcNGWord
	};
}; // err_code

/**
 * @brief   ���O�o�̓N���X
 * @note    �W���o�� / UTF-8�̃��O�o��
 * @note    UTF-8�o�͂̂��߁AUnicode�r���h�O��
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
	 * @brief   ���O�t�@�C���I�[�v��
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
		assert( m_ofs.is_open() );
		CString cStr = _T("#SINDYSTDLOG");
		m_ofs << CT2A( cStr ) << std::endl;
	};



	/**
	 * @brief   ���O�o��
	 * @note    SINDYSTDLOG�`��(���̂ɉ��s�R�[�h���܂܂��ꍇ�ɑΉ�)
	 * @param   layer       [in]  ���C����
	 * @param   id          [in]  ObjectID
	 * @param   obj         [in]  �I�u�W�F�N�g�R���e�i
	 * @param   errDef      [in]  �G���[���ʎq
	 */
	void OutputLog( const CString& layer, long id, const GeoClass& obj, err_def::ECode errDef )
	{
		using namespace sindy::schema::sj::buildingname_point;
		assert( m_ofs.is_open() );
		cLonLat lonLat( obj.GetGeometry() );
		CString name = obj.GetFieldValue(kName);
		name.Replace(_T("\r\n"), _T("<���s>"));
		CString cStr;
		cStr.Format( _T("0\t%s\t%ld\t%lf\t%lf\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\t\t\t\t\t\t\t"),
			layer, id, lonLat.GetLon(), lonLat.GetLat(), GetErrLevelStr( GetErrLevel( errDef ) ), GetErrCode( errDef ), GetErrMsg( errDef ), 
			name, obj.GetFieldValue( kBuildingOID ), obj.GetFieldValue( kPriority ), obj.GetFieldValue( kMatchPer ) );
		m_ofs << CT2A( cStr ) << std::endl;
	};

	/**
	 * @brief   ���O�o��
	 * @note    SINDYSTDLOG�`��(���̂ɉ��s�R�[�h���܂܂��ꍇ�ɑΉ�)
	 * @param   layer       [in]  ���C����
	 * @param   id          [in]  ObjectID
	 * @param   obj         [in]  �I�u�W�F�N�g�R���e�i
	 * @param   errDef      [in]  �G���[���ʎq
	 * @param   ngWord      [in]  NG���[�h
	 */
	void OutputLog( const CString& layer, long id, const GeoClass& obj, err_def::ECode errDef, const CString& ngWord )
	{
		using namespace sindy::schema::sj::buildingname_point;
		assert( m_ofs.is_open() );
		cLonLat lonLat( obj.GetGeometry() );
		CString name = obj.GetFieldValue(kName);
		name.Replace(_T("\r\n"), _T("<���s>"));
		CString cStr;
		cStr.Format( _T("0\t%s\t%ld\t%lf\t%lf\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\t\t\t\t\t\t\t%s"),
			layer, id, lonLat.GetLon(), lonLat.GetLat(), GetErrLevelStr( GetErrLevel( errDef ) ), GetErrCode( errDef ), GetErrMsg( errDef ), 
			name, obj.GetFieldValue( kBuildingOID ), obj.GetFieldValue( kPriority ), obj.GetFieldValue( kMatchPer ),
			ngWord );
		m_ofs << CT2A( cStr ) << std::endl;
	};

	/**
	 * @brief   ���O�o��
	 * @note    SINDYSTDLOG�`��(���̂ɉ��s�R�[�h���܂܂��ꍇ�ɑΉ�)
	 * @param   layer         [in]  ���C����
	 * @param   id            [in]  ObjectID
	 * @param   obj           [in]  �I�u�W�F�N�g�R���e�i
	 * @param   errDef        [in]  �G���[���ʎq
	 * @param   posBuildID    [in]  �����ƌ`ID
	 * @param   posBuildClass [in]  �����ƌ`��ʃR�[�h
	 */
	void OutputLog( const CString& layer, long id, const GeoClass& obj, err_def::ECode errDef, const CString& posBuildID, const CString& posBuildClass )
	{
		using namespace sindy::schema::sj::buildingname_point;
		assert( m_ofs.is_open() );
		cLonLat lonLat( obj.GetGeometry() );
		CString name = obj.GetFieldValue(kName);
		name.Replace(_T("\r\n"), _T("<���s>"));
		CString cStr;
		cStr.Format( _T("0\t%s\t%ld\t%lf\t%lf\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\t\t\t\t\t\t"),
			layer, id, lonLat.GetLon(), lonLat.GetLat(), GetErrLevelStr( GetErrLevel( errDef ) ), GetErrCode( errDef ), GetErrMsg( errDef ), 
			name, obj.GetFieldValue( kBuildingOID ), obj.GetFieldValue( kPriority ), obj.GetFieldValue( kMatchPer ),
			posBuildID,  posBuildClass );
		m_ofs << CT2A( cStr ) << std::endl;
	};

	/**
	 * @brief   ���O�o��
	 * @note    SINDYSTDLOG�`��(���̂ɉ��s�R�[�h���܂܂��ꍇ�ɑΉ�)
	 * @param   layer         [in]  ���C����
	 * @param   id            [in]  ObjectID
	 * @param   obj           [in]  �I�u�W�F�N�g�R���e�i
	 * @param   errDef        [in]  �G���[���ʎq
	 * @param   otherName     [in]  ��r�I�u�W�F�N�g����
	 * @param   otherID       [in]  ��r�I�u�W�F�N�gID
	 * @param   bufferRange   [in]  �o�b�t�@����(m)
	 * @param   otherLonLat   [in]  ��r�I�u�W�F�N�g�ܓx�o�x
	 */
	void OutputLog( const CString& layer, long id, const GeoClass& obj, err_def::ECode errDef, 
		const CString& otherName, const CString& otherID, const CString& bufferRange, const cLonLat& otherLonLat )
	{
		using namespace sindy::schema::sj::buildingname_point;
		assert( m_ofs.is_open() );
		cLonLat lonLat( obj.GetGeometry() );
		CString name = obj.GetFieldValue(kName);
		name.Replace(_T("\r\n"), _T("<���s>"));
		CString cStr;
		if( otherLonLat.GetLon() == 0.0 || otherLonLat.GetLat() == 0.0 ){
			cStr.Format( _T("0\t%s\t%ld\t%lf\t%lf\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\t\t%s\t%s\t\t\t%s\t"),
				layer, id, lonLat.GetLon(), lonLat.GetLat(), GetErrLevelStr( GetErrLevel( errDef ) ), GetErrCode( errDef ), GetErrMsg( errDef ), 
				name, obj.GetFieldValue( kBuildingOID ), obj.GetFieldValue( kPriority ), obj.GetFieldValue( kMatchPer ),
				otherName,  otherID, bufferRange );
		}
		else{
			cStr.Format( _T("0\t%s\t%ld\t%lf\t%lf\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\t\t%s\t%s\t%lf\t%lf\t%s\t"),
				layer, id, lonLat.GetLon(), lonLat.GetLat(), GetErrLevelStr( GetErrLevel( errDef ) ), GetErrCode( errDef ), GetErrMsg( errDef ), 
				obj.GetFieldValue( kName ), obj.GetFieldValue( kBuildingOID ), obj.GetFieldValue( kPriority ), obj.GetFieldValue( kMatchPer ), 
				otherName,  otherID, otherLonLat.GetLon(), otherLonLat.GetLat(), bufferRange );
		}
		m_ofs << CT2A( cStr ) << std::endl;
	};

	/**
	 * @brief   ���O�o��
	 * @note    �R�����g�`��
	 * @param   msg [in]  �t���[���b�Z�[�W
	 */
	void OutputLog( const CString& msg )
	{
		assert( m_ofs.is_open() );
		CString cStr;
		cStr.Format( _T("# %s"), msg );
		m_ofs << CT2A( cStr ) << std::endl;
	};

	/**
	 * @brief   ���O�o��
	 * @note    �R�����g�`��
	 * @param   errDef   [in]  �G���[���ʎq
	 * @param   msg      [in]  �t���[���b�Z�[�W
	 */
	void OutputLog( err_def::ECode errDef, const CString& msg )
	{
		assert( m_ofs.is_open() );
		CString cStr;
		cStr.Format( _T("# %s\t%s\t%s"), GetErrLevelStr( GetErrLevel( errDef ) ), GetErrMsg( errDef ), msg );
		m_ofs << CT2A( cStr ) << std::endl;
	};

	/**
	 * @brief   �W���o��
	 * @note    �R�����g�`��
	 * @param   msg [in]  �t���[���b�Z�[�W
	 */
	void OutputStdErr( const CString& msg )
	{
		std::cerr << "# " << CT2A( msg ) << std::endl;
	};

	/**
	 * @brief   �W���o��
	 * @param   errDef   [in]  �G���[���ʎq
	 * @param   msg      [in]  �t���[���b�Z�[�W
	 */
	void OutputStdErr( err_def::ECode errDef, const CString& msg )
	{
		std::cerr << "#" << CT2A( GetErrLevelStr( GetErrLevel( errDef ) ) ) << "\t" 
			<< CT2A( GetErrMsg( errDef ) )<< "\t" << CT2A( msg ) << std::endl;
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
			case ok:      return _T("OK");
			default:      return _T("");
		}
	};

	/**
	 * @brief   �G���[���x�����擾
	 * @param   errDef [in]  �G���[���ʎq
	 * @retval  err_level    �G���[���x��
	 */
	err_level::ECode GetErrLevel( err_def::ECode errDef )
	{
		switch( errDef ){
			using namespace err_def;

			case OpenNgBlCFail:
			case OpenNgWordFail:
			case OpenBldcOKWordFail:
			case OpenBldcNGWordFail:
			case OpenReplaceFail:
			case OpenJoinAnnoFail:
			case OpenMeshFail:
			case OpenLogFail:
			case OpenExcludMarkFail:
			case GetMeshFail:
			case GetObjFail:
				return err_level::fatal;

			case StrBlNameDB:
			case StrBuildingDB:
			case StrAnnoDB:
			case StrMeshDB:
			case StrLogFile:
			case StrMeshList:
			case StrNgBuild:
			case StrReplace:
			case StrErrWord:
			case StrWarnWord:
			case StrBldcOKWord:
			case StrBldcNGWord:
			case StrChkFixclass:
			case StrJoinAnno:
			case StrExcludeMark:
			case StrBufferRange:
			case StrStartTime:
			case StrEndTime:
			case StrStart:
			case StrEnd:
			case StrQueryStart:
			case StrQueryEnd:
				return err_level::info;

			case eUnMatchBuildID:
			case eDisjointBuild:
			case eHoleUnMatchID:
			case eNgBlClass:
			case eDupPointInOne:
			case eNgSpace:
			case eNgParentheses:
			case eLeftParentOnly:
			case eRightParentOnly:
			case ePipeExist:
			case eTopZenHyphen:
			case eTopHiraLowY:
			case eNgHiraLowY:
			case eTopKataLowY:
			case eNgKataLowY:
			case eTopHiraLowTsu:
			case eNgHiraLowTsu:
			case eTopKataLowTsu:
			case eNgKataLowTsu:
			case eTopHiraLowAN:
			case eTopKataLowAN:
			case eOutRangeWord:
			case eNoName:
			case eNgWord:
			case eLengthOver:
			case eNamePipeInvalid:
			case eYomiPipeInvalid:
			case eDifferentName:
			case eYomiKanaInvalid:
			case eRequiredInfoInvalid:
			case eInsufficientInfo:
			case eNoBldClass2:
			case eSameBldClass:
			case eNGNameByBldcNGWord:
			case eDifferentCount:
			case eKaisuZero:
				return err_level::error;

			case wHoleMatchID:
			case wSameNameInBuf:
			case wLikeNameInBuf:
			case wNgHyphenWord:
			case wNgHiraLowTsu:
			case wNgKataLowTsu:
			case wNgWord:
			case wSameAnnoOwnNo:
			case wLikeAnnoOwnNo:
			case wSameAnnoOther:
			case wLikeAnnoOther:
			case wNotLikeAnnoOne:
			case wSameAnnoOthers:
			case wLikeAnnoOthers:
			case wNotLikeAnnoDup:
			case wNGNameByBldcOKWord:
			case wConflictBldcWordChk:
			case wConflictOKWordChk:
				return err_level::warning;

			case oOKWordByBldcOKWord:
			case oOkWordByMultiBldcOKWord:
				return err_level::ok;

			case Ok:
			default:
				return err_level::none;
		}
	};

	/**
	 * @brief   �G���[���ʎq����G���[�R�[�h�擾
	 * @param   errDef   [in]  �G���[���ʎq
	 * @retval  CString  �G���[�R�[�h
	 */
	CString GetErrCode( err_def::ECode errDef )
	{
		switch( errDef ){
			using namespace err_def;

			case eUnMatchBuildID:  return _T("E_1_1");
			case eDisjointBuild:   return _T("E_1_2");
			case eHoleUnMatchID:   return _T("E_1_3");
			case wHoleMatchID:     return _T("W_1_4");
			case eNgBlClass:       return _T("E_2_1");
			case eDupPointInOne:   return _T("E_3_1");
			case wSameNameInBuf:   return _T("W_4_1");
			case wLikeNameInBuf:   return _T("W_4_2");
			case eNgSpace:         return _T("E_5_1");
			case eNgParentheses:   return _T("E_5_2");
			case eLeftParentOnly:  return _T("E_5_3");
			case eRightParentOnly: return _T("E_5_4");
			case ePipeExist:       return _T("E_5_5");
			case eTopZenHyphen:    return _T("E_5_6");
			case wNgHyphenWord:    return _T("W_5_7");
			case eTopHiraLowY:     return _T("E_5_8");
			case eNgHiraLowY:      return _T("E_5_9");
			case eTopKataLowY:     return _T("E_5_10");
			case eNgKataLowY:      return _T("E_5_11");
			case eTopHiraLowTsu:   return _T("E_5_12");
			case wNgHiraLowTsu:    return _T("W_5_13");
			case eNgHiraLowTsu:    return _T("E_5_14");
			case eTopKataLowTsu:   return _T("E_5_15");
			case wNgKataLowTsu:    return _T("W_5_16");
			case eNgKataLowTsu:    return _T("E_5_17");
			case eOutRangeWord:    return _T("E_5_18");
			case eNoName:          return _T("E_5_19");
			case wNgWord:          return _T("W_5_20");
			case eNgWord:          return _T("E_5_21");
			case eTopHiraLowAN:	   return _T("E_5_22");
			case eTopKataLowAN:    return _T("E_5_23");
			case eLengthOver:      return _T("E_5_24");
			case wSameAnnoOwnNo:   return _T("W_6_1");
			case wLikeAnnoOwnNo:   return _T("W_6_2");
			case wSameAnnoOther:   return _T("W_6_3");
			case wLikeAnnoOther:   return _T("W_6_4");
			case wNotLikeAnnoOne:  return _T("W_6_5");
			case wSameAnnoOthers:  return _T("W_6_6");
			case wLikeAnnoOthers:  return _T("W_6_7");
			case wNotLikeAnnoDup:  return _T("W_6_8");
			// ��������t�����l���
			case eNamePipeInvalid:     return _T("E_6_1");
			case eYomiKanaInvalid:     return _T("E_6_2");
			case eYomiPipeInvalid:     return _T("E_6_3");
			case eRequiredInfoInvalid: return _T("E_7_1");
			case eInsufficientInfo:    return _T("E_7_2");
			case eNoBldClass2:         return _T("E_7_3");
			case eSameBldClass:        return _T("E_7_4");
			case eDifferentName:       return _T("E_7_5");
			case eDifferentCount:      return _T("E_7_6");
			case eKaisuZero:           return _T("E_7_7");
			// �������猚����ʃR�[�h�ʃ��[�h�`�F�b�N
			case wNGNameByBldcOKWord:		return _T( "W_8_1" );
			case wConflictBldcWordChk:		return _T( "W_8_2" );
			case wConflictOKWordChk:		return _T( "W_8_3" );
			case eNGNameByBldcNGWord:		return _T( "E_8_1" );
			case oOKWordByBldcOKWord:		return _T( "O_8_1" );
			case oOkWordByMultiBldcOKWord:	return _T( "O_8_2" );

			default:               return _T("");
		}
	};

	/**
	 * @brief   �G���[���ʎq����G���[���b�Z�[�W�擾
	 * @param   errDef   [in]  �G���[���ʎq
	 * @retval  CString �G���[���b�Z�[�W
	 */
	CString GetErrMsg( err_def::ECode errDef )
	{
		switch( errDef ){
			using namespace err_def;

			// FATAL
			case OpenNgBlCFail:      return _T("Open NG Build Class File Failure");
			case OpenNgWordFail:     return _T("Open NG Word File Failure");
			case OpenBldcOKWordFail: return _T("Open BuildClass OK Word File Failure");
			case OpenBldcNGWordFail: return _T("Open BuildClass NG Word File Failure");
			case OpenReplaceFail:    return _T("Open Replace Word File Failure");
			case OpenJoinAnnoFail:   return _T("Open Join Anno Str File Failure");
			case OpenMeshFail:       return _T("Open Meshlist Failure");
			case OpenLogFail:        return _T("Open LogFile Failure");
			case OpenExcludMarkFail: return _T("Open ExcludMark Failure");
			case GetMeshFail:        return _T("���b�V���|���S���擾���s");
			case GetObjFail:         return _T("���b�V�����̊e��I�u�W�F�N�g�擾���s");

			// INFO
			case StrBlNameDB:      return _T("Build Name DB");
			case StrBuildingDB:    return _T("Building DB");
			case StrAnnoDB:        return _T("Anno DB");
			case StrMeshDB:        return _T("Mesh DB");
			case StrLogFile:       return _T("Log File");
			case StrMeshList:      return _T("Mesh List");
			case StrNgBuild:       return _T("NG Build Class File");
			case StrReplace:       return _T("Replace Word File");
			case StrErrWord:       return _T("Error NG Word File");
			case StrWarnWord:      return _T("Warning NG Word File");
			case StrBldcOKWord:    return _T("BuildClass OK Word File");
			case StrBldcNGWord:    return _T("BuildClass NG Word File");
			case StrChkFixclass:   return _T("Check FIXCLASS_C for BuildClass OK/NG Word Check");
			case StrJoinAnno:      return _T("Join Anno Str File");
			case StrExcludeMark:   return _T("Exclude Mark File");
			case StrBufferRange:   return _T("Buffer Range(m)");
			case StrStartTime:     return _T("Start Time");
			case StrEndTime:       return _T("End Time");

			// Error/Warning
			case eUnMatchBuildID:  return _T("�ƌ`ID����v���Ȃ�");
			case eDisjointBuild:   return _T("�ƌ`�O�ɑ��݂���");
			case eHoleUnMatchID:   return _T("�ƌ`ID����v���Ȃ��ƌ`�̒����������ɑ��݂���");
			case wHoleMatchID:     return _T("�ƌ`ID����v����ƌ`�̒����������ɑ��݂���");
			case eNgBlClass:       return _T("�쐬���O������ʉƌ`��ɑ��݂���");
			case eDupPointInOne:   return _T("�ƌ`���ɕ������݂���");
			case wSameNameInBuf:   return _T("�אډƌ`�ɖ��̊��S��v�f�[�^�����݂���");
			case wLikeNameInBuf:   return _T("�אډƌ`�ɖ��̕�܃f�[�^�����݂���");
			case eNgSpace:         return _T("�s���ȃX�y�[�X");
			case eNgParentheses:   return _T("�J�b�R���s����");
			case eLeftParentOnly:  return _T("�u�i�v�����Ȃ�");
			case eRightParentOnly: return _T("�u�j�v�����Ȃ�");
			case ePipeExist:       return _T("�u�b�v���܂܂�Ă���");
			case eTopZenHyphen:    return _T("�擪�Ɂu�[�v�u�|�v�u�\�v�u�]�v");
			case wNgHyphenWord:    return _T("�s���ȃn�C�t��");
			case eTopHiraLowY:     return _T("�擪�Ɂu����v");
			case eNgHiraLowY:      return _T("�s���ȁu����v");
			case eTopKataLowY:     return _T("�擪�Ɂu�������v");
			case eNgKataLowY:      return _T("�s���ȁu�������v");
			case eTopHiraLowTsu:   return _T("�擪�Ɂu���v");
			case wNgHiraLowTsu:    return _T("�s���ȁu���v");
			case eNgHiraLowTsu:    return _T("�s���ȁu���v");
			case eTopKataLowTsu:   return _T("�擪�Ɂu�b�v");
			case wNgKataLowTsu:    return _T("�s���ȁu�b�v");
			case eNgKataLowTsu:    return _T("�s���ȁu�b�v");
			case eTopHiraLowAN:    return _T("�擪�Ɂu������������v");
			case eTopKataLowAN:    return _T("�擪�Ɂu�@�B�D�F�H���v");
			case eOutRangeWord:    return _T("�d�l�͈͊O�̕����R�[�h������");
			case eNoName:          return _T("���̂���");
			case wNgWord:          return _T("NG���[�h");
			case eNgWord:          return _T("NG���[�h");
			case eLengthOver:      return _T("�ő啶�����𒴂��Ă���");
			case wSameAnnoOwnNo:   return _T("�ƌ`���ɒ��L���Ȃ��A���ӂ̒��L�ɖ��̊��S��v�f�[�^�����݂���");
			case wLikeAnnoOwnNo:   return _T("�ƌ`���ɒ��L���Ȃ��A���ӂ̒��L�ɖ��̕�܃f�[�^�����݂���");
			case wSameAnnoOther:   return _T("�ƌ`���ɒ��L��1���݂��邪���̗ގ������A���ӂ̒��L�ɖ��̊��S��v�f�[�^�����݂���");
			case wLikeAnnoOther:   return _T("�ƌ`���ɒ��L��1���݂��邪���̗ގ������A���ӂ̒��L�ɖ��̕�܃f�[�^�����݂���");
			case wNotLikeAnnoOne:  return _T("�ƌ`���ɒ��L��1���݂��邪���̗ގ����Ȃ�");
			case wSameAnnoOthers:  return _T("�ƌ`���ɒ��L���������݂��邪���̗ގ������A���ӂ̒��L�ɖ��̊��S��v�f�[�^�����݂���");
			case wLikeAnnoOthers:  return _T("�ƌ`���ɒ��L���������݂��邪���̗ގ������A���ӂ̒��L�ɖ��̕�܃f�[�^�����݂���");
			case wNotLikeAnnoDup:  return _T("�ƌ`���ɒ��L���������݂��邪���̗ގ����Ȃ�");
			// ��������t�����l���
			case eNamePipeInvalid: return _T("���́i�����j�́b�i�p�C�v�j���擪�E�����ɁA�܂��͘A�����đ��݂��Ă���");
			case eYomiPipeInvalid: return _T("���~�i�����j�́b�i�p�C�v�j���擪�E�����ɁA�܂��͘A�����đ��݂��Ă���");
			case eDifferentName:   return _T("���́i�����j�́b�i�p�C�v�j�������������񂪖��̂ƈ�v���Ȃ�");
			case eYomiKanaInvalid: return _T("���~�i�����j�ɑS�p�J�i�A�b�i�p�C�v�j�ȊO�̕������܂܂�Ă���");
			case eRequiredInfoInvalid: return _T("�t�����K�{���ڂ̂����ꂩ�̏�񂪌������Ă���");
			case eInsufficientInfo: return _T("�t�����K�{���ڈȊO�̕t�����̂ݏ�񂪓����Ă���");
			case eNoBldClass2:     return _T("��ʂQ���������Ă���");
			case eSameBldClass:    return _T("��ʂP�A�Q�A�R�ɓ����R�[�h�������Ă���");
			case eDifferentCount:  return _T("���̂ƃ��~�̕��������Ⴄ");
			case eKaisuZero:       return _T("�K����0�������Ă���");
			// �������猚����ʃR�[�h�ʃ��[�h�`�F�b�N
			case wNGNameByBldcOKWord:  return _T( "�擾���ꂽ��ʂɑ΂���OK���[�h�͑��݂��܂��񂪁A�ʂ̎�ʂ�OK���[�h�����݂��܂��B��ʂ̊m�F�����Ă��������B" );
			case wConflictBldcWordChk: return _T( "OK���[�h�`�F�b�N����OK��NG���[�h�`�F�b�N����NG���������Ă��܂��B��ʂ̊m�F�����Ă��������B" );
			case wConflictOKWordChk:   return _T( "�擾���ꂽ��ʂɑ΂���OK���[�h�����݂��܂����A�ʂ̎�ʂ�OK���[�h�����݂��܂��B�K�v�ɉ����Ď�ʂ̊m�F�����Ă��������B" );
			case eNGNameByBldcNGWord:  return _T( "��ʂɑ΂���NG���[�h�����̂ɑ��݂��܂��B��ʂ̊m�F�����Ă��������B" );
			// OK
			case oOKWordByBldcOKWord: return _T( "���̂�OK���[�h�����݂��AOK���[�h�Ǝ�ʂ̑g�ݍ��킹�ɍ��v���Ă��܂��B��ʊm��R�[�h��1�ɂ��Ă��������B" );
			case oOkWordByMultiBldcOKWord: return _T( "���̂ɕ�����ʂ�OK���[�h�����݂��AOK���[�h�Ǝ�ʂ̑g�ݍ��킹��1�ɍ��v���Ă��܂��B��ʊm��R�[�h��1�ɂ��Ă��������B" );

			default:               return _T("");
		}
	};

private:

	std::ofstream m_ofs; //!< ���O�o�̓X�g���[��
};
