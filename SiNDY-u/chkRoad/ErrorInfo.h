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
			kmeshnum		= 0,	///< ���b�V���R�[�h����
			kmeshcode		= 1,	///< 2�����b�V���R�[�h
			kfileapp		= 2,	///< �t�@�C�����ʎq
			kscale			= 3,	///< �k��
			kmaxpili		= 4,	///< �|���S���ő�`��_��
			klayercount		= 5,	///< ���C����
			klayerid		= 6,	///< ���C��ID
			klayeradd		= 7		///< ���C���A�h���X
		};
	};

	namespace link
	{
		enum ECode
		{
			kLayer			= 0,	///< ���C��
			kDisp			= 1,	///< �\�����
			kNavi			= 2,	///< �o�H���
			kManage			= 3,	///< �Ǘ��҃R�[�h
			kLinkAttr		= 4,	///< �����N���
			kBypass			= 5,	///< �o�C�p�X�t���O
			kAuto			= 6,	///< �����Ԑ�p�t���O
			kWidth			= 7,	///< ���H����
			kLaneCount		= 8,	///< �Ԑ���
			kRest			= 9,	///< ��ʋK����ʃR�[�h
			kCond			= 10,	///< ��ʋK��������ʃR�[�h
			kParkClass		= 11,	///< ���ԏ���
			kParkLink		= 12,	///< ���ԏꃊ���N���
			kAdmin			= 13,	///< �s�����R�[�h
			kPoints			= 14,	///< �⊮�_
			kRoadNo			= 15,	///< ���H����ID
			kAreaFlag		= 16,	///< �L��t���O
			kAttr			= 17,	///< ������ʃR�[�h(�����N������)
#if IN_VERSION >= 590
			kReservedE		= 18,	///< Reserve�̈�(�����N������)(���㉺�֌W)
			kReservedD		= 19,	///< Reserve�̈�(�����N������)(���n�ʂ���̑��΍�)
#else
			kUpDown			= 18,	///< �㉺�֌W(�����N������)
			kCrossHigh		= 19,	///< �n�ʂ���̑��΍�(�����N������)
#endif
			kReservedC		= 20,	///< Reserve�̈�(�����N������)
			kGradFlag		= 21,	///< �X�΃t���O(�����N������)
			kGradFlag2		= 22,	///< �X�΃t���O(�����N������)
			kGradAngle		= 23,	///< �X�Ίp�x(�����N������)
			kGradAngle2		= 24,	///< �X�Ίp�x(�����N������)
			kVicsOrd		= 25,	///< ������VICS�����N�ԍ�
			kVicsRev		= 26,	///< �t����VICS�����N�ԍ�
			kVicsPart		= 27,	///< ����VICS�敪
			kVicsPart2		= 28,	///< ����VICS�敪
			kVicsMesh		= 29,	///< ����VICS���b�V���R�[�h
			kVicsMesh2		= 30	///< ����VICS���b�V���R�[�h
		};
	};

	namespace node
	{
		enum ECode
		{
			kLayer			= 0,	///< ���C��
			kSignal			= 1,	///< �M���@�t���O
			kClass			= 2,	///< �m�[�h���
			kXY				= 3,	///< ���K��X���W
			kHeight1		= 4,	///< ���H�����L���t���O
			kHeight2		= 5,	///< ���H�����L���t���O(�����Ȃ̂ɍ����L��)
			kReserve1		= 6,	///< Reserve�̈�1
			kReserve2		= 7,	///< Reserve�̈�2
			kReserve3		= 8,	///< Reserve�̈�3
			kReserve4		= 9,	///< Reserve�̈�4
			kReserve5		= 10,	///< Reserve�̈�5
			kConnectCount	= 11,	///< �ڑ������N��
			kConnectNode	= 12,	///< �ڑ��m�[�h
			kConnectNodeIDReserved = 13,	///< �ڑ��m�[�hID Reserved�̈�
			kConnectNodeID	= 14,	///< �ڑ��m�[�hID
			kNodeID			= 15	///< �m�[�hID
		};
	};

	namespace reg
	{
		enum ECode
		{
			kLayer			= 0,	///< ���C��	
			kNodes			= 1,	///< �m�[�h��ɖ���
			kTraffic		= 2,	///< �K������
			kDay			= 3,	///< �j���R�[�h
			kAllDay			= 4,	///< �S�j���w��
			kAllNone		= 5,	///< �j���E���ԁE������0�`�F�b�N
			kMonthTime		= 6,	///< �����w��L��E���Ԏw�薳��
			kTimeMonth		= 7,	///< ���Ԏw��L��E�����w�薳��
			kDayTime		= 8,	///< �j���w��L��E���Ԏw�薳��
			kDayMonth		= 9,	///< �j���w��L��E�����w�薳��
			kTime			= 10,	///< ���ԃR�[�g
			kMonth			= 11,	///< �����R�[�h
			kDay2			= 12,	///< �j���R�[�h(��������)
			kTime2			= 13,	///< ���ԃR�[�h(��������)
			kMonth2			= 14,	///< �����R�[�h(��������)
			kClass			= 15,	///< �K�����
			kLink			= 16,	///< �ʍs�K��
			kLink2			= 17,	///< �ʍs�K�����u�������E��ƒ��v�ȊO
			kReserve1		= 18,	///< Reserve�̈�1
			kReserve2		= 19,	///< Reserve�̈�2
			kReserve3		= 20,	///< Reserve�̈�3
			kReserve4		= 21,	///< Reserve�̈�4
			kSeasonF		= 22,	///< �G�ߋK���t���O
			kSeasonReserved	= 23,	///< �G�ߋK���t���O��Reserve�̈�
			kSeasonYear		= 24,	///< �G�ߋK���ʔN�`�F�b�N
			kSeasonWeek		= 25,	///< �G�ߋK���S�j���`�F�b�N
			kSeason0Month	= 26	///< �G�ߋK��0���`�F�b�N
		};
	};

	namespace node_info
	{
		enum ECode
		{
			kLayer			= 0,	///< ���C��
			kInfoClass		= 1,	///< �m�[�h�����
			kGuideClass		= 2,	///< �n����
			kBothClass		= 3,	///< ���ʈē��{�n����
			kGuideCode		= 4,	///< �ē��R�[�h
			kBothCode		= 5,	///< �����U���{�ē��R�[�h
			kNodes			= 6,	///< �m�[�h��ɖ���
			kKanjiHalf		= 7,	///< ���ʈē���������(���p�����L��)�i�g�p���Ȃ��j
			kKanjiNone		= 8,	///< ���ʈē���������(�g�p�֎~�����L��)
			kKanaHalf		= 9,	///< �J�i����(���p�����L��)
			kKanaNone		= 10,	///< �J�i����(�g�p�֎~�����L��)
			kReserve1		= 11,	///< Reserve�̈�1
			kReserve2		= 12,	///< Reserve�̈�2
			kReserve3		= 13,	///< Reserve�̈�3
			kReserve4		= 14,	///< Reserve�̈�4
			kReserve5		= 15,	///< Reserve�̈�5
			kReserve6		= 16,	///< Reserve�̈�6
			kKanjiHalfN		= 17,	///< ��������(���p�����L��)�i�g�p���Ȃ��j
			kKanjiNoneN		= 18	///< ��������(�g�p�֎~�����L��)
		};
	};

	namespace lane
	{
		enum ECode
		{
			kLayer			= 0,	///< ���C��
			kCount			= 1,	///< ���o�惌�[����
			kLeftDiff		= 2,	///< �����������[����
			kRightDiff		= 3,	///< �E���������[����
			kFromCount		= 4,	///< �����_�i�����[����
			kInfo			= 5,	///< ���[�����
			kReserve1		= 6,	///< Reserve�̈�1
			kReserve2		= 7,	///< Reserve�̈�2
			kReserve3		= 8		///< Reserve�̈�3
		};
	};

	namespace link_info
	{
		enum ECode
		{
			kLayer			= 0,	///< ���C��
			kMainDisp		= 1,	///< �\�����
			kMainReserve	= 2,	///< ��v�H��::Reserved
			kMainSection	= 3,	///< ��v�H��::��]�敪
			kLength			= 4,	///< �����N��
			kSpeed			= 5,	///< �K�����x
			kAllNone		= 6,	///< �S��0
			kSubDisp		= 7,	///< �d�v�H��::���H���
			kSubReserve		= 8,	///< �d�v�H��::Reserved
			kSubSection		= 9		///< �d�v�H��::��]�敪
		};
	};

	namespace linkex
	{
		enum ECode
		{
			kLayer			= 0,	///< ���C��
			kKeyID			= 1,	///< �L�[ID
			kSection		= 2,	///< ��]�敪
			kSpeed			= 3,	///< �K�����x
			kIsLand			= 4,	///< �����t���O
			kArea			= 5,	///< �׊X�H��ƃG���A���ʃt���O
			kData			= 6		///< �s���ȃf�[�^
		};
	};
};

