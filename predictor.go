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

func New(isOutputConsole, isOutputPartImg, isOutputResultImg bool) *Predictor {
	pred := &Predictor{
		ctx: C.OCR_NewPredictor(C.bool(isOutputConsole), C.bool(isOutputPartImg), C.bool(isOutputResultImg)),
	}
	return pred
}

func (p *Predictor) Close() {
	C.OCR_DeletePredictor(p.ctx)
	p.ctx = nil
}

// SetNumThread 设置工作线程数
func (p *Predictor) SetNumThread(numOfThread int) {
	C.OCR_PredictorSetNumThread(p.ctx, C.int(numOfThread))
}

// InitModels 初始化模型
//
//	detPath dbNet模型文件名(含扩展名)
//	clsPath angleNet模型文件名(含扩展名)
//	recPath crnnNet模型文件名(含扩展名)
//	keysPath keys.txt文件名(含扩展名)
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

// DetectFileImage 识别文件图片
//
//	imgPath 目标图片路径，可以相对路径也可以绝对路径。
//	imgName 图片的文件名
//	padding 图像预处理，在图片外周添加白边，用于提升识别率，文字框没有正确框住所有文字时，增加此值。
//	maxSideLen 按图片最长边的长度，此值为0代表不缩放，例：1024，如果图片长边大于1024则把图像整体缩小到1024再进行图像分割计算，如果图片长边小于1024则不缩放，如果图片长边小于32，则缩放到32。
//	boxScoreThresh 文字框置信度阈值，文字框没有正确框住所有文字时，减小此值。
//	boxThresh
//	unClipRatio 单个文字框大小倍率，越大时单个文字框越大。此项与图片的大小相关，越大的图片此值应该越大。
//	doAngle 启用文字方向检测，只有图片倒置的情况下(旋转90~270度的图片)，才需要启用文字方向检测。
//	mostAngle
func (p *Predictor) DetectFileImage(imgDir, imgName string, padding, maxSideLen int, boxScoreThresh, boxThresh, unClipRatio float32, doAngle, mostAngle bool) string {
	cImgDir := C.CString(imgDir)
	defer C.free(unsafe.Pointer(cImgDir))

	cImgFile := C.CString(imgName)
	defer C.free(unsafe.Pointer(cImgFile))

	result := C.OCR_PredictorDetectFileImage(p.ctx,
		cImgDir, cImgFile,
		C.int(padding), C.int(maxSideLen),
		C.float(boxScoreThresh), C.float(boxThresh), C.float(unClipRatio),
		C.bool(doAngle), C.bool(mostAngle),
	)

	strResult := C.OCR_ResultGetString(result)
	defer C.OCR_DeleteResult(result)

	goResult := C.GoString(strResult)

	return string(goResult)
}

// DetectMemoryImage 识别内存图片
func (p *Predictor) DetectMemoryImage(imgBuffer []byte, padding, maxSideLen int, boxScoreThresh, boxThresh, unClipRatio float32, doAngle, mostAngle bool) string {
	cImageBuffer := C.CString(string(imgBuffer))
	defer C.free(unsafe.Pointer(cImageBuffer))

	result := C.OCR_PredictorDetectMemoryImage(p.ctx,
		cImageBuffer, C.int(len(imgBuffer)),
		C.int(padding), C.int(maxSideLen),
		C.float(boxScoreThresh), C.float(boxThresh), C.float(unClipRatio),
		C.bool(doAngle), C.bool(mostAngle),
	)
	defer C.OCR_DeleteResult(result)

	return ""

	strResult := C.OCR_ResultGetString(result)
	goResult := C.GoString(strResult)
	return string(goResult)
}
