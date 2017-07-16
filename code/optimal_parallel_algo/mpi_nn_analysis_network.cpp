/*#include "mpi_alert_cnn.cpp"
#include "softmax_backprpagation.cpp"*/
//#include "macro.cpp"
/*
double mpi_parallel_convolutional_neural_network_testing(mpi_alert_parallel_convolutional_net_layer **p,uint16 *feture,uint16 layer,uint16 no_class,int root_id,uint32 x_begin,uint32 y_begin,uint32 image_x,uint32 image_y)
{
    int i,j;
    for( i=0;i<layer;i++)
            {

                if(i==0)
                {
                    //p[i]->input=image;
                    //p[i]->frowork_oparation(true,root_id,x_begin,y_begin);//
                    p[i]->frowork_oparation(true,x_begin,y_begin);//
                    //frowork_oparation(true,x_begin,y_begine);

                    continue;
                }
                for(j=0;j<feture[i-1];j++)
                {
                    p[i]->input=p[i-1]->output[j];
                    p[i]->frowork_oparation(true,x_begin,y_begin);
                }
            }

    mpi_parallel_softmax_error s;
    float** test_result=s.mean_test_squre_error(p[layer-1]->output[0],image_x,image_y,no_class);
    uint32 total=s.percentage_of_mach_found(test_result,root_id,image_x,image_y);
    return (total/(image_x*image_y));


}

void mpi_parallel_convolutional_neural_network_training(mpi_alert_parallel_convolutional_net_layer **p,uint16 *feture,uint16 layer,int root_id,uint32 x_begin,uint32 y_begin,uint32 image_x,uint32 image_y)
{
float gradiant=1.0;int i,j,k;

for( i=0;i<layer;i++)
        {

            if(i==0)
            {
                //for(k=0;k<image_x;k++)
               // {
                    //MPI_Recv(void* data,int count, MPI_Datatype datatype,int source,int tag, MPI_Comm communicator,MPI_Status* status)
                   // p[i]->input=image;
                    p[i]->frowork_oparation(false,x_begin,y_begin);
                    continue;

                //}


            }
            for(j=0;j<feture[i-1];j++)
            {
                p[i]->input=p[i-1]->output[j];
                p[i]->frowork_oparation(false,x_begin,y_begin);
            }

            for(j=i;j>=0;j--)
            {
                gradiant=p[j]->partial_training(gradiant);
            }

        }
        //parallel_softmax_error s;

        //float de=s.dirivation_error(train_array,p[layer-1]->output[0],image_x,image_y);
     /*   mpi_parallel_softmax_error s;

        float de=s.dirivation_error(p[layer-1]->output[0],root_id,image_x,image_y);

        for(j=layer-1;j>=0;j--)
        {
           de=p[j]->back_propagation(de);
        }

}*/

double mpi_parallel_convolutional_neural_network_testing(mpi_alert_parallel_convolutional_net_layer **p,uint16 *feture,uint16 layer,uint16 no_class,float **image,float **level,uint32 x_begin,uint32 y_begin,uint32 image_x,uint32 image_y)
{
    int i,j;
    for( i=0;i<layer;i++)
            {

                if(i==0)
                {
                    p[i]->input=image;
                    //p[i]->frowork_oparation(true,root_id,x_begin,y_begin);//

                    p[i]->frowork_oparation(true,x_begin,y_begin);//
                    //frowork_oparation(true,x_begin,y_begine);

                    continue;
                }
                for(j=0;j<feture[i-1];j++)
                {
                    p[i]->input=p[i-1]->output[j];
                    p[i]->frowork_oparation(true,x_begin,y_begin);
                }
            }

    mpi_parallel_softmax_error s;

    float** test_result=s.mean_test_squre_error(p[layer-1]->output[0],image_x,image_y,no_class);

    uint32 total=s.percentage_of_mach_found(test_result,level,image_x,image_y);

    return (total/(image_x*image_y));


}

void mpi_parallel_convolutional_neural_network_training(mpi_alert_parallel_convolutional_net_layer **p,uint16 *feture,uint16 layer,float **image,float** level,uint32 x_begin,uint32 y_begin,uint32 image_x,uint32 image_y)
{
float gradiant=1.0;int i,j,k;

for( i=0;i<layer;i++)
        {

            if(i==0)
            {
                //for(k=0;k<image_x;k++)
               // {
                    //MPI_Recv(void* data,int count, MPI_Datatype datatype,int source,int tag, MPI_Comm communicator,MPI_Status* status)
                    p[i]->input=image;
                    p[i]->frowork_oparation(false,x_begin,y_begin);
                    continue;

                //}


            }
            for(j=0;j<feture[i-1];j++)
            {
                p[i]->input=p[i-1]->output[j];
                p[i]->frowork_oparation(false,x_begin,y_begin);
            }

            for(j=i;j>=0;j--)
            {
                gradiant=p[j]->partial_training(gradiant);
            }

        }
        //parallel_softmax_error s;

        //float de=s.dirivation_error(train_array,p[layer-1]->output[0],image_x,image_y);
        mpi_parallel_softmax_error s;

        float de=s.dirivation_error(p[layer-1]->output[0],level,image_x,image_y);

        for(j=layer-1;j>=0;j--)
        {
           de=p[j]->back_propagation(de);
        }

}

