package ocrlite

/*
#include <stdlib.h>
#include "predictor.hpp"
*/
import "C"
import "unsafe"

type Predictor struct {
	ctx C.OCR_PredictorContext
}

func New() *Predictor {
	pred := &Predictor{
		ctx: C.OCR_NewPredictor(),
	}
	return pred
}

func (p *Predictor) Close() {
	C.OCR_PredictorDelete(p.ctx)
	p.ctx = nil
}

func (p *Predictor) SetNumThread(numOfThread int) {
	C.OCR_PredictorSetNumThread(p.ctx, C.int(numOfThread))
}

func (p *Predictor) InitModels(detPath, clsPath, recPath, keysPath string) bool {
	cDetPath := C.CString(detPath)
	defer C.free(unsafe.Pointer(cDetPath))

	cClsPath := C.CString(clsPath)
	defer C.free(unsafe.Pointer(cClsPath))

	cRecPath := C.CString(recPath)
	defer C.free(unsafe.Pointer(cRecPath))

	cKeysPath := C.CString(keysPath)
	defer C.free(unsafe.Pointer(cKeysPath))

	return bool(C.OCR_PredictorInitModels(p.ctx, cDetPath, cClsPath, cRecPath, cKeysPath))
}

func (p *Predictor) Detect(imgDir, imgFile string, padding, maxSideLen int, boxScoreThresh, boxThresh, unClipRatio float32, doAngle, mostAngle bool) {
	cImgDir := C.CString(imgDir)
	defer C.free(unsafe.Pointer(cImgDir))

	cImgFile := C.CString(imgFile)
	defer C.free(unsafe.Pointer(cImgFile))

	C.OCR_PredictorDetectFile(p.ctx,
		cImgDir, cImgFile,
		C.int(padding), C.int(maxSideLen),
		C.float(boxScoreThresh), C.float(boxThresh), C.float(unClipRatio),
		C.bool(doAngle), C.bool(mostAngle),
	)
}
