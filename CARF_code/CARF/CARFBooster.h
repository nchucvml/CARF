/*
 * CARF_Booster.h : The base class of the boosting framwork. 
 * 
 * Chun-Rong Huang, Wei-Yun Huang, Yi-Sheng Liao, Chien-Cheng Lee, and Yu-Wei Yeh,
 * "A  Content-Adaptive Resizing Framework for Boosting Computation Speed of Background
 * Modeling Methods," IEEE Transactions on Systems, Man, and Cybernetics: Systems, 2020.
 *
 * Copyright © 2020 Computer Vision and Multimedia Lab (CVML), National Chung Hsing University.
 * All rights reserved.
 */

#pragma once

#include "CARF.h"
#include <opencv2\highgui\highgui.hpp>


enum DSPALGO { DA_ORI, DA_CARF, DA_NN, DA_BILINEAR, DA_BICUBIC, DA_NUM };
const std::string daNames[DA_NUM] = { "ORI", "CARF", "NN", "BILINEAR", "BICUBIC" };


class CCARFBooster
{
private:
	CCARF m_carf;

	cv::Size m_imgSize;
	cv::Size m_dspSize;

	DSPALGO m_dspAlgo;

public:
	CCARFBooster(void);
	~CCARFBooster(void);

public:
	// Set the size for downsampled images
	inline void SetDspSize(cv::Size dspSize) { m_dspSize = dspSize; }
	// Set the algorithm for image resizing
	inline void SetDspAlgo(DSPALGO dspAlgo) { m_dspAlgo = dspAlgo; }

protected:
	void Downsample(const cv::Mat &srcImg, cv::Mat &resultImg);
	void Upsample(const cv::Mat &srcImg, cv::Mat &resultImg);

protected:
	// Please override the function if the algorithm need some preprocessing procedure before image downsampling
	virtual void PreProcess(const cv::Mat &srcImg, cv::Mat &resultImg);
	// Please override the function if the algorithm need some preprocessing procedure after image upsampling
	virtual void PostProcess(const cv::Mat &srcImg, cv::Mat &resultImg);

protected:
	// Please implement the algorithm at derived classes
    virtual void AlgorithmImpl(const cv::Mat &srcImg, cv::Mat &resultImg) = 0;

public:
	// Execute algorithm with the boosting framework
	bool Boost(const cv::Mat &srcImg, cv::Mat &resultImg);
};

