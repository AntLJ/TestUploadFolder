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

// crd_cnv, z19 �𓝍��J�n	1999.12.27 by t.kudo

#ifndef __CRD_CNV_HEADDER__
#define	__CRD_CNV_HEADDER__

#define _CRT_SECURE_NO_DEPRECATE 1 // ANSI-C�W���̊댯�Ȋ֐��̌x�����o���Ȃ�
#define _SCL_SECURE_NO_DEPRECATE 1 // C4996�̌x�����o���Ȃ�
#pragma warning(disable: 4996)  // _SCL_SECURE_NO_DEPRECATE��_CRT_SECURE_NO_DEPRECATE��C4996���������Ȃ��I�I

// �C���N���[�h�t�@�C��
#include <stdio.h>
#include <stdlib.h>

// 2009.01.13
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <string>

#if _MSC_VER >= 1400
#define _USE_MATH_DEFINES
#endif // _MSC_VER >= 1400

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

// DLL�쐬�̂��ߒǉ� 2009.06.25
#ifdef	CRD_CNV_EXPORTS
#define	CRD_CNV_API	__declspec(dllexport)
#else
#ifdef	_USE_CRD_CNV_DLL	// DLL���g�p����ꍇ
#define	CRD_CNV_API	__declspec(dllimport)
#else
#define	CRD_CNV_API			// ���̑�(�X�^�e�B�b�N���C�u�����쐬�E�g�p���j
#endif	// _USE_CRD_CNV_DLL
#endif	// CRD_CNV_EXPORTS

// �\���̒�`
struct	dms			/// �o�ܓx��\��(�x���b)
{
	int		deg;	///< �x
	int		min;	///< �� 
	double	sec;	///< �b
};

struct	c19_origin	/// �P�X���W�n���_�p�\����
{
	dms		lon;	///< �o�x
	dms 	lat;	///< �ܓx	
};

struct	d_lonlat	/// �o�ܓx��\��(10�i����)
{
	double	lon;	///< �o�x
	double	lat;	///< �ܓx
};

struct	xy_coord	/// ���s���W�n��XY��\��(19���W�n�A�K�E�X�E�N�����[�Q�����W�n)
{
	double	x;		///< X���W�i�c�����j
	double	y;		///< Y���W�i�������j
};

struct	coord19		/// 19���W�n�̌n��XY��\��
{
	int		sys;	///< �n
	double	x;		///< X���W�i�c�����j
	double	y;		///< Y���W�i�������j
};

// 1998.7.22 �쐬
struct	sys_num		/// �P�X���W�n�̌n�ԍ�
{
	int		total;	///< �Y����
	int		sys[2];	///< �n�ԍ�
};

/**
 * @brief �n�����W�ϊ��N���X
*/
class CRD_CNV_API	crd_cnv
{
	private:
		/**
		* @class param_7
		* @brief ���n�n�ϊ��p�p�����[�^�i7�p�����[�^�j
		* 
		* 2007.01.21	[Bug 5755] �̗v�]�ɂ��ǉ�
		*/
		struct	param_7
		{
			double	T1;	///< X�������̕��s�ړ��ʁicm�j
			double	T2;	///< Y�������̕��s�ړ��ʁicm�j
			double	T3;	///< Z�������̕��s�ړ��ʁicm�j
			double	D;	///< �X�P�[���␳��(1D-8)
			double	R1;	///< X�����̉�]�i1/1000�b�j
			double	R2;	///< Y�����̉�]�i1/1000�b�j
			double	R3;	///< Z�����̉�]�i1/1000�b�j

			param_7() {
				T1	= 0.0;
				T2	= 0.0;
				T3	= 0.0;
				D	= 0.0;
				R1	= 0.0;
				R2	= 0.0;
				R3	= 0.0;
			}
		};
	private:
		d_lonlat			Rad_Origin[19];					///< �P�X���W���_

		std::map<std::string, int>	m_mCode_Sys19;			///< �ǂݍ��񂾃p�����[�^���i�[����

		/**
		* @brief �P�X���W�n�̕\����郁���o�֐�
		* 
		* 2000.1.12 checked
		*/
		void				Make19tbl	(	void	);
		
		/**
		* @brief �ԓ�����ܓxPhi�܂ł̎q�ߐ��ǒ��𓾂�
		* 
		* 2000.1.12 checked
		* @return �q�ߐ��ǒ�(m)
		*/
		double				M_Arc_Len	(
									double	Ra,				///< �n���ȉ~�̂̒����a�i���[�g���j
									double	ea2,			///< [in] ��ꗣ�S����2��
									double	Phi				///< [in] �ܓx
								);

		d_lonlat			Rad_Origin_Gauss[3];		///< �K�E�X�E�N�����[�Q�����W�n�p 2000.9.19
		/**
		* @brief �K�E�X�E�N�����[�Q�����W�n�̕\����郁���o�֐�
		*/
		void				Make_Gauss_tbl	(	void	);
		// 2001.08.29 �e�X�g�p
		//double			M_Arc_Len_WGS84	(double);	// 2001.08.29 checked

		// ���ۓs�s�n�}�֘A�̃��b�V�����_�ʒu 2006.09.11
		d_lonlat			ORIGIN_JPN;
		d_lonlat			ORIGIN_EU;
		d_lonlat			ORIGIN_NA;
		d_lonlat			ORIGIN_AH;
		d_lonlat			ORIGIN_SEAA;
		d_lonlat			ORIGIN_SA;	// 2008.12.01
		d_lonlat			ORIGIN_SJ;	// 2011.12.08

		//--------------------------------------------------------------------
		/**
		* 2006.09.06 �@�\����p
		* @brief ���b�V�� + (X,Y)����o�ܓx�֕ϊ�(double��)
		* @retval	0	����
		* @retval	-1	���s
		*/
		//--------------------------------------------------------------------
		int			MeshtoLL2_Core(
							unsigned int	meshcode,		///< [in]  ���b�V���R�[�h
							double			x,				///< [in]  ���K�����WX
							double			y,				///< [in]  ���K�����WY
							double			*lon,			///< [out]  �o�x
							double			*lat,			///< [out]  �ܓx
							double			offset_lon,		///< [in]  ���b�V�����_�̃I�t�Z�b�g�l�i�o�x�j
							double			offset_lat		///< [in]  ���b�V�����_�̃I�t�Z�b�g�l�i�ܓx�j
						);

		//--------------------------------------------------------------------
		/**
		* 2006.09.07 �@�\����p
		* @brief ���b�V��(������) + (X,Y)����o�ܓx�֕ϊ�(double��)
		* @retval	0	����
		* @retval -1	���s
		*/
		//--------------------------------------------------------------------
		int			MeshtoLL2_Core(
							char*	meshcode,		///< [in]  ���b�V���R�[�h
							double	x,				///< [in]  ���K�����WX
							double	y,				///< [in]  ���K�����WY
							double	*lon,			///< [out]  �o�x
							double	*lat,			///< [out]  �ܓx
							double	offset_lon,		///< [in]  ���b�V�����_�̃I�t�Z�b�g�l�i�o�x�j
							double	offset_lat		///< [in]  ���b�V�����_�̃I�t�Z�b�g�l�i�ܓx�j
						);

		//--------------------------------------------------------------------
		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���E���ʔ�(���{��))
		*
		* 2006.09.06
		* @retval	0	����
		* @retval -1	���s
		*/
		//--------------------------------------------------------------------
		int			LLtoMesh_Core(
							double			lon,			///< [in]  �o�x
							double			lat,			///< [in]  �ܓx
							int				level,			///< [in]  ���b�V���̃��x�� (1,2,64)
							unsigned int	*meshcode,		///< [out]  ���b�V���R�[�h
							int				*x,				///< [out]  ���K��X
							int				*y,				///< [out]  ���K��Y
							double			offset_lon,		///< [in]  ���b�V�����_�̃I�t�Z�b�g�l�i�o�x�j
							double			offset_lat,		///< [in]  ���b�V�����_�̃I�t�Z�b�g�l�i�ܓx�j
							int				no_mill=0		///< [in]  XY���W��1000000�ɂȂ邱�Ƃ����e���邩�i0=���e,1=���e���Ȃ�=�ׂ̃��b�V����0�Ƃ��ĕԂ��j 
						);

		//--------------------------------------------------------------------
		/**
		* @brief �o�ܓx����A���b�V���R�[�h(������) + (x,y) �����߂�(���E���ʔ�(���{��))
		*
		* 2006.09.08
		* @retval	0	����
		* @retval -1	���s
		*/
		//--------------------------------------------------------------------
		int			LLtoMesh_Core(
							double	lon,			///< [in]  �o�x
							double	lat,			///< [in]  �ܓx
							int		level,			///< [in]  ���b�V���̃��x�� (1,2,64)
							char	*meshcode,		///< [out]  ���b�V���R�[�h
							int		*x,				///< [out]  ���K��X
							int		*y,				///< [out]  ���K��Y
							double	offset_lon,		///< [in]  ���b�V�����_�̃I�t�Z�b�g�l�i�o�x�j
							double	offset_lat,		///< [in]  ���b�V�����_�̃I�t�Z�b�g�l�i�ܓx�j
							int		no_mill=0		///< [in]  XY���W��1000000�ɂȂ邱�Ƃ����e���邩�i0=���e,1=���e���Ȃ�=�ׂ̃��b�V����0�Ƃ��ĕԂ��j 
						);


		//--------------------------------------------------------------------
		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
		*
		* �w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂� 2006.09.07
		* @retval	0	����
		* @retval -1	���s
		*/
		//--------------------------------------------------------------------
		int			LLtoMesh2_Core(
							double			c_lon,			///< [in]  �o�x
							double			c_lat,			///< [in]  �ܓx
							unsigned int	c_meshcode,		///< [in]  ���b�V���R�[�h
							double			*c_x,			///< [out]  ���K��X
							double			*c_y,			///< [out]  ���K��Y
							double			offset_lon,		///< [in]  ���b�V�����_�̃I�t�Z�b�g�l�i�o�x�j
							double			offset_lat		///< [in]  ���b�V�����_�̃I�t�Z�b�g�l�i�ܓx�j
						);

