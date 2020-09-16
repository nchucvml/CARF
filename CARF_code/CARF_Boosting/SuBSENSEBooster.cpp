#include "stdafx.h"
#include "SuBSENSEBooster.h"


CSuBSENSEBooster::CSuBSENSEBooster()
	: m_isTrained(false), m_k(0)
{
}


CSuBSENSEBooster::~CSuBSENSEBooster()
{
}


void CSuBSENSEBooster::AlgorithmImpl(const cv::Mat &srcFrame, cv::Mat &foregroundMask)
{
	if (!m_isTrained)
	{
		m_oCurrSegmMask.create(srcFrame.size(), CV_8UC1);
		m_oCurrReconstrBGImg.create(srcFrame.size(), srcFrame.type());
		cv::Mat oSequenceROI(srcFrame.size(), CV_8UC1, cv::Scalar_<uchar>(255)); // for optimal results, pass a constrained ROI to the algorithm (ex: for CDnet, use ROI.bmp)
		
		m_oBGSAlg.initialize(srcFrame, oSequenceROI);

		m_isTrained = true;
	}
	else
	{
		m_oBGSAlg(srcFrame, m_oCurrSegmMask, double(m_k <= 100)); // lower rate in the early frames helps bootstrap the model when foreground is present
		m_oBGSAlg.getBackgroundImage(m_oCurrReconstrBGImg);
		m_k++;

		foregroundMask = m_oCurrSegmMask;
	}
}