/**
 * @brief �G���[�R�[�h
 */
struct ErrorMessage 
{
	int				iCode;			///< �G���[�R�[�h
	LPCTSTR			strMessage;		//!< �G���[���b�Z�[�W
};

/**
 * @brief �w�b�_���G���[�R�[�h
 */
static ErrorMessage HeadError[] = {
	{ error::head::kmeshnum,	_T("\tHEAD\t���b�V���R�[�h�������s���ł��B") },
	{ error::head::kmeshcode,	_T("\tHEAD\t���b�V���R�[�h���s���ł��B")},
	{ error::head::kfileapp,	_T("\tHEAD\t�t�@�C�����ʎq���s���ł��B")},
	{ error::head::kscale,		_T("\tHEAD\t�k�ڂ��s���ł��B") },
	{ error::head::kmaxpili,	_T("\tHEAD\t�|���S���ő�`��_�����s���ł��B")},
	{ error::head::klayercount, _T("\tHEAD\t���C�������s���ł��B")},
	{ error::head::klayerid,	_T("\tHEAD\t���C��ID���s���ł��B")},
	{ error::head::klayeradd,	_T("\tHEAD\t���C���A�h���X���s���ł��B")}
};
const static int INHeadEnd				= error::head::klayeradd;

/**
 * @brief �����N���C���G���[�R�[�h
 */
