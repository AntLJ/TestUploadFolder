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

// SINDY2HWYNODE.cpp: SINDY2HWYNODE �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "SINDY2HWYNODE.h"
#include "SINDY2INCODE.h"
#include "INHwyCommonUtility.h"
#include <iomanip>
#include <boost/foreach.hpp>

using namespace in::schema::hwynode;

HighwayNode::HighwayNode() :
	eMesh(0),
	eNode(0),
	eRoadCode(0),
	eSequence(0),
	eFacilPos(0),
	eDirection(0),
	eFacilClass(0),
	eTollClass(0),
	eInOut(0),
	eNoEtcCarFlag(false),
	eNoGuideTollFlag(false),
	eEtcFacilCode(0),
	eEtcNoGuideTollFlag(false),
	eAppNode(0),
	eFlowNode(0)
{
	// �t���O��̏�����
	eTollFuncFlags.assign(false);
	eEtcTollFuncFlags.assign(false);
}


SINDY2HWYNODE::SINDY2HWYNODE(SINDY2INHWYINFO *cHwyInfo) :
SINDY2INHWY(cHwyInfo)
{
}

SINDY2HWYNODE::~SINDY2HWYNODE()
{
}

/**
 * @brief SiNDY�̗������@�\�R�[�hTollFunc_c����I/N�̗������@�\�R�[�h�̃t���O��ւ̕ϊ�
 * @param[in]	cTollFunc	SiNDY�̗������@�\�R�[�h
 * @param[out]	crTollFuncFlags	I/N�̗������@�\�R�[�h�̃t���O����i�[����o�b�t�@
 */
void SindyTollClassCode2INTollClassFlags(long cTollFunc, TollFuncFlags& crTollFuncFlags)
{
	switch (cTollFunc)
	{
		using namespace sindy::highway_node;
		using namespace toll_func_flags;
		
		case tollfunc_no:	// �@�\����
			break;
		case tollfunc_issue:	// �����@�\
			crTollFuncFlags[ticket] = true;
			break;
		case tollfunc_pay:	// ���Z�@�\
			crTollFuncFlags[adjust] = true;
			break;
		case tollfunc_single:	// �P�Ɨ�����
			crTollFuncFlags[alone] = true;
			break;
		case tollfunc_singleissue:	// �P�Ɨ������{�����@�\
			crTollFuncFlags[alone] = true;
			crTollFuncFlags[ticket] = true;
			break;
		case tollfunc_singlepay:	// �P�Ɨ������{���Z�@�\
			crTollFuncFlags[alone] = true;
			crTollFuncFlags[adjust] = true;
			break;
		case tollfunc_check:	// U�^�[���p���D�@�\
			crTollFuncFlags[check_ticket] = true;
			break;
		case tollfunc_issue2:	// �P�Ɨ������������@�\
			crTollFuncFlags[alone_invalid_ticket] = true;
			break;
		case tollfunc_payissue:	// ���Z�@�\�{�����@�\
			crTollFuncFlags[adjust] = true;
			crTollFuncFlags[ticket] = true;
			break;
		default:
			ATLASSERT(0);
	}
}

/**
 * @brief �������@�\�R�[�h�̃t���O��̎擾
 * @note	Highway_Node�t�B�[�`�����痿�����@�\�R�[�h���擾���A���̒l�ɉ����ăt���O����Z�b�g
 * @param[in]	cpHwyNode	�Ώۃn�C�E�F�C�m�[�h
 * @param[out]	crTollFuncFlags	I/N�̗������@�\�R�[�h�̃t���O����i�[����o�b�t�@
 */
void GetTollFuncFlags(IFeature* cpHwyNode, TollFuncFlags& crTollFuncFlags)
{
	try
	{
		long aTollFunc = cpHwyNode->GetValue( cpHwyNode->Fields->_FindField(_bstr_t(sindy::highway_node::TOLLFUNC_C.c_str())) );
		SindyTollClassCode2INTollClassFlags(aTollFunc, crTollFuncFlags);
	}
	catch (...)
	{
		throw std::runtime_error("�������@�\�R�[�h�̎擾�Ɏ��s");
	}
}

/**
 * @brief �������@�\�R�[�h�̃t���O��̎擾
 * @note	GetTollFuncFlags()��ETC��
 * @param[in]	cpHwyNode	�Ώۃn�C�E�F�C�m�[�h
 * @param[out]	crTollFuncFlags	I/N�̗������@�\�R�[�h�̃t���O����i�[����o�b�t�@
 */
void GetEtcTollFuncFlags(IFeature* cpHwyNode, TollFuncFlags& crTollFuncFlags)
{
	try
	{
		long aTollFunc = cpHwyNode->GetValue( cpHwyNode->Fields->_FindField(_bstr_t(sindy::highway_node::ETC_TOLLFUNC_C.c_str())) );
		SindyTollClassCode2INTollClassFlags(aTollFunc, crTollFuncFlags);
	}
	catch (...)
	{
		throw std::runtime_error("�������@�\�R�[�h(ETC)�̎擾�Ɏ��s");
	}
}

