/* 
 * CARF_Booster.cpp : The The implementation of CARFBooster.h.
 * 
 * Chun-Rong Huang, Wei-Yun Huang, Yi-Sheng Liao, Chien-Cheng Lee, and Yu-Wei Yeh,           
 * "A  Content-Adaptive Resizing Framework for Boosting Computation Speed of Background      
 * Modeling Methods," IEEE Transactions on Systems, Man, and Cybernetics: Systems, 2020.
 *
 * Copyright © 2020 Computer Vision and Multimedia Lab (CVML), National Chung Hsing University. 
 * All rights reserved.
 */

#include "stdafx.h"
#include "CARFBooster.h"


CCARFBooster::CCARFBooster(void)
	: m_dspSize(cv::Size(320, 240)), m_dspAlgo(DA_CARF)
{
}


CCARFBooster::~CCARFBooster(void)
{
}


void CCARFBooster::Downsample(const cv::Mat &srcImg, cv::Mat &resultImg)
{
	m_imgSize = srcImg.size();

	switch (m_dspAlgo)
	{
	case DA_ORI:
		resultImg = srcImg;
		break;

	case DA_CARF:
		m_carf.Downsample(srcImg, cv::Mat(), m_dspSize);
		m_carf.ConstructDspImage(resultImg);
		break;

	case DA_NN:
		cv::resize(srcImg, resultImg, m_dspSize, 0, 0, cv::INTER_NEAREST);
		break;

	case DA_BILINEAR:
		cv::resize(srcImg, resultImg, m_dspSize, 0, 0, cv::INTER_LINEAR);
		break;

	case DA_BICUBIC:
		cv::resize(srcImg, resultImg, m_dspSize, 0, 0, cv::INTER_CUBIC);
		break;

	default:
		break;
	}
}


void CCARFBooster::Upsample(const cv::Mat &srcImg, cv::Mat &resultImg)
{
	switch (m_dspAlgo)
	{
	case DA_ORI:
		resultImg = srcImg;
		break;

	case DA_CARF:
		m_carf.UspMapping(srcImg, resultImg);
		break;

	case DA_NN:
		cv::resize(srcImg, resultImg, m_imgSize, 0, 0, cv::INTER_NEAREST);
		break;

	case DA_BILINEAR:
		cv::resize(srcImg, resultImg, m_imgSize, 0, 0, cv::INTER_LINEAR);
		break;

	case DA_BICUBIC:
		cv::resize(srcImg, resultImg, m_imgSize, 0, 0, cv::INTER_CUBIC);
		break;

	default:
		break;
	}
}


void CCARFBooster::PreProcess(const cv::Mat &srcImg, cv::Mat &resultImg)
{
	resultImg = srcImg;
}


void CCARFBooster::PostProcess(const cv::Mat &srcImg, cv::Mat &resultImg)
{
	resultImg = srcImg;
}


bool CCARFBooster::Boost(const cv::Mat &srcImg, cv::Mat &resultImg)
{
	if(srcImg.empty())
		return false;

	cv::Mat preprocImg, dspImg, dspResult, postprocImg;

	this->PreProcess(srcImg, preprocImg);
	this->Downsample(preprocImg, dspImg);

	this->AlgorithmImpl(dspImg, dspResult);
	
	if(!dspResult.empty()) { this->Upsample(dspResult, postprocImg); }
	this->PostProcess(postprocImg, resultImg);

	return true;
}
