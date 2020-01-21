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
* @file define.h
*
* @brief SiNDY�֘A�̌Œ�l��`�t�@�C��
*/
#ifndef __DEFINE_MXCOMMANDS
#define __DEFINE_MXCOMMANDS 1	//!< �d���C���N���[�h�h�~

#include "define_enum.h" // enum ��`

//#define TOPOLOGY_EDIT 15	// ���L�ҏW�̍ő�l�i�I�u�W�F�N�g���j CFeedbackFeatureSelection�ACFeatureSelection���쐬�����̂ł���Ȃ�
//#define MAX_LAYER     20    // �ǂݍ��ރ��C�����̍ő�l�iSnapEnvObj.h�Q�Ɓj CSnapCollection���쐬�����̂ł���Ȃ�
/// ���C�����̍ő咷�iSearchToolDlg.h  QueryBy* �Q�Ɓj
//#define MAX_LAYERNAME_LEN 256	
/// �������ʂ̃��X�g�\���ő吔�iSearchDlg.[h,cpp] �Q�Ɓj
//#define MAX_SEARCHED_ITEM 100	
// �I�u�W�F�N�g�̃t�B�[���h�l�� 4KB �ɐ����iAttrDlg.cpp�Q�Ɓj
#define MAX_FIELD_VALUE   4096	
//#define MAX_FIELD_NAME    256	// �I�u�W�F�N�g�̃t�B�[���h���ő�l�iAttrGridCtrlObj.cpp�Q�Ɓj
//#define MAX_SUBDOMAIN_LEN 8192	// ��̃J�����ɑ΂���T�u�h���C���̑������iAttrGridCtrlObj.cpp�Q�Ɓj

const static INT MAX_WORKPURPOSE_LENGTH = 512;		//!< �u��ƖړI�v�̈������̍ő�o�C�g��
const static INT MAX_WORKPURPOSEKEYS_KENGTH = 4092;	//!< �u��ƖړI�v�t�@�C���̃L�[�̑����o�C�g��
/// �J�X�^���A�v���P�[�V�����Ŏg�p����ꍇ�ɂ̓R�����g���O���Ă�������
//#ifndef USE_MAPCONTROL
//#define USE_MAPCONTROL 1
//#endif // ifndef USE_MAPCONTROL
/// �J�X�^���A�v���P�[�V������ ISnapEnvObj ���g�p����ꍇ�̓R�����g���O���Ă�������
//#ifndef USE_SNAPENV
//#define USE_SNAPENV 1
//#endif // ifndef USE_SNAPENV

/// ���C���̃G�C���A�X���̒�`
//#define ALIAS_NODE_NAME   (_T("node"))
//#define ALIAS_ROAD_NAME   (_T("road"))

