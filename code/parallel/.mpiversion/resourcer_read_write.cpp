
//#define TRAIN_LIKE ""
//#define x_to_pixel ()
//using namespace std;
class read_write{

private:

    static ifstream train_fp;                 // handle for input image file
    static ifstream grid_fp;                // handle for output image file
    string file_full_path_3;
    string file_full_path_m;
    string file_full_path_a;
    string file_full_path_p;
    string file_train_path;
    string file_grid_path;



public:
    float xmax,ymax;
    uint16 **train_array;
    uint16 ***image_3;
    uint16 ***image_M;
    uint16 **image_P;
    uint16 ***image_SWIR;
    static uint32 train_x,train_y;
    uint32 image_3_x,image_3_y,image_3_z;
    uint32 image_m_x,image_m_y,image_m_z;
    uint32 image_p_x,image_p_y;
    uint32 image_swir_x,image_swir_y,image_swir_z;
    static uint32 object_count;
    unsigned char *im_3_array;// handle for input image file
    unsigned char *im_m_array;
    unsigned char *im_p_array;
    unsigned char *im_swir_array;
        //TIFF *out_filep;                // handle for output image file

private:

    uint16** read_init_image_str_to_array(uint32 x,uint32 y,bool b)
    {
        uint16** p=0;
        p=new uint16*[x];
        if(!b)
        {
        for(int i=0;i<x;i++)
        {
            p[i]=new uint16[y]();
        }
        return p;
        }
        else
        {
            for(int i=0;i<x;i++)
            {
               p[i]=new uint16[y]();
                for(int j=0;j<y;j++)
                {
                    p[i][j]=(uint16)im_p_array[i*y+j];

                }
            }

            return p;
        }
    }

    uint16*** read_init_image_str_to_array(uint32 x,uint32 y,uint16 z,char fl)
    {
        uint16*** p=0;
        p=new uint16**[x];
        for(int i=0;i<x;i++)
        {
            p[i]=new uint16*[y];
            for(int j=0;j<y;j++)
            {

                //for(int k=0;k<z;k++)
                if(z==3)
                {
                    p[i][j]=new uint16[3];
                    p[i][j][0]=uint16((im_3_array[i*y+j])&0x1f);
                    p[i][j][1]=uint16((im_3_array[i*y+j])&0x3e0);
                    p[i][j][2]=uint16((im_3_array[i*y+j])&0x7c00);
                }

                if(z==8)
                {
                    switch(fl)
                    {

                    case 'M':
                        p[i][j]=new uint16[8];
                        p[i][j][0]=uint16((im_m_array[i*y+j])&0x3);
                        p[i][j][1]=uint16((im_m_array[i*y+j])&0xc);
                        p[i][j][2]=uint16((im_m_array[i*y+j])&0x30);
                        p[i][j][3]=uint16((im_m_array[i*y+j])&0xc0);
                        p[i][j][4]=uint16((im_m_array[i*y+j])&0x300);
                        p[i][j][5]=uint16((im_m_array[i*y+j])&0xc00);
                        p[i][j][6]=uint16((im_m_array[i*y+j])&0x3000);
                        p[i][j][7]=uint16((im_m_array[i*y+j])&0xc00);
                        break;
                    case 'A':
                        p[i][j]=new uint16[8];
                        p[i][j][0]=uint16((im_swir_array[i*y+j])&0x3);
                        p[i][j][1]=uint16((im_swir_array[i*y+j])&0xc);
                        p[i][j][2]=uint16((im_swir_array[i*y+j])&0x30);
                        p[i][j][3]=uint16((im_swir_array[i*y+j])&0xc0);
                        p[i][j][4]=uint16((im_swir_array[i*y+j])&0x300);
                        p[i][j][5]=uint16((im_swir_array[i*y+j])&0xc00);
                        p[i][j][6]=uint16((im_swir_array[i*y+j])&0x3000);
                        p[i][j][7]=uint16((im_swir_array[i*y+j])&0xc00);
                        break;
                    }

                }
            }
        }
        return p;
    }

