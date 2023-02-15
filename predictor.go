package ocrlite

/*
#include <stdlib.h>
#include "predictor.hpp"
*/
import "C"

type Predictor struct {
	ctx C.OCR_PredictorContext
}

func New() (*Predictor, error) {
	pred := &Predictor{}

	return pred, nil
}
