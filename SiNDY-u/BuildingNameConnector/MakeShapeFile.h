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

#include <stdio.h>
#include <vector>
#include <set>
#include <stdio.h>
#include <io.h>
#include <string>
#include "stdafx.h"

using	namespace	std;

#ifndef	_MAKESHAPEFILE_H_
#define	_MAKESHAPEFILE_H_

/**
* @brief �W�I���g����Shape�ɏo�͂���
*/
class	MakeShapeFile
{
	private:
		IFeatureClassPtr	m_ipFClass;
		IFeatureCursorPtr	m_ipFeatureCursor;
		long				m_lORG_ID_Indx; // ���f�[�^��ID
		long				m_lBUILDING_ID_Indx; // ������ID
		long				m_lNAME_Indx; // ��������
	
		/**
		* @brief �t�@�C���̃t���p�X������A�V�F�C�v�t�@�C�����𒊏o����֐�
		* @return ����=true, ���s=false
		*/
		bool    GetFeatureClass	(
		            const std::string& sourceFile, ///< [in] �\�[�X�t�@�C����
		            std::string&       className   ///< [in] �p�X��
				);

		/**
		* @brief �t���p�X������A�t�@�C�����������āA�f�B���N�g������Ԃ��֐�
		* @return ����=true, ���s=false
		*/
		bool    GetFilePath	(
		            const std::string& sourceFile, ///< [in] �\�[�X�t�@�C����
		            std::string&       filePath    ///< [in] �p�X��
		        );
	public:

		/**
		* @brief �R���X�g���N�^
		*/
		MakeShapeFile()
		{
			m_lORG_ID_Indx      = -1; // ���f�[�^��ID
			m_lBUILDING_ID_Indx = -1; // ������ID
			m_lNAME_Indx        = -1; // ��������
		}
		/**
		* @brief �f�X�g���N�^
		*/
		~MakeShapeFile(){}

		/**
		* @brief �f�[�^���V�F�[�v�t�@�C���ɕϊ�
		* @return ����=true, ���s=false
		*/
		bool    Init (
		            std::string&     c_sOutFName, ///< [in] �o�̓V�F�C�v�t�@�C����
		            esriGeometryType c_eGeomType  ///< [in] �W�I���g���^�C�v
		        );

		/**
		* @brief �f�[�^���V�F�[�v�t�@�C���ɕϊ�
		* @return ����=true, ���s=false
		*/
		bool    OuttoShapeFile (
		            IGeometryPtr&    c_ipOutputGeo,///< [in] �V�F�C�v�t�@�C���ɏo�͂������W�I���g��
		            long             cOrg_ID,      ///< [in] ���f�[�^��ID
                    long             cBuilding_ID, ///< [in] ����ID
		            CString&         cName         ///< [in] ��������
		        );

};

//	�t�@�C���̃t���p�X������A�V�F�C�v�t�@�C�����𒊏o����֐�
inline bool	MakeShapeFile::
GetFeatureClass ( const std::string&  sourceFile, // �\�[�X�t�@�C����
                  std::string&        className ) // �p�X��
{
	std::string	tempName;
	std::string	temp;

	tempName	= sourceFile;

	char*	p1	= NULL;
	p1	= strrchr( (char*)tempName.c_str(), _T('\\'));
	if( p1 == NULL ) {
		className	= tempName;	// '\'�}�[�N���݂���Ȃ��̂Ńp�X�����܂܂Ȃ�
	}else {
		className	= p1+1;
	}
	return	( true );
}

//	�t���p�X������A�t�@�C�����������āA�f�B���N�g������Ԃ��֐�
inline bool	MakeShapeFile::
GetFilePath	( const std::string&  sourceFile, // �\�[�X�t�@�C����
              std::string&        filePath )  // �p�X��
{
	std::string	temp;
	temp	= sourceFile;

	char*		pathEnd;
	pathEnd		= strrchr((char*)temp.c_str(), _T('\\'));
	if( pathEnd == NULL ) {
		return	( false );
	}
	*pathEnd	= _T('\0');		// ��������؂�
	
	filePath	= temp;
	return	( true );
}

