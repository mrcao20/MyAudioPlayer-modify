#include "McAudioPlayer.h"
#include "McSingleApplication.h"

QString commandLineFilePathArgument() {
	const QStringList args = QCoreApplication::arguments();
	for (const QString &arg : args.mid(1)) {
		if (!arg.startsWith(QLatin1Char('-')))
			return arg;
	}
	return "";
}

int main(int argc, char *argv[]) {
	McSingleApplication a(argc, argv);
	if (a.isRunning())
		return 0;
	McAudioPlayer w;
	w.init();
	a.w = &w;
	QString filePath = commandLineFilePathArgument();
	if (!filePath.isEmpty())
		w.playFile(filePath);
	w.show();
	return a.exec();
}
