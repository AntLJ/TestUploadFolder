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

// SINDY2HIGHWAY.cpp: SINDY2HIGHWAY �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "SINDY2HIGHWAY.h"
#include "SINDY2INCODE.h"
#include "INHwySchema.h"
#include "INHwyCommonUtility.h"
#include <boost/foreach.hpp>

SINDY2HIGHWAY::SINDY2HIGHWAY(SINDY2INHWYINFO *cHwyInfo) :
SINDY2INHWY(cHwyInfo)
//mHighwayPoint()
{

}

SINDY2HIGHWAY::~SINDY2HIGHWAY()
{

}

HRESULT SINDY2HIGHWAY::Convert()
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HIGHWAY::Convert";

#ifdef	_DEBUG
	debug = 0;
#endif
	for (GDBOBJITER aIter = mpHwyInfo->eHighwayText.begin(); aIter != mpHwyInfo->eHighwayText.end(); aIter++)
	{
#ifdef	_DEBUG
		std::cerr << "Creating highway.txt... : " << ++debug << "/" << mpHwyInfo->eHighwayText.size() << "\r";
#endif
		_IRowPtr ipHwyText = aIter->second;

		// �ΏۃI�u�W�F�N�g���H
		bool aTarget = true;
		if (FAILED(hr = isTarget(ipHwyText, &aTarget)))
		{
			PrintError(ipHwyText, "�n�C�E�F�C���[�h�t���O�̎擾�Ɏ��s");
			return hr;
		}
		if (! aTarget) continue;

		HighwayText aHighwayPoint;

		// �{�ݏ��̎擾
		if (FAILED(hr = GetFacilInfo(mpHwyInfo, ipHwyText, &aHighwayPoint.eFacilInfo)))
		{
			PrintError(ipHwyText, "�{�ݏ��̎擾�Ɏ��s");
			return hr;
		}

		// �H�����V�[�P���X�ԍ�
		if (FAILED(hr = GetSequence(ipHwyText, &aHighwayPoint.eRoadSeq)))
		{
			PrintError(ipHwyText, "�H�����V�[�P���X�̎擾�Ɏ��s");
			return hr;
		}

		// ���̎{�ݔԍ�
		if (FAILED(hr = GetNextID(mpHwyInfo, ipHwyText, &aHighwayPoint.eNext)))
		{
			PrintError(ipHwyText, "���̎{�ݔԍ��̎擾�Ɏ��s");
			return hr;
		}

		// ���̎{�݂܂ł̋�ԋ���
		if (FAILED(hr = GetDistance(mpHwyInfo, ipHwyText, &aHighwayPoint.eDistance)))
		{
			PrintError(ipHwyText, "��ԋ����̎擾�Ɏ��s");
			return hr;
		}

		// �I�[�t���O
		if (FAILED(hr = GetTermFlag(ipHwyText, &aHighwayPoint.eTerm)))
		{
			PrintError(ipHwyText, "�I�[�t���O�̎擾�Ɏ��s");
			return hr;
		}

		// �O������{�݂̎擾

		if (FAILED(hr = GetToFacil(mpHwyInfo, ipHwyText, &aHighwayPoint.eTo)))
		{
			PrintError(ipHwyText, "�O������{�ݏ��̎擾�Ɏ��s");
			return hr;
		}

		// �������{�݂̎擾
		if (FAILED(hr = GetFromFacil(mpHwyInfo, ipHwyText, &aHighwayPoint.eFrom)))
		{
			PrintError(ipHwyText, "�������{�ݏ��̎擾�Ɏ��s");
			return hr;
		}

		mHighwayTextArray.insert(aHighwayPoint);
	}

#ifdef	_DEBUG
	std::cerr << std::endl;
#endif

	return hr;
}

