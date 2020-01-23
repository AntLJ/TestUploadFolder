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

// Bug9215�Ή��Fbool2��pair�^��bool3�^�ɕύX
struct POIUSABLEINFO
{
	bool	bQualityF;		// �ʒu�i������F
	bool	bPPRateF;		// �s���|�C���g������F
	bool	bImportantF;	// �d�v�R���e���cF

	POIUSABLEINFO() : bQualityF(false), bPPRateF(false), bImportantF(false){}
};

/**
 * @class	CMasterListCache
 * @brief	���X�g���}�X�^�[�n�L���b�V���N���X
 */
class CMasterListCache
{
public:
	CMasterListCache(void);
	~CMasterListCache(void);

	/**
	 * @brief	�L���b�V���쐬
	 * @param	lpcszFile	[in]	�}�X�^���X�g�t�@�C��
	 * @return	bool
	 */
	bool Create( const _TCHAR* lpcszFile );

	/**
	 * @brief	���YPOI�R���e���c���p�\���擾
	 * @param	lGrpCode	[in]	���ރR�[�h
	 * @param	lChainCode	[in]	�X�܃R�[�h
	 * @return	���p�\���
	 */
	const POIUSABLEINFO* GetPOIMasterInfo( long lGrpCode, long lChainCode )
	{
		std::multimap<long, std::map<long, POIUSABLEINFO> >::const_iterator it = m_mapCache.find( lGrpCode );
		if( it != m_mapCache.end() )
		{
			std::map<long, POIUSABLEINFO>::const_iterator itS = it->second.find(lChainCode);
			return itS != it->second.end()? &(itS->second) : NULL;
		}
		else
			return NULL;
	}

private:

	std::multimap<long, std::map<long, POIUSABLEINFO> >	m_mapCache;	// �L�[:���� �l:(�L�[:�X�� �l:�ʒu�t���O�APP�t���O)

};
