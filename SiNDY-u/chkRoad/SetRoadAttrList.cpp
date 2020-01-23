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
// 全レイヤデータの基準値をセット
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
// INヘッダ情報基準値をセット
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setINHead(HeadAttrList& rINHead)
{
	// .mnt レイヤID格納
	rINHead.eMnt.insert( 0x00300000 );			///< リンクレイヤ
	rINHead.eMnt.insert( 0x00300800 );			///< ノードレイヤ(6.01)
	rINHead.eMnt.insert( 0x00300004 );			///< 新交通規制レイヤ

	// ,rdgレイヤID格納
	rINHead.eRdg.insert( 0x00300030 );			///< 交差点誘導情報レイヤ
	rINHead.eRdg.insert( 0x00300040 );			///< ノード情報レイヤ
	rINHead.eRdg.insert( 0x00300080 );			///< レーン情報レイヤ
	rINHead.eRdg.insert( 0x00300200 );			///< リンク情報レイヤ
	rINHead.eRdg.insert( 0x00300400 );			///< リンク拡張レイヤ

	// エラーコード格納
	for( int iCnt = 0; iCnt < INHeadEnd + 1; ++iCnt )
		rINHead.eError[HeadError[iCnt].iCode] = HeadError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// リンクレイヤデータ基準値をセット
//////////////////////////////////////////////////////////////////////
bool CSetRoadAttrList::setLinkLayer( LinkAttrList& rLinkAttr, CString& rPolDir )
{
	// 管理者コード
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::jh);				///< 高速道路会社
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::other_public);	///< 道路公社
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::country);		///< 国
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::pref);			///< 都道府県
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::city);			///< 指定市
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::other_city);		///< 他の市町村 （含 東京２３区）
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::others);			///< その他の管理者
	rLinkAttr.eLinkInfo.eManager.insert(in::road::manager::unknown);		///< 未調査・作業中

	// リンク種別コード
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::hibunri_honsen);		///< 本線（上下線非分離）
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::bunri_honsen);		///< 本線（上下線分離）
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::junction);			///< 連結路（本線間の渡り線）
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::crossing);			///< 交差点内リンク
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::ramp);				///< 連結路（ランプ）
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::side);				///< 本線と同一路線の側道リンク
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::sa);					///< SA等側線リンク
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::bicycle);			///< 自転車道等リンク（自動車通行は不可）
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::connection);			///< 本線側道接続リンク
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::roundabout);			///< 環状交差点リンク
	rLinkAttr.eLinkInfo.eLinkAttr.insert(in::road::link_class::unknown);			///< 未調査・作業中

	//バイパスフラグ
	rLinkAttr.eLinkInfo.eBypass.insert(in::road::bypass::valid);			///< バイパス有り
	rLinkAttr.eLinkInfo.eBypass.insert(in::road::bypass::invalid);			///< バイパス無し

	// 自動車専用フラグ
	rLinkAttr.eLinkInfo.eAuto.insert(1);									///< 自動車専用道路
	rLinkAttr.eLinkInfo.eAuto.insert(0);									///< 自動車専用道路以外

	// 道路幅員
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::over_13m);			///< 幅員13m以上
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::under_13m);			///< 幅員5.5m～13m未満
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::over_3m);			///< 幅員3.0m～5.5m未満
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::under_3m);			///< 幅員3.0m未満
	rLinkAttr.eLinkInfo.eWidth.insert(in::road::width::unknown);			///< 未調査・作業中

	// 車線数
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::one);					///< 1車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::two);					///< 2車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::three);				///< 3車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::four);					///< 4車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::five);					///< 5車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::six);					///< 6車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::seven);				///< 7車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::eight);				///< 8車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::nine);					///< 9車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::ten);					///< 10車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::eleven);				///< 11車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::twelve);				///< 12車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::thirteen);				///< 13車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::forteen);				///< 14車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::fifteen);				///< 15車線
	rLinkAttr.eLinkInfo.eLane.insert(in::road::lane::unknown);				///< 未調査・作業中

	// 交通規制種別コード
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::no_rest);		///< 規制無し
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::no_passage);		///< 通行禁止(条件なし)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::no_passage2);	///< 通行禁止(条件あり)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_order);	///< 一方通行(正方向・条件なし)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_reverse);	///< 一方通行(逆方向・条件なし)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_order2);	///< 一方通行(正方向・条件あり)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_reverse2);///< 一方通行(逆方向・条件あり)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::oneway_both);	///< 一方通行(正逆切り替えあり)
	rLinkAttr.eLinkInfo.eRest.insert(in::road::rest_class::unknown);		///< 未調査・作業中

	// 交通規制条件コード
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::car);				///< 車種のみ
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::time);			///< 時刻のみ
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::day);				///< 曜日のみ
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::car_and_time);	///< 車種及び時刻
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::car_and_day);		///< 車種及び曜日
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::time_and_day);	///< 時刻及び曜日
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::all);				///< 車種、時刻及び曜日
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::others);			///< その他の条件
	rLinkAttr.eLinkInfo.eCond.insert(in::road::rest_cond::unknown);			///< 未調査・作業中

	// 道路表示種別コード
	rLinkAttr.eDispClass.insert(in::road::road_class::highway2);			///< 都市間高速道路
	rLinkAttr.eDispClass.insert(in::road::road_class::highway);				///< 都市間高速道路(有料)
	rLinkAttr.eDispClass.insert(in::road::road_class::city_highway);		///< 都市高速道路(有料)
	rLinkAttr.eDispClass.insert(in::road::road_class::kokudo);				///< 一般国道
	rLinkAttr.eDispClass.insert(in::road::road_class::kokudo2);				///< 一般国道(有料)
	rLinkAttr.eDispClass.insert(in::road::road_class::pref_mlocal);			///< 主要地方道(都府県)
	rLinkAttr.eDispClass.insert(in::road::road_class::pref_mlocal2);		///< 主要地方道(都府県)(有料)
	rLinkAttr.eDispClass.insert(in::road::road_class::city_mlocal);			///< 主要地方道(指定市)
	rLinkAttr.eDispClass.insert(in::road::road_class::city_mlocal2);		///< 主要地方道(指定市)(有料)
	rLinkAttr.eDispClass.insert(in::road::road_class::pref_road);			///< 一般都道府県道
	rLinkAttr.eDispClass.insert(in::road::road_class::pref_road2);			///< 一般都道府県道(有料)
	rLinkAttr.eDispClass.insert(in::road::road_class::city_road);			///< 指定市の一般市道
	rLinkAttr.eDispClass.insert(in::road::road_class::city_road2);			///< 指定市の一般市道(有料)
	rLinkAttr.eDispClass.insert(in::road::road_class::other_road);			///< その他の道路
	rLinkAttr.eDispClass.insert(in::road::road_class::other_road2);			///< その他の道路(有料)
	rLinkAttr.eDispClass.insert(in::road::road_class::other_road3);			///< 格下げその他道
	rLinkAttr.eDispClass.insert(in::road::road_class::detail_road);			///< 細街路1
	rLinkAttr.eDispClass.insert(in::road::road_class::detail_road2);		///< 細街路2
	rLinkAttr.eDispClass.insert(in::road::road_class::detail_road3);		///< 細街路3
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link);			///< ダミーリンク
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link2);			///< 庭園路
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link3);			///< マッチング用リンク
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link4);			///< PEC用マッチングリンク
	rLinkAttr.eDispClass.insert(in::road::road_class::dummy_link5);			///< BT用マッチングリンク
	rLinkAttr.eDispClass.insert(in::road::road_class::ferry);				///< フェリー航路
	rLinkAttr.eDispClass.insert(in::road::road_class::parking);				///< 駐車場内道路

	// 道路経路種別コード
	rLinkAttr.eNaviClass.insert(in::road::navi_class::highway);				///< 都市間高速道路
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_highway);		///< 都市高速道路
	rLinkAttr.eNaviClass.insert(in::road::navi_class::kokudo);				///< 一般国道
	rLinkAttr.eNaviClass.insert(in::road::navi_class::kokudo2);				///< 一般国道(有料)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::pref_mlocal);			///< 主要地方道(都府県)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::pref_mlocal2);		///< 主要地方道(都府県)(有料)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_mlocal);			///< 主要地方道(指定市)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_mlocal2);		///< 主要地方道(指定市)(有料)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::pref_road);			///< 一般都道府県道
	rLinkAttr.eNaviClass.insert(in::road::navi_class::pref_road2);			///< 一般都道府県道(有料)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_road);			///< 指定市の一般市道
	rLinkAttr.eNaviClass.insert(in::road::navi_class::city_road2);			///< 指定市の一般市道(有料)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::other_road);			///< その他の道路
	rLinkAttr.eNaviClass.insert(in::road::navi_class::other_road2);			///< その他の道路(有料)
	rLinkAttr.eNaviClass.insert(in::road::navi_class::other_road3);			///< 格下げその他道
	rLinkAttr.eNaviClass.insert(in::road::navi_class::detail_road);			///< 細街路1
	rLinkAttr.eNaviClass.insert(in::road::navi_class::detail_road2);		///< 細街路2
	rLinkAttr.eNaviClass.insert(in::road::navi_class::detail_road3);		///< 細街路3
	rLinkAttr.eNaviClass.insert(in::road::navi_class::connectway);			///< 接続道路
	rLinkAttr.eNaviClass.insert(in::road::navi_class::kkd_route);			///< KD経路誘導不適区間
	rLinkAttr.eNaviClass.insert(in::road::navi_class::dummy_link3);			///< マッチング用リンク
	rLinkAttr.eNaviClass.insert(in::road::navi_class::dummy_link4);			///< PEC用マッチングリンク
	rLinkAttr.eNaviClass.insert(in::road::navi_class::ferry);				///< フェリー航路
	rLinkAttr.eNaviClass.insert(in::road::navi_class::parking);				///< 駐車場内道路
	rLinkAttr.eNaviClass.insert(in::road::navi_class::dummy_link5);			///< BT用マッチングリンク

	// 広域フラグ
