/*
 * CARF_MultiBooster.h : The base class of the boosting framwork. 
 * 
* Chun-Rong Huang, Wei-Yun Huang, Yi-Sheng Liao, Chien-Cheng Lee, and Yu-Wei Yeh,
* "A Content-Adaptive Resizing Framework for Boosting Computation Speed of Background
* Modeling Methods," IEEE Transactions on Systems, Man, and Cybernetics: Systems, 2020.
*
* Copyright © 2020 Computer Vision and Multimedia Lab (CVML), National Chung Hsing University.
* All rights reserved.
 */

#pragma once

#include "CARFBooster.h"
#include <opencv2\highgui\highgui.hpp>


class CCARFMultiBooster :
	public CCARFBooster
{
public:
	CCARFMultiBooster(void);
	~CCARFMultiBooster(void);

protected:
	virtual void AlgorithmImpl(const cv::Mat &srcImg, cv::Mat &resultImg) {}
	// Please implement the algorithm at derived classes
	virtual void AlgorithmImpl(const std::vector<cv::Mat> &srcImgs, cv::Mat &resultImg) = 0;

public:
	// Execute algorithm with the boosting framework
	bool Boost(const std::vector<cv::Mat> &srcImgs, cv::Mat &resultImg);
};

