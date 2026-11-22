#include "jack/ast/classnode.h"
#include "jack/ast/parameternode.h"
#include "jack/ast/variabledeclarationnode.h"
#include "jack/ast/subroutinedeclarationnode.h"
#include "jack/ast/letstatementnode.h"
#include "jack/ast/returnstatementnode.h"
#include "jack/ast/integerliteralnode.h"
#include "jack/ast/binaryexpressionnode.h"
#include "jack/ast/variablereferencenode.h"
#include "jack/ast/keywordconstantnode.h"
#include "jack/ast/arrayaccessnode.h"
#include "jack/ast/stringliteralnode.h"
#include "jack/ast/subroutinecallnode.h"
#include "jack/vm/vmcodegenerator.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <stdexcept>

static SourceSpan sp{1,1,1,1};
int main() {
  ClassNode klass(sp,"Calc");
  klass.adicionarVariavelClasse(std::make_unique<VariableDeclarationNode>(sp,VariableKind::Field,"int",std::vector<std::string>{"fieldA","fieldB"}));
  klass.adicionarVariavelClasse(std::make_unique<VariableDeclarationNode>(sp,VariableKind::Static,"int",std::vector<std::string>{"count"}));
  auto fn=std::make_unique<SubroutineDeclarationNode>(sp,SubroutineKind::Function,"int","compute");
  fn->adicionarVariavelLocal(std::make_unique<VariableDeclarationNode>(sp,VariableKind::Var,"int",std::vector<std::string>{"x"}));
  auto plus=std::make_unique<BinaryExpressionNode>(sp,std::make_unique<IntegerLiteralNode>(sp,2,"2"),BinaryOperator::Add,std::make_unique<IntegerLiteralNode>(sp,3,"3"));
  auto mult=std::make_unique<BinaryExpressionNode>(sp,std::move(plus),BinaryOperator::Multiply,std::make_unique<IntegerLiteralNode>(sp,4,"4"));
  fn->adicionarInstrucao(std::make_unique<LetStatementNode>(sp,std::make_unique<VariableReferenceNode>(sp,"x"),std::move(mult)));
  fn->adicionarInstrucao(std::make_unique<ReturnStatementNode>(sp,std::make_unique<VariableReferenceNode>(sp,"x")));
  klass.adicionarSubrotina(std::move(fn));
  auto ctor=std::make_unique<SubroutineDeclarationNode>(sp,SubroutineKind::Constructor,"Calc","new");
  ctor->adicionarInstrucao(std::make_unique<ReturnStatementNode>(sp,std::make_unique<KeywordConstantNode>(sp,KeywordConstant::This)));
  klass.adicionarSubrotina(std::move(ctor));
  auto method=std::make_unique<SubroutineDeclarationNode>(sp,SubroutineKind::Method,"void","set");
  method->adicionarParametro(std::make_unique<ParameterNode>(sp,"int","v"));
  method->adicionarInstrucao(std::make_unique<ReturnStatementNode>(sp));
  klass.adicionarSubrotina(std::move(method));
  VMCodeGenerator generator;
  std::ostringstream out;
  auto result=generator.generate(klass,out);
  if(!result.success) { for(auto& d:result.diagnostics) std::cerr<<d.message<<"\n"; return 1; }
  const std::string expected =
    "function Calc.compute 1\n"
    "push constant 2\n"
    "push constant 3\n"
    "add\n"
    "push constant 4\n"
    "call Math.multiply 2\n"
    "pop local 0\n"
    "push local 0\n"
    "return\n"
    "function Calc.new 0\n"
    "push constant 2\n"
    "call Memory.alloc 1\n"
    "pop pointer 0\n"
    "push pointer 0\n"
    "return\n"
    "function Calc.set 0\n"
    "push argument 0\n"
    "pop pointer 0\n"
    "push constant 0\n"
    "return\n";
  if(out.str()!=expected) { std::cerr<<out.str(); return 2; }
  // Falha não deve publicar conteúdo parcial.
  ClassNode invalid(sp,"Bad");
  auto bad=std::make_unique<SubroutineDeclarationNode>(sp,SubroutineKind::Function,"void","f");
  bad->adicionarInstrucao(std::make_unique<ReturnStatementNode>(sp,std::make_unique<KeywordConstantNode>(sp,KeywordConstant::This)));
  invalid.adicionarSubrotina(std::move(bad));
  std::ostringstream untouched;
  auto failure=generator.generate(invalid,untouched);
  if(failure.success || !untouched.str().empty() || failure.diagnostics.empty()) return 3;
  std::cout << "VMCodeGenerator smoke checks: PASS\n";
}
