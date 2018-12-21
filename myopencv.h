#include <opencv2/opencv.hpp>

#ifndef __MYOPENCV__
#define __MYOPENCV__

using namespace cv;
using namespace std;

void basic_test_constructor();
void basic_test_subMat(char *name);
void basic_test_split(char *name);
void basic_test_deepCopy(char *name);
void basic_test_size();
void basic_traversal(char *name);
void color_test(char *name);
void frame_diff();
void morph_testing(char * name);

void histogram_Test(char *name);
void show_hist(Mat & img, char *wname);
void sharpening(char *name);
void change_contrast_brightness(char *name);

void draw_log_scale();
void negative_test(char *name);
void process_ImageLog(char *name);
void process_ImageLog_math(char *name);
void process_Log_LUT(char *name);
void process_Image_exp(char *name);
void draw_exp_scale();
void process_Gamma_LUT(char *name);
void show_hist(Mat & img, char *wname);
void smoothing(char *name);
void GaussianTest(char *name);
void myfilter_test(char *name);
void myfilter2_test(char *name);
void get_logTable(uchar[]);
void get_gamma_LUT(uchar table[], double gamma);


void color_darken(char *name);
void color_enhance(char *name);
void color_inverse(char *name);
void color_inverse_rgb(char *name);
void test_color();
void color_correction(char *name);
void color_ratio(char *name);

void color_sharpen(char *name);
void color_smooth(char*);
void color_laplacian(char *name);
void color_segment(char *name);
void change_colortone(char *name);
void mouse_event(char *name);



void edges(char*name);
void Sobel_edges(char*name);
void Scharr_edges(char*name);
void Sobel_Magnitudes(char *name);
void Canny_edges(char*name);
void Canny_test_thresholds(char *name);
void noisy_Canny(char *name);
void Laplacian_Test(char *name);



void extract_line(char* name);
void contours(char* name);

void count_Polygon(char* name);
void approxContour(char * name);

void frame_MOG2(char *);

int checkPoly(vector<Point>&, float);
void sol_problem_1(char *name);
void sol_problem_2(char *name);
void sol_problem_3(char *name);
void sol_problem_4(char *name);
void sol_problem_5(char *name);
void sol_problem_6(char *name);
void sol_problem_7(char *name);
void sol_problem_9(char *name);

void gamma_tranformTest(char *name);
void thresh_test(char *name);
void thresh_otsu(char *name);
void adaptive_threshold_test(char *name);
void vignette(char *name);

#endif