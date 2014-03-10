#include "stdafx.h"
#include "config.h"

std::string input_filename;

double meanshift_sp;
double meanshift_sr;

int bilateral_size1;
int bilateral_size2;
double bilateral_sigma1;
double bilateral_sigma2;

double swt_canny_ratio;
bool swt_dark_on_light;

double segment_sigma;
double segment_c;
int segment_minsize;

double merge_color_thres;
double merge_swt_thres;

double swt_filter_variance_mean_ratio;

std::string ocr_lang;
