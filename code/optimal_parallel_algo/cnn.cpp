
//idef Gaussian(w,x,y,z,d):
// return 1/(4*(math.pi)*(math.pi)*d*d*d*d)*(math.exp(-(w*w+x*x+y*y+z*z)/(2*d*d)))

#define LR 0.5

//#define  WEIGHT_INIT(weight)\


class parallel_convolutional_net_layer
{
private:

    float***** input_gradient;
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

    void front_part_operation_for_forword()
    {
        uint16 i,j,k,m,n;

//#pragma omp barrier
#pragma omp parallel for private(j,k,m,n) //collapse(3)
        for(i=0;i<fu;i++)
        {
        float weight_total=weight_sum(weight[i],w_row,w_colum);
//#pragma omp for private(k,m,n)
        for(j=0;j<in_rows-w_row;j+=slot)
//#pragma omp for private(m,n)
        for( k=0;k<in_colums-w_colum;k+=slot)
//#pragma omp for private(n)
        for( m=0;m<w_row;m++)
//#pragma omp for
        for( n=0;n<w_colum;n++)
        {
             output[i][j+m][k+n]=output[i][j+m][k+n]+(weight[i][w_colum*m+n]*input[j+m][k+n])/weight_total;
             //cout<<output[i][j+m][k+n];
             input_gradient[i][m][n][j][k]=input[j+m][k+n];


        }


        }
    }


    void weight_sum(float **&row_wish_sum,float **&colum_wish_sum,float *&weight_total)
    {
        uint16 i,j,k,m=w_row,n=w_colum,futures=fu;
        float **s=weight;
        row_wish_sum=new float*[futures];colum_wish_sum=new float*[futures];
        weight_total=new float[futures];float temp;

#pragma omp parallel for private(i,j) reduction(+:temp) //collapse(2)
        for(k=0;k<futures;k++)
        {
            row_wish_sum[k]=new float[m];
            colum_wish_sum[k]=new float[n];
            //temp=0;
            temp=0;

        for(i=0;i<m;i++)
        {
            row_wish_sum[k][i]=0;
            for(j=0;j<n;j++)
            {
                if(i==0)
                {
                    colum_wish_sum[k][j]=s[k][i*n+j];
                }
                else
                    colum_wish_sum[k][j]+=s[k][i*n+j];
                row_wish_sum[k][i]+=s[k][i*n+j];
                temp+=s[k][i*n+j];
            }
        }
        weight_total[k]=temp+s[k][weight_size-1];
        }

#pragma omp parallel for private(i,j)  //collapse(2)
        for(k=0;k<futures;k++)
        {
        for(i=0;i<m;i++)
        {
             row_wish_sum[k][i]=row_wish_sum[k][i]/weight_total[k];

        }
        for(j=0;j<n;j++)
        {
            colum_wish_sum[k][j]=colum_wish_sum[k][j]/weight_total[k];
        }

        }
        //return temp;
    }
    //uint16_t in_rows,in_colums;











