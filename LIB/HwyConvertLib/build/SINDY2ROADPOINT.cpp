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

// SINDY2ROADPOINT.cpp: SINDY2ROADPOINT �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/foreach.hpp>
#include "SINDY2ROADPOINT.h"
#include "SINDY2INCODE.h"
#include "SINDY2HIGHWAY.h"
#include "SINDY2HWYNODE.h"
#include "NvUnit.h"
#include "INHwySchema.h"
#include "SindyFindHwyNode.h"
#include "INHwyCommonUtility.h"
#include "SindyFindMainLineFromJCTInterflow.h"
#include "SindyHighwayTools.h"

using namespace in::schema::roadpoint;

RoadPoint::RoadPoint() :
ePrefMesh(0),
ePrefX(0),
ePrefY(0),
eAccessPointID(0),
eMesh(0),
eX(0),
eY(0),
eAdmin(0),
eFacilInfoPointID(0),
eSeq(0),
eSearchClass(0),
eName(""),
eYomi(""),
eRoadCode(0),
eFacilClassAndInOutCode(0),
eFacilClass(0),
eInOut(0),
ePntName(""),
ePntYomi(""),
eSapaInfo(),
eOriginal(0),
eDummyFacil(0),
ePrefMeshForSirius(0),
ePrefXForSirius(0),
ePrefYForSirius(0),
eDirection(0)
{
}

/**
 * @brief �t�����̃A�N�Z�X�|�C���g���擾����
 * @note	ipAccessPoint��Road_Code, Road_Seq, InOut_C�������ŁADirection_C���㉺�t�̃A�N�Z�X�|�C���g���擾����B
 * @param[in]	cHwyInfo		HWY���
 * @param[in]	ipAccessPoint	�ΏۃA�N�Z�X�|�C���g
 * @param[out]	ipAccessPoint2	�t�����̃A�N�Z�X�|�C���g��Ԃ��o�b�t�@
 * @return	����
 */
static HRESULT FindRevdirAccessPoint(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, IFeature **ipAccessPoint2)
{
	HRESULT hr = S_OK;
	const char *func = "FindRevdirAccessPoint";

	*ipAccessPoint2 = NULL;

	long aRoadCode, aRoadSeq;
	if (FAILED(hr = cHwyInfo->GetFacilCode(ipAccessPoint, &aRoadCode, &aRoadSeq))) return hr;

	CComVariant vaInOut;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::INOUT_C, &vaInOut))) return hr;
	vaInOut.ChangeType(VT_I4);

	CComVariant vaDirection;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::DIRECTION_C, &vaDirection))) return hr;
	vaDirection.ChangeType(VT_I4);

	IGeometryPtr ipGeometry;
	if (FAILED(hr = ipAccessPoint->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);

	GDBPNTRANGE aRange;
	if (FAILED(hr = cHwyInfo->eAccessPoint.SearchByPoint((IPointPtr)ipGeometry, &aRange))) return hr;
	if (aRange.first == aRange.second) return S_OK;

	for (GDBPNTITER aIter = aRange.first; aIter != aRange.second; aIter++)
	{
		IFeaturePtr ipFeature = aIter->second;

		long aRoadCode2 = 0, aRoadSeq2 = 0;
		if (FAILED(hr = cHwyInfo->GetFacilCode(ipFeature, &aRoadCode2, &aRoadSeq2))) return hr;

		if (aRoadCode != aRoadCode2 || aRoadSeq != aRoadSeq2) continue;

		CComVariant vaInOut2;
		if (FAILED(hr = GDBGetValue(ipFeature, sindy::access_point::INOUT_C, &vaInOut2))) return hr;
		vaInOut2.ChangeType(VT_I4);

		if (vaInOut.lVal != vaInOut2.lVal) continue;

		CComVariant vaDirection2;
		if (FAILED(hr = GDBGetValue(ipFeature, sindy::access_point::DIRECTION_C, &vaDirection2))) return hr;
		vaDirection2.ChangeType(VT_I4);

		if ((vaDirection.lVal == sindy::access_point::direction_up && vaDirection2.lVal == sindy::access_point::direction_down) ||
			(vaDirection.lVal == sindy::access_point::direction_down && vaDirection2.lVal == sindy::access_point::direction_up))
		{
			(*ipAccessPoint2) = ipFeature;	// �t�����A�N�Z�X�|�C���g�͂����P�������݂����Ȃ��iHC1017,1040�j�̂ŁA�P�����������_�ŏI��
			(*ipAccessPoint2)->AddRef();
			break;
		}
	}

	return S_OK;
}

/**
 * @brief �|�C���g�ʒu�R�[�h�̎擾�i�A���AIC/�����v�̃A�N�Z�X�|�C���g�̏d�Ȃ�͍l�����Ȃ��j
 * @param[in]	cHwyInfo		HWY���
 * @param[in]	ipAccessPoint	�ΏۃA�N�Z�X�|�C���g
 * @param[out]	cLineType		�|�C���g�ʒu�R�[�h��Ԃ��o�b�t�@
 * @return	����
 */
