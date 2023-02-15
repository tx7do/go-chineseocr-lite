
#include "predictor.hpp"
#include "ocrlite.hpp"
#include "ocrlite_port.hpp"
#include "ocr_struct.hpp"

#include <memory>
#include <string>

struct Predictor
{
    std::shared_ptr<OcrLite> _ocrlite;
    OcrResult _latestResult;
    Predictor() : _ocrlite(std::make_shared<OcrLite>()) {}
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
    auto predictor = (Predictor*) pred;
    if (predictor == nullptr)
    {
        throw std::runtime_error(std::string("Invalid pointer to the predictor in OCR_PredictorDelete."));
    }
    delete predictor;
}

void OCR_PredictorSetNumThread(OCR_PredictorContext pred, int numOfThread)
{
    auto predictor = (Predictor*) pred;
    predictor->_ocrlite->setNumThread(numOfThread);
}

bool OCR_PredictorInitModels(OCR_PredictorContext pred, const char* detPath, const char* clsPath, const char* recPath, const char* keysPath)
{
    auto predictor = (Predictor*) pred;
    return predictor->_ocrlite->initModels(detPath, clsPath, recPath, keysPath);
}

const char* OCR_PredictorDetectFile(OCR_PredictorContext pred, const char* imgDir, const char* imgName, int padding, int maxSideLen, float boxScoreThresh, float boxThresh, float unClipRatio, bool doAngle, bool mostAngle)
{
    auto predictor = (Predictor*) pred;

    predictor->_ocrlite->enableResultTxt(imgDir, imgName);

    predictor->_ocrlite->Logger("=====Input Params=====\n");
    predictor->_ocrlite->Logger(
            "padding(%d),maxSideLen(%d),boxScoreThresh(%f),boxThresh(%f),unClipRatio(%f),doAngle(%d),mostAngle(%d)\n",
            padding, maxSideLen, boxScoreThresh, boxThresh, unClipRatio, doAngle, mostAngle);

    try
    {
        predictor->_latestResult = predictor->_ocrlite->detect(imgDir, imgName, padding, maxSideLen, boxScoreThresh, boxThresh, unClipRatio, doAngle, mostAngle);
        // predictor->_ocrlite->Logger("Result String:\n %s\n", result.strRes.c_str());
        return predictor->_latestResult.strRes.c_str();
    }
    catch (std::exception& e)
    {
        predictor->_ocrlite->Logger("Detect Error:\n %s\n", e.what());
        return "";
    }
}
