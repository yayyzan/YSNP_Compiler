#ifndef ast_SizeOf_hpp
#define ast_SizeOf_hpp

#include <iostream>
#include <string>

class SizeOf : public Node
{
private:
    std::string type;

public:
    SizeOf(std::string _type) : type(_type)
    {
    }

    SizeOf(NodePointer _node)
    {
        children.push_back(_node);
    }

    ~SizeOf()
    {
        if (children.size() > 0)
        {
            delete children[0];
        }
    }

    virtual void Print(std::ostream &dst) const override
    {
        if (children.size() == 0)
        {
            dst << " SIZEOF " << type;
        }
        else
        {
            dst << "SIZEOF";
            children[0]->Print(dst);
        }
    }

    virtual std::string findtype() const override
    {
        return type;
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {

        if (children.size() == 0)
        {
            if (type == "char")
            {
                dst << " li x" << destreg << ", 1" << std::endl;
            }
            else if (type == "double")
            {
                dst << " li x" << destreg << ", 8" << std::endl;
            }
            else
            {
                dst << " li x" << destreg << ", 4" << std::endl;
            }
        }
        else
        {
            if (children[0]->isarray())
            {
                dst << " li x" << destreg << ", " << 4 * children[0]->getarraysize() << std::endl;
            }
            else
            {
                dst << " li x" << destreg << ", 4" << std::endl;
            }
        }
    }
};

#endif
