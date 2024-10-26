#ifndef ast_Operators_hpp
#define ast_Operators_hpp

#include "ast_Dereference.hpp"
#include <string>
#include <iostream>
#include <cmath>
#include <typeinfo>

class Operator : public Node
{
protected:
    Operator(NodePointer _left, NodePointer _right)
    {
        children.push_back(_left);
        children.push_back(_right);
    }

public:
    virtual ~Operator()
    {
        delete children[0];
        delete children[1];
    }

    virtual const char *getOpcode() const = 0;

    NodePointer getLeft() const
    {
        return children[0];
    }

    NodePointer getRight() const
    {
        return children[1];
    }

    virtual std::string findtype_expr(Context &context) const override
    {
        std::string var1 = getLeft()->findtype_expr(context);
        std::string var2 = getRight()->findtype_expr(context);
        if (var1 == "unsigned" || var2 == "unsigned")
        {
            return "unsigned";
        }
        else
        {
            return "int";
        }
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << "( ";
        children[0]->Print(dst);
        dst << " ";
        dst << getOpcode();
        dst << " ";
        children[1]->Print(dst);
        dst << " )";
    }
};

class AddOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "+";
    }

public:
    AddOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() + getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int olddestreg = destreg;
        if (getRight()->isfunction())
        {
            destreg = context.saved_cregfile.check_reg();
            context.saved_cregfile.occ_reg(destreg);
        }
        if (getRight()->ispointerwcontext(context) && !getLeft()->ispointerwcontext(context))
        {
            getLeft()->riscv(dst, context, destreg);
            dst << " slli x" << destreg << ", x" << destreg << ", 2" << std::endl;
        }
        else
        {
            getLeft()->riscv(dst, context, destreg);
        }
        int destreg1 = context.cregfile.check_reg();
        context.cregfile.occ_reg(destreg1);
        if (getLeft()->ispointerwcontext(context) && !getRight()->ispointerwcontext(context))
        {
            getRight()->riscv(dst, context, destreg1);
            dst << " slli x" << destreg1 << ", x" << destreg1 << ", 2" << std::endl;
        }
        else
        {
            getRight()->riscv(dst, context, destreg1);
        }
        dst << " add x" << olddestreg << ", x" << destreg << ", x" << destreg1 << std::endl;
        if (getRight()->isfunction())
        {
            context.saved_cregfile.free_reg(destreg);
        }
        context.cregfile.free_reg(destreg1);
    }
};

class SubtractOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "-";
    }

public:
    SubtractOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }
    virtual int evalconst() const override
    {
        return getLeft()->evalconst() - getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int olddestreg = destreg;
        if (getRight()->isfunction())
        {
            destreg = context.saved_cregfile.check_reg();
            context.saved_cregfile.occ_reg(destreg);
        }
        getLeft()->riscv(dst, context, destreg);
        int destreg1 = context.cregfile.check_reg();
        context.cregfile.occ_reg(destreg1);
        if (getLeft()->ispointerwcontext(context) && !getRight()->ispointerwcontext(context))
        {
            getRight()->riscv(dst, context, destreg1);
            dst << " slli x" << destreg1 << ", x" << destreg1 << ", 2" << std::endl;
            dst << " neg x" << destreg1 << ", x" << destreg1 << std::endl;
        }
        else
        {
            getRight()->riscv(dst, context, destreg1);
        }
        if (getLeft()->ispointerwcontext(context) && !getRight()->ispointerwcontext(context))
        {
            dst << " add x" << olddestreg << ", x" << destreg << ", x" << destreg1 << std::endl;
        }
        else
        {
            dst << " sub x" << olddestreg << ", x" << destreg << ", x" << destreg1 << std::endl;
        }
        if (getRight()->isfunction())
        {
            context.saved_cregfile.free_reg(destreg);
        }
        context.cregfile.free_reg(destreg1);
    }
};

class MultiplyOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "*";
    }

public:
    MultiplyOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }
    virtual int evalconst() const override
    {
        return getLeft()->evalconst() * getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int olddestreg = destreg;
        if (getRight()->isfunction())
        {
            destreg = context.saved_cregfile.check_reg();
            context.saved_cregfile.occ_reg(destreg);
        }
        getLeft()->riscv(dst, context, destreg);
        int destreg1 = context.cregfile.check_reg();
        context.cregfile.occ_reg(destreg1);
        getRight()->riscv(dst, context, destreg1);
        dst << " mul x" << olddestreg << ", x" << destreg << ", x" << destreg1 << std::endl;
        if (getRight()->isfunction())
        {
            context.saved_cregfile.free_reg(destreg);
        }
        context.cregfile.free_reg(destreg1);
    }
};

class DivideOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "/";
    }

public:
    DivideOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() / getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        if (findtype_expr(context) == "unsigned" || findtype_expr(context) == "unsigned")
        {
            int olddestreg = destreg;
            if (getRight()->isfunction())
            {
                destreg = context.saved_cregfile.check_reg();
                context.saved_cregfile.occ_reg(destreg);
            }
            getLeft()->riscv(dst, context, destreg);
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " divu x" << olddestreg << ", x" << destreg << ", x" << destreg1 << std::endl;
            if (getRight()->isfunction())
            {
                context.saved_cregfile.free_reg(destreg);
            }
            context.cregfile.free_reg(destreg1);
        }
        else
        {
            int olddestreg = destreg;
            if (getRight()->isfunction())
            {
                destreg = context.saved_cregfile.check_reg();
                context.saved_cregfile.occ_reg(destreg);
            }
            getLeft()->riscv(dst, context, destreg);
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " div x" << olddestreg << ", x" << destreg << ", x" << destreg1 << std::endl;
            if (getRight()->isfunction())
            {
                context.saved_cregfile.free_reg(destreg);
            }
            context.cregfile.free_reg(destreg1);
        }
    }
};

class ModuloOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "%";
    }

public:
    ModuloOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() % getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        if (findtype_expr(context) == "unsigned" || findtype_expr(context) == "unsigned")
        {
            int olddestreg = destreg;
            if (getRight()->isfunction())
            {
                destreg = context.saved_cregfile.check_reg();
                context.saved_cregfile.occ_reg(destreg);
            }
            getLeft()->riscv(dst, context, destreg);
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " remu x" << olddestreg << ", x" << destreg << ", x" << destreg1 << std::endl;
            if (getRight()->isfunction())
            {
                context.saved_cregfile.free_reg(destreg);
            }
            context.cregfile.free_reg(destreg1);
        }
        else
        {
            int olddestreg = destreg;
            if (getRight()->isfunction())
            {
                destreg = context.saved_cregfile.check_reg();
                context.saved_cregfile.occ_reg(destreg);
            }
            getLeft()->riscv(dst, context, destreg);
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " rem x" << olddestreg << ", x" << destreg << ", x" << destreg1 << std::endl;
            if (getRight()->isfunction())
            {
                context.saved_cregfile.free_reg(destreg);
            }
            context.cregfile.free_reg(destreg1);
        }
    }
};

class LOrOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "||";
    }

public:
    LOrOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() || getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {

        int olddestreg = destreg;
        if (getRight()->isfunction())
        {
            destreg = context.saved_cregfile.check_reg();
            context.saved_cregfile.occ_reg(destreg);
        }
        getLeft()->riscv(dst, context, destreg);
        int destreg1 = context.cregfile.check_reg();
        context.cregfile.occ_reg(destreg1);
        getRight()->riscv(dst, context, destreg1);
        dst << " slti x" << destreg << ", x" << destreg << ", 1" << std::endl;
        dst << " slti x" << destreg1 << ", x" << destreg1 << ", 1" << std::endl;
        dst << " and x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
        dst << " slti x" << destreg << ", x" << destreg << ", 1" << std::endl;
        if (getRight()->isfunction())
        {
            context.saved_cregfile.free_reg(destreg);
        }
        context.cregfile.free_reg(destreg1);
    }
};

class LAndOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "&&";
    }

public:
    LAndOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() && getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int olddestreg = destreg;
        if (getRight()->isfunction())
        {
            destreg = context.saved_cregfile.check_reg();
            context.saved_cregfile.occ_reg(destreg);
        }
        getLeft()->riscv(dst, context, destreg);
        int destreg1 = context.cregfile.check_reg();
        context.cregfile.occ_reg(destreg1);
        getRight()->riscv(dst, context, destreg1);
        dst << " slti x" << destreg << ", x" << destreg << ", 1" << std::endl;
        dst << " slti x" << destreg1 << ", x" << destreg1 << ", 1" << std::endl;
        dst << " or x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
        dst << " slti x" << destreg << ", x" << destreg << ", 1" << std::endl;
        if (getRight()->isfunction())
        {
            context.saved_cregfile.free_reg(destreg);
        }
        context.cregfile.free_reg(destreg1);
    }
};

class BOrOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "|";
    }

