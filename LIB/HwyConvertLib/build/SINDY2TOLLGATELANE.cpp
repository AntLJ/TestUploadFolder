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

#include "SINDY2TOLLGATELANE.h"
#include "SINDY2HWYNODE.h"
#include "SindyAttrDef.h"
#include <boost/xpressive/xpressive.hpp>
#include <boost/foreach.hpp>

SINDY2TOLLGATELANE::SINDY2TOLLGATELANE(SINDY2INHWYINFO *cHwyInfo)
: SINDY2INHWY(cHwyInfo)
{
}

SINDY2TOLLGATELANE::~SINDY2TOLLGATELANE()
{
}

/**
 * @brief �^����ꂽ�n�C�E�F�C�m�[�h��TollClass_c���������ɓ�������̂��ۂ��𔻒肷��
 * @note	�������ɓ�����̂́ATollClass_c = �{���㗿�����A�����v�㗿�����A�o���A�̂����ꂩ
 * @param[in]	cpHwyNode	�Ώۃn�C�E�F�C�m�[�h
 * @return	�^����ꂽ�n�C�E�F�C�m�[�h��TollClass_c���������ɓ�������̂��ۂ�
 */
bool isTollGateHwyNode(IFeature* cpHwyNode)
{
	static long aTollClassFieldIndex = cpHwyNode->Fields->_FindField(sindy::highway_node::TOLLCLASS_C.c_str());
	long aTollClass = cpHwyNode->GetValue(aTollClassFieldIndex);
	bool aResult = false;
	switch (aTollClass)
	{
		using namespace sindy::highway_node;
		case tollclass_honsen:
		case tollclass_ramp:
		case tollclass_barrier:
			aResult = true;
			break;
		case tollclass_other:
		case tollclass_inout:
			aResult = false;
			break;
		default:
			ATLASSERT(0);
	}

	return aResult;
}

/**
 * @brief �^����ꂽ�n�C�E�F�C�m�[�h��DummyToll_f��1�ł��邩�ۂ��𔻒肷��
 * @param[in]	cpHwyNode	�Ώۃn�C�E�F�C�m�[�h
 * @return	�^����ꂽ�n�C�E�F�C�m�[�h��DummyToll_f��1�ł��邩�ۂ�
 */
bool isDummyTollHwyNode(IFeature* cpHwyNode)
{
	static long aDummyTollFieldIndex = cpHwyNode->Fields->_FindField(sindy::highway_node::DUMMYTOLL_F.c_str());
	long aDummyTollFlag= cpHwyNode->GetValue(aDummyTollFieldIndex);
	return static_cast<bool>(aDummyTollFlag);
}

/**
 * @brief SiNDY����K�v�ȃf�[�^���擾���ăR���e�i�Ɋi�[����
 */