    void initilize_xmax_ymax(string file_name)
    {

        //if((i_no%10)==2)
             //       {
        string grid_line="";
         int ii=0;
         while(std::getline(grid_fp,grid_line))
         {
           if(ii==0)
           {
             ii++;
             continue;
            }
            if((grid_line.substr(0,8)).compare(file_name)==0)
            {
             int grid_line_size=grid_line.size();
             int x_position;
             x_position=grid_line.substr(9,grid_line_size).find(",");
             xmax=stof(grid_line.substr(9,x_position));
             ymax=std::stof(grid_line.substr(10+x_position,grid_line_size));
             // ii=0;
             break;
             }
             else continue;
             }
         grid_fp.clear();
         grid_fp.seekg(0, ios::beg);
    }

    void load_train(string file_name)
    {
       // const char *tr_level_path=train_level.c_str();
       // const char *grid_file_path=grid_file.c_str();


        std::string line;

        int i_no=0;
        //string image_name="";
        //string grid_line="";
       // string sub_cell="";
        int x_position=0;
        int train_class=0;
        float dx=x_scale_to_pixel();
        float dy=y_scale_to_pixel();

        train_array=read_init_image_str_to_array(train_x,train_y,false);
    while(std::getline(train_fp,line))
        {
            //cout<<line<<"\n";
            if(i_no==0){i_no++;continue;}
           // if(i_no==6) break;
            i_no++;
            if(train_class==10){train_class=0;}
            train_class++;
            int ii=0;

            if(line.substr(0,8).compare(file_name)!=0)
                continue;
           line.erase(0,24);
           stringstream  lineStream(line);
           string        cell;


            while(std::getline(lineStream,cell,','))
            {

                if((!cell.compare("EMPTY"))||!cell.compare(" EMPTY")) continue;
                static int count=0;
                while(true)
                {
                    int rep=0;
                   if(cell.find("(")!=(std::string::npos))
                    {
                       rep=cell.find("(");
                       cell.erase(rep,1);
                    }
                   else if(cell.find(")")!=(std::string::npos))
                    {
                       rep=cell.find(")");
                       cell.erase(rep,1);
                    }
                   else break;
                }
                //cout<<cell;

                string fr="",sc="";
                int p=0;
               // int co=0;
                //cout<<"cell="<<cell;
                bool pt;
                for(int ci=0;ci<cell.size();ci++)
                {
                    if(cell[ci]==' '){
                        if(cell[ci+1]=='0')
                            pt=true;
                        else if(cell[ci+1]=='-')
                            pt=false;
                        continue;
                    }

                    if(!pt&&cell[ci]!=' ')
                        sc=sc+cell[ci];
                    else if(pt&&(cell[ci]!=' '))
                        fr=fr+cell[ci];

                }
               // di=0;

                if(fr=="")fr="0.0";
                if(sc=="")sc="0.0";
                //cout<<"Hello"<<"\n"<<sc;
                try{
                float x=stof(fr);
                float y=stof(sc);

                int x1=(x*dx),y1=(y*dy);
                //cout<<"\n"<<x1<<"\t"<<y1<<"\n";
                //cout<<x1;
                train_array[x1][y1]=train_class;
                //throw x1;
                }
                catch(int e)
                {
                    cout<<"\nerror\t"<<e;
                }

            }
        }
    train_fp.clear();
    train_fp.seekg(0, ios::beg);
    }

float x_scale_to_pixel()
{
    float w1=(train_x*train_x)/(train_x+1);
    float dx=w1/xmax;
   // cout<<"Hello"<<dx<<"\n";
    return dx;
}
float y_scale_to_pixel()
{
    float h1=(train_y*train_y)/(train_y+1);
    float dy=h1/ymax;
    return dy;
}
float x_to_poly()
{
    float w1=(train_x*train_x)/(train_x+1);
    float dx=xmax/w1;
    return dx;
}
float y_to_poly()
{
    float h1=(train_y*train_y)/(train_y+1);
    float dy=ymax/h1;
    return dy;
}

void read_tiff_imag_to_str_array(string file_path,unsigned char* in_image,uint32 rows,uint32 columns)
{
    uint16 BitsPerSample;           // normally 8 for grayscale image
    uint16 SamplesPerPixel;         // normally 1 for grayscale image
    uint16 PhotoMetric;             // normally 1 for grayscale image

   // unsigned char* in_image;
    TIFF *in_filep;                 // handle for input image file
    //TIFF *out_filep;                // handle for output image file
    int check_io;                   // status of I/O operation
  //  cout<<file_path;
    // Open input image file
    //file_path="../data/6040_2_2_P.tif";
    in_filep = TIFFOpen(file_path.c_str(), "r");

    if (in_filep == NULL)
    cout<<"Could not open input file!";

    TIFFGetField(in_filep, TIFFTAG_IMAGELENGTH, &rows);
    TIFFGetField(in_filep, TIFFTAG_IMAGEWIDTH, &columns);
    TIFFGetField(in_filep, TIFFTAG_BITSPERSAMPLE, &BitsPerSample);
    TIFFGetField(in_filep, TIFFTAG_SAMPLESPERPIXEL, &SamplesPerPixel);
    TIFFGetField(in_filep, TIFFTAG_PHOTOMETRIC, &PhotoMetric);

    // Allocate memory to hold image arrays
    in_image = (unsigned char*) _TIFFmalloc(rows*columns);

    if (in_image == NULL)
    cout<<"Could not allocate memory!";
    //cout<<rows;
    // Read image pixel values from file, row by row
    cout<<"hello";

    for (int r = 0; r < rows; r++)
    {
        check_io = TIFFReadScanline(in_filep, &in_image[r*columns], r, 1);
        if (check_io != 1)
        cout<<"Could not read image from file!";
    }
    /*
*/
    // Deallocate the image memory, close file streams, and exit
    _TIFFfree(in_image);
    TIFFClose(in_filep);
}

void write_image_array_to_str_array(char fl)
{
    uint32 x=0,y=0,z=0;
    uint16*** p=nullptr;
    unsigned char* cp=nullptr;
    switch(fl)
    {
    case 'P':
        x=image_p_x;y=image_p_y;
        break;
    case 'A':
        x=image_swir_x;y=image_swir_y;z=8;
        p=image_SWIR;
        break;
    case 'M':
        x=image_m_x;y=image_m_y;z=8;
        p=image_M;
        break;
    case '3':
        x=image_3_x;y=image_3_y;z=3;
        p=image_3;
        break;
    }



    if(z==0)
    {
      for(int i=0;i<x;i++)
        {
           // *p[i]=new uint16[y]();
          //unsigned char a[16];
            for(int j=0;j<y;j++)
            {
                im_p_array[i*y+j]=(unsigned char)image_P[i][j];
                //p[i][j]=new uint16((uint16)im_p_array[i*y+j]);

            }
        }
     }
    else
    {
        for(int i=0;i<x;i++)
          {
             // *p[i]=new uint16[y]();
              int a;
              for(int j=0;j<y;j++)
              {
                  for(int k=1;k<=z;k++)
                  {
                  //itoa(image_SWIR[i][j][8-k],a,10);
                    a=p[i][j][8-k];
                    if(k!=8)
                    a=a<<2;
                  }
                  cp[i*y+j]=(unsigned char)a;
              }
          }
    }
}

void write_imag_str_array_to_tiff(string out_path,char fl)
{

double red_mean;                // mean red value of fed image

    uint32 r;                       // row index
    uint32 c;                       // column index
    uint32 rows;                    // number of rows in image
    uint32 columns;                 // number of columns in image
    uint16 BitsPerSample;           // normally 8 for grayscale image
    uint16 SamplesPerPixel;         // normally 1 for grayscale image
    uint16 PhotoMetric;             // normally 1 for grayscale image
    //unsigned char *in_image;        // pointer for input image array
    unsigned char *out_image;       // pointer for output image array
    TIFF *in_filep;                 // handle for input image file
    TIFF *out_filep;                // handle for output image file
    int check_io;                   // status of I/O operation



    // Open output image file
    out_filep = TIFFOpen (out_path.c_str(), "w");
    if (out_filep == NULL)
    cout<<"Could not open output file!";

    // Determine the size of the input image
    TIFFGetField(in_filep, TIFFTAG_IMAGELENGTH, &rows);
    TIFFGetField(in_filep, TIFFTAG_IMAGEWIDTH, &columns);
    TIFFGetField(in_filep, TIFFTAG_BITSPERSAMPLE, &BitsPerSample);
    TIFFGetField(in_filep, TIFFTAG_SAMPLESPERPIXEL, &SamplesPerPixel);
    TIFFGetField(in_filep, TIFFTAG_PHOTOMETRIC, &PhotoMetric);
    //TIFFGetField(in_filep, TIFFTAG_PHOTOMETRIC, &PlanarConfig);
    //The following statements are helpful in debugging

    // Specify TIFF header fields for output image
    TIFFSetField(out_filep, TIFFTAG_IMAGELENGTH, rows);
    TIFFSetField(out_filep, TIFFTAG_IMAGEWIDTH, columns);
    TIFFSetField(out_filep, TIFFTAG_BITSPERSAMPLE, BitsPerSample);
    TIFFSetField(out_filep, TIFFTAG_SAMPLESPERPIXEL, SamplesPerPixel);
    TIFFSetField(out_filep, TIFFTAG_PLANARCONFIG, 1);
    TIFFSetField(out_filep, TIFFTAG_PHOTOMETRIC, PhotoMetric);
    switch(fl)
    {
    case 'P':
        out_image=im_p_array;
        rows=image_p_x;columns=image_p_y;
        break;
    case 'A':
        out_image=im_swir_array;
        rows=image_swir_x;columns=image_swir_y;
        break;
    case 'M':
        out_image=im_m_array;
        rows=image_m_x;columns=image_m_y;
        break;
    case '3':
        out_image=im_3_array;
        rows=image_3_x;columns=image_3_y;
        break;
    }

    out_image = (unsigned char *) _TIFFmalloc(rows*columns);
    if (out_image == NULL)
    cout<<"Could not allocate memory!";


    // Write new image to file, row by row
    for (r = 0; r < rows; r++)
    {
        check_io = TIFFWriteScanline(out_filep, &out_image[r*columns], r, 1);
        if (check_io != 1)
        cout<<"Could not read image from file!";
    }


    TIFFClose(out_filep);
    exit (0);
}
public:
    read_write(string file_name)
    {

        string folder="../data/";
        string train_file="train_wkt_v4.csv";
        //";

        string grid_file="grid_sizes.csv";
        file_full_path_3=folder+file_name+".tif";
        file_full_path_m=folder+file_name+"_M.tif";
        file_full_path_a=folder+file_name+"_A.tif";
        file_full_path_p=folder+file_name+"_P.tif";
        file_train_path=folder+train_file;
        file_grid_path=folder+grid_file;

        object_count++;
        this->initilize_xmax_ymax(file_name);
        this->load_train(file_name);
        read_tiff_imag_to_str_array(file_full_path_p,im_p_array,image_p_x,image_p_y);
       // this->read_init_image_str_to_array()

    }