public:
    BOrOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() | getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int olddestreg = destreg;
        if (getRight()->isfunction())
        {
            destreg = context.saved_cregfile.check_reg();
            context.saved_cregfile.occ_reg(destreg);
        }
        getLeft()->riscv(dst, context, destreg);
        int destreg1 = context.cregfile.check_reg();
        context.cregfile.occ_reg(destreg1);
        getRight()->riscv(dst, context, destreg1);
        dst << " or x" << olddestreg << ", x" << destreg << ", x" << destreg1 << std::endl;
        if (getRight()->isfunction())
        {
            context.saved_cregfile.free_reg(destreg);
        }
        context.cregfile.free_reg(destreg1);
    }
};

class BXorOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "^";
    }

public:
    BXorOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() ^ getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int olddestreg = destreg;
        if (getRight()->isfunction())
        {
            destreg = context.saved_cregfile.check_reg();
            context.saved_cregfile.occ_reg(destreg);
        }
        getLeft()->riscv(dst, context, destreg);
        int destreg1 = context.cregfile.check_reg();
        context.cregfile.occ_reg(destreg1);
        getRight()->riscv(dst, context, destreg1);
        dst << " xor x" << olddestreg << ", x" << destreg << ", x" << destreg1 << std::endl;
        if (getRight()->isfunction())
        {
            context.saved_cregfile.free_reg(destreg);
        }
        context.cregfile.free_reg(destreg1);
    }
};

class BAndOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "&";
    }

public:
    BAndOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() & getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int olddestreg = destreg;
        if (getRight()->isfunction())
        {
            destreg = context.saved_cregfile.check_reg();
            context.saved_cregfile.occ_reg(destreg);
        }
        getLeft()->riscv(dst, context, destreg);
        int destreg1 = context.cregfile.check_reg();
        context.cregfile.occ_reg(destreg1);
        getRight()->riscv(dst, context, destreg1);
        dst << " and x" << olddestreg << ", x" << destreg << ", x" << destreg1 << std::endl;
        if (getRight()->isfunction())
        {
            context.saved_cregfile.free_reg(destreg);
        }
        context.cregfile.free_reg(destreg1);
    }
};

class EQOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "==";
    }

public:
    EQOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() == getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int olddestreg = destreg;
        if (getRight()->isfunction())
        {
            destreg = context.saved_cregfile.check_reg();
            context.saved_cregfile.occ_reg(destreg);
        }
        getLeft()->riscv(dst, context, destreg);
        int destreg1 = context.cregfile.check_reg();
        context.cregfile.occ_reg(destreg1);
        getRight()->riscv(dst, context, destreg1);
        dst << " sub x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
        dst << " seqz x" << destreg << ", x" << destreg << std::endl;
        if (getRight()->isfunction())
        {
            context.saved_cregfile.free_reg(destreg);
        }
        context.cregfile.free_reg(destreg1);
    }
};

class NEQOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "!=";
    }

public:
    NEQOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() != getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int olddestreg = destreg;
        if (getRight()->isfunction())
        {
            destreg = context.saved_cregfile.check_reg();
            context.saved_cregfile.occ_reg(destreg);
        }
        getLeft()->riscv(dst, context, destreg);
        int destreg1 = context.cregfile.check_reg();
        context.cregfile.occ_reg(destreg1);
        getRight()->riscv(dst, context, destreg1);
        dst << " sub x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
        dst << " snez x" << destreg << ", x" << destreg << std::endl;
        if (getRight()->isfunction())
        {
            context.saved_cregfile.free_reg(destreg);
        }
        context.cregfile.free_reg(destreg1);
    }
};

class LTOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "<";
    }

public:
    LTOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() < getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        if (findtype_expr(context) == "unsigned" || findtype_expr(context) == "unsigned")
        {
            int olddestreg = destreg;
            if (getRight()->isfunction())
            {
                destreg = context.saved_cregfile.check_reg();
                context.saved_cregfile.occ_reg(destreg);
            }
            getLeft()->riscv(dst, context, destreg);
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " sltu x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
            if (getRight()->isfunction())
            {
                context.saved_cregfile.free_reg(destreg);
            }
            context.cregfile.free_reg(destreg1);
        }
        else
        {
            int olddestreg = destreg;
            if (getRight()->isfunction())
            {
                destreg = context.saved_cregfile.check_reg();
                context.saved_cregfile.occ_reg(destreg);
            }
            getLeft()->riscv(dst, context, destreg);
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " slt x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
            if (getRight()->isfunction())
            {
                context.saved_cregfile.free_reg(destreg);
            }
            context.cregfile.free_reg(destreg1);
        }
    }
};

class GTOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return ">";
    }

public:
    GTOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() > getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        if (findtype_expr(context) == "unsigned" || findtype_expr(context) == "unsigned")
        {
            int olddestreg = destreg;
            if (getRight()->isfunction())
            {
                destreg = context.saved_cregfile.check_reg();
                context.saved_cregfile.occ_reg(destreg);
            }
            getLeft()->riscv(dst, context, destreg);
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " sgtu x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
            if (getRight()->isfunction())
            {
                context.saved_cregfile.free_reg(destreg);
            }
            context.cregfile.free_reg(destreg1);
        }
        else
        {
            int olddestreg = destreg;
            if (getRight()->isfunction())
            {
                destreg = context.saved_cregfile.check_reg();
                context.saved_cregfile.occ_reg(destreg);
            }
            getLeft()->riscv(dst, context, destreg);
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " sgt x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
            if (getRight()->isfunction())
            {
                context.saved_cregfile.free_reg(destreg);
            }
            context.cregfile.free_reg(destreg1);
        }
    }
};

class LTEQOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "<=";
    }

public:
    LTEQOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() <= getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        if (findtype_expr(context) == "unsigned" || findtype_expr(context) == "unsigned")
        {
            int olddestreg = destreg;
            if (getRight()->isfunction())
            {
                destreg = context.saved_cregfile.check_reg();
                context.saved_cregfile.occ_reg(destreg);
            }
            getLeft()->riscv(dst, context, destreg);
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " sgtu x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
            dst << " xori x" << destreg << ", x" << destreg << ", 1" << std::endl;
            if (getRight()->isfunction())
            {
                context.saved_cregfile.free_reg(destreg);
            }
            context.cregfile.free_reg(destreg1);
        }
        else
        {
            int olddestreg = destreg;
            if (getRight()->isfunction())
            {
                destreg = context.saved_cregfile.check_reg();
                context.saved_cregfile.occ_reg(destreg);
            }
            getLeft()->riscv(dst, context, destreg);
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " sgt x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
            dst << " xori x" << destreg << ", x" << destreg << ", 1" << std::endl;
            if (getRight()->isfunction())
            {
                context.saved_cregfile.free_reg(destreg);
            }
            context.cregfile.free_reg(destreg1);
        }
    }
};

class GTEQOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return ">=";
    }

public:
    GTEQOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() >= getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        std::cout << "seiufbweif" << std::endl;
        if (findtype_expr(context) == "unsigned" || findtype_expr(context) == "unsigned")
        {
            int olddestreg = destreg;
            if (getRight()->isfunction())
            {
                destreg = context.saved_cregfile.check_reg();
                context.saved_cregfile.occ_reg(destreg);
            }
            getLeft()->riscv(dst, context, destreg);
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " sltu x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
            dst << " xori x" << destreg << ", x" << destreg << ", 1" << std::endl;
            if (getRight()->isfunction())
            {
                context.saved_cregfile.free_reg(destreg);
            }
            context.cregfile.free_reg(destreg1);
        }
        else
        {
            int olddestreg = destreg;
            if (getRight()->isfunction())
            {
                destreg = context.saved_cregfile.check_reg();
                context.saved_cregfile.occ_reg(destreg);
            }
            getLeft()->riscv(dst, context, destreg);
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " slt x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
            dst << " xori x" << destreg << ", x" << destreg << ", 1" << std::endl;
            if (getRight()->isfunction())
            {
                context.saved_cregfile.free_reg(destreg);
            }
            context.cregfile.free_reg(destreg1);
        }
    }
};

class LeftShiftOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "<<";
    }

public:
    LeftShiftOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() << getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int olddestreg = destreg;
        if (getRight()->isfunction())
        {
            destreg = context.saved_cregfile.check_reg();
            context.saved_cregfile.occ_reg(destreg);
        }
        getLeft()->riscv(dst, context, destreg);
        int destreg1 = context.cregfile.check_reg();
        context.cregfile.occ_reg(destreg1);
        getRight()->riscv(dst, context, destreg1);
        dst << " sll x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
        if (getRight()->isfunction())
        {
            context.saved_cregfile.free_reg(destreg);
        }
        context.cregfile.free_reg(destreg1);
    }
};

class RightShiftOperator
    : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return ">>";
    }

