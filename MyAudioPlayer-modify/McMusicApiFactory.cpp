#include "McMusicApiFactory.h"
#include <McQQApi.h>
#include <McYApi.h>

McMusicApiFactory::McMusicApiFactory()
{
}

McMusicApiInterface *McMusicApiFactory::getMusicApi(const QString &apiName) {
	McMusicApiInterface *api = NULL;
	if (apiName == "qqmusic") {
		McQQApi *qqApi = new McQQApi();
		api = qqApi;
	}
	else if (apiName == "ymusic") {
		McYApi *yApi = new McYApi();
		api = yApi;
	}
	return api;
}
