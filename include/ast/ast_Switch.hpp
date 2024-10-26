#ifndef ast_Switch_hpp
#define ast_Switch_hpp

#include <iostream>
#include <vector>

class Switch : public Node
{
public:
    Switch(NodePointer _check, NodePointer _stmt)
    {
        children.push_back(_check);
        children.push_back(_stmt);
    }

    ~Switch()
    {
        delete children[0];
        delete children[1];
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << " SWITCH -> CHECKING: ";
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
       int destreg1 = context.cregfile.check_reg();
       context.cregfile.occ_reg(destreg1);
       children[0]->riscv(dst, context, destreg1);
       children[1]->riscvwsymbol(dst, context, destreg, std::to_string(destreg1));
       dst << break1 << ":" << std::endl;
       context.break_labels.pop_back();
       context.cregfile.free_reg(destreg1);
    }
};

#endif
