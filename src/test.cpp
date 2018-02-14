#include <cstdio>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>

#include "lbf.hpp"

using namespace cv;
using namespace std;
using namespace lbf;

int camera(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "model_name cascade_name camera_index\n");
        return -1;
    }

    const char *model_name = argv[0];
    const char *cascade_name = argv[1];
    int max_face = atoi(argv[2]);

    Config &config = Config::GetInstance();
    int landmark_n = config.landmark_n;
    double bbox[4];
    double scale = 1.3;
    vector<double> x(landmark_n), y(landmark_n);
    cv::VideoCapture vc(0);
    Mat frame, frameCopy, image, img;
    vc >> frame;

    CascadeClassifier cascade;
    if(!cascade.load(cascade_name)){
        fprintf(stderr, "Could not load classifier cascade %s\n", cascade_name);
        return -1;
    }

    LbfCascador lbf_cascador;
    FILE *model = fopen(model_name, "rb");
    if(!model) {
        fprintf(stderr, "Could not load lbf model %s\n", model_name);
        return -1;
    }
    lbf_cascador.Read(model);
    fclose(model);

    for(;;){
    	vector<BBox> bboxes;
    	vector<Mat> shapes;
    	vector<Rect> faces;
    	vc >> frame;
        Mat gray, smallImg;
        double t;
   	cvtColor(frame, gray, CV_BGR2GRAY);

    	smallImg = Mat(cvRound (frame.rows/scale), cvRound(frame.cols/scale), CV_8UC1 );
    	resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);
    	equalizeHist(smallImg, smallImg);
	
	
    	//t = (double)cvGetTickCount();
	
	
    	cascade.detectMultiScale( smallImg, faces,
    		1.1, 2, 0|CV_HAAR_SCALE_IMAGE,
    		Size(40, 40) );
	
	
	
    	//t = (double)cvGetTickCount() - t;
    	//printf("detection time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
	
	int flage = 1;
        if(faces.size() != 0)
	{

    	for(vector<Rect>::iterator r = faces.begin(); r != faces.end(); r++){
	  
	    if (flage > max_face)
	    {
	      break;
	    }
	  
    	    Mat shape;
    	    bbox[0] = r->x*scale;
    	    bbox[1] = r->y*scale;
            bbox[2] = r->width*scale;
    	    bbox[3] = r->height*scale;
    	    BBox bbox_(bbox[0], bbox[1], bbox[2], bbox[3]);
    	  //  t =(double)cvGetTickCount();
    	    shape = lbf_cascador.Predict(gray, bbox_);
    	  //  t = (double)cvGetTickCount() - t;
    	  //  printf("alignment time = %g ms\n", t/((double)cvGetTickFrequency()*1000.) );
    	    shapes.push_back(shape);
    	    bboxes.push_back(bbox_);
	    flage++;
    	}

    	for(int i = 0; i < shapes.size(); i++) {
    	    frame = drawShapeInImage(frame, shapes[i], bboxes[i]);
    	}
    	imshow("camera", frame);

        if(waitKey(1) == 'q')
            break;
	}
	else{
	  imshow("camera", frame);

        if(waitKey(1) == 'q')
            break;
	  
	}
    }

    return 0;
}
