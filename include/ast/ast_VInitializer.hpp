#ifndef ast_VInitializer_hpp
#define ast_VInitializer_hpp

#include <string>
#include <iostream>
#include <vector>

class VInitializer : public Node
{
public:
    VInitializer(NodePointer _variable, NodePointer _value)
    {
        children.push_back(_variable);
        children.push_back(_value);
    }

    virtual ~VInitializer()
    {
        delete children[0];
        delete children[1];
    }

    virtual bool isarray() const override
    {
        return children[0]->isarray();
    }

    virtual bool ispointer() const override
    {
        return children[0]->ispointer();
    }


    virtual void Print(std::ostream &dst) const override
    {
        if (isarray())
        {
            dst << children[0]->findname();
        }
        children[0]->Print(dst);
        dst << " VALUE: ";
        children[1]->Print(dst);
    }

    virtual std::string findname() const override
    {
        return children[0]->findname();
    }

    virtual int getarraysize() const override
    {
        return children[0]->getarraysize();
    }

      virtual int evalconst() const override
    {
        return children[1]->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        if (isarray())
        {
            std::string pointer = findname();
            children[1]->riscvwsymbol(dst, context, destreg, pointer);
        }
        else if (context.stack.is_global())
        {
            dst << ".data" << std::endl << std::endl;
            dst << findname() << ":" << std::endl;
            dst << " .word " << context.stack.find_symbol(findname())->val << std::endl;
            dst << std::endl;
        }
        else
        {
            destreg = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg);
            children[1]->riscv(dst, context, destreg);
            int offset = context.stack.find_symbol(children[0]->findname())->offset;
            dst << " sw x" << destreg << ", " << offset << "(s0)" << std::endl;
            context.cregfile.free_reg(destreg);
        }
    }

    virtual bool isinit() const override
    {
        return 1;
    }
};

#endif
