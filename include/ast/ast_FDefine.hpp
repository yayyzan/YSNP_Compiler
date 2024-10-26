#ifndef ast_FDefine_hpp
#define ast_FDefine_hpp

#include "ast_context.hpp"

#include <iostream>
#include <vector>
class FDefine : public Node
{
public:
    FDefine(NodePointer _arg1, NodePointer _arg2, NodePointer _arg3)
    {
        children.push_back(_arg1);
        children.push_back(_arg2);
        children.push_back(_arg3);
    }
    ~FDefine()
    {
        delete children[0];
        delete children[1];
        delete children[2];
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << std::endl;
        dst << " FUNCTION -> TYPE: ";
        children[0]->Print(dst);
        dst << " NAME: ";
        children[1]->Print(dst);
        dst << " BODY: ";
        dst << "\n";
        children[2]->Print(dst);
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        context.return_label = context.make_name("return");
        std::string temp = context.return_label;
        int offsetting = children[1]->offset();
        offsetting += children[2]->offset();
        offsetting += 64;
        if (offsetting % 16 != 0)
        {

            offsetting = offsetting + (16 - (offsetting % 16));
        }
        context.stack_offset = offsetting;
        context.curr_offset = offsetting;
        dst << ".text" << std::endl;
        dst << ".globl " << children[1]->findname() << std::endl;
        dst << std::endl;

        dst << children[1]->findname() << ":" << std::endl;
        dst << " addi sp, sp, " << -offsetting << std::endl;
        dst << " sw ra, " << offsetting - 4 << "(sp)" << std::endl;
        dst << " sw s0, " << offsetting - 8 << "(sp)" << std::endl;
        dst << " sw x9, " << offsetting - 12 << "(sp)" << std::endl;
        dst << " sw x18, " << offsetting - 16 << "(sp)" << std::endl;
        dst << " sw x19, " << offsetting - 20 << "(sp)" << std::endl;
        dst << " sw x20, " << offsetting - 24 << "(sp)" << std::endl;
        dst << " sw x21, " << offsetting - 28 << "(sp)" << std::endl;
        dst << " sw x22, " << offsetting - 32 << "(sp)" << std::endl;
        dst << " sw x23, " << offsetting - 36 << "(sp)" << std::endl;
        dst << " sw x24, " << offsetting - 40 << "(sp)" << std::endl;
        dst << " sw x25, " << offsetting - 44 << "(sp)" << std::endl;
        dst << " sw x26, " << offsetting - 48 << "(sp)" << std::endl;
        dst << " sw x27, " << offsetting - 52 << "(sp)" << std::endl;
        dst << " addi s0, sp, " << offsetting << std::endl;

        if (children[1]->isparam())
        {
            context.stack.new_scope();
            children[1]->riscv(dst, context, destreg);
            children[2]->riscv(dst, context, destreg);
            context.stack.leave_scope();
        }

        else
        {
            children[2]->riscv(dst, context, destreg);
        }

        dst << temp << ":" << std::endl;
        dst << " lw ra, " << context.stack_offset - 4 << "(sp)" << std::endl;
        dst << " lw s0, " << context.stack_offset - 8 << "(sp)" << std::endl;
        dst << " lw x9, " << context.stack_offset - 12 << "(sp)" << std::endl;
        dst << " lw x18, " << context.stack_offset - 16 << "(sp)" << std::endl;
        dst << " lw x19, " << context.stack_offset - 20 << "(sp)" << std::endl;
        dst << " lw x20, " << context.stack_offset - 24 << "(sp)" << std::endl;
        dst << " lw x21, " << context.stack_offset - 28 << "(sp)" << std::endl;
        dst << " lw x22, " << context.stack_offset - 32 << "(sp)" << std::endl;
        dst << " lw x23, " << context.stack_offset - 36 << "(sp)" << std::endl;
        dst << " lw x24, " << context.stack_offset - 40 << "(sp)" << std::endl;
        dst << " lw x25, " << context.stack_offset - 44 << "(sp)" << std::endl;
        dst << " lw x26, " << context.stack_offset - 48 << "(sp)" << std::endl;
        dst << " lw x27, " << context.stack_offset - 52 << "(sp)" << std::endl;
        dst << " addi sp, sp, " << context.stack_offset << std::endl;
        dst << " jr ra" << std::endl << std::endl;
        context.curr_offset = 0;
    }
};

#endif
