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

// 由于读入的图片为一组，故图片名大部分字符相同，仅做区分的编号数字不同
bool sort(QStringList &list, SortAlgorithm sa);

// 获取当前程序所在路径，将统一定位到release文件夹下
QString currAppPath();

// 显示警告提示框
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
	// 由于List是基于节点的，当把所有节点全部清空之后空间就会被释放，因此不需要调用swap
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
