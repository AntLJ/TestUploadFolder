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

#ifndef __SINDY_DATASETNAME_H__
#define __SINDY_DATASETNAME_H__

#include "SiNDYImplicitCast.h"

namespace sindy {

//////////////////////////////////////////////////////////////////////
// �N���X��`
//////////////////////////////////////////////////////////////////////

/**
 * @brief �f�[�^�Z�b�g����CComBSTR�^�Ŏ擾����N���X
 */
class CDatasetNameBSTR_ : public CComBSTR
{
public:
// �\�z
	CDatasetNameBSTR_(IDataset* ipDataset);
	CDatasetNameBSTR_(IDatasetName* ipDatasetName);
	CDatasetNameBSTR_(const DatasetPtr& ipDataset);

// ����
	CComBSTR name() const;

// �ÓI����
	static CComBSTR getNameOf(BSTR bstrDatasetName);

private:
	template <typename TDatasetPtr>
	void setDatasetName(TDatasetPtr const& rDatasetPtr)
	{
		if(rDatasetPtr == 0)
			return;

		rDatasetPtr->get_Name(operator&());
	}
};

typedef InterfacePtr_<CDatasetNameBSTR_> CDatasetNameBSTR;

/**
 * @brief �f�[�^�Z�b�g����CString�^�Ŏ擾����N���X
 */
class CDatasetNameString : public CString
{
public:
// �\�z
	/// IDataset::get_Name() (with implicit cast)
	template <typename T>
	CDatasetNameString(T const& rT) : CString(CDatasetNameBSTR(rT))
	{
	}

	CDatasetNameString(const CDatasetNameBSTR& rName);

// ����
	CString owner() const;
	LPCTSTR name() const;

	bool isInf() const;

	CString complementOwner(LPCTSTR lpszDatasetName);

// �ÓI����
	static CString getOwnerOf(LPCTSTR lpszDatasetName);
	static LPCTSTR getNameOf(LPCTSTR lpszDatasetName);
	static bool isInf(LPCTSTR lpszDatasetName);
};

// CDatasetNameBSTR_�N���X����
//////////////////////////////////////////////////////////////////////

inline CDatasetNameBSTR_::CDatasetNameBSTR_(IDataset* ipDataset)
{
	setDatasetName(ipDataset);
}

/// IDatasetName::get_Name()
inline CDatasetNameBSTR_::CDatasetNameBSTR_(IDatasetName* ipDatasetName)
{
	setDatasetName(ipDatasetName);
}

/// IDataset::get_Name() (with implicit cast)
inline CDatasetNameBSTR_::CDatasetNameBSTR_(const DatasetPtr& ipDataset)
{
	setDatasetName(ipDataset);
}

inline CComBSTR CDatasetNameBSTR_::name() const
{
	return getNameOf(operator BSTR());
}

inline CComBSTR CDatasetNameBSTR_::getNameOf(BSTR bstrDatasetName)
{
	wchar_t* p = ::wcschr(bstrDatasetName, L'.');
	return p ? CComBSTR(p + 1) : bstrDatasetName;
}

// CDatasetNameString�N���X����
//////////////////////////////////////////////////////////////////////

inline CDatasetNameString::CDatasetNameString(const CDatasetNameBSTR& rName) :
CString(rName)
{
}

inline CString CDatasetNameString::owner() const
{
	return getOwnerOf(operator LPCTSTR());
}

inline LPCTSTR CDatasetNameString::name() const
{
	return getNameOf(operator LPCTSTR());
}

inline bool CDatasetNameString::isInf() const
{
	return isInf(operator LPCTSTR());
}

inline CString CDatasetNameString::complementOwner(LPCTSTR lpszDatasetName)
{
	CString strDatasetName = owner();
	if(! strDatasetName.IsEmpty()) { strDatasetName += _T('.'); }
	strDatasetName += getNameOf(lpszDatasetName);
	return strDatasetName;
}

inline CString CDatasetNameString::getOwnerOf(LPCTSTR lpszDatasetName)
{
	LPTSTR p = ::_tcschr(lpszDatasetName, _T('.'));
	return p ? CString(lpszDatasetName, p - lpszDatasetName) : CString();
}

inline LPCTSTR CDatasetNameString::getNameOf(LPCTSTR lpszDatasetName)
{
	LPTSTR p = ::_tcschr(lpszDatasetName, _T('.'));
	return p ? (p + 1) : lpszDatasetName;
}

inline bool CDatasetNameString::isInf(LPCTSTR lpszDatasetName)
{
	return ::_tcsnicmp(getNameOf(lpszDatasetName), _T("INF_"), 4) == 0;
}

} // namespace sindy

#endif // __SINDY_DATASETNAME_H__
