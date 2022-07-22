#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <getopt.h>

#pragma pack (push, 1)
typedef struct
{
        unsigned short signature;
        unsigned int filesize;
        unsigned short reserved1;
        unsigned short reserved2;
        unsigned int pixelArrOffset;
} BitmapFileHeader;

typedef struct
{
        unsigned int headerSize;
        unsigned int width;
        unsigned int height;
        unsigned short planes;
        unsigned short bitsPerPixel;
        unsigned int compression;
        unsigned int imageSize;
        unsigned int xPixelsPerMeter;
        unsigned int yPixelsPerMeter;
        unsigned int colorsInColorTable;
        unsigned int importantColorCount;
} BitmapInfoHeader;

typedef struct
{
        unsigned char b;
        unsigned char g;
        unsigned char r;
} Rgb;

#pragma pack(pop)

void printFileHeader(BitmapFileHeader header){
        printf("signature:\t%x (%hu)\n", header.signature, header.signature);
        printf("filesize:\t%x (%u)\n", header.filesize, header.filesize);
        printf("reserved1:\t%x (%hu)\n", header.reserved1, header.reserved1);
        printf("reserved2:\t%x (%hu)\n", header.reserved2, header.reserved2);
        printf("pixelArrOffset:\t%x (%u)\n", header.pixelArrOffset, header.pixelArrOffset);
}

void printInfoHeader(BitmapInfoHeader header){
        printf("headerSize:\t%x (%u)\n", header.headerSize, header.headerSize);
        printf("width:     \t%x (%u)\n", header.width, header.width);
        printf("height:    \t%x (%u)\n", header.height, header.height);
        printf("planes:    \t%x (%hu)\n", header.planes, header.planes);
        printf("bitsPerPixel:\t%x (%hu)\n", header.bitsPerPixel, header.bitsPerPixel);
        printf("compression:\t%x (%u)\n", header.compression, header.compression);
        printf("imageSize:\t%x (%u)\n", header.imageSize, header.imageSize);
        printf("xPixelsPerMeter:\t%x (%u)\n", header.xPixelsPerMeter, header.xPixelsPerMeter);
        printf("yPixelsPerMeter:\t%x (%u)\n", header.yPixelsPerMeter, header.yPixelsPerMeter);
        printf("colorsInColorTable:\t%x (%u)\n", header.colorsInColorTable, header.colorsInColorTable);
        printf("importantColorCount:\t%x (%u)\n", header.importantColorCount, header.importantColorCount);
}

void swap(char *a, char *b){
        char t = *a;
        *a = *b;
        *b = t;
}

void swap_st(Rgb* a, Rgb* b) {
        Rgb t = *a;
        *a=*b;
        *b=t;
}

int isNum(char *n) {
    for(int i = 0; i < strlen(n); i++) {
        if (isdigit(n[i]))
            continue;
        else
            return 0;
    }

    return 1;
}

int Defence(int a, int b, int c) {
	if ( (a>=0) && (b>=0) && (a<=b) && (b<c) ) {
		return 1;
	}
	else {
		return 0;
	}
}

int checkLeftRight(int xleft, int xright, int yleft, int yright, int H, int W) {
	if (Defence(xleft, xright, W) && Defence(yright, yleft, H)) {
		return 1;
	} 
	else {
		return 0;
	}
}

int checkSquare(int xleft, int xright, int yleft, int yright) {
    if ((xright-xleft)==(yleft-yright)) {
        return 1;
    }
    else {
        return 0;
    }
}

void Reflection (Rgb **arr, BitmapFileHeader bmfh, BitmapInfoHeader bmif, unsigned int H, unsigned int W, int xleft, int xright, int yleft, int yright, int axis) {
        int count=0;
        switch (axis) {
            case 1:
                for (int i=yright; i<yleft; i++) {
                        for (int count=0; count<((xright-xleft)/2); count++) {
                                swap_st(&arr[i][xleft+count], &arr[i][xright-count]);
                        }
                }
                break;
            case 2:
                while (count<(yleft-yright)/2) {
                        for (int j=xleft; j<xright; j++) {
                                swap_st(&arr[yright+count][j], &arr[yleft-count][j]);
                        }
                        count++;
                }
                break;
            default:
                    printf("Введите, пожалуйста, одну из предложенных цифр\n");
        }
        FILE *ff = fopen("out.bmp", "wb");

        bmif.height = H;
        bmif.width = W;
        fwrite(&bmfh, 1, sizeof(BitmapFileHeader),ff);
        fwrite(&bmif, 1, sizeof(BitmapInfoHeader),ff);
        unsigned int wi = (W) * sizeof(Rgb) + (W)%4;
        for(int i=0; i<H; i++){
                fwrite(arr[i],1,wi,ff);
        }
        fclose(ff);

}