		//--------------------------------------------------------------------
		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
		*
		* �w�肵�����b�V���R�[�h(������)�ɂ������āA���K�����W�l�����߂� 2006.09.08
		* @retval	0	����
		* @retval -1	���s
		*/
		//--------------------------------------------------------------------
		int			LLtoMesh2_Core(
							double	c_lon,			///< [in]  �o�x
							double	c_lat,			///< [in]  �ܓx
							char	*c_meshcode,	///< [out]  ���b�V���R�[�h
							double	*c_x,			///< [out]  ���K��X
							double	*c_y,			///< [out]  ���K��Y
							double	offset_lon,		///< [in]  ���b�V�����_�̃I�t�Z�b�g�l�i�o�x�j
							double	offset_lat		///< [in]  ���b�V�����_�̃I�t�Z�b�g�l�i�ܓx�j
						);

		//=======================================================================
		//	���L3�̃����o�֐��ABLHtoXYZ(), XYZtoBLH(), XYZtoXYZ()	�́A
		//  [Bug 5755] �̗v�]�ɂ��ǉ� 2008.01.21
		//	���y�⑫�����z
		//	����3�̑��n�n�ϊ��֐��́A��c ���v�������A�u���E���n�n�ƍ��W�ϊ��v
		//  �ŉ������Ă����@�����ɍ쐬����Ă��܂��B
		//=======================================================================
		//--------------------------------------------------------------------
		/**
		* @brief �n�����W�l����O�����������W�l�ւ̕ϊ��@
		*
		* (B,L,He)->(X,Y,Z)
		* @return	����
		*/
		//--------------------------------------------------------------------
		void		BLHtoXYZ(
							double	c_dA,			///< [in]  ��]�ȉ~�̒����a(m)
							double	c_dInvF,		///< [in]  �G����f�̋t��1/f
							double	c_dLon,			///< [in]  �o�x
							double	c_dLat,			///< [in]  �ܓx
							double	c_dHe,			///< [in]  �ȉ~�̍�(m)
							double*	c_dX,			///< [out]  ���_�����X�������̋���(m)
							double*	c_dY,			///< [out]  ���_�����Y�������̋���(m)
							double*	c_dZ			///< [out]  ���_�����Z�������̋���(m)
						);

		//--------------------------------------------------------------------
		/**
		* @brief �O�����������W�l����n�����W�l�ւ̕ϊ�
		*
		* (X,Y,Z)->(B,L,He)
		* @return	����
		*/
		//--------------------------------------------------------------------
		void		XYZtoBLH(
							double	c_dA,			///< [in]  ��]�ȉ~�̒����a(m)
							double	c_dInvF,		///< [in]  �G����f�̋t��1/f
							double	c_dX,			///< [in]  ���_�����X�������̋���(m)
							double	c_dY,			///< [in]  ���_�����Y�������̋���(m)
							double	c_dZ,			///< [in]  ���_�����Z�������̋���(m)
							double*	c_dLon,			///< [out]  �o�x
							double*	c_dLat,			///< [out]  �ܓx
							double*	c_dHe			///< [out]  �ȉ~�̍�(m)
						);


		//---------------------------------------------------------------------
		/**
		* @brief �O�����������W�Ԃł̍��W�ϊ�
		* @return	����
		*/
		//---------------------------------------------------------------------
		void		XYZtoXYZ(
							param_7	c_pParam7,		///< [in]  �ϊ��Ɏg�p����7�p�����[�^
							double	c_dXi,			///< [in]  ���_�����X�������̋���(m)
							double	c_dYi,			///< [in]  ���_�����Y�������̋���(m)
							double	c_dZi,			///< [in]  ���_�����Z�������̋���(m)
							double*	c_dXo,			///< [out]  ���_�����X�������̋���(m)
							double*	c_dYo,			///< [out]  ���_�����Y�������̋���(m)
							double*	c_dZo			///< [out]  ���_�����Z�������̋���(m)
						);

		//---------------------------------------------------------------------
		/**
		* @brief �o�ܓx�̑��n�n���ݕϊ��֐�
		*
		* 7�p�����[�^�ƕϊ��O��̑ȉ~�̂̏�񂪂���΂��̊֐��Ōo�ܓx�̑���
		* �ϊ����\�B���󓌋����n�n(Tokyo97)�Ɛ��E���n�n(JGD2000)�Ԃ̕ϊ�����
		* �s��Ȃ��̂ƁA����������邽�ߎ�肠�����v���C�x�[�g�ɂ��Ă���
		* @return	����
		*/
		//---------------------------------------------------------------------
		void		BLHtoBLH(
							double	c_dBefore_A,	///< [in]  �ϊ��O���W�n�̏�������ȉ~�̒����a(m)
							double	c_dBefore_InvF,	///< [in]  �ϊ��O���W�n�̏�������ȉ~�̝̂G����
							double	c_dAfter_A,		///< [in]  �ϊ��O���W�n�̏�������ȉ~�̒����a(m)
							double	c_dAfter_InvF,	///< [in]  �ϊ��O���W�n�̏�������ȉ~�̝̂G����
							param_7	c_pBtoAParam7,	///< [in]  �ϊ��Ɏg�p����7�p�����[�^
							double	c_dBefore_Lon,	///< [in]  �ϊ��O�o�x
							double	c_dBefore_Lat,	///< [in]  �ϊ��O�ܓx
							double	c_dBefore_He,	///< [in]  �ϊ��O�ȉ~�̍�(m)
							double*	c_dAfter_Lon,	///< [out]  �ϊ���o�x
							double*	c_dAfter_Lat,	///< [out]  �ϊ���ܓx
							double*	c_dAfter_He		///< [out]  �ϊ���ȉ~�̍�(m)
						);

		//---------------------------------------------------------------------
		/**
		* @brief �o�ܓx����P�X���W�n�̂w�x�֕ϊ�(�ȉ~�̏����w��)
		* @return ����
		*/
		//---------------------------------------------------------------------
		void	LLtoXY19_Core	(
							double		Ra,			///< [in] �n���ȉ~�̂̒����a�i���[�g���j
							double		ea2,		///< [in] ��ꗣ�S����2��
							double		eb2,		///< [in] ��񗣐S����2��
							int			axis,		///< [in] �Y��������W�n�̔ԍ�
							double		lon,		///< [in] �o�x
							double		lat,		///< [in] �ܓx
							int			mode,		///< [in] �ϊ����[�h(0=���{���n�n,1=���E���n�n)
							double		*x,			///< [out] 19���W�n��X
							double		*y			///< [out] 19���W�n��Y
						);

		//------------------------------------------------------------
		/**
		* @brief �P�X���W�n�̂w�x����o�ܓx�֕ϊ�(�ȉ~�̏����w��)
		* @return ����
		*/
		//------------------------------------------------------------
		void 	XY19toLL_Core	(
							double		Ra,			///< [in] �n���ȉ~�̂̒����a�i���[�g���j
							double		ea2,		///< [in] ��ꗣ�S����2��
							double		eb2,		///< [in] ��񗣐S����2��
							double		f,			///< [in] �ȉ~�̝̂G����
							int			axis,		///< [in] �Y��������W�n�̔ԍ�
							double		x,			///< [in] �k�����̍��W�l
							double		y,			///< [in] �������̍��W�l
							int			mode,		///< [in] �ϊ����[�h(0=���{���n�n,1=���E���n�n)
							double		*lon,		///< [in] �o�x
							double		*lat		///< [in] �ܓx
						);

		//---------------------------------------------------------------------
		/**
		* @brief �Q�n�_�̌o�ܓx����A�Q�_�Ԃ̋��� + ����(X,Y)m�����߂�
		* 
		* ���Fzcl����v�Z���͂قƂ�ǑS�����p�����Ă�����Ă���B(class ZLonLat)
		* @return		�Q�n�_�Ԃ̋���
		*/
		//---------------------------------------------------------------------
		double	GetDistXY_Core	(
							double		Ra,			///< [in] �n���ȉ~�̂̒����a�i���[�g���j
							double		ea2,		///< [in] ��ꗣ�S����2��
							double		eLon1,		///< [in] �o�x�P
							double		eLat1,		///< [in] �ܓx�P
							double		eLon2,		///< [in] �o�x�Q
							double		eLat2,		///< [in] �ܓx�Q
							double		*cDist_X,	///< [out] ����X(m)
							double		*cDist_Y	///< [out] ����Y(m)
						);

		//---------------------------------------------------------------------
		/**
		* @brief	�ԓ�����ܓxPhi�܂ł̎q�ߐ��ǒ��𓾂�
		*
		* �y�Q�l�z
		*	http://vldb.gsi.go.jp/sokuchi/surveycalc/algorithm/b2s/b2s.htm
		* @return �q�ߐ��ǒ�(m)
		*/
		//---------------------------------------------------------------------
		double	M_Arc_Len_Core	(
							double		Ra,		///< [in] �n���ȉ~�̂̒����a�i���[�g���j
							double		ea2,	///< [in] ��ꗣ�S����2��
							double		Phi		///< [in] �ܓx
						);

	private:
		
		//------------------------------------------------------------------------------------------------
		/* 2009�N01��13�� �n�斈�̃p�����[�^�ł̕ϊ��p�ɒǉ� */
		//------------------------------------------------------------------------------------------------
		enum {
			JGD2000_RP_UNINITED	= 0,///< ��������
			CONVED_REGIONPRM,		///< �n�斈�p�����[�^�ŕϊ�
			CONVED_3PRM				///< 3�p�����[�^�ŕϊ����ꂽ
		};

		/**
		* @class param_info
		* @brief TKY2JGD�̒n��p�����[�^�e�[�u���p
		*/
		struct	param_info	
		{
			int		e_imeshcode;	///< 3�����b�V���R�[�h
			double	e_dDlon;		///< �o�x
			double	e_dDlat;		///< �ܓx

			param_info(){		///< �R���X�g���N�^
				e_imeshcode	= 0;
				e_dDlon		= 0.0;
				e_dDlat		= 0.0;
			};
			~param_info() {	///< �f�X�g���N�^
			};
			int	operator<(const param_info &c_info) const{ ///< ��r���Z�q��`
				int ret	= 0;
				if(	(e_imeshcode != c_info.e_imeshcode) ) {
					if(e_imeshcode < c_info.e_imeshcode)
						ret	= 1;
				}
				return(ret);
			};
		};

		std::map<int, std::set<param_info, std::less<param_info>>> m_outCode2nearCodes;	///< �p�����[�^�O��3�����b�V���ƍŋߖT�p�����[�^�Ƃ̃}�b�v�i�p�����[�^�O�̌v�Z�������������邽�߁j

		std::set<param_info, std::less<param_info> >	m_spParam;				///< �ǂݍ��񂾃p�����[�^���i�[����
		std::set<int, std::less<int> >					m_siTokyo97_Area;		///< IPC��Tokyo97�ɊY������2�����b�V��
		bool											m_bJGD2000_RP_Inited;	///< JGD2000�ϊ��@�\�������t���O

		//---------------------------------------------------------------------
		/** 
		* @brief �o�ܓx����A3�����b�V���R�[�h + (x,y) �����߂�
		* @return	����
		*/
		//---------------------------------------------------------------------
		void		LLto3rdMesh	(
							double	lon,			///< [in]  �o�x
							double	lat,			///< [in]  �ܓx
							int		*meshcode,		///< [out]  3�����b�V���R�[�h
							int		*x,				///< [out]  ���K��X
							int		*y,				///< [out]  ���K��Y
							int		no_mill			///< [in]  XY���W��1000000�ɂȂ邱�Ƃ����e���邩�i0=���e,1=���e���Ȃ�=�ׂ̃��b�V����0�Ƃ��ĕԂ��j
						);

		//---------------------------------------------------------------------
		/**
		* @brief 3�����b�V���R�[�h + (x,y) ����A�o�ܓx�����߂� 2007.09.13
		* @return	����	true
		* @return	���s	false
		*/
		//---------------------------------------------------------------------
		bool		Mesh3rdtoLL(
							int		meshcode,		///< [in]  ���b�V���R�[�h
							int		x,				///< [in]  ���K��X
							int		y,				///< [in]  ���K��Y
							double*	lon,			///< [out]  �o�x
							double*	lat				///< [out]  �ܓx
						);

		//---------------------------------------------------------------------
		/**
		* @brief ���_���b�V������(x,y)���ꂽ����3�����b�V���R�[�h�����߂�
		* @return	�����F���b�V���R�[�h
		* @return	���s�F -1
		*/
		//---------------------------------------------------------------------
		int			Get3rdMeshCode(
							int		meshcode,		///< [in]  3�����b�V���R�[�h
							int		xdist,			///< [in]  ���_���b�V������X�����։����b�V������Ă��邩
							int		ydist			///< [in]  ���_���b�V������Y�����։����b�V������Ă��邩
						);

		//---------------------------------------------------------------------
		/**
		* @brief �o�C���j�A��ԂŒl�����߂Ă���
		* @return	�v�Z�l
		*/
		//---------------------------------------------------------------------
		double		Bilinear(
							double	c_dLU,			///< [in]  ����̒l
							double	c_dRU,			///< [in]  �E��̒l
							double	c_dLD,			///< [in]  �����̒l
							double	c_dRD,			///< [in]  �E���̒l
							double	c_dPtX,			///< [in]  ��`����X�����ʒu(0.0<=X<=1.0)
							double	c_dPtY			///< [in]  ��`����Y�����ʒu(0.0<=Y<=1.0)
						);

		//---------------------------------------------------------------------
		/** 
		* @brief �p�����[�^�e�[�u�����獶���A����A�E��A�E���̃p�����[�^���擾����
		* @return	���s == 0
		* @return	�p�����[�^�͈͓̔��ŕϊ� == 1
		* @return	�p�����[�^�͈̔͊O�ŕϊ�(���x���͈͓����Â�) == 2
		*/
		//---------------------------------------------------------------------
		bool		GetParam(
							int			c_iMeshCode,///< [in]  ���b�V���R�[�h
							param_info*	c_pLD_Prm,	///< [out]  �����̃p�����[�^
							param_info*	c_pLU_Prm,	///< [out]  ����̃p�����[�^
							param_info*	c_pRU_Prm,	///< [out]  �E��̃p�����[�^
							param_info*	c_pRD_Prm,	///< [out]  �E���̃p�����[�^
							bool		c_bIPC_Flag	///< [in]	IPC�ŗL�ϊ������邩�ǂ��� true=IPC�ŗL�ϊ� false=���Ԉ�ʑΉ��ϊ�
						);

		//---------------------------------------------------------------------
		/** 
		* @brief �p�����[�^�e�[�u�����狗���I�Ɉ�ԋ߂��p�����[�^���擾����
		* @return	���s == 0
		* @return	�p�����[�^�͈͓̔��ŕϊ� == 1
		* @return	�p�����[�^�͈̔͊O�ŕϊ�(���x���͈͓����Â�) == 2
		*/
		//---------------------------------------------------------------------
		bool		GetNearestParam(
							double		c_dLon,			///< [in]   �o�x
							double		c_dLat,			///< [in]   �ܓx
							param_info*	c_pNearest_Prm,	///< [out]  �����̃p�����[�^
							bool		c_bIPC_Flag		///< [in]	IPC�ŗL�ϊ������邩�ǂ��� true=IPC�ŗL�ϊ� false=���Ԉ�ʑΉ��ϊ�
						);

		//---------------------------------------------------------------------
		/** 
		* @brief �p�����[�^�͈̔͂��`�F�b�N����
		* @return	�͈͓� == true
		* @return	�͈͊O == false
		*/
		//---------------------------------------------------------------------
		bool		ChkParamRange(
						long	c_lLD3rdMesh,			///< [in] ������3�����b�V���R�[�h
						long	c_lRU3rdMesh,			///< [in] �E���3�����b�V���R�[�h
						long	c_lTgt3rdMesh			///< [in] �^�[�Q�b�g��3�����b�V���R�[�h
					);

		//---------------------------------------------------------------------
		/**
		* @brief IPC��Tokyo97�G���A�̃��b�V�����ǂ����𒲂ׂ�
		* @return	���s == false
		* @return	���� == true
		*/
		//---------------------------------------------------------------------
		bool		Is_IPC_Tokyo97_Area(
							int				c_iMeshCode		///< [in]  2�����b�V���R�[�h
						);

		//---------------------------------------------------------------------
		/**
		* @brief �o�ܓx�̑��n�n���ݕϊ�
		* @return	�n��p�����[�^�͈͓̔��ŕϊ� == 1
		* @return	�n��p�����[�^�͈̔͊O�ŕϊ�(���x���͈͓����Â�) == 2
		*/
		//---------------------------------------------------------------------
		int			LLConv(
							double	c_dBefore_Lon,	///< [in]  �ϊ��O�o�x
							double	c_dBefore_Lat,	///< [in]  �ϊ��O�ܓx
							double*	c_dAfter_Lon,	///< [out]  �ϊ���o�x
							double*	c_dAfter_Lat,	///< [out]  �ϊ���ܓx
							int		c_iMode,		///< [in]  �ϊ����[�h�i0=Tokyo->JGD2000�ւ̕ϊ� 1=JGD2000->Tokyo�ւ̕ϊ��j
							bool	c_bIPC_Flag		///< [in]  IPC�ŗL�ϊ������邩�ǂ���true=IPC�ŗL�ϊ� false=���Ԉ�ʑΉ��ϊ�
						);

	public:
		//---------------------------------------------------------------------
		/**
		* @brief JGD2000�p�����[�^�t�@�C����ǂݍ��ݓ����e�[�u�����쐬
		*
		* @code
		* <��>
		* double	blon = 0.0, blat = 0.0; // �ϊ��O�o�ܓx
		* double	alon = 0.0, alat = 0.0; // �ϊ���o�ܓx
		* crd_cnv	crd;
		* crd.JGD2000_RP_Init("TKY2JGD.par");
		* crd.TOKYOtoJGD2000_RP(blon, blat, &alon, &alat, false);
		* @endcode
		* @return	���� == true, ���s == false
		*/
		//---------------------------------------------------------------------
		bool		JGD2000_RP_Init(
							char*			c_fname			///< [in]  �ǂݍ��݃t�@�C����
						);

		//------------------------------------------------------------------------------------------------

	public:

		crd_cnv()		///< �R���X�g���N�^
		{
			// 19���W�n���_�̏�����
			Rad_Origin[0].lat	= 0.575958653158128774940394123404985293745994567871093750000000;
			Rad_Origin[0].lon	= 2.260201381332656467293418245390057563781738281250000000000000;
			Rad_Origin[1].lat	= 0.575958653158128774940394123404985293745994567871093750000000;
			Rad_Origin[1].lon	= 2.286381320112572002045681074378080666065216064453125000000000;
			Rad_Origin[2].lat	= 0.628318530717958623199592693708837032318115234375000000000000;
			Rad_Origin[2].lon	= 2.306743494719171838625015880097635090351104736328125000000000;
			Rad_Origin[3].lat	= 0.575958653158128774940394123404985293745994567871093750000000;
			Rad_Origin[3].lon	= 2.330014551412429746335419622482731938362121582031250000000000;
			Rad_Origin[4].lat	= 0.628318530717958623199592693708837032318115234375000000000000;
			Rad_Origin[4].lon	= 2.344558961845716105187875655246898531913757324218750000000000;
			Rad_Origin[5].lat	= 0.628318530717958623199592693708837032318115234375000000000000;
			Rad_Origin[5].lon	= 2.373647782712288378803577870712615549564361572265625000000000;
			Rad_Origin[6].lat	= 0.628318530717958623199592693708837032318115234375000000000000;
			Rad_Origin[6].lon	= 2.394009957318888659472122526494786143302917480468750000000000;
			Rad_Origin[7].lat	= 0.628318530717958623199592693708837032318115234375000000000000;
			Rad_Origin[7].lon	= 2.417281014012146567182526268879882991313934326171875000000000;
			Rad_Origin[8].lat	= 0.628318530717958623199592693708837032318115234375000000000000;
			Rad_Origin[8].lon	= 2.440552070705404030803720161202363669872283935546875000000000;
			Rad_Origin[9].lat	= 0.698131700797731791219291608285857364535331726074218750000000;
			Rad_Origin[9].lon	= 2.458005363225347572608825430506840348243713378906250000000000;
			Rad_Origin[10].lat	= 0.767944870877504959238990522862877696752548217773437500000000;
			Rad_Origin[10].lon	= 2.447824275922047210229948177584446966648101806640625000000000;
			Rad_Origin[11].lat	= 0.767944870877504959238990522862877696752548217773437500000000;
			Rad_Origin[11].lon	= 2.482730860961933849750948866130784153938293457031250000000000;
			Rad_Origin[12].lat	= 0.767944870877504959238990522862877696752548217773437500000000;
			Rad_Origin[12].lon	= 2.517637446001820489271949554677121341228485107421875000000000;
			Rad_Origin[13].lat	= 0.453785605518525703150345407266286201775074005126953125000000;
			Rad_Origin[13].lon	= 2.478367537831947853277370086289010941982269287109375000000000;
			Rad_Origin[14].lat	= 0.453785605518525703150345407266286201775074005126953125000000;
			Rad_Origin[14].lon	= 2.225294796292770271861627406906336545944213867187500000000000;
			Rad_Origin[15].lat	= 0.453785605518525703150345407266286201775074005126953125000000;
			Rad_Origin[15].lon	= 2.164208272472968541677573739434592425823211669921875000000000;
			Rad_Origin[16].lat	= 0.453785605518525703150345407266286201775074005126953125000000;
			Rad_Origin[16].lon	= 2.286381320112572002045681074378080666065216064453125000000000;
			Rad_Origin[17].lat	= 0.349065850398865895609645804142928682267665863037109375000000;
			Rad_Origin[17].lon	= 2.373647782712288378803577870712615549564361572265625000000000;
			Rad_Origin[18].lat	= 0.453785605518525703150345407266286201775074005126953125000000;
			Rad_Origin[18].lon	= 2.687807048071267690403374217567034065723419189453125000000000;

			// ���{
			ORIGIN_JPN.lon	= 100.0;	// ���o100�x
			ORIGIN_JPN.lat	= 0.0;
			// �����[���b�p
			ORIGIN_EU.lon	= -30.0;	// ���o30�x
			ORIGIN_EU.lat	= 10.0;
			// �k�A�����J
			ORIGIN_NA.lon	= -150.0;// ���o150�x 2003.05.08�C��
			ORIGIN_NA.lat	= 10.0;
			// �A���X�J�E�n���C
			ORIGIN_AH.lon	= -180.0;// ���o 2005.12.22�C��
			ORIGIN_AH.lat	= 10.0;
			// ����A�W�A�E�I�[�X�g�����A
			ORIGIN_SEAA.lon	= 80.0;// ���o80�x
			ORIGIN_SEAA.lat	= -45.0;// ���45�x
			// ��A�����J 2008.12.01 �ǉ�
			ORIGIN_SA.lon	= -120.0;	// ���o120�x
			ORIGIN_SA.lat	= -56.0;	// ���56�x
			// �V�C�O�s�s�n�}�p 2008.12.01 �ǉ�
			ORIGIN_SJ.lon	= -180.0;	// ���o180�x
			ORIGIN_SJ.lat	= -90.0;	// ���90�x

			//--------------------------------------------------------------------------------------------------------
			// IPC�ňȑO���{���n�n��Tokyo97�֑Ή��������Ȃ����ӏ������b�V���P��
			// �œo�^�B
			// ��IPC�ł͈ȑO����̑傫�ȗ����ɂ��āATokyo97�ɍ��킹�鏈�����s�����B
			//   TKY2JGD.par�̃V�t�g�ʂ́A���{���n�n�����E���n�n�֒��ڕϊ����邽�߂�
			//   �V�t�g�ʂł��邽�߁ATokyo97�Ή����s�����n��ł͒n�斈�̃p�����[�^��
			//   �g�p����Ƃ��ꂪ�傫���Ȃ��Ă��܂����߁A���̂悤�ȑΉ����s���K�v������B

			// �{�Ó��A�ɗǕ���
			m_siTokyo97_Area.insert ( 372501 );	m_siTokyo97_Area.insert ( 372502 );	m_siTokyo97_Area.insert ( 372503 );
			m_siTokyo97_Area.insert ( 372511 );	m_siTokyo97_Area.insert ( 372512 );	m_siTokyo97_Area.insert ( 372513 );
			m_siTokyo97_Area.insert ( 372521 );	m_siTokyo97_Area.insert ( 372522 );	m_siTokyo97_Area.insert ( 372523 );
			m_siTokyo97_Area.insert ( 372531 );	m_siTokyo97_Area.insert ( 372532 );
			// ���Ǌԓ��A���[��
			m_siTokyo97_Area.insert ( 362475 );	m_siTokyo97_Area.insert ( 372405 );	m_siTokyo97_Area.insert ( 372415 );
			// �Ί_��
			m_siTokyo97_Area.insert ( 362430 );	m_siTokyo97_Area.insert ( 362431 );	m_siTokyo97_Area.insert ( 362440 );	
			m_siTokyo97_Area.insert ( 362441 );	m_siTokyo97_Area.insert ( 362442 );	m_siTokyo97_Area.insert ( 362450 );	
			m_siTokyo97_Area.insert ( 362451 );	m_siTokyo97_Area.insert ( 362452 );	m_siTokyo97_Area.insert ( 362461 );	
			m_siTokyo97_Area.insert ( 362462 );	m_siTokyo97_Area.insert ( 362472 );	
			// ���\��
			m_siTokyo97_Area.insert ( 362326 );	m_siTokyo97_Area.insert ( 362327 );	m_siTokyo97_Area.insert ( 362420 );
			m_siTokyo97_Area.insert ( 362335 );	m_siTokyo97_Area.insert ( 362336 );	m_siTokyo97_Area.insert ( 362337 );
			m_siTokyo97_Area.insert ( 362345 );	m_siTokyo97_Area.insert ( 362346 );	m_siTokyo97_Area.insert ( 362347 );
			m_siTokyo97_Area.insert ( 362356 );
			// ����_��
			m_siTokyo97_Area.insert ( 362324 );
			// �g�Ɗԓ�
			m_siTokyo97_Area.insert ( 362305 );	m_siTokyo97_Area.insert ( 362306 );
			// �^�ߍ���
			m_siTokyo97_Area.insert ( 362257 );	m_siTokyo97_Area.insert ( 362350 );
			// �k�哌��
			m_siTokyo97_Area.insert ( 383172 );
			// ��哌��
			m_siTokyo97_Area.insert ( 383151 ); m_siTokyo97_Area.insert ( 383152 );
			m_siTokyo97_Area.insert ( 383161 ); m_siTokyo97_Area.insert ( 383162 );
			// ����
			m_siTokyo97_Area.insert ( 483956 );
			// ������
			m_siTokyo97_Area.insert ( 374102 );	m_siTokyo97_Area.insert ( 374112 );	m_siTokyo97_Area.insert ( 374113 );
			//--------------------------------------------------------------------------------------------------------

			m_bJGD2000_RP_Inited	= false;

		}
		~crd_cnv()		///< �f�X�g���N�^
		{
		}

		//---------------------------------------------------------------------
		/**
		    ���_�R�[�h
			���W�ϊ��̐��E���ʉ��̂��ߒǉ��@2006.09.06 
		*/
		//---------------------------------------------------------------------
		enum {
			JPN_ORIGIN	= 0,	///< ���{
			EU_ORIGIN,			///< �����[���b�p
			NA_ORIGIN,			///< �k�A�����J
			AH_ORIGIN,			///< �A���X�J�E�n���C
			SEAA_ORIGIN,		///< ����A�W�A�E�I�[�X�g�����A
			SA_ORIGIN,			///< ��A�����J 2008.12.01 �ǉ�
			SJ_ORIGIN			///< �V���ۓs�s�n�} 2011.12.08 �ǉ�
		};

		//---------------------------------------------------------------------
		/**
		* @brief 19���W�n�̖k�C����5���R�[�h�ƌn�̑Ή���������
		*
		* Sys_Number()�́A(�o�x,�ܓx)�{5���R�[�h�w��ł��g�p����ۂɎg�p����B
		* �������w�肵�Ȃ��A����NULL���w�肵���ꍇ�́A�f�t�H���g�ݒ肳���B
		* �f�t�H���g�ݒ�����̂܂܎g�p����ꍇ�́ASys_Number_Init()�w��͕s�v�B
		* �f�t�H���g�ݒ�ł́A�Z��DB��Y1005�����̃R�[�h�ɑΉ��B
		* �t�@�C�����w�肵���ꍇ�́A�t�@�C������R�[�h�ƌn�̑Ή���ǂݍ��ށB
		* �t�@�C���w���Y1005����A5���R�[�h�ɒǉ����̕ύX�����������ꍇ��
		* �g�p����B
		* ���t�@�C���̃t�H�[�}�b�g�́A
		* <5���R�[�h>\\t<�n�ԍ�>
		* �擪��'#'�������ꍇ�̓R�����g�s�Ƃ��ĔF�������
		*
		* @code
		* <��>
		* char*	code	= "01202";
		* crd_cnv	crd;
		* crd.Sys_Number_Init("Hokkaido_Sys19.tbl");
		* crd.Sys_Number(140.8, 40.8, code);
		* @endcode
		* @return	���� == true, ���s == false
		*/
		//---------------------------------------------------------------------
		bool		Sys_Number_Init(
							char*			c_fname=NULL	///< [in]  �ǂݍ��݃t�@�C����
						);

		/**
		* 
		* @brief �o�ܓx�����ԋ߂��Ƃ����19���W�n�̌n�𓾂�
		* 
		* �i�Ȃ񂩖��ʂȂ��Ƃ����Ă�����ۂ�?�j2000/1/12 checked �H�H
		* @code
		* <��>
		* int		axis = 0;	// �n�ԍ����i�[
		* crd_cnv	crd;
		* axis = crd.Sys_Number(140.0, 39.0);
		* printf("�n = %d\n", axis );
		* @endcode
		* @return     �n�ԍ�
		*/
		int			Sys_Number	(
							double	lon,	///< [in]  �o�x
							double	lat		///< [in]  �ܓx
						);

		/**
		* 
		* @brief �o�ܓx�Ǝs�撬���R�[�h(5��)����19���W�n�̌n�𓾂�
		* 
		* <�Q�l�����N> ���y�n���@��HP�A
		*  �n���@�z�[��  > ���ʖ@  > ���ʒ��p���W�n�i�����\�l�N���y��ʏȍ�����㍆�j 
		* http://www.gsi.go.jp/LAW/heimencho.html
		* @code
		* <��>
		* int		axis = 0;	// �n�ԍ����i�[
		* crd_cnv	crd;
		* axis = crd.Sys_Number(140.0, 39.0);
		* printf("�n = %d\n", axis );
		* @endcode
		* @return     �n�ԍ�(�s�撬���R�[�h��o�ܓx�ɊԈႢ������ꍇ��0��ԋp)
		*/
		int			Sys_Number	(
							double	lon,	///< [in]  �o�x
							double	lat,	///< [in]  �ܓx
							char*	code5	///< [in]  �s�撬���R�[�h(5��)
						);

		/**
		* @brief ���b�V���R�[�h����Y������P�X���W�n�̐��Ɣԍ��𓾂�
		* 
		* �Y������n���Ȃ��ꍇ�A���A�ԍ��Ƃ���0��Ԃ� 2000.1.12 checked
		* @code
		* <��>
		* int		meshcode = 594141;
		* sys_num*	kei = NULL;
		* crd_cnv	crd;
		* kei	=	crd.Sys_Number(meshcode);
		* printf("�Y����=%d: �n1=%d �n2=%d\n", kei->total, kei->sys[0], kei->sys[1]);
		* @endcode
		* @return �\���� sys_num �ւ̃|�C���^
		*/
		sys_num*	Sys_Number	(
							int		meshcode	///< [in]  ���b�V���R�[�h
						);

		/**
		* @brief ���b�V���R�[�h����Y������P�X���W�n�̐��Ɣԍ��𓾂�
		* 
		* �Y������n���Ȃ��ꍇ�A���A�ԍ��Ƃ���0��Ԃ� 2000.1.12 checked
		* @code
		* <��>
		* int		meshcode = 594141;
		* sys_num	kei;
		* crd_cnv	crd;
		* crd.Sys_Number(meshcode, &kei);
		* printf("�Y����=%d: �n1=%d �n2=%d\n", kei.total, kei.sys[0], kei.sys[1]);
		* @endcode
		*/
		void		Sys_Number	(
							int		meshcode,		///< [in]  ���b�V���R�[�h
							sys_num *sys			///< [out]  �n�ԍ�
						);

		/**
		* @brief �o�ܓx����P�X���W�n�̂w�x�֕ϊ�
		* @code
		* <��>
		* int		axis = 10;
		* double	lon = 140.0, lat = 40;
		* xy_coord*	ret_xyc = NULL;
		* crd_cnv	crd;
		* ret_xyc = crd.LLtoXY19(axis, lon, lat);
		* printf("�n=%d: (X,Y)=(%0.10f,%0.10f)(m)\n", axis, ret_xyc->x, ret_xyc->y);
		* @endcode
		* @return �\���� xy_coord �ւ̃|�C���^
		*/
		xy_coord*	LLtoXY19	(
							int		axis,	///< [in]  �Y��������W�n�̔ԍ�
							double	lon,	///< [in]  �o�x
							double	lat		///< [in]  �ܓx
						);

		/**
		* @brief �o�ܓx����ł��߂��P�X���W�n�̌n�Ƃw�x��Ԃ�
		* @code
		* <��>
		* double	lon = 140.0, lat = 40;
		* coord19*	ret_c19 = NULL;
		* crd_cnv	crd;
		* ret_c19 = crd.LLtoXY19(lon, lat);
		* printf("�n=%d: (X,Y)=(%0.10f,%0.10f)(m)\n", ret_c19->sys, ret_c19->x, ret_c19->y);
		* @endcode
		* @return �\���� coord19 �ւ̃|�C���^
		*/
		coord19*	LLtoXY19 (
							double  lon,	///< [in]  �o�x
							double  lat		///< [in]  �ܓx
						);	
		/**
		* @brief �o�ܓx����P�X���W�n�̂w�x�֕ϊ�
		* @code
		* <��>
		* int		axis = 10;
		* double	lon = 140.0, lat = 40;
		* xy_coord	ret_xyc;
		* crd_cnv	crd;
		* crd.LLtoXY19(axis, lon, lat, &ret_xyc);
		* printf("�n=%d: (X,Y)=(%0.10f,%0.10f)(m)\n", axis, ret_xyc.x, ret_xyc.y);
		* @endcode
		*/
		void		LLtoXY19	(
							int			axis,       ///< [in]  �Y��������W�n�̔ԍ�
							double		lon,        ///< [in]  �o�x
							double		lat,        ///< [in]  �ܓx
							xy_coord	*coord		///< [out]  19���W�n��XY
						);

		/**
		* @brief �o�ܓx����ł��߂��P�X���W�n�̌n�Ƃw�x��Ԃ�
		* 
		* 1998.7.14
		* @code
		* <��>
		* double	lon = 140.0, lat = 40;
		* coord19	ret_c19;
		* crd_cnv	crd;
		* crd.LLtoXY19(lon, lat, &ret_c19);
		* printf("�n=%d: (X,Y)=(%0.10f,%0.10f)(m)\n", ret_c19.sys, ret_c19.x, ret_c19.y);
		* @endcode
		*/
		void		LLtoXY19 (
							double		lon,		///< [in]  �o�x
							double		lat,		///< [in]  �ܓx
							coord19		*sys_xy		///< [out]  19���W�n�̌n��XY��\��
						);

		/**
		* @brief �o�ܓx����P�X���W�n�̂w�x�֕ϊ�
		* 
		* 2000.1.12
		* @code
		* <��>
		* int		axis = 10;
		* double	lon = 140.0, lat = 40;
		* double	x = 0.0, y = 0.0;	// 19���W�n��XY
		* crd_cnv	crd;
		* crd.LLtoXY19(axis, lon, lat, &x, &y);
		* printf("�n=%d: (X,Y)=(%0.10f,%0.10f)(m)\n", axis, x, y);
		* @endcode
		*/
		void		LLtoXY19	(	
							int			axis,		///< [in]  �Y��������W�n�̔ԍ�
							double		lon,		///< [in]  �o�x
							double		lat,		///< [in]  �ܓx
							double		*x,			///< [out]  19���W�n��X(�ܓx����)
							double		*y			///< [out]  19���W�n��Y(�o�x����)
						);

		/**
		* @brief �o�ܓx����P�X���W�n�̂w�x�֕ϊ��i���E���n�n�Łj
		* 
		* 2000.1.12
		* @code
		* <��>
		* int		axis = 10;
		* double	lon = 140.0, lat = 40;
		* double	x = 0.0, y = 0.0;	// 19���W�n��XY
		* crd_cnv	crd;
		* crd.LLtoXY19_JGD2000(axis, lon, lat, &x, &y);
		* printf("�n=%d: (X,Y)=(%0.10f,%0.10f)(m)\n", axis, x, y);
		* @endcode
		*/
		void		LLtoXY19_JGD2000	(	
							int			axis,		///< [in]  �Y��������W�n�̔ԍ�
							double		lon,		///< [in]  �o�x
							double		lat,		///< [in]  �ܓx
							double		*x,			///< [out]  19���W�n��X(�ܓx����)
							double		*y			///< [out]  19���W�n��Y(�o�x����)
						);

		/**
		* @brief �P�X���W�n�̂w�x����o�ܓx�֕ϊ�
		* @code
		* <��>
		* int		axis = 10;
		* double	x = 10000.0, y = 5000.0;	// 19���W�n��XY
		* d_lonlat*	ll;
		* crd_cnv	crd;
		* ll = crd.XY19toLL(axis, x, y);
		* printf("lon=%0.10f, lat=%0.10f\n", ll->lon, ll->lat);
		* @endcode
		* @return	�\���� d_lonlat �ւ̃|�C���^
		*/
		d_lonlat*	XY19toLL	(	
							int		axis,		///< [in]  �Y��������W�n�̔ԍ�
							double	x,			///< [in]  19���W�n��X(�ܓx����)
							double	y			///< [in]  19���W�n��Y(�o�x����)
						);
		
		/**
		* @brief �P�X���W�n�̂w�x����o�ܓx�֕ϊ�
		* @code
		* <��>
		* int		axis = 10;
		* double	x = 10000.0, y = 5000.0;	// 19���W�n��XY
		* d_lonlat	ll;
		* crd_cnv	crd;
		* crd.XY19toLL(axis, x, y, &ll);
		* printf("lon=%0.10f, lat=%0.10f\n", ll.lon, ll.lat);
		* @endcode
		*/
		void		XY19toLL	(
							int			axis,		///< [in]  �Y��������W�n�̔ԍ�
							double		x,			///< [in]  19���W�n��X(�ܓx����)
							double		y,			///< [in]  19���W�n��Y(�o�x����)
							d_lonlat	*lonlat		///< [out]  10�i�ŕ\�����o�x�ܓx(ex:130.23432342....)
						);
		
		/**
		* @brief �P�X���W�n�̂w�x����o�ܓx�֕ϊ�
		* @code
		* <��>
		* int		axis = 10;
		* double	x = 10000.0, y = 5000.0;	// 19���W�n��XY
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.XY19toLL(axis, x, y, &lon, &lat);
		* printf("lon=%0.10f, lat=%0.10f\n", lon, lat);
		* @endcode
		*/
		void 		XY19toLL	(
							int			axis,	///< [in]  �Y��������W�n�̔ԍ�
							double		x,		///< [in]  19���W�n��X(�ܓx����)
							double		y,		///< [in]  19���W�n��Y(�o�x����)
							double		*lon,	///< [out]  �o�x
							double		*lat	///< [out]  �ܓx
					);

	/**
		* @brief �P�X���W�n�̂w�x����o�ܓx�֕ϊ��i���E���n�n�Łj
		* @code
		* <��>
		* int		axis = 10;
		* double	x = 10000.0, y = 5000.0;	// 19���W�n��XY
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.XY19toLL_JGD2000(axis, x, y, &lon, &lat);
		* printf("lon=%0.10f, lat=%0.10f\n", lon, lat);
		* @endcode
		*/
		void 		XY19toLL_JGD2000	(
							int			axis,	///< [in]  �Y��������W�n�̔ԍ�
							double		X,		///< [in]  19���W�n��X(�ܓx����)
							double		Y,		///< [in]  19���W�n��Y(�o�x����)
							double		*lon,	///< [out]  �o�x
							double		*lat	///< [out]  �ܓx
					);
		
		/**
		* @brief �P�X���W�n��(X,Y)����A���b�V���R�[�h + (x,y) �����߂�
		* @code
		* <��>
		* int		axis = 10;
		* double	x_19 = 10000.0, y_19 = 5000.0;	// 19���W�n��XY
		* int		meshlv = 64;	// �s�s�n�}���x���ɐݒ�
		* int		mesh = 0;
		* int		x = 0, y = 0;
		* crd_cnv	crd;
		* crd.XY19toMesh(axis, x_19, y_19, meshlv, &mesh, &x, &y);
		* printf("mesh=%d, x=%d, y=%d\n", mesh, x, y);
		* @endcode
		* @retval		0	����
		* @retval		-1	���s
		*/
		int			XY19toMesh	(	
							int		sys,		///< [in]  19���W�n�̌n�ԍ�
							double	x_19,		///< [in]  19���W�n��X(�ܓx����)
							double	y_19,		///< [in]  19���W�n��Y(�o�x����)
							int		level,		///< [in]  ���b�V���̃��x�� (1,2,64)
							int		*meshcode,	///< [out]  ���b�V���R�[�h
							int		*x,			///< [out]  ���K��X
							int		*y			///< [out]  ���K��Y
						);
		
		/**
		* @brief �P�X���W�n�̌n�ԍ��ƃ��b�V���R�[�h + (x,y)����A�P�X���W�n��(X,Y)�����߂�
		* @code
		* <��>
		* int		axis = 10;
		* int		mesh = 594141;
		* int		x = 100000, y = 200000;	// ���b�V���̐��K��XY
		* double	x_19 = 0.0, y_19 = 0.0;	// 19���W�n��XY
		* crd_cnv	crd;
		* crd.MeshtoXY19(axis, mesh, x, y, &x_19, &y_19);
		* printf("�n=%d: (X,Y)=(%0.10f,%0.10f)(m)\n", axis, x_19, y_19);
		* @endcode
		*/
		void		MeshtoXY19	(		
							int		sys,		///< [in]  19���W�n�̌n�ԍ�
							int		meshcode,	///< [in]  ���b�V���R�[�h
							int		x,			///< [in]  ���K��X
							int		y,			///< [in]  ���K��Y
							double	*x_19,		///< [out]  19���W�n��X(�ܓx����)
							double	*y_19		///< [out]  19���W�n��Y(�o�x����)
						);
		
		/**
		* @brief ���o����AUTM���W�̃]�[���ԍ��𓾂�
		* 
		* �����E�l���ǂ����邩��������Ɩ��
		*  2001.08.28
		* @code
		* <��>
		* double	lon = 140.0, lat = 40.0;
		* int		zone = 0;
		* double	ori_lon = 0.0, ori_lat = 0.0;	// �]�[���̌��_�o�ܓx
		* crd_cnv	crd;
		* zone	= crd.GetUTMZone(lon, lat, &ori_lon, &ori_lat);
		* printf("�]�[��=%d: ���_(lon,lat)=(%0.10f,%0.10f)\n", zone, ori_lon, ori_lat);
		* @endcode
		* @return  �]�[���ԍ�: �Ώ۔͈͊O
		* @return  -1 : �Ώ۔͈͊O
		*/
		int			GetUTMZone	(	
							double	c_lon,			///< [in]  ���o?�x
							double	c_lat,			///< [in]  �ܓx(�_�~�[)
							double	*c_orig_lon,	///< [out]  ���_�o�x
							double	*c_orig_lat		///< [out]  ���_�ܓx
						);
		
		/**
		* @brief UTM�̃]�[���ԍ����猴�_�o�ܓx�𓾂�
		* 
		* �����E�l���ǂ����邩��������Ɩ��
		* 2001.08.28
		* @code
		* <��>
		* int		zone = 54;
		* double	ori_lon = 0.0, ori_lat = 0.0;	// �]�[���̌��_�o�ܓx
		* crd_cnv	crd;
		* crd.GetUTMOrigLL(zone, &ori_lon, &ori_lat);
		* printf("�]�[��=%d: ���_(lon,lat)=(%0.10f,%0.10f)\n", zone, ori_lon, ori_lat);
		* @endcode
		* @retval	0	�]�[���ԍ�����������
		* @retval	-1	�]�[���ԍ���������Ȃ�
		*/
		int			GetUTMOrigLL	(
							int		c_zone,			///< [in]  �]�[���ԍ�
							double	*c_orig_lon,	///< [out]  ���_�o�x
							double	*c_orig_lat		///< [out]  ���_�ܓx
						);
		
		/**
		* @brief UTM���W�n�̂w�x����o�ܓx(Bessel)�֕ϊ�
		*
		* 2001.08.28
		* @code
		* <��>
		* int		zone = 54;
		* double	x = 100000, y = 150000;
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.UTMtoLL(zone, x, y, &lon, &lat);
		* printf("(lon,lat)=(%0.10f,%0.10f)\n", lon, lat);
		* @endcode
		* @retval	0	����
		* @retval	-1 	���s
		*/
		int 		UTMtoLL	(
							int		c_zone,		///< [in]  UTM�̃]�[���ԍ�
							double	x,			///< [in]  �k�����̍��W�l(m)
							double	y,			///< [in]  �������̍��W�l(m)
							double	*c_lon,		///< [out]  �o�x(decimal)
							double	*c_lat		///< [out]  �ܓx(decimal)
						);
		int 		UTMtoLL_WGS84	(
							int		c_zone,		///< [in]  UTM�̃]�[���ԍ�
							double	x,			///< [in]  �k�����̍��W�l(m)
							double	y,			///< [in]  �������̍��W�l(m)
							double	*c_lon,		///< [out]  �o�x(decimal)
							double	*c_lat		///< [out]  �ܓx(decimal)
						);
		int 		UTMtoLL_Core	(
							double ra,			///< [in] �n���ȉ~�̂̒����a�i���[�g���j
							double ea2,			///< [in] ��ꗣ�S����2��
							double eb2,			///< [in] ��񗣐S����2��
							int		c_zone,		///< [in]  UTM�̃]�[���ԍ�
							double	x,			///< [in]  �k�����̍��W�l(m)
							double	y,			///< [in]  �������̍��W�l(m)
							double	*c_lon,		///< [out]  �o�x(decimal)
							double	*c_lat		///< [out]  �ܓx(decimal)
						);
		
		/**
		* @brief �o�ܓx(Bessel)����UTM���W�ɂ���
		* @code
		* <��>
		* int		zone = 54;
		* double	lon = 140.0, lat = 40.0;
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoUTM(zone, lon, lat, &x, &y);
		* printf("�]�[��=%d: (X,Y)=(%0.10f,%0.10f)\n", zone, x, y);
		* @endcode
		* @retval	0	����
		* @retval	-1 	���s
		*/
		int			LLtoUTM	(
							int		c_zone,		///< [in]  �]�[���ԍ�
							double	lon,		///< [in]  �o�x
							double	lat,		///< [in]  �ܓx
							double	*c_x,		///< [out]  UTM���W�n��X���W�l(m)
							double	*c_y		///< [out]  UTM���W�n��Y���W�l(m)
						);
		int			LLtoUTM_WGS84	(
							int		c_zone,		///< [in]  �]�[���ԍ�
							double	lon,		///< [in]  �o�x
							double	lat,		///< [in]  �ܓx
							double	*c_x,		///< [out]  UTM���W�n��X���W�l(m)
							double	*c_y		///< [out]  UTM���W�n��Y���W�l(m)
						);
		int			LLtoUTM_Core	(
							double ra,			///< [in] �n���ȉ~�̂̒����a�i���[�g���j
							double ea2,			///< [in] ��ꗣ�S����2��
							double eb2,			///< [in] ��񗣐S����2��
							int		c_zone,		///< [in]  �]�[���ԍ�
							double	lon,		///< [in]  �o�x
							double	lat,		///< [in]  �ܓx
							double	*c_x,		///< [out]  UTM���W�n��X���W�l(m)
							double	*c_y		///< [out]  UTM���W�n��Y���W�l(m)
						);
		/**
		* @brief �o�ܓx�Ƃ�������o�x������X(m),�ܓx������Y(m)���ꂽ�Ƃ���̌o�ܓx�����߂�i����Łj
		* 
		* ���������Ȃ��P�[�X������Ƃ̕񍐂��������̂Ŏ����񐔂̐�����ݒ�(50��)
	�@	*   �v�Z���ň�ԋ߂����ʂɂȂ����o�ܓx��Ԃ��悤�ɕύX�B
		* @code
		* <��>
		* int	digit = 0;	// ����
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* int	count = 0;
		* // ��������100m�A�k������200m�͂Ȃꂽ�Ƃ���̍��W�����߂�
		* count	= crd.GetLLPt_Core(140.0, 39.0, 100.0, 200.0, &lon, &lat);
		* printf("(�o�x,�ܓx,count)=(%0.10f,%0.10f,%d)\n", lon, lat, count);
		* @endcode
		* @return		�����܂ł̉� \n
		* @return		-1 = �����񐔓��Ɏ������Ȃ�
		*/
		int			GetLLPt_Core(
							double		ra,			///< [in]  �n���ȉ~�̂̒����a�im�j
							double		ea2,		///< [in]  ��ꗣ�S����2��
							double		eLon1,		///< [in]  �o�x1
							double		eLat1,		///< [in]  �ܓx1
							double		cDist_X,	///< [in]  ����X(m)
							double		cDist_Y,	///< [in]  ����Y(m)
							int			mode,		///< [in]  �ϊ����[�h(0=���{���n�n,1=���E���n�n)
							double		*eLon2,		///< [out]  �o�x2
							double		*eLat2		///< [out]  �ܓx2
						);

		/**
		* @brief �P�O�i����x�A���A�b�֕ϊ�
		* @code
		* <��>
		* double	lon = 140.50423;// �o�x(10�i)
		* dms		mes_pt;			// �x���b�l
		* crd_cnv	crd;
		* crd.DECtoDMS(lon, &mes_pt);
		* printf("%0.10f=%d�x%d��%0.5f�b\n", lon, mes_pt.deg, mes_pt.min, mes_pt.sec);
		* @endcode
		*/
		void		DECtoDMS	(
							double		lonlat,			///< [in]  �o�ܓx�i10�i�j
							dms			*mes_pt			///< [out]  �ϊ���̑��_�̌o�ܓx�i�x�A���A�b�j
						);
		
		/**
		* @brief �P�O�i����x�A���A�b�֕ϊ�
		* @code
		* <��>
		* double	lon = 140.50423;// �o�x(10�i)
		* dms*		mes_pt = NULL;	// �x���b�l
		* crd_cnv	crd;
		* mes_pt	= crd.DECtoDMS(lon);
		* printf("%0.10f=%d�x%d��%0.5f�b\n", lon, mes_pt->deg, mes_pt->min, mes_pt->sec);
		* @endcode
		* @return	�\���� dms �ւ̃|�C���^
		*/
		dms*		DECtoDMS	(	double	lonlat	);	///< [in]  �o�ܓx�i10�i�j
		
		/**
		* @brief �x�A���A�b����P�O�i�֕ϊ�
		* 
		* 2000.03.25
		* @code
		* <��>
		* dms		mes_pt;		// �x���b�l
		* double	lon = 0.0;	// �o�x(10�i)
		* crd_cnv	crd;
		* mes_pt.deg = 140; mes_pt.min = 30; mes_pt.sec = 15.25;
		* crd.DMStoDEC(mes_pt, &lon);
		* printf("%d�x%d��%0.5f�b=%0.10f\n", mes_pt.deg, mes_pt.min, mes_pt.sec, lon);
		* @endcode
		*/
		void		DMStoDEC	(
							dms		mes_pt,		///< [in]  �o�ܓx�i10�i�j
							double	*lonlat		///< [out]  �ϊ���̑��_�̌o�ܓx�i�x�A���A�b�j
						);
		
		/**
		* @brief �x�A���A�b����P�O�i�֕ϊ�
		* 
		* 2000.03.25
		* @code
		* <��>
		* dms		mes_pt;		// �x���b�l
		* double	lon = 0.0;	// �o�x(10�i)
		* crd_cnv	crd;
		* mes_pt.deg = 140; mes_pt.min = 30; mes_pt.sec = 15.25;
		* lon		= crd.DMStoDEC(mes_pt);
		* printf("%d�x%d��%0.5f�b=%0.10f\n", mes_pt.deg, mes_pt.min, mes_pt.sec, lon);
		* @endcode
		* @return	double�̈ܓx����ьo�x�l
		*/
		double		DMStoDEC	(	dms		mes_pt	);	///< [in]  �o�ܓx�i10�i�j
		
		/**
		* @brief �Q�n�_�̌o�ܓx����A�Q�_�Ԃ̋��������߂�(�P��: m)
		* 
		* ���Fzcl����v�Z���͂قƂ�ǑS�����p�����Ă�����Ă���B(class ZLonLat)
		* @code
		* <��>
		* double	length = 0.0;
		* double	lon1 = 140.0, lat1 = 40.0, lon2 = 141.0, lat2 = 40.0;
		* crd_cnv	crd;
		* length	= crd.GetDist(lon1, lat1, lon2, lat2);
		* printf("����=%0.10f(m)\n", length);
		* @endcode
		* @return		�Q�n�_�Ԃ̋���
		*/
		double		GetDist	(
							double		eLon1,		///< [in]  �o�x1
							double		eLat1,		///< [in]  �ܓx1
							double		eLon2,		///< [in]  �o�x2
							double		eLat2		///< [in]  �ܓx2
						);
		
		/**
		* @brief �Q�n�_�̌o�ܓx����A�Q�_�Ԃ̋��������߂�(�P��: m)
		* 
		* ���Fzcl����v�Z���͂قƂ�ǑS�����p�����Ă�����Ă���B(class ZLonLat)
		* @code
		* <��>
		* double	length = 0.0;
		* d_lonlat	lonlat1, lonlat2;
		* lonlat1.lon = 140.0; lonlat1.lat = 40.0; lonlat2.lon = 141.0; lonlat2.lat = 40.0;
		* crd_cnv	crd;
		* length	= crd.GetDist(lonlat1, lonlat2);
		* printf("����=%0.10f(m)\n", length);
		* @endcode
		* @return		�Q�n�_�Ԃ̋���
		*/
		double		GetDist	(
							d_lonlat	lonlat1,	///< [in]  �o�ܓx1
							d_lonlat	lonlat2		///< [in]  �o�ܓx2
						);

		/**
		* @brief �Q�n�_�̌o�ܓx����A�Q�_�Ԃ̋��������߂�(�P��: m)�i���E���n�n�Łj
		* 
		* ���Fzcl����v�Z���͂قƂ�ǑS�����p�����Ă�����Ă���B(class ZLonLat)
		* @code
		* <��>
		* double	length = 0.0;
		* double	lon1 = 140.0, lat1 = 40.0, lon2 = 141.0, lat2 = 40.0;
		* crd_cnv	crd;
		* length	= crd.GetDist_JGD2000(lon1, lat1, lon2, lat2);
		* printf("����=%0.10f(m)\n", length);
		* @endcode
		* @return		�Q�n�_�Ԃ̋���
		*/
		double		GetDist_JGD2000	(
							double		eLon1,		///< [in]  �o�x1
							double		eLat1,		///< [in]  �ܓx1
							double		eLon2,		///< [in]  �o�x2
							double		eLat2		///< [in]  �ܓx2
						);
		
		/**
		* @brief �Q�n�_�̃��b�V��+(x,y)����A�Q�_�Ԃ̋��������߂�i�P��: m) 
		* 
		* 2001.07.23
		* @code
		* <��>
		* double	length = 0.0;
		* crd_cnv	crd;
		* length	= crd.GetMeshDist(594141, 10000, 50000, 594142, 1000000, 200000);
		* printf("����=%0.10f(m)\n", length);
		* @endcode
		* @return		�Q�n�_�Ԃ̋���
		*/
		double		GetMeshDist(
							int			c_mesh1,	///< [in]  ���b�V���R�[�h1
							int			c_x1,		///< [in]  X���W1
							int			c_y1,		///< [in]  Y���W1
							int			c_mesh2,	///< [in]  ���b�V���R�[�h2
							int			c_x2,		///< [in]  X���W2
							int			c_y2		///< [in]  Y���W2
						);
		
		/**
		* @brief �Q�n�_�̌o�ܓx����A�Q�_�Ԃ̋��� + ����(X,Y)m�����߂�
		* 
		* ���Fzcl����v�Z���͂قƂ�ǑS�����p�����Ă�����Ă���B(class ZLonLat)
		* @code
		* <��>
		* double	length = 0.0;
		* double	xdist = 0.0, ydist = 0.0; // X,Y�����̋���
		* crd_cnv	crd;
		* length	= crd.GetDistXY(140.0, 40.0, 141.0, 41.0, &xdist, &ydist);
		* printf("����=%0.10f(m) ����(X)=%0.10f(m) ����(Y)=%0.10f(m)\n", length, xdist, ydist);
		* @endcode
		* @return		�Q�n�_�Ԃ̋���
		*/
		double		GetDistXY	(
							double		eLon1,		///< [in]  �o�x1
							double		eLat1,		///< [in]  �ܓx1
							double		eLon2,		///< [in]  �o�x2
							double		eLat2,		///< [in]  �ܓx2
							double		*cDist_X,	///< [out]  ����X(m)
							double		*cDist_Y	///< [out]  ����Y(m)
						);

		/**
		* @brief �Q�n�_�̌o�ܓx����A�Q�_�Ԃ̋��� + ����(X,Y)m�����߂�i���E���n�n�Łj
		* 
		* ���Fzcl����v�Z���͂قƂ�ǑS�����p�����Ă�����Ă���B(class ZLonLat)
		* @code
		* <��>
		* double	length = 0.0;
		* double	lon1 = 0.0, lat1 = 0.0, lon2 = 0.0, lat2 = 0.0;
		* crd_cnv	crd;
		* length	= crd.GetDistXY_JGD2000(lon1, lat1, lon2, lat2, &xdist, &ydist);
		* printf("����=%0.10f(m) ����(X)=%0.10f(m) ����(Y)=%0.10f(m)\n", length, xdist, ydist);
		* @endcode
		* @return		�Q�n�_�Ԃ̋���
		*/
		double		GetDistXY_JGD2000	(
							double		eLon1,		///< [in]  �o�x1
							double		eLat1,		///< [in]  �ܓx1
							double		eLon2,		///< [in]  �o�x2
							double		eLat2,		///< [in]  �ܓx2
							double		*cDist_X,	///< [out]  ����X(m)
							double		*cDist_Y	///< [out]  ����Y(m)
						);

		/**
		* @brief ���b�V��+(x,y) ��2�_�̐��K���|�C���g�ł̍��������߂�
		*
		* �����b�V���̌�������������̂Ƃ��̂�
		* @code
		* <��>
		* double	xdiff = 0.0, ydiff = 0.0; // X,Y�����̋���
		* crd_cnv	crd;
		* crd.GetNrmDiff(594141, 10000, 50000, 594142, 1000000, 200000, &xdiff, &ydiff);
		* printf("����X=%0.10f, ����Y=%0.10f\n", xdiff, ydiff);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		int			GetNrmDiff(
							int			c_mesh1,	///< [in]  ���b�V���R�[�h
							int			c_x1,		///< [in]  ���K�����WX
							int			c_y1,		///< [in]  ���K�����WY
							int			c_mesh2,	///< [in]  ���b�V���R�[�h
							int			c_x2,		///< [in]  ���K�����WX
							int			c_y2,		///< [in]  ���K�����WY
							double		*c_dx,		///< [out]  ���K�����W����X
							double		*c_dy		///< [out]  ���K�����W����Y
						);
		
		/**
		* @brief ���b�V��+(x,y) ��2�_�̐��K���|�C���g�ł̍��������߂�
		* @code
		* <��>
		* double	xdiff = 0.0, ydiff = 0.0; // X,Y�����̋���
		* crd_cnv	crd;
		* crd.GetNrmDiff2(594141, 10000, 50000, 594142, 1000000, 200000, &xdiff, &ydiff);
		* printf("����X=%0.10f, ����Y=%0.10f\n", xdiff, ydiff);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		int			GetNrmDiff2(
							int			c_mesh1,	///< [in]  ���b�V���R�[�h
							int			c_x1,		///< [in]  ���K�����WX
							int			c_y1,		///< [in]  ���K�����WY
							int			c_mesh2,	///< [in]  ���b�V���R�[�h
							int			c_x2,		///< [in]  ���K�����WX
							int			c_y2,		///< [in]  ���K�����WY
							double		*c_dx,		///< [out]  ���K�����W����X
							double		*c_dy		///< [out]  ���K�����W����Y
						);
		
		/**
		* @brief ���b�V�� + (X,Y)����o�ܓx�֕ϊ�
		* @code
		* <��>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.MeshtoLL(594141, 10000, 20000, &lon, &lat);
		* printf("(�o�x,�ܓx)=(%0.10f, %0.10f)\n", lon, lat);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		int			MeshtoLL	(
							int		meshcode,		///< [in]  ���b�V���R�[�h
							int		x,				///< [in]  ���K�����WX
							int		y,				///< [in]  ���K�����WY
							double	*lon,			///< [out]  �o�x
							double	*lat			///< [out]  �ܓx
						);

		/**
		* @brief ���b�V�� + (X,Y)����o�ܓx�֕ϊ�(double)
		* @code
		* <��>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.MeshtoLL2(594141, 10000.25, 20000.125, &lon, &lat);
		* printf("(�o�x,�ܓx)=(%0.10f, %0.10f)\n", lon, lat);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		int			MeshtoLL2	(
							int		meshcode,		///< [in]  ���b�V���R�[�h
							double	x,				///< [in]  ���K�����WX
							double	y,				///< [in]  ���K�����WY
							double	*lon,			///< [out]  �o�x
							double	*lat			///< [out]  �ܓx
						);
		
		/**
		* @brief ���b�V�� + (X,Y)����1/256�b�P�ʂ̌o�ܓx�֕ϊ�
		* @code
		* <��>
		* int		lon256 = 0, lat256 = 0;
		* crd_cnv	crd;
		* crd.MeshtoLL256(604000, 0, 0, &lon256, &lat256);
		* printf("1/256�b(�o�x,�ܓx)=(%d, %d)\n", lon256, lat256);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		int			MeshtoLL256	(
							int		meshcode,	///< [in]  ���b�V���R�[�h
							int		x,			///< [in]  ���K�����WX
							int		y,			///< [in]  ���K�����WY
							int		*lon256,	///< [out]  �o�x
							int		*lat256		///< [out]  �ܓx
						);
		
		/**
		* @brief �P�O�i�����̌o�ܓx����1/256�b�P�ʂ̌o�ܓx�֕ϊ�
		* 
		* �o�x�A�ܓx�̓��͂�int�ɂȂ��Ă����̂��C�� 2008.01.21
		* @code
		* <��>
		* int		lon256 = 0, lat256 = 0;
		* crd_cnv	crd;
		* crd.LLtoLL256(140.0, 40.0, &lon256, &lat256);
		* printf("1/256�b(�o�x,�ܓx)=(%d, %d)\n", lon256, lat256);
		* @endcode
		*/
		void		LLtoLL256	(
							double	lon,		///< [in]  �o�x
							double	lat,		///< [in]  �ܓx
							int		*lon256,	///< [out]  �o�x(1/256�P��)
							int		*lat256		///< [out]  �ܓx(1/256�P��)
						);
		
		/**
		* @brief 1/256�b�P�ʂ̌o�ܓx����A�P�O�i�����̌o�ܓx�֕ϊ�
		* @code
		* <��>
		* double	lon = 0, lat = 0;
		* crd_cnv	crd;
		* crd.LL256toLL(129024000, 36864000, &lon, &lat);
		* printf("(�o�x,�ܓx)=(%0.10f, %0.10f)\n", lon, lat);
		* @endcode
		*/
		void		LL256toLL	(
							int		lon256,		///< [in]  �o�x
							int		lat256,		///< [in]  �ܓx
							double	*lon,		///< [out]  �o�x
							double	*lat		///< [out]  �ܓx
						);
		
		/**
		* @brief ���b�V���̊p����p�̋��������߂�(m)
		* @code
		* <��>
		* double	udist = 0.0, ddist = 0.0, ldist = 0.0, rdist = 0.0;
		* crd_cnv	crd;
		* crd.CtoCDist(594141, &udist, &ddist, &ldist, &rdist);
		* printf("���=%0.5f(m), ����=%0.5f(m)\n", udist, ddist);
		* printf("����=%0.5f(m), �E��=%0.5f(m)\n", ldist, rdist);
		* @endcode
		*/
		void		CtoCDist	(
							int		meshcode,	///< [in]  ���b�V���R�[�h
							double	*udist,		///< [out]  ���b�V���̏�Ӌ���
							double	*ddist,		///< [out]  ���b�V���̉��Ӌ���
							double	*ldist,		///< [out]  ���b�V���̍��Ӌ���
							double	*rdist		///< [out]  ���b�V���̉E�Ӌ���
						);
		
		/**
		* @brief ���_���b�V������(x,y)���b�V�����ꂽ���̃��b�V���R�[�h�����߂�
		* 
		* 2001.3.20 �}�C�i�X�����ł̃o�O���C��
		* @code
		* <��>
		* int		meshcode = 0;	// ���߂郁�b�V���R�[�h
		* crd_cnv	crd;
		* meshcode = crd.GetMeshCode(594141, 1, 10);
		* printf("���b�V�� %d ��X����=%d,Y����=%d �ׂ̃��b�V���� %d\n", 594141, 1, 10, meshcode );
		* @endcode
		* @return		�����F���b�V���R�[�h
		* @return		���s�F -1
		*/
		int			GetMeshCode(
							int		meshcode,	///< [in]  ���b�V���R�[�h
							int		xdist,		///< [in]  ���_���b�V������X�����։����b�V������Ă��邩
							int		ydist		///< [in]  ���_���b�V������Y�����։����b�V������Ă��邩
						);

		/**
		* @brief ���_���b�V������(x,y)���b�V�����ꂽ���̃��b�V���R�[�h�����߂�i�V���ۓs�s�n�}�Łj
		* 
		* 2001.3.20 �}�C�i�X�����ł̃o�O���C��
		* @code
		* <��>
		* int		meshcode = 0;	// ���߂郁�b�V���R�[�h
		* crd_cnv	crd;
		* meshcode = crd.GetMeshCode(594141, 1, 10);
		* printf("���b�V�� %d ��X����=%d,Y����=%d �ׂ̃��b�V���� %d\n", 594141, 1, 10, meshcode );
		* @endcode
		* @return		�����F���b�V���R�[�h
		* @return		���s�F -1
		*/
		int			GetMeshCode_SJ(
							unsigned int	meshcode,	///< [in]  ���b�V���R�[�h
							int				xdist,		///< [in]  ���_���b�V������X�����։����b�V������Ă��邩
							int				ydist		///< [in]  ���_���b�V������Y�����։����b�V������Ă��邩
						);
		
		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�
		* @code
		* <��>
		* int		lv = 2;	// 2�����b�V��
		* int		mesh = 0, x = 0, y = 0;
		* crd_cnv	crd;
		* // 594141,1000000,1000000 �̂悤�ɂȂ��Ă����e����ꍇ
		* crd.LLtoMesh(141.250, 39.750, lv, &mesh, &x, &y, 0);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* // 594141,1000000,1000000 �͋��e���Ȃ��ꍇ��594152,0,0�ƕԂ������ꍇ
		* crd.LLtoMesh(141.250, 39.750, lv, &mesh, &x, &y, 1);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		int			LLtoMesh	(
							double		lon,		///< [in]  �o�x
							double		lat,		///< [in]  �ܓx
							int			level,		///< [in]  ���b�V���̃��x�� (1,2,64)
							int			*meshcode,	///< [out]  ���b�V���R�[�h
							int			*x,			///< [out]  ���K��X
							int			*y,			///< [out]  ���K��Y
							int			no_mill=0	///< [in]  XY���W��1000000�ɂȂ邱�Ƃ����e���邩�i0=���e,1=���e���Ȃ�=�ׂ̃��b�V����0�Ƃ��ĕԂ��j 
						);
		
		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
		* 
		* �w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
		* @code
		* <��>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoMesh2(141.250, 39.750, 594141, &x, &y);
		* printf("(mesh,X,Y)=(%d,%0.5f,%0.5f)\n", 594141, x, y);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		int			LLtoMesh2	(
							double		c_lon,			///< [in]  �o�x
							double		c_lat,			///< [in]  �ܓx
							int			c_meshcode,		///< [in]  ���b�V���R�[�h
							double		*c_x,			///< [out]  ���K��X
							double		*c_y			///< [out]  ���K��Y
						);
		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
		* 
		* �w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
		* @code
		* <��>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoMesh2(141.250, 39.750, 594141, &x, &y);
		* printf("(mesh,X,Y)=(%d,%0.10f,%0.10f)\n", 594141, x, y);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		int			LLtoMesh2	(
							double		c_lon,			///< [in]  �o�x
							double		c_lat,			///< [in]  �ܓx
							int			c_meshcode,		///< [in]  ���b�V���R�[�h
							int			*c_x,			///< [out]  ���K��X
							int			*c_y			///< [out]  ���K��Y
						);
		
		/**
		@brief �������b�V����(x,y)����A�~�����b�V����(x,y)�ɕϊ�
		* @code
		* <��>
		* int	mesh = 0, x = 0, y = 0;
		* crd_cnv	crd;
		* // 2�����b�V����s�s�n�}���b�V���ɕϊ�
		* crd.MeshtoMesh(594141, 0, 0, 64, &mesh, &x, &y);
		* printf("�ϊ���(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		int			MeshtoMesh	(
							int		s_mesh,		///< [in]  �����b�V���R�[�h
							int		s_x,		///< [in]  ���K��X
							int		s_y,		///< [in]  ���K��Y
							int		level,		///< [in]  ���b�V���̎��� (1,2,64)
							int		*d_mesh,	///< [out]  �ϊ��チ�b�V���R�[�h
							int		*d_x,		///< [out]  ���K��X
							int		*d_y		///< [out]  ���K��Y
						);
		
		/**
		* @brief �������b�V����(x,y)����A�~�����b�V����(x,y)�ɕϊ�
		*
		* �����ł̋��ߕ���MeshtoMesh�Ƃ͈Ⴄ�B���Â�ǂ��炩�Ɍ��߂�\��B 2000.10.06
		* @code
		* <��>
		* int	mesh = 0, x = 0, y = 0;
		* crd_cnv	crd;
		* // 1�����b�V����2�����b�V���ɕϊ�
		* crd.MeshtoMesh2(5940, 0, 0, 2, &mesh, &x, &y);
		* printf("�ϊ���(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		int			MeshtoMesh2	(
							int		s_mesh,		///< [in]  �����b�V���R�[�h
							int		s_x,		///< [in]  ���K��X
							int		s_y,		///< [in]  ���K��Y
							int		level,		///< [in]  ���b�V���̎��� (1,2,64)
							int		*d_mesh,	///< [out]  �ϊ��チ�b�V���R�[�h
							int		*d_x,		///< [out]  ���K��X
							int		*d_y		///< [out]  ���K��Y
						);
		
		/**
		* @brief ���b�V���R�[�h�̌�������
		* 
		* 2000.4.6
		* @code
		* <��>
		* int	digit = 0;	// ����
		* crd_cnv	crd;
		* digit	= crd.GetMeshDigit(594141);
		* printf("���b�V������=%d\n", digit);
		* @endcode
		* @return		���� = �R�[�h�̌��� 2,4,6,8
		* @return		���s = -1
		*/
		int			GetMeshDigit	(	unsigned int	s_mesh	);		///< [in]  �����b�V���R�[�h

		/**
		* @brief �o�ܓx�Ƃ�������o�x������X(m),�ܓx������Y(m)���ꂽ�Ƃ���̌o�ܓx�����߂�
		* 
		* ���������Ȃ��P�[�X������Ƃ̕񍐂��������̂Ŏ����񐔂̐�����ݒ�(50��)
	�@	*   �v�Z���ň�ԋ߂����ʂɂȂ����o�ܓx��Ԃ��悤�ɕύX�B
		* @code
		* <��>
		* int	digit = 0;	// ����
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* int	count = 0;
		* // ��������100m�A�k������200m�͂Ȃꂽ�Ƃ���̍��W�����߂�
		* count	= crd.GetLLPt(140.0, 39.0, 100.0, 200.0, &lon, &lat);
		* printf("(�o�x,�ܓx,count)=(%0.10f,%0.10f,%d)\n", lon, lat, count);
		* @endcode
		* @return		�����܂ł̉� \n
		* @return		-1 = �����񐔓��Ɏ������Ȃ�
		* 2000.4.13
		*/
		int			GetLLPt(
							double		eLon1,		///< [in]  �o�x1
							double		eLat1,		///< [in]  �ܓx1
							double		cDist_X,	///< [in]  ����X(m)
							double		cDist_Y,	///< [in]  ����Y(m)
							double		*eLon2,		///< [out]  �o�x2
							double		*eLat2		///< [out]  �ܓx2
						);

		/**
		* @brief �o�ܓx�Ƃ�������o�x������X(m),�ܓx������Y(m)���ꂽ�Ƃ���̌o�ܓx�����߂�i���E���n�n�Łj
		* 
		* ���������Ȃ��P�[�X������Ƃ̕񍐂��������̂Ŏ����񐔂̐�����ݒ�(50��)
	�@	*   �v�Z���ň�ԋ߂����ʂɂȂ����o�ܓx��Ԃ��悤�ɕύX�B
		* @code
		* <��>
		* int	digit = 0;	// ����
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* int	count = 0;
		* // ��������100m�A�k������200m�͂Ȃꂽ�Ƃ���̍��W�����߂�
		* count	= crd.GetLLPt(140.0, 39.0, 100.0, 200.0, &lon, &lat);
		* printf("(�o�x,�ܓx,count)=(%0.10f,%0.10f,%d)\n", lon, lat, count);
		* @endcode
		* @return		�����܂ł̉� \n
		* @return		-1 = �����񐔓��Ɏ������Ȃ�
		*/
		int			GetLLPt_JGD2000(
							double		eLon1,		///< [in]  �o�x1
							double		eLat1,		///< [in]  �ܓx1
							double		cDist_X,	///< [in]  ����X(m)
							double		cDist_Y,	///< [in]  ����Y(m)
							double		*eLon2,		///< [out]  �o�x2
							double		*eLat2		///< [out]  �ܓx2
						);
		
		/**
		* @brief �o�ܓx����K�E�X�E�N�����[�Q�����W�n��X,Y(m)�֕ϊ�
		* 
		* 2000.9.19
		* @code
		* <��>
		* xy_coord*	ret_xyc = NULL;
		* crd_cnv	crd;
		* ret_xyc	= crd.LLtoXYGs(3, 8.5, 50.0);
		* printf("�]�[��=%d: �ʒu(X,Y)=(%0.10f,%0.10f)\n", 2, ret_xyc->x, ret_xyc->y);
		* @endcode
		* @return	�\���� xy_coord �ւ̃|�C���^
		*/
		xy_coord*	LLtoXYGs	(
							int			axis,	///< [in]  �Y������]�[���̔ԍ�(2�`4)
							double		lon,	///< [in]  �o�x
							double		lat		///< [in]  �ܓx
						);
		
		/**
		* @brief �o�ܓx����K�E�X�E�N�����[�Q�����W�n��X,Y(m)�֕ϊ�
		* 
		* 2000.9.19
		* @code
		* <��>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoXYGs(3, 8.5, 50.0, &x, &y);
		* printf("�]�[��=%d: �ʒu(X,Y)=(%0.10f,%0.10f)\n", 2, x, y);
		* @endcode
		*/
		void		LLtoXYGs	(
							int			axis,	///< [in]  �Y������]�[���̔ԍ�(2�`4)
							double		lon,	///< [in]  �o�x
							double		lat,	///< [in]  �ܓx
							double		*x,		///< [out]  �K�E�X�E�N�����[�Q�����W�n��X
							double		*y		///< [out]  �K�E�X�E�N�����[�Q�����W�n��Y
						);
		
		/**
		* @brief �K�E�X�E�N�����[�Q�����W�n��X,Y(m)����o�ܓx�֕ϊ�
		* 
		* 2000.9.20
		* @code
		* <��>
		* d_lonlat	ll;
		* crd_cnv	crd;
		* crd.XYGstoLL(3, 5540399.35009, 3464156.57117, &ll);
		* printf("(�o�x,�ܓx)=(%0.10f,%0.10f)\n", ll.lon, ll.lat);
		* @endcode
		*/
		void 		XYGstoLL	(
							int			axis,		///< [in]  �Y������]�[���̔ԍ�(2�`4)
							double		x,			///< [in]  �k�����̍��W�l(m)
							double		y,			///< [in]  �������̍��W�l(m)
							d_lonlat	*lonlat		///< [out]  10�i�ŕ\�����o�ܓx(ex:130.23432342....)
						);
		
		/**
		* @brief �K�E�X�E�N�����[�Q�����W�n��X,Y(m)����o�ܓx�֕ϊ�
		* 
		* 2000.9.20
		* @code
		* <��>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.XYGstoLL(3, 5540399.35009, 3464156.57117, &lon, &lat);
		* printf("(�o�x,�ܓx)=(%0.10f,%0.10f)\n", lon, lat);
		* @endcode
		*/
		void		XYGstoLL	(
							int			axis,	///< [in]  �Y������]�[���̔ԍ�(2�`4)
							double		X,		///< [in]  �k�����̍��W�l(m)
							double		Y,		///< [in]  �������̍��W�l(m)
							double		*lon,	///< [out]  �o�x
							double		*lat	///< [out]  �ܓx
						);
		/**
		* @brief �x�b�Z���ȉ~�̂̈ܓx�o�x���AWGS84�̈ܓx�o�x�����߂�
		* 
		* 2000.10.06
		* 
		* ���l���s���m�ł��邽�߁A�m�F�̏ケ�̃��\�b�h�ɂ��Ă͍폜����\��
		*�i��������?�j
		*/
		/* 2009.06.09 �R�����g�A�E�g
		void		BslLLtoWGS84LL	(
							double		bsl_lon,		///< �x�b�Z���o�x
							double		bsl_lat,		///< �x�b�Z���ܓx
							double		bsl_hei,		///< �x�b�Z���ȉ~�̍�
							double		*wgs84_lon,		///< WGS84�o�x
							double		*wgs84_lat,		///< WGS84�ܓx
							double		*wgs84_hei		///< WGS84�ȉ~�̍�
						);
		*/

		/**
		* �x�b�Z���ȉ~�̂̈ܓx�o�x���AWGS84�̈ܓx�o�x�����߂�
		* WGS84�̈ܓx�o�x���x�b�Z���ȉ~�̂̈ܓx�o�x�֕ϊ�	�i��������?�j
		* // 2000.10.10
		*/
		/* 2009.05.21 �����Ȃ̂ŃR�����g�A�E�g
		void		WGS84LLtoBslLL	(
							double		*wgs84_lon,		///< WGS84�o�x
							double		*wgs84_lat,		///< WGS84�ܓx
							double		bsl_lon,		///< �x�b�Z���o�x
							double		bsl_lat			///< �x�b�Z���ܓx
						);
						*/
		/**
		* @brief ���b�V�� + (X,Y)����o�ܓx�֕ϊ�(EU������)
		*
		* 2001.2.28
		* @code
		* <��>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.MeshtoLL_EU(60700000, 0, 0, &lon, &lat);
		* printf("(�o�x,�ܓx)=(%0.10f, %0.10f)\n", lon, lat);
		* @endcode
		* @retval	0	����
		* @retval	-1	���s
		*/
		int			MeshtoLL_EU(
							unsigned int	meshcode,	///< [in]  ���b�V���R�[�h
							int				x,			///< [in]  ���K�����WX
							int				y,			///< [in]  ���K�����WY
							double			*lon,		///< [out]  �o�x
							double			*lat		///< [out]  �ܓx
						);
		
		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�
		* 
		* 2002.2.28
		* @code
		* <��>
		* int		lv = 64;	// �s�s�n�}���b�V��
		* int		mesh = 0, x = 0, y = 0;
		* crd_cnv	crd;
		* // xxxxxxxx,1000000,1000000 �̂悤�ɂȂ��Ă����e����ꍇ
		* crd.LLtoMesh_EU(40.0, 50.0, lv, &mesh, &x, &y, 0);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* // xxxxxxxx,1000000,1000000 �͋��e���Ȃ��ꍇ��xxxxxxxx,0,0�ƕԂ������ꍇ
		* crd.LLtoMesh_EU(40.0, 50.0, lv, &mesh, &x, &y, 1);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* @endcode
		* @retval	0	����
		* @retval	-1	���s
		*/
		int			LLtoMesh_EU(
							double			lon,		///< [in]  �o�x
							double			lat,		///< [in]  �ܓx
							int				level,		///< [in]  ���b�V���̃��x�� (1,2,64)
							unsigned int	*meshcode,	///< [out]  ���b�V���R�[�h
							int				*x,			///< [out]  ���K��X
							int				*y,			///< [out]  ���K��Y
							int				no_mill=0	///< [in]  XY���W��1000000�ɂȂ邱�Ƃ����e���邩�i0=���e,1=���e���Ȃ�=�ׂ̃��b�V����0�Ƃ��ĕԂ��j2006.09.06 
						);

		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
		* �w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
		* @code
		* <��>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoMesh_EU2(40.0, 50.0, 60700000, &x, &y);
		* printf("(mesh,X,Y)=(%d,%0.5f,%0.5f)\n", 60700000, x, y);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		int			LLtoMesh_EU2	(
							double			c_lon,			///< [in]  �o�x
							double			c_lat,			///< [in]  �ܓx
							unsigned int	c_meshcode,		///< [in]  ���b�V���R�[�h
							double			*c_x,			///< [out]  ���K��X
							double			*c_y			///< [out]  ���K��Y
						);
		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
		* �w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
		* @code
		* <��>
		* int		x = 0, y = 0;
		* crd_cnv	crd;
		* crd.LLtoMesh_EU2(40.0, 50.0, 60700000, &x, &y);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", 60700000, x, y);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		int			LLtoMesh_EU2	(
							double			c_lon,			///< [in]  �o�x
							double			c_lat,			///< [in]  �ܓx
							unsigned int	c_meshcode,		///< [in]  ���b�V���R�[�h
							int				*c_x,			///< [out]  ���K��X
							int				*c_y			///< [out]  ���K��Y
						);
		
		
		/**
		* @brief ���b�V�� + (X,Y)����o�ܓx�֕ϊ�(NA������) 
		* 
		* 2002.2.28
		* @code
		* <��>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.MeshtoLL_NA(75200000, 0, 0, &lon, &lat);
		* printf("(�o�x,�ܓx)=(%0.10f, %0.10f)\n", lon, lat);
		* @endcode
		* @retval	0	����
		* @retval	-1	���s
		*/
		int			MeshtoLL_NA(
							unsigned int	meshcode,	///< [in]  ���b�V���R�[�h
							int			x,			///< [in]  ���K�����WX
							int			y,			///< [in]  ���K�����WY
							double		*lon,		///< [out]  �o�x
							double		*lat		///< [out]  �ܓx
						);
		
