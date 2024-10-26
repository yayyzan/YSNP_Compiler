#ifndef ast_FunctionCall_hpp
#define ast_FunctionCall_hpp

#include <iostream>
#include <vector>

class FunctionCall : public Node

{
public:
    FunctionCall(NodePointer _funcName)
    {
        children.push_back(_funcName);
    }

    FunctionCall(NodePointer _funcName, std::vector<NodePointer> _arguments)
    {
        children.push_back(_funcName);
        children.insert(children.end(), _arguments.begin(), _arguments.end());
    }

    ~FunctionCall()
    {
        for (unsigned i = 0; i < children.size(); i++)
        {
            delete children[i];
        }
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << " FUNCTION CALL -> NAME: ";
        children[0]->Print(dst);
    }

    virtual bool isfunction() const override
    {
        return 1;
    }

    virtual int offset() const override{
        if (children.size() > 1){
            if (children.size() > 9){
                return 4* (children.size()-9);
            }
            else{
                return 0;
            }
        }
        else{
                return 0;
            }
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        if (children.size() > 1)
        {
            for (unsigned i = 1; i < children.size(); i++)
            {
                if (i < 9){
                int temp = context.func_cregfile.check_reg();
                context.func_cregfile.occ_reg(temp);
                children[i]->riscv(dst, context, temp);
                }
                else{
                int temp = context.cregfile.check_reg();
                context.cregfile.occ_reg(temp);
                children[i]->riscv(dst, context, temp);
                dst << " sw x" << temp << ", " << (i-9)*4 << "(sp)" << std::endl;
                context.cregfile.free_reg(temp);
                }
            }
            context.func_cregfile.free_all();
        }
        dst << " jal " << children[0]->findname() << std::endl;
        dst << " mv x" << destreg << ", x10" << std::endl;
    }
};

#endif
