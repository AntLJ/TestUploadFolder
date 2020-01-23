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
#include "AddrCount.h"
#include <AddrLib/GouPoints.h>
#include <AddrLib/CSAddrPoint.h>

bool AddrCount::Init(bool cCharF, ETarget cTarget, const CString& cTgtOpt, IFeatureClassPtr cGouPoint, IFeatureClassPtr cCityAdmin, IFeatureClassPtr cCSAddrPoint)
{
	mCharF			= cCharF;
	mTarget			= cTarget;
	mTgtOpt			= cTgtOpt;
	mGouPoint		= cGouPoint;
	mCityAdmin		= cCityAdmin;
	mCSAddrPoint	= CFeatureClass(cCSAddrPoint);

	mFMapCSPoint	= CSPFieldMap(mCSAddrPoint.GetFieldMap());
	mNameCSPoint	= CSPTableNameString(mCSAddrPoint.GetNameString());

	if(!fnCreateAddrList()) return false;

	return true;
}

bool AddrCount::Count()
{
	// 5桁コードリストのループ
	for(CAddrCodeList::const_iterator itrList = mList.begin(); itrList != mList.end(); ++itrList)
	{
		cerr << CT2CA(*itrList) << " : カウント中" << endl;

		CGouPoints aGouPts(mCityAdmin);
		aGouPts.SelectByAddrCode(*itrList);

		AddrSet aAddrSet;

		// 行政界のループ
		for(CCityAdmin::const_rows_iterator itrAdmin = aGouPts.begin(aGouPts.GetTableType()); itrAdmin != aGouPts.end(aGouPts.GetTableType()); ++itrAdmin)
		{
			const CSPCityAdmin& rAdmin = boost::static_pointer_cast<CCityAdmin>(*itrAdmin);
			CString aAddrCode2 = rAdmin->GetOazaAzaCode2();

			// 文字付き住所モードでなく、住居表示 or 地番整備済み／街区符号がNULLの場合はスキップ
			if(!mCharF && rAdmin->GetGaikuFugo() == 0 &&
				(rAdmin->GetAddrClass() == city_admin::addr_class::kChibanMaintenance ||
				rAdmin->GetAddrClass() == city_admin::addr_class::kResidenceIndicationMaintenance))
			{
				cerr << "#Notice 住居表示 or 地番整備済み／街区符号がNULL : " << CT2A(rAdmin->GetOazaAzaCode()) << endl;
				continue;
			}

			// 号ポイントのループ
			for(CGouPoint::const_rows_iterator itrGou = rAdmin->begin(sindyTableType::gou_point); itrGou != rAdmin->end(sindyTableType::gou_point); ++itrGou)
			{
				const CSPGouPoint pGou = boost::static_pointer_cast<CGouPoint>(*itrGou);

				if(!mCharF) {	// 文字付き住所モードでない

					CString aAddrCode(pGou->GetAddrCode());		// 20桁コード

					if(aAddrCode.IsEmpty()) {
//						cerr << "#Notice 20桁住所コードが空文字列 : " << pGou->GetOID() << " : " << CT2A(pGou->GetGouNo()) << endl;
						continue;
					}

					// 20桁コードをいったんsetに投入
					aAddrSet.insert(aAddrCode);

					// 行政界に ADDRCODE2 が設定されている場合
					if(aAddrCode2 != _T("000000")) {
						aAddrCode = aAddrCode.Left(5) + aAddrCode2 + aAddrCode.Mid(11);
						aAddrSet.insert(aAddrCode);
					}

				} else {		// 文字付き住所モードの場合

					CString aAddrCode(rAdmin->GetAddrCode());	// 行政界の住所コード(11 or 16桁)
					CString	aExGaiku(rAdmin->GetExGaikuFugo());	// 拡張街区符号

					CString	aGouNo(pGou->GetGouNo());			// 号番号
					vector<CString> aTokens;
					fnTokenizeGouNo(aGouNo, aTokens);			// 号番号をトークンに分解

					if(!aExGaiku.IsEmpty()) {	// 拡張街区符号あり
						// 拡張号番号フラグが立っているものは20桁コード化できるもの以外をカウント対象とする
						if(!(pGou->IsExGouNo() && aGouNo.Left(1) != _T("#") && 0 < aTokens.size() && aTokens.size() <= 2 &&
							aTokens[0].GetLength() <= 5 && (aTokens.size() < 2 || aTokens[1].GetLength() <= 4))) {
							aAddrCode = aAddrCode + "|" + aExGaiku + "|" + aGouNo;
							aAddrSet.insert(aAddrCode);
						}
					} else {
						// 拡張号番号フラグが立っている場合は16桁コードを11桁に削る
						if(pGou->IsExGouNo() && aAddrCode.GetLength() == 16) aAddrCode = aAddrCode.Left(11);
						if(aGouNo.Left(1) == _T("#")) {	// 先頭に「#」
							aAddrCode = aAddrCode + "|" + aGouNo;
							aAddrSet.insert(aAddrCode);
						}
						else if(aGouNo != _T("*")) {
							if(aTokens.size() > 0) {
								if(aAddrCode.GetLength() == 16) {	// 街区符号界
									if(aTokens.size() > 1 || aTokens[0].GetLength() > 4) {
										// ハイフンが１つ以上 or 先頭の数字が５桁以上
										aAddrCode = aAddrCode + "|" + aGouNo;
										aAddrSet.insert(aAddrCode);
									}
								} else {	// 街区符号界でない
									if(aTokens.size() > 2 || aTokens[0].GetLength() > 5 || (aTokens.size() > 1 && aTokens[1].GetLength() > 4)) {
										// ハイフンが２つ以上 or 先頭の数字が６桁以上 or ２つ目の数字が５桁以上
										aAddrCode = aAddrCode + "|" + aGouNo;
										aAddrSet.insert(aAddrCode);
									}
								}
							}
						}
					}
				}
			}
		}

		// CSポイントのカウント（文字付き住所モードの際はカウントしない）
		if(!mCharF) {
			IQueryFilterPtr ipFilter(CLSID_QueryFilter);
			CString aWhere;
			aWhere.Format(_T("%s LIKE '%s%%'"), cs_addr_point::kAddrCode, *itrList);
			ipFilter->put_WhereClause(CComBSTR(aWhere));
			_ICursorPtr ipCursor = mCSAddrPoint.Search(ipFilter, VARIANT_FALSE);
			if(ipCursor) {
				_IRowPtr ipRow;
				while(ipCursor->NextRow(&ipRow) == S_OK) {
					CCSAddrPoint aCSPt(ipRow, sindyTableType::cs_addr_point, false, mFMapCSPoint, mNameCSPoint);
					CString aAddrCode = aCSPt.GetAddrCode();
					aAddrSet.insert(aAddrCode);
				}
			}
		}

		// ユニーク件数のカウント
		for(AddrSet::const_iterator itrSet = aAddrSet.begin(); itrSet != aAddrSet.end(); ++itrSet)
		{
#ifdef _DEBUG
			if(mCharF) cerr << "#Notice 文字付き住所 : " << CT2A(*itrSet) << endl;
#endif
			(*this)[(*itrSet).Left(11)]++;
		}
	}

	return true;
}