HRESULT SINDY2HIGHWAY::Write(std::string cFileName, long cFormat, bool bIsGuide)
{
	const char *func = "SINDY2HIGHWAY::Write";

	std::ofstream aFile(cFileName.c_str(), ios::binary);
	if (! aFile.is_open()) GDBERROR2(E_ACCESSDENIED, func);

	using namespace in::schema::highway::file_format;

	BOOST_FOREACH (const HighwayText& rHighwayText, mHighwayTextArray)
	{
		// �H���R�[�h
		aFile << std::dec << rHighwayText.eFacilInfo.eRoadCode;

		switch(cFormat)
		{
			case highway_smartic_added:
				// �{�ݎ�ʁ{�o�����R�[�h
				aFile << "\t";
				aFile << std::dec << rHighwayText.eFacilInfo.eFacilClassAndInOutCode;
				break;

			case highway_etc_specific_toll:
				// �H�����V�[�P���X�ԍ�
				aFile << "\t";
				aFile << std::dec << rHighwayText.eRoadSeq;

				// �{�ݎ�ʃR�[�h
				aFile << "\t";
				aFile << std::dec << rHighwayText.eFacilInfo.eFacilClass;

				// �o�����R�[�h
				aFile << "\t";
				aFile << std::dec << rHighwayText.eFacilInfo.eInOut;
				break;

			default:
				ATLASSERT(0);
		}

		// �����t���O
		aFile << "\t";
		aFile << std::dec << rHighwayText.eFacilInfo.eLineDir;

		// �����ʘH�����V�[�P���X
		aFile << "\t";
		aFile << std::dec << rHighwayText.eFacilInfo.eOrderSeq;

		switch (cFormat)
		{
			case highway_smartic_added:
				// �H�����V�[�P���X�ԍ�
				aFile << "\t";
				aFile << std::dec << rHighwayText.eRoadSeq;
				break;

			case highway_etc_specific_toll:
				break;
			default:
				ATLASSERT(0);
		}
		// ���̎{�ݔԍ�
		aFile << "\t";
		aFile << std::dec << rHighwayText.eNext;

		// ���̎{�݂܂ł̋�ԋ���
		aFile << "\t";
		aFile << std::dec << rHighwayText.eDistance;

		// �I�[�t���O
		aFile << "\t";
		aFile << std::dec << rHighwayText.eTerm;

		// �O�����ւ̕���
		aFile << "\t";
		aFile << std::dec << rHighwayText.eTo.size();

		// ������ւ̕���
		aFile << "\t";
		aFile << std::dec << rHighwayText.eFrom.size();

		// �O�����ւ̕�����
		for (unsigned long j = 0; j < rHighwayText.eTo.size(); j++)
		{
			// �����񎯕ʃt���O
			aFile << "\t";
			aFile << "0";

			// �����H���̕����t���O
			aFile << "\t";
			aFile << std::dec << rHighwayText.eTo[j].eLineDir;

			// �����̘H���R�[�h
			aFile << "\t";
			aFile << std::dec << rHighwayText.eTo[j].eRoadCode;

			// �����̎{�ݎ�ʃR�[�h
			aFile << "\t";
			aFile << std::dec << rHighwayText.eTo[j].eFacilClassAndInOutCode;

			// �����̘H�����V�[�P���X�ԍ�
			aFile << "\t";
			aFile << std::dec << rHighwayText.eTo[j].eOrderSeq;

			if (bIsGuide) {
				// �����̕�������R�[�h
				aFile << "\t";
				aFile << std::dec << rHighwayText.eTo[j].eBranchCode;
			}
		}

		// ������ւ̕�����
		for (unsigned long j = 0; j < rHighwayText.eFrom.size(); j++)
		{
			// �����񎯕ʃt���O
			aFile << "\t";
			aFile << "1";

			// �����H���̕����t���O
			aFile << "\t";
			aFile << std::dec << rHighwayText.eFrom[j].eLineDir;

			// �����̘H���R�[�h
			aFile << "\t";
			aFile << std::dec << rHighwayText.eFrom[j].eRoadCode;

			// �����̎{�ݎ�ʃR�[�h
			aFile << "\t";
			aFile << std::dec << rHighwayText.eFrom[j].eFacilClassAndInOutCode;

			// �����̘H�����V�[�P���X�ԍ�
			aFile << "\t";
			aFile << std::dec << rHighwayText.eFrom[j].eOrderSeq;

			if (bIsGuide) {
				// �����̕�������R�[�h
				aFile << "\t";
				aFile << std::dec << rHighwayText.eFrom[j].eBranchCode;
			}
		}

		aFile << std::endl;
	}

	return S_OK;
}