HRESULT SINDY2TOLLGATELANE::Convert(void)
{
	HRESULT hr = S_OK;
	for (GDBOBJITER aIt = mpHwyInfo->eHighwayNode.begin(); aIt != mpHwyInfo->eHighwayNode.end(); ++aIt)
	{
		TollGateLaneInfo aTollGateLaneInfo;
		aTollGateLaneInfo.m_HwyNodeOID = aIt->first;
		IFeaturePtr ipHwyNode = aIt->second;

		// ���[����Ԃ̎擾
		if (FAILED(hr = GetLaneStr(ipHwyNode, aTollGateLaneInfo.m_LaneStr)))
		{
			PrintError(ipHwyNode, "���[����Ԃ̎擾�Ɏ��s");
			return hr;
		}

		// ���[�����̎擾
		aTollGateLaneInfo.m_LaneNum = aTollGateLaneInfo.m_LaneStr.size();

		// ���[����񂪂Ȃ��n�C�E�F�C�m�[�h�Ȃ�X�L�b�v
		if (aTollGateLaneInfo.m_LaneNum == 0)
		{
			continue;
		}

		// TollClass_c���������i�{���㗿�����A�����v�㗿�����A�o���A�̂����ꂩ�j�łȂ���΃G���[���b�Z�[�W���o�͂��ăX�L�b�v
		if (! isTollGateHwyNode(ipHwyNode))
		{
			PrintError(ipHwyNode, "�������ł͂Ȃ��n�C�E�F�C�m�[�h�ɗ��������[����񂪂���");
			return hr;
		}

		// DummyToll_f=1�Ȃ�G���[���b�Z�[�W���o�͂��ăX�L�b�v
		if (isDummyTollHwyNode(ipHwyNode))
		{
			PrintError(ipHwyNode, "�_�~�[�������̃n�C�E�F�C�m�[�h�ɗ��������[����񂪂���");
			return hr;
		}

		// 2�����b�V���R�[�h�̎擾
		if (FAILED(hr = GetMeshCode(ipHwyNode, aTollGateLaneInfo.m_MeshCode)))
		{
			PrintError(ipHwyNode, "2�����b�V���R�[�h�̎擾�Ɏ��s");
			return hr;
		}

		// �������m�[�hID�̎擾
		if (FAILED(hr = GetTollGateNodeID(ipHwyNode, aTollGateLaneInfo.m_TollGateNodeID)))
		{
			PrintError(ipHwyNode, "�������m�[�hID�̎擾�Ɏ��s");
			return hr;
		}

		// �����m�[�hID�̎擾
		if (FAILED(hr = GetApproachNodeID(ipHwyNode, aTollGateLaneInfo.m_ApproachNodeID)))
		{
			PrintError(ipHwyNode, "�����m�[�hID�̎擾�Ɏ��s");
			return hr;
		}

		// �R���e�i�Ɋi�[
		bool aInsertResult = m_TollGateLaneInfoArray.insert(aTollGateLaneInfo).second;
#ifdef _DEBUG
		// �f�o�b�O���̂݁A�o�͑ΏۊO�Ƃ������Ƃ�ʒm
		if (! aInsertResult)
		{
			std::cout << "\t" << "Highway_Node" << "\t" << aTollGateLaneInfo.m_HwyNodeOID <<  "\t\t\t\t\t" << "���ɓ��ꃌ�R�[�h�����݂���̂ŁA�o�͑ΏۊO�Ƃ���" << std::endl;
		}
#endif
	}

	return S_OK;	
}

/**
 * @brief �^����ꂽ_variant_t�^�̒l��VT_NULL�Ȃ�A�󕶎���ɂ��ĕԂ�
 * @note	�^����ꂽ�l�͕������\�����Ƃ��O��
 * @param[in]	cVariant	�Ώە�����
 * @return	�ϊ���̕�����
 **/
_variant_t Null2Empty(_variant_t cVariant)
{
	if (cVariant.vt == VT_NULL)
	{
		cVariant = _bstr_t("");
	}

	return cVariant;
}

/**
 * @brief ���[����Ԃ̎擾
 * @param[in]	cpHwyNode	�Ώۃn�C�E�F�C�m�[�h
 * @param[out]	crLaneStr	�擾�������[������Ԃ��o�b�t�@
 * @note	�擾����������ɕs���ȕ������܂܂�Ă���ꍇ�̓G���[���b�Z�[�W���o�͂��AE_FAIL��Ԃ�
 */
HRESULT SINDY2TOLLGATELANE::GetLaneStr(IFeaturePtr cpHwyNode, std::string& crLaneStr)
{
	try
	{
		static long aTollGateLaneFieldIndex = cpHwyNode->Fields->_FindField(sindy::highway_node::TOLLGATE_LANE.c_str());
		crLaneStr = COLE2T(Null2Empty(cpHwyNode->GetValue(aTollGateLaneFieldIndex)).bstrVal);
	}
	catch (...)
	{
		return E_FAIL;
	}

	// �s���ȕ������܂܂�Ă��Ȃ����`�F�b�N
	using namespace boost::xpressive;
	sregex aRegex = sregex::compile("[^0-9]");	// ���p�����ȊO�͕s��
	if (regex_search(crLaneStr, aRegex))
	{
		PrintError(cpHwyNode, "Highway_Node::TollGate_Lane�ɕs���ȕ������܂܂�Ă���");
		return E_FAIL;
	}

	return S_OK;
}

