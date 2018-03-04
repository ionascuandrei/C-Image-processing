#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma pack(1)
struct bmp_fileheader
{
    unsigned char  fileMarker1; 
    unsigned char  fileMarker2; 
    unsigned int   bfSize; 
    unsigned short unused1; 
    unsigned short unused2; 
    unsigned int   imageDataOffset; 
};

struct bmp_infoheader
{
    unsigned int   biSize; 
    signed int     width; 
    signed int     height; 
    unsigned short planes;
    unsigned short bitPix; 
    unsigned int   biCompression; 
    unsigned int   biSizeImage;
    int            biXPelsPerMeter;
    int            biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
};
#pragma pack()

struct pixel
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
};

int main()
{
    //deschidere fisier de input
    char input_file[]="input.txt";
    FILE *input=fopen(input_file,"rt");
    if(input==NULL)
        return -1;
  
    //declare variabile din input.txt
    char image_name[100],image_compressed[100];
    int prag;
  
    //extragere variabile din input.txt si inchhdirere
    fscanf(input,"%s",image_name);
    fscanf(input,"%d",&prag);
    fscanf(input,"%s",image_compressed);
    fclose(input);
 
    //deschidere imagine + setare nume fisier TASK1 output
    FILE *our_image=fopen(image_name,"rb");
    if(our_image==NULL)
        return -2;
    int change_name=strlen(image_name)-4;
    strcpy(image_name+change_name,image_name+change_name+4);
    char image_name_simple[100];
    strcpy(image_name_simple,image_name);
    strcat(image_name,"_black_white.bmp");

    //declar variabile pentru structuri
    struct bmp_fileheader header;
    struct bmp_infoheader infoheader;
    struct pixel **image;
 
    //citesc file_header-ul
    fread(&header,14,1,our_image);
    //citesc info_header-ul
    fread(&infoheader,40,1,our_image);
    //setez pointer-ul la matricea de pixeli
    fseek(our_image, header.imageDataOffset, SEEK_SET);
 
    //aloc memorie pentru stocarea imaginii
    image=(struct pixel**)malloc(infoheader.height*sizeof(struct pixel*));
    int i,j;
    for(i=0;i<infoheader.height;i++)
     image[i]=(struct pixel*)malloc(infoheader.width*sizeof(struct pixel));
    //citesc matricea de pixeli
    for(i=0;i<infoheader.height;i++)
    {
        for(j=0;j<infoheader.width;j++)
        {
            fread(&(image[i][j].b),sizeof(unsigned char),1,our_image);
            fread(&(image[i][j].g),sizeof(unsigned char),1,our_image);
            fread(&(image[i][j].r),sizeof(unsigned char),1,our_image);
        }
    }
    //===================Task 1=========================
    for(i=0;i<infoheader.height;i++)
        for(j=0;j<infoheader.width;j++)
        {
        image[i][j].r=floor((image[i][j].g+image[i][j].r+image[i][j].b)/3);
            image[i][j].g=image[i][j].r;
            image[i][j].b=image[i][j].r;
        }
    //================Afisare TASK1======================
    FILE *out=fopen(image_name,"wb");
    if(out==NULL)
        return -1;
    fwrite(&header,sizeof(struct bmp_fileheader),1,out);
    fwrite(&infoheader,sizeof(struct bmp_infoheader),1,out);

    char zero=0;
    for(i=0;i<header.imageDataOffset-54;i++)
        fwrite(&zero,sizeof(char),1,out);
  
    for(i=0;i<infoheader.height;i++)
        for(j=0;j<infoheader.width;j++)
        {
            fwrite(&image[i][j].b,sizeof(unsigned char),1,out);
            fwrite(&image[i][j].g,sizeof(unsigned char),1,out);
            fwrite(&image[i][j].r,sizeof(unsigned char),1,out);
        }
    fclose(out);
    //====================================================

        //creare fisier de output TASK 2 - F1
    char image_name_f1[100];
    strcpy(image_name_f1,image_name_simple);
    strcat(image_name_f1,"_f1.bmp");

    //===================Task 2============================
    //declar o matrice auxiliara care sa fie copie a imaginii
    struct pixel **image2;
    int f1;

    image2=(struct pixel**)malloc(infoheader.height*sizeof(struct pixel*));
    for(i=0;i<infoheader.height;i++)
     image2[i]=(struct pixel*)malloc(infoheader.width*sizeof(struct pixel));

    for(i=0;i<infoheader.height;i++)
        for(j=0;j<infoheader.width;j++)
        {
            image2[i][j].r=image[i][j].r;
            image2[i][j].g=image[i][j].g;
            image2[i][j].b=image[i][j].b;
        }

    //======Schimbare pixeli din interiorul matricei, fara margini=====
    for(i=infoheader.height-2;i>0;i--)
        for(j=1;j<infoheader.width-1;j++)
        {
            f1=(image2[i-1][j-1].r*(-1) + image2[i-1][j].r*(-1) +
                image2[i-1][j+1].r*(-1) + image2[i][j-1].r*(-1) +
                image2[i][j].r*8 + image2[i][j+1].r*(-1) +
                image2[i+1][j-1].r*(-1) + image2[i+1][j].r*(-1) +
                image2[i+1][j+1].r*(-1) );

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }

            f1=(image2[i-1][j-1].g*(-1) + image2[i-1][j].g*(-1) +
                image2[i-1][j+1].g*(-1) + image2[i][j-1].g*(-1) +
                image2[i][j].g*8 + image2[i][j+1].g*(-1) +
                image2[i+1][j-1].g*(-1) + image2[i+1][j].g*(-1) +
                image2[i+1][j+1].g*(-1) );

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }

            f1=(image2[i-1][j-1].b*(-1) + image2[i-1][j].b*(-1) +
                image2[i-1][j+1].b*(-1) + image2[i][j-1].b*(-1) +
                image2[i][j].b*8 + image2[i][j+1].b*(-1) +
                image2[i+1][j-1].b*(-1) + image2[i+1][j].b*(-1) +
                image2[i+1][j+1].b*(-1) );

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }

    for(i=infoheader.height-1;i>=0;i--)
    for(j=0;j<infoheader.width;j++)
    {
    //=======Schimbare pixeli de pe marginea de jos + colturi=====
        if(i==infoheader.height-1 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i-1][j-1].r*(-1) + image2[i-1][j].r*(-1) +
                image2[i-1][j+1].r*(-1) + image2[i][j-1].r*(-1) +
                image2[i][j].r*8 + image2[i][j+1].r*(-1));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
        if(i==infoheader.height-1 && j==0)
        {
            f1=(image2[i-1][j].r*(-1) + image2[i-1][j+1].r*(-1) +
                image2[i][j].r*8 + image2[i][j+1].r*(-1));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }

        if(i==infoheader.height-1 && j==infoheader.width-1)
        {
            f1=( image2[i-1][j-1].r*(-1) + image2[i-1][j].r*(-1) +
                 image2[i][j-1].r*(-1) + image2[i][j].r*8);

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }

    //B
            if(i==infoheader.height-1 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i-1][j-1].b*(-1) + image2[i-1][j].b*(-1) +
                image2[i-1][j+1].b*(-1) + image2[i][j-1].b*(-1) +
                image2[i][j].b*8 + image2[i][j+1].b*(-1));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
        if(i==infoheader.height-1 && j==0)
        {
            f1=(image2[i-1][j].b*(-1) + image2[i-1][j+1].b*(-1) +
                image2[i][j].b*8 + image2[i][j+1].b*(-1));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }

        if(i==infoheader.height-1 && j==infoheader.width-1)
        {
            f1=( image2[i-1][j-1].b*(-1) + image2[i-1][j].b*(-1) +
                 image2[i][j-1].b*(-1) + image2[i][j].b*8);

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
    //G
            if(i==infoheader.height-1 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i-1][j-1].g*(-1) + image2[i-1][j].g*(-1) +
                image2[i-1][j+1].g*(-1) + image2[i][j-1].g*(-1) +
                image2[i][j].g*8 + image2[i][j+1].g*(-1));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
        if(i==infoheader.height-1 && j==0)
        {
            f1=(image2[i-1][j].g*(-1) + image2[i-1][j+1].g*(-1) +
                image2[i][j].g*8 + image2[i][j+1].g*(-1));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }

        if(i==infoheader.height-1 && j==infoheader.width-1)
        {
            f1=( image2[i-1][j-1].g*(-1) + image2[i-1][j].g*(-1) +
                 image2[i][j-1].g*(-1) + image2[i][j].g*8);

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
    //=======Schimbare pixeli de pe marginea de sus + colturi=========
        if(i==0 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i+1][j-1].r*(-1) + image2[i+1][j].r*(-1) +
                image2[i+1][j+1].r*(-1) + image2[i][j-1].r*(-1) +
                image2[i][j].r*8 + image2[i][j+1].r*(-1));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
        if(i==0 && j==0)
        {
            f1=(image2[i+1][j].r*(-1) + image2[i+1][j+1].r*(-1) +
                image2[i][j].r*8 + image2[i][j+1].r*(-1));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
        if(i==0 && j==infoheader.width-1)
        {
            f1=( image2[i+1][j-1].r*(-1) + image2[i+1][j].r*(-1) +
                 image2[i][j-1].r*(-1) + image2[i][j].r*8);

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
    //B
            if(i==0 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i+1][j-1].b*(-1) + image2[i+1][j].b*(-1) +
                image2[i+1][j+1].b*(-1) + image2[i][j-1].b*(-1) +
                image2[i][j].b*8 + image2[i][j+1].b*(-1));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
        if(i==0 && j==0)
        {
            f1=(image2[i+1][j].b*(-1) + image2[i+1][j+1].b*(-1) +
                image2[i][j].b*8 + image2[i][j+1].b*(-1));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
        if(i==0 && j==infoheader.width-1)
        {
            f1=( image2[i+1][j-1].b*(-1) + image2[i+1][j].b*(-1) +
                 image2[i][j-1].b*(-1) + image2[i][j].b*8);

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
    //G
            if(i==0 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i+1][j-1].g*(-1) + image2[i+1][j].g*(-1) +
                image2[i+1][j+1].g*(-1) + image2[i][j-1].g*(-1) +
                image2[i][j].g*8 + image2[i][j+1].g*(-1));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
        if(i==0 && j==0)
        {
            f1=(image2[i+1][j].g*(-1) + image2[i+1][j+1].g*(-1) +
                image2[i][j].g*8 + image2[i][j+1].g*(-1));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
        if(i==0 && j==infoheader.width-1)
        {
            f1=( image2[i+1][j-1].g*(-1) + image2[i+1][j].g*(-1) +
                 image2[i][j-1].g*(-1) + image2[i][j].g*8);

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
    //=======Schimbare pixeli de pe marginea din stanga=========
        if(j==0 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].r*(-1) + image2[i+1][j+1].r*(-1) +
                image2[i][j+1].r*(-1) + image2[i][j].r*8 +
                image2[i-1][j+1].r*(-1) + image2[i-1][j].r*(-1));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
    //B
            if(j==0 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].b*(-1) + image2[i+1][j+1].b*(-1) +
                image2[i][j+1].b*(-1) + image2[i][j].b*8 +
                image2[i-1][j+1].b*(-1) + image2[i-1][j].b*(-1));

             if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
    //G
        if(j==0 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].g*(-1) + image2[i+1][j+1].g*(-1) +
                image2[i][j+1].g*(-1) + image2[i][j].g*8 +
                image2[i-1][j+1].g*(-1) + image2[i-1][j].g*(-1));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
    //=======Schimbare pixeli de pe marginea din dreapta========
        if(j==infoheader.width-1 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].r*(-1) + image2[i+1][j-1].r*(-1) +
                image2[i][j-1].r*(-1) + image2[i][j].r*8 +
                image2[i-1][j-1].r*(-1) + image2[i-1][j].r*(-1));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
    //B
        if(j==infoheader.width-1 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].b*(-1) + image2[i+1][j-1].b*(-1) +
                image2[i][j-1].b*(-1) + image2[i][j].b*8 +
                image2[i-1][j-1].b*(-1) + image2[i-1][j].b*(-1));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
    //G
        if(j==infoheader.width-1 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].g*(-1) + image2[i+1][j-1].g*(-1) +
                image2[i][j-1].g*(-1) + image2[i][j].g*8 +
                image2[i-1][j-1].g*(-1) + image2[i-1][j].g*(-1));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
    }

    //================Afisare TASK2 F1=======================
    FILE *out_f1=fopen(image_name_f1,"wb");
    if(out_f1==NULL)
        return -1;
    fwrite(&header,sizeof(struct bmp_fileheader),1,out_f1);
    fwrite(&infoheader,sizeof(struct bmp_infoheader),1,out_f1);

    for(i=0;i<header.imageDataOffset-54;i++)
        fwrite(&zero,sizeof(char),1,out_f1);
    for(i=0;i<infoheader.height;i++)
        for(j=0;j<infoheader.width;j++)
        {
            fwrite(&image[i][j].b,sizeof(unsigned char),1,out_f1);
            fwrite(&image[i][j].g,sizeof(unsigned char),1,out_f1);
            fwrite(&image[i][j].r,sizeof(unsigned char),1,out_f1);
        }
    fclose(out_f1);
    //====================================================
        //creare fisier de output TASK 2 - F2
    char image_name_f2[100];
    strcpy(image_name_f2,image_name_simple);
    strcat(image_name_f2,"_f2.bmp");

    //====================Task 2 F2==========================
    //====Schimbare pixeli din interiorul matricei, fara margini====
    for(i=infoheader.height-2;i>0;i--)
        for(j=1;j<infoheader.width-1;j++)
        {
            f1=(image2[i-1][j-1].r*(0) + image2[i-1][j].r*(1) +
                image2[i-1][j+1].r*(0) + image2[i][j-1].r*(1) +
                image2[i][j].r*(-4) + image2[i][j+1].r*(1) +
                image2[i+1][j-1].r*(0) + image2[i+1][j].r*(1) +
                image2[i+1][j+1].r*(0) );

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }

            f1=(image2[i-1][j-1].g*(0) + image2[i-1][j].g*(1) +
                image2[i-1][j+1].g*(0) + image2[i][j-1].g*(1) +
                image2[i][j].g*(-4) + image2[i][j+1].g*(1) +
                image2[i+1][j-1].g*(0) + image2[i+1][j].g*(1) +
                image2[i+1][j+1].g*(0) );

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }

            f1=(image2[i-1][j-1].b*(0) + image2[i-1][j].b*(1) +
                image2[i-1][j+1].b*(0) + image2[i][j-1].b*(1) +
                image2[i][j].b*(-4) + image2[i][j+1].b*(1) +
                image2[i+1][j-1].b*(0) + image2[i+1][j].b*(1) +
                image2[i+1][j+1].b*(0) );

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }

    for(i=infoheader.height-1;i>=0;i--)
    for(j=0;j<infoheader.width;j++)
    {
    //=======Schimbare pixeli de pe marginea de jos + colturi=====
        if(i==infoheader.height-1 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i-1][j-1].r*(0) + image2[i-1][j].r*(1) +
                image2[i-1][j+1].r*(0) + image2[i][j-1].r*(1) +
                image2[i][j].r*(-4) + image2[i][j+1].r*(1));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
        if(i==infoheader.height-1 && j==0)
        {
            f1=(image2[i-1][j].r*(1) + image2[i-1][j+1].r*(0) +
                image2[i][j].r*(-4) + image2[i][j+1].r*(1));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }

        if(i==infoheader.height-1 && j==infoheader.width-1)
        {
            f1=( image2[i-1][j-1].r*(0) + image2[i-1][j].r*(1) +
                 image2[i][j-1].r*(1) + image2[i][j].r*(-4));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }

    //B
            if(i==infoheader.height-1 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i-1][j-1].b*(0) + image2[i-1][j].b*(1) +
                image2[i-1][j+1].b*(0) + image2[i][j-1].b*(1) +
                image2[i][j].b*(-4) + image2[i][j+1].b*(1));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
        if(i==infoheader.height-1 && j==0)
        {
            f1=(image2[i-1][j].b*(1) + image2[i-1][j+1].b*(0) +
                image2[i][j].b*(-4) + image2[i][j+1].b*(1));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }

        if(i==infoheader.height-1 && j==infoheader.width-1)
        {
            f1=( image2[i-1][j-1].b*(0) + image2[i-1][j].b*(1) +
                 image2[i][j-1].b*(1) + image2[i][j].b*(-4));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
    //G
            if(i==infoheader.height-1 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i-1][j-1].g*(0) + image2[i-1][j].g*(1) +
                image2[i-1][j+1].g*(0) + image2[i][j-1].g*(1) +
                image2[i][j].g*(-4) + image2[i][j+1].g*(1));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
        if(i==infoheader.height-1 && j==0)
        {
            f1=(image2[i-1][j].g*(1) + image2[i-1][j+1].g*(0) +
                image2[i][j].g*(-4) + image2[i][j+1].g*(1));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }

        if(i==infoheader.height-1 && j==infoheader.width-1)
        {
            f1=( image2[i-1][j-1].g*(0) + image2[i-1][j].g*(1) +
                 image2[i][j-1].g*(1) + image2[i][j].g*(-4));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
    //=======Schimbare pixeli de pe marginea de sus + colturi=========
        if(i==0 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i+1][j-1].r*(0) + image2[i+1][j].r*(1) +
                image2[i+1][j+1].r*(0) + image2[i][j-1].r*(1) +
                image2[i][j].r*(-4) + image2[i][j+1].r*(1));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
        if(i==0 && j==0)
        {
            f1=(image2[i+1][j].r*(1) + image2[i+1][j+1].r*(0) +
                image2[i][j].r*(-4) + image2[i][j+1].r*(1));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
        if(i==0 && j==infoheader.width-1)
        {
            f1=( image2[i+1][j-1].r*0 + image2[i+1][j].r*1 +
                 image2[i][j-1].r*1 + image2[i][j].r*(-4));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
    //B
            if(i==0 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i+1][j-1].b*(0) + image2[i+1][j].b*(1) +
                image2[i+1][j+1].b*(0) + image2[i][j-1].b*(1) +
                image2[i][j].b*(-4) + image2[i][j+1].b*(1));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
        if(i==0 && j==0)
        {
            f1=(image2[i+1][j].b*(1) + image2[i+1][j+1].b*(0) +
                image2[i][j].b*(-4) + image2[i][j+1].b*(1));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
        if(i==0 && j==infoheader.width-1)
        {
            f1=( image2[i+1][j-1].b*0 + image2[i+1][j].b*1 +
                 image2[i][j-1].b*1 + image2[i][j].b*(-4));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
    //G
            if(i==0 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i+1][j-1].g*(0) + image2[i+1][j].g*(1) +
                image2[i+1][j+1].g*(0) + image2[i][j-1].g*(1) +
                image2[i][j].g*(-4) + image2[i][j+1].g*(1));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
        if(i==0 && j==0)
        {
            f1=(image2[i+1][j].g*(1) + image2[i+1][j+1].g*(0) +
                image2[i][j].g*(-4) + image2[i][j+1].g*(1));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
        if(i==0 && j==infoheader.width-1)
        {
            f1=( image2[i+1][j-1].g*0 + image2[i+1][j].g*1 +
                 image2[i][j-1].g*1 + image2[i][j].g*(-4));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
    //=======Schimbare pixeli de pe marginea din stanga=========
        if(j==0 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].r*(1) + image2[i+1][j+1].r*(0) +
                image2[i][j+1].r*(1) + image2[i][j].r*(-4) +
                image2[i-1][j+1].r*(0) + image2[i-1][j].r*(1));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
    //B
            if(j==0 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].b*(1) + image2[i+1][j+1].b*(0) +
                image2[i][j+1].b*(1) + image2[i][j].b*(-4) +
                image2[i-1][j+1].b*(0) + image2[i-1][j].b*(1));

             if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
    //G
        if(j==0 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].g*(1) + image2[i+1][j+1].g*(0) +
                image2[i][j+1].g*(1) + image2[i][j].g*(-4) +
                image2[i-1][j+1].g*(0) + image2[i-1][j].g*(1));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
    //=======Schimbare pixeli de pe marginea din dreapta========
        if(j==infoheader.width-1 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].r*(1) + image2[i+1][j-1].r*(0) +
                image2[i][j-1].r*(1) + image2[i][j].r*(-4) +
                image2[i-1][j-1].r*(0) + image2[i-1][j].r*(1));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
    //B
        if(j==infoheader.width-1 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].b*(1) + image2[i+1][j-1].b*(0) +
                image2[i][j-1].b*(1) + image2[i][j].b*(-4) +
                image2[i-1][j-1].b*(0) + image2[i-1][j].b*(1));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
    //G
        if(j==infoheader.width-1 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].g*(1) + image2[i+1][j-1].g*(0) +
                image2[i][j-1].g*(1) + image2[i][j].g*(-4) +
                image2[i-1][j-1].g*(0) + image2[i-1][j].g*(1));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
    }


    //================Afisare TASK2 F2========================
    FILE *out_f2=fopen(image_name_f2,"wb");
    if(out_f2==NULL)
        return -1;
    fwrite(&header,sizeof(struct bmp_fileheader),1,out_f2);
    fwrite(&infoheader,sizeof(struct bmp_infoheader),1,out_f2);

    for(i=0;i<header.imageDataOffset-54;i++)
        fwrite(&zero,sizeof(char),1,out_f2);
    for(i=0;i<infoheader.height;i++)
        for(j=0;j<infoheader.width;j++)
        {
            fwrite(&image[i][j].b,sizeof(unsigned char),1,out_f2);
            fwrite(&image[i][j].g,sizeof(unsigned char),1,out_f2);
            fwrite(&image[i][j].r,sizeof(unsigned char),1,out_f2);
        }
    fclose(out_f2);

    //=====================================================
        //creare fisier de output TASK 2 - F3
    char image_name_f3[100];
    strcpy(image_name_f3,image_name_simple);
    strcat(image_name_f3,"_f3.bmp");

    //===================Task 2 F3=========================

    for(i=infoheader.height-2;i>0;i--)
        for(j=1;j<infoheader.width-1;j++)
        {
            f1=(image2[i-1][j-1].r*(-1) + image2[i-1][j].r*0 +
                image2[i-1][j+1].r*(1) + image2[i][j-1].r*0 +
                image2[i][j].r*0 + image2[i][j+1].r*0 +
                image2[i+1][j-1].r*(1) + image2[i+1][j].r*0 +
                image2[i+1][j+1].r*(-1) );

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }

            f1=(image2[i-1][j-1].g*(-1) + image2[i-1][j].g*0 +
                image2[i-1][j+1].g*(1) + image2[i][j-1].g*0 +
                image2[i][j].g*0 + image2[i][j+1].g*0 +
                image2[i+1][j-1].g*(1) + image2[i+1][j].g*0 +
                image2[i+1][j+1].g*(-1) );

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }

            f1=(image2[i-1][j-1].b*(-1) + image2[i-1][j].b*0 +
                image2[i-1][j+1].b*(1) + image2[i][j-1].b*0 +
                image2[i][j].b*0 + image2[i][j+1].b*0 +
                image2[i+1][j-1].b*(1) + image2[i+1][j].b*0 +
                image2[i+1][j+1].b*(-1) );

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }

    for(i=infoheader.height-1;i>=0;i--)
    for(j=0;j<infoheader.width;j++)
    {
    //=======Schimbare pixeli de pe marginea de jos + colturi====
        if(i==infoheader.height-1 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i-1][j-1].r*(-1) + image2[i-1][j].r*0 +
                image2[i-1][j+1].r*(1) + image2[i][j-1].r*(0) +
                image2[i][j].r*(0) + image2[i][j+1].r*(0));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
        if(i==infoheader.height-1 && j==0)
        {
            f1=(image2[i-1][j].r*(0) + image2[i-1][j+1].r*(1) +
                image2[i][j].r*0 + image2[i][j+1].r*0);

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }

        if(i==infoheader.height-1 && j==infoheader.width-1)
        {
            f1=( image2[i-1][j-1].r*(-1) + image2[i-1][j].r*0 +
                 image2[i][j-1].r*0 + image2[i][j].r*0);

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }

    //B
            if(i==infoheader.height-1 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i-1][j-1].b*(-1) + image2[i-1][j].b*0 +
                image2[i-1][j+1].b*(1) + image2[i][j-1].b*0 +
                image2[i][j].b*0 + image2[i][j+1].b*0);

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
        if(i==infoheader.height-1 && j==0)
        {
            f1=(image2[i-1][j].b*0 + image2[i-1][j+1].b*(1) +
                image2[i][j].b*0 + image2[i][j+1].b*0);

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }

        if(i==infoheader.height-1 && j==infoheader.width-1)
        {
            f1=( image2[i-1][j-1].b*(-1) + image2[i-1][j].b*0 +
                 image2[i][j-1].b*0 + image2[i][j].b*0);

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
    //G
            if(i==infoheader.height-1 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i-1][j-1].g*(-1) + image2[i-1][j].g*0 +
                image2[i-1][j+1].g*(1) + image2[i][j-1].g*0 +
                image2[i][j].g*0 + image2[i][j+1].g*0);

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
        if(i==infoheader.height-1 && j==0)
        {
            f1=(image2[i-1][j].g*0 + image2[i-1][j+1].g*(1) +
                image2[i][j].g*0 + image2[i][j+1].g*0);

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }

        if(i==infoheader.height-1 && j==infoheader.width-1)
        {
            f1=( image2[i-1][j-1].g*(-1) + image2[i-1][j].g*0 +
                 image2[i][j-1].g*0 + image2[i][j].g*0);

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
    //=======Schimbare pixeli de pe marginea de sus + colturi=========
        if(i==0 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i+1][j-1].r*(1) + image2[i+1][j].r*(0) +
                image2[i+1][j+1].r*(-1) + image2[i][j-1].r*(0) +
                image2[i][j].r*(0) + image2[i][j+1].r*(0));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
        if(i==0 && j==0)
        {
            f1=(image2[i+1][j].r*(0) + image2[i+1][j+1].r*(-1) +
                image2[i][j].r*(0) + image2[i][j+1].r*(0));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
        if(i==0 && j==infoheader.width-1)
        {
            f1=( image2[i+1][j-1].r*(1) + image2[i+1][j].r*(0) +
                 image2[i][j-1].r*(0) + image2[i][j].r*(0));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
    //B
            if(i==0 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i+1][j-1].b*(1) + image2[i+1][j].b*(0) +
                image2[i+1][j+1].b*(-1) + image2[i][j-1].b*(0) +
                image2[i][j].b*(0) + image2[i][j+1].b*(0));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
        if(i==0 && j==0)
        {
            f1=(image2[i+1][j].b*(0) + image2[i+1][j+1].b*(-1) +
                image2[i][j].b*(0) + image2[i][j+1].b*(0));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
        if(i==0 && j==infoheader.width-1)
        {
            f1=( image2[i+1][j-1].b*(1) + image2[i+1][j].b*(0) +
                 image2[i][j-1].b*(0) + image2[i][j].b*(0));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
    //G
            if(i==0 && j!=0 && j!=infoheader.width-1)
        {
            f1=(image2[i+1][j-1].g*(1) + image2[i+1][j].g*(0) +
                image2[i+1][j+1].g*(-1) + image2[i][j-1].g*(0) +
                image2[i][j].g*(0) + image2[i][j+1].g*(0));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
        if(i==0 && j==0)
        {
            f1=(image2[i+1][j].g*(0) + image2[i+1][j+1].g*(-1) +
                image2[i][j].g*(0) + image2[i][j+1].g*(0));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
        if(i==0 && j==infoheader.width-1)
        {
            f1=( image2[i+1][j-1].g*(1) + image2[i+1][j].g*(0) +
                 image2[i][j-1].g*(0) + image2[i][j].g*(0));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
    //=======Schimbare pixeli de pe marginea din stanga=========
        if(j==0 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].r*(0) + image2[i+1][j+1].r*(-1) +
                image2[i][j+1].r*(0) + image2[i][j].r*(0) +
                image2[i-1][j+1].r*(1) + image2[i-1][j].r*(0));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
    //B
            if(j==0 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].b*(0) + image2[i+1][j+1].b*(-1) +
                image2[i][j+1].b*(0) + image2[i][j].b*(0) +
                image2[i-1][j+1].b*(1) + image2[i-1][j].b*(0));

             if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
    //G
        if(j==0 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].g*(0) + image2[i+1][j+1].g*(-1) +
                image2[i][j+1].g*(0) + image2[i][j].g*(0) +
                image2[i-1][j+1].g*(1) + image2[i-1][j].g*(0));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
    //=======Schimbare pixeli de pe marginea din dreapta========
        if(j==infoheader.width-1 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].r*(0) + image2[i+1][j-1].r*(1) +
                image2[i][j-1].r*(0) + image2[i][j].r*(0) +
                image2[i-1][j-1].r*(-1) + image2[i-1][j].r*(0));

            if(f1>255)
                image[i][j].r=255;
            else
            {
                if(f1<0)
                    image[i][j].r=0;
                else
                    image[i][j].r=f1;
            }
        }
    //B
        if(j==infoheader.width-1 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].b*(0) + image2[i+1][j-1].b*(1) +
                image2[i][j-1].b*(0) + image2[i][j].b*(0) +
                image2[i-1][j-1].b*(-1) + image2[i-1][j].b*(0));

            if(f1>255)
                image[i][j].b=255;
            else
            {
                if(f1<0)
                    image[i][j].b=0;
                else
                    image[i][j].b=f1;
            }
        }
    //G
        if(j==infoheader.width-1 && i!=0 && i!=infoheader.height-1)
        {
            f1=(image2[i+1][j].g*(0) + image2[i+1][j-1].g*(1) +
                image2[i][j-1].g*(0) + image2[i][j].g*(0) +
                image2[i-1][j-1].g*(-1) + image2[i-1][j].g*(0));

            if(f1>255)
                image[i][j].g=255;
            else
            {
                if(f1<0)
                    image[i][j].g=0;
                else
                    image[i][j].g=f1;
            }
        }
    }

    //================Afisare TASK2 F3=======================
    FILE *out_f3=fopen(image_name_f3,"wb");
    if(out_f3==NULL)
        return -1;
    fwrite(&header,sizeof(struct bmp_fileheader),1,out_f3);
    fwrite(&infoheader,sizeof(struct bmp_infoheader),1,out_f3);

    for(i=0;i<header.imageDataOffset-54;i++)
        fwrite(&zero,sizeof(char),1,out_f3);
    for(i=0;i<infoheader.height;i++)
        for(j=0;j<infoheader.width;j++)
        {
            fwrite(&image[i][j].b,sizeof(unsigned char),1,out_f3);
            fwrite(&image[i][j].g,sizeof(unsigned char),1,out_f3);
            fwrite(&image[i][j].r,sizeof(unsigned char),1,out_f3);
        }
    fclose(out_f3);

        //eliberare memorie
    for(i=0;i<infoheader.height;i++)
        free(image2[i]);
    free(image2);

    for(i=0;i<infoheader.height;i++)
        free(image[i]);
    
    //=====================Task 4==============================
   
    FILE *decompress=fopen(image_compressed,"rb");
    if(decompress==NULL)
        return -1;
    short int line;
    short int column;
    unsigned char r,g,b;

        //creare fisier de output TASK 4
    strcpy(image_compressed,"decompressed.bmp");

    //citesc file_header-ul
    fread(&header,14,1,decompress);
    //citesc info_header-ul
    fread(&infoheader,40,1,decompress);
    //setez pointer-ul la matricea de pixeli
    fseek(decompress, header.imageDataOffset, SEEK_SET);

    //aloc memorie pentru stocarea imaginii
    struct pixel **image4;
    image4=(struct pixel**)malloc(infoheader.height*sizeof(struct pixel*));
    for(i=0;i<infoheader.height;i++)
     image4[i]=(struct pixel*)malloc(infoheader.width*sizeof(struct pixel));
 
        fread(&line,sizeof(short int),1,decompress);
        fread(&column,sizeof(short int),1,decompress);

    for(i=infoheader.height-1;i>=0;i--)
        for(j=0;j<infoheader.width;j++)
        {
            if(infoheader.height-line==i && column-1==j)
            {
                fread(&(image4[i][j].r),sizeof(unsigned char),1,decompress);
                fread(&(image4[i][j].g),sizeof(unsigned char),1,decompress);
                fread(&(image4[i][j].b),sizeof(unsigned char),1,decompress);
                r=image4[i][j].r;
                g=image4[i][j].g;
                b=image4[i][j].b;
                fread(&line,sizeof(short int),1,decompress);
                fread(&column,sizeof(short int),1,decompress);
            }
            else
            {
                if((j<column-1 && i==infoheader.height-line) ||
                    j>infoheader.height-line)
                {
                    image4[i][j].r=r;
                    image4[i][j].g=g;
                    image4[i][j].b=b;
                }
            }
        }

    fclose(decompress);
    //================Afisare TASK 4=======================
 
    FILE *out_4=fopen(image_compressed,"wb");
    if(out_4==NULL)
        return -1;
    fwrite(&header,sizeof(struct bmp_fileheader),1,out_4);
    fwrite(&infoheader,sizeof(struct bmp_infoheader),1,out_4);

    for(i=0;i<header.imageDataOffset-54;i++)
        fwrite(&zero,sizeof(char),1,out_4);

    for(i=0;i<infoheader.height;i++)
        for(j=0;j<infoheader.width;j++)
        {
            fwrite(&image4[i][j].b,sizeof(unsigned char),1,out_4);
            fwrite(&image4[i][j].g,sizeof(unsigned char),1,out_4);
            fwrite(&image4[i][j].r,sizeof(unsigned char),1,out_4);
        }
    fclose(out_4);
    //=====================================================
    //eliberare memorie
	for(i=0;i<infoheader.height;i++)
        free(image4[i]);
    free(image4);
    return 0;
}
