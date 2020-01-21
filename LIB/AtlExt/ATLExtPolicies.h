/*
	Copyright(c) DevInstinct inc. 2002
	http://www.devinstinct.com

	Written by Martin Lapierre

	All rights reserved. No warrantees extended. Use at your own risk.
	You may use this code in your product in compiled form without royalty.
	You may not distribute it in source code form as part of a class library
	or product without the express written consent of DevInstinct inc.
*/

#pragma once

#include "ATLExtBase.h"

namespace ATLExt
{
/////////////////////////////
// Persistence policy classes
/////////////////////////////

// Marshalling information structure
typedef struct
{
	DWORD dwDestContext;
	void __RPC_FAR *pvDestContext;
	DWORD mshlflags;
} _MarshalInfo;


// _Persist<>
// The basic persistence policy class.
template <class T>
class _Persist
{
public:
	static HRESULT Load(T *pT, LPSTREAM pStm) 
	{
		return pStm->Read(pT, sizeof(T), NULL);
	}
	static HRESULT Save(T *pT, LPSTREAM pStm, _MarshalInfo *pMshlInfo = NULL)	
	{
		return pStm->Write(pT, sizeof(T), NULL);
	}
	static HRESULT GetSizeMax(T *pT, ULARGE_INTEGER *pcbSize, _MarshalInfo *pMshlInfo = NULL)	
	{
		pcbSize->QuadPart = sizeof(T); 
		return S_OK;
	}
	static void init(T*) {}
	static void destroy(T*) {}
}; // class _Persist

// _PersistInterface<>
// The Interface persistence policy class.
template <class T = IUnknown, const IID* pIID = &IID_IUnknown>
class _PersistInterface
{
	typedef enum
	{
		NONE,
		PERSIST_STREAM_INIT,
		PERSIST_STREAM,
		STANDARD_MARSHALLING
	} _PersistMethod;

public:
	static HRESULT Load(T **pT, LPSTREAM pStm);
	static HRESULT Save(T **pT, LPSTREAM pStm, _MarshalInfo *pMshlInfo = NULL);
	static HRESULT GetSizeMax(T **pT, ULARGE_INTEGER *pcbSize, _MarshalInfo *pMshlInfo = NULL);
	static void init(T **pT) {*pT = NULL;}
	static void destroy(T **pT) {if (*pT) (*pT)->Release();}
}; // class _PersistInterface

// _Persist<BSTR>
// Specialized persistence policy class for the BSTR type.
template <>
class _Persist<BSTR>
{
public:
	static HRESULT Load(BSTR *pT, LPSTREAM pStm);
	static HRESULT Save(BSTR *pT, LPSTREAM pStm, _MarshalInfo *pMshlInfo = NULL);
	static HRESULT GetSizeMax(BSTR *pT, ULARGE_INTEGER *pcbSize, _MarshalInfo *pMshlInfo = NULL);
	static void init(BSTR *pT) {*pT=NULL;}
	static void destroy(BSTR *pT) {SysFreeString(*pT);}
}; // class _Persist<BSTR>

// _Persist<VARIANT>
// Specialized persistence policy class for the VARIANT type.
template <>
class _Persist<VARIANT>
{
public:
	static HRESULT Load(VARIANT *pT, LPSTREAM pStm);
	static HRESULT Save(VARIANT *pT, LPSTREAM pStm, _MarshalInfo *pMshlInfo = NULL);
	static HRESULT GetSizeMax(VARIANT *pT, ULARGE_INTEGER *pcbSize, _MarshalInfo *pMshlInfo = NULL);
	static void init(VARIANT *pT) {VariantInit(pT);}
	static void destroy(VARIANT *pT) {VariantClear(pT);}
}; // class _Persist<VARIANT>


// Implementations

// _PersistInterface<>

template <class T, const IID* pIID>
inline HRESULT _PersistInterface<T, pIID>::Load(T **pT, LPSTREAM pStm)
{
	ATLASSERT(pT != NULL && *pT == NULL);
	ATLASSERT(pStm != NULL);

	_PersistMethod ePersistMethod(NONE);

	// First read the persisting method.
	TestHR(pStm->Read(&ePersistMethod, sizeof(_PersistMethod), NULL))

	switch (ePersistMethod)
	{
		// No Interface.
		case NONE:
			*pT = NULL;
			break;

		// IPersistStreamInit
		case PERSIST_STREAM_INIT:
			{
				// Can't use the V-Table trick here.
				CLSID clsid;
				CComPtr<IPersistStreamInit> ccpIPSI;
				TestHR(ReadClassStm(pStm, &clsid))
				TestHR(ccpIPSI.CoCreateInstance(clsid))
				TestHR(ccpIPSI->Load(pStm))
				TestHR(ccpIPSI->QueryInterface(*pIID, (void**)pT))
			}
			break;

		// IPersistStream
		case PERSIST_STREAM:
			TestHR(OleLoadFromStream(pStm, *pIID, (void**)pT))
			break;

		// Standard marshalling
		case STANDARD_MARSHALLING:
			TestHR(CoUnmarshalInterface(pStm, *pIID, (void**)pT))
			break;

		// Unsupported method.
		default:
			return E_FAIL;
	}
	return S_OK;
}

template <class T, const IID* pIID>
inline HRESULT _PersistInterface<T, pIID>::Save(T **pT, LPSTREAM pStm, _MarshalInfo *pMshlInfo) 
{
	ATLASSERT(pT != NULL);
	ATLASSERT(pStm != NULL);

	_PersistMethod ePersistMethod(NONE);
	
	if ( *pT == NULL ) // Nothing to persist.
		return pStm->Write(&ePersistMethod, sizeof(_PersistMethod), NULL);

	// Persistence method priority:
	// 1) IPersistStreamInit
	// 2) IPersistStream
	// 3) Standard marshalling

	CComPtr<IPersistStream> ccpIPS;
	CComQIPtr<IPersistStreamInit> ccpIPSI(*pT);

	if ( ccpIPSI ) // Look for IPersistStreamInit.
	{
		ePersistMethod = PERSIST_STREAM_INIT;
		ccpIPS = reinterpret_cast<IPersistStream*>(ccpIPSI.p); // Same V-Table.
	}
	else // Look for IPersistStream.
	{
		CComQIPtr<IPersistStream> ccpIPS2(*pT);
		if ( ccpIPS2 )
		{
			ePersistMethod = PERSIST_STREAM;
			ccpIPS.Attach( ccpIPS2.Detach() );
		}
	}
	
	// Commit IPersistStream / IPersistStreamInit to stream.
	if ( ccpIPS )
	{
		TestHR(pStm->Write(&ePersistMethod, sizeof(_PersistMethod), NULL))
		return OleSaveToStream(ccpIPS, pStm);
	}

	// Standard marshalling
	if ( pMshlInfo )
	{
		ePersistMethod = STANDARD_MARSHALLING;
		TestHR(pStm->Write(&ePersistMethod, sizeof(_PersistMethod), NULL))
		return CoMarshalInterface(pStm, *pIID, *pT, pMshlInfo->dwDestContext, 
		                          pMshlInfo->pvDestContext, pMshlInfo->mshlflags);
	}

	// No operation possible.
	return STG_E_CANTSAVE;
}

template <class T, const IID* pIID>
inline HRESULT _PersistInterface<T, pIID>::GetSizeMax(T **pT, ULARGE_INTEGER *pcbSize, _MarshalInfo *pMshlInfo)
{
	ATLASSERT(pT != NULL);
	ATLASSERT(pcbSize != NULL);

	if ( *pT == NULL ) // Nothing to persist.
	{
		pcbSize->QuadPart = sizeof(_PersistMethod);
		return S_OK;
	}

	// Persistence method priority:
	// 1) IPersistStreamInit
	// 2) IPersistStream
	// 3) Standard marshalling

	CComPtr<IPersistStream> ccpIPS;
	CComQIPtr<IPersistStreamInit> ccpIPSI(*pT);

	if ( ccpIPSI ) // Look for IPersistStreamInit.
		ccpIPS = reinterpret_cast<IPersistStream*>(ccpIPSI.p); // Same V-Table
	else // Look for IPersistStream.
	{
		CComQIPtr<IPersistStream> ccpIPS2(*pT);
		if ( ccpIPS2 )
			ccpIPS.Attach( ccpIPS2.Detach() );
	}
	
	// Get the IPersistStream / IPersistStreamInit required size.
	if ( ccpIPS )
	{
		TestHR(ccpIPS->GetSizeMax(pcbSize))
		pcbSize->QuadPart += sizeof(CLSID) + sizeof(_PersistMethod);
		return S_OK;
	}

	// Standard marshalling
	if ( pMshlInfo )
	{
		ULONG ulSize(0);
		TestHR(CoGetMarshalSizeMax(&ulSize, *pIID, *pT, pMshlInfo->dwDestContext, 
		                           pMshlInfo->pvDestContext, pMshlInfo->mshlflags))
		pcbSize->QuadPart = sizeof(_PersistMethod) + ulSize;
		return S_OK;
	}

	// No operation possible.
	return STG_E_CANTSAVE;
}


// _Persist<BSTR>

inline HRESULT _Persist<BSTR>::Load(BSTR *pT, LPSTREAM pStm)
{
	// Based on CComBSTR::ReadFromStream.
	ATLASSERT(pT != NULL);
	ATLASSERT(pStm != NULL);

	CComBSTR bstrRead;
	TestHR(bstrRead.ReadFromStream(pStm))
	*pT = bstrRead.Detach();
	return S_OK;
}

inline HRESULT _Persist<BSTR>::Save(BSTR *pT, LPSTREAM pStm, _MarshalInfo *pMshlInfo)	
{
	// Based on CComBSTR::WriteToStream.
	ATLASSERT(pT != NULL);
	ATLASSERT(pStm != NULL);

	CComBSTR bstrWrite;
	bstrWrite.Attach(*pT);
	TestHR(bstrWrite.WriteToStream(pStm))
	bstrWrite.Detach();
	return S_OK;
}

inline HRESULT _Persist<BSTR>::GetSizeMax(BSTR *pT, ULARGE_INTEGER *pcbSize, _MarshalInfo *pMshlInfo)	
{
	// Based on CComBSTR::WriteToStream.
	ATLASSERT(pT != NULL);
	ATLASSERT(pcbSize != NULL);

	pcbSize->QuadPart = sizeof(ULONG);
	pcbSize->QuadPart += *pT ? SysStringByteLen(*pT) + sizeof(OLECHAR) : 0;
	return S_OK;
}


// _Persist<VARIANT>

inline HRESULT _Persist<VARIANT>::Load(VARIANT *pT, LPSTREAM pStm)
{
	// Based on CComVariant::ReadFromStream.
	ATLASSERT(pT != NULL);
	ATLASSERT(pStm != NULL);

	TestHR(VariantClear(pT))

	VARTYPE vtRead;
	TestHR(pStm->Read(&vtRead, sizeof(VARTYPE), NULL))

	int cbRead(0);
	pT->vt = vtRead;
	pT->punkVal = NULL; // Set internal value to 0.
	switch (vtRead)
	{
		case VT_UNKNOWN:
			return _PersistInterface<>::Load( &pT->punkVal, pStm );
		case VT_DISPATCH:
			return _PersistInterface<IDispatch, &IID_IDispatch>::Load( &pT->pdispVal, pStm );
		case VT_UI1:
		case VT_I1:
			cbRead = sizeof(BYTE);
			break;
		case VT_I2:
		case VT_UI2:
		case VT_BOOL:
			cbRead = sizeof(short);
			break;
		case VT_I4:
		case VT_UI4:
		case VT_R4:
		case VT_INT:
		case VT_UINT:
		case VT_ERROR:
			cbRead = sizeof(long);
			break;
		case VT_R8:
		case VT_CY:
		case VT_DATE:
			cbRead = sizeof(double);
			break;
		default:
			break;
	}

	if (cbRead != 0)
		TestHR(pStm->Read((void*) &pT->bVal, cbRead, NULL))
	else
	{
		TestHR(_Persist<BSTR>::Load(&pT->bstrVal, pStm))
		pT->vt = VT_BSTR;
		if (vtRead != VT_BSTR)
			return VariantChangeType(pT, pT, VARIANT_NOVALUEPROP, vtRead);
	}
	return S_OK;
}

inline HRESULT _Persist<VARIANT>::Save(VARIANT *pT, LPSTREAM pStm, _MarshalInfo *pMshlInfo)	
{
	// Based on CComVariant::WriteToStream.
	ATLASSERT(pT != NULL);
	ATLASSERT(pStm != NULL);

	TestHR(pStm->Write(&pT->vt, sizeof(VARTYPE), NULL))

	int cbWrite = 0;
	switch (pT->vt)
	{
		case VT_UNKNOWN:
			return _PersistInterface<>::Save(&pT->punkVal, pStm, pMshlInfo);
		case VT_DISPATCH:
			return _PersistInterface<IDispatch, &IID_IDispatch>::Save(&pT->pdispVal, pStm, pMshlInfo);
		case VT_UI1:
		case VT_I1:
			cbWrite = sizeof(BYTE);
			break;
		case VT_I2:
		case VT_UI2:
		case VT_BOOL:
			cbWrite = sizeof(short);
			break;
		case VT_I4:
		case VT_UI4:
		case VT_R4:
		case VT_INT:
		case VT_UINT:
		case VT_ERROR:
			cbWrite = sizeof(long);
			break;
		case VT_R8:
		case VT_CY:
		case VT_DATE:
			cbWrite = sizeof(double);
			break;
		default:
			break;
	}
	if (cbWrite != 0)
		return pStm->Write((void*) &pT->bVal, cbWrite, NULL);

	if (pT->vt != VT_BSTR)
	{
		CComVariant varBSTR;
		TestHR(VariantChangeType(&varBSTR, pT, VARIANT_NOVALUEPROP, VT_BSTR))
		return _Persist<BSTR>::Save(&varBSTR.bstrVal, pStm);
	}
	else
		return _Persist<BSTR>::Save(&pT->bstrVal, pStm);
}


inline HRESULT _Persist<VARIANT>::GetSizeMax(VARIANT *pT, ULARGE_INTEGER *pcbSize, _MarshalInfo *pMshlInfo)	
{
	// Based on CComVariant::WriteToStream.
	ATLASSERT(pT != NULL);
	ATLASSERT(pcbSize != NULL);

	switch (pT->vt)
	{
		case VT_UNKNOWN:
			TestHR(_PersistInterface<>::GetSizeMax(&pT->punkVal, pcbSize, pMshlInfo))
			break;
		case VT_DISPATCH:
			TestHR((_PersistInterface<IDispatch, &IID_IDispatch>::GetSizeMax(&pT->pdispVal, pcbSize, pMshlInfo)))
			break;
		case VT_UI1:
		case VT_I1:
			pcbSize->QuadPart = sizeof(BYTE);
			break;
		case VT_I2:
		case VT_UI2:
		case VT_BOOL:
			pcbSize->QuadPart = sizeof(short);
			break;
		case VT_I4:
		case VT_UI4:
		case VT_R4:
		case VT_INT:
		case VT_UINT:
		case VT_ERROR:
			pcbSize->QuadPart = sizeof(long);
			break;
		case VT_R8:
		case VT_CY:
		case VT_DATE:
			pcbSize->QuadPart = sizeof(double);
			break;
		default:
			{
				if (pT->vt != VT_BSTR)
				{
					CComVariant varBSTR;
					TestHR(VariantChangeType(&varBSTR, pT, VARIANT_NOVALUEPROP, VT_BSTR))
					return _Persist<BSTR>::GetSizeMax(&varBSTR.bstrVal, pcbSize);
				}
				else
					return _Persist<BSTR>::GetSizeMax(&pT->bstrVal, pcbSize);
			}
			break;
	}

	pcbSize->QuadPart += sizeof(VARTYPE);

	return S_OK;
}

} // namespace ATLExt
