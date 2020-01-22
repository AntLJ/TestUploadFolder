#pragma once

// エラータイプ
namespace err_type{
	enum ERoadlistErrorType {
		no_problem       =  0,
		failed_load      = -1,
		no_line_roadlist = -2,
		less_column      = -3,
		no_roadcode      = -4,
		no_directioncode = -5,
		less_point       = -6,
		short_distance   = -7,
	};
};