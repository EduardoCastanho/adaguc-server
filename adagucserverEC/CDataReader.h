#ifndef CDataReader_H
#define CDataReader_H
#include "CDataSource.h"
#include "CServerError.h"
#include "CDefinitions.h"
#include "CDirReader.h"
#include "CPGSQLDB.h"
#include "CADAGUC_time.h"
#include "CCDFDataModel.h"
#include "CCDFNetCDFIO.h"
#include "CCDFHDF5IO.h"
#include <math.h>
#include "CStopWatch.h"
#include "CDebugger.h"
class CDataReader{
  private:
  DEF_ERRORFUNCTION();
    CDataSource *sourceImage;
    CDFReader * cdfReader;
    CDFObject *cdfObject;
    int status;
    CT::string FileName;
    CDFReader *getCDFReader(CDataSource *sourceImage);
    
    int createDBUpdateTables(CPGSQLDB *DB,CDataSource *sourceImage,int &removeNonExistingFiles);
    int DBLoopFiles(CPGSQLDB *DB,CDataSource *sourceImage,int removeNonExistingFiles,CDirReader *dirReader);
  
  public:
    CDFObject *getCDFObject(){
      return cdfObject;
    }
    int getTimeString(char * pszTime);
    int getTimeUnit(char * pszTime);
    const char *getFileName(){return FileName.c_str();}
    CDataReader(){cdfReader=NULL;cdfObject=NULL;}
    ~CDataReader(){
      if(cdfReader!=NULL){cdfReader->close();delete cdfReader;cdfReader=NULL;}
      if(cdfObject!=NULL){delete cdfObject;cdfObject=NULL;};
    }
    int open(CDataSource *dataSource,int mode,const char *cacheLocation);
    int close();
    int updatedb(const char *pszDBParams, CDataSource *dataSource,CT::string *tailPath,CT::string *_layerPathToScan);
};
//Table names need to be different between time and height.
// Therefore create unique tablenames like tablename_time and tablename_height
void makeCorrectTableName(CT::string *tableName,CT::string *dimName);
#endif
