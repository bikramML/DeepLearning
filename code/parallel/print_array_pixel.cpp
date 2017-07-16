//#include<iostream>
//#include<GL/glut.h>
//using namespace std;
//#define WIDTH 240
//#define HEIGHT 1
GLushort ***my_image;
int pixel_width,pixel_height;//[WIDTH][HEIGHT][3]; /* pixel data, R,G,B */
/* code omitted to write pixel values into ’image’ */
void display (void)
{
glClear(GL_COLOR_BUFFER_BIT);
glClearColor(1.0,1.0,1.0,1.0);
glRasterPos3f(-1, -1, 0.0);

glDrawPixels(pixel_width, pixel_height, GL_RGB, GL_UNSIGNED_SHORT, my_image);

glFlush();
}
void keyboard(unsigned char key, int x, int y) {
if (key == 27) { exit (0); }
/* escape key */

}
void pixel_array_print(int sd,char** n,int width,int height,uint16*** image_array)
{
//    cout<<"Basic oprngl programe "<<"\n";

    my_image=image_array;
    glutInit(&sd,n);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(width,height);
    pixel_width=width;
    pixel_height=height;
    glutInitWindowPosition(0,0);
    glutCreateWindow("Image window");
    glutDisplayFunc(display);
    glutKeyboardFunc (keyboard);
    glutMainLoop();
//	return 0;
}

