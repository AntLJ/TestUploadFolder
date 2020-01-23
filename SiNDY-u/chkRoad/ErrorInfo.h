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

namespace error
{
	namespace head
	{
		enum ECode
		{
			kmeshnum		= 0,	///< メッシュコード次数
			kmeshcode		= 1,	///< 2次メッシュコード
			kfileapp		= 2,	///< ファイル判別子
			kscale			= 3,	///< 縮尺
			kmaxpili		= 4,	///< ポリゴン最大形状点数
			klayercount		= 5,	///< レイヤ数
			klayerid		= 6,	///< レイヤID
			klayeradd		= 7		///< レイヤアドレス
		};
	};

	namespace link
	{
		enum ECode
		{
			kLayer			= 0,	///< レイヤ
			kDisp			= 1,	///< 表示種別
			kNavi			= 2,	///< 経路種別
			kManage			= 3,	///< 管理者コード
			kLinkAttr		= 4,	///< リンク種別
			kBypass			= 5,	///< バイパスフラグ
			kAuto			= 6,	///< 自動車専用フラグ
			kWidth			= 7,	///< 道路幅員
			kLaneCount		= 8,	///< 車線数
			kRest			= 9,	///< 交通規制種別コード
			kCond			= 10,	///< 交通規制条件種別コード
			kParkClass		= 11,	///< 駐車場種別
			kParkLink		= 12,	///< 駐車場リンク種別
			kAdmin			= 13,	///< 行政区域コード
			kPoints			= 14,	///< 補完点
			kRoadNo			= 15,	///< 道路名称ID
			kAreaFlag		= 16,	///< 広域フラグ
			kAttr			= 17,	///< 属性種別コード(リンク内属性)
#if IN_VERSION >= 590
			kReservedE		= 18,	///< Reserve領域(リンク内属性)(旧上下関係)
			kReservedD		= 19,	///< Reserve領域(リンク内属性)(旧地面からの相対高)
#else
			kUpDown			= 18,	///< 上下関係(リンク内属性)
			kCrossHigh		= 19,	///< 地面からの相対高(リンク内属性)
#endif
			kReservedC		= 20,	///< Reserve領域(リンク内属性)
			kGradFlag		= 21,	///< 傾斜フラグ(リンク内属性)
			kGradFlag2		= 22,	///< 傾斜フラグ(リンク内属性)
			kGradAngle		= 23,	///< 傾斜角度(リンク内属性)
			kGradAngle2		= 24,	///< 傾斜角度(リンク内属性)
			kVicsOrd		= 25,	///< 順方向VICSリンク番号
			kVicsRev		= 26,	///< 逆方向VICSリンク番号
			kVicsPart		= 27,	///< 中域VICS区分
			kVicsPart2		= 28,	///< 中域VICS区分
			kVicsMesh		= 29,	///< 中域VICSメッシュコード
			kVicsMesh2		= 30	///< 中域VICSメッシュコード
		};
	};

	namespace node
	{
		enum ECode
		{
			kLayer			= 0,	///< レイヤ
			kSignal			= 1,	///< 信号機フラグ
			kClass			= 2,	///< ノード種別
			kXY				= 3,	///< 正規化X座標
			kHeight1		= 4,	///< 道路高さ有効フラグ
			kHeight2		= 5,	///< 道路高さ有効フラグ(無効なのに高さ有り)
			kReserve1		= 6,	///< Reserve領域1
			kReserve2		= 7,	///< Reserve領域2
			kReserve3		= 8,	///< Reserve領域3
			kReserve4		= 9,	///< Reserve領域4
			kReserve5		= 10,	///< Reserve領域5
			kConnectCount	= 11,	///< 接続リンク数
			kConnectNode	= 12,	///< 接続ノード
			kConnectNodeIDReserved = 13,	///< 接続ノードID Reserved領域
			kConnectNodeID	= 14,	///< 接続ノードID
			kNodeID			= 15	///< ノードID
		};
	};

