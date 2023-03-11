
#include "predictor.hpp"
#include "ocrlite.hpp"
#include "ocr_struct.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgcodecs/legacy/constants_c.h>

#include <memory>
#include <string>

struct Predictor
{
	std::shared_ptr<OcrLite> _ocrlite;
	Predictor()
		: _ocrlite(std::make_shared<OcrLite>())
	{
	}
};

OCR_PredictorContext OCR_NewPredictor(bool isOutputConsole, bool isOutputPartImg, bool isOutputResultImg)
{
	const auto ctx = new Predictor();
	assert(ctx);

	ctx->_ocrlite->initLogger(isOutputConsole, isOutputPartImg, isOutputResultImg);

	return (OCR_PredictorContext)ctx;
}

void OCR_DeletePredictor(OCR_PredictorContext pred)
{
	auto predictor = (Predictor*)pred;
	assert(predictor);
	if (predictor == nullptr)
	{
		//throw std::runtime_error(std::string("Invalid pointer to the predictor in OCR_DeletePredictor."));
		return;
	}
	delete predictor;
}

void OCR_DeleteResult(OCR_PredictorResult res)
{
	auto result = (OcrResult*)res;
	//assert(result);
	if (result == nullptr)
	{
		//throw std::runtime_error(std::string("Invalid pointer to the result in OCR_DeleteResult."));
		return;
	}
	delete result;
}

const char* OCR_ResultGetString(OCR_PredictorResult res)
{
	auto result = (OcrResult*)res;
	//assert(result);
	if (result == nullptr)
	{
		//throw std::runtime_error(std::string("Invalid pointer to the result in OCR_ResultGetString."));
		return "";
	}
	return result->strRes.c_str();
}

void OCR_PredictorSetNumThread(OCR_PredictorContext pred, int numOfThread)
{
	auto predictor = (Predictor*)pred;
	assert(predictor);
    if (predictor == nullptr)
    {
        //throw std::runtime_error(std::string("Invalid pointer to the result in OCR_ResultGetString."));
        return;
    }

	predictor->_ocrlite->setNumThread(numOfThread);
}

bool OCR_PredictorInitModels(OCR_PredictorContext pred,
	const char* detPath,
	const char* clsPath,
	const char* recPath,
	const char* keysPath)
{
	auto predictor = (Predictor*)pred;
	assert(predictor);
    if (predictor == nullptr)
    {
        //throw std::runtime_error(std::string("Invalid pointer to the result in OCR_ResultGetString."));
        return false;
    }

	return predictor->_ocrlite->initModels(detPath, clsPath, recPath, keysPath);
}

OCR_PredictorResult OCR_PredictorDetectFileImage(OCR_PredictorContext pred,
	const char* imgDir, const char* imgName,
	int padding, int maxSideLen,
	float boxScoreThresh, float boxThresh, float unClipRatio,
	bool doAngle, bool mostAngle)
{
	auto predictor = (Predictor*)pred;
	assert(predictor);
    if (predictor == nullptr)
    {
        //throw std::runtime_error(std::string("Invalid pointer to the result in OCR_ResultGetString."));
        return nullptr;
    }

	//predictor->_ocrlite->enableResultTxt(imgDir, imgName);

	predictor->_ocrlite->log("=====Input Params=====");
	predictor->_ocrlite->log(
		"padding(%d),maxSideLen(%d),boxScoreThresh(%f),boxThresh(%f),unClipRatio(%f),doAngle(%d),mostAngle(%d)",
		padding, maxSideLen, boxScoreThresh, boxThresh, unClipRatio, doAngle, mostAngle);

    auto result = new OcrResult;
	try
	{
		*result = predictor->_ocrlite->detect(imgDir, imgName,
			padding, maxSideLen,
			boxScoreThresh, boxThresh, unClipRatio,
			doAngle, mostAngle);
		// predictor->_ocrlite->log("Result String:\n %s\n", result.strRes.c_str());
		return (OCR_PredictorResult) result;
	}
	catch (std::exception& e)
	{
		predictor->_ocrlite->log("Detect Error: %s", e.what());
		if (result != nullptr) delete result;
		return nullptr;
	}
}

OCR_PredictorResult OCR_PredictorDetectMemoryImage(OCR_PredictorContext pred,
	char* imageBuffer, int bufferLength,
	int padding, int maxSideLen,
	float boxScoreThresh, float boxThresh, float unClipRatio,
	bool doAngle, bool mostAngle)
{
	auto predictor = (Predictor*)pred;
	assert(predictor);
    if (predictor == nullptr)
    {
        //throw std::runtime_error(std::string("Invalid pointer to the result in OCR_ResultGetString."));
        return nullptr;
    }

	if (imageBuffer == nullptr || bufferLength < 0)
	{
	    predictor->_ocrlite->log("image byte buffer invalid");
	    return nullptr;
	}

	std::vector<char> buff;
	buff.resize(bufferLength);
	std::memcpy(buff.data(), imageBuffer, bufferLength);

	//predictor->_ocrlite->log("=====Decode Image=====\n");

	cv::Mat matImg;
	try
	{
	    matImg = cv::imdecode(cv::Mat(buff), CV_LOAD_IMAGE_UNCHANGED);
	}
	catch (std::exception& e)
	{
		predictor->_ocrlite->log("decode image error: %s", e.what());
		return nullptr;
	}

	if (matImg.data == nullptr)
	{
		predictor->_ocrlite->log("decode image data invalid");
		return nullptr;
	}

	predictor->_ocrlite->log("=====Decode Image Done=====");

	//predictor->_ocrlite->enableResultTxt("./", "memory.jpg");

	predictor->_ocrlite->log("=====Input Params=====");
	predictor->_ocrlite->log(
		"padding(%d),maxSideLen(%d),boxScoreThresh(%f),boxThresh(%f),unClipRatio(%f),doAngle(%d),mostAngle(%d)",
		padding, maxSideLen, boxScoreThresh, boxThresh, unClipRatio, doAngle, mostAngle);

    //auto result = new OcrResult;
	try
	{
		auto result = predictor->_ocrlite->detect(matImg,
			padding, maxSideLen,
			boxScoreThresh, boxThresh, unClipRatio,
			doAngle, mostAngle);
		// predictor->_ocrlite->log("Result String:\n %s\n", result.strRes.c_str());
		//return (OCR_PredictorResult) result;
		return nullptr;
	}
	catch (std::exception& e)
	{
		predictor->_ocrlite->log("Detect Error:\n %s\n", e.what());
//		if (result != nullptr) delete result;
		return nullptr;
	}
}
