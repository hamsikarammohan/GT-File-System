#include "gtfs.hpp"
#include<sys/stat.h>
#include<string.h>
#include<map>
#include<iostream>
#include<vector>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <ctime>
#include <algorithm>
using namespace std;
map< string, vector<write_t>> changes;
map< string, gtfs_t*> gtfs_map;
map<string,file_t*> temp_map;
map< string, map <string, file_t*> >directory_map;
vector<gtfs*> gtfs_objects;

#define VERBOSE_PRINT(verbose, str...) do { \
    if (verbose) cout << "VERBOSE: "<< __FILE__ << ":" << __LINE__ << " " << __func__ << "(): " << str; \
} while(0)

int sync_log_actual(gtfs_t* gtfs, string filename,string logfilename)
{

string line_;
string innerline_;
ifstream file_(logfilename.c_str());
if(file_.is_open())
{  
while(getline(file_,line_))
{
    int length=0;
    int offset=0;
    replace( line_.begin(), line_.end(), '#', '\n');
    string data_to_be_written="";
    stringstream stream(line_);
    int count=0;
    while(getline(stream,innerline_,'|'))
    {
    if(count % 4 == 0)
    {
    //cout<< innerline_ << "name"<< endl; 
    }
    else if(count % 4 == 1)
    {
        
        length=stoi(innerline_);
        cout<< "length error";
    }
    else if(count % 4 == 2)
    {
       
        offset=stoi(innerline_);
         cout<< "offset error";
    }
    else if(count % 4 == 3)
    {
        
    data_to_be_written=innerline_; 
    cout<<"data error";
    char c = '\n';
    if (data_to_be_written.find(c) != std::string::npos) {
        std::cout << "Character newline found" << std::endl;
    } else {
        std::cout << "Character not found" << std::endl;
    }

    }
    else
    {
    cout<< "default";
    }
    count++;
    }
    fstream file_actual_(filename.c_str());
    if(file_actual_.is_open())
    {
          file_actual_.seekp(offset);
          file_actual_<< data_to_be_written;
    }
    file_actual_.close();
    
}
file_.close();
}

}

int do_verbose;

gtfs_t* gtfs_init(string directory, int verbose_flag) {
    do_verbose = verbose_flag;
    gtfs_t *gtfs = NULL;
    VERBOSE_PRINT(do_verbose, "Initializing GTFileSystem inside directory " << directory << "\n");
    //TODO: Add any additional initializations and checks, and complete the functionality
    gtfs= new gtfs_t();
    gtfs->dirname=directory;
    struct stat direc;
    int file_present = stat(directory.c_str(), &direc);
    if(file_present == 0) {
        cout<<"Directory exists";
    }
    else{
    return NULL;
    }

    if(gtfs_map.find(directory) != gtfs_map.end()) {
   // gtfs_t gtfs_1 = &gtfs_map.at(directory);
  //  cout<< gtfs_1.dirname;
    return gtfs_map.at(directory);
    }
  //  auto it = find(gtfs_objects.begin(), gtfs_objects.end(), gtfs);
  //  if(it != gtfs_objects.end())
  //  {
  //      cout<<endl;
  //      cout<<"entered here";
  //        int index = it - gtfs_objects.begin();
  //        return gtfs_objects.at(index);
  //  }    
    else
    {
 
    map<string, file_t*> temp_file;
    directory_map.insert(make_pair(directory,temp_file));
    gtfs_map.insert(make_pair(directory,gtfs));  
    gtfs_objects.push_back(gtfs);
    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns non NULL.
    return gtfs;
    }

}

