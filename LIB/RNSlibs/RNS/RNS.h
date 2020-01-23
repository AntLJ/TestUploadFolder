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

// RNS.h
//
//////////////////////////////////////////////////////////////////////

#ifndef __RNS__
#define __RNS__

#ifdef _WIN32
#pragma warning(disable : 4290)
#endif // _WIN32 //

// �f�B���N�g����؂蕶��
#if defined(macintosh)
#define	_pathChr ':'
#elif defined(_WIN32)
#define	_pathChr '\\'
#else
#define	_pathChr '/'
#endif

#define RNS_LOCATION "[" __FILE__ "]"

#include "RNSSindyCountry.h"
#include <crd_cnv/coord_converter.h>

extern crd_cnv g_cnv;

/**
 * @brief �����񋓎q
 *
 * �e�X�̒l���r�b�gOR�������邱�Ƃ�
 */
typedef enum RNS_Direction {
	RNS_NO_DIR          =  0, ///< ��������
	RNS_UP              =  1, ///< ��
	RNS_DOWN            =  2, ///< ��
	RNS_UP_DOWN         =  3, ///< �㉺
	RNS_LEFT            =  4, ///< ��
	RNS_UP_LEFT         =  5, ///< ����
	RNS_DOWN_LEFT       =  6, ///< ����
	RNS_UP_DOWN_LEFT    =  7, ///< �㉺��
	RNS_RIGHT           =  8, ///< �E
	RNS_UP_RIGHT        =  9, ///< �E��
	RNS_DOWN_RIGHT      = 10, ///< �E��
	RNS_UP_DOWN_RIGTH   = 11, ///< �㉺�E
	RNS_LEFT_RIGHT      = 12, ///< ���E
	RNS_UP_LEFT_RIGHT   = 13, ///< �㍶�E
	RNS_DOWN_LEFT_RIGHT = 14, ///< �����E
	RNS_FULL_DIR        = 15, ///< �S����
};

/**
 * @brief �|�C���g�̍��W�n
 */
enum CCoordinate{
	kMeshXY            = 1, //!< ���b�V��XY
	kLonLat            = 2, //!< �o�ܓx
	kRectangularCoords = 3, //!< �������W
	kVector            = 4, //!< RNSVector�p
	kUnknownCoords     = -1 //!< �s��
};

/**
 * @brief �|�C���g�̍��W�n�A�����
 */
class CInfo {
	CCoordinate m_Coordinate;
	country::country_type m_Country;
public:
	// �f�t�H���g�R���X�g���N�^
	CInfo(){ m_Coordinate = CCoordinate::kUnknownCoords; m_Country = country::country_type::japan; }

	// �R���X�g���N�^
	CInfo(country::country_type countryType){ m_Coordinate = CCoordinate::kUnknownCoords; m_Country = countryType; }
	CInfo(CCoordinate coordinate, country::country_type countryType){ m_Coordinate = coordinate; m_Country = countryType; }

	CCoordinate coordinate(){ return m_Coordinate; }
	country::country_type country(){ return m_Country; }

	void operator=(CInfo &cInfo)
	{
		this->m_Coordinate = cInfo.m_Coordinate;
		this->m_Country = cInfo.m_Country;
	}

	bool operator==(CInfo &cInfo)
	{
		return (this->m_Coordinate == cInfo.m_Coordinate &&
			this->m_Country == cInfo.m_Country);
	}
};

#endif // __RNS__ //
