#ifndef ast_VDeclarator_hpp
#define ast_VDeclarator_hpp

#include <string>
#include <iostream>
#include <vector>
class VDeclarator : public Node
{
public:
    VDeclarator(NodePointer _variable)
    {
        children.push_back(_variable);
    }

    virtual ~VDeclarator()
    {
        delete children[0];
    }

    virtual bool isarray() const override
    {
        return children[0]->isarray();
    }

    virtual std::string findname() const override
    {
        return children[0]->findname();
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << children[0]->findname();
        if (isarray())
        {
            children[0]->Print(dst);
        }
    }

    virtual int getarraysize() const override
    {
        return children[0]->getarraysize();
    }

     virtual int evalconst() const override
    {
        return 0;
    }

    virtual bool isfunction() const override{
        return children[0]->isfunction();
    }
    
    virtual bool ispointer() const override{
        return children[0]->ispointer();
    }


    virtual bool isinit() const override
    {
        return 0;
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        dst << ".data" << std::endl;
        dst << std::endl;
        dst << children[0]->findname() << ":" << std::endl;
        if (children[0]->isarray())
        {
            if (context.stack.find_symbol(children[0]->findname())->type == "int" || context.stack.find_symbol(children[0]->findname())->type == "unsigned")
            {
                dst << " .zero " << getarraysize() * 4 << std::endl;
            }
        }
        else
        {
            if (context.stack.find_symbol(children[0]->findname())->type == "int" || context.stack.find_symbol(children[0]->findname())->type == "unsigned" )
            {
                dst << " .zero 4" << std::endl;
            }
        }
        dst << std::endl;
    }
};

#endif