/**
 * @brief I/N�̗�������ʃR�[�h�̎擾
 * @note	�^����ꂽ�n�C�E�F�C�m�[�h����SiNDY�̗�������ʃR�[�h���擾���A�Ή�����I/N�̗�������ʃR�[�h��Ԃ�
 * @param[in]	cpHwyNode	�Ώۃn�C�E�F�C�m�[�h
 * @return I/N�̗�������ʃR�[�h
 */
long GetTollClass(IFeature* cpHwyNode)
{
	long aSindyTollClass = 0;
	try
	{
		aSindyTollClass = cpHwyNode->GetValue( cpHwyNode->Fields->_FindField(_bstr_t(sindy::highway_node::TOLLCLASS_C.c_str())) );
	}
	catch (...)
	{
		throw std::runtime_error("��������ʃR�[�h�̎擾�Ɏ��s");
	}

	long aINTollClass = 0;
	switch (aSindyTollClass)
	{
		using namespace sindy::highway_node;
		case tollclass_other:
		case tollclass_inout:	// [Bug6309] hwynode_inout.txt�ɂ���������ʃR�[�h���o�͂��A���̒l��0�i�������ȊO�j�Ƃ���
			aINTollClass = in::schema::hwynode::tollclass::no_tollbooth;
			break;
		case tollclass_honsen:
			aINTollClass = in::schema::hwynode::tollclass::main_tollbooth;
			break;
		case tollclass_ramp:
			aINTollClass = in::schema::hwynode::tollclass::ramp_tollbooth;
			break;
		case tollclass_barrier:
			aINTollClass = in::schema::hwynode::tollclass::barrier;
			break;
		default:
			ATLASSERT(0);
	}

	return aINTollClass;
}

namespace {

/**
 * @brief �o�����R�[�h�̎擾
 * @note	�^����ꂽ�n�C�E�F�C�m�[�h����SiNDY�̕���E�����R�[�h���擾���A�Ή�����I/N�̏o�����R�[�h��Ԃ�
 * @param[in]	cpHwyNode	�Ώۃn�C�E�F�C�m�[�h
 * @return	�o�����R�[�h
 */
long GetInOut(IFeature* cpHwyNode)
{
	long aSindyInOut = 0;
	try
	{
		aSindyInOut = cpHwyNode->GetValue( cpHwyNode->Fields->_FindField(_bstr_t(sindy::highway_node::INOUT_C.c_str())) );
	}
	catch (...)
	{
		throw std::runtime_error("�o�����R�[�h�̎擾�Ɏ��s");
	}

	return Sindy2INCode(in::schema::hwynode::inout::tInOut, aSindyInOut);
}

}	// anonymous namespace

/**
 * @brief I/N��ETC�{�݃R�[�h�̎擾
 * @note	�^����ꂽ�n�C�E�F�C�m�[�h����SiNDY��ETC�{�݃R�[�h���擾���A�Ή�����I/N��ETC�{�݃R�[�h��Ԃ�
 * @param[in]	cpHwyNode	�Ώۃn�C�E�F�C�m�[�h
 * @return I/N��ETC�{�݃R�[�h
 */
long GetEtcFacil(IFeature* cpHwyNode)
{
	long aSindyEtcFacil = 0;
	try
	{
		aSindyEtcFacil = cpHwyNode->GetValue( cpHwyNode->Fields->_FindField(_bstr_t(sindy::highway_node::ETC_FACIL_C.c_str())) );
	}
	catch (...)
	{
		throw std::runtime_error("ETC�{�݃R�[�h�̎擾�Ɏ��s");
	}

	long aINEtcFacil = 0;
	switch (aSindyEtcFacil)
	{
		using namespace sindy::highway_node;
		case etc_facil_no_etc_facil:
			aINEtcFacil = in::schema::hwynode::etc_facil::no_etc_facil;
			break;
		case etc_facil_gate:
			aINEtcFacil = in::schema::hwynode::etc_facil::gate;
			break;
		case etc_facil_antenna:
			aINEtcFacil = in::schema::hwynode::etc_facil::antenna;
			break;
		default:
			ATLASSERT(0);
	}

	return aINEtcFacil;
}

/**
 * @brief SiNDY����t���O�̃t�B�[���h�̒l���擾
 * @note	�t���O�̓R�[�h�l�̕ϊ����K�v�Ȃ��̂ŋ��ʂ̊֐��ɂ���
 * @param[in]	cpHwyNode	�Ώۃn�C�E�F�C�m�[�h
 * @param[in]	cFieldName	�t�B�[���h��
 * @param[in]	cFieldAlias	�t�B�[���h�G�C���A�X�i�G���[���b�Z�[�W�p�j
 * @return	�擾�����t���O�̒l
 */
bool GetFlag(IFeature* cpHwyNode, std::string cFieldName, std::string cFieldAlias)
{
	bool aFlag = true;
	try
	{
		aFlag = cpHwyNode->GetValue( cpHwyNode->Fields->_FindField(_bstr_t(cFieldName.c_str())) );
	}
	catch (...)
	{
		throw std::runtime_error(cFieldAlias + "�̎擾�Ɏ��s");
	}

	return aFlag;
}


HRESULT SINDY2HWYNODE::Convert(long cFormat)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HWYNODE::Convert";

#ifdef	_DEBUG
	debug = 0;
