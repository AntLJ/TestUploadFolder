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
#include "Properties.h"
#include <boost/assign/list_of.hpp>

namespace
{

enum ERR_TYPE
{
	NONE = 0,   //!< エラーはない
	TIME_ONLY,  //!< 日付だけ指定されている
	ISNOT_DATE, //!< 日付に変換できない文字列を指定されている
};

inline bool isSuccess(ERR_TYPE err)
{
	return NONE==err;
}

/**
 * @brief 設定ファイルの指定値から、編集履歴フィルタの日付文字列を合成
 * @param dayStr  [in] 編集履歴フィルタの日付部分の指定値
 * @param timeStr [in] 編集履歴フィルタの時間部分の指定値
 * @return 日付文字列("YYYY/MM/DD HH:MM:SS")
 */
inline CString makeModifyString(const CString& dayStr, const CString& timeStr)
{
	if(dayStr.IsEmpty()) return _T("");

	CString date = dayStr;
	// 時間指定がなければ00:00:00が指定されたものとして扱う
	if(timeStr.IsEmpty())
		date.Append(_T(" 00:00:00"));
	else
		date.AppendFormat(_T(" %s"), timeStr);
	return date;
}

/**
 * @brief 編集履歴フィルタが正しく指定されているか検査
 * @param dayStr  [in] 編集履歴フィルタの日付部分の指定値
 * @param timeStr [in] 編集履歴フィルタの時間部分の指定値
 * @retval NONE     指定は正しい
 * @retval NONE以外 指定に誤りが存在する
 */
inline ERR_TYPE checkModifyFilter(const CString& dayStr, const CString& timeStr)
{
	if(dayStr.IsEmpty())
		return timeStr.IsEmpty()? NONE : TIME_ONLY;

	CComVariant checkDate(makeModifyString(dayStr, timeStr));
	if(FAILED(checkDate.ChangeType(VT_DATE)))
		return ISNOT_DATE;
	return NONE;
}

}

bool CProperties::Read(const CString& fileName, const CString& dataType)
{
	std::string strFileName = CT2CA(fileName);
	boost::property_tree::read_ini(strFileName, m_proper_tree);

	if(m_proper_tree.empty())
	{
		std::cerr << "設定ファイルに何も記載されていません" << std::endl;
		return false;
	}

	if( ! CheckValue(dataType))
		return false;

	CString dayStr = GetValue(properties::day);
	CString timeStr = GetValue(properties::time);
	switch(checkModifyFilter(dayStr, timeStr))
	{
	case TIME_ONLY:
		std::cerr << "timeだけを設定することはできません" << std::endl;
		return false;
		break;
	case ISNOT_DATE:
		std::cerr << "day, timeの指定が誤っています(day=YYYY/MM/DD time=HH:MM:SS)"
			<< std::endl;
		return false;
		break;
	default:
		break;
	}
	return true;
}

std::string CProperties::GetTag(const properties::value propValue) const
{
	switch(propValue)
	{
	case properties::target:    return "Check.target"; break;
	case properties::day:       return "Check.day"; break;
	case properties::time:      return "Check.time"; break;

	case properties::roadDB:	return "Database.road"; break;
	case properties::stationDB:	return "Database.station"; break;
	case properties::walkDB:	return "Database.walk"; break;
	case properties::meshDB:	return "Database.mesh"; break;
	case properties::poiDB:		return "Database.poi"; break;
	case properties::underDB:	return "Database.under";break;

	case properties::meshList:	return "File.mesh_list";break;
	case properties::heightNode_ini: return "File.height_node_ini";break;

	case properties::neighbor_threshold:	return "Threshold.neighbor"; break;
	case properties::refer_id_threshold:	return "Threshold.refer_id"; break;
	case properties::dist_threshold:		return "Threshold.dist"; break;
	case properties::angle_threshold:		return "Threshold.angle"; break;
	case properties::buddy_dist_threshold:	return "Threshold.buddy_dist"; break;
	case properties::buddy_angle_threshold:	return "Threshold.buddy_angle"; break;
	case properties::sindyTolerance_threshold:	return "Threshold.sindyTolerance"; break;
	case properties::microLogicalConnection:	return "Threshold.micro_LogicalConnection"; break;

	default:								return ""; break;
	}
}

CString CProperties::GetValue(const properties::value propValue) const 
{
	if(boost::optional<std::string> value = m_proper_tree.get_optional<std::string>(GetTag(propValue)))
		return value->c_str();
	return _T("");
}

