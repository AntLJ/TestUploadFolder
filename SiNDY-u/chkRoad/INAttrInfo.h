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

namespace in
{
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

		// ìπòHä«óùé“
		namespace manager
		{
			enum code
			{
				unknown			= 0,					///< ñ¢í≤ç∏ÅEçÏã∆íÜ
				jh				= 1,					///< çÇë¨ìπòHâÔé–
		//		public3			= 2,					///< ìπòHåˆíc3é–(îpé~)
				other_public	= 3,					///< ìπòHåˆé–
				country			= 4,					///< çë
				pref			= 5,					///< ìsìπï{åß
				city			= 6,					///< éwíËés
				other_city		= 7,					///< ëºÇÃésí¨ë∫(ä‹ìåãû23ãÊ)
				others			= 8						///< ÇªÇÃëºÇÃä«óùé“
			};
		};

		// ÉäÉìÉNéÌï 
		namespace link_class
		{
			enum code
			{
				unknown			= 0,					///< ñ¢í≤ç∏ÅEçÏã∆íÜ
				hibunri_honsen	= 1,					///< ñ{ê¸(è„â∫îÒï™ó£)
				bunri_honsen	= 2,					///< ñ{ê¸(è„â∫ï™ó£)
				junction		= 3,					///< òAåãòH(ñ{ê¸ä‘ÇÃìnÇËê¸)
				crossing		= 4,					///< åç∑ì_ì‡ÉäÉìÉN
				ramp			= 5,					///< òAåãòH(ÉâÉìÉv)ÉäÉìÉN
				side			= 6,					///< ñ{ê¸Ç∆ìØàÍòHê¸ÇÃë§ìπÉäÉìÉN
				sa				= 7,					///< SAìôë§ê¸ÉäÉìÉN
				bicycle			= 8,					///< é©ì]é‘ìπìô
				connection		= 9,					///< ñ{ê¸ë§ìπê⁄ë±ÉäÉìÉN
				roundabout		= 10					///< ä¬èÛåç∑ì_ÉäÉìÉN
			};
		};

		// ÉoÉCÉpÉXÉtÉâÉO
		namespace bypass
		{
			using namespace authenticity;
		};

		// é©ìÆé‘êÍópìπòHÉtÉâÉO
		namespace caronly
		{
			enum code
			{
				unknown			= 0,					///< ñ¢í≤ç∏ÅEçÏã∆íÜ
				caronly			= 1,					///< é©ìÆé‘êÍópìπòHÇ…äYìñ
				not_caronly		= 2						///< é©ìÆé‘êÍópìπòHÇ…îÒäYìñ
			};
		};

		// ìπòHïùàı
		namespace width
		{
			enum code
			{
				unknown			= 0,					///< ñ¢í≤ç∏ÅEçÏã∆íÜ
				over_13m		= 1,					///< ïùàı13.0mà»è„
				under_13m		= 2,					///< ïùàı5.5mÅ`13.0mñ¢ñû
				over_3m			= 3,					///< ïùàı3.0mÅ`5.5mñ¢ñû
				under_3m		= 4						///< ïùàı3.0mñ¢ñû
			};
		};

		// é‘ê¸êî
		namespace lane
		{
			enum code 
			{
				unknown			= 0,					///< ñ¢í≤ç∏ÅEçÏã∆íÜ
				one				= 1,					///< 1é‘ê¸
				two				= 2,					///< 2é‘ê¸
				three			= 3,					///< 3é‘ê¸
				four			= 4,					///< 4é‘ê¸
				five			= 5,					///< 5é‘ê¸
				six				= 6,					///< 6é‘ê¸
				seven			= 7,					///< 7é‘ê¸
				eight			= 8,					///< 8é‘ê¸
				nine			= 9,					///< 9é‘ê¸
				ten				= 10,					///< 10é‘ê¸
				eleven			= 11,					///< 11é‘ê¸
				twelve			= 12,					///< 12é‘ê¸
				thirteen		= 13,					///< 13é‘ê¸
				forteen			= 14,					///< 14é‘ê¸
				fifteen			= 15					///< 15é‘ê¸
			};
		};

		// è„â∫ä÷åW
		namespace updown
		{
			enum code
			{
				up1				= 1,					///< è„ÅEïΩçs
				up2				= 2,					///< è„ÅEñcÇÁÇÒÇ≈Ç¢ÇÈ
				down1			= 3,					///< â∫ÅEïΩçs
				down2			= 4,					///< â∫ÅEÇ÷Ç±ÇÒÇ≈Ç¢ÇÈ
				unknown			= 0						///< ñ¢í≤ç∏
			};
		};

