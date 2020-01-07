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

	int j = 0; //���° ��Ÿ������
	//�̹��� �ε�
	cv::Mat image = cv::imread("img1.bmp");

	cv::Mat gray;

	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY); //�׷��� �����Ϸ�


	const int width = image.cols;
	const int height = image.rows;

	
	
	
	cv::Mat Oct[18]; // 0 1 2 3 4 5~ 35     ��Ÿ�� 0 1 2
	
	cv::Mat gray2;//2����
	cv::resize(gray, gray2, cv::Size(gray.cols * 2, gray.rows * 2), 0, 0, cv::INTER_LINEAR);

		

	double Gau1[6] = { 1.56, 1.9654, 2.4763, 3.1198, 3.9307, 4.9523 }; // 0 1 2 3 4 5 ��Ÿ�� -1�� ����
	//double Gau1[6] = { 1.6, 2.0159, 2.5398, 3.2000, 4.0317, 5.0797 }; // 0 1 2 3 4 5


	int num = 0;
	int sizeG = 0;
	double Sig = 0;
	double beforeSig = 0;											//��Ÿ�� ���� ����
	cv::Mat imagenext;
	for (int i = 0; i < 6; i++)
	{
		sizeG = ceil(6 * Gau1[i]);
		if (sizeG % 2 == 0)
		{
			sizeG++;
		}

		cv::GaussianBlur(gray2, Oct[6 * j + i], cv::Size(sizeG, sizeG), Gau1[i], Gau1[i]); //�̶��� ����ص� �ñ׸� �� ���
		printf("0 : %d \n", Oct[0].cols);
		//0 ��Ÿ�� ������
	}
	num = num + 3;
	cv::resize(Oct[num], imagenext, cv::Size((gray2.cols / 2), (gray2.rows / 2)), 0, 0, cv::INTER_LINEAR);

	printf("imagenext : %d \n", imagenext.cols);
	num = num + 3;
	Oct[num] = imagenext; //�ٿ������Ÿ� ù��°�� ����    num�� 6

	j++; //j�� ���� 1�� ������
	// 0��° ��Ÿ�� ��

	
	
	for (int i = 1; i < 6; i++) //��Ÿ���� ù��°���� ������ �����س����Ƿ�
	{
		Sig = beforeSig * pow(2.0, 1.0 / 3.0);
		double currentSig = sqrt(pow(Sig, 2)) - pow((beforeSig), 2);
		cv::GaussianBlur(Oct[6 * j + i - 1], Oct[6*j + i], cv::Size(sizeG, sizeG), Sig, Sig);
		printf("1 : %d \n", Oct[6].cols);
		
		beforeSig = Sig;
		//1 ��Ÿ�� ������
	}
	num = num + 3; //num�� 9
	cv::resize(Oct[num], imagenext, cv::Size((gray.cols / 2), (gray.rows / 2)), 0, 0, cv::INTER_LINEAR);
	

	num = num + 3; //num�� 12
	Oct[num] = imagenext; //�ٿ������Ÿ� ù��°�� ����

	j++; //j�� ���� 2�� ������


	for (int i = 1; i < 6; i++) //��Ÿ���� ù��°���� ������ �����س����Ƿ�
	{
		Sig = beforeSig * pow(2.0, 1.0 / 3.0);
		double currentSig = sqrt(pow(Sig, 2)) - pow((beforeSig), 2);
		cv::GaussianBlur(Oct[6 * j + i - 1], Oct[6*j + i], cv::Size(sizeG, sizeG), Sig, Sig);
		printf("2 : %d \n", Oct[12].cols);

		beforeSig = Sig;
		//2 ��Ÿ�� ������
	}
											//��Ÿ�� 3���� ���� ��


	
														//DOG ���� ���� 
	cv::Mat DOG[15];//j���� ��Ÿ��� j*5���� DOG�� ǥ������. ���⼭ j�� 3�̹Ƿ� 15���� DOG
	int count = 0;
	

	for(int q = 0; q <= j; q++) //j�� 2
	{
		for (int i = 0; i < 5; i++)
		{
			if (((6 * q) + i + 1)%6 == 0) //������� 5��°�� 11��°�� �ѱ�(��Ÿ���� ��)
			{
				continue; //5���� 6���� �Ѿ��(��Ÿ���� ����) ������ DOG�� ������� �ʴ´�.
			}
			

			//DOG[count] = Oct[(6*q) + i] - Oct[(6*q) + i+1] + 127;
			
			cv::absdiff(Oct[(6 * q) + i], Oct[(6 * q) + i + 1], DOG[count]);
			
			
			//cv::subtract(Oct[(6 * q) + i], Oct[(6 * q) + i + 1], DOG[count]);
			count++;
			printf("%d - %d�� ����� DOG[%d]�� ���� �Ǿ����ϴ� \n", (6 * q) + i, (6 * q) + i + 1, count);

			
		}
	}

														//DOG ���� �Ϸ�
	
	// ���� ã��
	
	/*
	// min max �Լ� ���� �Ϸ�
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

	
	int check[27] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //26���� ������ ó�� ������ + �ϴ� 27��
	

																			  
		
	for (int q = 0; q <= j; q++) //q�� �ٷ� ��Ÿ�� j�� 2����
	{
		for (int i = 0; i < 5; i++)
		{
			if ((q * 5 + i) % 5 == 0) //0, 5, 10 �� ���� DOG�� ù��° ���� �ѱ��
			{
				
				continue;
			}
			
			else if ((q * 5 + i + 1) % 5 == 0) // 4 9 14�� ���� DOG�� ������ ���� �ѱ��
			{
				
				continue;
			}
			
			else //�������� ���
			{
				//printf("�������� ������� ���Խ��ϴ�. ���� ��Ÿ��� %d", (q * 5 + i));
				for (int j1 = 0; j1 < DOG[((q * 5) + i)].rows; j1++) //����
				{
					for (int i1 = 0; i1 < DOG[((q * 5) + i)].cols; i1++) //����
					{
						//printf("DOG :  %d   x : %d, y : %d \n", ((q * 5) + i), i1, j1);
						

						if (i1 == 0 || j1 == 0 || j1 == (DOG[((q * 5) + i)].rows - 1) || i1 == (DOG[((q * 5) + i)].cols - 1))
						{
							//printf("�����ڸ��� ���� ��ġ�� %d,%d \n",j1,i1);
							continue;//�����ڸ��� �ɷ�����
						}
						else
						{
							//int intensity3 = DOG[(q * 5 + i)].at<uchar>(400, 500); // (200, 500)�� �ȼ���

							//img.at<uchar>(y, x) 

							//printf("%d \n", intensity3);
							
							check[0] = DOG[(q * 5 + i)].at<uchar>(j1, i1); //����
							
							
							check[1] = DOG[(q * 5 + i)].at<uchar>(j1 - 1, i1 - 1);
							check[2] = DOG[(q * 5 + i)].at<uchar>(j1 - 1, i1);
							check[3] = DOG[(q * 5 + i)].at<uchar>(j1 - 1, i1 + 1);
							//������ 3��

							check[4] = DOG[(q * 5 + i)].at<uchar>(j1, i1 - 1); 
							check[5] = DOG[(q * 5 + i)].at<uchar>(j1, i1 + 1); 
							//�¿� 2��

							check[6] = DOG[(q * 5 + i)].at<uchar>(j1 + 1, i1 - 1);
							check[7] = DOG[(q * 5 + i)].at<uchar>(j1 + 1, i1);
							check[8] = DOG[(q * 5 + i)].at<uchar>(j1 + 1, i1 + 1);
							//������ 3��

							
							//o-1����
						
							check[9] = DOG[(q * 5 + i -1)].at<uchar>(j1 - 1, i1 - 1);
							check[10] = DOG[(q * 5 + i -1)].at<uchar>(j1 - 1, i1);
							check[11] = DOG[(q * 5 + i -1)].at<uchar>(j1 - 1, i1 + 1);
							//������ 3��

							check[12] = DOG[(q * 5 + i - 1)].at<uchar>(j1, i1 - 1);
							check[13] = DOG[(q * 5 + i - 1)].at<uchar>(j1, i1);
							check[14] = DOG[(q * 5 + i - 1)].at<uchar>(j1, i1 + 1);
							//�߰����� 3��

							check[15] = DOG[(q * 5 + i - 1)].at<uchar>(j1 + 1, i1 - 1);
							check[16] = DOG[(q * 5 + i - 1)].at<uchar>(j1 + 1, i1);
							check[17] = DOG[(q * 5 + i - 1)].at<uchar>(j1 + 1, i1 + 1);
							//�Ʒ����� 3��

							//o+1����

							check[18] = DOG[(q * 5 + i + 1)].at<uchar>(j1 - 1, i1 - 1);
							check[19] = DOG[(q * 5 + i + 1)].at<uchar>(j1 - 1, i1);
							check[20] = DOG[(q * 5 + i + 1)].at<uchar>(j1 - 1, i1 + 1);
							//������ 3��

							check[21] = DOG[(q * 5 + i + 1)].at<uchar>(j1, i1 - 1);
							check[22] = DOG[(q * 5 + i + 1)].at<uchar>(j1, i1);
							check[23] = DOG[(q * 5 + i + 1)].at<uchar>(j1, i1 + 1);
							//�߰����� 3��

							check[24] = DOG[(q * 5 + i + 1)].at<uchar>(j1 + 1, i1 - 1);
							check[25] = DOG[(q * 5 + i + 1)].at<uchar>(j1 + 1, i1);
							check[26] = DOG[(q * 5 + i + 1)].at<uchar>(j1 + 1, i1 + 1);
							//�Ʒ����� 3��

							// q+5 + i�� DOG �迭�� ��ȣ
							// 0 1 2 3 4		DOG : 0
							// 5 6 7 8 9		DOG : 1
							// 10 11 12 13 14	DOG : 2


							if (isMax(check,27)) //�ִ밪�̶��
							{
								//printf("�ִ�� ����!!!!! \n");
								cv::circle(image, cv::Point(i1*pow(2,q -1), j1*pow(2,q -1)), 1.6*pow(2, (q + i)/ 3.0), cv::Scalar(0,255,0), 1);
								
							}
							else if (isMin(check,27)) //�ּҰ��̶��
							{
								//printf("�ּҷ� ����!!!!! \n");
								cv::circle(image, cv::Point(i1*pow(2, q -1 ), j1*pow(2, q -1)), 1.6*pow(2, (q + i) / 3.0), cv::Scalar(0, 255, 0), 1);
							}

						}
						
						
					}
				}
				
			}

		}
	}
	


	//�̹����� line �׸��� ���
	//cv::Point start_point(width / 2, 0);		//line�� ������ 
	//cv::Point end_point(width / 2, height - 1);	//line�� ����	

	// �Ķ��� ������ �׸���. BGR����
	//cv::line(image, start_point, end_point, cv::Scalar(255, 0, 0), 1); 

	cv::namedWindow("Result");
	
	cv::imshow("Result", image);
	//�̹��� ����
	cv::imwrite("Result.jpg", image);

	cv::waitKey(0);

	return 0;
}


//���Լ� ��� ���� ������ false�� ����
int isMax(int* n, int size) {
	//int max = n[0];

	int is = n[0];
	int ok = true;
	//printf("�ƽ� ���� \n");
	for (int i = 1; i < size; i++)
		if (n[i] < is)
		{
			//printf("%d \n", abs(is - n[i]));
			is = n[0]; //�״��
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

	//printf("�� ���� \n");
	for (int i = 1; i < size; i++)
		if (n[i] > is)
		{
			//printf("%d \n", abs(is - n[i]));
			is = n[0]; //�״��
		}
		else
		{
			return false;
		}


return true;
}