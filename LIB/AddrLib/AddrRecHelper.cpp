#include "stdafx.h"
#include "AddrRecHelper.h"
#include "AddrRec.h"

#include <TDC/useful_headers/str_util.h>

#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsMisc.h>
#include <boost/assign.hpp>

namespace addr
{

using namespace cv_schema;
using namespace sindy;
using namespace sindy::schema;
using namespace AddrRecHelperEnum;

bool AddrRecHelper::makeAddrNames(AddrRec& record)
{
	bool ret = true;

	// 地番名称生成
	ret &= makeChibanName(record);

	// 号名称生成
	ret &= makeGouName(record);

	return ret;
}

bool AddrRecHelper::makeChibanName(AddrRec& record)
{
	// 地番コードがない場合、名称の生成はスキップ
	if( record.getChibancode().IsEmpty() )
		return true;

	// 文字付きではない場合
	if(!record.isChibanChar())
	{
		// 地番コードを数値化したものをセット
		long code = _tstol(record.getChibancode());
		record.setChibanName(uh::str_util::ToString(code));
		return true;
	}

	// 文字付きの場合
	uh::tstring sql = uh::str_util::format
						(
							_T("select %s from %s where %s = '%s' and %s = '%s'"), 
							pnt_conv::kPntName,
							pnt_conv::kTableName,
							pnt_conv::kAddrCode, record.getAdrcode11(), 
							pnt_conv::kPntCode, record.getChibancode()
						);

	CString resultName;
		
	// 文字付き地番名称を取得
	if( getName(sql, resultName) )
	{
		record.setChibanName(resultName);
		return true;
	}

	return false;
}

bool AddrRecHelper::makeGouName(AddrRec& record)
{
	// 号コードがない場合、名称の生成はスキップ
	if( record.getGoucode().IsEmpty() )
		return true;

	// 文字付きではない場合（単純に全角化する）
	if( !record.isGouChar() )
	{
		long code = _tstol(record.getGoucode());
		record.setGouName(uh::str_util::ToString(code));
		return true;
	}

	// 文字付きの場合
	uh::tstring sql = uh::str_util::format
						(
							_T("select %s from %s where %s = '%s' and %s = '%s'"), 
							gou_conv::kGouName,
							gou_conv::kTableName,
							gou_conv::kAddrCode, record.getAdrcode11(), 
							gou_conv::kGouCode, record.getGoucode()
						);

	CString resultName;
	std::vector<AttributeValue> attrValues = boost::assign::pair_list_of
			( gou_conv::kAddrCode, record.getAdrcode11() )
			( gou_conv::kGouCode, record.getGoucode() );
		
	// 文字付き号名称を取得
	if( getName(sql, resultName) )
	{
		record.setGouName(resultName);
		return true;
	}

	return false;
}

bool AddrRecHelper::getName(
						const uh::tstring& sql,
						CString& resultName)
{
	// まずは文字付き住所レコード変換テーブルを検索
	if( searchStrValue(m_charConvAdo, CT2CA(sql.c_str()), resultName) )
		return true;

	// なければ文字付き住所コード変換仮テーブルを検索
	if( searchStrValue(m_charConvTmpAdo, CT2CA(sql.c_str()), resultName) )
		return true;

	return false;
}

void AddrRecHelper::getCityAdminRecs(
						CTableContainer& cityAdminT,
						const CString& addrcode11,
						AddrRecPairs& caAddrRecPairs)
{
	// 都市地図行政界ポリゴンから11桁コードが一致するオブジェクトを取得
	auto query = AheInitQueryFilter
					(
						nullptr,
						nullptr,
						_T("%s = '%s' and (%s = '%s' or %s = '%s')"),
						city_admin::kCityCode, addrcode11.Mid(0, 5),
						city_admin::kAddrCode, addrcode11.Mid(5, 6),
						city_admin::kAddrCode2, addrcode11.Mid(5, 6)
						);

	cityAdminT._Select(query, true);

	for(const auto& ca : cityAdminT)
	{
		const auto& row = *CAST_ROWC(ca);

		// 住所レコードを生成
		AddrRec adrRec(addrcode11, row.CRowBase::GetShapeCopy());
		
		// 地番名称を設定
		// city_admin上
		//   拡張街区符号に値がある場合  ：拡張街区符号
		//   街区符号に値がある場合      ：街区符号
		//   それ以外（両方値がない場合）：空
		CString gaikuFugo = row.GetValue(city_admin::kGaikuFugo, CString()) ;
		CString extGaikuFugo = row.GetValue(city_admin::kExtGaikuFugo, CString()) ;
		CString chibanName = ( !gaikuFugo.IsEmpty() ) ? gaikuFugo : extGaikuFugo;
		int addrClass = row.GetValue(city_admin::kAddrClass, 0L);
	
		// 通常住所、文字付き住所それぞれで、レコード生成対象のcity_adminかを判定
		bool isTgtAreaByNormal = isMakeNormalAddrRec(addrClass, gaikuFugo);
		bool isTgtAreaByChar = isMakeCharAddrRec(addrClass, gaikuFugo, extGaikuFugo, _T("0"), false);
		
		// 通常・文字付き住所いずれもレコード対象ではない場合、falseを設定
		adrRec.setIsMatchTarget( isTgtAreaByNormal || isTgtAreaByChar );

		adrRec.setChibanName(chibanName);

		caAddrRecPairs.emplace_back(std::make_pair(adrRec, row));
	}
}

void AddrRecHelper::makeAdrRecordsFromGp(
						CTableContainer& gouPointT,
						const AddrRecPairs& caAdrRecPairs,
						AddrRecs& gpAdrRecs)
{
	for (const auto& caAdrRecPair : caAdrRecPairs)
	{
		makeAdrRecordsFromGp(gouPointT, caAdrRecPair.first.getAdrcode11(), caAdrRecPair.second, gpAdrRecs);
	}
}

void AddrRecHelper::makeAdrRecordsFromGp(
						CTableContainer& gouPointT,
						const CString& addrcode11,
						const CRowContainer& caRow,
						AddrRecs& gpAdrRecs)
{
	// 処理対象の都市地図行政界ポリゴンの形状に接触する号ポイントを取得
	auto query = AheInitSpatialFilter( nullptr, caRow.CRowBase::GetShapeCopy());
	auto query2 = AheInitQueryFilter
			(
				(IQueryFilterPtr)AheGetClone(query),
				nullptr
			);
	gouPointT._Select(query2, false);

	for(const auto& gp : gouPointT)
	{
		const auto& row = *CAST_ROWC(gp);

		// 号ポイントから住所レコード（AdrRecord）群を生成
		makeAdrRecordsFromGp(caRow, row, addrcode11, gpAdrRecs);
	}

	// 検索結果をクリア
	gouPointT.clear();
}

void AddrRecHelper::makeAdrRecordsFromGp(
						const CRowContainer& caRow,
						const CRowContainer& gpRow,
						const CString& addrcode11,
						AddrRecs& gpAdrRecs)
{
	bool ret = true;

	// 号ポイントから必要な属性値を取得（号番号取得時、[#]は取り除く）
	CString gouNo = gpRow.GetValue(gou_point::kGouNo, CString());
	bool expGouNo = (1 == gpRow.GetValue(gou_point::kExpGouNo, 0L)) ? true : false;
	CString gaikuFugo = caRow.GetValue(city_admin::kGaikuFugo, CString());
	CString extGaikuFugo = caRow.GetValue(city_admin::kExtGaikuFugo, CString());
	IGeometryPtr geo = gpRow.CRowBase::GetShapeCopy();
	int addrClass = caRow.GetValue(city_admin::kAddrClass, 0L);

	// 号番号が「*」の場合は、処理しない
	if( _T("*") == gouNo )
		return;

	// 通常住所生成対象の場合、通常住所を生成
	if( isMakeNormalAddrRec(addrClass, gaikuFugo) )
		makeNormalAdrRecordFromGp(gpAdrRecs, addrcode11, gaikuFugo, gouNo, expGouNo, geo);

	// 文字付き住所生成対象の場合、文字付き住所を生成
	if( isMakeCharAddrRec(addrClass, gaikuFugo, extGaikuFugo, gouNo, expGouNo) )
		makeCharAdrRecordFromGp(gpAdrRecs, addrcode11, extGaikuFugo, gaikuFugo, gouNo, expGouNo, geo);

	return;
}

bool AddrRecHelper::isMakeNormalAddrRec(int addrClass, const CString& gaikuFugo)
{
	// 住所整備種別が「1」「2」であるのに、街区符号に値がない場合、通常住所は生成しない
	if( gaikuFugo.IsEmpty() &&
		( city_admin::addr_class::kChibanMaintenance == addrClass ||
		  city_admin::addr_class::kResidenceIndicationMaintenance == addrClass ) )
		  return false;

	return true;
}

bool AddrRecHelper::isMakeCharAddrRec(
						int addrClass,
						const CString& gaikuFugo,
						const CString& extGaikuFugo, 
						const CString& gouNo,
						bool expGouNo)
{
	// 文字付き住所ではない場合、生成しない
	if( AddrStatus::kNormal == getAddrStatus(gaikuFugo, extGaikuFugo, gouNo, expGouNo) )
		return false;

	// city_adminの住所整備種別が「0」「2」の場合、文字付き住所は生成しない
	if( addrClass == city_admin::addr_class::kAzaUndeveloped ||
		addrClass == city_admin::addr_class::kChibanMaintenance )
		return false;

	return true;
}

void AddrRecHelper::makeCharAdrRecordFromGp(
						AddrRecs& addrRecs,
						const CString& addrcode,
						const CString& extGaikuFugo,
						const CString& gaikuFugo,
						const CString& gouNo,
						bool expGouNo,
						IGeometryPtr& geo)
{
	// CharAddrConvert.exeのCMakeAddrRec::MakeGouRecordByDBを参考に作成

	AddrRec rec(addrcode, geo);
	// 号番号内の[#]を除去して処理を行う
	CString gouNoRmShp = uh::str_util::replace(gouNo, _T("#"), _T(""));

	// city_adminの拡張街区符号が空ではなく、gou_pointの拡張号番号フラグがfalse(0)の場合
	if( !expGouNo && !extGaikuFugo.IsEmpty() )
	{
		// 地番名称：city_adminの拡張街区符号
		// 号名称　：gou_pointの号番号
		rec.setAddrNames(extGaikuFugo, gouNoRmShp);
	}
	else
	{
		// city_adminの街区符号に値が設定されていて、gou_pointの拡張号番号フラグがfalse(0)の場合
		if( !expGouNo && !gaikuFugo.IsEmpty() )
		{
			// 地番名称：city_adminの街区符号
			// 号名称　：gou_pointの号番号
			rec.setAddrNames(gaikuFugo, gouNoRmShp);
		}
		else // それ以外の場合
		{
			auto& splitGouNo = uh::str_util::split(gouNoRmShp, SEP_HYPHEN, true);

			// 地番名称
			// gou_pointの号番号の値をハイフン区切った場合、
			//   2つ以上に区切れる：分割後の1要素目
			//   区切れない　　　 ：gou_pointの号番号
			const CString strChibanName = ( 1 == splitGouNo.size() ) ? gouNoRmShp : splitGouNo[0]; 

			// 号名称
			// gou_pointの号番号の値をハイフン区切った場合、
			//   2つ以上に区切れる：gou_pointの号番号の1つ目のハイフンより後ろの文字全て
			//   区切れない       ：空
			CString strGouName;
			if( 1 != splitGouNo.size() )
			{
				splitGouNo.erase(splitGouNo.begin());
				strGouName = uh::str_util::join(splitGouNo, SEP_HYPHEN);
			}

			rec.setAddrNames(strChibanName, strGouName);
		}
	}
	addrRecs.emplace_back(rec);
}

void AddrRecHelper::makeNormalAdrRecordFromGp(
						AddrRecs& addrRecs,
						const CString& addrcode,
						const CString& gaikuFugo,
						const CString& gouNo,
						bool expGouNo,
						IGeometryPtr& geo)
{
	// CreateAddrWorkTxt.exeのCGP2WorkTxt::WriteDataを参考に作成

	// 号番号が、空か「*」の場合は生成しない
	if( gouNo.IsEmpty() || _T("*") == gouNo )
		return;
	
	// AddrLibのCGouPoint::GetAddrCode_NoCharからロジックをコピー
	// TODO:AddrLibと共通化出来るようにする

	// 号番号を分離(上位２つ)
	long lGouNo[2] = {0};
	CString strTmpGouNo( gouNo );

	// 数値orハイフンが含まれない場合は、生成しない
	int nPos = strTmpGouNo.FindOneOf( _T("0123456789-") );
	if( nPos == -1 )
		return;

	strTmpGouNo = strTmpGouNo.Mid( nPos );
	int nHaihunPos = strTmpGouNo.Find( _T("-") );
	if( 0 != nHaihunPos )
	{
		lGouNo[0] = _tstol( strTmpGouNo );
		if( nHaihunPos > 0 )
		{
			strTmpGouNo = strTmpGouNo.Mid( nHaihunPos );
			nPos = strTmpGouNo.FindOneOf( _T("0123456789") );
			if( nPos >= 0 )
				lGouNo[1] = _tstol( strTmpGouNo.Mid(nPos) );
		}
	}
	else
		return;

	AddrRec rec(addrcode, geo);
	CString gouNoFirst = (lGouNo[0] > 0) ? uh::str_util::ToString(lGouNo[0]) : _T("");
	CString gouNoSecond = (lGouNo[1] > 0) ? uh::str_util::ToString(lGouNo[1]) : _T("");

	bool isAddRec = true;
	//地番コード/号コードの桁数以上のものが存在する場合、リストには追加しない
	if( !gaikuFugo.IsEmpty() && !expGouNo && _tstol(gouNoFirst) <= 9999 )
		rec.setAddrNames(gaikuFugo, gouNoFirst);
	else if( !gouNoFirst.IsEmpty() && _tstol(gouNoFirst) <= 99999 && _tstol(gouNoSecond) <= 9999)
		rec.setAddrNames(gouNoFirst, gouNoSecond);
	else // これら以外の場合、住所レコードリストには追加しない
		isAddRec = false;

	if( isAddRec )
		addrRecs.emplace_back(rec);
}

void AddrRecHelper::makeAdrRecordsFromCs(
						CTableContainer& csPointT,
						const CString& whereClause,
						std::map<CString, AddrRecs>& addrRecs)
{
	// 11桁コードと合致する都市地図行政界ポリゴンを取得
	auto query = AheInitQueryFilter
					(
						nullptr,
						nullptr,
						( !whereClause.IsEmpty() ) ? whereClause : _T("")
					);

	csPointT._Select(query, true);

	for(const auto& cs : csPointT)
	{
		const auto& row = *CAST_ROWC(cs);

		AddrRec addrRec(row.GetValue(cs_addr_point::kAddrCode, CString()), row.CRowBase::GetShapeCopy());

		// 地番名称+号名称を取得
		CString chibanName = ( !addrRec.getChibancode().IsEmpty() ) ? 
								uh::str_util::ToString(_tstol(addrRec.getChibancode())) : _T("");
		CString gouName = ( !addrRec.getGoucode().IsEmpty() ) ? 
								uh::str_util::ToString(_tstol(addrRec.getGoucode())) : _T("");

		addrRec.setAddrNames(chibanName, gouName);
		addrRecs[addrRec.getAdrcode11()].emplace_back(addrRec);
	}

	// 検索結果をクリア
	csPointT.clear();
}

AddrStatus AddrRecHelper::getAddrStatus(
							const CString& gaikuFugo,
							const CString& extGaikuFugo, 
							const CString& gouNo,
							bool expGouNo)
{
	// 号番号の先頭[#]なら文字付き
	if( 0 == gouNo.Find( _T('#') ) )
		return ( !expGouNo && !extGaikuFugo.IsEmpty() ) ? kMojiGou_ExtGaiku : kMojiGou;

	const auto& splitGouNo = uh::str_util::split(gouNo, SEP_HYPHEN, true);
	const long lGouNoFirst = ( splitGouNo.size() > 0 && !splitGouNo[0].IsEmpty() ) ? _tstol(splitGouNo[0]) : 0;
	const long lGouNoSecond = ( splitGouNo.size() > 1 && !splitGouNo[1].IsEmpty() ) ? _tstol(splitGouNo[1]) : 0;

	if( !expGouNo && !extGaikuFugo.IsEmpty() )	// 拡張街区符号あり
	{
		// 区切りが2つ以上なら文字付き号
		if( splitGouNo.size() > 1 )
			return kSepOver_ExtGaiku;

		// 号が10,000以上なら文字付き号
		return ( lGouNoFirst > 9999 ) ? kNoOver_ExtGaiku : kExtGaiku;
	}

	// ・街区符号あり、かつ、拡張号ではなく、号番号の区切り（半角ハイフン）が2つ以上
	// ・拡張号で、かつ号番号の区切りが3つ以上
	// いずれかの場合、文字付きとして扱う
	long lGaikuFugo = (gaikuFugo.IsEmpty() ) ? 0 : _tstol(gaikuFugo);
	if( (!expGouNo && lGaikuFugo > 0 && splitGouNo.size() > 1) || (expGouNo && splitGouNo.size() > 2) )
		return kSepOver;

	// 街区符号なしで区切りが3つ以上なら文字付きとして扱う
	if( lGaikuFugo == 0 && splitGouNo.size() > 2 )
		return kSepOver;

	// 最後に地番が100,000以上 号が10,000以上なら文字付きとして扱う
	if( !expGouNo && lGaikuFugo > 0 )	// 街区符号あり
		return ( _tstol(splitGouNo[0]) > 9999 ) ? kNoOver : kNormal;

	return ( (lGouNoFirst > 99999) || (splitGouNo.size() > 1 && lGouNoSecond > 9999) )
				? kNoOver : kNormal;
}

bool AddrRecHelper::connectCharConvFile(const CString& charConvFile)
{
	return openDb(m_charConvAdo, charConvFile);
}

bool AddrRecHelper::connectCharConvTmpFile(const CString& charConvTmpFile)
{
	return openDb(m_charConvTmpAdo, charConvTmpFile);
}

bool AddrRecHelper::openDb(CADOBase& ado, const CString& file)
{
	// ADO使用
	try
	{
		ado.disconnect();

		_ConnectionPtr ipConn( _uuidof(Connection) );

		CString strCon;
		strCon.Format( _T("Driver={Microsoft Access Driver (*.mdb)}; DBQ=%s;"), file );
		if( SUCCEEDED(ipConn->Open(_bstr_t(strCon), _T(""), _T(""), adConnectUnspecified)) )
		{
			ado.attach( ipConn );
			return true;
		}
		else
		{
			return false; // 接続失敗
		}
	}
	catch( _com_error& e )
	{
		return false; // 接続失敗
	}
}

bool AddrRecHelper::searchStrValue(const CADOBase& ado, const LPCSTR& sqlStr, CString& value)
{
	bool ret = true;

	CADOCursor cCur( ado.search(sqlStr));

	if( S_OK != cCur.err() )
		return false;

	if( cCur.begin() == cCur.end() )
		return false;

	CComVariant vaValue = (*cCur.begin())[0];
	value = SUCCEEDED(vaValue.ChangeType(VT_BSTR)) ? CString(vaValue.bstrVal) : _T("");
	return true;
}

} 	// namespace addr