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

// SiNDYMapTableFinder.cpp: SiNDYMapTableFinder �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SiNDYMapTableFinder.h"
#include "SiNDYTableName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

namespace {

void copy(IArray* ipArray, _ISet* ipSet)
{
	long nCount;
	ipArray->get_Count(&nCount);

	for(long i = 0; i < nCount; i++) {
		IUnknownPtr ipUnknown;
		ipArray->get_Element(i, &ipUnknown);
		ipSet->Add(ipUnknown);
	}
}

} // anonymous namespace

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

SiNDYMapTableFinder::SiNDYMapTableFinder()
{
}

SiNDYMapTableFinder::SiNDYMapTableFinder(IMapPtr ipMap)
{
	setMap(ipMap);
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

void SiNDYMapTableFinder::setMap(IMapPtr ipMap)
{
	if(ipMap == m_ipMap)
		return;

	m_ipMap = ipMap;
	m_cAutoComplete.clear();
}

void SiNDYMapTableFinder::setPair(CString strTableName, ILayer* ipLayer)
{
	strTableName.MakeUpper();
	if(ipLayer) {
		m_cAutoComplete[strTableName] = ipLayer;
	}
	else {
		m_cAutoComplete.erase(strTableName);
	}
}

ILayerPtr SiNDYMapTableFinder::getPairLayer(CString strTableName) const
{
	strTableName.MakeUpper();
	AutoComplete::const_iterator it = m_cAutoComplete.find(strTableName);
	return it != m_cAutoComplete.end() ? it->second : 0;
}

ITablePtr SiNDYMapTableFinder::getTable(LPCTSTR lpszTableName)
{
	if(m_ipMap == 0 || ! lpszTableName)
		return 0;

	SiNDYTableName cTableName(lpszTableName);

	if(cTableName.isInfTableName()) {
		IDatasetPtr ipDataset = findFeatureClass(cTableName.complementOwnerName(schema::road_link::kTableName));
		if(ipDataset == 0)
			return 0;

		IWorkspacePtr ipWorkspace;
		if(ipDataset->get_Workspace(&ipWorkspace) != S_OK || ipWorkspace == 0)
			return 0;

		IFeatureWorkspacePtr ipFeatureWorkspace = ipWorkspace;
		if(ipFeatureWorkspace == 0)
			return 0;

		ITablePtr ipTable;
		ipFeatureWorkspace->OpenTable(CComBSTR(cTableName.fullNameString()), &ipTable);

		return ipTable;
	}
	else
		return findFeatureClass(cTableName.fullNameString());
}

IFeatureClassPtr SiNDYMapTableFinder::findFeatureClass(CString strTableName) const
{
	if(m_ipMap == 0 || strTableName.IsEmpty())
		return 0;

	strTableName.MakeUpper();

	AutoComplete& rAutoComplete = const_cast<AutoComplete&>(m_cAutoComplete);
	AutoComplete::iterator it = rAutoComplete.find(strTableName);

	IArrayPtr ipLayerArray(CLSID_Array);
	queryLayerArray(m_ipMap, strTableName, ipLayerArray);

	long nCount;
	ipLayerArray->get_Count(&nCount);
	if(it != rAutoComplete.end()) {
		for(long i = 0; i < nCount; i++) {
			IUnknownPtr ipUnknown;
			ipLayerArray->get_Element(i, &ipUnknown);
			IFeatureLayerPtr ipFeatureLayer(ipUnknown);

			if(ipFeatureLayer == it->second) {
				IFeatureClassPtr ipFeatureClass;
				ipFeatureLayer->get_FeatureClass(&ipFeatureClass);
				return ipFeatureClass;
			}
		}
		rAutoComplete.erase(it);
	}

	if(nCount == 0)
		return 0;

	IUnknownPtr ipUnknown;
	ipLayerArray->get_Element(0, &ipUnknown);
	IFeatureLayerPtr ipFeatureLayer(ipUnknown);
	if(ipFeatureLayer == 0)
		return 0;

	rAutoComplete[strTableName] = ipFeatureLayer;

	IFeatureClassPtr ipFeatureClass;
	ipFeatureLayer->get_FeatureClass(&ipFeatureClass);
	return ipFeatureClass;
}

void SiNDYMapTableFinder::queryLayerSet(const CString& rLayerName, _ISet* ipLayerSet) const
{
	IArrayPtr ipArray(CLSID_Array);
	queryLayerArray(rLayerName, ipArray);
	copy(ipArray, ipLayerSet);
}

void SiNDYMapTableFinder::queryLayerSet(IMxDocument* ipMxDocument, const CString& rLayerName, _ISet* ipLayerSet)
{
	IArrayPtr ipArray(CLSID_Array);
	queryLayerArray(ipMxDocument, rLayerName, ipArray);
	copy(ipArray, ipLayerSet);
}

void SiNDYMapTableFinder::queryLayerSet(IMap* ipMap, const CString& rLayerName, _ISet* ipLayerSet)
{
	IArrayPtr ipArray(CLSID_Array);
	queryLayerArray(ipMap, rLayerName, ipArray);
	copy(ipArray, ipLayerSet);
}

void SiNDYMapTableFinder::queryLayerSet(ILayer* ipLayer, const CString& rLayerName, _ISet* ipLayerSet)
{
	IArrayPtr ipArray(CLSID_Array);
	queryLayerArray(ipLayer, rLayerName, ipArray);
	copy(ipArray, ipLayerSet);
}

void SiNDYMapTableFinder::queryLayerArray(const CString& rLayerName, IArray* ipLayerArray) const
{
	queryLayerArray(m_ipMap, rLayerName, ipLayerArray);
}

/**
 * @brief �w�肵���h�L�������g����A�w�肵�����̂Ɓu���C�����v�u�G�C���A�X���v�u�t���l�[���v�u�e�[�u�����v�̂����ꂩ����v���郌�C���𒊏o����B
 *
 * @note �I������Ă��郌�C��������ꍇ�͂��̒�����A���C�����I������Ă��Ȃ��ꍇ��TOC�ɂ�����̂̒��ŊY��������̑S���C���̒�����B
 *
 * @param ipMxDocument [in] �Ώۃh�L�������g�B
 * @param rLayerName [in] ���o�Ώۖ��́B
 * @param ipLayerArray [out] ���o���ꂽ���C���B
 */
void SiNDYMapTableFinder::queryLayerArray(IMxDocument* ipMxDocument, const CString& rLayerName, IArray* ipLayerArray)
{
	if(ipMxDocument == 0 || ipLayerArray == 0 || rLayerName.IsEmpty())
		return;

	IContentsViewPtr ipContentsView;
	if(ipMxDocument->get_ContentsView(0, &ipContentsView) == S_OK) {
		CComVariant va;
		if(ipContentsView->get_SelectedItem(&va) == S_OK && va.vt == VT_UNKNOWN) {
			ILayerPtr ipLayer;
			if(va.punkVal->QueryInterface(&ipLayer) == S_OK) {
				queryLayerArray(ipLayer, rLayerName, ipLayerArray);
				return;
			}
		}
	}

	IMapPtr ipMap;
	if(ipMxDocument->get_FocusMap(&ipMap) == S_OK && ipMap != 0) {
		queryLayerArray(ipMap, rLayerName, ipLayerArray);
	}
}

/**
 * @brief �w�肵���}�b�v����A�w�肵�����̂Ɓu���C�����v�u�G�C���A�X���v�u�t���l�[���v�u�e�[�u�����v�̂����ꂩ����v���郌�C���𒊏o����B
 *
 * @param ipMap [in] �Ώۃ}�b�v�B
 * @param rLayerName [in] ���o�Ώۖ��́B
 * @param ipLayerArray [out] ���o���ꂽ���C���B
 */
void SiNDYMapTableFinder::queryLayerArray(IMap* ipMap, const CString& rLayerName, IArray* ipLayerArray)
{
	if(ipMap == 0 || ipLayerArray == 0 || rLayerName.IsEmpty())
		return;

	long nLayerCount;
	ipMap->get_LayerCount(&nLayerCount);

	for(int i = 0; i < nLayerCount; i++) {
		ILayerPtr ipLayer;
		ipMap->get_Layer(i, &ipLayer);
		queryLayerArray(ipLayer, rLayerName, ipLayerArray);
	}

	return;
}

/**
 * @brief �w�肵�����C������A�w�肵�����̂Ɓu���C�����v�u�G�C���A�X���v�u�t���l�[���v�u�e�[�u�����v�̂����ꂩ����v���郌�C���𒊏o����B
 *
 * @param ipLayer [in] �Ώۃ��C���B�R���|�W�b�g���C���ł��B
 * @param rLayerName [in] ���o�Ώۖ��́B
 * @param ipLayerArray [out] ���o���ꂽ���C���B
 */
void SiNDYMapTableFinder::queryLayerArray(ILayer* ipLayer, const CString& rLayerName, IArray* ipLayerArray)
{
	if(ipLayer == 0 || ipLayerArray == 0 || rLayerName.IsEmpty())
		return;

	ICompositeLayerPtr ipComposite = ipLayer;
	if(ipComposite != 0) {
		// �R���|�W�b�g���C���̏ꍇ�́A�����郌�C���ɑ΂��čċA�������s��
		long nCount;
		ipComposite->get_Count(&nCount);

		for(int i = 0; i < nCount; i++) {
			ILayerPtr ipChildLayer;
			ipComposite->get_Layer(i, &ipChildLayer);
			queryLayerArray(ipChildLayer, rLayerName, ipLayerArray);
		}
	}
	else {
		// ���C�����Ƃ̊��S��v������
		CComBSTR bstrName;
		ipLayer->get_Name(&bstrName);
		if(rLayerName.CompareNoCase(CString(bstrName)) == 0) {
			ipLayerArray->Add(static_cast<IUnknown*>(ipLayer));
			return;
		}

		// �t�B�[�`���N���X�̃G�C���A�X���Ƃ̊��S��v������
		IFeatureClassPtr ipFeatureClass;
		IFeatureLayerPtr ipFeatureLayer = ipLayer;
		if(ipFeatureLayer == 0 || ipFeatureLayer->get_FeatureClass(&ipFeatureClass) != S_OK || ipFeatureClass == 0)
			return;

		ipFeatureClass->get_AliasName(&bstrName);
		if(rLayerName.CompareNoCase(CString(bstrName)) == 0) {
			ipLayerArray->Add(static_cast<IUnknown*>(ipLayer));
			return;
		}

		// �f�[�^�Z�b�g�̃t���l�[�����A�e�[�u�����Ƃ̊��S��v������
		IDatasetPtr ipDataset = ipFeatureClass;
		if(ipDataset == 0)
			return;

		ipDataset->get_Name(&bstrName);
		SiNDYTableName cTableName = CString(bstrName);
		if(rLayerName.CompareNoCase(cTableName.fullNameString()) == 0 || rLayerName.CompareNoCase(cTableName.tableNameString()) == 0) {
			ipLayerArray->Add(static_cast<IUnknown*>(ipLayer));
			return;
		}
	}
}
