
#include "type.hpp"
#include <string.h>

// Mat_New creates a new empty Mat
Mat Mat_New() {
    return new cv::Mat();
}

// Mat_Close deletes an existing Mat
void Mat_Close(Mat m) {
    delete m;
}

// Mat_Empty tests if a Mat is empty
int Mat_Empty(Mat m) {
    return m->empty();
}
