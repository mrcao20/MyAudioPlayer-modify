#pragma once

#include "McMusicApiInterface.h"
#include <qstring.h>

class McMusicApiFactory {
public:
	explicit McMusicApiFactory();

	McMusicApiInterface *getMusicApi(const QString &apiName);
};

