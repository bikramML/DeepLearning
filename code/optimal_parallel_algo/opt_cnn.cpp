
//idef Gaussian(w,x,y,z,d):
// return 1/(4*(math.pi)*(math.pi)*d*d*d*d)*(math.exp(-(w*w+x*x+y*y+z*z)/(2*d*d)))

#define LR 0.5

//#define  WEIGHT_INIT(weight)\


class opt_parallel_convolutional_net_layer
{
private:

    //float***** input_gradient;
    //float**** output_gradient;
    uint32 in_rows,in_colums;
    uint32 w_row,w_colum,weight_size;
    uint16 slot=1;
    float weight_sum(float *s,uint16 x,uint16 y)
    {
        uint16 i,j;float temp=0;
#pragma omp parallel for private(j) reduction(+:temp) //collapse(2)
        for(i=0;i<x;i++)
            for(j=0;j<x;j++)
                temp+=s[i*y+j];
        return temp;
    }
    //uint16_t in_rows,in_colums;

    float weight_int()
    {
        return (rand()%weight_size) * sqrt(2.0/ weight_size);
    }



    void weight_sum(float *w_total,float **w_row_total,float **w_col_total)
    {
        uint16 i,j,k;
        uint32 weight_row=w_row,weight_colum=w_colum;
#pragma omp parallel for private(j,k)
        for(i=0;i<fu;i++)
        {
            w_row_total[i]=new float[weight_row];
            w_col_total[i]=new float[weight_colum];
#pragma omp simd private(k)  //collapse(2)
            for(j=0;j<weight_row;j++)
            {

                for(k=0;k<weight_colum;k++)
                {
                    w_row_total[i][j]+=weight[i][j*weight_colum+k];
                    w_col_total[i][k]+=weight[i][j*weight_colum+k];
                    w_total[i]+=weight[i][j*weight_colum+k];
                }
            }
            w_total[i]+=weight[i][weight_size-1];
        }

    }

