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

// エラーレベル列挙型
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

// エラーコード列挙型
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
		// ここから付加価値情報
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
		// ここから建物種別コード別ワードチェック
		wNGNameByBldcOKWord,
		wConflictBldcWordChk,
		wConflictOKWordChk,
		eNGNameByBldcNGWord
	};
}; // err_code

/**
 * @brief   ログ出力クラス
 * @note    標準出力 / UTF-8のログ出力
 * @note    UTF-8出力のため、Unicodeビルド前提
 */
class COutPut
{
public:

	/**
	 * @brief   コンストラクタ
	 */
	COutPut(){};

	/**
	 * @brief   デストラクタ
	 */
	~COutPut(){};

	/**
	 * @brief   ログファイルオープン
	 * @param   strFileName [in]  ログファイル名
	 * @retval  true  オープン成功
	 * @retval  false オープン失敗
	 */
	bool Open( const CString& strFileName ){
		m_ofs.open( strFileName );
		if( ! m_ofs )
			return false;
		return true;
	};

	/**
	 * @brief   クローズ
	 */
	void Close(){
		m_ofs.close();
	};

	/**
	 * @brief   ヘッダ出力
	 */
	void OutputHeader()
	{
		assert( m_ofs.is_open() );
		CString cStr = _T("#SINDYSTDLOG");
		m_ofs << CT2A( cStr ) << std::endl;
	};



	/**
	 * @brief   ログ出力
	 * @note    SINDYSTDLOG形式(名称に改行コードが含まれる場合に対応)
	 * @param   layer       [in]  レイヤ名
	 * @param   id          [in]  ObjectID
	 * @param   obj         [in]  オブジェクトコンテナ
	 * @param   errDef      [in]  エラー識別子
	 */
	void OutputLog( const CString& layer, long id, const GeoClass& obj, err_def::ECode errDef )
	{
		using namespace sindy::schema::sj::buildingname_point;
		assert( m_ofs.is_open() );
		cLonLat lonLat( obj.GetGeometry() );
		CString name = obj.GetFieldValue(kName);
		name.Replace(_T("\r\n"), _T("<改行>"));
		CString cStr;
		cStr.Format( _T("0\t%s\t%ld\t%lf\t%lf\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\t\t\t\t\t\t\t"),
			layer, id, lonLat.GetLon(), lonLat.GetLat(), GetErrLevelStr( GetErrLevel( errDef ) ), GetErrCode( errDef ), GetErrMsg( errDef ), 
			name, obj.GetFieldValue( kBuildingOID ), obj.GetFieldValue( kPriority ), obj.GetFieldValue( kMatchPer ) );
		m_ofs << CT2A( cStr ) << std::endl;
	};

	/**
	 * @brief   ログ出力
	 * @note    SINDYSTDLOG形式(名称に改行コードが含まれる場合に対応)
	 * @param   layer       [in]  レイヤ名
	 * @param   id          [in]  ObjectID
	 * @param   obj         [in]  オブジェクトコンテナ
	 * @param   errDef      [in]  エラー識別子
	 * @param   ngWord      [in]  NGワード
	 */
	void OutputLog( const CString& layer, long id, const GeoClass& obj, err_def::ECode errDef, const CString& ngWord )
	{
		using namespace sindy::schema::sj::buildingname_point;
		assert( m_ofs.is_open() );
		cLonLat lonLat( obj.GetGeometry() );
		CString name = obj.GetFieldValue(kName);
		name.Replace(_T("\r\n"), _T("<改行>"));
		CString cStr;
		cStr.Format( _T("0\t%s\t%ld\t%lf\t%lf\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\t\t\t\t\t\t\t%s"),
			layer, id, lonLat.GetLon(), lonLat.GetLat(), GetErrLevelStr( GetErrLevel( errDef ) ), GetErrCode( errDef ), GetErrMsg( errDef ), 
			name, obj.GetFieldValue( kBuildingOID ), obj.GetFieldValue( kPriority ), obj.GetFieldValue( kMatchPer ),
			ngWord );
		m_ofs << CT2A( cStr ) << std::endl;
	};

	/**
	 * @brief   ログ出力
	 * @note    SINDYSTDLOG形式(名称に改行コードが含まれる場合に対応)
	 * @param   layer         [in]  レイヤ名
	 * @param   id            [in]  ObjectID
	 * @param   obj           [in]  オブジェクトコンテナ
	 * @param   errDef        [in]  エラー識別子
	 * @param   posBuildID    [in]  所属家形ID
	 * @param   posBuildClass [in]  所属家形種別コード
	 */
	void OutputLog( const CString& layer, long id, const GeoClass& obj, err_def::ECode errDef, const CString& posBuildID, const CString& posBuildClass )
	{
		using namespace sindy::schema::sj::buildingname_point;
		assert( m_ofs.is_open() );
		cLonLat lonLat( obj.GetGeometry() );
		CString name = obj.GetFieldValue(kName);
		name.Replace(_T("\r\n"), _T("<改行>"));
		CString cStr;
		cStr.Format( _T("0\t%s\t%ld\t%lf\t%lf\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t\t\t\t\t\t\t"),
			layer, id, lonLat.GetLon(), lonLat.GetLat(), GetErrLevelStr( GetErrLevel( errDef ) ), GetErrCode( errDef ), GetErrMsg( errDef ), 
			name, obj.GetFieldValue( kBuildingOID ), obj.GetFieldValue( kPriority ), obj.GetFieldValue( kMatchPer ),
			posBuildID,  posBuildClass );
		m_ofs << CT2A( cStr ) << std::endl;
	};