double CProperties::GetValueDouble(const properties::value propValue) const
{
	if( boost::optional<double> vl = m_proper_tree.get_optional<double>(GetTag(propValue)) )
		return *vl;
	return 0.0;
}

properties::check_target CProperties::GetValueTarget(const properties::value propValue) const
{
	boost::optional<std::string> value = m_proper_tree.get_optional<std::string>(GetTag(propValue));
	if(!value) return properties::check_target::error;

	std::map<std::string, properties::check_target> tag2Id = boost::assign::map_list_of
		("all", properties::check_target::all )
		("humancar", properties::check_target::humancar )
		("walkable", properties::check_target::walkable )
		("height_node", properties::check_target::height_node )
		("link_relation", properties::check_target::link_relation )
		("under", properties::check_target::under )
		("other", properties::check_target::other )
		("release", properties::check_target::release )
		("pedxing", properties::check_target::pedxing )
		;

	auto it = tag2Id.find( value.get() );
	if(tag2Id.end()==it)
		return properties::check_target::error;
	return it->second;
}
CComVariant CProperties::getModifyFilter() const
{
	// 編集履歴フィルタ作成
	CString dayStr = GetValue(properties::day);
	CString timeStr = GetValue(properties::time);
	if(!isSuccess(checkModifyFilter(dayStr, timeStr)))
		return VT_NULL;

	CString modifyStr = makeModifyString(dayStr, timeStr);
	if(modifyStr.IsEmpty()) return VT_NULL;

	CComVariant date(modifyStr);
	date.ChangeType(VT_DATE);
	return date;
}

bool CProperties::CheckValue(const CString& dataType){
	using namespace sindy::schema;

	bool result = CheckTargetValue(properties::target);
	// DBが設定されているか確認
	result &= CheckStringValue(properties::stationDB);
	result &= CheckStringValue(properties::roadDB);
	result &= CheckStringValue(properties::walkDB);
	result &= CheckStringValue(properties::meshDB);
	result &= CheckStringValue(properties::poiDB);

	// 外部ファイルが設定されているか確認
	result &= CheckFileValue(properties::meshList);
	result &= CheckFileValue(properties::heightNode_ini);

	// 閾値が設定されているか確認
	result &= CheckStringValue(properties::dist_threshold);
	result &= CheckStringValue(properties::refer_id_threshold);
	result &= CheckStringValue(properties::neighbor_threshold);
	result &= CheckStringValue(properties::angle_threshold);
	result &= CheckStringValue(properties::buddy_dist_threshold);
	result &= CheckStringValue(properties::buddy_angle_threshold);

	if(_ttol(dataType) == walk_type::type_class::kDownConvert)
	{
		result &= CheckStringValue(properties::sindyTolerance_threshold);
		result &= CheckStringValue(properties::microLogicalConnection);
	}
	return result;
}

bool CProperties::CheckFileValue(const properties::value propValue)
{
	if( ! CheckStringValue(propValue) )
		return false;

	// ファイルの存在チェック
	CString file = GetValue(propValue);
	if( ! uh::isFile( file )){
		OutputStdErr(propValue, _T("指定されたファイルが存在しません->") + file);
		return false;
	}
	return true;
}

bool CProperties::CheckDirValue(const properties::value propValue)
{
	if( ! CheckStringValue(propValue) )
		return false;

	// ディレクトリの存在チェック
	CString dir = GetValue(propValue);
	if( ! uh::isDirectory( dir )){
		OutputStdErr(propValue, _T("指定されたディレクトリが存在しません->") + dir);
		return false;
	}
	return true;
}



bool CProperties::CheckStringValue(const properties::value propValue)
{
	// 空文字チェック
	CString str = GetValue(propValue);
	if( str.IsEmpty() ){
		OutputStdErr(propValue, "値が指定されていません");
		return false;
	}
	return true;
}

bool CProperties::CheckTargetValue(const properties::value propValue)
{
	properties::check_target target = GetValueTarget(propValue);
	if( target == properties::error ){
		OutputStdErr(propValue, "規定外のチェック対象です");
		return false;
	}
	return true;
}

void CProperties::OutputStdErr(const properties::value propValue, const CString message)
{
	std::cerr << GetTag(propValue) << ":\t " << CT2CA(message) << std::endl;
}
