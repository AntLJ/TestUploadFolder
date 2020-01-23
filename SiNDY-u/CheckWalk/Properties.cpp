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
	NONE = 0,   //!< �G���[�͂Ȃ�
	TIME_ONLY,  //!< ���t�����w�肳��Ă���
	ISNOT_DATE, //!< ���t�ɕϊ��ł��Ȃ���������w�肳��Ă���
};

inline bool isSuccess(ERR_TYPE err)
{
	return NONE==err;
}

/**
 * @brief �ݒ�t�@�C���̎w��l����A�ҏW�����t�B���^�̓��t�����������
 * @param dayStr  [in] �ҏW�����t�B���^�̓��t�����̎w��l
 * @param timeStr [in] �ҏW�����t�B���^�̎��ԕ����̎w��l
 * @return ���t������("YYYY/MM/DD HH:MM:SS")
 */
inline CString makeModifyString(const CString& dayStr, const CString& timeStr)
{
	if(dayStr.IsEmpty()) return _T("");

	CString date = dayStr;
	// ���Ԏw�肪�Ȃ����00:00:00���w�肳�ꂽ���̂Ƃ��Ĉ���
	if(timeStr.IsEmpty())
		date.Append(_T(" 00:00:00"));
	else
		date.AppendFormat(_T(" %s"), timeStr);
	return date;
}

/**
 * @brief �ҏW�����t�B���^���������w�肳��Ă��邩����
 * @param dayStr  [in] �ҏW�����t�B���^�̓��t�����̎w��l
 * @param timeStr [in] �ҏW�����t�B���^�̎��ԕ����̎w��l
 * @retval NONE     �w��͐�����
 * @retval NONE�ȊO �w��Ɍ�肪���݂���
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
		std::cerr << "�ݒ�t�@�C���ɉ����L�ڂ���Ă��܂���" << std::endl;
		return false;
	}

	if( ! CheckValue(dataType))
		return false;

	CString dayStr = GetValue(properties::day);
	CString timeStr = GetValue(properties::time);
	switch(checkModifyFilter(dayStr, timeStr))
	{
	case TIME_ONLY:
		std::cerr << "time������ݒ肷�邱�Ƃ͂ł��܂���" << std::endl;
		return false;
		break;
	case ISNOT_DATE:
		std::cerr << "day, time�̎w�肪����Ă��܂�(day=YYYY/MM/DD time=HH:MM:SS)"
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
	// �ҏW�����t�B���^�쐬
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
	// DB���ݒ肳��Ă��邩�m�F
	result &= CheckStringValue(properties::stationDB);
	result &= CheckStringValue(properties::roadDB);
	result &= CheckStringValue(properties::walkDB);
	result &= CheckStringValue(properties::meshDB);
	result &= CheckStringValue(properties::poiDB);

	// �O���t�@�C�����ݒ肳��Ă��邩�m�F
	result &= CheckFileValue(properties::meshList);
	result &= CheckFileValue(properties::heightNode_ini);

	// 臒l���ݒ肳��Ă��邩�m�F
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

	// �t�@�C���̑��݃`�F�b�N
	CString file = GetValue(propValue);
	if( ! uh::isFile( file )){
		OutputStdErr(propValue, _T("�w�肳�ꂽ�t�@�C�������݂��܂���->") + file);
		return false;
	}
	return true;
}

bool CProperties::CheckDirValue(const properties::value propValue)
{
	if( ! CheckStringValue(propValue) )
		return false;

	// �f�B���N�g���̑��݃`�F�b�N
	CString dir = GetValue(propValue);
	if( ! uh::isDirectory( dir )){
		OutputStdErr(propValue, _T("�w�肳�ꂽ�f�B���N�g�������݂��܂���->") + dir);
		return false;
	}
	return true;
}



bool CProperties::CheckStringValue(const properties::value propValue)
{
	// �󕶎��`�F�b�N
	CString str = GetValue(propValue);
	if( str.IsEmpty() ){
		OutputStdErr(propValue, "�l���w�肳��Ă��܂���");
		return false;
	}
	return true;
}

bool CProperties::CheckTargetValue(const properties::value propValue)
{
	properties::check_target target = GetValueTarget(propValue);
	if( target == properties::error ){
		OutputStdErr(propValue, "�K��O�̃`�F�b�N�Ώۂł�");
		return false;
	}
	return true;
}

void CProperties::OutputStdErr(const properties::value propValue, const CString message)
{
	std::cerr << GetTag(propValue) << ":\t " << CT2CA(message) << std::endl;
}
