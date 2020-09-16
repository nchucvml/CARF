/*
 * CARF.cpp : The implementation of CARF.h. 
 * 
 * Chun-Rong Huang, Wei-Yun Huang, Yi-Sheng Liao, Chien-Cheng Lee, and Yu-Wei Yeh,           
 * "A  Content-Adaptive Resizing Framework for Boosting Computation Speed of Background      
 * Modeling Methods," IEEE Transactions on Systems, Man, and Cybernetics: Systems, 2020.
 *
 * Copyright © 2020 Computer Vision and Multimedia Lab (CVML), National Chung Hsing University. 
 * All rights reserved.
 */

#include "stdafx.h"
#include "CARF.h"


void CCARF::BuildGridParallel::operator() (const cv::Range& range) const
{
	carfPtr->BuildImageGrid(range.start, range.end);
}


void CCARF::AssignLabelParallel::operator() (const cv::Range& range) const
{
	carfPtr->AssignLabel(labels, omega, range.start, range.end);
}


void CCARF::UspMappingParallel::operator() (const cv::Range& range) const
{
	carfPtr->Upsample(srcImage, dstImage, step, range.start, range.end);
}


CCARF::CCARF(void)
	: m_xNum(1), m_yNum(1), m_xRemain(nullptr), m_yRemain(nullptr), m_imageGrid(nullptr)
{
}


CCARF::~CCARF(void)
{
	if(m_imageGrid != nullptr) { delete []m_imageGrid; m_imageGrid = nullptr; }

	if(m_xRemain != nullptr) { delete []m_xRemain; m_xRemain = nullptr; }
	if(m_yRemain != nullptr) { delete []m_yRemain; m_yRemain = nullptr; }
}


bool CCARF::InitializeSp(const cv::Mat &matData, int xNum, int yNum)
{
	m_channels = matData.channels();

	if(m_channels == 3)
		m_matData = matData;
	else if(m_channels == 1)
		cv::cvtColor(matData, m_matData, CV_GRAY2BGR);
	else
		return false;

	if(!m_labelData.empty()) { m_labelData.release(); }

	if(m_imageGrid != nullptr) { delete []m_imageGrid; m_imageGrid = nullptr; }
	
	m_xNum = ((xNum > 0 && xNum <= m_matData.cols) ? xNum : m_xNum);
	m_yNum = ((yNum > 0 && yNum <= m_matData.rows) ? yNum : m_yNum);

	if(m_xRemain != nullptr) { delete []m_xRemain; m_xRemain = nullptr; } m_xRemain = new CRemain[m_xNum]();
	if(m_yRemain != nullptr) { delete []m_yRemain; m_yRemain = nullptr; } m_yRemain = new CRemain[m_yNum]();

	int xRemainNum = m_matData.cols % m_xNum;
	int yRemainNum = m_matData.rows % m_yNum;
	
	if(xRemainNum != 0)
	{
		float xRemainStep = (float)m_xNum / (float)xRemainNum;

		int idx = 0;
		int currIdx = -1, nextIdx = 0;
		int endIdx = m_xNum - 1;

		do 
		{
			nextIdx = (int)(xRemainStep * (idx + 1) + 0.5) - 1;
			for(int i = currIdx + 1; i < nextIdx; i++)
				m_xRemain[i].idx = idx;

			currIdx = nextIdx;
			idx++;
			
			m_xRemain[currIdx].remain = true;
			m_xRemain[currIdx].idx = idx - 1;
		} 
		while(currIdx < endIdx);
	}

	if(yRemainNum != 0)
	{
		float yRemainStep = (float)m_yNum / (float)yRemainNum;

		int idx = 0;
		int currIdx = -1, nextIdx = 0;
		int endIdx = m_yNum - 1;
		
		do 
		{
			nextIdx = (int)(yRemainStep * (idx + 1) + 0.5) - 1;
			for(int i = currIdx + 1; i < nextIdx; i++) m_yRemain[i].idx = idx;

			currIdx = nextIdx;
			idx++;

			m_yRemain[currIdx].remain = true;
			m_yRemain[currIdx].idx = idx - 1;
		} 
		while(currIdx < endIdx);
	}
	
	m_imageGrid = new CGrid[m_yNum * m_xNum];
	
	return true;
}