#endif

	for (GDBOBJITER aIter = mpHwyInfo->eHighwayNode.begin(); aIter != mpHwyInfo->eHighwayNode.end(); aIter++)
	{
#ifdef	_DEBUG
		std::cerr << "Creating hwynode.txt... : " << ++debug << "/" << mpHwyInfo->eHighwayNode.size() << "\r";
#endif
		IFeaturePtr ipHwyNode = aIter->second;
		HighwayNode aHwyNode;

		using namespace file_type;

		bool aTarget = true;
		if (FAILED(hr = isTarget(mpHwyInfo, ipHwyNode, &aTarget)))
		{
			PrintError(ipHwyNode, "�n�C�E�F�C���[�h�t���O�̎擾�Ɏ��s");
			return hr;
		}
		aHwyNode.eFileType = (! aTarget) ? tollnode : hwynode;

		bool aGuideNode = true;
		if (FAILED(hr = isGuideNode(ipHwyNode, &aGuideNode)))
		{
			PrintError(ipHwyNode, "�ē��m�[�h�t���O�̎擾�Ɏ��s");
			return hr;
		}
		if (aGuideNode) aHwyNode.eFileType = tollguide;

		// 2�����b�V���R�[�h
		if (FAILED(hr = GetMeshCode(ipHwyNode, &aHwyNode.eMesh)))
		{
			PrintError(ipHwyNode, "2�����b�V���R�[�h�̎擾�Ɏ��s");
			return hr;
		}

		// �m�[�hID
		if (FAILED(hr = GetNodeID(mpHwyInfo, ipHwyNode, &aHwyNode.eNode)))
		{
			PrintError(ipHwyNode, "�m�[�hID�̎擾�Ɏ��s");
			return hr;
		}

		// �H���R�[�h�E�H�����V�[�P���X�ԍ�
		if (FAILED(hr = mpHwyInfo->GetFacilCode(ipHwyNode, &aHwyNode.eRoadCode, &aHwyNode.eSequence)))
		{
			PrintError(ipHwyNode, "�H���R�[�h�E�H�����V�[�P���X�̎擾�Ɏ��s");
			return hr;
		}

		// �{�݈ʒu�R�[�h
		if (FAILED(hr = GetFacilPos(mpHwyInfo, ipHwyNode, &aHwyNode.eFacilPos)))
		{
			PrintError(ipHwyNode, "�{�݈ʒu�R�[�h�̎擾�Ɏ��s");
			return hr;
		}
		/// �{�݈ʒu�R�[�h���o�����m�[�h�p�̂��̂Ȃ�A�f�[�^�^�C�v���o�����m�[�h�ɕύX
		// TODO: �u�{�݈ʒu�R�[�h���o�����m�[�h�p�ł���v�ƁuTollClass_C���o�����m�[�h�ł���v�͓��l�Ȃ̂ŁA��҂Ŕ��肷�ׂ��B
		switch(aHwyNode.eFacilPos)
		{
			using namespace facil_pos_code;
			case ent_ramp_origin:
			case ext_ramp_end:
			case smartic_ent_ramp_origin:
			case smartic_ext_ramp_end:
				aHwyNode.eFileType = hwynode_inout;
				break;
			default:
				break;
		}

		// �㉺���R�[�h
		if (FAILED(hr = GetUpDown(ipHwyNode, &aHwyNode.eDirection)))
		{
			PrintError(ipHwyNode, "�㉺���R�[�h�̎擾�Ɏ��s");
			return hr;
		}

		// �{�ݎ�ʃR�[�h
		aHwyNode.eFacilClass = GetFacilClass(ipHwyNode, mpHwyInfo);

		// ��������ʃR�[�h
		aHwyNode.eTollClass = GetTollClass(ipHwyNode);

		// �o�����R�[�h
		aHwyNode.eInOut = GetInOut(ipHwyNode);

		// �������@�\�R�[�h�i��ʁj
		GetTollFuncFlags(ipHwyNode, aHwyNode.eTollFuncFlags);

		// ��U���������t���O�i��ʁj
		aHwyNode.eNoGuideTollFlag = GetFlag(ipHwyNode, sindy::highway_node::DUMMYTOLL_F, "��U���������t���O�i��ʁj");

		// ETC�����ʗ����Ή��Œǉ����ꂽ�t�B�[���h�̓ǂݍ���
		switch (cFormat)
		{
			using namespace file_format;
			case hwynode_smartic_added:
				break;
			case hwynode_etc_specific_toll:

				// ��ʎԗ��Ή��t���O
				aHwyNode.eNoEtcCarFlag = GetFlag(ipHwyNode, sindy::highway_node::NOETC_CAR_F, "��ʎԗ��Ή��t���O");

				// ETC�{�݃R�[�h
				aHwyNode.eEtcFacilCode = GetEtcFacil(ipHwyNode);

				// �������@�\�R�[�h(ETC)
				GetEtcTollFuncFlags(ipHwyNode, aHwyNode.eEtcTollFuncFlags);

				// ��U���������t���O�iETC�j
				aHwyNode.eEtcNoGuideTollFlag = GetFlag(ipHwyNode, sindy::highway_node::ETC_NOGUIDETOLL_F, "��U���������t���O�iETC�j");

				break;
			default:
				ATLASSERT(0);
		}

		// �������m�[�h�E���o���m�[�h
		if (FAILED(hr = GetInOutNode(ipHwyNode, &aHwyNode.eAppNode, &aHwyNode.eFlowNode)))
		{
			PrintError(ipHwyNode, "�����E���o�m�[�h�̎擾�Ɏ��s");
			return hr;
		}

		// �{�ݖ���
		if (FAILED(hr = GetName(mpHwyInfo, ipHwyNode, &aHwyNode.eName, &aHwyNode.eYomi)))
		{
			PrintError(ipHwyNode, "�{�ݖ��̂̎擾�Ɏ��s");
			return hr;
		}

		mHighwayNodeArray.push_back(aHwyNode);
	}

