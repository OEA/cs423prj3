#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<vector>

#include<iostream>

using namespace std;
///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {

	int num_categories = 15;
	int num_images = 500; //image'lerin ismine bakarak karar verdim en buyugu '*image_0415.jpg' tarzý bir þeydi
	
	string trainLabels[1500]; 
	string testLabels[2985]; 
	cv::Mat trainImages[1500]; // total number of train images.
	cv::Mat testImages[2985]; //total number of test images.

	string categories[15] = {"bedroom","Coast","Forest","Highway","industrial","Insidecity","kitchen","livingroom","Mountain","Office","OpenCountry","store","Street","Suburb","TallBuilding"};
	string testCategories[15] = {"bedroom","coast","forest","highway","industrial","insidecity","kitchen","livingRoom","mountain","office","opencountry","store","street","suburb","tallbuilding"};
	
	cv::Mat imgDeneme; 
	cv::Mat imgDeneme2; 
	imgDeneme = cv::imread("C:\\Users\\EGECAN\\Documents\\Visual Studio 2012\\Projects\\ConsoleApplication3\\x64\\Debug\\project3-data\\train\\bedroom\\image_0001.jpg");			// open image
	imgDeneme2 = cv::imread("C:\\Users\\EGECAN\\Documents\\Visual Studio 2012\\Projects\\ConsoleApplication3\\x64\\Debug\\project3-data\\test\\bedroom\\image_0003.jpg");			// open image


	string trainPath = "C:\\Users\\EGECAN\\Documents\\Visual Studio 2012\\Projects\\ConsoleApplication3\\x64\\Debug\\project3-data\\train\\";
	string testPath = "C:\\Users\\EGECAN\\Documents\\Visual Studio 2012\\Projects\\ConsoleApplication3\\x64\\Debug\\project3-data\\test\\";

	int testSize = 0;
	int trainSize = 0;
	
	char numstr[21];

	for(int i=0; i<num_categories;i++){
		for(int j=1; j<num_images;j++){
			trainPath = "C:\\Users\\EGECAN\\Documents\\Visual Studio 2012\\Projects\\ConsoleApplication3\\x64\\Debug\\project3-data\\train\\";
			testPath =  "C:\\Users\\EGECAN\\Documents\\Visual Studio 2012\\Projects\\ConsoleApplication3\\x64\\Debug\\project3-data\\test\\";
			
			
			string num = itoa(j,numstr,10);
			if(j<10){
				num = "00"+num;
			}else if(j<100){
				num = "0"+num;
			}
			trainPath = trainPath + categories[i]+"\\image_0"+num+".jpg";
			imgDeneme = cv::imread(trainPath);
			testPath = testPath + testCategories[i]+"\\image_0"+num+".jpg";
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
				cv::imshow(categories[i],imgDeneme2);
				testLabels[testSize] = categories[i];
				testImages[testSize] = imgDeneme2;
				testSize++;
				cv::waitKey(0);
			}
			
			
			//cout << trainPath << endl;
		}
	}
	//delete numstr;
	cout << trainSize << endl;
	cout << testSize << endl;

	return(0);
}