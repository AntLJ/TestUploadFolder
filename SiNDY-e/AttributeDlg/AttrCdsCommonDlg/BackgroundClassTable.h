// CDS�֘A���C���p ���e�ԍ�-��� �Ή��}�b�v
// CDS�Ή��ł́AANNOTATION_CLASS��BACKGROUND_CLASS�͎g�p���Ȃ��̂ŁA
// �����őΉ��\����������
// TODO: Excel�Ŏ������������y����

// �t�H�[�}�b�g
// BG_MAP( ���e�ԍ�, �R�[�h, �h���C��, ���C���� )

// �g�p��
// #define BG_MAP( msno, code, domain, layer ) if( 0 == m_strClassName.CompareNoCase(layer::kTableName) ) \
// 	                                           { BACKGROUND_RULE br = {0}; \
// 	                                             br.lClass = code; \
// 	                                             br.strClass_Name = domain; \
// 	                                             br.strMS_NO = msno; \
// 									             m_BackgroundRule.emplace_back(br); }
// #include "BackgroundClassTable.h"
// #undef BG_MAP

// CITY_SITE_GS
BG_MAP( _T("Z"), 0, _T("�������E��ƒ�"), city_site_gs )
BG_MAP( _T("3"), 14, _T("�A�͒n"), city_site_gs )
BG_MAP( _T("515"), 20, _T("�͐�"), city_site_gs )
BG_MAP( _T("520"), 23, _T("�C"), city_site_gs )
BG_MAP( _T("522"), 24, _T("�r�E��"), city_site_gs )
BG_MAP( _T("H1"), 30, _T("�s�s�ԍ����i�L���j"), city_site_gs )
BG_MAP( _T("R2"), 31, _T("����"), city_site_gs )
BG_MAP( _T("R3"), 32, _T("��v�n����"), city_site_gs )
BG_MAP( _T("R4"), 33, _T("��ʓs���{����"), city_site_gs )
BG_MAP( _T("6"), 34, _T("����"), city_site_gs )
BG_MAP( _T("16"), 35, _T("�뉀�H"), city_site_gs )
BG_MAP( _T("5A"), 37, _T("���������сi�W���j"), city_site_gs )
BG_MAP( _T("H1F"), 38, _T("�s�s�ԍ����i�����j"), city_site_gs )
BG_MAP( _T("15"), 51, _T("�ܑ��n���̑��~�n"), city_site_gs )
BG_MAP( _T("H2"), 70, _T("�s�s�����i�L���j"), city_site_gs )
BG_MAP( _T("H2F"), 71, _T("�s�s�����i�����j"), city_site_gs )
BG_MAP( _T("H3"), 72, _T("�L�����H�|���S��"), city_site_gs )
BG_MAP( _T("R6"), 73, _T("���̑��������H"), city_site_gs )
BG_MAP( _T("R7"), 74, _T("���̑���"), city_site_gs )
BG_MAP( _T(""), 101, _T("�������H�|���S���i���̌����j"), city_site_gs )
BG_MAP( _T(""), 111, _T("�s�s�������H�|���S���i���̌����j"), city_site_gs )
BG_MAP( _T(""), 121, _T("�����|���S���i���̌����j"), city_site_gs )
BG_MAP( _T(""), 131, _T("��v�n�����|���S���i���̌����j"), city_site_gs )
BG_MAP( _T(""), 141, _T("��ʓs���{�����|���S���i���̌����j"), city_site_gs )
BG_MAP( _T(""), 161, _T("�����|���S���i�S���H�j"), city_site_gs )
BG_MAP( _T(""), 171, _T("�L�����H�|���S���i���̌����j"), city_site_gs )

