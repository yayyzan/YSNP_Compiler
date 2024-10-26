#ifndef ast_Case_hpp
#define ast_Case_hpp

#include <iostream>
#include <vector>

class Case : public Node
{
public:
    Case(NodePointer _check, NodePointer _stmt)
    {
        children.push_back(_check);
        children.push_back(_stmt);
    }

    ~Case()
    {
        delete children[0];
        delete children[1];
    }

    virtual void Print(std::ostream &dst) const override
    {

        dst << " CASE -> CHECKING FOR: ";
        children[0]->Print(dst);
        dst << " STMT: ";
        children[1]->Print(dst);
        dst << std::endl;

    }

    virtual int offset() const override
    {
        return children[1]->offset();
    }

    virtual NodePointer checkcase() const override{
        return children[1];
    }

    virtual void riscvwsymbol(std::ostream &dst, Context &context, int destreg, std::string pointer) const override
    {
        std::string next = context.make_name("next");
        // context.case_labels.push_back(next);
        context.case_labels[context.case_labels.size()-1].push_back(next);
        std::cout << "now adding label " << next << " into vector" << std::endl;
        int temp = context.cregfile.check_reg();
        context.cregfile.occ_reg(temp);
        children[0]->riscv(dst, context, temp);
        dst << " beq x" << std::stoi(pointer) << ", x" << temp << ", " << next << std::endl;
        context.cregfile.free_reg(temp);
    }
};

#endif