	namespace reg
	{
		enum ECode
		{
			kLayer			= 0,	///< レイヤ	
			kNodes			= 1,	///< ノード列に矛盾
			kTraffic		= 2,	///< 規制条件
			kDay			= 3,	///< 曜日コード
			kAllDay			= 4,	///< 全曜日指定
			kAllNone		= 5,	///< 曜日・時間・月日の0チェック
			kMonthTime		= 6,	///< 月日指定有り・時間指定無し
			kTimeMonth		= 7,	///< 時間指定有り・月日指定無し
			kDayTime		= 8,	///< 曜日指定有り・時間指定無し
			kDayMonth		= 9,	///< 曜日指定有り・月日指定無し
			kTime			= 10,	///< 時間コート
			kMonth			= 11,	///< 月日コード
			kDay2			= 12,	///< 曜日コード(条件無し)
			kTime2			= 13,	///< 時間コード(条件無し)
			kMonth2			= 14,	///< 月日コード(条件無し)
			kClass			= 15,	///< 規制種別
			kLink			= 16,	///< 通行規制
			kLink2			= 17,	///< 通行規制が「未調査・作業中」以外
			kReserve1		= 18,	///< Reserve領域1
			kReserve2		= 19,	///< Reserve領域2
			kReserve3		= 20,	///< Reserve領域3
			kReserve4		= 21,	///< Reserve領域4
			kSeasonF		= 22,	///< 季節規制フラグ
			kSeasonReserved	= 23,	///< 季節規制フラグのReserve領域
			kSeasonYear		= 24,	///< 季節規制通年チェック
			kSeasonWeek		= 25,	///< 季節規制全曜日チェック
			kSeason0Month	= 26	///< 季節規制0月チェック
		};
	};

	namespace node_info
	{
		enum ECode
		{
			kLayer			= 0,	///< レイヤ
			kInfoClass		= 1,	///< ノード情報種別
			kGuideClass		= 2,	///< 地先種別
			kBothClass		= 3,	///< 方面案内＋地先種別
			kGuideCode		= 4,	///< 案内コード
			kBothCode		= 5,	///< 強制誘導＋案内コード
			kNodes			= 6,	///< ノード列に矛盾
			kKanjiHalf		= 7,	///< 方面案内漢字名称(半角文字有り)（使用しない）
			kKanjiNone		= 8,	///< 方面案内漢字名称(使用禁止文字有り)
			kKanaHalf		= 9,	///< カナ名称(半角文字有り)
			kKanaNone		= 10,	///< カナ名称(使用禁止文字有り)
			kReserve1		= 11,	///< Reserve領域1
			kReserve2		= 12,	///< Reserve領域2
			kReserve3		= 13,	///< Reserve領域3
			kReserve4		= 14,	///< Reserve領域4
			kReserve5		= 15,	///< Reserve領域5
			kReserve6		= 16,	///< Reserve領域6
			kKanjiHalfN		= 17,	///< 漢字名称(半角文字有り)（使用しない）
			kKanjiNoneN		= 18	///< 漢字名称(使用禁止文字有り)
		};
	};

	namespace lane
	{
		enum ECode
		{
			kLayer			= 0,	///< レイヤ
			kCount			= 1,	///< 流出先レーン数
			kLeftDiff		= 2,	///< 左側増減レーン数
			kRightDiff		= 3,	///< 右側増減レーン数
			kFromCount		= 4,	///< 交差点進入レーン数
			kInfo			= 5,	///< レーン情報
			kReserve1		= 6,	///< Reserve領域1
			kReserve2		= 7,	///< Reserve領域2
			kReserve3		= 8		///< Reserve領域3
		};
	};

	namespace link_info
	{
		enum ECode
		{
			kLayer			= 0,	///< レイヤ
			kMainDisp		= 1,	///< 表示種別
			kMainReserve	= 2,	///< 主要路線::Reserved
			kMainSection	= 3,	///< 主要路線::主従区分
			kLength			= 4,	///< リンク長
			kSpeed			= 5,	///< 規制速度
			kAllNone		= 6,	///< 全て0
			kSubDisp		= 7,	///< 重要路線::道路種別
			kSubReserve		= 8,	///< 重要路線::Reserved
			kSubSection		= 9		///< 重要路線::主従区分
		};
	};

