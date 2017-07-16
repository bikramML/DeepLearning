

#include "macro.cpp"

string folder="../data/";
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

    //omp_set_num_threads(16);
    struct timeval  TimeValue_Start;
    struct timezone TimeZone_Start;
    struct timeval  TimeValue_Final;
    struct timezone TimeZone_Final;
    double time_overhead,time_overhead_cnn,time_overhead_opt,time_overhead_para;
    long  time_start, time_end;



    srand (time(NULL));
    //MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    string *file_names=nullptr;
    uint16 no_of_file=file_selection(folder,grid_file,file_names);
    uint16 train_size=no_of_file*TRAINING_PERCENTAGE/100;
    uint16 test_size=no_of_file*TESTING_PERCENTAGE/100,temp=0,ram=0;
    string *train_file=new string[train_size];
    string *testing_file=new string[test_size];
    int i,j;

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

    read_write **rw;float ***temp_image;
    //cout<<"\n"<<train_size;

/*
    for(i=0;i<train_size;i++)
    {
    //ram=rand();
    parallel_image_preprocessing(train_file[i],rw,temp_image);

    }
  */
    //cout<<"\n finise";
    //int x;
    //cin>>x;
//cout<<rw[0]->image_M[6][0][0];
    //parallel_image_preprocessing(train_file[0],rw,temp_image);

  //  gettimeofday(&TimeValue_Start, &TimeZone_Start);


//    serial_image_preprocessing(train_file[0],rw,temp_image);
    /*

    opt_parallel_convolutional_net_layer **opt_cnn=new opt_parallel_convolutional_net_layer*[NO_LAYERS];
    for(i=0;i<NO_LAYERS;i++)
    {
        opt_cnn[i]=new opt_parallel_convolutional_net_layer(RGB_IMAGE_X,RGB_IMAGE_Y,WEIGHT_X_SIZE,WEIGHT_Y_SIZE);
        opt_cnn[i]->fu=future[i];
        opt_cnn[i]->init_weight();
    }


    //convolutional_neural_network_training<opt_parallel_convolutional_net_layer,parallel_softmax_error>(opt_cnn,future,NO_LAYERS,temp_image[0],RGB_IMAGE_X,RGB_IMAGE_Y,rw[0]->train_array);

    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_overhead_opt += (time_end - time_start)/1000000.0;

    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    parallel_convolutional_net_layer **p_cnn=new parallel_convolutional_net_layer*[NO_LAYERS];
    for(i=0;i<NO_LAYERS;i++)
    {
        p_cnn[i]=new parallel_convolutional_net_layer(RGB_IMAGE_X,RGB_IMAGE_Y,WEIGHT_X_SIZE,WEIGHT_Y_SIZE);
        p_cnn[i]->fu=future[i];
        p_cnn[i]->init_weight();
    }
    //convolutional_neural_network_training<parallel_convolutional_net_layer,parallel_softmax_error>(p_cnn,future,NO_LAYERS,temp_image[0],RGB_IMAGE_X,RGB_IMAGE_Y,rw[0]->train_array);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_overhead_para += (time_end - time_start)/1000000.0;
*/
    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    convolutional_net_layer **cnn=new convolutional_net_layer*[NO_LAYERS];
    for(i=0;i<NO_LAYERS;i++)
    {
        cnn[i]=new convolutional_net_layer(RGB_IMAGE_X,RGB_IMAGE_Y,WEIGHT_X_SIZE,WEIGHT_Y_SIZE);
        cnn[i]->fu=future[i];
        cnn[i]->init_weight();
    }
    //convolutional_neural_network_training<convolutional_net_layer,serial_softmax_error>(cnn,future,NO_LAYERS,temp_image[0],RGB_IMAGE_X,RGB_IMAGE_Y,rw[0]->train_array);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_overhead_cnn += (time_end - time_start)/1000000.0;

    parallel_image_preprocessing(train_file[0],rw,temp_image);
   // for(i=0;i<train_size;i++)
    //{
    //ram=rand();
    //parallel_image_preprocessing(train_file[i],rw,temp_image);
    //for(j=0;j<NO_IMAGES_PARTS;j++)
    //{
    // gettimeofday(&TimeValue_Start, &TimeZone_Start);
   // // convolutional_neural_network_training<opt_parallel_convolutional_net_layer,parallel_softmax_error>(opt_cnn,future,NO_LAYERS,temp_image[i],RGB_IMAGE_X,RGB_IMAGE_Y,rw[i]->train_array);
   //  gettimeofday(&TimeValue_Final, &TimeZone_Final);
   //  time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    // time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
     //time_overhead_opt += (time_end - time_start)/1000000.0;


     //gettimeofday(&TimeValue_Start, &TimeZone_Start);
     //convolutional_neural_network_training<parallel_convolutional_net_layer,parallel_softmax_error>(p_cnn,future,NO_LAYERS,temp_image[i],RGB_IMAGE_X,RGB_IMAGE_Y,rw[i]->train_array);


     //gettimeofday(&TimeValue_Final, &TimeZone_Final);
     //time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
     //time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
     //time_overhead_para += (time_end - time_start)/1000000.0;





     gettimeofday(&TimeValue_Start, &TimeZone_Start);
     for(i=0;i<NO_IMAGES_PARTS;i++)
     {
     convolutional_neural_network_training<convolutional_net_layer,serial_softmax_error>(cnn,future,NO_LAYERS,temp_image[i],RGB_IMAGE_X,RGB_IMAGE_Y,rw[i]->train_array);
     //break;
     }
        //cnn[NO_LAYERS]->output[0][0][0];
     gettimeofday(&TimeValue_Final, &TimeZone_Final);
     time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
     time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
     time_overhead_cnn = (time_end - time_start)/1000000.0;
   // }

   // }













   // cout<<"\nTotal time for training by optimal parallel algorithom ="<<time_overhead_opt;


