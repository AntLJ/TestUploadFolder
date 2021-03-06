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
#include "SetRoadAttrList.h"
#include "INAttrInfo.h"
#include "ErrorInfo.h"

CSetRoadAttrList::CSetRoadAttrList(void)
{
}

CSetRoadAttrList::~CSetRoadAttrList(void)
{
}

//////////////////////////////////////////////////////////////////////
// SCf[^ÌîlðZbg
//////////////////////////////////////////////////////////////////////
bool CSetRoadAttrList::setList( CString& rPolDir, AttrList& rAttr )
{
	setINHead( rAttr.eINHead );
	if( !setLinkLayer( rAttr.eLinkLayer, rPolDir ) )
		return false;
	setNodeLayer( rAttr.eNodeLayer );
	setRegLayer( rAttr.eRegLayer );
	setNInfoLayer( rAttr.eNInfoLayer );
	setLaneLayer( rAttr.eLaneLayer );
	setLInfoLayer( rAttr.eLInfoLayer, rAttr.eLinkLayer );
	setLinkExLayer( rAttr.eLinkExLayer, rAttr.eLInfoLayer );

	return true;
}

//////////////////////////////////////////////////////////////////////
// INwb_îñîlðZbg
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setINHead(HeadAttrList& rINHead)
{
	// .mnt CIDi[
	rINHead.eMnt.insert( 0x00300000 );			///< NC
	rINHead.eMnt.insert( 0x00300800 );			///< m[hC(6.01)
	rINHead.eMnt.insert( 0x00300004 );			///< VðÊK§C

	// ,rdgCIDi[
	rINHead.eRdg.insert( 0x00300030 );			///< ð·_U±îñC
	rINHead.eRdg.insert( 0x00300040 );			///< m[hîñC
	rINHead.eRdg.insert( 0x00300080 );			///< [îñC
	rINHead.eRdg.insert( 0x00300200 );			///< NîñC
	rINHead.eRdg.insert( 0x00300400 );			///< Ng£C

	// G[R[hi[
	for( int iCnt = 0; iCnt < INHeadEnd + 1; ++iCnt )
		rINHead.eError[HeadError[iCnt].iCode] = HeadError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// NCf[^îlðZbg
//////////////////////////////////////////////////////////////////////
bool CSetRoadAttrList::setLinkLayer( LinkAttrList& rLinkAttr, CString& rPolDir )
{
	// ÇÒR[h
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::jh);				///< ¬¹HïÐ
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::other_public);	///< ¹HöÐ
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::country);		///< 
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::pref);			///< s¹{§
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::city);			///< wès
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::other_city);		///< ¼Ìs¬º iÜ QRæj
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::others);			///< »Ì¼ÌÇÒ
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::unknown);		///< ¢²¸EìÆ

	// NíÊR[h
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::hibunri_honsen);		///< {üiãºüñª£j
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::bunri_honsen);		///< {üiãºüª£j
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::junction);			///< AHi{üÔÌnèüj
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::crossing);			///< ð·_àN
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::ramp);				///< AHivj
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::side);				///< {üÆ¯êHüÌ¤¹N
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::sa);					///< SA¤üN
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::bicycle);			///< ©]Ô¹Ni©®ÔÊsÍsÂj
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::connection);			///< {ü¤¹Ú±N
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::roundabout);			///< Âóð·_N
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::unknown);			///< ¢²¸EìÆ

	//oCpXtO
	rLinkAttr.eLinkInfo.eBypass.insert(in::road::bypass::valid);			///< oCpXLè
	rLinkAttr.eLinkInfo.eBypass.insert(in::road::bypass::invalid);			///< oCpX³µ

	// ©®ÔêptO
	rLinkAttr.eLinkInfo.eAuto.insert(1);									///< ©®Ôêp¹H
	rLinkAttr.eLinkInfo.eAuto.insert(0);									///< ©®Ôêp¹HÈO

	// ¹Hõ
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::over_13m);			///< õ13mÈã
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::under_13m);			///< õ5.5m`13m¢
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::over_3m);			///< õ3.0m`5.5m¢
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::under_3m);			///< õ3.0m¢
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::unknown);			///< ¢²¸EìÆ

	// Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::one);					///< 1Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::two);					///< 2Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::three);				///< 3Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::four);					///< 4Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::five);					///< 5Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::six);					///< 6Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::seven);				///< 7Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::eight);				///< 8Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::nine);					///< 9Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::ten);					///< 10Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::eleven);				///< 11Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::twelve);				///< 12Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::thirteen);				///< 13Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::forteen);				///< 14Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::fifteen);				///< 15Ôü
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::unknown);				///< ¢²¸EìÆ

	// ðÊK§íÊR[h
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::no_rest);		///< K§³µ
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::no_passage);		///< ÊsÖ~(ðÈµ)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::no_passage2);	///< ÊsÖ~(ð è)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_order);	///< êûÊs(³ûüEðÈµ)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_reverse);	///< êûÊs(tûüEðÈµ)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_order2);	///< êûÊs(³ûüEð è)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_reverse2);///< êûÊs(tûüEð è)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_both);	///< êûÊs(³tØèÖ¦ è)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::unknown);		///< ¢²¸EìÆ

	// ðÊK§ðR[h
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::car);				///< ÔíÌÝ
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::time);			///< ÌÝ
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::day);				///< júÌÝ
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::car_and_time);	///< ÔíyÑ
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::car_and_day);		///< ÔíyÑjú
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::time_and_day);	///< yÑjú
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::all);				///< ÔíAyÑjú
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::others);			///< »Ì¼Ìð
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::unknown);			///< ¢²¸EìÆ

	// ¹H\¦íÊR[h
	rLinkAttr.eDispClass.insert(in::road::road_class::highway2);			///< ssÔ¬¹H
	rLinkAttr.eDispClass.insert(in::road::road_class::highway);				///< ssÔ¬¹H(L¿)
	rLinkAttr.eDispClass.insert(in::road::road_class::city_highway);		///< ss¬¹H(L¿)
	rLinkAttr.eDispClass.insert(in::road::road_class::kokudo);				///< êÊ¹
	rLinkAttr.eDispClass.insert(in::road::road_class::kokudo2);				///< êÊ¹(L¿)
	rLinkAttr.eDispClass.insert(in::road::road_class::pref_mlocal);			///< åvnû¹(s{§)
	rLinkAttr.eDispClass.insert(in::road::road_class::pref_mlocal2);		///< åvnû¹(s{§)(L¿)
	rLinkAttr.eDispClass.insert(in::road::road_class::city_mlocal);			///< åvnû¹(wès)
	rLinkAttr.eDispClass.insert(in::road::road_class::city_mlocal2);		///< åvnû¹(wès)(L¿)
	rLinkAttr.eDispClass.insert(in::road::road_class::pref_road);			///< êÊs¹{§¹
	rLinkAttr.eDispClass.insert(in::road::road_class::pref_road2);			///< êÊs¹{§¹(L¿)
	rLinkAttr.eDispClass.insert(in::road::road_class::city_road);			///< wèsÌêÊs¹
	rLinkAttr.eDispClass.insert(in::road::road_class::city_road2);			///< wèsÌêÊs¹(L¿)
	rLinkAttr.eDispClass.insert(in::road::road_class::other_road);			///< »Ì¼Ì¹H
	rLinkAttr.eDispClass.insert(in::road::road_class::other_road2);			///< »Ì¼Ì¹H(L¿)
	rLinkAttr.eDispClass.insert(in::road::road_class::other_road3);			///< iº°»Ì¼¹
	rLinkAttr.eDispClass.insert(in::road::road_class::detail_road);			///< ×XH1
	rLinkAttr.eDispClass.insert(in::road::road_class::detail_road2);		///< ×XH2
	rLinkAttr.eDispClass.insert(in::road::road_class::detail_road3);		///< ×XH3
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link);			///< _~[N
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link2);			///< ëH
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link3);			///< }b`OpN
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link4);			///< PECp}b`ON
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link5);			///< BTp}b`ON
	rLinkAttr.eDispClass.insert(in::road::road_class::ferry);				///< tF[qH
	rLinkAttr.eDispClass.insert(in::road::road_class::parking);				///< Ôêà¹H

	// ¹HoHíÊR[h
	rLinkAttr.eNaviClass.insert(in::road::navi_class::highway);				///< ssÔ¬¹H
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_highway);		///< ss¬¹H
	rLinkAttr.eNaviClass.insert(in::road::navi_class::kokudo);				///< êÊ¹
	rLinkAttr.eNaviClass.insert(in::road::navi_class::kokudo2);				///< êÊ¹(L¿)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::pref_mlocal);			///< åvnû¹(s{§)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::pref_mlocal2);		///< åvnû¹(s{§)(L¿)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_mlocal);			///< åvnû¹(wès)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_mlocal2);		///< åvnû¹(wès)(L¿)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::pref_road);			///< êÊs¹{§¹
	rLinkAttr.eNaviClass.insert(in::road::navi_class::pref_road2);			///< êÊs¹{§¹(L¿)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_road);			///< wèsÌêÊs¹
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_road2);			///< wèsÌêÊs¹(L¿)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::other_road);			///< »Ì¼Ì¹H
	rLinkAttr.eNaviClass.insert(in::road::navi_class::other_road2);			///< »Ì¼Ì¹H(L¿)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::other_road3);			///< iº°»Ì¼¹
	rLinkAttr.eNaviClass.insert(in::road::navi_class::detail_road);			///< ×XH1
	rLinkAttr.eNaviClass.insert(in::road::navi_class::detail_road2);		///< ×XH2
	rLinkAttr.eNaviClass.insert(in::road::navi_class::detail_road3);		///< ×XH3
	rLinkAttr.eNaviClass.insert(in::road::navi_class::connectway);			///< Ú±¹H
	rLinkAttr.eNaviClass.insert(in::road::navi_class::kkd_route);			///< KDoHU±sKæÔ
	rLinkAttr.eNaviClass.insert(in::road::navi_class::dummy_link3);			///< }b`OpN
	rLinkAttr.eNaviClass.insert(in::road::navi_class::dummy_link4);			///< PECp}b`ON
	rLinkAttr.eNaviClass.insert(in::road::navi_class::ferry);				///< tF[qH
	rLinkAttr.eNaviClass.insert(in::road::navi_class::parking);				///< Ôêà¹H
	rLinkAttr.eNaviClass.insert(in::road::navi_class::dummy_link5);			///< BTp}b`ON

	// LætO
