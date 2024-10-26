#ifndef ast_FScope_hpp
#define ast_FScope_hpp

#include "ast_Default.hpp"
#include "ast_Case.hpp"
#include <iostream>
#include <vector>
#include <typeinfo>

class FScope : public Node
{
public:
    FScope(std::vector<NodePointer> _list)
        : Node(_list)
    {
    }

    FScope(std::vector<NodePointer> _list1, std::vector<NodePointer> _list2)
        : Node(_list1, _list2)
    {
    }

    FScope()
    {
    }

    ~FScope()
    {
        for (unsigned i = 0; i < children.size(); i++)
        {
            delete children[i];
        }
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << " !!SCOPE BEGIN!! " << std::endl;
        for (unsigned i = 0; i < children.size(); i++)
        {
            dst << " INDEX " << i << " : " << std::endl;
            children[i]->Print(dst);
            dst << std::endl;
        }
        dst << " !!END SCOPE!! " << std::endl;
    }

    virtual int offset() const override
    {
        int answer = 0;
        for (unsigned i = 0; i < children.size(); i++)
        {
            answer += children[i]->offset();
        }
        return answer;
    }

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
    {
        context.stack.new_scope();
        for (unsigned i = 0; i < children.size(); i++)
        {
            children[i]->riscv(dst, context, destreg);
        }
        context.print_symbol_table_stack(context.stack);
        context.stack.leave_scope();
    }

    virtual void riscvwsymbol(std::ostream &dst, Context &context, int destreg, std::string pointer) const override
    {
        int j = -1;
        std::string default1;
        context.case_labels.push_back({});
        context.stack.new_scope();
        for (unsigned i = 0; i < children.size(); i++)
        {
            if (typeid(*children[i]) == typeid(Default))
            {
                default1 = context.make_name("next");
                context.case_labels[context.case_labels.size()-1].push_back(default1);
                j = i;
            }
            else if (typeid(*children[i]) == typeid(Case))
            {
                children[i]->riscvwsymbol(dst, context, destreg, pointer);
            }
        }
        if (j >= 0)
        {
            children[j]->riscvwsymbol(dst, context, destreg, default1);
        }

        for (unsigned i = 0; i < children.size(); i++)
        {

                if (typeid(*children[i]) == typeid(Case) || typeid(*children[i]) == typeid(Default))
                {
                    dst << context.case_labels[context.case_labels.size()-1][0]  << ":" << std::endl;
                    context.case_labels[context.case_labels.size()-1].erase(context.case_labels[context.case_labels.size()-1].begin());
                    children[i]->checkcase()->riscv(dst, context, destreg);
                }
                else{
                    children[i]->riscv(dst, context, destreg);
                }

        }
        context.case_labels.pop_back();
        context.stack.leave_scope();
    }
};

#endif
