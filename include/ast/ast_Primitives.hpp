#ifndef ast_Primitives_hpp
#define ast_Primitives_hpp

#include "ast_context.hpp"
#include <string>
#include <iostream>

class Identifier : public Node
{
private:
    std::string id;

public:
    Identifier(const std::string &_id)
        : id(_id)
    {
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << id;
    }

    virtual std::string findname() const override
    {
        return id;
    }

   virtual std::string findtype_expr(Context &context) const override
    {
        if (context.stack.find_symbol(id)->type == "unsigned")
        {
            return "unsigned";
        }
        else
        {
            return "int";
        }
    }

    virtual bool ispointerwcontext(Context &context) const override{
        return context.stack.find_symbol(id)->type == "pointer";
    }

    virtual int evaluate(Context &context) const override
    {
        return context.stack.find_symbol(id)->val;
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        if (context.stack.find_symbol(id)->type == "enum")
        {
            dst << " li x" << destreg << ", " << context.stack.find_symbol(id)->val << std::endl;
        }
        else if (context.stack.check_global(id))
        {
            int temp = context.cregfile.check_reg();
            context.cregfile.occ_reg(temp);
            dst << " lui x" << temp << ", %hi(" << id << ")" << std::endl;
            dst << " lw x" << destreg << ", %lo(" << id << ")"
                << "(x" << temp << ")" << std::endl;
            context.cregfile.free_reg(temp);
        }
        else
        {
            int offset = context.stack.find_symbol(id)->offset;
            dst << " lw x" << destreg << ", " << offset << "(s0)" << std::endl;
        }
    }
};

class Integer : public Node
{
private:
    int val;

public:
    Integer(int _val)
        : val(_val)
    {
    }

    int getValue() const
    {
        return val;
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << val;
    }

    virtual int evaluate(Context &context) const override
    {
        return val;
    }

    virtual int findnum() const override
    {
        return val;
    }

    virtual int evalconst() const override
    {
        return val;
    }

    virtual std::string findtype_expr(Context &context) const override
    {
        return "int";
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        dst << " li x" << destreg << ", " << val << std::endl;
    }
};

#endif
