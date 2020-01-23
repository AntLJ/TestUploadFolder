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
#include "ErrorMsg.h"

CString CLog::GetErrCodeMsg(err_code::ECode errCode)
{
	using namespace err_code;
	switch(errCode)
	{
		case NgRWLDuplicateData:             return _T("データが重複している");
		case NgRWLExistFromNodeInRWN:        return _T("FROMNODEがRWNに存在しない");
		case NgRWLFromNodeAndRWNGeomSame:    return _T("FROMNODEIDのRWNと始点の形状が不一致");
		case NgRWLExistToNodeInRWN:          return _T("TONODEIDのRWNが存在しない");
		case NgRWLToNodeAndRWNGeomSame:      return _T("TONODEIDのRWNと終点の形状が不一致");
		case NgRWLSmallSegment:              return _T("微小セグメントが存在する");
		case NgRWLSameAttribute:             return _T("同一OBJECTID群の属性が不一致");
		case NgRWLSequenceDivID:             return _T("同一OBJECTID群のDIVIDが不連続");
		case NgRWLDuplicateVertex:           return _T("構成点が重複している");
		case NgRWLDuplicateVertexAllData:    return _T("全データ内で構成点が重複している");
		case NgRWLDuplicateSegment:          return _T("全データ内でセグメントが重複している");
		case NgRWLFirstDivLinkFromDivIDNot0: return _T("最初のDIVIDを持つリンクのFROMDIVIDが0でない");
		case NgRWLLastDivLinkToDivIDNot0:    return _T("最後のDIVIDを持つリンクのTODIVIDが0でない");
		case NgRWLLoopLink:                  return _T("ループリンクが存在する");
		case NgRWLGroupLink:                 return _T("グループ化できない");

		case NgRWNDuplicateData:             return _T("データが重複している");
		case NgRWNDivOIDisMinimumRWLOID:     return _T("DIVIDを持つノードOBJECTIDが接続リンクの最小OBJECTIDになっていない");
		case NgRWNSequenceDivID:             return _T("DIVIDを持つ同一OBJECTIDのノードがリンクの向きにDIVIDが連続していない");
		case NgRWNDuplicateGeom:             return _T("形状が重複している");
		case NgRWNExistInRWL:                return _T("RWLのFROMTONODEに存在しない");
		//case NgRWNGeomSameRWL:               return _T("RWLのFROMTONODEに形状一致しない");

		case NgRSADuplicateData:             return _T("データが重複している");

		case NgRWLExistInBefore:             return _T("変換前データに存在しない");
		case NgRWLSameAttrAsBefore:          return _T("変換前属性と不一致");
		case NgRWLSameShapeAsBefore:         return _T("変換前形状と不一致");
		case NgRWLSameAttrAsSplitData:       return _T("分割後データの属性同士が不一致");
		case NgRWLSameRoadAttrAsSplitData:   return _T("ROADATTR_Cが変換前属性と不一致（リンク内属性分割処理）");
		case NgRWLSameWalkableAsSplitData:   return _T("WALKABLE_Cが変換前属性と不一致（歩行者ノード分割・歩行者リンクマージ処理）");

		case NgRWNExistInBefore:             return _T("変換前データに存在しない");
		case NgRWNSameAttrAsBefore:          return _T("変換前属性と不一致");
		//case NgRWNSameShapeAsBefore:         return _T("変換前形状と不一致");

		case NgRWLExistInAfter:              return _T("変換後データに存在しない");
		case NgRWLMergeData:                 return _T("マージ対象リンクでない");
		case NgRWLMergeData2:                return _T("マージ対象リンクが人車共用でない");

		case NgRWNSameShape:                 return _T("変換後データに同一形状のデータが存在しない");

		case NgHNDuplicateData:              return _T("データが重複している");
		case NgHNExistInRWL:                 return _T("対応するリンクが存在しない");
		case NgHNSameHeight:                 return _T("同一OBJECTIDのリンクで接続点の高さが不一致");
		//case NgHNFrom0Height:                return _T("同一LINKIDIDのリンクで連続状態の開始点の高さが0でない");
		//case NgHNTo0Height:                  return _T("同一LINKIDIDのリンクで連続状態の終了点の高さが0でない");

		case NgHNExistInBefore:              return _T("変換前データに存在しない");
		//case NgHNSameAttrAsBefore:           return _T("変換前属性と不一致");
		//case NgHNSameShapeAsBefore:          return _T("変換前形状と不一致");

		case NgHNExistInAfter:               return _T("変換後データに存在しない");
		case NgHNSameLinkIdAsAfter:          return _T("変換後LINKIDと不一致");
		case NgHNSameAttrAsAfter:            return _T("変換後属性と不一致");

		case NgHNExistInAfterRWN:            return _T("変換後RWNにデータが存在しない");
		case NgHNReconstractHNData:          return _T("高さノードデータを再構築できない");
		case NgHNReconstractNGHNData:        return _T("高さノードデータを再構築したデータが不正");

		case NgOther:     return _T("おっす");
		
		default:                  return _T("エラーです");
	}
}

