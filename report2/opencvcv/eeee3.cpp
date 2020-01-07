#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <windows.h>
#include <stdio.h>


int isMax(int* n, int size);
int isMin(int* n, int size);


int main()
{

	int j = 0; //몇번째 옥타브인지
	//이미지 로드
	cv::Mat image = cv::imread("img1.bmp");

	cv::Mat gray;

	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY); //그레이 스케일로


	const int width = image.cols;
	const int height = image.rows;

	
	
	
	cv::Mat Oct[18]; // 0 1 2 3 4 5~ 35     옥타브 0 1 2
	
	cv::Mat gray2;//2배의
	cv::resize(gray, gray2, cv::Size(gray.cols * 2, gray.rows * 2), 0, 0, cv::INTER_LINEAR);

		

	double Gau1[6] = { 1.56, 1.9654, 2.4763, 3.1198, 3.9307, 4.9523 }; // 0 1 2 3 4 5 옥타브 -1의 값들
	//double Gau1[6] = { 1.6, 2.0159, 2.5398, 3.2000, 4.0317, 5.0797 }; // 0 1 2 3 4 5


	int num = 0;
	int sizeG = 0;
	double Sig = 0;
	double beforeSig = 0;											//옥타브 구현 시작
	cv::Mat imagenext;
	for (int i = 0; i < 6; i++)
	{
		sizeG = ceil(6 * Gau1[i]);
		if (sizeG % 2 == 0)
		{
			sizeG++;
		}

		cv::GaussianBlur(gray2, Oct[6 * j + i], cv::Size(sizeG, sizeG), Gau1[i], Gau1[i]); //이때는 계산해둔 시그마 값 사용
		printf("0 : %d \n", Oct[0].cols);
		//0 옥타브 끝내기
	}
	num = num + 3;
	cv::resize(Oct[num], imagenext, cv::Size((gray2.cols / 2), (gray2.rows / 2)), 0, 0, cv::INTER_LINEAR);

	printf("imagenext : %d \n", imagenext.cols);
	num = num + 3;
	Oct[num] = imagenext; //줄여놓은거를 첫번째로 저장    num은 6

	j++; //j는 이제 1로 높아짐
	// 0번째 옥타브 끝

	
	
	for (int i = 1; i < 6; i++) //옥타브의 첫번째꺼는 그전에 저장해놨으므로
	{
		Sig = beforeSig * pow(2.0, 1.0 / 3.0);
		double currentSig = sqrt(pow(Sig, 2)) - pow((beforeSig), 2);
		cv::GaussianBlur(Oct[6 * j + i - 1], Oct[6*j + i], cv::Size(sizeG, sizeG), Sig, Sig);
		printf("1 : %d \n", Oct[6].cols);
		
		beforeSig = Sig;
		//1 옥타브 끝내기
	}
	num = num + 3; //num은 9
	cv::resize(Oct[num], imagenext, cv::Size((gray.cols / 2), (gray.rows / 2)), 0, 0, cv::INTER_LINEAR);
	

	num = num + 3; //num은 12
	Oct[num] = imagenext; //줄여놓은거를 첫번째로 저장

	j++; //j는 이제 2로 높아짐


	for (int i = 1; i < 6; i++) //옥타브의 첫번째꺼는 그전에 저장해놨으므로
	{
		Sig = beforeSig * pow(2.0, 1.0 / 3.0);
		double currentSig = sqrt(pow(Sig, 2)) - pow((beforeSig), 2);
		cv::GaussianBlur(Oct[6 * j + i - 1], Oct[6*j + i], cv::Size(sizeG, sizeG), Sig, Sig);
		printf("2 : %d \n", Oct[12].cols);

		beforeSig = Sig;
		//2 옥타브 끝내기
	}
											//옥타브 3개로 구현 끝


	
														//DOG 구현 시작 
	cv::Mat DOG[15];//j개의 옥타브는 j*5개의 DOG로 표현가능. 여기서 j는 3이므로 15개의 DOG
	int count = 0;
	

	for(int q = 0; q <= j; q++) //j는 2
	{
		for (int i = 0; i < 5; i++)
		{
			if (((6 * q) + i + 1)%6 == 0) //예를들어 5번째나 11번째는 넘김(옥타브의 끝)
			{
				continue; //5에서 6으로 넘어가는(옥타브의 변경) 과정의 DOG는 계산하지 않는다.
			}
			

			//DOG[count] = Oct[(6*q) + i] - Oct[(6*q) + i+1] + 127;
			
			cv::absdiff(Oct[(6 * q) + i], Oct[(6 * q) + i + 1], DOG[count]);
			
			
			//cv::subtract(Oct[(6 * q) + i], Oct[(6 * q) + i + 1], DOG[count]);
			count++;
			printf("%d - %d의 결과로 DOG[%d]에 저장 되었습니다 \n", (6 * q) + i, (6 * q) + i + 1, count);

			
		}
	}

														//DOG 구현 완료
	
	// 극점 찾기
	
	/*
	// min max 함수 검증 완료
	int a[6] = { 2, 4, 7, 3, 4, 5 };
	bool what = isMin(a, 6);

	if (what)
	{
		printf("OK \n");
	}
	else
	{
		printf("NO \n");
	}
	*/

	
	int check[27] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //26개의 비교점과 처음 시작점 + 하니 27개
	

																			  
		
	for (int q = 0; q <= j; q++) //q가 바로 옥타브 j는 2까지
	{
		for (int i = 0; i < 5; i++)
		{
			if ((q * 5 + i) % 5 == 0) //0, 5, 10 과 같은 DOG의 첫번째 값들 넘기기
			{
				
				continue;
			}
			
			else if ((q * 5 + i + 1) % 5 == 0) // 4 9 14와 같은 DOG의 마지막 값들 넘기기
			{
				
				continue;
			}
			
			else //정상적인 계산
			{
				//printf("정상적인 계산으로 들어왔습니다. 현재 옥타브는 %d", (q * 5 + i));
				for (int j1 = 0; j1 < DOG[((q * 5) + i)].rows; j1++) //세로
				{
					for (int i1 = 0; i1 < DOG[((q * 5) + i)].cols; i1++) //가로
					{
						//printf("DOG :  %d   x : %d, y : %d \n", ((q * 5) + i), i1, j1);
						

						if (i1 == 0 || j1 == 0 || j1 == (DOG[((q * 5) + i)].rows - 1) || i1 == (DOG[((q * 5) + i)].cols - 1))
						{
							//printf("가장자리들 현재 위치는 %d,%d \n",j1,i1);
							continue;//가장자리들 걸러내기
						}
						else
						{
							//int intensity3 = DOG[(q * 5 + i)].at<uchar>(400, 500); // (200, 500)의 픽셀값

							//img.at<uchar>(y, x) 

							//printf("%d \n", intensity3);
							
							check[0] = DOG[(q * 5 + i)].at<uchar>(j1, i1); //비교점
							
							
							check[1] = DOG[(q * 5 + i)].at<uchar>(j1 - 1, i1 - 1);
							check[2] = DOG[(q * 5 + i)].at<uchar>(j1 - 1, i1);
							check[3] = DOG[(q * 5 + i)].at<uchar>(j1 - 1, i1 + 1);
							//위에서 3개

							check[4] = DOG[(q * 5 + i)].at<uchar>(j1, i1 - 1); 
							check[5] = DOG[(q * 5 + i)].at<uchar>(j1, i1 + 1); 
							//좌우 2개

							check[6] = DOG[(q * 5 + i)].at<uchar>(j1 + 1, i1 - 1);
							check[7] = DOG[(q * 5 + i)].at<uchar>(j1 + 1, i1);
							check[8] = DOG[(q * 5 + i)].at<uchar>(j1 + 1, i1 + 1);
							//위에서 3개

							
							//o-1에서
						
							check[9] = DOG[(q * 5 + i -1)].at<uchar>(j1 - 1, i1 - 1);
							check[10] = DOG[(q * 5 + i -1)].at<uchar>(j1 - 1, i1);
							check[11] = DOG[(q * 5 + i -1)].at<uchar>(j1 - 1, i1 + 1);
							//위에서 3개

							check[12] = DOG[(q * 5 + i - 1)].at<uchar>(j1, i1 - 1);
							check[13] = DOG[(q * 5 + i - 1)].at<uchar>(j1, i1);
							check[14] = DOG[(q * 5 + i - 1)].at<uchar>(j1, i1 + 1);
							//중간에서 3개

							check[15] = DOG[(q * 5 + i - 1)].at<uchar>(j1 + 1, i1 - 1);
							check[16] = DOG[(q * 5 + i - 1)].at<uchar>(j1 + 1, i1);
							check[17] = DOG[(q * 5 + i - 1)].at<uchar>(j1 + 1, i1 + 1);
							//아래에서 3개

							//o+1에서

							check[18] = DOG[(q * 5 + i + 1)].at<uchar>(j1 - 1, i1 - 1);
							check[19] = DOG[(q * 5 + i + 1)].at<uchar>(j1 - 1, i1);
							check[20] = DOG[(q * 5 + i + 1)].at<uchar>(j1 - 1, i1 + 1);
							//위에서 3개

							check[21] = DOG[(q * 5 + i + 1)].at<uchar>(j1, i1 - 1);
							check[22] = DOG[(q * 5 + i + 1)].at<uchar>(j1, i1);
							check[23] = DOG[(q * 5 + i + 1)].at<uchar>(j1, i1 + 1);
							//중간에서 3개

							check[24] = DOG[(q * 5 + i + 1)].at<uchar>(j1 + 1, i1 - 1);
							check[25] = DOG[(q * 5 + i + 1)].at<uchar>(j1 + 1, i1);
							check[26] = DOG[(q * 5 + i + 1)].at<uchar>(j1 + 1, i1 + 1);
							//아래에서 3개

							// q+5 + i는 DOG 배열의 번호
							// 0 1 2 3 4		DOG : 0
							// 5 6 7 8 9		DOG : 1
							// 10 11 12 13 14	DOG : 2


							if (isMax(check,27)) //최대값이라면
							{
								//printf("최대로 검출!!!!! \n");
								cv::circle(image, cv::Point(i1*pow(2,q -1), j1*pow(2,q -1)), 1.6*pow(2, (q + i)/ 3.0), cv::Scalar(0,255,0), 1);
								
							}
							else if (isMin(check,27)) //최소값이라면
							{
								//printf("최소로 검출!!!!! \n");
								cv::circle(image, cv::Point(i1*pow(2, q -1 ), j1*pow(2, q -1)), 1.6*pow(2, (q + i) / 3.0), cv::Scalar(0, 255, 0), 1);
							}

						}
						
						
					}
				}
				
			}

		}
	}
	


	//이미지에 line 그리는 방법
	//cv::Point start_point(width / 2, 0);		//line의 시작점 
	//cv::Point end_point(width / 2, height - 1);	//line의 끝점	

	// 파랑색 선으로 그리기. BGR순서
	//cv::line(image, start_point, end_point, cv::Scalar(255, 0, 0), 1); 

	cv::namedWindow("Result");
	
	cv::imshow("Result", image);
	//이미지 저장
	cv::imwrite("Result.jpg", image);

	cv::waitKey(0);

	return 0;
}


//두함수 모두 값이 같으면 false로 보냄
int isMax(int* n, int size) {
	//int max = n[0];

	int is = n[0];
	int ok = true;
	//printf("맥스 접근 \n");
	for (int i = 1; i < size; i++)
		if (n[i] < is)
		{
			//printf("%d \n", abs(is - n[i]));
			is = n[0]; //그대로
		}
		else
		{

			return false;
			
		}

return true;
}


int isMin(int* n, int size) {
	//int min = n[0];

	int is = n[0];
	int ok = true;

	//printf("민 접근 \n");
	for (int i = 1; i < size; i++)
		if (n[i] > is)
		{
			//printf("%d \n", abs(is - n[i]));
			is = n[0]; //그대로
		}
		else
		{
			return false;
		}


return true;
}