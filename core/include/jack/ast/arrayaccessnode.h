#ifndef JACK_AST_ARRAYACCESSNODE_H
#define JACK_AST_ARRAYACCESSNODE_H

#include "jack/ast/expressionnode.h"

#include <memory>
#include <string>

class ArrayAccessNode : public ExpressionNode {
public:
    ArrayAccessNode(
        const SourceSpan& sourceSpan,
        const std::string& name,
        std::unique_ptr<ExpressionNode> index
        );

    ~ArrayAccessNode() override;

    const std::string& obterNome() const;
    const ExpressionNode& obterIndice() const;

private:
    std::string name;
    std::unique_ptr<ExpressionNode> index;
};

#endif // JACK_AST_ARRAYACCESSNODE_H
