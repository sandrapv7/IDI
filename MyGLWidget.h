// MyGLWidget.h
#include "LL4GLWidget.h"
#include <QTimer>
class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();
    float angleX;
    float factorAngleX;
    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void iniMaterialTerra ();
    virtual void viewTransform();
    virtual void projectTransform();
    virtual void paintGL();
    virtual void modelTransformTerra ();
    virtual void modelTransformTrack (float radi, float rotY);
    virtual void modelTransformMineCart (float angle);
    virtual void modelTransformFantasma (float angle);
    virtual void modelTransformFocus();
    virtual void carregaShaders();
    virtual void iniFocus();
    virtual void initializeGL ();
    QTimer timer;


    int press;
    GLuint posFocusLoc;
    GLuint colFocus0Loc;
    GLuint colFocus1Loc;
    GLuint colFocus2Loc;
    glm::vec3 colFocus0;
    glm::vec3 colFocus1;
    glm::vec3 colFocus2;
    float fov;
    int start;
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
  private:
    int printOglError(const char file[], int line, const char func[]);
    float rotacion = 0;
  public slots:
    virtual void rotarcoche();
};
