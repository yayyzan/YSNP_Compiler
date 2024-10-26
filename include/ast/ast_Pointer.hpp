#ifndef ast_Pointer_hpp
#define ast_Pointer_hpp

#include <iostream>
#include <string>

class Pointer : public Node
{

public:
    Pointer(NodePointer _node)
    {
        children.push_back(_node);
    }

    ~Pointer()
    {

        delete children[0];
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << " POINTER ";
    }

    virtual std::string findname() const override
    {
        return children[0]->findname();
    }

    virtual bool ispointer() const override
    {
        return 1;
    }
};

#endif
