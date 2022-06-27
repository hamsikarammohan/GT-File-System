#include "gtfs.hpp"
#include<sys/stat.h>
#include<string.h>
#include<map>
#include<iostream>
#include<vector>
#include <fstream>
using namespace std;
map< string, vector<write_t>> changes;
map< string, gtfs_t> gtfs_map;
map< string, map <string, file_t>> directory_map;

#define VERBOSE_PRINT(verbose, str...) do { \
    if (verbose) cout << "VERBOSE: "<< __FILE__ << ":" << __LINE__ << " " << __func__ << "(): " << str; \
} while(0)

int do_verbose;

gtfs_t* gtfs_init(string directory, int verbose_flag) {
    do_verbose = verbose_flag;
    gtfs_t *gtfs = NULL;
    
    VERBOSE_PRINT(do_verbose, "Initializing GTFileSystem inside directory " << directory << "\n");
    gtfs= new gtfs_t();
    gtfs->dirname=directory;
    struct stat direc;
    int file_present = stat(directory.c_str(), &direc);
    //Log file is not present in the directory
    if(file_present == 0) {
        cout<<"Directory exists";
        }
    else{
    //mkdir(directory.c_str(),0777);
    //printf("Directory created//"); 
    return NULL;
    ////}
    }
    ///TODO: Add any additional initializations and checks, and complete the functionality
    
    if(gtfs_map.find(directory) != gtfs_map.end()) {
    return gtfs_map.at(directory);
    }    
    else
    {map<string, file_t> temp_file;
    directory_map.insert(make_pair(directory,temp_file));
    gtfs_map.insert(make_pair(directory,gtfs));  
    }
    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns non NULL.
    return gtfs;
}

int gtfs_clean(gtfs_t *gtfs) {
    int ret = -1;
    if (gtfs) {
        VERBOSE_PRINT(do_verbose, "Cleaning up GTFileSystem inside ectory " << gtfs->dirname << "\n");
    } else {
        VERBOSE_PRINT(do_verbose, "GTFileSystem does not exist\n");
        return ret;
    }
    //TODO: Add any additional initializations and checks, and complete the functionality

    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns 0.
    return ret;
}

