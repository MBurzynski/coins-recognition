#include <iostream>
#include "opencv/cv.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv/cv.hpp"

using namespace std;
using namespace cv;

//Zmienne
double sr_max;
int gr_1;
int gr_2;
int gr_5;
int gr_10;
int gr_20;
int gr_50;
int zl_1;
int zl_2;
int zl_5;
int rozmycie_slider;
int min_sred_slider;
int max_sred_slider;
int max_nominal_slider;
int kontrast_slider;
int rozmycie=3;
int min_dist=5;
int max_dist=50;
float max_nominal=5;
float kontrast=2;
char wartosc[20];
double max_promien=0;
float ogolna_wartosc=0;


//Stale

double sr_gr_1=15.5;
double sr_gr_2=17.5;
double sr_gr_5=19.5;
double sr_gr_10=16.5;
double sr_gr_20=18.5;
double sr_gr_50=20.5;
double sr_zl_1=23;
double sr_zl_2=21.5;
double sr_zl_5=24;

Mat obraz,wizualizacja;

void oblicz()
{
    gr_1=0;
    gr_2=0;
    gr_5=0;
    gr_10=0;
    gr_20=0;
    gr_50=0;
    zl_1=0;
    zl_2=0;
    zl_5=0;
    max_promien=0;
    Mat obraz;
    obraz = imread("img/7-2.jpg",1);
    if(obraz.empty())
    {
        cout<<"Blad wczytywania";
    }
    pyrDown(obraz,obraz);               //zmniejszenie obrazu
    pyrDown(obraz,obraz);

    wizualizacja=obraz.clone();

    rozmycie=2*rozmycie_slider+1;
    medianBlur(obraz,obraz,rozmycie);

    Mat kopiaHSV,output;

    cvtColor(obraz,kopiaHSV,COLOR_BGR2HSV);                     //do skali HSV
    medianBlur(kopiaHSV,kopiaHSV,15);
    inRange(kopiaHSV,Scalar(0,0,0),Scalar(30,105,255),output);  //Podział na srebrne i złote
    cvtColor(obraz,obraz,CV_BGR2GRAY);                          //konwersja do skali szarości



    vector<int> kolor_srodka;
    vector<int> kolor_obwodki;


    vector<Vec3f> circles;              //wektor okręgów
    HoughCircles(obraz,circles,CV_HOUGH_GRADIENT,3,min_sred_slider,400,80,min_sred_slider,max_sred_slider);         //znajdowanie okręgów

    for( size_t i = 0; i < circles.size(); i++ )                            //rysowanie okręgów
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        kolor_srodka.push_back(output.at<uchar>(center.y, center.x));      //wpisanie do tablicy koloru środka(255-srebrne,0-miedź)
        kolor_obwodki.push_back(output.at<uchar>(center.y, center.x+0.75*radius));      //wpisanie do tablicy koloru obwódki(255-srebrne,0-miedź)

        circle(wizualizacja, center, 2, Scalar(0,255,0), -1, 8,0);          //środek okręgu
        circle(wizualizacja, center, radius, Scalar(0,0,255), 3, 8, 0 );    //obwód
     }


    //ustalenie średnicy maksymalnego nominału

    if(max_nominal==5)
        sr_max=sr_zl_5;
    else if(max_nominal==2)
        sr_max=sr_zl_2;
    else if(max_nominal==1)
        sr_max=sr_zl_1;
    else if(max_nominal==0.5)
        sr_max=sr_gr_50;
    else if(max_nominal==0.2)
        sr_max=sr_gr_20;
    else if(max_nominal==0.1)
        sr_max=sr_gr_10;
    else if(max_nominal==0.05)
        sr_max=sr_gr_5;
    else if(max_nominal==0.02)
        sr_max=sr_gr_2;
    else if(max_nominal==0.01)
        sr_max=sr_gr_1;



    for(int i=0;i<circles.size();i++)       //znalezienie najwiekszego promienia
    {
        if(circles[i][2]>max_promien)
            max_promien=circles[i][2];
    }

        //Rozpoznawanie nominałów po średnicy i kolorze

    for(int i=0;i<circles.size();i++)
    {
        cout<<circles[i][2]<<endl;

        if(circles[i][2]<=(sr_gr_10/(sr_max))*max_promien && kolor_srodka[i]==0)
        {

            gr_1++;
            putText(wizualizacja,"1gr",Point(circles[i][0]-10,circles[i][1]+10),FONT_HERSHEY_SIMPLEX,0.8,Scalar(0,0,255),2);

        }
        else if(circles[i][2]>(sr_gr_1/(sr_max)-0.05)*max_promien && circles[i][2]<=(sr_gr_2/(sr_max))*max_promien && kolor_srodka[i]==255)
        {
            gr_10++;
            putText(wizualizacja,"10gr",Point(circles[i][0]-10,circles[i][1]+10),FONT_HERSHEY_SIMPLEX,0.8,Scalar(0,0,255),2);
        }


        else if(circles[i][2]>(sr_gr_10/(sr_max))*max_promien && circles[i][2]<=(sr_gr_20/(sr_max))*max_promien && kolor_srodka[i]==0)
        {
            gr_2++;
            putText(wizualizacja,"2gr",Point(circles[i][0]-10,circles[i][1]+10),FONT_HERSHEY_SIMPLEX,0.8,Scalar(0,0,255),2);

        }
        else if(circles[i][2]>(sr_gr_2/(sr_max))*max_promien && circles[i][2]<=(sr_gr_5/(sr_max))*max_promien && kolor_srodka[i]==255)
        {
            gr_20++;
            putText(wizualizacja,"20gr",Point(circles[i][0]-10,circles[i][1]+10),FONT_HERSHEY_SIMPLEX,0.8,Scalar(0,0,255),2);

        }
        else if(circles[i][2]>(sr_gr_20/(sr_max))*max_promien && circles[i][2]<=(sr_gr_50/(sr_max))*max_promien && kolor_srodka[i]==0)
        {
            gr_5++;
            putText(wizualizacja,"5gr",Point(circles[i][0]-10,circles[i][1]+10),FONT_HERSHEY_SIMPLEX,0.8,Scalar(0,0,255),2);

        }
        else if(circles[i][2]>(sr_gr_5/(sr_max))*max_promien && circles[i][2]<=(sr_zl_2/(sr_max))*max_promien && kolor_srodka[i]==255 && kolor_obwodki[i]==255)
        {
            gr_50++;
            putText(wizualizacja,"50gr",Point(circles[i][0]-10,circles[i][1]+10),FONT_HERSHEY_SIMPLEX,0.8,Scalar(0,0,255),2);

        }
        else if(circles[i][2]>(sr_gr_50/(sr_max))*max_promien && circles[i][2]<=(sr_zl_1/(sr_max))*max_promien && kolor_srodka[i]==255 && kolor_obwodki[i]==0)
        {
            zl_2++;
            putText(wizualizacja,"2zl",Point(circles[i][0]-10,circles[i][1]+10),FONT_HERSHEY_SIMPLEX,0.8,Scalar(0,0,255),2);

        }
        else if(circles[i][2]>(sr_zl_2/(sr_max))*max_promien && circles[i][2]<=(sr_zl_5/(sr_max))*max_promien && kolor_srodka[i]==255 && kolor_obwodki[i]==255)
        {
            zl_1++;
            putText(wizualizacja,"1zl",Point(circles[i][0]-10,circles[i][1]+10),FONT_HERSHEY_SIMPLEX,0.8,Scalar(0,0,255),2);

        }
        else if(circles[i][2]>(sr_zl_1/(sr_max))*max_promien && circles[i][2]<=(sr_zl_5/(sr_max)+0.03)*max_promien && kolor_srodka[i]==0)
        {
            zl_5++;
            putText(wizualizacja,"5zl",Point(circles[i][0]-10,circles[i][1]+10),FONT_HERSHEY_SIMPLEX,0.8,Scalar(0,0,255),2);

        }

    }

    cout<<"Liczba 1 groszowek: "<<gr_1<<endl;
    cout<<"Liczba 2 groszowek: "<<gr_2<<endl;
    cout<<"Liczba 5 groszowek: "<<gr_5<<endl;
    cout<<"Liczba 10 groszowek: "<<gr_10<<endl;
    cout<<"Liczba 20 groszowek: "<<gr_20<<endl;
    cout<<"Liczba 50 groszowek: "<<gr_50<<endl;
    cout<<"Liczba 1 zlotowek: "<<zl_1<<endl;
    cout<<"Liczba 2 zlotowek: "<<zl_2<<endl;
    cout<<"Liczba 5 zlotowek: "<<zl_5<<endl;
    ogolna_wartosc=gr_1*0.01+gr_2*0.02+gr_5*0.05+gr_10*0.1+gr_20*0.2+gr_50*0.5+zl_1*1+zl_2*2+zl_5*5;
    cout<<"Ogolna wartosc: "<<ogolna_wartosc<<endl;


    //Wpisanie wartości monet na zdjęcie

    sprintf(wartosc,"Ogolna wartosc= %.2f zl",ogolna_wartosc);
    putText(wizualizacja,wartosc,Point(50,50),FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,255),2);



    imshow("Rozpoznawanie nominalow monet",wizualizacja);
}

