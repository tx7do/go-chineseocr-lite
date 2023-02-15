package ocrlite

/*
#include <stdlib.h>
#include "type.hpp"
*/
import "C"
import "errors"

const (
	// MatChannels1 is a single channel Mat.
	MatChannels1 = 0

	// MatChannels2 is 2 channel Mat.
	MatChannels2 = 8

	// MatChannels3 is 3 channel Mat.
	MatChannels3 = 16

	// MatChannels4 is 4 channel Mat.
	MatChannels4 = 24
)

// MatType is the type for the various different kinds of Mat you can create.
type MatType int

const (
	// MatTypeCV8U is a Mat of 8-bit unsigned int
	MatTypeCV8U MatType = 0

	// MatTypeCV8S is a Mat of 8-bit signed int
	MatTypeCV8S MatType = 1

	// MatTypeCV16U is a Mat of 16-bit unsigned int
	MatTypeCV16U MatType = 2

	// MatTypeCV16S is a Mat of 16-bit signed int
	MatTypeCV16S MatType = 3

	// MatTypeCV16SC2 is a Mat of 16-bit signed int with 2 channels
	MatTypeCV16SC2 = MatTypeCV16S + MatChannels2

	// MatTypeCV32S is a Mat of 32-bit signed int
	MatTypeCV32S MatType = 4

	// MatTypeCV32F is a Mat of 32-bit float
	MatTypeCV32F MatType = 5

	// MatTypeCV64F is a Mat of 64-bit float
	MatTypeCV64F MatType = 6

	// MatTypeCV8UC1 is a Mat of 8-bit unsigned int with a single channel
	MatTypeCV8UC1 = MatTypeCV8U + MatChannels1

	// MatTypeCV8UC2 is a Mat of 8-bit unsigned int with 2 channels
	MatTypeCV8UC2 = MatTypeCV8U + MatChannels2

	// MatTypeCV8UC3 is a Mat of 8-bit unsigned int with 3 channels
	MatTypeCV8UC3 = MatTypeCV8U + MatChannels3

	// MatTypeCV8UC4 is a Mat of 8-bit unsigned int with 4 channels
	MatTypeCV8UC4 = MatTypeCV8U + MatChannels4

	// MatTypeCV8SC1 is a Mat of 8-bit signed int with a single channel
	MatTypeCV8SC1 = MatTypeCV8S + MatChannels1

	// MatTypeCV8SC2 is a Mat of 8-bit signed int with 2 channels
	MatTypeCV8SC2 = MatTypeCV8S + MatChannels2

	// MatTypeCV8SC3 is a Mat of 8-bit signed int with 3 channels
	MatTypeCV8SC3 = MatTypeCV8S + MatChannels3

	// MatTypeCV8SC4 is a Mat of 8-bit signed int with 4 channels
	MatTypeCV8SC4 = MatTypeCV8S + MatChannels4

	// MatTypeCV16UC1 is a Mat of 16-bit unsigned int with a single channel
	MatTypeCV16UC1 = MatTypeCV16U + MatChannels1

	// MatTypeCV16UC2 is a Mat of 16-bit unsigned int with 2 channels
	MatTypeCV16UC2 = MatTypeCV16U + MatChannels2

	// MatTypeCV16UC3 is a Mat of 16-bit unsigned int with 3 channels
	MatTypeCV16UC3 = MatTypeCV16U + MatChannels3

	// MatTypeCV16UC4 is a Mat of 16-bit unsigned int with 4 channels
	MatTypeCV16UC4 = MatTypeCV16U + MatChannels4

	// MatTypeCV16SC1 is a Mat of 16-bit signed int with a single channel
	MatTypeCV16SC1 = MatTypeCV16S + MatChannels1

	// MatTypeCV16SC3 is a Mat of 16-bit signed int with 3 channels
	MatTypeCV16SC3 = MatTypeCV16S + MatChannels3

	// MatTypeCV16SC4 is a Mat of 16-bit signed int with 4 channels
	MatTypeCV16SC4 = MatTypeCV16S + MatChannels4

	// MatTypeCV32SC1 is a Mat of 32-bit signed int with a single channel
	MatTypeCV32SC1 = MatTypeCV32S + MatChannels1

	// MatTypeCV32SC2 is a Mat of 32-bit signed int with 2 channels
	MatTypeCV32SC2 = MatTypeCV32S + MatChannels2

	// MatTypeCV32SC3 is a Mat of 32-bit signed int with 3 channels
	MatTypeCV32SC3 = MatTypeCV32S + MatChannels3

	// MatTypeCV32SC4 is a Mat of 32-bit signed int with 4 channels
	MatTypeCV32SC4 = MatTypeCV32S + MatChannels4

	// MatTypeCV32FC1 is a Mat of 32-bit float int with a single channel
	MatTypeCV32FC1 = MatTypeCV32F + MatChannels1

	// MatTypeCV32FC2 is a Mat of 32-bit float int with 2 channels
	MatTypeCV32FC2 = MatTypeCV32F + MatChannels2

	// MatTypeCV32FC3 is a Mat of 32-bit float int with 3 channels
	MatTypeCV32FC3 = MatTypeCV32F + MatChannels3

	// MatTypeCV32FC4 is a Mat of 32-bit float int with 4 channels
	MatTypeCV32FC4 = MatTypeCV32F + MatChannels4

	// MatTypeCV64FC1 is a Mat of 64-bit float int with a single channel
	MatTypeCV64FC1 = MatTypeCV64F + MatChannels1

	// MatTypeCV64FC2 is a Mat of 64-bit float int with 2 channels
	MatTypeCV64FC2 = MatTypeCV64F + MatChannels2

	// MatTypeCV64FC3 is a Mat of 64-bit float int with 3 channels
	MatTypeCV64FC3 = MatTypeCV64F + MatChannels3

	// MatTypeCV64FC4 is a Mat of 64-bit float int with 4 channels
	MatTypeCV64FC4 = MatTypeCV64F + MatChannels4
)

