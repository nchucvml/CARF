/*
 * ReadMe.txt : The document for CARF
 * 
 * Chun-Rong Huang, Wei-Yun Huang, Yi-Sheng Liao, Chien-Cheng Lee, and Yu-Wei Yeh,           
 * "A  Content-Adaptive Resizing Framework for Boosting Computation Speed of Background      
 * Modeling Methods," IEEE Transactions on Systems, Man, and Cybernetics: Systems, 2020.
 *
 * Copyright © 2020 Computer Vision and Multimedia Lab (CVML), National Chung Hsing University. 
 * All rights reserved.
 */

======================================================================================================================================================================================================================================================================================

// Superpixel based Image Downsampling

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

======================================================================================================================================================================================================================================================================================

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

======================================================================================================================================================================================================================================================================================

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

======================================================================================================================================================================================================================================================================================
