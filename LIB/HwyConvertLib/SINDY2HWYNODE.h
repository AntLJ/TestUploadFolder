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

// SINDY2HWYNODE.h: SINDY2HWYNODE �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDY2HWYNODE_H__39508166_2AD5_413F_AD9E_83011BE51FEC__INCLUDED_)
#define AFX_SINDY2HWYNODE_H__39508166_2AD5_413F_AD9E_83011BE51FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SINDY2INHWY.h"
#include "INHwySchema.h"
#include <boost/array.hpp>


// �������@�\�R�[�h�̃t���O��
typedef boost::array<
	bool,
	in::schema::hwynode::toll_func_flags::num_of_tollfunc_value
> TollFuncFlags;


// hwynode.txt�̂P���R�[�h�̏����i�[����\����
struct HighwayNode
{
	long		eFileType;		// ���̃f�[�^���o�͂��ׂ��t�@�C���̎��

	long		eMesh;			// 2�����b�V���R�[�h
	long		eNode;			// �m�[�hID
	long		eRoadCode;		// �H���R�[�h
	long		eSequence;		// �H�����V�[�P���X�ԍ�
	long		eFacilPos;		// �{�݈ʒu�R�[�h
	long		eDirection;		// �㉺���R�[�h

	long		eFacilClass;	// �{�ݎ�ʃR�[�h
	long		eTollClass;		// ��������ʃR�[�h
	long		eInOut;			// �o�����R�[�h

	bool			eNoEtcCarFlag;			// ��ʎԗ��Ή��t���O
	TollFuncFlags	eTollFuncFlags;			// �������@�\�R�[�h�i��ʁj
	bool			eNoGuideTollFlag;		// ��U���������t���O�i��ʁj

	long			eEtcFacilCode;			// ETC�{�݃R�[�h
	TollFuncFlags	eEtcTollFuncFlags;		// �������@�\�R�[�h(ETC)�̏�5��
	bool			eEtcNoGuideTollFlag;	// ��U���������t���O�iETC�j

	long		eAppNode;		// �����m�[�hID
	long		eFlowNode;		// ���o�m�[�hID
	std::string	eName;			// ��������
	std::string	eYomi;			// �ǂݖ���

	HighwayNode();
	virtual ~HighwayNode() {}
};


typedef	long	RoadCode;
typedef	long	RoadSeq;
typedef std::multimap<RoadCode, RoadSeq>	FacilCodeArray;

// tollguide.txt�ɏo�͂���f�[�^���i�[����\����
struct TollGuide
{
	INMNode			eNode;				// ���b�V���R�[�h�{�m�[�hID
	TollFuncFlags	eTollFuncFlags;		// �Ή��������@�\�R�[�h
	FacilCodeArray	eFacilCodeArray;	// �{�ݏ��i�H���R�[�h�ƘH�����V�[�P���X�̑g�j�̗�

	TollGuide() { eTollFuncFlags.assign(false); }
	virtual ~TollGuide() {}
};

// tollguide.txt�̃��R�[�h���A���b�V���{�m�[�hID����Ƃ��ĕ��ׂ邽�߂̃t�@���N�^
struct lessMNode : public std::binary_function<INMNode,INMNode,bool>
{
	bool operator ()(const INMNode &cNode1, const INMNode &cNode2) const
	{
		return (cNode1.eMesh != cNode2.eMesh) ? cNode1.eMesh < cNode2.eMesh : cNode1.eNode < cNode2.eNode;
	}
};

// tollguide.txt�̃��R�[�h��
typedef std::map<INMNode, TollGuide, lessMNode> TollGuideArray;


class SINDY2HWYNODE : public SINDY2INHWY
{
public:
	SINDY2HWYNODE(SINDY2INHWYINFO *cHwyInfo);
	virtual ~SINDY2HWYNODE();

	virtual HRESULT Convert(long cFormat);
	virtual HRESULT Write(std::string cFileName, long cFileType, long cFormat);

	static HRESULT isTarget(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, bool *cTarget);
	static HRESULT isGuideNode(IFeaturePtr ipHwyNode, bool *cGuide);
	static HRESULT GetMeshCode(IFeaturePtr ipHwyNode, long *cMesh);
	static HRESULT GetNodeID(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, long *cNodeID);
	static HRESULT GetFacilPos(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, long *cFacilPos);
	static HRESULT GetUpDown(IFeaturePtr ipHwyNode, long *cUpDown);
	static HRESULT GetInOutNode(IFeaturePtr ipHwyNode, long *cAppNode, long *cFlowNode);
	static HRESULT GetName(SINDY2INHWYINFO *cHwyInfo, IFeaturePtr ipHwyNode, std::string *cName, std::string *cYomi);

private:
	void	WriteHwyNode		(std::ofstream& fout, long cFormat);
	void	WriteHwyNodeInOut	(std::ofstream& fout, long cFormat);
	void	WriteTollNode		(std::ofstream& fout, long cFormat);
	void	WriteTollGuide		(std::ofstream& fout, long cFormat);

	std::auto_ptr<TollGuideArray> makeTollGuideArray(void);

private:
	typedef std::vector<HighwayNode> HighwayNodeArray;
	HighwayNodeArray mHighwayNodeArray;

public:
	typedef HighwayNodeArray::const_iterator Iter;

	/// �f�[�^��̐擪���w��const_iterator��Ԃ�
	Iter begin() const { return mHighwayNodeArray.begin(); }

	/// �f�[�^��̖������w��const_iterator��Ԃ�
	Iter end() const { return mHighwayNodeArray.end(); }

	/// �f�[�^��̃T�C�Y��Ԃ�
	std::size_t size() const { return mHighwayNodeArray.size(); }
};

#endif // !defined(AFX_SINDY2HWYNODE_H__39508166_2AD5_413F_AD9E_83011BE51FEC__INCLUDED_)