//    cout<<"\nTotal time for training by parallel algorithom ="<<time_overhead_para;


    cout<<"\nTotal time for training by serial algorithom="<<time_overhead_cnn;



//time_overhead_opt=time_overhead_cnn=time_overhead_para=0;


   // for(i=0;i<test_size;i++)
   // {
    //ram=rand();
   // parallel_image_preprocessing(train_file[0],rw,temp_image);
    //for(j=0;j<NO_IMAGES_PARTS;j++)
    //{
//     gettimeofday(&TimeValue_Start, &TimeZone_Start);
//     convolutional_neural_network_testing<opt_parallel_convolutional_net_layer,parallel_softmax_error>(opt_cnn,future,NO_LAYERS,temp_image[i],RGB_IMAGE_X,RGB_IMAGE_Y,NO_CLASS,rw[i]->train_array);
//     gettimeofday(&TimeValue_Final, &TimeZone_Final);
 //    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
 //    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
  //   time_overhead_opt += (time_end - time_start)/1000000.0;


  //   gettimeofday(&TimeValue_Start, &TimeZone_Start);
  //   convolutional_neural_network_testing<parallel_convolutional_net_layer,parallel_softmax_error>(p_cnn,future,NO_LAYERS,temp_image[i],RGB_IMAGE_X,RGB_IMAGE_Y,NO_CLASS,rw[i]->train_array);


  //   gettimeofday(&TimeValue_Final, &TimeZone_Final);
  //   time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
   //  time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
     //time_overhead_para += (time_end - time_start)/1000000.0;





     gettimeofday(&TimeValue_Start, &TimeZone_Start);
     float result=0;
     for(i=0;i<NO_IMAGES_PARTS;i++)
     {
     result+=convolutional_neural_network_testing<convolutional_net_layer,serial_softmax_error>(cnn,future,NO_LAYERS,temp_image[i],RGB_IMAGE_X,RGB_IMAGE_Y,NO_CLASS,rw[i]->train_array);
    // break;
     }
      cout<<(result/NO_IMAGES_PARTS);
     gettimeofday(&TimeValue_Final, &TimeZone_Final);
     time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
     time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
     time_overhead_cnn = (time_end - time_start)/1000000.0;
    //}

   // }
















/*
    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    convolutional_neural_network_testing<opt_parallel_convolutional_net_layer,parallel_softmax_error>(opt_cnn,future,NO_LAYERS,temp_image[0],RGB_IMAGE_X,RGB_IMAGE_Y,NO_CLASS,rw[0]->train_array);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_overhead = (time_end - time_start)/1000000.0;
    */
 //   cout<<"\nTotal time for testing by optimal parallel algorithom="<<time_overhead_opt;
/*
    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    convolutional_neural_network_testing<parallel_convolutional_net_layer,parallel_softmax_error>(p_cnn,future,NO_LAYERS,temp_image[0],RGB_IMAGE_X,RGB_IMAGE_Y,NO_CLASS,rw[0]->train_array);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_overhead = (time_end - time_start)/1000000.0;
    */
   // cout<<"\nTotal time for testing by parallel algorithom="<<time_overhead_para;
   /* gettimeofday(&TimeValue_Start, &TimeZone_Start);
   convolutional_neural_network_testing<convolutional_net_layer,serial_softmax_error>(cnn,future,NO_LAYERS,temp_image[0],RGB_IMAGE_X,RGB_IMAGE_Y,NO_CLASS,rw[0]->train_array);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_overhead = (time_end - time_start)/1000000.0;
    */
    cout<<"\nTotal time for testing by serial algorithom="<<time_overhead_cnn;

    cout<<"\n Percentage of match found="<<result<<"\n";

/*


    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    convolutional_neural_network_testing<opt_parallel_convolutional_net_layer,parallel_softmax_error>(cnn,future,NO_LAYERS,temp_image[0],RGB_IMAGE_X,RGB_IMAGE_Y,NO_CLASS,rw[0]->train_array);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_overhead += (time_end - time_start)/1000000.0;
    cout<<"Total time ="<<time_overhead;
*/
    //convolutional_neural_network_testing(cnn,future,NO_LAYERS,temp_image[0],RGB_IMAGE_X,RGB_IMAGE_Y,NO_CLASS);

}
