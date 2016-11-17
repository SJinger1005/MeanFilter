//DSPE_2012706141_�Ѽ���_2����

#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>

#define WIDTH 512
#define HEIGHT 512

//========����ü ����
typedef unsigned char uchar;

//========�Լ� ����
void ReadFile(uchar *Ori, char *Filename, int Width, int Height);
void WriteFile(uchar *Img, char *Filename, int Width, int Height);
void MeanFilter(uchar *input, uchar *output, int padsize, int Width, int Height);
void ImgPadding_Center(uchar *ori, uchar *padimg, int padwidth_num, int padheight_num, int Width, int Height);
void Ver(uchar *input, uchar *output, int Width, int Height);
void Hor(uchar *input, uchar *output, int Width, int Height);
void ImgPadding_Corner(uchar *ori, uchar *padimg, int padwidth_num, int padheight_num, int Width, int Height);

void main()
{
	//���� ���� �� ��� ���� �޸� ���� �Ҵ�
	uchar *ori, *out;

	ori = (uchar *)malloc(sizeof(uchar)*HEIGHT*WIDTH);
	out = (uchar *)malloc(sizeof(uchar)*HEIGHT*WIDTH);

	ReadFile(ori, "Lena.raw", WIDTH, HEIGHT);		//���� ����
	WriteFile(ori, "Lena_Copy.raw", WIDTH, HEIGHT);	//���� ���� ����

	//�е��� ���� �������� �� �޸� �Ҵ�
	int padnum = 3;//�е� �� ������(�߽� �������� �� ��� ��ŭ Ŀ����)
	uchar *pad;
	pad = (uchar *)malloc(sizeof(uchar)*(HEIGHT + padnum * 2)*(WIDTH + padnum * 2));


	ImgPadding_Center(ori, pad, padnum * 2, padnum * 2, WIDTH, HEIGHT);	//�е� �ϱ�
	WriteFile(pad, "Lena_Pad_Center.raw", (WIDTH + padnum * 2), (HEIGHT + padnum * 2));	//�߾� �е� ��� ���

	MeanFilter(pad, out, padnum, WIDTH, HEIGHT);	//��� ���͸�
	WriteFile(out, "Lena_Mean_Center.raw", WIDTH, HEIGHT);	//��� ���

	Ver(ori, out, WIDTH, HEIGHT);//���� ���� ����
	WriteFile(out, "Lena_Ver.raw", WIDTH, HEIGHT);//��� ���

	Hor(ori, out, WIDTH, HEIGHT);//���� ���� ����
	WriteFile(out, "Lena_Hor.raw", WIDTH, HEIGHT);//��� ���

	ImgPadding_Corner(ori, pad, padnum, padnum, WIDTH, HEIGHT);	//�е� �ϱ�
	WriteFile(pad, "Lena_Pad_Corner.raw", (WIDTH + padnum * 2), (HEIGHT + padnum * 2));	//��� ���
	
	MeanFilter(pad, out, padnum, WIDTH, HEIGHT);	//��� ���͸�
	WriteFile(out, "Lena_Mean_Corner.raw", WIDTH, HEIGHT);	//��� ���


	//�޸� ����
	free(ori);
	free(out);
	free(pad);
}

void ReadFile(uchar *Ori, char *Filename, int Width, int Height)
{
	//���������� ���� �� ���� ����

	FILE *fin;
	fin = fopen(Filename, "rb");

	while (1)
	{
		if (fin == NULL)
		{
			printf("ERROR::FILE DOES NOT EXIST\n");
			break;	//���� �̸��� ã�� ���ϴ°�� while���� �������� ���α׷� ����
		}

		fread(Ori, sizeof(uchar), Width*Height, fin);	//���� �б�
		fclose(fin);
		break;
	}
}
void WriteFile(uchar *Img, char *Filename, int Width, int Height)
{
	//���� ������ ���� �� ���� ����
	FILE *fout;
	fout = fopen(Filename, "wb");

	while (1)
	{
		if (fout == NULL)
		{
			printf("ERROR::FILE DOES NOT WRITTEN\n");
			break;	//������ ������ �ʴ� ��� ���� �˸� �� ���α׷� ����
		}

		fwrite(Img, sizeof(uchar), Width*Height, fout);//���� ����
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
					sum += input[(j + h)*(Pwidth - 1) + (i + w)];	//�е� �� �̹����� �е� ������ ��ŭ ���ϰ� ��.
				}
			}

			sum /= (pnum*pnum);//��� ���ϱ�

			output[j*Width + i] = (uchar)sum;//����� ����
		}
	}
}
void ImgPadding_Center(uchar *ori, uchar *padimg, int padwidth_num, int padheight_num, int Width, int Height)
{
	//�е� ������ ����(3�̸� �� ��� 3���̹Ƿ� �� 7�� ������)
	int add_width = padwidth_num / 2;
	int add_height = padheight_num / 2;

	//�е� �ϴ� ��� ����� ���� ���� �� �� ��� ���� �е� ����

	for (int h = 0; h < Height; h++)	//���
	{
		for (int w = 0; w < Width; w++)
		{
			padimg[(h + add_height)*(padwidth_num + Width) + (w + add_width)] = ori[h*Width + w];
		}
	}

	for (int h = 0; h < Height + padheight_num; h++)
	{
		if (h < add_height)	//���� �е�
		{
			for (int w = 0; w < Width; w++)
			{
				padimg[h*(padwidth_num + Width) + (w + add_width)] = ori[0 * Width + w];
			}
		}
		else if (h >= add_height + Height)	//�Ʒ��� �е�
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
			if (w < add_width)	//���� �е�
			{
				padimg[h*(padwidth_num + Width) + w] = padimg[h*(padwidth_num + Width) + add_width];
			}
			else if (w >= add_width + Width)	//������ �е�
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
			output[(Height - 1)*Width + i] = 128;	//���� ó��(������ ��)
		}

	int temp = 0;

	for (int j = 0; j < Height - 1; j++)
	{
		for (int i = 0; i < Width; i++)
		{
			temp = input[j*Width + i] - input[(j + 1)*Width + i];	//���� ���� ����

			if (temp + 128>255)		//Ŭ���ο� ���� ���� ó��
				output[j*Width + i] = 255;
			else if (temp + 128 < 0)
				output[j*Width + i] = 0;
			else
				output[j*Width + i] = temp + 128;	//����ȭ
		}
	}
}
void Hor(uchar *input, uchar *output, int Width, int Height)
{
	for (int i = 0; i < Width; i++)
	{
		output[(Height - 1)*Width + i] = 128;	//���� ó��(������ ��)
	}

	int temp = 0;

	for (int j = 0; j < Height; j++)
	{
		for (int i = 0; i < Width - 1; i++)
		{
			temp = input[j*Width + i] - input[j*Width + i + 1];	//���� ���� ����

			if (temp + 128>255)		//Ŭ���ο� ���� ���� ó��
				output[j*Width + i] = 255;
			else if (temp + 128 < 0)
				output[j*Width + i] = 0;
			else
				output[j*Width + i] = temp + 128;	//����ȭ
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