int gtfs_clean(gtfs_t *gtfs) {
    int ret = -1;
    if (gtfs) {
        VERBOSE_PRINT(do_verbose, "Cleaning up GTFileSystem inside directory " << gtfs->dirname << "\n");
    } else {
        VERBOSE_PRINT(do_verbose, "GTFileSystem does not exist\n");
        return ret;
    }
    //TODO: Add any additional initializations and checks, and complete the functionality
    struct dirent *ent;
    DIR *dir =opendir(gtfs->dirname.c_str());
    changes.clear();

    if(dir == NULL)
    {
        VERBOSE_PRINT(do_verbose,"Directory does not exist");
        return -1;
    }

    while((ent=readdir(dir))!=NULL)
    {
        string fname="";
        fname= string(ent->d_name);

        if(fname.size()<4)
        {
            continue;
        }

        string extendedname= fname.substr(fname.size()-4);
        string textextension= ".txt";
        if(extendedname.compare(textextension)==0)
        {
            string lfile= gtfs->dirname;
            lfile.append("/");
            lfile.append(fname);
            lfile.append(".log");
        

        int syncing_status=sync_log_actual(gtfs , fname ,lfile);
        if(syncing_status==0)
        {
            cout<<endl;
            cout<< fname << "synced succesfully"<<endl;
        }
        if(syncing_status!=0)
        {
            cout<< "Could not sync file";
            return -1;
        }
        ofstream l_file;
        l_file.open(lfile,ofstream::trunc);
        l_file.close();
        }     
    }
      cout<< "Successfully cleaned files";
        if(closedir(dir) == -1)
        {
            cout<< "could not close the directory";
            return -1;
        }
        else
        {
            return 0;
        }


    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns 0.
    return ret;
}

file_t* gtfs_open_file(gtfs_t* gtfs, string filename, int file_length) {
  //  file_t *fl = NULL;

    if (gtfs) {
        VERBOSE_PRINT(do_verbose, "Opening file " << filename << " inside directory " << gtfs->dirname << "\n");
    } else {
        VERBOSE_PRINT(do_verbose, "GTFileSystem does not exist\n");
        return NULL;
    }
    //TODO: Add any additional initializations and checks, and complete the functionality
  //  map<string, map<string,file_t*>>::iterator it;
  /*  for (auto i : directory_map)
    {
        if(i.first==(gtfs->dirname))
        {
       // cout<<endl<<"entering inside the second map loop"<<endl;
       // cout<<"  "<<i.first<<"  checking equality  "<<gtfs->dirname<<endl;
       cout<<"  Printing i.second.size " <<i.second.size();
        for (auto it=i.second.begin(); it!=i.second.end(); ++it)
        {
         //   cout<<"  "<<j.first<<"  checking equality  "<<filename;
            cout<< "Trying to print the filename " <<it->first;
            if(it->first==filename)
            {
                cout<<endl<<"There is a file name match"<<endl;
                if(it->second->file_length >= file_length)
                {
                    cout<< endl;
                    cout<< "You are requesting an existing file but of smaller length sorry cant process this"<<endl; 
                    return nullptr;
                }
                else
                {
                    cout<< endl;
                    cout<< "Oh I see you have requesting an existing file but of larger length..Let me update the length"<<endl;
                    it->second->file_length=file_length;
                    cout<< "Updated and sending the newly updated file to you";
                    return it->second;
                }


            }
        }
        }
    } */
    for(auto i: temp_map)
    {
            cout<<"filename" << i.first; 
            if(i.first==filename)
            {
              //  cout<<endl<<"There is a file name match"<<endl;
                if(i.second->file_length >= file_length)
                {
                    cout<< endl;
                //    cout<< "You are requesting an existing file but of smaller length sorry cant process this"<<endl; 
                    return nullptr;
                }
                else
                {
                    cout<< endl;
                  //  cout<< "Oh I see you have requesting an existing file but of larger length..Let me update the length"<<endl;
                    i.second->file_length=file_length;
                    i.second->open_status=1;
                  //  cout<< "Updated and sending the newly updated file to you";
                    return i.second;
                }
            }
    }

    cout<< endl;
   // cout<< "Oh no worries you dont have an existing file Lemme create a new one for yah :)"<<endl;
    file_t* fl = new file_t();
    fl->filename=filename;
    fl->file_length=file_length;
    fl->open_status=1;
    string lfile=gtfs->dirname;
    lfile.append("/");
    lfile.append(filename);
    string fname=lfile;
    struct stat buffer;
    //checking and creating the actual file
    int file_exist = stat(lfile.c_str(), &buffer);
    if(file_exist != 0) {
        ofstream logfile(lfile);
        logfile.close();
        
  //  cout<<"file created now pushing it into the global directory_map";
    // map<string, file_t*> file_map;
   // std::map<string,file_t*> file_map = new std::map<string,file_t*>;
   // std::map<int,int> *mymap = new std::map<int,int>;
    temp_map.insert(make_pair(fl->filename,fl));
    directory_map.insert(make_pair(gtfs->dirname,temp_map));
    }
    //checking and syncing the log file
    lfile.append(".log");
    struct stat buffer1;
    int file_exist1 = stat(lfile.c_str(), &buffer1);

    if(file_exist1 == 0) {
        int sync_result= sync_log_actual(gtfs, fname ,lfile);
        }
    std::ofstream ofs;
    ofs.open(lfile,std::ofstream::trunc);
    ofs.close();
    if(changes.find(filename) != changes.end()) {
        cout<< "cleaned the onmemorylog"<< endl;
        changes.erase(filename);
     }
 

    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns non NULL.
    return fl;
}

