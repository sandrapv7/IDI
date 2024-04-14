#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QTimer>
#include <QTime>
#include <iostream>
#include <QVector>
#include <QList>
#include <QString>
class MyLabel: public QLabel {
    Q_OBJECT
    public:
        MyLabel (QWidget *parent=0);
        int ronda;
        bool turnosimon;
        bool coloreado;
        int inputjugador;
        int posicion;
        int nivel;
        int puntuacio;
        QVector<int> simon;
        QVector<int> jugador;
        QTimer timer;
        int boton;
        int inicio = 0;
        bool ganado = false;

    public slots:
        void iniciar();
        void resetear();
        void mostrarSecuencia();
        void colorear();
        void mirarinput();
        void boton1();
        void boton2();
        void boton3();
        void boton4();
        void valoresiniciales();
        void perdido();

    signals:
        void puntuacion(const QString);

};

#endif // MYLABEL_H
