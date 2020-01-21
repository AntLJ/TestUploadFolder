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

// MrxLoad.h: CMrxLoad �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MRXLOAD_H__F1D0113A_62FF_44D3_865C_8A9DBDF0715B__INCLUDED_)
#define AFX_MRXLOAD_H__F1D0113A_62FF_44D3_865C_8A9DBDF0715B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include <io.h>
#include <fcntl.h>	//typedef O_RDONLY
#include <vector>	//typedef STL

typedef unsigned long	U_LONG;
typedef unsigned short	U_SHORT;

#pragma pack(push, 2)
/**
 * @brief �}�b�v���l�q�w�w�b�_�f�[�^�e�[�u���i�Q�T�Ubyte�j
 */
struct MRXHead{
	char	eVersion[8];			///< �t�@�C���t�H�[�}�b�g�o�[�W����
	short	eSokuchi;				///< ���n�n�R�[�h
	short	eReserve1;				///< (�\��)
	long	eScale;					///< ��f�[�^�k��
	char	eMeshcode[16];			///< ���b�V���ԍ�
	long	eMinX;					///< �}�s���|�쐼�[�o�x
	long	eMaxX;					///< �}�s���|�k���[�o�x
	long	eMinY;					///< �}�s���|�쐼�[�ܓx
	long	eMaxY;					///< �}�s���|�k���[�ܓx
	char	eLastUpdate[16];		///< �ŏI�X�V��
	char	eReleaseID[16];			///< �����[�X���ʏ��
	char	eReserve2[48];			///< (�\��)
	U_LONG	eNodeOffset;			///< �m�[�h���R�[�h�J�n�ʒu
	U_LONG	eNodeNs;				///< �m�[�h���R�[�h��
	U_LONG	eLinkOffset;			///< �����N���R�[�h�J�n�ʒu
	U_LONG	eLinkNs;				///< �����N���R�[�h��
	U_LONG	eGroupLineOffset;		///< �O���[�v���J�n�ʒu
	U_LONG	eGroupLineNs;			///< �O���[�v����
	char	eReserve3[104];			///< (�\��)
};

/**
 * @brief �}�b�v���l�c�w�w�b�_�f�[�^�e�[�u���i�Q�T�Ubyte�j
 */
struct MDXHead{
	char	eVersion[8];			///< �t�@�C���t�H�[�}�b�g�o�[�W����
	short	eSokuchi;				///< ���n�n�R�[�h
	short	eReserve1;				///< (�\��)
	long	eScale;					///< ��f�[�^�k��
	char	eMeshcode[16];			///< ���b�V���ԍ�
	long	eMinX;					///< �}�s���|�쐼�[�o�x
	long	eMaxX;					///< �}�s���|�k���[�o�x
	long	eMinY;					///< �}�s���|�쐼�[�ܓx
	long	eMaxY;					///< �}�s���|�k���[�ܓx
	char	eLastUpdate[16];		///< �ŏI�X�V��
	char	eReleaseID[16];			///< �����[�X���ʏ��
	char	eReserve2[48];			///< (�\��)
	U_LONG	eLineOffset;			///< �����R�[�h�J�n�ʒu
	U_LONG	eLineNs;				///< �����R�[�h��
	U_LONG	ePolyOffset;			///< �|���S�����R�[�h�J�n�ʒu
	U_LONG	ePolyNs;				///< �|���S�����R�[�h��
	char	eReserve3[16];			///< (�\��)
	U_LONG	eGroupLineOffset;		///< �O���[�v���J�n�ʒu
	U_LONG	eGroupLineNs;			///< �O���[�v����
	char	eReserve4[88];			///< (�\��)
};

/**
 * @brief �}�b�v���l�q�w�m�[�h�f�[�^�e�[�u��
 */
