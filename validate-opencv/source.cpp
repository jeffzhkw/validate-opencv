#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;



int black_white_grid() {
	// Define 8K resolution: 7680 x 4320 pixels
	int width = 7680;
	int height = 4320;

	// Create an 8K black image
	cv::Mat image(cv::Size(width, height), CV_8UC3, cv::Scalar(0, 0, 0));

	// Calculate the size of each grid
	int gridWidth = width / 4;
	int gridHeight = height / 6;

	// Set the first grid to white
	cv::rectangle(image, cv::Point(0, 0), cv::Point(gridWidth-1, gridHeight-1), cv::Scalar(255, 255, 255), -1);

	// Save the image
	cv::imwrite("8K_image_with_grid.png", image);

	return 0;
}

int black_white_grid_24() {
	// Define 8K resolution: 7680 x 4320 pixels
	int width = 7680;
	int height = 4320;

	// Calculate the size of each grid
	int gridWidth = width / 4;
	int gridHeight = height / 6;

	for (int row = 0; row < 6; ++row) {
		for (int col = 0; col < 4; ++col) {
			// Create an 8K black image for each iteration
			cv::Mat image(cv::Size(width, height), CV_8UC3, cv::Scalar(0, 0, 0));

			// Calculate the position of the top-left corner of the white grid
			int x_start = col * gridWidth;
			int y_start = row * gridHeight;

			// Calculate the position of the bottom-right corner of the white grid
			int x_end = x_start + gridWidth - 1;
			int y_end = y_start + gridHeight - 1;

			// Set the current grid to white
			cv::rectangle(image, cv::Point(x_start, y_start), cv::Point(x_end, y_end), cv::Scalar(255, 255, 255), -1);

			// Generate a filename for the current image
			std::string filename = "8K_image_with_white_grid_at_" + std::to_string(row*4+col+1) + ".png";

			// Save the image
			cv::imwrite(filename, image);
		}
	}

	return 0;
}

int main(int argc, char** argv)
{
	//black_white_grid();
	//black_white_grid_24();
	
	// 加载原始图像
	for (int i = 1; i < 25; i++) {

		char input_filename[200];
		sprintf_s(input_filename, "8K_image_with_white_grid_at_%d.png", i);
		

	Mat image = imread(input_filename);
	//Mat image = imread("input_image.jpg");

	// 定义目标图像大小
	int target_width = 7680;  // 8K宽度
	int target_height = 4320; // 8K高度

	// 计算每个小图像的宽度和高度
	int small_width = target_width / 4;
	int small_height = target_height / 6;

	// 循环裁剪图像并保存
	int count = 1;  // 图像计数器
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			// 计算当前小图像的起始和结束坐标
			int start_y = i * small_height;
			int end_y = start_y + small_height;
			int start_x = j * small_width;
			int end_x = start_x + small_width;

			// 裁剪图像
			Mat small_image = image(Range(start_y, end_y), Range(start_x, end_x));

			// 缩放图像到目标大小
			//resize(small_image, small_image, Size(target_width, target_height));

			// 保存图像
			string file_name = "output_image_" + to_string(count) + ".jpg";
			imwrite(file_name, small_image);

			// 增加图像计数器
			count++;
		}
	}
	
	vector<int> png_params;
	png_params.push_back(IMWRITE_PNG_COMPRESSION);
	png_params.push_back(0);



	char filename[200];
	Mat src;


	cv::Mat viewimg[24];
	cv::Mat ge_img(4320, 7680, CV_8UC3);
	
	//int currWhite = 1; //range: 1-24

	//for (int currWhite = 1; currWhite < 25; currWhite++) {

	
	for (unsigned int a = 0; a <24; a++) {

		/*if (a + 1 == currWhite) {
			sprintf_s(filename, "white.jpg");

			viewimg[a] = imread(filename, IMREAD_UNCHANGED);
		}
		else {
			sprintf_s(filename, "black.jpg");
			viewimg[a] = imread(filename, IMREAD_UNCHANGED);
		}*/
		
		sprintf_s(filename, "output_image_%d.jpg", a + 1);
		viewimg[a] = imread(filename, IMREAD_UNCHANGED);

		if (viewimg[a].empty()) {
			cout << a << endl;
			system("pause");
			waitKey(0);
			return -1;
		}
	}

	int viewp, ge_height = 4320, ge_width = 7680, img_num = 24;
	cout << ge_height << ", " << ge_width << endl;
	double A;

	float D, tg_lens =-0.154916;
	double line= 11.983080;
 
	

		for (int i = 0; i < ge_height; i++) {
			
			for (int j = 0; j < ge_width; j++) {
				
				for (int k = 0; k < 3; k++)
				{
					D = 3 * j + k + 3 * i * tg_lens + 12 * line /img_num  ;
					A = fmod(D, line);

					while (A < 0)
					{
						A = A + line;
					}
					viewp = cvFloor(A / (line / img_num));
					if (viewp == img_num)
					{
						viewp = img_num - 1;
					}
					/*if (viewp > 9)cout << viewp;*/
					
					//viewp = cvFloor(A / (line / img_num));
					///*cout << viewp << endl;*/
					//if (viewp == img_num) {
					//	viewp = img_num - 1;
					//}
					/*if (viewp > 4)cout << viewp;*/

					if (viewp > 23)cout << viewp;
					ge_img.at<Vec3b>(i, j)[abs(k-2)] = viewimg[img_num - viewp - 1].at<Vec3b>(i/6 , j/4)[abs(k - 2)];
					
					/*cout << ge_img.at<Vec3b>(i, j)[k] << endl;*/
					
				}
			}
			
		}
		char output_filename[200];
		sprintf_s(output_filename, "output_%d.png", i);
		imwrite(output_filename, ge_img, png_params);
		//namedWindow("input");
		/*imshow("input", ge_img);
		waitKey(10000);
		destroyAllWindows();*/
	}
	}