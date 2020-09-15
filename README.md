# CARF
##A Content-Adaptive Resizing Framework for Boosting Computation Speed of Background Modeling Methods

*The source code will be available soon.

Abstract—Recently, most background modeling (BM) methods 
claim to achieve real-time efficiency for low-resolution and
standard-definition surveillance videos. With the increasing resolutions
of surveillance cameras, full high-definition (full HD)
surveillance videos have become the main trend and thus processing
high-resolution videos becomes a novel issue in intelligent
video surveillance. In this paper, we propose a novel contentadaptive
resizing framework (CARF) to boost the computation
speed of BM methods in high-resolution surveillance videos. For
each frame, we apply superpixels to separate the content of the
frame to homogeneous and boundary sets. Two novel downsampling
and upsampling layers based on the homogeneous and
boundary sets are proposed. The front one downsamples highresolution
frames to low-resolution frames for obtaining efficient
foreground segmentation results based on BM methods. The
later one upsamples the low-resolution foreground segmentation
results to the original resolution frames based on the superpixels.
By simultaneously coupling both layers, experimental results
show that the proposed method can achieve better quantitative
and qualitative results compared with state-of-the-art methods.
Moreover, the computation speed of the proposed method without
GPU accelerations is also significantly faster than that of the
state-of-the-art methods.

*Flow chart of CARF
![image](https://github.com/nchucvml/CARF/blob/master/flowchart.png)

*If you use the CARF codes, please cite the following paper.
Chun-Rong Huang, Wei-Yun Huang, Yi-Sheng Liao, Chien-Cheng Lee, and Yu-Wei Yeh, "A Content-Adaptive Resizing Framework for
Boosting Computation Speed of Background Modeling Methods," accepted by IEEE Transactions on Systems, Man, and Cybernetics: Systems, 2020.

*The demo videos are available at https://www.youtube.com/playlist?list=PLeFabaAzO2xwAr_Ya9ui8hWEtFpAieTYR

