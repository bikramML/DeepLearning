


GLenum doubleBuffer;
//GLubyte *ubImage;
//uint16 *ubImage;
GLushort *ubImage;
uint32 rows=0, colums=0;
static void Init(float ***Pixel,uint32 x,uint32 y)
{
  uint32 i,j;
//  GLubyte *img;
  GLushort* img;
  GLsizei imgWidth = 128;
  ubImage=new GLushort[x*y*3];
  //glMatrixMode(GL_COLOR);//(GL_PROJECTION);
  //glLoadIdentity();
  //gluPerspective(60.0, 1.0, 0.1, 1000.0);
  //glMatrixMode(GL_MODELVIEW);
  //glDisable(GL_DITHER);

  /* Create image */
  img = ubImage;
  rows=x;colums=y;
  for(i=0;i<x;i++)
      for(j=0;j<3*y;)
      {
          img[i*y+j]=((GLushort)Pixel[0][i][j]);//65536);
          img[i*y+j+1]=((GLuint)Pixel[1][i][j]);//65536);
          img[i*y+j+2]=((GLuint)Pixel[2][i][j]);//65536);
         // img[i*y+j+3]=1;

       //   cout<<img[i*y+j]<<"\t";
          j=j+3;
      }
}

/* ARGSUSED1 */
static void
Key(unsigned char key, int x, int y)
{
  switch (key) {
  case 27:
    exit(0);
  }
}


static void
Draw()
{
//65535
  glClearColor(1.0, 1.0, 1.0, 1.0);
  //glColor3f ( 1.0f, 0.0f, 0.0f );
  glColor3us( 0,0,0);
                //glUniform3ui(0,0,0,0);
                glClear(GL_COLOR_BUFFER_BIT);
                //glMatrixMode ( GL_PROJECTION );
                //    glLoadIdentity ( );
               //      gluOrtho2D ( 0.0, 400.0, 1.0, 400.0 );
            //    glPointSize ( 4.0 );
              #if GL_EXT_abgr
              //  glRasterPos3f(-0.8, -0.8, -1.5);GL_ABGR_EXT
                glDrawPixels(rows,colums,GL_ABGR_EXT ,GL_UNSIGNED_SHORT, ubImage);
              #endif

                //glRasterPos3f(0.2, -0.8, -1.5);GL_RGBA
                glDrawPixels(rows,colums, GL_RGBA,GL_UNSIGNED_SHORT, ubImage);



                if (doubleBuffer) {
                  glutSwapBuffers();
                } else {
                  glFlush();
                }
              }
              int print_pixwl(int argc, char **argv,float ***Pixel,uint32 x,uint32 y)
              {
                GLenum type;

                glutInit(&argc, argv);


                type = GLUT_RGB;
                type |= (doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE;
                glutInitDisplayMode(type);
                glutCreateWindow("ABGR print");
                if (!glutExtensionSupported("GL_EXT_abgr")) {
                  printf("Couldn't find abgr extension.\n");
                  exit(0);
                }
              #if !GL_EXT_abgr
                printf("WARNING: client-side OpenGL has no ABGR extension support!\n");
                printf("         Drawing only RGBA (and not ABGR) images and textures.\n");
              #endif
                Init(Pixel, x,y);
                glutKeyboardFunc(Key);
                glutInitWindowSize(6000,6000);   // Set the window's initial width & height
                glutInitWindowPosition(0,0);
                glutDisplayFunc(Draw);
                glutMainLoop();
                return 0;             /* ANSI C requires main to return int. */
              }
