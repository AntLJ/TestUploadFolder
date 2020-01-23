#pragma once
#include <vector>
#include <atlstr.h>

typedef std::vector<CString> FIELD_LIST; //!< フィールドリスト

namespace field_lists
{

	/**
	* @brief	道路リンクのフィールド群
	*/
	extern 	const FIELD_LIST roadLinkFieldList;
	/**
	* @brief	道路ノードのフィールド群
	*/
	extern 	const FIELD_LIST roadNodeFieldList;
	/**
	* @brief	リンク内属性のフィールド群
	*/
	extern 	const FIELD_LIST segmentAttrFieldList;
	/**
	* @brief	歩行者リンクのフィールド群
	*/
	extern 	const FIELD_LIST walkLinkFieldList;
	/**
	* @brief	歩行者ノードのフィールド群
	*/
	extern 	const FIELD_LIST walkNodeFieldList;
	/**
	* @brief	駅出入口POIのフィールド群
	*/
	extern 	const FIELD_LIST stationGatePointFieldList;
	/**
	* @brief	POIポイントのフィールド群
	*/
	extern 	const FIELD_LIST poiPointFieldList;
	/**
	* @brief 地下街出入口のフィールド群
	*/
	extern 	const FIELD_LIST underGroundAeraFiledList;
	/**
	* @brief	歩行者路線コードのフィールド群
	*/
	extern 	const FIELD_LIST walkCodeFieldList;
	/**
	* @brief	歩行者通行禁止規制テーブルのフィールド群
	*/
	extern 	const FIELD_LIST walkNoPassageFieldList;
	/**
	* @brief	歩行者一方通行規制テーブルのフィールド群
	*/
	extern 	const FIELD_LIST walkOnewayFieldList;
	/**
	* @brief	リンク紐付けテーブルのフィールド群
	*/
	extern 	const FIELD_LIST linkRelationFieldList;
	/**
	* @brief	重なりリンク高さノードのフィールド群
	*/
	extern 	const FIELD_LIST heightNodeFieldList;
	/**
	* @brief	スクランブルエリアのフィールド群
	*/
	extern 	const FIELD_LIST scrambleAreaList;
	/**
	* @brief	横断ポイントのフィールド群
	*/
	extern 	const FIELD_LIST pedxingList;
}