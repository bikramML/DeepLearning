
//idef Gaussian(w,x,y,z,d):
// return 1/(4*(math.pi)*(math.pi)*d*d*d*d)*(math.exp(-(w*w+x*x+y*y+z*z)/(2*d*d)))

#define LR 0.5

//#define  WEIGHT_INIT(weight)\


class convolutional_net_layer
{
private:

    float***** input_gradient;
    //float**** output_gradient;
    bool check_frist=true;
    uint32 in_rows,in_colums;
    uint32 w_row,w_colum,weight_size;
    uint16 slot=1;
    float weight_sum(float *s,uint16 x,uint16 y)
    {
        uint16 i,j;float temp=0;
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

    void front_part_operation_for_forword(bool test)
    {
        uint16 i,j,k,m,n;
        /**/
        if(check_frist)
        {
        input_gradient=new float****[fu];
        //cout<<fu<<w_row<<w_colum<<in_rows<<in_colums;

                for(i=0;i<fu;i++)
                {

                    input_gradient[i]=new float***[w_row];

                for(j=0;j<w_row;j++)
                {
                    input_gradient[i][j]=new float**[w_colum];
                    for(k=0;k<w_colum;k++)
                    {
                        input_gradient[i][j][k]=new float*[in_rows-w_row];
                        for(m=0;m<in_rows-w_row;m++)
                        {
                            input_gradient[i][j][k][m]=new float[in_colums-w_colum];

                        }
                    }
                }
             }
                check_frist=false;
        }

        for(i=0;i<fu;i++)
        {
        float weight_total=weight_sum(weight[0],w_row,w_colum);
        for(j=0;j<in_rows-w_row;j+=slot)
        for( k=0;k<in_colums-w_colum;k+=slot)
        for( m=0;m<w_row;m++)
        for( n=0;n<w_colum;n++)
        {
             output[i][j+m][k+n]=output[i][j+m][k+n]+(weight[i][w_colum*m+n]*input[j+m][k+n])/weight_total;
             //cout<<output[i][j+m][k+n];
             if(!test)
             input_gradient[i][m][n][j][k]=input[j+m][k+n];


        }


        }
    }



    void back_part_operation_for_forword(bool test)
    {

        uint16 i,j,k,m,n;

        for(i=0;i<fu;i++)
        {

        for(j=0;j<in_rows-w_row;j+=slot)
        for( k=0;k<in_colums-w_colum;k+=slot)
         {
            //cout<<w_rows;
        for( m=0;m<w_row;m++)
        for( n=0;n<w_colum;n++)
        {


             output[i][j+m][k+n]=(1/1+(exp(-output[i][j+m][k+n])));
           // if(!test)
        //     gradient[i][w_colum*m+n]=gradient[i][w_colum*m+n]+input_gradient[i][m][n][j][k]*output[i][j+m][k+n];
        }

        }
        }
        //delete[] input_gradient;
    }
public:
    float*** output=nullptr;
    float **input;

    float** weight;
    float** gradient;
    uint16 fu=10;
   // uint32 weight_size;
    convolutional_net_layer(uint16 image_rows,uint16 image_colums,uint16 w_rows,uint16 w_colums)
    {

        weight_size=(w_rows*w_colums+1);
        w_row=w_rows;w_colum=w_colums;
        in_rows=image_rows;in_colums=image_colums;
  //
    }
    void reset_output()
    {
        uint32 j,k,m;
        for(j=0;j<fu;j++)
            for(k=0;k<in_rows;k++)
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


               for(j=0;j<fu;j++)
               {

               //    break;
                   weight[j]=new float[weight_size];
                   gradient[j]=new float[weight_size]();
                   output[j]=new float*[in_rows]();
                   //cout<<"Hello"<<in_rows;
                   for( k=0;k<in_rows;k++)
                   {
                         output[j][k]=new float[in_colums]();

                   }
                    for( k=0;k<weight_size;k++)
                    {
                          weight[j][k]=weight_int();

                    }

                    for(k=0;k<in_rows;k++)
                        for(uint16 m=0;m<in_colums;m++)
                    {
                        // Add baise
                         gradient[i][weight_size-1]=1;
                    }

               }


    }

    void frowork_oparation(bool test)
    {
        //input;

          reset_output();
          front_part_operation_for_forword(test);
          back_part_operation_for_forword(test);
    }

    float partial_training(float gradient_sum)
    {
        uint16 i,j,k;
        float sum=0;
        for(i=0;i<fu;i++)
        for(j=0;j<w_row;j++)
         for( k=0;k<w_colum;k++)
            {
             gradient[i][w_colum*j+k]=gradient[i][w_colum*j+k]*gradient_sum;
             sum =sum+gradient[i][w_colum*j+k];
         }
        return sum;
    }


    float back_propagation(float gradient_sum)
    {

        uint16 i,j,k,m,n;
        float sum=0;

        for(i=0;i<fu;i++)
        {

        for(j=0;j<in_rows-w_row;j+=slot)
        for( k=0;k<in_colums-w_colum;k+=slot)
         {
            //cout<<w_rows;
        for( m=0;m<w_row;m++)
        for( n=0;n<w_colum;n++)
        {


            // output[i][j+m][k+n]=(1/1+(exp(-output[i][j+m][k+n])));
            //if(!test)
             gradient[i][w_colum*m+n]=gradient[i][w_colum*m+n]+input_gradient[i][m][n][j][k]*output[i][j+m][k+n];
        }

        }
        }






        for(i=0;i<fu;i++)
        for(j=0;j<w_row;j++)
         for( k=0;k<w_colum;k++)
            {
          //for(m=0;m<)
             gradient[i][w_colum*j+k]=gradient[i][w_colum*j+k]*gradient_sum;
             weight[i][j*w_colum+k]=weight[i][j*w_colum+k]-LR*gradient[i][w_colum*j+k];
             sum =sum+gradient[i][w_colum*j+k];
             gradient[i][w_colum*j+k]=0;
         }
        return sum;
    }


};