//	�f�[�^���V�F�[�v�t�@�C���ɕϊ�
inline bool	MakeShapeFile::
Init ( std::string&     c_sOutFName,  // �o�̓V�F�C�v�t�@�C����
       esriGeometryType c_eGeomType ) // �W�I���g���^�C�v
{

	HRESULT	hr;
	std::string	a_sPathName;	// �f�B���N�g����
	std::string	a_sShpName;		// �p�X�𔲂����V�F�C�v�t�@�C����

	if(!GetFeatureClass ( c_sOutFName, a_sShpName ) ){
		return	( false );
	}
	if(!GetFilePath	( c_sOutFName, a_sPathName ) ){
		return	( false );
	}
		
	//-------------------------------------------------------------------------------
	// Shape�t�@�C���̃I�[�v������	
	//-------------------------------------------------------------------------------
	IWorkspaceFactoryPtr	a_ipWSFactory(CLSID_ShapefileWorkspaceFactory);
	IPropertySetPtr			a_ipPropertySet(CLSID_PropertySet);
	IWorkspacePtr			a_ipSWSpace;

	if (a_ipWSFactory == 0)
		return	( false );

	// ���[�N�X�y�[�X���I�[�v��
	CComVariant		aVar_Dir((char*)a_sPathName.c_str());
	_bstr_t			a_bTStr;
	a_bTStr			= _T("DATABASE");
	if( FAILED (a_ipPropertySet->SetProperty(a_bTStr, aVar_Dir) )) {
		return	( false );
	}
	// ���[�N�X�y�[�X�̃I�[�v��
	if( FAILED (a_ipWSFactory->Open(a_ipPropertySet, 0, &a_ipSWSpace) )) {
		return	( false );
	}
	if(a_ipSWSpace == NULL) {
		return	( false );
	}

	// �t�B�[�`���N���X�̃I�[�v��
	IFeatureWorkspacePtr	ipFWorkspace(a_ipSWSpace);

	IFieldsPtr				ipFields(CLSID_Fields);
	IGeometryDefPtr			ipGDef(CLSID_GeometryDef);
	IGeometryDefEditPtr		ipGDefEdit(ipGDef);
	ISpatialReferencePtr	ipSpRef(CLSID_UnknownCoordinateSystem);

	ipGDefEdit->put_GeometryType(c_eGeomType);
	// ��ԎQ�Ƃ�ݒ肵�Ȃ���prj�t�@�C�����o�͂���Ȃ�
	if( FAILED (ipGDefEdit->putref_SpatialReference(ipSpRef)) ) {
		return	( false );
	}

	// �W�I���g��(Shape)�E�t�B�[���h�ǉ�
	//-------------------------------------------------------------
	IFieldsEditPtr	ipFieldsEdit(ipFields);
	IFieldPtr		ipField(CLSID_Field);

	IFieldEditPtr	ipFieldEdit1(ipField);
	if( SUCCEEDED (ipFieldEdit1->put_Name(T2BSTR(_T("Shape"))) ) &&
		SUCCEEDED (ipFieldEdit1->put_Type(esriFieldTypeGeometry)) &&
		SUCCEEDED (ipFieldEdit1->put_IsNullable(VARIANT_FALSE)) &&
		SUCCEEDED (ipFieldEdit1->putref_GeometryDef(ipGDefEdit) ) ) {
	
		if(ipFieldsEdit->AddField(ipFieldEdit1) != S_OK) {
			return	( false );
		}
	}else {
		return	( false );
	}

	const long cStrMaxSize = 254;

	_bstr_t bsORG_ID = _T("ORG_ID");
	_bstr_t bsBUILDING_ID = _T("BLD_ID");
	_bstr_t bsNAME   = _T("NAME");

	{
		IFieldPtr		ipField2(CLSID_Field);
		IFieldEditPtr	ipFieldEdit2(ipField2);

		// ���f�[�^��OBJECTID
		if( SUCCEEDED (ipFieldEdit2->put_Name(bsORG_ID)) &&
			SUCCEEDED (ipFieldEdit2->put_Type(esriFieldTypeInteger) ) &&
			SUCCEEDED (ipFieldEdit2->put_IsNullable(VARIANT_FALSE) ) ) {

			if(ipFieldsEdit->AddField(ipFieldEdit2) != S_OK) {
				return	( false );
			}
		}else {
			return	( false );
		}
	}
	{
		IFieldPtr		ipField2(CLSID_Field);
		IFieldEditPtr	ipFieldEdit2(ipField2);

		// �R�t����������ID
		if( SUCCEEDED (ipFieldEdit2->put_Name(bsBUILDING_ID) ) &&
			SUCCEEDED (ipFieldEdit2->put_Type(esriFieldTypeInteger) ) &&
			SUCCEEDED (ipFieldEdit2->put_IsNullable(VARIANT_FALSE) ) ) {

			if(ipFieldsEdit->AddField(ipFieldEdit2) != S_OK) {
				return	( false );
			}
		}else {
			return	( false );
		}
	}
	{
		IFieldPtr		ipField2(CLSID_Field);
		IFieldEditPtr	ipFieldEdit2(ipField2);

		// ���f�[�^�̖���
		if( SUCCEEDED (ipFieldEdit2->put_Name(bsNAME) ) &&
			SUCCEEDED (ipFieldEdit2->put_Type(esriFieldTypeString) ) &&
			SUCCEEDED (ipFieldEdit2->put_Precision(cStrMaxSize) ) &&
			SUCCEEDED (ipFieldEdit2->put_IsNullable(VARIANT_TRUE) ) ) {
			if(ipFieldsEdit->AddField(ipFieldEdit2) != S_OK) {
				return	( false );
			}
		}else {
			return	( false );
		}
	}


	//-------------------------------------------------------------

	_bstr_t		a_bSName, a_bGeoFName, a_sTmpStr;
	a_bSName	= (char*)a_sShpName.c_str();
	a_bGeoFName	= _T("Shape");
	a_sTmpStr	= _T("");

	if( FAILED ( ipFWorkspace->CreateFeatureClass(a_bSName, ipFields,
		NULL, NULL, esriFTSimple, a_bGeoFName, a_sTmpStr, &m_ipFClass) )){
		return	( false );
	}

	if( FAILED ( m_ipFClass->Insert(VARIANT_TRUE, &m_ipFeatureCursor) ) ) {
		return	( false );
	}

	// �����t�B�[���h�̃C���f�b�N�X���擾
	if( FAILED(m_ipFClass->FindField(bsORG_ID, &m_lORG_ID_Indx ) ) ) {
		return false;
	}
	if( FAILED(m_ipFClass->FindField(bsBUILDING_ID, &m_lBUILDING_ID_Indx ) ) ) {
		return false;
	}
	if( FAILED(m_ipFClass->FindField(bsNAME, &m_lNAME_Indx ) ) ) {
		return false;
	}


	return	( true );
}