#if IN_VERSION >= 590
	rLinkAttr.eAreaFlag.insert(in::road::wide_area::wide_area);				///< 広域に存在する
	rLinkAttr.eAreaFlag.insert(in::road::wide_area::not_wide_area);			///< 広域に存在しない
#else
	rLinkAttr.eAreaFlag.insert(in::road::wide_area::wide_area1);			///< 広域1
	rLinkAttr.eAreaFlag.insert(in::road::wide_area::wide_area2);			///< 広域2
#endif

	// 駐車場種別
	rLinkAttr.eParkClass.insert(in::road::parking_class::solid);			///< 立体駐車場
	rLinkAttr.eParkClass.insert(in::road::parking_class::under);			///< 地下駐車場
	rLinkAttr.eParkClass.insert(in::road::parking_class::both);				///< 立体かつ地下駐車場
	rLinkAttr.eParkClass.insert(in::road::parking_class::tower);			///< タワーパーキング
	rLinkAttr.eParkClass.insert(in::road::parking_class::auto_tower);		///< 自動回転タワーパーキング
	rLinkAttr.eParkClass.insert(in::road::parking_class::flat);				///< 平面駐車場
	rLinkAttr.eParkClass.insert(in::road::parking_class::unknown);			///< 不明

	// 駐車場リンク種別
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::enter);	///< 入口リンク
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::exit);		///< 出口リンク
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::entrance);	///< 出入口リンク
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::premises);	///< 構内道路リンク
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::junction);	///< レベル間の渡りリンク
	rLinkAttr.eParkLinkClass.insert(in::road::parkinglink_class::unknown);	///< 未調査・作業中

	// 上下関係
	rLinkAttr.eUpDown.insert(in::road::updown::up1);						///< 上・平行
	rLinkAttr.eUpDown.insert(in::road::updown::up2);						///< 上・膨らんでいる
	rLinkAttr.eUpDown.insert(in::road::updown::down1);						///< 下・平行
	rLinkAttr.eUpDown.insert(in::road::updown::down2);						///< 下・へこんでいる
	rLinkAttr.eUpDown.insert(in::road::updown::unknown);					///< 未調査

	// 属性種別コード(リンク内属性)
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

	// 属性種別コード(傾斜フラグ)
	rLinkAttr.eGradFlag.insert(in::road::grad::unknown);
	rLinkAttr.eGradFlag.insert(in::road::grad::up);
	rLinkAttr.eGradFlag.insert(in::road::grad::down);
	rLinkAttr.eGradFlag.insert(in::road::grad::flat);

	// エラーコード格納
	for( int iCnt = 0; iCnt < INLinkEnd + 1; ++iCnt )
		rLinkAttr.eError[LinkError[iCnt].iCode] = LinkError[iCnt].strMessage;

	// 行政区域コード
	CString cstrPol = rPolDir + "\\pol_code.txt";
	ifstream ifpol( cstrPol );
	if( ifpol == NULL )
	{
		cout << "# ERROR\tpol_code.txtが開けません。\t" << static_cast<LPCTSTR>(cstrPol) << endl;
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
// ノードレイヤデータ基準値をセット
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setNodeLayer(NodeAttrList &rNodeAttr)
{
	// 信号フラグ
	rNodeAttr.eSignal.insert(in::road::signal::valid);						///< 信号機有り
	rNodeAttr.eSignal.insert(in::road::signal::invalid);					///< 信号機無し

	// ノード種別コード
	rNodeAttr.eNodeClass.insert(in::road::node_class::crossing);			///< 交差点ノード
	rNodeAttr.eNodeClass.insert(in::road::node_class::terminate);			///< 行き止まり点ノード
	rNodeAttr.eNodeClass.insert(in::road::node_class::dummy);				///< ダミーノード
	rNodeAttr.eNodeClass.insert(in::road::node_class::edge);				///< 区画変更点ノード
	rNodeAttr.eNodeClass.insert(in::road::node_class::attr_change);			///< 属性変化点ノード
	rNodeAttr.eNodeClass.insert(in::road::node_class::unknown);				///< 未調査・作業中

	// 道路高さ有効フラグ
	rNodeAttr.eHeight.insert(in::road::height::valid);						///< 高さ有効
	rNodeAttr.eHeight.insert(in::road::height::invalid);					///< 高さ無効

	// エラーコード格納
	for( int iCnt = 0; iCnt < INNodeEnd + 1; ++iCnt )
		rNodeAttr.eError[NodeError[iCnt].iCode] = NodeError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// 新交通規制レイヤデータ基準値をセット
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setRegLayer(RegAttrList &rRegAttr)
{
	// 曜日指定をセット
	// 曜日コード
	rRegAttr.eDays.insert(in::road::dayofweek::holiday);					///< 日・休日
	rRegAttr.eDays.insert(in::road::dayofweek::except_holiday);				///< 日・休日を除く
	rRegAttr.eDays.insert(in::road::dayofweek::public_holiday);				///< 休日（日曜は除く）
	rRegAttr.eDays.insert(in::road::dayofweek::except_public_holiday);		///< 休日を除く
	rRegAttr.eDays.insert(in::road::dayofweek::weekday);					///< 平日
	rRegAttr.eDays.insert(in::road::dayofweek::sunday);						///< 日曜
	rRegAttr.eDays.insert(in::road::dayofweek::monday);						///< 月曜	
	rRegAttr.eDays.insert(in::road::dayofweek::tuesday);					///< 火曜
	rRegAttr.eDays.insert(in::road::dayofweek::wednesday);					///< 水曜
	rRegAttr.eDays.insert(in::road::dayofweek::thursday);					///< 木曜
	rRegAttr.eDays.insert(in::road::dayofweek::friday);						///< 金曜
	rRegAttr.eDays.insert(in::road::dayofweek::saturday);					///< 土曜
	rRegAttr.eDays.insert(in::road::dayofweek::weekend);					///< 土日・休日
	rRegAttr.eDays.insert(in::road::dayofweek::except_weekend);				///< 土日・休日を除く
	rRegAttr.eDays.insert(in::road::dayofweek::allday);						///< 全曜日
	rRegAttr.eDays.insert(in::road::dayofweek::except_sunday);				///< 日曜日を除く
	rRegAttr.eDays.insert(in::road::dayofweek::pre_holiday);				///< 日・休日の前日
	rRegAttr.eDays.insert(in::road::dayofweek::unknown);		///< 該当なし

	// 規制条件をセット
	rRegAttr.eCond.insert(in::road::regulation_cond::no_condition);			///< 通常規制(条件無し)
	rRegAttr.eCond.insert(in::road::regulation_cond::with_condition);		///< 条件付き規制
	rRegAttr.eCond.insert(in::road::regulation_cond::unknown);				///< 未調査

	// 通行規制をセット
	rRegAttr.eLink.insert(in::road::passage_regulation::no_regulation);		///< 規制無し
	rRegAttr.eLink.insert(in::road::passage_regulation::no_passage);		///< 通行禁止
	rRegAttr.eLink.insert(in::road::passage_regulation::oneway_order);		///< 一方通行(正方向)
	rRegAttr.eLink.insert(in::road::passage_regulation::oneway_reverse);	///< 一方通行(逆方向)
	rRegAttr.eLink.insert(in::road::passage_regulation::bad_passage);		///< 通行不可
	rRegAttr.eLink.insert(in::road::passage_regulation::relation_passage);	///< 通行禁止(関係者以外)
	rRegAttr.eLink.insert(in::road::passage_regulation::etc_only);			///< ETC専用
	rRegAttr.eLink.insert(0);												///< 未調査・対象外(ノード列用)

	// エラーコード格納
	for( int iCnt = 0; iCnt < INRegEnd + 1; ++iCnt )
		rRegAttr.eError[RegError[iCnt].iCode] = RegError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// ノード情報レイヤデータ基準値をセット
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setNInfoLayer(NInfoAttrList &rNInfoAttr)
{
	// ノード種別コードをセット
	rNInfoAttr.eClass.insert(in::road::node_info_class::crossname_nodir);		///< 交差点名称(方向性なし)
	rNInfoAttr.eClass.insert(in::road::node_info_class::crossname_dir);			///< 交差点名称(方向性あり)
	rNInfoAttr.eClass.insert(in::road::node_info_class::highway_ic_entrance);	///< 高速道路IC入口ノード(一般道からの入路ノード)
	rNInfoAttr.eClass.insert(in::road::node_info_class::highway_ic_enter);		///< 高速道路IC入路
	rNInfoAttr.eClass.insert(in::road::node_info_class::highway_ic_exit);		///< 高速道路IC出路
	rNInfoAttr.eClass.insert(in::road::node_info_class::jct_enter);				///< 入路JCTノード
	rNInfoAttr.eClass.insert(in::road::node_info_class::jct_exit);				///< 出路JCTノード
	rNInfoAttr.eClass.insert(in::road::node_info_class::sapa_enter);			///< SAPA入路
	rNInfoAttr.eClass.insert(in::road::node_info_class::sapa_exit);				///< SAPA出路
	rNInfoAttr.eClass.insert(in::road::node_info_class::highway_dirguide);		///< 方面案内(高速道路上)
	rNInfoAttr.eClass.insert(in::road::node_info_class::general_dirguide);		///< 案内看板(一般道方面案内)
	rNInfoAttr.eClass.insert(in::road::node_info_class::noguide);				///< ノーガイド
	rNInfoAttr.eClass.insert(in::road::node_info_class::guide_code);			///< 強制誘導(コード)
	rNInfoAttr.eClass.insert(in::road::node_info_class::guide_string);			///< 強制誘導(文字列)
	rNInfoAttr.eClass.insert(in::road::node_info_class::illust_file);			///< イラストファイル名(誘導専用)
	rNInfoAttr.eClass.insert(in::road::node_info_class::illust_file2);			///< イラストファイル名(ハイウェイモード専用)
	rNInfoAttr.eClass.insert(in::road::node_info_class::illust_file3);			///< イラストファイル名(誘導、ハイウェイモード用)

	// 地先種別をセット
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::general);			///< 地先(一般)
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::highway);			///< 地先(有料)
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::kokudo_no);			///< 国道番号
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::kendo_no);			///< 県道番号
	rNInfoAttr.eGuideClass.insert(in::road::dirguide_class::others);			///< その他

	// 案内コード
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::noguide);				///< 道なり(方面案内なし)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::straight);				///< 直進(12時方向)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right_front);			///< 右前方向(1時方向)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right_ahead);			///< 右手前方向(2時方向)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right);					///< 右折(3時方向)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right_behind);			///< 右斜め後ろ方向(4時方向)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::right_back);			///< 右戻る方向(5時方向)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::back);					///< 戻る方向(6時方向)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left_back);				///< 左戻る方向(7時方向)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left_behind);			///< 左斜め後ろ方向(8時方向)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left);					///< 左折(9時方向)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left_ahead);			///< 左手前方向(10時方向)
	rNInfoAttr.eGuideCode.insert(in::road::guide_class::left_front);			///< 左前方向(11時方向)
	rNInfoAttr.eGuideCode.insert(0xff);											///< 未設定

	// 使用禁止文字コードをセット
	/* 使用禁止文字(。の前は全角スペース)
	   -------------------------------------------
	   　。，：；？゛゜゜´｀゜´｀¨＾￣＿ヽヾ
       ゝゞ〃仝〇―‐／＼∥｜…‥‘’“”（）〔
       〕［］｛｝〈〉《》「」『』【】＋±×÷＝≠
	   ＜＞≦≧∞∴♂♀°′″℃￥＄￠￡％＃＆＊＠
	   §☆★○●◎◇◆□■△▲▽▼※〒→←↑↓〓
	   ∈∋⊆⊇⊂⊃∪∩∧∨￢⇒⇔∀∃∠⊥⌒∂∇≡
	   ≒≪≫√∽∝∵∫∬Å‰♯♭♪†‡¶◯
	   -------------------------------------------
	 */

	// 許可されている文字（方面案内用）
	rNInfoAttr.eUseOKCharDirCode.insert(0x8141);	// '、'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8144);	// '．'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8145);	// '・'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8158);	// '々'
	rNInfoAttr.eUseOKCharDirCode.insert(0x815b);	// 'ー'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8160);	// '～'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8169);	// '（'
	rNInfoAttr.eUseOKCharDirCode.insert(0x816a);	// '）'
	rNInfoAttr.eUseOKCharDirCode.insert(0x817c);	// '－'
	rNInfoAttr.eUseOKCharDirCode.insert(0x8159);	// '〆'	[bug7330]
	rNInfoAttr.eUseOKCharDirCode.insert(0x8396);	// 'ヶ'

	// 許可されている文字（交差点名称用）
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8141);	// '、'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8144);	// '．'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8145);	// '・'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8158);	// '々'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x815b);	// 'ー'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8160);	// '～'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8169);	// '（'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x816a);	// '）'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x817c);	// '－'
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8159);	// '〆'	[bug7330]
	rNInfoAttr.eUseOKCharNodeCode.insert(0x8396);	// 'ヶ'

	// エラーコード格納
	for( int iCnt = 0; iCnt < INNInfoEnd + 1; ++iCnt )
		rNInfoAttr.eError[NInfoError[iCnt].iCode] = NInfoError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// レーン情報レイヤデータ基準値をセット
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setLaneLayer(LaneAttrList& rLaneAttr)
{
	// エラーコード格納
	for( int iCnt = 0; iCnt < INLaneEnd + 1; ++iCnt )
		rLaneAttr.eError[LaneError[iCnt].iCode] = LaneError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// リンク情報レイヤデータ基準値をセット
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setLInfoLayer(LInfoAttrList &rLInfoAttr, LinkAttrList& rLinkAttr)
{
	// 表示種別コードをセット
	rLInfoAttr.eDisp = rLinkAttr.eDispClass;

	// 規制速度をセット
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_under30km);			///< 30km/h未満
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_40km);				///< 40km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_50km);				///< 50km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_60km);				///< 60km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_70km);				///< 70km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_80km);				///< 80km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::limit_100km);				///< 100km/h
	rLInfoAttr.eSpeed.insert(in::road::limit_speed::unknown);					///< 未調査

	// 主従区分
	rLInfoAttr.eSection.insert(in::road::kubun_code::main_line);				///< 主路線
	rLInfoAttr.eSection.insert(in::road::kubun_code::sub_line);					///< 従路線
	rLInfoAttr.eSection.insert(in::road::kubun_code::unknown);					///< 未調査・作業中

	// エラーコード格納
	for( int iCnt = 0; iCnt < INLInfoEnd + 1; ++iCnt )
		rLInfoAttr.eError[LInfoError[iCnt].iCode] = LInfoError[iCnt].strMessage;
}	

