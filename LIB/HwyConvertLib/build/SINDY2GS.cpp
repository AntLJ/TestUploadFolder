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

// SINDY2GS.cpp: SINDY2GS �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "SINDY2GS.h"
#include "SINDY2INCODE.h"

SINDY2GS::SINDY2GS(SINDY2INHWYINFO *cHwyInfo) :
SINDY2INHWY(cHwyInfo),
mGsList()
{

}

SINDY2GS::~SINDY2GS()
{

}

HRESULT SINDY2GS::Convert()
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2GS::Convert";

	for (GDBOBJITER aIter = mpHwyInfo->eAccessPoint.begin(); aIter != mpHwyInfo->eAccessPoint.end(); aIter++)
	{
		IFeaturePtr ipAccessPoint = aIter->second;

		bool aHwyMode = true;
		if (FAILED(hr = mpHwyInfo->isHwyMode(ipAccessPoint, &aHwyMode)))
		{
			PrintError(ipAccessPoint, "�n�C�E�F�C���[�h�t���O�̎擾�Ɏ��s");
			return hr;
		}
		if (! aHwyMode) continue;

		CComVariant vaGsCode;
		if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::GS_C, &vaGsCode)))
		{
			PrintError(ipAccessPoint, "�K�\�����X�^���h�̎擾�Ɏ��s");
			return hr;
		}
		vaGsCode.ChangeType(VT_I4);

		CComVariant vaGs;
		if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::GS_F, &vaGs)))
		{
			PrintError(ipAccessPoint, "�K�\�����X�^���h�t���O�̎擾�Ɏ��s");
			return hr;
		}
		vaGs.ChangeType(VT_I4);
		CComVariant vaMultipleStore;
		if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::MULTIPLESTORE_F, &vaMultipleStore)))
		{
			PrintError(ipAccessPoint, "�`�F�[���X�t���O�̎擾�Ɏ��s");
			return hr;
		}
		vaMultipleStore.ChangeType(VT_I4);

		// �K�\�����X�^���hor�`�F�[���X�̂Ȃ��A�N�Z�X�|�C���g�Ȃ�X�L�b�v
		if (! vaGs.lVal && ! vaMultipleStore.lVal) {
			continue;
		}

		long aRoadCode, aRoadSeq;
		if (FAILED(hr = mpHwyInfo->GetFacilCode(ipAccessPoint, &aRoadCode, &aRoadSeq)))
		{
			PrintError(ipAccessPoint, "�{�ݔԍ��̎擾�Ɏ��s");
			return hr;
		}

		IFeaturePtr ipFacilInfoPoint;
		if (FAILED(hr = mpHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint)))
		{
			PrintError(ipAccessPoint, "�Ή��{�ݏ��|�C���g�̎擾�Ɏ��s");
			return hr;
		}
		CComVariant vaName;
		if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::NAME_KANJI, &vaName)))
		{
			PrintError(ipAccessPoint, "�{�ݖ��̂̎擾�Ɏ��s");
			return hr;
		}
		std::string aName;
		if (vaName.vt != VT_NULL && vaName.vt != VT_EMPTY)
		{
			vaName.ChangeType(VT_BSTR);
			aName = COLE2T(vaName.bstrVal);
		}

		CComVariant vaLineDir;
		if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::DIRECTION_C, &vaLineDir)))
		{
			PrintError(ipAccessPoint, "�㉺�������̎擾�Ɏ��s");
			return hr;
		}
		vaLineDir.ChangeType(VT_I4);

		GS aGsInfo;
		aGsInfo.eGsType  = vaGsCode.lVal;
		aGsInfo.eLineDir  = vaLineDir.lVal;
		aGsInfo.eName     = aName;
		aGsInfo.eRoadCode = aRoadCode;
		aGsInfo.eSequence = aRoadSeq;

		std::vector<AccessStore>	aAccessStoreList;
		if (FAILED(hr = GetAccessStore(mpHwyInfo, ipAccessPoint, &aAccessStoreList))) 
			return hr;

		for (std::vector<AccessStore>::const_iterator aIt = aAccessStoreList.begin(); aIt != aAccessStoreList.end(); ++aIt)
		{
			switch (aIt->eStoreType)
			{
			case gs:
				if (vaGs.lVal) {
					aGsInfo.eAccessStoreInfo.push_back(*aIt);
				}
				break;
			case multiple_store:
				if (vaMultipleStore.lVal) {
					aGsInfo.eAccessStoreInfo.push_back(*aIt);
				}
				break;
			default:
				ATLASSERT(0);
			}
		}

		mGsList.push_back(aGsInfo);
	}

	return S_OK;
}

