# CARF
## A Content-Adaptive Resizing Framework for Boosting Computation Speed of Background Modeling Methods

* The source code will be available soon.

Abstract—Recently, most background modeling (BM) methods 
claim to achieve real-time efficiency for low-resolution and
standard-definition surveillance videos. With the increasing resolutions
of surveillance cameras, full high-definition (full HD)
surveillance videos have become the main trend and thus processing
high-resolution videos becomes a novel issue in intelligent
video surveillance. In this paper, we propose a novel contentadaptive
resizing framework (CARF) to boost the computation
speed of BM methods in high-resolution surveillance videos. For
each frame, we apply superpixels to separate the content of the
frame to homogeneous and boundary sets. Two novel downsampling
and upsampling layers based on the homogeneous and
boundary sets are proposed. The front one downsamples highresolution
frames to low-resolution frames for obtaining efficient
foreground segmentation results based on BM methods. The
later one upsamples the low-resolution foreground segmentation
results to the original resolution frames based on the superpixels.
By simultaneously coupling both layers, experimental results
show that the proposed method can achieve better quantitative
and qualitative results compared with state-of-the-art methods.
Moreover, the computation speed of the proposed method without
GPU accelerations is also significantly faster than that of the
state-of-the-art methods.

* Flowchart of CARF

![image](https://github.com/nchucvml/CARF/blob/master/flowchart.png)

* If you use the CARF codes, please cite the following paper.
Chun-Rong Huang, Wei-Yun Huang, Yi-Sheng Liao, Chien-Cheng Lee, and Yu-Wei Yeh, "A Content-Adaptive Resizing Framework for
Boosting Computation Speed of Background Modeling Methods," accepted by IEEE Transactions on Systems, Man, and Cybernetics: Systems, 2020.

Paper Open Access in https://ieeexplore.ieee.org/document/9197636

* The demo videos are available at https://www.youtube.com/playlist?list=PLeFabaAzO2xwAr_Ya9ui8hWEtFpAieTYR


* ReadMe.txt : The document for CARF

==========  Include file ==========

#include "CARF.h"

========== Initialization ==========

CCARF carf;

========== Resize image ==========

// Construct the superpixel information for the input matData
carf.Downsample(const cv::Mat &matData, cv::Mat &labels, cv::Size dspSize, float omega = 0.0f, bool tbbBoost = true);

// Construct the downsampled image
carf.ConstructDspImage(cv::Mat &image) const;

// Upsample the srcImage using superpixel information
carf.UspMapping(const cv::Mat &srcImage, cv::Mat &dstImage, bool tbbBoost = true);

=====================================================================================

// The boosting framework for boundary segmentation in High-Resolution Images

========== Include file ==========

#include "CARFBooster.h"

========== To apply algorithm to the boosting framework ==========

1. Inherit the "CCARFBooster"

2. virtual void AlgorithmImpl(const cv::Mat &srcImg, cv::Mat &resultImg);
// Implement the algorithm at the virtual function "AlgorithmImpl."

3. virtual void PreProcess(const cv::Mat &srcImg, cv::Mat &resultImg);
// Override the function if the algorithm need some preprocessing procedure before image downsampling

4. virtual void PostProcess(const cv::Mat &srcImg, cv::Mat &resultImg);
// Override the function if the algorithm need some preprocessing procedure after image upsampling

=====================================================================================

// To use the boosted algorithm

========== Include file ========== 
#include "DeriveClassName.h"

========== Initialization ========== 

CDeriveClassName deriveClass;

// Set the size for downsampled image
deriveClass.SetDspSize(cv::Size dspSize); 

// Set the algorithm for image resizing
deriveClass.SetDspAlgo(DSPALGO dspAlgo); 

========== DSPALGO ==========

DA_ORI: Without image resizing
DA_CARF: Superpixel based Image Downsampling
DA_NN: Nearest neighbor interpolation
DA_BILINEAR: Bilinear interpolation
DA_BICUBIC: Bicubic interpolation

========== Run algorithm ==========

// Execute algorithm with the boosting framework
deriveClass.Boost(const cv::Mat &srcImg, cv::Mat &resultImg);


* Used background modeling methods

GMM method is originally from opencv
Vibe method is originally from https://blog.csdn.net/zouxy09/article/details/9622285
SubSense method is originally from https://bitbucket.org/pierre_luc_st_charles/subsense/src/master/
