#ifndef ast_FDeclare_hpp
#define ast_FDeclare_hpp

#include <iostream>
#include <string>
#include <vector>
class FDeclare : public Node
{
public:
        FDeclare(NodePointer _name)
        {
                children.push_back(_name);
        }

        FDeclare(NodePointer _name, NodePointer _params)
        {
                children.push_back(_name);
                children.push_back(_params);
        }

        ~FDeclare()
        {
                for (unsigned i = 0; i < children.size(); i++)
                {
                        delete children[i];
                }
        }

        virtual void Print(std::ostream &dst) const override
        {

                children[0]->Print(dst);
                if (children.size() == 2)
                {
                        children[1]->Print(dst);
                }
        }

        virtual bool isfunction() const override{
                return 1;
        }

        virtual std::string findname() const override
        {
                return children[0]->findname();
        }

        virtual bool isparam() const override
        {
                return children.size() == 2;
        }

        virtual int offset() const override
        {
                if (children.size() == 2)
                {
                        return children[1]->offset();
                }
                return 0;
        }

        virtual void riscv(std::ostream &dst, Context &context, int destreg) const override
        {
                if (children.size() == 2)
                {
                        children[1]->riscv(dst, context, destreg);
                }
        }
};

#endif
