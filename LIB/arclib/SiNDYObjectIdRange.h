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
 * @brief �e�[�u���̍ŏ��E�ő�OBJECTID�����߂�N���X
 *
 * - first : �ŏ�OBJECTID
 * - second : �ő�OBJECTID
 */
class ObjectIdRange : public std::pair<long, long>
{
public:
	/**
	 * @brief �ʒm�N���X
	 */
	class Notificator
	{
	public:
		/**
		 * @brief OBJECTID�͈͒ʒm�B
		 */
		virtual void onRange(const ObjectIdRange&) {}
	};

// �\�z/����
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
	 * @brief �����̃e�[�u���̍ŏ��E�ő�OBJECTID�����߂�
	 *
	 * @param first [in] �e�[�u���ꗗ�n�_�����q�B
	 * @param second [in] �e�[�u���ꗗ�I�_�����q�B
	 * @param pNotificator [in,optional] �ʒm�N���X�B
	 */
	template <typename InIt>
	ObjectIdRange(InIt _first, InIt _last, Notificator* pNotificator = 0)
	{
		setRange(_first, _last, std::less<long>(), std::greater<long>(), pNotificator);
	}

	/**
	 * @brief �����̃e�[�u���̍ŏ��E�ő�OBJECTID�����߂�
	 *
	 * @param _first [in] �e�[�u���ꗗ�n�_�����q�B
	 * @param _last [in] �e�[�u���ꗗ�I�_�����q�B
	 * @param cMinFunc [in] �ŏ�OBJECTID�����߂邽�߂̑召��r�t�@���N�^�Bstd::less<long>�ȂǁB
	 * @param cMaxFunc [in] �ő�OBJECTID�����߂邽�߂̑召��r�t�@���N�^�Bstd::greater<long>�ȂǁB
	 * @param pNotificator [in,optional] �ʒm�N���X�B
	 */
	template <typename InIt, typename MinFunc, typename MaxFunc>
	ObjectIdRange(InIt _first, InIt _last, MinFunc cMinFunc, MaxFunc cMaxFunc, Notificator* pNotificator = 0)
	{
		setRange(_first, _last, cMinFunc, cMaxFunc, pNotificator);
	}

// �ݒ�/�擾
	long getMin() const { return first; }
	long getMax() const { return second; }

// ���f
	operator bool () const { return first && second; }
	bool operator!() const { return ! operator bool(); }

private:
// �ݒ�/�擾
	bool setRange(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrTableName)
	{
		if(! (ipFeatureWorkspace && bstrTableName))
			return false;

		// �N�G����`�p��
		IQueryDefPtr ipQueryDef;
		ipFeatureWorkspace->CreateQueryDef(&ipQueryDef);

		// �e�[�u��
		ipQueryDef->put_Tables(bstrTableName);

		// �T�u�t�B�[���h�ݒ�
		ipQueryDef->put_SubFields(CComBSTR(_T("MIN(OBJECTID),MAX(OBJECTID)")));

		// �]��
		_ICursorPtr ipCursor;
		if(ipQueryDef->Evaluate(&ipCursor) != S_OK)
			return false;

		// ���ʎ��o��
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
		std::vector<long> aMinIdList; //< �ŏ�OBJECTID�ꗗ
		std::vector<long> aMaxIdList; //< �ő�OBJECTID�ꗗ
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
