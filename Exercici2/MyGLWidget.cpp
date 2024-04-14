#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

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
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::iniEscena()
{
  pressC = 0;
  radiEscena = sqrt(20*20+20*20+3*3)/2;
  centreEscena = glm::vec3(0,1.5,0);
}

void MyGLWidget::iniCamera(){
  factorAngleX = 45;
  factorAngleY = 0;
  fov = float(M_PI)/4.0f;
  znear =  1;
  zfar  = 40;
  viewTransform();
}


void MyGLWidget::iniCamera2(){
  glm::mat4 car(1.0f);
  car = glm::rotate(car, rot2, glm::vec3(0,1,0));
  glm::vec4 iobs = car*glm::vec4(6.20,0.5,0.45,1);
  glm::vec4 ivrp = car*glm::vec4(6.20,0.5,-0.05,1);
  obs = glm::vec3(iobs.x,iobs.y,iobs.z);
  vrp = glm::vec3(ivrp.x,ivrp.y,ivrp.z);
  up = glm::vec3(0, 1, 0);
  fov2 = 2*asin(radiEscena/(2*radiEscena));
  znear2 = 0.25;
  zfar2  = 30;
  ra2 = ra;
  viewTransform2();
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  Proj = glm::perspective (fov, ra, znear, zfar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}


void MyGLWidget::projectTransform2 ()
{
  glm::mat4 Proj(1.0f);
  Proj = glm::perspective (fov2, ra2, znear2, zfar2);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform ()
{
  glm::mat4 View(1.0);
  View = glm::translate(View, glm::vec3(0., 0., -2*radiEscena));
  View = glm::rotate(View, factorAngleX, glm::vec3(1., 0., 0.));
  View = glm::rotate(View, -factorAngleY, glm::vec3(0., 1., 0.));
  View = glm::translate(View, -centreEscena);
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::viewTransform2 ()
{
  glm::mat4 View(1.0f);
  View = glm::lookAt (obs, vrp, up);
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::resizeGL (int w, int h)
{
  ample = w;
  alt = h;

  ra = float(ample)/float(alt);

  if (pressC == 0) {
      ra2 = ra;
      float fovini = fov;
      fov = 2*atan(tan(fovini/2.0)/ra);
      projectTransform();
      glViewport (0, 0, ample, alt);
  }
  else {
      if (ra > 1) ra2 = ra;
      projectTransform2();
  }

}

void MyGLWidget::paintGL ()
{
  glViewport (0, 0, ample, alt);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Road
  glBindVertexArray (VAO_models[ROAD]);
  RoadTransform(glm::vec3(4, 0.01, -4),0.0f);
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);//BIEN

  glBindVertexArray (VAO_models[ROAD]);
  RoadTransform(glm::vec3(4, 0.01, 4),-M_PI/2);
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);colorLoc = glGetUniformLocation(program->programId(), "color");

  glBindVertexArray (VAO_models[ROAD]);
  RoadTransform(glm::vec3(-4, 0.01, -4),M_PI/2);
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);

  glBindVertexArray (VAO_models[ROAD]);
  RoadTransform(glm::vec3(-4, 0.01, 4),-M_PI);
  glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);

  // Car

  glBindVertexArray (VAO_models[CAR]);
  CarTransform(6.2f,float(M_PI*2),rot2);
  glUniform3fv(cocheLoc, 1, &verd[0]);
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);

  glBindVertexArray (VAO_models[CAR]);
  CarTransform(7.5f,float(M_PI*2),rot3);
  glUniform3fv(cocheLoc, 1, &rojo[0]);
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);

  glBindVertexArray (VAO_models[CAR]);
  CarTransform(4.5f,float(M_PI*2),rot1);
  glUniform3fv(cocheLoc, 1, &azul[0]);
  glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);

  glUniform3fv (cocheLoc,1,&blanc[0]);

  // Pipe
  glBindVertexArray (VAO_models[PIPE]);
  PipeTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);

  // Terra update();
  glBindVertexArray (VAO_Terra);
  TerraTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::TerraTransform ()
{
  float escala1 = 20/8;
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG,glm::vec3(escala1,escala1,escala1));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::RoadTransform (glm::vec3 pos, float angle)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::scale(TG, glm::vec3(20/escalaModels[ROAD]));
  TG = glm::rotate(TG, angle, glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseModels[ROAD]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::PipeTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(0,0,0));
  TG = glm::scale(TG, glm::vec3(3*escalaModels[PIPE]));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::CarTransform (float radi, float angle, float rot)
{
  glm::mat4 TG(1.0f);
  TG = glm::rotate(TG, rot, glm::vec3(0,1,0));
  TG = glm::translate(TG, glm::vec3(radi,0,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[CAR]));
  TG = glm::rotate(TG, angle , glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseModels[CAR]);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
void MyGLWidget::mousePressEvent (QMouseEvent *e)
{
  xClick = e->x();
  yClick = e->y();

  if (e->button() & Qt::LeftButton &&
      ! (e->modifiers() & (Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *)
{
  DoingInteractive = NONE;
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
  update ();
}


void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { 
      rot1 += glm::radians(4.0f);
      rot2 += glm::radians(3.0f);
      rot3 += glm::radians(2.0f);
      if (pressC == 1) iniCamera2();
      break;
    }
    case Qt::Key_C: { 
      pressC = 1;
      iniCamera2();
      projectTransform2();
      viewTransform2();
      break;
        }
    case Qt::Key_R: { 
      pressC = 0;
      iniCamera();
      projectTransform();
      viewTransform();
      break;
        }  
    case Qt::Key_T: { 
      break;
        }          
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Identificador per als  atributs
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");
  cocheLoc = glGetUniformLocation (program->programId(), "ColorCoche");

  // Identificadors dels uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc  = glGetUniformLocation (program->programId(), "Proj");
  viewLoc  = glGetUniformLocation (program->programId(), "View");

}


