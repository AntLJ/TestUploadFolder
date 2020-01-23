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
#include <sindy/schema/rule.h>

namespace sindy {

namespace schema {

namespace annotation_class {

	const _TCHAR kTableName[]			= _T("ANNOTATION_CLASS");	//!< �e�[�u����
	const _TCHAR kClass[]				= _T("CLASS");				//!< SiNDY���L��ʃR�[�h
	const _TCHAR kMSNo[]				= _T("MS_NO");				//!< ���e�ԍ�
	const _TCHAR kInClass[]				= _T("IN_CLASS");			//!< IN���L��ʃR�[�h
	const _TCHAR kClassName[]			= _T("CLASS_NAME");			//!< ��ʖ���
	const _TCHAR kTopFlag[]				= _T("TOP_FLAG");			//!< TOP_FLAG
	const _TCHAR kMiddleFlag[]			= _T("MIDDLE_FLAG");		//!< MIDDLE_FLAG
	const _TCHAR kBaseFlag[]			= _T("BASE_FLAG");			//!< BASE_FLAG
	const _TCHAR kCityFlag[]			= _T("CITY_FLAG");			//!< CITY_FLAG
	const _TCHAR kDispType[]			= _T("DISP_TYPE");			//!< �\���^�C�v
	const _TCHAR kTS1DispRule[]			= _T("T_S1DISPRULE");		//!< �g�b�v�}�b�vS1�\�����[��
	const _TCHAR kTS2DispRule[]			= _T("T_S2DISPRULE");		//!< �g�b�v�}�b�vS2�\�����[��
	const _TCHAR kTS3DispRule[]			= _T("T_S3DISPRULE");		//!< �g�b�v�}�b�vS3�\�����[��
	const _TCHAR kTS4DispRule[]			= _T("T_S4DISPRULE");		//!< �g�b�v�}�b�vS4�\�����[��
	const _TCHAR kTTruePosition[]		= _T("T_TRUEPOSITION_F");	//!< �g�b�v�}�b�v�^�ʒu�t���O
	const _TCHAR kTFontSize[]			= _T("T_FONTSIZE");			//!< �g�b�v�}�b�v�t�H���g�T�C�Y
	const _TCHAR kTExceptScale[]		= _T("T_EXCEPTSCALE_F");	//!< �g�b�v�}�b�v�X�P�[���̒������t���O
	const _TCHAR kTDoubleAnno[]			= _T("T_DOUBLEANNO_F");		//!< �g�b�v�}�b�v�Q�s���L�t���O
	const _TCHAR kTCopyPaste[]			= _T("T_COPYPASTE_F");		//!< �g�b�v�}�b�v�R�s�[�y�[�X�g�t���O
	const _TCHAR kTIDenticalData[]		= _T("T_IDENTICALDATA_F");	//!< �g�b�v�}�b�v����f�[�^�̑��݃t���O
	const _TCHAR kMS1DispRule[]			= _T("M_S1DISPRULE");		//!< �~�h���}�b�vS1�\�����[��
	const _TCHAR kMS2DispRule[]			= _T("M_S2DISPRULE");		//!< �~�h���}�b�vS2�\�����[��
	const _TCHAR kMS3DispRule[]			= _T("M_S3DISPRULE");		//!< �~�h���}�b�vS3�\�����[��
	const _TCHAR kMS4DispRule[]			= _T("M_S4DISPRULE");		//!< �~�h���}�b�vS4�\�����[��
	const _TCHAR kMTruePosition[]		= _T("M_TRUEPOSITION_F");	//!< �~�h���}�b�v�^�ʒu�t���O
	const _TCHAR kMFontSize[]			= _T("M_FONTSIZE");			//!< �~�h���}�b�v�t�H���g�T�C�Y
	const _TCHAR kMExceptScale[]		= _T("M_EXCEPTSCALE_F");	//!< �~�h���}�b�v�X�P�[���̒������t���O
	const _TCHAR kMDoubleAnno[]			= _T("M_DOUBLEANNO_F");		//!< �~�h���}�b�v�Q�s���L�t���O
	const _TCHAR kMCopyPaste[]			= _T("M_COPYPASTE_F");		//!< �~�h���}�b�v�R�s�[�y�[�X�g�t���O
	const _TCHAR kMIDenticalData[]		= _T("M_IDENTICALDATA_F");	//!< �~�h���}�b�v����f�[�^�̑��݃t���O
	const _TCHAR kBS1DispRule[]			= _T("B_S1DISPRULE");		//!< �x�[�X�}�b�vS1�\�����[��
	const _TCHAR kBS2DispRule[]			= _T("B_S2DISPRULE");		//!< �x�[�X�}�b�vS2�\�����[��
	const _TCHAR kBS3DispRule[]			= _T("B_S3DISPRULE");		//!< �x�[�X�}�b�vS3�\�����[��
	const _TCHAR kBS4DispRule[]			= _T("B_S4DISPRULE");		//!< �x�[�X�}�b�vS4�\�����[��
	const _TCHAR kBTruePosition[]		= _T("B_TRUEPOSITION_F");	//!< �x�[�X�}�b�v�^�ʒu�t���O
	const _TCHAR kBFontSize[]			= _T("B_FONTSIZE");			//!< �x�[�X�}�b�v�t�H���g�T�C�Y
	const _TCHAR kBExceptScale[]		= _T("B_EXCEPTSCALE_F");	//!< �x�[�X�}�b�v�X�P�[���̒������t���O
	const _TCHAR kBDoubleAnno[]			= _T("B_DOUBLEANNO_F");		//!< �x�[�X�}�b�v�Q�s���L�t���O
	const _TCHAR kBCopyPaste[]			= _T("B_COPYPASTE_F");		//!< �x�[�X�}�b�v�R�s�[�y�[�X�g�t���O
	const _TCHAR kBIDenticalData[]		= _T("B_IDENTICALDATA_F");	//!< �x�[�X�}�b�v����f�[�^�̑��݃t���O
	const _TCHAR kCDispRule[]			= _T("C_SDISPRULE");		//!< �s�s�n�}�\�����[��
	const _TCHAR kCTruePosition[]		= _T("C_TRUEPOSITION_F");	//!< �s�s�n�}�^�ʒu�t���O
	const _TCHAR kCFontSize[]			= _T("C_FONTSIZE");			//!< �s�s�n�}�t�H���g�T�C�Y
	const _TCHAR kCExceptScale[]		= _T("C_EXCEPTSCALE_F");	//!< �s�s�n�}�X�P�[���̒������t���O
	const _TCHAR kCDoubleAnno[]			= _T("C_DOUBLEANNO_F");		//!< �s�s�n�}�Q�s���L�t���O
	const _TCHAR kCCopyPaste[]			= _T("C_COPYPASTE_F");		//!< �s�s�n�}�R�s�[�y�[�X�g�t���O
	const _TCHAR kCIDenticalData[]		= _T("C_IDENTICALDATA_F");	//!< �s�s�n�}����f�[�^�̑��݃t���O
	const _TCHAR kRotationRule[]		= _T("ROT_RULE");			//!< ��]�Ɋ֘A���郋�[��
} // annotation_class
namespace background_class {