static HRESULT GetLineType(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cLineType)
{
	HRESULT hr = S_OK;
	const char *func = "GetLineType";

	// �H���R�[�h�̎擾
	CComVariant vaRoadCode;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::ROAD_CODE, &vaRoadCode))) return hr;
	vaRoadCode.ChangeType(VT_I4);

	// Direction_C���擾
	CComVariant vaDirection;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::DIRECTION_C, &vaDirection))) return hr;
	vaDirection.ChangeType(VT_I4);

	// Road_Code_List��Row���擾
	_IRowPtr ipRoadCode;
	if (FAILED(cHwyInfo->eRoadCodeList.SearchByRoadCode(vaRoadCode.lVal, &ipRoadCode))) return hr;
	if (ipRoadCode == NULL)
		return GDBERROR2(E_UNEXPECTED, func);

	// ROAD_CODE_LIST::LINEDIRTYPE_C�̎擾
	CComVariant vaLineDirType;
	if (FAILED(hr = GDBGetValue(ipRoadCode, sindy::road_code_list::LINEDIRTYPE_C, &vaLineDirType))) return hr;
	vaLineDirType.ChangeType(VT_I4);

	// DIRECTION_C��LINEDIRTYPE_C����ALineType�����߂�
	switch (vaDirection.lVal)
	{
		case sindy::direction::direction_no:					*cLineType = point_pos_code::direction; break;
		case sindy::direction::direction_up:
			switch (vaLineDirType.lVal)
			{
			case sindy::road_code_list::linedirtype_updown:		*cLineType = point_pos_code::up; break;
			case sindy::road_code_list::linedirtype_inout:		*cLineType = point_pos_code::in; break;
			case sindy::road_code_list::linedirtype_eastwest:	*cLineType = point_pos_code::west; break;
			default: return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		case sindy::direction::direction_down:
			switch (vaLineDirType.lVal)
			{
			case sindy::road_code_list::linedirtype_updown:		*cLineType = point_pos_code::down; break;
			case sindy::road_code_list::linedirtype_inout:		*cLineType = point_pos_code::out; break;
			case sindy::road_code_list::linedirtype_eastwest:	*cLineType = point_pos_code::east; break;
			default: return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		case sindy::direction::direction_common:				*cLineType = point_pos_code::entrance; break;
		default: return GDBERROR2(E_UNEXPECTED, func);
	}

	return S_OK;
}


static void AdjustMeshCode(ZMeshPos *cMeshPos)
{
	if (cMeshPos->eX < 0)
	{
		while (1)
		{
			cMeshPos->eX += 1000000;
			cMeshPos->eMesh = ZMap::GetMovedMesh(cMeshPos->eMesh, -1, 0);
			if (0 <= cMeshPos->eX && cMeshPos->eX <= 1000000) break;
		}
	}
	else if (cMeshPos->eX > 1000000)
	{
		while (1)
		{
			cMeshPos->eX -= 1000000;
			cMeshPos->eMesh = ZMap::GetMovedMesh(cMeshPos->eMesh, 1, 0);
			if (0 <= cMeshPos->eX && cMeshPos->eX <= 1000000) break;
		}
	}

	if (cMeshPos->eY < 0)
	{
		while (1)
		{
			cMeshPos->eY += 1000000;
			cMeshPos->eMesh = ZMap::GetMovedMesh(cMeshPos->eMesh, 0, -1);
			if (0 <= cMeshPos->eY && cMeshPos->eY <= 1000000) break;
		}
	}
	else if (cMeshPos->eY > 1000000)
	{
		while (1)
		{
			cMeshPos->eY -= 1000000;
			cMeshPos->eMesh = ZMap::GetMovedMesh(cMeshPos->eMesh, 0, 1);
			if (0 <= cMeshPos->eY && cMeshPos->eY <= 1000000) break;
		}
	}

	return;
}

namespace {

/**
 * @brief I/N�̏o�����R�[�h�̎擾
 * @param[in]	cpAccessPoint	�Ώ�AccessPoint�I�u�W�F�N�g
 * @return	I/N�̏o�����R�[�h
 */
long GetInOut(IFeature* cpAccessPoint)
{
	long aSindyInOut = cpAccessPoint->GetValue( cpAccessPoint->Fields->_FindField(_bstr_t(sindy::access_point::INOUT_C.c_str())) );
	return Sindy2INCode(in::schema::roadpoint::inout::tInOut, aSindyInOut);
}

/**
 * @brief �����N�W������㉺����ʃR�[�h�����肷��
 * @note	�㉺����������������u�㉺�����ʁv�Ƃ���
 * @param[in]	cLinkList	�����N�W��
 * @return	���߂�ꂽ�㉺����ʃR�[�h�i���߂��Ȃ��ꍇ��NULL�j
 */
std::auto_ptr<long> DecideDirectionByLinks( const GDBOBJLIST& cLinkList )
{
	bool aIsRoadLinkFound = false;
	long aINDirection = in::schema::fclpoint::direction::nodir;
	for (GDBOBJITER aLinkIt = cLinkList.begin(); aLinkIt != cLinkList.end(); ++aLinkIt)
	{
		const IFeaturePtr& ipLink = aLinkIt->second;
#ifdef	_DEBUG
		long aLinkID = ipLink->OID;
#endif
		long aRoadLinkUpDownClassFieldIndex = ipLink->Fields->_FindField(_bstr_t(sindy::road_link::UPDOWNCLASS_C.c_str()));
		long aUpDownClass = ipLink->GetValue(aRoadLinkUpDownClassFieldIndex);
		switch (aUpDownClass)
		{
			using namespace sindy::road_link;
			using namespace in::schema::fclpoint::direction;
			case updownclass_up:
				if (aINDirection == nodir) { aINDirection = up; }
				if (aINDirection == down)  { aINDirection = common; }
				aIsRoadLinkFound = true;
				break;
			case updownclass_down:
				if (aINDirection == nodir) { aINDirection = down; }
				if (aINDirection == up)    { aINDirection = common; }
				aIsRoadLinkFound = true;
				break;
			case updownclass_nodir:
			case updownclass_nothwy:	// ��ʗL�����H�ł�UpDownClass_c���u�ΏۊO�v�Ȃ̂ŁA���̏ꍇ�́u�����Ȃ��v�ɂ���
				aIsRoadLinkFound = true;
				break;
			case updownclass_common:
				aINDirection = in::schema::fclpoint::direction::common;
				aIsRoadLinkFound = true;
				break;
			case updownclass_etcramp:
			case updownclass_etcaccess:
				ATLASSERT(0);
				break;
			default:
				ATLASSERT(0);
		}
	}

	return aIsRoadLinkFound ? std::auto_ptr<long>(new long(aINDirection)) : std::auto_ptr<long>(NULL);
}

/**
 * @brief �����N�̏W������㉺����ʃR�[�h�����߂�
 * @note	�^����ꂽ�t�B�[�`���ɐڑ����郊���N�̏W���̒��ɗ^����ꂽ�H���R�[�h���������N���܂܂�Ă�����A
 *			���̃����N��UpDownClass_c���狁�߂���㉺����ʃR�[�h��Ԃ��B
 * @note	[Bug9550] ������Ԃ̃����N��L����ԂƓ��l�Ɉ����悤�ɏC���B
 * @param[in]	cHwyInfo	Road_Link�t�B�[�`���N���X�擾�p��SINDY2INHWYINFO
 * @param[in]	cpFeature	�Ώۃt�B�[�`���iHighway_Node�܂���Access_Point)
 * @param[in]	cRoadCode	�����N�W���̂��������ꂩ�̃����N�������Ă���Ɗ��҂����H���R�[�h
 * @return	���߂�ꂽ�㉺����ʃR�[�h�i��L�̂悤�ȃ����N�������炸�A���߂��Ȃ������ꍇ��NULL�j
 */
std::auto_ptr<long> DecideDirectionByConnectedLinks(SINDY2INHWYINFO *cHwyInfo, IFeature* cpFeature, long cRoadCode)
{
#ifdef _DEBUG
	long aOID = cpFeature->GetOID();
#endif
	HRESULT hr = S_OK;
	using namespace sindy_highway_tools;

	// �Ή��m�[�h�̎擾
	IFeaturePtr ipNode;
	if (FAILED(hr = cHwyInfo->GetHwyNode(cpFeature, &ipNode)))
		throw std::runtime_error("�Ή��m�[�h�̎擾�Ɏ��s");
	if (ipNode == NULL)
		throw std::runtime_error("�Ή�����m�[�h�����݂��Ȃ�");

	// �ڑ������N�̎擾
	GDBOBJLIST aLinkList;
	if (FAILED(hr = cHwyInfo->GetLink(ipNode, sindy::rnw::relation::fromto, &aLinkList)))
		throw std::runtime_error("�ڑ������N�̎擾�Ɏ��s");

	IFeatureClassPtr ipRoadLinkFeatureClass(cHwyInfo->eRoadLink.GetFeatureClass());
	static long aRoadLinkRoadCodeFieldIndex = ipRoadLinkFeatureClass->Fields->_FindField(_bstr_t(sindy::road_link::ROAD_CODE.c_str()));

	// �ڑ������N�̒�����A�^����ꂽ�H���R�[�h�������̂����o��
	GDBOBJLIST aTargetLinkList;
	for (GDBOBJITER aLinkIt = aLinkList.begin(); aLinkIt != aLinkList.end(); ++aLinkIt)
	{
		const _IRowPtr& ipLink = aLinkIt->second;
		long aLinkID = ipLink->OID;

		long aRoadLinkRoadCode = ipLink->GetValue(aRoadLinkRoadCodeFieldIndex);
		if (aRoadLinkRoadCode == cRoadCode || aRoadLinkRoadCode == cRoadCode + FREEZONE_ADDITIONAL_ROADCODE )		// [Bug9550] ������Ԃ̃����N���L����ԂƓ��l�Ɉ���
		{
			aTargetLinkList.insert( std::make_pair( aLinkID, ipLink ) );
		}
	}

	// �����N�W������㉺����ʃR�[�h�����߂�
	return DecideDirectionByLinks( aTargetLinkList );
}

/**
 * @brief �㉺����ʃR�[�h�̍X�V
 * @note	�u�����̃I�u�W�F�N�g����㉺����ʃR�[�h�����肷��v�Ƃ��������ŁA
			�P�̃I�u�W�F�N�g����㉺����ʃR�[�h�����߂�x�ɑS�̂̏㉺����ʃR�[�h���X�V���鏈�������̊֐��ōs��
 * @param[in]		cNewDirection		�V���Ɍ�������SiNDY��Direction_C
 * @param[in/out]	cAllDirection		�S�̂̏㉺����ʃR�[�h�i�X�V�Ώہj
 */
void updateINDirectionCode( long cNewDirection, long& cAllDirection )
{
	switch (cNewDirection)
	{
		using namespace in::schema::fclpoint::direction;
		case nodir:
			break;
		case up:
			if		(cAllDirection == nodir)	{ cAllDirection = up; }
			else if (cAllDirection == down)		{ cAllDirection = common; }
			break;
		case down:
			if		(cAllDirection == nodir)	{ cAllDirection = down; }
			else if	(cAllDirection == up)		{ cAllDirection = common; }
			break;
		case common:
			cAllDirection = common;
			break;
		default:
			ATLASSERT(0);
	}
}

/**
 * @brief JCT�̑��H���ւ̍����_�ɂ���A�N�Z�X�|�C���g�̏㉺����ʃR�[�h�����߂�
 * @param[in]	cpAccessPoint	�ΏۃA�N�Z�X�|�C���g
 * @param[in]	cHwyInfo	�ǐՃN���XSindyFindHwyNode�ɓn�����߂�SINDY2INHWYINFO
 * @return	���߂�ꂽ�㉺����ʃR�[�h
 */
long DirectionOfInterflowAP( IFeature* cpAccessPoint, SINDY2INHWYINFO *cHwyInfo )
{
	HRESULT hr = S_OK;
#ifdef	_DEBUG
	long aOID = cpAccessPoint->GetOID();
#endif

	// �Ή��m�[�h�̎擾
	IFeaturePtr ipNode;
	if (FAILED(hr = cHwyInfo->GetHwyNode(cpAccessPoint, &ipNode)))
		{ throw std::runtime_error("�Ή��m�[�h�̎擾�Ɏ��s"); }
	if (ipNode == NULL)
		{ throw std::runtime_error("�Ή�����m�[�h�����݂��Ȃ�"); }

	// �ǐՂ̐ݒ�
	SindyFindHwyNode aFindHwyNode(cHwyInfo);
	static long aRoadCodeFieldIndex = cpAccessPoint->Fields->_FindField(_bstr_t(sindy::access_point::ROAD_CODE.c_str()));
	long aRoadCode = cpAccessPoint->GetValue( aRoadCodeFieldIndex );
	static long aRoadSeqFieldIndex = cpAccessPoint->Fields->_FindField(_bstr_t(sindy::access_point::ROAD_SEQ.c_str()));
	long aRoadSeq  = cpAccessPoint->GetValue( aRoadSeqFieldIndex );
	aFindHwyNode.set_facil_code( aRoadCode, aRoadSeq );

	// �ǐ�
	bool aReachiability = true;
	if (FAILED(hr = SindyRoadLinkTrace(cHwyInfo, ipNode, &aFindHwyNode,  &aReachiability)))
	{
		ATLASSERT(0);
	}
	const HwyNodeArray aFoundHwyNodeArray = aFindHwyNode.getFoundHwyNodeArray();

	long aINDirection = in::schema::fclpoint::direction::nodir;
	if (aFoundHwyNodeArray.size() != 0)
	{
		// �e�n�C�E�F�C�m�[�h�ɂ��ď㉺����ʃR�[�h�����肵�A�㉺����������������u�㉺�����ʁv�ɂ���
		BOOST_FOREACH (IFeaturePtr ipHwyNode, aFoundHwyNodeArray)
		{
			// �ڑ������N����㉺����ʃR�[�h�����߂�
			std::auto_ptr<long> pDirection = DecideDirectionByConnectedLinks(cHwyInfo, ipHwyNode, aRoadCode);
			if (! pDirection.get())		// [Bug9550] NULL�̏ꍇ�ɗ����Ă����̂ŁA�����Ȃ��悤�ɏC���B
				{ continue; }
			updateINDirectionCode( *pDirection, aINDirection );
		}
	}
	// [Bug7303] �n�C�E�F�C�m�[�h��������Ȃ��ꍇ�͎�@��ς���
	else
	{
		// �ڑ������N�̎擾
		GDBOBJLIST aLinkList;
		if (FAILED(hr = cHwyInfo->GetLink(ipNode, sindy::rnw::relation::fromto, &aLinkList)))
			{ throw std::runtime_error("�ڑ������N�̎擾�Ɏ��s"); }

		// �ڑ������N�Q������0�ȊO�̘H���R�[�h��S�Ď擾
		typedef long RoadCode;
		std::set<RoadCode> aRoadCodes;
		BOOST_FOREACH ( const GDBOBJLIST::value_type& aElement, aLinkList )
		{
			_IRowPtr ipConnectedLink = aElement.second;
			long aLinkID = ipConnectedLink->OID;

			static long aRoadLinkRoadCodeFieldIndex = ipConnectedLink->Fields->_FindField( _bstr_t(sindy::road_link::ROAD_CODE.c_str()) );
			long aConnectedLinkRoadCode = ipConnectedLink->GetValue( aRoadLinkRoadCodeFieldIndex );
			if ( aConnectedLinkRoadCode != 0 )
			{
				aRoadCodes.insert( aConnectedLinkRoadCode );
			}
		}

		// �H���R�[�h�̐��Ɠ����񐔕������A�A�N�Z�X�|�C���g�̈ʒu����ǐՂ����s���A�ړI�̃����N������������aAllFoundLinks�Ɋi�[
		GDBOBJLIST aAllFoundLinks;
		BOOST_FOREACH ( long aConnectedLinkRoadCode, aRoadCodes )
		{
			SindyFindMainLineFromJCTInterflow aSindyFindMainLineFromJCTInterflow( aRoadCode, aConnectedLinkRoadCode );
			aSindyFindMainLineFromJCTInterflow.set_direction(sindy::rnw::relation::back);

			bool aReachiability = true;
			if (FAILED(hr = SindyRoadLinkTrace(cHwyInfo, ipNode, &aSindyFindMainLineFromJCTInterflow, &aReachiability)))
			{
				ATLASSERT(0);
			}

			const GDBOBJLIST& aFoundLinks = aSindyFindMainLineFromJCTInterflow.getFoundLinks();
			std::copy( aFoundLinks.begin(), aFoundLinks.end(), std::inserter( aAllFoundLinks, aAllFoundLinks.begin() ) );
		}

		// �����N�W������㉺����ʃR�[�h�����߂�
		std::auto_ptr<long> pDirection = DecideDirectionByLinks( aAllFoundLinks );
		if (pDirection.get())
		{
			aINDirection = *pDirection;
		}
	}

	return aINDirection;
}

/**
 * @brief fclpoint.txt�p�̏㉺����ʃR�[�h�����߂�
 * @note	[Bug6428]�Ή��̂��ߒǉ�
 * @param[in]	cpAccessPoint	�Ώ�AccessPoint�I�u�W�F�N�g
 * @param[in]	cHwyInfo	Facil_Info_Point�擾�p��SINDY2INHWYINFO
 * @param[in]	cPntPos		�|�C���g�ʒu�R�[�h
 * @return	���߂�ꂽ�㉺����ʃR�[�h
 * @note	���s�����ꍇ��std::runtime_error��O�𑗏o����
 */
long GetDirection(IFeature* cpAccessPoint, SINDY2INHWYINFO *cHwyInfo, long cPntPos)
{
#ifdef _DEBUG
	long aOID = cpAccessPoint->GetOID();
	std::cout << "\t" << "Access_Point" << "\t" << aOID << "\t" << "GetDirection()" << std::endl;
#endif
	HRESULT hr = S_OK;

	// �A�N�Z�X�|�C���g��������{�݂̎{�ݎ�ʃR�[�h�̎擾
	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(cpAccessPoint, &ipFacilInfoPoint)))
		throw std::runtime_error("Access_Point��������{�݂�Facil_Info_Point�̎擾�Ɏ��s");
	long aFacilClass = ipFacilInfoPoint->GetValue( ipFacilInfoPoint->Fields->_FindField(_bstr_t(sindy::facil_info_point::FACILCLASS_C.c_str())) );

	// �n�C�E�F�C���[�h�t���O�̎擾
	bool aHwyModeFlag = (cpAccessPoint->GetValue( cpAccessPoint->Fields->_FindField(_bstr_t(sindy::access_point::HWYMODE_F.c_str())) ).lVal != 0);
	long aINDirection = 0;
	// ������{�݂�JCT�ȊO�ł��邩�A�܂��̓A�N�Z�X�|�C���g���n�C�E�F�C���[�h�ΏۊO�̏ꍇ
	if (aFacilClass != sindy::facil_info_point::facilclass_jct || ! aHwyModeFlag)
	{
		// �A�N�Z�X�|�C���g��Direction_C�ɂ��㉺����ʃR�[�h�����肷��
		static long aDirectionCFieldIndex = cpAccessPoint->Fields->_FindField(_bstr_t(sindy::access_point::DIRECTION_C.c_str()));
		long aSindyDirection = cpAccessPoint->GetValue( aDirectionCFieldIndex );

		// �|�C���g�ʒu�R�[�h���u�����v�������́u�o���v�A�Ȃ����A�@������{�݂�IC,�����v,�X�}�[�gIC�̏ꍇ�̂�
		// �d�Ȃ��Ă���A�N�Z�X�|�C���g�����݂��邱�Ƃ��l�����ď㉺����ʃR�[�h�����肷��
		if (cPntPos == point_pos_code::entrance)
		{
			IFeaturePtr ipAccessPoint2;
			switch (aFacilClass)
			{
				case sindy::facil_info_point::facilclass_ramp:
				case sindy::facil_info_point::facilclass_ic:
				case sindy::facil_info_point::facilclass_smartic:
					if (FAILED(hr = FindRevdirAccessPoint(cHwyInfo, cpAccessPoint, &ipAccessPoint2))) return hr;
						break;
				default:
					break;
			}
			if (ipAccessPoint2 != NULL)
				aSindyDirection = sindy::direction::direction_common;
		}
		aINDirection = Sindy2INCode(in::schema::fclpoint::direction::tDirection, aSindyDirection);
	}
	// �A�N�Z�X�|�C���g���n�C�E�F�C���[�h�ΏۂŁA��������{�݂�JCT�ł���ꍇ
	else
	{
		long aAccessPointRoadCode = cpAccessPoint->GetValue( cpAccessPoint->Fields->_FindField(_bstr_t(sindy::access_point::ROAD_CODE.c_str())) );

		// �ڑ������N�̒��ɁA�H���R�[�h���A�N�Z�X�|�C���g�Ɠ����������N���܂܂�Ă�����A
		// ���ڒ��̃A�N�Z�X�|�C���g�͂��ꂪ������{����ɂ�����̂ƌ��Ȃ��A���̃����N��UpDownClass_c�ŏ㉺����ʃR�[�h�����肷��
		std::auto_ptr<long> pDirection = DecideDirectionByConnectedLinks( cHwyInfo, cpAccessPoint, aAccessPointRoadCode );
		if (pDirection.get())
		{
			aINDirection = *pDirection;
		}
		// ��L�̂悤�ȃ����N��������Ȃ���΁A���ڒ��̃A�N�Z�X�|�C���g�͂��ꂪ������{����ɂȂ����́i�����A���H������̍����_�ɂ�����́j�ƌ��Ȃ��A
		// �n��������N���t�ɒH���Ė{�������N��T���A���̃����N��UpDownClass_c�ŏ㉺����ʃR�[�h�����肷��
		else
		{
			aINDirection = DirectionOfInterflowAP( cpAccessPoint, cHwyInfo );
		}
	}

	// SiNDY�̐����d�l���画�f����ƁA�㉺����ʃR�[�h���u�����Ȃ��v�ɂȂ邱�Ƃ͂Ȃ��͂��Ȃ̂ŁA�Ȃ�����G���[���b�Z�[�W���o���Ă���
	if (aINDirection == in::schema::fclpoint::direction::nodir)
	{
		PrintError(cpAccessPoint, "�㉺����ʃR�[�h���u�����Ȃ��v");
	}

	return aINDirection;
}

}	// anonymous namespace


