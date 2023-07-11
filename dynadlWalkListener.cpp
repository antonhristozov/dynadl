/*
 * ExprInterface.cpp
 *
 *  Created on: Jan 4, 2023
 *      Author: anton
 */

#include <iostream>
#include <fstream>  
#include "dynadlWalkListener.h"
#include "dataTypes.h"

  // Constructor of the listener class
  dynadlWalkListener :: dynadlWalkListener(Platforms platform){
      in_function_definition = false;
      current_function = 0;
      extra_tab = false;
      platform_ = platform;
      dataManager = new dynadlDataManager(platform);
  }

  void dynadlWalkListener :: enterProg(dynadlParser::ProgContext * /*ctx*/){

  }

  void dynadlWalkListener :: exitProg(dynadlParser::ProgContext * /*ctx*/){

  }


  void dynadlWalkListener :: enterPackages(dynadlParser::PackagesContext * /*ctx*/){

  }

  void dynadlWalkListener :: exitPackages(dynadlParser::PackagesContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterMain_function_definition(dynadlParser::Main_function_definitionContext * /*ctx*/){
         cout << endl << "Enter function main"  << endl;
         current_func_struct.func_statements.clear();
         current_func_struct.function_id = "main";
         dataManager->addFuncDefinition(current_func_struct);
         in_function_definition = true;
  }

  void dynadlWalkListener :: exitMain_function_definition(dynadlParser::Main_function_definitionContext * /*ctx*/){
         cout << "Exit function main" << endl;
         in_function_definition = false;
         current_function++;
  }

  void dynadlWalkListener :: enterPckg(dynadlParser::PckgContext * ctx)  { 
       cout << endl << "Package ";
       current_package_id = ctx->IDENTIFIER()->getSymbol()->getText();
       cout << current_package_id << endl;
       dataManager->addPackage(current_package_id);
       /* Extract key value pairs that pertain to the package */
       KeyValueStruct kv_pair;
       for(dynadlParser::KvContext * kvctx : ctx->kv()){
           if(kvctx->key()->key_type()->KEY_ID()){
              cout << '\t' << kvctx->key()->key_type()->KEY_ID()->getSymbol()->getText() << " ";
              kv_pair.key = kvctx->key()->key_type()->KEY_ID()->getSymbol()->getText();
           }
           else if(kvctx->key()->key_type()->KEY_DESC()){
              cout << '\t' << kvctx->key()->key_type()->KEY_DESC()->getSymbol()->getText() << " ";
              kv_pair.key = kvctx->key()->key_type()->KEY_DESC()->getSymbol()->getText();
           }
           else if(kvctx->key()->key_type()->KEY_COMMENT()){
              cout << '\t' << kvctx->key()->key_type()->KEY_COMMENT()->getSymbol()->getText() << " ";
              kv_pair.key = kvctx->key()->key_type()->KEY_COMMENT()->getSymbol()->getText();
           }
           if(kvctx->value()->integer()){
              cout << '\t' << kvctx->value()->integer()->INTEGER()->getSymbol()->getText() << endl;
              kv_pair.value = kvctx->value()->integer()->INTEGER()->getSymbol()->getText();
           }
           else if(kvctx->value()->realnum()){
              cout << '\t' << kvctx->value()->realnum()->REAL()->getSymbol()->getText() << endl;
              kv_pair.value = kvctx->value()->realnum()->REAL()->getSymbol()->getText();
           }
           else if(kvctx->value()->stringliteral()){
              cout << '\t' << kvctx->value()->stringliteral()->STRINGLITERAL()->getSymbol()->getText() << endl;
              kv_pair.value = kvctx->value()->stringliteral()->STRINGLITERAL()->getSymbol()->getText();
           }
           dataManager->addKeyValuePackage(current_package_id, kv_pair);
       }
       /* Iterate through the systems that are part of the package */
       for(dynadlParser::SystemContext * sysctx : ctx->system()){
          if(sysctx->IDENTIFIER()){
             current_system_id = sysctx->IDENTIFIER()->getSymbol()->getText();
             dataManager->addSystem(current_package_id,current_system_id);
             cout << '\t' << current_system_id << endl;
          }
          /* Extract key value pairs that pertain to the system */
          for(dynadlParser::KvContext * syskvctx : sysctx->kv()){
             if(syskvctx->key()->key_type()->KEY_ID()){
                cout << '\t' << '\t' << syskvctx->key()->key_type()->KEY_ID()->getSymbol()->getText() << " ";
                kv_pair.key = syskvctx->key()->key_type()->KEY_ID()->getSymbol()->getText();
             }
             else if(syskvctx->key()->key_type()->KEY_DESC()){
                cout << '\t' <<  '\t' <<  syskvctx->key()->key_type()->KEY_DESC()->getSymbol()->getText() << " ";
                kv_pair.key = syskvctx->key()->key_type()->KEY_DESC()->getSymbol()->getText();
             }
             else if(syskvctx->key()->key_type()->KEY_COMMENT()){
                cout << '\t' <<  '\t' << syskvctx->key()->key_type()->KEY_COMMENT()->getSymbol()->getText() << " ";
                kv_pair.key = syskvctx->key()->key_type()->KEY_COMMENT()->getSymbol()->getText();
             }
             if(syskvctx->value()->integer()){
                cout << '\t'  << syskvctx->value()->integer()->INTEGER()->getSymbol()->getText() << endl;
                kv_pair.value = syskvctx->value()->integer()->INTEGER()->getSymbol()->getText();
             }
             else if(syskvctx->value()->realnum()){
                cout << '\t' << syskvctx->value()->realnum()->REAL()->getSymbol()->getText() << endl;
                kv_pair.value = syskvctx->value()->realnum()->REAL()->getSymbol()->getText();
             }
             else if(syskvctx->value()->stringliteral()){
                cout << '\t' << syskvctx->value()->stringliteral()->STRINGLITERAL()->getSymbol()->getText() << endl;
                kv_pair.value = syskvctx->value()->stringliteral()->STRINGLITERAL()->getSymbol()->getText();
             }
             dataManager->addKeyValueSystem(current_package_id, current_system_id, kv_pair);
          }

          /* Iterate through the components that are part of the system */
          for(dynadlParser::ComponentContext * compctx : sysctx->component()){
             CompStruct current_component;
             string current_component_id;
             if(compctx->IDENTIFIER()){
                current_component_id = compctx->IDENTIFIER()->getSymbol()->getText();
                current_component.component_id = current_component_id; 
                cout << '\t' << '\t' << current_component_id << endl;
             }
             else{
                // If there is no IDENTIFIER() then just skip to the next iteration
                continue;
             }
             /* Iterate through the interfaces that are part of the component */
             int i = 0;
             for(dynadlParser::IfaceContext * ifacectx : compctx->iface()){
               InterfaceStruct iface;
                current_component.comp_interfaces.push_back(iface);
                if(ifacectx->IDENTIFIER()){
                     cout << '\t' << '\t' << '\t' << "Iface name: " << ifacectx->IDENTIFIER()->getSymbol()->getText() << endl;
                     current_component.comp_interfaces[i].iface_name = ifacectx->IDENTIFIER()->getSymbol()->getText();
                }
                /* Iterate through the declarations in each interface */
                for(dynadlParser::DeclarationContext * declctx : ifacectx->declaration()){
                    if(declctx->message_decl()){ 
                       cout << '\t' << '\t' << '\t' << declctx->message_decl()->SEND_RECEIVE()->getSymbol()->getText() << " message "
                            << declctx->message_decl()->IDENTIFIER()->getSymbol()->getText() << endl;   
                       if(!strcmp(declctx->message_decl()->SEND_RECEIVE()->getSymbol()->getText().c_str(),"send")){
                           current_component.comp_interfaces[i].send_msgs.push_back(declctx->message_decl()->IDENTIFIER()->getSymbol()->getText());
                       }
                       else if(!strcmp(declctx->message_decl()->SEND_RECEIVE()->getSymbol()->getText().c_str(),"receive")){
                          current_component.comp_interfaces[i].recv_msgs.push_back(declctx->message_decl()->IDENTIFIER()->getSymbol()->getText());
                       }
                    }
                    else if(declctx->service_decl()){
                       cout << '\t' << '\t' << '\t' << declctx->service_decl()->SEND_RECEIVE()->getSymbol()->getText() << " service "
                            << declctx->service_decl()->IDENTIFIER()->getSymbol()->getText() << endl;
                       if(!strcmp(declctx->service_decl()->SEND_RECEIVE()->getSymbol()->getText().c_str(),"send")){
                           current_component.comp_interfaces[i].send_srvcs.push_back(declctx->service_decl()->IDENTIFIER()->getSymbol()->getText());
                       }
                       else if(!strcmp(declctx->service_decl()->SEND_RECEIVE()->getSymbol()->getText().c_str(),"receive")){
                          current_component.comp_interfaces[i].recv_srvcs.push_back(declctx->service_decl()->IDENTIFIER()->getSymbol()->getText());
                       }  
                    }
                    else if(declctx->action_decl()){
                       cout  << '\t'<< '\t' << '\t' << declctx->action_decl()->SEND_RECEIVE()->getSymbol()->getText() << " action "
                             << declctx->action_decl()->IDENTIFIER()->getSymbol()->getText() << endl;
                       if(!strcmp(declctx->action_decl()->SEND_RECEIVE()->getSymbol()->getText().c_str(),"send")){
                           current_component.comp_interfaces[i].send_actns.push_back(declctx->action_decl()->IDENTIFIER()->getSymbol()->getText());
                       }
                       else if(!strcmp(declctx->action_decl()->SEND_RECEIVE()->getSymbol()->getText().c_str(),"receive")){
                          current_component.comp_interfaces[i].recv_actns.push_back(declctx->action_decl()->IDENTIFIER()->getSymbol()->getText());
                       }    
                    }
                    else if(declctx->component_configuration_decl()){
                       cout << '\t' << '\t' << '\t' << "component configuration " << declctx->component_configuration_decl()->IDENTIFIER()->getSymbol()->getText() << endl;
                       current_component.comp_interfaces[i].comp_conf.push_back(declctx->component_configuration_decl()->IDENTIFIER()->getSymbol()->getText());    
                    }
                    else if(declctx->component_state_decl()){
                       cout << '\t' << '\t' << '\t' << "component state " << declctx->component_state_decl()->IDENTIFIER()->getSymbol()->getText() << endl;
                       current_component.comp_interfaces[i].comp_state.push_back(declctx->component_state_decl()->IDENTIFIER()->getSymbol()->getText());
                    }
                    else if(declctx->diagnostics_decl()){
                       cout << '\t' << '\t' << '\t' << "diagnostic service " << declctx->diagnostics_decl()->IDENTIFIER()->getSymbol()->getText() << endl;
                       current_component.comp_interfaces[i].diag_srvcs.push_back(declctx->diagnostics_decl()->IDENTIFIER()->getSymbol()->getText());
                    }
                }
                i++;
             }
             dataManager->addComponent(current_package_id,current_system_id,current_component);
          }
          /* Iterate through the system configurations that are part of the system */
         SystemConfig sys_cfg;
         for(dynadlParser::System_configurationContext * syscfgctx : sysctx->system_configuration()){
            if(syscfgctx->IDENTIFIER()){
               cout << '\t' << '\t' << syscfgctx->IDENTIFIER()->getSymbol()->getText() << endl;
               sys_cfg.system_config_id = syscfgctx->IDENTIFIER()->getSymbol()->getText();
               sys_cfg.config_components.clear();
               for(dynadlParser::Component_declContext * compdclctx : syscfgctx->component_decl()){
                  cout << '\t' << '\t' << '\t' << compdclctx->IDENTIFIER()->getSymbol()->getText() << endl;
                  sys_cfg.config_components.push_back(compdclctx->IDENTIFIER()->getSymbol()->getText());
               }
               dataManager->addSysConfig(current_package_id,current_system_id,sys_cfg);
            }
         }
       }
  }
  
  void dynadlWalkListener :: exitPckg(dynadlParser::PckgContext * /*ctx*/) {
         cout << "Exit package " << current_package_id << endl;
  }

  void dynadlWalkListener :: enterScript(dynadlParser::ScriptContext * ctx){
  
  }

  void dynadlWalkListener :: exitScript(dynadlParser::ScriptContext * /*ctx*/){
  
  }

  void dynadlWalkListener :: enterBlock(dynadlParser::BlockContext * /*ctx*/){

  }

  void dynadlWalkListener :: exitBlock(dynadlParser::BlockContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterStart_command(dynadlParser::Start_commandContext * ctx){
      CmdStruct cmdStruct;
      VarStruct varStruct;
      if(extra_tab){
         cout << '\t';
      }
      cout << '\t' << "start ";
      if(ctx->IDENTIFIER()){
         cout << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
         if(in_function_definition){
            cmdStruct.cmd_id = "start";
            varStruct.var_type = "string";
            varStruct.var_id = ctx->IDENTIFIER()->getSymbol()->getText();
            cmdStruct.cmd_args.push_back(varStruct);
            current_statement_struct.func_cmds.push_back(cmdStruct); 
         }
      }
      else{
         cout << "Missing identifier in start command" << endl;
      } 
  }

  void dynadlWalkListener :: exitStart_command(dynadlParser::Start_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterLoad_command(dynadlParser::Load_commandContext * ctx){
       CmdStruct cmdStruct;
       VarStruct varStruct;
       if(extra_tab){
          cout << '\t';
       }
       cout << '\t' << "load ";
       if(ctx->IDENTIFIER()){
          cout << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
          if(in_function_definition){
             cmdStruct.cmd_id = "load";
             varStruct.var_type = "string";
             varStruct.var_id = ctx->IDENTIFIER()->getSymbol()->getText();
             cmdStruct.cmd_args.push_back(varStruct);
             current_statement_struct.func_cmds.push_back(cmdStruct); 
          }
       }
  }

  void dynadlWalkListener :: exitLoad_command(dynadlParser::Load_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterUnload_command(dynadlParser::Unload_commandContext * ctx){
     CmdStruct cmdStruct;
     VarStruct varStruct;
     if(extra_tab){
         cout << '\t';
     }
     cout << '\t' << "unload ";
     if(ctx->IDENTIFIER()){
        cout << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
        if(in_function_definition){
           cmdStruct.cmd_id = "unload";
           varStruct.var_type = "string";
           varStruct.var_id = ctx->IDENTIFIER()->getSymbol()->getText();
           cmdStruct.cmd_args.push_back(varStruct);
           current_statement_struct.func_cmds.push_back(cmdStruct);
        }
     }
  }

  void dynadlWalkListener :: exitUnload_command(dynadlParser::Unload_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterStop_command(dynadlParser::Stop_commandContext * ctx){
     CmdStruct cmdStruct;
     VarStruct varStruct;
     if(extra_tab){
         cout << '\t';
     }
     cout << '\t' << "stop ";
     if(ctx->IDENTIFIER()){
        cout << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
        if(in_function_definition){
           cmdStruct.cmd_id = "stop";
           varStruct.var_type = "string";
           varStruct.var_id = ctx->IDENTIFIER()->getSymbol()->getText();
           cmdStruct.cmd_args.push_back(varStruct);
           current_statement_struct.func_cmds.push_back(cmdStruct); 
        }
     }
  }

  void dynadlWalkListener :: exitStop_command(dynadlParser::Stop_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterSave_command(dynadlParser::Save_commandContext * ctx){
     CmdStruct cmdStruct;
     VarStruct varStruct;
     if(extra_tab){
         cout << '\t';
     }
     cout << '\t' << "save ";
     if(ctx->IDENTIFIER()){
        cout << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
        if(in_function_definition){
           cmdStruct.cmd_id = "save";
           varStruct.var_type = "string";
           varStruct.var_id = ctx->IDENTIFIER()->getSymbol()->getText();
           cmdStruct.cmd_args.push_back(varStruct);
           current_statement_struct.func_cmds.push_back(cmdStruct);
        }
     }
  }

  void dynadlWalkListener :: exitSave_command(dynadlParser::Save_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterRestore_command(dynadlParser::Restore_commandContext * ctx){
     CmdStruct cmdStruct;
     VarStruct varStruct;
     if(extra_tab){
         cout << '\t';
     }     
     cout << '\t' << "restore ";
     if(ctx->IDENTIFIER()){
        cout << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
        if(in_function_definition){
           cmdStruct.cmd_id = "restore";
           varStruct.var_type = "string";
           varStruct.var_id = ctx->IDENTIFIER()->getSymbol()->getText();
           cmdStruct.cmd_args.push_back(varStruct);
           current_statement_struct.func_cmds.push_back(cmdStruct);
        }
     }   
  }

  void dynadlWalkListener :: exitRestore_command(dynadlParser::Restore_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }


  void dynadlWalkListener :: enterSelect_package_command(dynadlParser::Select_package_commandContext * ctx){
     CmdStruct cmdStruct;
     VarStruct varStruct;
     if(extra_tab){
         cout << '\t';
     }
     if(ctx->PACKAGE()){
        cout << '\t' << "select " << ctx->PACKAGE()->getSymbol()->getText() << " ";
     }
     if(ctx->IDENTIFIER()){
        cout << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
        if(in_function_definition){
           cmdStruct.cmd_id = "select";
           varStruct.var_type = "string";
           varStruct.var_id = ctx->PACKAGE()->getSymbol()->getText();
           cmdStruct.cmd_args.push_back(varStruct);
           varStruct.var_type = "string";
           varStruct.var_id = ctx->IDENTIFIER()->getSymbol()->getText();
           cmdStruct.cmd_args.push_back(varStruct);
           current_statement_struct.func_cmds.push_back(cmdStruct);
        }
     }
  }

  void dynadlWalkListener :: exitSelect_package_command(dynadlParser::Select_package_commandContext * ctx){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterSelect_system_command(dynadlParser::Select_system_commandContext * ctx){
     CmdStruct cmdStruct;
     VarStruct varStruct;
     if(extra_tab){
         cout << '\t';
     }
     if(ctx->SYSTEM()){
        cout << '\t' << "select " << ctx->SYSTEM()->getSymbol()->getText() << " ";
     }
     if(ctx->IDENTIFIER()){
        cout << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
        if(in_function_definition){
           cmdStruct.cmd_id = "select";
           varStruct.var_type = "string";
           varStruct.var_id = ctx->SYSTEM()->getSymbol()->getText();
           cmdStruct.cmd_args.push_back(varStruct);
           varStruct.var_type = "string";
           varStruct.var_id = ctx->IDENTIFIER()->getSymbol()->getText();
           cmdStruct.cmd_args.push_back(varStruct);
           current_statement_struct.func_cmds.push_back(cmdStruct);
        }
     }
  }

  void dynadlWalkListener :: exitSelect_system_command(dynadlParser::Select_system_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterSwap_command(dynadlParser::Swap_commandContext * ctx){
     CmdStruct cmdStruct;
     VarStruct varStruct;
     if(extra_tab){
         cout << '\t';
     }
     cout << '\t' << "swap ";
     if(in_function_definition){
        cmdStruct.cmd_id = "swap";
     }
     for(antlr4::tree::TerminalNode * tnode : ctx->IDENTIFIER()){
        cout << tnode->getSymbol()->getText() << " ";
        if(in_function_definition){
             varStruct.var_type = "string";
             varStruct.var_id = tnode->getSymbol()->getText();
             cmdStruct.cmd_args.push_back(varStruct);
        }
     }
     cout << endl;
     if(in_function_definition){
        if(ctx->ONCE()){
             varStruct.var_type = "string";
             varStruct.var_id = ctx->ONCE()->getSymbol()->getText();
             cmdStruct.cmd_args.push_back(varStruct);                 
        }
        else if(ctx->AT()){
             varStruct.var_type = "string";
             varStruct.var_id = ctx->AT()->getSymbol()->getText();
             cmdStruct.cmd_args.push_back(varStruct);
             varStruct.var_type = "string";
             if(ctx->dbl()->DOUBLE()){
                    varStruct.var_id = ctx->dbl()->DOUBLE()->getSymbol()->getText();
             }
             else if(ctx->integer()->INTEGER()){
                    varStruct.var_id = ctx->integer()->INTEGER()->getSymbol()->getText();
             }
             cmdStruct.cmd_args.push_back(varStruct);                                      
        }
        current_statement_struct.func_cmds.push_back(cmdStruct);
     }
  }

  void dynadlWalkListener :: exitSwap_command(dynadlParser::Swap_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterDelay_command(dynadlParser::Delay_commandContext * ctx){
            CmdStruct cmdStruct;
            VarStruct varStruct;
            if(extra_tab){
               cout << '\t';
            }
            cout << '\t' << "delay ";
            if(in_function_definition){
               cmdStruct.cmd_id = "delay";
            }
            if(ctx->integer()){
               cout << ctx->integer()->INTEGER()->getSymbol()->getText() << endl;
               if(in_function_definition){
                  varStruct.var_type = "integer";
                  varStruct.var_id = ctx->integer()->INTEGER()->getSymbol()->getText();
                  cmdStruct.cmd_args.push_back(varStruct);
               }
            }else if(ctx->dbl()){
               cout << ctx->dbl()->DOUBLE()->getSymbol()->getText() << endl;
               if(in_function_definition){
                  varStruct.var_type = "double";
                  varStruct.var_id = ctx->dbl()->DOUBLE()->getSymbol()->getText();
                  cmdStruct.cmd_args.push_back(varStruct);
               }    
            }
            if(in_function_definition){
                  current_statement_struct.func_cmds.push_back(cmdStruct); 
            }

  }

  void dynadlWalkListener :: exitDelay_command(dynadlParser::Delay_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterTimer_command(dynadlParser::Timer_commandContext * ctx){
     CmdStruct cmdStruct;
     VarStruct varStruct;
     if(extra_tab){
         cout << '\t';
     }
     cout << '\t' << "timer ";
     cout << ctx->SINGLE_PERIODIC()->getSymbol()->getText() << " ";
     if(in_function_definition){
        cmdStruct.cmd_id = "timer";
        varStruct.var_type = "string";
        varStruct.var_id = ctx->SINGLE_PERIODIC()->getSymbol()->getText();
        cmdStruct.cmd_args.push_back(varStruct);
     }
     if(ctx->integer()){
        cout << ctx->integer()->INTEGER()->getSymbol()->getText() << " ";
        varStruct.var_type = "integer";
        varStruct.var_id = ctx->integer()->INTEGER()->getSymbol()->getText();
        cmdStruct.cmd_args.push_back(varStruct);
     }
     else if(ctx->dbl()){
        cout << ctx->dbl()->DOUBLE()->getSymbol()->getText() << " ";
        varStruct.var_type = "double";
        varStruct.var_id = ctx->dbl()->DOUBLE()->getSymbol()->getText();
        cmdStruct.cmd_args.push_back(varStruct);
     }
     varStruct.var_type = "string";
     varStruct.var_id = ctx->IDENTIFIER()->getSymbol()->getText();
     cmdStruct.cmd_args.push_back(varStruct);
     cout << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
     if(in_function_definition){
           current_statement_struct.func_cmds.push_back(cmdStruct);
     }
  }
  
  void dynadlWalkListener :: exitTimer_command(dynadlParser::Timer_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterExec_command(dynadlParser::Exec_commandContext * ctx){
     CmdStruct cmdStruct;
     VarStruct varStruct;
     if(extra_tab){
         cout << '\t';
     }
     cout << '\t' << "exec ";
     if(ctx->STRINGLITERAL()){
        cout << ctx->STRINGLITERAL()->getSymbol()->getText() << endl;
        if(in_function_definition){
           cmdStruct.cmd_id = "exec";
           varStruct.var_type = "string";
           varStruct.var_id = ctx->STRINGLITERAL()->getSymbol()->getText();
           cmdStruct.cmd_args.push_back(varStruct);
           current_statement_struct.func_cmds.push_back(cmdStruct); 
        }
     }
     else{
         cout << "Missing argument for exec command" << endl;
     }
  }
  
  void dynadlWalkListener :: exitExec_command(dynadlParser::Exec_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }


  void dynadlWalkListener :: enterEvent_command(dynadlParser::Event_commandContext * ctx){
    CmdStruct cmdStruct;
     VarStruct varStruct;
     if(extra_tab){
         cout << '\t';
     }
     cout << '\t' << "event ";
     if(ctx->IDENTIFIER()){
        cout << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
        if(in_function_definition){
           cmdStruct.cmd_id = "event";
           varStruct.var_type = "string";
           varStruct.var_id = ctx->IDENTIFIER()->getSymbol()->getText();
           cmdStruct.cmd_args.push_back(varStruct);
           current_statement_struct.func_cmds.push_back(cmdStruct); 
        }
     }
     else{
         cout << "Missing argument for event command" << endl;
     }
  }

  void dynadlWalkListener :: exitEvent_command(dynadlParser::Event_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterPrint_command(dynadlParser::Print_commandContext * ctx){
     CmdStruct cmdStruct;
     VarStruct varStruct;
     if(extra_tab){
        cout << '\t';
     }
     cout << '\t' << "print ";
     if(in_function_definition){
        cmdStruct.cmd_id = "print";
        if(ctx->STRINGLITERAL()){
           cout << ctx->STRINGLITERAL()->getSymbol()->getText() << endl;
           varStruct.var_type = "string";
           varStruct.var_id = ctx->STRINGLITERAL()->getSymbol()->getText();
        }
        else if(ctx->IDENTIFIER()){
           cout << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
           varStruct.var_type = "identifier";
           varStruct.var_id = ctx->IDENTIFIER()->getSymbol()->getText();
        }

        cmdStruct.cmd_args.push_back(varStruct);
        current_statement_struct.func_cmds.push_back(cmdStruct);
     }
  }

  void dynadlWalkListener :: exitPrint_command(dynadlParser::Print_commandContext * ctx){
      dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
      clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterLog_command(dynadlParser::Log_commandContext * ctx){
     CmdStruct cmdStruct;
     VarStruct varStruct;
     if(extra_tab){
        cout << '\t';
     }
     cout << '\t' << "log ";
     if(in_function_definition){
        cmdStruct.cmd_id = "log";
     }
 
     for(antlr4::tree::TerminalNode *tnode : ctx->STRINGLITERAL()){
         cout << tnode->getSymbol()->getText() << " ";
         if(in_function_definition){
            varStruct.var_type = "string";
            varStruct.var_id = tnode->getSymbol()->getText();
            cmdStruct.cmd_args.push_back(varStruct);
         }
     }
     if(in_function_definition){
           current_statement_struct.func_cmds.push_back(cmdStruct); 
     }
     cout << endl;
  }


  void dynadlWalkListener :: exitLog_command(dynadlParser::Log_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterSpawn_command(dynadlParser::Spawn_commandContext * ctx){
     CmdStruct cmdStruct;
     VarStruct varStruct;
     if(extra_tab){
        cout << '\t';
     }     
     cout << '\t' << "spawn ";
     if(ctx->IDENTIFIER()){
        cout << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
        if(in_function_definition){
           cmdStruct.cmd_id = "spawn";
           varStruct.var_type = "string";
           varStruct.var_id = ctx->IDENTIFIER()->getSymbol()->getText();
           cmdStruct.cmd_args.push_back(varStruct);
           current_statement_struct.func_cmds.push_back(cmdStruct); 
        }
     }
     else{
        cout << "Missing argument for spawn command" << endl; 
     }
  }

  void dynadlWalkListener :: exitSpawn_command(dynadlParser::Spawn_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterWait_command(dynadlParser::Wait_commandContext * ctx){
     CmdStruct cmdStruct;
     VarStruct varStruct;
     if(extra_tab){
        cout << '\t';
     }     
     cout << '\t' << "wait ";
     if(in_function_definition){
        cmdStruct.cmd_id = "wait";
     }
     bool b = false;
     for(antlr4::tree::TerminalNode * tnode : ctx->IDENTIFIER()){
        if(in_function_definition){
           varStruct.var_type = "string";
           varStruct.var_id = tnode->getSymbol()->getText();
           cmdStruct.cmd_args.push_back(varStruct);                  
        }
        if(b == false){
           cout << tnode->getSymbol()->getText() << " ";
           b = true;
        }
        else{
         cout << tnode->getSymbol()->getText() << endl;
        }
     }
     if(in_function_definition){
           current_statement_struct.func_cmds.push_back(cmdStruct);
     }
  }

  void dynadlWalkListener :: exitWait_command(dynadlParser::Wait_commandContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterEvent_parameter_list(dynadlParser::Event_parameter_listContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener ::exitEvent_parameter_list(dynadlParser::Event_parameter_listContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener ::enterEvent_parameter(dynadlParser::Event_parameterContext * /*ctx*/){

  }

  void dynadlWalkListener ::exitEvent_parameter(dynadlParser::Event_parameterContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();       
  }
 
  void dynadlWalkListener ::enterIf_statement(dynadlParser::If_statementContext  *ctx){
       cout << '\t' << "if" << " ";
       extra_tab = true;
       current_statement_struct.statement_id = "if";
       if(ctx->expression()->basic_expression()){
          if(ctx->expression()->basic_expression()->IDENTIFIER()){
             cout << ctx->expression()->basic_expression()->IDENTIFIER()->getSymbol()->getText() << endl;
             current_statement_struct.expression = ctx->expression()->basic_expression()->IDENTIFIER()->getSymbol()->getText();
          }
       }
       else{
         cout << "No expression defined for if statement" << endl;
       }
       if(ctx->ELSE()){
             current_statement_struct.statement_id = "ifelse";           
       }
       dynadlParser::BlockContext *blk_it = ctx->block(0);
       current_statement_struct.stmts_to_skip = blk_it->commands().size() + blk_it->inc_statement().size() + blk_it->dec_statement().size();
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener ::exitIf_statement(dynadlParser::If_statementContext * ctx){
       cout << '\t' << "endif" << endl;
       extra_tab = false;
       current_statement_struct.statement_id = "endif";
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterInc_statement(dynadlParser::Inc_statementContext * ctx){
      if(extra_tab){
         cout << '\t';
      }
      current_statement_struct.statement_id = "inc";
      if(ctx->IDENTIFIER()){
         cout << '\t' << "inc " << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
         current_statement_struct.expression = ctx->IDENTIFIER()->getSymbol()->getText();
      }
  }

  void dynadlWalkListener :: exitInc_statement(dynadlParser::Inc_statementContext * /*ctx*/){
     dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
     clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterDec_statement(dynadlParser::Dec_statementContext * ctx){
      if(extra_tab){
         cout << '\t';
      }
      current_statement_struct.statement_id = "dec";
      if(ctx->IDENTIFIER()){
         cout << '\t' << "dec " << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
         current_statement_struct.expression = ctx->IDENTIFIER()->getSymbol()->getText();
      }
  }

  void dynadlWalkListener :: exitDec_statement(dynadlParser::Dec_statementContext * /*ctx*/){
      dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
      clearCurrentStatementStruct();
  }



  void dynadlWalkListener ::enterExpression(dynadlParser::ExpressionContext * ctx){
 
  }

  void dynadlWalkListener ::exitExpression(dynadlParser::ExpressionContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterBasic_expression(dynadlParser::Basic_expressionContext * /*ctx*/){

  }

  void dynadlWalkListener :: exitBasic_expression(dynadlParser::Basic_expressionContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterCall_command(dynadlParser::Call_commandContext * ctx){
   CmdStruct cmdStruct;
   VarStruct varStruct;
   if(extra_tab){
      cout << '\t';
   }   
   cout << '\t' << "call ";
   if(ctx->IDENTIFIER()){
      cout << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
      if(in_function_definition){
         cmdStruct.cmd_id = "call";
         varStruct.var_type = "string";
         varStruct.var_id = ctx->IDENTIFIER()->getSymbol()->getText();
         cmdStruct.cmd_args.push_back(varStruct);
         current_statement_struct.func_cmds.push_back(cmdStruct);
      }
   }
   else{
      cout << "Missing identifier for call command" << endl;
   }
  }
  void dynadlWalkListener :: exitCall_command(dynadlParser::Call_commandContext * /*ctx*/){
      dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
      clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterStatement(dynadlParser::StatementContext * /*ctx*/){
  }

  void dynadlWalkListener :: exitStatement(dynadlParser::StatementContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterWhile_statement(dynadlParser::While_statementContext * ctx){
       cout << '\t' << "while" << " ";
       extra_tab = true;
       clearCurrentStatementStruct();
       current_statement_struct.statement_id = "while";
       dynadlParser ::ExpressionContext *exprctx = ctx->expression();
       if(exprctx){
          if(exprctx->basic_expression()->IDENTIFIER()){
             cout << exprctx->basic_expression()->IDENTIFIER()->getSymbol()->getText() << endl;
             current_statement_struct.expression = exprctx->basic_expression()->IDENTIFIER()->getSymbol()->getText();
          }
       }
       else{
          cout << "No expression define for while statement" << endl;
       }
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: exitWhile_statement(dynadlParser::While_statementContext * /*ctx*/){
       cout << '\t' << "endwhile" << endl;
       extra_tab = false;
       current_statement_struct.statement_id = "endwhile";
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterBasic_statement(dynadlParser::Basic_statementContext * /*ctx*/){

  }
  void dynadlWalkListener :: exitBasic_statement(dynadlParser::Basic_statementContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterVariable_declaration_statement(dynadlParser::Variable_declaration_statementContext * ctx){
       current_statement_struct.statement_id = "declaration";
       current_statement_struct.expression = "";
       current_statement_struct.func_cmds.clear();
       current_statement_struct.var_decl.var_id.clear();
       current_statement_struct.var_decl.var_type.clear();
       current_statement_struct.var_decl.var_value.clear();
        dynadlParser::Variable_declarationContext * vdeclctx = ctx->variable_declaration();
        dynadlParser::Real_base_typeContext *rbtctx = vdeclctx->real_base_type();
        string idstring = find_type_idstring(rbtctx);
        current_statement_struct.var_decl.var_type = idstring;
        cout << '\t' << idstring << " ";
        if(vdeclctx->IDENTIFIER()){
           cout << '\t' << vdeclctx->IDENTIFIER()->getSymbol()->getText() << " ";
           current_statement_struct.var_decl.var_id = vdeclctx->IDENTIFIER()->getSymbol()->getText();
        }
        else{
           cout << "Missing identifier in variable declaration" << endl;
        }
        dynadlParser::ExpressionContext *exprctx = ctx->expression();
        if(exprctx){
           dynadlParser :: ValueContext * valctx = exprctx->basic_expression()->value();
           if(valctx){
             if(valctx->integer()){
                cout << valctx->integer()->INTEGER()->getSymbol()->getText() << endl;
                current_statement_struct.var_decl.var_value = valctx->integer()->INTEGER()->getSymbol()->getText();
             }
             else if(valctx->realnum()){
                cout << valctx->realnum()->REAL()->getSymbol()->getText() << endl;
                current_statement_struct.var_decl.var_value = valctx->realnum()->REAL()->getSymbol()->getText();
             }
             else if(valctx->stringliteral()){
                cout << valctx->stringliteral()->STRINGLITERAL()->getSymbol()->getText() << endl;
                current_statement_struct.var_decl.var_value = valctx->stringliteral()->STRINGLITERAL()->getSymbol()->getText();
             }  
           }
        }
        else{
           cout << "Missing expression in variable declaration" << endl;         
        }
  }

  void dynadlWalkListener :: exitVariable_declaration_statement(dynadlParser::Variable_declaration_statementContext * /*ctx*/){
       dataManager->addStatementDefinition(current_func_struct.function_id,current_statement_struct);
       clearCurrentStatementStruct();
  }

  void dynadlWalkListener :: enterVariable_declaration_list(dynadlParser::Variable_declaration_listContext * /*ctx*/){

  }
  void dynadlWalkListener :: exitVariable_declaration_list(dynadlParser::Variable_declaration_listContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterVariable_declaration(dynadlParser::Variable_declarationContext * /*ctx*/){

  }
  void dynadlWalkListener :: exitVariable_declaration(dynadlParser::Variable_declarationContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterIdentifier_list(dynadlParser::Identifier_listContext * /*ctx*/){

  }

  void dynadlWalkListener :: exitIdentifier_list(dynadlParser::Identifier_listContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterSystem(dynadlParser::SystemContext * /*ctx*/)  { 
  
  }
  
  void dynadlWalkListener :: exitSystem(dynadlParser::SystemContext * /*ctx*/)  { 
  
  }
  
  void dynadlWalkListener :: enterKey_type(dynadlParser::Key_typeContext * /*ctx*/){
  
  }
  
  void dynadlWalkListener :: exitKey_type(dynadlParser::Key_typeContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterList_kv(dynadlParser::List_kvContext * /*ctx*/){  
  }

  void dynadlWalkListener :: exitList_kv(dynadlParser::List_kvContext * /*ctx*/){

  }
  void dynadlWalkListener :: enterKv(dynadlParser::KvContext * /*ctx*/){

  }
  void dynadlWalkListener :: exitKv(dynadlParser::KvContext * /*ctx*/){

  }
  void dynadlWalkListener :: enterValue(dynadlParser::ValueContext * /*ctx*/){

  }
  void dynadlWalkListener :: exitValue(dynadlParser::ValueContext * /*ctx*/){

  }
  void dynadlWalkListener :: enterKey(dynadlParser::KeyContext * /*ctx*/){

  }
  void dynadlWalkListener :: exitKey(dynadlParser::KeyContext * /*ctx*/){

  }
  void dynadlWalkListener :: enterRealnum(dynadlParser::RealnumContext * /*ctx*/){

  }
  void dynadlWalkListener :: exitRealnum(dynadlParser::RealnumContext * /*ctx*/){

  }
  void dynadlWalkListener :: enterStringliteral(dynadlParser::StringliteralContext * /*ctx*/){

  }
  void dynadlWalkListener :: exitStringliteral(dynadlParser::StringliteralContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterComponent(dynadlParser::ComponentContext * /*ctx*/)  { 

  }

  void dynadlWalkListener :: exitComponent(dynadlParser::ComponentContext * /*ctx*/)  { 

  }

  void dynadlWalkListener :: enterComponent_state_decl(dynadlParser::Component_state_declContext * /*ctx*/){
   
  }
  void dynadlWalkListener :: exitComponent_state_decl(dynadlParser::Component_state_declContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterComponent_configuration_decl(dynadlParser::Component_configuration_declContext * /*ctx*/){
     
  }

  void dynadlWalkListener :: exitComponent_configuration_decl(dynadlParser::Component_configuration_declContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterComponent_configuration(dynadlParser::Component_configurationContext * ctx){
       CompConfiguration compConfiguraton;
       cout << endl << "Component Configuration Definition" << endl;
       cout << '\t' << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
       compConfiguraton.config_id = ctx->IDENTIFIER()->getSymbol()->getText();
       for(dynadlParser::Cfg_fieldContext * fctx : ctx->cfg_field()){
           VarDeclStruct varDeclStruct;
           dynadlParser :: Real_base_typeContext  * rbtctx = fctx->field_type()->base_type()->real_base_type();
           string idstring = find_type_idstring(rbtctx);
           cout << '\t' << '\t' << idstring << " ";
           if(fctx->IDENTIFIER()){
              cout << fctx->IDENTIFIER()->getSymbol()->getText() << endl;
              varDeclStruct.var_id = fctx->IDENTIFIER()->getSymbol()->getText();
           }
           else{
              cout << "Missing identifier for component" << endl;
           }
           varDeclStruct.var_type = idstring;
           if(fctx->value()){
              if(fctx->value()->stringliteral()){
                 varDeclStruct.var_value =  fctx->value()->stringliteral()->STRINGLITERAL()->getSymbol()->getText();   
              }
              else if(fctx->value()->dbl()){
                 varDeclStruct.var_value = fctx->value()->dbl()->DOUBLE()->getSymbol()->getText(); 
              }
              else if(fctx->value()->integer()){
                 varDeclStruct.var_value = fctx->value()->integer()->INTEGER()->getSymbol()->getText();
              }
              else if(fctx->value()->realnum()){
                 fctx->value()->realnum()->REAL()->getSymbol()->getText();
              }
           }
           compConfiguraton.config_fields.push_back(varDeclStruct);
       }
       dataManager->addCompConfigDefinition(compConfiguraton);
  }

  void dynadlWalkListener :: exitComponent_configuration(dynadlParser::Component_configurationContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterComponent_state(dynadlParser::Component_stateContext * ctx){
       CompState compState;
       cout << endl << "Component State Definition" << endl;
       if(ctx->IDENTIFIER()){
          cout << '\t' << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
          compState.state_id = ctx->IDENTIFIER()->getSymbol()->getText();
       }
       else{
          cout << "Missing identifier" << endl;
       }
       for(dynadlParser::FieldContext * fctx : ctx->field()){
           VarStruct varStruct;
           dynadlParser :: Real_base_typeContext  * rbtctx = fctx->field_type()->base_type()->real_base_type();
           string idstring = find_type_idstring(rbtctx);
           cout << '\t' << '\t' << idstring << " ";
           cout << fctx->IDENTIFIER()->getSymbol()->getText() << endl;
           varStruct.var_id = fctx->IDENTIFIER()->getSymbol()->getText();
           varStruct.var_type = idstring;
           compState.state_fields.push_back(varStruct);
       }
       dataManager->addCompStateDefinition(compState);
  }

  void dynadlWalkListener :: exitComponent_state(dynadlParser::Component_stateContext * /*ctx*/){

  }


  void dynadlWalkListener :: enterComponent_decl(dynadlParser::Component_declContext * /*ctx*/){

  }

  void dynadlWalkListener :: exitComponent_decl(dynadlParser::Component_declContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterSystem_configuration(dynadlParser::System_configurationContext * /*ctx*/){

  }

  void dynadlWalkListener :: exitSystem_configuration(dynadlParser::System_configurationContext * /*ctx*/){

  }

  void dynadlWalkListener :: enterDiagnostics_decl(dynadlParser::Diagnostics_declContext * /*ctx*/){

  }

  void dynadlWalkListener :: exitDiagnostics_decl(dynadlParser::Diagnostics_declContext * /*ctx*/){

  }



  void dynadlWalkListener :: enterIface(dynadlParser::IfaceContext * /*ctx*/){

  }

  void dynadlWalkListener :: exitIface(dynadlParser::IfaceContext * /*ctx*/){

  }


  void dynadlWalkListener :: enterDeclaration(dynadlParser::DeclarationContext * /*ctx*/){

   }

  void dynadlWalkListener :: exitDeclaration(dynadlParser::DeclarationContext * /*ctx*/){ }

  void dynadlWalkListener :: enterMessage_decl(dynadlParser::Message_declContext * /*ctx*/){ }
  void dynadlWalkListener :: exitMessage_decl(dynadlParser::Message_declContext * /*ctx*/){ }

  void dynadlWalkListener :: enterService_decl(dynadlParser::Service_declContext * /*ctx*/){ }
  void dynadlWalkListener :: exitService_decl(dynadlParser::Service_declContext * /*ctx*/){ }

  void dynadlWalkListener :: enterAction_decl(dynadlParser::Action_declContext * /*ctx*/) { }
  void dynadlWalkListener :: exitAction_decl(dynadlParser::Action_declContext * /*ctx*/) { }

  void dynadlWalkListener :: enterDefinition(dynadlParser::DefinitionContext *ctx) {}
  void dynadlWalkListener :: exitDefinition(dynadlParser::DefinitionContext *ctx) {}


   void dynadlWalkListener :: enterMessage(dynadlParser::MessageContext *ctx) {
       MsgStruct msgStruct;
       cout << endl << "Message Definition" << endl;
       if(ctx->IDENTIFIER()){ 
          cout << '\t' << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
          msgStruct.msg_id = ctx->IDENTIFIER()->getSymbol()->getText();
       }
       else{
          cout << "Missing identifier" << endl;
       }
       for(dynadlParser::FieldContext * fctx : ctx->field()){
           VarStruct varStruct;
           dynadlParser :: Real_base_typeContext  * rbtctx = fctx->field_type()->base_type()->real_base_type();
           string idstring = find_type_idstring(rbtctx);
           cout << '\t' << '\t' << idstring << " ";
           cout << '\t' << fctx->IDENTIFIER()->getSymbol()->getText() << endl;
           varStruct.var_type = idstring;
           varStruct.var_id = fctx->IDENTIFIER()->getSymbol()->getText();
           msgStruct.msg_fields.push_back(varStruct);
       }
       dataManager->addMsgDefinition(msgStruct);
   }

   void dynadlWalkListener :: exitMessage(dynadlParser::MessageContext *ctx) {

   }

   void dynadlWalkListener :: enterAction(dynadlParser::ActionContext *ctx) {
       ActnStruct actnStruct;
       cout << endl << "Action Definition" << endl;
       if(ctx->IDENTIFIER()){
          cout << '\t' << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
          actnStruct.actn_id = ctx->IDENTIFIER()->getSymbol()->getText();
       }
       else{
          cout << "Missing identifier" << endl;
       }
       int i=0;
       int j= 0;
       for(dynadlParser::FieldContext * fctx : ctx->field()){
           VarStruct varStruct;
           string inoutupd;
           if(ctx->IN_OUT(i)){
              inoutupd = ctx->IN_OUT(i++)->getSymbol()->getText();
           }
           else if(ctx->UPDATE(j)){
              inoutupd = ctx->UPDATE(j++)->getSymbol()->getText();
           }
           cout << '\t' << '\t' << inoutupd << " ";
           dynadlParser :: Real_base_typeContext  * rbtctx = fctx->field_type()->base_type()->real_base_type();
           string idstring = find_type_idstring(rbtctx);
           cout << '\t' << idstring << " ";
           cout << '\t' << fctx->IDENTIFIER()->getSymbol()->getText() << endl;
           varStruct.var_type = idstring;
           varStruct.var_id = fctx->IDENTIFIER()->getSymbol()->getText();
           if(!strcmp(inoutupd.c_str(),"in")){
              actnStruct.actn_rqst_fields.push_back(varStruct);
           }
           else if(!strcmp(inoutupd.c_str(),"out")){
              actnStruct.actn_rspnd_fields.push_back(varStruct);
           }
           else if(!strcmp(inoutupd.c_str(),"update")){
              actnStruct.actn_updt_fields.push_back(varStruct);
           }
       }
       dataManager->addActnDefinition(actnStruct);
   }

   void dynadlWalkListener :: exitAction(dynadlParser::ActionContext *ctx) {

   }

   void dynadlWalkListener :: enterService(dynadlParser::ServiceContext *ctx) {
       SrvcStruct srvcStruct;
       DiagStruct diagStruct;
       cout << endl << "Service Definition" << endl;
       if(ctx->DIAGNOSTICS()){
          diagStruct.diag_id = ctx->IDENTIFIER()->getSymbol()->getText();
       }
       else if(ctx->IDENTIFIER()){
          srvcStruct.srvc_id = ctx->IDENTIFIER()->getSymbol()->getText();
       }
       cout << '\t' << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
       int i = 0;
       for(dynadlParser::FieldContext * fctx : ctx->field()){
           VarStruct varStruct;
           string inout = ctx->IN_OUT(i++)->getSymbol()->getText();
           cout << '\t' << '\t' << inout << " ";
           dynadlParser :: Real_base_typeContext  * rbtctx = fctx->field_type()->base_type()->real_base_type();
           string idstring = find_type_idstring(rbtctx);
           cout << '\t'  << idstring << " ";
           cout << '\t'  << fctx->IDENTIFIER()->getSymbol()->getText() << endl;
           varStruct.var_type = idstring;
           varStruct.var_id = fctx->IDENTIFIER()->getSymbol()->getText();
           if(ctx->DIAGNOSTICS()){
              if(!strcmp(inout.c_str(),"in")){
                 diagStruct.diag_rqst_fields.push_back(varStruct);
              }
              else{
                 diagStruct.diag_rspnd_fields.push_back(varStruct);
              }
           }
           else{
              if(!strcmp(inout.c_str(),"in")){
                 srvcStruct.srvc_rqst_fields.push_back(varStruct);
              }
              else{
                 srvcStruct.srvc_rspnd_fields.push_back(varStruct);
              }
           }
       }
       if(ctx->DIAGNOSTICS()){
          dataManager->addDiagDefinition(diagStruct);
       }
       else{
          dataManager->addSrvcDefinition(srvcStruct);
       }
   }

   void dynadlWalkListener :: exitService(dynadlParser::ServiceContext *ctx) {

   }

   void dynadlWalkListener :: enterField(dynadlParser::FieldContext *ctx) {
   }

   void dynadlWalkListener :: exitField(dynadlParser::FieldContext *ctx) {
   }

   void dynadlWalkListener :: enterCfg_field(dynadlParser::Cfg_fieldContext * /*ctx*/){

   }

   void dynadlWalkListener :: exitCfg_field(dynadlParser::Cfg_fieldContext * /*ctx*/){

   }


   void dynadlWalkListener :: enterField_id(dynadlParser::Field_idContext *ctx) {

   }
   void dynadlWalkListener :: exitField_id(dynadlParser::Field_idContext *ctx) {
  
   }

   void dynadlWalkListener :: enterCommands(dynadlParser::CommandsContext * /*ctx*/) {

   }

   void dynadlWalkListener :: exitCommands(dynadlParser::CommandsContext * /*ctx*/){

   }

   void dynadlWalkListener :: enterFunction_definition(dynadlParser::Function_definitionContext * ctx){
       cout << endl << "Enter function " << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
       current_func_struct.func_statements.clear();
       if(ctx->IDENTIFIER()){
          current_func_struct.function_id = ctx->IDENTIFIER()->getSymbol()->getText();
       }
       else{
         cout << "Missing identifier for function definition" << endl;
       }
       dataManager->addFuncDefinition(current_func_struct);
       in_function_definition = true;
   }

   void dynadlWalkListener :: exitFunction_definition(dynadlParser::Function_definitionContext * ctx){
       cout << "Exit function " << ctx->IDENTIFIER()->getSymbol()->getText() << endl;
       in_function_definition = false;
       current_function++;
       current_statement_struct.func_cmds.clear();
   }

   void dynadlWalkListener :: enterType_annotations(dynadlParser::Type_annotationsContext *ctx) {}
   void dynadlWalkListener :: exitType_annotations(dynadlParser::Type_annotationsContext *ctx) {}

   void dynadlWalkListener :: enterType_annotation(dynadlParser::Type_annotationContext *ctx) {}
   void dynadlWalkListener :: exitType_annotation(dynadlParser::Type_annotationContext *ctx) {}

   void dynadlWalkListener :: enterAnnotation_value(dynadlParser::Annotation_valueContext *ctx) {}
   void dynadlWalkListener :: exitAnnotation_value(dynadlParser::Annotation_valueContext *ctx) {}

   void dynadlWalkListener :: enterField_type(dynadlParser::Field_typeContext *ctx) {
   }
   void dynadlWalkListener :: exitField_type(dynadlParser::Field_typeContext *ctx) {
   }

   void dynadlWalkListener :: enterBase_type(dynadlParser::Base_typeContext *ctx) {}
   void dynadlWalkListener :: exitBase_type(dynadlParser::Base_typeContext *ctx) {}

   void dynadlWalkListener :: enterContainer_type(dynadlParser::Container_typeContext *ctx) {}
   void dynadlWalkListener :: exitContainer_type(dynadlParser::Container_typeContext *ctx) {}

   void dynadlWalkListener :: enterConst_value(dynadlParser::Const_valueContext *ctx) {}
   void dynadlWalkListener :: exitConst_value(dynadlParser::Const_valueContext *ctx) {}

   void dynadlWalkListener :: enterInteger(dynadlParser::IntegerContext *ctx) {}
   void dynadlWalkListener :: exitInteger(dynadlParser::IntegerContext *ctx) {}

   void dynadlWalkListener :: enterDbl(dynadlParser::DblContext * /*ctx*/){

   }

   void dynadlWalkListener :: exitDbl(dynadlParser::DblContext * /*ctx*/){

   }

   void dynadlWalkListener :: enterList_separator(dynadlParser::List_separatorContext *ctx) {}
   void dynadlWalkListener :: exitList_separator(dynadlParser::List_separatorContext *ctx) {}

   void dynadlWalkListener :: enterReal_base_type(dynadlParser::Real_base_typeContext *ctx) {}
   void dynadlWalkListener :: exitReal_base_type(dynadlParser::Real_base_typeContext *ctx) {}

   void dynadlWalkListener :: enterEveryRule(antlr4::ParserRuleContext * ctx) {
   }
   void dynadlWalkListener :: exitEveryRule(antlr4::ParserRuleContext * ctx) {
   }
   void dynadlWalkListener :: visitTerminal(antlr4::tree::TerminalNode * node) {
   }
   void dynadlWalkListener :: visitErrorNode(antlr4::tree::ErrorNode * node) {
   }


   string dynadlWalkListener :: find_type_idstring(dynadlParser::Real_base_typeContext *rbtctx){
           antlr4::tree::TerminalNode *tkn = NULL;
           if(tkn = rbtctx->TYPE_BOOL()){
               ;
           }
           else if(tkn = rbtctx->TYPE_BYTE()){
                ;
           }
           else if(tkn = rbtctx->TYPE_CHAR()){
                ;
           }
           else if(tkn = rbtctx->TYPE_INT16()){
                ;
           }
           else if(tkn = rbtctx->TYPE_UINT16()){
                ;
           }           
           else if(tkn = rbtctx->TYPE_INT32()){
                ;
           }
           else if(tkn = rbtctx->TYPE_UINT32()){
                ;
           }           
           else if(tkn = rbtctx->TYPE_INT64()){
              ;
           }
           else if(tkn = rbtctx->TYPE_UINT64()){
              ;
           }           
           else if(tkn = rbtctx->TYPE_DOUBLE()){
              ;
           }
           else if(tkn = rbtctx->TYPE_STRING()){
              ;
           }
           else if(tkn = rbtctx->TYPE_BINARY()){
              ;
           }
           else if(tkn = rbtctx->TYPE_FLOAT32()){
               ;
           }
          else if(tkn = rbtctx->TYPE_FLOAT64()){
               ;
           }           
           if(tkn){
              return tkn->getSymbol()->getText();
           }
           else{
              return string("");
           } 
   }


void dynadlWalkListener :: printDefinitions(void){
    dataManager->printDefinitions();
}

void dynadlWalkListener :: printPackages(void){
    dataManager->printPackages();
}

void dynadlWalkListener :: printFunctions(void){
    dataManager->printFunctions();
}

void dynadlWalkListener :: clearCurrentStatementStruct(void){
     current_statement_struct.expression.erase();
     current_statement_struct.statement_id.erase();
     current_statement_struct.func_cmds.clear();
     current_statement_struct.var_decl.var_id.erase();
     current_statement_struct.var_decl.var_type.erase();
     current_statement_struct.var_decl.var_value.erase();
}

void dynadlWalkListener :: generate(){
    dataManager->generate();
}


void dynadlWalkListener :: executeRuntimeScript(string func_name){
    dataManager->executeRuntimeScript(func_name);
}