HRESULT SINDY2GS::Write(std::string cFileName, EType cType, EFormat cFormat)
{
	const char *func = "SINDY2GS::Write";

	std::ofstream aFile(cFileName.c_str(), ios::binary);
	if (! aFile.is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	for (unsigned long i = 0; i < mGsList.size(); i++)
	{
		switch (cFormat)
		{
		case gs_old:

			// GS_C��gs_no�ł���Ώo�͂��Ȃ�
			if (mGsList[i].eGsType == sindy::access_point::gs_no) continue;

			// �H���R�[�h
			aFile << std::dec << mGsList[i].eRoadCode;

			// �H�����V�[�P���X�ԍ�
			aFile << "\t";
			aFile << std::dec << mGsList[i].eSequence;

			// �{�ݖ���
			aFile << "\t";
			aFile << SINDY2INCODE::SJIS2EUC(mGsList[i].eName.c_str());

			// �|�C���g�ʒu����
			aFile << "\t";
			switch (mGsList[i].eLineDir)
			{
			case sindy::access_point::direction_up:		aFile << SINDY2INCODE::SJIS2EUC("���"); break;
			case sindy::access_point::direction_down:	aFile << SINDY2INCODE::SJIS2EUC("����"); break;

			/// ���E����ȊO�̓G���[�Ƃ���
			case sindy::access_point::direction_no:		//aFile << SINDY2INCODE::SJIS2EUC("�����Ȃ�"); break;
			case sindy::access_point::direction_common:	//aFile << SINDY2INCODE::SJIS2EUC("�㉺������"); break;
			default:
				GDBERROR2(E_NOINTERFACE, func);
			}

			// �K�\�����X�^���h����
			aFile << "\t";
			switch (mGsList[i].eGsType)
			{
			case sindy::access_point::gs_other:		aFile << SINDY2INCODE::SJIS2EUC("���̑�"); break;
			case sindy::access_point::gs_esso:		aFile << SINDY2INCODE::SJIS2EUC("�G�b�\"); break;
			case sindy::access_point::gs_mobil:		aFile << SINDY2INCODE::SJIS2EUC("���[�r��"); break;
			case sindy::access_point::gs_kygnus:	aFile << SINDY2INCODE::SJIS2EUC("�L�O�i�X"); break;
			case sindy::access_point::gs_cosmo:		aFile << SINDY2INCODE::SJIS2EUC("�R�X��"); break;
			case sindy::access_point::gs_shell:		aFile << SINDY2INCODE::SJIS2EUC("�V�F��"); break;
			case sindy::access_point::gs_jomo:		aFile << SINDY2INCODE::SJIS2EUC("�W����"); break;
			case sindy::access_point::gs_general:	aFile << SINDY2INCODE::SJIS2EUC("�[�l����"); break;
			case sindy::access_point::gs_kyushu:	aFile << SINDY2INCODE::SJIS2EUC("��B"); break;
			case sindy::access_point::gs_idemitsu:	aFile << SINDY2INCODE::SJIS2EUC("�o��"); break;
			case sindy::access_point::gs_taiyo:		aFile << SINDY2INCODE::SJIS2EUC("���z"); break;
			case sindy::access_point::gs_eneos:		aFile << SINDY2INCODE::SJIS2EUC("�d�m�d�n�r"); break;
			default:								aFile << SINDY2INCODE::SJIS2EUC("�H�H�H"); break;
			}
			aFile << std::endl;

			break;

		case gs_new:
			// Access_Store�̐������ʃ��R�[�h�Ƃ��ďo��
			for (std::vector<AccessStore>::const_iterator aIt = mGsList[i].eAccessStoreInfo.begin(); aIt != mGsList[i].eAccessStoreInfo.end(); ++aIt)
			{
				// gs/multiple_store�̂����A�w�肳�ꂽ�^�C�v�ɍ��v���郌�R�[�h�������o��
				if (aIt->eStoreType != cType) { continue; }

				// �H���R�[�h
				aFile << std::dec << mGsList[i].eRoadCode;

				// �H�����V�[�P���X�ԍ�
				aFile << "\t";
				aFile << std::dec << mGsList[i].eSequence;

				// �㉺���t���O
				aFile << "\t";
				switch (mGsList[i].eLineDir)
				{
				case sindy::access_point::direction_up:		aFile << direction_up;		break;
				case sindy::access_point::direction_down:	aFile << direction_down;	break;

				/// ���E����ȊO�̓G���[�Ƃ���
				case sindy::access_point::direction_no:
				case sindy::access_point::direction_common:
				default:
					GDBERROR2(E_NOINTERFACE, func);
				}

				// �`�F�[���X�}�X�^�[�R�[�h
				aFile << "\t";
				aFile << aIt->eStoreCode;
				
				// �c�Ǝ���
				aFile << "\t";
				aFile.width(2); aFile.fill('0');
				aFile << aIt->eStartHour;
				aFile.width(2); aFile.fill('0');
				aFile << aIt->eStartMin;
				aFile.width(2); aFile.fill('0');
				aFile << aIt->eEndHour;
				aFile.width(2); aFile.fill('0');
				aFile << aIt->eEndMin;

				// �j���t���O
				aFile << "\t";	aFile << aIt->eSunday;
				aFile << "\t";	aFile << aIt->eMonday;
				aFile << "\t";	aFile << aIt->eTuesday;
				aFile << "\t";	aFile << aIt->eWednesday;
				aFile << "\t";	aFile << aIt->eThursday;
				aFile << "\t";	aFile << aIt->eFriday;
				aFile << "\t";	aFile << aIt->eSaturday;

				// ���ԃt���O
				aFile << "\t";	aFile << aIt->eBon;
				aFile << "\t";	aFile << aIt->eYearEnd;
				aFile << "\t";	aFile << aIt->eNewYear;
				aFile << "\t";	aFile << aIt->eGoldenWeek;

				// �j�Փ��t���O
				aFile << "\t";	aFile << aIt->eHoliday;

				aFile << std::endl;
			}
			break;

		default:
			ATLASSERT(0);
		}
	}

	return S_OK;
}


HRESULT SINDY2GS::GetAccessStore(SINDY2INHWYINFO* cHwyInfo, IFeaturePtr cpAccessPoint, std::vector<AccessStore>* cpAccessStoreList)
{
	HRESULT hr = S_OK;

	long aOID = cpAccessPoint->GetOID();
	GDBOBJRANGE aRange;
	if (FAILED(hr = cHwyInfo->eAccessStore.SearchByAccessPointID(aOID, &aRange)))
	{
		PrintError(cpAccessPoint, "�Ή�����f�[�^��Access_Store�ɑ��݂��Ȃ�");
		return hr;
	}

	for (GDB2OBJITER aIt = aRange.first; aIt != aRange.second; ++aIt)
	{
		AccessStore aAccessStore;

		// �`�F�[���X�}�X�^�[�R�[�h
		CComVariant	vaStoreCode;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::MULTIPLESTORE_C, &vaStoreCode)))
		{
			PrintError(cpAccessPoint, "�`�F�[���X�}�X�^�[�R�[�h�̎擾�Ɏ��s");
			return hr;
		}
		vaStoreCode.ChangeType(VT_I4);
		aAccessStore.eStoreCode = vaStoreCode.lVal;

		// �c�ƊJ�n����
		CComVariant vaStartHour;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::START_HOUR, &vaStartHour)))
		{
			PrintError(cpAccessPoint, "�c�ƊJ�n���Ԃ̎擾�Ɏ��s");
			return hr;
		}
		vaStartHour.ChangeType(VT_I4);
		aAccessStore.eStartHour = vaStartHour.lVal;

		// �c�ƊJ�n��
		CComVariant vaStartMin;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::START_MIN, &vaStartMin)))
		{
			PrintError(cpAccessPoint, "�c�ƊJ�n���̎擾�Ɏ��s");
			return hr;
		}
		vaStartMin.ChangeType(VT_I4);
		aAccessStore.eStartMin = vaStartMin.lVal;
		
		// �c�ƏI������
		CComVariant vaEndHour;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::END_HOUR, &vaEndHour)))
		{
			PrintError(cpAccessPoint, "�c�ƏI�����Ԃ̎擾�Ɏ��s");
			return hr;
		}
		vaEndHour.ChangeType(VT_I4);
		aAccessStore.eEndHour = vaEndHour.lVal;
		
		// �c�ƏI����
		CComVariant vaEndMin;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::END_MIN, &vaEndMin)))
		{
			PrintError(cpAccessPoint, "�c�ƏI�����̎擾�Ɏ��s");
			return hr;
		}
		vaEndMin.ChangeType(VT_I4);
		aAccessStore.eEndMin = vaEndMin.lVal;
		
		// ���j���c�ƃt���O
		CComVariant vaSunday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::SUNDAY_F, &vaSunday)))
		{
			PrintError(cpAccessPoint, "���j���c�ƃt���O�̎擾�Ɏ��s");
			return hr;
		}
		vaSunday.ChangeType(VT_BOOL);
		aAccessStore.eSunday = vaSunday.boolVal ? true : false;
		
		// ���j���c�ƃt���O
		CComVariant vaMonday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::MONDAY_F, &vaMonday)))
		{
			PrintError(cpAccessPoint, "���j���c�ƃt���O�̎擾�Ɏ��s");
			return hr;
		}
		vaMonday.ChangeType(VT_BOOL);
		aAccessStore.eMonday = vaMonday.boolVal ? true : false;
		
		// �Ηj���c�ƃt���O
		CComVariant vaTuesday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::TUESDAY_F, &vaTuesday)))
		{
			PrintError(cpAccessPoint, "�Ηj���c�ƃt���O�̎擾�Ɏ��s");
			return hr;
		}
		vaTuesday.ChangeType(VT_BOOL);
		aAccessStore.eTuesday = vaTuesday.boolVal ? true : false;
		
		// ���j���c�ƃt���O
		CComVariant vaWednesday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::WEDNESDAY_F, &vaWednesday)))
		{
			PrintError(cpAccessPoint, "���j���c�ƃt���O�̎擾�Ɏ��s");
			return hr;
		}
		vaWednesday.ChangeType(VT_BOOL);
		aAccessStore.eWednesday = vaWednesday.boolVal ? true : false;
		
		// �ؗj���c�ƃt���O
		CComVariant vaThursday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::THURSDAY_F, &vaThursday)))
		{
			PrintError(cpAccessPoint, "�ؗj���c�ƃt���O�̎擾�Ɏ��s");
			return hr;
		}
		vaThursday.ChangeType(VT_BOOL);
		aAccessStore.eThursday = vaThursday.boolVal ? true : false;
		
		// ���j���c�ƃt���O
		CComVariant vaFriday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::FRIDAY_F, &vaFriday)))
		{
			PrintError(cpAccessPoint, "���j���c�ƃt���O�̎擾�Ɏ��s");
			return hr;
		}
		vaFriday.ChangeType(VT_BOOL);
		aAccessStore.eFriday = vaFriday.boolVal ? true : false;
		
		// �y�j���c�ƃt���O
		CComVariant vaSaturday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::SATURDAY_F, &vaSaturday)))
		{
			PrintError(cpAccessPoint, "�y�j���c�ƃt���O�̎擾�Ɏ��s");
			return hr;
		}
		vaSaturday.ChangeType(VT_BOOL);
		aAccessStore.eSaturday = vaSaturday.boolVal ? true : false;
		
		// ���~�c�ƃt���O
		CComVariant vaBon;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::BON_F, &vaBon)))
		{
			PrintError(cpAccessPoint, "���~�c�ƃt���O�̎擾�Ɏ��s");
			return hr;
		}
		vaBon.ChangeType(VT_BOOL);
		aAccessStore.eBon = vaBon.boolVal ? true : false;
		
		// �N���c�ƃt���O
		CComVariant vaYearEnd;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::YEAREND_F, &vaYearEnd)))
		{
			PrintError(cpAccessPoint, "�N���c�ƃt���O�̎擾�Ɏ��s");
			return hr;
		}
		vaYearEnd.ChangeType(VT_BOOL);
		aAccessStore.eYearEnd = vaYearEnd.boolVal ? true : false;
		
		// �N�n�c�ƃt���O
		CComVariant vaNewYear;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::NEWYEAR_F, &vaNewYear)))
		{
			PrintError(cpAccessPoint, "�N�n�c�ƃt���O�̎擾�Ɏ��s");
			return hr;
		}
		vaNewYear.ChangeType(VT_BOOL);
		aAccessStore.eNewYear = vaNewYear.boolVal ? true : false;
		
		// �S�[���f���E�B�[�N�c�ƃt���O
		CComVariant vaGoldenWeek;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::GOLDENWEEK_F, &vaGoldenWeek)))
		{
			PrintError(cpAccessPoint, "�S�[���f���E�B�[�N�c�ƃt���O�̎擾�Ɏ��s");
			return hr;
		}
		vaGoldenWeek.ChangeType(VT_BOOL);
		aAccessStore.eGoldenWeek = vaGoldenWeek.boolVal ? true : false;
		
		// �j�Փ��c�ƃt���O
		CComVariant vaHoliday;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::HOLIDAY_F, &vaHoliday)))
		{
			PrintError(cpAccessPoint, "�j�Փ��c�ƃt���O�̎擾�Ɏ��s");
			return hr;
		}
		vaHoliday.ChangeType(VT_BOOL);
		aAccessStore.eHoliday = vaHoliday.boolVal ? true : false;
		
		// �X�܎�ʃR�[�h
		CComVariant vaStoreType;
		if (FAILED(hr = GDBGetValue(aIt->second, sindy::access_store::STORETYPE_C, &vaStoreType)))
		{
			PrintError(cpAccessPoint, "�X�܎�ʃR�[�h�̎擾�Ɏ��s");
			return hr;
		}
		vaStoreType.ChangeType(VT_I4);
		switch (vaStoreType.lVal)
		{
		case sindy::access_store::storetype_gs:
			aAccessStore.eStoreType = gs;
			break;
		case sindy::access_store::storetype_store:
			aAccessStore.eStoreType = multiple_store;
			break;
		default:
			ATLASSERT(0);
		}
		
		cpAccessStoreList->push_back(aAccessStore);
	}
	
	return S_OK;
}

