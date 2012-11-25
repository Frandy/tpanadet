


%%

all_line:
	| EOL
	| all_line? elem_line
	| all_line? cmd_line

elem_line:
	ckt_elem EOL
	
ckt_elem:
	twonode_elem
	| fournode_elem
	| x_elem
	
/*
how to use ? after token 
*/	
	
	NetlistDriver* driver;
	
twonode_elem:
	rcls_name node node param  fault?  {driver->AddRCLs(name,p,n,param,fault);}
	| source_name node node dc_param? ac_param? tran_param?	 {driver->AddSource(name,p,n,source_param)}

rcls_name:
	[rcls](TEXT|INT)?

	
fournode_elem:
	efgh_elem node node node node param
	| o_elem node node OPAMP node node
	| mos_elem node node node node model model_param fault?	

node:
	text
	| INT

x_elem:
	x_name node+ ( param* )


%union{
	FaultType tFaultType
}

%type <tFaultType> fault

fault:
	/* empty */ { $$ = FaultType::NONE }
	| fault_equal? SHORT { $$ = FaultType::SHORT }
	| fault_equal? OPEN  { $$ = FaultType::OPEN }

fault_equal:
	FAULT EQUAL	
	
%union{
	ParamExpr* pParamExpr;
};
	
%type <pParamExpr> param expr term factor dc_param ac_param tran_param	
	
dc_param:	
	DC? param
ac_param:
	AC? param
tran_param:
	SIN_COS LBRACK V0 A0 w phi RBRACK	/* V0 + A0 sin( w *t + phi ) */	

// another kind of sin function, put time variable in paramMap	
	
param:
	value
	| LQUOTA expr RQUOTA 		

value:
	INT
	| FLOATU	
		
expr:
	expr + term
	| expr - term

term : 
	term * factor
	| term / factor
	
factor:
	value
	| text
	| MINUS factor
	| LBRACK expr RBRACK
	| func

/* func_name LBRACK func_param RBRACK */	
func:
	UNIFORM LBRACK func_param COMMA func_param RBRACK  /* uniform ( a, b ) */
	| GAUSS LBARCK func_param COMMA func_param COMMA func_param RBRACK /* gauss ( u, d, b) */	
/*
	| ABS LBRACK func_param RBRACK
	| SIN LBRACK func_param RBRACK
	| LIN LBRACK func_param COMMA func_param COMMA func_param RBRACK  /* line ( a, b, step) */ /* similar with linear sweep */
	| LAPLACE LSBRACK func_param+ RSBRACK LSBRACK func_param+ RSBRACK /* laplace numerator / denominator s-coef */
*/

func_param:
	expr

cmd_line:
	dot_cmd EOL
	
dot_cmd:
	dot_dc
	| dot_ac
	| dot_tran
	| dot_subckt_begin
	| dot_subckt_end
	| dot_param
	
doc_dc:
	DOTDC elem_name value value value 
	| DOTDC elem_name value value value  SWEEP elem_name value value

dot_ac:
	DOTAC sample value value value
	| DOTAC sample value value value SWEEP value	

sample:
	LIN
	| DEC 	

dot_subckt_begin:
	DOTSUBCKT subckt_name node+ 			{ driver->AddSubckt();subcktStack.push();}
	| DOTSUBCKT subckt_name node+ [ key_default_param ]

%type <pParamExpr> key_default_param
	
key_default_param:
	TEXT
	| TEXT EQUAL value	

dot_subckt_end:
	DOTENDS 				{ driver->subcktStack.pop_all();}
	| DOTENDS subckt_name 	{ driver->subcktStack.pop_();}


%%