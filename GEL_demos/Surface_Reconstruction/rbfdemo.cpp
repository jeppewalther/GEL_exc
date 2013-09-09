/* GEL demo program.
 
    This program illustrates height field reconstruction from points and implicit
    surface polygonization. This should be a good starting point for exercises
    in reconstruction using radial basis functions. Please refer to the keyboard
    function for a list of hotkeys.
 
    Andreas Bærentzen 2012
    
 */

#ifdef WIN32
#include <Windows.h>
#endif

#include <GL/glew.h>
#include <GLGraphics/gel_glut.h>

#include <iostream>
#include <CGLA/eigensolution.h>
#include <CGLA/Vec2d.h>
#include <CGLA/Vec3d.h>
#include <CGLA/Mat3x3d.h>
#include <CGLA/Mat2x2d.h>
#include <CGLA/Mat2x3d.h>
#include <Util/Timer.h>

#include <GLGraphics/GLViewController.h>
#include <GLGraphics/draw.h>
#include <Util/ArgExtracter.h>

//#include <HMesh/HalfEdgeWalker.h>
#include <HMesh/x3d_save.h>
#include <HMesh/x3d_load.h>
#include <HMesh/Manifold.h>
#include <HMesh/triangulate.h>
//#include <HMesh/caps_and_needles.h>

#include <Geometry/RGrid.h>
#include <Geometry/Polygonizer.h>
#include <Geometry/KDTree.h>


#include <LinAlg/Matrix.h>
#include <LinAlg/Vector.h>
#include <LinAlg/LapackFunc.h>
#include <LinAlg/LinAlgIO.h>


#include <Util/string_utils.h>

using namespace std;
using namespace HMesh;
using namespace Geometry;
using namespace GLGraphics;
using namespace CGLA;
using namespace Util;
using namespace LinAlg;


namespace
{
	GLViewController *view_ctrl;       // This is the class used to help us navigate with the mouse (virtual trackball)
	int WINX = 800, WINY = 800;        // Predefine window dimensions
	Manifold mani;					   // The mesh we view
	int do_wire = false;			   // Whether or not to use wireframe
	bool create_display_list = true;   // used to check whether we need to redo the display list (for efficient rendering).
	
	// Below a set of points used to define a surface.
	Vec3f pts[6] = 
	{
	Vec3f(1,1,1),
  Vec3f(10,15,20),
	Vec3f(9,8,13),
	Vec3f(-12,7,7),
	Vec3f(-10,-10,30),
	Vec3f(12,-7,10)
	};

	
}
/*
int main(int argc, char **argv)
{

  //KDTree<Vec3f,int> tree;
   // for(int i=0;i<5;++i)
   //     {
   //         Vec3f p0(1,2,3);
   //         tree.insert(p0, i);
   //     }
  //tree.build();
  //
 // string a = " hej ";
 // string b = " hej ";
 // trim(a, b);
//
  //cout << a << b << endl;
  
  
  CMatrix A(3.0,3.0);
  //CVector h(6.0);
  A.set(0,0,0)

  for(int i =0; i<6; ++i){
    A.set(i,0,0);
    A.set(i,1,1);
    A.set(i,2,2);
    A.set(i,3,3);
    A.set(i,4,4);
    h[i] = 7;
  }
  //LinAlg::Invert(A);

  CVector x = LinearSolve(A,h);
  
  
  cout << x << endl;
  
}
*/

void polygonize(Manifold & m, ImplicitFunction & imp, int N,
                const Vec3f& _p0, const Vec3f& _p7)
{    
	// Create a polygonizer. Cell side length = 0.1 and go up to 50 cells.
	Polygonizer pol(&imp, 0.1f, 50);	
    
	// Start polygonizer in vicinity of 1,1,1
	pol.march(1,1,1);
    
	// Build HMesh from triangles
	vector<int> indices;
	vector<Vec3f> verts;
	vector<int> faces;
	
	// Extract vertices
 	for(int i = 0; i < pol.no_vertices(); ++i){
        verts.push_back(*reinterpret_cast<Vec3f*>(&pol.get_vertex(i)));
	}
    
	// Extract triangles.
 	for(int i = 0; i < pol.no_triangles(); ++i)
	{
        faces.push_back(3);
        TRIANGLE f = pol.get_triangle(i);
        indices.push_back(f.v0);
        indices.push_back(f.v1);
        indices.push_back(f.v2);
	}
	
	// Build manifold
	m.build(verts.size(), reinterpret_cast<float*>(&verts[0]), faces.size(), &faces[0], &indices[0]);
  m.cleanup();
}    


// Create an implicit function: Just union of two spheres 
class TestImplicit:public ImplicitFunction {
    const Vec3f c0, c1;
public:
	
    TestImplicit():c0(0, 0, 0), c1(2, 2, 2) {
    } float eval(float x, float y, float z) {
        Vec3f p(x, y, z);
        return min(2.0-length(p - c0) ,2.0-length(p - c1));
    }
};

 //Fit a polynomial to six points and produce a height field from the polynomial.
	//We simply create a regular 2D grid of points and four each cell in this grid, 
	//we produce a face.