		// åí ãKêßéÌï ÉRÅ[Éh
		namespace rest_class
		{
			enum code
			{
				unknown			= 0,					///< ñ¢í≤ç∏ÅEçÏã∆íÜ
				no_rest			= 1,					///< ãKêßÇ»Çµ
				no_passage		= 2,					///< í çsã÷é~(èåèÇ»Çµ)
				no_passage2		= 3,					///< í çsã÷é~(èåèÇ†ÇË)
				oneway_order	= 4,					///< àÍï˚í çs(ê≥ï˚å¸ÅEèåèÇ»Çµ)
				oneway_reverse	= 5,					///< àÍï˚í çs(ãtï˚å¸ÅEèåèÇ»Çµ)
				oneway_order2	= 6,					///< àÍï˚í çs(ê≥ï˚å¸ÅEèåèÇ†ÇË)
				oneway_reverse2	= 7,					///< àÍï˚í çs(ãtï˚å¸ÅEèåèÇ†ÇË)
				oneway_both		= 8						///< àÍï˚í çs(ê≥ãtêÿÇËë÷Ç¶Ç†ÇË)
			};
		};

		// ï\é¶éÌï ÉRÅ[Éh
		namespace road_class
		{
			enum code
			{
				highway2		= 0x05001000,			///< ìsésä‘çÇë¨ìπòH
				highway			= 0x05001100,			///< ìsésä‘çÇë¨ìπòH(óLóø)
				city_highway	= 0x05002100,			///< ìsésçÇë¨ìπòH(óLóø)
				kokudo			= 0x05003000,			///< àÍî çëìπ
				kokudo2			= 0x05003100,			///< àÍî çëìπ(óLóø)
				pref_mlocal		= 0x05004000,			///< éÂóvínï˚ìπ(ìsï{åß)
				pref_mlocal2	= 0x05004100,			///< éÂóvínï˚ìπ(ìsï{åß)(óLóø)
				city_mlocal		= 0x05005000,			///< éÂóvínï˚ìπ(éwíËés)
				city_mlocal2	= 0x05005100,			///< éÂóvínï˚ìπ(éwíËés)(óLóø)
				pref_road		= 0x05006000,			///< àÍî ìsìπï{åßìπ
				pref_road2		= 0x05006100,			///< àÍî ìsìπï{åßìπ(óLóø)
				city_road		= 0x05007000,			///< éwíËésÇÃàÍî ésìπ
				city_road2		= 0x05007100,			///< éwíËésÇÃàÍî ésìπ(óLóø)
				other_road		= 0x05009000,			///< ÇªÇÃëºÇÃìπòH
				other_road2		= 0x05009100,			///< ÇªÇÃëºÇÃìπòH(óLóø)
				other_road3		= 0x05008000,			///< äiâ∫Ç∞ÇªÇÃëºìπ
				detail_road		= 0x05009010,			///< ç◊äXòH1
				detail_road2	= 0x05009020,			///< ç◊äXòH2
				detail_road3	= 0x05009030,			///< ç◊äXòH3
				dummy_link		= 0x0500d000,			///< É_É~Å[ÉäÉìÉN
				dummy_link2		= 0x0500d010,			///< íÎâÄòH
				dummy_link3		= 0x0500d020,			///< É}ÉbÉ`ÉìÉOópÉäÉìÉN
				dummy_link4		= 0x0500d030,			///< PECópÉ}ÉbÉ`ÉìÉOÉäÉìÉN
				dummy_link5		= 0x0500d100,			///< BTópÉ}ÉbÉ`ÉìÉOÉäÉìÉN
				ferry			= 0x0500f100,			///< ÉtÉFÉäÅ[çqòH
				parking			= 0x0500e100,			///< íìé‘èÍì‡ìπòH
				parking_shape	= 0x0500e000			///< íìé‘èÍäOå`
			};
		};

