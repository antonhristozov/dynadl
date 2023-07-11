/*
 * ExprInterface.cpp
 *
 *  Created on: Jan 4, 2022
 *      Author: anton
 */


#include <iostream>
#include <fstream>
#include "antlr4-runtime.h"
#include <tree/ParseTree.h>
#include "dynadlLexer.h"
#include "dynadlParser.h"
#include "dynadlListener.h"
#include "dynadlDataManager.h"
using namespace antlrcpp;
using namespace antlr4;
using namespace std;


class dynadlWalkListener : public dynadlListener {
public:
  dynadlWalkListener(Platforms platform);
  void enterProg(dynadlParser::ProgContext * /*ctx*/);
  void exitProg(dynadlParser::ProgContext * /*ctx*/);

  void enterPackages(dynadlParser::PackagesContext * /*ctx*/);
  void exitPackages(dynadlParser::PackagesContext * /*ctx*/);

  void enterMain_function_definition(dynadlParser::Main_function_definitionContext * /*ctx*/);
  void exitMain_function_definition(dynadlParser::Main_function_definitionContext * /*ctx*/);

  void enterPckg(dynadlParser::PckgContext * /*ctx*/);
  void exitPckg(dynadlParser::PckgContext * /*ctx*/);

  void enterScript(dynadlParser::ScriptContext * /*ctx*/);
  void exitScript(dynadlParser::ScriptContext * /*ctx*/);

  void enterBlock(dynadlParser::BlockContext * /*ctx*/);
  void exitBlock(dynadlParser::BlockContext * /*ctx*/);

  void enterStart_command(dynadlParser::Start_commandContext * /*ctx*/);
  void exitStart_command(dynadlParser::Start_commandContext * /*ctx*/);

  void enterLoad_command(dynadlParser::Load_commandContext * /*ctx*/);
  void exitLoad_command(dynadlParser::Load_commandContext * /*ctx*/);

  void enterUnload_command(dynadlParser::Unload_commandContext * /*ctx*/);
  void exitUnload_command(dynadlParser::Unload_commandContext * /*ctx*/);

  void enterStop_command(dynadlParser::Stop_commandContext * /*ctx*/);
  void exitStop_command(dynadlParser::Stop_commandContext * /*ctx*/);

  void enterSave_command(dynadlParser::Save_commandContext * /*ctx*/);
  void exitSave_command(dynadlParser::Save_commandContext * /*ctx*/);

  void enterRestore_command(dynadlParser::Restore_commandContext * /*ctx*/);
  void exitRestore_command(dynadlParser::Restore_commandContext * /*ctx*/);

  void enterSelect_package_command(dynadlParser::Select_package_commandContext * /*ctx*/);
  void exitSelect_package_command(dynadlParser::Select_package_commandContext * /*ctx*/);

  void enterSelect_system_command(dynadlParser::Select_system_commandContext * /*ctx*/);
  void exitSelect_system_command(dynadlParser::Select_system_commandContext * /*ctx*/);

  void enterSwap_command(dynadlParser::Swap_commandContext * /*ctx*/);
  void exitSwap_command(dynadlParser::Swap_commandContext * /*ctx*/);

  void enterDelay_command(dynadlParser::Delay_commandContext * /*ctx*/);
  void exitDelay_command(dynadlParser::Delay_commandContext * /*ctx*/);

  void enterTimer_command(dynadlParser::Timer_commandContext * /*ctx*/);
  void exitTimer_command(dynadlParser::Timer_commandContext * /*ctx*/);

  void enterExec_command(dynadlParser::Exec_commandContext * /*ctx*/);
  void exitExec_command(dynadlParser::Exec_commandContext * /*ctx*/);


  void enterEvent_command(dynadlParser::Event_commandContext * /*ctx*/);
  void exitEvent_command(dynadlParser::Event_commandContext * /*ctx*/);

  void enterPrint_command(dynadlParser::Print_commandContext * /*ctx*/);
  void exitPrint_command(dynadlParser::Print_commandContext * /*ctx*/);

  void enterLog_command(dynadlParser::Log_commandContext * /*ctx*/);
  void exitLog_command(dynadlParser::Log_commandContext * /*ctx*/);

  void enterSpawn_command(dynadlParser::Spawn_commandContext * /*ctx*/);
  void exitSpawn_command(dynadlParser::Spawn_commandContext * /*ctx*/);

  void enterWait_command(dynadlParser::Wait_commandContext * /*ctx*/);
  void exitWait_command(dynadlParser::Wait_commandContext * /*ctx*/);

  void enterEvent_parameter_list(dynadlParser::Event_parameter_listContext * /*ctx*/);
  void exitEvent_parameter_list(dynadlParser::Event_parameter_listContext * /*ctx*/);

  void enterEvent_parameter(dynadlParser::Event_parameterContext * /*ctx*/);
  void exitEvent_parameter(dynadlParser::Event_parameterContext * /*ctx*/);

