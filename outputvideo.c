#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./libs/gd/include/gd.h"
#include "./libs/gd/include/gdfontg.h"
#define q 0.15
#define c 100
#define number_ 1000
typedef struct rgb_ rgb; //структура, описывающая цвет
struct rgb_ 
	{
	float r;
	float g;
	float b;
	};


rgb SetColor_V( int omega, float amplitude) //функция выбора цвета
{

	float pi = 3.14;
	float FiOtOmega=pi * omega / 5; // вспомогательный параметр
	rgb cl;
	float Brightness=250* pow((amplitude/900),6);

	cl.r=1000*Brightness*0.5*( 1+sin(FiOtOmega + 2.2) ); //Перевод частот в оттенок
	cl.g=1000*Brightness*0.5*( 1+sin(FiOtOmega + pi*2/3 + 2.2) );
	cl.b=1000*Brightness*0.5*( 1+sin(FiOtOmega + pi*2/3*2 + 2.2 ) );

	return cl;
}


void circle(gdImagePtr im, float R, int w) //рисует круг радиусом r, цвет зависит от w (частоты)
{
	rgb cl;
	float ampl = R / q;
	int our_color;
        cl = SetColor_V(w, ampl);
	our_color = gdImageColorAllocate(im, (int)cl.r, (int)cl.g, (int)cl.b);
	gdImageFilledArc(im, c, c, (int)R, (int)R, 0, 360, our_color, 100); 
}




void outputvideo()
        {
	printf("Starting video creation\n");
	FILE *in;
	in = fopen("data_audio.txt","r");
	int  a[number_+1][number_+1];
	int   out_data[2][number_+1];
        int i,j,p,e,k;

	for (i=1;i<=number_ -1;i++) //считываем массив из файла
		{
		for (j=1;j<=number_ -1;j++)
			{
			fscanf(in, "%d",&a[i][j]);
			}
		fscanf(in, "\n");
		}
	 
        
        for (i=1;i<=number_ -1;i++)
                {
                for (j=1;j<=number_ -1;j++)
                        {
             		out_data[1][j]=j;
             		out_data[2][j]=a[i][j];
                        }
                        //out_data - двумерный массив, содержащий в первой строке значения частот, во второй - амплитуд для отрезка времени i
        	for (k=1;k<number_ - 1;k++) //сортировка массива по убыванию амплитуд
			{ 			
			for (p=k+1;p<=number_ -1;p++)
                                {
                   		if (out_data[2][k]<out_data[2][p])
                         		{
                         		e=out_data[2][k];
                         		out_data[2][k]=out_data[2][p];
                         		out_data[2][p]=e;

                         		e=out_data[1][k];
                         		out_data[1][k]=out_data[1][p];
                         		out_data[1][p]=e;
                         		}	
                                }
                        }
		int max = out_data[2][1];
		int min = out_data[2][number_ -1];
		for (k = 1; k <= number_ -1; k++)
			{ 
			out_data[2][k] = 1000*(out_data[2][k]-min)/(max-min);
			}  		
		

		
		gdImagePtr im; //Начало прорисовки изображения для i
		FILE *out;
		int white;
		im = gdImageCreate(200,200);
        	white = gdImageColorAllocate(im, 250, 250, 250);
       		for (k=1;k<=number_ -1;k=k+5) //круги создаются с шагом 10, чтобы сократить время работы программы и увеличить качество визуализации (радиусы кругов не будут слишком близкими)
			{		
			float R = out_data[2][k] * q;
			circle(im, R, out_data[1][k]);
			}
	
		char s[]="picture0000.gif";
		
		int z=i;
		int dj;
		char vj; //вспомогательные переменные, для формирования имени картинки

		dj=z % 10;//картинке передается имя, включающее ее номер
		vj = (char)dj + '0';
		s[10] = vj;
			
		z = z / 10;
		dj=z % 10;
		s[9]=(char)dj + 48;
			
		z = z /10;
		dj=z % 10;
		s[8]=(char)dj + 48;
			
		z = z / 10;
		dj=z%10;
		s[7]=(char)dj + 48;
					
	
		out = fopen(s, "wb");
        	gdImageGif (im, out);
      		fclose(out);
        	gdImageDestroy(im);
		int proc;
		if (i%10==0)
			{
			proc = i/10;
			printf("Video completed %d percent \n", proc); 
			}     		
		
                }
	
	
        }