	namespace linkex
	{
		enum ECode
		{
			kLayer			= 0,	///< レイヤ
			kKeyID			= 1,	///< キーID
			kSection		= 2,	///< 主従区分
			kSpeed			= 3,	///< 規制速度
			kIsLand			= 4,	///< 浮島フラグ
			kArea			= 5,	///< 細街路作業エリア判別フラグ
			kData			= 6		///< 不正なデータ
		};
	};
};

/**
 * @brief エラーコード
 */
struct ErrorMessage 
{
	int				iCode;			///< エラーコード
	LPCTSTR			strMessage;		//!< エラーメッセージ
};

/**
 * @brief ヘッダ情報エラーコード
 */
static ErrorMessage HeadError[] = {
	{ error::head::kmeshnum,	_T("\tHEAD\tメッシュコード次数が不正です。") },
	{ error::head::kmeshcode,	_T("\tHEAD\tメッシュコードが不正です。")},
	{ error::head::kfileapp,	_T("\tHEAD\tファイル判別子が不正です。")},
	{ error::head::kscale,		_T("\tHEAD\t縮尺が不正です。") },
	{ error::head::kmaxpili,	_T("\tHEAD\tポリゴン最大形状点数が不正です。")},
	{ error::head::klayercount, _T("\tHEAD\tレイヤ数が不正です。")},
	{ error::head::klayerid,	_T("\tHEAD\tレイヤIDが不正です。")},
	{ error::head::klayeradd,	_T("\tHEAD\tレイヤアドレスが不正です。")}
};
const static int INHeadEnd				= error::head::klayeradd;

/**
 * @brief リンクレイヤエラーコード
 */
static ErrorMessage LinkError[] = {
	{ error::link::kLayer,		_T("\tLINK\tリンクレイヤが存在しません。")},
	{ error::link::kDisp,		_T("\tLINK\t表示種別が不正です。")},
	{ error::link::kNavi,		_T("\tLINK\t経路種別が不正です。")},
	{ error::link::kManage,		_T("\tLINK\tリンク情報::管理者コードが不正です。")},
	{ error::link::kLinkAttr,	_T("\tLINK\tリンク情報::リンク種別コードが不正です。")},
	{ error::link::kBypass,		_T("\tLINK\tリンク情報::バイパスフラグが不正です。")},
	{ error::link::kAuto,		_T("\tLINK\tリンク情報::自動車専用道路フラグが不正です。")},
	{ error::link::kWidth,		_T("\tLINK\tリンク情報::道路幅員が不正です。")},
	{ error::link::kLaneCount,	_T("\tLINK\tリンク情報::車線数コードが不正です。")},
	{ error::link::kRest,		_T("\tLINK\tリンク情報::交通規制種別コードが不正です。")},
	{ error::link::kCond,		_T("\tLINK\tリンク情報::交通規制条件種別コードが不正です。")},
	{ error::link::kParkClass,	_T("\tLINK\t道路情報::駐車場種別が不正です。")},
	{ error::link::kParkLink,	_T("\tLINK\t道路情報::駐車場リンク種別が不正です。")},
	{ error::link::kAdmin,		_T("\tLINK\t行政区域コードが不正です。")},
	{ error::link::kPoints,		_T("\tLINK\t補間点に重複があります。")},
	{ error::link::kRoadNo,		_T("\tLINK\t道路情報::道路番号が不正です。")},
	{ error::link::kAreaFlag,	_T("\tLINK\t広域フラグが不正です。")},
	{ error::link::kAttr,		_T("\tLINK\t属性種別コード::リンク内属性が不正です。")},
#if IN_VERSION >= 590
	{ error::link::kReservedE,	_T("\tLINK\t属性種別コード::ReservedEが0ではありません。(旧上下関係チェック)")},
	{ error::link::kReservedD,	_T("\tLINK\t属性種別コード::ReservedDが0ではありません。(旧地面からの相対高チェック)")},
#else
	{ error::link::kUpDown,		_T("\tLINK\t属性種別コード::上下関係(リンク内属性)が不正です。")},
	{ error::link::kCrossHigh,	_T("\tLINK\t属性種別コード::地面からの相対高が不正です。")},
#endif
	{ error::link::kReservedC,	_T("\tLINK\t属性種別コード::ReservedCが0ではありません。")},
	{ error::link::kGradFlag,	_T("\tLINK\t属性種別コード::傾斜フラグが不正です(傾斜角度と不整合)。")},
	{ error::link::kGradFlag2,	_T("\tLINK\t属性種別コード::傾斜フラグが0ではありません。")},
	{ error::link::kGradAngle,	_T("\tLINK\t属性種別コード::傾斜角度が不正です(傾斜フラグと不整合)。")},
	{ error::link::kGradAngle2,	_T("\tLINK\t属性種別コード::傾斜角度が0ではありません。")},
	{ error::link::kVicsOrd,	_T("\tLINK\tVICS情報::順方向VICSリンク番号が不正です。")},
	{ error::link::kVicsRev,	_T("\tLINK\tVICS情報::逆方向VICSリンク番号が不正です。")},
	{ error::link::kVicsPart,	_T("\tLINK\tVICS情報::中域VICS区分が不正です。")},
	{ error::link::kVicsPart2,	_T("\tLINK\tVICS情報::中域VICS区分0ではありません。")},
	{ error::link::kVicsMesh,	_T("\tLINK\tVICS情報::中域VICSメッシュコードが不正です。")},
	{ error::link::kVicsMesh2,	_T("\tLINK\tVICS情報::中域VICSメッシュコードが0ではありません。")},

};
const static int INLinkEnd			= error::link::kVicsMesh2;

