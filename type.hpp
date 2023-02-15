#pragma once

#include <stdbool.h>
#include <stdint.h>

// Wrapper for std::vector<string>
typedef struct CStrings {
    const char** strs;
    int length;
} CStrings;

typedef struct ByteArray {
    char* data;
    int length;
} ByteArray;

// Wrapper for std::vector<int>
typedef struct IntVector {
    int* val;
    int length;
} IntVector;

// Wrapper for std::vector<float>
typedef struct FloatVector {
    float* val;
    int length;
} FloatVector;

#ifdef __cplusplus
#include <opencv2/opencv.hpp>
extern "C" {
#endif

#ifdef __cplusplus
typedef cv::Mat* Mat;
typedef cv::TermCriteria* TermCriteria;
typedef cv::RNG* RNG;
typedef std::vector< cv::Point >* PointVector;
typedef std::vector< std::vector< cv::Point > >* PointsVector;
typedef std::vector< cv::Point2f >* Point2fVector;
typedef std::vector< std::vector< cv::Point2f> >* Points2fVector;
typedef std::vector< cv::Point3f >* Point3fVector;
typedef std::vector< std::vector< cv::Point3f > >* Points3fVector;
#else
typedef void* Mat;
typedef void* TermCriteria;
typedef void* RNG;
typedef void* PointVector;
typedef void* PointsVector;
typedef void* Point2fVector;
typedef void* Points2fVector;
typedef void* Point3fVector;
typedef void* Points3fVector;
#endif

// Wrapper for an individual cv::Point2f
typedef struct Point2f {
    float x;
    float y;
} Point2f;

typedef struct Point3f {
    float x;
    float y;
    float z;
} Point3f;

// Wrapper for an individual cv::cvPoint
typedef struct Point {
    int x;
    int y;
} Point;

Mat Mat_New();
void Mat_Close(Mat m);
int Mat_Empty(Mat m);

#ifdef __cplusplus
}
#endif