// ���C���̃e�[�u�����̒�`
const static LPCTSTR NODE_TABLE_NAME			= (_T("ROAD_NODE"));		//!< �m�[�h�e�[�u������
const static LPCTSTR ROAD_TABLE_NAME			= (_T("ROAD_LINK"));		//!< ���H�����N�e�[�u������
const static LPCTSTR WNODE_TABLE_NAME			= (_T("WALK_NODE"));		//!< ���s�җp�m�[�h�e�[�u������
const static LPCTSTR WROAD_TABLE_NAME			= (_T("WALK_LINK"));		//!< ���s�җp���H�����N�e�[�u������
const static LPCTSTR PLINK_TABLE_NAME			= (_T("PEC_LINK"));			//!< ���ԏꃊ���N�e�[�u������
const static LPCTSTR INTERSECTION_TABLE_NAME	= (_T("INTERSECTION"));		//!< ���������_�e�[�u������
const static LPCTSTR BASE_LAND_TABLE_NAME		= (_T("BASE_LAND"));		//!< ���k���n�e�[�u������
const static LPCTSTR BASE_CONTOUR_TABLE_NAME	= (_T("BASE_CONTOUR"));		//!< BASE_CONTOUR�e�[�u������
const static LPCTSTR MIDDLE_CONTOUR_TABLE_NAME	= (_T("MIDDLE_CONTOUR"));	//!< MIDDLE_CONTOUR�e�[�u������
const static LPCTSTR MIDDLE_LINE_TABLE_NAME		= (_T("MIDDLE_LINE"));		//!< MIDDLE_LINE�e�[�u������
const static LPCTSTR MIDDLE_SITE_TABLE_NAME		= (_T("MIDDLE_SITE"));		//!< MIDDLE_SITE�e�[�u������
const static LPCTSTR BASESITE_TABLE_NAME		= (_T("BASE_SITE"));		//!< ���k�w�i�e�[�u������
const static LPCTSTR BASELINE_TABLE_NAME		= (_T("BASE_LINE"));		//!< ���k�w�i���C���e�[�u������
const static LPCTSTR BASERAILWAY_TABLE_NAME		= (_T("BASE_RAILWAY"));		//!< ���k�S���e�[�u������
const static LPCTSTR BASESTATION_TABLE_NAME		= (_T("BASE_STATION"));		//!< ���k�w�Ƀ|���S���e�[�u������
const static LPCTSTR GRAD_TABLE_NAME			= ( _T("GRAD"));			//!< �X�΃e�[�u������
const static LPCTSTR SEGMENTATTR_TABLE_NAME		= ( _T("SEGMENT_ATTR"));	//!< �����N�������e�[�u������
const static LPCTSTR CITY_LINE_TABLE_NAME		= ( _T("CITY_DISP_LINE"));	//!< �s�s�n�}���L�\���ʒu�e�[�u������
const static LPCTSTR CITY_ANNOTATION_TABLE_NAME	= ( _T("CITY_ANNOTATION"));	//!< �s�s�n�}���L�^�ʒu�e�[�u������
const static LPCTSTR BUILDING_LINE_TABLE_NAME	= ( _T("BUILDING_LINE"));	//!< �s�s�n�}�ƌ`���C���i�������j�e�[�u������
const static LPCTSTR BUILDING_TABLE_NAME		= ( _T("BUILDING"));		//!< �s�s�n�}�ƌ`�e�[�u������
const static LPCTSTR BUILDING_STEP_TABLE_NAME	= ( _T("BUILDING_STEP"));	//!< �s�s�n�}�����K���e�[�u������
const static LPCTSTR CITYLINE_TABLE_NAME		= ( _T("CITY_LINE"));		//!< �s�s�n�}�w�i���C���e�[�u������
const static LPCTSTR CITYSITE_TABLE_NAME		= ( _T("CITY_SITE"));		//!< �s�s�n�}�w�i�|���S���e�[�u������
const static LPCTSTR CITYRAILWAY_TABLE_NAME		= ( _T("CITY_RAILWAY"));	//!< �s�s�n�}�S���e�[�u������
const static LPCTSTR CITYSTATION_TABLE_NAME		= ( _T("CITY_STATION"));	//!< �s�s�n�}�w�Ƀe�[�u������
const static LPCTSTR ADMINPOLY_TABLE_NAME		= ( _T("ADMIN_POLY"));		//!< �s���E�|���S���e�[�u������
const static LPCTSTR BASE_SC1_LINE_TABLE_NAME	= ( _T("B_SC1DISP_LINE"));	//!< ���kS1���L�\���ʒu�e�[�u������
const static LPCTSTR BASE_SC2_LINE_TABLE_NAME	= ( _T("B_SC2DISP_LINE"));	//!< ���kS2���L�\���ʒu�e�[�u������
const static LPCTSTR BASE_SC3_LINE_TABLE_NAME	= ( _T("B_SC3DISP_LINE"));	//!< ���kS3���L�\���ʒu�e�[�u������
const static LPCTSTR BASE_SC4_LINE_TABLE_NAME	= ( _T("B_SC4DISP_LINE"));	//!< ���kS4���L�\���ʒu�e�[�u������
const static LPCTSTR BASE_ANNOTATION_TABLE_NAME	= ( _T("BASE_ANNOTATION"));	//!< ���k���L�^�ʒu�e�[�u������
const static LPCTSTR MIDDLE_SC1_LINE_TABLE_NAME	= ( _T("M_SC1DISP_LINE"));	//!< �~�h��S1���L�\���ʒu�e�[�u������
const static LPCTSTR MIDDLE_SC2_LINE_TABLE_NAME	= ( _T("M_SC2DISP_LINE"));	//!< �~�h��S2���L�\���ʒu�e�[�u������
const static LPCTSTR MIDDLE_SC3_LINE_TABLE_NAME	= ( _T("M_SC3DISP_LINE"));	//!< �~�h��S3���L�\���ʒu�e�[�u������
const static LPCTSTR MIDDLE_SC4_LINE_TABLE_NAME	= ( _T("M_SC4DISP_LINE"));	//!< �~�h��S4���L�\���ʒu�e�[�u������
const static LPCTSTR MIDDLE_ANNOTATION_TABLE_NAME	= ( _T("MIDDLE_ANNOTATION"));	//!< �~�h�����L�^�ʒu�e�[�u������
const static LPCTSTR TOP_SC1_LINE_TABLE_NAME	= ( _T("T_SC1DISP_LINE"));	//!< �g�b�vS1���L�\���ʒu�e�[�u������
const static LPCTSTR TOP_SC2_LINE_TABLE_NAME	= ( _T("T_SC2DISP_LINE"));	//!< �g�b�vS2���L�\���ʒu�e�[�u������
const static LPCTSTR TOP_SC3_LINE_TABLE_NAME	= ( _T("T_SC3DISP_LINE"));	//!< �g�b�vS3���L�\���ʒu�e�[�u������
const static LPCTSTR TOP_SC4_LINE_TABLE_NAME	= ( _T("T_SC4DISP_LINE"));	//!< �g�b�vS4���L�\���ʒu�e�[�u������
const static LPCTSTR TOP_ANNOTATION_TABLE_NAME	= ( _T("TOP_ANNOTATION"));	//!< �g�b�v���L�^�ʒu�e�[�u������
const static LPCTSTR LOADPOINT_TABLE_NAME		= ( _T("ROAD_POINT"));		//!< roadpoint, normalpoint�e�[�u������
const static LPCTSTR HWYNODE_TABLE_NAME			= ( _T("HWY_NODE"));		//!< hwynode�Atollnode�e�[�u������
const static LPCTSTR GOU_POINT_TABLE_NAME		= ( _T("GOU_POINT"));		//!< ���|�C���g�e�[�u������
const static LPCTSTR BASE_REP_POINT_TABLE_NAME	= ( _T("BASE_REP_POINT"));	//!< ���k��\�_�e�[�u������
const static LPCTSTR BASEMESH_TABLE_NAME		= ( _T("BASEMESH"));		//!< 2�����b�V���e�[�u������
const static LPCTSTR CITYMESH_TABLE_NAME		= ( _T("CITYMESH"));		//!< �s�s�n�}���b�V���e�[�u������
const static LPCTSTR MIDDLEMESH_TABLE_NAME		= ( _T("MIDDLEMESH"));		//!< �~�h�����b�V���e�[�u������
const static LPCTSTR TOPMESH_TABLE_NAME			= ( _T("TOPMESH"));			//!< �g�b�v���b�V���e�[�u������
const static LPCTSTR BASE_ADMIN_TABLE_NAME		= ( _T("BASE_ADMIN"));		//!< ���k�s���E�e�[�u������
const static LPCTSTR CITY_ADMIN_TABLE_NAME		= ( _T("CITY_ADMIN"));		//!< �s�s�n�}�s���E�e�[�u������
const static LPCTSTR ACCESS_POINT_TABLE_NAME	= ( _T("ACCESS_POINT"));	//!< �����E�L�����H�����֌W�|�C���g�e�[�u������
const static LPCTSTR HIGHWAY_NODE_TABLE_NAME	= ( _T("HIGHWAY_NODE"));	//!< �n�C�E�F�C�֘A���򍇗��E�������|�C���g�e�[�u������
const static LPCTSTR KLINK_POINT_TABLE_NAME		= ( _T("KLINK_POINT"));		//!< �ƌ`�����N�|�C���g�e�[�u������
const static LPCTSTR FACILINFO_TABLE_NAME		= ( _T("FACIL_INFO_POINT"));//!< �{�ݏ��|�C���g�e�[�u������
const static LPCTSTR SAPA_INFO_TABLE_NAME		= ( _T("SAPA_INFO_TABLE")); //!< �{�ݏ�񖼏�
const static LPCTSTR DISP_ONLY_LINK_TABLE_NAME	= ( _T("DISP_ONLY_LINK"));	//!< �\����p���H�e�[�u������
const static LPCTSTR PARKING_SHAPE_TABLE_NAME	= ( _T("PARKING_SHAPE"));	//!< ���ԏ�O�`�|���S���e�[�u������
const static LPCTSTR SURVEY_POINT_TABLE_NAME	= ( _T("SURVEY_POINT"));	//!< ���n�����|�C���g�e�[�u������
const static LPCTSTR CS_ADDRPOINT_TABLE_NAME	= ( _T("CS_ADDR_POINT"));	//!< �Z���N���[���Ή��|�C���g�e�[�u������
const static LPCTSTR CLAIMPOLY_ADDR_TABLE_NAME	= ( _T("CLAIMPOLY_ADDR"));	//!< �N���[���|���S���i�Z���j�e�[�u������
const static LPCTSTR CLAIMPOLY_MAP_TABLE_NAME	= ( _T("CLAIMPOLY_MAP"));	//!< �N���[���|���S���i�n�}�j�e�[�u������
const static LPCTSTR TOP_SITE_TABLE_NAME		= ( _T("TOP_SITE"));		//!< �g�b�v�w�i�e�[�u������
const static LPCTSTR TOP_LINE_TABLE_NAME		= ( _T("TOP_LINE"));		//!< �g�b�v���C���e�[�u������
const static LPCTSTR TOP_CONTOUR_TABLE_NAME		= ( _T("TOP_CONTOUR"));		//!< �g�b�v�i�ʃe�[�u������
const static LPCTSTR CITY_REP_POINT_TABLE_NAME	= ( _T("CITY_REP_POINT"));	//!< �s�s�n�}��\�_�e�[�u������
const static LPCTSTR HIMA_LOG_TABLE_NAME		= ( _T("HIMA_LOG"));		//!< �Ђ܂�胍�O�e�[�u������

