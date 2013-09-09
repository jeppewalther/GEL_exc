GEL framework 
=============

This C++ code package is intended for use in a course in computational
geometry processing, and concrete exercises for which this package is
convenient are found in our book "Guide to Computational Geometry
Processing", published by Springer (ISBN 978-1-4471-4074-0).

This package is also what we use in our own course 02585 Computational
Geometry Processing at the Technical University of Denmark. As such,
the code base has seen use by a number of students in the last few
years.

Structure
---------

Below this directory are the directories GEL and GEL_demos. The most
important subdirectories are

dependencies/include - source for dependencies
dependencies/lib - lib for dependencies
GEL/GEL_WIN - build files for Windows/Visual Studio 
GEL/doc - documents (intro.pdf) and Doxygen documentation 
GEL/src - the source hierarchy 
GEL_demos/Edge_Flipper - demo useful for mesh generation 
GEL_demos/Mesh_Viewer - demo for visualization/manipulation of meshes 
GEL_demos/Surface_Reconstruction - Demo useful for surface reconstruction

Compilation
-----------

Using the build files mentioned above, it should be straight forward
to build both the library and the three demo programs on a Mac or
Windows computer. For Windows we have Visual Studio solutions in GEL/GEL_WIN for GEL itself and then solutions for each of the demos. For Mac, the XCode project contains targets for both the GEL library and the demos.
I am sorry to say that Linux users are on their own.

Documentation
-------------

The document ./GEL/doc/intro.pdf is an introduction to GEL. The intent of this document is to give an overview and a starting point. For a reference manual, please peruse ./GEL/doc/html/index.html which is the Doxygen documentation.

Data
----

There is no specific data directory, but appropriate data files -
mostly point sets and meshes are included in the subdirectories of
GEL_demos along with the source code for the examples.

Notes to instructor
-------------------

Some pieces of GEL are left out where the students are asked to fill
in the missing pieces. The instructor should be aware that the
complete GEL is an open source framework, and students might obtain it
- thus having what amounts to a solution to the problems. That can be
an advantage - personally I think problems in books are more useful if
solutions are available - but it also means that cheating is possible.

Feedback
--------

If you find severe issues with GEL or think that you have praise or criticism that we need to hear, you may send an email to jab 'at' imm.dtu.dk. You may, however, want to be aware of the fact that we don't have the resources to hire a programmer to make everything nice :-(

J. Andreas Bærentzen, 2012
