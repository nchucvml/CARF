/*
 * CARF_Boosting.cpp : The implementation of CARF_Booster.h. 
 *
 * Chun-Rong Huang, Wei-Yun Huang, Yi-Sheng Liao, Chien-Cheng Lee, and Yu-Wei Yeh,
 * "A  Content-Adaptive Resizing Framework for Boosting Computation Speed of Background
 * Modeling Methods," IEEE Transactions on Systems, Man, and Cybernetics: Systems, 2020.
 *
 * Copyright © 2020 Computer Vision and Multimedia Lab (CVML), National Chung Hsing University.
 * All rights reserved.
 */
#include "stdafx.h"
#include <direct.h>
#include "GMMBoooster.h"
#include "VibeBooster.h"
#include "SuBSENSEBooster.h"
#include <Afxtempl.h>
#include <fstream>
#include <iomanip>

#ifdef DEBUG
#pragma comment(lib, "opencv_core2413d.lib")
#pragma comment(lib, "opencv_highgui2413d.lib")
#pragma comment(lib, "opencv_imgproc2413d.lib")
#pragma comment(lib, "opencv_features2d2413d.lib")
#pragma comment(lib, "opencv_calib3d2413d.lib")
#pragma comment(lib, "opencv_video2413d.lib")
#pragma comment(lib, "opencv_flann2413d.lib")
#pragma comment(lib, "opencv_nonfree2413d.lib")
#else
#pragma comment(lib, "opencv_core2413.lib")
#pragma comment(lib, "opencv_highgui2413.lib")
#pragma comment(lib, "opencv_imgproc2413.lib")
#pragma comment(lib, "opencv_features2d2413.lib")
#pragma comment(lib, "opencv_calib3d2413.lib")
#pragma comment(lib, "opencv_video2413.lib")
#pragma comment(lib, "opencv_flann2413.lib")
#pragma comment(lib, "opencv_nonfree2413.lib")
#endif


