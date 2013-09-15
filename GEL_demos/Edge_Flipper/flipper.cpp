/* GEL demo program.
 
 Creates a very small simple mesh as a Manifold (cf. the HMesh namespace of the 
 GEL library). The mesh is visualized and pressing any key will cause an edge to
 flip (if the edge is legally flippable). While the example seems pointless at first
 glance it provides a good starting point for implementing 2D Delaunay triangulation
 and also includes point loading code. 
 
 Andreas Bærentzen and Kasper Steenstrup 2013
 
 */

#include <iostream>
#include <fstream>
//#include <vector>

//#include <HMesh/Manifold.h>
//#include <HMesh/AttributeVector.h>
//#include <HMesh/triangulate.h>

//include <GLGraphics/gel_glut.h>
//#include <CGLA/Vec2d.h>

//using namespace std;
//using namespace CGLA;
//using namespace HMesh;

//// The range of the input data.
//Vec2d dmin(99e99), dmax(-99e99);

//Manifold m; // The triangle mesh data structure.
//HalfEdgeIDIterator flipper = m.halfedges_begin(); // The halfedge we try to flip.
//HalfEdgeAttributeVector<int> touched;

///*
// * Draw the triangle mesh. This function is called from GLUT (a library
// * which enables OpenGL drawing in windows.)
// */

//void display()
//{
//    // Set up correct OpenGL projection
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluOrtho2D(dmin[0], dmax[0], dmin[1], dmax[1]);
//    glMatrixMode(GL_MODELVIEW);

//    // Specify that we want to draw triangle outlines
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//    // Black on white.
//    glClearColor(1,1,1,0);
//    glColor3f(0,0,0);

//    // Clear the screen.
//    glClear(GL_COLOR_BUFFER_BIT);

//  for(FaceID f: m.faces()){
//    glBegin(GL_POLYGON);
//    for(Walker w = m.walker(f); !w.full_circle(); w = w.next())
//      glVertex3dv(m.pos(w.vertex()).get());
//    glEnd();
//  }

//    // Draw flipper.
//    glColor3f(1,0,0);
//    glBegin(GL_LINES);
//  Walker hew = m.walker(*flipper);
//  glVertex3dv(m.pos(hew.vertex()).get());
//    glVertex3dv(m.pos(hew.opp().vertex()).get());
//    glEnd();

//    glFinish();
//}

///*
// * As the name indicates, the function below creates a manifold (i.e. a
// * triangle mesh) consisting of just a single triangle.
// */

//void create_single_triangle_manifold(const Vec3f& p1,
//                                                                         const Vec3f& p2,
//                                                                         const Vec3f& p3,
//                                                                         Manifold& mani)
//{
//    // Create vector of vertices
//    vector<Vec3f> vertices(3);
//    vertices[0] = p1;
//    vertices[1] = p2;
//    vertices[2] = p3;

//    // Create vector of faces. Each element corresponds to a face and tells
//    // how many vertices that face contains. In the case of a triangle
//    // mesh, each face has three vertices.
//    vector<int> faces(1);
//    faces[0] = 3;

//    // Create the index vector. Each element is an index into the vertex list
//    //
//    vector<int> indices(3);
//    indices[0]=0;
//    indices[1]=1;
//    indices[2]=2;

//  mani.build(3,           // Number of vertices.
//                     reinterpret_cast<float*>(&vertices[0]),// Pointer to vertices.
//                         1,           // Number of faces.
//                         &faces[0],   // Pointer to faces.
//                         &indices[0]);// Pointer to indices.


//}


//void keyfun(unsigned char c, int x, int y)
//{
//    /*
//     * A little game, try to flip an edge when user presses any key.
//     * Not all edges can be flipped. Boundary edges cannot. Edges also
//     * cannot be flipped if it will render the mesh invalid.
//     */

//    if(boundary(m, *flipper)) // If this is a boundary edge just drop the idea.
//        cout << "boundary edge" << endl;
//    else if(precond_flip_edge(m, *flipper))
//  {
//    m.flip_edge(*flipper);
//        cout << "flipped" << endl;
//  }
//    else
//    cout << "could not flip" << endl;

//    do
//        {
//            ++flipper; // Get the next halfedge
//            // If we have passed the last halfedge, go to the first.
//            if(flipper==m.halfedges_end())
//                {
//                    flipper = m.halfedges_begin();
//                    break;
//                }
//        }
//    while(touched[*flipper] == 0); // Only visit halfedges marked '1'


//    // Function call below informs glut that display should be called to
//    // show the window again.
//    glutPostRedisplay();
//}

///*
// * mark halfedges.
// *
// * Remember that a geometric edge corresponds to two halfedges.
// * this function loops over all halfedges and tags precisely
// * one halfedge in every pair of halfedges with '1'. Its opposite edge
// * is tagged with '0'.
// */

//void mark_halfedges()
//{
//    // Give all halfedges a mark of 0
//  for(HalfEdgeID h: m.halfedges())
//  {
//      if(m.walker(h).opp().halfedge() < h)
//      touched[h] = 0;
//    else
//      touched[h] = 1;
//    }
//}


int main(int argc, char** argv)
{
//    /*
//     * Read and parse a point set.
//     */

//    /* Open a data stream for reading.
//     * We first open data.txt. There is also kote1.txt which contains height
//     * values in addition to x,y positions.
//     */

//    ifstream data("data.txt");
//    vector<Vec2d> pts;
//    if(data.good())
//        while(!data.eof())
//            {
//                double x,y;
//                data >> x >> y;

//                if(data.good())
//                    {
//                        Vec2d p(x,y);
//                        pts.push_back(p);
//                        dmin = v_min(p,dmin);
//                        dmax = v_max(p,dmax);
//                    }
//            }
//    cout << "Loaded " << pts.size() << " points "  <<  endl;

//    /*
//     * Build a triangle mesh with a single triangle consisting of the
//     * first three vertices.
//     */

//    create_single_triangle_manifold(Vec3f(pts[0][0], pts[0][1], 0),
//                                                                    Vec3f(pts[1][0], pts[1][1], 0),
//                                                                    Vec3f(pts[2][0], pts[2][1], 0),
//                                                                    m);

//    // Split an edge - now we have two triangles
//    HalfEdgeIDIterator h = m.halfedges_begin();
//    m.split_edge(*h); // split the edge
//  triangulate_face_by_edge_split(m, m.walker(*h).face());

//    // Get the first triangle.
//    FaceIDIterator f1 =m.faces_begin();

//    // Create a vertex at centre of f1
//    m.split_face_by_vertex(*f1);


//    // Mark halfedges so we can visit one from each pair.
//    mark_halfedges();

//    // Set flipper to point to the first halfedge.
//    flipper = m.halfedges_begin();

//    /*
//     * Initialize GLUT, the system used to show OpenGL windows.
//     */
//    glutInit(&argc, argv);
//    glutInitWindowSize(512,512);
//    glutInitDisplayMode(GLUT_RGBA);
//    glutCreateWindow("Delaunay");

//    glutDisplayFunc(display); // This function is called from glut to draw
//    glutKeyboardFunc(keyfun); // Parse keyboard input
	
//    // Pass control to glut
//    glutMainLoop();
    return 0;
}
