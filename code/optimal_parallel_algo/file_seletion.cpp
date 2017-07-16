
uint16 file_selection(string folder,string grid_file,string *&file_names)
{

    //if((i_no%10)==2)
         //       {
    ifstream grid_fp((folder+grid_file).c_str());
    file_names=new string[NO_OF_FILES];
    string grid_line="";
     int ii=0;
     uint16 i=0;
     while(std::getline(grid_fp,grid_line))
     {
       if(ii==0)
       {
         ii++;
         continue;
        }
       string file_name=grid_line.substr(0,8);
       ifstream fp1((folder+file_name+".tif").c_str());
       ifstream fp2((folder+file_name+"_A.tif").c_str());
       ifstream fp3((folder+file_name+"_P.tif").c_str());
       ifstream fp4((folder+file_name+"_M.tif").c_str());
        if(fp1&&fp2&&fp3&&fp4)
        {
            fp1.close();
            fp2.close();fp3.close();fp4.close();
            file_names[i]=file_name;
            i++;
        }
         }
     return ++i;
}

