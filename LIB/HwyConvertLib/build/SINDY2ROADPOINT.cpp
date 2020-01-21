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

// SINDY2ROADPOINT.cpp: SINDY2ROADPOINT クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/foreach.hpp>
#include "SINDY2ROADPOINT.h"
#include "SINDY2INCODE.h"
#include "SINDY2HIGHWAY.h"
#include "SINDY2HWYNODE.h"
#include "NvUnit.h"
#include "INHwySchema.h"
#include "SindyFindHwyNode.h"
#include "INHwyCommonUtility.h"
#include "SindyFindMainLineFromJCTInterflow.h"
#include "SindyHighwayTools.h"

using namespace in::schema::roadpoint;

RoadPoint::RoadPoint() :
ePrefMesh(0),
ePrefX(0),
ePrefY(0),
eAccessPointID(0),
eMesh(0),
eX(0),
eY(0),
eAdmin(0),
eFacilInfoPointID(0),
eSeq(0),
eSearchClass(0),
eName(""),
eYomi(""),
eRoadCode(0),
eFacilClassAndInOutCode(0),
eFacilClass(0),
eInOut(0),
ePntName(""),
ePntYomi(""),
eSapaInfo(),
eOriginal(0),
eDummyFacil(0),
ePrefMeshForSirius(0),
ePrefXForSirius(0),
ePrefYForSirius(0),
eDirection(0)
{
}

/**
 * @brief 逆方向のアクセスポイントを取得する
 * @note	ipAccessPointとRoad_Code, Road_Seq, InOut_Cが同じで、Direction_Cが上下逆のアクセスポイントを取得する。
 * @param[in]	cHwyInfo		HWY情報
 * @param[in]	ipAccessPoint	対象アクセスポイント
 * @param[out]	ipAccessPoint2	逆方向のアクセスポイントを返すバッファ
 * @return	結果
 */
static HRESULT FindRevdirAccessPoint(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, IFeature **ipAccessPoint2)
{
	HRESULT hr = S_OK;
	const char *func = "FindRevdirAccessPoint";

	*ipAccessPoint2 = NULL;

	long aRoadCode, aRoadSeq;
	if (FAILED(hr = cHwyInfo->GetFacilCode(ipAccessPoint, &aRoadCode, &aRoadSeq))) return hr;

	CComVariant vaInOut;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::INOUT_C, &vaInOut))) return hr;
	vaInOut.ChangeType(VT_I4);

	CComVariant vaDirection;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::DIRECTION_C, &vaDirection))) return hr;
	vaDirection.ChangeType(VT_I4);

	IGeometryPtr ipGeometry;
	if (FAILED(hr = ipAccessPoint->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);

	GDBPNTRANGE aRange;
	if (FAILED(hr = cHwyInfo->eAccessPoint.SearchByPoint((IPointPtr)ipGeometry, &aRange))) return hr;
	if (aRange.first == aRange.second) return S_OK;

	for (GDBPNTITER aIter = aRange.first; aIter != aRange.second; aIter++)
	{
		IFeaturePtr ipFeature = aIter->second;

		long aRoadCode2 = 0, aRoadSeq2 = 0;
		if (FAILED(hr = cHwyInfo->GetFacilCode(ipFeature, &aRoadCode2, &aRoadSeq2))) return hr;

		if (aRoadCode != aRoadCode2 || aRoadSeq != aRoadSeq2) continue;

		CComVariant vaInOut2;
		if (FAILED(hr = GDBGetValue(ipFeature, sindy::access_point::INOUT_C, &vaInOut2))) return hr;
		vaInOut2.ChangeType(VT_I4);

		if (vaInOut.lVal != vaInOut2.lVal) continue;

		CComVariant vaDirection2;
		if (FAILED(hr = GDBGetValue(ipFeature, sindy::access_point::DIRECTION_C, &vaDirection2))) return hr;
		vaDirection2.ChangeType(VT_I4);

		if ((vaDirection.lVal == sindy::access_point::direction_up && vaDirection2.lVal == sindy::access_point::direction_down) ||
			(vaDirection.lVal == sindy::access_point::direction_down && vaDirection2.lVal == sindy::access_point::direction_up))
		{
			(*ipAccessPoint2) = ipFeature;	// 逆方向アクセスポイントはただ１つしか存在し得ない（HC1017,1040）ので、１つ見つかった時点で終了
			(*ipAccessPoint2)->AddRef();
			break;
		}
	}

	return S_OK;
}

/**
 * @brief ポイント位置コードの取得（但し、IC/ランプのアクセスポイントの重なりは考慮しない）
 * @param[in]	cHwyInfo		HWY情報
 * @param[in]	ipAccessPoint	対象アクセスポイント
 * @param[out]	cLineType		ポイント位置コードを返すバッファ
 * @return	結果
 */