//////////////////////////////////////////////////////////////////////
// リンク拡張レイヤデータ基準値をセット
//////////////////////////////////////////////////////////////////////
void CSetRoadAttrList::setLinkExLayer(LinkExAttrList &rLinkExAttr, LInfoAttrList& rLInfoAttr)
{
	// リンク情報キーをセット
	rLinkExAttr.eKey.insert(in::road::lext_key::kubun_code);			///< 主従区分コード
	rLinkExAttr.eKey.insert(in::road::lext_key::limit_speed);			///< 規制速度
	rLinkExAttr.eKey.insert(in::road::lext_key::trip_speed);			///< 旅行速度
	rLinkExAttr.eKey.insert(in::road::lext_key::island_flag);			///< 浮島許容フラグ
	rLinkExAttr.eKey.insert(in::road::lext_key::area_flag);				///< 細街路エリアフラグ
	rLinkExAttr.eKey.insert(in::road::lext_key::oneside_lane);			///< 片側車線数

	// 規制速度をセット
	rLinkExAttr.eSpeed = rLInfoAttr.eSpeed;

	// 主従区分をセット
	rLinkExAttr.eSection = rLInfoAttr.eSection;

	// 浮島フラグをセット
	rLinkExAttr.eIsland.insert(in::road::island_flag::valid);			///< 許容する
	rLinkExAttr.eIsland.insert(in::road::island_flag::invalid);			///< 許容しない

	// 細街路エリア判別フラグ
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_1998);			///< 1998年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_1999);			///< 1999年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2000_spr);		///< 2000年度春
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2000_aut);		///< 2000年度秋
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2001);			///< 2001年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2002);			///< 2002年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2003);			///< 2003年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2004);			///< 2004年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2005);			///< 2005年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2006);			///< 2006年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2007);			///< 2007年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::unknown);			///< 未調査・作業中

	// 細街路L2(0x80000000)
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_1998 | in::road::area_flag::rest);			///< 1998年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_1999 | in::road::area_flag::rest);			///< 1999年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2000_spr | in::road::area_flag::rest);		///< 2000年度春
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2000_aut | in::road::area_flag::rest);		///< 2000年度秋
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2001 | in::road::area_flag::rest);			///< 2001年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2002 | in::road::area_flag::rest);			///< 2002年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2003 | in::road::area_flag::rest);			///< 2003年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2004 | in::road::area_flag::rest);			///< 2004年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2005 | in::road::area_flag::rest);			///< 2005年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::year_2006 | in::road::area_flag::rest);			///< 2006年度
	rLinkExAttr.eDetail.insert(in::road::area_flag::unknown | in::road::area_flag::rest);				///< 未調査・作業中

	// エラーコード格納
	for( int iCnt = 0; iCnt < INLinkExEnd + 1; ++iCnt )
		rLinkExAttr.eError[LinkExError[iCnt].iCode] = LinkExError[iCnt].strMessage;
}

//////////////////////////////////////////////////////////////////////
// 10進数→16進数への変換
//////////////////////////////////////////////////////////////////////
void SetObjectID(CString &rCstrID, int rID)
{
	CString cstrID, cstrTmp;
	cstrID.Format("%x", rID);

	// 空いた桁に0を挿入
	for(int iLength = 0; iLength < (8 - cstrID.GetLength()); ++iLength)
		cstrTmp += "0";

	rCstrID = "0x" + cstrTmp + cstrID;
}
