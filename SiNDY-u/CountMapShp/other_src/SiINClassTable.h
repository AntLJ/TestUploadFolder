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

/*
**	SiINClassTable.h
**
**		IN class table class  header file
**
**		Programmed by H.Miyahara
**
**		EDITION:
**			1999/4/13	zcl ����ҏW
*/

#ifndef	___IN_CLASS_TABLE___
#define	___IN_CLASS_TABLE___

//#include "SiZIN.h"
#include <string>
#include <vector>
#include <set>
#include "stdafx.h"

using	namespace	std;

#define		TYPE_POLY	2
#define		TYPE_LINE	1
#define		TYPE_POINT	0

/**
	SiNDY�̑����ƁAIN�f�[�^��ʂ̑Ή��\���쐬����N���X
*/
class SiINClassTable
{
public:
	/**
		�ǂݍ��݃��x���̒�`
	*/
	enum	Read_Lv	{
		all	= 0,
		mid,
		base,
		city,
		top
	};
public:
	struct Peace
	{
	//	char	eSiFieldName[11];	///< SiNDY��̃��C����
		string	eSiFieldName;
		int		eData_Type;			///< �f�[�^�^�C�v
		int		eUnder_Flag;		///< �n��A�n���t���O
		int		eSiClass;			///< SiNDY��ʃR�[�h
		int		eINClass;			// IN���
		char	eName[128];			// ��ʖ���
		int		m_Top_ExF;			///< �g�b�v�}�b�v���݃t���O 2010.08.18
		int		m_Mid_ExF;			///< �~�h���}�b�v���݃t���O 2010.08.18
		int		m_Base_ExF;			///< �x�[�X�}�b�v���݃t���O 2010.08.18
		int		m_City_ExF;			///< �s�s�n�}���݃t���O 2010.08.18
#ifdef	DB_ROT_RULE	// 2006.10.12
		char	eRot_Rule[12];		///< ���L�̉�]���[�� 2006.10.11
#endif
		int		eIndex;

		Peace	(void) {
			eData_Type	= TYPE_POINT;	// �|�C���g�ŏ�����
			eUnder_Flag	= 0;
			eSiClass	= 0;
			eINClass	= 0;
			memset ( eName, '\0', 128 );
#ifdef	DB_ROT_RULE	// 2006.10.12
			memset ( eRot_Rule, '\0', 12 );	// 2006.10.11
#endif
			m_Top_ExF	= 0;
			m_Mid_ExF	= 0;
			m_Base_ExF	= 0;
			m_City_ExF	= 0;
			eIndex		= 0;
		}
		Peace	(int cClass) : eSiClass(cClass) { }
		Peace	(int cClass, int cIndex) : eSiClass(cClass), eIndex(cIndex) { }

		void	setData	(string c_FieldName, int cData_Type, int cUnder_Flag, int cSiClass) {
				//strcpy		(eSiFieldName, c_FieldName);
				eSiFieldName= c_FieldName;
				eData_Type	= cData_Type;
				eUnder_Flag	= cUnder_Flag;
				eSiClass	= cSiClass;
		}
		void	setData (string c_FieldName, char* c_Name){
				eSiFieldName	= c_FieldName;
			//	strcpy	(eSiFieldName, c_FieldName);
				strcpy_s(eName, c_Name);
		}
		/**
			�t�B�[�`������f�[�^���Z�b�g����
			���S�e�[�u�� = 0, �s�s�n�} = 3x, �x�[�X = 2x, �~�h�� = 1x
			@return	����	true
			@return	���s	false
		*/
		bool	Set_Data	(
					_IRowPtr		c_Feature,			///< �t�F�b�`���Ă����t�B�[�`��[IN]
					int			c_iData_Level		///< �f�[�^�̃��x��	�@
				);
		/**
			Unicode�̕����񂩂�SJIS�ɂ���
		*/
		string	Str_to_SJIS		(
					CComBSTR*	c_CStr		// ������(Unicode)
				);
		/**
			���C��ID����A�����̃t�B�[���h���ƃf�[�^�^�C�v�l������
			@return	����	true
			@return	���s	false
		*/
		bool	Get_Field_DType	(
					int			c_sLayer_ID,	///< ���C��ID[IN]
					string*		c_spFName,		///< �t�B�[���h��[OUT]
					int*		c_ipDType		///< �f�[�^�^�C�v[OUT]
				);
	};

	
	// ��r�֐�
	/*
	struct lessC : public binary_function<Peace, Peace, bool>
	{
		bool operator () (const Peace cPeace1, const Peace cPeace2) const
		{
			return cPeace1.eSiClass < cPeace2.eSiClass;
		}
	};
	*/

