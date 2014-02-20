#include <stdio.h>
#include <math.h>
#include "./libs/bass/bass.h"
#include <unistd.h>
#include <stdlib.h>


int main(){
const int height = 1000;
const int witdh  = 1000;
unsigned long long int data_audio[height][witdh];

if (BASS_Init(-1, 44100, BASS_DEVICE_3D, 0, NULL)) {	

//	printf("Bass succes init\n");

	} else {

		printf("Fail with initialization of audio library\n");
		printf("Bass error code = %d\n", BASS_ErrorGetCode());
		return 1;
	}
double pos_sec, len_sec, pos_v, len_v;
char name[300];
unsigned int length;

	printf("Enter name of audio file or way to it.\nIt could be WAV, MP3, MP2, MP1 or OGG file.  \nFile name: ");
	scanf("%s", name);

	printf("Enter the position in seconds: ");
	scanf("%lf", &pos_sec);
	if (pos_sec < 0){
		printf("Position is negative. Changed to zero");
		pos_sec = 0;
	}
	
	printf("Enter length of analising part in seconds.\n(40 is better for video)\nLength: ");

	scanf("%d", &length);

HSAMPLE samp;
HSTREAM chan;

	samp = BASS_SampleLoad(FALSE, name, 0, 0, 1, BASS_SAMPLE_LOOP);
	if (!samp) {
		printf("Sample crash\n");
                printf("Bass error code = %d\n", BASS_ErrorGetCode());
		return 1;
	}
	chan = BASS_SampleGetChannel(samp, FALSE);	
	if (!chan){
		printf("Channel crash\n");
                printf("Bass error code = %d\n", BASS_ErrorGetCode());
		return 1;
	}
	len_v = BASS_ChannelGetLength(chan,0);
	len_sec = BASS_ChannelBytes2Seconds(chan, len_v);
	pos_v = len_v * (pos_sec / len_sec);
	BASS_ChannelSetPosition(chan, pos_v, 0);

	if (pos_sec > len_sec) {
		pos_sec = len_sec - 1;
		printf("Position is too large for this file. Changed to last second\n");
		}

	if (pos_sec + length > len_sec) {
		length = len_sec - pos_sec;
		printf("Length is too larg for this file. Changed to the very end.\n");
		usleep(5000000);		
		}

	if (BASS_ChannelPlay(chan, FALSE)) {
                printf("Start playing & decoding\n");
        } else {
                printf("Fail with playing, Error code = %d\n",BASS_ErrorGetCode());
		return 1;
}	

int i = 0, w = -1; 
	while ((i < height - 1) && (BASS_ChannelGetPosition(chan, 0) < len_v)) {
		if (100*i/height != w) {
			printf("%d%c\n", 100*i/height, '%');
			w = 100*i/height;
			}
		usleep(length*1000);			
		if (BASS_ChannelGetData(chan, data_audio[i], BASS_DATA_FFT4096)){
	               /* printf("Bass sucsess got   ");
			for (k = 0; k < 90; k++){
			printf("%lld ", data_audio[i][k]);
				}
			printf("\n");
			*/				
       		} else {
        	        printf("Fail with getting\n");
       		}
		
		i++;
	}
	printf("100%c",'%');
	BASS_ChannelStop(chan);
	BASS_SampleFree(samp);	
//printf("    %u     ",data_audio[100][100]);
unsigned long long int  j, min = 100000, max = 0; 

for (i = 0; i < height; i++)
	{
		for (j = 0; j < witdh; j++)
		{
			data_audio[i][j] = data_audio[i][j]/10000000;
			data_audio[i][j] = data_audio[i][j]/10000000;
				if (data_audio[i][j]!=0) 
			{
				if (data_audio[i][j] > max) 
				{
					max = data_audio[i][j];
				}
				if (data_audio[i][j] < min)
				{
					min = data_audio[i][j];
				}
			}
	}
}

FILE *output, *out;
out = fopen("./data.txt","w");
output = fopen("./data_audio.txt","w");
fprintf(out, "%s\n%d\n%d\n", name, (int)pos_sec, (int)length);
for (i = 0; i < height; i++)
	{
		for (j = 0; j < witdh; j++)
		{ 
			if (data_audio[i][j]!=0) 
			{
				data_audio[i][j] = 1000*(data_audio[i][j]-min)/(max-min);
				fprintf(output, "%d ",(int) data_audio[i][j]);                    //переводит значения в формат от 0 до 1000
			}
		}
}
fclose(output);
fclose(out);
printf("\nMusic decoded\n");
return 0;
}
