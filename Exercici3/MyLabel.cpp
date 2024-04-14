#include "MyLabel.h"

MyLabel::MyLabel (QWidget *parent):QLabel(parent){
    connect(&timer, SIGNAL(timeout()),this,SLOT(colorear()));
}

void MyLabel::iniciar(){
    if (inicio == 0) {
        valoresiniciales();
        int color = 1+rand()%(5-1);
        simon.append(color);
        setStyleSheet("background-color : rgb(255,255,255); color : rgb(0,0,0)");
        setText("ES EL TURNO DE SIMON");
        mostrarSecuencia();
    }
}

void MyLabel::valoresiniciales() {
    inicio = 1;
    nivel = 0;
    posicion = 0;
    ronda = 0;
    boton = 0;
    ganado = false;
    simon.clear();
    emit puntuacion("0");
    turnosimon = true;
    puntuacio = 0;
    coloreado = false;
    inputjugador = 0;
}

void MyLabel::resetear(){
    inicio = 0;
    timer.stop();
    emit puntuacion("0");
    if (ganado) {
        setStyleSheet("background-color : rgb(255,255,255); color : rgb(0,0,0)");
        setText("HAS GANADO");

    }

    else{
        setStyleSheet("background-color : rgb(255,255,255); color : rgb(0,0,0)");
        setText("INICIA LA PARTIDA");
    }
}

void MyLabel::perdido(){
    inicio = 0;
    setText("HAS PERDIDO");
}



void MyLabel::mostrarSecuencia(){
    if (posicion < simon.size() and coloreado) {
        timer.start(1000);
    }
    else if (posicion < simon.size() and not coloreado) {
        timer.start(1000);
    }
    else {
        setStyleSheet("background-color : rgb(255,255,255); color : rgb(0,0,0)");
        setText("ES TU TURNO");
        turnosimon = false;
    }

}

void MyLabel::colorear(){
    if (not coloreado) {
        if (simon[posicion] == 1) setStyleSheet("background-color : rgb(255,0,0); color : rgb(255,0,0)");
        else if (simon[posicion] == 2) setStyleSheet("background-color : rgb(0,255,0); color : rgb(0,255,0)");
        else if (simon[posicion] == 3) setStyleSheet("background-color : rgb(0,0,255); color : rgb(0,0,255)");
        else if (simon[posicion] == 4) setStyleSheet("background-color : rgb(255,255,0); color : rgb(255,255,0)");
        coloreado = true;
    }

    else {
        setStyleSheet("background-color : rgb(255,255,255); color : rgb(255,255,255)");
        coloreado = false;
        ++posicion;
    }
    timer.stop();
    mostrarSecuencia();
}

void MyLabel::mirarinput(){
    //Si es el turno del jugador
    if (not turnosimon) {

        //Si coinciden los numeros y ha llegado al final de la secuencia
        if (simon[nivel] == boton and inputjugador - 1 == ronda) {
           ++ronda;                                 //incrementa la ronda
           ++puntuacio;                             //gana puntuacion
           QString string = QString::number(puntuacio);
           emit puntuacion(string);
           int color = 1+rand()%(5-1);
           simon.append(color);                     //añado un nuevo color al vector
           inputjugador = 0;
           nivel = 0;                               //volvemos al inicio del vector
           turnosimon = true;
           posicion = 0;
           if (puntuacio == 10) {
               ganado = true;
               resetear();
           }
           else mostrarSecuencia();
        }

        //Si los resultados no coinciden
        else if (simon[nivel] != boton ) {
           emit puntuacion("0");
           perdido();
        }

        //Si ha hecho bien el color pero aun no ha llegado al final de la secuencia pasamos al siguiente valor del vector.
        else {
           ++nivel;
        }
    }
}

void MyLabel::boton1(){
   boton = 1;
   ++inputjugador;
   mirarinput();
}

void MyLabel::boton2(){
   boton = 2;
   ++inputjugador;
   mirarinput();

}

void MyLabel::boton3(){
   boton = 3;
   ++inputjugador;
   mirarinput();

}

void MyLabel::boton4(){
   boton = 4;
   ++inputjugador;
   mirarinput();

}


