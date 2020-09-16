#include "stdafx.h"
#include "GMMBoooster.h"


CGMMBoooster::CGMMBoooster()
{
	m_gmm = new cv::BackgroundSubtractorMOG();
}


CGMMBoooster::~CGMMBoooster()
{
	m_gmm.release();
}


void CGMMBoooster::AlgorithmImpl(const cv::Mat &srcFrame, cv::Mat &foregroundMask)
{
	(*m_gmm)(srcFrame, foregroundMask, 0.001);
}
