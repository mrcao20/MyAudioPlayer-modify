#pragma once

#include "McMusicApiInterface.h"
#include <qscopedpointer.h>

struct McYApiData;

class McYApi : public McMusicApiInterface {
public:
	explicit McYApi();
	virtual ~McYApi();

	QString getSongUrl(const QString &songId) override;

private:
	QScopedPointer<McYApiData> d;
};