void make_height_field()
{
	// Let us find the coefficients for this polynomial a x*x + b*y*y +c*x*y + d*x + e*y + f
	// such that the graph passes through the six points defined in the pts array above
	// We will find the coefficients as the solution to a linear system A*x = b
	CMatrix A(6,6);
	CVector b(6);
	
	// Create the matrix using the xy values of each point to define the corresponding row
	// of A. Assign the height value of the point to the corresponding coordinate of b.
	for(int i=0;i<6;++i)
	{
		A.set(i,0,pts[i][0]*pts[i][0]);
		A.set(i,1,pts[i][1]*pts[i][1]);
		A.set(i,2,pts[i][0]*pts[i][1]);
		A.set(i,3,pts[i][0]);
		A.set(i,4,pts[i][1]);
		A.set(i,5,1);
		b[i] = pts[i][2];
	}
	
	// Now solve for the coefficients x
	CVector x;
	LinearSolve(A,b,x);
	
	// Next we create a mesh using the value of the polynomial as height.
	int N = 20;
    vector <Vec3f> p(N*N); // A vector of 3D points to use as vertices
    for (int j = 0; j < N; ++j)
        for (int i = 0; i < N; ++i) {
            Vec2d pos = Vec2d(i-N/2, j-N/2)*35.0/N;
            float s = x[0]*pos[0]*pos[0]+x[1]*pos[1]*pos[1]+x[2]*pos[0]*pos[1]+x[3]*pos[0]+x[4]*pos[1]+x[5];
            p[j * N + i] = Vec3f(pos[0], pos[1], s);
        }
	
    vector <int> faces((N-1)*(N-1), 4); // A vector of faces. Each entry is the number of vertices in a face.
										// Since all faces are quads, we fill the vector with 4
    vector <int> indices;				// A long list of indices to the vertices that make up the faces.
	
	// In the loop below, we fill the indices vector with indices to the right vertices in the grid.
    for (int j = 0; j < (N-1); ++j) 
        for (int i = 0; i < (N-1); ++i) {
            int idx0 = i + j * N;
            int idx1 = (i + 1) + j * N;
            int idx2 = i + 1 + (j + 1) * N;
            int idx3 = i + (j + 1) * N;
            indices.push_back(idx0);
            indices.push_back(idx1);
            indices.push_back(idx2);
            indices.push_back(idx3);
        }
	
    // Build manifold. We pass the function a manifold together with information which
	// is basically an indexed face set.
    mani.build(p.size(), reinterpret_cast<float*>(&p[0]), faces.size(), &faces[0], &indices[0]);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();	
    view_ctrl->set_gl_modelview();
	
    if(create_display_list) {
        create_display_list = false;
		
        glNewList(1, GL_COMPILE);
		
        draw(mani);
		
        if(do_wire) {
            glDisable(GL_LIGHTING);
            glDepthFunc(GL_LEQUAL);
            glEnable(GL_POLYGON_OFFSET_LINE);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glColor3f(1, 0, 0);
            glPolygonOffset(-2, -2);
			
            draw(mani);
			
            glPolygonOffset(0, 0);
            glDisable(GL_POLYGON_OFFSET_LINE);
            glDepthFunc(GL_LESS);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_LIGHTING);
        }

		//Uncomment to see points used to define height field
		glPointSize(4);
		glDisable(GL_LIGHTING);
		glColor3f(1,0,0);
		glBegin(GL_POINTS);
		for(int i=0;i<6;++i)
			glVertex3fv(pts[i].get());
		glEnd();
		glEnable(GL_LIGHTING);
		glPointSize(1);
		
		glEndList();
    }

	glCallList(1);

	
    glPopMatrix();
    glutSwapBuffers();
}

void animate()
{
    view_ctrl->try_spin();
    glutPostRedisplay();
}


void reshape(int W, int H)
{
    view_ctrl->reshape(W, H);
}


void mouse(int button, int state, int x, int y)
{
    Vec2i pos(x, y);
    if(state == GLUT_DOWN) {
        if(button == GLUT_LEFT_BUTTON)
            view_ctrl->grab_ball(ROTATE_ACTION, pos);
        else if(button == GLUT_MIDDLE_BUTTON)
            view_ctrl->grab_ball(ZOOM_ACTION, pos);
        else if(button == GLUT_RIGHT_BUTTON)
            view_ctrl->grab_ball(PAN_ACTION, pos);
    }
    else if(state == GLUT_UP)
        view_ctrl->release_ball();
}

void motion(int x, int y)
{
    Vec2i pos(x, y);
    view_ctrl->roll_ball(Vec2i(x, y));
}


void keyboard(unsigned char key, int x, int y)
{
	Vec3f c;
	float r;
    switch (key) {
		case 'q':
			cout << "Bye" << endl;
			exit(0);
			break;
		case 'w':
			cout << "Showing wire" << endl;
			do_wire = !do_wire;
			break;
		case 'h':
		case 'i':
			mani.clear();
			if(key=='h')
				make_height_field();
			else
			{
				TestImplicit imp;
				polygonize(mani, imp, 50, Vec3f(0, 0, 0), Vec3f(2,2,2));
			}
			bsphere(mani,(Vec3d)c, r);
			delete view_ctrl;
			view_ctrl = new GLViewController(WINX, WINY, c, 1.5*r);
			break;
    }
    create_display_list = true;
}

int main(int argc, char **argv)
{
  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WINX, WINY);
    glutInit(&argc, argv);
    glutCreateWindow("RBF Viewer");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(animate);
	
	make_height_field();
    Vec3f c(0, 0, 0);
    float r = 3;
	bsphere(mani,(Vec3d)c, r);
    view_ctrl = new GLViewController(WINX, WINY, c, 1.5*r);
	
    // Initialization of extension wrangler. Very important call.
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        cout << "GLEW failed to initialize. Maybe you have an OpenGL driver problem" << endl;
    }
	
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0f, 1.0f, 1.0f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    glEnable(GL_NORMALIZE);
	
    glutMainLoop();
    
}

