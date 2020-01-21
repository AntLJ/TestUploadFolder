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

#include "SINDY2TOLLGATELANE.h"
#include "SINDY2HWYNODE.h"
#include "SindyAttrDef.h"
#include <boost/xpressive/xpressive.hpp>
#include <boost/foreach.hpp>

SINDY2TOLLGATELANE::SINDY2TOLLGATELANE(SINDY2INHWYINFO *cHwyInfo)
: SINDY2INHWY(cHwyInfo)
{
}

SINDY2TOLLGATELANE::~SINDY2TOLLGATELANE()
{
}

/**
 * @brief 与えられたハイウェイノードのTollClass_cが料金所に当たるものか否かを判定する
 * @note	料金所に当たるのは、TollClass_c = 本線上料金所、ランプ上料金所、バリアのいずれか
 * @param[in]	cpHwyNode	対象ハイウェイノード
 * @return	与えられたハイウェイノードのTollClass_cが料金所に当たるものか否か
 */
bool isTollGateHwyNode(IFeature* cpHwyNode)
{
	static long aTollClassFieldIndex = cpHwyNode->Fields->_FindField(sindy::highway_node::TOLLCLASS_C.c_str());
	long aTollClass = cpHwyNode->GetValue(aTollClassFieldIndex);
	bool aResult = false;
	switch (aTollClass)
	{
		using namespace sindy::highway_node;
		case tollclass_honsen:
		case tollclass_ramp:
		case tollclass_barrier:
			aResult = true;
			break;
		case tollclass_other:
		case tollclass_inout:
			aResult = false;
			break;
		default:
			ATLASSERT(0);
	}

	return aResult;
}

/**
 * @brief 与えられたハイウェイノードのDummyToll_fが1であるか否かを判定する
 * @param[in]	cpHwyNode	対象ハイウェイノード
 * @return	与えられたハイウェイノードのDummyToll_fが1であるか否か
 */
bool isDummyTollHwyNode(IFeature* cpHwyNode)
{
	static long aDummyTollFieldIndex = cpHwyNode->Fields->_FindField(sindy::highway_node::DUMMYTOLL_F.c_str());
	long aDummyTollFlag= cpHwyNode->GetValue(aDummyTollFieldIndex);
	return static_cast<bool>(aDummyTollFlag);
}

/**
 * @brief SiNDYから必要なデータを取得してコンテナに格納する
 */
HRESULT SINDY2TOLLGATELANE::Convert(void)
{
	HRESULT hr = S_OK;
	for (GDBOBJITER aIt = mpHwyInfo->eHighwayNode.begin(); aIt != mpHwyInfo->eHighwayNode.end(); ++aIt)
	{
		TollGateLaneInfo aTollGateLaneInfo;
		aTollGateLaneInfo.m_HwyNodeOID = aIt->first;
		IFeaturePtr ipHwyNode = aIt->second;

		// レーン状態の取得
		if (FAILED(hr = GetLaneStr(ipHwyNode, aTollGateLaneInfo.m_LaneStr)))
		{
			PrintError(ipHwyNode, "レーン状態の取得に失敗");
			return hr;
		}

		// レーン数の取得
		aTollGateLaneInfo.m_LaneNum = aTollGateLaneInfo.m_LaneStr.size();

		// レーン情報がないハイウェイノードならスキップ
		if (aTollGateLaneInfo.m_LaneNum == 0)
		{
			continue;
		}

		// TollClass_cが料金所（本線上料金所、ランプ上料金所、バリアのいずれか）でなければエラーメッセージを出力してスキップ
		if (! isTollGateHwyNode(ipHwyNode))
		{
			PrintError(ipHwyNode, "料金所ではないハイウェイノードに料金所レーン情報がある");
			return hr;
		}

		// DummyToll_f=1ならエラーメッセージを出力してスキップ
		if (isDummyTollHwyNode(ipHwyNode))
		{
			PrintError(ipHwyNode, "ダミー料金所のハイウェイノードに料金所レーン情報がある");
			return hr;
		}

		// 2次メッシュコードの取得
		if (FAILED(hr = GetMeshCode(ipHwyNode, aTollGateLaneInfo.m_MeshCode)))
		{
			PrintError(ipHwyNode, "2次メッシュコードの取得に失敗");
			return hr;
		}

		// 料金所ノードIDの取得
		if (FAILED(hr = GetTollGateNodeID(ipHwyNode, aTollGateLaneInfo.m_TollGateNodeID)))
		{
			PrintError(ipHwyNode, "料金所ノードIDの取得に失敗");
			return hr;
		}

		// 流入ノードIDの取得
		if (FAILED(hr = GetApproachNodeID(ipHwyNode, aTollGateLaneInfo.m_ApproachNodeID)))
		{
			PrintError(ipHwyNode, "流入ノードIDの取得に失敗");
			return hr;
		}

		// コンテナに格納
		bool aInsertResult = m_TollGateLaneInfoArray.insert(aTollGateLaneInfo).second;
#ifdef _DEBUG
		// デバッグ時のみ、出力対象外としたことを通知
		if (! aInsertResult)
		{
			std::cout << "\t" << "Highway_Node" << "\t" << aTollGateLaneInfo.m_HwyNodeOID <<  "\t\t\t\t\t" << "既に同一レコードが存在するので、出力対象外とする" << std::endl;
		}
#endif
	}

	return S_OK;	
}