#ifdef	_DEBUG
	std::cerr << std::endl;
#endif

	return S_OK;
}


/**
 * @brief tollguide.txt�ɏo�͂��郌�R�[�h��̍쐬
 * @note	�����̂�SINDY2HWYNODE::Write()���番��
 * @return	�쐬�������R�[�h��
 */
std::auto_ptr<TollGuideArray> SINDY2HWYNODE::makeTollGuideArray(void)
{
	std::auto_ptr<TollGuideArray> pTollGuideArray(new TollGuideArray);

	BOOST_FOREACH (const HighwayNode& aHighwayNode, mHighwayNodeArray)
	{
		using namespace file_type;
		if (aHighwayNode.eFileType != tollguide) continue;

		TollGuide aTollGuide;
		aTollGuide.eNode.eMesh = aHighwayNode.eMesh;
		aTollGuide.eNode.eNode = aHighwayNode.eNode;

		// [Bug6316] tollguide�̗������ē��p�m�[�h�|�C���g�ɑΉ��������@�\�R�[�h���i�[���ꂽ��A���̃t���O��true�ɂ���
		static bool aIsStraged = false;

		if (!aIsStraged)
		{
			// �ē��������n�C�E�F�C�m�[�h�ɑΉ������U���������n�C�E�F�C�m�[�h��T���āA�������@�\�R�[�h�̃t���O����R�s�[����
			long aRoadCode = aHighwayNode.eRoadCode;
			long aSequence = aHighwayNode.eSequence;
			long aFacilPos = aHighwayNode.eFacilPos;

			for (unsigned long aIndex = 0; aIndex < mHighwayNodeArray.size(); aIndex++) {
				if( mHighwayNodeArray[aIndex].eFileType != hwynode )			{ continue; }
				if( mHighwayNodeArray[aIndex].eRoadCode != aRoadCode )	{ continue; }
				if( mHighwayNodeArray[aIndex].eSequence != aSequence )	{ continue; }
				if( ! mHighwayNodeArray[aIndex].eNoGuideTollFlag )		{ continue; }	// ��U���������t���O��1�̃n�C�E�F�C�m�[�h���Ώ�
				if( mHighwayNodeArray[aIndex].eFacilPos != aFacilPos )	{ continue; }	// �{�݈ʒu�R�[�h
				
				aTollGuide.eTollFuncFlags = mHighwayNodeArray[aIndex].eTollFuncFlags;
			}
		}
		else {
			aTollGuide.eTollFuncFlags = aHighwayNode.eTollFuncFlags;
		}

		// TollGuideArray�ɓ�����W�̃��R�[�h�����ɂ��邩�`�F�b�N
		TollGuideArray::iterator aIter = pTollGuideArray->find(aTollGuide.eNode);
		if (aIter == pTollGuideArray->end())
		{
			// �Ȃ��ꍇ�́A�V�������R�[�h��ǉ�
			aTollGuide.eFacilCodeArray.insert(std::make_pair(aHighwayNode.eRoadCode,aHighwayNode.eSequence));
			pTollGuideArray->insert(std::make_pair(aTollGuide.eNode, aTollGuide));
		}
		else
		{
			// ����ꍇ�́A�������R�[�h�ɘH���R�[�h�ƘH�����V�[�P���X�̑g��ǉ�
			aIter->second.eFacilCodeArray.insert(std::make_pair(aHighwayNode.eRoadCode, aHighwayNode.eSequence));
		}
	}

	return pTollGuideArray;
}

/**
 * @brief hwynode.txt, hwynode_inout.txt, tollnode.txt�ɋ��ʂ̃t�B�[���h�̏o��
 * @note	�A���A��������ʃR�[�h�̂݁Ahwynode_inout.txt�ɂ͏o�͂��Ȃ�
 * @note	[Bug6309]�Ή�����WriteHwyNode()���番��
 * @param[in]	fout	�t�@�C���o�̓X�g���[��
 * @param[in]	cFormat	�o�̓t�@�C���̃t�H�[�}�b�g
 * @param[in]	crHighwayNode	�o�͑Ώۃf�[�^
 */
