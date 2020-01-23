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

typedef set<int> ISet;				///< �l�̏W��
typedef map<int, LPCTSTR>	MError;	///< �L�[�̓G���[�R�[�h, �l�̓G���[���b�Z�[�W


/////////////////////////////////////////////
//  IN�w�b�_�֘A
/////////////////////////////////////////////
/**
 * @brief �w�b�_��񑮐�
 */
struct HeadAttrList
{
	ISet		eMnt;			///< .mnt�֘A���C��
	ISet		eRdg;			///< .rdg�֘A���C��
	MError		eError;			///< �G���[���b�Z�[�W
};

/////////////////////////////////////////////
//  �����N���C���֘A
/////////////////////////////////////////////
/**
 * @brief �����N���
 */
struct LinkInfo
{
	ISet	eManager;			///< �Ǘ��҃R�[�h
	ISet	eLinkAttr;			///< �����N���
	ISet	eBypass;			///< �o�C�p�X�t���O
	ISet	eAuto;				///< �����Ԑ�p�t���O
	ISet	eWidth;				///< ���H����
	ISet	eLane;				///< �Ԑ����R�[�h
	ISet	eRest;				///< ��ʋK����ʃR�[�h
	ISet	eCond;				///< ��ʋK��������ʃR�[�h
};

/**
 * @brief �����N����
 */
struct LinkAttrList
{
	LinkInfo	eLinkInfo;		///< �����N���
	ISet		eAdminCode;		///< �s�����R�[�h
	ISet		eDispClass;		///< �\���p��ʃR�[�h
	ISet		eNaviClass;		///< �o�H�p��ʃR�[�h
	ISet		eAreaFlag;		///< �L��t���O
	ISet		eParkClass;		///< ���ԏ���
	ISet		eParkLinkClass;	///< ���ԏꃊ���N���
	ISet		eUpDown;		///< �㉺�֌W
	ISet		eAttrClass;		///< ������ʃR�[�h
	ISet		eGradFlag;		///< �X�΃t���O
	ISet		eGradValue;		///< �X�Ίp�x
	MError		eError;			///< �G���[���b�Z�[�W
}; 

/////////////////////////////////////////////
//  �m�[�h���C���֘A
/////////////////////////////////////////////
/**
 * @brief �m�[�h����
 */
struct NodeAttrList
{
	ISet		eSignal;		///< �M���@�t���O
	ISet		eNodeClass;		///< �m�[�h��ʃR�[�h
	ISet		eHeight;		///< ���H�����L���t���O
	MError		eError;			///< �G���[���b�Z�[�W
};

/////////////////////////////////////////////
//  �V��ʋK�����C���֘A
/////////////////////////////////////////////
/**
 * @brief �V��ʋK������
 */
struct RegAttrList
{
	ISet			eDays;			///< �j���R�[�h
	ISet			eCond;			///< �K������
	ISet			eLink;			///< �ʍs�K���R�[�h
	MError			eError;			///< �G���[���b�Z�[�W
};

/////////////////////////////////////////////
//  �m�[�h��񃌃C���֘A
/////////////////////////////////////////////
/**
 * @brief �m�[�h��񑮐�
 */
struct NInfoAttrList
{
	ISet			eClass;			///< �m�[�h����ʃR�[�h
	ISet			eGuideClass;	///< �n����
	ISet			eGuideCode;		///< �ē��R�[�h
	ISet			eUseOKCharDirCode;	///< �g�p�������i���ʈē��p�L���j
	ISet			eUseOKCharNodeCode;	///< �g�p�������i�L���j
	MError			eError;			///< �G���[���b�Z�[�W
};

/////////////////////////////////////////////
//  ���[����񃌃C���֘A
/////////////////////////////////////////////
/**
 * @brief ���[����񑮐�
 */
struct LaneAttrList
{
	MError			eError;			///< �G���[���b�Z�[�W
};

/////////////////////////////////////////////
//  �����N��񃌃C���֘A
/////////////////////////////////////////////
/**
 * @brief �����N��񑮐�
 */
struct LInfoAttrList
{
	ISet			eDisp;			///< �\����ʃR�[�h
	ISet			eSpeed;			///< �K�����x
	ISet			eSection;		///< ��]�敪
	MError			eError;			///< �G���[���b�Z�[�W
};

/////////////////////////////////////////////
//  �����N�g�����C���֘A
/////////////////////////////////////////////
/**
 * @brief �����N�g������
 */
struct LinkExAttrList
{
	ISet			eKey;			///< �����N���L�[
	ISet			eSpeed;			///< �K�����x
	ISet			eSection;		///< ��]�敪
	ISet			eIsland;		///< �����t���O
	ISet			eDetail;		///< �׊X�H��ƔN�x�t���O
	MError			eError;			///< �G���[���b�Z�[�W
};

/**
 * @brief �����N��񑮐�
 */