		// åoòHéÌï ÉRÅ[Éh
		namespace navi_class
		{
			enum code
			{
				highway			= 0x05001100,			///< ìsésä‘çÇë¨ìπòH
				city_highway	= 0x05002100,			///< ìsésçÇë¨ìπòH
				kokudo			= 0x05003000,			///< àÍî çëìπ
				kokudo2			= 0x05003100,			///< àÍî çëìπ(óLóø)
				pref_mlocal		= 0x05004000,			///< éÂóvínï˚ìπ(ìsï{åß)
				pref_mlocal2	= 0x05004100,			///< éÂóvínï˚ìπ(ìsï{åß)(óLóø)
				city_mlocal		= 0x05005000,			///< éÂóvínï˚ìπ(éwíËés)
				city_mlocal2	= 0x05005100,			///< éÂóvínï˚ìπ(éwíËés)(óLóø)
				pref_road		= 0x05006000,			///< àÍî ìsìπï{åßìπ
				pref_road2		= 0x05006100,			///< àÍî ìsìπï{åßìπ(óLóø)
				city_road		= 0x05007000,			///< éwíËésÇÃàÍî ésìπ
				city_road2		= 0x05007100,			///< éwíËésÇÃàÍî ésìπ(óLóø)
				other_road		= 0x05009000,			///< ÇªÇÃëºÇÃìπòH
				other_road2		= 0x05009100,			///< ÇªÇÃëºÇÃìπòH(óLóø)
				other_road3		= 0x05008000,			///< äiâ∫Ç∞ÇªÇÃëºìπ
				detail_road		= 0x05009010,			///< ç◊äXòH1
				detail_road2	= 0x05009020,			///< ç◊äXòH2
				detail_road3	= 0x05009030,			///< ç◊äXòH3
				dummy_link3		= 0x0500d020,			///< É}ÉbÉ`ÉìÉOópÉäÉìÉN
				dummy_link4		= 0x0500d030,			///< PECópÉ}ÉbÉ`ÉìÉOÉäÉìÉN
				dummy_link5		= 0x0500d100,			///< BTópÉ}ÉbÉ`ÉìÉOÉäÉìÉN
				ferry			= 0x0500f100,			///< ÉtÉFÉäÅ[çqòH
				parking			= 0x0500e100,			///< íìé‘èÍì‡ìπòH
				connectway		= 0x0500c000,			///< ê⁄ë±ìπòH
				kkd_route		= 0x0500b000			///< KKDåoòHóUì±ïsìKãÊä‘
			};
		};

		// çLàÊÉtÉâÉO
		namespace wide_area
		{
			enum code
			{
#if IN_VERSION >= 590
				not_wide_area	= 0x00000000,			///< çLàÊÇ…ë∂ç›ÇµÇ»Ç¢
				wide_area		= 0x00000001,			///< çLàÊÇ…ë∂ç›Ç∑ÇÈ
#else
				not_wide_area	= 0x00000000,			///< çLàÊÇ…ë∂ç›ÇµÇ»Ç¢
				wide_area1		= 0x00000001,			///< çLàÊ1
				wide_area2		= 0x00000002			///< çLàÊ2
#endif
			};
		};

		// íìé‘èÍéÌï 
		namespace parking_class
		{
			enum code
			{
				unknown			= 0,					///< ïsñæ
				solid			= 1,					///< óßëÃíìé‘èÍ
				under			= 2,					///< ínâ∫íìé‘èÍ
				both			= 3,					///< óßëÃÇ©Ç¬ínâ∫íìé‘èÍ
				tower			= 5,					///< É^ÉèÅ[ÉpÅ[ÉLÉìÉO
				auto_tower		= 11,					///< é©ìÆâÒì]É^ÉèÅ[ÉpÅ[ÉLÉìÉO
				flat			= 15					///< ïΩñ íìé‘èÍ
			};
		};

		// íìé‘èÍÉäÉìÉNéÌï 
		namespace parkinglink_class
		{
			enum code
			{
				unknown			= 0,					///< ñ¢í≤ç∏ÅEçÏã∆íÜ
				enter			= 1,					///< ì¸å˚ÉäÉìÉN
				exit			= 2,					///< èoå˚ÉäÉìÉN
				entrance		= 3,					///< èoì¸å˚ÉäÉìÉN
				premises		= 4,					///< ç\ì‡ìπòHÉäÉìÉN
				junction		= 5						///< ÉåÉxÉãä‘ÇÃìnÇËÉäÉìÉN
			};
		};

		// VICSãÊï™
		namespace vics_class
		{
			enum code
			{
				highway			= 1,					///< çÇë¨ìπòH
				city_highway	= 2,					///< ìsésçÇë¨ìπòH
				street			= 3,					///< àÍî ìπòH
				others			= 4						///< ÇªÇÃëº
			};
		};

		// ÉäÉìÉNå`èÛì_ëÆê´ÅiåXéŒÉtÉâÉOÅj
		namespace grad
		{
			enum code
			{
				unknown			= 0,					///< ñ¢í≤ç∏
				up				= 1,					///< è„ÇË
				down			= 2,					///< â∫ÇË
				flat			= 3						///< ïΩíR
			};
		};

