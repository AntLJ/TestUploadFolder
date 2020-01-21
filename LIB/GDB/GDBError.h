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
 *	@file
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		êVãKçÏê¨
 */

#ifndef	___GDB_ERROR_H___
#define	___GDB_ERROR_H___

#include <iostream>
#include <string>

HRESULT GDBERROR(HRESULT retval, const char *message = 0, std::ostream &stream = std::cout);

std::string GDBGetErrorInfo();

const std::string	GDB_ERR_UNEXPECTED =	"Unexpected failure.";
const std::string	GDB_ERR_NOTIMPL =		"Not implemented.";
const std::string	GDB_ERR_OUTOFMEMORY =	"Failed to allocate necessary memory.";
const std::string	GDB_ERR_INVALIDARG =	"One or more arguments are invalid.";
const std::string	GDB_ERR_NOINTERFACE =	"No such interface is supported.";
const std::string	GDB_ERR_POINTER =		"Invalid pointer.";
const std::string	GDB_ERR_HANDLE =		"Invalid handle.";
const std::string	GDB_ERR_ABORT =			"Operation aborted.";
const std::string	GDB_ERR_FAIL =			"Unspecified failure.";
const std::string	GDB_ERR_ACCESSDENIED =	"General access denied.";

std::string GDBGetMessage(HRESULT hr);

HRESULT GDBERROR2(HRESULT retval, const char *message = 0, std::ostream &stream = std::cout);

#endif	//___GDB_ERROR_H___