CString CLog::GetErrTypeMsg(err_type::ECode errType)
{
	using namespace err_type;
	switch(errType)
	{
		case fatal:   return _T("FATAL");
		case error:   return _T("ERROR");
		case warning: return _T("WARNING");
		case info:    return _T("INFO");
		default:      return _T("UNKOWN");
	}
}

void CLog::PrintLog(
			bool bCout,
			bool bCerr,
			bool bRun,
			bool bErr,
			err_type::ECode errType,
			const CString& strMsg1,
			const CString& strMsg2,
			const CString& strMsg3)
{
	CString strPrintMsg;
	strPrintMsg.Format(_T("%s\t\t\t\t\t%s\t%s\t%s\t\t"), GetErrTypeMsg(errType), strMsg1, strMsg2, strMsg3);
	PrintLog(bCout, bCerr, bRun, bErr, strPrintMsg);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					err_type::ECode errType,
					err_code::ECode errCode,
					const CString& strTableName,
					long lOID,
					long lDivID,
					const CString& strOtherMsg1,
					const CString& strOtherMsg2,
					const CString& strOtherMsg3)
{
	// 最終的なログ出力部分のフォーマットをここで設定
	CString strDivID;
	if(-1 != lDivID)
		strDivID.Format(_T("%ld"), lDivID);
	CString strPrintMsg;
	if(lOID > 0)
		strPrintMsg.Format(_T("%s\t%s\t%ld\t%s\t%ld\t%s\t%s\t%s\t%s\t"), GetErrTypeMsg(errType), strTableName, lOID, strDivID, errCode, GetErrCodeMsg(errCode), strOtherMsg1, strOtherMsg2, strOtherMsg3);
	else
		strPrintMsg.Format(_T("%s\t%s\t\t\t%ld\t%ld\t%s\t%s\t%s\t%s\t"), GetErrTypeMsg(errType), strTableName, errCode, GetErrCodeMsg(errCode), strOtherMsg1, strOtherMsg2, strOtherMsg3);

	PrintLog(bCout, bCerr, bRun, bErr, strPrintMsg);
}

void CLog::PrintLog(bool bCout,
					bool bCerr,
					bool bRun,
					bool bErr,
					const CString& strMsg,
					bool bLineBreak)
{
	if(bCout){
		if(bLineBreak)
			std::cout << CT2A(strMsg) << std::endl;
		else
			std::cout << CT2A(strMsg) << std::flush;
	}

	if(bCerr){
		if(bLineBreak)
			std::cerr << CT2A(strMsg) << std::endl;
		else
			std::cerr << CT2A(strMsg) << std::flush;
	}

	if(bRun){
		if(bLineBreak)
			m_ofsRunLog << CT2A(strMsg) << std::endl;
		else
			m_ofsRunLog << CT2A(strMsg) << std::flush;
	}

	if(bErr){
		if(bLineBreak)
			m_ofsErrLog << CT2A(strMsg) << std::endl;
		else
			m_ofsErrLog << CT2A(strMsg) << std::flush;
	}
}

bool CLog::SetLogFile(const CString& strFilePath, std::ofstream& ofsLogFile)
{
	ofsLogFile.open(strFilePath);
	if(!ofsLogFile.is_open()){
		std::cerr << CT2A(strFilePath) << "のオープンに失敗" << std::endl;
		return false;
	}
	return true;
}

bool CLog::SetRunLog(const CString& strFilePath)
{
	return SetLogFile(strFilePath, m_ofsRunLog);
}

bool CLog::SetErrLog(const CString& strFilePath)
{
	return SetLogFile(strFilePath, m_ofsErrLog);
}
