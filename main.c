//DSPE_2012706141_한성균_2주차

#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>

#define WIDTH 512
#define HEIGHT 512

//========구조체 선언
typedef unsigned char uchar;

//========함수 선언
void ReadFile(uchar *Ori, char *Filename, int Width, int Height);
void WriteFile(uchar *Img, char *Filename, int Width, int Height);
void MeanFilter(uchar *input, uchar *output, int padsize, int Width, int Height);
void ImgPadding_Center(uchar *ori, uchar *padimg, int padwidth_num, int padheight_num, int Width, int Height);
void Ver(uchar *input, uchar *output, int Width, int Height);
void Hor(uchar *input, uchar *output, int Width, int Height);
void ImgPadding_Corner(uchar *ori, uchar *padimg, int padwidth_num, int padheight_num, int Width, int Height);

void main()
{
	//원본 파일 및 출력 파일 메모리 동적 할당
	uchar *ori, *out;

	ori = (uchar *)malloc(sizeof(uchar)*HEIGHT*WIDTH);
	out = (uchar *)malloc(sizeof(uchar)*HEIGHT*WIDTH);

	ReadFile(ori, "Lena.raw", WIDTH, HEIGHT);		//파일 열기
	WriteFile(ori, "Lena_Copy.raw", WIDTH, HEIGHT);	//파일 복사 저장

	//패딩을 위한 변수선언 및 메모리 할당
	int padnum = 3;//패딩 할 사이즈(중심 기준으로 좌 우로 얼만큼 커질지)
	uchar *pad;
	pad = (uchar *)malloc(sizeof(uchar)*(HEIGHT + padnum * 2)*(WIDTH + padnum * 2));


	ImgPadding_Center(ori, pad, padnum * 2, padnum * 2, WIDTH, HEIGHT);	//패딩 하기
	WriteFile(pad, "Lena_Pad_Center.raw", (WIDTH + padnum * 2), (HEIGHT + padnum * 2));	//중앙 패딩 결과 출력

	MeanFilter(pad, out, padnum, WIDTH, HEIGHT);	//평균 필터링
	WriteFile(out, "Lena_Mean_Center.raw", WIDTH, HEIGHT);	//결과 출력

	Ver(ori, out, WIDTH, HEIGHT);//세로 방향 빼기
	WriteFile(out, "Lena_Ver.raw", WIDTH, HEIGHT);//결과 출력

	Hor(ori, out, WIDTH, HEIGHT);//가로 방향 빼기
	WriteFile(out, "Lena_Hor.raw", WIDTH, HEIGHT);//결과 출력

	ImgPadding_Corner(ori, pad, padnum, padnum, WIDTH, HEIGHT);	//패딩 하기
	WriteFile(pad, "Lena_Pad_Corner.raw", (WIDTH + padnum * 2), (HEIGHT + padnum * 2));	//결과 출력
	
	MeanFilter(pad, out, padnum, WIDTH, HEIGHT);	//평균 필터링
	WriteFile(out, "Lena_Mean_Corner.raw", WIDTH, HEIGHT);	//결과 출력


	//메모리 해제
	free(ori);
	free(out);
	free(pad);
}

