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

#include "common.h"
#include "LogSys.h"
#include <boost/tokenizer.hpp>

long BldNamePtInfo::GetContentsCode() const
{
	long contentsCode = -1;
	switch( e_srcType )
	{
	case sindy::schema::sj::buildingname_src_point::source_type::kAnnoPOI:		// ���LPOI�i����J�e�S���j
	case sindy::schema::sj::buildingname_src_point::source_type::kCompAnnoPOI:	// ���LPOI�i�⊮�J�e�S���j
		{
			std::vector< long > tokens;
			if( TokenizeSrcID( tokens ) ) {
				contentsCode = tokens[0] * 100000 + tokens[1];
			} else {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�o�T����ID����R���e���c�R�[�h���擾�ł��܂��� : ") + e_srcID );
			}
		}
	}
	return contentsCode;
}

long BldNamePtInfo::GetContentsSeq() const
{
	long contentsSeq = -1;
	switch( e_srcType )
	{
	case sindy::schema::sj::buildingname_src_point::source_type::kAnnoPOI:		// ���LPOI�i����J�e�S���j
	case sindy::schema::sj::buildingname_src_point::source_type::kCompAnnoPOI:	// ���LPOI�i�⊮�J�e�S���j
		{
			std::vector< long > tokens;
			if( TokenizeSrcID( tokens ) ) {
				contentsSeq = tokens[2];
			} else {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�o�T����ID����R���e���c���V�[�P���X���擾�ł��܂��� : ") + e_srcID );
			}
		}
	}
	return contentsSeq;
}

bool BldNamePtInfo::TokenizeSrcID( std::vector< long > &tokens ) const
{
	// �o�T����ID�����p�����Ɣ��p�n�C�t���݂̂ō\������邩�`�F�b�N
	if( 0 != e_srcID.Compare( e_srcID.SpanIncluding( _T("0123456789-") ) ) )
		return false;

	// ���p�n�C�t������ؕ����Ƃ��ăg�[�N���ɕ���
	std::string strSrcID = CT2A( e_srcID );
	boost::tokenizer< boost::char_separator< char > > strTokens( strSrcID, boost::char_separator< char >( "-" ) );
	for( const auto &itr : strTokens ) {
		tokens.push_back( atol( itr.c_str() ) );
	}

	// �g�[�N���̐���3�ł��邱�Ƃ��`�F�b�N
	if( 3 != tokens.size() )
		return false;

	return true;
}

namespace common_proc {
	bool GetValue(_IRowPtr ipRow, LPCTSTR talbleName, LPCTSTR fieldName, const std::map<CString, long>& indeces, CComVariant* val)
	{
		CString errMsg;
		CString temp = fieldName;
		temp.MakeLower();
		std::map<CString, long>::const_iterator fieldItr = indeces.find(temp);
		if (fieldItr == indeces.end()) {
			errMsg.Format(_T("%s�t�B�[���h�̃C���f�b�N�X�擾�Ɏ��s���܂���"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		if (FAILED(ipRow->get_Value(fieldItr->second, val))) {
			errMsg.Format(_T("%s�t�B�[���h�̒l�擾�Ɏ��s���܂���"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		return true;
	}

	bool SetValue(IRowBufferPtr ipBuffer, LPCTSTR tableName, LPCTSTR fieldName, const std::map<CString, long>& indeces, const CComVariant& val)
	{
		CString errMsg;
		CString temp = fieldName;
		temp.MakeLower();
		std::map<CString, long>::const_iterator fieldItr = indeces.find(temp);
		if (fieldItr == indeces.end()) {
			errMsg.Format(_T("%s�t�B�[���h�̃C���f�b�N�X�擾�Ɏ��s���܂���(�����t�^��)"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, tableName);
			return false;
		}
		if (FAILED(ipBuffer->put_Value(fieldItr->second, val))) {
			errMsg.Format(_T("%s�t�B�[���h�̒l�t�^�Ɏ��s���܂���"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, tableName);
			return false;
		}	
		return true;
	}

	bool SelectHeadNameFeature(const IDFeatureMap& tgtFeatures, LPCTSTR tableName, LPCTSTR fieldName, const std::map<CString, long>& indeces, long* headID, IFeaturePtr& ipHeadFeature)
	{
		// �Ώۃt�B�[�`�������O�̏ꍇ �� �G���[
		if (0 == tgtFeatures.size()) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�����Ώۃ|�C���g�I��Ώۃt�B�[�`������0���ł���"), tableName);
			return false;
		}

		// �Ώۃt�B�[�`�������P�̏ꍇ
		if (1 == tgtFeatures.size()) {
			*headID = tgtFeatures.begin()->first;
			ipHeadFeature = tgtFeatures.begin()->second.m_T;
			return true;
		}

		// �Ώۃt�B�[�`�����������̏ꍇ
		long minLength = LONG_MAX;
		for (const auto &feature : tgtFeatures) {
			IFeaturePtr ipFeature = feature.second.m_T;
			//�@���̂̒����擾
			CComVariant val;
			if (! ::common_proc::GetValue((_IRowPtr)ipFeature, tableName, fieldName, indeces, &val)) 
				return false;
			long length = _tcslen((CString)val.bstrVal);
			// �����񂪒Z���ꍇ�A�X�V�B
			// �����񒷂���v�����ꍇ�́AOBJECTID���������ق����D�悳���BtgtFeatures��OBJECTID���Ȃ̂ŁA�X�V���Ȃꂯ���OBJECTID���������ق����̗p�����B
			if (length < minLength) {
				minLength = length;
				*headID = feature.first;
				ipHeadFeature = ipFeature;
			}
		}
		return true;
	}

	bool SearchByWhereClause(IFeatureClassPtr ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, const CString& whereClause, bool bRecycling)
	{
		CString errMsg;

		// �t�B�[�`���N���X���̎擾�i���O�o�͗p�j
		CComBSTR strName;
		((IDatasetPtr)ipFeatureClass)->get_Name(&strName);

		// �t�B���^�쐬
		IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
		if (FAILED(ipQueryFilter->put_WhereClause(CComBSTR(whereClause)))) {
			errMsg.Format(_T("�t�B���^�̍쐬�Ɏ��s���܂��� : %s"), whereClause);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, (CString)strName);
			return false;
		}

		// ����
		if (FAILED(ipFeatureClass->Search(ipQueryFilter, bRecycling ? VARIANT_TRUE : VARIANT_FALSE, &ipFeatureCursor))) {
			errMsg.Format(_T("�J�[�\���̎擾�Ɏ��s���܂��� : %s"), whereClause);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, (CString)strName);
			return false;
		}

		return true;
	}

	bool SearchByWhereClause(ITablePtr ipTable, _ICursorPtr& ipCursor, const CString& whereClause, bool bRecycling)
	{
		CString errMsg;

		// �e�[�u�����̎擾�i���O�o�͗p�j
		CComBSTR strName;
		((IDatasetPtr)ipTable)->get_Name(&strName);

		// �t�B���^�쐬
		IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
		if (FAILED(ipQueryFilter->put_WhereClause(CComBSTR(whereClause)))) {
			errMsg.Format(_T("�t�B���^�̍쐬�Ɏ��s���܂��� : %s"), whereClause);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, (CString)strName);
			return false;
		}

		// ����
		if (FAILED(ipTable->Search(ipQueryFilter, bRecycling ? VARIANT_TRUE : VARIANT_FALSE, &ipCursor))) {
			errMsg.Format(_T("�J�[�\���̎擾�Ɏ��s���܂��� : %s"), whereClause);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, (CString)strName);
			return false;
		}

		return true;
	}

	bool SearchByGeometry(IFeatureClassPtr ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, const IGeometryPtr& ipGeometry, esriSpatialRelEnum spRel, bool bRecycling)
	{
		// �t�B�[�`���N���X���̎擾�i���O�o�͗p�j
		CComBSTR strName;
		((IDatasetPtr)ipFeatureClass)->get_Name(&strName);

		// �t�B���^�쐬
		ISpatialFilterPtr ipSpFilter(CLSID_SpatialFilter);
		if (FAILED(ipSpFilter->putref_Geometry(ipGeometry)) || FAILED(ipSpFilter->put_SpatialRel(spRel))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B���^�̍쐬�Ɏ��s���܂���"), (CString)strName);
			return false;
		}
		// �J�[�\���擾
		if (FAILED(ipFeatureClass->Search(ipSpFilter, bRecycling ? VARIANT_TRUE : VARIANT_FALSE, &ipFeatureCursor))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�J�[�\���̎擾�Ɏ��s���܂���"), (CString)strName);
			return false;
		}

		return true;
	}

	bool GetCodedValues(const ITablePtr& ipTable, LPCTSTR talbleName, LPCTSTR fieldName, const std::map<CString, long>& indeces, std::set<long>& codedValues)
	{
		CString errMsg;
		CString temp = fieldName;
		temp.MakeLower();
		std::map<CString, long>::const_iterator fieldItr = indeces.find(temp);
		if (fieldItr == indeces.end()) {
			errMsg.Format(_T("%s�t�B�[���h�̃C���f�b�N�X�擾�Ɏ��s���܂���"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		IFieldsPtr ipFields;
		if (FAILED(ipTable->get_Fields(&ipFields))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�t�B�[���h��̎擾�Ɏ��s���܂���"), talbleName);
			return false;
		}
		IFieldPtr ipField;
		if (FAILED(ipFields->get_Field(fieldItr->second, &ipField))) {
			errMsg.Format(_T("%s�t�B�[���h�̎擾�Ɏ��s���܂���"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		IDomainPtr ipDomain;
		if (FAILED(ipField->get_Domain(&ipDomain))) {
			errMsg.Format(_T("%s�t�B�[���h�̃h���C���擾�Ɏ��s���܂���"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		esriDomainType domainType;
		if (FAILED(ipDomain->get_Type(&domainType))) {
			errMsg.Format(_T("%s�t�B�[���h�̃h���C���^�C�v�擾�Ɏ��s���܂���"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		if (esriDTCodedValue != domainType) {
			errMsg.Format(_T("%s�t�B�[���h�̃h���C���^�C�v���R�[�h�l�h���C���ł͂���܂���"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		ICodedValueDomainPtr ipCodedValueDomain(ipDomain);
		long codeCount = 0;
		if (FAILED(ipCodedValueDomain->get_CodeCount(&codeCount))) {
			errMsg.Format(_T("%s�t�B�[���h�̃R�[�h�l���擾�Ɏ��s���܂���"), fieldName);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
			return false;
		}
		for(long i = 0; i < codeCount; ++i) {
			CComVariant varValue;
			if (FAILED(ipCodedValueDomain->get_Value(i, &varValue))) {
				errMsg.Format(_T("%s�t�B�[���h�̃R�[�h�l�擾�Ɏ��s���܂���"), fieldName);
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, talbleName);
				return false;
			}
			codedValues.insert(varValue.lVal);
		}
		return true;
	}
}
