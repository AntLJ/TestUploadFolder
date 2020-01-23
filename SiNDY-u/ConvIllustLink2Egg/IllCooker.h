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

#pragma once
#include "common.h"
#include "PgDataBaseEx.h"

#include <sindy/schema/illust.h>

/**
 *	@class IllCooker
 *	@brief ilustlink���R�[�h��inf_illust, lq_illust���R�[�h�ϊ��N���X
 */
class IllCooker
{
public:
	IllCooker(void);
	~IllCooker(void);

	/**
	 *	@brief	�C���X�g���R�[�h�ϊ��֐�
	 *	@param	orgRlDataBase [in] �ϊ������H�����NPostgreSQLDB�n���h�����O�N���X
	 *	@param	tgtRows [in] �ϊ��ΏۃC���X�g�����N���R�[�h
	 *	@param	id2names [in] �C���X�g�����Nobjectid�ƃC���X�g���Ή��\
	 *	@param	roadLinkTable [in] road_link�e�[�u����
	 *	@param	infIllInfos [out] inf_illust���R�[�h���
	 *	@param	lqIllInfos [out] lq_illust���R�[�h���
	 *	@return true ����
	 *	@return false ���s
	 */
	bool Cook(std::shared_ptr<PgDataBaseEx> orgRlDataBase,
			  const std::vector<ns::RowPtr>& tgtRows, 
		      const std::map<OID, IllFileNames>& id2names,
			  LPCTSTR roadLinkTable,
			  std::map<OID,InfIllInfo>& infIllInfos,
			  std::map<LqIllKey, LqIllInfo>& lqIllInfos);

private:
	/**
	 *	@brief	�C���X�g�����N���R�[�h���擾�֐�
	 *	@param	tgtRows	[in] �ϊ��ΏۃC���X�g�����N���R�[�h
	 *	@param	tgtIllLinkInfo [out] �C���X�g�����N���R�[�h���
	 *	@return true ����
	 *	@return false ���s
	 */
	bool GetIllLinkInfo(const std::vector<ns::RowPtr>& tgtRows, 
						std::map<OID, IllLinkInfo>& tgtIllLinkInfo);
	/**
	 *	@brief	�C���X�g�����N�ϊ��֐�
	 *	@param	orgRlDataBase [in] �ϊ������H�����NPostgreSQLDB�n���h�����O�N���X
	 *	@param	procIllLinkInfo [in] �ϊ��ΏۃC���X�g�����N���
	 *	@param	procNodes [in] �ϊ��ΏۃC���X�g�����N�֘A�m�[�hID�Q
	 *	@param	id2names [in] �C���X�g�����Nobjectid�ƃC���X�g���Ή��\
	 *	@param	infIllInfos [out] inf_illust���R�[�h���
	 *	@param	lqIllInfos [out] lq_illust���R�[�h���
	 *	@return true ����
	 *	@return false ���s
	 */
	bool ConvertIllLink(std::shared_ptr<PgDataBaseEx> orgRlDataBase,
						LPCTSTR roadLinkTable,
						const std::vector<IllLinkInfo>& procIllLinkInfo, 
					    const std::vector<OID>&  procTgtNodes,
						const std::map<OID, IllFileNames>& id2names,
					    std::map<OID, InfIllInfo>& infIllInfos,
					    std::map<LqIllKey, LqIllInfo>& lqIllInfos);
	/**
	 *	@brief	�C���X�g�����N�֘A���H�����N�擾�֐�
	 *	@param	orgRlDataBase [in] �ϊ������H�����NPostgreSQLDB�n���h�����O�N���X
	 *	@param	procTgtNodes [in] �ϊ��ΏۃC���X�g�����N�֘A�m�[�hID�Q
	 *	@param	modeDiff [in] �����e�[�u�����[�hON/OFF (true:ON false:OFF)
	 *	@param	relRoadLinkInfos [out] �C���X�g�����N�֘A���H�����N���
	 *	@return true ����
	 *	@return false ���s
	 */
	bool GetRelRoadLink(std::shared_ptr<PgDataBaseEx> orgRlDataBase,
						LPCTSTR roadLinkTable,
						const std::vector<OID>& procTgtNodes,
						std::map<RoadLinkKey, RoadLinkInfo>& relRoadLinkInfos);
	/**
	 *	@brief	���H�����N���擾�֐�
	 *	@param	rows [in] �Ώۓ��H�����Nrows�|�C���^
	 *	@param	relRoadLinkInfos [out] �C���X�g�����N�֘A���H�����N���
	 *	@return true ����
	 *	@return false ���s
	 */
	bool GetLinkInfo(const ns::RowsPtr& rows, std::map<RoadLinkKey, RoadLinkInfo>& relRoadLinkInfos);
	/**
	 *	@brief	�m�[�hID��˃����NID��ϊ��֐�
	 *	@param	illLinkId [in]
	 *	@param	tgtNodeIds [in] �ϊ����m�[�hID��
	 *	@param	relRoadLinkInfos [in] �֘A���H�����N���
	 *	@param  lqInfos [out] �����NID��
	 *	@return true ����
	 *	@return false ���s
	 */
	bool nq2lq(OID illLinkId,
			   const std::vector<OID>& tgtNodeIds,
			   const std::map<RoadLinkKey, RoadLinkInfo>& relRoadLinkInfos,
			   std::vector<LinkQueueInfo>& lqInfos);
};

