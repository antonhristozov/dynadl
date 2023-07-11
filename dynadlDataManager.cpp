#include "dynadlDataManager.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <thread>
#include <sys/stat.h>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include "dataTypes.h"

using namespace boost::interprocess;
using json = nlohmann::json;

// Print the definitions section
void dynadlDataManager::printDefinitions(void){
    std::cout << endl << "Printing Defintions :" << endl << endl;
    std::cout << endl << "Messages :" << endl << endl;
    for( MsgStruct msgctx : msgDefinitions){
       std::cout << msgctx.msg_id << endl;
       for(VarStruct msg_fld : msgctx.msg_fields){
           std::cout << '\t' << msg_fld.var_type << " " << msg_fld.var_id << endl;
       }
    }
    std::cout << endl << "Services :" << endl << endl;
    for( SrvcStruct srvcctx : srvcDefinitions){
       std::cout << srvcctx.srvc_id << endl;
       for(VarStruct srvc_fld : srvcctx.srvc_rqst_fields){
           std::cout << "\tin " << srvc_fld.var_type << " " << srvc_fld.var_id << endl;
       }
       for(VarStruct srvc_fld : srvcctx.srvc_rspnd_fields){
           std::cout << "\tout " << srvc_fld.var_type << " " << srvc_fld.var_id << endl;
       }
    }
    std::cout << endl << "Actions :" << endl << endl;
    for( ActnStruct actnctx : actnDefinitions){
       std::cout << actnctx.actn_id << endl;
       for(VarStruct actn_fld : actnctx.actn_rqst_fields){
           std::cout << "\tin " << actn_fld.var_type << " " << actn_fld.var_id << endl;
       }
       for(VarStruct actn_fld : actnctx.actn_rspnd_fields){
           std::cout << "\tout " << actn_fld.var_type << " " << actn_fld.var_id << endl;
       }
       for(VarStruct actn_fld : actnctx.actn_updt_fields){
           std::cout << "\tupdate " << actn_fld.var_type << " " << actn_fld.var_id << endl;
       }       
    }
    std::cout << endl << "Component Configuration Parameters :" << endl << endl;
    for( CompConfiguration compconfigctx : compConfigDefinitions){
       cout << compconfigctx.config_id << endl;
       for(VarDeclStruct cfg_fld : compconfigctx.config_fields){
           std::cout << '\t' << cfg_fld.var_type << " " << cfg_fld.var_id << " " << cfg_fld.var_value << endl;
       }
    }
    std::cout << endl << "Component State Parameters :" << endl << endl;
    for( CompState compstatectx : compStateDefinitions){
       std::cout << compstatectx.state_id << endl;
       for(VarStruct state_fld : compstatectx.state_fields){
           std::cout << '\t' << state_fld.var_type << " " << state_fld.var_id << endl;
       }
    }
    std::cout << endl << "Diagnostic Services :" << endl << endl;
    for( DiagStruct diagctx : diagDefinitions){
       std::cout << diagctx.diag_id << endl;
       for(VarStruct diag_fld : diagctx.diag_rqst_fields){
           std::cout << "\tin " << diag_fld.var_type << " " << diag_fld.var_id << endl;
       }
       for(VarStruct diag_fld : diagctx.diag_rspnd_fields){
           std::cout << "\tout " << diag_fld.var_type << " " << diag_fld.var_id << endl;
       }
    }
    std::cout.flush(); 
}

// Print the packages/systems/component architecture section
void dynadlDataManager::printPackages(void){
    std::cout << endl << "Printing Packages :" << endl << endl;
    for(PackageStruct pckgsctx : packages){
         std::cout << pckgsctx.package_id << endl;
         for(SystemStruct sysctx: pckgsctx.package_systems){
             std::cout << '\t' << sysctx.system_id << endl;
             for(KeyValueStruct kvctx : sysctx.kv_system_params){
                std::cout << "\t\t" << kvctx.key << " " << kvctx.value << endl;
             }
             for(CompStruct compctx : sysctx.system_components){
                std::cout << "\t\t" << compctx.component_id << endl;
                for(InterfaceStruct ifacectx : compctx.comp_interfaces){
                   for(string sendmsgctx : ifacectx.send_msgs){
                      std::cout << "\t\t\t" << sendmsgctx << endl;
                   }
                   for(string recvmsgctx : ifacectx.recv_msgs){
                      std::cout << "\t\t\t" << recvmsgctx << endl;
                   }
                   for(string sendsrvcctx : ifacectx.send_srvcs){
                      std::cout << "\t\t\t" << sendsrvcctx << endl;
                   }
                   for(string recvsrvcctx : ifacectx.recv_srvcs){
                      std::cout << "\t\t\t" << recvsrvcctx << endl;
                   }
                   for(string sendactnsctx : ifacectx.send_actns){
                      std::cout << "\t\t\t" << sendactnsctx << endl;
                   }                   
                   for(string recvactnsctx : ifacectx.recv_actns){
                      std::cout << "\t\t\t" << recvactnsctx << endl;
                   }
                   for(string compconfctx : ifacectx.comp_conf){
                      std::cout << "\t\t\t" << compconfctx << endl;
                   }
                   for(string compstatectx : ifacectx.comp_state){
                      std::cout << "\t\t\t" << compstatectx << endl;
                   }
                   for(string diagsrvcctx : ifacectx.diag_srvcs){
                      std::cout << "\t\t\t" << diagsrvcctx << endl;
                   }
                } 
             }
             for(SystemConfig syscfgctx :sysctx.system_configs){
                std::cout << "\t\t" << syscfgctx.system_config_id << endl;   
             }
         }
    }
    std::cout.flush();
}

// Print the function definitions
void dynadlDataManager::printFunctions(void){
    std::cout << endl << "Printing Functions :" << endl << endl;    
    for(FunctionStruct funcctx : funcDefinitions){
        std::cout << "Function : " << funcctx.function_id << "()" << endl;
        for(StatementStruct stmt_struct : funcctx.func_statements){
            if(!stmt_struct.statement_id.empty()){
                if(!strcmp(stmt_struct.statement_id.c_str(),"declaration")){
                   std::cout << "\t";    
                }
                else{
                   std::cout << "\t" << stmt_struct.statement_id << " ";
                }
            }
            if(!stmt_struct.expression.empty()){
               std::cout << stmt_struct.expression << " " << endl;
            }
            if(!stmt_struct.var_decl.var_id.empty()){
                std::cout << stmt_struct.var_decl.var_id << " " << stmt_struct.var_decl.var_type << " " << stmt_struct.var_decl.var_value << endl;
            }
            for(CmdStruct cmd_struct : stmt_struct.func_cmds){
               std::cout << "\t\t" << cmd_struct.cmd_id << " ";
               for(VarStruct cmdargctx :cmd_struct.cmd_args){
                  std::cout << cmdargctx.var_id << " ";
               }
               std::cout << endl;
            }
        } 
    }
    std::cout.flush();
}

