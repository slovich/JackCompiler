#ifndef JACK_AST_SUBROUTINECALLNODE_H
#define JACK_AST_SUBROUTINECALLNODE_H

#include "jack/ast/expressionnode.h"

#include <memory>
#include <optional>
#include <string>
#include <vector>

class SubroutineCallNode : public ExpressionNode {
public:
    SubroutineCallNode(
        const SourceSpan& sourceSpan,
        const std::optional<std::string>& qualifier,
        const std::string& subroutineName
        );

    ~SubroutineCallNode() override;

    const std::optional<std::string>& obterQualificador() const;
    const std::string& obterNomeSubrotina() const;

    void adicionarArgumento(std::unique_ptr<ExpressionNode> argumento);

    const std::vector<std::unique_ptr<ExpressionNode>>& obterArgumentos() const;

private:
    std::optional<std::string> qualifier;
    std::string subroutineName;
    std::vector<std::unique_ptr<ExpressionNode>> arguments;
};

#endif // JACK_AST_SUBROUTINECALLNODE_H
