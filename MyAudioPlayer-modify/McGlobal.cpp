#include "McGlobal.h"
#include <qregularexpression.h>
#include <qapplication.h>
#include <qmessagebox.h>

namespace Mc {

void countingSort(QList<int> &list) {

}

int hoare_partition(QList<int> &arr, int l, int r) {
	int key = arr[l];
	while (l < r) {
		while (r > l) {
			if (arr[r] < key) {
				arr[l] = arr[r];
				break;
			}
			r--;
		}
		while (l < r) {
			if (arr[l] > key) {
				arr[r] = arr[l];
				break;
			}
			l++;
		}
	}
	arr[l] = key;
	return l;
}

// l����ߵ��±꣬r���ұߵ��±�
void quick_sort(QList<int> &arr, int l, int r) {
	if (l >= r)
		return;
	int m = hoare_partition(arr, l, r);
	quick_sort(arr, l, m);
	quick_sort(arr, m + 1, r);
}

void sort(QList<int> &list, SortAlgorithm sa) {
	switch (sa) {
	case CountingSort:
		countingSort(list);
		break;
	case QuickSort:
		quick_sort(list, 0, list.size() - 1);
		break;
	default:
		break;
	}
}

bool sort(QStringList &list, SortAlgorithm sa) {
	QList<int> l;
	QString prefix = "";
	QString suffix = "";
	bool getPre_SufFix = true;
	QRegularExpression re(R"((\D*)(\d+)(\D+))");
	for (const QString &str : list) {
		QRegularExpressionMatch match = re.match(str);
		QStringList texts = match.capturedTexts();
		if (texts.size() != 4) {
			mcOutput << "ͼƬ������ʧ�ܣ����飡";
			return false;
		}
		if (getPre_SufFix) {
			getPre_SufFix = false;
			prefix = texts.at(1);
			suffix = texts.at(3);
		}
		l.append(texts.at(2).toInt());

	}
	sort(l, sa);
	list.clear();
	for (int i : l) {
		list.append(prefix + QString::number(i) + suffix);
	}
	return true;
}

QString currAppPath() {
#ifdef DEBUG
	QString path = qApp->applicationDirPath() + "/../Release";
#else
	QString path = qApp->applicationDirPath();
#endif // _DEBUG
	return path;
}

void showWarning(const QString &str) {
	QMessageBox::warning(NULL, "Warning", str);
}

}

#ifdef OPENCV
namespace Mc {

QImage cvMat2QImage(const cv::Mat& mat) {
	// 8-bits unsigned, NO. OF CHANNELS = 1
	if (mat.type() == CV_8UC1) {
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)
		image.setColorCount(256);
		for (int i = 0; i < 256; i++) {
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++) {
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3
	else if (mat.type() == CV_8UC3) {
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4) {
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else {
		mcOutput << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}
}

cv::Mat QImage2cvMat(const QImage &image) {
	cv::Mat mat;
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	default:
		mcOutput << "ERROR: QImage could not be converted to Mat.";
	}
	return mat;
}

}
#endif // OPENCV
