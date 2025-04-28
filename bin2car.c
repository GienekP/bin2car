/*--------------------------------------------------------------------*/
/* BIN2CAR                                                            */
/* by GienekP                                                         */
/* (c) 2025                                                           */
/*--------------------------------------------------------------------*/
#include <stdio.h>
/*--------------------------------------------------------------------*/
typedef unsigned char U8;
/*--------------------------------------------------------------------*/
#define CARMAX (1024*1024)
/*--------------------------------------------------------------------*/
void bin2car(const char *filebin, const char *filecar, U8 code)
{
	U8 header[16]={0x43, 0x41, 0x52, 0x54, 0x00, 0x00, 0x00, code,
		           0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00};
	U8 data[CARMAX];
	unsigned int i,size=0,sum=0;
	FILE *pf;
	for (i=0; i<CARMAX; i++) {data[i]=0xFF;};
	pf=fopen(filebin,"rb");
	if (pf)
	{
		i=fread(data,sizeof(U8),CARMAX,pf);
		fclose(pf);
		switch (code)
		{
			case 0x2A: {size=1024*1024;} break;
			case 0x4B: {size=1024*1024;} break;
			case 0x6C: {size=128*1024;} break;
			case 0x6D: {size=256*1024;} break;
			case 0x6E: {size=512*1024;} break;
			case 0x6F: {size=1024*1024;} break;
			case 0x70: {size=512*1024;} break;			
			default: {size=i;} break;
		};
		if (((i+8192)<size) && (code==0x2A))
		{
			for (i=0; i<8192; i++) {data[(size-8192)+i]=data[i];};
		};
		for (i=0; i<size; i++) {sum+=data[i];};
		header[8]=((sum>>24)&0xFF);
		header[9]=((sum>>16)&0xFF);
		header[10]=((sum>>8)&0xFF);
		header[11]=(sum&0xFF);
		pf=fopen(filecar,"wb");
		if (pf)
		{		
			fwrite(header,sizeof(U8),16,pf);
			i=fwrite(data,sizeof(U8),size,pf);
			if (i==size) {printf("Save %ikB of CAR ( 0x%02X )\n",size/1024,(unsigned int)(code));};
			fclose(pf);
		};		
	};	
}
/*--------------------------------------------------------------------*/
U8 char2hex(char c)
{
	char ret=0;
	switch (c)
	{
		case '1': {ret=1;} break;
		case '2': {ret=2;} break;
		case '3': {ret=3;} break;
		case '4': {ret=4;} break;
		case '5': {ret=5;} break;
		case '6': {ret=6;} break;
		case '7': {ret=7;} break;
		case '8': {ret=8;} break;
		case '9': {ret=9;} break;
		case 'a':
		case 'A': {ret=10;} break;
		case 'b':
		case 'B': {ret=11;} break;
		case 'c':
		case 'C': {ret=12;} break;
		case 'd':
		case 'D': {ret=13;} break;
		case 'e':
		case 'E': {ret=14;} break;
		case 'f':
		case 'F': {ret=15;} break;		
		default: {ret=0;} break;
	};
	return ret;
}
/*--------------------------------------------------------------------*/
U8 str2hex(const char *str)
{
	U8 ret=0;
	U8 a=char2hex(str[0]);
	U8 b=char2hex(str[1]);
	if (str[0])
	{
		ret=a;
		if (str[1])
		{
			ret<<=4;
			ret|=b;
		};
	};
	return ret;
}
/*--------------------------------------------------------------------*/
int main( int argc, char* argv[] )
{	
	printf("BIN2CAR - ver: %s\n",__DATE__);
	if (argc==3)
	{
		bin2car(argv[1],argv[2],0x2A);
	} else
	if (argc==4)
	{
		bin2car(argv[1],argv[2],str2hex(argv[3]));
	}
	else
	{
		printf("(c) GienekP\n"
			"use:\nbin2car file.bin file.car 2A\n"
			"  2A - Atarimax 1 MB Flash (old)\n"
			"  4B - Atarimax 1 MB Flash (new)\n"
			"  6C - J(atari)Cart128(kB)\n"
			"  6D - J(atari)Cart256(kB)\n"
			"  6E - J(atari)Cart512(kB)\n"
			"  6F - J(atari)Cart1024(kB)\n"
			"  70 - DCart\n");
	};
	printf("\n");
	return 0;
}
/*--------------------------------------------------------------------*/

