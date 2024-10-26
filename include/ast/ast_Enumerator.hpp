#ifndef ast_Enumerator_hpp
#define ast_Enumerator_hpp

#include <iostream>
#include <vector>

class Enumerator : public Node
{
public:
    Enumerator(std::vector<NodePointer> _list)
        : Node(_list)
    {
    }

    Enumerator()
    {
    }

    ~Enumerator()
    {
        for (unsigned i = 0; i < children.size(); i++)
        {
            delete children[i];
        }
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << " ENUMERATION " << std::endl;
        for (unsigned i = 0; i < children.size(); i++)
        {
            dst << " VARIABLE " << i << " : " << std::endl;
            children[i]->Print(dst);
            dst << std::endl;
        }
        dst << " END ENUMERATION " << std::endl;
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override{
        children[0]->riscv(dst, context, destreg);
        for (int i = 1; i < children.size(); i++){
            children[i]->riscvwsymbol(dst, context, destreg, children[i-1]->findname());
        }
    }

};

#endif