void on_trackbar1(int,void*)
{
    oblicz();

}

void on_trackbar2(int,void*)
{
    oblicz();

}

void on_trackbar3(int,void*)
{
    oblicz();

}

void on_trackbar4(int,void*)
{
    if(max_nominal_slider==0)
        max_nominal=0.01;
    else if(max_nominal_slider==1)
        max_nominal=0.02;
    else if(max_nominal_slider==2)
        max_nominal=0.05;
    else if(max_nominal_slider==3)
        max_nominal=0.1;
    else if(max_nominal_slider==4)
        max_nominal=0.2;
    else if(max_nominal_slider==5)
        max_nominal=0.5;
    else if(max_nominal_slider==6)
        max_nominal=1;
    else if(max_nominal_slider==7)
        max_nominal=2;
    else if(max_nominal_slider==8)
        max_nominal=5;


    oblicz();

}




int main( int argc, char** argv )
{

    namedWindow( "Rozpoznawanie nominalow monet");

    /// Poczatkowe wartosci
    rozmycie_slider = 3;
    min_sred_slider = 28;
    max_sred_slider = 65;
    max_nominal_slider = 8;



    /// Trackbars
    char rozmycie_trackbar[10];
    char max_sred_trackbar[50];
    char min_sred_trackbar[50];
    char max_nominal_trackbar[100];
    sprintf(rozmycie_trackbar,"Rozmycie");
    sprintf(max_sred_trackbar,"Max srednica");
    sprintf(min_sred_trackbar,"Min srednica");
    sprintf(max_nominal_trackbar,"Max nominal");


    createTrackbar( rozmycie_trackbar, "Rozpoznawanie nominalow monet", &rozmycie_slider, 3, on_trackbar1 );
    createTrackbar( max_sred_trackbar, "Rozpoznawanie nominalow monet", &max_sred_slider, 80, on_trackbar2 );
    createTrackbar( min_sred_trackbar, "Rozpoznawanie nominalow monet", &min_sred_slider, 30, on_trackbar3 );
    createTrackbar( max_nominal_trackbar, "Rozpoznawanie nominalow monet", &max_nominal_slider, 8, on_trackbar4 );


    on_trackbar1(rozmycie_slider,0);
    on_trackbar2(max_sred_slider,0);
    on_trackbar3(min_sred_slider,0);
    on_trackbar4(max_nominal_slider,0);


    waitKey(0);
    return 0;
}
