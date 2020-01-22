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

namespace data_output
{
	/**
	 * @brief	�i�r����Pnt���R�[�h�o��
	 * @param	stream		[in]	�o�̓X�g���[��
	 * @param	rPntList	[in]	Pnt���R�[�h���X�g
	 */
	void OutPntFile( std::ofstream& stream, const PNTMAP& rPntList );

	/**
	 * @brief	�O���񋟌���Pnt���R�[�h�o��
	 * @note	Bug7599
	 * @param	stream		[in]	�o�̓X�g���[��
	 * @param	rPntList	[in]	Pnt���R�[�h���X�g
	 */
	void OutPntFile_Outside( std::ofstream& stream, const PNTMAP& rPntList );

	/**
	 * @brief	�i�r����Gou���R�[�h�o��
	 * @param	stream		[in]	�o�̓X�g���[��
	 * @param	rGouList	[in]	Gou���R�[�h���X�g
	 */
	void OutGouFile( std::ofstream& stream, const GOUMAP& rGouList );

	/**
	 * @brief	�O���񋟌���Gou���R�[�h�o��
	 * @note	Bug7599
	 * @param	stream		[in]	�o�̓X�g���[��
	 * @param	rGouList	[in]	Gou���R�[�h���X�g
	 */
	void OutGouFile_Outside( std::ofstream& stream, const GOUMAP& rGouList );
}
