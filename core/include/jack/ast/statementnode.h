#ifndef JACK_AST_STATEMENTNODE_H
#define JACK_AST_STATEMENTNODE_H

#include "jack/ast/astnode.h"

class StatementNode : public ASTNode {
public:
    explicit StatementNode(const SourceSpan& sourceSpan);

    ~StatementNode() override = default;
};

#endif // JACK_AST_STATEMENTNODE_H
