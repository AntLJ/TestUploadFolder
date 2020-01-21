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

#ifndef __SINDY_OBJECTID_RANGE_H__
#define __SINDY_OBJECTID_RANGE_H__

#include <utility>
#include <algorithm>
#include <functional>
#include <vector>

namespace sindy {

#pragma warning(push)
#pragma warning(disable : 4290)

/**
 * @brief テーブルの最小・最大OBJECTIDを求めるクラス
 *
 * - first : 最小OBJECTID
 * - second : 最大OBJECTID
 */
class ObjectIdRange : public std::pair<long, long>
{
public:
	/**
	 * @brief 通知クラス
	 */
	class Notificator
	{
	public:
		/**
		 * @brief OBJECTID範囲通知。
		 */
		virtual void onRange(const ObjectIdRange&) {}
	};

// 構築/消滅
	template <typename TTable>
	ObjectIdRange(TTable const& rTable)
	{
		IDatasetPtr ipDataset(rTable);

		CComBSTR bstrName;
		ipDataset->get_Name(&bstrName);

		IWorkspacePtr ipWorkspace;
		ipDataset->get_Workspace(&ipWorkspace);
		setRange(IFeatureWorkspacePtr(ipWorkspace), bstrName);
	}

	ObjectIdRange()
	{
	}

	ObjectIdRange(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrTableName)
	{
		setRange(ipFeatureWorkspace, bstrTableName);
	}

	/**
	 * @brief 複数のテーブルの最小・最大OBJECTIDを求める
	 *
	 * @param first [in] テーブル一覧始点反復子。
	 * @param second [in] テーブル一覧終点反復子。
	 * @param pNotificator [in,optional] 通知クラス。
	 */
	template <typename InIt>
	ObjectIdRange(InIt _first, InIt _last, Notificator* pNotificator = 0)
	{
		setRange(_first, _last, std::less<long>(), std::greater<long>(), pNotificator);
	}

	/**
	 * @brief 複数のテーブルの最小・最大OBJECTIDを求める
	 *
	 * @param _first [in] テーブル一覧始点反復子。
	 * @param _last [in] テーブル一覧終点反復子。
	 * @param cMinFunc [in] 最小OBJECTIDを求めるための大小比較ファンクタ。std::less<long>など。
	 * @param cMaxFunc [in] 最大OBJECTIDを求めるための大小比較ファンクタ。std::greater<long>など。
	 * @param pNotificator [in,optional] 通知クラス。
	 */
	template <typename InIt, typename MinFunc, typename MaxFunc>
	ObjectIdRange(InIt _first, InIt _last, MinFunc cMinFunc, MaxFunc cMaxFunc, Notificator* pNotificator = 0)
	{
		setRange(_first, _last, cMinFunc, cMaxFunc, pNotificator);
	}

// 設定/取得
	long getMin() const { return first; }
	long getMax() const { return second; }

// 判断
	operator bool () const { return first && second; }
	bool operator!() const { return ! operator bool(); }

private:
// 設定/取得
	bool setRange(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrTableName)
	{
		if(! (ipFeatureWorkspace && bstrTableName))
			return false;

		// クエリ定義用意
		IQueryDefPtr ipQueryDef;
		ipFeatureWorkspace->CreateQueryDef(&ipQueryDef);

		// テーブル
		ipQueryDef->put_Tables(bstrTableName);

		// サブフィールド設定
		ipQueryDef->put_SubFields(CComBSTR(_T("MIN(OBJECTID),MAX(OBJECTID)")));

		// 評価
		_ICursorPtr ipCursor;
		if(ipQueryDef->Evaluate(&ipCursor) != S_OK)
			return false;

		// 結果取り出し
		_IRowPtr ipRow;
		if(ipCursor->NextRow(&ipRow) != S_OK)
			return false;

		_variant_t vMin, vMax;
		ipRow->get_Value(0, &vMin);
		ipRow->get_Value(1, &vMax);

		if(! (isValidValue(vMin) && isValidValue(vMax)))
			return false;

		first = vMin;
		second = vMax;

		return true;
	}

	template <typename InIt, typename MinFunc, typename MaxFunc>
	void setRange(InIt _first, InIt _last, MinFunc cMinFunc, MaxFunc cMaxFunc, Notificator* pNotificator = 0)
	{
		std::vector<long> aMinIdList; //< 最小OBJECTID一覧
		std::vector<long> aMaxIdList; //< 最大OBJECTID一覧
		for(; _first != _last; ++_first) {
			ObjectIdRange aRange(*_first);
			if(pNotificator) pNotificator->onRange(aRange);
			aMinIdList.push_back(aRange.first);
			aMaxIdList.push_back(aRange.second);
		}
		first = *std::min_element(aMinIdList.begin(), aMinIdList.end(), cMinFunc);
		second = *std::min_element(aMaxIdList.begin(), aMaxIdList.end(), cMaxFunc);
	}

	static bool isValidValue(VARIANT& var)
	{
		return var.vt != VT_NULL && var.vt != VT_EMPTY && var.vt != VT_ERROR;
	}
};
#pragma warning(pop)

};

#endif // __SINDY_OBJECTID_RANGE_H__