// Execute the function contents.
// We will first call main and the other calls to the other 
// functions will happen from main
void dynadlDataManager::executeRuntimeScript(string func_name){
    cout << endl;
    if(strcmp(func_name.c_str(),"main") != 0){
       std::cout << '\t';
    }
    std::cout << "Executing function " << func_name << "()" << endl << endl;
    bool skip_while = false;
    bool skip_if = false;
    int number_while_stmts = 0;
    bool function_found = false;
    vector<VarDeclStruct> var_decl_vector;
    VarDeclStruct while_var;
    int if_stmts = 0;
    bool in_ifelse = false;
    for(FunctionStruct funcctx : funcDefinitions){
        if(!strcmp(funcctx.function_id.c_str(), func_name.c_str())){
           function_found = true;
           for (auto stmt_struct = begin (funcctx.func_statements); stmt_struct != end (funcctx.func_statements); ++stmt_struct) {
               // If it is a statement
               if(!stmt_struct->statement_id.empty()){
                  if(!strcmp(stmt_struct->statement_id.c_str(),"declaration")){
                     std::cout << "\t\t";
                     if(skip_while || skip_if){
                        std::cout << "\t";
                     }
                     int i = 0;
                     for(VarDeclStruct vardeclctx : var_decl_vector){
                        if(!strcmp(vardeclctx.var_id.c_str(),stmt_struct->var_decl.var_id.c_str())){
                           std::cout << "Variable with the same name already declared: " << stmt_struct->var_decl.var_id << endl;
                           exit(1);
                        }
                        i++;
                     }
                     var_decl_vector.push_back(stmt_struct->var_decl);
                     // Print the variable type and id
                     if(!stmt_struct->var_decl.var_id.empty()){
                        std::cout << stmt_struct->var_decl.var_id << " " << stmt_struct->var_decl.var_type << " " << stmt_struct->var_decl.var_value << endl;
                     }
                  }
                  else{
                     // It is not a declaration then it must one of the statements
                     std::cout << "\t\t" << stmt_struct->statement_id << " ";
                     // If there is an expression, print it
                     if(!stmt_struct->expression.empty()){
                        std::cout << stmt_struct->expression << " " << endl;
                     }
                     if(!strcmp(stmt_struct->statement_id.c_str(),"while")){
                        if(skip_while || skip_if){
                           std::cout << "\t";
                        }
                       for(VarDeclStruct vardeclctx : var_decl_vector){
                           if(!strcmp(vardeclctx.var_id.c_str(),stmt_struct->expression.c_str())){
                              int v = stoi(vardeclctx.var_value);
                              if(v > 0){
                                 number_while_stmts = 0; 
                              }
                              else{
                                skip_while = true;
                              }
                              while_var = vardeclctx;
                              break;
                           }          
                       }              
                     }
                     else if(!strcmp(stmt_struct->statement_id.c_str(),"if")){
                        int i = 0;
                        if_stmts = stmt_struct->stmts_to_skip;
                        for(VarDeclStruct vardeclctx : var_decl_vector){
                            if(!strcmp(vardeclctx.var_id.c_str(),stmt_struct->expression.c_str())){
                               int v = stoi(vardeclctx.var_value);
                               if(v > 0){
                                  skip_if = false;
                               }
                               else{
                                 skip_if = true;
                               }
                               break;
                            }
                            i++;
                        }
                        if(skip_while || skip_if){
                           std::cout << "\t";
                        }
                     }
                     else if(!strcmp(stmt_struct->statement_id.c_str(),"ifelse")){
                        int i = 0;
                        if_stmts = stmt_struct->stmts_to_skip;
                        in_ifelse = true;
                        for(VarDeclStruct vardeclctx : var_decl_vector){
                            if(!strcmp(vardeclctx.var_id.c_str(),stmt_struct->expression.c_str())){
                               int v = stoi(vardeclctx.var_value);
                               if(v > 0){
                                  skip_if = false;
                               }
                               else{
                                /* Skip the if part and will just do the else statements */ 
                                int j = 0;
                                stmt_struct++; // Skip the current ifelse statement
                                 while(j < stmt_struct->stmts_to_skip){
                                    stmt_struct++;
                                    j++; 
                                 }
                               }
                               break;
                            }
                            i++;
                        }
                        if(skip_while || skip_if){
                           std::cout << "\t";
                        }
                     }
                     else if(!strcmp(stmt_struct->statement_id.c_str(),"inc")){
                        if(skip_while || skip_if){
                           std::cout << "\t";
                           continue;
                        }
                        number_while_stmts++;
                        int i = 0;
                        for(VarDeclStruct vardeclctx : var_decl_vector){
                            if(!strcmp(vardeclctx.var_id.c_str(),stmt_struct->expression.c_str())){
                               int v = stoi(vardeclctx.var_value);
                               v++;
                               vardeclctx.var_value = to_string(v);
                               var_decl_vector[i] = vardeclctx;
                               break;
                            }
                            i++;
                        }
                     }
                     else if(!strcmp(stmt_struct->statement_id.c_str(),"dec")){
                        if(skip_while || skip_if){
                           std::cout << "\t";
                           continue;
                        }
                        number_while_stmts++;
                        int i = 0;
                        for(VarDeclStruct vardeclctx : var_decl_vector){
                            if(!strcmp(vardeclctx.var_id.c_str(),stmt_struct->expression.c_str())){
                               int v = stoi(vardeclctx.var_value);
                               v--;
                               vardeclctx.var_value = to_string(v);
                               var_decl_vector[i] = vardeclctx;
                               break;
                            }
                            i++;
                        }
                     }
                     else if(!strcmp(stmt_struct->statement_id.c_str(),"endwhile")){
                       for(VarDeclStruct vardeclctx : var_decl_vector){
                           if(!strcmp(vardeclctx.var_id.c_str(),while_var.var_id.c_str())){
                              int v = stoi(vardeclctx.var_value);
                              if(v > 0){
                                 while(number_while_stmts > 0){
                                    stmt_struct--;    
                                    number_while_stmts--;
                                 } 
                              }
                              break;
                           }
                       }
                        std::cout << endl;
                     }
                     else if(!strcmp(stmt_struct->statement_id.c_str(),"endif")){
                        if(skip_if){
                           skip_if = false;
                        }
                        if(in_ifelse){
                           in_ifelse = false;
                        }
                        std::cout << endl;
                     }
                  }
               }
               // Execute the commands that are in the function, one at a time
               if(!skip_if && !skip_while){
                  if(stmt_struct->func_cmds.size() > 0){
                     executeCommands(&(*stmt_struct),&var_decl_vector);
                     number_while_stmts++;
                     if(if_stmts > 0){
                        if_stmts--;
                     }
                     if(in_ifelse && if_stmts == 0){
                        skip_if = true;
                     }
                  }
                  
               }
           }
        }
    }
    if(function_found == false){
        std::cout << endl << "Function " << func_name << " not defined!" << endl << endl;
    }
}