		// ÉäÉìÉNå`èÛì_ëÆê´ÅiÉäÉìÉNì‡ëÆê´Åj
		namespace linkpoint_attr
		{
			enum code
			{
				deformseg			= 0x00000080,		///< ÉfÉtÉHÉãÉÅÉZÉOÉÅÉìÉg
#if IN_VERSION >= 550
				dirtroad			= 0x00000020,		///< ñ¢ï‹ëï
#else
				tollgate			= 0x00000040,		///< óøã‡èä
				walkbridge			= 0x00000020,		///< ï‡ìπã¥
#endif
				underpass			= 0x00000010,		///< ÉAÉìÉ_Å[ÉpÉX
				crossing			= 0x00000008,		///< ì•êÿ
				doumon				= 0x00000004,		///< ì¥ñÂìô
				tunnel				= 0x00000002,		///< ÉgÉìÉlÉã
				bridge				= 0x00000001		///< ã¥ÅEçÇâÀ
			};

		};

		// ÉmÅ[ÉhéÌï 
		namespace node_class
		{
			enum code
			{
				unknown				= 0x01000000,		///< ñ¢í≤ç∏ÅEçÏã∆íÜ
				crossing			= 0x01000001,		///< åç∑ì_ÉmÅ[Éh
				terminate			= 0x01000002,		///< çsÇ´é~Ç‹ÇËì_ÉmÅ[Éh
				dummy				= 0x01000003,		///< É_É~Å[ÉmÅ[Éh
				edge				= 0x01000004,		///< ãÊâÊïœçXì_ÉmÅ[Éh
				attr_change			= 0x01000005		///< ëÆê´ïœâªì_ÉmÅ[Éh
			};
		};

		// êMçÜã@ÉtÉâÉO
		namespace signal
		{
			using namespace authenticity;
		};

		// ìπòHçÇÇ≥óLå¯ÅEñ≥å¯ÉtÉâÉO
		namespace height
		{
			using namespace authenticity;
		};

		// åí ãKêßèåèÉRÅ[Éh
		namespace rest_cond
		{
			enum code
			{
				unknown					= 0,			///< ñ¢í≤ç∏ÅEçÏã∆íÜ
				car						= 1,			///< é‘éÌÇÃÇ›
				time					= 2,			///< éûçèÇÃÇ›
				day						= 3,			///< ójì˙ÇÃÇ›
				car_and_time			= 4,			///< é‘éÌãyÇ—éûçè
				car_and_day				= 5,			///< é‘éÌãyÇ—ójì˙
				time_and_day			= 6,			///< éûçèãyÇ—ójì˙
				all						= 7,			///< é‘éÌÅAéûçèãyÇ—ójì˙
				others					= 8				///< ÇªÇÃëºÇÃèåè
			};
		};

		// ÉmÅ[ÉhãKêßéÌï 
		namespace turn_class
		{
			enum code
			{
				turn_class				= 0x01000000	///< ÉmÅ[ÉhãKêßéÌï 
			};
		}

		// ójì˙éwíËÉRÅ[Éh
		namespace dayofweek
		{
			enum code
			{
				unknown					= 0,			///< ñ¢í≤ç∏ÅEçÏã∆íÜ
				holiday					= 1,			///< ì˙ÅEãxì˙
				except_holiday			= 2,			///< ì˙ÅEãxì˙ÇèúÇ≠
				public_holiday			= 3,			///< ãxì˙(ì˙ójÇÕèúÇ≠)
				except_public_holiday	= 4,			///< ãxì˙ÇèúÇ≠
				weekday					= 5,			///< ïΩì˙
				sunday					= 6,			///< ì˙ój
				monday					= 7,			///< åéój
				tuesday					= 8,			///< âŒój
				wednesday				= 9,			///< êÖój
				thursday				= 10,			///< ñÿój
				friday					= 11,			///< ã‡ój
				saturday				= 12,			///< ìyój
				weekend					= 13,			///< ìyì˙ÅEãxì˙
				except_weekend			= 14,			///< ìyì˙ÅEãxì˙ÇèúÇ≠
				allday					= 15,			///< ëSójì˙
				except_sunday			= 16,			///< ì˙ójì˙ÇèúÇ≠
				pre_holiday				= 17			///< ì˙ÅEãxì˙ÇÃëOì˙
			};
		};