HRESULT SINDY2HIGHWAY::GetToFacil(SINDY2INHWYINFO *cHwyInfo, _IRowPtr ipHwyText, std::vector<FacilInfo> *cToFacil)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HIGHWAY::GetToFacil";

	cToFacil->clear();

	// ���򐔂̎擾
	CComVariant vaCount;
	if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::FORE_COUNT, &vaCount))) return hr;
	vaCount.ChangeType(VT_I4);

	for (long i = 0; i < vaCount.lVal; i++)
	{
		FacilInfo aFacilInfo;
		// �O������ID�擾
		CComVariant vaID;
		if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::FOREID[i], &vaID))) return hr;
		vaID.ChangeType(VT_I4);
		// �����̕�����擾
		_IRowPtr ipRow;
		if (FAILED(hr = cHwyInfo->eHighwayText.SearchByObjectID(vaID.lVal, &ipRow))) return hr;
		if (ipRow == NULL) return GDBERROR2(E_UNEXPECTED, func);
		if (FAILED(hr = GetFacilInfo(cHwyInfo, ipRow, &aFacilInfo))) return hr;
		// �O����������R�[�h�擾
		CComVariant vaForDir;
		if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::FORDIR[i], &vaForDir))) return hr;
		vaForDir.ChangeType(VT_I4);
		aFacilInfo.eBranchCode = vaForDir.lVal;
		// �R���e�i�Ɋi�[
		cToFacil->push_back(aFacilInfo);
	}

	return S_OK;
}

HRESULT SINDY2HIGHWAY::GetFromFacil(SINDY2INHWYINFO *cHwyInfo, _IRowPtr ipHwyText, std::vector<FacilInfo> *cFromFacil)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HIGHWAY::GetFromFacil";

	cFromFacil->clear();

	// ���򐔂̎擾
	CComVariant vaCount;
	if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::BACK_COUNT, &vaCount))) return hr;
	vaCount.ChangeType(VT_I4);

	for (long i = 0; i < vaCount.lVal; i++)
	{
		FacilInfo aFacilInfo;
		// �������ID�擾
		CComVariant vaID;
		if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::BACKID[i], &vaID))) return hr;
		vaID.ChangeType(VT_I4);
		// �����̕�����擾
		_IRowPtr ipRow;
		if (FAILED(hr = cHwyInfo->eHighwayText.SearchByObjectID(vaID.lVal, &ipRow))) return hr;
		if (ipRow == NULL) return GDBERROR2(E_UNEXPECTED, func);
		if (FAILED(hr = GetFacilInfo(cHwyInfo, ipRow, &aFacilInfo))) return hr;
		// �����������R�[�h�擾
		CComVariant vaBackDir;
		if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::BACKDIR[i], &vaBackDir))) return hr;
		vaBackDir.ChangeType(VT_I4);
		aFacilInfo.eBranchCode = vaBackDir.lVal;
		// �R���e�i�Ɋi�[
		cFromFacil->push_back(aFacilInfo);
	}

	return S_OK;
}

HRESULT SINDY2HIGHWAY::GetTermFlag(_IRowPtr ipHwyText, long *cTerm)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HIGHWAY::GetTermFlag";

	*cTerm = 0;

	CComVariant vaPrevID, vaNextID;
	if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::PREV_ID, &vaPrevID))) return hr;
	if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::NEXT_ID, &vaNextID))) return hr;
	vaPrevID.ChangeType(VT_I4); vaNextID.ChangeType(VT_I4);

	if (! vaPrevID.lVal || ! vaNextID.lVal) *cTerm = 1;

	return S_OK;
}