int _tmain(int argc, const char** argv)
{


	std::stringstream  thetempnumber;
	std::string datasetPath;
	std::string resultPath;

	const int danumber = 1;
	std::string datasets[danumber] = {"highway"};

	std::string dsTypes[danumber] = {".jpg"};

	int dsnumber[danumber] = {1700};

	float timeGMM2 = 0;
	float timeVIBE2 = 0;
	float timeSuBSENSE2 = 0;
	float timeGMM4 = 0;
	float timeVIBE4 = 0;
	float timeSuBSENSE4 = 0;
	
	for(int da = DA_CARF; da <= DA_CARF; da++)
		for (int ds = 0; ds < danumber; ds++)
		{
			{
				
				CGMMBoooster gmmbooster1, gmmbooster2;
				CVibeBooster vibebooster1, vibebooster2;
				CSuBSENSEBooster subsensebooster1, subsensebooster2;

				DSPALGO dspAlgo = (DSPALGO)da;

				gmmbooster1.SetDspAlgo(dspAlgo);
				vibebooster1.SetDspAlgo(dspAlgo);
				subsensebooster1.SetDspAlgo(dspAlgo);

				gmmbooster2.SetDspAlgo(dspAlgo);
				vibebooster2.SetDspAlgo(dspAlgo);
				subsensebooster2.SetDspAlgo(dspAlgo);

				datasetPath = ".\\Datasets\\" + datasets[ds];

				cv::Mat frame;
				cv::Mat frame2;
				cv::Mat frame4;
				std::stringstream frameIdStr;
				std::string tempcac;
				resultPath = ".\\Results\\";
				_mkdir(".\\Results");
				resultPath = resultPath + datasets[ds];
				_mkdir(resultPath.c_str());
				std::string GMM2 = resultPath + "\\GMM2";
				_mkdir(GMM2.c_str());
				std::string VIBE2 = resultPath + "\\VIBE2";
				_mkdir(VIBE2.c_str());
				std::string SuBSENSE2 = resultPath + "\\SuBSENSE2";
				_mkdir(SuBSENSE2.c_str());
				std::string GMM4 = resultPath + "\\GMM4";
				_mkdir(GMM4.c_str());
				std::string VIBE4 = resultPath + "\\VIBE4";
				_mkdir(VIBE4.c_str());
				std::string SuBSENSE4 = resultPath + "\\SuBSENSE4";
				_mkdir(SuBSENSE4.c_str());
				for (int i = 0; i < dsnumber[ds]; i++)
				{
					thetempnumber.str("");
					thetempnumber.clear();
					
					frameIdStr.str("");
					frameIdStr.clear();
					
					thetempnumber << "\\in" <<  setw(6) << setfill('0')<<i + 1  << dsTypes[ds];
					tempcac = datasetPath + thetempnumber.str();
					frame = cv::imread(tempcac);
					
					std::cout << tempcac << endl;
					std::cout << frame.size() << endl;
					{
						gmmbooster1.SetDspSize(cv::Size(frame.cols>>1, frame.rows>>1));
						vibebooster1.SetDspSize(cv::Size(frame.cols>>1, frame.rows>>1));
						subsensebooster1.SetDspSize(cv::Size(frame.cols>>1, frame.rows>>1));

						cv::Mat gmmResult1, vibeResult1, subsenseResult1;
						cv::Mat gmmResult2, vibeResult2, subsenseResult2;

						double time = 0.0;

						time = (double)cv::getTickCount();
						gmmbooster1.Boost(frame, gmmResult1);
						timeGMM2 += ((double)cv::getTickCount() - time) / cv::getTickFrequency();


						time = (double)cv::getTickCount();
						vibebooster1.Boost(frame, vibeResult1);
						timeVIBE2 += ((double)cv::getTickCount() - time) / cv::getTickFrequency();


						time = (double)cv::getTickCount();
						subsensebooster1.Boost(frame, subsenseResult1);
						timeSuBSENSE2 += ((double)cv::getTickCount() - time) / cv::getTickFrequency();


						cv::threshold(gmmResult1, gmmResult1, 0, 255, cv::THRESH_BINARY);
						cv::threshold(vibeResult1, vibeResult1, 0, 255, cv::THRESH_BINARY);
						cv::threshold(subsenseResult1, subsenseResult1, 0, 255, cv::THRESH_BINARY);

						gmmbooster2.SetDspSize(cv::Size(frame.cols>>2 , frame.rows>>2 ));
						vibebooster2.SetDspSize(cv::Size(frame.cols>>2 , frame.rows>>2 ));
						subsensebooster2.SetDspSize(cv::Size(frame.cols>>2 , frame.rows>>2 ));

						time = (double)cv::getTickCount();
						gmmbooster2.Boost(frame, gmmResult2);
						timeGMM4 += ((double)cv::getTickCount() - time) / cv::getTickFrequency();


						time = (double)cv::getTickCount();
						vibebooster2.Boost(frame, vibeResult2);
						timeVIBE4 += ((double)cv::getTickCount() - time) / cv::getTickFrequency();


						time = (double)cv::getTickCount();
						subsensebooster2.Boost(frame, subsenseResult2);
						timeSuBSENSE4 += ((double)cv::getTickCount() - time) / cv::getTickFrequency();


						cv::threshold(gmmResult2, gmmResult2, 0, 255, cv::THRESH_BINARY);
						cv::threshold(vibeResult2, vibeResult2, 0, 255, cv::THRESH_BINARY);
						cv::threshold(subsenseResult2, subsenseResult2, 0, 255, cv::THRESH_BINARY);


					frameIdStr << "\\bin" << std::setfill('0') << std::setw(6) << i << ".png";

					cv::imwrite(GMM2 + frameIdStr.str(), gmmResult1);
					cv::imwrite(GMM4 + frameIdStr.str(), gmmResult2);
					if (i != 0)
						{
						cv::imwrite(VIBE2 + frameIdStr.str(), vibeResult1);
						cv::imwrite(SuBSENSE2 + frameIdStr.str(), subsenseResult1);
						cv::imwrite(VIBE4 + frameIdStr.str(), vibeResult2);
						cv::imwrite(SuBSENSE4 + frameIdStr.str(), subsenseResult2);
						}
					}

				}
				std::fstream statsResult(resultPath+"\\result.txt", std::ios::out);
				statsResult << "timeGMM2" << endl;
				statsResult << timeGMM2/dsnumber[ds] << endl;
				statsResult << "timeVIBE2" << endl;
				statsResult << timeVIBE2 / dsnumber[ds] << endl;
				statsResult << "timeSuBSENSE2" << endl;
				statsResult << timeSuBSENSE2 / dsnumber[ds] << endl;
				statsResult << "timeGMM4" << endl;
				statsResult << timeGMM4 / dsnumber[ds] << endl;
				statsResult << "timeVIBE4" << endl;
				statsResult << timeVIBE4 / dsnumber[ds] << endl;
				statsResult << "timeSuBSENSE4" << endl;
				statsResult << timeSuBSENSE4 / dsnumber[ds] << endl;
				statsResult.close();

			}
		}

	return 0;

}




