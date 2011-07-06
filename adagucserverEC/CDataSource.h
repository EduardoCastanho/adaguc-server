#ifndef CDataSource_H
#define CDataSource_H
#include "CXMLSerializerInterface.h"
#include "CServerParams.h"
#include "CServerConfig_CPPXSD.h"
#include "CDebugger.h"
#include "Definitions.h"
#include "CTypes.h"
#include "CCDFDataModel.h"
#include "COGCDims.h"

/**
 * This class represents data to be used further in the server. Specific  metadata and data is filled in by CDataReader
 * This class is used for both image drawing (WMS) and data output (WCS)
 */
class CDataSource{
  private:
   DEF_ERRORFUNCTION();
 
  public:
  class StatusFlag{
    public:
      CT::string meaning;
      double value;
  };
    
  class DataClass{
    public:
      DataClass();
      ~DataClass();
      bool hasStatusFlag,hasNodataValue,scaleOffsetIsApplied;
      double dfNodataValue,dfscale_factor,dfadd_offset;
      void *data;

      std::vector<StatusFlag*> statusFlagList;
      CDF::Variable *cdfVariable;
      CDFObject *cdfObject;
      CDFType dataType;
      CT::string variableName,units;

      const char *getFlagMeaning(double value);
  };
  
  class Statistics{
    private:
      double min,max;
    public:
      double getMinimum();
      double getMaximum();
      int calculate(CDataSource *dataSource);      // TODO this currently works only for float data
  };
  
  class TimeStep{
    public:
      CT::string fileName;   //Filename of the file to load
      CT::string timeString; //String of the current time
      CCDFDims   dims;//Dimension index in the corresponding name and file
  };
  
  int currentAnimationStep;
  std::vector <TimeStep*> timeSteps;
  bool stretchMinMax;
  std::vector <COGCDims*> requiredDims;
  Statistics *statistics;
  //The actual dataset data (can have multiple variables)
  std::vector <DataClass *> dataObject;
  //source image parameters
  double dfBBOX[4],dfCellSizeX,dfCellSizeY;
  int dWidth,dHeight;
  CT::string nativeEPSG;
  CT::string nativeProj4;

  //Used for scaling the legend to the palette range of 0-240
  float legendScale,legendOffset,legendLog,legendLowerRange,legendUpperRange;
  bool legendValueRange;

  //Number of metadata items
  size_t nrMetadataItems;
  CT::string *metaData;

  //Configured?
  bool isConfigured;

  //Numver of dims
  int dNetCDFNumDims;
  int dLayerType;
  CT::string layerName;

  //Current value index of the dim
  //int dOGCDimValues[MAX_DIMS];
  
  CServerParams *srvParams;

  //Link to the XML configuration
  CServerConfig::XMLE_Layer * cfgLayer;
  CServerConfig::XMLE_Configuration *cfg;
  
  //Link to the root CDFObject, is owned by the datareader.
  
  
  CDataSource();
  ~CDataSource();
  void setCFGLayer(CServerParams *_srvParams,CServerConfig::XMLE_Configuration *_cfg,CServerConfig::XMLE_Layer * _cfgLayer,const char *_layerName);
  void addTimeStep(const char * pszName,const char *pszTimeString);
  const char *getFileName();
  void setTimeStep(int timeStep);
  int getCurrentTimeStep();
  size_t getDimensionIndex(const char *name);
  size_t getDimensionIndex(int i);
  int getNumTimeSteps();
  const char *getLayerName();
  
  
  int attachCDFObject(CDFObject *cdfObject){
    if(cdfObject==NULL){
      CDBError("cdfObject==NULL");
      return 1;
    }
    if(isConfigured==false){
      CDBError("Datasource %s is not configured",cfgLayer->Name[0]->value.c_str());
      return 1;
    }
    if(dataObject.size()<=0){
      CDBError("No variables found for datasource %s",cfgLayer->Name[0]->value.c_str());
      return 1;
    }
  
    for(size_t varNr=0;varNr<dataObject.size();varNr++){
      dataObject[varNr]->cdfObject = cdfObject;
      dataObject[varNr]->cdfVariable = cdfObject->getVariableNE(dataObject[varNr]->variableName.c_str());
      if(dataObject[varNr]->cdfVariable==NULL){
        CDBError("attachCDFObject: variable \"%s\" does not exist",dataObject[varNr]->variableName.c_str());
        return 1;
      }
    }
    return 0;
  }
  void detachCDFObject(){
    for(size_t j=0;j<dataObject.size();j++){
      dataObject[j]->cdfVariable = NULL;
      dataObject[j]->cdfObject = NULL;
    }
  }
  
};

#endif