	struct lessC : public binary_function<Peace, Peace, bool>
	{
		// SiNDY��̃��C�����ƁASiNDY��̎�ʃR�[�h�����̔�r�Ń��j�[�N�ɂȂ�͂�
		int operator () (const Peace cPeace1, const Peace cPeace2) const
		{
			int	ret	= 0;
			/*
			int	cmp = strcmp(cPeace1.eSiFieldName, cPeace2.eSiFieldName);	// ���C������
			if(cmp != 0) {
				if(cmp < 0) {
					ret	= 1;
				}
			}else
			*/
			if( cPeace1.eSiFieldName != cPeace2.eSiFieldName ) {
				if( cPeace1.eSiFieldName < cPeace2.eSiFieldName ) {
					ret	= 1;
				}
			}else
			if( cPeace1.eData_Type != cPeace2.eData_Type ) {		// �f�[�^�^�C�v
				if( cPeace1.eData_Type < cPeace2.eData_Type ) {
					ret	= 1;
				}
			}else
			if( cPeace1.eUnder_Flag != cPeace2.eUnder_Flag ) {		// �n��A�n���t���O
				if( cPeace1.eUnder_Flag < cPeace2.eUnder_Flag ) {
					ret	= 1;
				}
			}else
			if(cPeace1.eSiClass != cPeace2.eSiClass) {
				if(cPeace1.eSiClass < cPeace2.eSiClass) {
					ret	= 1;
				}	
			}
			return (ret);
		}
	};
public:
	static multiset<Peace, lessC>	mTable;
	//static set<Peace>	mTable;
public:

	SiINClassTable	() {
	}
	/*
	static const char*	GetName			(char *cFieldName, int cClass );
	*/
	//static const int	GetINClassCode	(char *cFieldName, int cData_Type, int cUnder_Flag, int cSiClass );
	static const int	GetINClassCode	(string cFieldName, int cData_Type, int cUnder_Flag, int cSiClass );

	/**
		SiNDY�̏�񂩂�IN�����̑Ή��������o��
	*/
	Peace				GetSiToINInfo	(
							string		cFieldName,
							int			cData_Type,
							int			cUnder_Flag,
							int			cSiClass
						);

	/**
		Sindy�AIN�����Ή��e�[�u���̔z����쐬����
		@return �Ԃ�l	���� = 0, ���s = -1
	*/
	 int	Read_Table		(
				char	*c_RFName
			);

	 /**
		DB���瑮���Ή��e�[�u����ǂݍ���
		Sindy�AIN�����Ή��e�[�u���̔z����쐬����
		���f�[�^���x�����w�肵�ĕK�v�Ƃ��郌�R�[�h�����ǂݍ���
		�@�S�e�[�u�� = 0, �s�s�n�} = 3x, �x�[�X = 2x, �~�h�� = 1x
		@return �Ԃ�l	���� = 0, ���s = -1
	 */
	int		Read_DB_Table	(
				char					*c_cpLayer_Name,		///< ���C������
				IFeatureWorkspacePtr	c_ipWorkSpace,			///< �t�B�[�`�����[�N�X�y�[�X
				int						c_iData_Level			///< �f�[�^�̃��x��
			);

#ifdef	DB_ROT_RULE	// 2006.10.18
	/**
		�����e�[�u����ROT_RULE�t�B�[���h�̏����擾���Ă��Ă��邩�̃`�F�b�N
		@return	bool
	*/
	bool	Read_ROT_RULE	( void ) {
				Peace	a_pFirstRec	= *(mTable.begin());
				if( a_pFirstRec.eRot_Rule[0] == '\0' ) {
					return ( false );
				}
				return ( true );
			}
#endif

};

#endif	// ___IN_CLASS_TABLE___

