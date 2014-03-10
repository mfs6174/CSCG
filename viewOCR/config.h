#ifndef CONFIG_H
#define CONFIG_H

extern std::string input_filename;

extern double meanshift_sp;
extern double meanshift_sr;

extern int bilateral_size1;
extern int bilateral_size2;
extern double bilateral_sigma1;
extern double bilateral_sigma2;

extern double swt_canny_ratio;
extern bool swt_dark_on_light;

extern double segment_sigma;
extern double segment_c;
extern int segment_minsize;

extern double merge_color_thres;
extern double merge_swt_thres;

extern double swt_filter_variance_mean_ratio;

extern std::string ocr_lang;
#endif
