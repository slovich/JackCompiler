#include "jack/ast/classnode.h"
#include "jack/ast/variabledeclarationnode.h"
#include "jack/ast/subroutinedeclarationnode.h"
#include "jack/ast/parameternode.h"
#include "jack/ast/letstatementnode.h"
#include "jack/ast/dostatementnode.h"
#include "jack/ast/returnstatementnode.h"
#include "jack/ast/arrayaccessnode.h"
#include "jack/ast/variablereferencenode.h"
#include "jack/ast/stringliteralnode.h"
#include "jack/ast/subroutinecallnode.h"
#include "jack/ast/ifstatementnode.h"
#include "jack/ast/whilestatementnode.h"
#include "jack/ast/keywordconstantnode.h"
#include "jack/vm/vmcodegenerator.h"
#include <memory>
#include <sstream>
#include <iostream>
static SourceSpan s{4,5,4,6};
int main(){
 ClassNode c(s,"C");
 c.adicionarVariavelClasse(std::make_unique<VariableDeclarationNode>(s,VariableKind::Field,"Array",std::vector<std::string>{"a"}));
 auto m=std::make_unique<SubroutineDeclarationNode>(s,SubroutineKind::Method,"void","work");
 m->adicionarParametro(std::make_unique<ParameterNode>(s,"int","i"));
 m->adicionarParametro(std::make_unique<ParameterNode>(s,"int","j"));
 m->adicionarVariavelLocal(std::make_unique<VariableDeclarationNode>(s,VariableKind::Var,"int",std::vector<std::string>{"v"}));
 m->adicionarVariavelLocal(std::make_unique<VariableDeclarationNode>(s,VariableKind::Var,"Widget",std::vector<std::string>{"obj"}));
 m->adicionarInstrucao(std::make_unique<LetStatementNode>(s,
   std::make_unique<ArrayAccessNode>(s,"a",std::make_unique<VariableReferenceNode>(s,"i")),
   std::make_unique<ArrayAccessNode>(s,"a",std::make_unique<VariableReferenceNode>(s,"j"))));
 m->adicionarInstrucao(std::make_unique<LetStatementNode>(s,std::make_unique<VariableReferenceNode>(s,"v"),std::make_unique<StringLiteralNode>(s,"Hi")));
 auto objCall=std::make_unique<SubroutineCallNode>(s,std::optional<std::string>("obj"),"m"); objCall->adicionarArgumento(std::make_unique<VariableReferenceNode>(s,"v"));
 m->adicionarInstrucao(std::make_unique<DoStatementNode>(s,std::move(objCall)));
 auto clsCall=std::make_unique<SubroutineCallNode>(s,std::optional<std::string>("Math"),"multiply"); clsCall->adicionarArgumento(std::make_unique<VariableReferenceNode>(s,"i")); clsCall->adicionarArgumento(std::make_unique<VariableReferenceNode>(s,"j"));
 m->adicionarInstrucao(std::make_unique<DoStatementNode>(s,std::move(clsCall)));
 auto selfCall=std::make_unique<SubroutineCallNode>(s,std::nullopt,"other"); selfCall->adicionarArgumento(std::make_unique<VariableReferenceNode>(s,"i"));
 m->adicionarInstrucao(std::make_unique<DoStatementNode>(s,std::move(selfCall)));
 auto iff=std::make_unique<IfStatementNode>(s,std::make_unique<KeywordConstantNode>(s,KeywordConstant::True));
 iff->adicionarInstrucaoThen(std::make_unique<ReturnStatementNode>(s)); iff->definirPossuiElse(); iff->adicionarInstrucaoElse(std::make_unique<ReturnStatementNode>(s));
 m->adicionarInstrucao(std::move(iff));
 auto wh=std::make_unique<WhileStatementNode>(s,std::make_unique<KeywordConstantNode>(s,KeywordConstant::False)); wh->adicionarInstrucao(std::make_unique<ReturnStatementNode>(s)); m->adicionarInstrucao(std::move(wh));
 c.adicionarSubrotina(std::move(m));
 VMCodeGenerator g; std::ostringstream out; auto r=g.generate(c,out); if(!r.success){for(auto&d:r.diagnostics)std::cerr<<d.message<<"\n";return 1;}
 const auto x=out.str();
 const char* fragments[]={"pop temp 0\npop pointer 1\npush temp 0\npop that 0\n", "push constant 2\ncall String.new 1\npush constant 72\ncall String.appendChar 2\npush constant 105\ncall String.appendChar 2\npop local 0\n", "call Widget.m 2\npop temp 0\n", "call Math.multiply 2\npop temp 0\n", "call C.other 2\npop temp 0\n", "label IF_FALSE0", "label WHILE_EXP2"};
 for(auto f:fragments) if(x.find(f)==std::string::npos){std::cerr<<"Missing: "<<f<<"\n"<<x;return 2;}
 std::cout<<"Extended VMCodeGenerator checks: PASS\n";
}
