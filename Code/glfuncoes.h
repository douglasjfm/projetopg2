
#include "Main.h"
#include "util.h"
#include <math.h>
#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <vector>

// OpenCV includes
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

void handleKeyboardPressed(unsigned char key, int x, int y);
void handleKeyboardUp(unsigned char key, int x, int y);
void cameraTranslate(double ctx, double cty, double ctz);
void cameraRotateY(double angle);
void normalizeCamera();
void idleFunction();
void myreshape(GLsizei w, GLsizei h);
void drawGrid();
void FimDoPrograma();