int gtfs_close_file(gtfs_t* gtfs, file_t* fl) {
    int ret = -1;
    if (gtfs and fl) {
        VERBOSE_PRINT(do_verbose, "Closing file " << fl->filename << " inside directory " << gtfs->dirname << "\n");
    } else {
        VERBOSE_PRINT(do_verbose, "GTFileSystem or file does not exist\n");
        return ret;
    }
    //TODO: Add any additional initializations and checks, and complete the functionality
    cout<<endl;
    if(fl->open_status==1)
    {
    fl->open_status=0;
    cout<< "File status has been changed to close"<<endl;;
    return 0;
    }
    else
    {
        cout << "you are trying to close a file which has not been opened"<<endl;
        return 0;
    }
    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns 0.
    return ret;
}

int gtfs_remove_file(gtfs_t* gtfs, file_t* fl) {
    int ret = -1;
    if (gtfs and fl) {
        VERBOSE_PRINT(do_verbose, "Removing file " << fl->filename << " inside directory " << gtfs->dirname << "\n");
    } else {
        VERBOSE_PRINT(do_verbose, "GTFileSystem or file does not exist\n");
        return ret;
    }
    //TODO: Add any additional initializations and checks, and complete the functionality
    if(fl->open_status==1)
    {
    //    cout<<"sorry cannot remove file it is still open";
        return -1;
    }
    else
    {
            string l_file=gtfs->dirname;
            l_file.append("/");
            l_file.append(fl->filename);
            ifstream myfile;
            myfile.open(l_file);
            int status;
            if(myfile)
            {
                myfile.close();
                status=remove(l_file.c_str());
                cout<< " removed the text file if status 0 " <<status;
            }
            else
            {
                cout<< "Lmao your file does not exist...what are you trying to delete???";
                return -1;
            }
          
            l_file.append(".log");
            myfile.open(l_file);
            if(myfile)
            {
                myfile.close();
                status=remove(l_file.c_str());
               cout<< " removed the log file if status 0 " <<status;
            }
            else
            {
            //    cout<< "Lmao your logfile does not exist...what are you trying to delete???";
                return -1;
            }


        return 0;
        
    }
    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns 0.
    return ret;
}