	const _TCHAR kTableName[]			= _T("BACKGROUND_CLASS");	//!< �e�[�u����

	const _TCHAR kClass[]				= _T("CLASS");				//!< ��ʃR�[�h
	const _TCHAR kClassName[]			= _T("CLASS_NAME");			//!< ��ʖ���
	const _TCHAR kMSNo[]				= _T("MS_NO");				//!< ���e�ԍ�
	const _TCHAR kLayerID[]				= _T("LAYERID");			//!< �w�i���C����ID
	const _TCHAR kUnderGround[]			= _T("UNDERGROUND_F");		//!< �n���t���O
	const _TCHAR kInClass[]				= _T("IN_CLASS");			//!< IN��ʃR�[�h
} // background_class
namespace alley_mainte_list {

	const _TCHAR kTableName[]			= _T("ALLEY_MAINTE_LIST");	//!< �e�[�u����

	const _TCHAR kAlleyMainteList[]		= _T("ALLEY_MAINTE_LIST");	//!< �׊X�H��Ə��R�[�h
	const _TCHAR kWorkYear[]			= _T("WORK_YEAR");			//!< ��ƔN�x
	const _TCHAR Regulation[]			= _T("REGULATION_F");		//!< �K�������t���O
} // allay_mainte_list
namespace connectuser {

	const _TCHAR kTableName[]			= _T("CONNECTUSER");		//!< �e�[�u����

	const _TCHAR kConnectUserID[]		= _T("CONNECTUSR_ID");		//!< �ڑ����[�UID
	const _TCHAR kName[]				= _T("NAME");				//!< �ڑ����[�U��
} // connectuser
namespace creatablelayers {

	const _TCHAR kTableName[]			= _T("CREATABLELAYERS");	//!< �e�[�u����

	const _TCHAR kPurposeID[]			= _T("PURPOSE_ID");			//!< ��ƖړIID
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< ���C����ID
	const _TCHAR kAutoFlag[]			= _T("AUTO_F");				//!< �����쐬�t���O
	const _TCHAR kAutoValue[]			= _T("AUTO_VALUE");			//!< �����쐬�����l
} // creatablelayers
namespace deletablelayers {

	const _TCHAR kTableName[]			= _T("DELETABLELAYERS");	//!< �e�[�u����

	const _TCHAR kPurposeID[]			= _T("PURPOSE_ID");			//!< ��ƖړIID
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< ���C����ID
	const _TCHAR kRistrict1[]			= _T("RISTRICT1");			//!< �����������[���P
	const _TCHAR kRistrict2[]			= _T("RISTRICT2");			//!< �����������[���Q
	const _TCHAR kRistrict3[]			= _T("RISTRICT3");			//!< �����������[���R
	const _TCHAR kRistrict4[]			= _T("RISTRICT4");			//!< �����������[���S
	const _TCHAR kRistrict5[]			= _T("RISTRICT5");			//!< �����������[���T
	const _TCHAR kRistrict6[]			= _T("RISTRICT6");			//!< �����������[���U
	const _TCHAR kRistrict7[]			= _T("RISTRICT7");			//!< �����������[���V
	const _TCHAR kRistrict8[]			= _T("RISTRICT8");			//!< �����������[���W
	const _TCHAR kRistrict9[]			= _T("RISTRICT9");			//!< �����������[���X
	const _TCHAR kRistrict10[]			= _T("RISTRICT10");			//!< �����������[���P�O
} // deletablelayers
namespace dispnavirelation {

	const _TCHAR kTableName[]			= _T("DISPNAVIRELATION");	//!< �e�[�u����

	const _TCHAR kDispClassID[]			= _T("DISPCLASS_ID");		//!< ���H�����N�\�����ID
	const _TCHAR kNaviClassID[]			= _T("NAVICLASS_ID");		//!< ���H�����N�o�H���ID
	const _TCHAR kRelValue[]			= _T("RELVALUE");			//!< ���[���l
} // dispnavirelation
namespace dispotherrelation {

	const _TCHAR kTableName[]			= _T("DISPOTHERRELATION");	//!< �e�[�u����

	const _TCHAR kDispClassID[]			= _T("DISPCLASS_ID");		//!< ���H�����N�\�����ID
	const _TCHAR kOtherAttrID[]			= _T("OTHERATTR_ID");		//!< ���H�����N����ID
	const _TCHAR kRelValue[]			= _T("RELVALUE");			//!< ���[���l
} // dispotherrelation
namespace editattrlayers {

	const _TCHAR kTableName[]			= _T("EDITATTRLAYERS");		//!< �e�[�u����

	const _TCHAR kPurposeID[]			= _T("PURPOSE_ID");			//!< ��ƖړIID
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< ���C����ID
	const _TCHAR kRistrict1[]			= _T("RISTRICT1");			//!< �����������[���P
	const _TCHAR kRistrict2[]			= _T("RISTRICT2");			//!< �����������[���Q
	const _TCHAR kRistrict3[]			= _T("RISTRICT3");			//!< �����������[���R
	const _TCHAR kRistrict4[]			= _T("RISTRICT4");			//!< �����������[���S
	const _TCHAR kRistrict5[]			= _T("RISTRICT5");			//!< �����������[���T
	const _TCHAR kRistrict6[]			= _T("RISTRICT6");			//!< �����������[���U
	const _TCHAR kRistrict7[]			= _T("RISTRICT7");			//!< �����������[���V
	const _TCHAR kRistrict8[]			= _T("RISTRICT8");			//!< �����������[���W
	const _TCHAR kRistrict9[]			= _T("RISTRICT9");			//!< �����������[���X
	const _TCHAR kRistrict10[]			= _T("RISTRICT10");			//!< �����������[���P�O
} // editattrlayers
namespace edithistory {