static HRESULT GetLineType(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cLineType)
{
	HRESULT hr = S_OK;
	const char *func = "GetLineType";

	// 路線コードの取得
	CComVariant vaRoadCode;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::ROAD_CODE, &vaRoadCode))) return hr;
	vaRoadCode.ChangeType(VT_I4);

	// Direction_Cを取得
	CComVariant vaDirection;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::DIRECTION_C, &vaDirection))) return hr;
	vaDirection.ChangeType(VT_I4);

	// Road_Code_ListのRowを取得
	_IRowPtr ipRoadCode;
	if (FAILED(cHwyInfo->eRoadCodeList.SearchByRoadCode(vaRoadCode.lVal, &ipRoadCode))) return hr;
	if (ipRoadCode == NULL)
		return GDBERROR2(E_UNEXPECTED, func);

	// ROAD_CODE_LIST::LINEDIRTYPE_Cの取得
	CComVariant vaLineDirType;
	if (FAILED(hr = GDBGetValue(ipRoadCode, sindy::road_code_list::LINEDIRTYPE_C, &vaLineDirType))) return hr;
	vaLineDirType.ChangeType(VT_I4);

	// DIRECTION_CとLINEDIRTYPE_Cから、LineTypeを求める
	switch (vaDirection.lVal)
	{
		case sindy::direction::direction_no:					*cLineType = point_pos_code::direction; break;
		case sindy::direction::direction_up:
			switch (vaLineDirType.lVal)
			{
			case sindy::road_code_list::linedirtype_updown:		*cLineType = point_pos_code::up; break;
			case sindy::road_code_list::linedirtype_inout:		*cLineType = point_pos_code::in; break;
			case sindy::road_code_list::linedirtype_eastwest:	*cLineType = point_pos_code::west; break;
			default: return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		case sindy::direction::direction_down:
			switch (vaLineDirType.lVal)
			{
			case sindy::road_code_list::linedirtype_updown:		*cLineType = point_pos_code::down; break;
			case sindy::road_code_list::linedirtype_inout:		*cLineType = point_pos_code::out; break;
			case sindy::road_code_list::linedirtype_eastwest:	*cLineType = point_pos_code::east; break;
			default: return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		case sindy::direction::direction_common:				*cLineType = point_pos_code::entrance; break;
		default: return GDBERROR2(E_UNEXPECTED, func);
	}

	return S_OK;
}


static void AdjustMeshCode(ZMeshPos *cMeshPos)
{
	if (cMeshPos->eX < 0)
	{
		while (1)
		{
			cMeshPos->eX += 1000000;
			cMeshPos->eMesh = ZMap::GetMovedMesh(cMeshPos->eMesh, -1, 0);
			if (0 <= cMeshPos->eX && cMeshPos->eX <= 1000000) break;
		}
	}
	else if (cMeshPos->eX > 1000000)
	{
		while (1)
		{
			cMeshPos->eX -= 1000000;
			cMeshPos->eMesh = ZMap::GetMovedMesh(cMeshPos->eMesh, 1, 0);
			if (0 <= cMeshPos->eX && cMeshPos->eX <= 1000000) break;
		}
	}

	if (cMeshPos->eY < 0)
	{
		while (1)
		{
			cMeshPos->eY += 1000000;
			cMeshPos->eMesh = ZMap::GetMovedMesh(cMeshPos->eMesh, 0, -1);
			if (0 <= cMeshPos->eY && cMeshPos->eY <= 1000000) break;
		}
	}
	else if (cMeshPos->eY > 1000000)
	{
		while (1)
		{
			cMeshPos->eY -= 1000000;
			cMeshPos->eMesh = ZMap::GetMovedMesh(cMeshPos->eMesh, 0, 1);
			if (0 <= cMeshPos->eY && cMeshPos->eY <= 1000000) break;
		}
	}

	return;
}

namespace {

/**
 * @brief I/Nの出入口コードの取得
 * @param[in]	cpAccessPoint	対象AccessPointオブジェクト
 * @return	I/Nの出入口コード
 */
long GetInOut(IFeature* cpAccessPoint)
{
	long aSindyInOut = cpAccessPoint->GetValue( cpAccessPoint->Fields->_FindField(_bstr_t(sindy::access_point::INOUT_C.c_str())) );
	return Sindy2INCode(in::schema::roadpoint::inout::tInOut, aSindyInOut);
}

/**
 * @brief リンク集合から上下線種別コードを決定する
 * @note	上下両方が見つかったら「上下線共通」とする
 * @param[in]	cLinkList	リンク集合
 * @return	求められた上下線種別コード（求められない場合はNULL）
 */
std::auto_ptr<long> DecideDirectionByLinks( const GDBOBJLIST& cLinkList )
{
	bool aIsRoadLinkFound = false;
	long aINDirection = in::schema::fclpoint::direction::nodir;
	for (GDBOBJITER aLinkIt = cLinkList.begin(); aLinkIt != cLinkList.end(); ++aLinkIt)
	{
		const IFeaturePtr& ipLink = aLinkIt->second;
#ifdef	_DEBUG
		long aLinkID = ipLink->OID;
#endif
		long aRoadLinkUpDownClassFieldIndex = ipLink->Fields->_FindField(_bstr_t(sindy::road_link::UPDOWNCLASS_C.c_str()));
		long aUpDownClass = ipLink->GetValue(aRoadLinkUpDownClassFieldIndex);
		switch (aUpDownClass)
		{
			using namespace sindy::road_link;
			using namespace in::schema::fclpoint::direction;
			case updownclass_up:
				if (aINDirection == nodir) { aINDirection = up; }
				if (aINDirection == down)  { aINDirection = common; }
				aIsRoadLinkFound = true;
				break;
			case updownclass_down:
				if (aINDirection == nodir) { aINDirection = down; }
				if (aINDirection == up)    { aINDirection = common; }
				aIsRoadLinkFound = true;
				break;
			case updownclass_nodir:
			case updownclass_nothwy:	// 一般有料道路ではUpDownClass_c＝「対象外」なので、その場合は「方向なし」にする
				aIsRoadLinkFound = true;
				break;
			case updownclass_common:
				aINDirection = in::schema::fclpoint::direction::common;
				aIsRoadLinkFound = true;
				break;
			case updownclass_etcramp:
			case updownclass_etcaccess:
				ATLASSERT(0);
				break;
			default:
				ATLASSERT(0);
		}
	}

	return aIsRoadLinkFound ? std::auto_ptr<long>(new long(aINDirection)) : std::auto_ptr<long>(NULL);
}

/**
 * @brief リンクの集合から上下線種別コードを求める
 * @note	与えられたフィーチャに接続するリンクの集合の中に与えられた路線コードを持つリンクが含まれていたら、
 *			そのリンクのUpDownClass_cから求められる上下線種別コードを返す。
 * @note	[Bug9550] 無料区間のリンクを有料区間と同様に扱うように修正。
 * @param[in]	cHwyInfo	Road_Linkフィーチャクラス取得用のSINDY2INHWYINFO
 * @param[in]	cpFeature	対象フィーチャ（Highway_NodeまたはAccess_Point)
 * @param[in]	cRoadCode	リンク集合のうちいずれかのリンクが持っていると期待される路線コード
 * @return	求められた上下線種別コード（上記のようなリンクが見つからず、求められなかった場合はNULL）
 */
std::auto_ptr<long> DecideDirectionByConnectedLinks(SINDY2INHWYINFO *cHwyInfo, IFeature* cpFeature, long cRoadCode)
{
#ifdef _DEBUG
	long aOID = cpFeature->GetOID();
#endif
	HRESULT hr = S_OK;
	using namespace sindy_highway_tools;

	// 対応ノードの取得
	IFeaturePtr ipNode;
	if (FAILED(hr = cHwyInfo->GetHwyNode(cpFeature, &ipNode)))
		throw std::runtime_error("対応ノードの取得に失敗");
	if (ipNode == NULL)
		throw std::runtime_error("対応するノードが存在しない");

	// 接続リンクの取得
	GDBOBJLIST aLinkList;
	if (FAILED(hr = cHwyInfo->GetLink(ipNode, sindy::rnw::relation::fromto, &aLinkList)))
		throw std::runtime_error("接続リンクの取得に失敗");

	IFeatureClassPtr ipRoadLinkFeatureClass(cHwyInfo->eRoadLink.GetFeatureClass());
	static long aRoadLinkRoadCodeFieldIndex = ipRoadLinkFeatureClass->Fields->_FindField(_bstr_t(sindy::road_link::ROAD_CODE.c_str()));

	// 接続リンクの中から、与えられた路線コードを持つものを取り出す
	GDBOBJLIST aTargetLinkList;
	for (GDBOBJITER aLinkIt = aLinkList.begin(); aLinkIt != aLinkList.end(); ++aLinkIt)
	{
		const _IRowPtr& ipLink = aLinkIt->second;
		long aLinkID = ipLink->OID;

		long aRoadLinkRoadCode = ipLink->GetValue(aRoadLinkRoadCodeFieldIndex);
		if (aRoadLinkRoadCode == cRoadCode || aRoadLinkRoadCode == cRoadCode + FREEZONE_ADDITIONAL_ROADCODE )		// [Bug9550] 無料区間のリンクも有料区間と同様に扱う
		{
			aTargetLinkList.insert( std::make_pair( aLinkID, ipLink ) );
		}
	}

	// リンク集合から上下線種別コードを求める
	return DecideDirectionByLinks( aTargetLinkList );
}

/**
 * @brief 上下線種別コードの更新
 * @note	「複数のオブジェクトから上下線種別コードを決定する」という処理で、
			１つのオブジェクトから上下線種別コードを決める度に全体の上下線種別コードを更新する処理をこの関数で行う
 * @param[in]		cNewDirection		新たに見つかったSiNDYのDirection_C
 * @param[in/out]	cAllDirection		全体の上下線種別コード（更新対象）
 */
void updateINDirectionCode( long cNewDirection, long& cAllDirection )
{
	switch (cNewDirection)
	{
		using namespace in::schema::fclpoint::direction;
		case nodir:
			break;
		case up:
			if		(cAllDirection == nodir)	{ cAllDirection = up; }
			else if (cAllDirection == down)		{ cAllDirection = common; }
			break;
		case down:
			if		(cAllDirection == nodir)	{ cAllDirection = down; }
			else if	(cAllDirection == up)		{ cAllDirection = common; }
			break;
		case common:
			cAllDirection = common;
			break;
		default:
			ATLASSERT(0);
	}
}

/**
 * @brief JCTの他路線への合流点にあるアクセスポイントの上下線種別コードを求める
 * @param[in]	cpAccessPoint	対象アクセスポイント
 * @param[in]	cHwyInfo	追跡クラスSindyFindHwyNodeに渡すためのSINDY2INHWYINFO
 * @return	求められた上下線種別コード
 */
long DirectionOfInterflowAP( IFeature* cpAccessPoint, SINDY2INHWYINFO *cHwyInfo )
{
	HRESULT hr = S_OK;
#ifdef	_DEBUG
	long aOID = cpAccessPoint->GetOID();
#endif

	// 対応ノードの取得
	IFeaturePtr ipNode;
	if (FAILED(hr = cHwyInfo->GetHwyNode(cpAccessPoint, &ipNode)))
		{ throw std::runtime_error("対応ノードの取得に失敗"); }
	if (ipNode == NULL)
		{ throw std::runtime_error("対応するノードが存在しない"); }

	// 追跡の設定
	SindyFindHwyNode aFindHwyNode(cHwyInfo);
	static long aRoadCodeFieldIndex = cpAccessPoint->Fields->_FindField(_bstr_t(sindy::access_point::ROAD_CODE.c_str()));
	long aRoadCode = cpAccessPoint->GetValue( aRoadCodeFieldIndex );
	static long aRoadSeqFieldIndex = cpAccessPoint->Fields->_FindField(_bstr_t(sindy::access_point::ROAD_SEQ.c_str()));
	long aRoadSeq  = cpAccessPoint->GetValue( aRoadSeqFieldIndex );
	aFindHwyNode.set_facil_code( aRoadCode, aRoadSeq );

	// 追跡
	bool aReachiability = true;
	if (FAILED(hr = SindyRoadLinkTrace(cHwyInfo, ipNode, &aFindHwyNode,  &aReachiability)))
	{
		ATLASSERT(0);
	}
	const HwyNodeArray aFoundHwyNodeArray = aFindHwyNode.getFoundHwyNodeArray();

	long aINDirection = in::schema::fclpoint::direction::nodir;
	if (aFoundHwyNodeArray.size() != 0)
	{
		// 各ハイウェイノードについて上下線種別コードを決定し、上下両方が見つかったら「上下線共通」にする
		BOOST_FOREACH (IFeaturePtr ipHwyNode, aFoundHwyNodeArray)
		{
			// 接続リンクから上下線種別コードを求める
			std::auto_ptr<long> pDirection = DecideDirectionByConnectedLinks(cHwyInfo, ipHwyNode, aRoadCode);
			if (! pDirection.get())		// [Bug9550] NULLの場合に落ちていたので、落ちないように修正。
				{ continue; }
			updateINDirectionCode( *pDirection, aINDirection );
		}
	}
	// [Bug7303] ハイウェイノードが見つからない場合は手法を変える
	else
	{
		// 接続リンクの取得
		GDBOBJLIST aLinkList;
		if (FAILED(hr = cHwyInfo->GetLink(ipNode, sindy::rnw::relation::fromto, &aLinkList)))
			{ throw std::runtime_error("接続リンクの取得に失敗"); }

		// 接続リンク群が持つ0以外の路線コードを全て取得
		typedef long RoadCode;
		std::set<RoadCode> aRoadCodes;
		BOOST_FOREACH ( const GDBOBJLIST::value_type& aElement, aLinkList )
		{
			_IRowPtr ipConnectedLink = aElement.second;
			long aLinkID = ipConnectedLink->OID;

			static long aRoadLinkRoadCodeFieldIndex = ipConnectedLink->Fields->_FindField( _bstr_t(sindy::road_link::ROAD_CODE.c_str()) );
			long aConnectedLinkRoadCode = ipConnectedLink->GetValue( aRoadLinkRoadCodeFieldIndex );
			if ( aConnectedLinkRoadCode != 0 )
			{
				aRoadCodes.insert( aConnectedLinkRoadCode );
			}
		}

		// 路線コードの数と同じ回数分だけ、アクセスポイントの位置から追跡を実行し、目的のリンクが見つかったらaAllFoundLinksに格納
		GDBOBJLIST aAllFoundLinks;
		BOOST_FOREACH ( long aConnectedLinkRoadCode, aRoadCodes )
		{
			SindyFindMainLineFromJCTInterflow aSindyFindMainLineFromJCTInterflow( aRoadCode, aConnectedLinkRoadCode );
			aSindyFindMainLineFromJCTInterflow.set_direction(sindy::rnw::relation::back);

			bool aReachiability = true;
			if (FAILED(hr = SindyRoadLinkTrace(cHwyInfo, ipNode, &aSindyFindMainLineFromJCTInterflow, &aReachiability)))
			{
				ATLASSERT(0);
			}

			const GDBOBJLIST& aFoundLinks = aSindyFindMainLineFromJCTInterflow.getFoundLinks();
			std::copy( aFoundLinks.begin(), aFoundLinks.end(), std::inserter( aAllFoundLinks, aAllFoundLinks.begin() ) );
		}

		// リンク集合から上下線種別コードを求める
		std::auto_ptr<long> pDirection = DecideDirectionByLinks( aAllFoundLinks );
		if (pDirection.get())
		{
			aINDirection = *pDirection;
		}
	}

	return aINDirection;
}

/**
 * @brief fclpoint.txt用の上下線種別コードを求める
 * @note	[Bug6428]対応のため追加
 * @param[in]	cpAccessPoint	対象AccessPointオブジェクト
 * @param[in]	cHwyInfo	Facil_Info_Point取得用のSINDY2INHWYINFO
 * @param[in]	cPntPos		ポイント位置コード
 * @return	求められた上下線種別コード
 * @note	失敗した場合はstd::runtime_error例外を送出する
 */
long GetDirection(IFeature* cpAccessPoint, SINDY2INHWYINFO *cHwyInfo, long cPntPos)
{
#ifdef _DEBUG
	long aOID = cpAccessPoint->GetOID();
	std::cout << "\t" << "Access_Point" << "\t" << aOID << "\t" << "GetDirection()" << std::endl;
#endif
	HRESULT hr = S_OK;

	// アクセスポイントが属する施設の施設種別コードの取得
	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(cpAccessPoint, &ipFacilInfoPoint)))
		throw std::runtime_error("Access_Pointが属する施設のFacil_Info_Pointの取得に失敗");
	long aFacilClass = ipFacilInfoPoint->GetValue( ipFacilInfoPoint->Fields->_FindField(_bstr_t(sindy::facil_info_point::FACILCLASS_C.c_str())) );

	// ハイウェイモードフラグの取得
	bool aHwyModeFlag = (cpAccessPoint->GetValue( cpAccessPoint->Fields->_FindField(_bstr_t(sindy::access_point::HWYMODE_F.c_str())) ).lVal != 0);
	long aINDirection = 0;
	// 属する施設がJCT以外であるか、またはアクセスポイントがハイウェイモード対象外の場合
	if (aFacilClass != sindy::facil_info_point::facilclass_jct || ! aHwyModeFlag)
	{
		// アクセスポイントのDirection_Cにより上下線種別コードを決定する
		static long aDirectionCFieldIndex = cpAccessPoint->Fields->_FindField(_bstr_t(sindy::access_point::DIRECTION_C.c_str()));
		long aSindyDirection = cpAccessPoint->GetValue( aDirectionCFieldIndex );

		// ポイント位置コードが「入口」もしくは「出口」、なおかつ、　属する施設がIC,ランプ,スマートICの場合のみ
		// 重なっているアクセスポイントが存在することを考慮して上下線種別コードを決定する
		if (cPntPos == point_pos_code::entrance)
		{
			IFeaturePtr ipAccessPoint2;
			switch (aFacilClass)
			{
				case sindy::facil_info_point::facilclass_ramp:
				case sindy::facil_info_point::facilclass_ic:
				case sindy::facil_info_point::facilclass_smartic:
					if (FAILED(hr = FindRevdirAccessPoint(cHwyInfo, cpAccessPoint, &ipAccessPoint2))) return hr;
						break;
				default:
					break;
			}
			if (ipAccessPoint2 != NULL)
				aSindyDirection = sindy::direction::direction_common;
		}
		aINDirection = Sindy2INCode(in::schema::fclpoint::direction::tDirection, aSindyDirection);
	}
	// アクセスポイントがハイウェイモード対象で、かつ属する施設がJCTである場合
	else
	{
		long aAccessPointRoadCode = cpAccessPoint->GetValue( cpAccessPoint->Fields->_FindField(_bstr_t(sindy::access_point::ROAD_CODE.c_str())) );

		// 接続リンクの中に、路線コードがアクセスポイントと等しいリンクが含まれていたら、
		// 着目中のアクセスポイントはそれが属する本線上にあるものと見なし、そのリンクのUpDownClass_cで上下線種別コードを決定する
		std::auto_ptr<long> pDirection = DecideDirectionByConnectedLinks( cHwyInfo, cpAccessPoint, aAccessPointRoadCode );
		if (pDirection.get())
		{
			aINDirection = *pDirection;
		}
		// 上記のようなリンクが見つからなければ、着目中のアクセスポイントはそれが属する本線上にないもの（即ち、他路線からの合流点にあるもの）と見なし、
		// 渡り線リンクを逆に辿って本線リンクを探し、そのリンクのUpDownClass_cで上下線種別コードを決定する
		else
		{
			aINDirection = DirectionOfInterflowAP( cpAccessPoint, cHwyInfo );
		}
	}

	// SiNDYの整備仕様から判断すると、上下線種別コードが「方向なし」になることはないはずなので、なったらエラーメッセージを出しておく
	if (aINDirection == in::schema::fclpoint::direction::nodir)
	{
		PrintError(cpAccessPoint, "上下線種別コードが「方向なし」");
	}

	return aINDirection;
}

}	// anonymous namespace


