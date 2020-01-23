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

#include "stdafx.h"
#include "Utility.h"

namespace util
{

void SchemaGuard::lock()
{
	if(!m_target) return;
	m_target->ChangeSchemaLock(esriExclusiveSchemaLock);
}

void SchemaGuard::releaseLock()
{
	if(!m_target) return;
	m_target->ChangeSchemaLock(esriSharedSchemaLock);
}


bool isDirectory( const uh::tstring& path )
{
	if( uh::isDirectory(path.c_str()) )
	{
		// FGDBはフォルダとみなさない
		if( !eq( path.substr( path.length() - 4 ), _T(".gdb") ) )
			return true;
	}
	return false;
}

bool isFile( const uh::tstring& path )
{
	if( uh::isFile( path.c_str() ) )
		return true;
	if( uh::isDirectory(path.c_str())
		&& eq( path.substr( path.length() - 4 ), _T(".gdb") ) )
	{
		return true;
	}
	return false;
}

bool isEqual(ISpatialReferencePtr src, ISpatialReferencePtr dst)
{
	VARIANT_BOOL ret = VARIANT_FALSE;
	IClonePtr(src)->IsEqual(IClonePtr(dst), &ret);
	return ret==VARIANT_TRUE;
}

IEnumDatasetPtr getDatasets( IWorkspacePtr workspace, esriDatasetType type )
{
	if( !workspace ) return nullptr;
	IEnumDatasetPtr retSet;
	workspace->get_Datasets( type, &retSet );
	return retSet;
}

IUIDPtr copyUID(IUIDPtr srcUID)
{
	if(!srcUID) return nullptr;

	CComVariant vaGUID;
	long lSubType = 0;
	srcUID->get_Value(&vaGUID);
	srcUID->get_SubType( &lSubType );

	IUIDPtr cpyUID(CLSID_UID);
	cpyUID->put_Value( vaGUID );
	cpyUID->put_SubType( lSubType );
	return cpyUID;
}

bool isNumber( const uh::tstring& str )
{
	return std::all_of(str.cbegin(), str.cend(), _istdigit );
}

uh::tstring eraseOwnerName( const uh::tstring& str)
{
	// 生成時にはユーザー名が不要なので削除
	uh::tstring tmp(str);
	int index = tmp.find(_T("."));
	if( index >= 0 )
		tmp.erase(0, index+1);
	return tmp;
}

std::map<uh::tstring,long> createFieldMap(ITablePtr layer, long& shapeIndex)
{
	std::map<uh::tstring,long> result;
	shapeIndex = -1L;

	if(!layer) return result;
	IFieldsPtr fields;
	layer->get_Fields(&fields);
	long fieldCount = 0;
	fields->get_FieldCount(&fieldCount);
	for(long index=0;index<fieldCount;++index)
	{
		IFieldPtr field;
		fields->get_Field(index, &field);

		esriFieldType type = esriFieldTypeInteger;
		field->get_Type(&type);
		switch(type)
		{
		case esriFieldTypeGeometry:
			shapeIndex = index;
			break;
		default:
			break;
		}
		CComBSTR name;
		field->get_Name(&name);
		CString nameTmp(name.m_str);
		nameTmp.MakeUpper();
		result.emplace(nameTmp.GetString(), index);
	}
	return result;
}

} // util
