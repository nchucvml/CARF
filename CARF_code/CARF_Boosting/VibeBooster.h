#pragma once

#include "..\CARF\CARFBooster.h"
#include "Vibe.h"


class CVibeBooster : public CCARFBooster
{
private:
	ViBe_BGS m_vibe;
	bool m_isTrained;

public:
	CVibeBooster(void);
	~CVibeBooster(void);

protected:
    virtual void AlgorithmImpl(const cv::Mat &srcFrame, cv::Mat &foregroundMask);
};