void Copying (Rgb **arr, BitmapFileHeader bmfh, BitmapInfoHeader bmif, unsigned int H, unsigned int W, int xleft, int xright, int yleft, int yright, int xplace, int yplace) {
        Rgb **copy = malloc((yleft-yright)*sizeof(Rgb*));
        for (int i=0; i<(yleft-yright); i++) {
            copy[i] = malloc((xright-xleft) * sizeof(Rgb) + (xright-xleft)%4);
            for (int j=0; j<(xright-xleft); j++) {
                copy[i][j]=arr[yright+i][xleft+j];
            }
        }
	yplace-=(yleft-yright);

	for (int i=yplace; i<(yplace+(yleft-yright)); i++) {
		for (int j=xplace; j<(xplace+(xright-xleft)); j++) {
			arr[i][j]=copy[i-yplace][j-xplace];
		}
	}


        FILE *ff = fopen("out.bmp", "wb");
        bmif.height = H;
        bmif.width = W;
        fwrite(&bmfh, 1, sizeof(BitmapFileHeader),ff);
        fwrite(&bmif, 1, sizeof(BitmapInfoHeader),ff);
        unsigned int wi = (W) * sizeof(Rgb) + (W)%4;
        for(int i=0; i<H; i++){
                fwrite(arr[i],1,wi,ff);
        }
        fclose(ff);
	free(copy);

}

void circle (Rgb **arr, BitmapFileHeader bmfh, BitmapInfoHeader bmif, unsigned int H, unsigned int W, int xleft, int xright, int yleft, int yright, int red, int green, int blue, int r, int count, int xcenter, int ycenter) {
    for (int i=0; i<=count; i++) {
        for (int y=yright; y<=yleft; y++) {
            for (int x=xleft; x<=xright; x++) {
		if ( ((xcenter-x)*(xcenter-x)+(ycenter-y)*(ycenter-y))<=(r*r) && ((xcenter-x)*(xcenter-x)+(ycenter-y)*(ycenter-y))>=((r-1)*(r-1)) ) {
                    arr[y][x].r=red;
                    arr[y][x].g=green;
                    arr[y][x].b=blue;
                }
            }
        }
        r++;
	yleft++;
	yright--;
	xleft--;
	xright++;
    }
    
    FILE *ff = fopen("out.bmp", "wb");
        bmif.height = H;
        bmif.width = W;
        fwrite(&bmfh, 1, sizeof(BitmapFileHeader),ff);
        fwrite(&bmif, 1, sizeof(BitmapInfoHeader),ff);
        unsigned int wi = (W) * sizeof(Rgb) + (W)%4;
        for(int i=0; i<H; i++){
                fwrite(arr[i],1,wi,ff);
        }
        fclose(ff);

}

void circle_fill (Rgb **arr, BitmapFileHeader bmfh, BitmapInfoHeader bmif, unsigned int H, unsigned int W, int xleft, int xright, int yleft, int yright, int red, int green, int blue, int r, int xcenter, int ycenter) {
    while (r>=0) {
        for (int y=yright; y<=yleft; y++) {
            for (int x=xleft; x<=xright; x++) {
                if (((xcenter-x)*(xcenter-x)+(ycenter-y)*(ycenter-y))<(r*r)) {
                    arr[y][x].r=red;
                    arr[y][x].g=green;
                    arr[y][x].b=blue;
                }
            }
        }
        r--;
    }
    
    FILE *ff = fopen("out.bmp", "wb");
        bmif.height = H;
        bmif.width = W;
        fwrite(&bmfh, 1, sizeof(BitmapFileHeader),ff);
        fwrite(&bmif, 1, sizeof(BitmapInfoHeader),ff);
        unsigned int wi = (W) * sizeof(Rgb) + (W)%4;
        for(int i=0; i<H; i++){
                fwrite(arr[i],1,wi,ff);
        }
        fclose(ff);

}

int checkColor (int a) {
    if (a>=0 && a<=255) {
        return 1;
    }
    else {
        return 0;
    }
}