  void enterIf_statement(dynadlParser::If_statementContext * /*ctx*/);
  void exitIf_statement(dynadlParser::If_statementContext * /*ctx*/);

  void enterInc_statement(dynadlParser::Inc_statementContext * /*ctx*/);
  void exitInc_statement(dynadlParser::Inc_statementContext * /*ctx*/);

  void enterDec_statement(dynadlParser::Dec_statementContext * /*ctx*/);
  void exitDec_statement(dynadlParser::Dec_statementContext * /*ctx*/);

  void enterExpression(dynadlParser::ExpressionContext * /*ctx*/);
  void exitExpression(dynadlParser::ExpressionContext * /*ctx*/);

  void enterBasic_expression(dynadlParser::Basic_expressionContext * /*ctx*/);
  void exitBasic_expression(dynadlParser::Basic_expressionContext * /*ctx*/);

  void enterCall_command(dynadlParser::Call_commandContext * /*ctx*/);
  void exitCall_command(dynadlParser::Call_commandContext * /*ctx*/);

  void enterStatement(dynadlParser::StatementContext * /*ctx*/);
  void exitStatement(dynadlParser::StatementContext * /*ctx*/);

  void enterWhile_statement(dynadlParser::While_statementContext * /*ctx*/);
  void exitWhile_statement(dynadlParser::While_statementContext * /*ctx*/);

  void enterBasic_statement(dynadlParser::Basic_statementContext * /*ctx*/);
  void exitBasic_statement(dynadlParser::Basic_statementContext * /*ctx*/);

  void enterVariable_declaration_statement(dynadlParser::Variable_declaration_statementContext * /*ctx*/);
  void exitVariable_declaration_statement(dynadlParser::Variable_declaration_statementContext * /*ctx*/);

  void enterVariable_declaration_list(dynadlParser::Variable_declaration_listContext * /*ctx*/);
  void exitVariable_declaration_list(dynadlParser::Variable_declaration_listContext * /*ctx*/);

  void enterVariable_declaration(dynadlParser::Variable_declarationContext * /*ctx*/);
  void exitVariable_declaration(dynadlParser::Variable_declarationContext * /*ctx*/);

  void enterIdentifier_list(dynadlParser::Identifier_listContext * /*ctx*/);
  void exitIdentifier_list(dynadlParser::Identifier_listContext * /*ctx*/);


  void enterSystem(dynadlParser::SystemContext * /*ctx*/);
  void exitSystem(dynadlParser::SystemContext * /*ctx*/);

  void enterKey_type(dynadlParser::Key_typeContext * /*ctx*/);
  void exitKey_type(dynadlParser::Key_typeContext * /*ctx*/);

  void enterList_kv(dynadlParser::List_kvContext * /*ctx*/);
  void exitList_kv(dynadlParser::List_kvContext * /*ctx*/);

  void enterKv(dynadlParser::KvContext * /*ctx*/);
  void exitKv(dynadlParser::KvContext * /*ctx*/);

  void enterValue(dynadlParser::ValueContext * /*ctx*/);
  void exitValue(dynadlParser::ValueContext * /*ctx*/);

  void enterKey(dynadlParser::KeyContext * /*ctx*/);
  void exitKey(dynadlParser::KeyContext * /*ctx*/);

  void enterRealnum(dynadlParser::RealnumContext * /*ctx*/);
  void exitRealnum(dynadlParser::RealnumContext * /*ctx*/);

  void enterStringliteral(dynadlParser::StringliteralContext * /*ctx*/);
  void exitStringliteral(dynadlParser::StringliteralContext * /*ctx*/);

  void enterComponent(dynadlParser::ComponentContext * /*ctx*/);
  void exitComponent(dynadlParser::ComponentContext * /*ctx*/);

  void enterComponent_state_decl(dynadlParser::Component_state_declContext * /*ctx*/);
  void exitComponent_state_decl(dynadlParser::Component_state_declContext * /*ctx*/);

  void enterComponent_configuration_decl(dynadlParser::Component_configuration_declContext * /*ctx*/);
  void exitComponent_configuration_decl(dynadlParser::Component_configuration_declContext * /*ctx*/);

  void enterDiagnostics_decl(dynadlParser::Diagnostics_declContext * /*ctx*/);
  void exitDiagnostics_decl(dynadlParser::Diagnostics_declContext * /*ctx*/);

  void enterComponent_configuration(dynadlParser::Component_configurationContext * /*ctx*/);
  void exitComponent_configuration(dynadlParser::Component_configurationContext * /*ctx*/);

  void enterComponent_decl(dynadlParser::Component_declContext * /*ctx*/);
  void exitComponent_decl(dynadlParser::Component_declContext * /*ctx*/);

  void enterSystem_configuration(dynadlParser::System_configurationContext * /*ctx*/);
  void exitSystem_configuration(dynadlParser::System_configurationContext * /*ctx*/);

