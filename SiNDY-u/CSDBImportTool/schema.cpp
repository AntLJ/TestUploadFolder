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
#include "schema.h"

tFieldSchema g_CSDBFieldDef[] = {
{ 	_T("ID"),	_T( "�Ǘ��ԍ�" ),	esriFieldTypeString,	14,	0,	VARIANT_FALSE },
{ 	_T( "STATUS" ),	_T( "�X�e�[�^�X" ),	esriFieldTypeString,	255,	0,	VARIANT_FALSE },
{ 	_T( "ERR_TYPE" ),	_T( "�s��Ώۃf�[�^" ),	esriFieldTypeString,	255,	0,	VARIANT_TRUE },
{ 	_T( "ERR_TITLE" ),	_T( "�����i�T���j" ),	esriFieldTypeString,	4000,	0,	VARIANT_FALSE },
{ 	_T( "TDC_F" ),	_T( "TDC�S���t���O" ),	esriFieldTypeSmallInteger,	1,	0,	VARIANT_FALSE },
{ 	_T( "WORK_F" ),	_T( "����H���t���O" ),	esriFieldTypeSmallInteger,	1,	0,	VARIANT_FALSE },
{ 	_T( "INQ1" ),	_T( "��������_(�e�L�X�g)" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "SIRIUS_ID" ),	_T( "SiRiUS-ID" ),	esriFieldTypeString,	255,	0,	VARIANT_TRUE },
{ 	_T( "POI_ID" ),	_T( "POI�w�ԍ�" ),	esriFieldTypeString,	255,	0,	VARIANT_TRUE },
{ 	_T( "LOC01_LAT" ),	_T( "�s������n�__�ܓx" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC01_LONG" ),	_T( "�s������n�__�o�x" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC02_LAT" ),	_T( "�o���n_�ܓx" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC02_LONG" ),	_T( "�o���n_�o�x" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC03_LAT" ),	_T( "�ړI�n_�ܓx" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC03_LONG" ),	_T( "�ړI�n_�o�x" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC04_LAT" ),	_T( "����n_�ܓx" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "LOC04_LONG" ),	_T( "����n_�o�x" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_01FCT" ),	_T( "No1:�����̐���" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_01DTL" ),	_T( "No1:�΍����j�e�L�X�g" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_01TGT" ),	_T( "No1:�΍��\��f�[�^" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_02FCT" ),	_T( "No2:�����̐���" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_02DTL" ),	_T( "No2:�΍����j�e�L�X�g" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_02TGT" ),	_T( "No2:�΍��\��f�[�^" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_03FCT" ),	_T( "No3:�����̐���" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_03DTL" ),	_T( "No3:�΍����j�e�L�X�g" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_03TGT" ),	_T( "No3:�΍��\��f�[�^" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_04FCT" ),	_T( "No4:�����̐���" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_04DTL" ),	_T( "No4:�΍����j�e�L�X�g" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_04TGT" ),	_T( "No4:�΍��\��f�[�^" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_05FCT" ),	_T( "No5:�����̐���" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_05DTL" ),	_T( "No5:�΍����j�e�L�X�g" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_05TGT" ),	_T( "No5:�΍��\��f�[�^" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE }
};

tFieldSchema g_NAVIFieldDef[] = {
{ 	_T("ID"),	_T( "�Ǘ��ԍ�" ),	esriFieldTypeString,	14,	0,	VARIANT_FALSE },
{ 	_T( "STATUS" ),	_T( "�X�e�[�^�X" ),	esriFieldTypeString,	255,	0,	VARIANT_FALSE },
{ 	_T( "ERR_TYPE" ),	_T( "�s��Ώۃf�[�^" ),	esriFieldTypeString,	255,	0,	VARIANT_FALSE },
{ 	_T( "ERR_TITLE" ),	_T( "�����i�T���j" ),	esriFieldTypeString,	4000,	0,	VARIANT_FALSE },
{ 	_T( "TDC_F" ),	_T( "TDC�S���t���O" ),	esriFieldTypeSmallInteger,	1,	0,	VARIANT_FALSE },
{ 	_T( "WORK_F" ),	_T( "����H���t���O" ),	esriFieldTypeSmallInteger,	1,	0,	VARIANT_FALSE },
{ 	_T( "INQ1" ),	_T( "�Ǐ�ڍ�" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "SIRIUS_ID" ),	_T( "SiRiUS-ID" ),	esriFieldTypeString,	255,	0,	VARIANT_TRUE },
{ 	_T( "POI_ID" ),	_T( "POI�w�ԍ�" ),	esriFieldTypeString,	255,	0,	VARIANT_TRUE },
{ 	_T( "LOC01_LAT" ),	_T( "�s������n�__�ܓx" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC01_LONG" ),	_T( "�s������n�__�o�x" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC02_LAT" ),	_T( "�o���n_�ܓx" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC02_LONG" ),	_T( "�o���n_�o�x" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC03_LAT" ),	_T( "�ړI�n_�ܓx" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC03_LONG" ),	_T( "�ړI�n_�o�x" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC04_LAT" ),	_T( "����n_�ܓx" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "LOC04_LONG" ),	_T( "����n_�o�x" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_01FCT" ),	_T( "No1:�����̐���" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_01DTL" ),	_T( "No1:�΍����j�e�L�X�g" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_01TGT" ),	_T( "No1:�΍��\��f�[�^" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_02FCT" ),	_T( "No2:�����̐���" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_02DTL" ),	_T( "No2:�΍����j�e�L�X�g" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_02TGT" ),	_T( "No2:�΍��\��f�[�^" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_03FCT" ),	_T( "No3:�����̐���" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_03DTL" ),	_T( "No3:�΍����j�e�L�X�g" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_03TGT" ),	_T( "No3:�΍��\��f�[�^" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_04FCT" ),	_T( "No4:�����̐���" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_04DTL" ),	_T( "No4:�΍����j�e�L�X�g" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_04TGT" ),	_T( "No4:�΍��\��f�[�^" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_05FCT" ),	_T( "No5:�����̐���" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_05DTL" ),	_T( "No5:�΍����j�e�L�X�g" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_05TGT" ),	_T( "No5:�΍��\��f�[�^" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE }
};
