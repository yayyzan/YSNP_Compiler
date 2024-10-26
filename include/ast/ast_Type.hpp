#ifndef ast_Type_hpp
#define ast_Type_hpp

#include <iostream>
#include <string>

class Type : public Node
{
private:
    std::string type;

public:
    Type(std::string _type) : type(_type)
    {
    }

    Type(NodePointer _node){
        children.push_back(_node);
    }

    ~Type(){
        if (children.size() > 0){
            delete children[0];
        }
    }

    virtual void Print(std::ostream &dst) const override
    {
        if(children.size()>0){
            children[0]->Print(dst);
        }
        dst << type;
    }

    virtual std::string findtype() const override
    {
        return type;
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override{
        if(children.size() != 0){
            children[0]->riscv(dst, context, destreg);
        }
    }
};

#endif
