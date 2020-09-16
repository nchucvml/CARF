#include "stdafx.h"
#include "VibeBooster.h"


CVibeBooster::CVibeBooster(void) : m_isTrained(false)
{
}


CVibeBooster::~CVibeBooster(void)
{
}


void CVibeBooster::AlgorithmImpl(const cv::Mat &srcFrame, cv::Mat &foregroundMask)
{
	cv::Mat srcGray;

	cvtColor(srcFrame, srcGray, CV_RGB2GRAY);

	if(!m_isTrained)
	{
		m_vibe.init(srcGray);
		m_vibe.processFirstFrame(srcGray);

		m_isTrained = true;
	}
	else
	{
		m_vibe.testAndUpdate(srcGray);
		foregroundMask = m_vibe.getMask();
	}
}