static ErrorMessage LinkError[] = {
	{ error::link::kLayer,		_T("\tLINK\t�����N���C�������݂��܂���B")},
	{ error::link::kDisp,		_T("\tLINK\t�\����ʂ��s���ł��B")},
	{ error::link::kNavi,		_T("\tLINK\t�o�H��ʂ��s���ł��B")},
	{ error::link::kManage,		_T("\tLINK\t�����N���::�Ǘ��҃R�[�h���s���ł��B")},
	{ error::link::kLinkAttr,	_T("\tLINK\t�����N���::�����N��ʃR�[�h���s���ł��B")},
	{ error::link::kBypass,		_T("\tLINK\t�����N���::�o�C�p�X�t���O���s���ł��B")},
	{ error::link::kAuto,		_T("\tLINK\t�����N���::�����Ԑ�p���H�t���O���s���ł��B")},
	{ error::link::kWidth,		_T("\tLINK\t�����N���::���H�������s���ł��B")},
	{ error::link::kLaneCount,	_T("\tLINK\t�����N���::�Ԑ����R�[�h���s���ł��B")},
	{ error::link::kRest,		_T("\tLINK\t�����N���::��ʋK����ʃR�[�h���s���ł��B")},
	{ error::link::kCond,		_T("\tLINK\t�����N���::��ʋK��������ʃR�[�h���s���ł��B")},
	{ error::link::kParkClass,	_T("\tLINK\t���H���::���ԏ��ʂ��s���ł��B")},
	{ error::link::kParkLink,	_T("\tLINK\t���H���::���ԏꃊ���N��ʂ��s���ł��B")},
	{ error::link::kAdmin,		_T("\tLINK\t�s�����R�[�h���s���ł��B")},
	{ error::link::kPoints,		_T("\tLINK\t��ԓ_�ɏd��������܂��B")},
	{ error::link::kRoadNo,		_T("\tLINK\t���H���::���H�ԍ����s���ł��B")},
	{ error::link::kAreaFlag,	_T("\tLINK\t�L��t���O���s���ł��B")},
	{ error::link::kAttr,		_T("\tLINK\t������ʃR�[�h::�����N���������s���ł��B")},
#if IN_VERSION >= 590
	{ error::link::kReservedE,	_T("\tLINK\t������ʃR�[�h::ReservedE��0�ł͂���܂���B(���㉺�֌W�`�F�b�N)")},
	{ error::link::kReservedD,	_T("\tLINK\t������ʃR�[�h::ReservedD��0�ł͂���܂���B(���n�ʂ���̑��΍��`�F�b�N)")},
#else
	{ error::link::kUpDown,		_T("\tLINK\t������ʃR�[�h::�㉺�֌W(�����N������)���s���ł��B")},
	{ error::link::kCrossHigh,	_T("\tLINK\t������ʃR�[�h::�n�ʂ���̑��΍����s���ł��B")},
#endif
	{ error::link::kReservedC,	_T("\tLINK\t������ʃR�[�h::ReservedC��0�ł͂���܂���B")},
	{ error::link::kGradFlag,	_T("\tLINK\t������ʃR�[�h::�X�΃t���O���s���ł�(�X�Ίp�x�ƕs����)�B")},
	{ error::link::kGradFlag2,	_T("\tLINK\t������ʃR�[�h::�X�΃t���O��0�ł͂���܂���B")},
	{ error::link::kGradAngle,	_T("\tLINK\t������ʃR�[�h::�X�Ίp�x���s���ł�(�X�΃t���O�ƕs����)�B")},
	{ error::link::kGradAngle2,	_T("\tLINK\t������ʃR�[�h::�X�Ίp�x��0�ł͂���܂���B")},
	{ error::link::kVicsOrd,	_T("\tLINK\tVICS���::������VICS�����N�ԍ����s���ł��B")},
	{ error::link::kVicsRev,	_T("\tLINK\tVICS���::�t����VICS�����N�ԍ����s���ł��B")},
	{ error::link::kVicsPart,	_T("\tLINK\tVICS���::����VICS�敪���s���ł��B")},
	{ error::link::kVicsPart2,	_T("\tLINK\tVICS���::����VICS�敪0�ł͂���܂���B")},
	{ error::link::kVicsMesh,	_T("\tLINK\tVICS���::����VICS���b�V���R�[�h���s���ł��B")},
	{ error::link::kVicsMesh2,	_T("\tLINK\tVICS���::����VICS���b�V���R�[�h��0�ł͂���܂���B")},

};
const static int INLinkEnd			= error::link::kVicsMesh2;

