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

#ifndef	_POINT_ADMIN_H
#define	_POINT_ADMIN_H

// 2002.03.20 by kudo
// 2003.04.25 �����Ǘ��p�̃o�b�t�@���w��T�C�Y�ȍ~�͐؂�̂Ă�AlimitBuf()��ǉ�
#include <vector>
#include <set>

/**
	�_�Ɠ_�̋����ƈʒu���Ǘ�����N���X
*/
class Pt_Dist_Admin_Array
{
	private:
		struct	Pt_Dist_Admin
		{
			public:
				LONG	eIndx_1;	///< �_��P�̃C���f�b�N�X
				LONG	eIndx_2;	///< �_��Q�̃C���f�b�N�X
				double	eDist;		///< 2�_�Ԃ̋���
			public:
						Pt_Dist_Admin	()
						{
							eIndx_1	= 0;
							eIndx_2 = 0;
							eDist	= 0.0;
						}
						Pt_Dist_Admin	(	LONG	cIndx_1,	///< �C���f�b�N�X�P
											LONG	cIndx_2,	///< �C���f�b�N�X�Q
											double	cDist)		///< ����
						{
							eIndx_1	= cIndx_1;
							eIndx_2	= cIndx_2;
							eDist	= cDist;
						}
						~Pt_Dist_Admin	() {}
				void	operator=(const Pt_Dist_Admin cData)	///< ������Z�q��`
				{
					eIndx_1	= cData.eIndx_1;
					eIndx_2	= cData.eIndx_2;
					eDist	= cData.eDist;
				}
			};

		struct lessC : public std::binary_function<Pt_Dist_Admin, Pt_Dist_Admin, bool>
		{
			// ���������̔�r�ł悢
			bool operator () (const Pt_Dist_Admin cData1, const Pt_Dist_Admin cData2) const
			{
				bool	ret	= false;
				if(cData1.eDist != cData2.eDist) {
					if(cData1.eDist < cData2.eDist) {
						ret	= true;
					}
				}
				return (ret);
			}
		};
		std::multiset<Pt_Dist_Admin, lessC>				m_PtDistTbl;
		std::multiset<Pt_Dist_Admin, lessC>::iterator	m_CurIndx;
	public:
		Pt_Dist_Admin_Array	()	///< �R���X�g���N�^
		{
			m_PtDistTbl.erase(m_PtDistTbl.begin(),m_PtDistTbl.end());
			m_CurIndx	= m_PtDistTbl.begin();
		}
		~Pt_Dist_Admin_Array() {}	///< �f�X�g���N�^

		/**
			�f�[�^��ǉ�����
		*/
		void	pushData	(	LONG	cIndx1,	///< �C���f�b�N�X�P[IN]
								LONG	cIndx2,	///< �C���f�b�N�X�Q[IN]
								double	cDist)	///< ����[IN]
		{
			Pt_Dist_Admin		aTmpDat(cIndx1, cIndx2, cDist);
			m_PtDistTbl.insert	(aTmpDat);
		}

		/**
			�����Ǘ��o�b�t�@���ŏ��̈ʒu����w��T�C�Y�܂�
			�ɐ؂�l�߂�
			@return ����
		*/
		void	limitBuf	(	LONG	cSize	)	///< ���~�b�g�T�C�Y
		{
			std::multiset<Pt_Dist_Admin, lessC>				a_TmpPtDistTbl;
			std::multiset<Pt_Dist_Admin, lessC>::iterator	a_Indx;
			// �w��T�C�Y�����݂̃o�b�t�@�T�C�Y���傫���΂����͉������Ȃ�
			int		a_iCur_Size		= m_PtDistTbl.size();
			if( a_iCur_Size > cSize ) {
				int	i = 0;
				for( a_Indx = m_PtDistTbl.begin(), i = 0; i < cSize; a_Indx++, i++ )
				{
					a_TmpPtDistTbl.insert	( *a_Indx );
				}
				// ��x����
				m_PtDistTbl.clear();
				m_PtDistTbl	= a_TmpPtDistTbl;	// �S���R�s�[
			}
		}

		/**
			�ŏ��̃f�[�^�����o��
			@retval	true	���o����
			@retval false	���o���Ȃ�
		*/
		int		getBegin	(	LONG	*cIndx1,	///< �C���f�b�N�X�P[OUT]
								LONG	*cIndx2,	///< �C���f�b�N�X�Q[OUT]
								double	*cDist)		///< ����[OUT]
		{
			m_CurIndx		= m_PtDistTbl.begin();
			if(m_CurIndx == m_PtDistTbl.end()) {	// �󂾂����ꍇ
				*cIndx1		= 0;
				*cIndx2		= 0;
				*cDist		= 0.0;
				return	(false);	
			}
			Pt_Dist_Admin	aTmpDat;
			aTmpDat			= *m_CurIndx;
			*cIndx1			= aTmpDat.eIndx_1;
			*cIndx2			= aTmpDat.eIndx_2;
			*cDist			= aTmpDat.eDist;
			return	(true);
		}

		/**
			���̃f�[�^�����o��
			@retval	true	���o����
			@retval false	���o���Ȃ�
		*/
		int		getNext		(	LONG	*cIndx1,	///< �C���f�b�N�X�P[OUT]
								LONG	*cIndx2,	///< �C���f�b�N�X�Q[OUT]
								double	*cDist)		///< ����[OUT]
		{
			m_CurIndx++;	// �C�e���[�^���P�i�߂�
			if(m_CurIndx == m_PtDistTbl.end()) {	// �󂾂����ꍇ
				*cIndx1		= 0;
				*cIndx2		= 0;
				*cDist		= 0.0;
				return	(false);	
			}
			Pt_Dist_Admin	aTmpDat;
			aTmpDat			= *m_CurIndx;
			*cIndx1			= aTmpDat.eIndx_1;
			*cIndx2			= aTmpDat.eIndx_2;
			*cDist			= aTmpDat.eDist;
			return	(true);
		}
};

#endif	//	_POINT_ADMIN_H
