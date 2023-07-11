grammar dynadl;

prog
    : packages
    ;

packages
    : ML_COMMENT* SL_COMMENT* definition* pckg* (function_definition)*
      main_function_definition EOF
    ;    

script
    : (commands | statement)*
    ;

commands
    :  (start_command | load_command | unload_command | stop_command 
      | save_command | restore_command | select_package_command | select_system_command 
      | swap_command | delay_command | event_command | call_command | print_command
      | timer_command | exec_command | log_command | spawn_command | wait_command)
    ;

block
    : (commands | dec_statement | inc_statement)*
    ;

pckg
    : PACKAGE IDENTIFIER '{' kv+ system* system '}'  | COMMA
    ;

system
    : SYSTEM IDENTIFIER '{' kv+ component* component system_configuration* '}' | COMMA
    ;

component
    : 'component' IDENTIFIER '{' iface* iface '}' | COMMA
    ;

system_configuration
    : 'system' 'configuration' IDENTIFIER '{' component_decl* '}' | COMMA
    ;

iface
    : 'interface' IDENTIFIER '{' declaration*  declaration '}'
    ;

declaration
    : variable_declaration | message_decl | service_decl |
      action_decl | component_configuration_decl  | 
      component_state_decl | diagnostics_decl | COMMA
    ;

message_decl
    :  SEND_RECEIVE  'message' IDENTIFIER 
    ;

service_decl
    : SEND_RECEIVE  'service' IDENTIFIER 
    ;

action_decl
    : SEND_RECEIVE  'action' IDENTIFIER 
    ;

component_state_decl
    : 'component' 'state' IDENTIFIER 
    ;

component_configuration_decl
    : 'component' 'configuration' IDENTIFIER 
    ;

diagnostics_decl
    : DIAGNOSTICS IDENTIFIER
    ;

component_decl
    : 'component' IDENTIFIER 
    ;

definition
    : variable_declaration_statement | message | service | 
      action | component_configuration | component_state | COMMA
    ;

message
    : 'message' IDENTIFIER '{' field* '}' type_annotations?
    ;

component_state
    : 'component' 'state' IDENTIFIER '{' field* '}' type_annotations?
    ;

component_configuration
    : 'component' 'configuration' IDENTIFIER '{' cfg_field* '}' type_annotations?
    ;

service
    : DIAGNOSTICS? 'service' IDENTIFIER '{' (IN_OUT field)* '}' type_annotations?
    ;

action
    : 'action' IDENTIFIER '{' ((IN_OUT | UPDATE) field)* '}' type_annotations?
    ;

field
    : field_id? field_type IDENTIFIER ('=' const_value)? type_annotations? list_separator?
    ;

field_id
    : integer ':'
    ;

cfg_field
    : field_type IDENTIFIER '=' value list_separator?
    ;

function_definition
    : 'function' IDENTIFIER '(' ')' '{' script '}'
    ;

main_function_definition
    : 'function' 'main' '(' ')' '{' script '}'
    ;

type_annotations
    : '(' type_annotation* ')'
    ;

type_annotation
    : IDENTIFIER ('=' annotation_value)? list_separator?
    ;

annotation_value
    : dbl | integer | LITERAL
    ;

field_type
    : base_type | IDENTIFIER | container_type
    ;

base_type
    : real_base_type type_annotations?
    ;

container_type
    : type_annotations?
    ;

const_value
    : integer | dbl | LITERAL | IDENTIFIER
    ;

integer
    : INTEGER | HEX_INTEGER
    ;

INTEGER
    : ('+' | '-')? DIGIT+
    ;

HEX_INTEGER
    : '-'? '0x' HEX_DIGIT+
    ;

dbl
    : DOUBLE
    ;

DOUBLE
    : ('+' | '-')? ( DIGIT+ ('.' DIGIT+)? | '.' DIGIT+ ) (('E' | 'e') INTEGER)?
    ;

list_separator
    : COMMA | ';'
    ;

real_base_type
    :  TYPE_BOOL | TYPE_BYTE | TYPE_CHAR | TYPE_INT16 | TYPE_UINT16 | TYPE_INT32 
       | TYPE_UINT32 | TYPE_INT64 | TYPE_UINT64 | TYPE_FLOAT32 | TYPE_FLOAT64 
       | TYPE_DOUBLE | TYPE_STRING | TYPE_BINARY 
    ;

start_command
    : 'start' IDENTIFIER
    ;

load_command
    : 'load' IDENTIFIER
    ;

unload_command
    : 'unload' IDENTIFIER
    ;

stop_command
    : 'stop' IDENTIFIER
    ;

save_command
    : 'save' IDENTIFIER
    ;

restore_command
    : 'restore' IDENTIFIER
    ;

select_package_command
    : 'select' PACKAGE IDENTIFIER
    ;

select_system_command
    : 'select' SYSTEM IDENTIFIER
    ;

delay_command
    : 'delay' (dbl | integer)