/**
 * @brief �m�[�h���C���G���[�R�[�h
 */
static ErrorMessage NodeError[] = {
	{ error::node::kLayer,			_T("\tNODE\t�m�[�h���C�������݂��܂���B")},
	{ error::node::kSignal,			_T("\tNODE\t�M���t���O�̒l���s���ł��B")},
	{ error::node::kClass,			_T("\tNODE\t�m�[�h��ʃR�[�h���s���ł��B")},
	{ error::node::kXY,				_T("\tNODE\t���K�����W���s���ł��B")},
	{ error::node::kHeight1,		_T("\tNODE\t���H�����L���t���O���s���ł��B")},
	{ error::node::kHeight2,		_T("\tNODE\t���H�����L���t���O�������Ȃ̂ɓ��H���΍����������Ă��܂��B")},
	{ error::node::kReserve1,		_T("\tNODE\tReserved1��0�ł͂���܂���B")},
	{ error::node::kReserve2,		_T("\tNODE\tReserved2��0�ł͂���܂���B")},
	{ error::node::kReserve3,		_T("\tNODE\tReserved3��0�ł͂���܂���B")},
	{ error::node::kReserve4,		_T("\tNODE\tReserved4��0�ł͂���܂���B")},
	{ error::node::kReserve5,		_T("\tNODE\tReserved5��0�ł͂���܂���B")},
	{ error::node::kConnectCount,	_T("\tNODE\t�ڑ������N�{�����s���ł��B")},
	{ error::node::kConnectNode,	_T("\tNODE\t�ڑ��m�[�h��������܂���B")},
	{ error::node::kConnectNodeIDReserved,	_T("\tNODE\t�ڑ��m�[�hID��Reserved��0�ł͂���܂���B")},
	{ error::node::kConnectNodeID,	_T("\tNODE\t�ڑ��m�[�hID��28bit(2^28�ȉ�)�ł͂���܂���B")},
	{ error::node::kNodeID,			_T("\tNODE\t�m�[�hID��28bit(2^28�ȉ�)�ł͂���܂���B")}
};
const static int INNodeEnd			= error::node::kNodeID;

/**
 * @brief �V��ʋK�����C���G���[�R�[�h
 */
