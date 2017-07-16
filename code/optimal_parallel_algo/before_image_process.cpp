float weight_int(uint16 weight_size)
    {
        return (rand()%weight_size) * sqrt(2.0/ weight_size);
    }
void init_weight(uint16 fu,uint16 weight_size,float **&weight)
    {
        //weight_size=125;
               uint16 i,j,k;

               weight=new float*[fu];
               //cout<<weight_size;

#pragma omp parallel for private(k) //collapse(3)
               for(j=0;j<fu;j++)
               {

               //    break;
                   weight[j]=new float[weight_size];
                   //cout<<"Hello"<<in_rows;

#pragma omp parallel for
                    for( k=0;k<weight_size;k++)
                    {
                          weight[j][k]=weight_int(weight_size);

                    }

               }


}



void parallel_image_preprocessing(string image_name,read_write  **&images,float  ***&temp_images)
{
    uint16 x=0;static bool frist=true;
    if(frist)
    images=new read_write*[NO_IMAGES_PARTS];
    if(frist)
    temp_images=new float**[NO_IMAGES_PARTS];

    static float **nirup =new float*[RGB_IMAGE_X];
    static float **nir_1 =new float*[RGB_IMAGE_X];
    static float **nir_2 =new float*[RGB_IMAGE_X];
    static float **nir_3 =new float*[RGB_IMAGE_X];
    //static float **nir_4 =new float*[RGB_IMAGE_X];
    if(frist)
        for(uint32 j=0;j<NO_IMAGES_PARTS;j++)
        {
            temp_images[j]=new float*[RGB_IMAGE_X];
        for(uint32 i=0;i<RGB_IMAGE_X;i++)
        {
            temp_images[j][i] =new float[RGB_IMAGE_Y];
           // nir_4[i] =new float[RGB_IMAGE_Y];
        }
        }
    if(frist)
    for(uint32 i=0;i<RGB_IMAGE_X;i++)
    {
        nirup[i] =new float[RGB_IMAGE_Y];
        nir_1[i] =new float[RGB_IMAGE_Y];
        nir_2[i] =new float[RGB_IMAGE_Y];
        nir_3[i] =new float[RGB_IMAGE_Y];
       // nir_4[i] =new float[RGB_IMAGE_Y];
    }
    static parallel_image_matrix nir_max(nirup,RGB_IMAGE_X,RGB_IMAGE_Y);
    static parallel_image_matrix nir11(nir_1,RGB_IMAGE_X,RGB_IMAGE_Y);
    static parallel_image_matrix nir12(nir_2,RGB_IMAGE_X,RGB_IMAGE_Y);
    static parallel_image_matrix nir13(nir_3,RGB_IMAGE_X,RGB_IMAGE_Y);


    //images[0]=new read_write("6040_1_3",SWIR_X,SWIR_Y,0,0);
     //images[1]=new read_write("6040_1_3",SWIR_X,SWIR_Y,0,Y_PART);

  //if()
    
    for(uint32 i=0;i<IMAGE_X_SIZE;i+=X_PART)
        for(uint32 j=0;j<IMAGE_Y_SIZE;j+=Y_PART)
     {
//            cout<<x<<" "<<image_name<<" "<<SWIR_X<<" "<<SWIR_Y<<" "<<i<<" "<<j<<"\n";
            if(frist)
            images[x]=new read_write(image_name,SWIR_X,SWIR_Y,i,j);
            else
                images[x]->reset(image_name,SWIR_X,SWIR_Y,i,j);
        parallel_image_matrix nir1(images[x]->image_M[6],M_IMAGE_X,M_IMAGE_Y);

         parallel_image_matrix r(images[x]->image_3[0],RGB_IMAGE_X,RGB_IMAGE_Y);
         parallel_image_matrix g(images[x]->image_3[1],RGB_IMAGE_X,RGB_IMAGE_Y);
         parallel_image_matrix b(images[x]->image_3[2],RGB_IMAGE_X,RGB_IMAGE_Y);
         nir1.matrix_size_incr(RGB_IMAGE_X,RGB_IMAGE_Y,nir_max);
        // nir1.matrix_size_incr(RGB_IMAGE_X,RGB_IMAGE_Y,nir_max);
         nir_max.matrix_scale_up(0.1,nir11);
         nir_max.matrix_scale_up(0.3,nir12);
         nir_max.matrix_scale_up(0.2,nir13);
         //cout<<images[x]->image_M[6][0][0];
         //uint16 image_x=250,image_y=250;

         //nir1.matrix_size_incr(RGB_IMAGE_X,RGB_IMAGE_Y);
         //parallel_image_matrix nir11(nir1.matrix_scale_up(0.1),RGB_IMAGE_X,RGB_IMAGE_Y);
         //parallel_image_matrix nir12(nir1.matrix_scale_up(0.3),RGB_IMAGE_X,RGB_IMAGE_Y);
         //parallel_image_matrix nir13(nir1.matrix_scale_up(0.2),RGB_IMAGE_X,RGB_IMAGE_Y);
         nir11.matrix_multi(r);
         nir12.matrix_multi(g);
         nir13.matrix_multi(b);
         nir12.matrix_add(nir13);
         nir11.matrix_add(nir12);
         nir11.matrix_transfar_parameter(temp_images[x]);
         //parallel_image_matrix temp=(nir12+nir13);

         //temp=(temp+nir11);
      //     nir11.~parallel_image_matrix();
       //    nir12.~parallel_image_matrix();
       //    nir13.~parallel_image_matrix();
        // temp_images[x] = temp.p;

         x=x+1;
     }
    //cout<<x;
    if(frist) frist=false;

}