#if IN_VERSION >= 590
	rLinkAttr.eAreaFlag.insert(in::road::wide_area::wide_area);				///< LæÉ¶Ý·é
	rLinkAttr.eAreaFlag.insert(in::road::wide_area::not_wide_area);			///< LæÉ¶ÝµÈ¢
#else
	rLinkAttr.eAreaFlag.insert(in::road::wide_area::wide_area1);			///< Læ1
	rLinkAttr.eAreaFlag.insert(in::road::wide_area::wide_area2);			///< Læ2
#endif

	// ÔêíÊ
	rLinkAttr.eParkClass.insert(in::road::parking_class::solid);			///< §ÌÔê
	rLinkAttr.eParkClass.insert(in::road::parking_class::under);			///< nºÔê
	rLinkAttr.eParkClass.insert(in::road::parking_class::both);				///< §Ì©ÂnºÔê
	rLinkAttr.eParkClass.insert(in::road::parking_class::tower);			///< ^[p[LO
	rLinkAttr.eParkClass.insert(in::road::parking_class::auto_tower);		///< ©®ñ]^[p[LO
	rLinkAttr.eParkClass.insert(in::road::parking_class::flat);				///< ½ÊÔê
	rLinkAttr.eParkClass.insert(in::road::parking_class::unknown);			///< s¾

	// ÔêNíÊ
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::enter);	///< üûN
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::exit);		///< oûN
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::entrance);	///< oüûN
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::premises);	///< \à¹HN
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::junction);	///< xÔÌnèN
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::unknown);	///< ¢²¸EìÆ

	// ãºÖW
	rLinkAttr.eUpDown.insert(in::road::updown::up1);						///< ãE½s
	rLinkAttr.eUpDown.insert(in::road::updown::up2);						///< ãEcçñÅ¢é
	rLinkAttr.eUpDown.insert(in::road::updown::down1);						///< ºE½s
	rLinkAttr.eUpDown.insert(in::road::updown::down2);						///< ºEÖ±ñÅ¢é
	rLinkAttr.eUpDown.insert(in::road::updown::unknown);					///< ¢²¸

	// ®«íÊR[h(Nà®«)
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::bridge);
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::tunnel);
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::doumon);
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::crossing);
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::underpass);
#if IN_VERSION >= 550
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::dirtroad);
#else
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::tollgate);
	rLinkAttr.eAttrClass.insert(in::road::linkpoint_attr::walkbridge);
