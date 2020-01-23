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

/**
 * @file MoveFeedback.h
 * @brief CMoveFeedback �N���X��`�t�@�C��
 * @author �n�}DB���암�V�X�e���J���O���[�v �Ð� �M�G
 * @version $Id$
 */
#ifndef _MOVEFEEDBACK_H_
#define _MOVEFEEDBACK_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"
#include "Row.h"
#include "EditDef.h"

namespace sindy {

class SINDYLIB_API CMoveFeedback  
{
	struct NEWLINEFEEDBACK
	{
		INewLineFeedbackPtr Feedback;
		double X;
		double Y;
	};
	struct MOVEPOINTFEEDBACK
	{
		IMovePointFeedbackPtr Feedback;
		double X;
		double Y;
	};
public:
	CMoveFeedback();
	virtual ~CMoveFeedback();

public:
	/**
	 * @brief <b>�L�k�֎~�ȃe�[�u���^�C�v��ݒ肷��</b>\n
	 * Stop�ŃN���A����܂��B
	 * @param emCode	[in]	�L�k���֎~�������e�[�u���^�C�v
	 */
	void SetCantElasticType( sindyTableType::ECode emCode );
	std::pair<double,double> Stop( double* pdScale = NULL, double* pdAngle = NULL );
	void SetDisplay( IScreenDisplay* ipDisp ){ m_ipDisp = ipDisp; }

	/**
	 * @brief <b>�t�B�[�h�o�b�N���X�^�[�g����</b>\n
	 * �ړ����t�B�[�`���̒�����A�N�G���`��Ɉ�����������̂ɂ���
	 * �t�B�[�h�o�b�N�𔭐������܂��B�ǂ̂悤�Ƀt�B�[�h�o�b�N�𔭐������邩��
	 * ���Ă�emMode�ɂ���ĈقȂ�܂��B�ڂ����́ACFeatures::Move���Q�Ƃ���
	 * ���������B\n
	 * �G���[�ɂȂ�Ȃ��Ă��A�ړ��Ώۂ�������Ȃ��ꍇ�t�B�[�h�o�b�N���J�n����܂���B
	 * ���̂Ƃ���bFeedbackStarted��false�ɂȂ�̂ŁA�`�F�b�N���Ă��������B\n
	 * ���̊֐����ĂԑO�ɁASegDisplay�ŃX�N���[���f�B�X�v���C���Z�b�g���Ă����K�v������܂��B
	 * @param cGeom				[in]	�N�G���`��
	 * @param ipStartPoint		[in]	�t�B�[�h�o�b�N�J�n�_�iMouseUp�̈ʒu�j
	 * @param listFeatures		[in]	�ړ����t�B�[�`��
	 * @param emMove			[in]	�ړ����[�h
	 * @param bFeedbackStarted	[out]	�t�B�[�h�o�b�N���X�^�[�g������true
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode Start( const CGeometry& cGeom, IPoint* ipStartPoint, const CContainer& listFeatures, sindyeMoveMode emMode, bool& bFeedbackStarted );

	void MoveTo( IPoint* Point );
	void Refresh( OLE_HANDLE hDC );
	void Rotate( double dAngle );
	void Scale( double dScale );

	/**
	 * @brief <b>��]�E�g��E�k�����ł��邩�ǂ���</b>\n
	 * @return true : ��]�E�g��E�k�����邱�Ƃ��ł���
	 */
	bool CanTransform();
	void GetMovePoint(double* pX, double* pY);

	inline sindyeMoveMode GetMoveMode() const { return m_emMoveMode; }
private:
	typedef std::list<NEWLINEFEEDBACK>::iterator segment_feedback_iterator;
	typedef std::list<MOVEPOINTFEEDBACK>::iterator point_feedback_iterator;
	segment_feedback_iterator segment_feedback_begin(){ return m_listSegmentFeedback.begin(); }
	segment_feedback_iterator segment_feedback_end(){ return m_listSegmentFeedback.end(); }
	point_feedback_iterator point_feedback_begin(){ return m_listPointFeedback.begin(); }
	point_feedback_iterator point_feedback_end(){ return m_listPointFeedback.end(); }
	void segment_feedback_push_back( IPoint* StartPoint );
	void point_feedback_push_back( IPoint* StartPoint );
	void SetBasePoint( IPoint* Point ){	Point->QueryCoords( &m_dBaseX, &m_dBaseY );	}

	IPointPtr GetCenterPoint( const CGeometry& cGeom );

private:
#pragma warning(push)
#pragma warning(disable:4251)
	IScreenDisplayPtr					m_ipDisp;				//!< �t�B�[�h�o�b�N�\���ɕK�v
	IMoveLineFeedbackPtr				m_ipMoveLineFeedback;	//!< ���s�ړ������t�B�[�h�o�b�N�p
	std::list<MOVEPOINTFEEDBACK>		m_listPointFeedback;	//!< �|�C���g�I�u�W�F�N�g�t�B�[�h�o�b�N�p
	std::list<NEWLINEFEEDBACK>			m_listSegmentFeedback;	//!< �ړ������Ɣ�ړ������̋��ڃt�B�[�h�o�b�N�p
	double								m_dBaseX, m_dBaseY;		//!< �ړ��J�n�x�[�X�|�C���g
	double								m_dMoveX, m_dMoveY;		//!< �ړ����ێ��p
	std::list<sindyTableType::ECode>	m_listCantElastic;		//!< �L�k�֎~�^�C�v�ێ��p
	sindyeMoveMode						m_emMoveMode;			//!< �ړ����[�h
	double								m_dAngle;				//!< ��]�p�x
	double								m_dScale;				//!< �g�嗦
#pragma warning(pop)
};

} // sindy

#endif // _MOVEFEEDBACK_H_