struct MRX_Node{
	short	eObjectCode;			///< �I�u�W�F�N�g�R�[�h�i11 ���H�m�[�h�j
	U_LONG	eNodeNo;				///< �m�[�h�ԍ�
	char	eNodeType[8];			///< �m�[�h��ʃR�[�h
	char	eNodeAttr[8];			///< �������ރR�[�h
	char	eLavel[16];				///< �G�������g���x��
	long	eLon;					///< �o�x[1/1000�b]
	long	eLat;					///< �ܓx[1/1000�b]
	long	eHeight;				///< �W���l[1/10m]
	char	eNextMesh[8];			///< �אڃ��b�V���ԍ��i�}�s��m�[�h�̏ꍇ�̂݁j
	U_LONG	eNextMeshNodeNo;		///< �אڃ��b�V���ڑ��m�[�h�ԍ�
	short	eLinkCount;				///< �ڑ������N�{��
};

/**
 * @brief �}�b�v���l�q�w�m�[�h�f�[�^�R���e�i
 */
struct MRXNode{
	MRX_Node		eNodes;			///< �m�[�h�v�f��
	std::vector<U_LONG>	vLinkID;		///< �m�[�h�ڑ������N�ԍ��R���e�i
};

/**
 * @brief �}�b�v���l�q�w�����N�`��_���W�Ǘ��e�[�u��
 */
struct	MRX_Pos{
	long			ePos[3];		///< �`��_���W
};

/**
 * @brief �}�b�v���l�q�w�����N�f�[�^�e�[�u��
 */
struct MRX_Link{
	short	eObjectCode;			///< �I�u�W�F�N�g�R�[�h�i21 ���H�����N�j
	U_LONG	eLinkNo;				///< �����N�ԍ�
	char	eLinkType[8];			///< �����N��ʃR�[�h
	char	eLinkAttr[8];			///< �������ރR�[�h
	char	eLavel[16];				///< �G�������g���x��
	long	eWidth;					///< �����l
	U_LONG	eSnodeID;				///< �n�_�m�[�h�ԍ�
	U_LONG	eEnodeID;				///< �I�_�m�[�h�ԍ�
	short	eRegCode;				///< ��ʋK��ʃR�[�h
	short	eLength;				///< �����N��
	long	eCount;					///< �`��_����	
};

/**
 * @brief �}�b�v���l�q�w�����N�\���_�R���e�i
 */
struct MRXLink{
	long			eType;					///< �����N���ʃR�[�h(1:�ʂ��݂��A2:����݂��A3:�a�ؓ��H)
	U_LONG			eID;					///< �\�����̎��ʔԍ�
	int				eMeshcode;				///< ���b�V���R�[�h
	long			ePrimId;				///< �����NOBJECTID
	short			eLinkDir;				///< �����N�\���_�̌���
	long			eSeq;					///< �\���v�f�ԍ�
	long			eCount;					///< �\���v�f��
	long			eLastF;					///< �����N��I�[�t���O
	MRX_Link		eLinks;					///< �����N�v�f��
	std::vector<MRX_Pos>	vLinkPos;		///< �����N�`��_���W�R���e�i
};

/**
 * @brief �}�b�v���l�q�w�O���[�v�f�[�^�e�[�u���i�a�؁E�ʂ��݂��j
 */
struct MRX_GroupId{
	U_LONG	eID;					///< �\�����̎��ʔԍ�
	short	eDir;					///< �\�����̌����i1:�n�_���I�_�A2:�I�_���n�_�j
};

/**
 * @brief �}�b�v���l�q�w�O���[�v�f�[�^�e�[�u���i�a�؁E�ʂ��݂��j
 */
struct MRX_Group{
	short	eObjectCode;			///< �I�u�W�F�N�g�R�[�h�i91 ���H�O���[�v�j
	U_LONG	eGroupID;				///< ���ʔԍ�
	char	eKindCode[8];			///< �}�����ރR�[�h
	char	eAttrCode[8];			///< �������ރR�[�h
	char	eLavel[16];				///< �G�������g���x��
	short	eDrawSeq;				///< �`�揇
	short	eStatCode;				///< ��Ԏ�ʃR�[�h
	long	eHVal;					///< �g�l
	long	eCount;					///< �\���v�f��
};

