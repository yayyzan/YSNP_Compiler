#ifndef ast_Return_hpp
#define ast_Return_hpp

#include "ast_context.hpp"
#include <iostream>

class Return : public Node
{
public:
    Return(NodePointer _expression)
    {
        children.push_back(_expression);
    }

    ~Return()
    {
        delete children[0];
    }

    virtual int offset() const override{
        if(children[0]->isfunction()){
            return children[0]->offset();
        }
        else{
            return 0;
        }
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << " RETURN ";
        children[0]->Print(dst);
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        children[0]->riscv(dst, context, destreg);
        dst << " j " << context.return_label << std::endl;
    }
};

#endif