// CompareType is used for Compare operations to indicate which kind of
// comparison to use.
type CompareType int

const (
	// CompareEQ src1 is equal to src2.
	CompareEQ CompareType = 0

	// CompareGT src1 is greater than src2.
	CompareGT CompareType = 1

	// CompareGE src1 is greater than or equal to src2.
	CompareGE CompareType = 2

	// CompareLT src1 is less than src2.
	CompareLT CompareType = 3

	// CompareLE src1 is less than or equal to src2.
	CompareLE CompareType = 4

	// CompareNE src1 is unequal to src2.
	CompareNE CompareType = 5
)

type Point2f struct {
	X float32
	Y float32
}

func NewPoint2f(x, y float32) Point2f {
	return Point2f{x, y}
}

var ErrEmptyByteSlice = errors.New("empty byte array")

// Mat represents an n-dimensional dense numerical single-channel
// or multi-channel array. It can be used to store real or complex-valued
// vectors and matrices, grayscale or color images, voxel volumes,
// vector fields, point clouds, tensors, and histograms.
//
// For further details, please see:
// http://docs.opencv.org/master/d3/d63/classcv_1_1Mat.html
type Mat struct {
	p C.Mat

	// Non-nil if Mat was created with a []byte (using NewMatFromBytes()). Nil otherwise.
	d []byte
}

// NewMat returns a new empty Mat.
func NewMat() Mat {
	return newMat(C.Mat_New())
}

// newMat returns a new Mat from a C Mat
func newMat(p C.Mat) Mat {
	return Mat{p: p}
}

// Close the Mat object.
func (m *Mat) Close() error {
	C.Mat_Close(m.p)
	m.p = nil
	m.d = nil
	return nil
}

// Empty determines if the Mat is empty or not.
func (m *Mat) Empty() bool {
	isEmpty := C.Mat_Empty(m.p)
	return isEmpty != 0
}
