#pragma once
#include <vector>
#include <atlstr.h>

typedef std::vector<CString> FIELD_LIST; //!< �t�B�[���h���X�g

namespace field_lists
{

	/**
	* @brief	���H�����N�̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST roadLinkFieldList;
	/**
	* @brief	���H�m�[�h�̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST roadNodeFieldList;
	/**
	* @brief	�����N�������̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST segmentAttrFieldList;
	/**
	* @brief	���s�҃����N�̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST walkLinkFieldList;
	/**
	* @brief	���s�҃m�[�h�̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST walkNodeFieldList;
	/**
	* @brief	�w�o����POI�̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST stationGatePointFieldList;
	/**
	* @brief	POI�|�C���g�̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST poiPointFieldList;
	/**
	* @brief �n���X�o�����̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST underGroundAeraFiledList;
	/**
	* @brief	���s�ҘH���R�[�h�̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST walkCodeFieldList;
	/**
	* @brief	���s�Ғʍs�֎~�K���e�[�u���̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST walkNoPassageFieldList;
	/**
	* @brief	���s�҈���ʍs�K���e�[�u���̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST walkOnewayFieldList;
	/**
	* @brief	�����N�R�t���e�[�u���̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST linkRelationFieldList;
	/**
	* @brief	�d�Ȃ胊���N�����m�[�h�̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST heightNodeFieldList;
	/**
	* @brief	�X�N�����u���G���A�̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST scrambleAreaList;
	/**
	* @brief	���f�|�C���g�̃t�B�[���h�Q
	*/
	extern 	const FIELD_LIST pedxingList;
}