#ifndef JACK_AST_PROGRAMNODE_H
#define JACK_AST_PROGRAMNODE_H

#include "jack/ast/astnode.h"

#include <memory>
#include <vector>

class ClassNode;

class ProgramNode : public ASTNode {
public:
    explicit ProgramNode(const SourceSpan& sourceSpan);

    void adicionarClasse(std::unique_ptr<ClassNode> classe);

    const std::vector<std::unique_ptr<ClassNode>>& obterClasses() const;

private:
    std::vector<std::unique_ptr<ClassNode>> classes;
};

#endif // JACK_AST_PROGRAMNODE_H
