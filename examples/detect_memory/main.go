package main

import (
	ocrlite "github.com/tx7do/go-chineseocr-lite"
	"log"
	"os"
)

func init() {
}

func main() {
	var numThread = 4

	var padding = 50
	var maxSideLen = 1024
	var boxScoreThresh float32 = 0.618
	var boxThresh float32 = 0.300
	var unClipRatio float32 = 2.6
	var doAngle = true
	var mostAngle = true

	var modelPath = "../../models/"

	pred := ocrlite.New(true, false, true)

	pred.SetNumThread(numThread)

	pred.InitModels(
		modelPath+"dbnet.onnx",
		modelPath+"angle_net.onnx",
		modelPath+"crnn_lite_lstm.onnx",
		modelPath+"keys.txt",
	)

	f, err := os.ReadFile("../_fixtures/2.jpg")
	if err != nil {
		log.Fatalf("load file failed: %s", err.Error())
		return
	}

	result := pred.DetectMemoryImage(f,
		padding, maxSideLen,
		boxScoreThresh, boxThresh, unClipRatio,
		doAngle, mostAngle,
	)
	log.Println(result)
}
