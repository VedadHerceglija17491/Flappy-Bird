#include "mbed.h"
#define dp23 P0_0
 
 
#include "stdio.h"
#include "SPI_TFT_ILI9341.h"
#include "string"
#include "Arial12x12.h"
#include "Arial24x23.h"
#include "Arial28x28.h"
#include "font_big.h"
//#include "SDFileSystem.h"
 
SPI_TFT_ILI9341 TFT(dp2,dp1,dp6,dp24,dp23,dp25,"TFT"); // mosi, miso, sclk, cs, reset, dc
AnalogIn ulaz(dp9);
#define precnik 10
int visina_ptice = 100;
bool restart = true;

// Analogni ulazi sa dzojstika
AnalogIn VRx ( dp11 );
AnalogIn VRy ( dp10 );
// Taster sa dzojstika
InterruptIn SW ( dp9 );
InterruptIn taster1(dp1);
Timer provjera;

void crtaj_pipe(int x, int visina){ // funkcija crta pipe oko tacke x visine visina
        TFT.fillrect(x-20,0,x+20,visina,Green);
        TFT.fillrect(x-20,visina+65,x+20,240,Green);
        //brisanje
        TFT.fillrect(x+20,0,x+21,240,Blue); 
        //TFT.fillrect(x-21,0,x-20,240,Blue);
        TFT.fillrect(0,0,3,240,Blue);
    }
void crtaj_pticu(int yStaro, int yNovo){
        //brisanje
        TFT.fillcircle(50,yStaro,precnik,Blue);
        //crtanje
        TFT.fillcircle(50,yNovo,precnik,Yellow);
    }  
    void crtaj_pocetni(){
        TFT.locate (130 ,110);
        TFT.set_font (( unsigned char *) Arial12x12 );
        printf ("WELCOME!");
        TFT.locate (65 ,210);
        printf ("This game was made by");
        TFT.locate (10 , 225);
        printf ("Haris Ajdinovic and Vedad Herceglija in 2018");
        wait(2);
    }
     void crtaj_restart(){
        TFT.locate (130 ,110);
        TFT.set_font (( unsigned char *) Arial12x12 );
        printf ("Up Joystick to restart game");
        //wait(2);
    }
    void high_score(int current_score){
       // TFT.fillrect(0,0,120,16,Red);
        TFT.locate (2,2);
        TFT.set_font (( unsigned char *) Arial12x12 );
        printf ("CURRENT SCORE: %d", current_score);
    }


void skoci() {
    provjera.start(); //kod za debauncing
    if(provjera.read_ms()>1)
    {  
        provjera.reset();
        provjera.stop();
        crtaj_pticu(visina_ptice, visina_ptice -= 17);
    }
}
void restartuj() {
    provjera.start(); //kod za debauncing
    if(provjera.read_ms()>1)
    {  
        provjera.reset();
        provjera.stop();
        restart = true;
    }
}
 bool jeLiUdario(int v){
     return ((visina_ptice - (precnik)) <= v or (visina_ptice+(precnik)) >= v+65);
}
    


int main()
{
    //Inicijalizacija;
    TFT.claim(stdout);
    TFT.set_orientation(1);
    TFT.background(Blue);     
    TFT.foreground(Black);
    // Aktivacija pull - up otpornika
    SW . mode ( PullUp );

    TFT.cls();
    taster1.fall(&skoci);
    SW.fall(&restartuj);

    int brojac = 0;
    int score = 1;
    int i=30,j=150,k=270;
     high_score(score);
    while(1){
        brojac=0;
        if(restart){
        crtaj_pocetni();
        TFT.cls();
        restart = false;
        while(1) {
             if ( VRy >2.0/3.0) 
             skoci();
            brojac++;
            i%=347;
            j%=347;
            k%=347;
            visina_ptice%=240;
            crtaj_pipe(320 - i+10,50); 
            crtaj_pipe(320 - j+10,70);
            crtaj_pipe(320 - k+10,90);
            i+=2,j+=2,k+=2;  //ako bude trebalo ubrzati, povecaj inkrement
            crtaj_pticu(visina_ptice,visina_ptice += 4);
            if((320-i+10+20) >= 50 and (320-i+10-20) <= 50)
                if(jeLiUdario(50)){
               
                    break;
                    }
            if((320-j+10+20) >= 50 and (320-j+10-20) <= 50)
                if(jeLiUdario(70))
               
                    break;
            if((320-k+10+20) >= 50 and (320-k+10-20) <= 50)
                if(jeLiUdario(90))
                    break;
            if(brojac == 40)
            {
                score++;
                brojac = 0;
                high_score(score);
            }
            high_score(score);
            
        // TFT.cls();
        }
        TFT.cls();
        while(1){
        crtaj_restart();
        if ( VRy >1.0/3.0)
        {
             restartuj();
               TFT.cls();
             break;
             
        }
        
        }
        }
    }
}
 