/**
 * @brief 与えられた_variant_t型の値がVT_NULLなら、空文字列にして返す
 * @note	与えられた値は文字列を表すことが前提
 * @param[in]	cVariant	対象文字列
 * @return	変換後の文字列
 **/
_variant_t Null2Empty(_variant_t cVariant)
{
	if (cVariant.vt == VT_NULL)
	{
		cVariant = _bstr_t("");
	}

	return cVariant;
}

/**
 * @brief レーン状態の取得
 * @param[in]	cpHwyNode	対象ハイウェイノード
 * @param[out]	crLaneStr	取得したレーン情報を返すバッファ
 * @note	取得した文字列に不正な文字が含まれている場合はエラーメッセージを出力し、E_FAILを返す
 */
HRESULT SINDY2TOLLGATELANE::GetLaneStr(IFeaturePtr cpHwyNode, std::string& crLaneStr)
{
	try
	{
		static long aTollGateLaneFieldIndex = cpHwyNode->Fields->_FindField(sindy::highway_node::TOLLGATE_LANE.c_str());
		crLaneStr = COLE2T(Null2Empty(cpHwyNode->GetValue(aTollGateLaneFieldIndex)).bstrVal);
	}
	catch (...)
	{
		return E_FAIL;
	}

	// 不正な文字が含まれていないかチェック
	using namespace boost::xpressive;
	sregex aRegex = sregex::compile("[^0-9]");	// 半角数字以外は不正
	if (regex_search(crLaneStr, aRegex))
	{
		PrintError(cpHwyNode, "Highway_Node::TollGate_Laneに不正な文字が含まれている");
		return E_FAIL;
	}

	return S_OK;
}

/**
 * @brief 2次メッシュコードの取得
 * @note	SINDY2HWYNODE::GetMeshCode()と同じなので、そのまま使用
 * @param[in]	ipHwyNode	対象ハイウェイノード
 * @param[out]	crMesh	取得した2次メッシュコードを返すバッファ
 */
HRESULT SINDY2TOLLGATELANE::GetMeshCode(IFeaturePtr cpHwyNode, long& crMesh)
{
	return SINDY2HWYNODE::GetMeshCode(cpHwyNode, &crMesh);
}

/**
 * @brief 料金所ノードIDの取得
 * @param[in]	cpHwyNode	対象ハイウェイノード
 * @param[out]	crNodeID	取得したノードIDを返すバッファ
 */
HRESULT SINDY2TOLLGATELANE::GetTollGateNodeID(IFeaturePtr cpHwyNode, long& crNodeID)
{
	// 対応ノードの取得
	IFeaturePtr ipNode;
	HRESULT hr = S_OK;
	if (FAILED(hr = mpHwyInfo->GetHwyNode(cpHwyNode, &ipNode)))
		return hr;

	crNodeID = ipNode->OID;
	return S_OK;
}

/**
 * @brief 流入ノードIDの取得
 * @param[in]	cpHwyNode	対象ハイウェイノード
 * @param[out]	crNodeID	取得した流入ノードIDを返すバッファ
 */
HRESULT SINDY2TOLLGATELANE::GetApproachNodeID(IFeaturePtr cpHwyNode, long& crNodeID)
{
	try
	{
		static long aApproachNodeIDFieldIndex = cpHwyNode->Fields->_FindField(sindy::highway_node::APPROACHNODE_ID.c_str());
		crNodeID = cpHwyNode->GetValue(aApproachNodeIDFieldIndex);
	}
	catch (...)
	{
		return E_FAIL;
	}
	return S_OK;
}

/**
 * @brief コンテナに格納されているデータをファイルに出力する
 * @param[in]	cFilePath	出力ファイルのパス
 */
HRESULT SINDY2TOLLGATELANE::Write(std::string cFilePath)
{
	const char *func = "SINDY2TOLLGATELANE::Write";

	// ファイル出力ストリームの作成
	std::ofstream fout(cFilePath.c_str(), ios::binary);
	if (! fout.is_open())
	{
		return GDBERROR2(E_ACCESSDENIED, func);
	}

	std::string fd = "\t";	// フィールドデリミタ

	BOOST_FOREACH (const TollGateLaneInfo& aTollGateLaneInfo, m_TollGateLaneInfoArray)
	{
		// 2次メッシュコード
		fout << aTollGateLaneInfo.m_MeshCode << fd;

		// 料金所ノードID
		fout << aTollGateLaneInfo.m_TollGateNodeID << fd;

		// 流入ノードID
		fout << aTollGateLaneInfo.m_ApproachNodeID << fd;

		// レーン数
		fout << aTollGateLaneInfo.m_LaneNum << fd;

		// レーン状態
		fout << aTollGateLaneInfo.m_LaneStr << std::endl;
	}

	return S_OK;
}

