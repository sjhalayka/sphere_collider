#ifndef main_H
#define main_H

#include "uv_camera.h"
#include "custom_math.h"

using custom_math::vector_3;
using custom_math::line_segment_3;


#include <cstdlib>
#include <GL/glut.h>       //GLUT Library

#include <iostream>
using std::cout;
using std::endl;

#include <iomanip>
using std::setprecision;

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <sstream>
using std::ostringstream;
using std::istringstream;

#include <fstream>
using std::ofstream;
using std::ifstream;

#include <set>
using std::set;

#include <map>
using std::map;

#include <utility>
using std::pair;


void idle_func(void);
void init_opengl(const int &width, const int &height);
void reshape_func(int width, int height);
void display_func(void);
void keyboard_func(unsigned char key, int x, int y);
void mouse_func(int button, int state, int x, int y);
void motion_func(int x, int y);
void passive_motion_func(int x, int y);

void render_string(int x, const int y, void *font, const string &text);
void draw_objects(void);




size_t n = 1000;// static_cast<size_t>(ceil(mass * mass));

float emitter_radius = 1;
float receiver_radius = 1;// mass;

vector<vector_3> threeD_oscillators;


const float pi = 4.0f * atanf(1.0f);
const double c_meters = 299792458;


vector<line_segment_3> threeD_line_segments;
vector<line_segment_3> threeD_line_segments_intersected;


vector<line_segment_3> twoD_line_segments;
vector<line_segment_3> oneD_line_segments;


custom_math::vector_3 background_colour(0.0f, 0.0f, 0.0f);
custom_math::vector_3 control_list_colour(1.0f, 1.0f, 1.0f);

bool draw_axis = true;
bool draw_control_list = true;

uv_camera main_camera;


GLint win_id = 0;
GLint win_x = 800, win_y = 600;
double camera_w = receiver_radius * 10.0;// 3.086e25f;

double camera_fov = 45.0f;
double camera_x_transform = 0;
double camera_y_transform = 0;
double u_spacer = 0.01f;
double v_spacer = 0.5f*u_spacer;
double w_spacer = 0.1f;
double camera_near = 1.0f;
double camera_far = camera_w * 10.0;

bool lmb_down = false;
bool mmb_down = false;
bool rmb_down = false;
int mouse_x = 0;
int mouse_y = 0;






// https://paulbourke.net/geometry/circlesphere/raysphere.c
int RaySphere(vector_3 p1, vector_3 p2, vector_3 sc, double r, double* mu1, double* mu2)
{
    double a, b, c;
    double bb4ac;
    vector_3 dp;

    dp.x = p2.x - p1.x;
    dp.y = p2.y - p1.y;
    dp.z = p2.z - p1.z;
    a = dp.x * dp.x + dp.y * dp.y + dp.z * dp.z;
    b = 2 * (dp.x * (p1.x - sc.x) + dp.y * (p1.y - sc.y) + dp.z * (p1.z - sc.z));
    c = sc.x * sc.x + sc.y * sc.y + sc.z * sc.z;
    c += p1.x * p1.x + p1.y * p1.y + p1.z * p1.z;
    c -= 2 * (sc.x * p1.x + sc.y * p1.y + sc.z * p1.z);
    c -= r * r;
    bb4ac = b * b - 4 * a * c;
    if (fabs(a) < 1e-5 || bb4ac < 0) {
        *mu1 = 0;
        *mu2 = 0;
        return(FALSE);
    }

    *mu1 = (-b + sqrt(bb4ac)) / (2 * a);
    *mu2 = (-b - sqrt(bb4ac)) / (2 * a);

    return(TRUE);
}










#endif