static ErrorMessage RegError[] = {
	{ error::reg::kLayer,			_T("\t�V��ʋK�����C�������݂��܂���B")},
	{ error::reg::kNodes,			_T("\t�m�[�h��ɖ���������܂��B")},
	{ error::reg::kTraffic,			_T("\t�K���������s���ł��B")},
	{ error::reg::kDay,				_T("\t�j���w�肪�s���ł��B")},
	{ error::reg::kAllDay,			_T("\t�J�n���A�I�����A�J�n���ԁA�I�����Ԃ��S���ł��B")},
	{ error::reg::kAllNone,			_T("\t�J�n���A�I�����A�J�n���ԁA�I�����Ԃ����ׂ�0�ł��B")},
	{ error::reg::kMonthTime,		_T("\t�����w�肪����̂ɁA���Ԏw�肪����܂���B")},
	{ error::reg::kTimeMonth,		_T("\t���Ԏw�肪����̂ɁA�����w�肪����܂���B")},
	{ error::reg::kDayTime,			_T("\t�j���w�肪����̂ɁA���Ԏw�肪����܂���B")},
	{ error::reg::kDayMonth,		_T("\t�j���w�肪����̂ɁA�����w�肪����܂���B")},
	{ error::reg::kTime,			_T("\t���Ԏw�肪�s���ł��B")},
	{ error::reg::kMonth,			_T("\t�����w�肪�s���ł��B")},
	{ error::reg::kDay2,			_T("\t���������K���Ȃ̂ɗj���w�肪0�ł͂���܂���B")},
	{ error::reg::kTime2,			_T("\t���������K���Ȃ̂Ɏ��Ԏw�肪0�ł͂���܂���B")},
	{ error::reg::kMonth2,			_T("\t���������K���Ȃ̂Ɍ����w�肪0�ł͂���܂���B")},
	{ error::reg::kClass,			_T("\t�K����ʂ̒l���s���ł��B")},
	{ error::reg::kLink,			_T("\t�ʍs�K�����s���ł��B")},
	{ error::reg::kLink2,			_T("\t�ʍs�K����0�ł͂���܂���B")},
	{ error::reg::kReserve1,		_T("\tReserved1��0�ł͂���܂���B")},
	{ error::reg::kReserve2,		_T("\tReserved2��0�ł͂���܂���B")},
	{ error::reg::kReserve3,		_T("\tReserved3��0�ł͂���܂���B")},
	{ error::reg::kReserve4,		_T("\tReserved4��0�ł͂���܂���B")},
	{ error::reg::kSeasonF,			_T("\t�G�ߋK���t���O���s���ł��B")},
	{ error::reg::kSeasonReserved,	_T("\t�G�ߋK���t���O��Reserved�̈悪0�ł͂���܂���B")},
	{ error::reg::kSeasonYear,		_T("\t�G�ߋK��������̂ɒʔN�K���ł��B")},
	{ error::reg::kSeasonWeek,		_T("\t�G�ߋK��������̂ɑS�j���ł͂���܂񂹂�B")},
	{ error::reg::kSeason0Month,	_T("\t�G�ߋK��������̂�0���K���ł��B")}
};
const static int INRegEnd			= error::reg::kSeason0Month;

/**
 * @brief �m�[�h��񃌃C���G���[�R�[�h
 */
static ErrorMessage NInfoError[] = {
	{ error::node_info::kLayer,			_T("\tNODEINFO\t�m�[�h��񃌃C�������݂��܂���B")},
	{ error::node_info::kInfoClass,		_T("\tNODEINFO\t�m�[�h����ʃR�[�h���s���ł��B")},
	{ error::node_info::kGuideClass,	_T("\tNODEINFO\t�n���ʂ��s���ł��B")},
	{ error::node_info::kBothClass,		_T("\tNODEINFO\t���ʈē��ȊO�ɒn���ʂ��t���Ă��܂��B")},
	{ error::node_info::kGuideCode,		_T("\tNODEINFO\t�ē��R�[�h���s���ł��B")},
	{ error::node_info::kBothCode,		_T("\tNODEINFO\t�����U���ȊO�Ɉē��R�[�h���t���Ă��܂��B")},
	{ error::node_info::kNodes,			_T("\tNODEINFO\t�m�[�h��ɖ���������܂��B")},
	{ error::node_info::kKanjiHalf,		_T("\tNODEINFO\t���ʈē��������̂ɔ��p���������݂��܂��B")},
	{ error::node_info::kKanjiNone,		_T("\tNODEINFO\t���ʈē��̊������̂Ɏg�p�֎~���������݂��܂��B")},
	{ error::node_info::kKanaHalf,		_T("\tNODEINFO\t�J�i���̂ɔ��p���������݂��܂��B")},
	{ error::node_info::kKanaNone,		_T("\tNODEINFO\t�J�i���̂Ɏg�p�֎~���������݂��܂��B")},
	{ error::node_info::kReserve1,		_T("\tNODEINFO\tReserved1��0�ł͂���܂���B")},
	{ error::node_info::kReserve2,		_T("\tNODEINFO\tReserved2��0�ł͂���܂���B")},
	{ error::node_info::kReserve3,		_T("\tNODEINFO\tReserved3��0�ł͂���܂���B")},
	{ error::node_info::kReserve4,		_T("\tNODEINFO\tReserved4��0�ł͂���܂���B")},
	{ error::node_info::kReserve5,		_T("\tNODEINFO\tReserved5��0�ł͂���܂���B")},
	{ error::node_info::kReserve6,		_T("\tNODEINFO\tReserved6��0�ł͂���܂���B")},
	{ error::node_info::kKanjiHalfN,	_T("\tNODEINFO\t�������̂ɔ��p���������݂��܂��B")},
	{ error::node_info::kKanjiNoneN,	_T("\tNODEINFO\t�������̂Ɏg�p�֎~���������݂��܂��B")}
};
const static int INNInfoEnd			= error::node_info::kKanjiNoneN;