	/**
	 * @brief   ログ出力
	 * @note    SINDYSTDLOG形式(名称に改行コードが含まれる場合に対応)
	 * @param   layer         [in]  レイヤ名
	 * @param   id            [in]  ObjectID
	 * @param   obj           [in]  オブジェクトコンテナ
	 * @param   errDef        [in]  エラー識別子
	 * @param   otherName     [in]  比較オブジェクト名称
	 * @param   otherID       [in]  比較オブジェクトID
	 * @param   bufferRange   [in]  バッファ距離(m)
	 * @param   otherLonLat   [in]  比較オブジェクト緯度経度
	 */
	void OutputLog( const CString& layer, long id, const GeoClass& obj, err_def::ECode errDef, 
		const CString& otherName, const CString& otherID, const CString& bufferRange, const cLonLat& otherLonLat )
	{
		using namespace sindy::schema::sj::buildingname_point;
		assert( m_ofs.is_open() );
		cLonLat lonLat( obj.GetGeometry() );
		CString name = obj.GetFieldValue(kName);
		name.Replace(_T("\r\n"), _T("<改行>"));
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
	 * @brief   ログ出力
	 * @note    コメント形式
	 * @param   msg [in]  フリーメッセージ
	 */
	void OutputLog( const CString& msg )
	{
		assert( m_ofs.is_open() );
		CString cStr;
		cStr.Format( _T("# %s"), msg );
		m_ofs << CT2A( cStr ) << std::endl;
	};

	/**
	 * @brief   ログ出力
	 * @note    コメント形式
	 * @param   errDef   [in]  エラー識別子
	 * @param   msg      [in]  フリーメッセージ
	 */
	void OutputLog( err_def::ECode errDef, const CString& msg )
	{
		assert( m_ofs.is_open() );
		CString cStr;
		cStr.Format( _T("# %s\t%s\t%s"), GetErrLevelStr( GetErrLevel( errDef ) ), GetErrMsg( errDef ), msg );
		m_ofs << CT2A( cStr ) << std::endl;
	};

	/**
	 * @brief   標準出力
	 * @note    コメント形式
	 * @param   msg [in]  フリーメッセージ
	 */
	void OutputStdErr( const CString& msg )
	{
		std::cerr << "# " << CT2A( msg ) << std::endl;
	};

	/**
	 * @brief   標準出力
	 * @param   errDef   [in]  エラー識別子
	 * @param   msg      [in]  フリーメッセージ
	 */
	void OutputStdErr( err_def::ECode errDef, const CString& msg )
	{
		std::cerr << "#" << CT2A( GetErrLevelStr( GetErrLevel( errDef ) ) ) << "\t" 
			<< CT2A( GetErrMsg( errDef ) )<< "\t" << CT2A( msg ) << std::endl;
	};

private:

	/** 
	 * @brief   エラーレベルの文字列取得
	 * @param   errLevel [in]  エラーレベル
	 * @retval  CString  エラーレベル文字列
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
	 * @brief   エラーレベルを取得
	 * @param   errDef [in]  エラー識別子
	 * @retval  err_level    エラーレベル
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
	 * @brief   エラー識別子からエラーコード取得
	 * @param   errDef   [in]  エラー識別子
	 * @retval  CString  エラーコード
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
			// ここから付加価値情報
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
			// ここから建物種別コード別ワードチェック
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
	 * @brief   エラー識別子からエラーメッセージ取得
	 * @param   errDef   [in]  エラー識別子
	 * @retval  CString エラーメッセージ
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
			case GetMeshFail:        return _T("メッシュポリゴン取得失敗");
			case GetObjFail:         return _T("メッシュ内の各種オブジェクト取得失敗");

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
			case eUnMatchBuildID:  return _T("家形IDが一致しない");
			case eDisjointBuild:   return _T("家形外に存在する");
			case eHoleUnMatchID:   return _T("家形IDが一致しない家形の中抜き部分に存在する");
			case wHoleMatchID:     return _T("家形IDが一致する家形の中抜き部分に存在する");
			case eNgBlClass:       return _T("作成除外建物種別家形上に存在する");
			case eDupPointInOne:   return _T("家形内に複数存在する");
			case wSameNameInBuf:   return _T("隣接家形に名称完全一致データが存在する");
			case wLikeNameInBuf:   return _T("隣接家形に名称包含データが存在する");
			case eNgSpace:         return _T("不正なスペース");
			case eNgParentheses:   return _T("カッコが不整合");
			case eLeftParentOnly:  return _T("「（」しかない");
			case eRightParentOnly: return _T("「）」しかない");
			case ePipeExist:       return _T("「｜」が含まれている");
			case eTopZenHyphen:    return _T("先頭に「ー」「－」「―」「‐」");
			case wNgHyphenWord:    return _T("不正なハイフン");
			case eTopHiraLowY:     return _T("先頭に「ゃゅょ」");
			case eNgHiraLowY:      return _T("不正な「ゃゅょ」");
			case eTopKataLowY:     return _T("先頭に「ャュョ」");
			case eNgKataLowY:      return _T("不正な「ャュョ」");
			case eTopHiraLowTsu:   return _T("先頭に「っ」");
			case wNgHiraLowTsu:    return _T("不正な「っ」");
			case eNgHiraLowTsu:    return _T("不正な「っ」");
			case eTopKataLowTsu:   return _T("先頭に「ッ」");
			case wNgKataLowTsu:    return _T("不正な「ッ」");
			case eNgKataLowTsu:    return _T("不正な「ッ」");
			case eTopHiraLowAN:    return _T("先頭に「ぁぃぅぇぉん」");
			case eTopKataLowAN:    return _T("先頭に「ァィゥェォン」");
			case eOutRangeWord:    return _T("仕様範囲外の文字コードがある");
			case eNoName:          return _T("名称が空");
			case wNgWord:          return _T("NGワード");
			case eNgWord:          return _T("NGワード");
			case eLengthOver:      return _T("最大文字長を超えている");
			case wSameAnnoOwnNo:   return _T("家形内に注記がなく、周辺の注記に名称完全一致データが存在する");
			case wLikeAnnoOwnNo:   return _T("家形内に注記がなく、周辺の注記に名称包含データが存在する");
			case wSameAnnoOther:   return _T("家形内に注記が1つ存在するが名称類似せず、周辺の注記に名称完全一致データが存在する");
			case wLikeAnnoOther:   return _T("家形内に注記が1つ存在するが名称類似せず、周辺の注記に名称包含データが存在する");
			case wNotLikeAnnoOne:  return _T("家形内に注記が1つ存在するが名称類似しない");
			case wSameAnnoOthers:  return _T("家形内に注記が複数存在するが名称類似せず、周辺の注記に名称完全一致データが存在する");
			case wLikeAnnoOthers:  return _T("家形内に注記が複数存在するが名称類似せず、周辺の注記に名称包含データが存在する");
			case wNotLikeAnnoDup:  return _T("家形内に注記が複数存在するが名称類似しない");
			// ここから付加価値情報
			case eNamePipeInvalid: return _T("名称（分割）の｜（パイプ）が先頭・末尾に、または連続して存在している");
			case eYomiPipeInvalid: return _T("ヨミ（分割）の｜（パイプ）が先頭・末尾に、または連続して存在している");
			case eDifferentName:   return _T("名称（分割）の｜（パイプ）を除いた文字列が名称と一致しない");
			case eYomiKanaInvalid: return _T("ヨミ（分割）に全角カナ、｜（パイプ）以外の文字が含まれている");
			case eRequiredInfoInvalid: return _T("付加情報必須項目のいずれかの情報が欠落している");
			case eInsufficientInfo: return _T("付加情報必須項目以外の付加情報のみ情報が入っている");
			case eNoBldClass2:     return _T("種別２が欠落している");
			case eSameBldClass:    return _T("種別１、２、３に同じコードが入っている");
			case eDifferentCount:  return _T("名称とヨミの分割数が違う");
			case eKaisuZero:       return _T("階数に0が入っている");
			// ここから建物種別コード別ワードチェック
			case wNGNameByBldcOKWord:  return _T( "取得された種別に対するOKワードは存在しませんが、別の種別のOKワードが存在します。種別の確認をしてください。" );
			case wConflictBldcWordChk: return _T( "OKワードチェック結果OKとNGワードチェック結果NGが競合しています。種別の確認をしてください。" );
			case wConflictOKWordChk:   return _T( "取得された種別に対するOKワードが存在しますが、別の種別のOKワードも存在します。必要に応じて種別の確認をしてください。" );
			case eNGNameByBldcNGWord:  return _T( "種別に対するNGワードが名称に存在します。種別の確認をしてください。" );
			// OK
			case oOKWordByBldcOKWord: return _T( "名称にOKワードが存在し、OKワードと種別の組み合わせに合致しています。種別確定コードを1にしてください。" );
			case oOkWordByMultiBldcOKWord: return _T( "名称に複数種別のOKワードが存在し、OKワードと種別の組み合わせの1つに合致しています。種別確定コードを1にしてください。" );

			default:               return _T("");
		}
	};

private:

	std::ofstream m_ofs; //!< ログ出力ストリーム
};