char* gtfs_read_file(gtfs_t* gtfs, file_t* fl, int offset, int length) {
    char* ret_data = new char[length];
    int found=0;        
  //  cout<< endl;
  //  cout<< "default value= " << ret_data << endl;
    if (gtfs and fl) {
        VERBOSE_PRINT(do_verbose, "Reading " << length << " bytes starting from offset " << offset << " inside file " << fl->filename << "\n");
    } else {
        VERBOSE_PRINT(do_verbose, "GTFileSystem or file does not exist\n");
        return NULL;
    }
    //TODO: Add any additional initializations and checks, and complete the functionality
    if(changes.find(fl->filename) != changes.end()) {
       vector<write_t> file_log; 
    file_log=changes.at(fl->filename);
  //  cout<<endl;
  //  cout << "the size of file log is" <<file_log.size();
    for(write_t i : file_log)
        {
         if(i.length==length && i.offset==offset)
         {
             ret_data=i.data;
             int found=1;
             return ret_data; 
         }
        } 
     } 
    if(found==0)
    {
    string filename=gtfs->dirname;
    filename.append("/");
    filename.append(fl->filename);
    fstream fs(filename.c_str());
    fs >> std::noskipws;
    struct stat buffer;
    char buffer1[length];
    int file_exist = stat(filename.c_str(), &buffer);
    if(file_exist==0)
    {
         fs.seekp(offset);
         fs.read(buffer1, length);
    }
    fs.close();
  //  cout<< "buffer is " << buffer1<< "between this" <<endl;
    if(buffer1=="")
    {
        return (char*)("");
    }
    stringstream stream1(buffer1);
    char c;
    stream1.get(c);
    int count=0;
    while(count!=length)
    {
        ret_data[count]=c;
        stream1.get(c);
        count++;
    }
    }
  //  int whitespaces= count( file.begin(), file.end(), ' ' );
 //   cout<< ret_data<<" this line is giving the shit"<<endl;

    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns pointer to data read.
    
    return ret_data;
    
}

write_t* gtfs_write_file(gtfs_t* gtfs, file_t* fl, int offset, int length, const char* data) {
    write_t *write_id =  new write_t();
    if (gtfs and fl) {
        VERBOSE_PRINT(do_verbose, "Writting " << length << " bytes starting from offset " << offset << " inside file " << fl->filename << "\n");
    } else {
        VERBOSE_PRINT(do_verbose, "GTFileSystem or file does not exist\n");
        return NULL;
    }
    //TODO: Add any additional initializations and checks, and complete the functionality
    string path = gtfs->dirname;
    path.append("/");
    path.append(fl->filename);
   // cout<< fl->filename<<endl;
    write_id-> dir_filename= path;
   
    write_id->offset=offset;
    write_id->length=length;
    write_id->data = (char*)data;
           // if(c=='\\n')
       // {
      //      cout<< "new line character is present"<<endl;
      //  }
     
     stringstream stream1((char*)data);
        int count =0;
        char c;
  /*  for(int i=0;count<length;i++)
    {
      stream1.get(c);
        if(c=='\n')
        {
            cout<< "new line character is present"<<endl;
        }
        count++;
    }*/
   //strcpy(write_id->data, (char*)data);
    string logfile_name = write_id->dir_filename;
    logfile_name.append(".log");
   // write_id->log_filename=logfile_name;
    write_id->filename = fl->filename;
    struct stat buffer;
    int file_exist = stat(logfile_name.c_str(), &buffer);
    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns non NULL.
     if(file_exist != 0) {
        ofstream logfile(logfile_name);
        logfile.close();
        }
    vector<write_t> file_log;
    
    if(changes.find(fl->filename) != changes.end()) {
        file_log=changes.at(fl->filename);
        file_log.push_back(*write_id);

        changes.erase(fl->filename);
        changes.insert(make_pair(fl->filename, file_log));
     }
     else
     {
         file_log.push_back(*write_id);
     changes.insert(make_pair(fl->filename,file_log));
     }
 //      file_log.back().data= write_id->data;
    return write_id;
}

int gtfs_sync_write_file(write_t* write_id) {
    int ret = -1;
    if (write_id) {
        VERBOSE_PRINT(do_verbose, "Persisting write of " << write_id->length << " bytes starting from offset " << write_id->offset << " inside file " << write_id->filename << "\n");
    } else {
        VERBOSE_PRINT(do_verbose, "Write operation does not exist\n");
        return ret;
    }
    //TODO: Add any additional initializations and checks, and complete the functionality
    struct stat buffer;
    string lfile=write_id->dir_filename;
    lfile.append(".log");
    int exists=stat(lfile.c_str(),&buffer);
    if(exists==0)
    {
        ofstream outfile;
        outfile.open(lfile.c_str(),ios::app);
        string s="";
        string s1=write_id->dir_filename;
        s1.append("|");
        string s2=to_string(write_id->length);
        s2.append("|");
        string s3=to_string(write_id->offset);
        s3.append("|");
        string s4=write_id->data;
        
 //       char c = '\n';
 //   if (s4.find(c) != std::string::npos) {
 //       std::cout << "Character newline found" << std::endl;
 //   } else {
 //       std::cout << "Character not found" << std::endl;
 //   }

       replace( s4.begin(), s4.end(), '\n', '#');

        s.append(s1);
        s.append(s2);
        s.append(s3);
        s.append(s4);
        if(outfile.is_open())
        {
           outfile << s << "\n";
   //           outfile << s;
        }
        outfile.close();
    }
    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns number of bytes written.
    return ret;
}