/// �֘A�e�[�u���̃e�[�u�����̂̒�`
const static LPCTSTR DUPLILINK_TABLE_NAME		= ( _T("DUPLI_LINK"));
const static LPCTSTR HIGHWEYTEXT_TABLE_NAME		= ( _T("HIGHWAY_TEXT"));
const static LPCTSTR SAPAINFO_TABLE_NAME		= ( _T("SAPA_INFO_TABLE"));
const static LPCTSTR TOLLINFO_TABLE_NAME		= ( _T("TOLL_INFO_TABLE"));
const static LPCTSTR TIME_NOPASSAGE_TABLE_NAME	= ( _T("TIME_NOPASSAGE"));
const static LPCTSTR TIME_ONEWAY_TABLE_NAME		= ( _T("TIME_ONEWAY"));
const static LPCTSTR INF_TURNREG_TABLE_NAME		= ( _T("INF_TURNREG"));
const static LPCTSTR LQ_TURNREG_TABLE_NAME		= ( _T("LQ_TURNREG"));
const static LPCTSTR LQ_VICS_TABLE_NAME			= ( _T("LQ_VICS"));
const static LPCTSTR INF_VICS_TABLE_NAME		= ( _T("INF_VICS"));
const static LPCTSTR INF_BYWAY_TABLE_NAME		= ( _T("INF_BYWAY"));
const static LPCTSTR INF_ROUTE_TABLE_NAME		= ( _T("INF_ROUTE"));
const static LPCTSTR INF_INTERNAVI_TABLE_NAME	= ( _T("INF_INTERNAVI"));
const static LPCTSTR INF_IPCVICS_TABLE_NAME		= ( _T("INF_IPCVICS")); // [Bug 6473]Inf_ipcVics�ALq_ipcVics�ɑΉ����ė~����
const static LPCTSTR INF_INTERSECTION_TABLE_NAME	= ( _T("INF_INTERSECTION")); // �i�������ʌ����_����

/// �Q�ƃe�[�u���̃e�[�u�����̂̒�`
const static LPCTSTR REF_ROADCODE_TABLE_NAME	= ( _T("ROAD_CODE_LIST"));
const static LPCTSTR REF_ANNOCLASS_TABLE_NAME	= ( _T("ANNOTATION_CLASS"));
const static LPCTSTR REF_ALLEY_TABLE_NAME		= ( _T("ALLEY_MAINTE_LIST"));
const static LPCTSTR REF_ANNOBASE_TABLE_NAME	= ( _T("ANNO_BASE_CLASS"));
const static LPCTSTR REF_ANNOCITY_TABLE_NAME	= ( _T("ANNO_CITY_CLASS"));
const static LPCTSTR REF_ANNOMID_TABLE_NAME		= ( _T("ANNO_MIDDLE_CLASS"));
const static LPCTSTR REF_BACKGROUND_TABLE_NAME	= ( _T("BACKGROUND_CLASS"));