/**
 * @brief ノードレイヤエラーコード
 */
static ErrorMessage NodeError[] = {
	{ error::node::kLayer,			_T("\tNODE\tノードレイヤが存在しません。")},
	{ error::node::kSignal,			_T("\tNODE\t信号フラグの値が不正です。")},
	{ error::node::kClass,			_T("\tNODE\tノード種別コードが不正です。")},
	{ error::node::kXY,				_T("\tNODE\t正規化座標が不正です。")},
	{ error::node::kHeight1,		_T("\tNODE\t道路高さ有効フラグが不正です。")},
	{ error::node::kHeight2,		_T("\tNODE\t道路高さ有効フラグが無効なのに道路相対高さが入っています。")},
	{ error::node::kReserve1,		_T("\tNODE\tReserved1が0ではありません。")},
	{ error::node::kReserve2,		_T("\tNODE\tReserved2が0ではありません。")},
	{ error::node::kReserve3,		_T("\tNODE\tReserved3が0ではありません。")},
	{ error::node::kReserve4,		_T("\tNODE\tReserved4が0ではありません。")},
	{ error::node::kReserve5,		_T("\tNODE\tReserved5が0ではありません。")},
	{ error::node::kConnectCount,	_T("\tNODE\t接続リンク本数が不正です。")},
	{ error::node::kConnectNode,	_T("\tNODE\t接続ノードが見つかりません。")},
	{ error::node::kConnectNodeIDReserved,	_T("\tNODE\t接続ノードIDのReservedが0ではありません。")},
	{ error::node::kConnectNodeID,	_T("\tNODE\t接続ノードIDが28bit(2^28以下)ではありません。")},
	{ error::node::kNodeID,			_T("\tNODE\tノードIDが28bit(2^28以下)ではありません。")}
};
const static int INNodeEnd			= error::node::kNodeID;

/**
 * @brief 新交通規制レイヤエラーコード
 */
