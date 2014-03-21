GPP=g++ -g -pg -Wall `pkg-config --libs --cflags opencv` -msse -msse2 -msse3 -o
All :	main.o  Saliency.o Segment
	$(GPP) SAL.bin main.o  Saliency.o Segmentation/segment.o
main.o:	main.cpp  stdafx.h Saliency.h
	$(GPP) main.o -c main.cpp
Saliency.o:	Saliency.cpp Saliency.h
	$(GPP) Saliency.o -c Saliency.cpp	
Segment	:	Segmentation/disjoint-set.h Segmentation/segment-graph.h Segmentation/segment-image.h Segmentation/segment-image.cpp stdafx.h
	$(GPP) Segmentation/segment.o -c Segmentation/segment-image.cpp