/**
 * @brief ���[����񃌃C���G���[�R�[�h
 */
static ErrorMessage LaneError[] = {
	{ error::lane::kLayer,				_T("\tLANE\t���[����񃌃C�������݂��܂���B")},
	{ error::lane::kCount,				_T("\tLANE\t���o�惌�[�������s���ł��B")},
	{ error::lane::kLeftDiff,			_T("\tLANE\t�����������[�������s���ł��B")},
	{ error::lane::kRightDiff,			_T("\tLANE\t�E���������[�������s���ł��B")},
	{ error::lane::kFromCount,			_T("\tLANE\t�����_�i�����[�������s���ł��B")},
	{ error::lane::kInfo,				_T("\tLANE\t���[����񂪕s���ł��B")},
	{ error::lane::kReserve1,			_T("\tLANE\tReserved1��0�ł͂���܂���B")},
	{ error::lane::kReserve2,			_T("\tLANE\tReserved2��0�ł͂���܂���B")},
	{ error::lane::kReserve3,			_T("\tLANE\tReserved3��0�ł͂���܂���B")}
};
const static int INLaneEnd			= error::lane::kReserve3;

/**
 * @brief �����N��񃌃C���G���[�R�[�h
 */
static ErrorMessage LInfoError[] = {
	{ error::link_info::kLayer,			_T("\tLINKINFO\t�����N��񃌃C�������݂��Ă��܂��B")},
	{ error::link_info::kMainDisp,		_T("\tLINKINFO\t��H�����::���H��ʃR�[�h���s���ł��B")},
	{ error::link_info::kMainReserve,	_T("\tLINKINFO\t��H�����::Reserved��0�ł͂���܂���B")},
	{ error::link_info::kMainSection,	_T("\tLINKINFO\t��H�����::��]�敪�R�[�h���s���ł��B")},
	{ error::link_info::kLength,		_T("\tLINKINFO\t�����N�����s���ł��B")},
	{ error::link_info::kSpeed,			_T("\tLINKINFO\t�K�����x�R�[�h���s���ł��B")},
	{ error::link_info::kAllNone,		_T("\tLINKINFO\t�f�[�^�ɈӖ�������܂���")},
	{ error::link_info::kSubDisp,		_T("\tLINKINFO\t�d�v�H�����::���H��ʃR�[�h���s���ł��B")},
	{ error::link_info::kSubReserve,	_T("\tLINKINFO\t�d�v�H�����::Reserved��0�ł͂���܂���B")},
	{ error::link_info::kSubSection,	_T("\tLINKINFO\t�d�v�H�����::��]�敪�R�[�h���s���ł��B")}
};
const static int INLInfoEnd			= error::link_info::kSubSection;

/**
 * @brief �����N�g�����C���G���[�R�[�h
 */
static ErrorMessage LinkExError[] = {
	{ error::linkex::kLayer,			_T("\tLINKEX\t�����N�g�����C�������݂��܂���B")},
	{ error::linkex::kKeyID,			_T("\tLINKEX\t�����N���L�[�h�c���s���ł��B")},
	{ error::linkex::kSection,			_T("\tLINKEX\t��]�敪��ʃR�[�h���s���ł��B")},
	{ error::linkex::kSpeed,			_T("\tLINKEX\t�K�����x���s���ł��B")},
	{ error::linkex::kIsLand,			_T("\tLINKEX\t�����t���O���s���ł��B")},
	{ error::linkex::kArea,				_T("\tLINKEX\t�׊X�H��ƃG���A���ʃt���O���s���ł��B")},
	{ error::linkex::kData,				_T("\tLINKEX\t�����N���̒l���s���ł��B")}
};
const static int INLinkExEnd			= error::linkex::kData;