    void front_part_operation_for_forword(bool test)
    {
        uint32 i,j,k,m,n;
        float **row_wish_sum;
        float **colum_wish_sum;
        float *weight_total;
        input=new float*[in_rows];
        weight_sum(row_wish_sum,colum_wish_sum,weight_total);
//        MPI_Status st;
        float *midile_val=new float[fu];
        for(i=0;i<fu;i++)
            midile_val[i]=(weight_total[i]-weight[i][weight_size-1])/weight_total[i];

//#pragma omp barrier
#pragma omp parallel for private(i,k,m,n) //collapse(3)

        for(j=0;j<in_rows;j+=slot)
        {
  //          if(first)
  //          {
    //         input[j]=new float[in_colums];
           //  cout<<in_rows;
//            MPI_Recv(input[j],in_colums, MPI_FLOAT,0,in_tag,MPI_COMM_WORLD,&st);
//            }

//#pragma omp for private(m,n)
  //          #pragma omp parallel for private(i,m,n)

        for( k=0;k<in_colums;k+=slot)
            for(i=0;i<fu;i++)
            {
                cout<<input[0][0];
/*

            if((j>=w_row)&&(k>=w_colum))
            {
                if((j)>=(RGB_IMAGE_X-w_row))
                {
                    if((j)>=(RGB_IMAGE_Y-w_colum))
                    {
                         for(m=(RGB_IMAGE_X-j-1);m<w_row;m++)
                            {
                                for(n=(RGB_IMAGE_Y-k-1);n<w_colum;n++)
                                {
                                    cout<<input[j][k];
                                    //output[i][j][k]=output[i][j][k]+(weight[i][w_colum*m+n]*input[j][k])/weight_total[i];
                                }
                            }
                        }
                        else
                        {
                            for(m=(RGB_IMAGE_X-j-1);m<w_row;m++)
                            {
                                //output[i][j][k]=output[i][j][k]+(row_wish_sum[i][m]*input[j+m][k+n]);

                            }
                        }

                    }
                else
                {
                    if((j)>=(RGB_IMAGE_Y-w_colum))
                    {
                              for(n=(RGB_IMAGE_Y-k-1);n<w_colum;n++)
                                {
                                  //  output[i][j][k]=output[i][j][k]+(colum_wish_sum[i][n]*input[j][k]);
                                }
                   }
                    else
                    {
                       // output[i][j][k]=output[i][j][k]+midile_val[i]*input[j][k];
                    }

                }
            }

            /*    else
            {

                if(((j)>=w_row)&&((k)<=w_colum))
                {
                    if((j)>=(RGB_IMAGE_X-w_row))
                    {
                        for(m=0;m<(RGB_IMAGE_X-j-1);m++)
                        {
                            for(n=0;n<k;n++)
                            {
                                output[i][j][k]=output[i][j][k]+(weight[i][w_colum*m+n]*input[j][k])/weight_total[i];
                            }
                        }
                    }
                    else
                    {
                        for(n=0;n<k;n++)
                        {
                            output[i][j][k]=output[i][j][k]+(colum_wish_sum[i][n]*input[j][k]);
                        }
                    }

                }
           //     else if((j<w_row)&&(k<w_colum))
                {
                    for(m=0;m<=j;m++)
                    {
                        for(n=0;n<=k;n++)
                        {
                            output[i][j][k]=output[i][j][k]+(weight[i][w_colum*m+n]*input[j][k])/weight_total[i];
                        }
                    }
                }
             //   else if((j<w_row)&&(k>w_colum))
                {
                    if((j)>=(RGB_IMAGE_Y-w_colum))
                    {
                    for(m=0;m<j;m++)
                    {
                        for(n=(RGB_IMAGE_Y-k-1);n<w_colum;n++)
                        {
                            output[i][j][k]=output[i][j][k]+(weight[i][w_colum*m+n]*input[j][k])/weight_total[i];
                        }
                    }
                   }
                else
                {
                    for(m=0;m<j;m++)
                    {
                        output[i][j][k]=output[i][j][k]+(row_wish_sum[i][m]*input[j][k]);
                    }
                }

            }
               if(!test)
                {
//#pragma omp for private(n)
        if((j<in_rows-w_row)&&(k<in_colums-w_colum))
        {
        for( m=0;m<w_row;m++)
//#pragma omp for
        for( n=0;n<w_colum;n++)
        {
             //output[i][j+m][k+n]=output[i][j+m][k+n]+(weight[i][w_colum*m+n]*input[j+m][k+n])/weight_total;
             //cout<<output[i][j+m][k+n];
             input_gradient[i][m][n][j][k]=input[j+m][k+n];


        }
        }

        }
        }


        */}
    }

}



    void back_part_operation_for_forword(bool test)
    {

        uint16 i,j,k,m,n;
#pragma omp parallel for private(j,k,m,n) //collapse(3)
        for(i=0;i<fu;i++)
        {
//#pragma omp for private(k,m,n)
        for(j=0;j<in_rows-w_row;j+=slot)
//#pragma omp for private(m,n)
        for( k=0;k<in_colums-w_colum;k+=slot)
         {
            //cout<<w_rows;
//#pragma omp for private(n)
        for( m=0;m<w_row;m++)
//#pragma omp for
        for( n=0;n<w_colum;n++)
        {


             output[i][j+m][k+n]=(1/1+(exp(-output[i][j+m][k+n])));
             if(!test)
             gradient[i][w_colum*m+n]=gradient[i][w_colum*m+n]+input_gradient[i][m][n][j][k]*output[i][j+m][k+n];
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
    parallel_convolutional_net_layer(uint16 image_rows,uint16 image_colums,uint16 w_rows,uint16 w_colums)
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
               input_gradient=new float****[fu];
       #pragma omp parallel for private(j,k,m) //collapse(3)
               for(i=0;i<fu;i++)
               {

                   input_gradient[i]=new float***[w_row];
       //#pragma omp for private(k,m)
               for(j=0;j<w_row;j++)
               {
                   input_gradient[i][j]=new float**[w_colum];
       //#pragma omp for private(m)
                   for(k=0;k<w_colum;k++)
                   {
                       input_gradient[i][j][k]=new float*[in_rows-w_row];
       //#pragma omp for
                       for(m=0;m<in_rows-w_row;m++)
                       {
                           input_gradient[i][j][k][m]=new float[in_colums-w_colum];

                       }
                   }
               }
               }
#pragma omp parallel for private(k) //collapse(3)
               for(j=0;j<fu;j++)
               {

               //    break;
                   weight[j]=new float[weight_size];
                   gradient[j]=new float[weight_size]();
                   output[j]=new float*[in_rows]();
                   //cout<<"Hello"<<in_rows;
//#pragma omp for
#pragma omp parallel for
                   for( k=0;k<in_rows;k++)
                   {
                         output[j][k]=new float[in_colums]();

                   }
//#pragma omp barrier
#pragma omp parallel for
                    for( k=0;k<weight_size;k++)
                    {
                          weight[j][k]=weight_int();

                    }
//#pragma omp barrier
//#pragma omp for
                    for(k=0;k<in_rows;k++)
//#pragma omp for
                        for(uint16 m=0;m<in_colums;m++)
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
          front_part_operation_for_forword();
          //back_part_operation_for_forword(test);
    }

    float partial_training(float gradient_sum)
    {
        uint16 i,j,k;
        float sum=0;
#pragma omp parallel for private(j,k) reduction(+:sum) //collapse(3)
        for(i=0;i<fu;i++)
//#pragma omp for private(j)
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
//#pragma omp for private(k)
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