SINDY2ROADPOINT::SINDY2ROADPOINT(SINDY2INHWYINFO *cHwyInfo) :
SINDY2INHWY(cHwyInfo)
{
}

SINDY2ROADPOINT::~SINDY2ROADPOINT()
{
}


HRESULT SINDY2ROADPOINT::Convert()
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::Convert";

	typedef std::set<long> OIDArray;
	OIDArray aNotForOutputIDs;		// 出力対象外オブジェクトID列

#ifdef	_DEBUG
	long count = 0;
#endif
	for (GDBOBJITER aIter = mpHwyInfo->eAccessPoint.begin(); aIter != mpHwyInfo->eAccessPoint.end(); aIter++)
	{
#ifdef	_DEBUG
		std::cerr << "Creating RoadPoint..." << ++count << "/" << mpHwyInfo->eAccessPoint.size() << "\r";
#endif
		IFeaturePtr ipAccessPoint = aIter->second;

		// 出力対象外オブジェクトID列に含まれていたらスキップ
		long aOID = ipAccessPoint->GetOID();
		OIDArray::iterator aIDIt = aNotForOutputIDs.find(aOID);
		if (aIDIt != aNotForOutputIDs.end())
			continue;

		RoadPoint aRoadPoint;

		// HWYMODE対象かどうかを確認する
		if (FAILED(hr = GetHwyModeFlag(mpHwyInfo, ipAccessPoint, &aRoadPoint.eHwyModeFlag)))
		{
			PrintError(ipAccessPoint, "ハイウェイモードフラグの取得に失敗");
			return hr;
		}

		// 施設代表点を取得する
		if (FAILED(hr = GetPrefPoint(mpHwyInfo, ipAccessPoint, &aRoadPoint.ePrefMesh, &aRoadPoint.ePrefX, &aRoadPoint.ePrefY)))
		{
			PrintError(ipAccessPoint, "施設代表点の取得に失敗");
			return hr;
		}

		// SiRiUS用データの施設代表点を取得する
		if (FAILED(hr = GetPrefPointForSirius(mpHwyInfo, ipAccessPoint, &aRoadPoint.ePrefMeshForSirius, &aRoadPoint.ePrefXForSirius, &aRoadPoint.ePrefYForSirius)))
		{
			PrintError(ipAccessPoint, "SiRiUS用データの設代表点の取得に失敗");
			return hr;
		}

		// メッシュ内シーケンスの取得
		if (FAILED(hr = GetMeshSequence(ipAccessPoint, &aRoadPoint.eAccessPointID)))
		{
			PrintError(ipAccessPoint, "メッシュ内シーケンスの取得に失敗");
			return hr;
		}

		// 2次メッシュコード・正規化座標
		if (FAILED(hr = GetBaseMeshCoord(ipAccessPoint, &aRoadPoint.eMesh, &aRoadPoint.eX, &aRoadPoint.eY)))
		{
			PrintError(ipAccessPoint, "正規化座標の取得に失敗");
			return hr;
		}

		// 市区町村コードの取得
		if (FAILED(hr = GetBaseAdmin(mpHwyInfo, ipAccessPoint, &aRoadPoint.eAdmin)))
		{
			PrintError(ipAccessPoint, "市区町村コードの取得に失敗");
			return hr;
		}

		// 施設代表ポイントの市区町村コードの取得
		if (FAILED(hr = GetFacilInfoPointAdmin(mpHwyInfo, ipAccessPoint, &aRoadPoint.eFacilInfoPointAdmin)))
		{
			PrintError(ipAccessPoint, "施設代表ポイントの市区町村コードの取得に失敗");
			return hr;
		}

		// 施設代表ポイントのObjectIDの取得
		if (FAILED(hr = GetFacilInfoPointID(mpHwyInfo, ipAccessPoint, &aRoadPoint.eFacilInfoPointID)))
		{
			PrintError(ipAccessPoint, "施設代表ポイントのObjectIDの取得に失敗");
			return hr;
		}

		// 路線内シーケンス番号の取得
		if (FAILED(hr = GetSequence(ipAccessPoint, &aRoadPoint.eSeq)))
		{
			PrintError(ipAccessPoint, "路線内シーケンスの取得に失敗");
			return hr;
		}

		// 検索用施設種別コードの取得
		if (FAILED(hr = GetSearchClass(mpHwyInfo, ipAccessPoint, &aRoadPoint.eSearchClass)))
		{
			PrintError(ipAccessPoint, "検索用施設種別コードの取得に失敗");
			return hr;
		}

		// 路線コードの取得
		if (FAILED(hr = GetRoadCode(mpHwyInfo, ipAccessPoint, &aRoadPoint.eRoadCode)))
		{
			PrintError(ipAccessPoint, "路線コードの取得に失敗");
			return hr;
		}

		// 施設種別＋出入口コードの取得
		if (FAILED(hr = GetFacilClassAndInOutCode(mpHwyInfo, ipAccessPoint, &aRoadPoint.eFacilClassAndInOutCode)))
		{
			PrintError(ipAccessPoint, "施設種別＋出入口コードの取得に失敗");
			return hr;
		}

		// 施設種別コード
		aRoadPoint.eFacilClass = GetFacilClass(ipAccessPoint, mpHwyInfo);

		// 出入口コード
		aRoadPoint.eInOut = GetInOut(ipAccessPoint);

		// ポイント位置コードの取得
		if (FAILED(hr = GetPntPos(mpHwyInfo, ipAccessPoint, &aRoadPoint.ePntPos, &aNotForOutputIDs)))
		{
			PrintError(ipAccessPoint, "ポイント位置コードの取得に失敗");
			return hr;
		}

		// ポイント位置名称、ポイント位置読みの取得
		if (FAILED(hr = GetPntName(ipAccessPoint, aRoadPoint.ePntPos, &aRoadPoint.ePntName, &aRoadPoint.ePntYomi)))
		{
			PrintError(ipAccessPoint, "ポイント位置名称の取得に失敗");
			return hr;
		}

		// 施設名称＋読み
		if (FAILED(hr = GetName(mpHwyInfo, ipAccessPoint, &aRoadPoint.eName, &aRoadPoint.eYomi)))
		{
			PrintError(ipAccessPoint, "施設名称の取得に失敗");
			return hr;
		}

		// SA/PA情報
		if (FAILED(hr = GetSapaInfo(ipAccessPoint, &aRoadPoint.eSapaInfo)))
		{
			PrintError(ipAccessPoint, "SA/PA情報の取得に失敗");
			return hr;
		}

		// 独自名称フラグ
		if (FAILED(hr = GetOriginalFlag(mpHwyInfo, ipAccessPoint, &aRoadPoint.eOriginal)))
		{
			PrintError(ipAccessPoint, "独自名称フラグの取得に失敗");
			return hr;
		}

		// ダミー施設情報フラグ
		if (FAILED(hr =GetDummyFacilFlag(mpHwyInfo, ipAccessPoint, &aRoadPoint.eDummyFacil)))
		{
			PrintError(ipAccessPoint, "ダミー施設情報フラグの取得に失敗");
			return hr;
		}

		// 上下線種別コード
		try {
			aRoadPoint.eDirection = GetDirection(ipAccessPoint, mpHwyInfo, aRoadPoint.ePntPos);
		}
		catch (...)
		{
			PrintError(ipAccessPoint, "上下線種別コードの取得に失敗");
			return E_FAIL;
		}

		mRoadPointArray.insert(aRoadPoint);
	}

	return S_OK;
}