void WriteHwyNodeCommonFields(std::ofstream& fout, long cFileType, long cFormat, const HighwayNode& crHighwayNode)
{
	using namespace file_format;
	std::string fd = "\t";	// �t�B�[���h�f���~�^

	// �Q�����b�V���R�[�h
	fout << std::dec << crHighwayNode.eMesh;

	// �m�[�hID
	fout << fd;
	fout << std::dec << crHighwayNode.eNode;

	// �H���R�[�h
	fout << fd;
	fout << setw(6) << setfill('0') << std::dec << crHighwayNode.eRoadCode;

	// �H�����V�[�P���X�ԍ�
	fout << fd;
	fout << std::dec << crHighwayNode.eSequence;

	switch (cFormat)
	{
		case hwynode_smartic_added:
			// �{�݈ʒu�R�[�h
			fout << fd;
			fout << std::dec << crHighwayNode.eFacilPos;

			// �㉺���R�[�h
			fout << fd;
			fout << std::dec << crHighwayNode.eDirection;

			break;

		case hwynode_etc_specific_toll:
			// �㉺���R�[�h
			fout << fd;
			fout << std::dec << crHighwayNode.eDirection;

			// �{�ݎ�ʃR�[�h
			fout << fd;
			fout << std::dec << crHighwayNode.eFacilClass;

			// �o�����R�[�h
			fout << fd;
			fout << std::dec << crHighwayNode.eInOut;

			// ��������ʃR�[�h
			// [Bug6309] hwynode_inout.txt�ɂ���������ʃR�[�h���o�͂���悤�ɕύX
			fout << fd;
			fout << std::dec << crHighwayNode.eTollClass;
			break;

		default:
			ATLASSERT(0);
	}
}

/**
 * @brief �������@�\�R�[�h�̃t���O��i5���j���o��
 * @param[in]	fout	�t�@�C���o�̓X�g���[��
 * @param[in]	crHighwayNode	�o�͑Ώۃf�[�^
 */
void WriteTollFuncFlags(std::ofstream& fout, const HighwayNode& crHighwayNode)
{
	ATLASSERT(crHighwayNode.eTollFuncFlags.size() == 5);
	BOOST_FOREACH (bool aFlag, crHighwayNode.eTollFuncFlags)
		fout << aFlag ? 1 : 0;
}

/**
 * @brief �����֘A�t�B�[���h�̏o��
 * @note	hwynode.txt��tollnode.txt�̋��ʕ���
 * @note	[Bug6309]�Ή�����WriteHwyNode()���番��
 * @param[in]	fout	�t�@�C���o�̓X�g���[��
 * @param[in]	cFormat	�o�̓t�@�C���̃t�H�[�}�b�g
 * @param[in]	crHighwayNode	�o�͑Ώۃf�[�^
 */
void WriteTollFields(std::ofstream& fout, long cFormat, const HighwayNode& crHighwayNode)
{
	using namespace file_format;
	std::string fd = "\t";	// �t�B�[���h�f���~�^

	if (cFormat == hwynode_etc_specific_toll)
	{
		// ��ʎԗ��Ή��t���O
		fout << fd;
		fout << crHighwayNode.eNoEtcCarFlag ? 1 : 0;
	}

	// �������@�\�R�[�h�̃t���O��i5���j�i�u��������ʁv�܂��́u�������@�\�R�[�h�i��ʁj�v�j
	fout << fd;
	WriteTollFuncFlags(fout, crHighwayNode);

	if (cFormat == hwynode_etc_specific_toll)
	{
		// �����ʗ����Ή��t�H�[�}�b�g�ł́A�������@�\�R�[�h�i��ʁj�Ɣ�U���������t���O�i��ʁj��ʃt�B�[���h�ɂ���
		fout << fd;
	}

	// ��U���������t���O�i��ʁj
	fout << crHighwayNode.eNoGuideTollFlag ? 1 : 0;

	if (cFormat == hwynode_etc_specific_toll)
	{
		// ETC�{�݃R�[�h
		fout << fd;
		fout << crHighwayNode.eEtcFacilCode;

		// �������@�\�R�[�h(ETC)�̏�5��
		fout << fd;
		ATLASSERT(crHighwayNode.eEtcTollFuncFlags.size() == 5);
		BOOST_FOREACH (bool aFlag, crHighwayNode.eEtcTollFuncFlags)
			fout << aFlag ? 1 : 0;

		// �������@�\�R�[�h(ETC)�̉�2���i�L���b�V���o�b�N������/���Z���j
		for (int i = 0; i < 2; ++i)
			fout << 0;		// ���'0'���o�͂���

		// ��U���������t���O(ETC)
		fout << fd;
		fout << crHighwayNode.eEtcNoGuideTollFlag ? 1 : 0;
	}
}

/**
 * @brief tollnode.txt��p�t�B�[���h�̏o��
 * @note	[Bug6309]�Ή�����WriteHwyNode()���番��
 * @note	WriteHwyNodeCommonFields(), WriteTollFields()�ƈ���āAfile_format�Ɉˑ����Ȃ�
 * @param[in]	fout	�t�@�C���o�̓X�g���[��
 * @param[in]	crHighwayNode	�o�͑Ώۃf�[�^
 */
void WriteTollNodeFields(std::ofstream& fout, const HighwayNode& crHighwayNode)
{
	std::string fd = "\t";	// �t�B�[���h�f���~�^

	// �����m�[�hID
	fout << fd;
	fout << std::dec << crHighwayNode.eAppNode;

	// ���o�m�[�hID
	fout << fd;
	fout << std::dec << crHighwayNode.eFlowNode;

	// �{�ݖ���
	fout << fd;
	if (crHighwayNode.eName.size())
		fout << SINDY2INCODE::SJIS2EUC(crHighwayNode.eName.c_str());

	// �{�ݖ��̓ǂ�
	fout << fd;
	if (crHighwayNode.eYomi.size())
		fout << SINDY2INCODE::SJIS2EUC(crHighwayNode.eYomi.c_str());
}

