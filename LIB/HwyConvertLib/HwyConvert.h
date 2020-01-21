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

#pragma once
#include "type.h"
#include <boost/noncopyable.hpp>

namespace hwy_convert_lib
{
	// TODO: SindyHighwayConvert以外から使用した場合、std::cout,cerrへの出力が邪魔なので、出力しないようにする。

	using namespace in::schema;

	/**
	 * @brief HWY変換クラス
	 * @note	本ライブラリの中心となるクラス。
	 *			本ライブラリはHWY変換ツール（SindyHighwayConvert.exe）のプロジェクトの一部をライブラリ化したもので、
	 *			基本的にはその構成ファイルをそのまま使用しているが、
	 *			SindyHighwayConvert以外のプロジェクトから簡単に使用できるようなインターフェイスを提供する必要があり、
	 *			その役目を担うのがこのクラスである。
	 */
	class HwyConvert : boost::noncopyable
	{
	public:
		/**
		 * @brief コンストラクタ
		 * @param[in]	roadDBStr		道路DB接続情報文字列
		 * @param[in]	addrDBStr		住所DB接続情報文字列
		 * @note 「接続情報文字列」は、sindy::create_workspace_name()がサポートしている文字列。
		 * @see	http://orias.mr.ipc.pioneer.co.jp/pukiwiki/index.php?sindy_workspace
		 */
		HwyConvert(
			const std::string& roadDBStr,
			const std::string& addrDBStr
		);

		//----- データ取得関数群 -----
		// shared_ptr< const SINDY2HIGHWAY >などを返そうと思ったが、
		// SINDY2HIGHWAYなどのメンバ関数が全て非constなので、そうすると使えなくなってしまう。
		// 仕方がないので非constのまま返す。使用時は注意されたし。
			/**
			 * @brief highway.txt系データを作成して返す
			 * @return	作成したデータ（失敗した場合はNULL）
			 */
			SPSINDY2HIGHWAY			highway();

			/**
			 * @brief gs.txt系データを作成して返す
			 * @return	作成したデータ（失敗した場合はNULL）
			 */
			SPSINDY2GS				gs();

			/**
			 * @brief road.code.doc系データを作成して返す
			 * @return	作成したデータ（失敗した場合はNULL）
			 */
			SPSINDY2ROADCODE		roadCode();

			/**
			 * @brief roadpoint.txt系データを作成して返す
			 * @param[in]	pFNCTPath	施設名称変換テーブル（FNCT=FacilNameChangeTable）ファイルのパス（指定されなければ変換しない）
			 * @return	作成したデータ（失敗した場合はNULL）
			 * @note	pFNCTPathの型がuh::tstringではなくwstringなのは、MBCS/Unicode両方の構成から使用できるようにするため。
			 */
			SPSINDY2ROADPOINT		roadPoint(
				const std::wstring* pFNCTPath = NULL
			);

			/**
			 * @brief hwynode.txt系データを作成して返す
			 * @param[in]	format	フォーマット
			 * @note	2013/4/2現在、ETC距離別料金対応フォーマットは未採用なので、formatのデフォルトは非対応にしておく。
			 * @return	作成したデータ（失敗した場合はNULL）
			 */
			SPSINDY2HWYNODE			hwyNode(
				 hwynode::file_format::EFormat format = hwynode::file_format::hwynode_smartic_added
			);

			/**
			 * @brief hwysame.txt系データを作成して返す
			 * @param[in]	pFNCTPath	施設名称変換テーブル（FNCT=FacilNameChangeTable）ファイルのパス（指定されなければ変換しない）
			 * @return	作成したデータ（失敗した場合はNULL）
			 */
			SPSINDY2HWYSAME			hwySame(
				const std::wstring* pFNCTPath = NULL
			);

			/**
			 * @brief tollgate_lane.txt系データを作成して返す
			 * @return	作成したデータ（失敗した場合はNULL）
			 */
			SPSINDY2TOLLGATELANE	tollGateLane();

	private:
		SPSINDY2INHWYINFO		m_pHwyInfo;				//!< HWY情報
		SPSindyDBConnection		m_pRoadDBConnection;	//!< 道路DB接続情報
		SPSindyDBConnection		m_pAddrDBConnection;	//!< 住所DB接続情報

		SPSINDY2HIGHWAY			m_pHighway;				//!< highway.txt系データ
		SPSINDY2GS				m_pGS;					//!< gs.txt系データ
		SPSINDY2ROADCODE		m_pRoadCode;			//!< road.code.doc系データ
		SPSINDY2ROADPOINT		m_pRoadPoint;			//!< roadpoint.txt系データ（施設名称変更なし）
		SPSINDY2ROADPOINT		m_pRoadPointFNC;		//!< roadpoint.txt系データ（施設名称変更あり）
		SPSINDY2HWYNODE			m_pHwyNode;				//!< hwynode.txt系データ
		SPSINDY2HWYSAME			m_pHwySame;				//!< hwysame.txt系データ（施設名称変更なし）
		SPSINDY2HWYSAME			m_pHwySameFNC;			//!< hwysame.txt系データ（施設名称変更あり）
		SPSINDY2TOLLGATELANE	m_pTollGateLane;		//!< tollgate_lane.txt系データ
	};

} // namespace hwy_convert_lib