// Execute a single command as the StatementStruct contains only one command
// The var_decl_vector_ptr is used by the print command when the value of a variable needs to be printed 
void dynadlDataManager::executeCommands(StatementStruct *stmt_struct,vector<VarDeclStruct> *var_decl_vector_ptr){
               for(CmdStruct cmd_struct : stmt_struct->func_cmds){
                  std::cout << "\t\t" << cmd_struct.cmd_id << " ";
                  string cmd;
                  if(!strcmp(cmd_struct.cmd_id.c_str(),"exec")){
                     for(VarStruct cmdargctx :cmd_struct.cmd_args){
                         cmd += cmdargctx.var_id + " ";
                     }
                     std::cout << "\t\t";
                     system(cmd.c_str());
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"load")){
                     string cmd = selected_package + "/" + selected_system + "/";
                     vector<VarStruct> ::iterator var_it = cmd_struct.cmd_args.begin();
                     cmd += var_it->var_id + "/" + var_it->var_id + " &";
                     cout << cmd;
                     message_queue::remove(var_it->var_id.c_str());
                     message_queue *mq = new message_queue
                               (open_or_create
                                ,var_it->var_id.c_str()
                                ,10
                                ,sizeof(MyStruct)
                     );
                     // Remember which name is associated with whcih message queue pointer 
                     name_to_msgqueue_map[var_it->var_id] = mq;
                     system(cmd.c_str());
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"start")){
                      vector<VarStruct> ::iterator var_it = cmd_struct.cmd_args.begin();
                      cout << var_it->var_id << endl;
                      sendCmd(var_it->var_id,cmd_struct.cmd_id);
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"stop")){
                      vector<VarStruct> ::iterator var_it = cmd_struct.cmd_args.begin();
                      cout << var_it->var_id << endl;
                      sendCmd(var_it->var_id,cmd_struct.cmd_id);
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"save")){
                      vector<VarStruct> ::iterator var_it = cmd_struct.cmd_args.begin();
                      cout << var_it->var_id << endl;
                      sendCmd(var_it->var_id,cmd_struct.cmd_id);
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"restore")){
                      vector<VarStruct> ::iterator var_it = cmd_struct.cmd_args.begin();
                      cout << var_it->var_id << endl;
                      sendCmd(var_it->var_id,cmd_struct.cmd_id);
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"swap")){
                      vector<VarStruct> ::iterator var_it = cmd_struct.cmd_args.begin();
                      string first;
                      string second;
                      std::cout << var_it->var_id << " ";
                      first = var_it->var_id;
                      var_it++;
                      second = var_it->var_id;
                      std::cout << var_it->var_id << " ";
                      var_it++;
                      std::cout << var_it->var_id << " ";
                      if(!strcmp(var_it->var_id.c_str(),"at")){
                         var_it++;
                         std::cout << var_it->var_id << endl;
                         std::cout << "at option not implemented yet" << endl;
                      }
                      else{
                         std::cout << endl;
                         sendCmd(first,string("stop"));
                         sendCmd(second,string("start"));
                      }
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"call")){
                      vector<VarStruct> ::iterator var_it = cmd_struct.cmd_args.begin();
                      std::cout << var_it->var_id << endl;
                      executeRuntimeScript(var_it->var_id);
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"spawn")){
                      vector<VarStruct> ::iterator var_it = cmd_struct.cmd_args.begin();
                      std::cout << var_it->var_id << endl;
                      thread *th_spawn = new thread(spawnFunction,this ,var_it->var_id);
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"event")){
                      vector<VarStruct> ::iterator var_it = cmd_struct.cmd_args.begin();
                      std::cout << var_it->var_id << endl;
                      if(event_to_func_map.count(var_it->var_id) > 0){
                         executeRuntimeScript(event_to_func_map[var_it->var_id]);
                      }
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"wait")){
                      vector<VarStruct> ::iterator var_it = cmd_struct.cmd_args.begin();
                      string ev;
                      std::cout << var_it->var_id << " ";
                      ev = var_it->var_id;
                      var_it++;
                      std::cout << var_it->var_id << endl;
                      /* Store the event and its function in a map */
                      event_to_func_map[ev] = var_it->var_id;
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"unload")){
                      vector<VarStruct> ::iterator var_it = cmd_struct.cmd_args.begin();
                      std::cout << var_it->var_id << endl;
                      sendCmd(var_it->var_id,cmd_struct.cmd_id);
                      /* Delete the message queue as we do not need it any more */
                      delete(name_to_msgqueue_map[var_it->var_id]);
                      /* Delete the entry in the map */
                      name_to_msgqueue_map.erase(var_it->var_id);
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"log")){
                     string cmd = "echo ";
                     int i =0;
                     for(VarStruct cmdargctx :cmd_struct.cmd_args){
                         if(i==0){
                            cmd += cmdargctx.var_id + " >> ";
                         }
                         else{
                            cmd += cmdargctx.var_id;
                         }
                         i++;
                     }
                     system(cmd.c_str());
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"delay")){
                     vector<VarStruct> ::iterator var_it = cmd_struct.cmd_args.begin();
                     useconds_t useconds = (useconds_t) (stof(var_it->var_id) * 1000000.0);
                     usleep(useconds);
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"print")){
                     vector<VarStruct> ::iterator var_it = cmd_struct.cmd_args.begin();
                     if(!strcmp(var_it->var_type.c_str(),"string")){
                        std::cout << '\t' << '\t' << var_it->var_id << endl << "\t\t";
                     }
                     else if(!strcmp(var_it->var_type.c_str(),"identifier")){
                        int i = 0;
                        for(VarDeclStruct vardeclctx : *var_decl_vector_ptr){
                            if(!strcmp(vardeclctx.var_id.c_str(),var_it->var_id.c_str())){
                               std::cout << '\t' << '\t' << vardeclctx.var_id << " is " << vardeclctx.var_value << endl;
                               break;
                            }
                            i++;
                        }
                     }
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"select")){
                     int i =0;
                     string sysorpckg;
                     for(VarStruct cmdargctx :cmd_struct.cmd_args){
                         if(i==0){
                            sysorpckg = cmdargctx.var_id;
                         }
                         else{
                            if(!strcmp(sysorpckg.c_str(),"package")){
                               cout << "package " << cmdargctx.var_id;
                               selected_package = cmdargctx.var_id;
                            }
                            else if(!strcmp(sysorpckg.c_str(),"system")){
                               cout << "system " << cmdargctx.var_id; 
                               selected_system = cmdargctx.var_id;
                            }
                         }
                         i++;
                     }
                  }
                  else if(!strcmp(cmd_struct.cmd_id.c_str(),"timer")){
                     vector<VarStruct> ::iterator var_it = cmd_struct.cmd_args.begin();
                     bool periodic = false;
                     string delay;
                     std::cout << "\t\t" << var_it->var_id << " ";
                     if(!strcmp(var_it->var_id.c_str(),"periodic")){
                        periodic = true;
                     }
                     var_it++;
                     std::cout << var_it->var_id << " ";
                     delay = var_it->var_id;
                     var_it++;
                     std::cout << var_it->var_id << endl << "\t\t";
                     if(periodic == false){
                        /* For single timer just delay here */
                        useconds_t useconds = (useconds_t) (stof(delay) * 1000000.0);
                        usleep(useconds);
                        executeRuntimeScript(var_it->var_id);
                     }
                     else{
                        thread * th1 = new thread(startTimer,this,delay,var_it->var_id);
                     }
                  }
                  std::cout << endl;
               }
}

// We will send a command to a particular component through the Boost message queue class
void dynadlDataManager::sendCmd(string queue_name,string cmd){
    message_queue *mq = name_to_msgqueue_map[queue_name];
    MyStruct mystruct;
    mystruct.id = 1;
    strcpy(mystruct.cmd,cmd.c_str());
    if(mq){
         try{
            if(mq->try_send(&mystruct, sizeof(MyStruct), 0) == false){
               std::cout << "Could not send a struct" << std::endl;
            }
            else{
               std::cout << '\t' << '\t' << "msgid: " << mystruct.id << " msg: " << mystruct.cmd << " sent to " << queue_name << std::endl;
            }
         }
         catch(interprocess_exception &ex){
            std::cout << ex.what() << std::endl;
            return;
         }         
    }
}

// This function is called in its own thread
void dynadlDataManager::startTimer(dynadlDataManager *dataManager ,string delay, string func_name){
   useconds_t useconds = (useconds_t) (stof(delay) * 1000000.0);
   for(;;){
       dataManager->executeRuntimeScript(func_name);
       usleep(useconds);
   }
}

// This function is called in its own thread
void dynadlDataManager::spawnFunction(dynadlDataManager *dataManager ,string func_name){
   dataManager->executeRuntimeScript(func_name); 
}

