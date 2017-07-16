
#include<graphics.h>
//#include<conio.h>
//#include<stdio.h>
int main()
{
    int wid1;// wid2;
    int gd = DETECT,gm;
   initgraph(&gd,&gm,NULL);




      wid1 = initwindow(400, 300);

     // wid2 = initwindow(300, 400, 200, 100);





      setcurrentwindow(wid1);
      int color=0,devlimit=1,remain=0;
      //float remain=0;
      int colors[4]={0,7,8,15};
      uint16 max=2048;
      uint16 intal=max/4;
      uint32 no=0;

      for(uint32 x_value=0;x_value<no;x_value++)
        for(uint32 y_value=0;y_value<no;y_value++)
      {
          devlimit=intal;
          int j;
          for(j=0;j<4;j++)
          {
              remain=(input[x_value][y_value]/devlimit);
              if(remain>0)
              {
                  devlimit=devlimit*intal;
              }
              else
              {
                  break;
              }

          }
          if(j==4)
              color=colors[3];
          else if(j>0)
              color=colors[j-1];
          else
              color=colors[0];
        putpixel ( x_value, y_value, color);
      }



      getch();

      closegraph();

      return 0;

}

/*#include <GL/gl.h>
#include <GL/glut.h>

      void display()
      {
      }

      int main(int argc, char **argv)
      {
      glutInit(&argc, argv);
      glutDisplayFunc(display);
      glutMainLoop();
      }
*/
