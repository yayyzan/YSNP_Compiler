#ifndef ast_Address_hpp
#define ast_Address_hpp

#include <iostream>
#include <string>

class Address : public Node
{

public:
    Address(NodePointer _node)
    {
        children.push_back(_node);
    }

    ~Address()
    {
        delete children[0];
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << " ADDRESS ";
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        dst << " addi x" << destreg << ", "
            << "s0, " << context.stack.find_symbol(children[0]->findname())->offset << std::endl;
    }
};

#endif