// CITY_LINE_GS
BG_MAP( _T("Z"), 0, _T("�������E��ƒ�"), city_line_gs )
BG_MAP( _T("2"), 2, _T("�j���\��"), city_line_gs )
BG_MAP( _T("8"), 4, _T("����"), city_line_gs )
BG_MAP( _T("9"), 5, _T("���h�_��"), city_line_gs )
BG_MAP( _T("KK2"), 15, _T("�K�i������"), city_line_gs )
BG_MAP( _T("5"), 20, _T("���������сi���C���j"), city_line_gs )
BG_MAP( _T("4"), 21, _T("���H�w�i"), city_line_gs )
BG_MAP( _T(""), 101, _T("������"), city_line_gs )
BG_MAP( _T(""), 111, _T("�s���{���E"), city_line_gs )
BG_MAP( _T(""), 112, _T("�s�����E"), city_line_gs )
BG_MAP( _T(""), 113, _T("��E"), city_line_gs )
BG_MAP( _T(""), 114, _T("�厚�E"), city_line_gs )
BG_MAP( _T(""), 115, _T("�������E"), city_line_gs )
BG_MAP( _T(""), 121, _T("�͐�"), city_line_gs )
BG_MAP( _T(""), 122, _T("���r�E��"), city_line_gs )
BG_MAP( _T(""), 123, _T("�C�ݐ�"), city_line_gs )
BG_MAP( _T(""), 131, _T("�����H"), city_line_gs )
BG_MAP( _T(""), 132, _T("�뉀�H"), city_line_gs )
BG_MAP( _T(""), 133, _T("������"), city_line_gs )
BG_MAP( _T(""), 141, _T("�^���N"), city_line_gs )

// CITY_RAILWAY_GS
BG_MAP( _T("Z"), 0, _T("�������E��ƒ�"), city_railway_gs )
BG_MAP( _T("20"), 1, _T("���ʓS��"), city_railway_gs )
BG_MAP( _T("25"), 2, _T("�n���S"), city_railway_gs )
BG_MAP( _T("30"), 3, _T("�H�ʓS��"), city_railway_gs )
BG_MAP( _T(""), 101, _T("����O��"), city_railway_gs )

// BUILDING_GS
BG_MAP( _T("Z"), 0, _T("�������E��ƒ�"), building_gs )
BG_MAP( _T("20"), 1, _T("���ʓS���w�i�n��j"), building_gs )
BG_MAP( _T("25"), 2, _T("�n���S�w�i�n��j"), building_gs )
BG_MAP( _T("20H"), 3, _T("�n���S�ȊO�w�z�[���i�n��j"), building_gs )
BG_MAP( _T("25H"), 4, _T("�n���S�w�z�[���i�n��j"), building_gs )
BG_MAP( _T("1"), 5, _T("������"), building_gs )
BG_MAP( _T("3"), 6, _T("�a�@"), building_gs )
BG_MAP( _T("2"), 7, _T("�w�Z"), building_gs )
BG_MAP( _T("6"), 8, _T("���w�W���Z��"), building_gs )
BG_MAP( _T("7"), 9, _T("�H��E��s�E�r��"), building_gs )
BG_MAP( _T("5"), 11, _T("�z�e��"), building_gs )
BG_MAP( _T("4"), 12, _T("��y�E�f�p�[�g"), building_gs )
BG_MAP( _T("9"), 13, _T("�w�r��"), building_gs )
BG_MAP( _T("12"), 16, _T("�s��"), building_gs )
BG_MAP( _T("13"), 17, _T("�H��"), building_gs )
BG_MAP( _T("14"), 18, _T("�q��"), building_gs )
BG_MAP( _T("30"), 19, _T("�H�ʓS���w�i�n��j"), building_gs )
BG_MAP( _T("16"), 26, _T("�A�[�P�[�h�i�ʁj"), building_gs )
BG_MAP( _T("17"), 27, _T("�������i�ʁj"), building_gs )
BG_MAP( _T("27"), 28, _T("�V��ʉw�i�n��j"), building_gs )
BG_MAP( _T("26"), 29, _T("���m���[���w�i�n��j"), building_gs )
BG_MAP( _T("28"), 30, _T("�P�[�u���J�[�w�i�n��j"), building_gs )
BG_MAP( _T("18"), 31, _T("���q���E�ČR"), building_gs )
BG_MAP( _T("10"), 32, _T("�K�\�����X�^���h"), building_gs )
BG_MAP( _T(""), 99, _T("�e�i���g�����m�F"), building_gs )

// CITY_STATION_GS
BG_MAP( _T("Z"), 0, _T("�������E��ƒ�"), city_station_gs )
BG_MAP( _T("20"), 1, _T("���ʓS���w�i�n���j"), city_station_gs )
BG_MAP( _T("25"), 2, _T("�n���S�w�i�n���j"), city_station_gs )
BG_MAP( _T("27"), 3, _T("�V��ʃV�X�e���i�n���j"), city_station_gs )
BG_MAP( _T("26"), 4, _T("���m���[���i�n���j"), city_station_gs )
BG_MAP( _T("28"), 5, _T("�P�[�u���J�[�i�n���j"), city_station_gs )