#ifndef ast_Break_hpp
#define ast_Break_hpp

#include <iostream>
#include <string>

class Break : public Node
{
public:
    Break()
    {}

    virtual void Print(std::ostream &dst) const override
    {
        dst << "BREAK" << std::endl;
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override{
        dst << " j " << context.break_labels[context.break_labels.size()-1] << std::endl;
    }
};

#endif