HRESULT SINDY2ROADPOINT::Write(std::string cFileName, bool cHwyModeFlag, long cFormat, long cJctDataType)
{
	HRESULT hr = S_OK;

	RoadPointArray* pData;
	if (cJctDataType == jct_data_type::jct_branch)	{ pData = &mRoadPointBranchArray; }
	else											{ pData = &mRoadPointArray; }

	switch(cFormat)
	{
		using namespace file_format;
		case roadpoint_smartic_added:
		case roadpoint_etc_specific_toll:
			hr = Write(pData, cFileName, cHwyModeFlag, cFormat);
			break;
		case fclrep:
		case fclpoint:
			hr = WriteForSirius(pData, cFileName, cFormat);
			break;
		default:
			ATLASSERT(0);
	}

	return hr;
}

HRESULT SINDY2ROADPOINT::Write(RoadPointArray* cpData, std::string cFileName, bool cHwyModeFlag, long cFormat)
{
	const char *func = "SINDY2ROADPOINT::Write";

	using namespace file_format;
	ATLASSERT(cFormat != fclrep && cFormat != fclpoint);

	std::ofstream aFile(cFileName.c_str(), ios::binary);
	if (! aFile.is_open()) return GDBERROR2(E_UNEXPECTED, func);

	BOOST_FOREACH (const RoadPoint& aRoadPoint, *cpData)
	{
		if (cHwyModeFlag != aRoadPoint.eHwyModeFlag)
			continue;

		// 代表ポイント2次メッシュコード
		aFile << std::dec << aRoadPoint.ePrefMesh;

		// 代表ポイント正規化X座標
		aFile << "\t";
		aFile << std::dec << aRoadPoint.ePrefX;

		// 代表ポイント正規化Y座標
		aFile << "\t";
		aFile << std::dec << aRoadPoint.ePrefY;

		// メッシュ内シーケンス番号
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eAccessPointID;

		// 施設ポイント2次メッシュコード
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eMesh;

		// 施設ポイント正規化X座標
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eX;

		// 施設ポイント正規化Y座標
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eY;

		// 市区町村コード
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eAdmin;

		switch(cFormat)
		{
			case roadpoint_etc_specific_toll:

				// 路線コード
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eRoadCode;
		}

		// 路線内シーケンス番号
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eSeq;

		// 道路施設名称
		aFile << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.eName.c_str());

		// 道路施設名称読み
		aFile << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.eYomi.c_str());

		switch(cFormat)
		{
			case roadpoint_smartic_added:

				// 路線コード
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eRoadCode;

				// 施設種別コード
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eFacilClassAndInOutCode;

				break;

			case roadpoint_etc_specific_toll:

				// 施設種別コード
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eFacilClass;

				// 出入口コード
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eInOut;

				break;

			default:
				ATLASSERT(0);
		}

		// ポイント位置コード
		aFile << "\t";
		aFile << std::dec << aRoadPoint.ePntPos;

		// ポイント位置名称＆読み
		aFile << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.ePntName.c_str());
		aFile << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.ePntYomi.c_str());


		// SA/PA情報
		aFile << "\t";
		for (unsigned long j = 0; j < aRoadPoint.eSapaInfo.size(); j++)
			aFile << std::dec << aRoadPoint.eSapaInfo[j];

		// 独自名称フラグ
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eOriginal;

		// ダミー施設情報フラグ
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eDummyFacil;

		aFile << std::endl;
	}

	return S_OK;
}