bool AddrCount::fnCreateAddrList()
{
	mList.SetObject(mCityAdmin);
	switch(mTarget) {
		case kTgtAll:
			if(!mList.MakeCodeListAll(5)) {
				cerr << "#Error 住所コードリストの生成に失敗（全国指定）" << endl;
				return false;
			}
			break;
		case kTgtRange:
			if(!mList.MakeCodeListFromRange(mTgtOpt.Left(5), mTgtOpt.Right(5))) {
				cerr << "#Error 住所コードリストの生成に失敗（範囲指定） : " << CT2A(mTgtOpt) << endl;
				return false;
			}
			break;
		case kTgtList:
			if(!mList.AddFromFile(mTgtOpt)) {
				cerr << "#Error 住所コードリストファイルの読み込みに失敗 : " << CT2A(mTgtOpt) << endl;
				return false;
			}
			break;
		case kTgtSingle:
			mList.Add(mTgtOpt);
			break;
		default:
			// ここに来ることはまずない
			cerr << "#Error カウント対象指定が不正" << endl;
			return false;
	}

	return true;
}

void AddrCount::fnTokenizeGouNo(const CString& cGouNo, vector<CString>& cTokens)
{
	int aPos(0);
	CString aToken = cGouNo.Tokenize(_T("-"), aPos);
	while(!aToken.IsEmpty()) {
		cTokens.push_back(aToken);
		aToken = cGouNo.Tokenize(_T("-"), aPos);
	}
}
