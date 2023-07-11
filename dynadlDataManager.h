#include "dataTypes.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <map>
#include <boost/interprocess/ipc/message_queue.hpp>

using json = nlohmann::json;

class dynadlDataManager{
   private:
      vector<PackageStruct> packages;
      vector<MsgStruct> msgDefinitions;
      vector<SrvcStruct> srvcDefinitions;
      vector<ActnStruct> actnDefinitions;
      vector<CompConfiguration> compConfigDefinitions;
      vector<CompState> compStateDefinitions;
      vector<DiagStruct> diagDefinitions;
      vector<FunctionStruct> funcDefinitions;
      string selected_package;
      string selected_system;
      std::map<std::string,boost::interprocess::message_queue *> name_to_msgqueue_map;
      std::map<std::string,std::string> event_to_func_map;
      Platforms platform_;
   public:
      dynadlDataManager(Platforms platform){platform_ = platform;}
      ~dynadlDataManager(){}
      void addPackage(string pckg_id);
      void addSystem(string pckg_id,string sys_id);
      void addComponent(string pckg_id,string sys_id,CompStruct comp);
      void addKeyValuePackage(string pckg_id, KeyValueStruct kv_pair);
      void addKeyValueSystem(string pckg_id, string sys_id, KeyValueStruct kv_pair);
      void addSysConfig(string pckg_id,string sys_id,SystemConfig sys_cfg);
      void addMsgDefinition(MsgStruct msg_struct);
      void addSrvcDefinition(SrvcStruct srvc_struct);
      void addActnDefinition(ActnStruct actn_struct);
      void addCompConfigDefinition(CompConfiguration comp_config_struct);
      void addCompStateDefinition(CompState comp_state);
      void addDiagDefinition(DiagStruct diag_struct);
      void addFuncDefinition(FunctionStruct func_struct);
      void addStatementDefinition(string func_id,StatementStruct stmt_struct);
      void printDefinitions(void);
      void printPackages(void);
      void printFunctions(void);
      void generate();
      void generateCode(string current_folder,CompStruct compctx, json j_config, json j_state, json j_diag);
      void executeRuntimeScript(string func_name);
      void executeCommands(StatementStruct *stmt_struct,vector<VarDeclStruct> *var_decl_vector_ptr);
      void sendCmd(string queue_name,string cmd);
      static void startTimer(dynadlDataManager *dataManager ,string delay, string func_name);
      static void spawnFunction(dynadlDataManager *dataManager ,string func_name);
};