file_t* gtfs_open_file(gtfs_t* gtfs, string filename, int file_length) {
    file_t *fl = new file_t();
    fl->filename=filename;
    fl->file_length=file_length;
    if (gtfs) {
        VERBOSE_PRINT(do_verbose, "Opening file " << filename << " inside directory " << gtfs->dirname << "\n");
    } else {
        VERBOSE_PRINT(do_verbose, "GTFileSystem does not exist\n");
        return NULL;
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
    string directory_name=gtfs.dir_filename;
    //.remove(fl.filename.c_str());
    //if(directory_map.find(directory) != directory_map.end())
    //    {
    //        changes.erase(fl->filename)
    //    }
    map <string, file_t> temp;
    temp=directory_map.at(directory_name);
    if(temp.find(fl->filename) != temp.end())
        {
            temp.erase(fl->filename)
        }
    if(changes.find(fl->filename) != changes.end())
        { write_t t1=changes.at(fl->filename);
            remove(t1.log_filename);
            remove(t1.filename);
            changes.erase(fl->filename)
        }
    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns 0.
    return ret;
}

char* gtfs_read_file(gtfs_t* gtfs, file_t* fl, int offset, int length) {
    char* ret_data = NULL;
    if (gtfs and fl) {
        VERBOSE_PRINT(do_verbose, "Reading " << length << " bytes starting from offset " << offset << " inside file " << fl->filename <<"\n");
    } else {
        VERBOSE_PRINT(do_verbose, "GTFileSystem or file does not exist\n");
        return NULL;
    }
    //TODO: Add any additional initializations and checks, and complete the functionality
	struct stat buffer;
    int file_exist = stat(fl->filename.c_str(), &buffer);
    cout<< fl->filename <<endl;
    //File is not present in the directory
    if(file_exist != 0) {
        ofstream ofs(fl->filename);
        ofs.close();
        }

    vector<write_t> file_changes;
    
    if(changes.find(fl->filename) != changes.end()) {
    file_changes=changes.at(fl->filename);
    }

    ofstream outfile;
    outfile.open(fl->filename.c_str(),ios::out|ios::in);
    cout<< file_changes.size() <<endl;
    for(int i=0; i<file_changes.size(); i++)
    {  
    
    outfile.seekp(file_changes[i].offset, ios::beg);
    outfile.write(file_changes[i].data,file_changes[i].length);
    }
   

    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns pointer to data read.
    return ret_data;
}

write_t* gtfs_write_file(gtfs_t* gtfs, file_t* fl, int offset, int length, const char* data) {
    
    //if(gtfs)
    {VERBOSE_PRINT(do_verbose,"HI");}
    if (gtfs and fl) {
        VERBOSE_PRINT(do_verbose, "Writting " << length << " bytes starting from offset " << offset << " inside file " << fl->filename << "\n");
    } else {
        VERBOSE_PRINT(do_verbose, "GTFileSystem or file does not exist\n");
        return NULL;
    }
    //TODO: Add any additional initializations and checks, and complete the functionality
    //if(gtfs_map.find(directory) != g
    
    write_t *write_id = new write_t();
    string path = gtfs->dirname;
    path.append("/");
    path.append(fl->filename);
  //  cout<< fl->filename<<endl;
    write_id-> dir_filename= path;
 //   cout<< path << endl ;
    write_id->offset=offset;
    write_id->length=length;
    write_id->data = (char*)data;
  // strcpy(write_id->data, data);
    string logfile_name = write_id->dir_filename;
    logfile_name.append(".log");
    write_id->log_filename=logfile_name;
    write_id->filename = fl->filename;
    struct stat buffer;
    int file_exist = stat(logfile_name.c_str(), &buffer);
    //Log file is not present in the directory
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
     cout << write_id->data << endl;
     
   
   // file_log.end()->data=write_id->data;
  // file_log.end()->data.push_back(write_id->data);
    file_log.back().data= write_id->data;
   // if(changes.find(fl->filename) != changes.end()) {
  //  	changes.erase(fl->filename);
  //  }
    
    
    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns non NULL.
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
    int exists=stat(write_id->log_filename.c_str(),&buffer);
    if(exists==0)
    {
        ofstream outfile;
        string s="";
        outfile.open(write_id->log_filename.c_str(),ios::app);
        string s1=write_id->dir_filename;
        s1.append("|");
        string s2=to_string(write_id->length);
        s2.append("|");
        string s3=to_string(write_id->offset);
        s3.append("|");
        string s4=write_id->data;
        s.append(s1);
        s.append(s2);
        s.append(s3);
        s.append(s4);
        if(outfile.is_open())
        {
            outfile << s << "\n";
        }
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
    if(write_id->openflag==1)
    {cout<<"File open, cannot abort"}
    if(changes.find(write_id->filename) != changes.end()) {
    write_id->data.clear();
    return 0;
    }
    VERBOSE_PRINT(do_verbose, "Success.\n"); //On success returns 0.
    return ret;
   //return 0;
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
    //return ret;
    return 0;
}

int gtfs_sync_write_file_n_bytes(write_t* write_id, int bytes){
    int ret = -1;
    if (write_id) {
        VERBOSE_PRINT(do_verbose, "Persisting [ " << bytes << " bytes ] write of " << write_id->length << " bytes starting from offset " << write_id->offset << " inside file " << write_id->filename << "\n");
    } else {
        VERBOSE_PRINT(do_verbose, "Write operation does not exist\n");
        return ret;
    }
    changes.find(write_id->filename);
    VERBOSE_PRINT(do_verbose, "Success\n"); //On success returns 0.
    return ret;
}