public:
    RightShiftOperator(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual int evalconst() const override
    {
        return getLeft()->evalconst() >> getRight()->evalconst();
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        if (findtype_expr(context) == "unsigned" || findtype_expr(context) == "unsigned")
        {
            int olddestreg = destreg;
            if (getRight()->isfunction())
            {
                destreg = context.saved_cregfile.check_reg();
                context.saved_cregfile.occ_reg(destreg);
            }
            getLeft()->riscv(dst, context, destreg);
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " srl x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
            if (getRight()->isfunction())
            {
                context.saved_cregfile.free_reg(destreg);
            }
            context.cregfile.free_reg(destreg1);
        }
        else
        {
            int olddestreg = destreg;
            if (getRight()->isfunction())
            {
                destreg = context.saved_cregfile.check_reg();
                context.saved_cregfile.occ_reg(destreg);
            }
            getLeft()->riscv(dst, context, destreg);
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " sra x" << destreg << ", x" << destreg << ", x" << destreg1 << std::endl;
            if (getRight()->isfunction())
            {
                context.saved_cregfile.free_reg(destreg);
            }
            context.cregfile.free_reg(destreg1);
        }
    }
};

class Assign : public Operator
{
protected:
    virtual const char *getOpcode() const override
    {
        return "assignment";
    }

public:
    Assign(NodePointer _left, NodePointer _right)
        : Operator(_left, _right)
    {
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int offset = context.stack.find_symbol(getLeft()->findname())->offset;
        if (getLeft()->isarray())
        {
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            NodePointer index = getLeft()->getindex();
            if (context.stack.check_global(getLeft()->findname()))
            {
                if (context.stack.find_symbol(getLeft()->findname())->type == "int" || context.stack.find_symbol(getLeft()->findname())->type == "unsigned")
                {
                    dst << " lui x" << destreg1 << ", %hi(" << getLeft()->findname() << ")" << std::endl;
                    dst << " addi x" << destreg1 << ", x" << destreg1 << ", %lo(" << getLeft()->findname() << ")" << std::endl;
                    int destreg2 = context.cregfile.check_reg();
                    context.cregfile.occ_reg(destreg2);
                    index->riscv(dst, context, destreg2);
                    dst << " slli x" << destreg2 << ", x" << destreg2 << ", 2" << std::endl;
                    dst << " add x" << destreg1 << ", x" << destreg1 << ", x" << destreg2 << std::endl;
                    getRight()->riscv(dst, context, destreg2);
                    dst << " sw x" << destreg2 << ", 0(x" << destreg1 << ")" << std::endl;
                    context.cregfile.free_reg(destreg2);
                }
            }
            else
            {
                index->riscv(dst, context, destreg1);
                if (context.stack.find_symbol(getLeft()->findname())->type == "int" || context.stack.find_symbol(getLeft()->findname())->type == "unsigned")
                {
                    dst << " slli x" << destreg1 << ", x" << destreg1 << ", 2" << std::endl;
                    int destreg2 = context.cregfile.check_reg();
                    context.cregfile.occ_reg(destreg2);
                    dst << " li x" << destreg2 << ", " << offset << std::endl;
                    dst << " add x" << destreg1 << ", x" << destreg1 << ", x" << destreg2 << std::endl;
                    dst << " add x" << destreg1 << ", x" << destreg1 << ", s0" << std::endl;
                    getRight()->riscv(dst, context, destreg2);
                    dst << " sw x" << destreg2 << ", 0"
                        << "(x" << destreg1 << ")" << std::endl;
                    context.cregfile.free_reg(destreg2);
                }
            }
            context.cregfile.free_reg(destreg1);
        }
        else if (context.stack.check_global(getLeft()->findname()))
        {
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            int destreg2 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg2);
            dst << " lui x" << destreg1 << ", %hi(" << getLeft()->findname() << ")" << std::endl;
            getRight()->riscv(dst, context, destreg2);
            dst << " sw x" << destreg2 << ", %lo(" << getLeft()->findname() << ")"
                << "(x" << destreg1 << ")" << std::endl;
        }
        else if (typeid(*getLeft()) == typeid(Dereference))
        {
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " lw x" << destreg << ", " << context.stack.find_symbol(getLeft()->findname())->offset << "(s0)" << std::endl;
            dst << " sw x" << destreg1 << ", 0(x" <<destreg<<")"<<std::endl;
        }
        else
        {
            int destreg1 = context.cregfile.check_reg();
            context.cregfile.occ_reg(destreg1);
            getRight()->riscv(dst, context, destreg1);
            dst << " sw x" << destreg1 << ", " << offset << "(s0)" << std::endl;
            context.cregfile.free_reg(destreg1);
        }
    }
};

#endif
