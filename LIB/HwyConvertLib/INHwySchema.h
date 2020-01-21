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

#include <map>

// I/NとSiNDYのコード値の対応を表すテーブル
typedef long SindyCode;
typedef long INCode;
typedef std::map<SindyCode, INCode> CodeTable;

INCode Sindy2INCode(const CodeTable& cCodeTable, SindyCode cSindyCode);


// I/N HWY系テキストファイルのコード値などの定義

namespace in {

namespace schema {

namespace hwy_common_field
{
	namespace facil_class
	{
		enum EFacilClass	// 施設種別コードのコード値
		{					// [Bug6309] スマートICと料金所のコード値を交換
			sa			= 1,	// SA
			pa			= 2,	// PA
			jct			= 3,	// JCT
			ramp		= 4,	// ランプ
			ic			= 5,	// IC
			smartic		= 6,	// スマートIC
			tollbooth	= 7,	// 料金所
		};
		extern CodeTable tFacilClass;
	}

	namespace inout
	{
		enum EInOut		// 出入口コードのコード値
		{
			no_inout	= 0, ///< 区別なし
			in			= 1, ///< 入口・合流
			out			= 2, ///< 出口・分岐
		};
		extern CodeTable tInOut;
	}

	namespace facil_class_and_inout_code
	{
		enum EFacilClassAndInOutCode		// 施設の種類と出入口の情報を合わせた「施設種別コード」
		{								// fclpoint.txtの「施設ポイント種別コード」もこれと全く同じ
			sa			= 2,		// SA
			pa			= 3,		// PA
			jct			= 4,		// JCT
			ramp_ent	= 5,		// ランプ入口
			ramp_ext	= 6,		// ランプ出口
			ic_ent		= 7,		// IC入口
			ic_ext		= 8,		// IC出口
			tollgate	= 9,		// 本線料金所
			smartic_ent = 10,		// スマートIC入口
			smartic_ext = 11,		// スマートIC出口
									// roadpoint_normal.txt用
			jct_ent		= 30,		// JCT入路
			jct_ext		= 31		// JCT出路
		};
	}

}	// namespace hwy_common_field


// hwynode.txtとその派生ファイル(hwynode_inout.txt, tollnode.txt, tollguide.txt)
namespace hwynode
{
	using namespace hwy_common_field;

	namespace file_type
	{
		enum EFileType		// 出力ファイルの種類
		{
			hwynode			= 1,
			tollnode		= 2,
			tollguide		= 3,
			hwynode_inout	= 4
		};
	}

	namespace file_format
	{
		enum EFormat						// ファイルフォーマット
		{
			hwynode_smartic_added,			/// スマートIC対応後のフォーマット(5.61以降)
			hwynode_etc_specific_toll,		/// ETC距離別料金対応フォーマット
		};
	}

	namespace toll_func_flags
	{
		enum ETollFuncFlags		// 料金所機能コードのフラグ列の左からの桁数と意味
		{
			ticket,					// 発券所
			adjust,					// 精算所
			alone,					// 単独料金所
			check_ticket,			// 検札所
			alone_invalid_ticket,	// 単独料金無効発券機能
			num_of_tollfunc_value
		};
	}

	namespace facil_pos_code
	{
		enum EPosition					// 施設位置コードのコード値
		{
			ic_ent					= 1,	// IC入路
			ic_ext					= 2,	// IC出路
			sa_ent					= 3,	// SA入路
			sa_ext					= 4,	// SA出路
			pa_ent					= 5,	// PA入路
			pa_ext					= 6,	// PA出路
			jct_ent					= 7,	// JCT入路
			jct_ext					= 8,	// JCT出路
			line_tollgate			= 9,	// 本線料金所
			ramp_ent_tollgate		= 10,	// ランプ上入路料金所
			ramp_ext_tollgate		= 11,	// ランプ上出路料金所
			barrier					= 12,	// バリア
			smartic_ent				= 13,	// スマートIC入口
			smartic_ext				= 14,	// スマートIC出口
											/// 以下は出入口ノード専用
			ent_ramp_origin			= 21,	// 入口ランプ始点
			ext_ramp_end			= 22,	// 出口ランプ終点
			smartic_ent_ramp_origin = 23,	// スマートIC入口ランプ始点
			smartic_ext_ramp_end	= 24	// スマートIC出口ランプ終点

		};
	}

	namespace direction
	{
		enum EDirection	// 上下線コードのコード値
		{
			down	= 0,	// 順方向(下り)
			up		= 1,	// 逆方向(上り)
			nodir	= 2,	// 上下線区別無し
			common	= 3		// 上下線共通
		};
		extern CodeTable tDirection;
	}

