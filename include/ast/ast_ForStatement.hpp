#ifndef ast_ForStatement_hpp
#define ast_ForStatement_hpp

#include <string>
#include <iostream>
#include <vector>

class ForStatement : public Node
{
public:
    ForStatement(NodePointer _condition, NodePointer _stmt, NodePointer _process)
    {
        children.push_back(_condition);
        children.push_back(_stmt);
        children.push_back(_process);
    }

    ForStatement(NodePointer _declaration, NodePointer _condition, NodePointer _stmt, NodePointer _process)
    {
        children.push_back(_condition);
        children.push_back(_stmt);
        children.push_back(_process);
        children.push_back(_declaration);
    }

    ~ForStatement()
    {
        if (children.size() == 4)
        {
            delete children[3];
        }
        delete children[0];
        delete children[1];
        delete children[2];
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << " FOR LOOP -> ";
        if (children.size() == 4)
        {
            dst << " DECLARATION: ";
            children[3]->Print(dst);
        }
        dst << " CONDITION: ";
        children[0]->Print(dst);
        dst << " STMT: ";
        children[1]->Print(dst);
        dst << " BODY: ";
        children[2]->Print(dst);
    }

    virtual int offset() const override
    {
        return children[2]->offset();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        std::string loop = context.make_name("loop");
        std::string begin = context.make_name("begin");
        std::string break1 = context.make_name("break");
        std::string cont = context.make_name("continue");
        context.cont_labels.push_back(cont);
        context.break_labels.push_back(break1);
        dst << " nop" << std::endl;
        if (children.size() == 4)
        {
            children[3]->riscv(dst, context, destreg);
        }
        dst << " j " << begin << std::endl;
        dst << loop << ":" << std::endl;
        children[2]->riscv(dst, context, destreg);
        dst << cont << ":" << std::endl;
        children[1]->riscv(dst, context, destreg);
        dst << begin << ":" << std::endl;
        int temp = context.cregfile.check_reg();
        context.cregfile.occ_reg(temp);
        children[0]->riscv(dst, context, destreg);
        dst << " bne x" << destreg << ", zero, " << loop << std::endl;
        dst << break1 << ":"  << std::endl;
        context.break_labels.pop_back();
        context.cregfile.free_reg(temp);
    }
};

#endif
