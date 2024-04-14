// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {

}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:{
        // Donem valors al material del terra
        amb = glm::vec3(1.0,0.0,0.0);
        diff = glm::vec3(0.4,0.0,0.0);
        spec = glm::vec3(0.8,0.0,0.0);
        shin = 100;
      }

            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }      // FANTASMA

    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  carregaShaders();

  creaBuffersTrack();
  creaBuffersFantasma();
  creaBuffersMineCart();
  creaBuffersTerra();

  iniEscena();
  iniCamera();
  iniFocus();
  connect(&timer,SIGNAL(timeout()), this, SLOT(rotarcoche()));
}
void MyGLWidget::iniMaterialTerra ()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.1,0.1,0.1);
  diff = glm::vec3(0.4,0.0,0.0);
  spec = glm::vec3(0.8,0.8,0.8);
  shin = 100;
}

void MyGLWidget::iniEscena ()
{
  centreEsc = glm::vec3(0,2,0);
  radiEsc = sqrt(30*30+30*30+4*4)/2;
  press = 0;
  start = 0;

}

void MyGLWidget::iniCamera ()
{
  angleY = 0.0;
  angleX = 0.0;
  fov = float(M_PI)/4.0f;
  viewTransform();
}

void MyGLWidget::iniFocus() {
  colFocus0 = glm::vec3(0.6,0.6,0);
  glUniform3fv(colFocus0Loc,1, &colFocus0[0]);
  colFocus1 = glm::vec3(0.6,0.6,0.0);
  glUniform3fv(colFocus1Loc,1, &colFocus1[0]);
  colFocus2 = glm::vec3(0.8,0.8,0.8);
  glUniform3fv(colFocus2Loc,1, &colFocus2[0]);
}


void MyGLWidget::viewTransform ()
{
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, factorAngleX, glm::vec3(1., 0., 0.));
  View = glm::rotate(View, -factorAngleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  Proj = glm::perspective(fov, ra, radiEsc, 3.0f*radiEsc);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  float escala1 = 30/10;
  glm::mat4 TG = glm::mat4(1.f);  // Matriu de transformació
  TG = glm::scale(TG,glm::vec3(escala1,escala1,escala1));
  TG = glm::translate(TG, glm::vec3(-5,0,-5));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTrack (float radi, float rotY)
{
  glm::mat4 TG;
  TG = glm::rotate(glm::mat4(1.f), float(glm::radians(rotY)), glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(radi,0,0));
  TG = glm::rotate(TG, float(glm::radians(float(90))), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaTrack, escalaTrack, escalaTrack));
  TG = glm::translate(TG, -centreBaseTrack);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::modelTransformMineCart (float angle)
{
  glm::mat4 TG(1.0f);
  TG = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(10,0,0));
  TG = glm::rotate(TG, float(M_PI*2), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaMineCart, escalaMineCart, escalaMineCart));
  TG = glm::translate(TG, -centreBaseMineCart);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma (float angle)
{
  glm::mat4 TG;
  TG = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(10,1,0));
  TG = glm::rotate(TG, float(M_PI), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaFantasma*3/4, escalaFantasma*3/4, escalaFantasma*3/4));
  TG = glm::translate(TG, -centreBaseFantasma);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::rotarcoche () {
    rotacion += glm::radians(5.0f);
    update();
}
void MyGLWidget::paintGL ()
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  modelTransformFocus();
  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // TRACK
  float angulo = 0;
  for (int i = 0; i < 90; ++i) {
      glBindVertexArray (VAO_Track);
      angulo = angulo + float(4);
      modelTransformTrack (10, angulo);
      glDrawArrays(GL_TRIANGLES, 0, track.faces().size()*3);
  }

  if (press == 1) {
      // FANTASMA
      glBindVertexArray (VAO_Fantasma);
      modelTransformFantasma (rotacion);
      glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);
  }

  // MINE CART
  glBindVertexArray (VAO_MineCart);
  modelTransformMineCart (rotacion);
  glDrawArrays(GL_TRIANGLES, 0, mineCart.faces().size()*3);

  glBindVertexArray(0);
}
void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    makeCurrent();
    if (DoingInteractive == ROTATE) {
        factorAngleY -= (e->x()-xClick)* M_PI/ample;
        factorAngleX += (e->y()-yClick)* M_PI/alt;
    }
    xClick = e->x();
    yClick = e->y();
    viewTransform();
    update();

}


void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
    rotacion += glm::radians(5.0f);
    break;
	}
  case Qt::Key_L: {
    if (press == 0) {
        press = 1;
        glClearColor(0, 0, 0, 0);
        colFocus2 = glm::vec3(0,0,0);
        glUniform3fv(colFocus2Loc,1, &colFocus2[0]);
    }
    else {
        press = 0;
        glClearColor(0.5, 0.7, 1.0, 1.0);
        colFocus2 = glm::vec3(0.8,0.8,0.8);
        glUniform3fv(colFocus2Loc,1, &colFocus2[0]);
    }
    break;
	}
  case Qt::Key_R: {
      // ...
    break;
	}
  case Qt::Key_S: {
    if (start == 0) {
        timer.start(50);
        start = 1;
    }
    else {
        timer.stop();
        start = 0;
    }
    break;
	}	
  default:  event->ignore(); break;
  }
  update();
}

void MyGLWidget::modelTransformFocus ()
{
  glm::mat4 TG;
  TG = glm::rotate(glm::mat4(1.f), rotacion, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(10,0,0));
  //TG = glm::rotate(TG, float(M_PI*2), glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseMineCart);
  glUniformMatrix4fv (posFocusLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("./shaders/basicLlumShader.frag");
  vs.compileSourceFile("./shaders/basicLlumShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “normal” del vertex shader
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  // Obtenim identificador per a l'atribut “matamb” del vertex shader
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  // Obtenim identificador per a l'atribut “matdiff” del vertex shader
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  // Obtenim identificador per a l'atribut “matspec” del vertex shader
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  // Obtenim identificador per a l'atribut “matshin” del vertex shader
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "Proj");
  viewLoc = glGetUniformLocation (program->programId(), "View");
  posFocusLoc = glGetUniformLocation (program->programId(), "posFocus");
  colFocus0Loc = glGetUniformLocation (program->programId(), "colFocus0");
  colFocus1Loc = glGetUniformLocation (program->programId(), "colFocus1");
  colFocus2Loc = glGetUniformLocation (program->programId(), "colFocus2");
}
