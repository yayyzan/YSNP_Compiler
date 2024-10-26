#ifndef ast_Default_hpp
#define ast_Default_hpp

#include <iostream>
#include <vector>


class Default : public Node
{
public:

    Default(NodePointer _default)
    {
        children.push_back(_default);
    }

    ~Default()
    {
       delete children[0];
    }

    virtual void Print(std::ostream &dst) const override
    {

        dst << " Default -> ";
        children[0]->Print(dst);

    }

    virtual int offset() const override
    {
        return children[0]->offset();
    }

    virtual NodePointer checkcase() const override{
        return children[0];
    }

    virtual void riscvwsymbol(std::ostream &dst, Context &context, int destreg, std::string pointer) const override
    {
        dst << " j " << pointer << std::endl;
    }
};

#endif
