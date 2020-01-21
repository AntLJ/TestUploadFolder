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

// SINDY2HWYNODE.cpp: SINDY2HWYNODE クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SINDY2HWYNODE.h"
#include "SINDY2INCODE.h"
#include "INHwyCommonUtility.h"
#include <iomanip>
#include <boost/foreach.hpp>

using namespace in::schema::hwynode;

HighwayNode::HighwayNode() :
	eMesh(0),
	eNode(0),
	eRoadCode(0),
	eSequence(0),
	eFacilPos(0),
	eDirection(0),
	eFacilClass(0),
	eTollClass(0),
	eInOut(0),
	eNoEtcCarFlag(false),
	eNoGuideTollFlag(false),
	eEtcFacilCode(0),
	eEtcNoGuideTollFlag(false),
	eAppNode(0),
	eFlowNode(0)
{
	// フラグ列の初期化
	eTollFuncFlags.assign(false);
	eEtcTollFuncFlags.assign(false);
}


SINDY2HWYNODE::SINDY2HWYNODE(SINDY2INHWYINFO *cHwyInfo) :
SINDY2INHWY(cHwyInfo)
{
}

SINDY2HWYNODE::~SINDY2HWYNODE()
{
}

/**
 * @brief SiNDYの料金所機能コードTollFunc_cからI/Nの料金所機能コードのフラグ列への変換
 * @param[in]	cTollFunc	SiNDYの料金所機能コード
 * @param[out]	crTollFuncFlags	I/Nの料金所機能コードのフラグ列を格納するバッファ
 */
void SindyTollClassCode2INTollClassFlags(long cTollFunc, TollFuncFlags& crTollFuncFlags)
{
	switch (cTollFunc)
	{
		using namespace sindy::highway_node;
		using namespace toll_func_flags;
		
		case tollfunc_no:	// 機能無し
			break;
		case tollfunc_issue:	// 発券機能
			crTollFuncFlags[ticket] = true;
			break;
		case tollfunc_pay:	// 精算機能
			crTollFuncFlags[adjust] = true;
			break;
		case tollfunc_single:	// 単独料金所
			crTollFuncFlags[alone] = true;
			break;
		case tollfunc_singleissue:	// 単独料金所＋発券機能
			crTollFuncFlags[alone] = true;
			crTollFuncFlags[ticket] = true;
			break;
		case tollfunc_singlepay:	// 単独料金所＋精算機能
			crTollFuncFlags[alone] = true;
			crTollFuncFlags[adjust] = true;
			break;
		case tollfunc_check:	// Uターン用検札機能
			crTollFuncFlags[check_ticket] = true;
			break;
		case tollfunc_issue2:	// 単独料金無効発券機能
			crTollFuncFlags[alone_invalid_ticket] = true;
			break;
		case tollfunc_payissue:	// 精算機能＋発券機能
			crTollFuncFlags[adjust] = true;
			crTollFuncFlags[ticket] = true;
			break;
		default:
			ATLASSERT(0);
	}
}

/**
 * @brief 料金所機能コードのフラグ列の取得
 * @note	Highway_Nodeフィーチャから料金所機能コードを取得し、その値に応じてフラグ列をセット
 * @param[in]	cpHwyNode	対象ハイウェイノード
 * @param[out]	crTollFuncFlags	I/Nの料金所機能コードのフラグ列を格納するバッファ
 */
void GetTollFuncFlags(IFeature* cpHwyNode, TollFuncFlags& crTollFuncFlags)
{
	try
	{
		long aTollFunc = cpHwyNode->GetValue( cpHwyNode->Fields->_FindField(_bstr_t(sindy::highway_node::TOLLFUNC_C.c_str())) );
		SindyTollClassCode2INTollClassFlags(aTollFunc, crTollFuncFlags);
	}
	catch (...)
	{
		throw std::runtime_error("料金所機能コードの取得に失敗");
	}
}

/**
 * @brief 料金所機能コードのフラグ列の取得
 * @note	GetTollFuncFlags()のETC版
 * @param[in]	cpHwyNode	対象ハイウェイノード
 * @param[out]	crTollFuncFlags	I/Nの料金所機能コードのフラグ列を格納するバッファ
 */
void GetEtcTollFuncFlags(IFeature* cpHwyNode, TollFuncFlags& crTollFuncFlags)
{
	try
	{
		long aTollFunc = cpHwyNode->GetValue( cpHwyNode->Fields->_FindField(_bstr_t(sindy::highway_node::ETC_TOLLFUNC_C.c_str())) );
		SindyTollClassCode2INTollClassFlags(aTollFunc, crTollFuncFlags);
	}
	catch (...)
	{
		throw std::runtime_error("料金所機能コード(ETC)の取得に失敗");
	}
}