  void enterComponent_state(dynadlParser::Component_stateContext * /*ctx*/);
  void exitComponent_state(dynadlParser::Component_stateContext * /*ctx*/);


  void enterIface(dynadlParser::IfaceContext * /*ctx*/);
  void exitIface(dynadlParser::IfaceContext * /*ctx*/);

  void enterDeclaration(dynadlParser::DeclarationContext * /*ctx*/);
  void exitDeclaration(dynadlParser::DeclarationContext * /*ctx*/);

  void enterMessage_decl(dynadlParser::Message_declContext * /*ctx*/);
  void exitMessage_decl(dynadlParser::Message_declContext * /*ctx*/);

  void enterService_decl(dynadlParser::Service_declContext * /*ctx*/);
  void exitService_decl(dynadlParser::Service_declContext * /*ctx*/);

  void enterAction_decl(dynadlParser::Action_declContext * /*ctx*/);
  void exitAction_decl(dynadlParser::Action_declContext * /*ctx*/);

  void enterDefinition(dynadlParser::DefinitionContext *ctx) ;
  void exitDefinition(dynadlParser::DefinitionContext *ctx) ;

   void enterMessage(dynadlParser::MessageContext *ctx) ;
   void exitMessage(dynadlParser::MessageContext *ctx) ;

   void enterAction(dynadlParser::ActionContext *ctx) ;
   void exitAction(dynadlParser::ActionContext *ctx) ;

   void enterService(dynadlParser::ServiceContext *ctx) ;
   void exitService(dynadlParser::ServiceContext *ctx) ;

   void enterField(dynadlParser::FieldContext *ctx) ;
   void exitField(dynadlParser::FieldContext *ctx) ;

   void enterCfg_field(dynadlParser::Cfg_fieldContext * /*ctx*/);
   void exitCfg_field(dynadlParser::Cfg_fieldContext * /*ctx*/);


   void enterField_id(dynadlParser::Field_idContext *ctx) ;
   void exitField_id(dynadlParser::Field_idContext *ctx) ;

   void enterCommands(dynadlParser::CommandsContext * /*ctx*/);
   void exitCommands(dynadlParser::CommandsContext * /*ctx*/);

   void enterFunction_definition(dynadlParser::Function_definitionContext * /*ctx*/);
   void exitFunction_definition(dynadlParser::Function_definitionContext * /*ctx*/);

   void enterType_annotations(dynadlParser::Type_annotationsContext *ctx) ;
   void exitType_annotations(dynadlParser::Type_annotationsContext *ctx) ;

   void enterType_annotation(dynadlParser::Type_annotationContext *ctx) ;
   void exitType_annotation(dynadlParser::Type_annotationContext *ctx) ;

   void enterAnnotation_value(dynadlParser::Annotation_valueContext *ctx) ;
   void exitAnnotation_value(dynadlParser::Annotation_valueContext *ctx) ;

   void enterField_type(dynadlParser::Field_typeContext *ctx) ;
   void exitField_type(dynadlParser::Field_typeContext *ctx) ;

   void enterBase_type(dynadlParser::Base_typeContext *ctx) ;
   void exitBase_type(dynadlParser::Base_typeContext *ctx) ;

   void enterContainer_type(dynadlParser::Container_typeContext *ctx) ;
   void exitContainer_type(dynadlParser::Container_typeContext *ctx) ;

   void enterConst_value(dynadlParser::Const_valueContext *ctx) ;
   void exitConst_value(dynadlParser::Const_valueContext *ctx) ;

   void enterInteger(dynadlParser::IntegerContext *ctx) ;
   void exitInteger(dynadlParser::IntegerContext *ctx) ;

   void enterDbl(dynadlParser::DblContext * /*ctx*/);
   void exitDbl(dynadlParser::DblContext * /*ctx*/);

   void enterList_separator(dynadlParser::List_separatorContext *ctx) ;
   void exitList_separator(dynadlParser::List_separatorContext *ctx) ;

   void enterReal_base_type(dynadlParser::Real_base_typeContext *ctx) ;
   void exitReal_base_type(dynadlParser::Real_base_typeContext *ctx) ;

   void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) ;
   void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) ;
   void visitTerminal(antlr4::tree::TerminalNode * /*node*/) ;
   void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) ;

   string find_type_idstring(dynadlParser::Real_base_typeContext *rbtctx);
   void printDefinitions(void);
   void printPackages(void);
   void printFunctions(void);
   void clearCurrentStatementStruct(void);
   void generate();
   void executeRuntimeScript(string func_name);
   private:
       Platforms platform_;
       dynadlDataManager * dataManager;
       string current_package_id;
       string current_system_id;
       bool in_function_definition;
       bool extra_tab;
       int current_function;
       FunctionStruct current_func_struct;
       StatementStruct current_statement_struct;
};





























