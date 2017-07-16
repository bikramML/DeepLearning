/*
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <tiffio.h>
#include <algorithm>
#include<iostream>
#include<math.h>
#include<graphics.h>
*/
#include <stdio.h>
#include <tiffio.h>
#include <stdlib.h>
#include <iostream>
#include<fstream>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include<math.h>
#include<graphics.h>
#include <GL/glut.h>
//#include<vector>



using namespace std;

#include "print_array_pixel.cpp"
//#include "pixel_print.cpp"
#include "image_read_view.cpp"
#include "convolutional_layer.cpp"
#include "softmax_backprpagation.cpp"
#include "image_marge.cpp"
#include "cnn.cpp"
#include "nn_analysis_network.cpp"

#define FOLDER_PATH "../../data/"
#define TRAIN_FILE "train_wkt_v4.csv"
#define GRID_FILE "grid_sizes.csv"
string folder="../data/";
string train_file="train_wkt_v4.csv";
//";

string grid_file="grid_sizes.csv";

ifstream read_write::train_fp((folder+train_file).c_str());                 // handle for input image file
ifstream read_write::grid_fp((folder+grid_file).c_str());                // handle for output image file
//float read_write::xmax=0,read_write::ymax=0;
uint32 read_write::train_x=3391,read_write::train_y=3349;

 uint32 read_write::object_count=0;
 float read_write::A_x_fact=1,read_write::A_y_fact=1,read_write::P_x_fact=25,read_write::P_y_fact=25,read_write::M_x_fact=4,read_write::M_y_fact=4,read_write::RGB_x_fact=25,read_write::RGB_y_fact=25;

//#define TRAIN_LIKE ""
//#define x_to_pixel ()

int array_check(float *s1,float *s2,uint32 size)
{
    uint32 i;
    for(i=0;i<size;i++)

        {
            if(s1[i]==s2[i])
            continue;
            return 1;
        }
    if(i==size)
        return 0 ;
}

main(int argc, char **argv)
{

read_write rw("6040_2_3",10,10,0,0);
//float **IR1=max_size_incr();

image_matrix nir1(rw.image_M[6],40,40);
image_matrix r(rw.image_3[0],250,250);
image_matrix g(rw.image_3[1],250,250);
image_matrix b(rw.image_3[2],250,250);
uint16 image_x=250,image_y=250;
nir1.matrix_size_incr(image_x,image_y);
image_matrix nir11(nir1.matrix_scale_up(0.1),250,250);
image_matrix nir12(nir1.matrix_scale_up(0.3),250,250);
image_matrix nir13(nir1.matrix_scale_up(0.2),250,250);
image_matrix temp=(nir12+nir13);
temp=(temp+nir11);

int j;
uint16 feture[]={10,10,10,10,1};
uint16 layer=5;
convolutional_layer *p[layer];int i;


for(i=0;i<layer;i++)
{
    p[i]=new convolutional_layer(250,250);

    p[i]->fu=feture[i];
    p[i]->init_weight();

}
//memcpy(nn_weights_init,p[0]->weight[0], (image_x*image_y+1)*sizeof(float));
//weight_print(p[0]->weight[0],image_x,image_y);

//cout<<"i="<<i<<"\n";
neural_network_training(p,feture,layer,temp.p,image_x,image_y,rw.train_array);

float **test=neural_network_testing(p,feture,layer,temp.p,image_x,image_y,10);




float nn_weights_init[5*5+1];



convolutional_net_layer *np[layer];//(250,250,5,5);
for(i=0;i<layer;i++)
{
    np[i]=new convolutional_net_layer(250,250,5,5);

    np[i]->fu=feture[i];
    np[i]->init_weight();
}
memcpy(nn_weights_init,p[0]->weight[0], (5*5+1)*sizeof(float));
convolutional_neural_network_training(np,feture,layer,temp.p,image_x,image_y,rw.train_array);
float **cnn_test=convolutional_neural_network_testing(np,feture,layer,temp.p,image_x,image_y,10);
i=array_check(nn_weights_init,p[0]->weight[0],(image_x*image_y+1));
cout<<"i="<<i<<"\n";
float total_nn_mach=0,total_cnn_mach=0;

for(i=0;i<image_x;i++)
    for(j=0;j<image_x;j++)
    {
        if(test[i][j]==rw.train_array[i][j])
            total_nn_mach++;
        if(cnn_test[i][j]==rw.train_array[i][j])
            total_cnn_mach++;
    }

cout<<"\n Percentage of mach found in neural network="<<(total_nn_mach/(image_x*image_y)*100);
cout<<"\n Percentage of mach found in convolutional neural network="<<(total_cnn_mach/(image_x*image_y)*100)<<"\n";
}