/**
 * @brief I/Nの料金所種別コードの取得
 * @note	与えられたハイウェイノードからSiNDYの料金所種別コードを取得し、対応するI/Nの料金所種別コードを返す
 * @param[in]	cpHwyNode	対象ハイウェイノード
 * @return I/Nの料金所種別コード
 */
long GetTollClass(IFeature* cpHwyNode)
{
	long aSindyTollClass = 0;
	try
	{
		aSindyTollClass = cpHwyNode->GetValue( cpHwyNode->Fields->_FindField(_bstr_t(sindy::highway_node::TOLLCLASS_C.c_str())) );
	}
	catch (...)
	{
		throw std::runtime_error("料金所種別コードの取得に失敗");
	}

	long aINTollClass = 0;
	switch (aSindyTollClass)
	{
		using namespace sindy::highway_node;
		case tollclass_other:
		case tollclass_inout:	// [Bug6309] hwynode_inout.txtにも料金所種別コードを出力し、その値は0（料金所以外）とする
			aINTollClass = in::schema::hwynode::tollclass::no_tollbooth;
			break;
		case tollclass_honsen:
			aINTollClass = in::schema::hwynode::tollclass::main_tollbooth;
			break;
		case tollclass_ramp:
			aINTollClass = in::schema::hwynode::tollclass::ramp_tollbooth;
			break;
		case tollclass_barrier:
			aINTollClass = in::schema::hwynode::tollclass::barrier;
			break;
		default:
			ATLASSERT(0);
	}

	return aINTollClass;
}

namespace {

/**
 * @brief 出入口コードの取得
 * @note	与えられたハイウェイノードからSiNDYの分岐・合流コードを取得し、対応するI/Nの出入口コードを返す
 * @param[in]	cpHwyNode	対象ハイウェイノード
 * @return	出入口コード
 */
long GetInOut(IFeature* cpHwyNode)
{
	long aSindyInOut = 0;
	try
	{
		aSindyInOut = cpHwyNode->GetValue( cpHwyNode->Fields->_FindField(_bstr_t(sindy::highway_node::INOUT_C.c_str())) );
	}
	catch (...)
	{
		throw std::runtime_error("出入口コードの取得に失敗");
	}

	return Sindy2INCode(in::schema::hwynode::inout::tInOut, aSindyInOut);
}

}	// anonymous namespace

/**
 * @brief I/NのETC施設コードの取得
 * @note	与えられたハイウェイノードからSiNDYのETC施設コードを取得し、対応するI/NのETC施設コードを返す
 * @param[in]	cpHwyNode	対象ハイウェイノード
 * @return I/NのETC施設コード
 */
long GetEtcFacil(IFeature* cpHwyNode)
{
	long aSindyEtcFacil = 0;
	try
	{
		aSindyEtcFacil = cpHwyNode->GetValue( cpHwyNode->Fields->_FindField(_bstr_t(sindy::highway_node::ETC_FACIL_C.c_str())) );
	}
	catch (...)
	{
		throw std::runtime_error("ETC施設コードの取得に失敗");
	}

	long aINEtcFacil = 0;
	switch (aSindyEtcFacil)
	{
		using namespace sindy::highway_node;
		case etc_facil_no_etc_facil:
			aINEtcFacil = in::schema::hwynode::etc_facil::no_etc_facil;
			break;
		case etc_facil_gate:
			aINEtcFacil = in::schema::hwynode::etc_facil::gate;
			break;
		case etc_facil_antenna:
			aINEtcFacil = in::schema::hwynode::etc_facil::antenna;
			break;
		default:
			ATLASSERT(0);
	}

	return aINEtcFacil;
}

/**
 * @brief SiNDYからフラグのフィールドの値を取得
 * @note	フラグはコード値の変換が必要ないので共通の関数にする
 * @param[in]	cpHwyNode	対象ハイウェイノード
 * @param[in]	cFieldName	フィールド名
 * @param[in]	cFieldAlias	フィールドエイリアス（エラーメッセージ用）
 * @return	取得したフラグの値
 */
bool GetFlag(IFeature* cpHwyNode, std::string cFieldName, std::string cFieldAlias)
{
	bool aFlag = true;
	try
	{
		aFlag = cpHwyNode->GetValue( cpHwyNode->Fields->_FindField(_bstr_t(cFieldName.c_str())) );
	}
	catch (...)
	{
		throw std::runtime_error(cFieldAlias + "の取得に失敗");
	}

	return aFlag;
}


HRESULT SINDY2HWYNODE::Convert(long cFormat)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HWYNODE::Convert";

#ifdef	_DEBUG
	debug = 0;