    void front_part_operation_for_forword(bool test)
    {
        uint16 i,j,k,m,n;
        float* weight_total=new float[fu];
        float **w_row_total=new float*[fu];float **w_col_total=new float*[fu];
        weight_sum(weight_total,w_row_total,w_col_total);
        float *midile_val=new float[fu];
        for(i=0;i<fu;i++)
            midile_val[i]=(weight_total[i]-weight[i][weight_size-1])/weight_total[i];
        //uint16 o,p;int defx,defy;
//#pragma omp barrier
#pragma omp parallel for private(i,k,m,n) //collapse(2)


        for(j=0;j<in_rows;j+=slot)//in_rows-w_row
//#pragma omp simd private(i,k,m,n) //collapse(3)

        for( k=0;k<in_colums;k+=slot) //in_colums-w_colum

            for(i=0;i<fu;i++)
            {
            //float weight_total=weight_sum(weight[i],w_row,w_colum);

        //for( m=0;m<w_row;m++)

        //for( n=0;n<w_colum;n++)
                if((j>=w_row)&&(k>=w_colum)) //j=5,6,7,  k=5,6,7
                {
                    if((j)>(RGB_IMAGE_X-w_row)) // j=196
                    {
                        if((k)>(RGB_IMAGE_Y-w_colum)) //k=46
                        {
                             for(m=(w_row-(RGB_IMAGE_X-j)-1);m<w_row;m++)
                                {
                                    for(n=(w_colum-(RGB_IMAGE_Y-k)-1);n<w_colum;n++)
                                    {
                                        //cout<<input[j][k];
                                        output[i][j][k]=output[i][j][k]+(weight[i][w_colum*m+n]*input[j][k])/weight_total[i];

                                    }
                                }
                             output[i][j][k]=(1/1+(exp(-output[i][j][k])));
                            // gradient[i][w_colum*m+n]=gradient[i][w_colum*m+n]+input_gradient[i][m][n][j][k]*output[i][j+m][k+n];
                            }
                            else  //k=5,..,45
                            {
                                for(m=(w_row-(RGB_IMAGE_X-j)-1);m<w_row;m++)
                                {
                                    output[i][j][k]=output[i][j][k]+(w_row_total[i][m]*input[j][k]);

                                }
                                output[i][j][k]=(1/1+(exp(-output[i][j][k])));
                            }

                        }
                    else //
                    {
                        if((k)>(RGB_IMAGE_Y-w_colum)) //k=46
                        {
                                  for(n=(w_colum-(RGB_IMAGE_Y-k)-1);n<w_colum;n++)
                                    {
                                        output[i][j][k]=output[i][j][k]+(w_col_total[i][n]*input[j][k]);
                                    }
                                  output[i][j][k]=(1/1+(exp(-output[i][j][k])));
                       }/*
                        if((j)>(RGB_IMAGE_X-w_row))
                        {
                                  for(m=(w_row-(RGB_IMAGE_X-j)-1);n<w_row;m++)
                                    {
                                        output[i][j][k]=output[i][j][k]+(w_col_total[i][n]*input[j][k]);
                                    }
                       }*/
                        else //j=5,195 k=5,45
                        {
                            output[i][j][k]=output[i][j][k]+midile_val[i]*input[j][k];
                            output[i][j][k]=(1/1+(exp(-output[i][j][k])));
                        }

                    }
                }
                else
                {
                    if(((j)>=w_row)&&((k)<=w_colum)) //j=5 k=0,5
                    {
                        if((j)>(RGB_IMAGE_X-w_row)) //j=196
                        {
                            for(m=0;m<(RGB_IMAGE_X-j-1);m++)
                            {
                                for(n=0;n<k;n++)
                                {
                                    output[i][j][k]=output[i][j][k]+(weight[i][w_colum*m+n]*input[j][k])/weight_total[i];
                                }
                            }
                            output[i][j][k]=(1/1+(exp(-output[i][j][k])));
                        }
                        else
                        {
                            for(n=0;n<k;n++)
                            {
                                output[i][j][k]=output[i][j][k]+(w_col_total[i][n]*input[j][k]);
                            }
                            output[i][j][k]=(1/1+(exp(-output[i][j][k])));
                        }

                    }
                    else if((j<w_row)&&(k<w_colum)) //j =0,4 k=0,4
                    {
                        for(m=0;m<=j;m++)
                        {
                            for(n=0;n<=k;n++)
                            {
                                output[i][j][k]=output[i][j][k]+(weight[i][w_colum*m+n]*input[j][k])/weight_total[i];
                            }
                        }
                        output[i][j][k]=(1/1+(exp(-output[i][j][k])));
                    }
                    else if((j<w_row)&&(k>w_colum)) //j=0,4 k=5,50
                    {
                        if((k)>=(RGB_IMAGE_Y-w_colum)) //j=195,200
                        {
                    //        #pragma omp parallel for
                        for(m=0;m<j;m++)
                        {
                            for(n=(RGB_IMAGE_Y-k-1);n<w_colum;n++)
                            {
                                output[i][j][k]=output[i][j][k]+(weight[i][w_colum*m+n]*input[j][k])/weight_total[i];
                            }
                        }
                        output[i][j][k]=(1/1+(exp(-output[i][j][k])));
                       }
                    else
                    {
                        for(m=0;m<j;m++)
                        {
                            output[i][j][k]=output[i][j][k]+(w_row_total[i][m]*input[j][k]);

                        }
                        output[i][j][k]=(1/1+(exp(-output[i][j][k])));
                    }
                }

             }

/*

                if(!test)
                 {

                //   o=j;p=k;

         for( m=0;m<w_row;m++)

         for( n=0;n<w_colum;n++)
         {
             defx=j-m;defy=k-n;
             if(((defx>=0)&&(j<(RGB_IMAGE_X-w_row+m)))&&((defy>=0)&&(k<(RGB_IMAGE_Y-w_colum+n)))) //defx=0,1
             {

                 input_gradient[i][m][n][defx][defy] =input[j][k];

             }


         }


        }

*/

        }
    }






    void back_part_operation_for_forword()
    {

        uint16 i,j,k,m,n;
#pragma omp parallel for private(j,k,m,n) //collapse(3)
        for(i=0;i<fu;i++)
        {

        for(j=0;j<in_rows-w_row;j+=slot)
//#pragma omp parallel for private(m,n)
        for( k=0;k<in_colums-w_colum;k+=slot)
         {
            //cout<<w_rows;
//#pragma omp parallel for private(n)
#pragma omp simd private(n) //collapse(2)
        for( m=0;m<w_row;m++)
//#pragma omp parallel for
        for( n=0;n<w_colum;n++)
        {


             //output[i][j+m][k+n]=(1/1+(exp(-output[i][j+m][k+n])));

             //gradient[i][w_colum*m+n]=gradient[i][w_colum*m+n]+input_gradient[i][m][n][j][k]*output[i][j+m][k+n];
                 gradient[i][w_colum*m+n]=gradient[i][w_colum*m+n]+input[j+m][k+n]*output[i][j+m][k+n];
        }

        }
        }
//#pragma omp barrier
        //delete[] input_gradient;
    }
public:
    float*** output=nullptr;
    float **input;