static ErrorMessage RegError[] = {
	{ error::reg::kLayer,			_T("\t新交通規制レイヤが存在しません。")},
	{ error::reg::kNodes,			_T("\tノード列に矛盾があります。")},
	{ error::reg::kTraffic,			_T("\t規制条件が不正です。")},
	{ error::reg::kDay,				_T("\t曜日指定が不正です。")},
	{ error::reg::kAllDay,			_T("\t開始日、終了日、開始時間、終了時間が全日です。")},
	{ error::reg::kAllNone,			_T("\t開始日、終了日、開始時間、終了時間がすべて0です。")},
	{ error::reg::kMonthTime,		_T("\t月日指定があるのに、時間指定がありません。")},
	{ error::reg::kTimeMonth,		_T("\t時間指定があるのに、月日指定がありません。")},
	{ error::reg::kDayTime,			_T("\t曜日指定があるのに、時間指定がありません。")},
	{ error::reg::kDayMonth,		_T("\t曜日指定があるのに、月日指定がありません。")},
	{ error::reg::kTime,			_T("\t時間指定が不正です。")},
	{ error::reg::kMonth,			_T("\t月日指定が不正です。")},
	{ error::reg::kDay2,			_T("\t条件無し規制なのに曜日指定が0ではありません。")},
	{ error::reg::kTime2,			_T("\t条件無し規制なのに時間指定が0ではありません。")},
	{ error::reg::kMonth2,			_T("\t条件無し規制なのに月日指定が0ではありません。")},
	{ error::reg::kClass,			_T("\t規制種別の値が不正です。")},
	{ error::reg::kLink,			_T("\t通行規制が不正です。")},
	{ error::reg::kLink2,			_T("\t通行規制が0ではありません。")},
	{ error::reg::kReserve1,		_T("\tReserved1が0ではありません。")},
	{ error::reg::kReserve2,		_T("\tReserved2が0ではありません。")},
	{ error::reg::kReserve3,		_T("\tReserved3が0ではありません。")},
	{ error::reg::kReserve4,		_T("\tReserved4が0ではありません。")},
	{ error::reg::kSeasonF,			_T("\t季節規制フラグが不正です。")},
	{ error::reg::kSeasonReserved,	_T("\t季節規制フラグのReserved領域が0ではありません。")},
	{ error::reg::kSeasonYear,		_T("\t季節規制があるのに通年規制です。")},
	{ error::reg::kSeasonWeek,		_T("\t季節規制があるのに全曜日ではありまんせん。")},
	{ error::reg::kSeason0Month,	_T("\t季節規制があるのに0月規制です。")}
};
const static int INRegEnd			= error::reg::kSeason0Month;

/**
 * @brief ノード情報レイヤエラーコード
 */
static ErrorMessage NInfoError[] = {
	{ error::node_info::kLayer,			_T("\tNODEINFO\tノード情報レイヤが存在しません。")},
	{ error::node_info::kInfoClass,		_T("\tNODEINFO\tノード情報種別コードが不正です。")},
	{ error::node_info::kGuideClass,	_T("\tNODEINFO\t地先種別が不正です。")},
	{ error::node_info::kBothClass,		_T("\tNODEINFO\t方面案内以外に地先種別が付いています。")},
	{ error::node_info::kGuideCode,		_T("\tNODEINFO\t案内コードが不正です。")},
	{ error::node_info::kBothCode,		_T("\tNODEINFO\t強制誘導以外に案内コードが付いています。")},
	{ error::node_info::kNodes,			_T("\tNODEINFO\tノード列に矛盾があります。")},
	{ error::node_info::kKanjiHalf,		_T("\tNODEINFO\t方面案内漢字名称に半角文字が存在します。")},
	{ error::node_info::kKanjiNone,		_T("\tNODEINFO\t方面案内の漢字名称に使用禁止文字が存在します。")},
	{ error::node_info::kKanaHalf,		_T("\tNODEINFO\tカナ名称に半角数字が存在します。")},
	{ error::node_info::kKanaNone,		_T("\tNODEINFO\tカナ名称に使用禁止文字が存在します。")},
	{ error::node_info::kReserve1,		_T("\tNODEINFO\tReserved1が0ではありません。")},
	{ error::node_info::kReserve2,		_T("\tNODEINFO\tReserved2が0ではありません。")},
	{ error::node_info::kReserve3,		_T("\tNODEINFO\tReserved3が0ではありません。")},
	{ error::node_info::kReserve4,		_T("\tNODEINFO\tReserved4が0ではありません。")},
	{ error::node_info::kReserve5,		_T("\tNODEINFO\tReserved5が0ではありません。")},
	{ error::node_info::kReserve6,		_T("\tNODEINFO\tReserved6が0ではありません。")},
	{ error::node_info::kKanjiHalfN,	_T("\tNODEINFO\t漢字名称に半角文字が存在します。")},
	{ error::node_info::kKanjiNoneN,	_T("\tNODEINFO\t漢字名称に使用禁止文字が存在します。")}
};
const static int INNInfoEnd			= error::node_info::kKanjiNoneN;

