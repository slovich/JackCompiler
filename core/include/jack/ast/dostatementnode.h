#ifndef JACK_AST_DOSTATEMENTNODE_H
#define JACK_AST_DOSTATEMENTNODE_H

#include "jack/ast/statementnode.h"

#include <memory>

class SubroutineCallNode;

class DoStatementNode : public StatementNode {
public:
    DoStatementNode(
        const SourceSpan& sourceSpan,
        std::unique_ptr<SubroutineCallNode> call
        );

    ~DoStatementNode() override;

    const SubroutineCallNode& obterChamada() const;

private:
    std::unique_ptr<SubroutineCallNode> call;
};

#endif // JACK_AST_DOSTATEMENTNODE_H
