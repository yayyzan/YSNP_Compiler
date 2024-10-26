#ifndef ast_ManipulateArray_hpp
#define ast_ManipulateArray_hpp

#include <iostream>
#include <string>
#include <vector>

class ManipulateArray : public Node
{
public:
    ManipulateArray(NodePointer _array, NodePointer _index)
    {
        children.push_back(_array);
        children.push_back(_index);
    }

    ~ManipulateArray()
    {
        for (unsigned i = 0; i < children.size(); i++)
        {
            delete children[i];
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

    virtual NodePointer getindex() const override
    {
        return children[1];
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << "ARRAY INDEXING: ";
        dst << " NAME: " << findname();
        dst << " INDEX: ";
        children[1]->Print(dst);
        dst << std::endl;
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int temp = context.cregfile.check_reg();
        context.cregfile.occ_reg(temp);
        if (context.stack.check_global(children[0]->findname()))
        {
            dst << " lui x" << temp << ", %hi(" << children[0]->findname() << ")" << std::endl;
            dst << " addi x" << temp << ", x" << temp << ", %lo(" << children[0]->findname() << ")" << std::endl;
            if (context.stack.find_symbol(children[0]->findname())->type == "int" || context.stack.find_symbol(children[0]->findname())->type == "unsigned")
            {
                int temp2 = context.cregfile.check_reg();
                context.cregfile.occ_reg(temp2);
                children[1]->riscv(dst, context, temp2);
                dst << " slli x" << temp2 << ", x" << temp2 << ", 2" << std::endl;
                dst << " add x" << temp2 << ", x" << temp2 << ", x" << temp << std::endl;
                dst << " lw x" << destreg << ", 0(x" << temp2 << ")" << std::endl;
                context.cregfile.free_reg(temp2);
            }
        }
        else
        {
            children[1]->riscv(dst, context, temp);
            if (context.stack.find_symbol(children[0]->findname())->type == "int" || context.stack.find_symbol(children[0]->findname())->type == "unsigned")
            {
                dst << " slli x" << temp << ", x" << temp << ", 2" << std::endl;
                int temp1 = context.cregfile.check_reg();
                context.cregfile.occ_reg(temp1);
                dst << " li x" << temp1 << ", " << context.stack.find_symbol(children[0]->findname())->offset << std::endl;
                dst << " add x" << temp << ", x" << temp << ", x" << temp1 << std::endl;
                context.cregfile.free_reg(temp1);
                dst << " add x" << temp << ", x" << temp << ", s0" << std::endl;
                dst << " lw x" << destreg << ", 0"
                    << "(x" << temp << ")" << std::endl;
            }
            else
            {
                dst << " slli x" << temp << ", x" << temp << ", 2" << std::endl;
                int temp1 = context.cregfile.check_reg();
                context.cregfile.occ_reg(temp1);
                dst << " lw x" << temp1 << ", " << context.stack.find_symbol(children[0]->findname())->offset << "(s0)" << std::endl;
                dst << " add x" << temp << ", x" << temp << ", x" << temp1 << std::endl;
                dst << " lw x" << destreg << ", 0"
                    << "(x" << temp << ")" << std::endl;
                context.cregfile.free_reg(temp1);
            }
        }
        context.cregfile.free_reg(temp);
    }
};

#endif
