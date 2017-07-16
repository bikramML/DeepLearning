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
#include<omp.h>
#include<sys/time.h>
//#include<vector>



using namespace std;

//#include "print_array_pixel.cpp"
//#include "pixel_print.cpp"
#include "image_read_view.cpp"
//#include "convolutional_layer.cpp"
//#include "softmax_backprpagation.cpp"
//#include "image_marge.cpp"
//#include "cnn.cpp"
//#include "nn_analysis_network.cpp"

//#define FOLDER_PATH "../../data/"
//#define TRAIN_FILE "train_wkt_v4.csv"
//#define GRID_FILE "grid_sizes.csv"
string folder="../../data/";
string train_file="train_wkt_v4.csv";
//";

string grid_file="grid_sizes.csv";

ifstream parallel_read_write::train_fp((folder+train_file).c_str());                 // handle for input image file
ifstream parallel_read_write::grid_fp((folder+grid_file).c_str());                // handle for output image file
//float read_write::xmax=0,read_write::ymax=0;
uint32 parallel_read_write::train_x=3391,parallel_read_write::train_y=3349;

 uint32 parallel_read_write::object_count=0;
 float parallel_read_write::A_x_fact=1,parallel_read_write::A_y_fact=1,parallel_read_write::P_x_fact=25,parallel_read_write::P_y_fact=25,parallel_read_write::M_x_fact=4,parallel_read_write::M_y_fact=4,parallel_read_write::RGB_x_fact=25,parallel_read_write::RGB_y_fact=25;

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


parallel_read_write rw("6040_2_2",10,10,0,0);
/*
//float **IR1=max_size_incr();
struct timeval  TimeValue_Start;
struct timezone TimeZone_Start;
struct timeval  TimeValue_Final;
struct timezone TimeZone_Final;
long  time_start, time_end;
double time_overhead;

parallel_image_matrix nir1(rw.image_M[6],40,40);
parallel_image_matrix r(rw.image_3[0],250,250);
parallel_image_matrix g(rw.image_3[1],250,250);
parallel_image_matrix b(rw.image_3[2],250,250);
uint16 image_x=250,image_y=250;
nir1.matrix_size_incr(image_x,image_y);
parallel_image_matrix nir11(nir1.matrix_scale_up(0.1),250,250);
parallel_image_matrix nir12(nir1.matrix_scale_up(0.3),250,250);
parallel_image_matrix nir13(nir1.matrix_scale_up(0.2),250,250);
parallel_image_matrix temp=(nir12+nir13);
temp=(temp+nir11);

int j;
gettimeofday(&TimeValue_Start, &TimeZone_Start);
uint16 feture[]={10,10,10,10,1};
uint16 layer=5;
parallel_convolutional_layer *p[layer];int i;


for(i=0;i<layer;i++)
{
    p[i]=new parallel_convolutional_layer(250,250);

    p[i]->fu=feture[i];
    p[i]->init_weight();

}

parallel_neural_network_training(p,feture,layer,temp.p,image_x,image_y,rw.train_array);

float **test=parallel_neural_network_testing(p,feture,layer,temp.p,image_x,image_y,10);

    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_overhead = (time_end - time_start)/1000000.0;
    cout<<"\n Total time after training and testing in neural network "<<time_overhead;

gettimeofday(&TimeValue_Start, &TimeZone_Start);
float nn_weights_init[5*5+1];



parallel_convolutional_net_layer *np[layer];//(250,250,5,5);
for(i=0;i<layer;i++)
{
    np[i]=new parallel_convolutional_net_layer(250,250,5,5);

    np[i]->fu=feture[i];
    np[i]->init_weight();
}
memcpy(nn_weights_init,p[0]->weight[0], (5*5+1)*sizeof(float));
parallel_convolutional_neural_network_training(np,feture,layer,temp.p,image_x,image_y,rw.train_array);
float **cnn_test=parallel_convolutional_neural_network_testing(np,feture,layer,temp.p,image_x,image_y,10);
//i=array_check(nn_weights_init,p[0]->weight[0],(image_x*image_y+1));
//cout<<"i="<<i<<"\n";
float total_nn_mach=0,total_cnn_mach=0;
for(i=0;i<image_x;i++)
    for(j=0;j<image_x;j++)
    {
        if(test[i][j]==rw.train_array[i][j])
            total_nn_mach++;
        if(cnn_test[i][j]==rw.train_array[i][j])
            total_cnn_mach++;
    }
gettimeofday(&TimeValue_Final, &TimeZone_Final);
time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
time_overhead = (time_end - time_start)/1000000.0;
cout<<"\n Total time after training and testing in convolutional neural network "<<time_overhead;

cout<<"\n Percentage of mach found in neural network="<<(total_nn_mach/(image_x*image_y)*100);
cout<<"\n Percentage of mach found in convolutional neural network="<<(total_cnn_mach/(image_x*image_y)*100)<<"\n";
*/
}
