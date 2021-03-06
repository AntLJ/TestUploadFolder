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
 *	INR[hðæèµ¤wb_
 *	@author	Fusatoshi Abe
 *	@date	2003/09/16		VKì¬
 */

#ifndef	___IN_ATTR_DEF_H___
#define	___IN_ATTR_DEF_H___

#include <string>

namespace in
{
	namespace disp_param
	{
		enum code
		{
			string					= 0x10,			// ¶ñ
			line					= 0x30,			// ü
			area					= 0x50			// Ê
		};
	};

	namespace road
	{
		namespace authenticity
		{
			enum code
			{
				valid			= 1,
				invalid			= 0
			};
		};

		// ¹HÇÒ
		namespace manager
		{
			enum code
			{
				unknown			= 0,					// ¢²¸EìÆ
				jh				= 1,					// ú{¹Höc
				public3			= 2,					// ¹Höc3Ð
				other_public	= 3,					// ¹HöÐ
				country			= 4,					// 
				pref			= 5,					// s¹{§
				city			= 6,					// wès
				other_city		= 7,					// ¼Ìs¬º(Ü23æ)
				others			= 8						// »Ì¼ÌÇÒ
			};
		};

		// NíÊ
		namespace link_class
		{
			enum code
			{
				unknown			= 0,					// ¢²¸EìÆ
				hibunri_honsen	= 1,					// {ü(ãºñª£)
				bunri_honsen	= 2,					// {ü(ãºª£)
				junction		= 3,					// AH({üÔÌnèü)
				crossing		= 4,					// ð·_àN
				ramp			= 5,					// AH(v)N
				side			= 6,					// {üÆ¯êHüÌ¤¹N
				sa				= 7,					// SA¤üN
				bicycle			= 8,					// ©]Ô¹
				connection		= 9,					// {ü¤¹Ú±N
				roundabout		= 10					// Âóð·_N
			};
		};

		// oCpXtO
		namespace bypass
		{
			using namespace authenticity;
		};

		// ©®Ôêp¹HtO
		namespace caronly
		{
			enum code
			{
				unknown			= 0,					// ¢²¸EìÆ
				caronly			= 1,					// ©®Ôêp¹HÉY
				not_caronly		= 2						// ©®Ôêp¹HÉñY
			};
		};

		// ¹Hõ
		namespace width
		{
			enum code
			{
				unknown			= 0,					// ¢²¸EìÆ
				over_13m		= 1,					// õ13.0mÈã
				under_13m		= 2,					// õ5.5m`13.0m¢
				over_3m			= 3,					// õ3.0m`5.5m¢
				under_3m		= 4						// õ3.0m¢
			};
		};

		// ðÊK§íÊR[h
		namespace rest_class
		{
			enum code
			{
				unknown			= 0,					// ¢²¸EìÆ
				no_rest			= 1,					// K§Èµ
				no_passage		= 2,					// ÊsÖ~(ðÈµ)
				no_passage2		= 3,					// ÊsÖ~(ð è)
				oneway_order	= 4,					// êûÊs(³ûüEðÈµ)
				oneway_reverse	= 5,					// êûÊs(tûüEðÈµ)
				oneway_order2	= 6,					// êûÊs(³ûüEð è)
				oneway_reverse2	= 7,					// êûÊs(tûüEð è)
				oneway_both		= 8						// êûÊs(³tØèÖ¦ è)
			};
		};

		// \¦íÊR[h
		namespace road_class
		{
			enum code
			{
				highway2		= 0x05001000,			// ssÔ¬¹H
				highway			= 0x05001100,			// ssÔ¬¹H(L¿)
				city_highway	= 0x05002100,			// ss¬¹H(L¿)
				kokudo			= 0x05003000,			// êÊ¹
				kokudo2			= 0x05003100,			// êÊ¹(L¿)
				pref_mlocal		= 0x05004000,			// åvnû¹(s{§)
				pref_mlocal2	= 0x05004100,			// åvnû¹(s{§)(L¿)
				city_mlocal		= 0x05005000,			// åvnû¹(wès)
				city_mlocal2	= 0x05005100,			// åvnû¹(wès)(L¿)
				pref_road		= 0x05006000,			// êÊs¹{§¹
				pref_road2		= 0x05006100,			// êÊs¹{§¹(L¿)
				city_road		= 0x05007000,			// wèsÌêÊs¹
				city_road2		= 0x05007100,			// wèsÌêÊs¹(L¿)
				other_road		= 0x05009000,			// »Ì¼Ì¹H
				other_road2		= 0x05009100,			// »Ì¼Ì¹H(L¿)
				other_road3		= 0x05008000,			// iº°»Ì¼¹
				detail_road		= 0x05009010,			// ×XH1
				detail_road2	= 0x05009020,			// ×XH2
				detail_road3	= 0x05009030,			// ×XH3
				dummy_link		= 0x0500d000,			// _~[N
				dummy_link2		= 0x0500d010,			// ëH
				dummy_link3		= 0x0500d020,			// }b`OpN
				dummy_link4		= 0x0500d030,			// PECp}b`ON
				dummy_link5		= 0x0500d100,			// BTp}b`ON
				ferry			= 0x0500f100,			// tF[qH
				parking			= 0x0500e100,			// Ôêà¹H
				parking_shape	= 0x0500e000			// ÔêO`
			};
		};

		// oHíÊR[h
		namespace navi_class
		{
			enum code
			{
				highway			= 0x05001100,			// ssÔ¬¹H
				city_highway	= 0x05002100,			// ss¬¹H
				kokudo			= 0x05003000,			// êÊ¹
				kokudo2			= 0x05003100,			// êÊ¹(L¿)
				pref_mlocal		= 0x05004000,			// åvnû¹(s{§)
				pref_mlocal2	= 0x05004100,			// åvnû¹(s{§)(L¿)
				city_mlocal		= 0x05005000,			// åvnû¹(wès)
				city_mlocal2	= 0x05005100,			// åvnû¹(wès)(L¿)
				pref_road		= 0x05006000,			// êÊs¹{§¹
				pref_road2		= 0x05006100,			// êÊs¹{§¹(L¿)
				city_road		= 0x05007000,			// wèsÌêÊs¹
				city_road2		= 0x05007100,			// wèsÌêÊs¹(L¿)
				other_road		= 0x05009000,			// »Ì¼Ì¹H
				other_road2		= 0x05009100,			// »Ì¼Ì¹H(L¿)
				other_road3		= 0x05008000,			// iº°»Ì¼¹
				detail_road		= 0x05009010,			// ×XH1
				detail_road2	= 0x05009020,			// ×XH2
				detail_road3	= 0x05009030,			// ×XH3
				dummy_link3		= 0x0500d020,			// }b`OpN
				dummy_link4		= 0x0500d030,			// PECp}b`ON
				dummy_link5		= 0x0500d100,			// BTp}b`ON
				ferry			= 0x0500f100,			// tF[qH
				parking			= 0x0500e100,			// Ôêà¹H
				connectway		= 0x0500c000,			// Ú±¹H
				kkd_route		= 0x0500b000			// KKDoHU±sKæÔ
			};
		};

		// LætO
		namespace wide_area
		{
			enum code
			{
				not_wide_area	= 0x00000000,			// LæÉ¶ÝµÈ¢
				wide_area		= 0x00000001,			// LæÉ¶Ý·é
			};
		};

		// ÔêíÊ
		namespace parking_class
		{
			enum code
			{
				unknown			= 0,					// s¾
				solid			= 1,					// §ÌÔê
				under			= 2,					// nºÔê
				both			= 3,					// §Ì©ÂnºÔê
				tower			= 5,					// ^[p[LO
				auto_tower		= 11,					// ©®ñ]^[p[LO
				flat			= 15					// ½ÊÔê
			};
		};

		// ÔêNíÊ
		namespace parkinglink_class
		{
			enum code
			{
				unknown			= 0,					// ¢²¸EìÆ
				enter			= 1,					// üûN
				exit			= 2,					// oûN
				entrance		= 3,					// oüûN
				premises		= 4,					// \à¹HN
				junction		= 5						// xÔÌnèN
			};
		};

		// VICSæª
		namespace vics_class
		{
			enum code
			{
				highway			= 1,					// ¬¹H
				city_highway	= 2,					// ss¬¹H
				street			= 3,					// êÊ¹H
				others			= 4						// »Ì¼
			};
		};

		// XÎtO
		namespace grad
		{
			enum code
			{
				unknown			= 0,					// ¢²¸
				up				= 1,					// ãè
				down			= 2,					// ºè
				flat			= 3						// ½R
			};
		};

		// Nà®«R[h
		namespace segment_attr
		{
			enum code
			{
				tollgate			= 0x00000040,		// ¿à
				dirtroad			= 0x00000020,		// ¢Ü
				underpass			= 0x00000010,		// A_[pX
				crossing			= 0x00000008,		// ¥Ø
				doumon				= 0x00000004,		// ´å
				tunnel				= 0x00000002,		// gl
				bridge				= 0x00000001		// ´EË
			};

		};

		// m[híÊ
		namespace node_class
		{
			enum code
			{
				unknown				= 0x01000000,		// ¢²¸EìÆ
				crossing			= 0x01000001,		// ð·_m[h
				terminate			= 0x01000002,		// s«~Üè_m[h
				dummy				= 0x01000003,		// _~[m[h
				edge				= 0x01000004,		// ææÏX_m[h
				attr_change			= 0x01000005		// ®«Ï»_m[h
			};
		};

		// M@tO
		namespace signal
		{
			using namespace authenticity;
		};

		// ¹H³LøE³øtO
		namespace height
		{
			using namespace authenticity;
		};

		// ðÊK§ðR[h
		namespace rest_cond
		{
			enum code
			{
				unknown					= 0,			// ¢²¸EìÆ
				car						= 1,			// ÔíÌÝ
				time					= 2,			// ÌÝ
				day						= 3,			// júÌÝ
				car_and_time			= 4,			// ÔíyÑ
				car_and_day				= 5,			// ÔíyÑjú
				time_and_day			= 6,			// yÑjú
				all						= 7,			// ÔíAyÑjú
				others					= 8				// »Ì¼Ìð
			};
		};

		// m[hK§íÊ
		namespace turn_class
		{
			enum code
			{
				turn_class				= 0x01000000	// m[hK§íÊ
			};
		}

		// júwèR[h
		namespace dayofweek
		{
			enum code
			{
				unknown					= 0,			// ¢²¸EìÆ
				holiday					= 1,			// úExú
				except_holiday			= 2,			// úExúð­
				public_holiday			= 3,			// xú(újÍ­)
				except_public_holiday	= 4,			// xúð­
				weekday					= 5,			// ½ú
				sunday					= 6,			// új
				monday					= 7,			// j
				tuesday					= 8,			// Îj
				wednesday				= 9,			// j
				thursday				= 10,			// Øj
				friday					= 11,			// àj
				saturday				= 12,			// yj
				weekend					= 13,			// yúExú
				except_weekend			= 14,			// yúExúð­
				allday					= 15,			// Sjú
				except_sunday			= 16,			// újúð­
				pre_holiday				= 17			// úExúÌOú
			};
		};

		// ð·_¼ÌíÊR[h
		namespace crossname_class
		{
			enum code
			{
				crossname				= 0x20015000
			};
		};

		// K§íÊ
		namespace regulation_class
		{
			enum code
			{
				unknown					= 0,			// ¢²¸EìÆ
				link					= 1,			// NK§
				node					= 2,			// m[hK§
				node_array				= 3				// m[hñK§
			};
		};

		// K§ð
		namespace regulation_cond
		{
			enum code
			{
				unknown					= 0,			// ¢²¸
				no_condition			= 1,			// ÊíK§(ðÈµ)
				with_condition			= 2				// ðt«K§
			};
		};

		// ÊsK§
		namespace passage_regulation
		{
			enum code
			{
				no_regulation			= 1,			// K§Èµ
				no_passage				= 2,			// ÊsÖ~
				oneway_order			= 4,			// êûÊs(³ûü)
				oneway_reverse			= 5,			// êûÊs(tûü)
				bad_passage				= 6,			// ÊssÂ
				relation_passage		= 7,			// ÊsÖ~(ÖWÒÈO)
				etc_only				= 8				// ETCêp
			};
		};

		// m[hîñíÊR[h
		namespace node_info_class
		{
			enum code
			{
				crossname_nodir			= 0x20015001,	// ð·_¼Ì(ûü«Èµ)
				crossname_dir			= 0x20015002,	// ð·_¼Ì(ûü« è)
				highway_ic_entrance		= 0x01000010,	// ¬¹HICüûm[h(êÊ¹©çÌüHm[h)
				highway_ic_enter		= 0x01000011,	// ¬¹HICüH
				highway_ic_exit			= 0x01000012,	// ¬¹HICoH
				jct_enter				= 0x01000041,	// üHJCTm[h
				jct_exit				= 0x01000042,	// oHJCTm[h
				sapa_enter				= 0x01000051,	// SAPAüH
				sapa_exit				= 0x01000052,	// SAPAoH
				highway_dirguide		= 0x01000060,	// ûÊÄà(¬¹Hã)
				general_dirguide		= 0x01000070,	// ÄàÅÂ(êÊ¹ûÊÄà)
				noguide					= 0x01000080,	// m[KCh
				guide_code				= 0x01000090,	// ­§U±(R[h)
				guide_string			= 0x01000091,	// ­§U±(¶ñ)
				illust_file				= 0x01000101,	// CXgt@C¼(U±êp)
				illust_file2			= 0x01000102,	// CXgt@C¼(nCEFC[hêp)
				illust_file3			= 0x01000103	// CXgt@C¼(U±AnCEFC[hp)
			};
		};

		// næíÊ
		namespace dirguide_class
		{
			enum code
			{
				general					= 1,			// næ(êÊ)
				highway					= 2,			// næ(L¿)
				kokudo_no				= 3,			// ¹Ô
				kendo_no				= 4,			// §¹Ô
				others					= 5				// »Ì¼
			};
		};

		// ­§U±
		namespace guide_class
		{
			enum code
			{
				noguide					= 0x00,			// ¹Èè(ûÊÄàÈµ)
				straight				= 0x01,			// ¼i(12ûü)
				right_front				= 0x02,			// EOûü(1ûü)
				right_ahead				= 0x03,			// EèOûü(2ûü)
				right					= 0x04,			// EÜ(3ûü)
				right_behind			= 0x05,			// EÎßãëûü(4ûü)
				right_back				= 0x06,			// Eßéûü(5ûü)
				back					= 0x07,			// ßéûü(6ûü)
				left_back				= 0x08,			// ¶ßéûü(7ûü)
				left_behind				= 0x09,			// ¶Îßãëûü(8ûü)
				left					= 0x0a,			// ¶Ü(9ûü)
				left_ahead				= 0x0b,			// ¶èOûü(10ûü)
				left_front				= 0x0c			// ¶Oûü(11ûü)
			};
		};

		// Ng£CL[R[h
		namespace link_info_class
		{
			enum code
			{
				kubun					= 1,			// å]æªR[h
				speed					= 2,			// K§¬xR[h
				trip					= 3,			// ·s¬x
				island					= 4,			// etO
				area					= 6,			// ×XHGA»ÊtO
				lane					= 7,			// Ð¤Ôü
			};
		};

		// L[ID
		namespace lext_key
		{
			enum code
			{
				kubun_code				= 1,			// å]æªR[h
				limit_speed				= 2,			// K§¬x
				trip_speed				= 3,			// ·s¬x
				island_flag				= 4,			// etO
				area_flag				= 6,			// ×XHGAtO
				oneside_lane			= 7,			// Ð¤Ôü
			};
		};

		// å]æªR[h
		namespace kubun_code
		{
			enum code
			{
				unknown					= 0,			// ¢²¸EìÆ
				main_line				= 1,			// åHü
				sub_line				= 2				// ]Hü
			};
		};

		// K§¬x
		namespace limit_speed
		{
			enum code
			{
				unknown					= 0,			// ¢²¸
				limit_under30km			= 1,			// 30km/h¢
				limit_40km				= 2,			// 40km/h
				limit_50km				= 3,			// 50km/h
				limit_60km				= 4,			// 60km/h
				limit_70km				= 5,			// 70km/h
				limit_80km				= 6,			// 80km/h
				limit_100km				= 7				// 100km/h
			};
		};

		// tO
		namespace island_flag
		{
			using namespace authenticity;
		};

		// ×XHGAtO
		namespace area_flag
		{
			enum code
			{
				unknown					= 0x00000000,	// ¢²¸
				year_1998				= 0x00000001,	// 1998Nx
				year_1999				= 0x00000002,	// 1999Nx
				year_2000_spr			= 0x00000004,	// 2000Nxt
				year_2000_aut			= 0x00000008,	// 2000NxH
				year_2001				= 0x00000010,	// 2001Nx
				year_2002				= 0x00000020,	// 2002Nx
				year_2003				= 0x00000040,	// 2003Nx
				year_2004				= 0x00000080,	// 2004Nx
				year_2005				= 0x00000100,	// 2005Nx
				year_2006				= 0x00000200,	// 2006Nx
				year_2007				= 0x00000400	// 2007Nx
			};

			enum kisei_chousa
			{
				no_rest					= 0x00000000,	// K§²¸Èµ
				rest					= 0x80000000	// K§²¸ è
			};
		};
	};
};

#endif	//___IN_ATTR_DEF_H___
