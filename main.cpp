    #include <iostream>
    #include<cv.h>
    #include<highgui.h>
    #define forr(i,N) for(int i=0;i<N;i++)
    using namespace std;
    using namespace cv;
    void MyFilledCircle( Mat img, Point center )
    {
     int thickness = -1;
     int lineType = 8;

     circle( img,
             center,
             20,
             Scalar( 0, 0, 255 ),
             thickness,
             lineType );
    }

    int main()
    {
        // Initialize capturing live feed from the camera
        CvCapture* capture = 0;
        capture = cvCaptureFromCAM(-1);
        Vec3b white;white.val[0]=255;white.val[1]=255;white.val[2]=255;
        // Couldn't get a device? Throw an error and quit
        if(!capture)
        {
            printf("Could not initialize capturing...\n");
            return -1;
        }

        // The two windows we'll be using
        //cvNamedWindow("video");
        //cvNamedWindow("thresh");

        // This image holds the "scribble" data...
        // the tracked positions of the ball
        IplImage* imgScribble = NULL;
        // An infinite loop
        while(true)
        {
            // Will hold a frame captured from the camera
            IplImage* frame = 0;
            frame = cvQueryFrame(capture);

        Mat img(frame);////Mat newimg2=img1;//Mat newimg2=img1;

        //CALCULATE  THE ROWS COLOUMNS AND THE CHANNELS IN THE IMAGE
        int rows=img.size().height;int cols=img.size().width;
        int c=img.channels();int al=100;Mat newimg=Mat::zeros(img.size(),img.type());
        printf("%d %d \n",rows,cols);

        ////THRESH THE IMAGE.............AL IS THE THRESHOLD VALUE
        ////REMOVE THE COLORED PIXELS AND GIVE THEM THE COLOR WHITE
        ////REMOVE THE PIXELS BELOW A PARTICULAR THRESHOLD(al) AND GIVE THEM
        ////COLOR BLACK.....
        forr(i,rows)
        {
            forr(j,cols)
            {
                {
                   Vec3b temp=img.at<Vec3b>(i,j);
                   if(temp.val[0]>al || temp.val[1]>al || temp.val[2]>al)
                   {
                     newimg.at<Vec3b>(i,j).val[0]=255;newimg.at<Vec3b>(i,j).val[1]=255;newimg.at<Vec3b>(i,j).val[2]=255;
                   }
                    else{ newimg.at<Vec3b>(i,j).val[0]=0;newimg.at<Vec3b>(i,j).val[1]=0;newimg.at<Vec3b>(i,j).val[2]=0;}
                }
            }
        }

        //FIND _MOMENT OF THE IMAGE
        //MOMENT IS CALCULATE AS .X=(SUM OF ALL X COORDINATES WHITE)/TOTAL X CORDS
        //MOMENT IS CALCULATE AS .Y=(SUM OF ALL Y COORDINATES WHITE)/TOTAL Y CORDS
        //AND THEN DRAW A CIRCLE AT THAT POINT

         double xx=0,yy=0,to=0;
         forr(x,rows)
         {
            forr(y,cols)
            {
              if(newimg.at<Vec3b>(x,y)==white)
               {
                   xx=xx+(x);yy=yy+y;to++;
               }
            }
         }
         printf("%llf moments %llf\n",xx,yy);
         xx=xx/(to);yy=yy/(to);
         //printf("%lf moments %lf\n",xx,yy);
        MyFilledCircle(newimg,Point(yy,xx));
        //	imshow("Scribble here ",img);
            // If we couldn't grab a frame... quit
            if(!frame)
                break;

            // If this is the first frame, we need to initialize it
            //cvShowImage("video", frame);
            imshow("video",newimg);
            // Wait for a keypress
            int cc = cvWaitKey(10);
            if(cc!=-1)
            {
                // If pressed, break out of the loop
                break;
            }
        }
        // We're done using the camera. Other applications can now use it
        cvReleaseCapture(&capture);
        return 0;
    }