#endif

	// ®«íÊR[h(XÎtO)
	rLinkAttr.eGradFlag.insert(in::road::grad::unknown);
	rLinkAttr.eGradFlag.insert(in::road::grad::up);
	rLinkAttr.eGradFlag.insert(in::road::grad::down);
	rLinkAttr.eGradFlag.insert(in::road::grad::flat);

	// G[R[hi[
	for( int iCnt = 0; iCnt < INLinkEnd + 1; ++iCnt )
		rLinkAttr.eError[LinkError[iCnt].iCode] = LinkError[iCnt].strMessage;

	// s­ææR[h
	CString cstrPol = rPolDir + "\\pol_code.txt";
	ifstream ifpol( cstrPol );
	if( ifpol == NULL )
	{
		cout << "# ERROR\tpol_code.txtªJ¯Ü¹ñB\t" << static_cast<LPCTSTR>(cstrPol) << endl;
		return false;
	}

	char rStrLine[1024] = {0};
	while(ifpol.getline( rStrLine, sizeof(rStrLine)))
	{
		CString csLine = rStrLine;
		CString csAdmin = csLine.Left( csLine.Find('\t') );
		int hoge = _ttoi(csAdmin);
		rLinkAttr.eAdminCode.insert( _ttoi(csAdmin) );
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// m[hCf[^îlðZbg
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setNodeLayer(NodeAttrList &rNodeAttr)
{
	// MtO
	rNodeAttr.eSignal.insert(in::road::signal::valid);						///< M@Lè
	rNodeAttr.eSignal.insert(in::road::signal::invalid);					///< M@³µ

	// m[híÊR[h
	rNodeAttr.eNodeClass.insert(in::road::node_class::crossing);			///< ð·_m[h
	rNodeAttr.eNodeClass.insert(in::road::node_class::terminate);			///< s«~Üè_m[h
	rNodeAttr.eNodeClass.insert(in::road::node_class::dummy);				///< _~[m[h
	rNodeAttr.eNodeClass.insert(in::road::node_class::edge);				///< ææÏX_m[h
	rNodeAttr.eNodeClass.insert(in::road::node_class::attr_change);			///< ®«Ï»_m[h
	rNodeAttr.eNodeClass.insert(in::road::node_class::unknown);				///< ¢²¸EìÆ

	// ¹H³LøtO
	rNodeAttr.eHeight.insert(in::road::height::valid);						///< ³Lø
	rNodeAttr.eHeight.insert(in::road::height::invalid);					///< ³³ø

	// G[R[hi[
	for( int iCnt = 0; iCnt < INNodeEnd + 1; ++iCnt )
		rNodeAttr.eError[NodeError[iCnt].iCode] = NodeError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// VðÊK§Cf[^îlðZbg
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setRegLayer(RegAttrList &rRegAttr)
{
	// júwèðZbg
	// júR[h
	rRegAttr.eDays.insert(in::road::dayofweek::holiday);					///< úExú
	rRegAttr.eDays.insert(in::road::dayofweek::except_holiday);				///< úExúð­
	rRegAttr.eDays.insert(in::road::dayofweek::public_holiday);				///< xúiújÍ­j
	rRegAttr.eDays.insert(in::road::dayofweek::except_public_holiday);		///< xúð­
	rRegAttr.eDays.insert(in::road::dayofweek::weekday);					///< ½ú
	rRegAttr.eDays.insert(in::road::dayofweek::sunday);						///< új
	rRegAttr.eDays.insert(in::road::dayofweek::monday);						///< j	
	rRegAttr.eDays.insert(in::road::dayofweek::tuesday);					///< Îj
	rRegAttr.eDays.insert(in::road::dayofweek::wednesday);					///< j
	rRegAttr.eDays.insert(in::road::dayofweek::thursday);					///< Øj
	rRegAttr.eDays.insert(in::road::dayofweek::friday);						///< àj
	rRegAttr.eDays.insert(in::road::dayofweek::saturday);					///< yj
	rRegAttr.eDays.insert(in::road::dayofweek::weekend);					///< yúExú
	rRegAttr.eDays.insert(in::road::dayofweek::except_weekend);				///< yúExúð­
	rRegAttr.eDays.insert(in::road::dayofweek::allday);						///< Sjú
	rRegAttr.eDays.insert(in::road::dayofweek::except_sunday);				///< újúð­
	rRegAttr.eDays.insert(in::road::dayofweek::pre_holiday);				///< úExúÌOú
	rRegAttr.eDays.insert(in::road::dayofweek::unknown);		///< YÈµ

	// K§ððZbg
	rRegAttr.eCond.insert(in::road::regulation_cond::no_condition);			///< ÊíK§(ð³µ)
	rRegAttr.eCond.insert(in::road::regulation_cond::with_condition);		///< ðt«K§
	rRegAttr.eCond.insert(in::road::regulation_cond::unknown);				///< ¢²¸

	// ÊsK§ðZbg
	rRegAttr.eLink.insert(in::road::passage_regulation::no_regulation);		///< K§³µ
	rRegAttr.eLink.insert(in::road::passage_regulation::no_passage);		///< ÊsÖ~
	rRegAttr.eLink.insert(in::road::passage_regulation::oneway_order);		///< êûÊs(³ûü)
	rRegAttr.eLink.insert(in::road::passage_regulation::oneway_reverse);	///< êûÊs(tûü)
	rRegAttr.eLink.insert(in::road::passage_regulation::bad_passage);		///< ÊssÂ
	rRegAttr.eLink.insert(in::road::passage_regulation::relation_passage);	///< ÊsÖ~(ÖWÒÈO)
	rRegAttr.eLink.insert(in::road::passage_regulation::etc_only);			///< ETCêp
	rRegAttr.eLink.insert(0);												///< ¢²¸EÎÛO(m[hñp)

	// G[R[hi[
	for( int iCnt = 0; iCnt < INRegEnd + 1; ++iCnt )
		rRegAttr.eError[RegError[iCnt].iCode] = RegError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// m[hîñCf[^îlðZbg
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setNInfoLayer(NInfoAttrList &rNInfoAttr)
{
	// m[híÊR[hðZbg
	rNInfoAttr.eClass.insert(in::road::node_info_class::crossname_nodir);		///< ð·_¼Ì(ûü«Èµ)
	rNInfoAttr.eClass.insert(in::road::node_info_class::crossname_dir);			///< ð·_¼Ì(ûü« è)
	rNInfoAttr.eClass.insert(in::road::node_info_class::highway_ic_entrance);	///< ¬¹HICüûm[h(êÊ¹©çÌüHm[h)
	rNInfoAttr.eClass.insert(in::road::node_info_class::highway_ic_enter);		///< ¬¹HICüH
	rNInfoAttr.eClass.insert(in::road::node_info_class::highway_ic_exit);		///< ¬¹HICoH
	rNInfoAttr.eClass.insert(in::road::node_info_class::jct_enter);				///< üHJCTm[h
	rNInfoAttr.eClass.insert(in::road::node_info_class::jct_exit);				///< oHJCTm[h
	rNInfoAttr.eClass.insert(in::road::node_info_class::sapa_enter);			///< SAPAüH
	rNInfoAttr.eClass.insert(in::road::node_info_class::sapa_exit);				///< SAPAoH
	rNInfoAttr.eClass.insert(in::road::node_info_class::highway_dirguide);		///< ûÊÄà(¬¹Hã)
	rNInfoAttr.eClass.insert(in::road::node_info_class::general_dirguide);		///< ÄàÅÂ(êÊ¹ûÊÄà)
	rNInfoAttr.eClass.insert(in::road::node_info_class::noguide);				///< m[KCh
	rNInfoAttr.eClass.insert(in::road::node_info_class::guide_code);			///< ­§U±(R[h)
	rNInfoAttr.eClass.insert(in::road::node_info_class::guide_string);			///< ­§U±(¶ñ)
	rNInfoAttr.eClass.insert(in::road::node_info_class::illust_file);			///< CXgt@C¼(U±êp)
	rNInfoAttr.eClass.insert(in::road::node_info_class::illust_file2);			///< CXgt@C¼(nCEFC[hêp)
	rNInfoAttr.eClass.insert(in::road::node_info_class::illust_file3);			///< CXgt@C¼(U±AnCEFC[hp)

	// næíÊðZbg
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::general);			///< næ(êÊ)
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::highway);			///< næ(L¿)
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::kokudo_no);			///< ¹Ô
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::kendo_no);			///< §¹Ô
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::others);			///< »Ì¼

	// ÄàR[h
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::noguide);				///< ¹Èè(ûÊÄàÈµ)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::straight);				///< ¼i(12ûü)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right_front);			///< EOûü(1ûü)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right_ahead);			///< EèOûü(2ûü)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right);					///< EÜ(3ûü)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right_behind);			///< EÎßãëûü(4ûü)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right_back);			///< Eßéûü(5ûü)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::back);					///< ßéûü(6ûü)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left_back);				///< ¶ßéûü(7ûü)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left_behind);			///< ¶Îßãëûü(8ûü)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left);					///< ¶Ü(9ûü)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left_ahead);			///< ¶èOûü(10ûü)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left_front);			///< ¶Oûü(11ûü)
	rNInfoAttr.eGuideCode.insert(0xff);											///< ¢Ýè

	// gpÖ~¶R[hðZbg
	/* gpÖ~¶(BÌOÍSpXy[X)
	   -------------------------------------------
	   @BCFGHJKKLMKLMNOPQRS
       TUVWZ\]^_abcdefghijk
       lmnopqrstuvwxyz{}~
	   
	    ¡¢£¤¥¦§¨©ª«¬
	   ¸¹º»¼½¾¿ÈÉÊËÌÍÎÚÛÜÝÞß
	   àáâãäåæçèðñòóôõö÷ü
	   -------------------------------------------
	 */

	// Â³êÄ¢é¶iûÊÄàpj
	rNInfoAttr.eUseOKCharDirCode.insert(0x8141);	// 'A'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8144);	// 'D'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8145);	// 'E'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8158);	// 'X'
	rNInfoAttr.eUseOKCharDirCode.insert(0x815b);	// '['
	rNInfoAttr.eUseOKCharDirCode.insert(0x8160);	// '`'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8169);	// 'i'
	rNInfoAttr.eUseOKCharDirCode.insert(0x816a);	// 'j'
	rNInfoAttr.eUseOKCharDirCode.insert(0x817c);	// '|'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8159);	// 'Y'	[bug7330]
	rNInfoAttr.eUseOKCharDirCode.insert(0x8396);	// ''

	// Â³êÄ¢é¶ið·_¼Ìpj
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8141);	// 'A'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8144);	// 'D'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8145);	// 'E'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8158);	// 'X'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x815b);	// '['
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8160);	// '`'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8169);	// 'i'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x816a);	// 'j'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x817c);	// '|'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8159);	// 'Y'	[bug7330]
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8396);	// ''

	// G[R[hi[
	for( int iCnt = 0; iCnt < INNInfoEnd + 1; ++iCnt )
		rNInfoAttr.eError[NInfoError[iCnt].iCode] = NInfoError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// [îñCf[^îlðZbg
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setLaneLayer(LaneAttrList& rLaneAttr)
{
	// G[R[hi[
	for( int iCnt = 0; iCnt < INLaneEnd + 1; ++iCnt )
		rLaneAttr.eError[LaneError[iCnt].iCode] = LaneError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// NîñCf[^îlðZbg
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setLInfoLayer(LInfoAttrList &rLInfoAttr, LinkAttrList& rLinkAttr)
{
	// \¦íÊR[hðZbg
	rLInfoAttr.eDisp = rLinkAttr.eDispClass;

	// K§¬xðZbg
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_under30km);			///< 30km/h¢
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_40km);				///< 40km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_50km);				///< 50km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_60km);				///< 60km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_70km);				///< 70km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_80km);				///< 80km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_100km);				///< 100km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::unknown);					///< ¢²¸

	// å]æª
	rLInfoAttr.eSection.insert(in::road::kubun_code::main_line);				///< åHü
	rLInfoAttr.eSection.insert(in::road::kubun_code::sub_line);					///< ]Hü
	rLInfoAttr.eSection.insert(in::road::kubun_code::unknown);					///< ¢²¸EìÆ

	// G[R[hi[
	for( int iCnt = 0; iCnt < INLInfoEnd + 1; ++iCnt )
		rLInfoAttr.eError[LInfoError[iCnt].iCode] = LInfoError[iCnt].strMessage;
}	

