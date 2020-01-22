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
#include <boost/operators.hpp>

/**
* @brief �����N�ԍ��i�����N�P�A�����N�Q�j��ʗp
*/
namespace linkNo{
	enum eCode{
		unkown, //!< �s��
		link1,  //!< ID1
		link2   //!< ID2
	};
}

/**
* @brief ���΍����p
*/
namespace height_info{
	enum eCode{
		flat,    //!< ���΍����Ȃ�
		higher,  //!< ����
		lower    //!< �Ⴂ
	};
}

/**
* @brief �����N�ƍ����Ǘ��p
*/
struct CLinkHeight : private boost::operators<CLinkHeight>
{
	CLinkHeight() : m_id(-1), m_height(0){}
	CLinkHeight( const IFeaturePtr link );
	~CLinkHeight(){}

	CString m_layer;        //!< ���C����
	long m_id;              //!< OID
	CString m_walkclass;    //!< ���s�Ҏ��
	long m_height;          //!< �i���΁H�j����
	IFeaturePtr m_feature;  //!< �����N�̃t�B�[�`��

	/// �������ׂ�
	height_info::eCode Compare ( const CLinkHeight& other) const;

	/// �����Ŕ���
	bool operator == (const CLinkHeight& other) const { 
		return m_height == other.m_height; 
	}
	bool operator < (const CLinkHeight& other) const { 
		return m_height < other.m_height; 
	}
};

class CRelativeLinks
{
public:
	CRelativeLinks();
	~CRelativeLinks();
	
	/**
	* @brief ������
	* @param heightNode [in] ��_�ƂȂ鑊�΍����m�[�h
	* @param ah [in] ArcHelper
	* @param err [out] �G���[���b�Z�[�W
	*/
	bool Reset( const IFeaturePtr& heightNode, const CArcHelper& ah, CString& err );
	
	/**
	* @brief ��������Q�����N����������
	* @param heightnNode [in] ��_�̍����m�[�h
	* @return ���H�����N�A�܂��͕��s�҃����N�̃��X�g
	*/
	std::list< CAdapt<IFeaturePtr> > GetCrossedLinks( const IFeaturePtr& heightNode );
	
	/**
	* @brief �i���΁H�j������ύX����
	* @param linkNo [in] �����N�ԍ��iID1 or ID2�j
	* @param height [in] �ݒ肷�鍂���i-2 �` 2�j
	*/
	void UpdateHeight( linkNo::eCode linkNo, long height ){ m_links[linkNo].m_height = height; }
	
	/**
	* @brief �����N���̑��ݔ���
	* @return 1�ł������true
	*/
	bool IsExist() const{ return !m_links.empty(); }
	
	/**
	* @brief �V�K���̔���
	* @return SketchTool�ŐV�K�ɍ��ꂽ�i�ł��낤�j���̂Ȃ�true
	*/
	bool IsCreated() const { return m_createF;}
	
	/**
	* @brief 1���̃����N���擾
	* @param linkNo [in] �����N�ԍ��iID1 or ID2�j
	*/
	const CLinkHeight& GetLinkInfo(linkNo::eCode linkNo ) {return m_links[linkNo];}
	
	/**
	* @brief ���ׂẴ����N���擾
	*/
	const std::map<linkNo::eCode, CLinkHeight>& GetLinkInfos() {return m_links;}
	
	/**
	* @brief ���΍����擾
	* @note linkNo�̃����N�̑����ɑ΂��鑊�΍�����Ԃ�
	* @param linkNo [out] ���΍����擾�Ώۂ̃����N
	* @return higher linkNo�͑�����������
	* @return lower linkNo�͑��������Ⴂ
	* @return flat �������N�̍����͓�����
	*/
	height_info::eCode GetRelativeHeight( linkNo::eCode linkNo );
	
	/**
	* @brief �_���`�F�b�N
	* @param msg [out] �G���[���b�Z�[�W�B�Ȃ���΋󕶎�
	* @return �G���[���Ȃ����true
	*/
	bool CheckLogic( CString& msg );

	/// map�̃��b�v�p
	typedef std::map<linkNo::eCode, CLinkHeight> LinkMap;
	LinkMap::const_iterator begin() {return m_links.begin();}
	LinkMap::const_iterator end() {return m_links.end();}
	const CLinkHeight& operator []( linkNo::eCode linkNo ){ return m_links[linkNo]; }

private:
	/**
	* @brief ������
	* @param links [in] ��������2�����N
	*/
	void Reset( const std::list< CAdapt<IFeaturePtr> >& links );

	bool m_createF;  //!< �V�K���̃t���O
	std::map<linkNo::eCode, CLinkHeight> m_links;   //<! �����N�ԍ����Ƃ̍������

	IFeaturePtr m_heightNode;   //!< �d�Ȃ荂���t�B�[�`��
	IFeatureClassPtr m_roadT;   //!< ���H�����N�t�B�[�`���N���X
	IFeatureClassPtr m_walkT;   //!< ���s�҃����N�t�B�[�`���N���X
};