SINDY2ROADPOINT::SINDY2ROADPOINT(SINDY2INHWYINFO *cHwyInfo) :
SINDY2INHWY(cHwyInfo)
{
}

SINDY2ROADPOINT::~SINDY2ROADPOINT()
{
}


HRESULT SINDY2ROADPOINT::Convert()
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::Convert";

	typedef std::set<long> OIDArray;
	OIDArray aNotForOutputIDs;		// �o�͑ΏۊO�I�u�W�F�N�gID��

#ifdef	_DEBUG
	long count = 0;
#endif
	for (GDBOBJITER aIter = mpHwyInfo->eAccessPoint.begin(); aIter != mpHwyInfo->eAccessPoint.end(); aIter++)
	{
#ifdef	_DEBUG
		std::cerr << "Creating RoadPoint..." << ++count << "/" << mpHwyInfo->eAccessPoint.size() << "\r";
#endif
		IFeaturePtr ipAccessPoint = aIter->second;

		// �o�͑ΏۊO�I�u�W�F�N�gID��Ɋ܂܂�Ă�����X�L�b�v
		long aOID = ipAccessPoint->GetOID();
		OIDArray::iterator aIDIt = aNotForOutputIDs.find(aOID);
		if (aIDIt != aNotForOutputIDs.end())
			continue;

		RoadPoint aRoadPoint;

		// HWYMODE�Ώۂ��ǂ������m�F����
		if (FAILED(hr = GetHwyModeFlag(mpHwyInfo, ipAccessPoint, &aRoadPoint.eHwyModeFlag)))
		{
			PrintError(ipAccessPoint, "�n�C�E�F�C���[�h�t���O�̎擾�Ɏ��s");
			return hr;
		}

		// �{�ݑ�\�_���擾����
		if (FAILED(hr = GetPrefPoint(mpHwyInfo, ipAccessPoint, &aRoadPoint.ePrefMesh, &aRoadPoint.ePrefX, &aRoadPoint.ePrefY)))
		{
			PrintError(ipAccessPoint, "�{�ݑ�\�_�̎擾�Ɏ��s");
			return hr;
		}

		// SiRiUS�p�f�[�^�̎{�ݑ�\�_���擾����
		if (FAILED(hr = GetPrefPointForSirius(mpHwyInfo, ipAccessPoint, &aRoadPoint.ePrefMeshForSirius, &aRoadPoint.ePrefXForSirius, &aRoadPoint.ePrefYForSirius)))
		{
			PrintError(ipAccessPoint, "SiRiUS�p�f�[�^�̐ݑ�\�_�̎擾�Ɏ��s");
			return hr;
		}

		// ���b�V�����V�[�P���X�̎擾
		if (FAILED(hr = GetMeshSequence(ipAccessPoint, &aRoadPoint.eAccessPointID)))
		{
			PrintError(ipAccessPoint, "���b�V�����V�[�P���X�̎擾�Ɏ��s");
			return hr;
		}

		// 2�����b�V���R�[�h�E���K�����W
		if (FAILED(hr = GetBaseMeshCoord(ipAccessPoint, &aRoadPoint.eMesh, &aRoadPoint.eX, &aRoadPoint.eY)))
		{
			PrintError(ipAccessPoint, "���K�����W�̎擾�Ɏ��s");
			return hr;
		}

		// �s�撬���R�[�h�̎擾
		if (FAILED(hr = GetBaseAdmin(mpHwyInfo, ipAccessPoint, &aRoadPoint.eAdmin)))
		{
			PrintError(ipAccessPoint, "�s�撬���R�[�h�̎擾�Ɏ��s");
			return hr;
		}

		// �{�ݑ�\�|�C���g�̎s�撬���R�[�h�̎擾
		if (FAILED(hr = GetFacilInfoPointAdmin(mpHwyInfo, ipAccessPoint, &aRoadPoint.eFacilInfoPointAdmin)))
		{
			PrintError(ipAccessPoint, "�{�ݑ�\�|�C���g�̎s�撬���R�[�h�̎擾�Ɏ��s");
			return hr;
		}

		// �{�ݑ�\�|�C���g��ObjectID�̎擾
		if (FAILED(hr = GetFacilInfoPointID(mpHwyInfo, ipAccessPoint, &aRoadPoint.eFacilInfoPointID)))
		{
			PrintError(ipAccessPoint, "�{�ݑ�\�|�C���g��ObjectID�̎擾�Ɏ��s");
			return hr;
		}

		// �H�����V�[�P���X�ԍ��̎擾
		if (FAILED(hr = GetSequence(ipAccessPoint, &aRoadPoint.eSeq)))
		{
			PrintError(ipAccessPoint, "�H�����V�[�P���X�̎擾�Ɏ��s");
			return hr;
		}

		// �����p�{�ݎ�ʃR�[�h�̎擾
		if (FAILED(hr = GetSearchClass(mpHwyInfo, ipAccessPoint, &aRoadPoint.eSearchClass)))
		{
			PrintError(ipAccessPoint, "�����p�{�ݎ�ʃR�[�h�̎擾�Ɏ��s");
			return hr;
		}

		// �H���R�[�h�̎擾
		if (FAILED(hr = GetRoadCode(mpHwyInfo, ipAccessPoint, &aRoadPoint.eRoadCode)))
		{
			PrintError(ipAccessPoint, "�H���R�[�h�̎擾�Ɏ��s");
			return hr;
		}

		// �{�ݎ�ʁ{�o�����R�[�h�̎擾
		if (FAILED(hr = GetFacilClassAndInOutCode(mpHwyInfo, ipAccessPoint, &aRoadPoint.eFacilClassAndInOutCode)))
		{
			PrintError(ipAccessPoint, "�{�ݎ�ʁ{�o�����R�[�h�̎擾�Ɏ��s");
			return hr;
		}

		// �{�ݎ�ʃR�[�h
		aRoadPoint.eFacilClass = GetFacilClass(ipAccessPoint, mpHwyInfo);

		// �o�����R�[�h
		aRoadPoint.eInOut = GetInOut(ipAccessPoint);

		// �|�C���g�ʒu�R�[�h�̎擾
		if (FAILED(hr = GetPntPos(mpHwyInfo, ipAccessPoint, &aRoadPoint.ePntPos, &aNotForOutputIDs)))
		{
			PrintError(ipAccessPoint, "�|�C���g�ʒu�R�[�h�̎擾�Ɏ��s");
			return hr;
		}

		// �|�C���g�ʒu���́A�|�C���g�ʒu�ǂ݂̎擾
		if (FAILED(hr = GetPntName(ipAccessPoint, aRoadPoint.ePntPos, &aRoadPoint.ePntName, &aRoadPoint.ePntYomi)))
		{
			PrintError(ipAccessPoint, "�|�C���g�ʒu���̂̎擾�Ɏ��s");
			return hr;
		}

		// �{�ݖ��́{�ǂ�
		if (FAILED(hr = GetName(mpHwyInfo, ipAccessPoint, &aRoadPoint.eName, &aRoadPoint.eYomi)))
		{
			PrintError(ipAccessPoint, "�{�ݖ��̂̎擾�Ɏ��s");
			return hr;
		}

		// SA/PA���
		if (FAILED(hr = GetSapaInfo(ipAccessPoint, &aRoadPoint.eSapaInfo)))
		{
			PrintError(ipAccessPoint, "SA/PA���̎擾�Ɏ��s");
			return hr;
		}

		// �Ǝ����̃t���O
		if (FAILED(hr = GetOriginalFlag(mpHwyInfo, ipAccessPoint, &aRoadPoint.eOriginal)))
		{
			PrintError(ipAccessPoint, "�Ǝ����̃t���O�̎擾�Ɏ��s");
			return hr;
		}

		// �_�~�[�{�ݏ��t���O
		if (FAILED(hr =GetDummyFacilFlag(mpHwyInfo, ipAccessPoint, &aRoadPoint.eDummyFacil)))
		{
			PrintError(ipAccessPoint, "�_�~�[�{�ݏ��t���O�̎擾�Ɏ��s");
			return hr;
		}

		// �㉺����ʃR�[�h
		try {
			aRoadPoint.eDirection = GetDirection(ipAccessPoint, mpHwyInfo, aRoadPoint.ePntPos);
		}
		catch (...)
		{
			PrintError(ipAccessPoint, "�㉺����ʃR�[�h�̎擾�Ɏ��s");
			return E_FAIL;
		}

		mRoadPointArray.insert(aRoadPoint);
	}

	return S_OK;
}

