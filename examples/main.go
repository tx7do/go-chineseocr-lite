package main

import (
	ocrlite "github.com/tx7do/go-chineseocr-lite"
	"log"
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

	pred := ocrlite.New(true, false, true)

	pred.SetNumThread(numThread)

	pred.InitModels(
		"../models/dbnet.onnx",
		"../models/angle_net.onnx",
		"../models/crnn_lite_lstm.onnx",
		"../models/keys.txt",
	)

	result := pred.Detect("_fixtures/", "2.jpg",
		padding, maxSideLen,
		boxScoreThresh, boxThresh, unClipRatio,
		doAngle, mostAngle,
	)
	log.Println(result)
}