/// �t�B�[���h����
const static LPCTSTR OPERATOR_FIELD				= ( _T( "OPERATOR"));
const static LPCTSTR MODIFYDATE_FIELD			= ( _T( "MODIFYDATE"));
const static LPCTSTR PURPOSE_C_FIELD			= ( _T( "PURPOSE_C"));
const static LPCTSTR UPDATETYPE_CFIELD			= ( _T( "UPDATETYPE_C"));
const static LPCTSTR UPDATE_CFIELD				= ( _T( "UPDATE_C"));
const static LPCTSTR USERCLAIM_F_FIELD			= ( _T( "USERCLAIM_F"));
const static LPCTSTR FIELDSURVEY_F_FIELD		= ( _T( "FIELDSURVEY_F"));
const static LPCTSTR FROM_FIELD					= ( _T( "FROM_NODE_ID"));
const static LPCTSTR TO_FIELD					= ( _T( "TO_NODE_ID"));
const static LPCTSTR NODECLASS_FIELD			= ( _T( "NODECLASS_C"));
const static LPCTSTR OBJECTID_FIELD				= ( _T("OBJECTID"));		// ObjectID
const static LPCTSTR ANNO_TARGET_STR_C_FIELD	= ( _T("TARGET_STR_C"));	// �����\�����C���[�̒��L�Ώ۔ԍ��t�B�[���h
const static LPCTSTR ANNO_ID_FIELD				= ( _T("ANNO_ID"));			// ���L�\�����C���[�̒��LID
const static LPCTSTR ANNO_FONT_SIZE_FIELD		= ( _T("FONTSIZE"));		// ���L�\�����C���[�̃t�H���g�T�C�Y
const static LPCTSTR ANNO_CLASS_C_FIELD			= ( _T("ANNOCLASS_C"));		// ��ʃR�[�h
const static LPCTSTR ANNO_DISPTYPE_C_FIELD		= ( _T("DISPTYPE_C"));		// ���L�\���^�C�v
const static LPCTSTR ANNO_SC1DISPTYPE_C_FIELD	= ( _T("SC1DISPTYPE_C"));	// S1���L�\���^�C�v
const static LPCTSTR ANNO_SC2DISPTYPE_C_FIELD	= ( _T("SC2DISPTYPE_C"));	// S2���L�\���^�C�v
const static LPCTSTR ANNO_SC3DISPTYPE_C_FIELD	= ( _T("SC3DISPTYPE_C"));	// S3���L�\���^�C�v
const static LPCTSTR ANNO_SC4DISPTYPE_C_FIELD	= ( _T("SC4DISPTYPE_C"));	// S4���L�\���^�C�v
const static LPCTSTR ANNO_NAME_STRING1_FIELD	= ( _T("NAME_STRING1"));	// ������t�B�[���h1
const static LPCTSTR ANNO_NAME_STRING2_FIELD	= ( _T("NAME_STRING2"));	// ������t�B�[���h2
const static LPCTSTR ANNO_NAME_STRING3_FIELD	= ( _T("NAME_STRING3"));	// ������t�B�[���h3 (���k�p)
const static LPCTSTR ANNO_S1DISPRULE_FIELD		= ( _T("S1DISPRULE"));		// S1���L�L�����[��
const static LPCTSTR ANNO_S2DISPRULE_FIELD		= ( _T("S2DISPRULE"));		// S2���L�L�����[��
const static LPCTSTR ANNO_S3DISPRULE_FIELD		= ( _T("S3DISPRULE"));		// S3���L�L�����[��
const static LPCTSTR ANNO_S4DISPRULE_FIELD		= ( _T("S4DISPRULE"));		// S4���L�L�����[��
const static LPCTSTR ANNO_S5DISPRULE_FIELD		= ( _T("S5DISPRULE"));		// S5(�s�s�n�})���L�L�����[��
const static LPCTSTR ANNO_TRUE_POSITION_F_FIELD = ( _T("TRUEPOSITION_F"));	// �^�ʒu�ʒu
const static LPCTSTR ANNO_EXCEPT_SCALE_F_FIELD	= ( _T("EXCEPTSCALE_F"));	// ������
const static LPCTSTR ANNO_DOUBLE_ANNO_F_FIELD	= ( _T("DOUBLEANNO_F"));	// ��s���L����
const static LPCTSTR ANNO_COPYPASTE_F_FIELD		= ( _T("COPYPASTE_F"));		// �R�s�[����
const static LPCTSTR ANNO_IDENTICALDATA_F_FIELD	= ( _T("IDENTICALDATA_F"));	// ����f�[�^���݋���
const static LPCTSTR ONEWAY_C_FIELD				= ( _T("ONEWAY_C"));
const static LPCTSTR NOPASSAGE_C_FIELD			= ( _T("NOPASSAGE_C"));
const static LPCTSTR DAYOFWEEK1_C_FIELD			= ( _T("DAYOFWEEK1_C"));
const static LPCTSTR DAYOFWEEK2_C_FIELD			= ( _T("DAYOFWEEK2_C"));
const static LPCTSTR DAYOFWEEK3_C_FIELD			= ( _T("DAYOFWEEK3_C"));
const static LPCTSTR DAYOFWEEK4_C_FIELD			= ( _T("DAYOFWEEK4_C"));
const static LPCTSTR TURNREG_ID_FIELD			= ( _T("TURNREG_ID"));
const static LPCTSTR VICS_ID_FIELD				= ( _T("VICS_ID"));
const static LPCTSTR VICSCLASS_C_FIELD			= ( _T("VICSCLASS_C"));
const static LPCTSTR VICS_LINK_ID_FIELD			= ( _T("VICS_LINK_ID"));
const static LPCTSTR BACKROAD_F_FIELD			= ( _T("BACKROAD_F"));
const static LPCTSTR BYWAY_F_FIELD				= ( _T("BYWAY_F"));
const static LPCTSTR ROUTECLASS_C_FIELD			= ( _T("ROUTECLASS_C"));