#endif

	for (GDBOBJITER aIter = mpHwyInfo->eHighwayNode.begin(); aIter != mpHwyInfo->eHighwayNode.end(); aIter++)
	{
#ifdef	_DEBUG
		std::cerr << "Creating hwynode.txt... : " << ++debug << "/" << mpHwyInfo->eHighwayNode.size() << "\r";
#endif
		IFeaturePtr ipHwyNode = aIter->second;
		HighwayNode aHwyNode;

		using namespace file_type;

		bool aTarget = true;
		if (FAILED(hr = isTarget(mpHwyInfo, ipHwyNode, &aTarget)))
		{
			PrintError(ipHwyNode, "ハイウェイモードフラグの取得に失敗");
			return hr;
		}
		aHwyNode.eFileType = (! aTarget) ? tollnode : hwynode;

		bool aGuideNode = true;
		if (FAILED(hr = isGuideNode(ipHwyNode, &aGuideNode)))
		{
			PrintError(ipHwyNode, "案内ノードフラグの取得に失敗");
			return hr;
		}
		if (aGuideNode) aHwyNode.eFileType = tollguide;

		// 2次メッシュコード
		if (FAILED(hr = GetMeshCode(ipHwyNode, &aHwyNode.eMesh)))
		{
			PrintError(ipHwyNode, "2次メッシュコードの取得に失敗");
			return hr;
		}

		// ノードID
		if (FAILED(hr = GetNodeID(mpHwyInfo, ipHwyNode, &aHwyNode.eNode)))
		{
			PrintError(ipHwyNode, "ノードIDの取得に失敗");
			return hr;
		}

		// 路線コード・路線内シーケンス番号
		if (FAILED(hr = mpHwyInfo->GetFacilCode(ipHwyNode, &aHwyNode.eRoadCode, &aHwyNode.eSequence)))
		{
			PrintError(ipHwyNode, "路線コード・路線内シーケンスの取得に失敗");
			return hr;
		}

		// 施設位置コード
		if (FAILED(hr = GetFacilPos(mpHwyInfo, ipHwyNode, &aHwyNode.eFacilPos)))
		{
			PrintError(ipHwyNode, "施設位置コードの取得に失敗");
			return hr;
		}
		/// 施設位置コードが出入口ノード用のものなら、データタイプを出入口ノードに変更
		// TODO: 「施設位置コードが出入口ノード用である」と「TollClass_Cが出入口ノードである」は同値なので、後者で判定すべき。
		switch(aHwyNode.eFacilPos)
		{
			using namespace facil_pos_code;
			case ent_ramp_origin:
			case ext_ramp_end:
			case smartic_ent_ramp_origin:
			case smartic_ext_ramp_end:
				aHwyNode.eFileType = hwynode_inout;
				break;
			default:
				break;
		}

		// 上下線コード
		if (FAILED(hr = GetUpDown(ipHwyNode, &aHwyNode.eDirection)))
		{
			PrintError(ipHwyNode, "上下線コードの取得に失敗");
			return hr;
		}

		// 施設種別コード
		aHwyNode.eFacilClass = GetFacilClass(ipHwyNode, mpHwyInfo);

		// 料金所種別コード
		aHwyNode.eTollClass = GetTollClass(ipHwyNode);

		// 出入口コード
		aHwyNode.eInOut = GetInOut(ipHwyNode);

		// 料金所機能コード（一般）
		GetTollFuncFlags(ipHwyNode, aHwyNode.eTollFuncFlags);

		// 非誘導料金所フラグ（一般）
		aHwyNode.eNoGuideTollFlag = GetFlag(ipHwyNode, sindy::highway_node::DUMMYTOLL_F, "非誘導料金所フラグ（一般）");

		// ETC距離別料金対応で追加されたフィールドの読み込み
		switch (cFormat)
		{
			using namespace file_format;
			case hwynode_smartic_added:
				break;
			case hwynode_etc_specific_toll:

				// 一般車両対応フラグ
				aHwyNode.eNoEtcCarFlag = GetFlag(ipHwyNode, sindy::highway_node::NOETC_CAR_F, "一般車両対応フラグ");

				// ETC施設コード
				aHwyNode.eEtcFacilCode = GetEtcFacil(ipHwyNode);

				// 料金所機能コード(ETC)
				GetEtcTollFuncFlags(ipHwyNode, aHwyNode.eEtcTollFuncFlags);

				// 非誘導料金所フラグ（ETC）
				aHwyNode.eEtcNoGuideTollFlag = GetFlag(ipHwyNode, sindy::highway_node::ETC_NOGUIDETOLL_F, "非誘導料金所フラグ（ETC）");

				break;
			default:
				ATLASSERT(0);
		}

		// 流入側ノード・流出側ノード
		if (FAILED(hr = GetInOutNode(ipHwyNode, &aHwyNode.eAppNode, &aHwyNode.eFlowNode)))
		{
			PrintError(ipHwyNode, "流入・流出ノードの取得に失敗");
			return hr;
		}

		// 施設名称
		if (FAILED(hr = GetName(mpHwyInfo, ipHwyNode, &aHwyNode.eName, &aHwyNode.eYomi)))
		{
			PrintError(ipHwyNode, "施設名称の取得に失敗");
			return hr;
		}

		mHighwayNodeArray.push_back(aHwyNode);
	}

