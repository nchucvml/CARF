/*
* CARF_Booster.cpp : The The implementation of CARFMultiBooster.h.
*
* Chun-Rong Huang, Wei-Yun Huang, Yi-Sheng Liao, Chien-Cheng Lee, and Yu-Wei Yeh,
* "A Content-Adaptive Resizing Framework for Boosting Computation Speed of Background
* Modeling Methods," IEEE Transactions on Systems, Man, and Cybernetics: Systems, 2020.
*
* Copyright © 2020 Computer Vision and Multimedia Lab (CVML), National Chung Hsing University.
* All rights reserved.
*/

#include "stdafx.h"
#include "CARFMultiBooster.h"


CCARFMultiBooster::CCARFMultiBooster(void)
{

}


CCARFMultiBooster::~CCARFMultiBooster(void)
{
}


bool CCARFMultiBooster::Boost(const std::vector<cv::Mat> &srcImgs, cv::Mat &resultImg)
{
	if (srcImgs.empty())
		return false;
	

}
