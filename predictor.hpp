#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// predictor handle
typedef void* OCR_PredictorContext;
// predictor result handle
typedef void* OCR_PredictorResult;

// destroy predictor result object
void OCR_DeleteResult(OCR_PredictorResult result);
const char* OCR_ResultGetString(OCR_PredictorResult result);

// create predictor object
OCR_PredictorContext OCR_NewPredictor(bool isOutputConsole, bool isOutputPartImg, bool isOutputResultImg);

// destroy predictor object
void OCR_DeletePredictor(OCR_PredictorContext pred);

// set predictor thread count
void OCR_PredictorSetNumThread(OCR_PredictorContext pred, int numOfThread);

// set predictor models
bool OCR_PredictorInitModels(OCR_PredictorContext pred,
	const char* detPath, const char* clsPath, const char* recPath,
	const char* keysPath);

// detect file image
OCR_PredictorResult OCR_PredictorDetectFileImage(OCR_PredictorContext pred,
	const char* dir, const char* file,
	int padding, int maxSideLen,
	float boxScoreThresh, float boxThresh, float unClipRatio,
	bool doAngle, bool mostAngle);

// detect memory image
OCR_PredictorResult OCR_PredictorDetectMemoryImage(OCR_PredictorContext pred,
	char* imageBuffer, int bufferLength,
	int padding, int maxSideLen,
	float boxScoreThresh, float boxThresh, float unClipRatio,
	bool doAngle, bool mostAngle);

#ifdef __cplusplus
}
#endif
