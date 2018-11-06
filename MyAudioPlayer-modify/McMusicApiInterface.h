#pragma once

#include <qstring.h>

class McMusicApiInterface {
public:
	virtual ~McMusicApiInterface() = default;

	virtual QString getSongUrl(const QString &id) = 0;
};