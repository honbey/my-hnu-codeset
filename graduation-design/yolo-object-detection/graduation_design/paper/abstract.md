## 摘要

计算机视觉已经越来越多的应用在实际生活中，本文针对当前日常生活中的口罩佩戴实现了基于 YOLO 模型的实时目标检测系统。目标检测经历了传统目标检测到基于深度学习目标检测的发展过程，YOLO 模型是优秀的深度学习目标检测模型。围绕 YOLO 模型，本文阐述了其实现，之后实现了采用预训练模型实现了实时的口罩佩戴检测系统，对日常生活的情况能够助准确地进行口罩佩戴检测而且有良好的检测速度。为提高目标检测的检测精度，同时针对 YOLO v3 模型的不足，本文研究了损失函数的优化，同时分析了不同的目标框损失的改进之处和优势，实验是将 IoU Loss 改为 GIoU Loss 等，采用同样的数据集与实验环境进行对比训练，对结果采用相同的测试集进行验证，实验结果表明相对于原模型准确度有了一定的提升，验证了 YOLO v4 模型的其中一个改进的效果。

**关键词：**YOLO, 目标检测, 深度学习, 交并比, 人脸检测

## Abstract

Computer vision has been more and more used in real life. In this paper, a real-time object detection system based on YOLO model is implemented for the current daily life of mask wearing. Target detection has experienced the development process from traditional target detection to target detection based on deep learning. YOLO model is an excellent model for deep learning target detection. Around the YOLO model, this paper describes its implementation, and then realizes the real-time mask wearing detection system by using the pre training model, which can help to accurately detect the mask wearing in daily life and has a good detection speed. In order to improve the detection accuracy of target detection, and aiming at the shortcomings of YOLO v3 model, this paper studies the optimization of loss function, and analyzes the improvement and advantages of different target box loss. The experiment is to change IOU loss to giou loss, and so on. The same data set is used for comparative training with the experimental environment, and the results are verified by the same test set, The experimental results show that compared with the original model, the accuracy has been improved to a certain extent, which verifies one of the improved effects of YOLO v4 model.

**Keyword:**YOLO, Object Detection, Deep Learning, IoU, Face Detection
