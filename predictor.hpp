#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "type.hpp"

#ifdef __cplusplus
extern "C" {
#endif

// predictor handle
typedef void* OCR_PredictorContext;

// create predictor object
OCR_PredictorContext OCR_NewPredictor(bool isOutputConsole, bool isOutputPartImg, bool isOutputResultImg);

// destroy predictor object
void OCR_PredictorDelete(OCR_PredictorContext pred);

// set predictor thread count
void OCR_PredictorSetNumThread(OCR_PredictorContext pred, int numOfThread);

// set predictor models
bool OCR_PredictorInitModels(OCR_PredictorContext pred, const char* detPath, const char* clsPath, const char* recPath, const char* keysPath);

// detect image
const char* OCR_PredictorDetectFile(OCR_PredictorContext pred, const char* dir, const char* file, int padding, int maxSideLen, float boxScoreThresh, float boxThresh, float unClipRatio, bool doAngle, bool mostAngle);

#ifdef __cplusplus
}
#endif
