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

/**
 *	SINDY2INCODE.cpp
 *	@author	Fusatoshi Abe
 *	@date	2003/09/30		新規作成
 *	@note	SINDY2INCODE::Convert()使用のため、SINDY2INROADからコピー
 */

#include "StdAfx.h"
#include "SINDY2INCODE.h"
#include "GDBError.h"

SINDY2INCODE::Code SINDY2INCODE::road_manager[] = {
	{sindy::road_link::roadmanager_unknown,		in::road::manager::unknown},
	{sindy::road_link::roadmanager_jh,			in::road::manager::jh},
	{sindy::road_link::roadmanager_public3,		in::road::manager::public3},
	{sindy::road_link::roadmanager_public,		in::road::manager::other_public},
	{sindy::road_link::roadmanager_national,	in::road::manager::country},
	{sindy::road_link::roadmanager_pref,		in::road::manager::pref},
	{sindy::road_link::roadmanager_city,		in::road::manager::city},
	{sindy::road_link::roadmanager_town,		in::road::manager::other_city},
	{sindy::road_link::roadmanager_other,		in::road::manager::others},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::link_class[] = {
	{sindy::road_link::linkclass_unknown,		in::road::link_class::unknown},
	{sindy::road_link::linkclass_hibunri,		in::road::link_class::hibunri_honsen},
	{sindy::road_link::linkclass_bunri,			in::road::link_class::bunri_honsen},
	{sindy::road_link::linkclass_junction,		in::road::link_class::junction},
	{sindy::road_link::linkclass_crossing,		in::road::link_class::crossing},
	{sindy::road_link::linkclass_ramp,			in::road::link_class::ramp},
	{sindy::road_link::linkclass_sidelink,		in::road::link_class::side},
	{sindy::road_link::linkclass_salink,		in::road::link_class::sa},
	{sindy::road_link::linkclass_bicycle,		in::road::link_class::bicycle},
	{sindy::road_link::linkclass_connect,		in::road::link_class::connection},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::bypass_flag[] = {
	{0,											in::road::bypass::invalid},
	{1,											in::road::bypass::valid},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::caronly_flag[] = {
	{0,											in::road::caronly::unknown},
	{1,											in::road::caronly::caronly},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::road_width[] = {
	{sindy::road_link::roadwidth_unknown,		in::road::width::unknown},
	{sindy::road_link::roadwidth_over13m,		in::road::width::over_13m},
	{sindy::road_link::roadwidth_under13m,		in::road::width::under_13m},
	{sindy::road_link::roadwidth_over3m,		in::road::width::over_3m},
	{sindy::road_link::roadwidth_under3m,		in::road::width::under_3m},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::road_class[] = {
	{sindy::road_link::roadclass_kokudo,		in::road::road_class::kokudo},
	{sindy::road_link::roadclass_prefmlocal,	in::road::road_class::pref_mlocal},
	{sindy::road_link::roadclass_citymlocal,	in::road::road_class::city_mlocal},
	{sindy::road_link::roadclass_prefroad,		in::road::road_class::pref_road},
	{sindy::road_link::roadclass_cityroad,		in::road::road_class::city_road},
	{sindy::road_link::roadclass_otherroad,		in::road::road_class::other_road},
	{sindy::road_link::roadclass_nodisproad,	in::road::road_class::other_road3},
	{sindy::road_link::roadclass_detail1,		in::road::road_class::detail_road},
	{sindy::road_link::roadclass_detail2,		in::road::road_class::detail_road2},
	{sindy::road_link::roadclass_detail3,		in::road::road_class::detail_road3},
	{sindy::road_link::roadclass_highway,		in::road::road_class::highway},
	{sindy::road_link::roadclass_cityhighway,	in::road::road_class::city_highway},
	{sindy::road_link::roadclass_kokudo2,		in::road::road_class::kokudo2},
	{sindy::road_link::roadclass_prefmlocal2,	in::road::road_class::pref_mlocal2},
	{sindy::road_link::roadclass_citymlocal2,	in::road::road_class::city_mlocal2},
	{sindy::road_link::roadclass_prefroad2,		in::road::road_class::pref_road2},
	{sindy::road_link::roadclass_cityroad2,		in::road::road_class::city_road2},
	{sindy::road_link::roadclass_otherroad2,	in::road::road_class::other_road2},
	{sindy::road_link::roadclass_ferry,			in::road::road_class::ferry},
	{sindy::road_link::roadclass_ferry_s2,		in::road::road_class::ferry},
	{sindy::road_link::roadclass_ferry_s3,		in::road::road_class::ferry},
	{sindy::road_link::roadclass_ferry_s4,		in::road::road_class::ferry},
	{sindy::road_link::roadclass_virtual,		in::road::road_class::dummy_link},
	{sindy::road_link::roadclass_garden,		in::road::road_class::dummy_link2},
	{sindy::road_link::roadclass_bridge,		in::road::road_class::dummy_link3},
	{sindy::road_link::roadclass_entrance,		in::road::road_class::dummy_link3},
	{sindy::road_link::roadclass_facil,			in::road::road_class::detail_road},
	{sindy::road_link::roadclass_parking,		in::road::road_class::parking},
	{sindy::road_link::roadclass_match,			in::road::road_class::dummy_link4},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::navi_class[] = {
	{sindy::road_link::naviclass_kokudo,		in::road::navi_class::kokudo},
	{sindy::road_link::naviclass_prefmlocal,	in::road::navi_class::pref_mlocal},
	{sindy::road_link::naviclass_citymlocal,	in::road::navi_class::city_mlocal},
	{sindy::road_link::naviclass_prefroad,		in::road::navi_class::pref_road},
	{sindy::road_link::naviclass_cityroad,		in::road::navi_class::city_road},
	{sindy::road_link::naviclass_otherroad,		in::road::navi_class::other_road},
	{sindy::road_link::naviclass_highway,		in::road::navi_class::highway},
	{sindy::road_link::naviclass_cityhighway,	in::road::navi_class::city_highway},
	{sindy::road_link::naviclass_kokudo2,		in::road::navi_class::kokudo2},
	{sindy::road_link::naviclass_prefmlocal2,	in::road::navi_class::pref_mlocal2},
	{sindy::road_link::naviclass_citymlocal2,	in::road::navi_class::city_mlocal2},
	{sindy::road_link::naviclass_prefroad2,		in::road::navi_class::pref_road2},
	{sindy::road_link::naviclass_cityroad2,		in::road::navi_class::city_road2},
	{sindy::road_link::naviclass_otherroad2,	in::road::navi_class::other_road2},
	{sindy::road_link::naviclass_ferry,			in::road::navi_class::ferry},
	{sindy::road_link::naviclass_rampway,		in::road::navi_class::connectway},
	{sindy::road_link::naviclass_notnavi,		in::road::navi_class::detail_road},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::wide_area[] = {
	{sindy::road_link::widearea_kouiki0,		in::road::wide_area::not_wide_area},
	{sindy::road_link::widearea_kouiki1,		in::road::wide_area::wide_area1},
	{sindy::road_link::widearea_kouiki2,		in::road::wide_area::wide_area2},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::parking_class[] = {
	{sindy::road_link::parkingclass_unknown,	in::road::parking_class::unknown},
	{sindy::road_link::parkingclass_solid,		in::road::parking_class::solid},
	{sindy::road_link::parkingclass_under,		in::road::parking_class::under},
	{sindy::road_link::parkingclass_both,		in::road::parking_class::both},
	{sindy::road_link::parkingclass_tower,		in::road::parking_class::tower},
	{sindy::road_link::parkingclass_auto,		in::road::parking_class::auto_tower},
	{sindy::road_link::parkingclass_flat,		in::road::parking_class::flat},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::parkinglink_class[] = {
	{sindy::road_link::parkinglinkclass_unknown,	in::road::parkinglink_class::unknown},
	{sindy::road_link::parkinglinkclass_enter,		in::road::parkinglink_class::enter},
	{sindy::road_link::parkinglinkclass_exit,		in::road::parkinglink_class::exit},
	{sindy::road_link::parkinglinkclass_entrance,	in::road::parkinglink_class::entrance},
	{sindy::road_link::parkinglinkclass_premises,	in::road::parkinglink_class::premises},
	{sindy::road_link::parkinglinkclass_junction,	in::road::parkinglink_class::junction},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::vics_class[] = {
	{sindy::inf_vics::vicsclass_highway,		in::road::vics_class::highway},
	{sindy::inf_vics::vicsclass_cityhighway,	in::road::vics_class::city_highway},
	{sindy::inf_vics::vicsclass_street,			in::road::vics_class::street},
	{sindy::inf_vics::vicsclass_other,			in::road::vics_class::others},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::segment_attr[] = {
	{sindy::segment_attr::linkattr_tunnel,		in::road::segment_attr::tunnel},
	{sindy::segment_attr::linkattr_underpass,	in::road::segment_attr::underpass},
	{sindy::segment_attr::linkattr_bridge,		in::road::segment_attr::bridge},
	{sindy::segment_attr::linkattr_crossing,	in::road::segment_attr::crossing},
	{sindy::segment_attr::linkattr_doumon,		in::road::segment_attr::doumon},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::kubun_code[] = {
	{sindy::road_link::kubun_unknown,			in::road::kubun_code::unknown},
	{sindy::road_link::kubun_main,				in::road::kubun_code::main_line},
	{sindy::road_link::kubun_sub,				in::road::kubun_code::sub_line},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::limit_speed[] = {
	{sindy::road_link::limitspeed_unknown,		in::road::limit_speed::unknown},
	{sindy::road_link::limitspeed_under30km,	in::road::limit_speed::limit_under30km},
	{sindy::road_link::limitspeed_40km,			in::road::limit_speed::limit_40km},
	{sindy::road_link::limitspeed_50km,			in::road::limit_speed::limit_50km},
	{sindy::road_link::limitspeed_60km,			in::road::limit_speed::limit_60km},
	{sindy::road_link::limitspeed_70km,			in::road::limit_speed::limit_70km},
	{sindy::road_link::limitspeed_80km,			in::road::limit_speed::limit_80km},
	{sindy::road_link::limitspeed_100km,		in::road::limit_speed::limit_100km},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::area_flag[] = {
	{sindy::road_link::alleymainte_notavailable,	in::road::area_flag::unknown},
	{sindy::road_link::alleymainte_1998,			in::road::area_flag::year_1998},
	{sindy::road_link::alleymainte_1999,			in::road::area_flag::year_1999},
	{sindy::road_link::alleymainte_2000spring,		in::road::area_flag::year_2000_spr},
	{sindy::road_link::alleymainte_2000autumn,		in::road::area_flag::year_2000_aut},
	{sindy::road_link::alleymainte_2001,			in::road::area_flag::year_2001},
	{sindy::road_link::alleymainte_2002,			in::road::area_flag::year_2002},
	{sindy::road_link::alleymainte_2003,			in::road::area_flag::year_2003},
	{sindy::road_link::alleymainte_2004,			in::road::area_flag::year_2004},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::signal_flag[] = {
	{sindy::road_node::signal_no,				in::road::signal::invalid},
	{sindy::road_node::signal_exist,			in::road::signal::valid},
	{sindy::road_node::signal_unknown,			in::road::signal::invalid},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::height_flag[] = {
	{sindy::road_node::height_no,				in::road::height::invalid},
	{sindy::road_node::height_valid,			in::road::height::valid},
	{sindy::road_node::height_auto,				in::road::height::invalid},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::dayofweek[] = {
	{sindy::dayofweek::dayofweek_holiday,			in::road::dayofweek::holiday},
	{sindy::dayofweek::dayofweek_except_holiday,	in::road::dayofweek::except_holiday},
	{sindy::dayofweek::dayofweek_pubholiday,		in::road::dayofweek::public_holiday},
	{sindy::dayofweek::dayofweek_except_pubholiday,	in::road::dayofweek::except_public_holiday},
	{sindy::dayofweek::dayofweek_weekday,			in::road::dayofweek::weekday},
	{sindy::dayofweek::dayofweek_sunday,			in::road::dayofweek::sunday},
	{sindy::dayofweek::dayofweek_monday,			in::road::dayofweek::monday},
	{sindy::dayofweek::dayofweek_tuesday,			in::road::dayofweek::tuesday},
	{sindy::dayofweek::dayofweek_wednesday,			in::road::dayofweek::wednesday},
	{sindy::dayofweek::dayofweek_thursday,			in::road::dayofweek::thursday},
	{sindy::dayofweek::dayofweek_friday,			in::road::dayofweek::friday},
	{sindy::dayofweek::dayofweek_saturday,			in::road::dayofweek::saturday},
	{sindy::dayofweek::dayofweek_weekend,			in::road::dayofweek::weekend},
	{sindy::dayofweek::dayofweek_except_weekend,	in::road::dayofweek::except_weekend},
	{sindy::dayofweek::dayofweek_allday,			in::road::dayofweek::allday},
	{sindy::dayofweek::dayofweek_except_sunday,		in::road::dayofweek::except_sunday},
	{sindy::dayofweek::dayofweek_pre_holiday,		in::road::dayofweek::pre_holiday},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::dirguide_class[] = {
	{sindy::inf_dirguide::guideclass_normal,	in::road::dirguide_class::general},
	{sindy::inf_dirguide::guideclass_toll,		in::road::dirguide_class::highway},
	{sindy::inf_dirguide::guideclass_kokudo,	in::road::dirguide_class::kokudo_no},
	{sindy::inf_dirguide::guideclass_pref,		in::road::dirguide_class::kendo_no},
	{sindy::inf_dirguide::guideclass_other,		in::road::dirguide_class::others},
	{sindy::inf_dirguide::guideclass_tstring,	in::road::dirguide_class::others},
	{sindy::inf_dirguide::guideclass_tkokudo,	in::road::dirguide_class::others},
	{sindy::inf_dirguide::guideclass_tpref,		in::road::dirguide_class::others},
	{-1, -1}
};

SINDY2INCODE::Code SINDY2INCODE::guide_class[] = {
	{sindy::inf_guide::guidetype_noguide,		in::road::guide_class::noguide},
	{sindy::inf_guide::guidetype_straight,		in::road::guide_class::straight},
	{sindy::inf_guide::guidetype_rightfront,	in::road::guide_class::right_front},
	{sindy::inf_guide::guidetype_rightahead,	in::road::guide_class::right_ahead},
	{sindy::inf_guide::guidetype_right,			in::road::guide_class::right},
	{sindy::inf_guide::guidetype_rightbehind,	in::road::guide_class::right_behind},
	{sindy::inf_guide::guidetype_rightback,		in::road::guide_class::right_back},
	{sindy::inf_guide::guidetype_back,			in::road::guide_class::back},
	{sindy::inf_guide::guidetype_leftback,		in::road::guide_class::left_back},
	{sindy::inf_guide::guidetype_leftbehind,	in::road::guide_class::left_behind},
	{sindy::inf_guide::guidetype_left,			in::road::guide_class::left},
	{sindy::inf_guide::guidetype_leftahead,		in::road::guide_class::left_ahead},
	{sindy::inf_guide::guidetype_leftfront,		in::road::guide_class::left_front},
	{-1, -1}
};

HRESULT SINDY2INCODE::Convert(Code *cTable, long cSindyCode, long *cInCode)
{
	const char *func = "SINDY2INCODE::Convert";

	*cInCode = 0;

	for (long i = 0; (cTable+i)->sindy_code != -1; i++)
	{
		if (cSindyCode == (cTable+i)->sindy_code)
		{
			*cInCode = (cTable+i)->in_code;
			return S_OK;
		}
	}
	return GDBERROR2(E_UNEXPECTED, func);
}

HRESULT SINDY2INCODE::Convert2(Code *cTable, long cInCode, long *cSindyCode)
{
	const char *func = "SINDY2INCODE::Convert2";

	*cSindyCode = 0;

	for (long i = 0; (cTable+i)->sindy_code != -1; i++)
	{
		if (cInCode == (cTable+i)->in_code)
		{
			*cSindyCode = (cTable+i)->sindy_code;
			return S_OK;
		}
	}
	return GDBERROR2(E_UNEXPECTED, func);
}

void SINDY2INCODE::wcs2mbs(BSTR bstrVal, std::string *cName)
{
	USES_CONVERSION;
/*
	char buff[BUFSIZ];
	long s = wcstombs(buff, bstrVal, BUFSIZ);
	buff[s] = '\0';
	*cName = buff;
*/
	*cName = OLE2T(bstrVal);
	return;
}
/* CreateLinkPatchでは使用しないのでコメントアウト
std::string SINDY2INCODE::SJIS2EUC(const char *cSjisString)
{
	std::string aEucString = "";
	sjistoeuc(cSjisString, &aEucString);
	return aEucString;
}

std::string SINDY2INCODE::EUC2SJIS(const char *cEucString)
{
	std::string aSjisString = "";
	euctosjis(cEucString, &aSjisString);
	return aSjisString;
}
*/