;

timer_command
    : 'timer' (SINGLE_PERIODIC) (dbl | integer) IDENTIFIER
;

print_command
    : 'print' (STRINGLITERAL | IDENTIFIER)
;

event_command
    : 'event' IDENTIFIER
    ;

wait_command
    : 'wait' IDENTIFIER IDENTIFIER ('('')')?
    ;


exec_command
    : 'exec' STRINGLITERAL
    ;

log_command
    : 'log' STRINGLITERAL STRINGLITERAL
    ;

spawn_command
    : 'spawn' IDENTIFIER ('('')')?
    ;

event_parameter_list
    : '(' ( event_parameter (',' event_parameter)* )? ')' 
    ;

event_parameter
    : real_base_type IDENTIFIER? 
    ;

expression
    : basic_expression
    ;

basic_expression
    : value
    | IDENTIFIER
    | real_base_type
    ;

call_command
    : 'call' IDENTIFIER ('('')')?
    ;

statement
    : if_statement
    | while_statement
    | inc_statement
    | dec_statement
    | basic_statement
    ;

while_statement
    : 'while' '(' expression ')' DO block ENDWHILE ;

if_statement
    : 'if' '(' expression ')' THEN block (ELSE block)? ENDIF
    ;

inc_statement
    : 'inc' IDENTIFIER
    ;

dec_statement
    : 'dec' IDENTIFIER
    ;

basic_statement
    : ( variable_declaration_statement | expression ) 
    ;

variable_declaration_statement
    : ( identifier_list | variable_declaration |
      '(' variable_declaration_list ')' ) ( '=' expression )?
    ;

variable_declaration_list
  : variable_declaration? (',' variable_declaration? )* ;

variable_declaration
  : real_base_type  IDENTIFIER 
  ;  

identifier_list
    : '(' ( IDENTIFIER? ',' )* IDENTIFIER? ')' 
    ;

swap_command
    : 'swap' IDENTIFIER IDENTIFIER ((AT (dbl | integer)) | ONCE) 
    ;

key_type
   :  KEY_ID | KEY_DESC | KEY_COMMENT
   ;

list_kv
   : '[' kv + ']'
   ;

kv
   : key value
   ;

value
   : integer
   | realnum
   | dbl
   | stringliteral
   | list_kv
   ;

key
   : key_type
   ;

realnum
   : REAL
   ;

stringliteral
   : STRINGLITERAL
   ;

STRINGLITERAL
   : '"' ~ '"'* '"'
   ;

REAL
   : SIGN? DIGIT* '.' DIGIT + MANTISSA?
   ;

SIGN
   : '+' | '-'
   ;

MANTISSA
   : 'E' SIGN DIGIT
   ;

KEY_ID: 'id';
KEY_DESC: 'description';
KEY_COMMENT: 'comment';

SEND_RECEIVE
   : 'send' | 'receive'
   ;

IN_OUT
   : 'in' | 'out'
   ;

UPDATE
   : 'update'
   ;

DIAGNOSTICS
   : 'diagnostics'
   ;

PACKAGE
   : 'package'
   ;

SYSTEM
   : 'system'
   ;

AT
   : 'at'
   ;

ONCE
   : 'once'
   ;

SINGLE_PERIODIC
   : 'single' | 'periodic'
   ;

DO
   : 'do'
   ;

ENDWHILE
   : 'endwhile'
   ;

ENDIF
   : 'endif'
   ;

THEN
   : 'then'
   ;

ELSE
   : 'else'
   ;

TYPE_BOOL: 'bool';
TYPE_BYTE: 'byte';
TYPE_CHAR: 'char';
TYPE_INT16: 'int16';
TYPE_UINT16: 'uint16';
TYPE_INT32: 'int32';
TYPE_UINT32: 'uint32';
TYPE_INT64: 'int64';
TYPE_UINT64: 'uint64';
TYPE_BINARY: 'binary';
TYPE_FLOAT32: 'float32';
TYPE_FLOAT64: 'float64';
TYPE_DOUBLE: 'double';
TYPE_STRING: 'string';

LITERAL
    : (('"' ~'"'* '"') | ('\'' ~'\''* '\''))
    ;

IDENTIFIER
    : (LETTER | '_') (LETTER | DIGIT | '.' | '_')*
    ;

COMMA
    : ','
    ;

fragment LETTER
    : 'A'..'Z' | 'a'..'z'
    ;

fragment DIGIT
    : '0'..'9'
    ;

fragment HEX_DIGIT
    : DIGIT | 'A'..'F' | 'a'..'f'
    ;

WS
    : (' ' | '\t' | '\r' '\n' | '\n')+ -> channel(HIDDEN)
    ;

SL_COMMENT
    : ('//' | '#') (~'\n')* ('\r')? '\n' -> channel(HIDDEN)
    ;

ML_COMMENT
    : '/*' .*? '*/' -> channel(HIDDEN)
    ;