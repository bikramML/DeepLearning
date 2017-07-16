void forword_operation_partial_training(float **image,convolutional_layer *c_l,uint16 layers,uint16 *futures,float *fact,bool train)
{
    float** in=image;
    float gradient_sum=0.0;
    int i,j;




    for(i=0;i<layers;i++)
    {
          c_l[i].fact=fact[i];
            c_l[i].fu=futures[i];
            /*
          c_l[i].in_rows=250;

          c_l[i].in_colums=250;
          c_l[i].weight_size=c_l[i].in_rows*c_l[i].in_colums+1;
          cout<<c_l[i].fu<<"\t"<<c_l[i].in_colums<<c_l[i].weight_size;*/
          c_l[i].init_weight();





        if(i==0)
        {
            c_l[i].input=in;
            c_l[i].frowork_oparation();

            continue;
        }

        else
        for(j=0;j<futures[i-1];j++)
        {

            c_l[i].input=c_l[i-1].output[j];
             c_l[i].frowork_oparation();

        }

        if((train)&&i>0)
        {
        gradient_sum=1.0;

        for(j=i;j>=0;j--)
        {

        gradient_sum=c_l[j].partial_training(gradient_sum);
        }
        }

    }
}




uint16** neural_network(float** train,float** train_level,float** test,uint32 train_x,uint32 train_y,uint16 layers,uint16 *futures,float *fact,uint16 no_class)
{
    uint16** test_reslt=nullptr;
    uint16 i,j,k;int l=0;

    // /////////// Train forword operation////////////
    convolutional_layer *c_l;//[10](train_x,train_y);

    for(i=0;i<layers;i++)
    {
        c_l=new convolutional_layer(train_x,train_y);

    }


    forword_operation_partial_training(train,c_l,layers,futures,fact,true);
     softmax_error s;
    // Back propagation /////////////////////
/*
    float de=s.dirivation_error(train_level,c_l[layers-1].output[0],train_x,train_y);
    for(l=layers-1;l>=0;l--)
    {
       de=c_l[j].back_propagation(de);
    }

    // /////////Test the images ////////////////////

    // /////////////// Testing ///////////////////////


    forword_operation_partial_training(test,c_l,layers,futures,fact,false);
    test_reslt=s.mean_test_squre_error(c_l[layers-1].output[0],train_x,train_y,no_class);
*/
    return test_reslt;
}