/// �t�B�[���h����(���L���[���֘A)
const static LPCTSTR ANNO_CLASS_FIELD				= ( _T("CLASS"));				// ���L���
const static LPCTSTR ANNO_MS_NO_FIELD				= ( _T("MS_NO"));				// ���e�ԍ�
const static LPCTSTR ANNO_CLASS_NAME_FIELD			= ( _T("CLASS_NAME"));			// ���L��ʖ���
const static LPCTSTR ANNO_TOP_FLAG_FIELD			= ( _T("TOP_FLAG"));			// �g�b�v�g�p��
const static LPCTSTR ANNO_MIDDLE_FLAG_FIELD			= ( _T("MIDDLE_FLAG"));			// �~�h���g�p��
const static LPCTSTR ANNO_BASE_FLAG_FIELD			= ( _T("BASE_FLAG"));			// ���k�g�p��
const static LPCTSTR ANNO_CITY_FLAG_FIELD			= ( _T("CITY_FLAG"));			// �s�s�n�}�g�p��
const static LPCTSTR ANNO_DISP_TYPE_FIELD			= ( _T("DISP_TYPE"));			// ���L�\���^�C�v
const static LPCTSTR ANNO_T_S1DISPRULE_FIELD		= ( _T("T_S1DISPRULE"));		// �g�b�vS1���L�L�����[��
const static LPCTSTR ANNO_T_S2DISPRULE_FIELD		= ( _T("T_S2DISPRULE"));		// �g�b�vS2���L�L�����[��
const static LPCTSTR ANNO_T_S3DISPRULE_FIELD		= ( _T("T_S3DISPRULE"));		// �g�b�vS3���L�L�����[��
const static LPCTSTR ANNO_T_S4DISPRULE_FIELD		= ( _T("T_S4DISPRULE"));		// �g�b�vS4���L�L�����[��
const static LPCTSTR ANNO_T_TRUE_POSITION_F_FIELD	= ( _T("T_TRUEPOSITION_F"));	// �g�b�v�^�ʒu�ʒu
const static LPCTSTR ANNO_T_FONTSIZE_FIELD			= ( _T("T_FONTSIZE"));			// �g�b�v�t�H���g�T�C�Y
const static LPCTSTR ANNO_T_EXCEPT_SCALE_F_FIELD	= ( _T("T_EXCEPTSCALE_F"));		// �g�b�v������
const static LPCTSTR ANNO_T_DOUBLE_ANNO_F_FIELD		= ( _T("T_DOUBLEANNO_F"));		// �g�b�v��s���L����
const static LPCTSTR ANNO_T_COPYPASTE_F_FIELD		= ( _T("T_COPYPASTE_F"));		// �g�b�v�R�s�[����
const static LPCTSTR ANNO_T_IDENTICALDATA_F_FIELD	= ( _T("T_IDENTICALDATA_F"));	// �g�b�v����f�[�^���݋���
const static LPCTSTR ANNO_M_S1DISPRULE_FIELD		= ( _T("M_S1DISPRULE"));		// �~�h��S1���L�L�����[��
const static LPCTSTR ANNO_M_S2DISPRULE_FIELD		= ( _T("M_S2DISPRULE"));		// �~�h��S2���L�L�����[��
const static LPCTSTR ANNO_M_S3DISPRULE_FIELD		= ( _T("M_S3DISPRULE"));		// �~�h��S3���L�L�����[��
const static LPCTSTR ANNO_M_S4DISPRULE_FIELD		= ( _T("M_S4DISPRULE"));		// �~�h��S4���L�L�����[��
const static LPCTSTR ANNO_M_TRUE_POSITION_F_FIELD	= ( _T("M_TRUEPOSITION_F"));	// �~�h���^�ʒu�ʒu
const static LPCTSTR ANNO_M_FONTSIZE_FIELD			= ( _T("M_FONTSIZE"));			// �~�h���t�H���g�T�C�Y
const static LPCTSTR ANNO_M_EXCEPT_SCALE_F_FIELD	= ( _T("M_EXCEPTSCALE_F"));		// �~�h��������
const static LPCTSTR ANNO_M_DOUBLE_ANNO_F_FIELD		= ( _T("M_DOUBLEANNO_F"));		// �~�h����s���L����
const static LPCTSTR ANNO_M_COPYPASTE_F_FIELD		= ( _T("M_COPYPASTE_F"));		// �~�h���R�s�[����
const static LPCTSTR ANNO_M_IDENTICALDATA_F_FIELD	= ( _T("M_IDENTICALDATA_F"));	// �~�h������f�[�^���݋���
const static LPCTSTR ANNO_B_S1DISPRULE_FIELD		= ( _T("B_S1DISPRULE"));		// ���kS1���L�L�����[��
const static LPCTSTR ANNO_B_S2DISPRULE_FIELD		= ( _T("B_S2DISPRULE"));		// ���kS2���L�L�����[��
const static LPCTSTR ANNO_B_S3DISPRULE_FIELD		= ( _T("B_S3DISPRULE"));		// ���kS3���L�L�����[��
const static LPCTSTR ANNO_B_S4DISPRULE_FIELD		= ( _T("B_S4DISPRULE"));		// ���kS4���L�L�����[��
const static LPCTSTR ANNO_B_TRUE_POSITION_F_FIELD	= ( _T("B_TRUEPOSITION_F"));	// ���k�^�ʒu�ʒu
const static LPCTSTR ANNO_B_FONTSIZE_FIELD			= ( _T("B_FONTSIZE"));			// ���k�t�H���g�T�C�Y
const static LPCTSTR ANNO_B_EXCEPT_SCALE_F_FIELD	= ( _T("B_EXCEPTSCALE_F"));		// ���k������
const static LPCTSTR ANNO_B_DOUBLE_ANNO_F_FIELD		= ( _T("B_DOUBLEANNO_F"));		// ���k��s���L����
const static LPCTSTR ANNO_B_COPYPASTE_F_FIELD		= ( _T("B_COPYPASTE_F"));		// ���k�R�s�[����
const static LPCTSTR ANNO_B_IDENTICALDATA_F_FIELD	= ( _T("B_IDENTICALDATA_F"));	// ���k����f�[�^���݋���
const static LPCTSTR ANNO_C_SDISPRULE_FIELD			= ( _T("C_SDISPRULE"));			// �s�s�n�}���L�L�����[��
const static LPCTSTR ANNO_C_TRUE_POSITION_F_FIELD	= ( _T("C_TRUEPOSITION_F"));	// �s�s�n�}�^�ʒu�ʒu
const static LPCTSTR ANNO_C_FONTSIZE_FIELD			= ( _T("C_FONTSIZE"));			// �s�s�n�}�t�H���g�T�C�Y
const static LPCTSTR ANNO_C_EXCEPT_SCALE_F_FIELD	= ( _T("C_EXCEPTSCALE_F"));		// �s�s�n�}������
const static LPCTSTR ANNO_C_DOUBLE_ANNO_F_FIELD		= ( _T("C_DOUBLEANNO_F"));		// �s�s�n�}��s���L����
const static LPCTSTR ANNO_C_COPYPASTE_F_FIELD		= ( _T("C_COPYPASTE_F"));		// �s�s�n�}�R�s�[����
const static LPCTSTR ANNO_C_IDENTICALDATA_F_FIELD	= ( _T("C_IDENTICALDATA_F"));	// �s�s�n�}����f�[�^���݋���


/// �t�B�[���h���́i�w�i�֘A�j
const static LPCTSTR BACKGROUND_CLASS				=( _T("CLASS"));			// ��ʃR�[�h
const static LPCTSTR BACKGROUND_CLASS_NAME			=( _T("CLASS_NAME"));		// �w�i��ʖ���
const static LPCTSTR BACKGROUND_MS_NO				=( _T("MS_NO"));			// ���e�ԍ�
const static LPCTSTR BACKGROUND_LAYERID				=( _T("LAYERID"));			// ���C��ID

