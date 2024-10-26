#ifndef ast_Continue_hpp
#define ast_Continue_hpp

#include <iostream>
#include <string>

class Continue : public Node
{
public:
    Continue()
    {}

    virtual void Print(std::ostream &dst) const override
    {
        dst << "CONTINUE" << std::endl;
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override{
        dst << " j " << context.cont_labels[context.break_labels.size()-1] << std::endl;
    }
};

#endif