	namespace tollclass
	{
		enum ETollClass		// 料金所種別コードのコード値
		{					// [Bug6309] 値と意味の対応を変更
							// SiNDY::TollClass_cの「出入口ノード」は、I/Nでは「料金所以外」に含まれる
			no_tollbooth	= 0,		///< 料金所以外（SiNDYでは分岐合流ノード）
			main_tollbooth	= 1,		///< 本線上料金所
			ramp_tollbooth	= 2,		///< ランプ上料金所
			barrier			= 3,		///< バリア
		};
	}

	namespace etc_facil
	{
		enum EEtcFacil	// ETC施設コードのコード値
		{
			no_etc_facil	= 0,	///< ETC非対応
			gate			= 1,	///< ゲート
			antenna			= 2,	///< アンテナ
		};
	}

}	// namespace hwynode


// roadpoint.txtとその派生ファイル(roadpoint.bunnki.txt, roadpoint_normal.txt)
namespace roadpoint
{
	using namespace hwy_common_field;

/*	namespace file_type
	{
		enum EFileType		// 出力ファイルの種類
		{
			roadpoint_hwy,		/// HWY
			roadpoint_normal,	/// 一般有料（非HWY）
		};
	}
*/
	namespace file_format
	{
		enum EFormat		// ファイルフォーマット
		{
			roadpoint_smartic_added,		/// スマートIC対応後のフォーマット(5.61以降)
			roadpoint_etc_specific_toll,	/// ETC距離別料金対応フォーマット
			fclrep,							/// SiRiUS用（施設代表ポイント）
			fclpoint,						/// SiRiUS用（施設ポイント）
		};
	}

	namespace jct_data_type
	{
		enum EJctDataType	// JCTの合流点のレコードを分岐点のレコードに置き換えたデータであるか否か
		{
			jct_interflow,		/// 合流（通常のデータ）
			jct_branch,			/// 分岐（合流点を分岐点に変えたデータ）
		};
	}

	namespace point_pos_code
	{
		enum EPointPos		// ポイント位置コードのコード値
		{
			entrance	= 0,			// 入口もしくは出口
			up			= 1,			// 上り
			down		= 2,			// 下り
			in			= 3,			// 内回り
			out			= 4,			// 外回り
			west		= 5,			// 西行き
			east		= 6,			// 東行き
			direction	= 7				// ××方面
		};
	}

}	// namespace roadpoint

namespace fclrep
{
	namespace searchclass
	{
		enum EFacilClass	// fclrep.txt用の施設種別コード（検索用施設種別コード）のコード値
		{					// roadpoint.txtの施設種別コードとは、スマートICと料金所のコード値が逆なので注意
			not_for_search	= 0,	// 検索対象外施設
			sa				= 1,	// SA
			pa				= 2,	// PA
			jct				= 3,	// JCT
			ramp			= 4,	// ランプ
			ic				= 5,	// IC
			tollbooth		= 6,	// 料金所
			smartic			= 7,	// スマートIC
		};
		extern CodeTable tSearchClass;
	}

} // namespace fclrep


namespace fclpoint
{
	namespace direction
	{
		enum EDirection	// 上下線コードのコード値
		{				// hwynode.txtの上下線コードとは、コード値の順序が異なるので注意
			nodir	= 0,	// 方向なし
			up		= 1,	// 上り
			down	= 2,	// 下り
			common	= 3		// 上下線共通
		};
		extern CodeTable tDirection;
	}

}	// namespace fclpoint


namespace highway
{
	using namespace hwy_common_field;

	namespace file_format
	{
		enum EFormat
		{
			highway_smartic_added,		/// スマートIC対応後のフォーマット(5.61以降)
			highway_etc_specific_toll,		/// ETC距離別料金対応フォーマット
		};
	}

	namespace direction
	{
		enum EDirection
		{
			fore		= 0,		// 順方向（通常は下り）
			back		= 1			// 逆方向（通常は上り）
		};
	}

}	// namespace highway


namespace road_code_doc
{
	namespace file_type
	{
		enum EFileType
		{
			road_code			= 1,
			ferry_code			= 2,
			road_code_highway	= 3,
			road_code_display	= 4
		};
	}

	namespace file_format
	{
		enum EFormat	/// ファイル間でフォーマットの違いがあるのはSiRiUS用のみ
		{
			roadtype			= 1,
			roadline			= 2
		};
	}

	namespace line_class
	{
		enum ELineClass
		{
			ferry = 1,			// フェリー航路
			street,				// 一般街道名称
			ordinary_tollroad,	// 一般有料道路
			highway,			// 高速道路
			tollroad,			// 有料道路
			metropolis,			// 首都高速道路
			hanshin,			// 阪神高速道路
			nagoya,				// 名古屋都市高速道路
			fukuoka,			// 福岡都市高速道路
			kitakyushu,			// 北九州都市高速道路
			hiroshima,			// 広島高速道路
			kyouto,				// 京都高速道路
		};
		extern CodeTable tLineClass;
		extern CodeTable tLineClassForSiRiUS;
	}

}	// namespace road_code_doc

}	// namespace schema

}	// namespace in