#ifdef	_DEBUG
	std::cerr << std::endl;
#endif

	return S_OK;
}


/**
 * @brief tollguide.txtに出力するレコード列の作成
 * @note	長いのでSINDY2HWYNODE::Write()から分離
 * @return	作成したレコード列
 */
std::auto_ptr<TollGuideArray> SINDY2HWYNODE::makeTollGuideArray(void)
{
	std::auto_ptr<TollGuideArray> pTollGuideArray(new TollGuideArray);

	BOOST_FOREACH (const HighwayNode& aHighwayNode, mHighwayNodeArray)
	{
		using namespace file_type;
		if (aHighwayNode.eFileType != tollguide) continue;

		TollGuide aTollGuide;
		aTollGuide.eNode.eMesh = aHighwayNode.eMesh;
		aTollGuide.eNode.eNode = aHighwayNode.eNode;

		// [Bug6316] tollguideの料金所案内用ノードポイントに対応料金所機能コードが格納されたら、このフラグをtrueにする
		static bool aIsStraged = false;

		if (!aIsStraged)
		{
			// 案内料金所ハイウェイノードに対応する非誘導料金所ハイウェイノードを探して、料金所機能コードのフラグ列をコピーする
			long aRoadCode = aHighwayNode.eRoadCode;
			long aSequence = aHighwayNode.eSequence;
			long aFacilPos = aHighwayNode.eFacilPos;

			for (unsigned long aIndex = 0; aIndex < mHighwayNodeArray.size(); aIndex++) {
				if( mHighwayNodeArray[aIndex].eFileType != hwynode )			{ continue; }
				if( mHighwayNodeArray[aIndex].eRoadCode != aRoadCode )	{ continue; }
				if( mHighwayNodeArray[aIndex].eSequence != aSequence )	{ continue; }
				if( ! mHighwayNodeArray[aIndex].eNoGuideTollFlag )		{ continue; }	// 非誘導料金所フラグが1のハイウェイノードが対象
				if( mHighwayNodeArray[aIndex].eFacilPos != aFacilPos )	{ continue; }	// 施設位置コード
				
				aTollGuide.eTollFuncFlags = mHighwayNodeArray[aIndex].eTollFuncFlags;
			}
		}
		else {
			aTollGuide.eTollFuncFlags = aHighwayNode.eTollFuncFlags;
		}

		// TollGuideArrayに同一座標のレコードが既にあるかチェック
		TollGuideArray::iterator aIter = pTollGuideArray->find(aTollGuide.eNode);
		if (aIter == pTollGuideArray->end())
		{
			// ない場合は、新しいレコードを追加
			aTollGuide.eFacilCodeArray.insert(std::make_pair(aHighwayNode.eRoadCode,aHighwayNode.eSequence));
			pTollGuideArray->insert(std::make_pair(aTollGuide.eNode, aTollGuide));
		}
		else
		{
			// ある場合は、既存レコードに路線コードと路線内シーケンスの組を追加
			aIter->second.eFacilCodeArray.insert(std::make_pair(aHighwayNode.eRoadCode, aHighwayNode.eSequence));
		}
	}

	return pTollGuideArray;
}

/**
 * @brief hwynode.txt, hwynode_inout.txt, tollnode.txtに共通のフィールドの出力
 * @note	但し、料金所種別コードのみ、hwynode_inout.txtには出力しない
 * @note	[Bug6309]対応時にWriteHwyNode()から分離
 * @param[in]	fout	ファイル出力ストリーム
 * @param[in]	cFormat	出力ファイルのフォーマット
 * @param[in]	crHighwayNode	出力対象データ
 */
void WriteHwyNodeCommonFields(std::ofstream& fout, long cFileType, long cFormat, const HighwayNode& crHighwayNode)
{
	using namespace file_format;
	std::string fd = "\t";	// フィールドデリミタ

	// ２次メッシュコード
	fout << std::dec << crHighwayNode.eMesh;

	// ノードID
	fout << fd;
	fout << std::dec << crHighwayNode.eNode;

	// 路線コード
	fout << fd;
	fout << setw(6) << setfill('0') << std::dec << crHighwayNode.eRoadCode;

	// 路線内シーケンス番号
	fout << fd;
	fout << std::dec << crHighwayNode.eSequence;

	switch (cFormat)
	{
		case hwynode_smartic_added:
			// 施設位置コード
			fout << fd;
			fout << std::dec << crHighwayNode.eFacilPos;

			// 上下線コード
			fout << fd;
			fout << std::dec << crHighwayNode.eDirection;

			break;

		case hwynode_etc_specific_toll:
			// 上下線コード
			fout << fd;
			fout << std::dec << crHighwayNode.eDirection;

			// 施設種別コード
			fout << fd;
			fout << std::dec << crHighwayNode.eFacilClass;

			// 出入口コード
			fout << fd;
			fout << std::dec << crHighwayNode.eInOut;

			// 料金所種別コード
			// [Bug6309] hwynode_inout.txtにも料金所種別コードを出力するように変更
			fout << fd;
			fout << std::dec << crHighwayNode.eTollClass;
			break;

		default:
			ATLASSERT(0);
	}
}

