float** neural_network_testing(convolutional_layer **p,uint16 *feture,uint16 layer,float** image,uint16 image_x,uint16 image_y,uint16 no_class)
{
    int i,j;
    for( i=0;i<layer;i++)
            {

                if(i==0)
                {
                    p[i]->input=image;
                    p[i]->frowork_oparation();
                    continue;
                }
                for(j=0;j<feture[i-1];j++)
                {
                    p[i]->input=p[i-1]->output[j];
                    p[i]->frowork_oparation();
                }
            }

    softmax_error s;
    float** test_result=s.mean_test_squre_error(p[layer-1]->output[0],image_x,image_y,no_class);
    return test_result;


}

void neural_network_training(convolutional_layer **p,uint16 *feture,uint16 layer,float** image,uint16 image_x,uint16 image_y,float** train_array)
{
float gradiant=1.0;int i,j;

for( i=0;i<layer;i++)
        {

            if(i==0)
            {
                p[i]->input=image;
                p[i]->frowork_oparation();
                continue;
            }
            for(j=0;j<feture[i-1];j++)
            {
                p[i]->input=p[i-1]->output[j];
                p[i]->frowork_oparation();
            }

            for(j=i;j>=0;j--)
            {
                gradiant=p[j]->partial_training(gradiant);
            }

        }
        softmax_error s;
        float de=s.dirivation_error(train_array,p[layer-1]->output[0],image_x,image_y);
        for(j=layer-1;j>=0;j--)
        {
           de=p[j]->back_propagation(de);
           //cout<<de<<"\n";
        }

}
float** convolutional_neural_network_testing(convolutional_net_layer **p,uint16 *feture,uint16 layer,float** image,uint16 image_x,uint16 image_y,uint16 no_class)
{
    int i,j;
    for( i=0;i<layer;i++)
            {

                if(i==0)
                {
                    p[i]->input=image;
                    p[i]->frowork_oparation();
                    continue;
                }
                for(j=0;j<feture[i-1];j++)
                {
                    p[i]->input=p[i-1]->output[j];
                    p[i]->frowork_oparation();
                }
            }

    softmax_error s;
    float** test_result=s.mean_test_squre_error(p[layer-1]->output[0],image_x,image_y,no_class);
    return test_result;


}

void convolutional_neural_network_training(convolutional_net_layer **p,uint16 *feture,uint16 layer,float** image,uint16 image_x,uint16 image_y,float** train_array)
{
float gradiant=1.0;int i,j;

for( i=0;i<layer;i++)
        {

            if(i==0)
            {
                p[i]->input=image;
                p[i]->frowork_oparation();
                continue;
            }
            for(j=0;j<feture[i-1];j++)
            {
                p[i]->input=p[i-1]->output[j];
                p[i]->frowork_oparation();
            }

            for(j=i;j>=0;j--)
            {
                gradiant=p[j]->partial_training(gradiant);
            }

        }
        softmax_error s;
        float de=s.dirivation_error(train_array,p[layer-1]->output[0],image_x,image_y);
        for(j=layer-1;j>=0;j--)
        {
           de=p[j]->back_propagation(de);
        }

}
