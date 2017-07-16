class parallel_read_write
{

private:

    static ifstream train_fp;                 // handle for input image file
    static ifstream grid_fp;                // handle for output image file
    static string data_folder_path;
/*    string file_full_path_3;
    string file_full_path_m;
    string file_full_path_a;
    string file_full_path_p;*/
    //string file_train_path;
    //string file_grid_path;
    static float A_x_fact,A_y_fact,P_x_fact,P_y_fact,M_x_fact,M_y_fact,RGB_x_fact,RGB_y_fact;
   // uint16 input_size_x;
   // uint16 input_size_y;



public:
    float xmax,ymax;
    float **train_array;
    float ***image_3=nullptr;
    float ***image_M=nullptr;
    float **image_P=nullptr;
    float ***image_SWIR=nullptr;
    static uint32 train_x,train_y;
    uint32 image_3_x,image_3_y;
    uint16 image_3_z;
    uint32 image_m_x,image_m_y;
    uint16 image_m_z;
    uint32 image_p_x,image_p_y;
    uint32 image_swir_x,image_swir_y;
    uint16 image_swir_z;


    static uint32 object_count;




 
	// Open the TIFF image
private:
      void find_file(string file_name)
    {
          string grid_line;
         //cout<<"Hello"<<omp_get_thread_num();

        if(!std::getline(grid_fp,grid_line))
        return;
        if((grid_line.substr(0,8)).compare(file_name)==0)
        {
         int grid_line_size=grid_line.size();
         int x_position;
         x_position=grid_line.substr(9,grid_line_size).find(",");
         xmax=stof(grid_line.substr(9,x_position));
         ymax=std::stof(grid_line.substr(10+x_position,grid_line_size));
         //#pragma omp taskwait
         #pragma omp cancel task;
         }

        else
        {
        #pragma omp task shared(file_name)
        {
                find_file(file_name);
        }
            #pragma omp taskwait
        }
    }

    void initilize_xmax_ymax(string file_name)
    {

        string grid_line="";
        std::getline(grid_fp,grid_line);
        #pragma omp parallel shared(file_name)
        #pragma omp single
        find_file(file_name);
        //#pragma omp barrier
         grid_fp.clear();
         grid_fp.seekg(0, ios::beg);
    }

void load_train_image_line(string file_name,uint16 start_x,uint16 start_y,int train_class)
{
    string line;bool ch=false;string cell="";
    //cout<<"\n"<<"Hello";

#pragma omp critical
    {
        ch=getline(train_fp,line);

        #pragma omp flush(train_fp)
    }
    if(!ch)
      {

    //#pragma omp cancel task;
    return;
      }
    //cout<<"We r here";
//#pragma omp critical
  //      {
     //   cout<<"\nsadsasd"<<line.substr(0,8)<<"===="<<file_name;
        if(train_class==10)
        {

            train_class=0;
        }
            train_class+=1;

//#pragma omp flush(train_class)
    //    }

    if(line.substr(0,8).compare(file_name)!=0){

    #pragma omp task firstprivate(file_name,start_x,start_y,train_class)//shared(file_name,start_x,start_y,train_class)
     load_train_image_line(file_name,start_x,start_y,train_class);
    // #pragma omp taskwait
    }
    else
    {

        line.erase(0,24);
 //   #pragma omp task firstprivate(start_x,start_y,train_class) shared(line)
        load_train_image_line_word(line,0,start_x,start_y,train_class);
   //#pragma omp task firstprivate(file_name,start_x,start_y,train_class)//shared(file_name,start_x,start_y,train_class)
        load_train_image_line(file_name,start_x,start_y,train_class);
 //#pragma omp taskwait
    }

}

void load_train_image_line_word(string line,long int p,uint16 start_x,uint16 start_y,int train_class)
{
char *cell=nullptr;
string x="",y="";
long int po=line.find(',',p);
if(po==-1)
    return;
else
{
    //cell=new char[po-p];
    int l=(po-p);
    cell=new char[l]();
    line.copy(cell,l,p);

//    cout<<"\n po= "<<po<<" p="<<p<<" "<<(po-p)<<" "<<cell;
    //line.copy(cell,po,p);
    ++po;
   // #pragma omp task firstprivate(start_x,start_y,train_class) shared(line)
    load_train_image_line_word(line,po,start_x,start_y,train_class);
    float dx=x_scale_to_pixel();
    float dy=y_scale_to_pixel();
    bool che_p=true;


    for(uint32 i=0;i<(po-p-1);i++)
    {

        if(cell[i]==' ')
        {
            if(cell[i+1]=='-')
            che_p= false;
            else if(cell[i+1]=='0')
            che_p=true;
        }
        if((cell[i]=='(')||(cell[i]==')')||(cell[i]==' '))
                {
                        continue;
                }
        if(che_p)
        {
            x=x+cell[i];
        }
        else
        {
            y=y+cell[i];
        }

    }
    if(x=="")x="0.0";
    if(y=="")y="0.0";
    try{
    long double fx=atof(x.c_str());
    long double fy=atof(y.c_str());
//cout<<"\n"<<x<<" "<<y;
    int x1=(fx*dx),y1=(fy*dy);
//cout<<"\n Helloin else"<<train_class<<"\n "<<x1<<" "<<y1;
    if((x1<start_x)||(y1<start_y))
        return;
    else
    {
    //      #pragma omp critical
        //if((x1>train_x)||(y1>train_y))
    //cout<<"\n Helloin else"<<train_class<<"\n "<<x1<<" "<<y1;
    if(!((x1>train_x)||(y1>train_y)))
    train_array[x1-start_x][y1-start_y]=train_class;
    }

    }
    catch(int e)
    {

        cout<<"\nerror\t"<<e;return;
    }

}
}



/*
if(po==-1)
{
    if(line.length()==(p+1))
    {

        return;
    }
    else
    {
        //cout<<"Hello"<<po<<"\n";
        po=line.length();
       // cout<<"Hello"<<po<<"\n";
       // cell=new char[po];
       // line.copy(cell,po,p);
    }

}

else
{
    if(line[po+1]=='\0')
    {

        return;
    }
    else
    {
     //   ++po;

        cell=new char[po];
        line.copy(cell,po,p);
        cout<<"Hello"<<po<<cell<<"\n";
       ++po;

#pragma omp task shared(line) //firstprivate(po,start_x,start_y,train_class)
        {
            load_train_image_line_word(line,po,start_x,start_y,train_class);
        }
    }
}

        float dx=x_scale_to_pixel();
        float dy=y_scale_to_pixel();
        string emp1="EMPTY ",emp2=" EMPTY",emp3="EMPTY",x="",y="";
        if((cell[0]==emp1[0])||(cell[1]==emp2[1])||(cell[2]=='E'))
        {
            return;
         //   cout<<"Empty";
        }

        bool che_p=true;

        for(int i=0;i<po;i++)
        {

            if(cell[i]==' ')
            {
                if(cell[i+1]=='-')
                che_p= false;
                else if(cell[i+1]=='0')
                che_p=true;
            }
            if((cell[i]=='(')||(cell[i]==')')||(cell[i]==' '))
                    {
                            continue;
                    }
            if(che_p)
            {
                x=x+cell[i];
            }
            else
            {
                y=y+cell[i];
            }
        }

            if(x=="")x="0.0";
            if(y=="")y="0.0";
            try{
            float fx=stof(x);
            float fy=stof(y);

            int x1=(fx*dx),y1=(fy*dy);

            if((x1<start_x)||(y1<start_y))
                return;
            else
            {
  //          #pragma omp critical
            //    if((x1>train_x)||(y1>train_y))
            cout<<"\n Helloin else"<<train_class<<"\n "<<x1<<" "<<y1;
            //train_array[x1-start_x][y1-start_y]=train_class;
            }

            }
            catch(int e)
            {
                cout<<"\nerror\t"<<e;return;
            }
    //return;
        }

    }*/


/*
    bool b=false;char *cell;
//    #pragma omp critical
  //  {
        b=std::getline(lineStream,cell,',');
       //#pragma omp flush(lineStream)
  //}
//#pragma omp flush(cell)

    if(!b)
    {
        //#pragma omp cancel task;
        return;
    }
    else if((!cell.compare("EMPTY"))||!cell.compare(" EMPTY"))
    {
        //cout<<"Helloin word emty"<<cell;
        return;
    }
    else
    {

//#pragma omp task firstprivate(start_x,start_y,train_class) shared(lineStream)
load_train_image_line_word(lineStream,start_x,start_y,train_class);
//cout<<"Helloin word emty"<<cell;
//#pragma omp cancel task

        float dx=x_scale_to_pixel();
        float dy=y_scale_to_pixel();
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
            float x=stof(fr);
            float y=stof(sc);

            int x1=(x*dx),y1=(y*dy);

            if((x1<start_x)||(y1<start_y))
                return;
            else
            {
  //          #pragma omp critical
            //    if((x1>train_x)||(y1>train_y))
            cout<<"\n Helloin else"<<train_class<<"\n "<<x1<<" "<<y1;
            //train_array[x1-start_x][y1-start_y]=train_class;
            }

            }
            catch(int e)
            {
                cout<<"\nerror\t"<<e;
            }
        //    #pragma omp taskwait
        return;
        }
*/



void load_train(string file_name,uint16 start_x,uint16 start_y)
{

     std::string line;int class_l=0;
    train_array=read_init_image_str_to_array(train_x,train_y,false);
    std::getline(train_fp,line);
    //uint32 no=0;

#pragma omp parallel firstprivate(file_name,start_x,start_y,class_l)
#pragma omp single
    load_train_image_line(file_name,start_x,start_y,class_l);
//#pragma omp taskwait
    //cout<<no;
}


  /*  void load_train(string file_name,uint16 start_x,uint16 start_y)
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
#pragma omp for private(line)
    for(;std::getline(train_fp,line);)
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

#pragma omp for private(cell)
            for(;std::getline(lineStream,cell,',');)
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
                train_array[x1-start_x][y1-start_y]=train_class;
                //throw x1;
                }
                catch(int e)
                {
                    cout<<"\nerror\t"<<e;
                }

            }
        }
    #pragma omp barrier
    train_fp.clear();
    train_fp.seekg(0, ios::beg);
    }
*/
float x_scale_to_pixel()
{
    float w1=(train_x*train_x)/(train_x+1);
    float dx=w1/xmax;

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
float** read_init_image_str_to_array(uint32 x,uint32 y,bool b)
{
    float** p=0;
    p=new float*[x];
    //cout<<x<<y;
    if(!b)
    {
#pragma omp for
    for(int i=0;i<x;i++)
    {
        p[i]=new float[y]();
    }

    return p;
    }

//
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
#pragma  omp parallel for schedule(static)
        for (int r1 = 0; r1 < height; r1++)
        {
            if(r1<x_start)
                continue;
            if((r1-x_start)==x_image_size)
            #pragma omp cancel for
#pragma omp critical
            {
            TIFFReadScanline(image, scanline, r1, 0);
            pixel_image[r1-x_start]=new float[y_image_size]();
            }
#pragma  omp parallel for
            for (int c1 = 0; c1 < (width); c1++)
            {
                if(c1<y_start)
                    continue;
                if((c1-y_start)==y_image_size)
               #pragma omp cancel for
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
#pragma  omp for
        for(int s=0;s<SamplesPerPixel;s++)
        {
            pixel_image[s]=new float*[x_image_size]();
#pragma  omp for
            for(int i=0;i<x_image_size;i++)
            {
                pixel_image[s][i]=new float[y_image_size]();

            }
        }
#pragma  omp for schedule(static)
        for (int r1 = 0; r1 < height; r1++)
        {
            if(r1<x_start)
                continue;
            if((r1-x_start)==x_image_size)
           #pragma omp break;
#pragma omp criticale
            TIFFReadScanline(image, scanline, r1, 0);
            //pixel_image[r1-x_start]=new float*[y_image_size]();
#pragma omp for
            for (int c1 = 0; c1 < (width); c1++)
            {

                if(c1<y_start)
                    continue;
                if((c1-y_start)==y_image_size)
                #pragma omp break;
               // pixel_image[r1-x_start][c1-y_start]=new float[SamplesPerPixel]();
#pragma omp for
                for(uint16 s=0;s<SamplesPerPixel;s++)
                {

                    pixel_image[s][r1-x_start][c1-y_start]=scanline[c1*SamplesPerPixel+s];

                }
            }
        }

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
          image_M=from_image_to_memory(image,image_m_x,image_m_y,image_m_z,x_start,y_start,image_size_x*M_x_fact,image_size_y*M_y_fact);

        //Opening A file
        file_path=path+"_A.tif";
        if((image = TIFFOpen(file_path.c_str(), "r")) == NULL){
        cout<<"Could not open incoming A image\n";
        exit(42);
    }
        init_image_size(image,&image_swir_x,&image_swir_y,&image_swir_z);
        //cout<<image_swir_x<<" "<<image_swir_y<<" "<<image_swir_z;
        image_SWIR=from_image_to_memory(image,image_swir_x,image_swir_y,image_swir_z,x_start,y_start,image_size_x*A_x_fact,image_size_y*A_y_fact);
        TIFFClose(image);

        //Opening RGB file

        file_path=path+".tif";
        if((image = TIFFOpen(file_path.c_str(), "r")) == NULL){
        cout<<"Could not open incoming RGB image\n";
        exit(42);
    }
        init_image_size(image,&image_3_x,&image_3_y,&image_3_z);
        image_3=from_image_to_memory(image,image_3_x,image_3_y,image_3_z,x_start,y_start,image_size_x*RGB_x_fact,image_size_y*RGB_y_fact);

        TIFFClose(image);
    }
    public:

    parallel_read_write(string file_name,uint16 image_size_x,uint16 image_size_y,uint16 x_start,uint16 y_start)
    {

        //string folder="../../data/";
        //string train_file="train_wkt_v4.csv";
        //";

        //string grid_file="grid_sizes.csv";
        //image_to_memory(data_folder_path+file_name,image_size_x,image_size_y,x_start,y_start);
      //  if(image_SWIR==nullptr)cout<<"aad";
       // file_train_path=folder+train_file;
       // file_grid_path=folder+grid_file;
        //cout<<"hello";
        object_count++;

        this->initilize_xmax_ymax(file_name);
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
