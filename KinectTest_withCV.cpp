#include <opencv2/opencv.hpp>
#include <XnCppWrapper.h>
#include <iostream>

using namespace std;
using namespace cv;

void CheckOpenNIError( XnStatus eResult, string sStatus )
{
    if( eResult != XN_STATUS_OK )
        cerr << sStatus << " Error : " << xnGetStatusString( eResult ) << endl;
}

int main( int argc, char** argv )
{
    XnStatus res = XN_STATUS_OK;
    int keyValue = -1;
    xn::ImageMetaData imageMD;// save rgbImg data
    xn::DepthMetaData depthMD;// save depthImg data
    Mat depthTmp(Size(640,480), CV_16SC1);// temp
    Mat rgbImg(Size(640,480), CV_8UC3), depthImg(Size(640,480), CV_8UC1);// show photo via opencv

    // 1. initial context
    xn::Context context;
    res = context.Init();
    CheckOpenNIError( res, "initialize context" );

    // 2. create depth generator
    xn::DepthGenerator depthGenerator;
    res = depthGenerator.Create( context );
    CheckOpenNIError( res, "Create depth generator" );

    // 3. create image generator
    xn::ImageGenerator imgGenerator;
    res = imgGenerator.Create( context );
    CheckOpenNIError( res, "Create image generator" );

    // 4. set map mode
    XnMapOutputMode mapMode;
    mapMode.nXRes = 640;
    mapMode.nYRes = 480;
    mapMode.nFPS = 30;
    res = depthGenerator.SetMapOutputMode( mapMode );
    CheckOpenNIError( res, "Set depth mapMode" );
    res = imgGenerator.SetMapOutputMode( mapMode );
    CheckOpenNIError( res, "Set image mapMode" );

    // 5. correct viewimage
    depthGenerator.GetAlternativeViewPointCap().SetViewPoint( imgGenerator );

    // 6. start generate data
    res = context.StartGeneratingAll();
    CheckOpenNIError( res, "generate date" );

    // 7. read data
    res = context.WaitNoneUpdateAll();
    CheckOpenNIError( res, "read data" );

    // 8. loop to show
    while ((keyValue != 27) && !(context.WaitNoneUpdateAll()))// Press ESC to quit!
    {

        imgGenerator.GetMetaData(imageMD);// generate data to save
        depthGenerator.GetMetaData(depthMD);// generate data to save

        memcpy(rgbImg.data,imageMD.Data(),640*480*3);// from memory copy data to opencv
        cvtColor(rgbImg,rgbImg,CV_RGB2BGR);// convert rgb to bgr
        memcpy(depthTmp.data,depthMD.Data(),640*480*2);// from memory copy data to opencv
        convertScaleAbs(depthTmp,depthImg,255/4096.0);

        imshow("depth",depthImg);
        imshow("img",rgbImg);
        keyValue = waitKey(1);
    }

    // 9. stop
    context.StopGeneratingAll();
    context.Release();

    return 0;
}