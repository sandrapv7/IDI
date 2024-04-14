#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void keyPressEvent (QKeyEvent *event);
    virtual void mousePressEvent (QMouseEvent *event);
    virtual void mouseReleaseEvent (QMouseEvent *event);
    virtual void mouseMoveEvent (QMouseEvent *event);

    virtual void iniEscena ();
    virtual void iniCamera ();

    virtual void iniCamera2 ();

    virtual void paintGL ( );

    virtual void viewTransform ();
    virtual void projectTransform ();

    virtual void viewTransform2 ();
    virtual void projectTransform2 ();

    virtual void TerraTransform();
    virtual void RoadTransform(glm::vec3 pos, float angle);
    virtual void PipeTransform();
    virtual void CarTransform(float radi, float angle, float rot);

    virtual void resizeGL (int width, int height);
    virtual void carregaShaders();

  private:
  
    int printOglError(const char file[], int line, const char func[]);
    float radiEscena;
    float fov, ra, znear, zfar;
    float fov2, ra2, znear2, zfar2;
    glm::vec3 centreEscena, obs, vrp, up;

    glm::vec3 blanc = glm::vec3(1.0,1.0,1.0);
    glm::vec3 verd = glm::vec3(0.0,1.0,0.0);
    glm::vec3 rojo = glm::vec3(1.0,0.0,0.0);
    glm::vec3 azul = glm::vec3(0.0,0.0,1.0);
    GLuint cocheLoc;

    float rot1 = 0;
    float rot2 = 0;
    float rot3 = 0;

    int pressC = 0;
};
