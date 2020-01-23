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
 *	@file CreateLinkPatch.cpp
 *	@brief �A�v���P�[�V�����N���X ������
 *	@author	F.Adachi
 *	@date	2005/10/18		�V�K�쐬
 *	$ID$
 */

#include "StdAfx.h"
#include "CreateLinkPatch.h"
#include "LogFile.h"
#include "GlobalFunction.h"
#include "SINDY2INCODE.h"
#include <sstream>
#include <set>
#include <cassert>
#include <iomanip>


const CString DBHandleBase::m_ProgramName = "CreateLinkPatch";

CreateLinkPatch::CreateLinkPatch(void)
{
}

CreateLinkPatch::~CreateLinkPatch()
{
}


/**
 *	@brief	������
 *	@param	cArgc	[in]	�R�}���h���C�������̐�
 *	@param	cArgv	[in]	�R�}���h���C�������̗�
 **/
void CreateLinkPatch::init( int cArgc, char** cArgv )
{
	std::string aFunc = "CreateLinkPatch::init()";

	// ���L�C���X�^���X�̐���
	m_pCmdLineArg = std::auto_ptr<CmdLineArg>(new CmdLineArg);
	m_pAppropriateAttribute = std::auto_ptr<AppropriateAttribute>(new AppropriateAttribute);
	m_pConfirmedLink = std::auto_ptr<ConfirmedLink>(new ConfirmedLink);
	m_pRoadLinkCollection = std::auto_ptr<RoadLinkCollection>(new RoadLinkCollection);
	m_pHighwayNodeCollection = std::auto_ptr<HighwayNodeCollection>(new HighwayNodeCollection);
	m_pLinkPatch = std::auto_ptr<LinkPatch>(new LinkPatch);

	std::auto_ptr<DBInfo> pDBInfo(new DBInfo);
	m_pCmdLineArg->setDBInfo( pDBInfo.get() );
	m_pCmdLineArg->analyze( cArgc, cArgv );
	this->setDBInfo( std::auto_ptr<const DBInfo>(pDBInfo) );

	// �o�̓t�@�C�����������݉\���`�F�b�N ///////
	std::ofstream aFout( m_pCmdLineArg->getLinkPatchFilePath() );
	if (!aFout) { throw std::runtime_error( "R1F:" + aFunc + " : �����N�p�b�`�t�@�C���I�[�v���G���[" ); }
	aFout.close();
}


/**
 *	@brief	��������
 **/
void CreateLinkPatch::process(void)
{
	read();
	checkMainLink();
	checkSubLink();
	write();
}


/**
 *	@brief	�ǂݍ���
 **/
void CreateLinkPatch::read(void)
{
	// Road_Link�̓ǂݍ���
	readRoadLink();

	// Highway_Node�̓ǂݍ���
	m_pHighwayNodeCollection->read( getWorkspace() );
}


/**
 *	@brief	Road_Link�f�[�^�̓ǂݍ���
 **/
void CreateLinkPatch::readRoadLink(void)
{
	// �H���R�[�h�w��t�@�C���̓ǂݍ���
	m_pAppropriateAttribute->read( m_pCmdLineArg->getApprAttrFilePath() );
	const std::map<long, long>* pTable = m_pAppropriateAttribute->getTable();

	std::string aSubFieldStr( "OBJECTID, ROAD_CODE, ROADCLASS_C, NAVICLASS_C, UPDOWNCLASS_C, MAIN_LINKCLASS_C, PROGMODIFYDATE, MODIFYPROGNAME, FROM_NODE_ID, TO_NODE_ID, SHAPE" );
	

	// �w�肳�ꂽ�H���R�[�h���������N�̓ǂݍ���
	for (std::map<long, long>::const_iterator aIter = pTable->begin(); aIter != pTable->end(); ++aIter ) {
		long aRoadCode = aIter->first;

		std::ostringstream aSout;
		aSout << "ROAD_CODE = " << aRoadCode;
		std::string aWhereStr = aSout.str();

		m_pRoadLinkCollection->read( getWorkspace(), aWhereStr, aSubFieldStr );
	}

	// �{���ȊO�̃����N�̓ǂݍ���
	{
		// �����v�����N���܂݂����ȏ����Ńt�B���^�����O
		std::string aWhereStr = "(ROAD_CODE = 0 AND UPDOWNCLASS_C <> 0 AND MAIN_LINKCLASS_C IN( 3, 5, 6, 7, 9)) OR UPDOWNCLASS_C IN (5, 6)";
		m_pRoadLinkCollection->read( getWorkspace(), aWhereStr, aSubFieldStr );
	}
}


/**
 *	@brief	�{�������N�̃`�F�b�N
 **/
void CreateLinkPatch::checkMainLink(void) const
{
	LogFile* pLog = LogFile::Instance();

	const std::map<long, long>* pTable = m_pAppropriateAttribute->getTable();

	// Road_Link�W���̒�����w�肳�ꂽ�H���R�[�h���������N�̂݃`�F�b�N
	for (std::map<long, long>::const_iterator aTableIter = pTable->begin(); aTableIter != pTable->end(); ++aTableIter) {
		long aRoadCode = aTableIter->first;
		long aRoadClass = aTableIter->second;

		const std::map<long, RoadLink>* pRoadLinkCollection = m_pRoadLinkCollection->getRoadLinkCollection();
		// �{���݂̂̃`�F�b�N
		for (RoadLinkCollection::RoadLinkIter aRoadLinkIter = pRoadLinkCollection->begin(); aRoadLinkIter != pRoadLinkCollection->end(); ++aRoadLinkIter ) {
			if (aRoadLinkIter->second.m_RoadCode != aRoadCode) { continue; }
			
			// �`�F�b�N�ς݃����N�W���ɒǉ�
			m_pConfirmedLink->insert( aRoadLinkIter->first );
			// ���H�o�H��ʂ̃`�F�b�N�i�s�v�H�j
///////		if (aRoadLinkIter->m_NaviClass == 0) { continue; }

			// ���H�W����ʂ̃`�F�b�N
			if (aRoadLinkIter->second.m_RoadClass != aRoadClass) {
				// ���H�W����ʂ��w�肳�ꂽ���̂ƈقȂ��FLAG==1�Ń��O�֏o��
				dispRoadLink( (std::ostream&)*pLog, aRoadLinkIter->first, aRoadCode, true, "MAIN" );
			}
			else {
				dispRoadLink( (std::ostream&)*pLog, aRoadLinkIter->first, aRoadCode, false, "MAIN" );
			}
		}
	}
}


/**
 *	@brief	�{���ȊO�̃����N�̃`�F�b�N
 **/
void CreateLinkPatch::checkSubLink(void) const
{
	LogFile* pLog = LogFile::Instance();

	const std::map<long, long>* pTable = m_pAppropriateAttribute->getTable();

	// Road_Link�W���̒�����w�肳�ꂽ�H���R�[�h���������N�̂݃`�F�b�N
	for (std::map<long, long>::const_iterator aTableIter = pTable->begin(); aTableIter != pTable->end(); ++aTableIter) {
		long aRoadCode = aTableIter->first;
		long aRoadClass = aTableIter->second;

		const std::map<long, RoadLink>* pRoadLinkCollection = m_pRoadLinkCollection->getRoadLinkCollection();
		for (RoadLinkCollection::RoadLinkIter aRoadLinkIter = pRoadLinkCollection->begin(); aRoadLinkIter != pRoadLinkCollection->end(); ++aRoadLinkIter ) {
			if (aRoadLinkIter->second.m_RoadCode != aRoadCode) { continue; }
									
			// �T�u�����N�̒T���J�n
			searchSubLink( aRoadLinkIter->first, aRoadCode );
		}
	}
}


/**
 * @brief	�T�u�����N�T��
 * @note	�^����ꂽ�������H�{����RoadLink�I�u�W�F�N�g�ɐڑ�����Ă���{���ȊO�̃����N�i�����v�����N���j��T���A�ǐՂ��J�n����
 * @param	cObjectID	[in]	�Ώ�RoadLink�I�u�W�F�N�g�̃I�u�W�F�N�gID
 * @param	cRoadCode	[in]	�T���J�n�ʒu�̘H���R�[�h
 **/
void CreateLinkPatch::searchSubLink( long cObjectID, long cRoadCode ) const
{
	RoadLink aRoadLink = m_pRoadLinkCollection->getRoadLink( cObjectID );

	IPointPtr ipFromNodePoint, ipToNodePoint;
	getNodePair( cObjectID, &ipFromNodePoint, &ipToNodePoint );

	int aCount = 0;

	// Road_Link�W���̒����瓯��IPoint���������N��T��
	const std::map<long, RoadLink>* pRoadLinkCol = m_pRoadLinkCollection->getRoadLinkCollection();
	for (RoadLinkCollection::RoadLinkIter aIter = pRoadLinkCol->begin(); aIter != pRoadLinkCol->end(); ++aIter ) {

		if ( isConnected( ipFromNodePoint, aIter->first ) ) {
			trackSubLink( aIter->first, ipFromNodePoint, cRoadCode );
		}
		if ( isConnected( ipToNodePoint, aIter->first ) ) {
			trackSubLink( aIter->first, ipToNodePoint, cRoadCode );
		}
		aCount++;
	}
}


/**
 *	@brief	From/To�m�[�h�̎擾
 *	@note	�^����ꂽRoadLink�I�u�W�F�N�g����From/To�m�[�h��IPoint���擾����
 *	@note	�iRoadLink�Ɏ��������������������c�j
 *	@param	cObjectID	[in]	�Ώ�RoadLink�I�u�W�F�N�g�̃I�u�W�F�N�gID
 *	@param	cpFromNodePoint	[out]	From�m�[�h�p�o�b�t�@
 *	@param	cpToNodePoint	[out]	To�m�[�h�p�o�b�t�@
 **/
void CreateLinkPatch::getNodePair( long cObjectID, IPoint** cpFromNodePoint, IPoint** cpToNodePoint ) const
{
	std::string aFunc = "CreateLinkPatch::getNodePair()";

	RoadLink aRoadLink = m_pRoadLinkCollection->getRoadLink( cObjectID );

	IPointCollectionPtr ipPointCol( aRoadLink.m_pShape );	// dynamic_cast�Ɠ����B���s�����ꍇ�̗�O��catch���ׂ�
	long aPointCount = 0;
	if (ipPointCol->get_PointCount( &aPointCount ) != S_OK ) { throw std::runtime_error( "R1F:" + aFunc + " : get_PointCount()���s"); }

	if (ipPointCol->get_Point( 0, cpFromNodePoint ) != S_OK ) { throw std::runtime_error( "R1F:" + aFunc + " : get_Point()���s"); }
	if (ipPointCol->get_Point( aPointCount-1, cpToNodePoint ) != S_OK ) { throw std::runtime_error( "R1F:" + aFunc + " : get_Point()���s"); }
}


/**
 *	@brief	IPoint��RoadLink�̐ڐG����
 *	@note	�^����ꂽIPoint���A�^����ꂽ�I�u�W�F�N�gID������RoadLink��From/To�m�[�h�ɂȂ��Ă��邩���ׂ�
 *	@param	cpPoint	[in]	�Ώ�IPoint�ւ̃|�C���^
 *	@param	cObjectID	[in]	�Ώ�RoadLink�I�u�W�F�N�g�̃I�u�W�F�N�gID
 *	@return	�ڐG���Ă��邩�ۂ�
 **/
bool CreateLinkPatch::isConnected( IPoint* cpPoint, long cObjectID ) const
{
	std::string aFunc = "CreateLinkPatch::isConnected()";

	RoadLink aRoadLink = m_pRoadLinkCollection->getRoadLink( cObjectID );

	if (cpPoint == NULL) { throw std::runtime_error( "R1F:" + aFunc + " : NULL" ); }

	IPointPtr ipFromPoint, ipToPoint;
	getNodePair( cObjectID, &ipFromPoint, &ipToPoint );
	
	int aIsSamePoint;
	cpPoint->Compare( ipFromPoint, &aIsSamePoint );
	if ( !aIsSamePoint ) { return true;	}
	else {
		cpPoint->Compare( ipToPoint, &aIsSamePoint );
		if ( !aIsSamePoint ) { return true;	}
	}

	return false;
}


/**
 * @brief	�T�u�����N�ǐ�
 * @note	�{���i�㉺�����j�ȊO�̃����N��H��A�I�������𖞂����܂ōċA�I�Ƀ`�F�b�N���s�Ȃ�
 * @param	cObjectID	[in]	�Ώ�RoadLink�I�u�W�F�N�g�̃I�u�W�F�N�gID
 * @param	cpPrevPoint	[in]	�P�O�̃����N�Ƃ̋��L�m�[�h�̍��W
 * @param	cRoadCode	[in]	�T���J�n�ʒu�̘H���R�[�h
 */
void CreateLinkPatch::trackSubLink( long cObjectID, IPointPtr cpPrevPoint, long cRoadCode ) const
{
	std::string aFunc = "CreateLinkPatch::trackSubLink()";

	RoadLink aRoadLink = m_pRoadLinkCollection->getRoadLink( cObjectID );

	LogFile* pLog = LogFile::Instance();
	static long aCount = 0;

	// ���ڃ����N���`�F�b�N�ς݂Ȃ�I��
	if ( m_pConfirmedLink->exist( cObjectID ) ) { return; }
	++aCount;
#ifdef	_DEBUG
	std::cout << "�ǐՍς݃����N�� : " << aCount << "\r";
#endif

	{
		// �`�F�b�N�ς݃����N�W���ɒǉ�
		m_pConfirmedLink->insert( cObjectID );
		// ���H�o�H��ʂ̃`�F�b�N�i�s�v�H�j
//		if (aRoadLink.m_NaviClass == 0) { }

		// ���H�W����ʂ̃`�F�b�N
		long aRoadClass = m_pAppropriateAttribute->getRoadClass( cRoadCode );
		if (aRoadLink.m_RoadClass != aRoadClass) {
			dispRoadLink( (std::ostream&)*pLog, cObjectID, cRoadCode, true, "SUB" );
		}
		else {
			dispRoadLink( (std::ostream&)*pLog, cObjectID, cRoadCode, false, "SUB" );
		}
	}

	// From/To�m�[�h��IPoint�擾
	IPointPtr ipFromNodePoint, ipToNodePoint;
	getNodePair( cObjectID, &ipFromNodePoint, &ipToNodePoint );

	// �i�s�����𒲂ׂ�
	int aResult;
	IPointPtr ipNextPoint;
	ipFromNodePoint->Compare( cpPrevPoint, &aResult );
	if (!aResult) { ipNextPoint = ipToNodePoint; }
	else {
		ipToNodePoint->Compare( cpPrevPoint, &aResult );
		if (!aResult) { ipNextPoint = ipFromNodePoint; }
		else { assert(0); }		// �ǂ����ɂ��q�b�g���Ȃ��ꍇ�ُ͈�
	}
	
	// �i�s������HighwayNode���`�F�b�N
	std::set<const HighwayNode*> aMultiplyHighwayNode;	// 051216 ������W�ɕ����̃n�C�E�F�C�m�[�h������ꍇ���l�����ĕύX
	m_pHighwayNodeCollection->getHighwayNode( ipNextPoint, &aMultiplyHighwayNode );
	for (std::set<const HighwayNode*>::const_iterator aIter = aMultiplyHighwayNode.begin(); aIter != aMultiplyHighwayNode.end(); ++aIter ) {
		const HighwayNode* pHighwayNode = *aIter;
		if (pHighwayNode != NULL) { 
			if (pHighwayNode->m_TollClass == 0 || pHighwayNode->m_TollClass == 4) { // 0;���򍇗��m�[�h 4:�o�����m�[�h
				return;
			}
		}
	}

	// �אڃ����N������΍ċA�I�ɒǐ�
	const std::map<long, RoadLink>* pRoadLinkCol = m_pRoadLinkCollection->getRoadLinkCollection();
	for (RoadLinkCollection::RoadLinkIter aIter = pRoadLinkCol->begin(); aIter != pRoadLinkCol->end(); ++aIter ) {
		if ( isConnected( ipNextPoint, aIter->first ) ) {
			trackSubLink( aIter->first, ipNextPoint, cRoadCode );
		}
	}
}


/**
 * @brief	���O�̌��o�����o��
 * @note	�P��ڂ̌Ăяo�����̂݃��O�̌��o����\������
 * @param	crStream	[in]	�o�̓X�g���[��
 */
void CreateLinkPatch::dispLogHeader( std::ostream& crStream ) const
{
	static unsigned aRowCount = 0;
	if (aRowCount == 0) {
		crStream << "# FREESTYLELOG" << std::endl;
		crStream << "FLAG	LAYER	OBJECTID	ROAD_CODE	ROADCLASS_C	NAVICLASS_C	UPDOWNCLASS_C	MAIN_LINKCLASS_C	FROM_NODE_ID	TO_NODE_ID	COMMENT	LINK_PATCH" << std::endl;
	}
	++aRowCount;
}


/**
 *	@brief	�w�肵��RoadLink�I�u�W�F�N�g�̒l���o��
 *	@param	crStream	[out]	�o�̓X�g���[���ւ̃��t�@�����X
 *	@param	cObjectID	[in]	�Ώ�RoadLink�I�u�W�F�N�g�̃I�u�W�F�N�gID
 *	@param	cRoadCode	[in]	�T���J�n�ʒu�̘H���R�[�h
 *	@param	cFlag	[in]	���O�̑�P�t�B�[���h�i�����ς݃t���O�j��'1'�ɂ��邩�ۂ�
 *	@param	cpComment	[in]	���O��COMMENT��ɏo�͂��镶����
 **/
void CreateLinkPatch::dispRoadLink( std::ostream& crStream, long cObjectID, long cRoadCode, bool cFlag, const char* cpComment ) const
{
	RoadLink aRoadLink = m_pRoadLinkCollection->getRoadLink( cObjectID );
	int aMeshCode = m_pRoadLinkCollection->getMeshCode( cObjectID );

	long aRoadClass = m_pAppropriateAttribute->getRoadClass( cRoadCode );
	std::string aINRoadClass = getINRoadClass( aRoadClass );

	std::stringstream aSout;
	aSout << "0x" << std::setw(8) << std::setfill('0') << std::right << aINRoadClass;

	dispLogHeader( crStream );
	crStream << ( cFlag ? 1 : 0 ) << "\t";
	crStream << "ROAD_LINK" << "\t";
	crStream << cObjectID << "\t";
	crStream << aRoadLink.m_RoadCode << "\t";
	crStream << aRoadLink.m_RoadClass << "\t";
	crStream << aRoadLink.m_NaviClass << "\t";
	crStream << aRoadLink.m_UpDownClass << "\t";
	crStream << aRoadLink.m_MainLinkClass << "\t";
	crStream << aRoadLink.m_FromNodeID << "\t";
	crStream << aRoadLink.m_ToNodeID << "\t";
	crStream << cpComment << "\t";
	crStream << "( \"DISP\", ";
	crStream << aSout.str();
	crStream << ", " << aMeshCode << ", " << aRoadLink.m_FromNodeID << ", " << aRoadLink.m_ToNodeID << " ),";	// 0x05001100
	crStream << std::endl;

	// LinkPatch�Ƀ��R�[�h���i�[
	if ( cFlag ) {
		std::string aLinkPatchFilePath = m_pCmdLineArg->getLinkPatchFilePath();
		if ( aLinkPatchFilePath.size() ) {
			LinkPatchRecord aRecord;
			aRecord.m_Attribute = "\"DISP\"";
			aRecord.m_RevisedValue = aSout.str();
			aRecord.m_MeshCode = aMeshCode;
			aRecord.m_NodeIDCollection.push_back( aRoadLink.m_FromNodeID );
			aRecord.m_NodeIDCollection.push_back( aRoadLink.m_ToNodeID );
			m_pLinkPatch->assign( aRecord );
		}
	}

}


/**
 *	@brief	��������
 **/
void CreateLinkPatch::write(void) const
{
	m_pLinkPatch->write( m_pCmdLineArg->getLinkPatchFilePath(), m_pCmdLineArg->useEUC() );
}


/**
 *	@brief	�^����ꂽSiNDY�̓��H�W����ʂɑΉ�����I/N�̓��H�W����ʂ�Ԃ�
 *	@param	cSindyRoadClass	SiNDY�̓��H�W�����
 *	@return	I/N�̓��H�W�����
 **/
std::string	CreateLinkPatch::getINRoadClass( long cSindyRoadClass ) const
{
	std::string aFunc = "CreateLinkPatch::getINRoadClass()";

	long aINCode;
	if (SINDY2INCODE::Convert( SINDY2INCODE::road_class, cSindyRoadClass, &aINCode ) != S_OK) {
		throw std::runtime_error( "R1F:" + aFunc + " : SINDY2INCODE::Convert()���s" );
	}
	std::stringstream aSout;
	aSout << std::hex << aINCode;

	return aSout.str();
}
