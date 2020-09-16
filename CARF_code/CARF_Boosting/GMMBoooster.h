#pragma once

#include "..\CARF\CARFBooster.h"

#include <opencv2\video\background_segm.hpp>


class CGMMBoooster :
	public CCARFBooster
{
private:
	cv::Ptr<cv::BackgroundSubtractor> m_gmm;

public:
	CGMMBoooster();
	~CGMMBoooster();

protected:
	virtual void AlgorithmImpl(const cv::Mat &srcFrame, cv::Mat &foregroundMask);
};

