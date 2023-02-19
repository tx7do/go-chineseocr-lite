
#include "predictor.hpp"
#include "ocrlite.hpp"
#include "ocr_struct.hpp"

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgcodecs/legacy/constants_c.h>
#include <opencv2/imgproc/types_c.h>

#include <memory>
#include <string>

struct Predictor
{
	std::shared_ptr<OcrLite> _ocrlite;
	OcrResult _latestResult;
	Predictor()
		: _ocrlite(std::make_shared<OcrLite>())
	{
	}
};

OCR_PredictorContext OCR_NewPredictor(bool isOutputConsole, bool isOutputPartImg, bool isOutputResultImg)
{
	const auto ctx = new Predictor();

	ctx->_ocrlite->initLogger(
		isOutputConsole,//isOutputConsole
		isOutputPartImg,//isOutputPartImg
		isOutputResultImg);//isOutputResultImg

	return (OCR_PredictorContext)ctx;
}

void OCR_PredictorDelete(OCR_PredictorContext pred)
{
	auto predictor = (Predictor*)pred;
	if (predictor == nullptr)
	{
		throw std::runtime_error(std::string("Invalid pointer to the predictor in OCR_PredictorDelete."));
	}
	delete predictor;
}

void OCR_PredictorSetNumThread(OCR_PredictorContext pred, int numOfThread)
{
	auto predictor = (Predictor*)pred;
	predictor->_ocrlite->setNumThread(numOfThread);
}

bool OCR_PredictorInitModels(OCR_PredictorContext pred,
	const char* detPath,
	const char* clsPath,
	const char* recPath,
	const char* keysPath)
{
	auto predictor = (Predictor*)pred;
	return predictor->_ocrlite->initModels(detPath, clsPath, recPath, keysPath);
}

const char* OCR_PredictorDetectFileImage(OCR_PredictorContext pred,
	const char* imgDir, const char* imgName,
	int padding, int maxSideLen,
	float boxScoreThresh, float boxThresh, float unClipRatio,
	bool doAngle, bool mostAngle)
{
	auto predictor = (Predictor*)pred;

	predictor->_ocrlite->enableResultTxt(imgDir, imgName);

	predictor->_ocrlite->log("=====Input Params=====\n");
	predictor->_ocrlite->log(
		"padding(%d),maxSideLen(%d),boxScoreThresh(%f),boxThresh(%f),unClipRatio(%f),doAngle(%d),mostAngle(%d)\n",
		padding, maxSideLen, boxScoreThresh, boxThresh, unClipRatio, doAngle, mostAngle);

	try
	{
		predictor->_latestResult = predictor->_ocrlite->detect(imgDir, imgName,
			padding, maxSideLen,
			boxScoreThresh, boxThresh, unClipRatio,
			doAngle, mostAngle);
		// predictor->_ocrlite->log("Result String:\n %s\n", result.strRes.c_str());
		return predictor->_latestResult.strRes.c_str();
	}
	catch (std::exception& e)
	{
		predictor->_ocrlite->log("Detect Error:\n %s\n", e.what());
		return "";
	}
}

const char* OCR_PredictorDetectMemoryImage(OCR_PredictorContext pred,
	char* imageBuffer, int bufferLength,
	int padding, int maxSideLen,
	float boxScoreThresh, float boxThresh, float unClipRatio,
	bool doAngle, bool mostAngle)
{
	auto predictor = (Predictor*)pred;

	std::vector<char> buff;
	buff.resize(bufferLength);
	std::memcpy(buff.data(), imageBuffer, bufferLength);

	predictor->_ocrlite->log("=====Decode Image=====\n");

	cv::Mat matImg;
	try
	{
	    matImg = cv::imdecode(cv::Mat(buff), CV_LOAD_IMAGE_COLOR);
	}
	catch (std::exception& e)
	{
		predictor->_ocrlite->log("decode image error:\n %s\n", e.what());
		return "";
	}

	predictor->_ocrlite->log("=====Decode Image Done=====\n");

	predictor->_ocrlite->enableResultTxt("memory", "images");

	predictor->_ocrlite->log("=====Input Params=====\n");
	predictor->_ocrlite->log(
		"padding(%d),maxSideLen(%d),boxScoreThresh(%f),boxThresh(%f),unClipRatio(%f),doAngle(%d),mostAngle(%d)\n",
		padding, maxSideLen, boxScoreThresh, boxThresh, unClipRatio, doAngle, mostAngle);

	try
	{
		predictor->_latestResult = predictor->_ocrlite->detect(matImg,
			padding, maxSideLen,
			boxScoreThresh, boxThresh, unClipRatio,
			doAngle, mostAngle);
		// predictor->_ocrlite->log("Result String:\n %s\n", result.strRes.c_str());
		return predictor->_latestResult.strRes.c_str();
	}
	catch (std::exception& e)
	{
		predictor->_ocrlite->log("Detect Error:\n %s\n", e.what());
		return "";
	}
}
