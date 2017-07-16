//idef Gaussian(w,x,y,z,d):
// return 1/(4*(math.pi)*(math.pi)*d*d*d*d)*(math.exp(-(w*w+x*x+y*y+z*z)/(2*d*d)))
#define NUM_LAYER 10
#define FUTURES_PER_LAYER 10
#define INPUTSIZE_HIGHT 128
#define INPUT_WIDE 128
#define WEIGHT_SIZE INPUTSIZE_HIGHT*INPUT_WIDE+1//1 for bais
#define LR 0.15
#include<omp.h>

//#define  WEIGHT_INIT(weight)\


class convolutional_layer_parallel
{
private:
    float** weight=nullptr;
    float** gradient=nullptr;
    uint16_t fu,weight_size;
    uint16_t in_rows,in_colums;
public:
    float** output=nullptr;
    convolutional_layer(uint16 w_rows,uint16 w_colums,uint16 out_rows,uint16 out_colums)
    {

    }

    void init_weight()
    {
   //            uint16 j,k;
               //weight=new float[fu][weight_size]();
               weight=new float*[fu];
               gradient=new float*[fu];
               output=new float*[fu];
#pragma omp parallel for default(none) shared(weight,gradient,output,weight_size)
               for(uint16 j=0;j<fu;j++)
               {
                   weight[j]=new float[weight_size]();
                   gradient[j]=new float[weight_size]();
                   output[j]=new float[weight_size-1]();
                    for(uint16 k=0;k<weight_size;k++)
                    {
                          weight[j][k]=weight_int();

                    }

                    for(k=0;k<weight_size-1;k++)
                    {
                        output[j][k] = weight[j][weight_size-1];// Add baise
                    }
               }

    }
    float weight_int()
    {
        return (rand()%weight_size) * sqrt(2.0/weight_size);
    }

    void front_part_operation_for_forword(float** input,float fact)
    {
        //uint16 i,j,k;

       // weight=new float[fu][weight_size]();
#pragma omp parallel for default(none) shared(fu,in_rows,in_colums,weight,gradient,output,weight_size)
        for(uint16 i=0;i<fu;i++)
        for(uint16 j=0;j<in_rows;j++)
        {
         for(uint16 k=0;k<in_colums;k++)
         {
             output[i][in_colums*j+k]=output[i][in_colums*j+k]+fact*weight[i][in_colums*j+k]*input[j][k];
             gradient[i][in_colums*j+k]=gradient[i][in_colums*j+k]+input[j][k];

         }

         gradient[i][in_colums*j+k]=1;
        }
    }

    void back_part_operation_for_forword()
    {
     //   uint16 i,j,k;
#pragma omp parallel for default(none) shared(fu,in_rows,in_colums,gradient,output)
        for(uint16 i=0;i<fu;i++)
        for(uint16 j=0;j<in_rows;j++)
         for(uint16 k=0;k<in_colums;k++)
         {
             output[i][in_colums*j+k]=(1/1+(exp(-output[i][in_colums*j+k])));
             gradient[i][in_colums*j+k]=gradient[i][in_colums*j+k]*(output[i][in_colums*j+k]*(1-output[i][in_colums*j+k]));
         }
    }
    float back_propagation(float gradient_sum)
    {
        uint16 i,j,k;
        float sum=0;
        #pragma omp parallel for default(none) shared(fu,in_rows,in_colums,weight,gradient,output,weight_size,sum)
        for(uint16 i=0;i<fu;i++)
        for(uint16 j=0;j<in_rows;j++)
         for(uint16 k=0;k<in_colums;k++)
            {
             gradient[i][in_colums*j+k]=gradient[i][in_colums*j+k]*gradient_sum;
             weight[i][j*in_colums+k]=weight[i][j*in_colums+k]-LR*gradient[i][in_colums*j+k];
             sum =sum+gradient[i][in_colums*j+k];
             gradient[i][in_colums*j+k]=0;
         }
        return sum;
    }
    /*void change_weight()
    {
        uint16 i=0,j=0,k=0;
      //  float sum=0;
        for(i=0;i<fu;i++)
        for(j=0;j<in_rows;j++)
         for( k=0;k<in_colums;k++)
         {


         }
    }*/

};
