#ifndef ast_Unary_hpp
#define ast_Unary_hpp

#include <string>
#include <iostream>

class Unary
    : public Node
{
protected:
    Unary(const NodePointer _expr)
    {
        children.push_back(_expr);
    }

public:
    virtual ~Unary()
    {
        delete children[0];
    }

    virtual const char *getOpcode() const = 0;

    NodePointer getExpr() const
    {
        return children[0];
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << "( ";
        dst << getOpcode();
        dst << " ";
        children[0]->Print(dst);
        dst << " )";
    }
};

class NegOperator
    : public Unary
{
public:
    NegOperator(const NodePointer _expr)
        : Unary(_expr)
    {
    }

    virtual const char *getOpcode() const override
    {
        return "-";
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        getExpr()->riscv(dst, context, destreg);
        dst << " neg x" << destreg << ", x" << destreg << std::endl;
    }
};

class PreIncrement
    : public Unary
{
public:
    PreIncrement(const NodePointer _expr)
        : Unary(_expr)
    {
    }

    virtual const char *getOpcode() const override
    {
        return "pre++";
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int offset = context.stack.find_symbol(getExpr()->findname())->offset;
        getExpr()->riscv(dst, context, destreg);
        dst << " addi x" << destreg << ", x" << destreg << ", 1" << std::endl;
        dst << " sw x" << destreg << ", " << offset << "(s0)" << std::endl;
    }
};

class PostIncrement
    : public Unary
{
public:
    PostIncrement(const NodePointer _expr)
        : Unary(_expr)
    {
    }

    virtual const char *getOpcode() const override
    {
        return "post++";
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int offset = context.stack.find_symbol(getExpr()->findname())->offset;
        getExpr()->riscv(dst, context, destreg);
        int temp = context.cregfile.check_reg();
        context.cregfile.occ_reg(temp);
        dst << " addi x" << temp << ", x" << destreg << ", 1" << std::endl;
        dst << " sw x" << temp << ", " << offset << "(s0)" << std::endl;
        context.cregfile.free_reg(temp);
    }
};

class PreDecrement
    : public Unary
{
public:
    PreDecrement(const NodePointer _expr)
        : Unary(_expr)
    {
    }

    virtual const char *getOpcode() const override
    {
        return "pre--";
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int offset = context.stack.find_symbol(getExpr()->findname())->offset;
        getExpr()->riscv(dst, context, destreg);
        dst << " addi x" << destreg << ", x" << destreg << ", -1" << std::endl;
        dst << " sw x" << destreg << ", " << offset << "(s0)" << std::endl;
    }
};

class PostDecrement
    : public Unary
{
public:
    PostDecrement(const NodePointer _expr)
        : Unary(_expr)
    {
    }

    virtual const char *getOpcode() const override
    {
        return "post--";
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        int offset = context.stack.find_symbol(getExpr()->findname())->offset;
        getExpr()->riscv(dst, context, destreg);
        int temp = context.cregfile.check_reg();
        context.cregfile.occ_reg(temp);
        dst << " addi x" << temp << ", x" << destreg << ", -1" << std::endl;
        dst << " sw x" << temp << ", " << offset << "(s0)" << std::endl;
        context.cregfile.free_reg(temp);
    }
};
#endif
