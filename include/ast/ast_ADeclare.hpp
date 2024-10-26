#ifndef ast_ADeclare_hpp
#define ast_ADeclare_hpp

#include <iostream>
#include <string>
#include <vector>
class ADeclare : public Node
{
public:
    ADeclare(NodePointer _name)
    {
        children.push_back(_name);
    }

    ADeclare(NodePointer _name, NodePointer _size)
    {
        children.push_back(_name);
        children.push_back(_size);
    }

    ~ADeclare()
    {
        for (unsigned i = 0; i < children.size(); i++)
        {
            delete children[i];
        }
    }

    virtual void Print(std::ostream &dst) const override
    {
        if (children.size() == 2)
        {
            dst << " SIZE: ";
            children[1]->Print(dst);
            dst << std::endl;
        }
        else
        {
            dst << " SIZE: not defined " << std::endl;
        }
    }

    virtual std::string findname() const override
    {
        return children[0]->findname();
    }

    virtual bool isarray() const override
    {
        return 1;
    }

    virtual int getarraysize() const override
    {
        if (children.size() == 2)
        {
            return children[1]->evalconst();
        }
        else
        {
            return 0;
        }
    }

};

#endif
