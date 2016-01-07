#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv\ml.h>
#include<vector>

#include<iostream>

using namespace std;
using namespace cv;

int main() {
	cout << "Started" << endl;
	int num_categories = 15;
	int num_images = 500; //image'lerin ismine bakarak karar verdim en buyugu '*image_0415.jpg' tarzý bir þeydi

	string trainLabels[1500]; 
	string testLabels[2985]; 
	Mat* trainImages = new Mat[1500]; // total number of train images.
	Mat* testImages = new Mat[2985]; //total number of test images.

	string categories[15] = {"bedroom","Coast","Forest","Highway","industrial","Insidecity","kitchen","livingroom","Mountain","Office","OpenCountry","store","Street","Suburb","TallBuilding"};
	string testCategories[15] = {"bedroom","coast","forest","highway","industrial","insidecity","kitchen","livingRoom","mountain","office","opencountry","store","street","suburb","tallbuilding"};

	Mat imgDeneme; 
	Mat imgDeneme2; 
	imgDeneme = cv::imread("C:\\Users\\EGECAN\\Documents\\Visual Studio 2012\\Projects\\ConsoleApplication3\\x64\\Debug\\project3-data\\train\\bedroom\\image_0001.jpg");			// open image
	imgDeneme2 = cv::imread("C:\\Users\\EGECAN\\Documents\\Visual Studio 2012\\Projects\\ConsoleApplication3\\x64\\Debug\\project3-data\\test\\bedroom\\image_0003.jpg");			// open image

	string trainPath = "C:\\Users\\EGECAN\\Documents\\Visual Studio 2012\\Projects\\ConsoleApplication3\\x64\\Debug\\project3-data\\train\\";
	string testPath = "C:\\Users\\EGECAN\\Documents\\Visual Studio 2012\\Projects\\ConsoleApplication3\\x64\\Debug\\project3-data\\test\\";

	int testSize = 0;
	int trainSize = 0;

	//char numstr[21];

	for(int i=0; i<num_categories;i++){
		for(int j=1; j<num_images;j++){
			trainPath = "C:\\Users\\EGECAN\\Documents\\Visual Studio 2012\\Projects\\ConsoleApplication3\\x64\\Debug\\project3-data\\train\\";
			testPath =  "C:\\Users\\EGECAN\\Documents\\Visual Studio 2012\\Projects\\ConsoleApplication3\\x64\\Debug\\project3-data\\test\\";


			string num = to_string(j);
			if(j<10){
				num = "00"+num;
			}else if(j<100){
				num = "0"+num;
			}
			trainPath = trainPath + categories[i]+"\\image_0"+num+".jpg";
			//cout << "crashed while reading train image" << endl;
			imgDeneme = cv::imread(trainPath);
			testPath = testPath + testCategories[i]+"\\image_0"+num+".jpg";
			//cout << "crashed while reading test image" << endl;
			imgDeneme2 = cv::imread(testPath);

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
	
	Mat* trainImagesAsTinyImage = new Mat[1500];
	Mat* testImagesAsTinyImage = new Mat[2985];
	
	for(int k=0;k<1500;k++){
		//cout << "checking for train image : " << k << endl;
		Mat resized;
		resized = Mat::zeros(16,16,CV_8UC1);
		//cout << "created the zeros image" << endl;
		trainImagesAsTinyImage[k] = resized;
		resize(trainImages[k],trainImagesAsTinyImage[k],trainImagesAsTinyImage[k].size(),0,0,INTER_NEAREST);
		//imshow("Image",trainImagesAsTinyImage[k]);
		//waitKey(0);
	}

	cout<< "checking for test images" << endl ;
	for(int k=0;k<2985;k++){
		//cout << "checking for train image : " << k << endl;
		Mat resized;
		resized = Mat::zeros(16,16,CV_8UC1);
		//cout << "created the zeros image" << endl;
		testImagesAsTinyImage[k] = resized;
		resize(testImages[k],testImagesAsTinyImage[k],testImagesAsTinyImage[k].size(),0,0,INTER_NEAREST);
	}

	//KNearest neighbors
	cout << "Started To create KNearest structures"<<endl;
	Ptr<ml::KNearest> knn = ml::KNearest::create();
	
	const int K = 10;
    // define training data storage matrices (one for attribute examples, one
    // for classifications)
	cout << "Started To create KNearest structures2"<<endl;
    Mat training_data = Mat(1500, 256, CV_32FC1);
	//Mat_<float> trainFeatures(1500,256);
    Mat training_classifications = Mat(1, 1500, CV_32FC1);
	cout << "Started To create KNearest structures3"<<endl;

	int categorizationCounter = 0;
	cout << "Starting the loop for KNearest"<<endl;
	for(int i=0;i<256;i++){
		for(int j=0;j<1500;j++){
			//cout << "Getting The pixel from image: "<<j << endl;
			//cout << "i / 16 : "<<(int)i/16<<" and i%16 : "<<(int)i%16<<endl;
			int i0 = i/16;
			int i1 = i%16;
			//cout << "Value got : "<<trainImagesAsTinyImage[j].at<uchar>(i0,i1)<<endl;
			//cout << "Value was : "<< training_data.at<double>(j,i) <<endl;
			
			training_data.at<float>(j,i) = (float)(((int)trainImagesAsTinyImage[j].at<unsigned char>(i0,i1)));
			//cout << "Got The pixel at row: "<<i/16<<" Col: "<<i%16 << endl;
			if(categorizationCounter<1500){
				for (int k = 0; k < 15; k++){
					if (categories[k]==trainLabels[j]){
						cout<<categories[k]<<endl;
						cout<<(float)k<<endl;
						training_classifications.at<float>(0, j) = (float) k;
					}
				}
				categorizationCounter++;
				//cout << "Categorized image : " << j << endl;
			}
			
		}
	}
	cout<<"categorization counter = "<<categorizationCounter<<endl;
	
	cout<<"Will train now"<<endl;
	knn->train(training_data, ml::ROW_SAMPLE,training_classifications);
	
	cout<<"Loop for prediction..."<<endl;
	Mat response,dist,testFeature;
	Mat* responses = new Mat[2985];
	double result = 0;
	for(int i=0;i<2985;i++){
		//cout<<"Creating the test feature: "<<i<<endl;
		testFeature = Mat(1,256,CV_32FC1);
		for(int j=0;j<256;j++){
			testFeature.at<float>(0,j) = (float)(((int)testImagesAsTinyImage[i].at<unsigned char>(j/16,j%16)));
		}
		//cout<<"Test feature created"<<endl;
		knn->findNearest(testFeature, K, noArray(), response, dist);
		//cerr << response << endl;
		//cerr << dist<< endl;
		responses[i] = response;
		string predicted = categories[(int)(response.at<float>(0,0))];
		string actual = testLabels[i];
		//cout<<"Predicted : "<<predicted<<" actual value was : "<<actual<<endl;;
		if(predicted.compare(actual)==0){
			result++;
		}else{

		}
	}
	//knn->predict(...);
	cout<<"Loop Completed"<<endl;
	cout<<result<<endl;
	cout<<"Result = %"<<((result*100)/2985)<<endl;
	//cerr<<responses<<endl;
	//cout<<"Loop for printing results"<<endl;
	
	cout << "Trying to delete test images"<<endl;
	delete[] testImages;
	cout << "Trying to delete train images"<<endl;
	delete[] trainImages;
	cout << "Trying to delete train images tiny"<<endl;
	delete[] trainImagesAsTinyImage;
	cout << "Trying to delete test images tiny"<<endl;
	delete[] testImagesAsTinyImage;
	
	return 0;
}