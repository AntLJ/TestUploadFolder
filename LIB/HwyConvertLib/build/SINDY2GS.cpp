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

// SINDY2GS.cpp: SINDY2GS クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SINDY2GS.h"
#include "SINDY2INCODE.h"

SINDY2GS::SINDY2GS(SINDY2INHWYINFO *cHwyInfo) :
SINDY2INHWY(cHwyInfo),
mGsList()
{

}

SINDY2GS::~SINDY2GS()
{

}

HRESULT SINDY2GS::Convert()
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2GS::Convert";

	for (GDBOBJITER aIter = mpHwyInfo->eAccessPoint.begin(); aIter != mpHwyInfo->eAccessPoint.end(); aIter++)
	{
		IFeaturePtr ipAccessPoint = aIter->second;

		bool aHwyMode = true;
		if (FAILED(hr = mpHwyInfo->isHwyMode(ipAccessPoint, &aHwyMode)))
		{
			PrintError(ipAccessPoint, "ハイウェイモードフラグの取得に失敗");
			return hr;
		}
		if (! aHwyMode) continue;

		CComVariant vaGsCode;
		if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::GS_C, &vaGsCode)))
		{
			PrintError(ipAccessPoint, "ガソリンスタンドの取得に失敗");
			return hr;
		}
		vaGsCode.ChangeType(VT_I4);

		CComVariant vaGs;
		if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::GS_F, &vaGs)))
		{
			PrintError(ipAccessPoint, "ガソリンスタンドフラグの取得に失敗");
			return hr;
		}
		vaGs.ChangeType(VT_I4);
		CComVariant vaMultipleStore;
		if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::MULTIPLESTORE_F, &vaMultipleStore)))
		{
			PrintError(ipAccessPoint, "チェーン店フラグの取得に失敗");
			return hr;
		}
		vaMultipleStore.ChangeType(VT_I4);

		// ガソリンスタンドorチェーン店のないアクセスポイントならスキップ
		if (! vaGs.lVal && ! vaMultipleStore.lVal) {
			continue;
		}

		long aRoadCode, aRoadSeq;
		if (FAILED(hr = mpHwyInfo->GetFacilCode(ipAccessPoint, &aRoadCode, &aRoadSeq)))
		{
			PrintError(ipAccessPoint, "施設番号の取得に失敗");
			return hr;
		}

		IFeaturePtr ipFacilInfoPoint;
		if (FAILED(hr = mpHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint)))
		{
			PrintError(ipAccessPoint, "対応施設情報ポイントの取得に失敗");
			return hr;
		}
		CComVariant vaName;
		if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::NAME_KANJI, &vaName)))
		{
			PrintError(ipAccessPoint, "施設名称の取得に失敗");
			return hr;
		}
		std::string aName;
		if (vaName.vt != VT_NULL && vaName.vt != VT_EMPTY)
		{
			vaName.ChangeType(VT_BSTR);
			aName = COLE2T(vaName.bstrVal);
		}

		CComVariant vaLineDir;
		if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::DIRECTION_C, &vaLineDir)))
		{
			PrintError(ipAccessPoint, "上下線属性の取得に失敗");
			return hr;
		}
		vaLineDir.ChangeType(VT_I4);

		GS aGsInfo;
		aGsInfo.eGsType  = vaGsCode.lVal;
		aGsInfo.eLineDir  = vaLineDir.lVal;
		aGsInfo.eName     = aName;
		aGsInfo.eRoadCode = aRoadCode;
		aGsInfo.eSequence = aRoadSeq;

		std::vector<AccessStore>	aAccessStoreList;
		if (FAILED(hr = GetAccessStore(mpHwyInfo, ipAccessPoint, &aAccessStoreList))) 
			return hr;

		for (std::vector<AccessStore>::const_iterator aIt = aAccessStoreList.begin(); aIt != aAccessStoreList.end(); ++aIt)
		{
			switch (aIt->eStoreType)
			{
			case gs:
				if (vaGs.lVal) {
					aGsInfo.eAccessStoreInfo.push_back(*aIt);
				}
				break;
			case multiple_store:
				if (vaMultipleStore.lVal) {
					aGsInfo.eAccessStoreInfo.push_back(*aIt);
				}
				break;
			default:
				ATLASSERT(0);
			}
		}

		mGsList.push_back(aGsInfo);
	}

	return S_OK;
}