// Interface files and executable code generation is done by this routine
// Posix is fully supported
// ROS is partially supported
// PX4 is not supported
void dynadlDataManager::generate(){
    std::cout << endl << "Generating Code:" << endl << endl;
    string current_folder;
    string current_file;
    std::ofstream *outfile;
    json j_diag;
    json j_config;
    json j_state;
    for(PackageStruct pckgsctx : packages){
         std::cout << pckgsctx.package_id << endl;
         current_folder = pckgsctx.package_id;
         mkdir(current_folder.c_str(),0777);
         for(SystemStruct sysctx: pckgsctx.package_systems){
             std::cout << "\t" << sysctx.system_id << endl;
             current_folder = pckgsctx.package_id + "/" + sysctx.system_id;
             mkdir(current_folder.c_str(),0777);
             for(CompStruct compctx : sysctx.system_components){
                std::cout << "\t\t" << compctx.component_id << endl;
                current_folder = pckgsctx.package_id + "/" + sysctx.system_id + "/" + compctx.component_id;
                mkdir(current_folder.c_str(),0777);
                for(InterfaceStruct ifacectx : compctx.comp_interfaces){
                   current_folder = pckgsctx.package_id + "/" + sysctx.system_id + "/" + compctx.component_id + "/" + ifacectx.iface_name;
                   mkdir(current_folder.c_str(),0777);
                   mkdir((current_folder + "/msg").c_str(),0777);
                   mkdir((current_folder + "/srv").c_str(),0777);
                   mkdir((current_folder + "/action").c_str(),0777);
                   mkdir((current_folder + "/config").c_str(),0777);
                   mkdir((current_folder + "/state").c_str(),0777);
                   mkdir((current_folder + "/diag").c_str(),0777);
                   for(string sendmsgctx : ifacectx.send_msgs){
                      std::cout << "\t\t\t" << "send " << sendmsgctx << endl;
                      current_file = current_folder + "/msg/" + sendmsgctx + ".msg.json";
                      outfile = new  ofstream(current_file);
                      json j;
                      int i = 0;
                      string text_string;
                      for( MsgStruct msgctx : msgDefinitions){
                          if(msgctx.msg_id == sendmsgctx){ 
                             j["id"] = msgctx.msg_id;
                             for(VarStruct msg_fld : msgctx.msg_fields){ 
                                std::cout << "\t\t\t\t" << msg_fld.var_type << " " << msg_fld.var_id << endl;
                                text_string += msg_fld.var_type + " " + msg_fld.var_id + "\n"; 
                                j["var" + to_string(i)] = {msg_fld.var_type, msg_fld.var_id};
                                i++;
                             }
                             *outfile << std::setw(4) << j << std::endl;
                          }
                      }
                      outfile->close();
                      if(platform_ == ROS){
                         current_file = current_folder + "/msg/" + sendmsgctx + ".msg";
                         outfile = new  ofstream(current_file);
                         *outfile << text_string;
                         outfile->close();
                      }
                   }
                   for(string recvmsgctx : ifacectx.recv_msgs){
                      std::cout << "\t\t\t" << "receive " << recvmsgctx << endl;
                      current_file = current_folder + "/msg/" + recvmsgctx + ".msg.json";
                      outfile = new  ofstream(current_file);
                      json j;
                      int i = 0;
                      string text_string;
                      for( MsgStruct msgctx : msgDefinitions){
                          if(msgctx.msg_id == recvmsgctx){
                             j["id"] = msgctx.msg_id;
                             for(VarStruct msg_fld : msgctx.msg_fields){ 
                                std::cout << "\t\t\t\t" << msg_fld.var_type << " " << msg_fld.var_id << endl;
                                text_string += msg_fld.var_type + " " + msg_fld.var_id + "\n";
                                j["var" + to_string(i)] = {msg_fld.var_type, msg_fld.var_id};
                                i++;
                             }
                             *outfile << std::setw(4) << j << std::endl;
                          }
                      }
                      outfile->close();
                      if(platform_ == ROS){
                         current_file = current_folder + "/msg/" + recvmsgctx + ".msg";
                         outfile = new  ofstream(current_file);
                         *outfile << text_string;
                         outfile->close();
                      }
                   }
                   for(string recvsrvcctx : ifacectx.recv_srvcs){
                      std::cout << "\t\t\t" << "receive " << recvsrvcctx << endl;
                      current_file = current_folder + "/srv/" + recvsrvcctx + ".srv.json";
                      outfile = new  ofstream(current_file);
                      json j;
                      int i = 0;
                      string text_string;
                      for( SrvcStruct srvcctx : srvcDefinitions){
                         if(srvcctx.srvc_id == recvsrvcctx){
                            j["id"] = srvcctx.srvc_id;
                            for(VarStruct srvc_fld : srvcctx.srvc_rqst_fields){
                                std::cout << "\t\t\t\tin " << srvc_fld.var_type << " " << srvc_fld.var_id << endl;
                                text_string += srvc_fld.var_type + " " + srvc_fld.var_id + "\n";
                                j["in"]["var" + to_string(i)] = {srvc_fld.var_type,srvc_fld.var_id};
                                i++;
                            }
                            text_string += "---\n";
                            for(VarStruct srvc_fld : srvcctx.srvc_rspnd_fields){
                               std::cout << "\t\t\t\tout " << srvc_fld.var_type << " " << srvc_fld.var_id << endl;
                               text_string += srvc_fld.var_type + " " + srvc_fld.var_id + "\n";
                               j["out"]["var" + to_string(i)] = {srvc_fld.var_type,srvc_fld.var_id};
                               i++;
                            }
                            *outfile << std::setw(4) << j << std::endl;
                         }
                      }
                      outfile->close();
                      if(platform_ == ROS){
                         current_file = current_folder + "/srv/" + recvsrvcctx + ".srv";
                         outfile = new  ofstream(current_file);
                         *outfile << text_string;
                         outfile->close();
                      }
                   }
                   for(string sendsrvcctx : ifacectx.send_srvcs){
                      std::cout << "\t\t\t" << "send " << sendsrvcctx << endl;
                      current_file = current_folder + "/srv/" + sendsrvcctx + ".srv.json";
                      outfile = new  ofstream(current_file);
                      json j;
                      int i = 0;
                      string text_string;
                      for( SrvcStruct srvcctx : srvcDefinitions){
                         if(srvcctx.srvc_id == sendsrvcctx){
                            j["id"] = srvcctx.srvc_id;
                            for(VarStruct srvc_fld : srvcctx.srvc_rqst_fields){
                                std::cout << "\t\t\t\tin " << srvc_fld.var_type << " " << srvc_fld.var_id << endl;
                                text_string += srvc_fld.var_type + " " + srvc_fld.var_id + "\n";
                                j["in"]["var" + to_string(i)] = {srvc_fld.var_type, srvc_fld.var_id};
                                i++;
                            }
                            text_string += "---\n";
                            for(VarStruct srvc_fld : srvcctx.srvc_rspnd_fields){
                               std::cout << "\t\t\t\tout " << srvc_fld.var_type << " " << srvc_fld.var_id << endl;
                               text_string += srvc_fld.var_type + " " + srvc_fld.var_id + "\n";
                               j["out"]["var" + to_string(i)] = {srvc_fld.var_type, srvc_fld.var_id};
                               i++;
                            }
                            *outfile << std::setw(4) << j << std::endl;
                         }
                      }
                      outfile->close();
                      if(platform_ == ROS){
                         current_file = current_folder + "/srv/" + sendsrvcctx + ".srv";
                         outfile = new  ofstream(current_file);
                         *outfile << text_string;
                         outfile->close();
                      }
                   }
                   for(string recvactnsctx : ifacectx.recv_actns){
                      std::cout << "\t\t\t" << "receive " << recvactnsctx << endl;
                      current_file = current_folder + "/action/" + recvactnsctx + ".action.json";
                      outfile = new  ofstream(current_file);
                      json j;
                      int i = 0;
                      string text_string;
                      for( ActnStruct actnctx : actnDefinitions){
                         if(actnctx.actn_id == recvactnsctx){
                            j["id"] = actnctx.actn_id;
                            for(VarStruct actn_fld : actnctx.actn_rqst_fields){
                               std::cout << "\t\t\t\tin " << actn_fld.var_type << " " << actn_fld.var_id << endl;
                               text_string += actn_fld.var_type + " " + actn_fld.var_id + "\n";
                               j["in"]["var" + to_string(i)] = {actn_fld.var_type,actn_fld.var_id};
                               i++;
                            }
                            text_string += "---\n";
                            for(VarStruct actn_fld : actnctx.actn_rspnd_fields){
                               std::cout << "\t\t\t\tout " << actn_fld.var_type << " " << actn_fld.var_id << endl;
                               text_string += actn_fld.var_type + " " + actn_fld.var_id + "\n";
                               j["out"]["var" + to_string(i)] = {actn_fld.var_type,actn_fld.var_id};
                               i++;
                            }
                            text_string += "---\n";
                            for(VarStruct actn_fld : actnctx.actn_updt_fields){
                               std::cout << "\t\t\t\tupdate " << actn_fld.var_type << " " << actn_fld.var_id << endl;
                               text_string += actn_fld.var_type + " " + actn_fld.var_id + "\n";
                               j["update"]["var" + to_string(i)] = {actn_fld.var_type,actn_fld.var_id};
                               i++;
                            }
                            *outfile << std::setw(4) << j << std::endl;
                         }
                      }
                      outfile->close();
                      if(platform_ == ROS){
                         current_file = current_folder + "/action/" + recvactnsctx + ".action";
                         outfile = new  ofstream(current_file);
                         *outfile << text_string;
                         outfile->close();
                      }
                   }
                   for(string sendactnsctx : ifacectx.send_actns){
                      std::cout << "\t\t\t" << "send " << sendactnsctx << endl;
                      current_file = current_folder + "/action/" + sendactnsctx + ".action.json";
                      outfile = new  ofstream(current_file);
                      json j;
                      int i = 0;
                      string text_string;
                      for( ActnStruct actnctx : actnDefinitions){
                         if(actnctx.actn_id == sendactnsctx){
                            j["id"] = actnctx.actn_id;
                            for(VarStruct actn_fld : actnctx.actn_rqst_fields){
                               std::cout << "\t\t\t\tin " << actn_fld.var_type << " " << actn_fld.var_id << endl;
                               text_string += actn_fld.var_type + " " + actn_fld.var_id + "\n";
                               j["in"]["var" + to_string(i)] = {actn_fld.var_type,actn_fld.var_id};
                               i++;
                            }
                            text_string += "---\n";
                            for(VarStruct actn_fld : actnctx.actn_rspnd_fields){
                               std::cout << "\t\t\t\tout " << actn_fld.var_type << " " << actn_fld.var_id << endl;
                               text_string += actn_fld.var_type + " " + actn_fld.var_id + "\n";
                               j["out"]["var" + to_string(i)] = {actn_fld.var_type,actn_fld.var_id};
                               i++;
                            }
                            text_string += "---\n";
                            for(VarStruct actn_fld : actnctx.actn_updt_fields){
                               std::cout << "\t\t\t\tupdate " << actn_fld.var_type << " " << actn_fld.var_id << endl;
                               text_string += actn_fld.var_type + " " + actn_fld.var_id + "\n";
                               j["update"]["var" + to_string(i)] = {actn_fld.var_type,actn_fld.var_id};
                               i++;
                            }
                            *outfile << std::setw(4) << j << std::endl;
                         }
                      }
                      outfile->close();
                      if(platform_ == ROS){
                         current_file = current_folder + "/action/" + sendactnsctx + ".action";
                         outfile = new  ofstream(current_file);
                         *outfile << text_string;
                         outfile->close();
                      }
                   }
                   for(string compconfctx : ifacectx.comp_conf){
                      std::cout << "\t\t\t" << compconfctx << endl;
                      current_file = current_folder + "/config/" + compconfctx + ".cfg.json";
                      outfile = new  ofstream(current_file);
                      int i = 0;
                      for( CompConfiguration compconfigctx : compConfigDefinitions){
                         if(compconfigctx.config_id == compconfctx){
                            j_config["id"] = compconfigctx.config_id;
                            for(VarDeclStruct cfg_fld : compconfigctx.config_fields){
                               std::cout << "\t\t\t\t" << cfg_fld.var_type << " " << cfg_fld.var_id << " " << cfg_fld.var_value << endl;
                               j_config["var" + to_string(i)] = {cfg_fld.var_type,cfg_fld.var_id,cfg_fld.var_value};
                               i++;
                            }
                            *outfile << std::setw(4) << j_config << std::endl;
                         }
                      }
                      outfile->close();
                   }
                   for(string statectx : ifacectx.comp_state){
                      std::cout << "\t\t\t" << statectx << endl;
                      current_file = current_folder + "/state/" + statectx + ".sta.json";
                      outfile = new  ofstream(current_file);
                      int i = 0;
                      for( CompState compstatectx : compStateDefinitions){
                           if(compstatectx.state_id == statectx){
                              j_state["id"] = compstatectx.state_id;
                              for(VarStruct state_fld : compstatectx.state_fields){
                                 std::cout << "\t\t\t\t" << state_fld.var_type << " " << state_fld.var_id << endl;
                                 j_state["var" + to_string(i)] = {state_fld.var_type,state_fld.var_id};
                                 i++;
                              }
                              *outfile << std::setw(4) << j_state << std::endl;
                           }
                      }
                      outfile->close();
                   }
                   for(string diagsrvcctx : ifacectx.diag_srvcs){
                      std::cout << "\t\t\t" << diagsrvcctx << endl;
                      current_file = current_folder + "/diag/" + diagsrvcctx + ".diag.json";
                      outfile = new  ofstream(current_file);
                      int i = 0;
                      for( DiagStruct diagctx : diagDefinitions){
                        if(diagsrvcctx == diagctx.diag_id){
                           j_diag["id"] = diagctx.diag_id;
                           for(VarStruct diag_fld : diagctx.diag_rqst_fields){
                              std::cout << "\t\t\t\tin " << diag_fld.var_type << " " << diag_fld.var_id << endl;
                              j_diag["in"]["var" + to_string(i)] = {diag_fld.var_type, diag_fld.var_id};
                              i++;
                           }
                           for(VarStruct diag_fld : diagctx.diag_rspnd_fields){
                               std::cout << "\t\t\t\tout " << diag_fld.var_type << " " << diag_fld.var_id << endl;
                              j_diag["out"]["var" + to_string(i)] = {diag_fld.var_type,diag_fld.var_id};
                              i++;
                           }
                           *outfile << std::setw(4) << j_diag << std::endl;
                        }
                      }
                      outfile->close();
                   }
                }
                // Call for each component to generate their own C++, headers and a Makefile
                current_folder = pckgsctx.package_id + "/" + sysctx.system_id + "/" + compctx.component_id;
                generateCode(current_folder,compctx, j_config,j_state, j_diag);
             }
         }
    }
}

