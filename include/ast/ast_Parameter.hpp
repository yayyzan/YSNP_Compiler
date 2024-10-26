#ifndef ast_Parameter_hpp
#define ast_Parameter_hpp

#include <string>
#include <iostream>
#include <vector>

class Parameter : public Node
{
public:
    Parameter(NodePointer _type, NodePointer _name)
    {
        children.push_back(_type);
        children.push_back(_name);
    }

    virtual ~Parameter()
    {
        delete children[0];
        delete children[1];
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << " PARAM DECLERATION -> TYPE: ";
        children[0]->Print(dst);
        dst << " NAME: ";
        children[1]->Print(dst);
    }

    virtual std::string findtype() const override
    {
        return children[0]->findtype();
    }

    virtual bool ispointer() const override{
        return children[1]->ispointer();
    }

    virtual int offset() const override
    {
        if (children[0]->findtype() == "int"||children[0]->findtype() == "unsigned")
        {
            return 4;
        }
        return 0;
    }

    virtual std::string findname() const override
    {
        return children[1]->findname();
    }
};

#endif
