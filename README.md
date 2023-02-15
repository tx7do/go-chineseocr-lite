# go-chineseocr-lite

文字识别过程：

图像输入 -> 预处理 -> 文本检测 -> 文本识别 -> 输出结果

1. DBNet（Differentiable Binarization Network）
2. Angle Network - 文字方向检测
3. CRNN-LSTM（Convolutional Recurrent Neural Network - Long Short Term Memory） - 文字识别