/**
 * @brief 料金所機能コードのフラグ列（5桁）を出力
 * @param[in]	fout	ファイル出力ストリーム
 * @param[in]	crHighwayNode	出力対象データ
 */
void WriteTollFuncFlags(std::ofstream& fout, const HighwayNode& crHighwayNode)
{
	ATLASSERT(crHighwayNode.eTollFuncFlags.size() == 5);
	BOOST_FOREACH (bool aFlag, crHighwayNode.eTollFuncFlags)
		fout << aFlag ? 1 : 0;
}

/**
 * @brief 料金関連フィールドの出力
 * @note	hwynode.txtとtollnode.txtの共通部分
 * @note	[Bug6309]対応時にWriteHwyNode()から分離
 * @param[in]	fout	ファイル出力ストリーム
 * @param[in]	cFormat	出力ファイルのフォーマット
 * @param[in]	crHighwayNode	出力対象データ
 */
void WriteTollFields(std::ofstream& fout, long cFormat, const HighwayNode& crHighwayNode)
{
	using namespace file_format;
	std::string fd = "\t";	// フィールドデリミタ

	if (cFormat == hwynode_etc_specific_toll)
	{
		// 一般車両対応フラグ
		fout << fd;
		fout << crHighwayNode.eNoEtcCarFlag ? 1 : 0;
	}

	// 料金所機能コードのフラグ列（5桁）（「料金所種別」または「料金所機能コード（一般）」）
	fout << fd;
	WriteTollFuncFlags(fout, crHighwayNode);

	if (cFormat == hwynode_etc_specific_toll)
	{
		// 距離別料金対応フォーマットでは、料金所機能コード（一般）と非誘導料金所フラグ（一般）を別フィールドにする
		fout << fd;
	}

	// 非誘導料金所フラグ（一般）
	fout << crHighwayNode.eNoGuideTollFlag ? 1 : 0;

	if (cFormat == hwynode_etc_specific_toll)
	{
		// ETC施設コード
		fout << fd;
		fout << crHighwayNode.eEtcFacilCode;

		// 料金所機能コード(ETC)の上5桁
		fout << fd;
		ATLASSERT(crHighwayNode.eEtcTollFuncFlags.size() == 5);
		BOOST_FOREACH (bool aFlag, crHighwayNode.eEtcTollFuncFlags)
			fout << aFlag ? 1 : 0;

		// 料金所機能コード(ETC)の下2桁（キャッシュバック式発券/精算所）
		for (int i = 0; i < 2; ++i)
			fout << 0;		// 常に'0'を出力する

		// 非誘導料金所フラグ(ETC)
		fout << fd;
		fout << crHighwayNode.eEtcNoGuideTollFlag ? 1 : 0;
	}
}

/**
 * @brief tollnode.txt専用フィールドの出力
 * @note	[Bug6309]対応時にWriteHwyNode()から分離
 * @note	WriteHwyNodeCommonFields(), WriteTollFields()と違って、file_formatに依存しない
 * @param[in]	fout	ファイル出力ストリーム
 * @param[in]	crHighwayNode	出力対象データ
 */
void WriteTollNodeFields(std::ofstream& fout, const HighwayNode& crHighwayNode)
{
	std::string fd = "\t";	// フィールドデリミタ

	// 流入ノードID
	fout << fd;
	fout << std::dec << crHighwayNode.eAppNode;

	// 流出ノードID
	fout << fd;
	fout << std::dec << crHighwayNode.eFlowNode;

	// 施設名称
	fout << fd;
	if (crHighwayNode.eName.size())
		fout << SINDY2INCODE::SJIS2EUC(crHighwayNode.eName.c_str());

	// 施設名称読み
	fout << fd;
	if (crHighwayNode.eYomi.size())
		fout << SINDY2INCODE::SJIS2EUC(crHighwayNode.eYomi.c_str());
}

/**
 * @brief hwynode.txtへの出力
 * @param[in]	fout	ファイル出力ストリーム
 * @param[in]	cFormat	出力ファイルのフォーマット
 */