HRESULT SINDY2HIGHWAY::GetDistance(SINDY2INHWYINFO *cHwyInfo, _IRowPtr ipHwyText, long *cDistance)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HIGHWAY::GetDistance";

	*cDistance = 0;

	CComVariant vaNextID;
	if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::NEXT_ID, &vaNextID))) return hr;
	vaNextID.ChangeType(VT_I4);
	if (! vaNextID.lVal) return S_OK;

	_IRowPtr ipNext;
	if (FAILED(hr = cHwyInfo->eHighwayText.SearchByObjectID(vaNextID.lVal, &ipNext))) return hr;
	if (ipNext == NULL) return GDBERROR2(E_UNEXPECTED, func);

	IFeaturePtr ipFacilInfoPoint1, ipFacilInfoPoint2;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipHwyText, &ipFacilInfoPoint1))) return hr;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipNext, &ipFacilInfoPoint2))) return hr;
	if (ipFacilInfoPoint1 == NULL || ipFacilInfoPoint2 == NULL) return GDBERROR2(E_UNEXPECTED, func);

	IGeometryPtr ipGeometry1, ipGeometry2;
	if (FAILED(hr = ipFacilInfoPoint1->get_ShapeCopy(&ipGeometry1))) return GDBERROR(hr, func);
	if (FAILED(hr = ipFacilInfoPoint2->get_ShapeCopy(&ipGeometry2))) return GDBERROR(hr, func);

	double dist = 0.0;
	if (FAILED(hr = GDBGetDistance((IPointPtr)ipGeometry1, (IPointPtr)ipGeometry2, &dist))) return hr;

	*cDistance = (long)(dist / 100.0);

	return S_OK;
}

HRESULT SINDY2HIGHWAY::GetNextID(SINDY2INHWYINFO *cHwyInfo, _IRowPtr ipHwyText, long *cNextID)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HIGHWAY::GetNextID";

	*cNextID = 0;

	CComVariant vaNextID;
	if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::NEXT_ID, &vaNextID))) return hr;
	vaNextID.ChangeType(VT_I4);
	if (! vaNextID.lVal) return S_OK;

	_IRowPtr ipNext;
	if (FAILED(hr = cHwyInfo->eHighwayText.SearchByObjectID(vaNextID.lVal, &ipNext))) return hr;
	if (ipNext == NULL) return GDBERROR2(E_UNEXPECTED, func);

	CComVariant vaSequence;
	if (FAILED(hr = GDBGetValue(ipNext, sindy::highway_text::ORDER_SEQ, &vaSequence))) return hr;
	vaSequence.ChangeType(VT_I4);

	*cNextID = vaSequence.lVal;

	return S_OK;
}

HRESULT SINDY2HIGHWAY::isTarget(_IRowPtr ipHwyText, bool *cTarget)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HIGHWAY::isTarget";

	*cTarget = true;

	// �q�{�݂͑ΏۊO
	CComVariant vaParentID;
	if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::PARENT_ID, &vaParentID))) return hr;
	vaParentID.ChangeType(VT_I4);
	if (vaParentID.lVal != 0) *cTarget = false;

	return S_OK;
}

HRESULT SINDY2HIGHWAY::GetSequence(_IRowPtr ipHwyText, long *cSequence)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HIGHWAY::GetSequence";

	*cSequence = 0;

	CComVariant vaSequence;
	if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::ROAD_SEQ, &vaSequence))) return hr;
	vaSequence.ChangeType(VT_I4);
	*cSequence = vaSequence.lVal;

	return S_OK;
}

HRESULT SINDY2HIGHWAY::GetName(_IRowPtr ipHwyText, std::string *cName)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HIGHWAY::GetName";

	*cName = "";

	CComVariant vaName;
	if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::NAME_KANJI, &vaName))) return hr;
	if (vaName.vt != VT_NULL && vaName.vt != VT_EMPTY)
	{
		vaName.ChangeType(VT_BSTR);
		*cName = COLE2T(vaName.bstrVal);
	}

	return S_OK;
}

/**
 * @brief I/N�̏o�����R�[�h�̎擾
 * @param[in]	cpHwyText	�Ώ�Highway_Text�I�u�W�F�N�g
 * @return	I/N�̏o�����R�[�h
 */
long GetInOut(_IRow* cpHwyText)
{
	long aSindyInOut = cpHwyText->GetValue( cpHwyText->Fields->_FindField(_bstr_t(sindy::highway_text::INOUT_C.c_str())) );
	return Sindy2INCode(in::schema::highway::inout::tInOut, aSindyInOut);
}