/**
 * @brief �}�b�v���l�q�w�O���[�v�f�[�^�R���e�i
 */
struct MRXGroup{
	MRX_Group			eGroup;		///< �O���[�v�v�f��
	std::vector<MRX_GroupId>	vGroupID;	///< �O���[�v�v�f�\�������N�񎯕ʃR���e�i
};

/**
 * @brief �}�b�v���l�q�w�����N��Ǘ��R���e�i
 */
struct MRXLQLink{
	long			eType;			///< �����N���ʃR�[�h(1:�ʂ��݂��A2:����݂��A3:�a�ؓ��H�A4:���H�����N)
	int				eMeshcode;		///< ���b�V���R�[�h
	U_LONG			eGroupID;		///< ���ʔԍ�
	long			ePrimId;		///< �����N��OBJECTID
	long			eCount;			///< �\���v�f��
	std::vector<MRX_Pos>	vLinkPos;		///< �����N�`��_���W�R���e�i
};

/**
 * @brief �}�b�v���l�c�w���֎���|���S���f�[�^�e�[�u��
 */
struct MDX_Polygon
{
	short	eObjectCode;			///< �I�u�W�F�N�g�R�[�h�i50 ���̑��|���S���j
	U_LONG	eGroupID;				///< ���ʔԍ�
	char	eKindCode[8];			///< �}�����ރR�[�h
	char	eLinkAttr[8];			///< �������ރR�[�h
	char	eLavel[16];				///< �G�������g���x��
	short	eDrawSeq;				///< �`�揇
	short	eStatCode;				///< ��Ԏ�ʃR�[�h
	long	eHVal;					///< �g�l(���g�p)
	long	eCount;					///< �\���v�f��
};

/**
 * @brief �}�b�v���l�c�w���֎���|���S���f�[�^�R���e�i
 */
struct MDXPolygon
{
	MDX_Polygon	eMdxPoly;				///< �|���S���v�f��
	std::vector<MRX_Pos> vPolygonPos;	///< �|���S���`��_���W�R���e�i
};

/**
 * @brief �}�b�v���l�c�w�����N�\���_�e�[�u��
 */
struct MDX_Link{
	short	eObjectCode;			///< �I�u�W�F�N�g�R�[�h�i40:���̑��̐��A41:���H�A42:�S���A43:�s���E�A44:�������j
	U_LONG	eGroupID;				///< ���ʔԍ�
	char	eKindCode[8];			///< �}�����ރR�[�h
	char	eLinkAttr[8];			///< �������ރR�[�h
	char	eLavel[16];				///< �G�������g���x��
	short	eDrawSeq;				///< �`�揇
	short	eStatCode;				///< ��Ԏ�ʃR�[�h
	long	eHVal;					///< �g�l�i[41:���H]���H�����A[44:������]�W���l�j
	long	eCount;					///< �\���v�f��
};

/**
 * @brief �}�b�v���l�c�w�����N�\���_�R���e�i
 */
struct MDXLink
{
	MDX_Link	eMdxLink;
	std::vector<MRX_Pos> vLinkPos;
};
#pragma pack(pop)

/**
 * @brief �}�b�v���l�q�w�f�[�^�R���e�i
 */
class MRX_Alldata
{
public:
// �\�z/����
	MRX_Alldata(){}
	~MRX_Alldata(){}

	inline MRXLink *FindLink(U_LONG aKeyID /*�����N�ԍ�*/){
		for(std::vector<MRXLink>::iterator it=m_Links.begin(); it!=m_Links.end(); it++ ){
			if( aKeyID==(*it).eLinks.eLinkNo ){
				return &(*it);
			}
		}
		return NULL;
	}

public:
	MRXHead					m_Header;	///< �w�b�_�[��
	std::vector<MRXNode>	m_Nodes;	///< �m�[�h�v�f���R���e�i
	std::vector<MRXLink>	m_Links;	///< �����N�v�f���R���e�i
	std::vector<MRXGroup>	m_Group;	///< �O���[�v�v�f���R���e�i
	std::vector<MRXLQLink>	m_LQ;		///< �����N��Ǘ��R���e�i
	std::vector<MDXPolygon>	m_Poly;		///< �|���S���v�f���R���e�i
};

