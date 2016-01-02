#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/xfeatures2d.hpp"
#include <vector>

#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;
///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {

	int num_categories = 15;
	int num_images = 500; //image'lerin ismine bakarak karar verdim en buyugu '*image_0415.jpg' tarzý bir þeydi
	
	string trainLabels[1500]; 
	string testLabels[2985]; 
	Mat trainImages[1500]; // total number of train images.
	Mat testImages[2985]; //total number of test images.

	string categories[15] = {"bedroom","Coast","Forest","Highway","industrial","Insidecity","kitchen","livingroom","Mountain","Office","OpenCountry","store","Street","Suburb","TallBuilding"};
	string testCategories[15] = {"bedroom","coast","forest","highway","industrial","insidecity","kitchen","livingRoom","mountain","office","opencountry","store","street","suburb","tallbuilding"};
	
	Mat imgDeneme;
	Mat imgDeneme2;
	imgDeneme = imread("/Users/omer/Downloads/project3-data/train/bedroom/image_0001.jpg");			// open image
	imgDeneme2 = imread("/Users/omer/Downloads/project3-data/test/bedroom/image_0003.jpg");			// open image
   
    
    
    
    //SIFT BASLANGIC
    Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
    vector<KeyPoint> keypoints_1;
    f2d->detect(imgDeneme, keypoints_1 );
    Mat output;
    drawKeypoints(imgDeneme, keypoints_1, output);
    imshow("Output", output);
    waitKey(0);
    //SIFT BITIS
   

	string trainPath = "";
	string testPath = "";

	int testSize = 0;
	int trainSize = 0;
	
	char numstr[21];

	for(int i=0; i<num_categories;i++){
		for(int j=1; j<num_images;j++){
			trainPath = "/Users/omer/Downloads/project3-data/train/";
			testPath =  "/Users/omer/Downloads/project3-data/test/";
			
			string num = "";//itoa(j,numstr,10);
            if(j<10){
                stringstream sstm;
                sstm << num << j;
				num = "000"+sstm.str();
            }else if (j < 100){
                stringstream sstm;
                sstm << num << j;
                num = "00"+sstm.str();
            }else {
                stringstream sstm;
                sstm << num << j;
                num = "0"+sstm.str();
            }
            
			trainPath = trainPath + categories[i]+"/image_"+num+".jpg";
            cout << trainPath << endl;
			imgDeneme = imread(trainPath);
			testPath = testPath + testCategories[i]+"/image_"+num+".jpg";
			imgDeneme2 = imread(testPath);

			if(imgDeneme.empty()){
				//cout << "not Train image : " << categories[i] << num << endl ;
			}else{
                
                
				trainLabels[trainSize] = categories[i];
				trainImages[trainSize] = imgDeneme;
				trainSize++;
			}
			
			if(imgDeneme2.empty()){
				//cout << "not Test image : " << num << endl ;
			}else{
				//cv::imshow(categories[i],imgDeneme2);
				testLabels[testSize] = categories[i];
				testImages[testSize] = imgDeneme2;
				testSize++;
				//cv::waitKey(0);
			}
			
			
			//cout << trainPath << endl;
		}
	}
	//delete numstr;
	cout << trainSize << endl;
	cout << testSize << endl;

	return(0);
}