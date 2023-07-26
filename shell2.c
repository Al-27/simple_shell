#include "main.h"
#include "shell.h"

void free_all(void* comm, char** args)
{
    char **ptmp = null;
    
    if(comm)
    {
        free(comm);
        comm = null;
    }
    
    if(args)
    {
        ptmp = args;
        while(*ptmp)
        {
            free(*ptmp);
            *ptmp=null;
            ptmp++;
            
        } 
        free(args);
        ptmp = null;
    }
        
}


int byte_occuren(char* str, char c){
    
    int count = 0;
    
    if(str)
        while(*str)
        {
            if(*str == c)
                count++;
            str++;
        }
    
    return count;
}


char* getEnvVar(char* var)
{
   char** env = __environ, *path = null;
   int lenVar = strlen(var);
    
   while(*env)
   {
        if(strncmp(*env,var,lenVar) == 0)
        {
            path = strdup(*env+(lenVar+1));
            break;
        }
        
        env++;
   }
   
   if(path)
   {
    return path;
   }
   return null;
}


char** splitStr(char* path, int nDelim, ...)
{
        /*
            split @path 
        */
    char **paths = null, *curp = path, delimiter[nDelim], *last_occ = null;
    int iLength_of_paths = 1, i = 0 , j = 0, d = 0, loop = 1;
    long int diff = 0;
    va_list va_delim;
    
    if(!path)
        return null;
    
    va_start(va_delim,nDelim);
        for(; d < nDelim ; d++)
        {
            delimiter[d] = (char)va_arg(va_delim,int);
            j = byte_occuren(path,delimiter[d]) ;
            iLength_of_paths += j ? j : 1 ;
            
            if( delimiter[d] == '&' || delimiter[d] == '|')
                iLength_of_paths -= (iLength_of_paths-1)/2;
            
            last_occ = strrchr(path,delimiter[d]) ;
            last_occ = last_occ ? last_occ : path;
            
            if ( diff < ( last_occ - path + 1) )
                diff = last_occ - path + 1;            
            
        }
    va_end(va_delim);
    
    
    if( strlen(path) > (size_t)diff )
    {
        iLength_of_paths++;
    }
    
    path = curp;
    paths = malloc(sizeof(char*) * (iLength_of_paths));
     
    if(!paths)
        return null;
    
   /* paths[iLength_of_paths] = null;*/
    
    for(; i < iLength_of_paths-1 ; i++)
    {
        
        paths[i] = malloc(BUFF_SIZE);
        
        if( !paths[i] )
            return null;
            
        memset(paths[i],0,BUFF_SIZE);
        
        for(j = 0; *path; path++,j++)
        {
            for(d = 0; d < nDelim ; d++)
                if(delimiter[d] == *path)
                    loop = 0;
            if(!loop)
                break;
                
            paths[i][j] = *path;
        }
        
        paths[i][j] = '\0';
        
        while(1)
        {
            loop = 0;
            
            for(d = 0; d < nDelim ; d++)
            {
                if(delimiter[d] == *path)
                    {
                        loop = 1;
                        break;
                    }
            }
            if(!loop)
                break;
                
            path++;
        }
        loop = 1;
    }
    
    if(paths[i-1][0]=='\0')
    {
        free(paths[i-1]);
        i--;
    }
    paths[i] = null;
    
    return paths;
}


char* does_exist(char* filename)
{
    char** binPaths = null, *path= getEnvVar("PATH"), *baseFile = null, *fullpath=null;
    void *pp = null;
    int found = 0;
    
    if( !path || !filename )
        return null;
    
    if( !(baseFile = malloc(BUFF_SIZE)) )
        return null;
    
    memset(baseFile,0,BUFF_SIZE);
    baseFile = strcat(baseFile, "/");
    baseFile = strcat(baseFile, filename);
    
    binPaths = splitStr(path,1,':');
    
    if(!binPaths)
        return null; 
    
    pp = binPaths;
    
    while(*binPaths && !found)
    {
        if( !(fullpath = malloc( BUFF_SIZE)) )
            return null;
        
        
        memset(fullpath,0,BUFF_SIZE);
        fullpath = strcat(fullpath,*binPaths);
        
        fullpath = strcat(fullpath,baseFile);
        
        if( access(fullpath, F_OK) == F_OK ){
            found = 1;
        }
        
        if( !found ){
            free(fullpath); 
            fullpath = null;
        }
        
        binPaths++;
    }
    
    binPaths = pp;
    
    free_all(baseFile, binPaths);
    free(path);
    
    if(!found)
        return filename;
    
    free(filename);
    return fullpath;
}