// Generates code for each component
void dynadlDataManager::generateCode(string current_folder,CompStruct compctx, json j_config, json j_state, json j_diag){
                // Generate code for each component
                string current_file;
                std::ofstream *outfile;

                // Generate the main file for the component
                // This file has the name of the component
                current_file = current_folder + "/" + compctx.component_id + ".cpp";
                outfile = new  ofstream(current_file);
                *outfile << "//Compile: g++ -std=c++11 -pthread " << compctx.component_id + ".cpp" + " -o " + compctx.component_id << endl << endl;
                *outfile << "#include <iostream>" << endl;
                *outfile << "#include <thread>" << endl;
                *outfile << "#include <unistd.h>" << endl;
                *outfile << "#include \"state.h\"" << endl;
                *outfile << "#include \"diagnostics.h\"" << endl;
                *outfile << "using namespace std;" << endl << endl;
                *outfile <<  endl << endl;
                *outfile <<  "void config(void);" << endl;
                *outfile <<  "void state(void);" << endl << endl;
                *outfile << "int main(int argc, char *argv[])" << endl;
                *outfile <<  "{" << endl;
                *outfile <<  "\tstd::cout << \"Starting " << compctx.component_id << "\"" << "<< endl;" << endl;
                *outfile <<  "\tconfig();" << endl;
                *outfile <<  "\tthread th1(diag,argv[0]);" << endl;
                *outfile <<  "\tfor(;;){" << endl;
                *outfile <<  "\t\tsleep(1);" << endl;
                *outfile <<  "\t}" << endl;
                *outfile <<  "\treturn 0;" << endl;
                *outfile <<  "}" << endl;
                outfile->close();

                // Generate all types that the grammar supports
                current_file = current_folder + "/" + "datatypes.h";
                outfile = new  ofstream(current_file);
                *outfile << "#ifndef DATATYPES_H" << endl;
                *outfile << "#define DATATYPES_H" << endl << endl;
                *outfile << "typedef int int16;" << endl;
                *outfile << "typedef unsigned int uint16;" << endl;
                *outfile << "typedef int int32;" << endl;
                *outfile << "typedef unsigned int uint32;" << endl;
                *outfile << "typedef int int64;" << endl;
                *outfile << "typedef unsigned int uint64;" << endl;
                *outfile << "typedef float float32;" << endl;
                *outfile << "typedef double float64;" << endl;
                *outfile << "typedef char byte;" << endl;
                *outfile << "typedef char binary;" << endl << endl;
                *outfile << "struct MyStruct{" << endl;
                *outfile << "int id;" << endl;
                *outfile << "char cmd[20];" << endl;
                *outfile << "};" << endl;
                *outfile << "#endif" << endl;
                outfile->close();

                // Generate diagnostics.h with the headers of all diagnostics routines 
                current_file = current_folder + "/" + "diagnostics" + ".h";
                outfile = new  ofstream(current_file);
                *outfile << "#ifndef DIAGNOSTICS_H" << endl;
                *outfile << "#define DIAGNOSTICS_H" << endl << endl;
                *outfile << "void d_unload(void);" << endl;
                *outfile << "void d_start(void);" << endl;
                *outfile << "void d_stop(void);" << endl;
                *outfile << "void d_save(void);" << endl;
                *outfile << "void d_restore(void);" << endl;
                *outfile << "bool get_start(void);" << endl << endl;
                *outfile << "void diag(std::string component_id);" << endl << endl;
                *outfile << "#endif" << endl;
                outfile->close();

                // Generate diagnostics.cpp which will be run in a separate thread
                current_file = current_folder + "/" + "diagnostics" + ".cpp";
                outfile = new  ofstream(current_file);
                *outfile << "#include <iostream>" << endl;
                *outfile << "#include <thread>" << endl;
                *outfile << "#include <mutex>" << endl;
                *outfile << "#include <unistd.h>" << endl;
                *outfile << "#include <boost/interprocess/detail/config_begin.hpp>" << endl;
                *outfile << "#include <boost/interprocess/ipc/message_queue.hpp>" << endl;
                *outfile << "#include \"datatypes.h\"" << endl;
                *outfile << "#include \"state.h\"" << endl;
                *outfile << "using namespace boost::interprocess;" << endl;
                *outfile << "using namespace std;" << endl << endl;
                *outfile << "mutex m;" << endl;
                *outfile << "bool can_start=false;" << endl << endl;
                string diag_struct_name_in;
                string diag_struct_name_out;
                json::iterator itn = j_diag.begin();
                string s = *(itn.value().begin());
                diag_struct_name_in = s + "_in";
                diag_struct_name_out = s + "_out"; 
                *outfile << "struct " << diag_struct_name_in << "{" << endl;
                for (json::iterator it = j_diag["in"].begin(); it != j_diag["in"].end(); ++it) {
                     json::iterator it2 = it.value().begin(); 
                     string s = *it2;
                     s.erase(remove( s.begin(), s.end(), '\"' ),s.end());
                     if(distance(it.value().begin(), it.value().end()) > 1){
                        *outfile << "\t" << s << " ";
                        it2++;
                        s = *it2;
                        s.erase(remove( s.begin(), s.end(), '\"' ),s.end());
                        *outfile <<  s << ";" << endl;
                     }
                }
                *outfile << "};" << endl << endl;
                *outfile << "struct " << diag_struct_name_out << "{" << endl;
                for (json::iterator it = j_diag["out"].begin(); it != j_diag["out"].end(); ++it) {
                     json::iterator it2 = it.value().begin(); 
                     string s = *it2;
                     s.erase(remove( s.begin(), s.end(), '\"' ),s.end());
                     if(distance(it.value().begin(), it.value().end()) > 1){
                        *outfile << "\t" << s << " ";
                        it2++;
                        s = *it2;
                        s.erase(remove( s.begin(), s.end(), '\"' ),s.end());
                        *outfile <<  s << ";" << endl;
                     }
                }
                *outfile << "};" << endl << endl;
                *outfile <<  "bool get_start(void)" << endl;
                *outfile <<  "{" << endl;
                *outfile << "\tlock_guard<mutex> guard(m);" << endl;
                *outfile << "\treturn can_start;" << endl;
                *outfile <<  "}" << endl << endl;
                *outfile <<  "void start(void)" << endl;
                *outfile <<  "{" << endl;
                *outfile << "\tlock_guard<mutex> guard(m);" << endl;
                *outfile << "\tcan_start = true;" << endl;
                *outfile <<  "}" << endl << endl;
                *outfile <<  "void stop(void)" << endl;
                *outfile <<  "{" << endl;
                *outfile << "\tlock_guard<mutex> guard(m);" << endl;
                *outfile << "\tcan_start = false;" << endl;
                *outfile <<  "}" << endl << endl;
                *outfile <<  "void d_unload(void)" << endl;
                *outfile <<  "{" << endl;
                *outfile <<  "std::cout << endl << endl;" << endl;
                *outfile <<  "\tflush(std::cout);" << endl;
                *outfile <<  "\texit(0);" << endl;
                *outfile <<  "}" << endl << endl;
                *outfile <<  "void d_start(void)" << endl;
                *outfile <<  "{" << endl;
                *outfile <<  "\tstart();" << endl;
                *outfile <<  "}" << endl << endl;
                *outfile <<  "void d_stop(void)" << endl;
                *outfile <<  "{" << endl;
                *outfile <<  "\tstop();" << endl;
                *outfile <<  "}" << endl << endl;
                *outfile <<  "void d_save(void)" << endl;
                *outfile <<  "{" << endl;
                *outfile <<  "\tsave();" << endl;
                *outfile <<  "}" << endl << endl;
                *outfile <<  "void d_restore(void)" << endl;
                *outfile <<  "{" << endl;
                *outfile <<  "\trestore();" << endl;
                *outfile <<  "}" << endl << endl;
                *outfile <<  "void diag(string component_id)" << endl;
                *outfile <<  "{" << endl;
                *outfile << "\t" << diag_struct_name_in << " diag_struct_in;" << endl;
                *outfile << "\t" << diag_struct_name_out << " diag_struct_out;" << endl;
                *outfile << "\t" << "MyStruct mystruct;" << endl;
                *outfile << "\ttry{" << endl;
                *outfile << "\t\t//Open a message queue."  << endl;
                *outfile << "\t\tmessage_queue mq"  << endl;
                *outfile << "\t\t(open_or_create"  << endl;
                *outfile << "\t\t," << "\"" << compctx.component_id << "\"" << endl;
                *outfile << "\t\t,10"              << endl; 
                *outfile << "\t\t,sizeof(MyStruct)"  << endl;
                *outfile << "\t\t);"                  << endl;
                *outfile << "\t\tunsigned int priority;"  << endl;
                *outfile << "\t\tmessage_queue::size_type recvd_size;"  << endl;
                *outfile << "\t\t//Receive some messages"  << endl;
                *outfile << "\t\tfor(;;){"  << endl;
                *outfile << "\t\t\tmq.receive(&mystruct, sizeof(MyStruct), recvd_size, priority);"  << endl;
                *outfile << "\t\t\tif(recvd_size != sizeof(MyStruct)){"  << endl;
                *outfile << "\t\t\t\tstd::cout << \"Received unexpected length\" << std::endl;" << endl;
                *outfile << "\t\t\t\tcontinue;" << endl;
                *outfile << "\t\t\t}"   << endl;
                *outfile << "\t\t\tstd::cout << \"" << compctx.component_id << "\" << \":: msgid:  \" << mystruct.id << \" msg:\" << mystruct.cmd << \" rcvd\" << std::endl;"  << endl;
                *outfile << "\t\t\tif(!strcmp(mystruct.cmd,\"start\")){" << endl;
				    *outfile << "\t\t\t\td_start();" << endl;
			       *outfile << "\t\t\t}" << endl;
			       *outfile << "\t\t\telse if(!strcmp(mystruct.cmd,\"stop\")){" << endl;
				    *outfile << "\t\t\t\td_stop();" << endl;
			       *outfile << "\t\t\t}" << endl;
			       *outfile << "\t\t\telse if(!strcmp(mystruct.cmd,\"unload\")){" << endl;
				    *outfile << "\t\t\t\td_unload();" << endl;
			       *outfile << "\t\t\t}" << endl;
			       *outfile << "\t\t\telse if(!strcmp(mystruct.cmd,\"save\")){" << endl;
				    *outfile << "\t\t\t\td_save();" << endl;
			       *outfile << "\t\t\t}" << endl;
			       *outfile << "\t\t\telse if(!strcmp(mystruct.cmd,\"restore\")){" << endl;
				    *outfile << "\t\t\t\td_restore();" << endl;
			       *outfile << "\t\t\t}" << endl;
			       *outfile << "\t\t\telse{" << endl;
				    *outfile << "\t\t\t\tstd::cout << \"Unknown command\" << endl;" << endl;
			       *outfile << "\t\t\t}" << endl;
                *outfile << "\t\t}" << endl;
                *outfile << "\t}"  << endl;
                *outfile << "\tcatch(interprocess_exception &ex){"  << endl;
                *outfile << "\t\tmessage_queue::remove(\"send_queue\");"  << endl;
                *outfile << "\t\tstd::cout << ex.what() << std::endl;"  << endl;
                *outfile << "\t\treturn;"  << endl;
                *outfile << "\t}"  << endl;
                *outfile << "\tmessage_queue::remove(\"send_queue\");"  << endl;
                *outfile << "\treturn;"   << endl;
                *outfile << "}" << endl;
                *outfile << "#include <boost/interprocess/detail/config_end.hpp>" << endl; 
                outfile->close();


                // Generate config.cpp used to read the component's configuration parameters at startup
                current_file = current_folder + "/" + "config" + ".cpp";
                outfile = new  ofstream(current_file);
                *outfile << "#include <iostream>" << endl;
                *outfile << "#include <unistd.h>" << endl;
                *outfile << "#include \"datatypes.h\"" << endl;
                *outfile << "using namespace std;" << endl << endl;
                string conf_struct_name;
                string cfg_string;
                for (json::iterator it = j_config.begin(); it != j_config.end(); ++it) {
                     json::iterator it2 = it.value().begin();
                     string s = *it2;
                     s.erase(remove( s.begin(), s.end(), '\"' ),s.end());
                     if(distance(it.value().begin(), it.value().end()) > 1){
                        bool is_string = false;
                        *outfile << "\t" << s << " ";
                        if(!strcmp(s.c_str(),"string")){
                           is_string = true;
                        }
                        it2++;
                        s = *it2;
                        s.erase(remove( s.begin(), s.end(), '\"' ),s.end());
                        *outfile <<  s << " = ";
                        cfg_string += "\tstd::cout << '\t' << cfg_struct.";
                        cfg_string += s + " << endl; \n";
                        it2++;
                        s = *it2;
                        if(!is_string){
                           s.erase(remove( s.begin(), s.end(), '\"' ),s.end());
                        }
                        *outfile <<  s << ";" << endl; 
                     }
                     else{
                        conf_struct_name = s;
                        *outfile << "struct " << s << "{" << endl;
                     }
                }
                *outfile << "}cfg_struct;" << endl << endl;
                *outfile << conf_struct_name << "* get_config_struct(void){" << endl;
                *outfile << "\treturn &cfg_struct;" << endl;
                *outfile << "}" << endl << endl; 
                *outfile <<  "void config(void)" << endl;
                *outfile <<  "{" << endl;
                *outfile << "\t" << "std::cout << \"" << compctx.component_id << "\" <<  \":: Configuration handling routine\" << endl;" << endl;
                *outfile << "\tstd::cout << \"" << compctx.component_id << "\" << \":: Configuraton parameters:\" << endl;" << endl;
                *outfile << cfg_string;
                *outfile <<  "}" << endl;
                outfile->close();

                // Generate state.h with all state related routines
                current_file = current_folder + "/" + "state" + ".h";
                outfile = new  ofstream(current_file);
                *outfile << "#ifndef STATE_H" << endl;
                *outfile << "#define STATE_H" << endl << endl;
                *outfile << "void save(void);" << endl;
                *outfile << "void restore(void);" << endl << endl;
                *outfile << "#endif" << endl;
                outfile->close();

                // Generate state.cpp that will save and restore the state of the component 
                current_file = current_folder + "/" + "state" + ".cpp";
                outfile = new  ofstream(current_file);
                *outfile << "#include <iostream>" << endl;
                *outfile << "#include <unistd.h>" << endl;
                *outfile << "#include <fstream>" << endl;
                *outfile << "#include <bitsery/bitsery.h>" << endl;
                *outfile << "#include <bitsery/adapter/stream.h>" << endl;
                *outfile << "#include <bitsery/traits/string.h>"  << endl;
                *outfile << "#include <string.h>"  << endl;
                *outfile << "#include \"datatypes.h\"" << endl;
                *outfile << "using namespace std;" << endl << endl;
                string state_struct_name;
                string ser_string;
                string des_string;
                bool is_string = false;
                for (json::iterator it = j_state.begin(); it != j_state.end(); ++it) {
                     json::iterator it2 = it.value().begin();
                     string s = *it2;
                     s.erase(remove( s.begin(), s.end(), '\"' ),s.end());
                     if(distance(it.value().begin(), it.value().end()) > 1){
                        *outfile << "\t" << s << " ";
                        ser_string += "\tser_ptr->";
                        des_string += "\tdes_ptr->";
                        if(!strcmp(s.c_str(),"char") || !strcmp(s.c_str(),"byte")){
                           ser_string += "value1b(state_struct.";
                           des_string += "value1b(state_struct.";
                        }
                        else if(!strcmp(s.c_str(),"int16") || !strcmp(s.c_str(),"uint16")){
                           ser_string += "value2b(state_struct.";
                           des_string += "value2b(state_struct.";
                        }
                        else if(!strcmp(s.c_str(),"int32") || !strcmp(s.c_str(),"uint32") || !strcmp(s.c_str(),"float32")){
                           ser_string += "value4b(state_struct.";
                           des_string += "value4b(state_struct.";
                        }
                        else if(!strcmp(s.c_str(),"int64") || !strcmp(s.c_str(),"uint64") || !strcmp(s.c_str(),"float64")  || !strcmp(s.c_str(),"double") ){
                           ser_string += "value8b(state_struct.";
                           des_string += "value8b(state_struct.";
                        }
                        else if(!strcmp(s.c_str(),"string") || !strcmp(s.c_str(),"binary")){
                           ser_string += "text1b(state_struct.";
                           des_string += "text1b(state_struct.";
                           is_string = true;
                        }
                        it2++;
                        s = *it2;
                        s.erase(remove( s.begin(), s.end(), '\"' ),s.end());
                        *outfile <<  s << ";" << endl;
                        if(is_string){
                           ser_string += s + ",state_struct." + s + ".length());\n";
                           des_string += s + ",state_struct." + s + ".length());\n";
                           is_string = false;
                        }
                        else{
                           ser_string += s + ");\n";
                           des_string += s + ");\n";
                        }
                     }
                     else{
                        state_struct_name = s;
                        *outfile << "struct " << s << "{" << endl;
                     }
                }
                *outfile << "}state_struct;" << endl << endl;
                *outfile << "auto ser_file_name = \"ser_" + compctx.component_id + ".bin\";" << endl;
                *outfile << "std::fstream * s_ptr;" << endl;
                *outfile << "bitsery::Serializer<bitsery::OutputBufferedStreamAdapter> * ser_ptr;" << endl;
                *outfile << "bitsery::Deserializer<bitsery::InputStreamAdapter> *des_ptr;" << endl << endl;
                *outfile <<  "void save(void)" << endl;
                *outfile <<  "{" << endl;
                *outfile <<  "\ts_ptr = new std::fstream {ser_file_name, s_ptr->binary | s_ptr->trunc | s_ptr->out};" << endl;
                *outfile <<  "\tser_ptr = new bitsery::Serializer<bitsery::OutputBufferedStreamAdapter> (*s_ptr);" << endl;
                *outfile <<  ser_string;
                *outfile <<  "\tser_ptr->adapter().flush();" << endl;
                *outfile <<  "\ts_ptr->close();" << endl;
                *outfile <<  "\tdelete(ser_ptr);" << endl;
                *outfile <<  "\tdelete(s_ptr);" << endl;
                *outfile <<  "}" << endl << endl;
                *outfile <<  "void restore(void)" << endl;
                *outfile <<  "{" << endl;
                *outfile <<  "\ts_ptr = new std::fstream {ser_file_name, s_ptr->binary | s_ptr->in};" << endl;
                *outfile <<  "\tdes_ptr = new bitsery::Deserializer<bitsery::InputStreamAdapter>(*s_ptr);" << endl;
                *outfile <<  des_string;
                *outfile <<  "\ts_ptr->close();" << endl;
                *outfile <<  "\tdelete(des_ptr);" << endl;
                *outfile <<  "\tdelete(s_ptr);" << endl;
                *outfile <<  "}" << endl << endl;
                *outfile <<  "void state(void)" << endl;
                *outfile <<  "{" << endl;
                *outfile << "\t" << "std::cout << \"State handling routine\" << endl;" << endl;
                *outfile <<  "}" << endl;
                outfile->close();

                // Generate the Makefile that will be used to build the generated .cpp and .h files 
                current_file = current_folder + "/" + "Makefile";
                outfile = new  ofstream(current_file);
                *outfile << "CC=g++" << endl;
                *outfile << "LDFLAGS := $(LDFLAGS)  -lpthread -lrt" << endl << endl;
                *outfile << "all: " << compctx.component_id << endl << endl;
                *outfile << compctx.component_id << ": diagnostics.cpp config.cpp state.cpp " << compctx.component_id << ".cpp state.h datatypes.h" << endl << endl;
                *outfile << "\t" << "$(CC) -c diagnostics.cpp -o diagnostics.o" << endl;
                *outfile << "\t" << "$(CC) -c config.cpp -o config.o" << endl;
                *outfile << "\t" << "$(CC) -c state.cpp -o state.o" << endl;
                *outfile << "\t" << "$(CC) -c " << compctx.component_id << ".cpp" << " -o " << compctx.component_id << ".o" << endl;
                *outfile << "\t" << "$(CC) diagnostics.o config.o state.o " << compctx.component_id << ".o $(LDFLAGS) " << "-o " << compctx.component_id << endl << endl;
                *outfile << "clean:" << endl;
                *outfile << "\t" << "rm -rf *.o component11" << endl;
                outfile->close();
                
                // Build the generated code for the component with the help of the generated Makefile
                std::cout << endl << "Building generated code for component " << compctx.component_id << ":" << endl << endl;
                string mk_cmd = "make -C " + current_folder;
                system(mk_cmd.c_str());
                std::cout << endl;
}



