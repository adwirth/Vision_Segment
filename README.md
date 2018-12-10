# exercise-AEO4492
Region growing and edge detection algorithm.

## Environment
* cmake
* OpenCV2

## Sources & Literature
* Shading and Highlight Invariant Color Image Segmentation 
https://pdfs.semanticscholar.org/e48a/dbe72960e376a2afeee05d5dbfa807f5fae1.pdf
* Color Image Edge Detection and Segmentation: A Comparison of the Vector Angle and the Euclidean Distance Color Similarity Measures
https://uwspace.uwaterloo.ca/bitstream/handle/10012/937/swesolko1999.pdf?sequence=1&isAllowed=y
* Multi seed region grow
https://github.com/imLogM/multi_seed_region_grow
* Command line parser by Florian Rappl
https://github.com/FlorianRappl/CmdParser
* Longest path on directed acyclic graph
https://www.geeksforgeeks.org/find-longest-path-directed-acyclic-graph/
* Median filter by Efstathios Chatzikyriakidis
https://efxa.org/2018/06/25/digital-image-processing-algorithms-implemented-with-c-and-opencv/
* Curve fitting by Xianshun Chen
https://github.com/chen0040/cpp-spline

## Algorithm
The Region module provides capabilities for segmenting and contouring regions on RGB images. Before segmentation an optional median filter is applied. The segmentation is based on a region growing algorithm with a similarity function using both color vector angle and lightness distances. The edge detection uses the segmented region, and then optionally performs average filtering and Bezier curve fitting on the resulting contours.

## Example commandlines:
-i images/test2.png -r images/test2r.png -p images/test2o.png -t1 0.001 -t2 0.032 -dim 100 -al 0.5 -ui -se -med -avg 5
-i images/test3.png -r images/test3r.png -p images/test3o.png -t1 0.001 -t2 0.032 -dim 100 -al 0.5 -ui -se -med -avg 5
-i images/test4.jpg -r images/test4r.jpg -p images/test4o.jpg -t1 0.001 -t2 0.032 -dim 100 -al 0.5 -ui -se -med -avg 5
-i images/yesthisfileiscorrupt.png -r images/test4r.jpg -p images/test4o.jpg -t1 0.001 -t2 0.032 -dim 100 -al 0.5 -ui -avg 5

## Algorithm notes
* Region grow could easily provide the perimeter too, but that would compromise modularity: in case of using other region segmentation methods (like clustering) we would still need a separate edge detection method.
*

## TODO
* Smooth image before segmentation
* Adjust contrast before segmentation
* Test cases
* Error handling
* Algorithm description
* Test build on Linux