//////////////////////////////////////////////////////////////////////
// Ng£Cf[^îlðZbg
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setLinkExLayer(LinkExAttrList &rLinkExAttr, LInfoAttrList& rLInfoAttr)
{
	// NîñL[ðZbg
	rLinkExAttr.eKey.insert(in::road::lext_key::kubun_code);			///< å]æªR[h
	rLinkExAttr.eKey.insert(in::road::lext_key::limit_speed);			///< K§¬x
	rLinkExAttr.eKey.insert(in::road::lext_key::trip_speed);			///< ·s¬x
	rLinkExAttr.eKey.insert(in::road::lext_key::island_flag);			///< etO
	rLinkExAttr.eKey.insert(in::road::lext_key::area_flag);				///< ×XHGAtO
	rLinkExAttr.eKey.insert(in::road::lext_key::oneside_lane);			///< Ð¤Ôü

	// K§¬xðZbg
	rLinkExAttr.eSpeed = rLInfoAttr.eSpeed;

	// å]æªðZbg
	rLinkExAttr.eSection = rLInfoAttr.eSection;

	// tOðZbg
	rLinkExAttr.eIsland.insert(in::road::island_flag::valid);			///< e·é
	rLinkExAttr.eIsland.insert(in::road::island_flag::invalid);			///< eµÈ¢

	// ×XHGA»ÊtO
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_1998);			///< 1998Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_1999);			///< 1999Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2000_spr);		///< 2000Nxt
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2000_aut);		///< 2000NxH
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2001);			///< 2001Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2002);			///< 2002Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2003);			///< 2003Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2004);			///< 2004Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2005);			///< 2005Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2006);			///< 2006Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2007);			///< 2007Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::unknown);			///< ¢²¸EìÆ

	// ×XHL2(0x80000000)
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_1998 | in::road::area_flag::rest);			///< 1998Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_1999 | in::road::area_flag::rest);			///< 1999Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2000_spr | in::road::area_flag::rest);		///< 2000Nxt
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2000_aut | in::road::area_flag::rest);		///< 2000NxH
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2001 | in::road::area_flag::rest);			///< 2001Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2002 | in::road::area_flag::rest);			///< 2002Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2003 | in::road::area_flag::rest);			///< 2003Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2004 | in::road::area_flag::rest);			///< 2004Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2005 | in::road::area_flag::rest);			///< 2005Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2006 | in::road::area_flag::rest);			///< 2006Nx
	rLinkExAttr.eDetail.insert(in::road::area_flag::unknown | in::road::area_flag::rest);				///< ¢²¸EìÆ

	// G[R[hi[
	for( int iCnt = 0; iCnt < INLinkExEnd + 1; ++iCnt )
		rLinkExAttr.eError[LinkExError[iCnt].iCode] = LinkExError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// 10i¨16iÖÌÏ·
//////////////////////////////////////////////////////////////////////
void SetObjectID(CString &rCstrID, int rID)
{
	CString cstrID, cstrTmp;
	cstrID.Format("%x", rID);

	// ó¢½É0ð}ü
	for(int iLength = 0; iLength < (8 - cstrID.GetLength()); ++iLength)
		cstrTmp += "0";

	rCstrID = "0x" + cstrTmp + cstrID;
}