HRESULT SINDY2ROADPOINT::Write(std::string cFileName, bool cHwyModeFlag, long cFormat, long cJctDataType)
{
	HRESULT hr = S_OK;

	RoadPointArray* pData;
	if (cJctDataType == jct_data_type::jct_branch)	{ pData = &mRoadPointBranchArray; }
	else											{ pData = &mRoadPointArray; }

	switch(cFormat)
	{
		using namespace file_format;
		case roadpoint_smartic_added:
		case roadpoint_etc_specific_toll:
			hr = Write(pData, cFileName, cHwyModeFlag, cFormat);
			break;
		case fclrep:
		case fclpoint:
			hr = WriteForSirius(pData, cFileName, cFormat);
			break;
		default:
			ATLASSERT(0);
	}

	return hr;
}

HRESULT SINDY2ROADPOINT::Write(RoadPointArray* cpData, std::string cFileName, bool cHwyModeFlag, long cFormat)
{
	const char *func = "SINDY2ROADPOINT::Write";

	using namespace file_format;
	ATLASSERT(cFormat != fclrep && cFormat != fclpoint);

	std::ofstream aFile(cFileName.c_str(), ios::binary);
	if (! aFile.is_open()) return GDBERROR2(E_UNEXPECTED, func);

	BOOST_FOREACH (const RoadPoint& aRoadPoint, *cpData)
	{
		if (cHwyModeFlag != aRoadPoint.eHwyModeFlag)
			continue;

		// ��\�|�C���g2�����b�V���R�[�h
		aFile << std::dec << aRoadPoint.ePrefMesh;

		// ��\�|�C���g���K��X���W
		aFile << "\t";
		aFile << std::dec << aRoadPoint.ePrefX;

		// ��\�|�C���g���K��Y���W
		aFile << "\t";
		aFile << std::dec << aRoadPoint.ePrefY;

		// ���b�V�����V�[�P���X�ԍ�
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eAccessPointID;

		// �{�݃|�C���g2�����b�V���R�[�h
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eMesh;

		// �{�݃|�C���g���K��X���W
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eX;

		// �{�݃|�C���g���K��Y���W
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eY;

		// �s�撬���R�[�h
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eAdmin;

		switch(cFormat)
		{
			case roadpoint_etc_specific_toll:

				// �H���R�[�h
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eRoadCode;
		}

		// �H�����V�[�P���X�ԍ�
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eSeq;

		// ���H�{�ݖ���
		aFile << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.eName.c_str());

		// ���H�{�ݖ��̓ǂ�
		aFile << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.eYomi.c_str());

		switch(cFormat)
		{
			case roadpoint_smartic_added:

				// �H���R�[�h
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eRoadCode;

				// �{�ݎ�ʃR�[�h
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eFacilClassAndInOutCode;

				break;

			case roadpoint_etc_specific_toll:

				// �{�ݎ�ʃR�[�h
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eFacilClass;

				// �o�����R�[�h
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eInOut;

				break;

			default:
				ATLASSERT(0);
		}

		// �|�C���g�ʒu�R�[�h
		aFile << "\t";
		aFile << std::dec << aRoadPoint.ePntPos;

		// �|�C���g�ʒu���́��ǂ�
		aFile << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.ePntName.c_str());
		aFile << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.ePntYomi.c_str());


		// SA/PA���
		aFile << "\t";
		for (unsigned long j = 0; j < aRoadPoint.eSapaInfo.size(); j++)
			aFile << std::dec << aRoadPoint.eSapaInfo[j];

		// �Ǝ����̃t���O
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eOriginal;

		// �_�~�[�{�ݏ��t���O
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eDummyFacil;

		aFile << std::endl;
	}

	return S_OK;
}