/**
 * @brief hwynode.txt�ւ̏o��
 * @param[in]	fout	�t�@�C���o�̓X�g���[��
 * @param[in]	cFormat	�o�̓t�@�C���̃t�H�[�}�b�g
 */
void SINDY2HWYNODE::WriteHwyNode(std::ofstream& fout, long cFormat)
{
	BOOST_FOREACH (const HighwayNode& aHighwayNode, mHighwayNodeArray)
	{
		if (aHighwayNode.eFileType != file_type::hwynode)
			continue;

		WriteHwyNodeCommonFields(fout, aHighwayNode.eFileType, cFormat, aHighwayNode);
		WriteTollFields(fout, cFormat, aHighwayNode);

		fout << std::endl;
	}
}

/**
 * @brief hwynode_inout.txt�ւ̏o��
 * @note	[Bug6309]�Ή�����WriteHwyNode()���番��
 * @param[in]	fout	�t�@�C���o�̓X�g���[��
 * @param[in]	cFormat	�o�̓t�@�C���̃t�H�[�}�b�g
 */
void SINDY2HWYNODE::WriteHwyNodeInOut(std::ofstream& fout, long cFormat)
{
	BOOST_FOREACH (const HighwayNode& aHighwayNode, mHighwayNodeArray)
	{
		if (aHighwayNode.eFileType != file_type::hwynode_inout)
			continue;

		WriteHwyNodeCommonFields(fout, aHighwayNode.eFileType, cFormat, aHighwayNode);

		// �����ʗ����Ή��ȑO��hwynode_inout.txt�ł́A�u��������ʁv���o�͂���
		if (cFormat == file_format::hwynode_smartic_added)
		{
			fout << "\t";
			WriteTollFuncFlags(fout, aHighwayNode);
		}

		fout << std::endl;
	}
}

/**
 * @brief tollnode.txt�ւ̏o��
 * @note	[Bug6309]�Ή�����WriteHwyNode()���番��
 * @param[in]	fout	�t�@�C���o�̓X�g���[��
 * @param[in]	cFormat	�o�̓t�@�C���̃t�H�[�}�b�g
 */
void SINDY2HWYNODE::WriteTollNode(std::ofstream& fout, long cFormat)
{
	BOOST_FOREACH (const HighwayNode& aHighwayNode, mHighwayNodeArray)
	{
		if (aHighwayNode.eFileType != file_type::tollnode)
			continue;

		WriteHwyNodeCommonFields(fout, aHighwayNode.eFileType, cFormat, aHighwayNode);
		WriteTollFields(fout, cFormat, aHighwayNode);
		WriteTollNodeFields(fout, aHighwayNode);

		fout << std::endl;
	}
}


/**
 * @brief tollguide.txt�ւ̏o��
 * @param[in]	fout	�t�@�C���o�̓X�g���[��
 * @param[in]	cFormat	�o�̓t�@�C���̃t�H�[�}�b�g
 */
void SINDY2HWYNODE::WriteTollGuide(std::ofstream& fout, long cFormat)
{
	// tollguide.txt�ɏo�͂��邽�߂̃��R�[�h����쐬
	std::auto_ptr<TollGuideArray> pTollGuideArray = makeTollGuideArray();

	std::string fd = "\t";	// �t�B�[���h�f���~�^

	for (TollGuideArray::iterator aIter = pTollGuideArray->begin(); aIter != pTollGuideArray->end(); aIter++)
	{
		TollGuide aTollGuide = aIter->second;

		// ���b�V���R�[�h
		fout << std::dec << aTollGuide.eNode.eMesh;

		// �m�[�hID
		fout << fd;
		fout << std::dec << aTollGuide.eNode.eNode;

		// �Ή��������@�\�R�[�h�i5���j
		fout << fd;
		BOOST_FOREACH (bool aFlag, aTollGuide.eTollFuncFlags)
		{
			fout << aFlag ? 1 : 0;
		}

		// �Ή��������@�\�R�[�h��6���ځi��U���������t���O�BETC�����ʗ����Ή���̃t�H�[�}�b�g�ł͏o�͂��Ȃ��j
		using namespace file_format;
		switch (cFormat)
		{
			case hwynode_smartic_added:
				fout << 1;					// ���'1'���o�͂���
				break;
			case hwynode_etc_specific_toll:
				break;
			default:
				ATLASSERT(0);
		}

		// �Ή��|�C���g��
		fout << fd;
		fout << std::dec << aTollGuide.eFacilCodeArray.size();

		// �Ή��{�ݏ��̗�
		for (FacilCodeArray::iterator aFacil = aTollGuide.eFacilCodeArray.begin(); aFacil != aTollGuide.eFacilCodeArray.end(); aFacil++)
		{
			fout << fd;
			fout << std::dec << aFacil->first;	// �H���R�[�h

			fout << fd;
			fout << std::dec << aFacil->second;	// �H�����V�[�P���X
		}

		fout << std::endl;
	}
}

