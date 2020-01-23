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

#include <boost/program_options.hpp>

typedef std::pair<long, long> POIMASTERDBINFO;

/**
 * @class	CMasterDBCache
 * @brief	DB���}�X�^�[�n�L���b�V���N���X
 */
class CMasterDBCache
{
public:
	CMasterDBCache(void);
	~CMasterDBCache(void);

	/**
	 * @brief	�L���b�V���쐬
	 * @param	ipWorkspace	[in]	���[�N�X�y�[�X
	 * @return	bool
	 */
	bool Create( IWorkspacePtr ipWorkspace );

	/**
	 * @brief	�f�[�^���ށE�X�܃R�[�h���擾
	 * @param	lContentsCode	[in]	�R���e���c�R�[�h
	 * @return	�f�[�^���ށE�X�܃R�[�h���
	 */
	const POIMASTERDBINFO* GetPOIMasterInfo( long lContentsCode )
	{
		std::map<long, POIMASTERDBINFO>::const_iterator it = m_mapInfo.find( lContentsCode );
		return it != m_mapInfo.end()? &(it->second) : NULL;
	}

	/**
	 * @brief	�R���e���c�R�[�h���X�g����AWHERE��쐬�iBug9215�Ή��j
	 * @return	WHERE���vector
	 */
	const std::vector<std::wstring> CreateWhereVecFromContentCode( const std::wstring& wsFieldName )
	{
		std::vector<std::wstring>	vecWhere;
		std::wostringstream	woStreamWhere;
		
		// �J�E���g�l
		int	iCnt = 1;
		for( std::set<long>::const_iterator itr = m_setContentsCode.begin() ; itr != m_setContentsCode.end() ; ++itr )
		{
			if( iCnt == 1 )
				woStreamWhere << wsFieldName << _T(" IN (") << *itr;
			else
				woStreamWhere << _T(",") << *itr;

			if( *itr == *(m_setContentsCode.rbegin()) || iCnt == 200 )
			{
				woStreamWhere << _T(")");
				vecWhere.push_back( woStreamWhere.str() );
				woStreamWhere.str(_T(""));
				iCnt = 0;
			}
			iCnt++;

		}
		return vecWhere;
	}

	CString	m_strContentsMaster;	//!< �R���e���c�}�X�^�擾�p������ cApp::Init()�ň����Ƃ��Ă��ď�����
private:

	std::map<long, POIMASTERDBINFO>	m_mapInfo;	//!< �L�[ : �R���e���c�R�[�h �l : (���ރR�[�h�A�X�܃R�[�h)
	std::set<long>					m_setContentsCode;

};