// The following routines are called by the listener class while the script is parsed 
// This populates data structures that can be used later in the code genration and runtime

void dynadlDataManager::addPackage(string pckg_id){
    PackageStruct pckgStruct;
    pckgStruct.package_id = pckg_id;
    packages.push_back(pckgStruct);
}

void dynadlDataManager::addSystem(string pckg_id,string sys_id){
   vector<PackageStruct> :: iterator pckg_it = packages.begin();
   while(pckg_it != packages.end()){
       if(pckg_it->package_id == pckg_id){
           SystemStruct systemStruct;
           systemStruct.system_id = sys_id;
           pckg_it->package_systems.push_back(systemStruct);
           break;
       }
       pckg_it++;
   }
}

void dynadlDataManager::addKeyValuePackage(string pckg_id, KeyValueStruct kv_pair){
   vector<PackageStruct> :: iterator pckg_it = packages.begin();
   while(pckg_it != packages.end()){
       if(pckg_it->package_id == pckg_id){

           pckg_it->kv_package_params.push_back(kv_pair);
           break;
       }
       pckg_it++;
   }
}

void dynadlDataManager::addKeyValueSystem(string pckg_id, string sys_id, KeyValueStruct kv_pair){
   vector<PackageStruct> :: iterator pckg_it = packages.begin();
   while(pckg_it != packages.end()){
       if(pckg_it->package_id == pckg_id){
           vector<SystemStruct> :: iterator sys_it = pckg_it->package_systems.begin();
           while(sys_it != pckg_it->package_systems.end()){
               if(sys_it->system_id == sys_id){
                   sys_it->kv_system_params.push_back(kv_pair);
                   break;        
               }
               sys_it++;
           }
       }
       pckg_it++;
   }
}


