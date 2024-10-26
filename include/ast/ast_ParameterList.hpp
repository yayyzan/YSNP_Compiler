#ifndef ast_ParameterList_hpp
#define ast_ParameterList_hpp

#include <iostream>
#include <vector>

class ParameterList : public Node
{
public:
    ParameterList(std::vector<NodePointer> _list)
        : Node(_list)
    {
    }

    ParameterList()
    {
    }

    ~ParameterList()
    {
        for (unsigned i = 0; i < children.size(); i++)
        {
            delete children[i];
        }
    }

    virtual void Print(std::ostream &dst) const override
    {
        dst << " PARAMS: " << std::endl;
        for (unsigned i = 0; i < children.size(); i++)
        {
            dst << " PARAM " << i << " : " << std::endl;
            children[i]->Print(dst);
            dst << std::endl;
        }
        dst << " END PARAMS " << std::endl;
    }

    virtual bool isparam() const override
    {
        return 1;
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
        for (unsigned i = 10; i < 10 + children.size(); ++i)
        {
            if (children[i - 10]->findtype() == "int")
            {
                if (i < 18)
                {
                    dst << " sw x" << i << ", " << -(context.curr_offset - 4) << "(s0)" << std::endl;
                    if(children[i-10]->ispointer()){
                        context.stack.add_symbol(children[i - 10]->findname(), 0, -(context.curr_offset - 4), "pointer");
                    }
                    else{
                    context.stack.add_symbol(children[i - 10]->findname(), 0, -(context.curr_offset - 4), "int");
                    }
                    context.curr_offset -= 4;
                }
                else
                {
                    if(children[i-10]->ispointer()){
                    context.stack.add_symbol(children[i - 10]->findname(), 0, (i - 18) * 4, "pointer");
                }
                    else{
                    context.stack.add_symbol(children[i - 10]->findname(), 0, (i - 18) * 4, "int");
                }
            }
            }
            else if (children[i - 10]->findtype() == "unsigned")
            {
                if (i < 18)
                {
                    dst << " sw x" << i << ", " << -(context.curr_offset - 4) << "(s0)" << std::endl;
                    if(children[i-10]->ispointer()){
                    context.stack.add_symbol(children[i - 10]->findname(), 0, -(context.curr_offset - 4), "pointer");
                    }
                    else{
                    context.stack.add_symbol(children[i - 10]->findname(), 0, -(context.curr_offset - 4), "unsigned");
                    }
                    context.curr_offset -= 4;
                }
                else
                {
                    if(children[i-10]->ispointer()){
                        context.stack.add_symbol(children[i - 10]->findname(), 0, (i - 18) * 4, "pointer");

                    }
                    else{
                    context.stack.add_symbol(children[i - 10]->findname(), 0, (i - 18) * 4, "unsigned");
                    }
                }
            }
        }
    }
};

#endif