/**
 * @brief �t�@�C���ւ̏o��
 * @param[in]	cFileName	�o�̓t�@�C����
 * @param[in]	cFileType	�o�̓t�@�C���̎��
 * @param[in]	cFormat	�o�̓t�@�C���̃t�H�[�}�b�g
 * @return	�K��S_OK��Ԃ��ivoid�ɂ��Ă��悢���AWrite()�̌Ăяo�������C������K�v������̂ŕۗ��j
 */
HRESULT SINDY2HWYNODE::Write(std::string cFileName, long cFileType, long cFormat)
{
	const char *func = "SINDY2HWYNODE::Write";

	std::ofstream fout(cFileName.c_str(), ios::binary);
	if (! fout.is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	switch (cFileType)
	{
		using namespace file_type;
		case tollnode:
			WriteTollNode(fout, cFormat);
			break;
		case tollguide:
			WriteTollGuide(fout, cFormat);
			break;
		case hwynode:
			WriteHwyNode(fout, cFormat);
			break;
		case hwynode_inout:
			WriteHwyNodeInOut(fout, cFormat);
			break;
		default:
			ATLASSERT(0);
	}

	return S_OK;
}

HRESULT SINDY2HWYNODE::GetInOutNode(IFeaturePtr ipHwyNode, long *cAppNode, long *cFlowNode)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HWYNODE::GetInOutNode";

	*cAppNode = *cFlowNode = 0;

	CComVariant vaAppNode, vaFlowNode;
	if (FAILED(hr = GDBGetValue(ipHwyNode, sindy::highway_node::APPROACHNODE_ID, &vaAppNode))) return hr;
	if (FAILED(hr = GDBGetValue(ipHwyNode, sindy::highway_node::FLOWNODE_ID, &vaFlowNode))) return hr;
	vaAppNode.ChangeType(VT_I4); vaFlowNode.ChangeType(VT_I4);

	*cAppNode = vaAppNode.lVal;
	*cFlowNode = vaFlowNode.lVal;

	return S_OK;
}

HRESULT SINDY2HWYNODE::GetName(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, std::string *cName, std::string *cYomi)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HWYNODE::GetName";

	*cName = *cYomi = "";

	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipHwyNode, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);

	CComVariant vaName, vaYomi;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::NAME_KANJI, &vaName))) return hr;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::NAME_YOMI, &vaYomi))) return hr;

	if (vaName.vt != VT_NULL && vaName.vt != VT_EMPTY)
	{
		vaName.ChangeType(VT_BSTR);
		*cName = COLE2T(vaName.bstrVal);
		if (*cName == " " || *cName == "�@") *cName = "";
	}

	if (vaYomi.vt != VT_NULL && vaName.vt != VT_EMPTY)
	{
		vaYomi.ChangeType(VT_BSTR);
		*cYomi = COLE2T(vaYomi.bstrVal);
		if (*cYomi == " " || *cYomi == "�@") *cYomi = "";
	}

	return S_OK;
}

HRESULT SINDY2HWYNODE::GetUpDown(IFeaturePtr ipHwyNode, long *cUpDown)
{
	const char *func = "SINDY2HWYNODE::GetUpDown";

	*cUpDown = 0;

	long aSindyDirection = ipHwyNode->GetValue( ipHwyNode->Fields->_FindField(_bstr_t(sindy::highway_node::DIRECTION_C.c_str())) );
	*cUpDown = Sindy2INCode(in::schema::hwynode::direction::tDirection, aSindyDirection);

	return S_OK;
}

HRESULT SINDY2HWYNODE::GetFacilPos(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, long *cFacilPos)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HWYNODE::GetFacilPos";

	*cFacilPos = 0;

#ifdef	_DEBUG
	long aOID = ipHwyNode->GetOID();
