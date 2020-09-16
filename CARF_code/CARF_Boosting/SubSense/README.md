This directory contains a cleaned version of the SuBSENSE method configuration presented in
the 2015 IEEE Trans. Image Process. paper 'SuBSENSE : A Universal Change Detection Method with
Local Adaptive Sensitivity'. The original public release (first commit in this repo) corresponds
to the configuration presented in the 2014 CVPRW paper 'Flexible Background Subtraction With
Self-Balanced Local Sensitivity'.

The main class used for background subtraction is `BackgroundSubtractionSuBSENSE`; all other files
contain either dependencies, utilities or interfaces for this method. It is based on OpenCV's
`BackgroundSubtractor` interface, and has been tested with versions 2.4.5 and 2.4.7. By default,
its constructor uses the parameters suggested in the paper.


TL;DR :

```cpp
BackgroundSubtractionSuBSENSE bgs(/*...*/);
bgs.initialize(/*...*/);
for(/*all frames in the video*/) {
    //...
    bgs(input,output);
    //...
}
```

**Note**: this repository is kept here as a stand-alone implementation reference; for the latest
version of the algorithm (with better optimization), see [our framework on github](https://github.com/plstcharles/litiv).

See LICENSE.txt for terms of use and contact information.
