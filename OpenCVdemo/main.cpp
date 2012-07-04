// The "Square Detector" program.
// It loads several images sequentially and tries to find squares in
// each image

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <math.h>
#include <string.h>

#include <assert.h>

#define SQR(X) ((X)*(X))
using namespace cv;
using namespace std;

void help()
{
    cout <<
    "\nA program using pyramid scaling, Canny, contours, contour simpification and\n"
    "memory storage (it's got it all folks) to find\n"
    "squares in a list of images pic1-6.png\n"
    "Returns sequence of squares detected on the image.\n"
    "the sequence is stored in the specified memory storage\n"
    "Call:\n"
    "./squares\n"
    "Using OpenCV version %s\n" << CV_VERSION << "\n" << endl;
}


int thresh = 50, N = 30;
const char* wndname = "Square Detection Demo";

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
void findSquares( const Mat& image, vector<vector<Point> >& squares )
{
    squares.clear();

    Mat pyr, timg, gray0(image.size(), CV_8U), gray;

    // down-scale and upscale the image to filter out the noise
    pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
    pyrUp(pyr, timg, image.size());
    vector<vector<Point> > contours;

    // find squares in every color plane of the image
    for( int c = 0; c < 3; c++ )
    {
        int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);

        // try several threshold levels
        for( int l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 0 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                blur(gray0, gray, Size(10,10));
                Canny(gray, gray, 0, thresh, 3);
                // dilate canny output to remove potential
                // holes between edge segments
                dilate(gray, gray, Mat(), Point(-1,-1));
            }
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= (l+1)*255/N;
            }

            // find contours and store them all as a list
            findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

            vector<Point> approx;

            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                    fabs(contourArea(Mat(approx))) > 1000 &&
                    isContourConvex(Mat(approx)) )
                {
                    double maxCosine = 0;

                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    //if( maxCosine < 0.3 )
                        squares.push_back(approx);
                }
            }
        }
    }
}


// the function draws all the squares in the image
void drawSquares( Mat& image, const vector<vector<Point> >& squares )
{
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(image, &p, &n, 1, true, Scalar(0,255,0), 1, CV_AA);
    }

    imshow(wndname, image);
}

void orderRect(vector<Point> &r)
{
    assert(r.size() == 4);
    vector<Point> order_y;
    for (int i = 0; i < 4; ++i)
        order_y.push_back(r[i]);
    for (int s = 0; s < 3; ++s)
        for (int i = 0; i < 3; ++i)
            if (order_y[i].y > order_y[i+1].y)
                std::swap(order_y[i], order_y[i+1]);

    r.clear();
    //bottom left
    r.push_back(order_y[0].x < order_y[1].x ? order_y[0] : order_y[1]);
    //bottom right
    r.push_back(order_y[0].x > order_y[1].x ? order_y[0] : order_y[1]);
    //top right
    r.push_back(order_y[2].x > order_y[3].x ? order_y[2] : order_y[3]);
    //top left
    r.push_back(order_y[2].x < order_y[3].x ? order_y[2] : order_y[3]);
}

bool rectEQ(vector<Point> &a, vector<Point> &b)
{
    assert(a.size() == 4);
    assert(b.size() == 4);

    int count = 0;
    for (int i = 0; i < 4; ++i)
        if (SQR(a[i].x - b[i].x) + SQR(a[i].y - b[i].y) > 400)
        {
            if(++count == 2)
                return false;
        }
    return true;
}

int main(int /*argc*/, char** /*argv*/)
{
    static const char* names[] = { "E:\\blokk\\input\\Scan3.JPG", 0 };
    help();
    namedWindow( wndname, 1 );
    vector<vector<Point> > squares;

    for( int i = 0; names[i] != 0; i++ )
    {
        Mat image = imread(names[i], 1);
        if( image.empty() )
        {
            cout << "Couldn't load " << names[i] << endl;
            continue;
        }

        cv::Size s;
        s.width = 300;
        s.height = 400;

        resize(image, image, s);

        findSquares(image, squares);

        for(vector<vector<Point> >::iterator i = squares.begin(); i != squares.end(); ++i)
            orderRect(*i);

        vector<vector<Point> > unique_squares;

        while(!squares.empty())
        {
            vector<Point> s = squares.back();
            squares.pop_back();
            bool put = true;
            for (int i = 0; i < unique_squares.size(); ++i)
                if (rectEQ(unique_squares[i], s))
                {
                    if (contourArea(Mat(unique_squares[i])) < contourArea(Mat(s)))
                    {
                        unique_squares.erase(unique_squares.begin()+i);
                        unique_squares.push_back(s);
                    }
                    put = false;
                    break;
                }
            if (put)
                unique_squares.push_back(s);
        }

        drawSquares(image, unique_squares);

        std::cout << unique_squares.size() << std::endl;

        int c = waitKey();
        if( (char)c == 27 )
            break;
    }

    return 0;
}
