#ifndef ast_IfElseStatement_hpp
#define ast_IfElseStatement_hpp

#include <iostream>
#include <vector>

class IfElseStatement : public Node
{
public:
    IfElseStatement(NodePointer _condition, NodePointer _if, NodePointer _else)
    {
        children.push_back(_condition);
        children.push_back(_if);
        children.push_back(_else);
    }

    IfElseStatement(NodePointer _condition, NodePointer _if)
    {
        children.push_back(_condition);
        children.push_back(_if);
    }

    ~IfElseStatement()
    {
        if (children.size() == 3)
        {
            delete children[2];
        }
        delete children[0];
        delete children[1];
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << " IF ELSE -> CONDITION: ";
        children[0]->Print(dst);
        dst << " IF: ";
        children[1]->Print(dst);
        if (children.size() == 3)
        {
            dst << " ELSE: ";
            children[2]->Print(dst);
        }
    }

    virtual int offset() const override
    {
        if (children.size() == 3)
        {
            return children[1]->offset() + children[2]->offset();
        }
        else
        {
            return children[1]->offset();
        }
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        std::string continuec = context.make_name("cont");
        std::string elsec = context.make_name("else");
        if (children.size() == 3)
        {
            children[0]->riscv(dst, context, destreg);
            dst << " beq x" << destreg << ", zero, " << elsec << std::endl;
            children[1]->riscv(dst, context, destreg);
            dst << " j " << continuec << std::endl;
            dst << elsec << ":" << std::endl;
            children[2]->riscv(dst, context, destreg);
            dst << continuec << ":" << std::endl;
        }
        else if (children.size() == 2)
        {
            children[0]->riscv(dst, context, destreg);
            dst << " beq x" << destreg << ", zero, " << continuec << std::endl;
            children[1]->riscv(dst, context, destreg);
            dst << continuec << ":" << std::endl;
        }
        else
        {
            dst << "error in if/else statements please check" << std::endl;
        }
    }
};

#endif