int gtfs_abort_write_file(write_t* write_id) {
    int ret = -1;
    if (write_id) {
        VERBOSE_PRINT(do_verbose, "Aborting write of " << write_id->length << " bytes starting from offset " << write_id->offset << " inside file " << write_id->filename << "\n");
    } else {
        VERBOSE_PRINT(do_verbose, "Write operation does not exist\n");
        return ret;
    }
    //TODO: Add any additional initializations and checks, and complete the functionality
    if(changes.find(write_id->filename) != changes.end()) {
        
        vector<write_t> file_log;
        file_log=changes.at(write_id->filename);
        int position=0;
        int recorded_position=-1;
        for(write_t i : file_log)
        {
         
         if(i.length==write_id->length && i.offset==write_id->offset && write_id->data==i.data)
         {
             recorded_position=position;
             break;
         }
          position++;
        } 
        if(position!=-1)
        {
        file_log.erase(file_log.begin()+recorded_position);
        changes.erase(write_id->filename);
        changes.insert(make_pair(write_id->filename, file_log));
        cout<< endl;
       // cout<< "YOO write aborted successfully";
        return 0;
        }
        else
        {
           // cout<<"YOO man you tripping  you tryna abort a write that dosent exist\n";
             return ret;
        }
       
    }
 
    VERBOSE_PRINT(do_verbose, "Success.\n"); //On success returns 0.
    return ret;
}

// BONUS: Implement below API calls to get bonus credits

int gtfs_clean_n_bytes(gtfs_t *gtfs, int bytes){
    int ret = -1;
    if (gtfs) {
        VERBOSE_PRINT(do_verbose, "Cleaning up [ " << bytes << " bytes ] GTFileSystem inside directory " << gtfs->dirname << "\n");
    } else {
        VERBOSE_PRINT(do_verbose, "GTFileSystem does not exist\n");
        return ret;
    }
    
    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns 0.
    return ret;
}

int gtfs_sync_write_file_n_bytes(write_t* write_id, int bytes){
    int ret = -1;
    if (write_id) {
        VERBOSE_PRINT(do_verbose, "Persisting [ " << bytes << " bytes ] write of " << write_id->length << " bytes starting from offset " << write_id->offset << " inside file " << write_id->filename << "\n");
    } else {
        VERBOSE_PRINT(do_verbose, "Write operation does not exist\n");
        return ret;
    }
    struct stat buffer;
    string lfile=write_id->dir_filename;
    lfile.append(".log");
    int exists=stat(lfile.c_str(),&buffer);
    if(exists==0)
    {
        ofstream outfile;
        outfile.open(lfile.c_str(),ios::app);
        string s="";
        string s1=write_id->dir_filename;
        s1.append("|");
        string s2=to_string(write_id->length);
        s2.append("|");
        string s3=to_string(write_id->offset);
        s3.append("|");
        string s4=write_id->data;
        string s5=s4.substr(0,bytes);
        cout<< "The substring is string 5  "<<endl;
        replace(s5.begin(), s5.end(), '\n', '#');

        s.append(s1);
        s.append(s2);
        s.append(s3);
        s.append(s5);
        if(outfile.is_open())
        {
           outfile << s << "\n";
   //           outfile << s;
        }
        outfile.close();
    }    

    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns 0.
    return 0;
}




	
	
	
