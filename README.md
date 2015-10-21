# Efficient Collision Detection in a scene with static and dynamic objects

This is the implementation for my bachelors thesis which can be found [here](http://www.openthesis.org/document/view/601881_0.pdf). The thesis document explores the problem of efficient collision detection with application to the chewing simulation of the human teeth model. A gummy bear is used as a substrate for the chewing simulation. The setup of the experiment is as follows.

![](https://github.com/musaeed/Distance-field/raw/master/screenshots/scene_config.jpg)

A distance field of the scene is calculated and stored in a 3D matrix. This distance field is used to detect collisions in the scene. Please refer to the document for more details and a detailed analysis of performance of this method as compared to some of the other state of the art methods for collision detection.

## Build and run

Please use the Makefile to compile and run the code. I am using the Assimp library to read the stl file format. You can install this library on your system by following the instructions [here](http://assimp.sourceforge.net/).

You also need the Open GL and GLUT libraries on your computer. They can be installed by using this command on the terminal.

```bash
sudo apt-get install freeglut3 freeglut3-dev
```
The program takes around 15 minutes to calculate the distance field for both the upper jaw and the lower jaw. This calculation has to be made just in the start. The field calculated is stored in memory for later use in the program which gives real time results.

Please use the following keys to navigate through the scene:


* w : move scene upwards
* s : move scene downwards
* a : move scene left
* d : move scene right
* k : zoom in
* l : zoom out
* t : move bear upwards
* z : move bear downwards
* g : move bear right
* h : move bear left
* b : move bear forward
* n : move bear backward


For any further questions please contact me at muhammad.omar555 [at] gmail . com.