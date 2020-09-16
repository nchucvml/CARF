#pragma once

#include "..\CARF\CARFBooster.h"

#include "SubSense\BackgroundSubtractorSuBSENSE.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>


class CSuBSENSEBooster :
	public CCARFBooster
{
private:
	BackgroundSubtractorSuBSENSE m_oBGSAlg;
	cv::Mat m_oCurrSegmMask;
	cv::Mat m_oCurrReconstrBGImg;

	int m_k;

	bool m_isTrained;

public:
	CSuBSENSEBooster();
	~CSuBSENSEBooster();

protected:
	virtual void AlgorithmImpl(const cv::Mat &srcFrame, cv::Mat &foregroundMask);
};