/// NULL OK �ȃt�B�[���h��NULL�l��`��
#define NULL_VALUE     (_T("<null>"))
#define EMPTY_VALUE    (_T("<empty>"))
#define TRUE_VALUE     (_T("<true>"))
#define FALSE_VALUE    (_T("<false>"))
#define GEOMETRY_VALUE (_T("<shape>"))

// ���[�j���O���x��
const static int WARNLEVEL_NONE   = 0;	//!< �����Ȃ��H�̃��[�j���O���x��
const static int WARNLEVEL_LOW    = 1;	//!< ���ʌ������[�j���O���x��
const static int WARNLEVEL_MIDDLE = 2;	//!< ���ʌ������[�j���O���x��
const static int WARNLEVEL_HIGH   = 3;	//!< ��ʌ������[�j���O���x��

// ���L�萔
//#ifdef _USE_ANNOTATION_TRICKY_DEFINE
#define ANNO_CITY_1PT			0.000008950		// �s�s�n�}���L������(1pt)
#define ANNO_BASE_S4_1PT		0.000035808		// ���kS4���L������(1pt)
#define ANNO_BASE_S3_1PT		0.000071608		// ���kS3���L������(1pt)
#define ANNO_BASE_S2_1PT		0.000143233		// ���kS2���L������(1pt)
#define ANNO_BASE_S1_1PT		0.000286458		// ���kS1���L������(1pt)
#define ANNO_MIDDLE_S4_1PT		0.000572800		// �~�h��S4���L������(1pt)
#define ANNO_MIDDLE_S3_1PT		0.001145866		// �~�h��S3���L������(1pt)
#define ANNO_MIDDLE_S2_1PT		0.002291733		// �~�h��S2���L������(1pt)
#define ANNO_MIDDLE_S1_1PT		0.004583333		// �~�h��S1���L������(1pt)
#define ANNO_TOP_S4_1PT			0.009166666		// �g�b�vS4���L������(1pt)	�O�̂�{�ɂ��������B���Ƃ��ƍ����ĂȂ��̂Ń`���[�Ă��Ɓ[
#define ANNO_TOP_S3_1PT			0.018333332		// �g�b�vS3���L������(1pt)	�O�̂�{�ɂ��������B���Ƃ��ƍ����ĂȂ��̂Ń`���[�Ă��Ɓ[
#define ANNO_TOP_S2_1PT			0.036666664		// �g�b�vS2���L������(1pt)	�O�̂�{�ɂ��������B���Ƃ��ƍ����ĂȂ��̂Ń`���[�Ă��Ɓ[
#define ANNO_TOP_S1_1PT			0.073333328		// �g�b�vS1���L������(1pt)	�O�̂�{�ɂ��������B���Ƃ��ƍ����ĂȂ��̂Ń`���[�Ă��Ɓ[
//#endif // ifdef _USE_ANNOTATION_TRICKY_DEFINE

#define ANNO_NONE_MULTI			1.363682647		// �����Ԋu�Ȃ��搔
#define ANNO_HALF_MULTI			1.931841325		// ���p�搔
#define ANNO_FULL_MULTI			2.613646288		// �S�p�搔
#define ANNO_DOUBLE_MULTI		3.977328935		// ��{�p�搔
//#define ANNO_ROAD_MULTI			1.705074488		// ���H�ԍ��E��v�n�����ԍ��搔 <- [bug 3974] ����1.7�{����́H�H�킯�킩���̂Ŕp�~

#define ANNO_POINT_DIFF_MULTI	1.092295158		// �t�H���g�T�C�Y���̏搔 ((������*�搔)*(xPoint-12Point))+(������*12Point) ��xPoint�̕����̕�����

#define	ANNO_HEIGHT_MULTI		1.261034048		// ���L�c�Ԋu�搔(1pt)

#define ANNO_CITY_SMALL_MAKR_RADIUS		5.460893855251		// �s�s�n�}���L���L�����a(Point) [�L���ƕ�����܂ł̋���]
#define ANNO_CITY_MARK_RADIUS			10.907821228503		// �s�s�n�}���L�L�����a(Point)
#define ANNO_BASE_SMALL_MARK_RADIUS		5.5186714682		// ���k���L���L�����a(Point)
#define ANNO_BASE_MARK_RADIUS			8.1396951808		// ���k���L�L�����a(Point)
#define ANNO_MIDDLE_SMALL_MARK_RADIUS	5.5186714682		// �~�h�����L���L�����a(Point)
#define ANNO_MIDDLE_MARK_RADIUS			8.1396951808		// �~�h�����L�L�����a(Point)
#define ANNO_TOP_SMALL_MARK_RADIUS		5.5186714682		// �g�b�v���L���L�����a(Point)
#define ANNO_TOP_MARK_RADIUS			8.1396951808		// �g�b�v���L�L�����a(Point)
#define ANNO_MARK_MULTI					0.8					// �L���T�C�Y�搔
//#define ANNO_MARK_TO_LEFT_MULTI			1.4					// �L���̍������搔(CET�ł́A�E�ƍ��ŁA���̕���������) <- [bug 3039] �����������̂Ŕp�~

#define ANNO_CITY_SCALE			3125.0		// �s�s�n�}��X�P�[��
#define ANNO_BASE_S4_SCALE		12500.0		// ���kS4��X�P�[��
#define ANNO_BASE_S3_SCALE		25000.0		// ���kS3��X�P�[��
#define ANNO_BASE_S2_SCALE		50000.0		// ���kS2��X�P�[��
#define	ANNO_BASE_S1_SCALE		100000.0	// ���kS1��X�P�[��
#define ANNO_MIDDLE_S4_SCALE	200000.0	// �~�h��S4��X�P�[��
#define ANNO_MIDDLE_S3_SCALE	400000.0	// �~�h��S3��X�P�[��
#define ANNO_MIDDLE_S2_SCALE	800000.0	// �~�h��S2��X�P�[��
#define	ANNO_MIDDLE_S1_SCALE	1600000.0	// �~�h��S1��X�P�[��
#define ANNO_TOP_S4_SCALE		3200000.0	// �g�b�vS4��X�P�[��
#define ANNO_TOP_S3_SCALE		6400000.0	// �g�b�vS3��X�P�[��
#define ANNO_TOP_S2_SCALE		12800000.0	// �g�b�vS2��X�P�[��
#define	ANNO_TOP_S1_SCALE		25600000.0	// �g�b�vS1��X�P�[��

// �g�p���郌�W�X�g���L�[
#define ROOT_KEY			HKEY_LOCAL_MACHINE
#define SOFTWARE_KEY		(_T("Software\\"))
#define INSTALL_KEY			(_T("INCREMENT P CORPORATION\\SiNDY-e\\"))
#define INSTALLROOT_KEY		(_T("InstallRoot"))
#define TOOLS_KEY			(_T("Tools\\"))		// Tools\�e�c�[�����Ƃ��邱�Ƃ�O�� HKEY_CURRENT_USER�ȉ��ɏ�������
#define PATH_KEY			(_T("Path"))
#define INIFILEPATH_KEY		(_T("InitFileName"))
#define CLSID_KEY			(_T("clsid"))
#define DEFAULTSYMBOL_KEY	(_T("�f�t�H���g"))
#define DEFAULT_KEY			(_T("unset"))		/// �_�~�[

#define	WARNING_SECTION		(_T("Warning"))
#define	LEVEL_KEY			(_T("Level"))

// �w���vID
//#include "HelpID.h"

//#define CURRENTUSER_KEY (_T("SOFTWARE\\Increment P Corporation\\SiNDY-e\\CurrentUser"))
//#define WORKNAME_KEY (_T("SOFTWARE\\Increment P Corporation\\SiNDY-e\\Work"))

//
// IMultiSymbolRenderer �Ŏg�p����`�惂�[�h
#define _MULTISYMBOL_VERTEX		1	//!< �\���_�`�惂�[�h
#define _MULTISYMBOL_ONEWAY		2	//!< ����ʍs�`�惂�[�h
#define _MULTISYMBOL_ANNOTATION	4	//!< ���L�`�惂�[�h
#define _MULTISYMBOL_NOLRTURN	8	//!< �E���܋֎~�`�惂�[�h
#define _MULTISYMBOL_GUIDE		16	//!< �����U���`�惂�[�h
#define _MULTISYMBOL_DIRGUIDE	32	//!< ���ʈē��`�惂�[�h
#define _MULTISYMBOL_VICS		64	//!< VICS�`�惂�[�h
#define _MULTISYMBOL_LANE		128	//!< ���[�����`�惂�[�h
#define _MULTISYMBOL_BYWAY		256	//!< �������`�惂�[�h
#define _MULTISYMBOL_S1ANNOPOINT	512	    //!< S1�^�ʒu�`�惂�[�h
#define _MULTISYMBOL_S2ANNOPOINT	1024	//!< S2�^�ʒu�`�惂�[�h
#define _MULTISYMBOL_S3ANNOPOINT	2048	//!< S3�^�ʒu�`�惂�[�h
#define _MULTISYMBOL_S4ANNOPOINT	4096	//!< S4�^�ʒu�`�惂�[�h
#define _MULTISYMBOL_ROUTE			8192	//!< �A�N�Z�X�^���p�����`�惂�[�h
#define _MULTISYMBOL_UTURN			16384	//!< U�^�[���\�ӏ��`�惂�[�h
#define _MULTISYMBOL_NOPASSAGE		32768	//!< �ʍs�֎~�K���`�惂�[�h
#define _MULTISYMBOL_INTERNAVI		65536	//!< �C���^�[�i�rVICS�`�惂�[�h
#define _MULTISYMBOL_ENDPOINT		131072	//!< �\���_�`�惂�[�h
#define _MULTISYMBOL_IPCVICS		262144	//!< IPC VICS�`�惂�[�h [Bug 6473]Inf_ipcVics�ALq_ipcVics�ɑΉ����ė~����
#define _MULTISYMBOL_INTERSECTION	524288	//!< �i�������ʌ����_�`�惂�[�h

// �c�[��
enum EDITOR_TOOL
{
	EDITTOOL,			//!< �t�B�[�`���ړ��c�[��
	SKETCHTOOL,			//!< �t�B�[�`���쐬�c�[��
	DELETETOOL,			//!< �t�B�[�`���폜�c�[��
	ATTRIBUTETOOL,		//!< �t�B�[�`�������ҏW�c�[��
};

// LQ �p
class ROWDEF
{
public:
	ROWDEF() : oid(-1), flg(-1), direction(-1), sequence(-1) {};
	~ROWDEF(){};
	LONG			oid;		// IRow�AIFeature �p
	_IRowPtr		obj;		// IRow�AIFeature �p
	IGeometryPtr	shp;		// IFeature �p
	LONG			flg;		// LQ �̃^�C�v�ێ��p
#if _ATL_VER < 0x0800
	CString			name;		// �e�[�u�����Ȃ�
#else
	ATL::CString    name;		// �e�[�u�����Ȃ�
#endif // if _ATL_VER < 0x0800
	LONG			direction;	// LINKDIR_C �i�[�p�i���H�����N���i�[����ꍇ�̂݁j
	LONG			sequence;	// SEQUENCE �i�[�p�i���H�����N���i�[����ꍇ�̂݁j
};

// �`��p
struct SYMBOL {
	ISimpleLineSymbolPtr			piNormalSymbol;
	ISimpleLineSymbolPtr			piNormalSymbolNarrow;
	ICartographicLineSymbolPtr		piArrowSymbol;
	ICartographicLineSymbolPtr		piArrowSymbolNarrow;
};

struct LQ {
	BOOL			bDrawAllow;
	BOOL			bIsNarrow;
	BOOL			bIsDispID;
	LONG			ID;
	COLORREF		ID_Color;
	COLORREF		Color;
	DOUBLE			Size;
	IGeometryPtr	piGeom;
	LONG			DrawMode;
};

inline bool __stdcall operator<(const ROWDEF& c1, const ROWDEF& c2){ return ( c1.sequence < c2.sequence ); };
inline bool __stdcall operator>(const ROWDEF& c1, const ROWDEF& c2){ return ( c1.sequence > c2.sequence ); };

// �F��`
const static COLORREF _COLOR_SELECTEXTENT = RGB(  100,   72,   41 );	//!< �I�[�o�[�r���[�E�����N��ҏW�c�[���̑I��͈͐F