		/**
		* @brief �k�A�����J�Ή��� 
		* 
		* �o�ܓx����A���b�V���R�[�h + (x,y) �����߂� 2001.2.28
		* @code
		* <��>
		* int		lv = 64;	// �s�s�n�}���b�V��
		* int		mesh = 0, x = 0, y = 0;
		* crd_cnv	crd;
		* // xxxxxxxx,1000000,1000000 �̂悤�ɂȂ��Ă����e����ꍇ
		* crd.LLtoMesh_NA(-130.0, 60.0, lv, &mesh, &x, &y, 0);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* // xxxxxxxx,1000000,1000000 �͋��e���Ȃ��ꍇ��xxxxxxxx,0,0�ƕԂ������ꍇ
		* crd.LLtoMesh_NA(-130.0, 60.0, lv, &mesh, &x, &y, 1);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* @endcode
		* @retval	0	����
		* @retval	-1	���s
		*/
		int			LLtoMesh_NA(
							double			lon,		///< [in]  �o�x
							double			lat,		///< [in]  �ܓx
							int				level,		///< [in]  ���b�V���̃��x�� (1,2,64)
							unsigned int	*meshcode,	///< [out]  ���b�V���R�[�h
							int				*x,			///< [out]  ���K��X
							int				*y,			///< [out]  ���K��Y
							int				no_mill=0	///< [in]  XY���W��1000000�ɂȂ邱�Ƃ����e���邩�i0=���e,1=���e���Ȃ�=�ׂ̃��b�V����0�Ƃ��ĕԂ��j2006.09.06 
						);

		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
		* 
		* �w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
		* @code
		* <��>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoMesh_NA2(-130.0, 60.0, 75200000, &x, &y);
		* printf("(mesh,X,Y)=(%d,%0.5f,%0.5f)\n", 75200000, x, y);
		* @endcode
		* @retval	0	����
		* @retval	-1	���s
		*/
		int			LLtoMesh_NA2(
							double			c_lon,			///< [in]  �o�x
							double			c_lat,			///< [in]  �ܓx
							unsigned int	c_meshcode,		///< [in]  ���b�V���R�[�h
							double			*c_x,			///< [out]  ���K��X
							double			*c_y			///< [out]  ���K��Y
						);
		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
		* 
		* �w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
		* @code
		* <��>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoMesh_NA2(-130.0, 60.0, 75200000, &x, &y);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", 75200000, x, y);
		* @endcode
		* @retval	0	����
		* @retval	-1	���s
		*/
		int			LLtoMesh_NA2(
							double			c_lon,			///< [in]  �o�x
							double			c_lat,			///< [in]  �ܓx
							unsigned int	c_meshcode,		///< [in]  ���b�V���R�[�h
							int				*c_x,			///< [out]  ���K��X
							int				*c_y			///< [out]  ���K��Y
						);

