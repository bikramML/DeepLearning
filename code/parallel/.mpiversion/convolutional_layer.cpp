//idef Gaussian(w,x,y,z,d):
// return 1/(4*(math.pi)*(math.pi)*d*d*d*d)*(math.exp(-(w*w+x*x+y*y+z*z)/(2*d*d)))

#define LR 0.5

//#define  WEIGHT_INIT(weight)\


class parallel_convolutional_layer
{
private:

    uint32 in_rows,in_colums;
    uint32 weight_size;

    //uint16_t in_rows,in_colums;

    float weight_int()
    {
        return (rand()%weight_size) * sqrt(2.0/weight_size);
    }
    float weight_sum(float *s,uint16 x,uint16 y)
    {
        uint16 i,j;float temp=0;
#pragma omp for private(j)
        for(i=0;i<x;i++)
            for(j=0;j<y;j++)
                temp+=s[i*y+j];
        return temp;
    }

    void front_part_operation_for_forword()
    {
        uint16 i,j,k;
#pragma omp for private(j,k)
        for(i=0;i<fu;i++)
        {
            float sum=weight_sum(weight[i],in_rows,in_colums);
#pragma omp for private(k)
        for(j=0;j<in_rows;j++)
        {
#pragma omp for private(k)
        for( k=0;k<in_colums;k++)
        {
             output[i][j][k]=(output[i][j][k]+weight[i][in_colums*j+k]*input[j][k])/sum;
             gradient[i][in_colums*j+k]=gradient[i][in_colums*j+k]+input[j][k];
        }
        }
        }
    }



    void back_part_operation_for_forword()
    {
        uint16 i,j,k;
#pragma omp for private(j,k)
        for(i=0;i<fu;i++)
#pragma omp for private(k)
        for(j=0;j<in_rows;j++)
         for( k=0;k<in_colums;k++)
         {
             output[i][j][k]=(1/1+(exp(-output[i][j][k])));
             
             gradient[i][in_colums*j+k]=gradient[i][in_colums*j+k]*(output[i][j][k]*(1-output[i][j][k]));

         }
    }
public:
    float*** output=nullptr;
    float **input;

    uint16 fu=10;

    float** weight;
    float** gradient;
    convolutional_layer(uint16 w_rows,uint16 w_colums)
    {

        weight_size=(w_rows*w_colums+1);
        in_rows=w_rows;in_colums=w_colums;
  //
    }
    void reset_output()
    {
        uint16 j,k,m;
#pragma omp for private(j,k,m)
        for(j=0;j<fu;j++)
#pragma omp for private(m)
        for(k=0;k<in_rows;k++)
#pragma omp for
            for(m=0;m<in_colums;m++)
                output[j][k][m] = weight[j][weight_size-1];
    }

    void init_weight()
    {
        //weight_size=125;
               uint16 j,k;

               weight=new float*[fu];
               gradient=new float*[fu];
               output=new float**[fu];
               //cout<<weight_size;

#pragma omp for private(j,k)
               for(j=0;j<fu;j++)
               {

               //    break;
                   weight[j]=new float[weight_size];
                   gradient[j]=new float[weight_size]();
                   output[j]=new float*[in_rows]();
                   //cout<<"Hello"<<in_rows;
#pragma omp for
                   for( k=0;k<in_rows;k++)
                   {
                         output[j][k]=new float[in_colums]();

                   }
#pragma omp barrier
#pragma omp for
                    for( k=0;k<weight_size;k++)
                    {
                          weight[j][k]=weight_int();

                    }
#pragma omp barrier
#pragma omp for private(m)
                    for(k=0;k<in_rows;k++)
#pragma omp for
                        for(uint16 m=0;m<in_colums;m++)
                    {
                       // Add baise
                        gradient[j][in_colums*k+m]=1;
                    }

               }


    }

    void frowork_oparation()
    {
        //input;
          reset_output();
          front_part_operation_for_forword();
          back_part_operation_for_forword();
    }

    float partial_training(float gradient_sum)
    {
        uint16 i,j,k;
        float sum=0;
#pragma omp for private(j,k)
        for(i=0;i<fu;i++)
#pragma omp for private(k)
        for(j=0;j<in_rows;j++)
#pragma omp for
         for( k=0;k<in_colums;k++)
            {
             gradient[i][in_colums*j+k]=gradient[i][in_colums*j+k]*gradient_sum;
             sum =sum+gradient[i][in_colums*j+k];
         }
        return sum;
    }


    float back_propagation(float gradient_sum)
    {
        uint16 i,j,k;
        float sum=0;
#pragma omp for private(j,k)
        for(i=0;i<fu;i++)
#pragma omp for private(k)
        for(j=0;j<in_rows;j++)
#pragma omp for
         for( k=0;k<in_colums;k++)
            {
             //cout<<gradient[i][in_colums*j+k]<<"\n";
             gradient[i][in_colums*j+k]=gradient[i][in_colums*j+k]*gradient_sum;
             weight[i][j*in_colums+k]=weight[i][j*in_colums+k]-LR*gradient[i][in_colums*j+k];
             sum =sum+gradient[i][in_colums*j+k];
             gradient[i][in_colums*j+k]=0;
         }
        return sum;
    }


};