HRESULT SINDY2ROADPOINT::WriteForSirius(RoadPointArray* cpData, std::string cFileName, long cFormat)
{
	const char *func = "SINDY2ROADPOINT::WriteForSirius";

	std::ofstream aFile(cFileName.c_str(), ios::binary);
	if (! aFile.is_open()) return GDBERROR2(E_UNEXPECTED, func);

	std::pair<long, long> aPrevFacilCode;

	BOOST_FOREACH (const RoadPoint& aRoadPoint, *cpData)
	{
		// [Bug6456] ��ʗL�����H�̏����o�͂���悤�ɏC��

		using namespace file_format;

		if (cFormat == fclrep)
		{
			// fclrep.txt�ł́AFacilCode���P�O�Ɠ����Ȃ�X�L�b�v
			std::pair<long, long> aCurrentFacilCode = std::make_pair(aRoadPoint.eRoadCode, aRoadPoint.eSeq);
			if (aPrevFacilCode == aCurrentFacilCode) { continue; }
			aPrevFacilCode = aCurrentFacilCode;
		}

		switch(cFormat)
		{
			case fclrep:
				// ��\�|�C���g2�����b�V���R�[�h
				aFile << std::dec << aRoadPoint.ePrefMeshForSirius;

				// ��\�|�C���g���K��X���W
				aFile << "\t";
				aFile << std::dec << aRoadPoint.ePrefXForSirius;

				// ��\�|�C���g���K��Y���W
				aFile << "\t";
				aFile << std::dec << aRoadPoint.ePrefYForSirius;

				// �{�ݑ�\�|�C���g�̓s���{���R�[�h
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eFacilInfoPointAdmin / 1000;
				// �{�ݑ�\�|�C���g�̎s�撬���R�[�h
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eFacilInfoPointAdmin % 1000;
				break;

			case fclpoint:
				// �{�݃|�C���g2�����b�V���R�[�h
				aFile << std::dec << aRoadPoint.eMesh;

				// �{�݃|�C���g���K��X���W
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eX;

				// �{�݃|�C���g���K��Y���W
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eY;

				// �s���{���R�[�h
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eAdmin / 1000;
				// �s�撬���R�[�h
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eAdmin % 1000;
				break;

			default:
				ATLASSERT(0);
		}


		// �Ǘ��c�̃R�[�h
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eRoadCode / 10000;

		// �H���V�[�P���X�ԍ�
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eRoadCode % 10000;

		// �H�����V�[�P���X�ԍ�
		// [Bug6309]�̂��łɃt�H�[�}�b�g�ύX �H���R�[�h�̎��Ɉړ�
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eSeq;

		if (cFormat == fclpoint)
		{
			// �㉺����ʃR�[�h
			aFile << "\t";
			aFile << std::dec << aRoadPoint.eDirection;
		}

		// �{��ID
		aFile << "\t";
		aFile << std::dec << aRoadPoint.eFacilInfoPointID;

		if (cFormat == fclpoint)
		{
			// �{�݃|�C���gID
			aFile << "\t";
			aFile << std::dec << aRoadPoint.eAccessPointID;

			// �_�~�[�{�ݏ��t���O
			aFile << "\t";
			aFile << std::dec << aRoadPoint.eDummyFacil;
		}
		
		switch(cFormat)
		{
			case fclrep:
				// �����p�{�ݎ�ʃR�[�h
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eSearchClass;

				// ���H�{�ݖ���
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.eName.c_str());

				// �{�ݖ��̓ǂ�
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.eYomi.c_str());

				break;

			case fclpoint:
				// �{�݃|�C���g��ʃR�[�h
				aFile << "\t";
				aFile << std::dec << aRoadPoint.eFacilClassAndInOutCode;

				// �|�C���g�ʒu�R�[�h
				aFile << "\t";
				aFile << std::dec << aRoadPoint.ePntPos;

				// �|�C���g�ʒu���́��ǂ�
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.ePntName.c_str());
				aFile << "\t";
				aFile << SINDY2INCODE::SJIS2EUC(aRoadPoint.ePntYomi.c_str());
				
				// SA/PA���
				aFile << "\t";
				for (unsigned long j = 0; j < aRoadPoint.eSapaInfo.size(); j++)
					aFile << std::dec << aRoadPoint.eSapaInfo[j];

				break;

			default:
				ATLASSERT(0);
		}

		aFile << std::endl;
	}

	return S_OK;
}

/**
 * @brief �e�X�g�p �i�[���ꂽ�l��S�ďo�͂���
 * @param[in]	cFileName	�o�̓t�@�C����
 */
HRESULT SINDY2ROADPOINT::WriteAll(std::string cFileName)
{
	const char *func = "SINDY2ROADPOINT::WriteAll";

	std::ofstream aFile(cFileName.c_str(), ios::binary);
	if (! aFile.is_open()) return GDBERROR2(E_UNEXPECTED, func);

	for (RoadPointArray::const_iterator aIt = mRoadPointArray.begin(); aIt != mRoadPointArray.end(); ++aIt)
	{
		aFile << aIt->eHwyModeFlag << "\t";
		aFile << aIt->ePrefMesh << "\t";
		aFile << aIt->ePrefX << "\t";
		aFile << aIt->ePrefY << "\t";
		aFile << aIt->eAccessPointID << "\t";
		aFile << aIt->eMesh << "\t";
		aFile << aIt->eX << "\t";
		aFile << aIt->eY << "\t";
		aFile << aIt->eAdmin << "\t";
		aFile << aIt->eFacilInfoPointAdmin << "\t";
		aFile << aIt->eFacilInfoPointID << "\t";
		aFile << aIt->eSeq << "\t";
		aFile << aIt->eSearchClass << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aIt->eName.c_str()) << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aIt->eYomi.c_str()) << "\t";
		aFile << aIt->eRoadCode << "\t";
		aFile << aIt->eFacilClassAndInOutCode << "\t";
		aFile << aIt->ePntPos << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aIt->ePntName.c_str()) << "\t";
		aFile << SINDY2INCODE::SJIS2EUC(aIt->ePntYomi.c_str()) << "\t";
		for (unsigned long i=0; i<aIt->eSapaInfo.size(); ++i) { aFile << aIt->eSapaInfo[i]; } aFile << "\t";
		aFile << aIt->eOriginal << "\t";
		aFile << aIt->eDummyFacil << "\t";
		aFile << aIt->ePrefMeshForSirius << "\t";
		aFile << aIt->ePrefXForSirius << "\t";
		aFile << aIt->ePrefYForSirius << "\t";
		aFile << std::endl;
	}
	
	return S_OK;
}

/**
 * @brief �����_�̃��R�[�h�𕪊�_�̃��R�[�h�ɒu���������f�[�^���쐬����
 * @note	roadpoint.bunnki.txt, fclpoint.bunnki.txt�p
 */