    float** weight;
    float** gradient;
    uint16 fu=10;
   // uint32 weight_size;
    opt_parallel_convolutional_net_layer(uint16 image_rows,uint16 image_colums,uint16 w_rows,uint16 w_colums)
    {

        weight_size=(w_rows*w_colums+1);
        w_row=w_rows;w_colum=w_colums;
        in_rows=image_rows;in_colums=image_colums;
  //
    }
    void reset_output()
    {
        uint32 j,k,m;
#pragma omp parallel for private(k,m) //collapse(3)
        for(j=0;j<fu;j++)
//#pragma omp for private(m)
            for(k=0;k<in_rows;k++)
//#pragma omp for
                for(m=0;m<in_colums;m++)
                    output[j][k][m] = weight[j][weight_size-1];
    }

    void init_weight()
    {
        //weight_size=125;
               uint16 i,j,k,m;

               weight=new float*[fu];
               gradient=new float*[fu];
               output=new float**[fu];
               //cout<<weight_size;
      //         input_gradient=new float****[fu];
      /* #pragma omp parallel for private(j,k,m) //collapse(3)
               for(i=0;i<fu;i++)
               {

        //           input_gradient[i]=new float***[w_row];
       //#pragma omp for private(k,m)
               for(j=0;j<w_row;j++)
               {
                   input_gradient[i][j]=new float**[w_colum];
       //#pragma omp for private(m)
                   for(k=0;k<w_colum;k++)
                   {
                       input_gradient[i][j][k]=new float*[in_rows];
       //#pragma omp for
                       for(m=0;m<in_rows-w_row;m++)
                       {
                           input_gradient[i][j][k][m]=new float[in_colums]();

                       }
                   }
               }
               }*/
#pragma omp parallel for private(k) //collapse(3)
               for(j=0;j<fu;j++)
               {

               //    break;
                   weight[j]=new float[weight_size];
                   gradient[j]=new float[weight_size]();
                   output[j]=new float*[in_rows]();
                   //cout<<"Hello"<<in_rows;
//#pragma omp for
#pragma omp simd
                   for( k=0;k<in_rows;k++)
                   {
                         output[j][k]=new float[in_colums]();

                   }
//#pragma omp barrier
//#pragma omp parallel for
                   #pragma omp simd
                    for( k=0;k<weight_size;k++)
                    {
                          weight[j][k]=weight_int();

                    }
//#pragma omp barrier
//#pragma omp for
                    #pragma omp simd private(m) //collapse(2)
                    for(k=0;k<in_rows;k++)
//#pragma omp for
                        for( m=0;m<in_colums;m++)
                    {
                        // Add baise
                         gradient[j][weight_size-1]=1;
                    }

               }


    }

    void frowork_oparation(bool test)
    {
        //input;

          reset_output();
          front_part_operation_for_forword(test);
          if(!test)
          back_part_operation_for_forword();
    }

    float partial_training(float gradient_sum)
    {
        uint16 i,j,k;
        float sum=0;
#pragma omp parallel for private(j,k) reduction(+:sum) //collapse(3)
        for(i=0;i<fu;i++)
#pragma omp simd private(j) //collapse(2)
        for(j=0;j<w_row;j++)
//#pragma omp for
         for( k=0;k<w_colum;k++)
            {
             gradient[i][w_colum*j+k]=gradient[i][w_colum*j+k]*gradient_sum;
             sum =sum+gradient[i][w_colum*j+k];
         }
        return sum;
    }


    float back_propagation(float gradient_sum)
    {

        uint16 i,j,k;
        float sum=0;
#pragma omp parallel for private(j,k) reduction(+:sum) //collapse(3)
        for(i=0;i<fu;i++)
#pragma omp simd private(k) //collapse(2)
        for(j=0;j<w_row;j++)
//#pragma omp for
         for( k=0;k<w_colum;k++)
            {
             gradient[i][w_colum*j+k]=gradient[i][w_colum*j+k]*gradient_sum;
             weight[i][j*w_colum+k]=weight[i][j*w_colum+k]-LR*gradient[i][w_colum*j+k];
             sum =sum+gradient[i][w_colum*j+k];
             gradient[i][w_colum*j+k]=0;
         }
        return sum;
    }


};

