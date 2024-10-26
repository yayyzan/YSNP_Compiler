#ifndef ast_VEnumerate_hpp
#define ast_VEnumerate_hpp

#include <iostream>
#include <vector>

class VEnumerate : public Node
{
public:
    std::string name;

    VEnumerate(std::string _name)
    {
        name = _name;
    }

    VEnumerate(std::string _name, NodePointer val)
    {
        name = _name;
        children.push_back(val);
    }

    ~VEnumerate()
    {
        for (unsigned i = 0; i < children.size(); i++)
        {
            delete children[i];
        }
    }

    virtual void Print(std::ostream &dst) const override
    {
        if (children.size() == 0)
        {
            dst << " NAME: " << name << " VALUE: INDEXED " << std::endl;
        }
        else
        {
            dst << " NAME: " << name;
            dst << " VALUE: ";
            children[0]->Print(dst);
        }
    }

    virtual std::string findname() const override{
        return name;
    }

     virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        if (children.size()==0){
            context.stack.add_symbol(name, 0, 0, "enum");
        }
        else{
            context.stack.add_symbol(name, children[0]->evalconst(), 0, "enum");
        }
    }

    virtual void riscvwsymbol(std::ostream &dst, Context &context, int destreg, std::string pointer) const override
    {
        int temp = context.stack.find_symbol(pointer)->val;
        if (children.size()==0){
            context.stack.add_symbol(name, temp + 1, 0, "enum");
        }
        else{
            context.stack.add_symbol(name, children[0]->evalconst(), 0, "enum");
        }
    }
};

#endif
