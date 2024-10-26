#ifndef ast_WhileStatement_hpp
#define ast_WhileStatement_hpp

#include <iostream>
#include <vector>

class WhileStatement : public Node
{
public:
    WhileStatement(NodePointer _condition, NodePointer _stmt)
    {
        children.push_back(_condition);
        children.push_back(_stmt);
    }

    ~WhileStatement()
    {
        delete children[0];
        delete children[1];
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << " WHILE -> CONDITION: ";
        children[0]->Print(dst);
        dst << " STMT: ";
        children[1]->Print(dst);
        dst << std::endl;
    }

    virtual int offset() const override
    {
        return children[1]->offset();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        std::string break1 = context.make_name("break");
        context.break_labels.push_back(break1);
        std::string cont = context.make_name("continue");
        context.cont_labels.push_back(cont);
        std::string loop = context.make_name("loop");
        std::string whiles = context.make_name("while");
        dst << " j " << loop << std::endl;
        dst << whiles << ":" << std::endl;
        children[1]->riscv(dst, context, destreg);
        dst << cont << ":" << std::endl;
        dst << loop << ":" << std::endl;
        destreg = context.cregfile.check_reg();
        context.cregfile.occ_reg(destreg);
        children[0]->riscv(dst, context, destreg);
        dst << " bne x" << destreg << ", zero, " << whiles << std::endl;
        dst <<  break1 << ":" << std::endl;
        context.break_labels.pop_back();
        context.cont_labels.pop_back();
        context.cregfile.free_reg(destreg);
    }
};

#endif
