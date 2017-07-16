#include<mpi.h>

#include "macro.cpp"

string folder="../../data/";
string train_file="train_wkt_v4.csv";
//";
// For only serial code //////////
string grid_file="grid_sizes.csv";

ifstream read_write::train_fp((folder+train_file).c_str());                 // handle for input image file
ifstream read_write::grid_fp((folder+grid_file).c_str());                // handle for output image file
//float read_write::xmax=0,read_write::ymax=0;

uint32 read_write::train_x=IMAGE_X_SIZE,read_write::train_y=IMAGE_Y_SIZE;
uint32 read_write::train_max_x=P_IMAGE_X,read_write::train_max_y=P_IMAGE_Y;

 uint32 read_write::object_count=0;
 float read_write::A_x_fact=1,read_write::A_y_fact=1,read_write::P_x_fact=P_FACT,read_write::P_y_fact=P_FACT,read_write::M_x_fact=M_FACT,read_write::M_y_fact=4,read_write::RGB_x_fact=RGB_FACT,read_write::RGB_y_fact=RGB_FACT;
 string read_write::data_folder_path=folder;
 string read_write::file_name_of_image="NAN";
 TIFF* read_write::P_image;
 TIFF* read_write::A_image;
 TIFF* read_write::M_image;
 TIFF* read_write::RGB_image;



int main(int argc, char** argv) {
    // Initialize the MPI environment


    // Get the number of processes
        MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);


    // Get the rank of the process
    int world_rank;
    //srand (time(NULL));
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


    string *file_names=nullptr;
    uint16 no_of_file=0;//file_selection(folder,grid_file,file_names);
    uint16 train_size=0;//no_of_file*TRAINING_PERCENTAGE/100;
    uint16 test_size=0,temp=0,ram=0;//no_of_file*TESTING_PERCENTAGE/100,temp=0,ram=0;
    string *train_file=nullptr;//new string[train_size];
    string *testing_file=nullptr;//new string[test_size];
    int i;
    uint16 future[]={10,10,10,10,1};
    /*
    for(i=0;i<train_size;i++)
    {
	//ram=rand();
	
        temp=rand()%(no_of_file-1)+0;
	train_file[i]=file_names[temp];
	
    }
    for(i=0;i<test_size;i++)
    {
	temp=rand()%(no_of_file-1)+0;
	testing_file[i]=file_names[temp];
    }
    uint16 future[]={10,10,10,10,1};
    train_size=1;test_size=1;
*/
/*
    mpi_alert_parallel_convolutional_net_layer **cp=new mpi_alert_parallel_convolutional_net_layer*[NO_LAYERS];
    for(int ii=0;ii<NO_LAYERS;ii++)
    {
        cp[ii]=new mpi_alert_parallel_convolutional_net_layer(RGB_IMAGE_X,RGB_IMAGE_Y,WEIGHT_X_SIZE,WEIGHT_Y_SIZE);
        cp[ii]->fu=future[ii];
        init_weight(future[ii],(WEIGHT_X_SIZE*WEIGHT_Y_SIZE+1),cp[ii]->weight);
        cp[ii]->init_weight();
    //}
   // read_write **rw;
   // float ***temp_image;
   // image_preprocessing(train_file[0],rw,temp_image);
   // mpi_parallel_convolutional_neural_network_training(cp,future,NO_LAYERS,temp_image[0],rw[0]->train_array,0,0,RGB_IMAGE_X,RGB_IMAGE_Y);
   // mpi_parallel_convolutional_neural_network_testing(cp,future,NO_LAYERS,10,temp_image[0],rw[0]->train_array,0,0,RGB_IMAGE_X,RGB_IMAGE_Y);

*/


if(world_rank==0)
{
      omp_set_num_threads(8);
//master(world_size);
    //cout<<"In master ";
    srand (time(NULL));

    no_of_file=file_selection(folder,grid_file,file_names);
    train_size=no_of_file*TRAINING_PERCENTAGE/100;
    test_size=no_of_file*TESTING_PERCENTAGE/100,temp=0,ram=0;
    train_file=new string[train_size];
    testing_file=new string[test_size];
    int i;

    for(i=0;i<train_size;i++)
    {
    //ram=rand();

        temp=rand()%(no_of_file-1)+0;
    train_file[i]=file_names[temp];

    }
    for(i=0;i<test_size;i++)
    {
    temp=rand()%(no_of_file-1)+0;
    testing_file[i]=file_names[temp];
    }
    //future[]={10,10,10,10,1};
    train_size=1;test_size=1;



    double avg_mach= master(world_size,future,train_file,train_size,testing_file,test_size);
//   cout<<"Avarage percentage of mach found ="<<avg_mach;
}


if(world_rank>0){
      omp_set_num_threads(NO_THREADS);
slave(future,world_size,world_rank);
}

MPI_Finalize();
}