HRESULT SINDY2ROADPOINT::WriteForSirius(RoadPointArray* cpData, std::string cFileName, long cFormat)
{
	const char *func = "SINDY2ROADPOINT::WriteForSirius";

	std::ofstream aFile(cFileName.c_str(), ios::binary);
	if (! aFile.is_open()) return GDBERROR2(E_UNEXPECTED, func);

	std::pair<long, long> aPrevFacilCode;

	BOOST_FOREACH (const RoadPoint& aRoadPoint, *cpData)
	{
		// [Bug6456] 一般有料道路の情報も出力するように修正

		using namespace file_format;

		if (cFormat == fclrep)
		{
			// fclrep.txtでは、FacilCodeが１つ前と同じならスキップ
			std::pair<long, long> aCurrentFacilCode = std::make_pair(aRoadPoint.eRoadCode, aRoadPoint.eSeq);
			if (aPrevFacilCode == aCurrentFacilCode) { continue; }
			aPrevFacilCode = aCurrentFacilCode;
		}

		switch(cFormat)
		{
			case fclrep:
				// 代表ポイント2次メッシュコード
				aFile << std::dec << aRoadPoint.ePrefMeshForSirius;

				// 代表ポイント正規化X座標
				aFile << "\t";
				aFile << std::dec << aRoadPoint.ePrefXForSirius;

				// 代表ポイント正規化Y座標
				aFile << "\t";
				aFile << std::dec << aRoadPoint.ePrefYForSirius;

				// 施設代表ポイントの都道府県コード
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eFacilInfoPointAdmin / 1000;
				// 施設代表ポイントの市区町村コード
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eFacilInfoPointAdmin % 1000;
				break;

			case fclpoint:
				// 施設ポイント2次メッシュコード
				aFile << std::dec << aRoadPoint.eMesh;

				// 施設ポイント正規化X座標
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eX;

				// 施設ポイント正規化Y座標
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eY;

				// 都道府県コード
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eAdmin / 1000;
				// 市区町村コード
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eAdmin % 1000;
				break;

			default:
				ATLASSERT(0);
		}


		// 管理団体コード
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eRoadCode / 10000;

		// 路線シーケンス番号
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eRoadCode % 10000;

		// 路線内シーケンス番号
		// [Bug6309]のついでにフォーマット変更 路線コードの次に移動
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eSeq;

		if (cFormat == fclpoint)
		{
			// 上下線種別コード
			aFile << "\t";
			aFile << std::dec << aRoadPoint.eDirection;
		}

		// 施設ID
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eFacilInfoPointID;

		if (cFormat == fclpoint)
		{
			// 施設ポイントID
			aFile << "\t";
			aFile << std::dec << aRoadPoint.eAccessPointID;

			// ダミー施設情報フラグ
			aFile << "\t";
			aFile << std::dec << aRoadPoint.eDummyFacil;
		}
		
		switch(cFormat)
		{
			case fclrep:
				// 検索用施設種別コード
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eSearchClass;

				// 道路施設名称
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.eName.c_str());

				// 施設名称読み
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.eYomi.c_str());

				break;

			case fclpoint:
				// 施設ポイント種別コード
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eFacilClassAndInOutCode;

				// ポイント位置コード
				aFile << "\t";
				aFile << std::dec << aRoadPoint.ePntPos;

				// ポイント位置名称＆読み
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.ePntName.c_str());
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.ePntYomi.c_str());
				
				// SA/PA情報
				aFile << "\t";
				for (unsigned long j = 0; j < aRoadPoint.eSapaInfo.size(); j++)
					aFile << std::dec << aRoadPoint.eSapaInfo[j];

				break;

			default:
				ATLASSERT(0);
		}

		aFile << std::endl;
	}

	return S_OK;
}

/**
 * @brief テスト用 格納された値を全て出力する
 * @param[in]	cFileName	出力ファイル名
 */
HRESULT SINDY2ROADPOINT::WriteAll(std::string cFileName)
{
	const char *func = "SINDY2ROADPOINT::WriteAll";

	std::ofstream aFile(cFileName.c_str(), ios::binary);
	if (! aFile.is_open()) return GDBERROR2(E_UNEXPECTED, func);

	for (RoadPointArray::const_iterator aIt = mRoadPointArray.begin(); aIt != mRoadPointArray.end(); ++aIt)
	{
		aFile << aIt->eHwyModeFlag << "\t";
		aFile << aIt->ePrefMesh << "\t";
		aFile << aIt->ePrefX << "\t";
		aFile << aIt->ePrefY << "\t";
		aFile << aIt->eAccessPointID << "\t";
		aFile << aIt->eMesh << "\t";
		aFile << aIt->eX << "\t";
		aFile << aIt->eY << "\t";
		aFile << aIt->eAdmin << "\t";
		aFile << aIt->eFacilInfoPointAdmin << "\t";
		aFile << aIt->eFacilInfoPointID << "\t";
		aFile << aIt->eSeq << "\t";
		aFile << aIt->eSearchClass << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aIt->eName.c_str()) << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aIt->eYomi.c_str()) << "\t";
		aFile << aIt->eRoadCode << "\t";
		aFile << aIt->eFacilClassAndInOutCode << "\t";
		aFile << aIt->ePntPos << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aIt->ePntName.c_str()) << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aIt->ePntYomi.c_str()) << "\t";
		for (unsigned long i=0; i<aIt->eSapaInfo.size(); ++i) { aFile << aIt->eSapaInfo[i]; } aFile << "\t";
		aFile << aIt->eOriginal << "\t";
		aFile << aIt->eDummyFacil << "\t";
		aFile << aIt->ePrefMeshForSirius << "\t";
		aFile << aIt->ePrefXForSirius << "\t";
		aFile << aIt->ePrefYForSirius << "\t";
		aFile << std::endl;
	}
	
	return S_OK;
}

/**
 * @brief 合流点のレコードを分岐点のレコードに置き換えたデータを作成する
 * @note	roadpoint.bunnki.txt, fclpoint.bunnki.txt用
 */