		// åç∑ì_ñºèÃéÌï ÉRÅ[Éh
		namespace crossname_class
		{
			enum code
			{
				crossname				= 0x20015000
			};
		};

		// ãKêßéÌï 
		namespace regulation_class
		{
			enum code
			{
				unknown					= 0,			///< ñ¢í≤ç∏ÅEçÏã∆íÜ
				link					= 1,			///< ÉäÉìÉNãKêß
				node					= 2,			///< ÉmÅ[ÉhãKêß
				node_array				= 3				///< ÉmÅ[ÉhóÒãKêß
			};
		};

		// ãKêßèåè
		namespace regulation_cond
		{
			enum code
			{
				unknown					= 0,			///< ñ¢í≤ç∏
				no_condition			= 1,			///< í èÌãKêß(èåèÇ»Çµ)
				with_condition			= 2				///< èåèïtÇ´ãKêß
			};
		};

		// í çsãKêß
		namespace passage_regulation
		{
			enum code
			{
				no_regulation			= 1,			///< ãKêßÇ»Çµ
				no_passage				= 2,			///< í çsã÷é~
				oneway_order			= 4,			///< àÍï˚í çs(ê≥ï˚å¸)
				oneway_reverse			= 5,			///< àÍï˚í çs(ãtï˚å¸)
				bad_passage				= 6,			///< í çsïsâ¬
				relation_passage		= 7,			///< í çsã÷é~(ä÷åWé“à»äO)
				etc_only				= 8				///< ETCêÍóp
			};
		};

		// ÉmÅ[ÉhèÓïÒéÌï ÉRÅ[Éh
		namespace node_info_class
		{
			enum code
			{
				crossname_nodir			= 0x20015001,	///< åç∑ì_ñºèÃ(ï˚å¸ê´Ç»Çµ)
				crossname_dir			= 0x20015002,	///< åç∑ì_ñºèÃ(ï˚å¸ê´Ç†ÇË)
				highway_ic_entrance		= 0x01000010,	///< çÇë¨ìπòHICì¸å˚ÉmÅ[Éh(àÍî ìπÇ©ÇÁÇÃì¸òHÉmÅ[Éh)
				highway_ic_enter		= 0x01000011,	///< çÇë¨ìπòHICì¸òH
				highway_ic_exit			= 0x01000012,	///< çÇë¨ìπòHICèoòH
				jct_enter				= 0x01000041,	///< ì¸òHJCTÉmÅ[Éh
				jct_exit				= 0x01000042,	///< èoòHJCTÉmÅ[Éh
				sapa_enter				= 0x01000051,	///< SAPAì¸òH
				sapa_exit				= 0x01000052,	///< SAPAèoòH
				highway_dirguide		= 0x01000060,	///< ï˚ñ àƒì‡(çÇë¨ìπòHè„)
				general_dirguide		= 0x01000070,	///< àƒì‡ä≈î¬(àÍî ìπï˚ñ àƒì‡)
				noguide					= 0x01000080,	///< ÉmÅ[ÉKÉCÉh
				guide_code				= 0x01000090,	///< ã≠êßóUì±(ÉRÅ[Éh)
				guide_string			= 0x01000091,	///< ã≠êßóUì±(ï∂éöóÒ)
				illust_file				= 0x01000101,	///< ÉCÉâÉXÉgÉtÉ@ÉCÉãñº(óUì±êÍóp)
				illust_file2			= 0x01000102,	///< ÉCÉâÉXÉgÉtÉ@ÉCÉãñº(ÉnÉCÉEÉFÉCÉÇÅ[ÉhêÍóp)
				illust_file3			= 0x01000103	///< ÉCÉâÉXÉgÉtÉ@ÉCÉãñº(óUì±ÅAÉnÉCÉEÉFÉCÉÇÅ[Éhóp)
			};
		};

		// ínêÊéÌï 
		namespace dirguide_class
		{
			enum code
			{
				general					= 1,			///< ínêÊ(àÍî )
				highway					= 2,			///< ínêÊ(óLóø)
				kokudo_no				= 3,			///< çëìπî‘çÜ
				kendo_no				= 4,			///< åßìπî‘çÜ
				others					= 5				///< ÇªÇÃëº
			};
		};