void Square_circle (Rgb **arr, BitmapFileHeader bmfh, BitmapInfoHeader bmif, unsigned int H, unsigned int W, int xleft, int xright, int yleft, int yright, int count, int red, int green, int blue, int variation, int red_fill, int green_fill, int blue_fill)  {
    int r = (xright-xleft)/2;
    int r_remember=r;
    int ycenter=(yleft-r);
    int xcenter = xright-r;
    circle(arr, bmfh, bmif, H, W, xleft, xright, yleft, yright, red, green, blue, r, count, xcenter, ycenter);
        switch (variation) {
            case 1:
		        r_remember--;
                circle_fill(arr, bmfh, bmif, H, W, xleft, xright, yleft, yright, red_fill, green_fill, blue_fill, r_remember, xcenter, ycenter);
                break;
            case 0:
                break;
            default:
                    printf("Введите, пожалуйста, одну из предложенных цифр\n");
        }
}

void circle_radius (Rgb **arr, BitmapFileHeader bmfh, BitmapInfoHeader bmif, unsigned int H, unsigned int W, int xcenter,int ycenter,int r, int count, int variation, int red, int green, int blue, int red_fill, int green_fill, int blue_fill) {
    int xleft, xright, yleft, yright;
    xright=xcenter+r;
    xleft=xcenter-r;
    yleft=ycenter+r;
    yright=ycenter-r;
    int r_remember=r;
    circle(arr, bmfh, bmif, H, W, xleft, xright, yleft, yright, red, green, blue, r, count, xcenter, ycenter);
        switch (variation) {
            case 1:
		        r_remember--;
                circle_fill(arr, bmfh, bmif, H, W, xleft, xright, yleft, yright, red_fill, green_fill, blue_fill, r_remember, xcenter, ycenter);
                break;
            case 0:
                break;
            default:
                    printf("Введите, пожалуйста, одну из предложенных цифр\n");
        }	
}

void printHelp()
{
    printf("Справка\n\n");
    printf("--reflect(-r) - Отражение заданной области по выбранной оси. Координаты задается при помощи -S и -F. Так же выберается ось, по которой происходит отбражение -A\n");
    printf("(Пример 1) file_name.bmp --reflect -S 10 100 -F 100 10 -A 1\n");
    printf("(Пример 2) file_name.bmp -r -S 10 100 -F 100 10 -A 2\n\n");
    printf("--copying(-c) - Копирование заданной области. Участок задается при помощи -S и -F и координат левого верхнего угла области-назначения -P\n");
    printf("(Пример) file_name.bmp --copying -S 10 200 -F 200 10 -P 300 300\n\n");
    printf("--disc_radius(-d) -  Рисование окружности. Окружность определяется при помощи координат ее центра -M  и радиуса -R. Толщина и цвет окантовки окружности выбираются при помощи -С (цвет) и -T (толщина). Заливка окружности (да(1)/нет(0)) и её цвет определяеются при помощи -V (наличие заливки) и -N (цвет)\n");
    printf("(Пример 1) file_name.bmp --disc_radius -M 50 150 -R 20 -C 255 0 0 -T 4 -V 0\n");
    printf("(Пример 2) file_name.bmp -d -M 50 150 -R 20 -C 255 0 0 -T 6 -V 1 -N 0 255 255\n\n");
    printf("--square_circle(-s) -  Рисование окружности. Окружность определяется при помощи -S и -F. Толщина и цвет окантовки окружности выбираются при помощи -С (цвет) и -T (толщина). Заливка окружности (да(1)/нет(0))) и её цвет определяеются при помощи -V (наличие заливки) и -N (цвет)\n");
    printf("(Пример 1) file_name.bmp --square_circle -S 10 100 -F 100 10 -C 255 0 0 -T 4 -V 0\n");
    printf("(Пример 2) file_name.bmp -s -S 10 100 -F 100 10 -C 255 0 0 -T 6 -V 1 -N 0 200 255\n\n");
    printf("--start(-S) - считывет координаты (целый числа) верхнего левого угла прямоугольной области (-S 10 100)\n");
    printf("--finish(-F) - считывет координаты (целый числа) нижнего правого угла прямоугольной области (-F 100 10)\n");
    printf("--placement(-P) - считывает координаты (целый числа) левого верхнего угла области-назначения (-P 200 200)");
    printf("--thickness(-T) - считывание толщины окантовки (-T 4)");
    printf("--color(-C) - считывет цвет окантовки. Цвет определяется тремя числами от 0 до 255. (-C 230 5 10)\n");
    printf("--nuance(-N) - считывет цвет заливки. Цвет определяется тремя числами от 0 до 255. (-N 0 255 10)\n");
    printf("--axis(-A) - считывает ось y (1) или ось x (2) (-A 1)\n");
    printf("--middle(-M) - считывает координаты (целые числа) центра окружности (-M 100 100)\n");
    printf("--variation(-V) - считывает наличие (1) или отсутствие (0) заливки. (-V 1)\n"); 
    printf("--radius(-R) - считывает заначение радиуса (-R 40)\n");
    printf("--info(-i) - выводит информацию о входном файле\n");
    printf("--help(-h) - вызов справки\n");
    printf("Строка команд должна начинаться названием файла ввода\n");

}

