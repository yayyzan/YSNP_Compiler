#ifndef ast_Dereference_hpp
#define ast_Dereference_hpp

#include <iostream>
#include <string>

class Dereference : public Node
{

public:
    Dereference(NodePointer _node)
    {
        children.push_back(_node);
    }

    ~Dereference()
    {
        if (children.size() > 0)
        {
            delete children[0];
        }
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << " DEREFERENCE ";
    }
    virtual std::string findname() const override
    {
        return children[0]->findname();
    }
    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        dst << " lw x" << destreg << ", " << context.stack.find_symbol(children[0]->findname())->offset << "(s0)" << std::endl;
        dst << " lw x" << destreg << ", 0(x" << destreg << ")" << std::endl;
    }
};

#endif