bool CCARF::BuildImageGrid(int begin, int end)
{
	if(m_imageGrid == nullptr || m_matData.empty())
		return false;

	int xPixelNum = m_matData.cols / m_xNum;
	int yPixelNum = m_matData.rows / m_yNum;

	int xRemainNum = m_matData.cols % m_xNum;
	int yRemainNum = m_matData.rows % m_yNum;

	m_xPixelNum = (xRemainNum == 0 ? xPixelNum : xPixelNum + 1) << 1;
	m_yPixelNum = (yRemainNum == 0 ? yPixelNum : yPixelNum + 1) << 1;

	begin = (begin == -1 ? 0 : begin);
	end = (end == -1 ? m_yNum : end);

	for(int i = begin; i < end; i++)
	{
		CGrid *gridPtr = &m_imageGrid[i * m_xNum];

		for(int j = 0; j < m_xNum; j++)
		{
			gridPtr->x = xPixelNum * j + m_xRemain[j].idx;
			gridPtr->y = yPixelNum * i + m_yRemain[i].idx;
			
			gridPtr->width = (m_xRemain[j].remain ? xPixelNum + 1 : xPixelNum);
			gridPtr->height = (m_yRemain[i].remain ? yPixelNum + 1 : yPixelNum);

			gridPtr->size = gridPtr->width * gridPtr->height;

			float *meanVectorPtr = gridPtr->meanVector;
			uchar *pixelPtr = (uchar*)m_matData.data + (gridPtr->y * m_matData.cols + gridPtr->x) * m_matData.channels();

			for(int k = gridPtr->y; k < gridPtr->y + gridPtr->height; k++)
			{
				for(int l = gridPtr->x; l < gridPtr->x + gridPtr->width; l++)
				{
					meanVectorPtr[0] += ((float)l / (float)m_xPixelNum);
					meanVectorPtr[1] += ((float)k / (float)m_yPixelNum);
					meanVectorPtr[2] += (float)*pixelPtr++;
					meanVectorPtr[3] += (float)*pixelPtr++;
					meanVectorPtr[4] += (float)*pixelPtr++;
				}
				
				pixelPtr += (m_matData.cols - gridPtr->width) * m_matData.channels();
			}
			
			gridPtr->meanVector[0] /= (float)(gridPtr->size);
			gridPtr->meanVector[1] /= (float)(gridPtr->size);
			gridPtr->meanVector[2] /= ((float)(gridPtr->size) * 255.0f);
			gridPtr->meanVector[3] /= ((float)(gridPtr->size) * 255.0f);
			gridPtr->meanVector[4] /= ((float)(gridPtr->size) * 255.0f);
			
			gridPtr++;
		}
	}
	
	return true;
}


void CCARF::AssignLabel(cv::Mat &labels, float omega, int begin, int end)
{
	static const int neighborNum = 9;

	static const int neighborIdxX[neighborNum] = { 0, 1, -1, 0, 0, -1, -1, 1, 1 };
	static const int neighborIdxY[neighborNum] = { 0, 0, 0, 1, -1, -1, 1, -1, 1 };
	
	CGrid *gridPtr;

	begin = (begin == -1 ? 0 : begin);
	end = (end == -1 ? m_yNum : end);

	for(int i = begin; i < end; i++)
	{
		gridPtr = &m_imageGrid[i * m_xNum];
		
		for(int j = 0; j < m_xNum; j++)
		{
			uchar *pixelPtr = (uchar*)m_matData.data + (gridPtr->y * m_matData.cols + gridPtr->x) * m_matData.channels();
			int *labelPtr = (int*)labels.data + (gridPtr->y * m_matData.cols + gridPtr->x);
			
			int gridSize = gridPtr->width * gridPtr->height;
			
			for(int k = gridPtr->y; k < gridPtr->y + gridPtr->height; k++)
			{
				for(int l = gridPtr->x; l < gridPtr->x + gridPtr->width; l++)
				{
					float maxGravitation = -FLT_MAX;
					int maxNeighborIdx = -1;

					float pixel[3];
					pixel[0] = (float)*pixelPtr++ / 255.0f;
					pixel[1] = (float)*pixelPtr++ / 255.0f;
					pixel[2] = (float)*pixelPtr++ / 255.0f;

					float xValue = (float)l / m_xPixelNum;
					float yValue = (float)k / m_yPixelNum;

					for(int m = 0; m < neighborNum; m++)
					{
						int neighborX = j + neighborIdxX[m];
						int neighborY = i + neighborIdxY[m];

						if(neighborX >= 0 && neighborX < m_xNum && neighborY >= 0 && neighborY < m_yNum)
						{
							CGrid *neighborGridPtr = &m_imageGrid[neighborY * m_xNum + neighborX];
							float *neighborMeanPtr = neighborGridPtr->meanVector;
							
							float distanceTerm = pow((float)xValue - neighborMeanPtr[0], 2) + pow((float)yValue - neighborMeanPtr[1], 2);
							float colorTerm = pow((pixel[0] - neighborMeanPtr[2]), 2) + pow((pixel[1] - neighborMeanPtr[3]), 2) + pow((pixel[2] - neighborMeanPtr[4]), 2);
							
							float gravitation = (float)neighborGridPtr->size / (distanceTerm * omega + colorTerm * (1 - omega));
							
							if(maxGravitation < gravitation)
							{
								maxGravitation = gravitation;
								maxNeighborIdx = m;
							}
						}
					}

					int maxNeighborX = j + neighborIdxX[maxNeighborIdx];
					int maxNeighborY = i + neighborIdxY[maxNeighborIdx];

					*labelPtr++ = maxNeighborY * m_xNum + maxNeighborX;
				}
				
				pixelPtr += (m_matData.cols - gridPtr->width) * m_matData.channels();
				labelPtr += (labels.cols - gridPtr->width);
			}

			gridPtr++;
		}
	}
}