		/**
		* @brief ���b�V�� + (X,Y)����o�ܓx�֕ϊ�(�A���X�J�E�n���C��) 
		* 
		* 2005.12.22
		* @code
		* <��>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.MeshtoLL_AH(75200000, 0, 0, &lon, &lat);
		* printf("(�o�x,�ܓx)=(%0.10f, %0.10f)\n", lon, lat);
		* @endcode
		* @retval	0	����
		* @retval	-1	���s
		*/
        int			MeshtoLL_AH(
							unsigned int	meshcode,		///< [in]  ���b�V���R�[�h
							int				x,				///< [in]  ���K�����WX
							int				y,				///< [in]  ���K�����WY
							double			*lon,			///< [out]  �o�x
							double			*lat			///< [out]  �ܓx
						);

        /**
        * @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(�A���X�J�E�n���C��) 
		* 
		* 2005.12.22
		* @code
		* <��>
		* int		lv = 64;	// �s�s�n�}���b�V��
		* int		mesh = 0, x = 0, y = 0;
		* crd_cnv	crd;
		* // xxxxxxxx,1000000,1000000 �̂悤�ɂȂ��Ă����e����ꍇ
		* crd.LLtoMesh_AH(-160.0, 60.0, lv, &mesh, &x, &y, 0);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* // xxxxxxxx,1000000,1000000 �͋��e���Ȃ��ꍇ��xxxxxxxx,0,0�ƕԂ������ꍇ
		* crd.LLtoMesh_AH(-160.0, 60.0, lv, &mesh, &x, &y, 1);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* @endcode
		* @retval 0   ����
        * @retval -1  ���s
        */
        int			LLtoMesh_AH(
							double			lon,			///< [in]  �o�x
							double      	lat,			///< [in]  �ܓx
							int      	  	level,			///< [in]  ���b�V���̃��x�� (1,2,64)
							unsigned int	*meshcode,		///< [out]  ���b�V���R�[�h
							int         	*x,				///< [out]  ���K��X
							int         	*y,				///< [out]  ���K��Y
							int				no_mill=0		///< [in]  XY���W��1000000�ɂȂ邱�Ƃ����e���邩�i0=���e,1=���e���Ȃ�=�ׂ̃��b�V����0�Ƃ��ĕԂ��j2006.09.06 
                        );

		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
		* 
		* �w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
		* @code
		* <��>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoMesh_AH2(-160.0, 60.0, 75200000, &x, &y);
		* printf("(mesh,X,Y)=(%d,%0.5f,%0.5f)\n", 75200000, x, y);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		int			LLtoMesh_AH2(
							double			c_lon,			///< [in]  �o�x
							double			c_lat,			///< [in]  �ܓx
							unsigned int	c_meshcode,		///< [in]  ���b�V���R�[�h
							double			*c_x,			///< [out]  ���K��X
							double			*c_y			///< [out]  ���K��Y
						);
		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)
		*
		* �w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
		* @code
		* <��>
		* double	x = 0.0, y = 0.0;
		* crd_cnv	crd;
		* crd.LLtoMesh_AH2(-160.0, 60.0, 75200000, &x, &y);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", 75200000, x, y);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		int			LLtoMesh_AH2(
							double			c_lon,			///< [in]  �o�x
							double			c_lat,			///< [in]  �ܓx
							unsigned int	c_meshcode,		///< [in]  ���b�V���R�[�h
							int				*c_x,			///< [out]  ���K��X
							int				*c_y			///< [out]  ���K��Y
						);

		//=============================================================================
		// �������牺�͐V�K�ǉ����\�b�h 2006,09.06
		//=============================================================================

		//---------------------------------------------------------------------
		/**
		* @brief ���b�V���R�[�h�̌�������(������p) 
		* 
		* 2006.9.7
		* @code
		* <��>
		* int	digit = 0;	// ����
		* crd_cnv	crd;
		* digit	= crd.GetMeshDigit("59414130");
		* printf("���b�V������=%d\n", digit);
		* @endcode
		* @return		���� = �R�[�h�̌��� 2,4,6,8
		* @return		���s = -1
		*/
		//---------------------------------------------------------------------
		int			GetMeshDigit	(
							char*	s_mesh			///< [in]  �����b�V���R�[�h
						);
		

		//---------------------------------------------------------------------
		/**
		* @brief ���b�V�� + (X,Y)����o�ܓx�֕ϊ�(���E���ʔ�(���{��)) 
		*
		* ���b�V���̃��x���w��ɂ��ẮA���E�̃��b�V���ł͐擪��0�ɂȂ�ꍇ������̂�
		* �ǂ̃��x���̃��b�V���Ƃ��Ĉ������𖾎����邽�߂ɑ��݂���B2006.09.06
		* ��j���b�V�� 5941 ���x�� 64 �� 00005941�̂悤�Ɉ���
		* @code
		* <��>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* // ��A�����J�p���b�V���Ƃ��Ĉ���
		* crd.MeshtoLL_World(69600000, 0, 0, 64, crd_cnv::SA_ORIGIN, &lon, &lat);
		* printf("��A�����J�p(�o�x,�ܓx)=(%0.10f, %0.10f)\n", lon, lat);
		* @endcode
		* @retval	0	����
		* @retval	-1	���s
		*/
		//---------------------------------------------------------------------
		int			MeshtoLL_World(
							unsigned int		c_meshcode,		///< [in]  ���b�V���R�[�h
							int					c_x,			///< [in]  ���K�����WX
							int					c_y,			///< [in]  ���K�����WY
							int					c_level,		///< [in]  ���b�V���̃��x�� (1,2,64)
							int					c_origin_code,	///< [in]  ���_�R�[�h
							double				*c_lon,			///< [out]  �o�x
							double				*c_lat			///< [out]  �ܓx
						);

		//---------------------------------------------------------------------
		/**
        * @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���E���ʔ�(���{��))
		* 
		* 2006.09.08
		* @code
		* <��>
		* int		lv = 64;	// �s�s�n�}���b�V��
		* int		mesh = 0, x = 0, y = 0;
		* crd_cnv	crd;
		* // xxxxxx,1000000,1000000 �̂悤�ɂȂ��Ă����e����ꍇ(��A�����J���b�V���Ƃ��Ĉ���)
		* crd.LLtoMesh_World(-60.0, -10.0, lv, crd_cnv::SA_ORIGIN, &mesh, &x, &y, 0);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* // xxxxxx,1000000,1000000 �͋��e���Ȃ��ꍇ��xxxxxx,0,0�̂悤�ɕԂ������ꍇ
		* crd.LLtoMesh_World(-60.0, -10.0, lv, crd_cnv::SA_ORIGIN, &mesh, &x, &y, 1);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", mesh, x, y);
		* @endcode
        * @retval 0   ����
        * @retval -1  ���s
		*/
		//---------------------------------------------------------------------
        int			LLtoMesh_World(
							double				c_lon,			///< [in]  �o�x
							double				c_lat,			///< [in]  �ܓx
							int					c_level,		///< [in]  ���b�V���̃��x�� (1,2,64)
							int					c_origin_code,	///< [in]  ���_�R�[�h
							unsigned int		*c_meshcode,	///< [out]  ���b�V���R�[�h
							int					*c_x,			///< [out]  ���K��X
							int					*c_y,			///< [out]  ���K��Y
							int					c_no_mill=0		///< [in]  XY���W��1000000�ɂȂ邱�Ƃ����e���邩�i0=���e,1=���e���Ȃ�=�ׂ̃��b�V����0�Ƃ��ĕԂ��j2006.09.06 
                        );
		
		//---------------------------------------------------------------------
		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)(���E���ʔ�(���{��)) 
		*
		* �w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
		* 
		* ���b�V���̃��x���w��ɂ��ẮA���E�̃��b�V���ł͐擪��0�ɂȂ�ꍇ������̂�
		* �ǂ̃��x���̃��b�V���Ƃ��Ĉ������𖾎����邽�߂ɑ��݂���B
		* ��j���b�V�� 333 ���x�� 64 �� 00003333�̂悤�Ɉ���
		* 2006.09.06
		* @code
		* <��>
		* int		lv = 64;	// �s�s�n�}���b�V��
		* double	x = 0, y = 0;
		* crd_cnv	crd;
		* // xxxxxx,1000000,1000000 �̂悤�ɂȂ��Ă����e����ꍇ(��A�����J���b�V���Ƃ��Ĉ���)
		* crd.LLtoMesh2_World(-60.0, -10.0, 69600000, 64, crd_cnv::SA_ORIGIN, &x, &y);
		* printf("(mesh,X,Y)=(%d,%0.10f,%0.10f)\n", 69600000, x, y);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		//---------------------------------------------------------------------
		int			LLtoMesh2_World(
							double				c_lon,			///< [in]  �o�x
							double				c_lat,			///< [in]  �ܓx
							unsigned int		c_meshcode,		///< [in]  ���b�V���R�[�h
							int					c_level,		///< [in]  ���b�V���̃��x�� (1,2,64)
							int					c_origin_code,	///< [in]  ���_�R�[�h
							double				*c_x,			///< [out]  ���K��X
							double				*c_y			///< [out]  ���K��Y
						);

		//---------------------------------------------------------------------
		/**
		* @brief �o�ܓx����A���b�V���R�[�h + (x,y) �����߂�(���b�V���w���)(���E���ʔ�(���{��)) 
		*
		* �w�肵�����b�V���R�[�h�ɂ������āA���K�����W�l�����߂�
		* 
		* ���b�V���̃��x���w��ɂ��ẮA���E�̃��b�V���ł͐擪��0�ɂȂ�ꍇ������̂�
		* �ǂ̃��x���̃��b�V���Ƃ��Ĉ������𖾎����邽�߂ɑ��݂���B
		* ��j���b�V�� 333 ���x�� 64 �� 00003333�̂悤�Ɉ���
		* 2006.09.12
		* @code
		* <��>
		* int		lv = 2;	// 2�����b�V��
		* int		x = 0, y = 0;
		* crd_cnv	crd;
		* // xxxxxx,1000000,1000000 �̂悤�ɂȂ��Ă����e����ꍇ(��A�����J���b�V���Ƃ��Ĉ���)
		* crd.LLtoMesh2_World(-60.0, -10.0, 69600000, 64, crd_cnv::SA_ORIGIN, &x, &y);
		* printf("(mesh,X,Y)=(%d,%d,%d)\n", 69600000, x, y);
		* @endcode
		* @retval	0	����
		* @retval -1	���s
		*/
		//---------------------------------------------------------------------
		int			LLtoMesh2_World(
							double				c_lon,			///< [in]  �o�x
							double				c_lat,			///< [in]  �ܓx
							unsigned int		c_meshcode,		///< [in]  ���b�V���R�[�h
							int					c_level,		///< [in]  ���b�V���̃��x�� (1,2,64)
							int					c_origin_code,	///< [in]  ���_�R�[�h
							int					*c_x,			///< [out]  ���K��X
							int					*c_y			///< [out]  ���K��Y
						);

		//=============================================================================
		// �����܂� 2006,09.06
		//=============================================================================

		//=======================================================================
		//	���L2�̃����o�֐��ATOKYOtoJGD2000(), JGD2000toTOKYO()	�́A
		//  [Bug 5755] �̗v�]�ɂ��ǉ� 2008.01.21
		//	���y�⑫�����z
		//	����2�̕ϊ��֐��́A��c ���v�������A�u���E���n�n�ƍ��W�ϊ��v
		//  �ŉ������Ă����@�����ɍ쐬����Ă��܂��B
		//=======================================================================
		//---------------------------------------------------------------------
		/**
		* @brief Tokyo->JGD2000�ւ̍��W�ϊ�
		* @code
		* <��>
		* double	lon = 0.0, lat = 0.0, he = 0.0;
		* crd_cnv	crd;
		* crd.TOKYOtoJGD2000(140.0, 40.0, 0.0, &lon, &lat, &he);
		* printf("���E���n�n�o�ܓx(lon,lat,he)=(%0.10f,%0.10f,%0.10f)\n", lon, lat, he);
		* @endcode
		* @return ����
		*/
		//---------------------------------------------------------------------
		void		TOKYOtoJGD2000	(
							double	c_dBefore_Lon,	///< [in]  �ϊ��O�o�x
							double	c_dBefore_Lat,	///< [in]  �ϊ��O�ܓx
							double	c_dBefore_He,	///< [in]  �ϊ��O�ȉ~�̍�(m)
							double*	c_dAfter_Lon,	///< [out]  �ϊ���o�x
							double*	c_dAfter_Lat,	///< [out]  �ϊ����ܓx
							double*	c_dAfter_He		///< [out]  �ϊ���ȉ~�̍�(m)
						);

		//---------------------------------------------------------------------
		/**
		* @brief JGD2000->Tokyo�ւ̍��W�ϊ�
		* @code
		* <��>
		* double	lon = 0.0, lat = 0.0, he = 0.0;
		* crd_cnv	crd;
		* crd.JGD2000toTOKYO(140.0, 40.0, 0.0, &lon, &lat, &he);
		* printf("�������n�n�o�ܓx(lon,lat,he)=(%0.10f,%0.10f,%0.10f)\n", lon, lat, he);
		* @endcode
		* @return ����
		*/
		//---------------------------------------------------------------------
		void		JGD2000toTOKYO	(
							double	c_dBefore_Lon,	///< [in]  �ϊ��O�o�x
							double	c_dBefore_Lat,	///< [in]  �ϊ��O�ܓx
							double	c_dBefore_He,	///< [in]  �ϊ��O�ȉ~�̍�(m)
							double*	c_dAfter_Lon,	///< [out]  �ϊ���o�x
							double*	c_dAfter_Lat,	///< [out]  �ϊ����ܓx
							double*	c_dAfter_He		///< [out]  �ϊ���ȉ~�̍�(m)
						);

		//=======================================================================
		//	���L2�̃����o�֐��ATOKYOtoJGD2000_RP(), JGD2000toTOKYO_RP()
		//  �́A�n�斈�p�����[�^�ł̕ϊ��AIPC�ŗL�ł̕ϊ��Ή��ӏ������邽�ߒǉ��B
		//	���y�⑫�����z
		//	����2�̕ϊ��֐��́A��c ���v�������A�u���E���n�n�ƍ��W�ϊ��v
		//  �ŉ������Ă����@�����ɍ쐬����Ă��܂��B
		//=======================================================================
		//---------------------------------------------------------------------
		/**
		* @brief �n�斈�p�����[�^���g�p����Tokyo->JGD2000�ւ̍��W�ϊ�
		* 
		* �g�p�O��JGD2000_RP_Init�Œn��p�����[�^��ǂݍ��ޕK�v������
		* @code
		* <��>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.JGD2000_RP_Init("TKY2JGD.par");	// �n��p�����[�^�ǂݍ���
		* // IPC�̕����I�ɓ������n�n��Tokyo97�ɑΉ������f�[�^�̍��W�l�ϊ����s���ꍇ
		* crd.TOKYOtoJGD2000_RP(124.25, 24.5, &lon, &lat, true);
		* printf("���E���n�n�o�ܓx(lon,lat)=(%0.10f,%0.10f)\n", lon, lat);
		* // ��ʂɏo����Ă���f�[�^(�������n�n�j�̍��W�l�ϊ����s���ꍇ
		* crd.TOKYOtoJGD2000_RP(124.25, 24.5, &lon, &lat, false);
		* printf("���E���n�n�o�ܓx(lon,lat)=(%0.10f,%0.10f)\n", lon, lat);
		* @endcode
		* @return	0 = �ϊ��G���[�i���������Ŏg�p�����ꍇ���j
		* @return	1 = �n�斈�̃p�����[�^�ŕϊ����ꂽ�ꍇ3�p�����[�^�ŕϊ����ꂽ�ꍇ
		* @return	2 = 3�p�����[�^�ŕϊ����ꂽ�ꍇ
		*/
		//---------------------------------------------------------------------
		int			TOKYOtoJGD2000_RP	(
							double	c_dBefore_Lon,	///< [in]  �ϊ��O�o�x
							double	c_dBefore_Lat,	///< [in]  �ϊ��O�ܓx
							double*	c_dAfter_Lon,	///< [out]  �ϊ���o�x
							double*	c_dAfter_Lat,	///< [out]  �ϊ����ܓx
							bool	c_bIPC_Flag		///< [in]  IPC�ŗL�ϊ������邩�ǂ���true=IPC�ŗL�ϊ� false=���Ԉ�ʑΉ��ϊ�
						);

		//---------------------------------------------------------------------
		/**
		* @brief �n�斈�p�����[�^���g�p����JGD2000->Tokyo�ւ̍��W�ϊ�
		* 
		* �g�p�O��JGD2000_RP_Init�Œn��p�����[�^��ǂݍ��ޕK�v������
		* @code
		* <��>
		* double	lon = 0.0, lat = 0.0;
		* crd_cnv	crd;
		* crd.JGD2000_RP_Init("TKY2JGD.par");	// �n��p�����[�^�ǂݍ���
		* // IPC�̕����I�ɓ������n�n��Tokyo97�ɑΉ������f�[�^�̍��W�l�ϊ����s���ꍇ
		* crd.JGD2000toTOKYO_RP(124.25040, 24.50547, &lon, &lat, true);
		* printf("�������n�n�o�ܓx(lon,lat)=(%0.10f,%0.10f)\n", lon, lat);
		* // ��ʂɏo����Ă���f�[�^(�������n�n�j�̍��W�l�ϊ����s���ꍇ
		* crd.JGD2000toTOKYO_RP(124.25040, 24.50547, &lon, &lat, false);
		* printf("�������n�n�o�ܓx(lon,lat)=(%0.10f,%0.10f)\n", lon, lat);
		* @endcode
		* @return	0 = �ϊ��G���[�i���������Ŏg�p�����ꍇ���j
		* @return	1 = �n�斈�̃p�����[�^�ŕϊ����ꂽ�ꍇ3�p�����[�^�ŕϊ����ꂽ�ꍇ
		* @return	2 = 3�p�����[�^�ŕϊ����ꂽ�ꍇ
		*/
		//---------------------------------------------------------------------
		int			JGD2000toTOKYO_RP	(
							double	c_dBefore_Lon,	///< [in]  �ϊ��O�o�x
							double	c_dBefore_Lat,	///< [in]  �ϊ��O�ܓx
							double*	c_dAfter_Lon,	///< [out]  �ϊ���o�x
							double*	c_dAfter_Lat,	///< [out]  �ϊ����ܓx
							bool	c_bIPC_Flag		///< [in]  IPC�ŗL�ϊ������邩�ǂ���true=IPC�ŗL�ϊ� false=���Ԉ�ʑΉ��ϊ�
						);

		//d_lonlat*	GetLLPt		(double, double, double, double);				// 2000.4.13
		//void		GetLLPt		(double, double, double, double, d_lonlat *);	// 2000.4.13
		//void		LLtoXYGs	(int, double, double, xy_coord *);			///< �o�ܓx����K�E�X���W�n��XY�֕ϊ�2000.9.19
		// 2001.08.29 �Ƃ肠�����e�X�g�p
		//int			WGS84UTMtoLL	(int, double, double, double *, double *);	// 2001.08.28 UTM���W����A�o�ܓx(WGS84)�ɂ��� 
		//int			LLtoWGS84UTM	(int, double, double, double *, double *);	// 2001.08.29 UTM���W����A�o�ܓx(WGS84)�ɂ��� 
};


//	WINDOWS�pdefine 1998.7.28 by �H��
#ifndef _USE_MATH_DEFINES // @see math.h
#ifndef M_PI
#ifndef PI
#define		PI			3.14159265358979323846
#endif
#define	M_PI	PI
#endif
#endif

#if defined(__cplusplus)
}
#endif

#endif	// __CRD_CNV_HEADDER__