// Program ID
const static LPCTSTR EDITTOOL_UID		= _T("EditTool.EditToolObj");
const static LPCTSTR DELETETOOL_UID		= _T("DeleteTool.DeleteToolObj");
const static LPCTSTR SKETCHTOOL_UID		= _T("SketchTool.SketchToolObj");
const static LPCTSTR ATTRTOOL_UID		= _T("AttributeTool.AttributeToolObj");
const static LPCTSTR LQEDITTOOL_UID		= _T("LQEditTool.LQEditToolObj");
const static LPCTSTR LQATTRTOOL_UID		= _T("LQAttributeTool.LQAttributeToolObj");
const static LPCTSTR LQSKETCHTOOL_UID	= _T("LQSketchTool.LQSketchToolObj");
const static LPCTSTR LQDELETETOOL_UID	= _T("LQDeleteTool.LQDeleteToolObj");
const static LPCTSTR SEARCHTOOL_UID		= _T("SearchTool.SearchToolObj");
const static LPCTSTR COPYTOOL_UID		= _T("FeatureCopyTool.FeatureCopyToolObj");
const static LPCTSTR MEARGETOOL_UID		= _T("MergeTool.MergeToolObj");
//const static LPCTSTR ERRJMPTOOL_UID		= _T("ErrorJumpTool.ErrorJumpToolObj");
const static LPCTSTR LOGHANDLETOOL_UID		= _T("LogHandleTool.LogHandleToolObj");
const static LPCTSTR SPLITTOOL_UID		= _T("SplitTool.SplitToolObj");
const static LPCTSTR OVERVIEWTOOL_UID	= _T("OverViewTool.OverViewToolObj");
const static LPCTSTR VIEWVERTOOL_UID	= _T("ViewVersionTool.ViewVersionToolObj");
const static LPCTSTR CHGSYMBOL_UID		= _T("ChangeSymbolMenu.ChangeSymbolMultiSpatialObj");
const static LPCTSTR GRADWIZA_UID		= _T("GradWizard.GradWizard");
const static LPCTSTR GRADEDITCMD_UID	= _T("GradEditTool.GradEditCmd");
const static LPCTSTR LDRASTERCMD_UID	= _T("LoadRasterCommand.LoadRasterCommandObj");
const static LPCTSTR MSICMD_UID			= _T("MSICommand.MSICommandObj");
const static LPCTSTR LOCALMENU_UID		= _T("LocalEditMenu.LocalEditMultiObj");
const static LPCTSTR LOCALEDIT_UID		= _T("LocalEditMgrCommand.LocalEditMgrCommandObj");
const static LPCTSTR MAP_UID			= _T("Map.Start");
const static LPCTSTR MULTI_UID			= _T("MultiRenderer.MultiRendererObj");
const static LPCTSTR ROADSNAP_UID		= _T("RoadIntersectionSnap.RoadIntersectionSnapAgent");
const static LPCTSTR BOOKMARKMENU_UID	= _T("MapBookMarkMenu.MapBookMarkMenuObj");
const static LPCTSTR ADDRESSBAR_UID		= _T("AddressToolBar.AddressToolBarObj");
const static LPCTSTR ATTRDLG_UID		= _T("AttributeDlg.AttributeDlgObj");
const static LPCTSTR FEATSELECTDLG_UID	= _T("FeatureSelectDlg.FeatureSelectDlgObj");
const static LPCTSTR TXTSTAMPER_UID		= _T("TextStamper.TextStamperObj");
const static LPCTSTR SNAPTOOL_UID		= _T("sindyEditor.SiNDYSnapCommand");
const static LPCTSTR V3DVIEWTOOL_UID	= _T("V3DViewTool.V3DViewToolObj");
const static LPCTSTR DRVSURVEYTOOL_UID	= _T("DrvSurvey.DrvSrvyToolObj");
const static LPCTSTR CHGSYMBOLVIEW_UID	= _T("ChangeSymbolViewTool.ChangeSymbolViewToolObj");
const static LPCTSTR TRAFFICREGULATIONTOOL_UID	= _T("TrafficRegulationTool.TrafficRegulationToolObj");
const static LPCTSTR CONVHPGLTOOL_UID	= _T("Conv.ConvHpglObj");
const static LPCTSTR LOCATIONVIEWTOOL_UID	= _T("LocationView.LocationViewToolObj");
const static LPCTSTR SPOTVIEW_UID		= _T("SpotView.SpotViewObj");
const static LPCTSTR TRACKLOGMGR_UID	= _T("TrackLogMgr.TrackLogMgrObj");
const static LPCTSTR TRACKPOINTVIEW_UID	= _T("TrackPointView.TrackPointViewObj");
const static LPCTSTR TRACKLOGSEARCH_UID	= _T("TrackLogSearch.TrackLogSearchObj");

#define PROP_PRIORITY_COMMON				10000;
#define PROP_PRIORITY_ATTRIBUTETOOL			5000;
#define PROP_PRIORITY_EDITTOOL				4999;
#define PROP_PRIORITY_DELETETOOL			4900;
#define PROP_PRIORITY_SKETCHTOOL			4800;
#define PROP_PRIORITY_COPYTOOL				4700;
#define PROP_PRIORITY_TRAFFICREGULATIONTOOL	4600;
#define PROP_PRIORITY_LQDELETETOOL			3900;
#define PROP_PRIORITY_LQSKETCHTOOL			3800;
#define PROP_PRIORITY_LQATTRIBUTETOOL		3700;
#define PROP_PRIOTIRY_RULE					10;

void _cdecl DEBUGPRINT( LPCTSTR lpszFormat, ...);
inline void _cdecl DEBUGPRINT(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
#ifndef UNICODE
	char szBuffer[512];

	nBuf = _vsnprintf_s(szBuffer, sizeof(szBuffer), lpszFormat, args);
	ATLASSERT(nBuf < sizeof(szBuffer)); //Output truncated as it was > sizeof(szBuffer)

	OutputDebugStringA(szBuffer);
#else
	WCHAR szBuffer[512];

	nBuf = _vsnwprintf_s(szBuffer, sizeof(szBuffer) / sizeof(WCHAR), lpszFormat, args);
	ATLASSERT(nBuf < sizeof(szBuffer));//Output truncated as it was > sizeof(szBuffer)

	OutputDebugStringW(szBuffer);
#endif
	va_end(args);
}

#endif