/**
 * @brief 2�����b�V���R�[�h�̎擾
 * @note	SINDY2HWYNODE::GetMeshCode()�Ɠ����Ȃ̂ŁA���̂܂܎g�p
 * @param[in]	ipHwyNode	�Ώۃn�C�E�F�C�m�[�h
 * @param[out]	crMesh	�擾����2�����b�V���R�[�h��Ԃ��o�b�t�@
 */
HRESULT SINDY2TOLLGATELANE::GetMeshCode(IFeaturePtr cpHwyNode, long& crMesh)
{
	return SINDY2HWYNODE::GetMeshCode(cpHwyNode, &crMesh);
}

/**
 * @brief �������m�[�hID�̎擾
 * @param[in]	cpHwyNode	�Ώۃn�C�E�F�C�m�[�h
 * @param[out]	crNodeID	�擾�����m�[�hID��Ԃ��o�b�t�@
 */
HRESULT SINDY2TOLLGATELANE::GetTollGateNodeID(IFeaturePtr cpHwyNode, long& crNodeID)
{
	// �Ή��m�[�h�̎擾
	IFeaturePtr ipNode;
	HRESULT hr = S_OK;
	if (FAILED(hr = mpHwyInfo->GetHwyNode(cpHwyNode, &ipNode)))
		return hr;

	crNodeID = ipNode->OID;
	return S_OK;
}

/**
 * @brief �����m�[�hID�̎擾
 * @param[in]	cpHwyNode	�Ώۃn�C�E�F�C�m�[�h
 * @param[out]	crNodeID	�擾���������m�[�hID��Ԃ��o�b�t�@
 */
HRESULT SINDY2TOLLGATELANE::GetApproachNodeID(IFeaturePtr cpHwyNode, long& crNodeID)
{
	try
	{
		static long aApproachNodeIDFieldIndex = cpHwyNode->Fields->_FindField(sindy::highway_node::APPROACHNODE_ID.c_str());
		crNodeID = cpHwyNode->GetValue(aApproachNodeIDFieldIndex);
	}
	catch (...)
	{
		return E_FAIL;
	}
	return S_OK;
}

/**
 * @brief �R���e�i�Ɋi�[����Ă���f�[�^���t�@�C���ɏo�͂���
 * @param[in]	cFilePath	�o�̓t�@�C���̃p�X
 */
HRESULT SINDY2TOLLGATELANE::Write(std::string cFilePath)
{
	const char *func = "SINDY2TOLLGATELANE::Write";

	// �t�@�C���o�̓X�g���[���̍쐬
	std::ofstream fout(cFilePath.c_str(), ios::binary);
	if (! fout.is_open())
	{
		return GDBERROR2(E_ACCESSDENIED, func);
	}

	std::string fd = "\t";	// �t�B�[���h�f���~�^

	BOOST_FOREACH (const TollGateLaneInfo& aTollGateLaneInfo, m_TollGateLaneInfoArray)
	{
		// 2�����b�V���R�[�h
		fout << aTollGateLaneInfo.m_MeshCode << fd;

		// �������m�[�hID
		fout << aTollGateLaneInfo.m_TollGateNodeID << fd;

		// �����m�[�hID
		fout << aTollGateLaneInfo.m_ApproachNodeID << fd;

		// ���[����
		fout << aTollGateLaneInfo.m_LaneNum << fd;

		// ���[�����
		fout << aTollGateLaneInfo.m_LaneStr << std::endl;
	}

	return S_OK;
}

