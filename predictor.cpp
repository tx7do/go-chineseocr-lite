
#include "predictor.hpp"
#include "ocrlite.hpp"
#include "ocrlite_port.hpp"
#include "ocr_struct.hpp"

#include <memory>
#include <string>

struct Predictor
{
    std::shared_ptr<OcrLite> _ocrlite;
    Predictor() : _ocrlite(std::make_shared<OcrLite>()) {}
};

OCR_PredictorContext OCR_NewPredictor()
{
    const auto ctx = new Predictor();
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

void OCR_PredictorDetectFile(OCR_PredictorContext pred, const char* dir, const char* file, int padding, int maxSideLen, float boxScoreThresh, float boxThresh, float unClipRatio, bool doAngle, bool mostAngle)
{
    auto predictor = (Predictor*) pred;
    OcrResult result = predictor->_ocrlite->detect(dir, file, padding, maxSideLen, boxScoreThresh, boxThresh, unClipRatio, doAngle, mostAngle);
    predictor->_ocrlite->Logger("%s\n", result.strRes.c_str());
}
