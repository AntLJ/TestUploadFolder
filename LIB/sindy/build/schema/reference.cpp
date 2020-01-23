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
#include <sindy/schema/reference.h>

namespace sindy {

namespace schema {

namespace citymesh {
	const TCHAR kTableName[]			= _T("CITYMESH");				//!< �e�[�u����

	const TCHAR kCityMeshCode[]			= _T("MESHCODE");				//!< ���b�V���R�[�h(���C�u�����g�p���Ă��鑼�c�[���Ő�����邽�߁A�c���Ă��� 2007/10/16 s_suzuki)
	const TCHAR kCityName[]				= _T("CITYNAME");				//!< �s�撬����
	const TCHAR kCreateYear[]			= _T("CREATE_YEAR");			//!< �쐬�N�x
	const TCHAR kUpdateYear[]			= _T("UPDATE_YEAR");			//!< ��ƔN�x
	const TCHAR kComplete[]				= _T("COMPLETE_F");				//!< ���S���t���O
	const TCHAR kSea[]					= _T("SEA_F");					//!< �C�t���O
	const TCHAR kZuka[]					= _T("ZUKA_F");					//!< �}���t���O
	const TCHAR kRaster[]				= _T("RASTER");					//!< ���X�^�[
	const TCHAR kCreateRasterYear[]		= _T("CREATE_RASTER_YEAR");		//!< ���X�^�[�쐬�N�x
	const TCHAR kCurrentAdjYear[]		= _T("CURRENT_ADJ_YEAR");		//!< �����C���N�x
	const TCHAR kPhotoShotDate[]		= _T("PHOTO_SHOT_DATE");		//!< �g�p�ʐ^�B�e�N����
	const TCHAR kFieldResearchYear[]	= _T("FIELD_RESEARCH_YEAR");	//!< ���n�������{�N�x�i�n�}�j
	const TCHAR kTrSurveyYear[]			= _T("TR_SURVEY_YEAR");			//!< ���n�������{�N�x�i�K���j
	const TCHAR kShaBgworkDate[]		= _T("SHA_BGWORK_DATE");		//!< ��C��ƔN���i���ʁj
	const TCHAR kShaAnnoworkDate[]		= _T("SHA_ANNOWORK_DATE");		//!< ��C��ƔN���i���L�j
	const TCHAR kCreateAnnoMsDate[]		= _T("CREATE_ANNO_MS_DATE");	//!< ���L���e�쐬�N��
	const TCHAR kAddrMainteYear[]		= _T("ADDR_MAINTE_YEAR");		//!< �Z�������������N�x
	const TCHAR kJmapPublicationYear[]	= _T("JMAP_PUBLICATION_YEAR");	//!< �Q��j�n�}���s�N�x
	const TCHAR kMapcubeWorkYear[]		= _T("MAPCUBE_WORK_YEAR");		//!< mapcube��ƔN�x
	const TCHAR kPdmUseYear[]			= _T("PDM_USE_YEAR");			//!< pdm�g�p�N�x
}

const _TCHAR basemesh::kTableName[] = _T("BASEMESH");

namespace middlemesh {
	const TCHAR kTableName[]			= _T("MIDDLEMESH");				//!< �e�[�u����
	const TCHAR kMiddleMeshCode[]		= _T("MESHCODE");				//!< ���b�V���R�[�h(���C�u�����g�p���Ă��鑼�c�[���Ő�����邽�߁A�c���Ă��� 2007/10/16 s_suzuki)
}

const _TCHAR topmesh::kTableName[] = _T("TOPMESH");
const _TCHAR org_vics::kTableName[] = _T("ORG_VICS");
const _TCHAR drive_map::kTableName[] = _T("DriveMap_Link");
const _TCHAR ivics::kTableName[] = _T("IVICS");

namespace townpage {
	const TCHAR kTableName[]			= _T("TOWNPAGE");				//!< �e�[�u����

	const TCHAR kIpCode[]				= _T("IPCODE");					//!< IP�R�[�h
	const TCHAR kDataKubun[]			= _T("DATA_KUBUN");				//!< �f�[�^�擾�敪
	const TCHAR kDataSeq[]				= _T("DATA_SEQ");				//!< �f�[�^�擾�V�[�P���X
	const TCHAR kPstIpCode[]			= _T("PST_IPCODE");				//!< ���W�񋟉�ЃR�[�h
	const TCHAR kAccCode[]				= _T("ACCCODE");				//!< ���x�R�[�h
	const TCHAR kPinPointFlag[]			= _T("PINPOINTFLAG");			//!< �s���|�C���g�t���O
	const TCHAR kPrefCode[]				= _T("PREFCODE");				//!< �s���{���R�[�h
	const TCHAR kCityCode[]				= _T("CITYCODE");				//!< �s�撬���R�[�h
	const TCHAR kAdrCode1[]				= _T("ADRCODE1");				//!< �厚�E�ʏ̃R�[�h
	const TCHAR kAdrCode2[]				= _T("ADRCODE2");				//!< ���E���ڃR�[�h
	const TCHAR kChibanCode[]			= _T("CHIBANCODE");				//!< �n�ԃR�[�h
	const TCHAR kJukyoCode[]			= _T("JUKYOCODE");				//!< �Z���ԍ��R�[�h
	const TCHAR kNttGrpCode[]			= _T("NTTGRPCODE");				//!< NTT���ރR�[�h
	const TCHAR kNttCode[]				= _T("NTTCODE");				//!< NTT�Ǝ�R�[�h
	const TCHAR kChainCode[]			= _T("CHAINCODE");				//!< �X�܃R�[�h
	const TCHAR kName[]					= _T("NAME");					//!< �\���p����
	const TCHAR kYomi[]					= _T("YOMI");					//!< �\���p���̓ǂ�
	const TCHAR kTelC[]					= _T("TEL_C");					//!< �s�X�ǔ�
	const TCHAR kTel[]					= _T("TEL");					//!< �d�b�ԍ�
	const TCHAR kNaCode[]				= _T("NACODE");					//!< �g�p�֎~���R�R�[�h
}






} // namespace schema

} // namespace sindy
