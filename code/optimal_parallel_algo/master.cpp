

void send_array_to_all(uint16 array_size,float *array,uint16 id,uint16 tage_array,uint16 world_size)
{
    //MPI_Status *st=new st[];
    for(uint16 i=1;i<world_size;i++)
    {
        if(i!=id)
        {
            //cout<<"array_size";
           // MPI_Send(&array_size,1,MPI_INT,i,tage_array_size,MPI_COMM_WORLD);
           MPI_Send(array,array_size,MPI_INT,i,tage_array,MPI_COMM_WORLD);
       }
    }

}
void send_array_by_part_to_all(uint16 array_size,float *array,uint16 id,uint16 tage_array,uint16 world_size)
{
    MPI_Request request;
    MPI_Status status;
    for(int i=1;i<world_size;i++)
    {
        //cout<<array_size<<" "<<world_size;
       // MPI_Send(&array_size,1,MPI_INT,i,tage_array_size,MPI_COMM_WORLD);
        //cout<<"In send fun";
        MPI_Send((array+(i-1)*array_size),array_size,MPI::FLOAT,i,tage_array,MPI_COMM_WORLD);
    //    cout<<"\t"<<*(arra+(i-1)*12);
    }
   // cout<<"out from send fun";

    //MPI_Wait(&request,&status);

   // cout<<"End in send fun";
}


double master(int world_size,uint16* future,string *training_image_name,uint16 no_training_images,string *testing_image_name,uint16 no_testing_images)
{
//    cout<<training_image_name[0];
    // Initialize weights
    //double grad_sum=0;
    struct timeval  TimeValue_Start;
    struct timezone TimeZone_Start;
    struct timeval  TimeValue_Final;
    struct timezone TimeZone_Final;
    double time_overhead;
    long  time_start, time_end;


    read_write **rw;float ***temp_images;MPI_Request request;
    uint16 weight_size=WEIGHT_X_SIZE*WEIGHT_Y_SIZE+1;
    uint32 total_no_training_set=no_training_images*NO_IMAGES_PARTS,total_no_testing_set=no_testing_images*NO_IMAGES_PARTS;
    //uint16 tage_array_size,uint16 tage_array;
    float **weight;
    gettimeofday(&TimeValue_Start, &TimeZone_Start);

     for(int i=0;i<NO_LAYERS;i++)
     {
        init_weight(future[i],weight_size,weight);

         // Send weight to every nodes //
        //send_array_to_all(weight_size,weight,0,1,2,world_size);

        for(uint16 j=0;j<future[i];j++)
        {
       // MPI_Bcast(&weight_size,1,MPI::UNSIGNED,0,MPI_COMM_WORLD);
       //     MPI_Ibcast(weight[j],weight_size,MPI::FLOAT,0,MPI_COMM_WORLD,&request) ;
        MPI_Bcast(weight[j],weight_size,MPI::FLOAT,0,MPI_COMM_WORLD);
        }
     }

     total_no_training_set=1;

     // Sen no of training images //
   //  MPI_Ibcast(&total_no_training_set,1,MPI::UNSIGNED,0,MPI_COMM_WORLD,&request) ;
     MPI_Bcast(&total_no_training_set,1,MPI::UNSIGNED,0,MPI_COMM_WORLD);

     gettimeofday(&TimeValue_Final, &TimeZone_Final);
     time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
     time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
     time_overhead = (time_end - time_start)/1000000.0;


    // Send no of training images

    // INIT IMAGES for training

    for(uint16 i=0;i<no_training_images;i++)//no_training_images
    {
        // iNIT IMAGES for training
        //image_preprocessing(training_image_name[i],rw,temp_images);
        parallel_image_preprocessing(training_image_name[i],rw,temp_images);
       // cout<<"No change ";
        // Send Image to nodes//
        gettimeofday(&TimeValue_Start, &TimeZone_Start);

        for(uint16 k=0;k<NO_IMAGES_PARTS;k++) //NO_IMAGES_PARTS
        {
        for(uint16 j=0;j<RGB_IMAGE_X;j++)
        {
            int x=(RGB_IMAGE_Y/(world_size-1));
            //cout<<RGB_IMAGE_Y<<" "<world_size;
          //  cout<<"\nIn master "<<j;
            send_array_by_part_to_all(x,temp_images[k][j],0,j,world_size);
            send_array_by_part_to_all(x,rw[k]->train_array[j],0,j+1,world_size);
        }
        // Send Level to nodes ////

        }
        gettimeofday(&TimeValue_Final, &TimeZone_Final);
        time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
        time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
        time_overhead += (time_end - time_start)/1000000.0;




    }

cout<<"\n total  training time ="<<time_overhead;
     // stop trainining

    //&total_no_training_set,1,MPI::UNSIGNED,0,MPI_COMM_WORLD
    //MPI_Bcast(&total_no_testing_set,1,MPI::UNSIGNED,0,MPI_COMM_WORLD);
    total_no_testing_set=1;
    //MPI_Ibcast(&total_no_testing_set,1,MPI::UNSIGNED,0,MPI_COMM_WORLD,&request);
    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    MPI_Bcast(&total_no_testing_set,1,MPI::UNSIGNED,0,MPI_COMM_WORLD);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_overhead = (time_end - time_start)/1000000.0;


     //  start testing

    for(uint16 i=0;i<no_testing_images;i++)
    {
        // iNIT IMAGES for training
    //    image_preprocessing(testing_image_name[i],rw,temp_images);
        parallel_image_preprocessing(testing_image_name[i],rw,temp_images);
        // Send Image to nodes//
        gettimeofday(&TimeValue_Start, &TimeZone_Start);
        for(uint16 k=0;k<NO_IMAGES_PARTS;k++)
        {
        for(uint16 j=0;j<RGB_IMAGE_X;j++)
        {
            send_array_by_part_to_all((RGB_IMAGE_Y/(world_size-1)),temp_images[k][j],0,j,world_size);
            send_array_by_part_to_all((RGB_IMAGE_Y/(world_size-1)),rw[k]->train_array[j],0,j+1,world_size);
        }
        // Send Level to nodes ////

        }
        gettimeofday(&TimeValue_Final, &TimeZone_Final);
        time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
        time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
        time_overhead += (time_end - time_start)/1000000.0;

    }

     //stop testing
 double avg_mach=0,temp=0;


    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    MPI_Reduce(&temp,&avg_mach,1,MPI::DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;
    time_overhead += (time_end - time_start)/1000000.0;
    cout<<" total testing time ="<<time_overhead<<"\n";

  //  double avg_mach=1;

    return (avg_mach/(world_size-1));
     // show the result



    }
