class parallel_softmax_error
{
public:
    float avg=0;
    /*
     * For Soft max****
    float** fully_connection_by_softmax(uint16 xd,uint16 yd,float **x)
    {
                float sum=0;
                float **s=new float*[xd]();
                uint16 i,j;
                for(i=0;i<xd;i++)
                {
                    s[i]=new float[yd]();
                    for(j=0;j<yd;j++)
                    sum=exp(x[i][j])+sum;
                }
               for(i=0;i<xd;i++)for(j=0;j<yd;j++)s[i][j]=exp(x[i][j])/sum;
               return s;
    }
    /*
     * Flowing function is a folt function need to deal with it later
     * couse of fault softmax function only cheack whether out put is a spacific
     * class or not.It is not good for mixe kind of image for clasification.
     * As it need to maintain multipal train image for each class.
    float** derivation_softmax(float **s,uint16 x_num_class,uint16 y_num_class){
        float **dsdf=new float*[x_num_class];

        uint16 i,j,o,p;
        for(i=0;i<y_num_class;i++)
            dsdf[i]=new float[y_num_class];
        for(i=0;i<x_num_class;i++)
          for (j=0;j<y_num_class;j++)
              for(o=0;o<x_num_class;o++)
                  for(p=0;p<y_num_class;p++)
                      if((i==o)&&(j==p))
                          dsdf[i][j] =dsdf[i][j]+ s[i][j]*(1-s[i][j]);
                      else
                          dsdf[i][j]=dsdf[i][j]-(s[i][j]*s[o][p]);
        return dsdf;
    }
    float** error_fun_for_softmax(float** t,float** out,uint16 x_cl,uint16 y_cl){
     float total=0.0;
       uint16 i,j;
     float **e=new float*[x_cl]();
     for (i=0;i++;i<x_cl)
     {
         e[i]=new float[y_cl]();
         for(j=0;j<x_cl;j++)
            {
             e[i][j]=-t[i][j]*(log(out[i][j]));
            }
     }
     return e;
    }

    uint16** test_class_for_softmax(float** out,uint16 x_cl,uint16 y_cl,uint16 num_class){
     float total=0.0;
     avg=0.0;
       uint16 i,j,k;
     uint16 **t=new uint16*[x_cl]();
     float temp=0.0;
     uint16 temp_cl=0;
     for (i=0;i++;i<x_cl)
     {
         t[i]=new uint16[y_cl]();
         for(j=0;j<x_cl;j++)
            {
             for(k=1;k<=num_class;k++)
             if(temp>-num_class*(log(out[i][j])))
             {
                    temp=-temp_cl*(log(out[i][j]));
                    temp_cl=k;
             }
             t[i][j]=temp_cl;
             avg=avg+temp/(x_cl+y_cl);
            }
     }
     return t;
    }*/

/*
 *
 * For SVM  *
 * */


/* I will see it later
    float** error_svm(float **t,float **out,uint16 x_cl,uint16 y_cl){
      float total=0.0;uint16 i,j,o,p;
        float **e=new float*[x_cl]();
        for(i=0;i<x_cl;i++)e[i]=new float[y_cl];
        for (i=0;i<x_cl;i++)
            for(j=0;j<y_cl;j++)
                {
                for (o=0;o<x_cl;o++)
                    for(p=0;p<y_cl;p++)

                {
                    if((i==o)&&(j==p))
                    continue;
                    total =total+ std::max(0.0,(out[o][p]-t[i][j]+100.0));
                }
            e[i][j]=total;
        }
        return e;

    }

    float** dirivation_error_svm(float** t,float** out,uint16 x_cl,uint16 y_cl){
        float **dedout=new float*[x_cl]();
        float total = 0.0;
        float **e=error_svm(t,out,x_cl,y_cl);
        uint i,j,o,p;
        for(i=0;i<x_cl;i++)dedout[i]=new float[y_cl]();
        for (i=0;i<x_cl;i++)
         for (j=0;j<y_cl;j++)
         {
             for (o=0;o<x_cl;o++)
              for (p=0;p<y_cl;p++)
             if((i==o)&&(j==p))
              continue;
             if(e[o][p]>0)
              dedout[o][p]=(-out[i][j]*e[o][p]);
         }

        return dedout;

    }*/


    // ////////// Mean scire error function //////////

    // Derivation of error with respect to out put


    float **mean_squre_error(float **t,float **out,uint16 x_cl,uint16 y_cl)
    {
        float **e=new float*[x_cl]();
        uint16 i,j;
#pragma omp for private(j)
        for( i=0;i<x_cl;i++)
        {
            e[i]=new float[y_cl]();
            for( j=0;j<y_cl;j++)
            e[i][j]=1/2*(t[i][j]-out[i][j])*(t[i][j]-out[i][j]);
        }
        return e;

    }

    float **mean_test_squre_error(float **out,uint16 x_cl,uint16 y_cl,uint16 no_class)
    {
        float **r=new float*[x_cl]();
        float multi=x_cl*y_cl;
        uint16 i,j,k,temp_class=0;
        float e_r=9999999,tm_err=0.0;
#pragma omp for private(j,k)
        for( i=0;i<x_cl;i++)
        {
            r[i]=new float[y_cl]();
#pragma omp for private(k)
            for( j=0;j<y_cl;j++)
            {
#pragma omp for
                for( k=1;k<=no_class;k++)
                {
                    tm_err=1/2*(k-out[i][j])*(k-out[i][j]);
                    if(tm_err<e_r)
                    {
                        e_r=tm_err;
                        temp_class=k;
                    }
                }
                r[i][j]=temp_class;
                avg=avg +e_r/multi;
            }

        }
        return r;

    }

    float dirivation_error(float **t,float **out,uint16 x_cl,uint16 y_cl)
    {
        float dedout=0.0,multi=x_cl*y_cl;
        uint16 i,j;
//        cout<<x_cl<<"\t"<<y_cl;
#pragma omp for private(j)
        for( i=0;i<x_cl;i++)
        {
            //dedout[i]=new float[y_cl]();
            for( j=0;j<y_cl;j++)
            dedout=dedout+2*(out[i][j]-t[i][j]);
        }
        //cout<<(dedout/multi);
        return (dedout/multi);

    }

};