bool CCARF::Upsample(const cv::Mat &srcImage, cv::Mat &dstImage, int &step, int begin, int end) const
{
	uchar *srcPtr = srcImage.data;

	for(int i = begin; i < end; i++)
	{
		uchar *dstPtr = dstImage.data + dstImage.cols * step * i;
		int *labelPtr = (int*)m_labelData.data + dstImage.cols * i;

		for(int j = 0; j < dstImage.cols; j++)
		{
			int labelIdx = *labelPtr++ * step;

			for(int k = 0; k < step; k++)
			{
				*dstPtr++ = srcPtr[labelIdx++];
			}
		}
	}

	return true;
}


int CCARF::Downsample(const cv::Mat &matData, cv::Mat &labels, cv::Size dspSize, float omega, bool tbbBoost)
{
	if(this->InitializeSp(matData, dspSize.width, dspSize.height))
	{
		if(tbbBoost)
			cv::parallel_for_(cv::Range(0, m_yNum), BuildGridParallel(this));
		else
			this->BuildImageGrid();

		m_labelData.release();
		m_labelData = cv::Mat::zeros(m_matData.size(), CV_32S);
	
		if(tbbBoost)
			cv::parallel_for_(cv::Range(0, m_yNum), AssignLabelParallel(m_labelData, omega, this));
		else
			this->AssignLabel(m_labelData, omega);
	
		labels = m_labelData;

		return m_xNum * m_yNum;
	}
	else
	{
		return 0;
	}
}


bool CCARF::ConstructDspImage(cv::Mat &image) const
{
	if(m_imageGrid == nullptr)
		return false;

	image.release();

	if(m_channels == 3)
		image.create(m_yNum, m_xNum, CV_8UC3);
	else if(m_channels == 1)
		image.create(m_yNum, m_xNum, CV_8U);
	else
		return false;

	uchar *pixelPtr = (uchar*)image.data;

	CGrid *gridPtr = m_imageGrid;
	float *meanVectorPtr;

	if(m_channels == 3)
	{
		for(int i = 0; i < m_yNum; i++)
		{
			for(int j = 0; j < m_xNum; j++)
			{
				meanVectorPtr = gridPtr++->meanVector;

				*pixelPtr++ = (uchar)(meanVectorPtr[2] * 255.0f);
				*pixelPtr++ = (uchar)(meanVectorPtr[3] * 255.0f);
				*pixelPtr++ = (uchar)(meanVectorPtr[4] * 255.0f);
			}
		}
	}
	else if(m_channels == 1)
	{
		for(int i = 0; i < m_yNum; i++)
		{
			for(int j = 0; j < m_xNum; j++)
			{
				meanVectorPtr = gridPtr++->meanVector;

				*pixelPtr++ = (uchar)(meanVectorPtr[2] * 255.0f);
			}
		}
	}
	else
		return false;

	return true;
}


bool CCARF::UspMapping(const cv::Mat &srcImage, cv::Mat &dstImage, bool tbbBoost)
{
	if(srcImage.empty() || m_labelData.empty())
		return false;

	cv::Mat mappingImg;
	mappingImg.create(m_labelData.size(), srcImage.type());

	int step = pow(2, (srcImage.depth() >> 1)) * srcImage.channels();
	
	if(tbbBoost)
		cv::parallel_for_(cv::Range(0, mappingImg.rows), UspMappingParallel(srcImage, mappingImg, step, this));
	else
		this->Upsample(srcImage, mappingImg, step, 0, mappingImg.rows);

	dstImage = mappingImg;

	return true;
}
