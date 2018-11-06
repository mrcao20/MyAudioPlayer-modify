#pragma once
#pragma execution_character_set("utf-8")

#include <qimage.h>
#include <qlist.h>
#include <qvector.h>
#include "McOutput.h"

#define mcOutput McOutput()

#ifdef _DEBUG
	#ifndef DEBUG
		#define DEBUG
	#endif
#else
	#ifndef RELEASE
		#define RELEASE
	#endif
#endif // _DEBUG

namespace Mc {

enum ShapeType{
	Rect,
	Polygon,
	Ellipse,
	Line,
	Pen
};

enum SortAlgorithm {
	QuickSort,
	CountingSort
};

void sort(QList<int> &list, SortAlgorithm sa);

// ���ڶ����ͼƬΪһ�飬��ͼƬ���󲿷��ַ���ͬ���������ֵı�����ֲ�ͬ
bool sort(QStringList &list, SortAlgorithm sa);

// ��ȡ��ǰ��������·������ͳһ��λ��release�ļ�����
QString currAppPath();

// ��ʾ������ʾ��
void showWarning(const QString &str);

template<typename T>
inline void clearItem(QVector<T *> &items) {
	for (T *item : items) {
		delete item;
	}
	items.swap(QVector<T *>());
}

template<typename T>
inline void clearItem(QList<T *> &items) {
	for (T *item : items) {
		delete item;
	}
	// ����List�ǻ��ڽڵ�ģ��������нڵ�ȫ�����֮��ռ�ͻᱻ�ͷţ���˲���Ҫ����swap
	items.clear();
}

template<typename T>
inline bool exists(const QVector<T> &items, int i) {
	return (i >= 0 && i < items.size());
}

template<typename T>
inline bool exists(const QList<T> &items, int i) {
	return (i >= 0 && i < items.size());
}

template<typename T>
inline void remove(QVector<T *> &items, T *t) {
	items.removeOne(t);
	delete t;
}

template<typename T>
inline void remove(QList<T *> &items, T *t) {
	items.removeOne(t);
	delete t;
}

}

#ifdef OPENCV
#include <opencv2/opencv.hpp>

namespace Mc {
	QImage cvMat2QImage(const cv::Mat& mat);
	cv::Mat QImage2cvMat(const QImage &image);
}
#endif // OPENCV