HRESULT SINDY2ROADPOINT::makeJctBranchData(void)
{
	HRESULT hr = S_OK;

	typedef std::pair<long, long> FacilCode;
	std::set<FacilCode> aCheckedFacil;	// チェック済みFacilCode

	using namespace in::schema::roadpoint;

	// 着目したアクセスポイントと同じFacilCodeを持つ全ての分岐ハイウェイノードを取得
	for (RoadPointArray::const_iterator aIt = mRoadPointArray.begin(); aIt != mRoadPointArray.end(); ++aIt)
	{
		// [Bug6456] 一般有料道路のレコードでも分岐データを作るように修正

		if (aIt->eFacilClassAndInOutCode == facil_class_and_inout_code::jct) 
		{
			// チェック済みのFacilCodeならスキップ
			FacilCode aFacilCode = std::make_pair(aIt->eRoadCode, aIt->eSeq);
			if (aCheckedFacil.find(aFacilCode) != aCheckedFacil.end()) { continue; }
			aCheckedFacil.insert(aFacilCode);

			// 同じFacilCodeを持つハイウェイノード集合の取得
			GDBHWYFACILRANGE aRange;
			mpHwyInfo->eHighwayNode.SearchByFacilCode(aFacilCode.first, aFacilCode.second, &aRange);

			// 着目中のレコードの座標を、その施設の分岐ハイウェイノードの座標に置き換える
			for (GDBHWYFACILITER aHwyNodeIt = aRange.first; aHwyNodeIt != aRange.second; ++aHwyNodeIt)
			{
				// 施設位置コードの取得
				long aFacilPos;
				if (FAILED(hr = SINDY2HWYNODE::GetFacilPos(mpHwyInfo, aHwyNodeIt->second, &aFacilPos)))
				{
					PrintError(aHwyNodeIt->second, "施設位置コードの取得に失敗");
					continue;
				}

				// JCT出路でなければスキップ
				if (aFacilPos != in::schema::hwynode::facil_pos_code::jct_ext) { continue; }

				// ハイウェイノードの正規化座標の取得
				long aMesh = 0, aX = 0, aY = 0;
				if (FAILED(hr = GetBaseMeshCoord(aHwyNodeIt->second, &aMesh, &aX, &aY)))
				{
					PrintError(aHwyNodeIt->second, "正規化座標の取得に失敗");
					continue;
				}

				RoadPoint aRoadPoint = *aIt;
				aRoadPoint.eMesh = aMesh;
				aRoadPoint.eX	 = aX;
				aRoadPoint.eY	 = aY;

				mRoadPointBranchArray.insert(aRoadPoint);
			}
		}
		else
		{
			mRoadPointBranchArray.insert(*aIt);
		}

	}

	aCheckedFacil.clear();
	RoadPointArray aData;

	for (RoadPointArray::const_iterator aIt1 = mRoadPointBranchArray.begin(); aIt1 != mRoadPointBranchArray.end(); ++aIt1)
	{

		// JCT以外ならスキップ
		if (aIt1->eFacilClassAndInOutCode != facil_class_and_inout_code::jct) {
			continue;
		}

		NvUnit aUnit1;		// ユニットコード
		aUnit1.eMesh = aIt1->eMesh;
		aUnit1.eX = 0;
		aUnit1.eY = 0;
		aUnit1.eLevel = 2;

		// チェック済みのFacilCodeならスキップ
		FacilCode aFacilCode = std::make_pair(aIt1->eRoadCode, aIt1->eSeq);
		if (aCheckedFacil.find(aFacilCode) != aCheckedFacil.end()) { continue; }
		aCheckedFacil.insert(aFacilCode);

		for (RoadPointArray::const_iterator aIt2 = mRoadPointBranchArray.begin(); aIt2 != mRoadPointBranchArray.end(); ++aIt2)
		{
			using namespace in::schema::roadpoint;
			if (aIt1 == aIt2)														{ continue; }
			if (aIt2->eFacilClassAndInOutCode != facil_class_and_inout_code::jct)	{ continue; }
			if (aIt1->eName != aIt2->eName)											{ continue; }
			if (aIt1->eRoadCode == aIt2->eRoadCode && aIt1->eSeq == aIt2->eSeq)		{ continue; }

			NvUnit aUnit2;
			aUnit2.eMesh = aIt2->eMesh;
			aUnit2.eX = 0;
			aUnit2.eY = 0;
			aUnit2.eLevel = 2;

			ZPoint aDiff = aUnit1.getDiff(aUnit2);

			int aDistance = int(aDiff.eX * aDiff.eX + aDiff.eY * aDiff.eY);
			if (aDistance > 2) { continue; }

			RoadPoint aRoadPoint = *aIt1;
			aRoadPoint.eMesh = aIt2->eMesh;
			aRoadPoint.eX = aIt2->eX;
			aRoadPoint.eY = aIt2->eY;
			
			aData.insert(aRoadPoint);
		}
	}

	for (RoadPointArray::const_iterator aIt = aData.begin(); aIt != aData.end(); ++aIt)
	{
		mRoadPointBranchArray.insert(*aIt);
	}

	return S_OK;
}


/**
 * @brief 施設名称変換（内部処理用関数）
 * @note	与えられたRoadPointArrayの施設名称を変換する
 * @param	cpFacilNameChangeTable	[in]	施設名称変換テーブル
 * @param	crData	[in/out]	変換対象データ
 * @param	cFileName	[in]	変換対象データに対応するファイル名（ログ出力のため指定）
 **/
void SINDY2ROADPOINT::changeFacilName(FacilNameChangeTable* cpFacilNameChangeTable, RoadPointArray& crData, std::string cFileName)
{
	std::vector<RoadPointArray::iterator>	aEraseRecordArray;	// 変換前の（削除すべき）レコードの列
	std::vector<RoadPoint>		aChangedRoadPointArray;			// 変換後の（追加すべき）レコードの列

	for (RoadPointArray::iterator aRPIt = crData.begin(); aRPIt != crData.end(); ++aRPIt)
	{
		std::auto_ptr<ChangedFacilName> pChangedFacilName = cpFacilNameChangeTable->getChangedFacilName(
			aRPIt->eRoadCode, aRPIt->eSeq, aRPIt->eName
		);
		if (pChangedFacilName.get() == NULL)
		{
			continue;	// 変換対象レコードでなければスキップ
		}
#ifdef	_DEBUG
		std::cout << cFileName << "\t" << "施設名称変換" << "\t" << aRPIt->eRoadCode << "\t" << aRPIt->eSeq << "\t";
		std::cout << aRPIt->eName << "→" << pChangedFacilName->m_Kanji << "\t";
		std::cout << aRPIt->eYomi << "→" << pChangedFacilName->m_Yomi << std::endl;
#endif
		// 施設名称変換
		RoadPoint aRoadPoint = *aRPIt;
		aRoadPoint.eName = pChangedFacilName->m_Kanji;
		aRoadPoint.eYomi = pChangedFacilName->m_Yomi;

		// 削除すべきレコードと追加すべきレコードを記録
		aEraseRecordArray.push_back(aRPIt);
		aChangedRoadPointArray.push_back(aRoadPoint);
	}

	// 削除
	for (std::vector<RoadPointArray::iterator>::iterator aIt = aEraseRecordArray.begin(); aIt != aEraseRecordArray.end(); ++aIt)
	{
		crData.erase(*aIt);
	}
	// 追加
	for (std::vector<RoadPoint>::iterator aIt = aChangedRoadPointArray.begin(); aIt != aChangedRoadPointArray.end(); ++aIt)
	{
		crData.insert(*aIt);
	}
}

void SINDY2ROADPOINT::changeFacilName(Customer cCustomer, FacilNameChangeTable* cpFacilNameChangeTable)
{
	if (! needFacilNameChange( cCustomer ) )
		{ return; }

	changeFacilName( cpFacilNameChangeTable );
}

void SINDY2ROADPOINT::changeFacilName( FacilNameChangeTable* cpFacilNameChangeTable )
{
	changeFacilName( cpFacilNameChangeTable, mRoadPointArray, "roadpoint.txt" );
	changeFacilName( cpFacilNameChangeTable, mRoadPointBranchArray, "roadpoint.bunnki.txt" );
}





HRESULT SINDY2ROADPOINT::GetFacilInfoPointID(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cID)
{
	HRESULT hr = S_OK;
	*cID = 0;

	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;

	long aOID;
	if (FAILED(hr = ipFacilInfoPoint->get_OID(&aOID))) return hr;
	*cID = aOID;

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetSearchClass(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cSearchClass)
{
	HRESULT hr = S_OK;
	*cSearchClass = 0;

	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;

	long aSindySearchClass = ipFacilInfoPoint->GetValue( ipFacilInfoPoint->Fields->_FindField(_bstr_t(sindy::facil_info_point::SEARCHCLASS_C.c_str())) );
	*cSearchClass = Sindy2INCode(in::schema::fclrep::searchclass::tSearchClass, aSindySearchClass);

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetRoadCode(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cRoadCode)
{
	HRESULT hr = S_OK;
	*cRoadCode = 0;

	CComVariant vaRoadCode;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::ROAD_CODE, &vaRoadCode))) return hr;

	*cRoadCode = vaRoadCode.lVal;

	return S_OK;
}