HRESULT SINDY2GS::Write(std::string cFileName, EType cType, EFormat cFormat)
{
	const char *func = "SINDY2GS::Write";

	std::ofstream aFile(cFileName.c_str(), ios::binary);
	if (! aFile.is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	for (unsigned long i = 0; i < mGsList.size(); i++)
	{
		switch (cFormat)
		{
		case gs_old:

			// GS_Cがgs_noであれば出力しない
			if (mGsList[i].eGsType == sindy::access_point::gs_no) continue;

			// 路線コード
			aFile << std::dec << mGsList[i].eRoadCode;

			// 路線内シーケンス番号
			aFile << "\t";
			aFile << std::dec << mGsList[i].eSequence;

			// 施設名称
			aFile << "\t";
			aFile << SINDY2INCODE::SJIS2EUC(mGsList[i].eName.c_str());

			// ポイント位置名称
			aFile << "\t";
			switch (mGsList[i].eLineDir)
			{
			case sindy::access_point::direction_up:		aFile << SINDY2INCODE::SJIS2EUC("上り"); break;
			case sindy::access_point::direction_down:	aFile << SINDY2INCODE::SJIS2EUC("下り"); break;

			/// 上り・下り以外はエラーとする
			case sindy::access_point::direction_no:		//aFile << SINDY2INCODE::SJIS2EUC("方向なし"); break;
			case sindy::access_point::direction_common:	//aFile << SINDY2INCODE::SJIS2EUC("上下線共通"); break;
			default:
				GDBERROR2(E_NOINTERFACE, func);
			}

			// ガソリンスタンド銘柄
			aFile << "\t";
			switch (mGsList[i].eGsType)
			{
			case sindy::access_point::gs_other:		aFile << SINDY2INCODE::SJIS2EUC("その他"); break;
			case sindy::access_point::gs_esso:		aFile << SINDY2INCODE::SJIS2EUC("エッソ"); break;
			case sindy::access_point::gs_mobil:		aFile << SINDY2INCODE::SJIS2EUC("モービル"); break;
			case sindy::access_point::gs_kygnus:	aFile << SINDY2INCODE::SJIS2EUC("キグナス"); break;
			case sindy::access_point::gs_cosmo:		aFile << SINDY2INCODE::SJIS2EUC("コスモ"); break;
			case sindy::access_point::gs_shell:		aFile << SINDY2INCODE::SJIS2EUC("シェル"); break;
			case sindy::access_point::gs_jomo:		aFile << SINDY2INCODE::SJIS2EUC("ジョモ"); break;
			case sindy::access_point::gs_general:	aFile << SINDY2INCODE::SJIS2EUC("ゼネラル"); break;
			case sindy::access_point::gs_kyushu:	aFile << SINDY2INCODE::SJIS2EUC("九州"); break;
			case sindy::access_point::gs_idemitsu:	aFile << SINDY2INCODE::SJIS2EUC("出光"); break;
			case sindy::access_point::gs_taiyo:		aFile << SINDY2INCODE::SJIS2EUC("太陽"); break;
			case sindy::access_point::gs_eneos:		aFile << SINDY2INCODE::SJIS2EUC("ＥＮＥＯＳ"); break;
			default:								aFile << SINDY2INCODE::SJIS2EUC("？？？"); break;
			}
			aFile << std::endl;

			break;

		case gs_new:
			// Access_Storeの数だけ別レコードとして出力
			for (std::vector<AccessStore>::const_iterator aIt = mGsList[i].eAccessStoreInfo.begin(); aIt != mGsList[i].eAccessStoreInfo.end(); ++aIt)
			{
				// gs/multiple_storeのうち、指定されたタイプに合致するレコードだけを出力
				if (aIt->eStoreType != cType) { continue; }

				// 路線コード
				aFile << std::dec << mGsList[i].eRoadCode;

				// 路線内シーケンス番号
				aFile << "\t";
				aFile << std::dec << mGsList[i].eSequence;

				// 上下線フラグ
				aFile << "\t";
				switch (mGsList[i].eLineDir)
				{
				case sindy::access_point::direction_up:		aFile << direction_up;		break;
				case sindy::access_point::direction_down:	aFile << direction_down;	break;

				/// 上り・下り以外はエラーとする
				case sindy::access_point::direction_no:
				case sindy::access_point::direction_common:
				default:
					GDBERROR2(E_NOINTERFACE, func);
				}

				// チェーン店マスターコード
				aFile << "\t";
				aFile << aIt->eStoreCode;
				
				// 営業時間
				aFile << "\t";
				aFile.width(2); aFile.fill('0');
				aFile << aIt->eStartHour;
				aFile.width(2); aFile.fill('0');
				aFile << aIt->eStartMin;
				aFile.width(2); aFile.fill('0');
				aFile << aIt->eEndHour;
				aFile.width(2); aFile.fill('0');
				aFile << aIt->eEndMin;

				// 曜日フラグ
				aFile << "\t";	aFile << aIt->eSunday;
				aFile << "\t";	aFile << aIt->eMonday;
				aFile << "\t";	aFile << aIt->eTuesday;
				aFile << "\t";	aFile << aIt->eWednesday;
				aFile << "\t";	aFile << aIt->eThursday;
				aFile << "\t";	aFile << aIt->eFriday;
				aFile << "\t";	aFile << aIt->eSaturday;

				// 期間フラグ
				aFile << "\t";	aFile << aIt->eBon;
				aFile << "\t";	aFile << aIt->eYearEnd;
				aFile << "\t";	aFile << aIt->eNewYear;
				aFile << "\t";	aFile << aIt->eGoldenWeek;

				// 祝祭日フラグ
				aFile << "\t";	aFile << aIt->eHoliday;

				aFile << std::endl;
			}
			break;

		default:
			ATLASSERT(0);
		}
	}

	return S_OK;
}


HRESULT SINDY2GS::GetAccessStore(SINDY2INHWYINFO* cHwyInfo, IFeaturePtr cpAccessPoint, std::vector<AccessStore>* cpAccessStoreList)
{
	HRESULT hr = S_OK;

	long aOID = cpAccessPoint->GetOID();
	GDBOBJRANGE aRange;
	if (FAILED(hr = cHwyInfo->eAccessStore.SearchByAccessPointID(aOID, &aRange)))
	{
		PrintError(cpAccessPoint, "対応するデータがAccess_Storeに存在しない");
		return hr;
	}

	for (GDB2OBJITER aIt = aRange.first; aIt != aRange.second; ++aIt)
	{
		AccessStore aAccessStore;

		// チェーン店マスターコード
		CComVariant	vaStoreCode;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::MULTIPLESTORE_C, &vaStoreCode)))
		{
			PrintError(cpAccessPoint, "チェーン店マスターコードの取得に失敗");
			return hr;
		}
		vaStoreCode.ChangeType(VT_I4);
		aAccessStore.eStoreCode = vaStoreCode.lVal;

		// 営業開始時間
		CComVariant vaStartHour;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::START_HOUR, &vaStartHour)))
		{
			PrintError(cpAccessPoint, "営業開始時間の取得に失敗");
			return hr;
		}
		vaStartHour.ChangeType(VT_I4);
		aAccessStore.eStartHour = vaStartHour.lVal;

		// 営業開始分
		CComVariant vaStartMin;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::START_MIN, &vaStartMin)))
		{
			PrintError(cpAccessPoint, "営業開始分の取得に失敗");
			return hr;
		}
		vaStartMin.ChangeType(VT_I4);
		aAccessStore.eStartMin = vaStartMin.lVal;
		
		// 営業終了時間
		CComVariant vaEndHour;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::END_HOUR, &vaEndHour)))
		{
			PrintError(cpAccessPoint, "営業終了時間の取得に失敗");
			return hr;
		}
		vaEndHour.ChangeType(VT_I4);
		aAccessStore.eEndHour = vaEndHour.lVal;
		
		// 営業終了分
		CComVariant vaEndMin;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::END_MIN, &vaEndMin)))
		{
			PrintError(cpAccessPoint, "営業終了分の取得に失敗");
			return hr;
		}
		vaEndMin.ChangeType(VT_I4);
		aAccessStore.eEndMin = vaEndMin.lVal;
		
		// 日曜日営業フラグ
		CComVariant vaSunday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::SUNDAY_F, &vaSunday)))
		{
			PrintError(cpAccessPoint, "日曜日営業フラグの取得に失敗");
			return hr;
		}
		vaSunday.ChangeType(VT_BOOL);
		aAccessStore.eSunday = vaSunday.boolVal ? true : false;
		
		// 月曜日営業フラグ
		CComVariant vaMonday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::MONDAY_F, &vaMonday)))
		{
			PrintError(cpAccessPoint, "月曜日営業フラグの取得に失敗");
			return hr;
		}
		vaMonday.ChangeType(VT_BOOL);
		aAccessStore.eMonday = vaMonday.boolVal ? true : false;
		
		// 火曜日営業フラグ
		CComVariant vaTuesday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::TUESDAY_F, &vaTuesday)))
		{
			PrintError(cpAccessPoint, "火曜日営業フラグの取得に失敗");
			return hr;
		}
		vaTuesday.ChangeType(VT_BOOL);
		aAccessStore.eTuesday = vaTuesday.boolVal ? true : false;
		
		// 水曜日営業フラグ
		CComVariant vaWednesday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::WEDNESDAY_F, &vaWednesday)))
		{
			PrintError(cpAccessPoint, "水曜日営業フラグの取得に失敗");
			return hr;
		}
		vaWednesday.ChangeType(VT_BOOL);
		aAccessStore.eWednesday = vaWednesday.boolVal ? true : false;
		
		// 木曜日営業フラグ
		CComVariant vaThursday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::THURSDAY_F, &vaThursday)))
		{
			PrintError(cpAccessPoint, "木曜日営業フラグの取得に失敗");
			return hr;
		}
		vaThursday.ChangeType(VT_BOOL);
		aAccessStore.eThursday = vaThursday.boolVal ? true : false;
		
		// 金曜日営業フラグ
		CComVariant vaFriday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::FRIDAY_F, &vaFriday)))
		{
			PrintError(cpAccessPoint, "金曜日営業フラグの取得に失敗");
			return hr;
		}
		vaFriday.ChangeType(VT_BOOL);
		aAccessStore.eFriday = vaFriday.boolVal ? true : false;
		
		// 土曜日営業フラグ
		CComVariant vaSaturday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::SATURDAY_F, &vaSaturday)))
		{
			PrintError(cpAccessPoint, "土曜日営業フラグの取得に失敗");
			return hr;
		}
		vaSaturday.ChangeType(VT_BOOL);
		aAccessStore.eSaturday = vaSaturday.boolVal ? true : false;
		
		// お盆営業フラグ
		CComVariant vaBon;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::BON_F, &vaBon)))
		{
			PrintError(cpAccessPoint, "お盆営業フラグの取得に失敗");
			return hr;
		}
		vaBon.ChangeType(VT_BOOL);
		aAccessStore.eBon = vaBon.boolVal ? true : false;
		
		// 年末営業フラグ
		CComVariant vaYearEnd;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::YEAREND_F, &vaYearEnd)))
		{
			PrintError(cpAccessPoint, "年末営業フラグの取得に失敗");
			return hr;
		}
		vaYearEnd.ChangeType(VT_BOOL);
		aAccessStore.eYearEnd = vaYearEnd.boolVal ? true : false;
		
		// 年始営業フラグ
		CComVariant vaNewYear;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::NEWYEAR_F, &vaNewYear)))
		{
			PrintError(cpAccessPoint, "年始営業フラグの取得に失敗");
			return hr;
		}
		vaNewYear.ChangeType(VT_BOOL);
		aAccessStore.eNewYear = vaNewYear.boolVal ? true : false;
		
		// ゴールデンウィーク営業フラグ
		CComVariant vaGoldenWeek;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::GOLDENWEEK_F, &vaGoldenWeek)))
		{
			PrintError(cpAccessPoint, "ゴールデンウィーク営業フラグの取得に失敗");
			return hr;
		}
		vaGoldenWeek.ChangeType(VT_BOOL);
		aAccessStore.eGoldenWeek = vaGoldenWeek.boolVal ? true : false;
		
		// 祝祭日営業フラグ
		CComVariant vaHoliday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::HOLIDAY_F, &vaHoliday)))
		{
			PrintError(cpAccessPoint, "祝祭日営業フラグの取得に失敗");
			return hr;
		}
		vaHoliday.ChangeType(VT_BOOL);
		aAccessStore.eHoliday = vaHoliday.boolVal ? true : false;
		
		// 店舗種別コード
		CComVariant vaStoreType;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::STORETYPE_C, &vaStoreType)))
		{
			PrintError(cpAccessPoint, "店舗種別コードの取得に失敗");
			return hr;
		}
		vaStoreType.ChangeType(VT_I4);
		switch (vaStoreType.lVal)
		{
		case sindy::access_store::storetype_gs:
			aAccessStore.eStoreType = gs;
			break;
		case sindy::access_store::storetype_store:
			aAccessStore.eStoreType = multiple_store;
			break;
		default:
			ATLASSERT(0);
		}
		
		cpAccessStoreList->push_back(aAccessStore);
	}
	
	return S_OK;
}

