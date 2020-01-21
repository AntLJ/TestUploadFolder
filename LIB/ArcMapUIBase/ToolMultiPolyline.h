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

#include "ToolPointer.h"

class CToolMultiPolyline : public CToolPolyline
{
public:
   /**
	* @brief  �R���X�g���N�^
	*/
	CToolMultiPolyline(){}
	CToolMultiPolyline( IApplication* ipApp, bool bWithArea=false ) : CToolPolyline(ipApp,bWithArea) {}

   /**
	* @brief  �f�X�g���N�^
	*/
	virtual ~CToolMultiPolyline() { init(); }


	HRESULT OnMouseDown(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled);
	HRESULT OnMouseUp(LONG Button, LONG Shift, IPoint* ipPoint, bool& bHandled);
	HRESULT OnDblClick(bool& bHandled);
	HRESULT OnKeyUp(LONG keyCode, LONG Shift, bool& bHandled);

   /**
	* @brief  �t�B�[�h�o�b�N�̃��X�^�[�g
	* @param[in] ipFeedbackGeom ���X�^�[�g����t�B�[�h�o�b�N�`��
	* @retval true   ���X�^�[�g����
	* @retval false  ���X�^�[�g���s
	*/
	bool Restart( IGeometry* ipFeedbackGeom );

   /**
	* @brief  �t�B�[�h�o�b�N�`����}���`�p�[�g�̃|�����C���ŕԂ�
	* @return �t�B�[�h�o�b�N�`��
	*/
	IGeometryPtr GetShape() const;

   /**
	* @brief  ������
	*/
	void init();

   /**
	* @brief  �t�B�[�h�o�b�N�`�󏉊���
	*/
	void clear();

   /**
	* @brief  �t�B�[�h�o�b�N�`����V���O���p�[�g�̃W�I���g���Q�ŕԂ�
	* @return �t�B�[�h�o�b�N�`��
	*/
	std::vector<CAdapt<IGeometryPtr>> GetShapeAsSingleParts() const { return m_FeedbackGeomList; };
	 

private:
	typedef std::vector<CAdapt<IGeometryPtr>>::const_iterator CIter;

	std::vector<CAdapt<IGeometryPtr>> m_FeedbackGeomList;	//!< �t�B�[�h�o�b�N�`��Q

};


