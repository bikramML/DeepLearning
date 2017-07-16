class read_write
{

private:

    static ifstream train_fp;                 // handle for input image file
    static ifstream grid_fp;                // handle for output image file
    static string data_folder_path;
/*    string file_full_path_3;
    string file_full_path_m;
    string file_full_path_a;
    string file_full_path_p;*/
   // string file_train_path;
   // string file_grid_path;
    static float A_x_fact,A_y_fact,P_x_fact,P_y_fact,M_x_fact,M_y_fact,RGB_x_fact,RGB_y_fact;
   // uint16 input_size_x;
   // uint16 input_size_y;



public:
    double  xmax,ymax;
    float **train_array;
    float ***image_3=nullptr;
    float ***image_M=nullptr;
    float **image_P=nullptr;
    float ***image_SWIR=nullptr;
    static uint32 train_x,train_y;
    static uint32 train_max_x,train_max_y;
    uint32 image_3_x,image_3_y;
    uint16 image_3_z;
    uint32 image_m_x,image_m_y;
    uint16 image_m_z;
    uint32 image_p_x,image_p_y;
    uint32 image_swir_x,image_swir_y;
    uint16 image_swir_z;


    static uint32 object_count;

   // unsigned char *im_3_array;// handle for input image file
   // unsigned char *im_m_array;
   // unsigned char *im_p_array;
   // unsigned char *im_swir_array;
        //TIFF *out_filep;                // handle for output image file


 
	// Open the TIFF image
private:
    
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

    void load_train(string file_name,uint16 start_x,uint16 start_y)
    {
       // const char *tr_level_path=train_level.c_str();
       // const char *grid_file_path=grid_file.c_str();
       // cout<<"heloo";

        std::string line;

        int i_no=0;
        //string image_name="";
        //string grid_line="";
       // string sub_cell="";
        int x_position=0;
        int train_class=0;
        double  dx=x_scale_to_pixel();
        double  dy=y_scale_to_pixel();
        //cout<<train_x<<" "<<train_y;
        train_array=init_taining_image();

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
                 //cout<<cell<<"\n";

                string fr="",sc="";
                int p=0;

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


                if(fr=="")fr="0.0";
                if(sc=="")sc="0.0";


                try{

              long  double  x=atof(fr.c_str());
              long  double  y=atof(sc.c_str());
              long  double sc_x=x*dx;
              long  double sc_y=y*dy;
                uint32 x1=sc_x,y1=sc_y;


                if(x1>train_x||y1>train_y)
                {
                     continue;
                    cout<<"\n folt value found ";

                }
                else if(!(x1<start_x)&&!(y1<start_y))
                if((x1-start_x)<train_max_x&&(y1-start_y)<train_max_y)
                {
                 //   cout<<x1-start_x<<" "<<y1-start_y<<"\n";
                train_array[x1-start_x][y1-start_y]=train_class;
                }
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

double  x_scale_to_pixel()
{
    float x=train_x;
    float  w1=(x*x)/(x+1);
    double  dx=w1/xmax;
    return dx;
}
double  y_scale_to_pixel()
{
    double  h1=(train_y*train_y)/(train_y+1);
    double  dy=h1/ymax;
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
float** init_taining_image()
{
    float** p=0;
    p=new float*[train_max_x];

    for(int i=0;i<train_max_x;i++)
    {
        p[i]=new float[train_max_y]();
    }
    return p;

}


    void init_image_size(TIFF *image,uint32 *width,uint32 *height)
    {
        TIFFGetField(image, TIFFTAG_IMAGEWIDTH, width);
        TIFFGetField(image, TIFFTAG_IMAGELENGTH, height);
    }
    void init_image_size(TIFF *image,uint32 *width,uint32 *height,uint16 *SamplesPerPixel)
    {
        TIFFGetField(image, TIFFTAG_IMAGEWIDTH, width);
        TIFFGetField(image, TIFFTAG_IMAGELENGTH, height);
        TIFFGetField(image, TIFFTAG_SAMPLESPERPIXEL, SamplesPerPixel);
        //cout<<*SamplesPerPixel;

    }
    float**  from_image_to_memory(TIFF *image,uint32 width,uint32 height,uint16 x_start,uint16 y_start,uint16 x_image_size,uint16 y_image_size)
    {
        uint16 line_size = width;
        float** pixel_image=nullptr;
        uint16 *scanline = (uint16 *)_TIFFmalloc(line_size*sizeof(uint16 *));
        pixel_image=new float*[x_image_size]();
        for (int r1 = 0; r1 < height; r1++)
        {
            if(r1<x_start)
                continue;
            if((r1-x_start)==x_image_size)
                break;
            TIFFReadScanline(image, scanline, r1, 0);
            pixel_image[r1-x_start]=new float[y_image_size]();
            for (int c1 = 0; c1 < (width); c1++)
            {
                if(c1<y_start)
                    continue;
                if((c1-y_start)==y_image_size)
                    break;
                pixel_image[r1-x_start][c1-y_start]=scanline[c1];
            }
        }
    _TIFFfree(scanline);
    return pixel_image;
    }
    float*** from_image_to_memory(TIFF *image,uint32 width,uint32 height,uint16 SamplesPerPixel,uint16 x_start,uint16 y_start,uint16 x_image_size,uint16 y_image_size)
    {
        //cout<<width<<" "<<height<<" "<<SamplesPerPixel;
         float*** pixel_image=nullptr;
        uint16 line_size = SamplesPerPixel*width;
        uint16 *scanline = (uint16 *)_TIFFmalloc(line_size*sizeof(uint16 *));
        pixel_image=new float**[SamplesPerPixel]();
        for(int s=0;s<SamplesPerPixel;s++)
        {
            pixel_image[s]=new float*[x_image_size]();
            for(int i=0;i<x_image_size;i++)
            {
                pixel_image[s][i]=new float[y_image_size]();

            }
        }

        for (int r1 = 0; r1 < height; r1++)
        {
            if(r1<x_start)
                continue;
            if((r1-x_start)==x_image_size)
                break;

            TIFFReadScanline(image, scanline, r1, 0);
            //pixel_image[r1-x_start]=new float*[y_image_size]();
            for (int c1 = 0; c1 < (width); c1++)
            {

                if(c1<y_start)
                    continue;
                if((c1-y_start)==y_image_size)
                    break;
               // pixel_image[r1-x_start][c1-y_start]=new float[SamplesPerPixel]();
                for(uint16 s=0;s<SamplesPerPixel;s++)
                {
                    //cout<<"Heloo";
                    pixel_image[s][r1-x_start][c1-y_start]=scanline[c1*SamplesPerPixel+s];
                   // cout<<pixel_image[r1-x_start][c1-y_start][s];
                }
            }
        }
//        if(image_SWIR==nullptr)cout<<"aad";
      _TIFFfree(scanline);
      return pixel_image;
    }
    void image_to_memory(string path,uint16 image_size_x,uint16 image_size_y,uint16 x_start,uint16 y_start)
    {
        TIFF *image;
        string file_path="";

       // Opening P image ///
        file_path=path+"_P.tif";
        if((image = TIFFOpen(file_path.c_str(), "r")) == NULL){
        cout<<"Could not open incoming P image\n";
        exit(42);
    }

        init_image_size(image,&image_p_x,&image_p_y);
        image_P=from_image_to_memory(image,image_p_x,image_p_y,x_start,y_start,image_size_x*P_x_fact,image_size_y*P_y_fact);
        TIFFClose(image);
        //Opening M file
        file_path=path+"_M.tif";
        if((image = TIFFOpen(file_path.c_str(), "r")) == NULL){
        cout<<"Could not open incoming M image\n";
        exit(42);
        }

          init_image_size(image,&image_m_x,&image_m_y,&image_m_z);
//          cout<<image_m_z;
          image_M=from_image_to_memory(image,image_m_x,image_m_y,image_m_z,x_start,y_start,image_size_x*M_x_fact,image_size_y*M_y_fact);



        //Opening RGB file

        file_path=path+".tif";
        if((image = TIFFOpen(file_path.c_str(), "r")) == NULL){
        cout<<"Could not open incoming RGB image\n";
        exit(42);
    }
        init_image_size(image,&image_3_x,&image_3_y,&image_3_z);
        image_3=from_image_to_memory(image,image_3_x,image_3_y,image_3_z,x_start,y_start,image_size_x*RGB_x_fact,image_size_y*RGB_y_fact);

        TIFFClose(image);

        //Opening A file
        file_path=path+"_A.tif";
        if((image = TIFFOpen(file_path.c_str(), "r")) == NULL){
        cout<<"Could not open incoming A image"<<file_path<<"\n";
        exit(42);
    }
        init_image_size(image,&image_swir_x,&image_swir_y,&image_swir_z);
        //cout<<image_swir_x<<" "<<image_swir_y<<" "<<image_swir_z;
        image_SWIR=from_image_to_memory(image,image_swir_x,image_swir_y,image_swir_z,x_start,y_start,image_size_x*A_x_fact,image_size_y*A_y_fact);
        TIFFClose(image);
    }
    public:

    read_write(string file_name,uint16 image_size_x,uint16 image_size_y,uint16 x_start,uint16 y_start)
    {

        //string folder="../../data/";
        //string train_file="train_wkt_v4.csv";
        //";

        //string grid_file="grid_sizes.csv";
        image_to_memory(data_folder_path+file_name,image_size_x,image_size_y,x_start,y_start);
      //  if(image_SWIR==nullptr)cout<<"aad";
      //  file_train_path=folder+train_file;
      //  file_grid_path=folder+grid_file;
        //cout<<"hello";
  //      object_count++;
        this->initilize_xmax_ymax(file_name);
   //     cout<<file_name<<x_start<<y_start;
        this->load_train(file_name,x_start,y_start);
    }

    /*
    ~read_write()
    {
        object_count--;

        if(object_count==0)
        {
           // _TIFFfree(train_fp);
            (train_fp).close();
           // _TIFFfree(grid_fp);
            (grid_fp).close();
            //TIFFClose(train_fp);
            //TIFFClose(grid_fp);
        }
    }*/
void view_image(uint32 no_x,uint32 no_y,float**input,bool color_pixel)
{
    int wid1;// wid2;
    int gd = DETECT,gm;
   initgraph(&gd,&gm,NULL);
   int color=0,devlimit=1,remain=0;
      int colors[4]={0,7,8,15};
      uint16 max=2048;
      uint16 intal=max/4;
      uint32 no=0;

      for(uint32 x_value=0;x_value<no_x;x_value++)
        for(uint32 y_value=0;y_value<no_y;y_value++)
      {
          devlimit=intal;
          int j;
          if(color_pixel)
          {
              for(j=0;j<16;j++){
                  if(input[x_value][y_value]==j)
                      break;
              }
          }
          else
          for(j=0;j<4;j++)
          {

              remain=(input[x_value][y_value]/devlimit);
              if(remain>0)
              {
                  devlimit=devlimit*intal;
              }
              else
              {
                  break;
              }

          }
          if(color_pixel)
          {
              color=input[x_value][y_value];
          }
          else if(j==4)
              color=colors[3];
          else if(j>0)
              color=colors[j-1];
          else
              color=colors[0];
        putpixel ( x_value, y_value, color);
      }
      getch();

      closegraph();

//      return 0;

}


};
/*
string folder="../data/";
string train_file="train_wkt_v4.csv";
//";

string grid_file="grid_sizes.csv";

ifstream read_write::train_fp((folder+train_file).c_str());                 // handle for input image file
ifstream read_write::grid_fp((folder+grid_file).c_str());                // handle for output image file
//float read_write::xmax=0,read_write::ymax=0;
uint32 read_write::train_x=136,read_write::train_y=134;

 uint32 read_write::object_count=0;
*/