	const _TCHAR kTableName[]			= _T("EDITHISTORY");		//!< �e�[�u����

	const _TCHAR kModifyDate[]			= _T("MODIFYDATE");			//!< �ύX����
	const _TCHAR kModifyTable[]			= _T("MODIFYTABLE");		//!< �ύX�������[���e�[�u����
	const _TCHAR kModifyOID[]			= _T("MODIFYOID");			//!< �ύX����OBJECTID�ԍ�
	const _TCHAR kUpdateType[]			= _T("UPDATETYPE_C");		//!< �ύX���@
	const _TCHAR kOperator[]			= _T("OPERATOR");			//!< �ύX�Җ�
	const _TCHAR kMessage[]				= _T("MESSAGE");			//!< ���b�Z�[�W
} // edithistory
namespace editmesh {

	const _TCHAR kTableName[]			= _T("EDITMESH");			//!< �e�[�u����

	const _TCHAR kMeshCode[]			= _T("MESHCODE");			//!< ���b�V���R�[�h
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< ���C����ID
	const _TCHAR kPurposeID[]			= _T("PURPOSE_ID");			//!< ��ƖړIID
	const _TCHAR kOperatorID[]			= _T("OPERATOR_ID");		//!< �I�y���[�^ID
} // editmesh
namespace layernames {

	const _TCHAR kTableName[]			= _T("LAYERNAMES");			//!< �e�[�u����

	const _TCHAR kLayerName[]			= _T("LAYERNAME");			//!< ���C����
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< ���C����ID
	const _TCHAR kLayerAliasName[]		= _T("LAYERALIASNAME");		//!< ���C���G�C���A�X��
} // layernames
namespace lockarea {

	const _TCHAR kTableName[]			= _T("LOCKAREA");			//!< �e�[�u����

	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< ���C����ID
	const _TCHAR kConnectUserID[]		= _T("CONNECTUSR_ID");		//!< �ڑ����[�U��ID
} // lockarea
namespace lockmesh {

	const _TCHAR kTableName[]			= _T("LOCKMESH");			//!< �e�[�u����

	const _TCHAR kMeshCode[]			= _T("MESHCODE");			//!< ���b�V���R�[�h
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< ���C����ID
	const _TCHAR kConnectUserID[]		= _T("CONNECTUSR_ID");		//!< �ڑ����[�U��ID
} // lockmesh
namespace movablelayers {

	const _TCHAR kTableName[]			= _T("MOVABLELAYERS");		//!< �e�[�u����

	const _TCHAR kPurposeID[]			= _T("PURPOSE_ID");			//!< ��ƖړIID
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< ���C����ID
	const _TCHAR kRistrict1[]			= _T("RISTRICT1");			//!< �����������[���P
	const _TCHAR kRistrict2[]			= _T("RISTRICT2");			//!< �����������[���Q
	const _TCHAR kRistrict3[]			= _T("RISTRICT3");			//!< �����������[���R
	const _TCHAR kRistrict4[]			= _T("RISTRICT4");			//!< �����������[���S
	const _TCHAR kRistrict5[]			= _T("RISTRICT5");			//!< �����������[���T
	const _TCHAR kRistrict6[]			= _T("RISTRICT6");			//!< �����������[���U
	const _TCHAR kRistrict7[]			= _T("RISTRICT7");			//!< �����������[���V
	const _TCHAR kRistrict8[]			= _T("RISTRICT8");			//!< �����������[���W
	const _TCHAR kRistrict9[]			= _T("RISTRICT9");			//!< �����������[���X
	const _TCHAR kRistrict10[]			= _T("RISTRICT10");			//!< �����������[���P�O
} // movablelayers
namespace soperator {

	const _TCHAR kTableName[]			= _T("OPERATOR");			//!< �e�[�u����

	const _TCHAR kOperatorID[]			= _T("OPERATOR_ID");		//!< �I�y���[�^��ID
	const _TCHAR kName[]				= _T("NAME");				//!< �I�y���[�^��
	const _TCHAR kAliasName[]			= _T("ALIASNAME");			//!< �I�y���[�^���G�C���A�X
} // operator
namespace roadattribute {

	const _TCHAR kTableName[]			= _T("ROADATTRIBUTE");		//!< �e�[�u����

	const _TCHAR kName[]				= _T("NAME");				//!< ���H�����N������
	const _TCHAR kAliasName[]			= _T("ALIASNAME");			//!< ���H�����N�������G�C���A�X
	const _TCHAR kNameID[]				= _T("NAME_ID");			//!< ���H�����N������ID
} // roadattribute
namespace roaddispclass {

	const _TCHAR kTableName[]			= _T("ROADDISPCLASS");		//!< �e�[�u����

	const _TCHAR kName[]				= _T("NAME");				//!< ���H�����N�\����ʖ�
	const _TCHAR kNameID[]				= _T("NAME_ID");			//!< ���H�����N�\����ʖ�ID
} // roaddispclass
namespace roadnaviclass {

	const _TCHAR kTableName[]			= _T("ROADNAVICLASS");		//!< �e�[�u����

	const _TCHAR kName[]				= _T("NAME");				//!< ���H�����N�o�H��ʖ�
	const _TCHAR kNameID[]				= _T("NAME_ID");			//!< ���H�����N�o�H��ʖ�ID
} // roadnaviclass
namespace shareddeletelayers {

	const _TCHAR kTableName[]			= _T("SHAREDDELETELAYERS");	//!< �e�[�u����

	const _TCHAR kSetID[]				= _T("SET_ID");				//!< ���L�폜���C���Z�b�g�ԍ�
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< ���C����ID
} // shareddeletelayers
namespace sharedmovelayers {

	const _TCHAR kTableName[]			= _T("SHAREDMOVELAYERS");	//!< �e�[�u����

	const _TCHAR kSetID[]				= _T("SET_ID");				//!< ���L�ړ����C���Z�b�g�ԍ�
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< ���C����ID
} // sharedmovelayers
namespace workpurpose {

	const _TCHAR kTableName[]			= _T("WORKPURPOSE");		//!< �e�[�u����

	const _TCHAR kPurpose[]				= _T("PURPOSE");			//!< ��ƖړI��
	const _TCHAR kMXD[]					= _T("MXD");				//!< MXD�t�@�C����
	const _TCHAR kAutoAttrGiving[]		= _T("AUTOATTRGIVING");		//!< ���������t�^�l
	const _TCHAR kWorkKind[]			= _T("WORKKIND");			//!< ��Ǝ��
	const _TCHAR kGencho[]				= _T("GENCHO_F");			//!< �������f��Ɨp�t���O
	const _TCHAR kClaim[]				= _T("CLAIME_F");			//!< �N���[���C����Ɨp�t���O
	const _TCHAR kChuki[]				= _T("CHUKI_F");			//!< ���L�ҏW��Ɨp�t���O
	const _TCHAR kPurposeID[]			= _T("PURPOSE_ID");			//!< ��ƖړI��ID
	const _TCHAR kDelete[]				= _T("DELETE_F");			//!< �폜�ς݃t���O
	const _TCHAR kTest[]				= _T("TEST_F");				//!< �e�X�g�p�t���O
	const _TCHAR kCategory[]			= _T("CATEGORY");			//!< ��ƃJ�e�S��
	const _TCHAR kLayerFilePath[]		= _T("LAYERFILES_PATH");	//!< ���C���t�@�C���p�X
	const _TCHAR kCanSave[]				= _T("CANSAVE_F");			//!< �ۑ��s�t���O
} // workpurpose
namespace history_table {

	const _TCHAR kTableName[]    = _T("EXPORT_INFO") ; //!< �e�[�u����

	const _TCHAR kObjectID[]     = _T("OBJECTID");     //!< �I�u�W�F�N�gID
	const _TCHAR kName[]         = _T("NAME");         //!< ����
	const _TCHAR kFeatureClass[] = _T("FEATURECLASS"); //!< �t�B�[�`���N���X��
	const _TCHAR kConnectInfo[]  = _T("CONNECT_INFO"); //!< �ڑ����
	const _TCHAR kUser[]         = _T("USER");         //!< ���[�U��
	const _TCHAR kMachine[]      = _T("MACHINE");      //!< �}�V����
	const _TCHAR kDate[]         = _T("EXP_DATE");     //!< ���t
	const _TCHAR kOperation[]    = _T("OPERATION");    //!< ��Ɩ�
} // history_table

} // schema

} // sindy
