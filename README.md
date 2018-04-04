# CopCAD
CopCAD is a ready to use Engineering Drawing tool. The tool provides an easy-to-use and fast way to user to render any three dimensional object as wireframe and visualise its orthographic projections. Also, given the three orthographic projections user can generate a wireframe to check the prototype of his/her model. 

The tool also allows user to rotate the object with easy to use sliders, and view orthographic projections of the rotated object.  

The software expects the projection drawing or the three dimensional model to be input in the form of its vertex set and edge set.

##Requirements
To run the software you require:

Qt package: Version 5.X.XX
```
To install over linux environment run 
$ sudo apt-get install qt5default
```

GCC Version 7+ (tested with 7.2.0)
```
A reference to update your gcc https://gist.github.com/application2000/73fd6f4bf1be6600a2cf9f56315a2d91
```

##Build steps
```
$ qmake EnngDrawing.pro
$ make
$ bin/CopCAD
```