/**
 * @brief �}�b�v���l�c�w�f�[�^�R���e�i
 */
class MDX_Alldata
{
public:
// �\�z/����
	MDX_Alldata(){}
	~MDX_Alldata(){}
public:
	MDXHead					m_Header;		///< �w�b�_�[��
	std::vector<MDXPolygon>	m_Poly;			///< �|���S���v�f���R���e�i
	std::vector<MDXLink>	m_Links;		///< �����N�v�f��
	std::vector<MRXGroup>   m_Group;		///< �|�����C���v�f���R���e�i
};


/**
* @class CMrxLoad
*
* @brief �}�b�v���l�q�w�f�[�^���Ǘ�����N���X
*/
class CMrxLoad
{
public:
// �\�z/����
	CMrxLoad();
	~CMrxLoad();

// �擾
	char*	getExtension(void) { return "mrx"; }

// �o�^
	bool	load(char *cFileStr);	///< �}�b�v���l�q�w�f�[�^�ǂݍ���	
	void	close(void);			///< �}�b�v���l�q�w�f�[�^�J��
	const MRX_Alldata&	getMRXdata(void) const { return(m_MrxInfo);	 }///< MRX�f�[�^�N���X���擾
	MRX_Alldata	*getMRXinfo(char *cFileStr){ 
		if( load(cFileStr)){ return(&m_MrxInfo); }	///< MRX�f�[�^�̎擾����
		else{                return(NULL);       }	///< MRX�f�[�^�̎擾���s
	}

//�f�B�o�O�����o��
	void	print();

// �擾
protected:
	bool	loadNodes(std::ifstream& ifp);	///< �m�[�h�v�f���̎擾	
	bool	loadLinks(std::ifstream& ifp);	///< �����N�v�f���̎擾	
	bool	loadGroups(std::ifstream& ifp);	///< �O���[�v���v�f���̎擾
	bool	bindGroups(void);		///< �O���[�v��񖈂ɓ��H������

private:
// �v���p�e�B
	MRX_Alldata		m_MrxInfo;		///< �}�b�v���l�q�w�f�[�^�Ǘ��f�[�^
};

/**
* @class CMdxLoad
*
* @brief �}�b�v���l�c�w�f�[�^���Ǘ�����N���X
*/
class CMdxLoad
{
public:
// �\�z/����
	CMdxLoad();
	~CMdxLoad();

// �擾
	char*	getExtension(void) { return "mdx"; }
// �o�^
	bool	load(char *cFileStr);	///< �}�b�v���l�c�w�f�[�^�ǂݍ���	
	void	close(void);			///< �}�b�v���l�c�w�f�[�^�J��
	const MDX_Alldata& getMDXdata(void) const { return(m_MdxInfo);	 }///< MDX�f�[�^�N���X���擾
	MDX_Alldata	*getMDXinfo(char *cFileStr){ 
		if( load(cFileStr)){ return(&m_MdxInfo); }	///< MDX�f�[�^�̎擾����
		else{                return(NULL);       }	///< MDX�f�[�^�̎擾���s
	}
// �擾
protected:
	bool	loadPoly(std::ifstream& ifp);	///< �|���S���v�f���̎擾
	bool	loadGroups(std::ifstream& ifp);	///< �O���[�v���v�f���̎擾
	bool	loadLinks(std::ifstream& ifp);	///< ���v�f���̎擾

private:
// �v���p�e�B
	MDX_Alldata		m_MdxInfo;		///< �}�b�v���l�c�w�f�[�^�Ǘ��f�[�^
};

#endif // !defined(AFX_MRXLOAD_H__F1D0113A_62FF_44D3_865C_8A9DBDF0715B__INCLUDED_)
