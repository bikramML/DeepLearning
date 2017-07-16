
class parallel_image_matrix
{
public:
    uint32 rows=0,colums=0;
    float** p;
    image_matrix(float **m,uint32 r,uint32 c)
    {
        p=m;rows=r;colums=c;
    }
    image_matrix(uint32 r,uint32 c)
    {
        rows=r;colums=c;
    }
    float** matrix_scale_up(float scaler)
    {
        uint16 i,j;
        float** r=new float*[rows];
#pragma omp for private(j)
        for(i=0;i<rows;i++)
        {
            r[i]=new float[colums];
            for(j=0;j<colums;j++)
            {
                r[i][j]=p[i][j]*scaler;
            }
        }
        return r;
    }

    float** operator+(const float& s)
    {
        uint16 i,j;
        float** r=new float*[rows];
#pragma omp for private(j)
        for(i=0;i<rows;i++)
        {
            r[i]=new float[colums];
            for(j=0;j<colums;j++)
            {
                r[i][j]=p[i][j]+s;
            }
        }
        return r;
    }
    image_matrix operator+(const image_matrix& m)
    {
        uint16 i,j;

        if((m.rows!=rows)||(m.colums!=colums))
        {
            cout<<"Matrix size miss mach";
            exit(0);
            //return this*;
        }
        image_matrix r(rows,colums);
         r.p=new float*[rows];
#pragma omp for private(j)
        for(i=0;i<rows;i++)
        {
            r.p[i]=new float[colums];
            for(j=0;j<colums;j++)
            {
                r.p[i][j]=m.p[i][j]+p[i][j];
            }
        }
        return r;
    }
    void operator=(const image_matrix& m)
    {
        this->colums=m.colums;
        this->rows=m.colums;
        this->p=m.p;
    }
    void matrix_size_incr(uint16 t_x,uint16 t_y)
    {
        uint16 r_x=t_x/rows,r_y=t_y/colums,i,j,k,l;
        float** r=new float*[t_x];
#pragma omp for
        for(i=0;i<t_x;i++)r[i]=new float[t_y]();
#pragma omp for private(l,i,j)
        for(k=0;k<rows;k++)
#pragma omp for private(i,j)
            for(l=0;l<colums;l++)
#pragma omp for private(j)
                for(i=0;i<r_x;i++)
                    for(j=0;j<r_y;j++)
                    {
                        r[k+i][l+j]=p[k][l];
                    }
        delete[] p;
        rows=t_x;colums=t_y;
        p=r;
        //return r;
    }

};