    ~read_write()
    {
        object_count--;
        /*
        delete[] im_3_array;// handle for input image file
        delete[] im_m_array;
        delete[] im_p_array;
        delete[] im_swir_array;
        delete[] train_array;
        delete[] image_3;
        delete[] image_M;
        delete[] image_P;
        delete[] image_SWIR;
        */
        if(object_count==0)
        {
           // _TIFFfree(train_fp);
            (train_fp).close();
           // _TIFFfree(grid_fp);
            (grid_fp).close();
            //TIFFClose(train_fp);
            //TIFFClose(grid_fp);
        }
    }

};
string folder="../data/";
string train_file="train_wkt_v4.csv";
//";

string grid_file="grid_sizes.csv";

ifstream read_write::train_fp((folder+train_file).c_str());                 // handle for input image file
ifstream read_write::grid_fp((folder+grid_file).c_str());                // handle for output image file
//float read_write::xmax=0,read_write::ymax=0;
uint32 read_write::train_x=136,read_write::train_y=134;

 uint32 read_write::object_count=0;


int main()
{
    read_write rw("6040_2_2");
    //for(int i=0;i<)
    cout<<rw.xmax<<"\n"<<rw.ymax<<"\n";
    /*
    for(int i=0;i<136;i++)
    {
        for(int j=0;j<134;j++)
            if(rw.train_array[i][j]!=0)
            cout<<"i="<<i<<"j"<<j<<"class="<<rw.train_array[i][j]<<"\n";

    }*/
    return 0;
}