void SINDY2HWYNODE::WriteHwyNode(std::ofstream& fout, long cFormat)
{
	BOOST_FOREACH (const HighwayNode& aHighwayNode, mHighwayNodeArray)
	{
		if (aHighwayNode.eFileType != file_type::hwynode)
			continue;

		WriteHwyNodeCommonFields(fout, aHighwayNode.eFileType, cFormat, aHighwayNode);
		WriteTollFields(fout, cFormat, aHighwayNode);

		fout << std::endl;
	}
}

/**
 * @brief hwynode_inout.txtへの出力
 * @note	[Bug6309]対応時にWriteHwyNode()から分離
 * @param[in]	fout	ファイル出力ストリーム
 * @param[in]	cFormat	出力ファイルのフォーマット
 */
void SINDY2HWYNODE::WriteHwyNodeInOut(std::ofstream& fout, long cFormat)
{
	BOOST_FOREACH (const HighwayNode& aHighwayNode, mHighwayNodeArray)
	{
		if (aHighwayNode.eFileType != file_type::hwynode_inout)
			continue;

		WriteHwyNodeCommonFields(fout, aHighwayNode.eFileType, cFormat, aHighwayNode);

		// 距離別料金対応以前のhwynode_inout.txtでは、「料金所種別」を出力する
		if (cFormat == file_format::hwynode_smartic_added)
		{
			fout << "\t";
			WriteTollFuncFlags(fout, aHighwayNode);
		}

		fout << std::endl;
	}
}

/**
 * @brief tollnode.txtへの出力
 * @note	[Bug6309]対応時にWriteHwyNode()から分離
 * @param[in]	fout	ファイル出力ストリーム
 * @param[in]	cFormat	出力ファイルのフォーマット
 */
void SINDY2HWYNODE::WriteTollNode(std::ofstream& fout, long cFormat)
{
	BOOST_FOREACH (const HighwayNode& aHighwayNode, mHighwayNodeArray)
	{
		if (aHighwayNode.eFileType != file_type::tollnode)
			continue;

		WriteHwyNodeCommonFields(fout, aHighwayNode.eFileType, cFormat, aHighwayNode);
		WriteTollFields(fout, cFormat, aHighwayNode);
		WriteTollNodeFields(fout, aHighwayNode);

		fout << std::endl;
	}
}


/**
 * @brief tollguide.txtへの出力
 * @param[in]	fout	ファイル出力ストリーム
 * @param[in]	cFormat	出力ファイルのフォーマット
 */
void SINDY2HWYNODE::WriteTollGuide(std::ofstream& fout, long cFormat)
{
	// tollguide.txtに出力するためのレコード列を作成
	std::auto_ptr<TollGuideArray> pTollGuideArray = makeTollGuideArray();

	std::string fd = "\t";	// フィールドデリミタ

	for (TollGuideArray::iterator aIter = pTollGuideArray->begin(); aIter != pTollGuideArray->end(); aIter++)
	{
		TollGuide aTollGuide = aIter->second;

		// メッシュコード
		fout << std::dec << aTollGuide.eNode.eMesh;

		// ノードID
		fout << fd;
		fout << std::dec << aTollGuide.eNode.eNode;

		// 対応料金所機能コード（5桁）
		fout << fd;
		BOOST_FOREACH (bool aFlag, aTollGuide.eTollFuncFlags)
		{
			fout << aFlag ? 1 : 0;
		}

		// 対応料金所機能コードの6桁目（非誘導料金所フラグ。ETC距離別料金対応後のフォーマットでは出力しない）
		using namespace file_format;
		switch (cFormat)
		{
			case hwynode_smartic_added:
				fout << 1;					// 常に'1'を出力する
				break;
			case hwynode_etc_specific_toll:
				break;
			default:
				ATLASSERT(0);
		}

		// 対応ポイント数
		fout << fd;
		fout << std::dec << aTollGuide.eFacilCodeArray.size();

		// 対応施設情報の列
		for (FacilCodeArray::iterator aFacil = aTollGuide.eFacilCodeArray.begin(); aFacil != aTollGuide.eFacilCodeArray.end(); aFacil++)
		{
			fout << fd;
			fout << std::dec << aFacil->first;	// 路線コード

			fout << fd;
			fout << std::dec << aFacil->second;	// 路線内シーケンス
		}

		fout << std::endl;
	}
}

/**
 * @brief ファイルへの出力
 * @param[in]	cFileName	出力ファイル名
 * @param[in]	cFileType	出力ファイルの種類
 * @param[in]	cFormat	出力ファイルのフォーマット
 * @return	必ずS_OKを返す（voidにしてもよいが、Write()の呼び出し側も修正する必要があるので保留）
 */
