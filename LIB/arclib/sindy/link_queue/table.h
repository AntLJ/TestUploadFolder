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

#ifndef SINDY_LINK_QUEUE_TABLE_H_
#define SINDY_LINK_QUEUE_TABLE_H_

namespace sindy {

class link_queue_table
{
public:
///@name �R���X�g���N�^�ƃf�X�g���N�^
//@{
	link_queue_table();
	link_queue_table(ITable* ipInfTable, ITable* ipLqTable);
	link_queue_table(IFeatureClass* ipRoadLinkClass, LPCTSTR lpszLinkQueueName);
	link_queue_table(IFeatureWorkspace* ipFeatureWorkspace, LPCTSTR lpszLinkQueueName, LPCTSTR lpszOwnerName = 0);
//@}

///@name �ݒ�
//@{
	void reset();
	void assign(ITable* ipInfTable, ITable* ipLqTable);
	void assign(IFeatureClass* ipRoadLinkClass, LPCTSTR lpszLinkQueueName);
	void assign(IFeatureWorkspace* ipFeatureWorkspace, LPCTSTR lpszLinkQueueName, LPCTSTR lpszOwnerName = 0);

	void set_inf_table(ITable* ipTable) { m_ipInfTable = ipTable; }
	void set_lq_table(ITable* ipTable) { m_ipLqTable = ipTable; }
//@}

///@name �擾
//@{
	ITable* get_inf_table() const { return m_ipInfTable; }
	ITable* get_lq_table() const { return m_ipLqTable; }
//@}

///@name ���Z
//@{
	operator bool() const { return m_ipInfTable != 0 && m_ipLqTable != 0; }
	bool operator!() const { return ! operator bool(); }
//@}

private:
// �ϐ�
	ITablePtr m_ipInfTable;
	ITablePtr m_ipLqTable;
};

//////////////////////////////////////////////////////////////////////
// LinkQueueTable ����
//////////////////////////////////////////////////////////////////////

// �\�z/����
//////////////////////////////////////////////////////////////////////

inline link_queue_table::link_queue_table()
{
}

inline link_queue_table::link_queue_table(ITable* ipInfTable, ITable* ipLqTable) :
m_ipInfTable(ipInfTable),
m_ipLqTable(ipLqTable)
{
}

inline link_queue_table::link_queue_table(IFeatureClass* ipRoadLinkClass, LPCTSTR lpszLinkQueueName)
{
	assign(ipRoadLinkClass, lpszLinkQueueName);
}

inline link_queue_table::link_queue_table(IFeatureWorkspace* ipFeatureWorkspace, LPCTSTR lpszLinkQueueName, LPCTSTR lpszOwnerName)
{
	assign(ipFeatureWorkspace, lpszLinkQueueName, lpszOwnerName);
}

// �ݒ�
//////////////////////////////////////////////////////////////////////

inline void link_queue_table::reset()
{
	m_ipInfTable = 0;
	m_ipLqTable = 0;
}

inline void link_queue_table::assign(ITable* ipInfTable, ITable* ipLqTable)
{
	m_ipInfTable = ipInfTable;
	m_ipLqTable = ipLqTable;
}

inline void link_queue_table::assign(IFeatureClass* ipRoadLinkClass, LPCTSTR lpszLinkQueueName)
{
	reset();

	IDatasetPtr ipDataset(ipRoadLinkClass);

	if(ipDataset == 0 || ! lpszLinkQueueName)
		return;

	IWorkspacePtr ipWorkspace;
	ipDataset->get_Workspace(&ipWorkspace);

	CComBSTR bstrName;
	IDatasetPtr(ipRoadLinkClass)->get_Name(&bstrName);

	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);

	if(bstrName.Length() > 0) {
		USES_CONVERSION;
		LPTSTR lpszName = OLE2T(bstrName);
		LPTSTR p = ::_tcschr(lpszName, _T('.'));
		if(p) {
			*p = 0;
			assign(ipFeatureWorkspace, lpszLinkQueueName, lpszName);
			return;
		}
	}

	assign(ipFeatureWorkspace, lpszLinkQueueName);
}

inline void link_queue_table::assign(IFeatureWorkspace* ipFeatureWorkspace, LPCTSTR lpszLinkQueueName, LPCTSTR lpszOwnerName)
{
	reset();

	// �K�{����NULL�`�F�b�N�B
	if(! (ipFeatureWorkspace && lpszLinkQueueName))
		return;

	// �e�[�u�����̒��`�F�b�N�B
	if(((lpszOwnerName ? ::_tcslen(lpszOwnerName) : 0) + ::_tcslen(lpszLinkQueueName)) >= 58)
		return;

	// �e�[�u�����⊮�p�z��p�ӁB
	TCHAR szInfTableName[64] = {0};// = _T("INF_");
	TCHAR szLqTableName[64] = {0};// = _T("LQ_");

	// �I�[�i���𓪂ɕt���B
	if(lpszOwnerName && lpszOwnerName[0]) {
		::_tcscat(szInfTableName, lpszOwnerName);
		::_tcscat(szInfTableName, _T("."));
		::_tcscat(szLqTableName, lpszOwnerName);
		::_tcscat(szLqTableName, _T("."));
	}

	// �e�[�u�����t���B
	::_tcscat(szInfTableName, _T("INF_"));
	::_tcscat(szInfTableName, lpszLinkQueueName);
	::_tcscat(szLqTableName, _T("LQ_"));
	::_tcscat(szLqTableName, lpszLinkQueueName);

	// �e�[�u�����J���B
	ipFeatureWorkspace->OpenTable(CComBSTR(szInfTableName), &m_ipInfTable);
	ipFeatureWorkspace->OpenTable(CComBSTR(szLqTableName), &m_ipLqTable);
}

} // namespace sindy

#endif // SINDY_LINK_QUEUE_TABLE_H_
