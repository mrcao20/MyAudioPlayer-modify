#pragma once

#include "McMusicApiInterface.h"
#include <qscopedpointer.h>

struct McQQApiData;

class McQQApi : public McMusicApiInterface {
public:
	explicit McQQApi();
	virtual ~McQQApi();

	QString getSongUrl(const QString &songMid) override;

private:
	QScopedPointer<McQQApiData> d;
};

