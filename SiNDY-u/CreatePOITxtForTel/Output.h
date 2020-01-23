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
#include "POIPointRecord.h"
#include "POISubPointRecord.h"

typedef std::multiset<POIPointRecord> POIPointRecordSet;
typedef std::multiset<POIPointRecord>::const_iterator POIPointRecordSetItr;
typedef std::multiset<POISubPointRecord> POISubPointRecordSet;

// �f�[�^�o�͗p�N���X
class Output
{
public:
	~Output();

	/*
	 * @brief �t�@�C�����J��
	 * @param [in] outputFilePath �t�@�C���p�X
	 */
	void Open( const uh::tstring& outputFilePath );

	/*
	 * @brief �t�@�C�����J���Ă��邩�ǂ����𒲂ׂ�
	 * @retval true �J���Ă���
	 * @retval false �J���ĂȂ�
	 */
	bool IsOpen();

	/*
	 * @brief POI_POINT�f�[�^����������
	 * @param [in] recordSet POI_POINT�f�[�^
	 */
	void POIPointOutput( const POIPointRecordSet& recordSet );

	/*
	 * @brief POI_SUB_POINT�f�[�^����������
	 * @param [in] recordSet POI_SUB_POINT�f�[�^
	 */
	void POISubPointOutput( const POISubPointRecordSet& recordSet );

private:
	std::ofstream m_ofs; //!< �t�@�C���o�̓X�g���[��
};

