#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
      
    void pintaMolinet (glm::vec3 posicioBase, float escala);  
      
    void creaBuffersPal();
    void creaBuffersAspa();
    
    void carregaShaders();
    
    void modelTransformPal(glm::vec3 posicioBase, float escala);
    void modelTransformAspa(glm::vec3 posicioPivot, float angle, float escala);
    
    // program
    QOpenGLShaderProgram *program;
    
    // attribute locations
    GLuint vertexLoc;
    GLuint colorLoc;
    
    // uniform locations
    GLuint TGLoc;

    // VAOs
    GLuint VAOPal;
    GLuint VAOAspa;

    // viewport
    GLint ample, alt;

    glm::vec3 vermell = glm::vec3(1.0, 0.0, 0.0);
    glm::vec3 vermelloscuro = glm::vec3(1.0, 0.0, 0.0) * glm::vec3(0.75, 0.75, 0.75);
    glm::vec3 verd = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 verdoscuro = glm::vec3(0.0, 1.0, 0.0) * glm::vec3(0.75, 0.75, 0.75);
    glm::vec3 blau = glm::vec3(0.0, 0.0, 1.0);
    glm::vec3 blauoscuro = glm::vec3(0.0, 0.0, 1.0) * glm::vec3(0.75, 0.75, 0.75);
    glm::vec3 groc = glm::vec3(1.0, 1.0, 0.0);
    glm::vec3 grocoscuro = glm::vec3(1.0, 1.0, 0.0) * glm::vec3(0.75, 0.75, 0.75);
    glm::vec3 cyan = glm::vec3(0.0, 1.0, 1.0);
    glm::vec3 cyanoscuro = glm::vec3(0.0, 1.0, 1.0) * glm::vec3(0.75, 0.75, 0.75);
    glm::vec3 magenta = glm::vec3(1.0, 0.25, 1.0);
    glm::vec3 magentaoscuro = glm::vec3(1.0, 0.25, 1.0) * glm::vec3(0.75, 0.75, 0.75);



    glm::vec3 marron = glm::vec3(0.5, 0.5, 0.5);
    float rotacion = 0;
    float scl = 0;

    bool inicio = true;
    //glm::vec3 vermell[6];

    /*glm::vec3 vermell[6];  // colors amb R, G i B

    vermell[0] = glm::vec3(1.0, 1.0, 1.0);
    vermell[1] = glm::vec3(1.0, 1.0, 1.0);
    vermell[2] = glm::vec3(1.0, 1.0, 1.0);
    vermell[3] = glm::vec3(0.75, 0.75, 0.75);
    vermell[4] = glm::vec3(0.75, 0.75, 0.75);
    vermell[5] = glm::vec3(0.75, 0.75, 0.75);*/
};
