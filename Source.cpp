#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/xfeatures2d.hpp"
#include <vector>

#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;
///////////////////////////////////////////////////////////////////////////////////////////////////

void trainBOW() {
    
    //To store all the descriptors that are extracted from all the images.
    Mat featuresUnclustered;
    for(int j=1; j<500;j++){
        
        Mat imgDeneme;
        string trainPath = "/Users/omer/Downloads/project3-data/train/bedroom";
        
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
        
        trainPath = trainPath+ "/image_"+num+".jpg";
        imgDeneme = imread(trainPath);
        
        if(imgDeneme.empty()){
            //cout << "not Train image : " << categories[i] << num << endl ;
        }else{
            
            //To store the keypoints that will be extracted by SIFT
            vector<KeyPoint> keypoints;
            //To store the SIFT descriptor of current image
            Mat descriptor;
            //The SIFT feature extractor and descriptor
            cv::xfeatures2d::SiftDescriptorExtractor detector;
            
            
            
            //SIFT BASLANGIC
            Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
            
            f2d->detect(imgDeneme, keypoints);
            //compute the descriptors for each keypoint
            f2d->compute(imgDeneme, keypoints,descriptor);
            
            featuresUnclustered.push_back(descriptor);
            

        }
        
    }
    
    //Construct BOWKMeansTrainer
    //the number of bags
    int dictionarySize=50;
    //define Term Criteria
    TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
    //retries number
    int retries=1;
    //necessary flags
    int flags=KMEANS_PP_CENTERS;
    //Create the BoW (or BoF) trainer
    BOWKMeansTrainer bowTrainer(dictionarySize,tc,retries,flags);
    //cluster the feature vectors
    Mat dictionary=bowTrainer.cluster(featuresUnclustered);
    //store the vocabulary
    FileStorage fs("dictionary.yml", FileStorage::WRITE);
    fs << "vocabulary" << dictionary;
    fs.release();
    
    

    
    Mat imgDeneme2;// open image
    imgDeneme2 = imread("/Users/omer/Downloads/project3-data/test/forest/image_0215.jpg");			// open image
    
    
    
    
    
    
    //
    
    
    
    //Step 2 - Obtain the BoF descriptor for given image/video frame.
    
    //prepare BOW descriptor extractor from the dictionary
    Mat dictionary1;
    FileStorage fs1("dictionary.yml", FileStorage::READ);
    fs1["vocabulary"] >> dictionary1;
    fs1.release();
    
    //To store the image file name
    char * filename = new char[100];
    //To store the image tag name - only for save the descriptor in a file
    char * imageTag = new char[10];
    
    //open the file to write the resultant descriptor
    FileStorage fs2("descriptor.yml", FileStorage::WRITE);
    
    Ptr<Feature2D> f2d2 = xfeatures2d::SIFT::create();
    //To store the keypoints that will be extracted by SIFT
    vector<KeyPoint> keypoints1;
    //Detect SIFT keypoints (or feature points)
    f2d2->detect(imgDeneme2,keypoints1);
    //To store the BoW (or BoF) representation of the image
    Mat bowDescriptor;
    //extract BoW (or BoF) descriptor from given image
    f2d2->compute(imgDeneme2,keypoints1,bowDescriptor);
    
    //prepare the yml (some what similar to xml) file
    sprintf(imageTag,"img1");
    //write the new BoF descriptor to the file
    fs2 << imageTag << bowDescriptor;
    
    //You may use this descriptor for classifying the image.
    
    
    
    ///KNN Implement edilecek
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match(dictionary1, bowDescriptor, matches);

    //////
    
    
    
    
    cout << (float)matches.size() << endl;
    //release the file storage
    fs2.release();
    

    waitKey(0);
    //SIFT BITIS

}


int main() {

	int num_categories = 15;
	int num_images = 500; //image'lerin ismine bakarak karar verdim en buyugu '*image_0415.jpg' tarzý bir þeydi
	
	string trainLabels[1500]; 
	string testLabels[2985]; 
	Mat trainImages[1500]; // total number of train images.
	Mat testImages[2985]; //total number of test images.

	string categories[15] = {"bedroom","Coast","Forest","Highway","industrial","Insidecity","kitchen","livingroom","Mountain","Office","OpenCountry","store","Street","Suburb","TallBuilding"};
	string testCategories[15] = {"bedroom","coast","forest","highway","industrial","insidecity","kitchen","livingRoom","mountain","office","opencountry","store","street","suburb","tallbuilding"};
	   

    trainBOW();
    
	string trainPath = "";
	string testPath = "";

	int testSize = 0;
	int trainSize = 0;
	
	char numstr[21];

    
    Mat imgDeneme;
    Mat imgDeneme2;
    
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