struct AttrList
{
	HeadAttrList	eINHead;		///< �w�b�_���
	LinkAttrList	eLinkLayer;		///< �����N���C��
	NodeAttrList	eNodeLayer;		///< �m�[�h���C��
	RegAttrList		eRegLayer;		///< �V��ʋK�����C��
	NInfoAttrList	eNInfoLayer;	///< �m�[�h��񃌃C��
	LaneAttrList	eLaneLayer;		///< ���[����񃌃C��
	LInfoAttrList	eLInfoLayer;	///< �����N��񃌃C��
	LinkExAttrList	eLinkExLayer;	///< �����N�g�����C��
};

/**
 * @brief 10�i����16�i���\���ɕϊ�����B
 * @note  �w���10�i���l��16�i���\���ɕϊ�����B
 *
 * @param	rCstrID		[out]	16�i���\���p������
 * @param	rId			[in]	10�i���l
 *
 * @return	�Ȃ�
 */
extern void SetObjectID(CString& rCstrID, int rId);


/**
 * @brief IN�f�[�^��l��ݒ肷��N���X
 */
class CSetRoadAttrList
{
public:
	CSetRoadAttrList(void);
	~CSetRoadAttrList(void);

	/**
	 * @brief IN�f�[�^�̊�l���i�[����B
	 * @note  �����Ƃ��āAIN�f�[�^�̊�l���i�[����B
	 *
	 * @param	rPolDir		[in]	pol_code.txt�i�[�f�B���N�g���p�X
	 * @param	rAttr		[out]	��l�i�[��
	 *
	 * @return	true	����I��
	 * @return false	�ُ�I��
	 */
	bool setList( CString& rPolDir, AttrList& rAttr );

private:

	/**
	 * @brief IN�f�[�^�w�b�_���̊�l���i�[����B
	 * @note  �����Ƃ��āAIN�f�[�^�w�b�_���̊�l���i�[����B
	 *
	 * @param	rINHead		[out]	IN�f�[�^�w�b�_����l�i�[��
	 *
	 * @return	�Ȃ�
	 */
	void setINHead( HeadAttrList& rINHead );

	/**
	 * @brief ���H�����N�̊�l���i�[����B
	 * @note  �����Ƃ��āA���H�����N�̊�l���i�[����B
	 *
	 * @param	rLinkAttr		[out]	���H�����N��l�i�[��
	 * @param	rPolDir			[in]	pol_code.txt�i�[�f�B���N�g���p�X
	 *
	 * @return	true	����I��
	 * @return	false	�ُ�I��
	 */
	bool setLinkLayer( LinkAttrList& rLinkAttr, CString& rPolDir );

	/**
	 * @brief ���H�m�[�h�̊�l���i�[����B
	 * @note  �����Ƃ��āA���H�m�[�h�̊�l���i�[����B
	 *
	 * @param	rNodeAttr		[out]	���H�m�[�h��l�i�[��
	 *
	 * @return	�Ȃ�
	 */
	void setNodeLayer( NodeAttrList& rNodeAttr );

	/**
	 * @brief �V��ʋK���̊�l���i�[����B
	 * @note  �����Ƃ��āA�V��ʋK���̊�l���i�[����B
	 *
	 * @param	rRegAttr		[out]	�V��ʋK����l�i�[��
	 *
	 * @return	�Ȃ�
	 */
	void setRegLayer( RegAttrList& rRegAttr );

	/**
	 * @brief �m�[�h���̊�l���i�[����B
	 * @note  �����Ƃ��āA�m�[�h���̊�l���i�[����B
	 *
	 * @param	rNInfoAttr		[out]	�m�[�h����l�i�[��
	 *
	 * @return	�Ȃ�
	 */
	void setNInfoLayer( NInfoAttrList& rNInfoAttr );

	/**
	 * @brief ���[�����̊�l���i�[����B
	 * @note  �����Ƃ��āA���[�����̊�l���i�[����B
	 *
	 * @param	rLaneAttr		[out]	���[������l�i�[��
	 *
	 * @return	�Ȃ�
	 */
	void setLaneLayer( LaneAttrList& rLaneAttr );

	/**
	 * @brief �����N���̊�l���i�[����B
	 * @note  �����Ƃ��āA�����N���̊�l���i�[����B
	 *
	 * @param	rLInfoAttr		[out]	�����N����l�i�[��
	 * @param	rLinkAttr		[in]	�����N��l�i�[��
	 *
	 * @return	�Ȃ�
	 */
	void setLInfoLayer( LInfoAttrList& rLInfoAttr, LinkAttrList& rLinkAttr );

	/**
	 * @brief �����N�g���̊�l���i�[����B
	 * @note  �����Ƃ��āA�����N�g���̊�l���i�[����B
	 *
	 * @param	rLinkExAttr		[out]	�����N�g����l�i�[��
	 * @param	rLInfoAttr		[in]	�����N����l�i�[��
	 *
	 * @return	�Ȃ�
	 */
	void setLinkExLayer( LinkExAttrList& rLinkExAttr, LInfoAttrList& rLInfoAttr );
};