HRESULT SINDY2HWYNODE::Write(std::string cFileName, long cFileType, long cFormat)
{
	const char *func = "SINDY2HWYNODE::Write";

	std::ofstream fout(cFileName.c_str(), ios::binary);
	if (! fout.is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	switch (cFileType)
	{
		using namespace file_type;
		case tollnode:
			WriteTollNode(fout, cFormat);
			break;
		case tollguide:
			WriteTollGuide(fout, cFormat);
			break;
		case hwynode:
			WriteHwyNode(fout, cFormat);
			break;
		case hwynode_inout:
			WriteHwyNodeInOut(fout, cFormat);
			break;
		default:
			ATLASSERT(0);
	}

	return S_OK;
}

HRESULT SINDY2HWYNODE::GetInOutNode(IFeaturePtr ipHwyNode, long *cAppNode, long *cFlowNode)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HWYNODE::GetInOutNode";

	*cAppNode = *cFlowNode = 0;

	CComVariant vaAppNode, vaFlowNode;
	if (FAILED(hr = GDBGetValue(ipHwyNode, sindy::highway_node::APPROACHNODE_ID, &vaAppNode))) return hr;
	if (FAILED(hr = GDBGetValue(ipHwyNode, sindy::highway_node::FLOWNODE_ID, &vaFlowNode))) return hr;
	vaAppNode.ChangeType(VT_I4); vaFlowNode.ChangeType(VT_I4);

	*cAppNode = vaAppNode.lVal;
	*cFlowNode = vaFlowNode.lVal;

	return S_OK;
}

HRESULT SINDY2HWYNODE::GetName(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, std::string *cName, std::string *cYomi)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HWYNODE::GetName";

	*cName = *cYomi = "";

	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipHwyNode, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);

	CComVariant vaName, vaYomi;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::NAME_KANJI, &vaName))) return hr;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::NAME_YOMI, &vaYomi))) return hr;

	if (vaName.vt != VT_NULL && vaName.vt != VT_EMPTY)
	{
		vaName.ChangeType(VT_BSTR);
		*cName = COLE2T(vaName.bstrVal);
		if (*cName == " " || *cName == "　") *cName = "";
	}

	if (vaYomi.vt != VT_NULL && vaName.vt != VT_EMPTY)
	{
		vaYomi.ChangeType(VT_BSTR);
		*cYomi = COLE2T(vaYomi.bstrVal);
		if (*cYomi == " " || *cYomi == "　") *cYomi = "";
	}

	return S_OK;
}

HRESULT SINDY2HWYNODE::GetUpDown(IFeaturePtr ipHwyNode, long *cUpDown)
{
	const char *func = "SINDY2HWYNODE::GetUpDown";

	*cUpDown = 0;

	long aSindyDirection = ipHwyNode->GetValue( ipHwyNode->Fields->_FindField(_bstr_t(sindy::highway_node::DIRECTION_C.c_str())) );
	*cUpDown = Sindy2INCode(in::schema::hwynode::direction::tDirection, aSindyDirection);

	return S_OK;
}

HRESULT SINDY2HWYNODE::GetFacilPos(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, long *cFacilPos)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HWYNODE::GetFacilPos";

	*cFacilPos = 0;

#ifdef	_DEBUG
	long aOID = ipHwyNode->GetOID();