		// ã≠êßóUì±
		namespace guide_class
		{
			enum code
			{
				noguide					= 0x00,			///< ìπÇ»ÇË(ï˚ñ àƒì‡Ç»Çµ)
				straight				= 0x01,			///< íºêi(12éûï˚å¸)
				right_front				= 0x02,			///< âEëOï˚å¸(1éûï˚å¸)
				right_ahead				= 0x03,			///< âEéËëOï˚å¸(2éûï˚å¸)
				right					= 0x04,			///< âEê‹(3éûï˚å¸)
				right_behind			= 0x05,			///< âEéŒÇﬂå„ÇÎï˚å¸(4éûï˚å¸)
				right_back				= 0x06,			///< âEñﬂÇÈï˚å¸(5éûï˚å¸)
				back					= 0x07,			///< ñﬂÇÈï˚å¸(6éûï˚å¸)
				left_back				= 0x08,			///< ç∂ñﬂÇÈï˚å¸(7éûï˚å¸)
				left_behind				= 0x09,			///< ç∂éŒÇﬂå„ÇÎï˚å¸(8éûï˚å¸)
				left					= 0x0a,			///< ç∂ê‹(9éûï˚å¸)
				left_ahead				= 0x0b,			///< ç∂éËëOï˚å¸(10éûï˚å¸)
				left_front				= 0x0c			///< ç∂ëOï˚å¸(11éûï˚å¸)
			};
		};

		// ÉäÉìÉNägí£ÉåÉCÉÑÉLÅ[ÉRÅ[Éh
		namespace link_info_class
		{
			enum code
			{
				kubun					= 1,			///< éÂè]ãÊï™ÉRÅ[Éh
				speed					= 2,			///< ãKêßë¨ìxÉRÅ[Éh
				trip					= 3,			///< ó∑çsë¨ìx
				island					= 4,			///< ïÇìáãñóeÉtÉâÉO
				area					= 6,			///< ç◊äXòHÉGÉäÉAîªï ÉtÉâÉO
				lane					= 7,			///< ï–ë§é‘ê¸êî
			};
		};

		// ÉLÅ[ID
		namespace lext_key
		{
			enum code
			{
				kubun_code				= 1,			///< éÂè]ãÊï™ÉRÅ[Éh
				limit_speed				= 2,			///< ãKêßë¨ìx
				trip_speed				= 3,			///< ó∑çsë¨ìx
				island_flag				= 4,			///< ïÇìáãñóeÉtÉâÉO
				area_flag				= 6,			///< ç◊äXòHÉGÉäÉAÉtÉâÉO
				oneside_lane			= 7,			///< ï–ë§é‘ê¸êî
			};
		};

		// éÂè]ãÊï™ÉRÅ[Éh
		namespace kubun_code
		{
			enum code
			{
				unknown					= 0,			///< ñ¢í≤ç∏ÅEçÏã∆íÜ
				main_line				= 1,			///< éÂòHê¸
				sub_line				= 2				///< è]òHê¸
			};
		};

		// ãKêßë¨ìx
		namespace limit_speed
		{
			enum code
			{
				unknown					= 0,			///< ñ¢í≤ç∏
				limit_under30km			= 1,			///< 30km/hñ¢ñû
				limit_40km				= 2,			///< 40km/h
				limit_50km				= 3,			///< 50km/h
				limit_60km				= 4,			///< 60km/h
				limit_70km				= 5,			///< 70km/h
				limit_80km				= 6,			///< 80km/h
				limit_100km				= 7				///< 100km/h
			};
		};

		// ïÇìáÉtÉâÉO
		namespace island_flag
		{
			using namespace authenticity;
		};

		// ç◊äXòHÉGÉäÉAÉtÉâÉO
		namespace area_flag
		{
			enum code
			{
				unknown					= 0x00000000,	///< ñ¢í≤ç∏
				year_1998				= 0x00000001,	///< 1998îNìx
				year_1999				= 0x00000002,	///< 1999îNìx
				year_2000_spr			= 0x00000004,	///< 2000îNìxèt
				year_2000_aut			= 0x00000008,	///< 2000îNìxèH
				year_2001				= 0x00000010,	///< 2001îNìx
				year_2002				= 0x00000020,	///< 2002îNìx
				year_2003				= 0x00000040,	///< 2003îNìx
				year_2004				= 0x00000080,	///< 2004îNìx
				year_2005				= 0x00000100,	///< 2005îNìx
				year_2006				= 0x00000200,	///< 2006îNìx
				year_2007				= 0x00000400	///< 2007îNìx
			};

			enum kisei_chousa
			{
				no_rest					= 0x00000000,	///< ãKêßí≤ç∏Ç»Çµ
				rest					= 0x80000000	///< ãKêßí≤ç∏Ç†ÇË
			};
		};
	};
};