#endif

	// ���������
	CComVariant vaTollClass;
	if (FAILED(hr = GDBGetValue(ipHwyNode, sindy::highway_node::TOLLCLASS_C, &vaTollClass))) return hr;
	vaTollClass.ChangeType(VT_I4);

	// �o���������i����E�����R�[�h�j
	CComVariant vaInOut;
	if (FAILED(hr = GDBGetValue(ipHwyNode, sindy::highway_node::INOUT_C, &vaInOut))) return hr;
	vaInOut.ChangeType(VT_I4);

	using namespace facil_pos_code;
	/// ���򍇗��m�[�h�̏ꍇ
	if (vaTollClass.lVal == sindy::highway_node::tollclass_other)
	{
		IFeaturePtr ipFacilInfoPoint = NULL;
		if (FAILED(hr = cHwyInfo->GetFacilPoint(ipHwyNode, &ipFacilInfoPoint))) return hr;
		if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);

		CComVariant vaFacilClass;
		if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::FACILCLASS_C, &vaFacilClass))) return hr;
		vaFacilClass.ChangeType(VT_I4);

		switch (vaFacilClass.lVal)
		{
			case sindy::facil_info_point::facilclass_sa:
				switch (vaInOut.lVal)
				{
				case sindy::inout::inout_enter:	*cFacilPos = sa_ent; break;
				case sindy::inout::inout_exit:	*cFacilPos = sa_ext; break;
				default: return GDBERROR2(E_UNEXPECTED, func);
				}
				break;
			case sindy::facil_info_point::facilclass_pa:
				switch (vaInOut.lVal)
				{
				case sindy::inout::inout_enter:	*cFacilPos = pa_ent; break;
				case sindy::inout::inout_exit:	*cFacilPos = pa_ext; break;
				default: return GDBERROR2(E_UNEXPECTED, func);
				}
				break;
			case sindy::facil_info_point::facilclass_jct:
				switch (vaInOut.lVal)
				{
				case sindy::inout::inout_enter:	*cFacilPos = jct_ent; break;
				case sindy::inout::inout_exit:	*cFacilPos = jct_ext; break;
				default: return GDBERROR2(E_UNEXPECTED, func);
				}
				break;
			case sindy::facil_info_point::facilclass_ramp:
			case sindy::facil_info_point::facilclass_ic:
				switch (vaInOut.lVal)
				{
				case sindy::inout::inout_enter:	*cFacilPos = ic_ent; break;
				case sindy::inout::inout_exit:	*cFacilPos = ic_ext; break;
				default: return GDBERROR2(E_UNEXPECTED, func);
				}
				break;
			case sindy::facil_info_point::facilclass_smartic:
				switch (vaInOut.lVal)
				{
				case sindy::inout::inout_enter:	*cFacilPos = smartic_ent; break;
				case sindy::inout::inout_exit:	*cFacilPos = smartic_ext; break;
				default: return GDBERROR2(E_UNEXPECTED, func);
				}
				break;

			default: return GDBERROR2(E_UNEXPECTED, func);
		}
	}
	/// �o�����m�[�h�̏ꍇ
	else if (vaTollClass.lVal == sindy::highway_node::tollclass_inout)
	{
		IFeaturePtr ipFacilInfoPoint;
		if (FAILED(hr = cHwyInfo->GetFacilPoint(ipHwyNode, &ipFacilInfoPoint))) return hr;
		if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);

		CComVariant vaFacilClass;
		if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::FACILCLASS_C, &vaFacilClass))) return hr;
		vaFacilClass.ChangeType(VT_I4);

		if (vaFacilClass.lVal == sindy::facil_info_point::facilclass_smartic)
		{
			switch (vaInOut.lVal)
			{
			case sindy::inout::inout_enter:				*cFacilPos = smartic_ent_ramp_origin; break;
			case sindy::inout::inout_exit:				*cFacilPos = smartic_ext_ramp_end; break;
			default: return GDBERROR2(E_UNEXPECTED, func);
			}
		}
		else
		{
			switch (vaInOut.lVal)
			{
			case sindy::inout::inout_enter:				*cFacilPos = ent_ramp_origin; break;
			case sindy::inout::inout_exit:				*cFacilPos = ext_ramp_end; break;
			default: return GDBERROR2(E_UNEXPECTED, func);
			}
		}
	}
	else
	{
		switch (vaTollClass.lVal)
		{
		case sindy::highway_node::tollclass_honsen:		*cFacilPos = line_tollgate; break;
		case sindy::highway_node::tollclass_ramp:
			switch (vaInOut.lVal)
			{
			case sindy::inout::inout_enter:				*cFacilPos = ramp_ent_tollgate; break;
			case sindy::inout::inout_exit:				*cFacilPos = ramp_ext_tollgate; break;
			default: return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		case sindy::highway_node::tollclass_barrier:	*cFacilPos = barrier; break;
		default: return GDBERROR2(E_UNEXPECTED, func);
		}
	}

	return S_OK;
}

HRESULT SINDY2HWYNODE::GetNodeID(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, long *cNodeID)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HWYNODE::GetNodeID";

	*cNodeID = 0;

	IFeaturePtr ipNode;
	if (FAILED(hr = cHwyInfo->GetHwyNode(ipHwyNode, &ipNode))) return hr;
	if (ipNode == NULL)
		return GDBERROR2(E_UNEXPECTED, func);

	if (FAILED(hr = ipNode->get_OID(cNodeID))) return GDBERROR(hr, func);

	return S_OK;
}

HRESULT SINDY2HWYNODE::GetMeshCode(IFeaturePtr ipHwyNode, long *cMesh)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HWYNODE::GetMeshCode";

	*cMesh = 0;

	IGeometryPtr ipGeometry;
	if (FAILED(hr = ipHwyNode->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);
	long aX = 0, aY = 0;
	return GDBGetBaseMeshNormalizeCoord((IPointPtr)ipGeometry, cMesh, &aX, &aY);
}

HRESULT SINDY2HWYNODE::isTarget(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, bool *cTarget)
{
	return cHwyInfo->isHwyMode(ipHwyNode, cTarget);
}

HRESULT SINDY2HWYNODE::isGuideNode(IFeaturePtr ipHwyNode, bool *cGuide)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2HWYNODE::isGuideNode";

	*cGuide = false;

	CComVariant vaGuide;
	if (FAILED(hr = GDBGetValue(ipHwyNode, sindy::highway_node::GUIDE_F, &vaGuide))) return hr;
	vaGuide.ChangeType(VT_I4);

	if (vaGuide.lVal) *cGuide = true;

	return S_OK;
}
