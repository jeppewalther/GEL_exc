/* GEL demo program.
 
 This is a simple mesh viewer. However, it features several visualization modes:
 - with face or vertex normals,
 - wireframe,
 - scalar field on surface visualization,
 - visualization of vector fields on surface,
 - pleasant glazed clay rendering.
 Several of these can be used in conjunction. Thus, the present code should be a 
 good starting point for mesh manipulation programs or visualization of data (e.g.
 curvature on meshes).
 
 Please refer to the keyboard function for a list of hotkeys.
 
 Andreas Bærentzen 2012
 */

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GLGraphics/gel_glut.h>
#include <iostream>

#include <CGLA/Vec3f.h>
#include <CGLA/Vec3d.h>

#include <GLGraphics/GLViewController.h>
#include <GLGraphics/ManifoldRenderer.h>
#include <Util/ArgExtracter.h>

#include <HMesh/Manifold.h>
#include <HMesh/load.h>

using namespace std;
using namespace HMesh;
using namespace GLGraphics;
using namespace Util;
using namespace CGLA;

namespace 
{
	GLViewController* view_ctrl;   // This is the class used to help us navigate with the mouse (virtual trackball)
	int WINX=800, WINY=800;        // Predefine window dimensions
	Manifold mani;                 // The mesh we view
	int do_wire = false;           // Whether or not to use wireframe
	ManifoldRenderer* renderer;    // A special renderer class is used to make visualization more flexible
	Vec3d bsphere_center(0,0,0);   // Bounding sphere center
	float bsphere_radius = 3;      // and radius
  bool remake_dpy_list = true;
}

/* Below is the OpenGL display function. This is the main loop of the program
 from where most things happen. All drawing done here. */
void display() 
{
    static GLuint dpy_list=0;
    
	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix(); // save transformation matrix
	view_ctrl->set_gl_modelview(); // Setup view point and orientation using virtual trackball
	
    if(remake_dpy_list)
    {
        glDeleteLists(dpy_list, 1);
        dpy_list = glGenLists(1);
        glNewList(dpy_list, GL_COMPILE);
        
        renderer->draw(); // Do actual drawing.
        
        // Below is some code to draw lines on top of the mesh if so desired.
        if(do_wire)
        {
            glDisable(GL_LIGHTING);
            glDepthFunc(GL_LEQUAL);
            glEnable(GL_POLYGON_OFFSET_LINE);
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            glColor3f(1,0,0);
            glPolygonOffset(-2,-2);
            
            draw(mani,false);
            
            glPolygonOffset(0,0);
            glDisable(GL_POLYGON_OFFSET_LINE);
            glDepthFunc(GL_LESS);
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glEnable(GL_LIGHTING);
        }
        glEndList();
        remake_dpy_list = false;
    }
    
    glCallList(dpy_list);
    
	glPopMatrix(); // Restore transformation
	glutSwapBuffers(); // Swap front and back buffers (displays the image)
}

/* This callback function is called regularly when nothing else is going on. 
 We use it to animate the model.*/
void animate() 
{
	view_ctrl->try_spin();
	glutPostRedisplay();
}


// This function is called when the window changes shape.
void reshape(int W, int H)
{
	WINX=W;
	WINY=H;
	view_ctrl->reshape(W,H);
}


/* The mouse callback is called when a mouse event has occurred. This is where user input 
 for the virtual trackball is registered. */ 
void mouse(int button, int state, int x, int y) 
{
	Vec2i pos(x,y);
	if (state==GLUT_DOWN) 
	{
		if (button==GLUT_LEFT_BUTTON) 
			view_ctrl->grab_ball(ROTATE_ACTION,pos);
		else if (button==GLUT_MIDDLE_BUTTON) 
			view_ctrl->grab_ball(ZOOM_ACTION,pos);
		else if (button==GLUT_RIGHT_BUTTON) 
			view_ctrl->grab_ball(PAN_ACTION,pos);
	}
	else if (state==GLUT_UP)
		view_ctrl->release_ball();
}

/* If the mouse moves with button depressed, we register motion here. */
void motion(int x, int y) {
	Vec2i pos(x,y);
	view_ctrl->roll_ball(Vec2i(x,y));
}