/// GetFacilPosInfo()から分離
HRESULT SINDY2ROADPOINT::GetFacilClassAndInOutCode(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cFacilClass)
{
	HRESULT hr = S_OK;
	*cFacilClass = 0;
	const char *func = "SINDY2ROADPOINT::GetFacilClassAndInOutCode";

	// 出入り口属性の取得
	CComVariant vaInOut;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::INOUT_C, &vaInOut))) return hr;
	vaInOut.ChangeType(VT_I4);

	// SiNDYの施設種別コードの取得
	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);
	CComVariant vaFacilClass;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::FACILCLASS_C, &vaFacilClass))) return hr;
	vaFacilClass.ChangeType(VT_I4);

	bool hwymode = true;
	if (FAILED(hr = cHwyInfo->isHwyMode(ipAccessPoint, &hwymode))) return hr;

	long aClass = 0;
	switch (vaFacilClass.lVal)
	{
		case sindy::facil_info_point::facilclass_sa:
			aClass = facil_class_and_inout_code::sa;
			break;
		case sindy::facil_info_point::facilclass_pa:
			aClass = facil_class_and_inout_code::pa;
			break;
		case sindy::facil_info_point::facilclass_jct:
			{
				if (hwymode)
				{
					aClass = facil_class_and_inout_code::jct;
				}
				else	/// roadpoint_normal.txtでは、JCTの入路と出路を区別する
				{
					switch (vaInOut.lVal)
					{
						case sindy::access_point::inout_enter:
							aClass = facil_class_and_inout_code::jct_ent;
							break;
						case sindy::access_point::inout_exit:
							aClass = facil_class_and_inout_code::jct_ext;
							break;
						default:
							return GDBERROR2(E_UNEXPECTED, func);
					}
				}
			}
			break;
		case sindy::facil_info_point::facilclass_ramp:
			switch (vaInOut.lVal)
			{
				case sindy::access_point::inout_enter:
					aClass = facil_class_and_inout_code::ramp_ent;
					break;
				case sindy::access_point::inout_exit:
					aClass = facil_class_and_inout_code::ramp_ext;
					break;
				default:
					return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		case sindy::facil_info_point::facilclass_ic:
			switch (vaInOut.lVal)
			{
			case sindy::access_point::inout_enter:
				aClass = facil_class_and_inout_code::ic_ent;
				break;
			case sindy::access_point::inout_exit:
				aClass = facil_class_and_inout_code::ic_ext;
				break;
			default:
				return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		case sindy::facil_info_point::facilclass_tollgate:
			aClass = facil_class_and_inout_code::tollgate;
			break;
		case sindy::facil_info_point::facilclass_smartic:
			switch (vaInOut.lVal)
			{
				case sindy::access_point::inout_enter:
					aClass = facil_class_and_inout_code::smartic_ent;
					break;
				case sindy::access_point::inout_exit:
					aClass = facil_class_and_inout_code::smartic_ext;
					break;
				default:
					return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		default:
			return GDBERROR2(E_UNEXPECTED, func);
	}

	*cFacilClass = aClass;

	return S_OK;
}


HRESULT SINDY2ROADPOINT::GetPntPos(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cPntPos, std::set<long> *cNotForOutputIDs)
{
	HRESULT hr = S_OK;
	*cPntPos = 0;
	const char *func = "SINDY2ROADPOINT::GetPntPos";

	// SiNDYの施設種別コードの取得
	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);
	CComVariant vaFacilClass;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::FACILCLASS_C, &vaFacilClass))) return hr;
	vaFacilClass.ChangeType(VT_I4);

	// 施設種別がIC、ランプ、スマートICの場合のみ、同位置に逆方向のアクセスポイントがあれば取得
	IFeaturePtr ipAccessPoint2;
	switch(vaFacilClass.lVal)
	{
		case sindy::facil_info_point::facilclass_ramp:
		case sindy::facil_info_point::facilclass_ic:
		case sindy::facil_info_point::facilclass_smartic:
			if (FAILED(hr = FindRevdirAccessPoint(cHwyInfo, ipAccessPoint, &ipAccessPoint2))) return hr;
			break;
		default:
			break;
	}

	// アクセスポイントの重なりを考慮しない場合のポイント位置コードを決定
	long aLineType = 0;
	if (FAILED(hr = GetLineType(cHwyInfo, ipAccessPoint, &aLineType))) return hr;

	/// IC、ランプ、スマートICのアクセスポイントが重なっていて、方向が逆であればentrance（入口もしくは出口）とする
	//  →上下線種別コード取得(GetDirection())時、ここで決定したポイント位置コード(入口もしくは出口)を使用している。
	//    ここでのポイント位置コードを「入口もしくは出口」にする条件に変更があった場合、GetDirection関数も見直す必要がある。
	if (ipAccessPoint2 != NULL)
	{
		long aLineType2 = 0;
		if (FAILED(hr = GetLineType(cHwyInfo, ipAccessPoint2, &aLineType2))) return hr;
		if ((aLineType == point_pos_code::up	&& aLineType2 == point_pos_code::down) ||
			(aLineType == point_pos_code::down	&& aLineType2 == point_pos_code::up	 ) ||
			(aLineType == point_pos_code::in	&& aLineType2 == point_pos_code::out ) ||
			(aLineType == point_pos_code::out	&& aLineType2 == point_pos_code::in  ) ||
			(aLineType == point_pos_code::west  && aLineType2 == point_pos_code::east) ||
			(aLineType == point_pos_code::east  && aLineType2 == point_pos_code::west))
		{
			aLineType = point_pos_code::entrance;
			long aOID = 0;
			if (FAILED(hr = ipAccessPoint2->get_OID(&aOID))) return GDBERROR(hr, func);
			if (cNotForOutputIDs != NULL)
			{
#ifdef	_DEBUG
				std::cout << "\t" << "Access_Point" << "\t" << aOID << "\t" << "出力対象外" << std::endl;
#endif
				cNotForOutputIDs->insert(aOID);
			}
		}
		else
		{
			ATLASSERT(0);	// ipAccessPoint2 != NULLの時点で条件を満たすので、ここに来ることはないはず
		}
	}

	*cPntPos = aLineType;

	return S_OK;
}


// ポイント位置コードから、ポイント位置名称と読みを求める
HRESULT SINDY2ROADPOINT::GetPntName(IFeaturePtr ipAccessPoint, long cPntPos, std::string *cName, std::string *cYomi)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetPntName";

	// 出入り口属性
	CComVariant vaInOut;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::INOUT_C, &vaInOut))) return hr;
	vaInOut.ChangeType(VT_I4);

	switch (cPntPos)
	{
		case point_pos_code::entrance:
			switch (vaInOut.lVal)
			{
				case sindy::inout::inout_enter:		*cName = "入口"; *cYomi = "イリグチ"; break;
				case sindy::inout::inout_exit:		*cName = "出口"; *cYomi = "デグチ"; break;
				default: return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		case point_pos_code::up:	*cName = "上り"; *cYomi = "ノボリ"; break;
		case point_pos_code::down:	*cName = "下り"; *cYomi = "クダリ"; break;
		case point_pos_code::in:		*cName = "内回り"; *cYomi = "ウチマワリ"; break;
		case point_pos_code::out:	*cName = "外回り"; *cYomi = "ソトマワリ"; break;
		case point_pos_code::west:	*cName = "西行き"; *cYomi = "ニシイキ"; break;
		case point_pos_code::east:	*cName = "東行き"; *cYomi = "ヒガシイキ"; break;
		case point_pos_code::direction:
			{
				CComVariant vaName, vaYomi;
				if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::DIRECTION_KANJI, &vaName))) return hr;
				if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::DIRECTION_YOMI, &vaYomi))) return hr;
				if (vaName.vt != VT_NULL && vaName.vt != VT_EMPTY)
				{
					vaName.ChangeType(VT_BSTR);
					*cName = COLE2T(vaName.bstrVal);
					if (*cName == " " || *cName == "　")
						std::cout << SindyErrorInfo(ipAccessPoint, "方面名称が入っていない");
				}
				if (vaYomi.vt != VT_NULL && vaYomi.vt != VT_EMPTY)
				{
					vaYomi.ChangeType(VT_BSTR);
					*cYomi = COLE2T(vaYomi.bstrVal);
					if (*cYomi == " " || *cYomi == "　")
						std::cout << SindyErrorInfo(ipAccessPoint, "方面読みが入っていない");
				}
			}
			break;
		default: return GDBERROR2(E_UNEXPECTED, func);
	}

	return S_OK;
}