void dynadlDataManager::addComponent(string pckg_id,string sys_id,CompStruct comp){
  vector<PackageStruct> :: iterator pckg_it = packages.begin();
   while(pckg_it != packages.end()){
       if(pckg_it->package_id == pckg_id){
           vector<SystemStruct> :: iterator sys_it = pckg_it->package_systems.begin();
           while(sys_it != pckg_it->package_systems.end()){
               if(sys_it->system_id == sys_id){
                   sys_it->system_components.push_back(comp);
                   break;
               }
               sys_it++;
           }
       }
       pckg_it++;
   }
}

void dynadlDataManager::addSysConfig(string pckg_id,string sys_id,SystemConfig sys_cfg){
  vector<PackageStruct> :: iterator pckg_it = packages.begin();
   while(pckg_it != packages.end()){
       if(pckg_it->package_id == pckg_id){
           vector<SystemStruct> :: iterator sys_it = pckg_it->package_systems.begin();
           while(sys_it != pckg_it->package_systems.end()){
               if(sys_it->system_id == sys_id){
                   sys_it->system_configs.push_back(sys_cfg);
                   break;        
               }
               sys_it++;
           }
       }
       pckg_it++;
   }
}

void dynadlDataManager::addMsgDefinition(MsgStruct msg_struct){
    msgDefinitions.push_back(msg_struct);
}

void dynadlDataManager::addSrvcDefinition(SrvcStruct srvc_struct){
    srvcDefinitions.push_back(srvc_struct);
}

void dynadlDataManager::addActnDefinition(ActnStruct actn_struct){
   actnDefinitions.push_back(actn_struct);
}

void dynadlDataManager::addCompConfigDefinition(CompConfiguration comp_config_struct){
    compConfigDefinitions.push_back(comp_config_struct);
}

void dynadlDataManager::addCompStateDefinition(CompState comp_state){
    compStateDefinitions.push_back(comp_state);
}

void dynadlDataManager::addDiagDefinition(DiagStruct diag_struct){
    diagDefinitions.push_back(diag_struct);
}

void dynadlDataManager::addFuncDefinition(FunctionStruct func_struct){
    funcDefinitions.push_back(func_struct);
}

void dynadlDataManager::addStatementDefinition(string func_id,StatementStruct stmt_struct){
    int i = 0;
    for(FunctionStruct funcdefctx :funcDefinitions){
        if(funcdefctx.function_id == func_id){
            funcDefinitions[i].func_statements.push_back(stmt_struct);
        }
        i++; 
    }
}

#include <boost/interprocess/detail/config_end.hpp>