HRESULT SINDY2ROADPOINT::makeJctBranchData(void)
{
	HRESULT hr = S_OK;

	typedef std::pair<long, long> FacilCode;
	std::set<FacilCode> aCheckedFacil;	// �`�F�b�N�ς�FacilCode

	using namespace in::schema::roadpoint;

	// ���ڂ����A�N�Z�X�|�C���g�Ɠ���FacilCode�����S�Ă̕���n�C�E�F�C�m�[�h���擾
	for (RoadPointArray::const_iterator aIt = mRoadPointArray.begin(); aIt != mRoadPointArray.end(); ++aIt)
	{
		// [Bug6456] ��ʗL�����H�̃��R�[�h�ł�����f�[�^�����悤�ɏC��

		if (aIt->eFacilClassAndInOutCode == facil_class_and_inout_code::jct) 
		{
			// �`�F�b�N�ς݂�FacilCode�Ȃ�X�L�b�v
			FacilCode aFacilCode = std::make_pair(aIt->eRoadCode, aIt->eSeq);
			if (aCheckedFacil.find(aFacilCode) != aCheckedFacil.end()) { continue; }
			aCheckedFacil.insert(aFacilCode);

			// ����FacilCode�����n�C�E�F�C�m�[�h�W���̎擾
			GDBHWYFACILRANGE aRange;
			mpHwyInfo->eHighwayNode.SearchByFacilCode(aFacilCode.first, aFacilCode.second, &aRange);

			// ���ڒ��̃��R�[�h�̍��W���A���̎{�݂̕���n�C�E�F�C�m�[�h�̍��W�ɒu��������
			for (GDBHWYFACILITER aHwyNodeIt = aRange.first; aHwyNodeIt != aRange.second; ++aHwyNodeIt)
			{
				// �{�݈ʒu�R�[�h�̎擾
				long aFacilPos;
				if (FAILED(hr = SINDY2HWYNODE::GetFacilPos(mpHwyInfo, aHwyNodeIt->second, &aFacilPos)))
				{
					PrintError(aHwyNodeIt->second, "�{�݈ʒu�R�[�h�̎擾�Ɏ��s");
					continue;
				}

				// JCT�o�H�łȂ���΃X�L�b�v
				if (aFacilPos != in::schema::hwynode::facil_pos_code::jct_ext) { continue; }

				// �n�C�E�F�C�m�[�h�̐��K�����W�̎擾
				long aMesh = 0, aX = 0, aY = 0;
				if (FAILED(hr = GetBaseMeshCoord(aHwyNodeIt->second, &aMesh, &aX, &aY)))
				{
					PrintError(aHwyNodeIt->second, "���K�����W�̎擾�Ɏ��s");
					continue;
				}

				RoadPoint aRoadPoint = *aIt;
				aRoadPoint.eMesh = aMesh;
				aRoadPoint.eX	 = aX;
				aRoadPoint.eY	 = aY;

				mRoadPointBranchArray.insert(aRoadPoint);
			}
		}
		else
		{
			mRoadPointBranchArray.insert(*aIt);
		}

	}

	aCheckedFacil.clear();
	RoadPointArray aData;

	for (RoadPointArray::const_iterator aIt1 = mRoadPointBranchArray.begin(); aIt1 != mRoadPointBranchArray.end(); ++aIt1)
	{

		// JCT�ȊO�Ȃ�X�L�b�v
		if (aIt1->eFacilClassAndInOutCode != facil_class_and_inout_code::jct) {
			continue;
		}

		NvUnit aUnit1;		// ���j�b�g�R�[�h
		aUnit1.eMesh = aIt1->eMesh;
		aUnit1.eX = 0;
		aUnit1.eY = 0;
		aUnit1.eLevel = 2;

		// �`�F�b�N�ς݂�FacilCode�Ȃ�X�L�b�v
		FacilCode aFacilCode = std::make_pair(aIt1->eRoadCode, aIt1->eSeq);
		if (aCheckedFacil.find(aFacilCode) != aCheckedFacil.end()) { continue; }
		aCheckedFacil.insert(aFacilCode);

		for (RoadPointArray::const_iterator aIt2 = mRoadPointBranchArray.begin(); aIt2 != mRoadPointBranchArray.end(); ++aIt2)
		{
			using namespace in::schema::roadpoint;
			if (aIt1 == aIt2)														{ continue; }
			if (aIt2->eFacilClassAndInOutCode != facil_class_and_inout_code::jct)	{ continue; }
			if (aIt1->eName != aIt2->eName)											{ continue; }
			if (aIt1->eRoadCode == aIt2->eRoadCode && aIt1->eSeq == aIt2->eSeq)		{ continue; }

			NvUnit aUnit2;
			aUnit2.eMesh = aIt2->eMesh;
			aUnit2.eX = 0;
			aUnit2.eY = 0;
			aUnit2.eLevel = 2;

			ZPoint aDiff = aUnit1.getDiff(aUnit2);

			int aDistance = int(aDiff.eX * aDiff.eX + aDiff.eY * aDiff.eY);
			if (aDistance > 2) { continue; }

			RoadPoint aRoadPoint = *aIt1;
			aRoadPoint.eMesh = aIt2->eMesh;
			aRoadPoint.eX = aIt2->eX;
			aRoadPoint.eY = aIt2->eY;
			
			aData.insert(aRoadPoint);
		}
	}

	for (RoadPointArray::const_iterator aIt = aData.begin(); aIt != aData.end(); ++aIt)
	{
		mRoadPointBranchArray.insert(*aIt);
	}

	return S_OK;
}


/**
 * @brief �{�ݖ��̕ϊ��i���������p�֐��j
 * @note	�^����ꂽRoadPointArray�̎{�ݖ��̂�ϊ�����
 * @param	cpFacilNameChangeTable	[in]	�{�ݖ��̕ϊ��e�[�u��
 * @param	crData	[in/out]	�ϊ��Ώۃf�[�^
 * @param	cFileName	[in]	�ϊ��Ώۃf�[�^�ɑΉ�����t�@�C�����i���O�o�͂̂��ߎw��j
 **/
void SINDY2ROADPOINT::changeFacilName(FacilNameChangeTable* cpFacilNameChangeTable, RoadPointArray& crData, std::string cFileName)
{
	std::vector<RoadPointArray::iterator>	aEraseRecordArray;	// �ϊ��O�́i�폜���ׂ��j���R�[�h�̗�
	std::vector<RoadPoint>		aChangedRoadPointArray;			// �ϊ���́i�ǉ����ׂ��j���R�[�h�̗�

	for (RoadPointArray::iterator aRPIt = crData.begin(); aRPIt != crData.end(); ++aRPIt)
	{
		std::auto_ptr<ChangedFacilName> pChangedFacilName = cpFacilNameChangeTable->getChangedFacilName(
			aRPIt->eRoadCode, aRPIt->eSeq, aRPIt->eName
		);
		if (pChangedFacilName.get() == NULL)
		{
			continue;	// �ϊ��Ώۃ��R�[�h�łȂ���΃X�L�b�v
		}
#ifdef	_DEBUG
		std::cout << cFileName << "\t" << "�{�ݖ��̕ϊ�" << "\t" << aRPIt->eRoadCode << "\t" << aRPIt->eSeq << "\t";
		std::cout << aRPIt->eName << "��" << pChangedFacilName->m_Kanji << "\t";
		std::cout << aRPIt->eYomi << "��" << pChangedFacilName->m_Yomi << std::endl;
#endif
		// �{�ݖ��̕ϊ�
		RoadPoint aRoadPoint = *aRPIt;
		aRoadPoint.eName = pChangedFacilName->m_Kanji;
		aRoadPoint.eYomi = pChangedFacilName->m_Yomi;

		// �폜���ׂ����R�[�h�ƒǉ����ׂ����R�[�h���L�^
		aEraseRecordArray.push_back(aRPIt);
		aChangedRoadPointArray.push_back(aRoadPoint);
	}

	// �폜
	for (std::vector<RoadPointArray::iterator>::iterator aIt = aEraseRecordArray.begin(); aIt != aEraseRecordArray.end(); ++aIt)
	{
		crData.erase(*aIt);
	}
	// �ǉ�
	for (std::vector<RoadPoint>::iterator aIt = aChangedRoadPointArray.begin(); aIt != aChangedRoadPointArray.end(); ++aIt)
	{
		crData.insert(*aIt);
	}
}

void SINDY2ROADPOINT::changeFacilName(Customer cCustomer, FacilNameChangeTable* cpFacilNameChangeTable)
{
	if (! needFacilNameChange( cCustomer ) )
		{ return; }

	changeFacilName( cpFacilNameChangeTable );
}

void SINDY2ROADPOINT::changeFacilName( FacilNameChangeTable* cpFacilNameChangeTable )
{
	changeFacilName( cpFacilNameChangeTable, mRoadPointArray, "roadpoint.txt" );
	changeFacilName( cpFacilNameChangeTable, mRoadPointBranchArray, "roadpoint.bunnki.txt" );
}





HRESULT SINDY2ROADPOINT::GetFacilInfoPointID(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cID)
{
	HRESULT hr = S_OK;
	*cID = 0;

	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;

	long aOID;
	if (FAILED(hr = ipFacilInfoPoint->get_OID(&aOID))) return hr;
	*cID = aOID;

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetSearchClass(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cSearchClass)
{
	HRESULT hr = S_OK;
	*cSearchClass = 0;

	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;

	long aSindySearchClass = ipFacilInfoPoint->GetValue( ipFacilInfoPoint->Fields->_FindField(_bstr_t(sindy::facil_info_point::SEARCHCLASS_C.c_str())) );
	*cSearchClass = Sindy2INCode(in::schema::fclrep::searchclass::tSearchClass, aSindySearchClass);

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetRoadCode(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cRoadCode)
{
	HRESULT hr = S_OK;
	*cRoadCode = 0;

	CComVariant vaRoadCode;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::ROAD_CODE, &vaRoadCode))) return hr;

	*cRoadCode = vaRoadCode.lVal;

	return S_OK;
}

