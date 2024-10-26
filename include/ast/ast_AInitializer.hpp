#ifndef ast_AInitializer_hpp
#define ast_AInitializer_hpp

#include <iostream>
#include <vector>

class AInitializer : public Node
{
public:
    AInitializer(std::vector<NodePointer> _list)
        : Node(_list)
    {
    }

    ~AInitializer()
    {
        for (unsigned i = 0; i < children.size(); i++)
        {
            delete children[i];
        }
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << " ARRAY INITIALIZERS: ";
        for (unsigned i = 0; i < children.size(); i++)
        {
            children[i]->Print(dst);
            if (i < children.size() - 1)
            {
                dst << ", ";
            }
        }
        dst << std::endl;
        dst << " END ARRAY" << std::endl;
    }

    virtual void riscvwsymbol(std::ostream &dst, Context &context, int destreg, std::string pointer) const override
    {
        symbol *temp = context.stack.find_symbol(pointer);
        int offset = temp->offset;
        if (!context.stack.is_global())
        {
            if (temp->type == "int"||temp->type=="unsigned")
            {
                for (int i = 0; i < children.size(); i++)
                {
                    destreg = context.cregfile.check_reg();
                    context.cregfile.occ_reg(destreg);
                    children[i]->riscv(dst, context, destreg);
                    dst << " sw x" << destreg << ", " << offset + (i * 4) << "(s0)" << std::endl;
                    context.cregfile.free_reg(destreg);
                }
            }
        }
        else
        {
            dst << ".data" << std::endl;
            dst << std::endl;
            dst << pointer << ":" << std::endl;
            if (temp->type == "int" || temp->type == "unsigned")
            {
                for (int i = 0; i < context.stack.find_symbol(pointer)->offset; i++)
                {
                    if (i < children.size())
                    {
                        dst << " .word " << children[i]->evalconst() << std::endl;
                    }
                    else
                    {
                        dst << " .zero " << 4 * (context.stack.find_symbol(pointer)->offset - i) << std::endl;
                        break;
                    }
                }
            }
            dst << std::endl;
        }
    }
};

#endif