int main(int argc, char **argv){

    int xleft=0, xright=0, yleft=0, yright=0, xcenter=0, ycenter=0, r=0, xplace=0, yplace=0;
    int red=0, green=0, blue=0, red_fill=0, green_fill=0, blue_fill=0;
    char* file_name=(char*)calloc(100, sizeof(char));
    int count=0, axis=0, variation=0;
    unsigned int H;
    unsigned int W;
    char *optstring = "rcdsS:F:P:T:C:N:A:M:V:R:ih";
    int flag=0, opt=0;
    int longIndex=0;

 struct option longOpts[] = {
        {"copying", no_argument, &flag, 'c'},
        {"reflect", no_argument, &flag, 'r'},
        {"disc_radius", no_argument, &flag, 'd'},
        {"square_circle", no_argument, &flag, 's'},
        {"help", no_argument, &flag, 'h'},
        {"info",no_argument, &flag, 'i'},
        {"variation", required_argument, NULL, 'V'},
        {"thickness", required_argument, NULL, 'T'},
        {"start", required_argument, NULL, 'S'},
        {"finish", required_argument, NULL, 'F'},
        {"color", required_argument, NULL, 'C'},
        {"nuance", required_argument, NULL, 'N'},
        {"axis", required_argument, NULL, 'A'},
        {"placement", required_argument, NULL, 'P'},
        {"middle", required_argument, NULL, 'M'},
        {"radius", required_argument, NULL, 'R'},
        {NULL, 0, NULL, 0}
    };   
    FILE *f = fopen(argv[1], "rb");
        if (!(f){
            printf("Невозможно открыть файл\n");
            return 0;
        }
        BitmapFileHeader bmfh;
        BitmapInfoHeader bmif;
        fread(&bmfh,1,sizeof(BitmapFileHeader),f);
        fread(&bmif,1,sizeof(BitmapInfoHeader),f);

        H = bmif.height;
        W = bmif.width;

        Rgb **arr = malloc(H*sizeof(Rgb*));
        for(int i=0; i<H; i++){
                arr[i] = malloc(W * sizeof(Rgb) + W%4);
                fread(arr[i],1,W * sizeof(Rgb) + W%4,f);
        }
    if (!(f)) {
        printf("Невозможно открыть файл\n");
        return 0;
    }
    opt = getopt_long(argc, argv, optstring, longOpts, &longIndex);
        while (opt!=-1) {
            switch (opt) {
                case 'S':
                    if (!isNum(optarg)){
                        printf("%s - не целое число\n", optarg);
                        return 0;
                    }

                    if (!isNum(argv[optind])){
                        printf("%s - не целое число\n", argv[optind]);
                        return 0;
                    }

                    xleft = atoi(optarg);
                    yleft = atoi(argv[optind]);
                    break;
                case 'F':
                    if (!isNum(optarg)){
                        printf("%s - не целое число\n", optarg);
                        return 0;
                    }

                    if (!isNum(argv[optind])){
                        printf("%s - не целое число\n", argv[optind]);
                        return 0;
                    }

                    xright = atoi(optarg);
                    yright = atoi(argv[optind]);
                    break;
                case 'R':
                    if(!isNum(optarg)) {
                        printf("%s - это не целое число\n", optarg);
                        return 0;
                    }   
                    r = atoi(optarg);
                    break;       
                case 'M':
                    if (!isNum(optarg)){
                        printf("%s - не целое число\n", optarg);
                        return 0;
                    }
                    if (!isNum(argv[optind])){
                        printf("%s - не целое число\n", argv[optind]);
                        return 0;
                    }
                    xcenter = atoi(optarg);
                    ycenter = atoi(argv[optind]);
                    break;

                case 'C':
                    if (!isNum(optarg)){
                        printf("%s - не целое число\n", optarg);
                        return 0;
                    }
                    if (!isNum(argv[optind])){
                        printf("%s - не целое число\n", argv[optind]);
                        return 0;
                    }
                    if (!isNum(argv[optind+1])){
                        printf("%s - не целое число\n", argv[optind]);
                        return 0;
                    }
                    red=atoi(optarg);
                    green=atoi(argv[optind]);
                    blue=atoi(argv[optind+1]);
                    break;
                case 'N':
                    if (!isNum(optarg)){
                        printf("%s - не целое число\n", optarg);
                        return 0;
                    }
                    if (!isNum(argv[optind])){
                        printf("%s - не целое число\n", argv[optind]);
                        return 0;
                    }
                    if (!isNum(argv[optind+1])){
                        printf("%s - не целое число\n", argv[optind]);
                        return 0;
                    }
                    red_fill=atoi(optarg);
                    green_fill=atoi(argv[optind]);
                    blue_fill=atoi(argv[optind+1]);
                    break;

                case 'A':
                    if (!strcmp("1", optarg) || !strcmp("2", optarg))
                        axis = atoi(optarg);
                    else{
                        printf("Неверно указан метод построения пентаграммы (возможные значения: 1 и 2)\n");
                        return 0;
                    } 
                    break;
                case 'P':
                    if (!isNum(optarg)){
                        printf("%s - не целое число\n", optarg);
                        return 0;
                    }
                    if (!isNum(argv[optind])){
                        printf("%s - не целое число\n", argv[optind]);
                        return 0;
                    }
                    xplace = atoi(optarg);
                    yplace = atoi(argv[optind]);
                    break;
                case 'T':
                    if(!isNum(optarg)) {
                        printf("%s - это не целое число\n", optarg);
                        return 0;
                    }   
                    count = atoi(optarg);
                    count--;
                    break;
                case 'V':
                    if (!strcmp("1", optarg) || !strcmp("0", optarg))
                        variation = atoi(optarg);
                    else{
                        printf("Неверно указан метод построения пентаграммы (возможные значения: 0 и 1)\n");
                        return 0;
                    } 
                    break;
                case 'c':
                    flag = 'c';
                    break;

                case 'r':
                    flag = 'r';
                    break;

                case 'd':
                    flag = 'd';
                    break;

                case 's':
                    flag = 's';
                    break;

                case 'i':
                    flag = 'i';
                    break;

                case 'h':
                    flag = 'h';
                    break;  
                }
            opt = getopt_long(argc, argv, optstring, longOpts, &longIndex);
        }
    
    switch (flag) {
        case 'r':
            if (!(checkLeftRight(xleft, xright, yleft, yright, H, W))) {
                printf("Неверно введены данные\n");
                return 0;
            }
            else {
                Reflection(arr, bmfh, bmif, H, W, xleft, xright, yleft, yright, axis);
            }
            break;
        case 'c':
            if (!(checkLeftRight(xleft, xright, yleft, yright, H, W)) || ((!(checkLeftRight(xplace, xplace, yplace, yplace, H, W))) || (!(checkLeftRight((xplace+(xright-xleft)), (xplace+(xright-xleft)), (yplace-(yleft-yright)), (yplace-(yleft-yright)), H, W))))) {
                printf("Неверно введены данные\n");
                return 0;
            }
            else {
                Copying(arr, bmfh, bmif, H, W,  xleft,  xright,  yleft,  yright,  xplace, yplace);                
            }
            break;
        case 'd':
            xright=xcenter+r;
            xleft=xcenter-r;
            yleft=ycenter+r;
            yright=ycenter-r;
            if (!(checkLeftRight(xleft, xright, yleft, yright, H, W)) || ( (xright+count>W) || (xleft-count<0) || (yleft+count>H) || (yright-count<0) || (count<=-1)) || ( (!(checkColor(red))) || (!(checkColor(green))) || (!(checkColor(blue))) ) || ( (!(checkColor(red_fill))) || (!(checkColor(green_fill))) || (!(checkColor(blue_fill))) ) ) {
                printf("Неверно введены данные\n");
                return 0;
            }
            else {
                circle_radius(arr, bmfh, bmif, H, W, xcenter, ycenter, r, count, variation, red, green, blue, red_fill, green_fill, blue_fill);
            }
            break;
        case 's':
            if (!(checkLeftRight(xleft, xright, yleft, yright, H, W)) || ( (xright+count>W) || (xleft-count<0) || (yleft+count>H) || (yright-count<0) || (count<=-1)) || ( (!(checkColor(red))) || (!(checkColor(green))) || (!(checkColor(blue))) ) || ( (!(checkColor(red_fill))) || (!(checkColor(green_fill))) || (!(checkColor(blue_fill))) ) ) {
                printf("Неверно введены данные\n");
                return 0;
            }
            else {
                Square_circle(arr, bmfh, bmif, H, W, xleft, xright, yleft, yright, count, red, green, blue, variation, red_fill, green_fill, blue_fill);
            }
            break;
        case 'i':
            printFileHeader(bmfh);
            printInfoHeader(bmif);
            break;
        case 'h':
            printHelp();
            break;
    }    
free (arr);    
return 0;
}