void ReadFile(uchar *Ori, char *Filename, int Width, int Height)
{
	//파일포인터 선언 및 파일 열기

	FILE *fin;
	fin = fopen(Filename, "rb");

	while (1)
	{
		if (fin == NULL)
		{
			printf("ERROR::FILE DOES NOT EXIST\n");
			break;	//파일 이름을 찾지 못하는경우 while문을 빠져나와 프로그램 종료
		}

		fread(Ori, sizeof(uchar), Width*Height, fin);	//파일 읽기
		fclose(fin);
		break;
	}
}
void WriteFile(uchar *Img, char *Filename, int Width, int Height)
{
	//파일 포인터 선언 및 파일 쓰기
	FILE *fout;
	fout = fopen(Filename, "wb");

	while (1)
	{
		if (fout == NULL)
		{
			printf("ERROR::FILE DOES NOT WRITTEN\n");
			break;	//파일이 쓰이지 않는 경우 오류 알림 및 프로그램 종료
		}

		fwrite(Img, sizeof(uchar), Width*Height, fout);//파일 쓰기
		fclose(fout);
		break;
	}
}
void MeanFilter(uchar *input, uchar *output, int padsize, int Width, int Height)
{
	int pnum = padsize * 2 + 1;
	int Pwidth = Width + pnum;
	int sum = 0;

	for (int j = 0; j < Height; j++)
	{
		for (int i = 0; i < Width; i++)
		{
			sum = 0;
			for (int h = 0; h < pnum; h++)
			{
				for (int w = 0; w < pnum; w++)
				{
					sum += input[(j + h)*(Pwidth - 1) + (i + w)];	//패딩 된 이미지를 패딩 사이즈 만큼 더하게 됨.
				}
			}

			sum /= (pnum*pnum);//평균 구하기

			output[j*Width + i] = (uchar)sum;//결과를 저장
		}
	}
}
void ImgPadding_Center(uchar *ori, uchar *padimg, int padwidth_num, int padheight_num, int Width, int Height)
{
	//패딩 사이즈 정의(3이면 좌 우로 3씩이므로 총 7의 사이즈)
	int add_width = padwidth_num / 2;
	int add_height = padheight_num / 2;

	//패딩 하는 경우 경우의 수를 생각 해 각 경우 별로 패딩 정의

	for (int h = 0; h < Height; h++)	//가운데
	{
		for (int w = 0; w < Width; w++)
		{
			padimg[(h + add_height)*(padwidth_num + Width) + (w + add_width)] = ori[h*Width + w];
		}
	}

	for (int h = 0; h < Height + padheight_num; h++)
	{
		if (h < add_height)	//위쪽 패딩
		{
			for (int w = 0; w < Width; w++)
			{
				padimg[h*(padwidth_num + Width) + (w + add_width)] = ori[0 * Width + w];
			}
		}
		else if (h >= add_height + Height)	//아래쪽 패딩
		{
			for (int w = 0; w < Width; w++)
			{
				padimg[h*(padwidth_num + Width) + (w + add_width)] = ori[(Height - 1)*Width + w];
			}
		}
	}

	for (int h = 0; h < Height + padheight_num; h++)
	{
		for (int w = 0; w < Width + padwidth_num; w++)
		{
			if (w < add_width)	//왼쪽 패딩
			{
				padimg[h*(padwidth_num + Width) + w] = padimg[h*(padwidth_num + Width) + add_width];
			}
			else if (w >= add_width + Width)	//오른쪽 패딩
			{
				padimg[h*(padwidth_num + Width) + w] = padimg[h*(padwidth_num + Width) + Width + add_width - 1];
			}
		}
	}

	
}
void Ver(uchar *input, uchar *output, int Width, int Height)
{
	for (int i = 0; i < Width; i++)
		{
			output[(Height - 1)*Width + i] = 128;	//예외 처리(마지막 줄)
		}

	int temp = 0;

	for (int j = 0; j < Height - 1; j++)
	{
		for (int i = 0; i < Width; i++)
		{
			temp = input[j*Width + i] - input[(j + 1)*Width + i];	//세로 방향 빼기

			if (temp + 128>255)		//클리핑에 대한 예외 처리
				output[j*Width + i] = 255;
			else if (temp + 128 < 0)
				output[j*Width + i] = 0;
			else
				output[j*Width + i] = temp + 128;	//가시화
		}
	}
}
void Hor(uchar *input, uchar *output, int Width, int Height)
{
	for (int i = 0; i < Width; i++)
	{
		output[(Height - 1)*Width + i] = 128;	//예외 처리(마지막 줄)
	}

	int temp = 0;

	for (int j = 0; j < Height; j++)
	{
		for (int i = 0; i < Width - 1; i++)
		{
			temp = input[j*Width + i] - input[j*Width + i + 1];	//가로 방향 빼기

			if (temp + 128>255)		//클리핑에 대한 예외 처리
				output[j*Width + i] = 255;
			else if (temp + 128 < 0)
				output[j*Width + i] = 0;
			else
				output[j*Width + i] = temp + 128;	//가시화
		}
	}
}
void ImgPadding_Corner(uchar *ori, uchar *padimg, int padwidth_num, int padheight_num, int Width, int Height)
{
	int pWnum = padwidth_num * 2;
	int pHnum = padheight_num * 2;
	int pwidth = Width + pWnum;
	int pheight = Height + pHnum;

	for (int j = 0; j < pheight; j++)
	{
		for (int i = 0; i < pwidth; i++)
		{
			if ((j < Height) && (i < Width))
			{
				padimg[j*pwidth + i] = ori[j*Width + i];
			}
			else
			{
				if (i >= Width)
				{
					padimg[j*pwidth + i] = ori[j*Width + Width - 1];
				}
				else
				{
					padimg[j*pwidth + i] = ori[(Height - 1)*Width + i];
				}
			}
		}
	}
}