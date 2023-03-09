package main

import (
	ocrlite "github.com/tx7do/go-chineseocr-lite"
	"log"
)

func init() {
}

func NewPredictor() *ocrlite.Predictor {
	var numThread = 4
	var modelPath = "../../models/"

	pred := ocrlite.New(true, false, true)

	pred.SetNumThread(numThread)

	pred.InitModels(
		modelPath+"dbnet.onnx",
		modelPath+"angle_net.onnx",
		modelPath+"crnn_lite_lstm.onnx",
		modelPath+"keys.txt",
	)

	return pred
}

func doMultiThread() {
	var padding = 5
	var maxSideLen = 2046
	var boxScoreThresh float32 = 0.618
	var boxThresh float32 = 0.350
	var unClipRatio float32 = 2.6
	const doAngle = true
	const mostAngle = true

	chs := make([]chan int, 3)

	for i := 0; i < 3; i++ {
		chs[i] = make(chan int)
	}

	go func() {

		pred := NewPredictor()

		result := pred.DetectFileImage("../_fixtures/", "2.jpg",
			padding, maxSideLen,
			boxScoreThresh, boxThresh, unClipRatio,
			doAngle, mostAngle,
		)
		log.Println("[1] Final Result is: ", result)

		chs[0] <- 1
	}()

	go func() {
		pred := NewPredictor()

		result := pred.DetectFileImage("../_fixtures/", "3.jpg",
			padding, maxSideLen,
			boxScoreThresh, boxThresh, unClipRatio,
			doAngle, mostAngle,
		)
		log.Println("[2] Final Result is: ", result)

		chs[1] <- 1
	}()

	go func() {
		pred := NewPredictor()

		result := pred.DetectFileImage("../_fixtures/", "4.jpg",
			padding, maxSideLen,
			boxScoreThresh, boxThresh, unClipRatio,
			doAngle, mostAngle,
		)
		log.Println("[3] Final Result is: ", result)

		chs[2] <- 1
	}()

	for _, ch := range chs {
		<-ch
	}
}

func doSingleThread() {
	var padding = 5
	var maxSideLen = 2046
	var boxScoreThresh float32 = 0.618
	var boxThresh float32 = 0.350
	var unClipRatio float32 = 2.6
	const doAngle = true
	const mostAngle = true

	pred := NewPredictor()

	result := pred.DetectFileImage("../_fixtures/", "2.jpg",
		padding, maxSideLen,
		boxScoreThresh, boxThresh, unClipRatio,
		doAngle, mostAngle,
	)
	log.Println("[1] Final Result is: ", result)
}

func main() {
	doSingleThread()
}