//	�f�[�^���V�F�[�v�t�@�C���ɕϊ�
inline bool	MakeShapeFile::
OuttoShapeFile ( IGeometryPtr& c_ipOutputGeo, // �V�F�C�v�t�@�C���ɏo�͂������W�I���g��
				 long          cOrg_ID,       // ���f�[�^��ID
                 long          cBuilding_ID,  // ����ID
				 CString&      cName )        // ��������
{
	//-----------------------------------------------------------------------------------
	// �f�[�^�o�͏���
	//-----------------------------------------------------------------------------------
	// �����t�^
	IFeatureBufferPtr	ipFeatureBuffer;
	if( FAILED (m_ipFClass->CreateFeatureBuffer(&ipFeatureBuffer) ) ) {
		return	( false );
	}
	IClonePtr ipClone;
	if( FAILED (((IClonePtr)c_ipOutputGeo)->Clone (&ipClone) ) ) {
		return (false);
	}
	if( FAILED ( ipFeatureBuffer->putref_Shape(IGeometryPtr(ipClone)) ) ) {
		return	( false );
	}
	if(FAILED (ipFeatureBuffer->put_Value(m_lORG_ID_Indx, CComVariant(cOrg_ID) ) ) ) {
		return	( false );
	}
	if(FAILED (ipFeatureBuffer->put_Value(m_lBUILDING_ID_Indx, CComVariant(cBuilding_ID) ) ) ) {
		return	( false );
	}
	if(FAILED (ipFeatureBuffer->put_Value(m_lNAME_Indx, CComVariant(cName) ) ) ) {
		return	( false );
	}
	_variant_t va;
	if(FAILED ( m_ipFeatureCursor->InsertFeature(ipFeatureBuffer, &va) ) ) {
		return	( false );
	}
	if( FAILED ( m_ipFeatureCursor->Flush() ) ) {
		return	( false );
	}
	return	( true );
}

#endif
