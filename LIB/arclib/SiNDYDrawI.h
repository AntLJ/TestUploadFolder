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

#ifndef __SINDY_DRAW_I_H__
#define __SINDY_DRAW_I_H__

class SiNDYDrawI
{
public:
// ��`
	/**
	 * @brief �r���[�|�[�g�_��`��N���X
	 */
	class ViewportPolyline
	{
	public:
	// �\�z/����
		ViewportPolyline();
		~ViewportPolyline();

	// �ݒ�/�擾
		void clear();

		void setPolyline(IFeature* ipFeature, double dScaleForGeoToViewport);
		void setPolyline(IPointCollection* ipPointCol, double dScaleForGeoToViewport);

		LPPOINT polyline() const { return m_pPoints; }
		unsigned int count() const { return m_nCount; }
		const RECT& rect() const { return m_Rect; }

	private:
	// �\�z/����
		ViewportPolyline(const ViewportPolyline&) {}

	// �ϐ�
		LPPOINT m_pPoints;
		unsigned int m_nCount;
		RECT m_Rect;
	};

	/// �}�E�X����ɂ���b�@�\
	enum EMouseMode {
		kNoneMode, ///< �@�\�Ȃ�
		kPanMode, ///< �ړ�
		kZoomInMode, ///< ���o�[�o���h�ɂ��g��
		kZoomOutMode, ///< ���o�[�o���h�ɂ��k��
		kPanZoomMode, ///< �}�E�X�̏㉺���ɂ��g�k
	};

// �\�z
	SiNDYDrawI();

// �ݒ�/�擾
	void setLMouseMode(EMouseMode eMouseMode) { m_eLMouseMode = eMouseMode; }
	void setRMouseMode(EMouseMode eMouseMode) { m_eRMouseMode = eMouseMode; }

	EMouseMode getLMouseMode() const { return m_eLMouseMode; }
	EMouseMode getRMouseMode() const { return m_eRMouseMode; }

// �ÓI�֐�
	static BOOL GeoToViewport(IPoint* ipGeoPoint, double dScaleForGeoToViewport, LPPOINT lpViewPoint);
	static BOOL GeoToViewport(double dX, double dY, double dScaleForGeoToViewport, LPPOINT lpViewPoint);

protected:
// �ϐ�
	double m_dCenterX; ///< ���S�o�x
	double m_dCenterY; ///< ���S�ܓx
	double m_dScale; ///< �{���iPixel/���j

	bool m_bDragging; ///< �h���b�O��
	bool m_bRButton; ///< �E�{�^���ɂ��h���b�O
	POINT m_posStart; ///< �h���b�O�J�n�ʒu
	POINT m_posPrev; ///< �O�J�[�\���ʒu
	EMouseMode m_eLMouseMode; ///< �W���̋���L
	EMouseMode m_eRMouseMode; ///< �W���̋���R
};

#endif // __SINDY_DRAW_I_H__