#endif

	// 料金所種別
	CComVariant vaTollClass;
	if (FAILED(hr = GDBGetValue(ipHwyNode, sindy::highway_node::TOLLCLASS_C, &vaTollClass))) return hr;
	vaTollClass.ChangeType(VT_I4);

	// 出入口属性（分岐・合流コード）
	CComVariant vaInOut;
	if (FAILED(hr = GDBGetValue(ipHwyNode, sindy::highway_node::INOUT_C, &vaInOut))) return hr;
	vaInOut.ChangeType(VT_I4);

	using namespace facil_pos_code;
	/// 分岐合流ノードの場合
	if (vaTollClass.lVal == sindy::highway_node::tollclass_other)
	{
		IFeaturePtr ipFacilInfoPoint = NULL;
		if (FAILED(hr = cHwyInfo->GetFacilPoint(ipHwyNode, &ipFacilInfoPoint))) return hr;
		if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);

		CComVariant vaFacilClass;
		if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::FACILCLASS_C, &vaFacilClass))) return hr;
		vaFacilClass.ChangeType(VT_I4);

		switch (vaFacilClass.lVal)
		{
			case sindy::facil_info_point::facilclass_sa:
				switch (vaInOut.lVal)
				{
				case sindy::inout::inout_enter:	*cFacilPos = sa_ent; break;
				case sindy::inout::inout_exit:	*cFacilPos = sa_ext; break;
				default: return GDBERROR2(E_UNEXPECTED, func);
				}
				break;
			case sindy::facil_info_point::facilclass_pa:
				switch (vaInOut.lVal)
				{
				case sindy::inout::inout_enter:	*cFacilPos = pa_ent; break;
				case sindy::inout::inout_exit:	*cFacilPos = pa_ext; break;
				default: return GDBERROR2(E_UNEXPECTED, func);
				}
				break;
			case sindy::facil_info_point::facilclass_jct:
				switch (vaInOut.lVal)
				{
				case sindy::inout::inout_enter:	*cFacilPos = jct_ent; break;
				case sindy::inout::inout_exit:	*cFacilPos = jct_ext; break;
				default: return GDBERROR2(E_UNEXPECTED, func);
				}
				break;
			case sindy::facil_info_point::facilclass_ramp:
			case sindy::facil_info_point::facilclass_ic:
				switch (vaInOut.lVal)
				{
				case sindy::inout::inout_enter:	*cFacilPos = ic_ent; break;
				case sindy::inout::inout_exit:	*cFacilPos = ic_ext; break;
				default: return GDBERROR2(E_UNEXPECTED, func);
				}
				break;
			case sindy::facil_info_point::facilclass_smartic:
				switch (vaInOut.lVal)
				{
				case sindy::inout::inout_enter:	*cFacilPos = smartic_ent; break;
				case sindy::inout::inout_exit:	*cFacilPos = smartic_ext; break;
				default: return GDBERROR2(E_UNEXPECTED, func);
				}
				break;

			default: return GDBERROR2(E_UNEXPECTED, func);
		}
	}
	/// 出入口ノードの場合
	else if (vaTollClass.lVal == sindy::highway_node::tollclass_inout)
	{
		IFeaturePtr ipFacilInfoPoint;
		if (FAILED(hr = cHwyInfo->GetFacilPoint(ipHwyNode, &ipFacilInfoPoint))) return hr;
		if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);

		CComVariant vaFacilClass;
		if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::FACILCLASS_C, &vaFacilClass))) return hr;
		vaFacilClass.ChangeType(VT_I4);

		if (vaFacilClass.lVal == sindy::facil_info_point::facilclass_smartic)
		{
			switch (vaInOut.lVal)
			{
			case sindy::inout::inout_enter:				*cFacilPos = smartic_ent_ramp_origin; break;
			case sindy::inout::inout_exit:				*cFacilPos = smartic_ext_ramp_end; break;
			default: return GDBERROR2(E_UNEXPECTED, func);
			}
		}
		else
		{
			switch (vaInOut.lVal)
			{
			case sindy::inout::inout_enter:				*cFacilPos = ent_ramp_origin; break;
			case sindy::inout::inout_exit:				*cFacilPos = ext_ramp_end; break;
			default: return GDBERROR2(E_UNEXPECTED, func);
			}
		}
	}
	else
	{
		switch (vaTollClass.lVal)
		{
		case sindy::highway_node::tollclass_honsen:		*cFacilPos = line_tollgate; break;
		case sindy::highway_node::tollclass_ramp:
			switch (vaInOut.lVal)
			{
			case sindy::inout::inout_enter:				*cFacilPos = ramp_ent_tollgate; break;
			case sindy::inout::inout_exit:				*cFacilPos = ramp_ext_tollgate; break;
			default: return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		case sindy::highway_node::tollclass_barrier:	*cFacilPos = barrier; break;
		default: return GDBERROR2(E_UNEXPECTED, func);
		}
	}

	return S_OK;
}

HRESULT SINDY2HWYNODE::GetNodeID(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, long *cNodeID)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HWYNODE::GetNodeID";

	*cNodeID = 0;

	IFeaturePtr ipNode;
	if (FAILED(hr = cHwyInfo->GetHwyNode(ipHwyNode, &ipNode))) return hr;
	if (ipNode == NULL)
		return GDBERROR2(E_UNEXPECTED, func);

	if (FAILED(hr = ipNode->get_OID(cNodeID))) return GDBERROR(hr, func);

	return S_OK;
}

HRESULT SINDY2HWYNODE::GetMeshCode(IFeaturePtr ipHwyNode, long *cMesh)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HWYNODE::GetMeshCode";

	*cMesh = 0;

	IGeometryPtr ipGeometry;
	if (FAILED(hr = ipHwyNode->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);
	long aX = 0, aY = 0;
	return GDBGetBaseMeshNormalizeCoord((IPointPtr)ipGeometry, cMesh, &aX, &aY);
}

HRESULT SINDY2HWYNODE::isTarget(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, bool *cTarget)
{
	return cHwyInfo->isHwyMode(ipHwyNode, cTarget);
}

HRESULT SINDY2HWYNODE::isGuideNode(IFeaturePtr ipHwyNode, bool *cGuide)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HWYNODE::isGuideNode";

	*cGuide = false;

	CComVariant vaGuide;
	if (FAILED(hr = GDBGetValue(ipHwyNode, sindy::highway_node::GUIDE_F, &vaGuide))) return hr;
	vaGuide.ChangeType(VT_I4);

	if (vaGuide.lVal) *cGuide = true;

	return S_OK;
}
