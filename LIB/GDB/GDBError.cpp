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

/**
 * @file
 * @note	cpp�t�@�C���ǉ��̌o�܂�GDB.cpp�Ɠ��l�B
 */

#include "GDB.h"

HRESULT GDBERROR(HRESULT retval, const char *message/* = 0 */, std::ostream &stream/* = std::cout */)
{
	IErrorInfoPtr ipErrorInfo = NULL;
	::GetErrorInfo(0, &ipErrorInfo);
	if (ipErrorInfo)
	{
		CComBSTR bstrError;
		ipErrorInfo->GetDescription(&bstrError);
		USES_CONVERSION;
		stream << "# ";
		stream << OLE2T(bstrError);
		if (message)
		{
			stream << "[";
			stream << message;
			stream << "]";
		}
		stream << std::endl;
	}
	return retval;
}

std::string GDBGetErrorInfo()
{
	std::string aMessage = "";

	IErrorInfoPtr ipErrorInfo = NULL;
	::GetErrorInfo(0, &ipErrorInfo);
	if (ipErrorInfo)
	{
		CComBSTR bstrError;
		ipErrorInfo->GetDescription(&bstrError);
		USES_CONVERSION;
		aMessage = OLE2T(bstrError);
	}
	return aMessage;
}

std::string GDBGetMessage(HRESULT hr)
{
	switch (hr)
	{
	case E_UNEXPECTED:		return GDB_ERR_UNEXPECTED;
	case E_NOTIMPL:			return GDB_ERR_NOTIMPL;
	case E_OUTOFMEMORY:		return GDB_ERR_OUTOFMEMORY;
	case E_INVALIDARG:		return GDB_ERR_INVALIDARG;
	case E_NOINTERFACE:		return GDB_ERR_NOINTERFACE;
	case E_POINTER:			return GDB_ERR_POINTER;
	case E_HANDLE:			return GDB_ERR_HANDLE;
	case E_ABORT:			return GDB_ERR_ABORT;
	case E_FAIL:			return GDB_ERR_FAIL;
	case E_ACCESSDENIED:	return GDB_ERR_ACCESSDENIED;
	default:				return "Error occurred.";
	}
}

HRESULT GDBERROR2(HRESULT retval, const char *message /* = 0 */, std::ostream &stream /* = std::cout */)
{
	std::string mess = GDBGetMessage(retval);
/*
	switch (retval)
	{
	case E_UNEXPECTED:		mess = GDB_ERR_UNEXPECTED; break;
	case E_NOTIMPL:			mess = GDB_ERR_NOTIMPL; break;
	case E_OUTOFMEMORY:		mess = GDB_ERR_OUTOFMEMORY; break;
	case E_INVALIDARG:		mess = GDB_ERR_INVALIDARG; break;
	case E_NOINTERFACE:		mess = GDB_ERR_NOINTERFACE; break;
	case E_POINTER:			mess = GDB_ERR_POINTER; break;
	case E_HANDLE:			mess = GDB_ERR_HANDLE; break;
	case E_ABORT:			mess = GDB_ERR_ABORT; break;
	case E_FAIL:			mess = GDB_ERR_FAIL; break;
	case E_ACCESSDENIED:	mess = GDB_ERR_ACCESSDENIED; break;
	default:				mess = "Error occurred."; break;
	}
*/
	stream << "# ";
	stream << mess;
	if (message) stream << "[" << message << "]";
	stream << std::endl;
	return retval;
}
