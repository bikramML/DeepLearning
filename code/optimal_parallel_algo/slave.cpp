//#include "macro.cpp"
void slave(uint16 *futures,int world_size,int rank)
{

//cout<<futures[0];
    // init cnn by weight //
    MPI_Status status;float **image_buff=new float*[RGB_IMAGE_X],**level_buff=new float*[RGB_IMAGE_X];
    int i,j,k;
    for(i=0;i<RGB_IMAGE_X;i++)
    {
        image_buff[i]=new float[RGB_IMAGE_Y];
        level_buff[i]=new float[RGB_IMAGE_Y];
    }

        MPI_Request request;
      mpi_alert_parallel_convolutional_net_layer **mpi_p_cnn=new mpi_alert_parallel_convolutional_net_layer*[NO_LAYERS];

      for( i=0;i<NO_LAYERS;i++)
      {
          mpi_p_cnn[i]=new mpi_alert_parallel_convolutional_net_layer(RGB_IMAGE_X,(RGB_IMAGE_Y/(world_size-1)),WEIGHT_X_SIZE,WEIGHT_Y_SIZE);
          mpi_p_cnn[i]->fu=futures[i];
          mpi_p_cnn[i]->weight=new float*[futures[i]];
          mpi_p_cnn[i]->process_id=rank;
          mpi_p_cnn[i]->world_size=world_size;
          for(j=0;j<futures[i];j++)
          {
              mpi_p_cnn[i]->weight[j]=new float[(WEIGHT_X_SIZE*WEIGHT_Y_SIZE+1)];
              //MPI_Ibcast(mpi_p_cnn[i]->weight[j],(WEIGHT_X_SIZE*WEIGHT_Y_SIZE+1),MPI::FLOAT,0,MPI_COMM_WORLD,&request) ;
             MPI_Bcast(mpi_p_cnn[i]->weight[j],(WEIGHT_X_SIZE*WEIGHT_Y_SIZE+1),MPI::FLOAT,0,MPI_COMM_WORLD);
           }
          mpi_p_cnn[i]->init_weight();

     }

    // start traing by image ///

      uint32 total_no_training_set,total_no_testing_set=0;
    MPI_Bcast(&total_no_training_set,1,MPI::UNSIGNED,0,MPI_COMM_WORLD);

    for(i=0;i<total_no_training_set;i++)
    {

        for(j=0;j<RGB_IMAGE_X;j++)
        {
         //   cout<<"in slave";
            MPI_Recv(image_buff[j],(RGB_IMAGE_Y/(world_size-1)), MPI_FLOAT,0,j,MPI_COMM_WORLD,&status);
            MPI_Recv(level_buff[j],(RGB_IMAGE_Y/(world_size-1)), MPI_FLOAT,0,j+1,MPI_COMM_WORLD,&status);
        }
        mpi_parallel_convolutional_neural_network_training(mpi_p_cnn,futures,NO_LAYERS,image_buff,level_buff,0,(RGB_IMAGE_Y/(world_size-1))*(rank-1),RGB_IMAGE_X,(RGB_IMAGE_Y/(world_size-1)));

    }
    //cout<<"train finish";

//cout<<"out train ";
    // stop traing //

    // start testing ///

    MPI_Bcast(&total_no_testing_set,1,MPI::UNSIGNED,0,MPI_COMM_WORLD);
  //  cout<<"test set="<<total_no_testing_set;

double avg_mach=0,temp=0;
    for(i=0;i<total_no_testing_set;i++)
    {

        for(j=0;j<RGB_IMAGE_X;j++)
        {
         //   cout<<"in slave";
            MPI_Recv(image_buff[j],(RGB_IMAGE_Y/(world_size-1)), MPI_FLOAT,0,j,MPI_COMM_WORLD,&status);
            MPI_Recv(level_buff[j],(RGB_IMAGE_Y/(world_size-1)), MPI_FLOAT,0,j+1,MPI_COMM_WORLD,&status);
        }
        temp=mpi_parallel_convolutional_neural_network_testing(mpi_p_cnn,futures,NO_LAYERS,10,image_buff,level_buff,0,(RGB_IMAGE_Y/(world_size-1))*(rank-1),RGB_IMAGE_X,(RGB_IMAGE_Y/(world_size-1)));
        avg_mach=avg_mach+(temp/total_no_testing_set);
    }


//    MPI_Ibcast(&total_no_testing_set,1,MPI::UNSIGNED,0,MPI_COMM_WORLD,&request) ;
   // double avg_mach=0;
   // double temp=0;
 /*   for(uint32 i=0;i<total_no_testing_set;i++)
    {
        //double temp=mpi_parallel_convolutional_neural_network_testing(mpi_p_cnn,futures,NO_LAYERS,(IMAGE_X_SIZE/(world_size-1)),(IMAGE_Y_SIZE/(world_size-1)),10,0,((IMAGE_Y_SIZE/(world_size-1)*(rank-1))));
        temp=mpi_parallel_convolutional_neural_network_testing(mpi_p_cnn,futures,NO_LAYERS,10,0,0,(RGB_IMAGE_Y/(world_size-1))*(rank-1),RGB_IMAGE_X,(RGB_IMAGE_Y/(world_size-1)));
        avg_mach=avg_mach+(temp/total_no_testing_set);
    }

    // stop testing //

    */
    MPI_Reduce(&avg_mach,&temp,1,MPI::DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

}