HRESULT SINDY2HIGHWAY::GetFacilInfo(SINDY2INHWYINFO *cHwyInfo, _IRowPtr ipHwyText, SINDY2HIGHWAY::FacilInfo *cFacilInfo)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HIGHWAY::GetFacilInfo";

	// �H���R�[�h�̎擾
	CComVariant vaRoadCode;
	if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::ROAD_CODE, &vaRoadCode))) return hr;
	vaRoadCode.ChangeType(VT_I4);
	cFacilInfo->eRoadCode = vaRoadCode.lVal;

	// �{�ݎ�ʁ{�o�����R�[�h�̎擾
	if (FAILED(hr = GetFacilClassAndInOutCode(cHwyInfo, ipHwyText, &cFacilInfo->eFacilClassAndInOutCode))) return hr;

	// �{�ݎ�ʃR�[�h�̎擾
	cFacilInfo->eFacilClass = GetFacilClass(ipHwyText, cHwyInfo);

	// �o�����R�[�h�̎擾
	cFacilInfo->eInOut = GetInOut(ipHwyText);

	// Highway_Text::Updown_c��������t���O�����߂�
	CComVariant vaUpDown;
	if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::UPDOWN_C, &vaUpDown))) return hr;
	vaUpDown.ChangeType(VT_I4);

	switch (vaUpDown.lVal)
	{
		using namespace in::schema::highway;
		case sindy::highway_text::updown_up:
			cFacilInfo->eLineDir = direction::back;
			break;
		case sindy::highway_text::updown_down:
			cFacilInfo->eLineDir = direction::fore;
			break;
		default:
			return GDBERROR2(E_UNEXPECTED, func);
	}

	// �����ʘH�����V�[�P���X�ԍ��̎擾
	CComVariant vaSequence;
	if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::ORDER_SEQ, &vaSequence))) return hr;
	vaSequence.ChangeType(VT_I4);
	cFacilInfo->eOrderSeq = vaSequence.lVal;

	return S_OK;
}

HRESULT SINDY2HIGHWAY::GetFacilClassAndInOutCode(SINDY2INHWYINFO *cHwyInfo, _IRowPtr ipHwyText, long *cClass)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HIGHWAY::GetFacilClassAndInOutCode";

	*cClass = 0;

	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipHwyText, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);

	CComVariant vaFacilClass;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::FACILCLASS_C, &vaFacilClass))) return hr;
	vaFacilClass.ChangeType(VT_I4);

	CComVariant vaInOut;
	if (FAILED(hr = GDBGetValue(ipHwyText, sindy::highway_text::INOUT_C, &vaInOut))) return hr;
	vaInOut.ChangeType(VT_I4);

	switch (vaFacilClass.lVal)
	{
		using namespace in::schema::highway;
		case sindy::facil_info_point::facilclass_sa:
			*cClass = facil_class_and_inout_code::sa;
			break;
		case sindy::facil_info_point::facilclass_pa:
			*cClass = facil_class_and_inout_code::pa;
			break;
		case sindy::facil_info_point::facilclass_jct:
			*cClass = facil_class_and_inout_code::jct;
			break;
		case sindy::facil_info_point::facilclass_ramp:
			switch (vaInOut.lVal)
			{
				case sindy::inout::inout_enter:
					*cClass = facil_class_and_inout_code::ramp_ent;
					break;
				case sindy::inout::inout_exit:
					*cClass = facil_class_and_inout_code::ramp_ext;
					break;
				default:
					return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		case sindy::facil_info_point::facilclass_ic:
			switch (vaInOut.lVal)
			{
				case sindy::inout::inout_enter:
					*cClass = facil_class_and_inout_code::ic_ent;
					break;
				case sindy::inout::inout_exit:
					*cClass = facil_class_and_inout_code::ic_ext;
					break;
				default:
					return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		case sindy::facil_info_point::facilclass_tollgate:
			*cClass = facil_class_and_inout_code::tollgate;
			break;
		case sindy::facil_info_point::facilclass_smartic:
			switch (vaInOut.lVal)
			{
				case sindy::access_point::inout_enter:
					*cClass = facil_class_and_inout_code::smartic_ent;
					break;
				case sindy::access_point::inout_exit:
					*cClass = facil_class_and_inout_code::smartic_ext;
					break;
				default:
					return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		default:
			return GDBERROR2(E_UNEXPECTED, func);
	}

	return S_OK;
}


