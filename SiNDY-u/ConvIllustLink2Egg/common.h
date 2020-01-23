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

// �R�}���h���C������
const LPCSTR REPDB = "rep_db";
const LPCSTR REPSCHEMA = "rep_schema";
const LPCSTR ILLSET = "illust_settings";
const LPCSTR INPUTLIST = "input_list";
const LPCSTR EGGDB = "egg_db";
const LPCSTR EGGSCHEMA = "egg_schema";
const LPCSTR RUNLOG = "runlog";
const LPCSTR ERRLOG = "errlog";


// �C���X�g�������ݒ�t�@�C���t�B�[���h��
const long ILLSETFNUM = 7;

// �C���X�g�������ݒ�t�@�C���p�\����
struct IllSetRecord
{
	CString e_illCls;			// �C���X�g��ʃR�[�h
	CString e_dstCode;			// �d������R�[�h
	CString e_prefix;			// �C���X�g�t�@�C���̃v���t�B�b�N�X
	CString e_suffix;			// �C���X�g�t�@�C���̊g���q
	CString e_nameRule;			// �C���X�g�t�@�C�������K��
	CString e_illDir;			// �C���X�g�t�@�C���i�[�f�B���N�g��
	CString e_excCond;			// �o�͏��O����
};

// �C���X�g�t�@�C�����t�B�[���h��
const long ILLFILEFNUM = 5;

// �C���X�g�t�@�C�����Ή��\�p�\����
struct ClassCode
{
	CString e_illCls;			// �C���X�g��ʃR�[�h
	CString e_dstCode;			// �d������R�[�h

	ClassCode(CString illCls, CString dstCode)
		:e_illCls(illCls), e_dstCode(dstCode)
	{}

	bool operator < (const ClassCode& classCode) const {
		if (e_illCls != classCode.e_illCls) 
			return e_illCls < classCode.e_illCls;
		else
			return e_dstCode < classCode.e_dstCode;
	}
};

typedef long OID;

struct IllFileNames
{
	CString e_orgIllName;
	CString e_tgtIllName;
};

// �C���X�g�����N���擾�p�\����
struct IllLinkInfo
{
	OID e_objectId;
	long e_illustClass;
	std::vector<OID> e_tgtNodeIds;
	

	IllLinkInfo(OID objectId, long illustClass, std::vector<OID> tgtNodeIds)
		:e_objectId(objectId), e_illustClass(illustClass), e_tgtNodeIds(tgtNodeIds)
	{}
};

// ���H�����N���擾�p
const long ForwardDir = sindy::schema::category::directional_link_relational_table::link_dir::kFore;
const long ReverseDir = sindy::schema::category::directional_link_relational_table::link_dir::kReverse;

struct RoadLinkKey
{
	long e_fromNodeId;
	long e_toNodeId;

	RoadLinkKey(long fromNodeId, long toNodeId)
		:e_fromNodeId(fromNodeId), e_toNodeId(toNodeId)
	{}
	bool operator < (const RoadLinkKey& roadLinkKey) const {
		if (e_fromNodeId != roadLinkKey.e_fromNodeId)
			return e_fromNodeId < roadLinkKey.e_fromNodeId;
		else
			return e_toNodeId < roadLinkKey.e_toNodeId;
	}
};
struct RoadLinkInfo
{
	OID	 e_objectId;
	long e_fromNodeId;
	long e_toNodeId;
};

// �����N����i�[�\����
struct LinkQueueInfo 
{
	long e_linkid;
	long e_linkDir;
};



// �C���X�g���(inf_illust)�p

struct InfIllInfo
{
	OID	 e_objectID;
	long e_illustCode;
	CString e_fileName;

	InfIllInfo(OID objectId, long illustCode, CString fileName)
		:e_objectID(objectId), e_illustCode(illustCode), e_fileName(fileName)
	{}
};
// �C���X�g�����N��(lq_illust)�p
struct LqIllKey
{
	long e_infId;			// �C���X�g���ID
	long e_Seq;				// �V�[�P���X

	LqIllKey(long infId, long seq) 
		:e_infId(infId), e_Seq(seq)
	{}

	bool operator < (const LqIllKey lqIllKey) const {
		if (e_infId != lqIllKey.e_infId)
			return e_infId < lqIllKey.e_infId;
		else
			return e_Seq < lqIllKey.e_Seq;
	}
};
struct LqIllInfo
{
	long e_infId;
	long e_linkId;			// �����NID
	long e_linkDir;			// �����N����
	long e_seq;				// �V�[�P���X

	LqIllInfo(long infId, long linkId, long linkDir, long seq)
		:e_infId(infId), e_linkId(linkId), e_linkDir(linkDir), e_seq(seq)
	{}
};
