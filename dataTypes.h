#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include <vector>
#include <string>

using namespace std;

enum Platforms {
    ROS,
    PX4,
    POSIX
};

struct VarStruct {
    string   var_type;
    string var_id;
};

struct KeyValueStruct {
    string key;
    string value;
};

struct VarDeclStruct {
    string   var_type;
    string var_id;
    string var_value;
};

struct CmdStruct{
    string cmd_id;
    vector<VarStruct> cmd_args;
};

struct StatementStruct{
    string statement_id;
    string expression;
    int stmts_to_skip;
    vector<CmdStruct> func_cmds;
    VarDeclStruct var_decl;
};

struct FunctionStruct{
    string function_id;
    vector<StatementStruct> func_statements;
};

struct MsgStruct{
    string msg_id;
    vector<VarStruct> msg_fields;
};

struct SrvcStruct{
    string srvc_id;
    vector<VarStruct> srvc_rqst_fields;
    vector<VarStruct> srvc_rspnd_fields;
};

struct ActnStruct{
    string actn_id;
    vector<VarStruct> actn_rqst_fields;
    vector<VarStruct> actn_rspnd_fields;
    vector<VarStruct> actn_updt_fields;
};

struct CompConfiguration{
    string config_id;
    vector<VarDeclStruct> config_fields;
};

struct CompState{
    string state_id;
    vector<VarStruct> state_fields;
};

struct DiagStruct{
    string diag_id;
    vector<VarStruct> diag_rqst_fields;
    vector<VarStruct> diag_rspnd_fields;
};


struct InterfaceStruct{
   string iface_name;
   vector<string> send_msgs;
   vector<string> recv_msgs;
   vector<string> send_srvcs;
   vector<string> recv_srvcs;
   vector<string> send_actns;
   vector<string> recv_actns;
   vector<string> comp_conf;
   vector<string> comp_state;
   vector<string> diag_srvcs;
};

struct CompStruct{
   string component_id;
   vector<InterfaceStruct> comp_interfaces;
};

struct SystemConfig{
   string system_config_id;
   vector<string> config_components;
};

struct SystemStruct{
    string system_id;
    vector<KeyValueStruct> kv_system_params;
    vector<CompStruct> system_components;
    vector<SystemConfig> system_configs;
};

struct PackageStruct{
    string package_id;
    vector<KeyValueStruct> kv_package_params;
    vector<SystemStruct> package_systems;   
};

struct MyStruct{
    int id;
    char cmd[20];
};

#endif