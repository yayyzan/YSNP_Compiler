#ifndef ast_VDeclaration_hpp
#define ast_VDeclaration_hpp

#include <string>
#include <iostream>
#include <vector>
class VDeclaration : public Node
{
public:
    VDeclaration(NodePointer _type, NodePointer _variable)
    {
        children.push_back(_type);
        children.push_back(_variable);
    }

    virtual ~VDeclaration()
    {
        delete children[0];
        delete children[1];
    }

    virtual void Print(std::ostream &dst) const override
    {
        if (children[1]->isarray())
        {
            dst << " ARRAY DECLERATION -> TYPE: ";
            children[0]->Print(dst);
            dst << " NAME: ";
            children[1]->Print(dst);
        }
        else
        {
            dst << " VARIABLE DECLERATION -> TYPE: ";
            children[0]->Print(dst);
            dst << " NAME: ";
            children[1]->Print(dst);
        }
    }

    virtual int offset() const override
    {
        if (children[1]->isarray())
        {
            if (children[0]->findtype() == "int" || children[0]->findtype() == "unsigned")
            {
                return 4 * children[1]->getarraysize();
            }
        }
        else if (children[0]->findtype() == "int" || children[0]->findtype() == "enum" || children[0]->findtype() == "unsigned")
        {
            return 4;
        }
        return 0;
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        if (!children[1]->isfunction())
        {
            if (!context.stack.is_global())
            {
                if (children[0]->findtype() == "int")
                {
                    if (children[1]->isarray())
                    {
                        context.stack.add_symbol(children[1]->findname(), 0, -(context.curr_offset - 4), "int");
                        context.curr_offset -= 4 * children[1]->getarraysize();
                    }
                    else if (children[1]->ispointer())
                    {
                        context.stack.add_symbol(children[1]->findname(), 0, -(context.curr_offset - 4), "pointer");
                        context.curr_offset -= 4;
                    }
                    else
                    {
                        context.stack.add_symbol(children[1]->findname(), 0, -(context.curr_offset - 4), "int");
                        context.curr_offset -= 4;
                    }
                }
                else if (children[0]->findtype() == "unsigned")
                {
                    if (children[1]->isarray())
                    {
                        context.stack.add_symbol(children[1]->findname(), 0, -(context.curr_offset - 4), "unsigned");
                        context.curr_offset -= 4 * children[1]->getarraysize();
                    }
                    else if (children[1]->ispointer())
                    {
                        context.stack.add_symbol(children[1]->findname(), 0, -(context.curr_offset - 4), "pointer");
                        context.curr_offset -= 4;
                    }
                    else
                    {
                        context.stack.add_symbol(children[1]->findname(), 0, -(context.curr_offset - 4), "unsigned");
                        context.curr_offset -= 4;
                    }
                }
            }
            else
            {
                if (children[0]->findtype() == "int")
                {
                    if (children[1]->isarray())
                    {
                        context.stack.add_symbol(children[1]->findname(), 0, children[1]->getarraysize(), "int");
                        if (!children[1]->isinit())
                        {
                            children[1]->riscv(dst, context, destreg);
                        }
                    }
                    else
                    {
                        context.stack.add_symbol(children[1]->findname(), children[1]->evalconst(), 0, "int");
                    }
                }
                else if (children[0]->findtype() == "unsigned")
                {
                    if (children[1]->isarray())
                    {
                        context.stack.add_symbol(children[1]->findname(), 0, children[1]->getarraysize(), "unsigned");
                        if (!children[1]->isinit())
                        {
                            children[1]->riscv(dst, context, destreg);
                        }
                    }
                    else
                    {
                        context.stack.add_symbol(children[1]->findname(), children[1]->evalconst(), 0, "unsigned");
                    }
                }
            }
            if (children[1]->isinit())
            {
                children[1]->riscv(dst, context, destreg);
            }
        }
    }
};

#endif