/* This is where you do something */
void compute_dual(Manifold& m)
{
    remake_dpy_list = true;
	//Insert code here for computing the dual of the mesh
    
}

/* We define a few different renderers that you can play with, */
enum RendererType
{
    NORMAL_RENDERER,           // Normal (dull) rendering
    GLAZED_RENDERER,           // Render model as a ceramic object
    REFLECTION_LINES_RENDERER, // Draw reflection lines
    SCALAR_FIELD_RENDERER,     // Draw a scalar field using color graduation
    LINE_FIELD_RENDERER        // Draw a line field using hatchings to indicate directions
};

/* This function constructs a new renderer and returns a pointer */
ManifoldRenderer* renderer_factory(RendererType rt, bool smooth)
{
    switch(rt)
    {
        case NORMAL_RENDERER: 
            return new NormalRenderer(mani,smooth); 
        case GLAZED_RENDERER: 
            return new GlazedRenderer(mani,smooth);
        case REFLECTION_LINES_RENDERER:
            return new ReflectionLineRenderer(mani,smooth);
        case SCALAR_FIELD_RENDERER: 
        {
            VertexAttributeVector<double> scalars(mani.no_vertices());
            double maximum=0.0;
            for(VertexID v : mani.vertices())
            {
                scalars[v] = mani.pos(v)[0];
                maximum = max(abs(scalars[v]), maximum);
            }
            
            return new ScalarFieldRenderer(mani, smooth, scalars, maximum);
        }
        case LINE_FIELD_RENDERER: 
        {
            VertexAttributeVector<Vec3d> lines(mani.no_vertices());
            for(VertexID v : mani.vertices())
            {
                lines[v] = Vec3d(0,0,1);
            }
            
            
            return new LineFieldRenderer(mani, smooth, lines, bsphere_radius);
        }
    }
    return 0;
}

/* Keyboard input - used to switch between various types of rendering and to exit the program. */
void keyboard(unsigned char key, int x, int y) 
{	
    static bool smooth=true;
    static RendererType current_renderer = NORMAL_RENDERER;
    switch(key) {
        case 'q': 
            exit(0); 
        case 'f':
            smooth = !smooth; 
            break;
        case 'w': 
            do_wire = !do_wire; 
            break;
        case 'n':
            current_renderer = NORMAL_RENDERER;
            break;
        case 'g':
            current_renderer = GLAZED_RENDERER;
            break;
        case 'r':
            current_renderer = REFLECTION_LINES_RENDERER;
            break;
        case 's': 
            current_renderer = SCALAR_FIELD_RENDERER;
            break;
        case 'l':
            current_renderer = LINE_FIELD_RENDERER;
            break;
        case 'd':
            compute_dual(mani);
            break;
    }
    if(key != 'w') 
    {
        delete renderer;
        renderer = renderer_factory(current_renderer, smooth);
    }
    remake_dpy_list = true;
}


/* Main - were things start. */
int main(int argc, char** argv)
{
    /* We check for user input and input a small bunny model if no 
     other model is provided. */
    ArgExtracter ae(argc, argv);
    string file = ae.get_last_arg();
    if (argc < 2)
        file = "bunny-little.x3d";
    mani.clear();
	if(!load(file, mani))
    {
        cout << "Failed to load mesh ... bailing out" << endl;
        exit(1);
    }
    
    // Initialization of glut (OpenGL - window system glue)
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(WINX, WINY);
    glutInit(&argc, argv);
    glutCreateWindow("GEL Example Program");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(animate);
    
    // Initialization of extension wrangler. Very important call.
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        cout << "GLEW failed to initialize. Maybe you have an OpenGL driver problem" << endl;
    }
    
    /* Set up the view controller (virtual trackball). For this we need the window dimensions and
     the bounding sphere of the model. */
    bsphere(mani, bsphere_center, bsphere_radius);
    view_ctrl = new GLViewController(WINX,WINY, (Vec3f)bsphere_center, bsphere_radius*2);
    renderer = new NormalRenderer(mani,true);
    
    /* OpenGL initialization. Mainly resets the modelview matrix and enables some things we 
     need */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0f, 1.0f, 1.0f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Pass control to GLUT
    glutMainLoop();
}