/// GetFacilPosInfo()���番��
HRESULT SINDY2ROADPOINT::GetFacilClassAndInOutCode(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cFacilClass)
{
	HRESULT hr = S_OK;
	*cFacilClass = 0;
	const char *func = "SINDY2ROADPOINT::GetFacilClassAndInOutCode";

	// �o����������̎擾
	CComVariant vaInOut;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::INOUT_C, &vaInOut))) return hr;
	vaInOut.ChangeType(VT_I4);

	// SiNDY�̎{�ݎ�ʃR�[�h�̎擾
	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);
	CComVariant vaFacilClass;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::FACILCLASS_C, &vaFacilClass))) return hr;
	vaFacilClass.ChangeType(VT_I4);

	bool hwymode = true;
	if (FAILED(hr = cHwyInfo->isHwyMode(ipAccessPoint, &hwymode))) return hr;

	long aClass = 0;
	switch (vaFacilClass.lVal)
	{
		case sindy::facil_info_point::facilclass_sa:
			aClass = facil_class_and_inout_code::sa;
			break;
		case sindy::facil_info_point::facilclass_pa:
			aClass = facil_class_and_inout_code::pa;
			break;
		case sindy::facil_info_point::facilclass_jct:
			{
				if (hwymode)
				{
					aClass = facil_class_and_inout_code::jct;
				}
				else	/// roadpoint_normal.txt�ł́AJCT�̓��H�Əo�H����ʂ���
				{
					switch (vaInOut.lVal)
					{
						case sindy::access_point::inout_enter:
							aClass = facil_class_and_inout_code::jct_ent;
							break;
						case sindy::access_point::inout_exit:
							aClass = facil_class_and_inout_code::jct_ext;
							break;
						default:
							return GDBERROR2(E_UNEXPECTED, func);
					}
				}
			}
			break;
		case sindy::facil_info_point::facilclass_ramp:
			switch (vaInOut.lVal)
			{
				case sindy::access_point::inout_enter:
					aClass = facil_class_and_inout_code::ramp_ent;
					break;
				case sindy::access_point::inout_exit:
					aClass = facil_class_and_inout_code::ramp_ext;
					break;
				default:
					return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		case sindy::facil_info_point::facilclass_ic:
			switch (vaInOut.lVal)
			{
			case sindy::access_point::inout_enter:
				aClass = facil_class_and_inout_code::ic_ent;
				break;
			case sindy::access_point::inout_exit:
				aClass = facil_class_and_inout_code::ic_ext;
				break;
			default:
				return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		case sindy::facil_info_point::facilclass_tollgate:
			aClass = facil_class_and_inout_code::tollgate;
			break;
		case sindy::facil_info_point::facilclass_smartic:
			switch (vaInOut.lVal)
			{
				case sindy::access_point::inout_enter:
					aClass = facil_class_and_inout_code::smartic_ent;
					break;
				case sindy::access_point::inout_exit:
					aClass = facil_class_and_inout_code::smartic_ext;
					break;
				default:
					return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		default:
			return GDBERROR2(E_UNEXPECTED, func);
	}

	*cFacilClass = aClass;

	return S_OK;
}


HRESULT SINDY2ROADPOINT::GetPntPos(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cPntPos, std::set<long> *cNotForOutputIDs)
{
	HRESULT hr = S_OK;
	*cPntPos = 0;
	const char *func = "SINDY2ROADPOINT::GetPntPos";

	// SiNDY�̎{�ݎ�ʃR�[�h�̎擾
	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);
	CComVariant vaFacilClass;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::FACILCLASS_C, &vaFacilClass))) return hr;
	vaFacilClass.ChangeType(VT_I4);

	// �{�ݎ�ʂ�IC�A�����v�A�X�}�[�gIC�̏ꍇ�̂݁A���ʒu�ɋt�����̃A�N�Z�X�|�C���g������Ύ擾
	IFeaturePtr ipAccessPoint2;
	switch(vaFacilClass.lVal)
	{
		case sindy::facil_info_point::facilclass_ramp:
		case sindy::facil_info_point::facilclass_ic:
		case sindy::facil_info_point::facilclass_smartic:
			if (FAILED(hr = FindRevdirAccessPoint(cHwyInfo, ipAccessPoint, &ipAccessPoint2))) return hr;
			break;
		default:
			break;
	}

	// �A�N�Z�X�|�C���g�̏d�Ȃ���l�����Ȃ��ꍇ�̃|�C���g�ʒu�R�[�h������
	long aLineType = 0;
	if (FAILED(hr = GetLineType(cHwyInfo, ipAccessPoint, &aLineType))) return hr;

	/// IC�A�����v�A�X�}�[�gIC�̃A�N�Z�X�|�C���g���d�Ȃ��Ă��āA�������t�ł����entrance�i�����������͏o���j�Ƃ���
	//  ���㉺����ʃR�[�h�擾(GetDirection())���A�����Ō��肵���|�C���g�ʒu�R�[�h(�����������͏o��)���g�p���Ă���B
	//    �����ł̃|�C���g�ʒu�R�[�h���u�����������͏o���v�ɂ�������ɕύX���������ꍇ�AGetDirection�֐����������K�v������B
	if (ipAccessPoint2 != NULL)
	{
		long aLineType2 = 0;
		if (FAILED(hr = GetLineType(cHwyInfo, ipAccessPoint2, &aLineType2))) return hr;
		if ((aLineType == point_pos_code::up	&& aLineType2 == point_pos_code::down) ||
			(aLineType == point_pos_code::down	&& aLineType2 == point_pos_code::up	 ) ||
			(aLineType == point_pos_code::in	&& aLineType2 == point_pos_code::out ) ||
			(aLineType == point_pos_code::out	&& aLineType2 == point_pos_code::in  ) ||
			(aLineType == point_pos_code::west  && aLineType2 == point_pos_code::east) ||
			(aLineType == point_pos_code::east  && aLineType2 == point_pos_code::west))
		{
			aLineType = point_pos_code::entrance;
			long aOID = 0;
			if (FAILED(hr = ipAccessPoint2->get_OID(&aOID))) return GDBERROR(hr, func);
			if (cNotForOutputIDs != NULL)
			{
#ifdef	_DEBUG
				std::cout << "\t" << "Access_Point" << "\t" << aOID << "\t" << "�o�͑ΏۊO" << std::endl;
#endif
				cNotForOutputIDs->insert(aOID);
			}
		}
		else
		{
			ATLASSERT(0);	// ipAccessPoint2 != NULL�̎��_�ŏ����𖞂����̂ŁA�����ɗ��邱�Ƃ͂Ȃ��͂�
		}
	}

	*cPntPos = aLineType;

	return S_OK;
}


// �|�C���g�ʒu�R�[�h����A�|�C���g�ʒu���̂Ɠǂ݂����߂�
HRESULT SINDY2ROADPOINT::GetPntName(IFeaturePtr ipAccessPoint, long cPntPos, std::string *cName, std::string *cYomi)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetPntName";

	// �o���������
	CComVariant vaInOut;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::INOUT_C, &vaInOut))) return hr;
	vaInOut.ChangeType(VT_I4);

	switch (cPntPos)
	{
		case point_pos_code::entrance:
			switch (vaInOut.lVal)
			{
				case sindy::inout::inout_enter:		*cName = "����"; *cYomi = "�C���O�`"; break;
				case sindy::inout::inout_exit:		*cName = "�o��"; *cYomi = "�f�O�`"; break;
				default: return GDBERROR2(E_UNEXPECTED, func);
			}
			break;
		case point_pos_code::up:	*cName = "���"; *cYomi = "�m�{��"; break;
		case point_pos_code::down:	*cName = "����"; *cYomi = "�N�_��"; break;
		case point_pos_code::in:		*cName = "�����"; *cYomi = "�E�`�}����"; break;
		case point_pos_code::out:	*cName = "�O���"; *cYomi = "�\�g�}����"; break;
		case point_pos_code::west:	*cName = "���s��"; *cYomi = "�j�V�C�L"; break;
		case point_pos_code::east:	*cName = "���s��"; *cYomi = "�q�K�V�C�L"; break;
		case point_pos_code::direction:
			{
				CComVariant vaName, vaYomi;
				if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::DIRECTION_KANJI, &vaName))) return hr;
				if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::DIRECTION_YOMI, &vaYomi))) return hr;
				if (vaName.vt != VT_NULL && vaName.vt != VT_EMPTY)
				{
					vaName.ChangeType(VT_BSTR);
					*cName = COLE2T(vaName.bstrVal);
					if (*cName == " " || *cName == "�@")
						std::cout << SindyErrorInfo(ipAccessPoint, "���ʖ��̂������Ă��Ȃ�");
				}
				if (vaYomi.vt != VT_NULL && vaYomi.vt != VT_EMPTY)
				{
					vaYomi.ChangeType(VT_BSTR);
					*cYomi = COLE2T(vaYomi.bstrVal);
					if (*cYomi == " " || *cYomi == "�@")
						std::cout << SindyErrorInfo(ipAccessPoint, "���ʓǂ݂������Ă��Ȃ�");
				}
			}
			break;
		default: return GDBERROR2(E_UNEXPECTED, func);
	}

	return S_OK;
}