/**
 * @brief レーン情報レイヤエラーコード
 */
static ErrorMessage LaneError[] = {
	{ error::lane::kLayer,				_T("\tLANE\tレーン情報レイヤが存在しません。")},
	{ error::lane::kCount,				_T("\tLANE\t流出先レーン数が不正です。")},
	{ error::lane::kLeftDiff,			_T("\tLANE\t左側増減レーン数が不正です。")},
	{ error::lane::kRightDiff,			_T("\tLANE\t右側増減レーン数が不正です。")},
	{ error::lane::kFromCount,			_T("\tLANE\t交差点進入レーン数が不正です。")},
	{ error::lane::kInfo,				_T("\tLANE\tレーン情報が不正です。")},
	{ error::lane::kReserve1,			_T("\tLANE\tReserved1が0ではありません。")},
	{ error::lane::kReserve2,			_T("\tLANE\tReserved2が0ではありません。")},
	{ error::lane::kReserve3,			_T("\tLANE\tReserved3が0ではありません。")}
};
const static int INLaneEnd			= error::lane::kReserve3;

/**
 * @brief リンク情報レイヤエラーコード
 */
static ErrorMessage LInfoError[] = {
	{ error::link_info::kLayer,			_T("\tLINKINFO\tリンク情報レイヤが存在しています。")},
	{ error::link_info::kMainDisp,		_T("\tLINKINFO\t主路線情報::道路種別コードが不正です。")},
	{ error::link_info::kMainReserve,	_T("\tLINKINFO\t主路線情報::Reservedが0ではありません。")},
	{ error::link_info::kMainSection,	_T("\tLINKINFO\t主路線情報::主従区分コードが不正です。")},
	{ error::link_info::kLength,		_T("\tLINKINFO\tリンク長が不正です。")},
	{ error::link_info::kSpeed,			_T("\tLINKINFO\t規制速度コードが不正です。")},
	{ error::link_info::kAllNone,		_T("\tLINKINFO\tデータに意味がありません")},
	{ error::link_info::kSubDisp,		_T("\tLINKINFO\t重要路線情報::道路種別コードが不正です。")},
	{ error::link_info::kSubReserve,	_T("\tLINKINFO\t重要路線情報::Reservedが0ではありません。")},
	{ error::link_info::kSubSection,	_T("\tLINKINFO\t重要路線情報::主従区分コードが不正です。")}
};
const static int INLInfoEnd			= error::link_info::kSubSection;

/**
 * @brief リンク拡張レイヤエラーコード
 */
static ErrorMessage LinkExError[] = {
	{ error::linkex::kLayer,			_T("\tLINKEX\tリンク拡張レイヤが存在しません。")},
	{ error::linkex::kKeyID,			_T("\tLINKEX\tリンク情報キーＩＤが不正です。")},
	{ error::linkex::kSection,			_T("\tLINKEX\t主従区分種別コードが不正です。")},
	{ error::linkex::kSpeed,			_T("\tLINKEX\t規制速度が不正です。")},
	{ error::linkex::kIsLand,			_T("\tLINKEX\t浮島フラグが不正です。")},
	{ error::linkex::kArea,				_T("\tLINKEX\t細街路作業エリア判別フラグが不正です。")},
	{ error::linkex::kData,				_T("\tLINKEX\tリンク情報の値が不正です。")}
};
const static int INLinkExEnd			= error::linkex::kData;
