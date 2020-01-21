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
 *	GDBWorkspace.h
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		êVãKçÏê¨
 */

#ifndef	___GDB_WORKSPACE_H___
#define	___GDB_WORKSPACE_H___

#include <string>

extern HRESULT GDBOpenWorkspace(const std::string &cServer, const std::string &cInstance, const std::string &cDatabase, const std::string &cUser, const std::string &cPassword, const std::string &cVersion, IWorkspace **ipWorkspace);
extern HRESULT GDBOpenFeatureClass(IWorkspacePtr ipWorkspace, const std::string &cAliasName, IFeatureClass **ipFeatureClass);
extern HRESULT GDBOpenTable(IWorkspacePtr ipWorkspace, const std::string &cAliasName, ITable **ipTable);
extern HRESULT GDBStartEditing(IWorkspacePtr ipWorkspace);
extern HRESULT GDBStopEditing(IWorkspacePtr ipWorkspace);
extern HRESULT GDBCreateNewVersion(IWorkspacePtr ipWorkspace, const std::string &cNewVersionName, IVersion **ipNewVersion);
extern HRESULT GDBCheckVersionExistence(IWorkspacePtr ipWorkspace, const std::string &cVersionName, bool *cExistence);
extern HRESULT GDBReconcile(IWorkspacePtr ipWorkspace, const std::string &cVersionName, bool *cConflict);
extern HRESULT GDBPost(IWorkspacePtr ipWorkspace, const std::string &cVersionName);
extern HRESULT GDBFillCache(IWorkspacePtr ipWorkspace, IEnvelopePtr ipEnvelope);
extern HRESULT GDBEmptyCache(IWorkspacePtr ipWorkspace);
extern HRESULT GDBGetVersionName(IWorkspacePtr ipWorkspace, std::string *cVersionName);
extern HRESULT GDBEmptyCache(IWorkspacePtr ipWorkspace);

#endif	//___GDB_WORKSPACE_H___