HRESULT SINDY2ROADPOINT::GetOriginalFlag(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cOriginal)
{
	HRESULT hr = S_OK;

	*cOriginal = 0;

	IFeaturePtr ipFacilInfoPoint = NULL;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;

	CComVariant vaDummyName;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::DUMMYNAME_F, &vaDummyName))) return hr;
	vaDummyName.ChangeType(VT_I4);

	*cOriginal = vaDummyName.lVal;

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetSapaInfo(IFeaturePtr ipAccessPoint, std::vector<long> *cSapaInfo)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetSapaInfo";

	unsigned long aDigit = 25;	// SA/PA情報の桁数

	cSapaInfo->clear();
	for (long i = 0; sindy::access_point::SAPAINFO3[i].size(); i++)
	{
		CComVariant vaInfo;
		if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::SAPAINFO3[i], &vaInfo))) return hr;
		vaInfo.ChangeType(VT_I4);
		cSapaInfo->push_back(vaInfo.lVal);
	}

	// リザーブ領域の作成
	while(cSapaInfo->size() < aDigit)
	{
		cSapaInfo->push_back(0);
	}

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetDummyFacilFlag(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cDummyFacilFlag)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetDummyFacilFlag";

	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);

	CComVariant vaDummyFacilFlag;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::DUMMYFACIL_F, &vaDummyFacilFlag))) return hr;
	vaDummyFacilFlag.ChangeType(VT_I4);

	*cDummyFacilFlag = vaDummyFacilFlag.lVal;

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetName(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, std::string *cName, std::string *cYomi)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetName";

	*cName = *cYomi = "";

	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);

	CComVariant vaName, vaYomi;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::NAME_KANJI, &vaName))) return hr;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::NAME_YOMI, &vaYomi))) return hr;

	if (vaName.vt != VT_NULL && vaName.vt != VT_EMPTY)
	{
		vaName.ChangeType(VT_BSTR);
		*cName = COLE2T(vaName.bstrVal);
	}

	if (vaYomi.vt != VT_NULL && vaYomi.vt != VT_EMPTY)
	{
		vaYomi.ChangeType(VT_BSTR);
		*cYomi = COLE2T(vaYomi.bstrVal);
	}

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetSequence(IFeaturePtr ipAccessPoint, long *cSequence)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetSequence";

	*cSequence = 0;

	CComVariant vaSequence;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::ROAD_SEQ, &vaSequence))) return hr;
	vaSequence.ChangeType(VT_I4);

	*cSequence = vaSequence.lVal;

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetBaseAdmin(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cAdminCode)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetBaseAdmin";

	*cAdminCode = 0;

	// bug11633 フィーチャが複数取得された場合、住所5桁コードが一番小さい値を返す
	vector<IFeaturePtr> ipBaseAdmins;
	if (FAILED(hr = cHwyInfo->GetBaseAdmin(ipAccessPoint, ipBaseAdmins))) return hr;
	if (0 == ipBaseAdmins.size())
	{
		std::cerr << "Admin code is zero." << std::endl; return S_OK;
	}

	set<long> citycodeSet;
	CComVariant vaCityCode;
	for(auto ipBaseAdmin : ipBaseAdmins)
	{
		if (FAILED(hr = GDBGetValue(ipBaseAdmin, sindy::base_admin::CITYCODE, &vaCityCode))) return hr;
		vaCityCode.ChangeType(VT_I4);
		citycodeSet.insert(vaCityCode.lVal);
	}

	// 住所5桁コードが一番小さい値（先頭要素）を返す
	*cAdminCode = *(citycodeSet.begin());

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetFacilInfoPointAdmin(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cAdminCode)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetFacilInfoPointAdmin";

	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);

	if (FAILED(hr = GetBaseAdmin(cHwyInfo, ipFacilInfoPoint, cAdminCode))) return hr;

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetBaseMeshCoord(IFeaturePtr ipAccessPoint, long *cMesh, long *cX, long *cY)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetBaseMeshCoord";

	IGeometryPtr ipGeometry;
	if (FAILED(hr = ipAccessPoint->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);

	return GDBGetBaseMeshNormalizeCoord((IPointPtr)ipGeometry, cMesh, cX, cY);
}

HRESULT SINDY2ROADPOINT::GetMeshSequence(IFeaturePtr ipAccessPoint, long *cMeshSeq)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetMeshSequence";

	// オブジェクトIDを流用
	if (FAILED(hr = ipAccessPoint->get_OID(cMeshSeq))) return GDBERROR(hr, func);

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetHwyModeFlag(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, bool *cHwyModeFlag)
{
	return cHwyInfo->isHwyMode(ipAccessPoint, cHwyModeFlag);
}


HRESULT SINDY2ROADPOINT::GetPrefPoint(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cMesh, long *cX, long *cY)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetPrefPoint";

	*cMesh = *cX = *cY = 0;

	// 施設情報ポイントの取得
	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL)
		return GDBERROR2(E_UNEXPECTED, func);

	// 自動計算フラグのチェック
	CComVariant vaAutoPos;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::AUTOPOSITION_F, &vaAutoPos))) return hr;
	vaAutoPos.ChangeType(VT_I4);

	// 自動計算フラグがOFFの場合は施設情報ポイントの座標を取得
	if (! vaAutoPos.lVal)
	{
		IGeometryPtr ipGeometry = NULL;
		if (FAILED(hr = ipFacilInfoPoint->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);
		if (FAILED(hr = GDBGetBaseMeshNormalizeCoord((IPointPtr)ipGeometry, cMesh, cX, cY)))
			return hr;
		return S_OK;
	}

	// 自動計算フラグがONの場合は、重心位置を取得する
	long aRoadCode = 0, aRoadSeq = 0;
	if (FAILED(hr = cHwyInfo->GetFacilCode(ipAccessPoint, &aRoadCode, &aRoadSeq))) return hr;
	GDBHWYFACILRANGE aRange;
	if (FAILED(hr = cHwyInfo->eAccessPoint.SearchByFacilCode(aRoadCode, aRoadSeq, &aRange)))
		return hr;
	if (aRange.first == aRange.second)
		return GDBERROR2(E_UNEXPECTED, func);

	long aPointCount = 0;
	std::vector<double> vecX, vecY; // ソートしてから足していく(bug10673)

	for (GDBHWYFACILITER aIter = aRange.first; aIter != aRange.second; aIter++, aPointCount++)
	{
		IFeaturePtr ipFeature = aIter->second;
		IGeometryPtr ipGeometry;
		if (FAILED(hr = ipFeature->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);
		IPointPtr ipPoint(ipGeometry);
		ATLASSERT(ipPoint != NULL);

		double aX = 0, aY = 0;
		if (FAILED(hr = ipPoint->get_X(&aX))) return GDBERROR(hr, func);
		if (FAILED(hr = ipPoint->get_Y(&aY))) return GDBERROR(hr, func);

		vecX.push_back(aX);
		vecY.push_back(aY);
	}

	std::sort(vecX.begin(), vecX.end()); // ソート
	std::sort(vecY.begin(), vecY.end()); // ソート
	
	double aTotalX = 0.0, aTotalY = 0.0;
	for(int i = 0; i < aPointCount; ++i)
	{
		aTotalX += vecX[i];
		aTotalY += vecY[i];
	}

	// 平均値の取得
	aTotalX /= (double)aPointCount;
	aTotalY /= (double)aPointCount;

	// メッシュコード合わせ(+100万×100万を超えないよう調整)
	ZMeshPos aMeshPos(aTotalX, aTotalY);
	AdjustMeshCode(&aMeshPos);

	*cX = aMeshPos.eX;
	*cY = aMeshPos.eY;
	*cMesh = aMeshPos.eMesh;

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetPrefPointForSirius(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cMesh, long *cX, long *cY)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetPrefPointForSirius";

	*cMesh = *cX = *cY = 0;

	// 施設情報ポイントの取得
	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL)
		return GDBERROR2(E_UNEXPECTED, func);

	// 重心位置を取得する
	long aRoadCode = 0, aRoadSeq = 0;
	if (FAILED(hr = cHwyInfo->GetFacilCode(ipAccessPoint, &aRoadCode, &aRoadSeq))) return hr;
	GDBHWYFACILRANGE aRange;
	if (FAILED(hr = cHwyInfo->eAccessPoint.SearchByFacilCode(aRoadCode, aRoadSeq, &aRange)))
		return hr;
	if (aRange.first == aRange.second)
		return GDBERROR2(E_UNEXPECTED, func);

	long aPointCount = 0;
	std::vector<double> vecX, vecY; // ソートしてから足していく(bug10673)
	
	for (GDBHWYFACILITER aIter = aRange.first; aIter != aRange.second; aIter++)
	{
		IFeaturePtr ipFeature = aIter->second;
		++aPointCount;

		IGeometryPtr ipGeometry;
		if (FAILED(hr = ipFeature->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);
		IPointPtr ipPoint(ipGeometry);
		ATLASSERT(ipPoint != NULL);

		double aX = 0, aY = 0;
		if (FAILED(hr = ipPoint->get_X(&aX))) return GDBERROR(hr, func);
		if (FAILED(hr = ipPoint->get_Y(&aY))) return GDBERROR(hr, func);

		vecX.push_back(aX);
		vecY.push_back(aY);
	}

	std::sort(vecX.begin(), vecX.end()); // ソート
	std::sort(vecY.begin(), vecY.end()); // ソート

	double aTotalX = 0.0, aTotalY = 0.0;
	for(int i = 0; i < aPointCount; ++i)
	{
		aTotalX += vecX[i];
		aTotalY += vecY[i];
	}

	// 平均値の取得
	aTotalX /= (double)aPointCount;
	aTotalY /= (double)aPointCount;

	// メッシュコード合わせ(+100万×100万を超えないよう調整)
	ZMeshPos aMeshPos(aTotalX, aTotalY);
	AdjustMeshCode(&aMeshPos);

	// 自動計算フラグのチェック
	CComVariant vaAutoPos;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::AUTOPOSITION_F, &vaAutoPos))) return hr;
	vaAutoPos.ChangeType(VT_I4);

	// 自動計算フラグがOFFの場合は施設情報ポイントの座標を取得
	if (! vaAutoPos.lVal)
	{
		IGeometryPtr ipGeometry;
		if (FAILED(hr = ipFacilInfoPoint->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);
		if (FAILED(hr = GDBGetBaseMeshNormalizeCoord((IPointPtr)ipGeometry, cMesh, cX, cY)))
			return hr;
		return S_OK;
	}

	*cX = aMeshPos.eX;
	*cY = aMeshPos.eY;
	*cMesh = aMeshPos.eMesh;

	return S_OK;
}


