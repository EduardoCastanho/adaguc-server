/******************************************************************************
 * 
 * Project:  ADAGUC Server
 * Purpose:  ADAGUC OGC Server
 * Author:   Maarten Plieger, plieger "at" knmi.nl
 * Date:     2013-06-01
 *
 ******************************************************************************
 *
 * Copyright 2013, Royal Netherlands Meteorological Institute (KNMI)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 ******************************************************************************/

#ifndef CDBFileScanner_H
#define CDBFileScanner_H

#include "CDebugger.h"
#include "CStopWatch.h"
#include "CDataReader.h"
#include "CDFObjectStore.h"
#include "CServerError.h"
#include "CDirReader.h"
#include "CPGSQLDB.h"
#include "CADAGUC_time.h"

/**
 * Class which scans files and updates the database.
 */
class CDBFileScanner{
private:
  DEF_ERRORFUNCTION();
  static int createDBUpdateTables(CPGSQLDB *DB,CDataSource *dataSource,int &removeNonExistingFiles,CDirReader *dirReader);
  static int DBLoopFiles(CPGSQLDB *DB,CDataSource *dataSource,int removeNonExistingFiles,CDirReader *dirReader);
  static std::vector <CT::string> tableNamesDone;
  static bool isTableAlreadyScanned(CT::string *tableName);
  //static int addIndexToTable(CPGSQLDB *DB,const char *tableName, const char * dimName);
public:
  /**
   * Populates dirReader with files defined by the path, exp and tailPath parameters
   * @param dirReader: An initialised dirReader should be provided, this dirReader will be filled with the found results.
   * @param path: This is usually the configured FilePath in the Layers configuration, can also be an OpenDAP URL
   * @param expr: The regular expression to reject/accept each individual file on. When NULL is provided, *.nc is used as default.
   * @param tailPath: The path under the root path to be scanned.
   */
  static int searchFileNames(CDirReader *dirReader,const char * path,const char * expr,const char *tailPath);
  
  /**
   * Updates the database for a specified dataSource
   * @param pszDBParams: The database connection parameters
   * @param dataSource: The datasource to update
   * @param tailPath: The path under the root path to be scanned.
   * @param _layerPathToScan: When matched with the datasources configured FilePath, the datasource is updated, otherwise it is ignored. 
   * This can be used to update a specific layer by defining its corresponding path. When all datasources are looped, only the datasource with matching paths are updated
   * When set to NULL, the datasource is updated always.
   */
  static int updatedb(const char *pszDBParams, CDataSource *dataSource,CT::string *tailPath,CT::string *_layerPathToScan);
  
};

#endif