HRESULT SINDY2ROADPOINT::GetOriginalFlag(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cOriginal)
{
	HRESULT hr = S_OK;

	*cOriginal = 0;

	IFeaturePtr ipFacilInfoPoint = NULL;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;

	CComVariant vaDummyName;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::DUMMYNAME_F, &vaDummyName))) return hr;
	vaDummyName.ChangeType(VT_I4);

	*cOriginal = vaDummyName.lVal;

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetSapaInfo(IFeaturePtr ipAccessPoint, std::vector<long> *cSapaInfo)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetSapaInfo";

	unsigned long aDigit = 25;	// SA/PA���̌���

	cSapaInfo->clear();
	for (long i = 0; sindy::access_point::SAPAINFO3[i].size(); i++)
	{
		CComVariant vaInfo;
		if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::SAPAINFO3[i], &vaInfo))) return hr;
		vaInfo.ChangeType(VT_I4);
		cSapaInfo->push_back(vaInfo.lVal);
	}

	// ���U�[�u�̈�̍쐬
	while(cSapaInfo->size() < aDigit)
	{
		cSapaInfo->push_back(0);
	}

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetDummyFacilFlag(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cDummyFacilFlag)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetDummyFacilFlag";

	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);

	CComVariant vaDummyFacilFlag;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::DUMMYFACIL_F, &vaDummyFacilFlag))) return hr;
	vaDummyFacilFlag.ChangeType(VT_I4);

	*cDummyFacilFlag = vaDummyFacilFlag.lVal;

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetName(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, std::string *cName, std::string *cYomi)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetName";

	*cName = *cYomi = "";

	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);

	CComVariant vaName, vaYomi;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::NAME_KANJI, &vaName))) return hr;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::NAME_YOMI, &vaYomi))) return hr;

	if (vaName.vt != VT_NULL && vaName.vt != VT_EMPTY)
	{
		vaName.ChangeType(VT_BSTR);
		*cName = COLE2T(vaName.bstrVal);
	}

	if (vaYomi.vt != VT_NULL && vaYomi.vt != VT_EMPTY)
	{
		vaYomi.ChangeType(VT_BSTR);
		*cYomi = COLE2T(vaYomi.bstrVal);
	}

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetSequence(IFeaturePtr ipAccessPoint, long *cSequence)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetSequence";

	*cSequence = 0;

	CComVariant vaSequence;
	if (FAILED(hr = GDBGetValue(ipAccessPoint, sindy::access_point::ROAD_SEQ, &vaSequence))) return hr;
	vaSequence.ChangeType(VT_I4);

	*cSequence = vaSequence.lVal;

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetBaseAdmin(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cAdminCode)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetBaseAdmin";

	*cAdminCode = 0;

	// bug11633 �t�B�[�`���������擾���ꂽ�ꍇ�A�Z��5���R�[�h����ԏ������l��Ԃ�
	vector<IFeaturePtr> ipBaseAdmins;
	if (FAILED(hr = cHwyInfo->GetBaseAdmin(ipAccessPoint, ipBaseAdmins))) return hr;
	if (0 == ipBaseAdmins.size())
	{
		std::cerr << "Admin code is zero." << std::endl; return S_OK;
	}

	set<long> citycodeSet;
	CComVariant vaCityCode;
	for(auto ipBaseAdmin : ipBaseAdmins)
	{
		if (FAILED(hr = GDBGetValue(ipBaseAdmin, sindy::base_admin::CITYCODE, &vaCityCode))) return hr;
		vaCityCode.ChangeType(VT_I4);
		citycodeSet.insert(vaCityCode.lVal);
	}

	// �Z��5���R�[�h����ԏ������l�i�擪�v�f�j��Ԃ�
	*cAdminCode = *(citycodeSet.begin());

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetFacilInfoPointAdmin(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cAdminCode)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetFacilInfoPointAdmin";

	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL) return GDBERROR2(E_UNEXPECTED, func);

	if (FAILED(hr = GetBaseAdmin(cHwyInfo, ipFacilInfoPoint, cAdminCode))) return hr;

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetBaseMeshCoord(IFeaturePtr ipAccessPoint, long *cMesh, long *cX, long *cY)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetBaseMeshCoord";

	IGeometryPtr ipGeometry;
	if (FAILED(hr = ipAccessPoint->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);

	return GDBGetBaseMeshNormalizeCoord((IPointPtr)ipGeometry, cMesh, cX, cY);
}

HRESULT SINDY2ROADPOINT::GetMeshSequence(IFeaturePtr ipAccessPoint, long *cMeshSeq)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetMeshSequence";

	// �I�u�W�F�N�gID�𗬗p
	if (FAILED(hr = ipAccessPoint->get_OID(cMeshSeq))) return GDBERROR(hr, func);

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetHwyModeFlag(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, bool *cHwyModeFlag)
{
	return cHwyInfo->isHwyMode(ipAccessPoint, cHwyModeFlag);
}


HRESULT SINDY2ROADPOINT::GetPrefPoint(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cMesh, long *cX, long *cY)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetPrefPoint";

	*cMesh = *cX = *cY = 0;

	// �{�ݏ��|�C���g�̎擾
	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL)
		return GDBERROR2(E_UNEXPECTED, func);

	// �����v�Z�t���O�̃`�F�b�N
	CComVariant vaAutoPos;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::AUTOPOSITION_F, &vaAutoPos))) return hr;
	vaAutoPos.ChangeType(VT_I4);

	// �����v�Z�t���O��OFF�̏ꍇ�͎{�ݏ��|�C���g�̍��W���擾
	if (! vaAutoPos.lVal)
	{
		IGeometryPtr ipGeometry = NULL;
		if (FAILED(hr = ipFacilInfoPoint->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);
		if (FAILED(hr = GDBGetBaseMeshNormalizeCoord((IPointPtr)ipGeometry, cMesh, cX, cY)))
			return hr;
		return S_OK;
	}

	// �����v�Z�t���O��ON�̏ꍇ�́A�d�S�ʒu���擾����
	long aRoadCode = 0, aRoadSeq = 0;
	if (FAILED(hr = cHwyInfo->GetFacilCode(ipAccessPoint, &aRoadCode, &aRoadSeq))) return hr;
	GDBHWYFACILRANGE aRange;
	if (FAILED(hr = cHwyInfo->eAccessPoint.SearchByFacilCode(aRoadCode, aRoadSeq, &aRange)))
		return hr;
	if (aRange.first == aRange.second)
		return GDBERROR2(E_UNEXPECTED, func);

	long aPointCount = 0;
	std::vector<double> vecX, vecY; // �\�[�g���Ă��瑫���Ă���(bug10673)

	for (GDBHWYFACILITER aIter = aRange.first; aIter != aRange.second; aIter++, aPointCount++)
	{
		IFeaturePtr ipFeature = aIter->second;
		IGeometryPtr ipGeometry;
		if (FAILED(hr = ipFeature->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);
		IPointPtr ipPoint(ipGeometry);
		ATLASSERT(ipPoint != NULL);

		double aX = 0, aY = 0;
		if (FAILED(hr = ipPoint->get_X(&aX))) return GDBERROR(hr, func);
		if (FAILED(hr = ipPoint->get_Y(&aY))) return GDBERROR(hr, func);

		vecX.push_back(aX);
		vecY.push_back(aY);
	}

	std::sort(vecX.begin(), vecX.end()); // �\�[�g
	std::sort(vecY.begin(), vecY.end()); // �\�[�g
	
	double aTotalX = 0.0, aTotalY = 0.0;
	for(int i = 0; i < aPointCount; ++i)
	{
		aTotalX += vecX[i];
		aTotalY += vecY[i];
	}

	// ���ϒl�̎擾
	aTotalX /= (double)aPointCount;
	aTotalY /= (double)aPointCount;

	// ���b�V���R�[�h���킹(+100���~100���𒴂��Ȃ��悤����)
	ZMeshPos aMeshPos(aTotalX, aTotalY);
	AdjustMeshCode(&aMeshPos);

	*cX = aMeshPos.eX;
	*cY = aMeshPos.eY;
	*cMesh = aMeshPos.eMesh;

	return S_OK;
}

HRESULT SINDY2ROADPOINT::GetPrefPointForSirius(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipAccessPoint, long *cMesh, long *cX, long *cY)
{
	HRESULT hr = S_OK;
	const char *func = "SINDY2ROADPOINT::GetPrefPointForSirius";

	*cMesh = *cX = *cY = 0;

	// �{�ݏ��|�C���g�̎擾
	IFeaturePtr ipFacilInfoPoint;
	if (FAILED(hr = cHwyInfo->GetFacilPoint(ipAccessPoint, &ipFacilInfoPoint))) return hr;
	if (ipFacilInfoPoint == NULL)
		return GDBERROR2(E_UNEXPECTED, func);

	// �d�S�ʒu���擾����
	long aRoadCode = 0, aRoadSeq = 0;
	if (FAILED(hr = cHwyInfo->GetFacilCode(ipAccessPoint, &aRoadCode, &aRoadSeq))) return hr;
	GDBHWYFACILRANGE aRange;
	if (FAILED(hr = cHwyInfo->eAccessPoint.SearchByFacilCode(aRoadCode, aRoadSeq, &aRange)))
		return hr;
	if (aRange.first == aRange.second)
		return GDBERROR2(E_UNEXPECTED, func);

	long aPointCount = 0;
	std::vector<double> vecX, vecY; // �\�[�g���Ă��瑫���Ă���(bug10673)
	
	for (GDBHWYFACILITER aIter = aRange.first; aIter != aRange.second; aIter++)
	{
		IFeaturePtr ipFeature = aIter->second;
		++aPointCount;

		IGeometryPtr ipGeometry;
		if (FAILED(hr = ipFeature->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);
		IPointPtr ipPoint(ipGeometry);
		ATLASSERT(ipPoint != NULL);

		double aX = 0, aY = 0;
		if (FAILED(hr = ipPoint->get_X(&aX))) return GDBERROR(hr, func);
		if (FAILED(hr = ipPoint->get_Y(&aY))) return GDBERROR(hr, func);

		vecX.push_back(aX);
		vecY.push_back(aY);
	}

	std::sort(vecX.begin(), vecX.end()); // �\�[�g
	std::sort(vecY.begin(), vecY.end()); // �\�[�g

	double aTotalX = 0.0, aTotalY = 0.0;
	for(int i = 0; i < aPointCount; ++i)
	{
		aTotalX += vecX[i];
		aTotalY += vecY[i];
	}

	// ���ϒl�̎擾
	aTotalX /= (double)aPointCount;
	aTotalY /= (double)aPointCount;

	// ���b�V���R�[�h���킹(+100���~100���𒴂��Ȃ��悤����)
	ZMeshPos aMeshPos(aTotalX, aTotalY);
	AdjustMeshCode(&aMeshPos);

	// �����v�Z�t���O�̃`�F�b�N
	CComVariant vaAutoPos;
	if (FAILED(hr = GDBGetValue(ipFacilInfoPoint, sindy::facil_info_point::AUTOPOSITION_F, &vaAutoPos))) return hr;
	vaAutoPos.ChangeType(VT_I4);

	// �����v�Z�t���O��OFF�̏ꍇ�͎{�ݏ��|�C���g�̍��W���擾
	if (! vaAutoPos.lVal)
	{
		IGeometryPtr ipGeometry;
		if (FAILED(hr = ipFacilInfoPoint->get_ShapeCopy(&ipGeometry))) return GDBERROR(hr, func);
		if (FAILED(hr = GDBGetBaseMeshNormalizeCoord((IPointPtr)ipGeometry, cMesh, cX, cY)))
			return hr;
		return S_OK;
	}

	*cX = aMeshPos.eX;
	*cY = aMeshPos.eY;
	*cMesh = aMeshPos.eMesh;

	return S_OK;
}


