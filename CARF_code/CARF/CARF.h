/*
 * CARF.h : Superpixel based image downsampling and upsampling. 
 *
 * Chun-Rong Huang, Wei-Yun Huang, Yi-Sheng Liao, Chien-Cheng Lee, and Yu-Wei Yeh,
 * "A  Content-Adaptive Resizing Framework for Boosting Computation Speed of Background
 * Modeling Methods," IEEE Transactions on Systems, Man, and Cybernetics: Systems, 2020.
 *
 * Copyright © 2020 Computer Vision and Multimedia Lab (CVML), National Chung Hsing University.
 * All rights reserved.
 */
 

#pragma once

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>


class CCARF
{
private:
	class CGrid
	{
	public:
		int x;
		int y;
		int width;
		int height;

		float meanVector[5];
		int size;

		CGrid() : meanVector() {}
		~CGrid() {}
	};

	class CRemain
	{
	public:
		bool remain;
		int idx;

		CRemain() : remain(false), idx(0) {}
		~CRemain() {}
	};
	
private:
	cv::Mat m_matData;
	cv::Mat m_labelData;

	int m_channels;

	int m_xNum;
	int m_yNum;

	CRemain *m_xRemain;
	CRemain *m_yRemain;

	int m_xPixelNum;
	int m_yPixelNum;

	CGrid *m_imageGrid;
	
public:
	CCARF(void);
	~CCARF(void);

private:
	// Speed up the grid construction using tbb
	class BuildGridParallel : public cv::ParallelLoopBody
	{
	private:
		CCARF *carfPtr;

	public:
		BuildGridParallel(CCARF *_carfPtr) 
			: carfPtr(_carfPtr) {}

		void operator() (const cv::Range& range) const;
	};

	// Speed up the pixel label assignment procedure using tbb
	class AssignLabelParallel : public cv::ParallelLoopBody
	{
	private:
		cv::Mat &labels;
		float &omega;

		CCARF *carfPtr;

	public:
		AssignLabelParallel(cv::Mat& _labels, float &_omega, CCARF *_carfPtr) 
			: labels(_labels), omega(_omega), carfPtr(_carfPtr) {}

		void operator() (const cv::Range& range) const;
	};

	// Speed up the upsampling procedure using tbb
	class UspMappingParallel : public cv::ParallelLoopBody
	{
	private:
		const cv::Mat &srcImage;
		cv::Mat &dstImage;

		int &step;

		CCARF *carfPtr;

	public:
		UspMappingParallel(const cv::Mat &_srcImage, cv::Mat &_dstImage, int &_step, CCARF *_carfPtr)
			: srcImage(_srcImage), dstImage(_dstImage), step(_step), carfPtr(_carfPtr) {}

		void operator() (const cv::Range& range) const;
	};

private:
	bool InitializeSp(const cv::Mat &matData, int xNum, int yNum);

	bool BuildImageGrid(int begin = -1, int end = -1);

	void AssignLabel(cv::Mat &labels, float omega, int begin = -1, int end = -1);

	bool Upsample(const cv::Mat &srcImage, cv::Mat &dstImage, int &step, int begin, int end) const;

public:
	// Construct the superpixel information for the input matData
	int Downsample(const cv::Mat &matData, cv::Mat &labels, cv::Size dspSize, float omega = 0.0f, bool tbbBoost = true);
	
	// Construct the downsampled image
	bool ConstructDspImage(cv::Mat &image) const;
	// Upsample srcImage using superpixel information
	bool UspMapping(const cv::Mat &srcImage, cv::Mat &dstImage, bool tbbBoost = true);
};

