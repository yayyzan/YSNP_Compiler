#ifndef ast_Node_hpp
#define ast_Node_hpp

#include "ast_context.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>

class Node;

typedef const Node *NodePointer;

class Node
{
protected:
    std::vector<NodePointer> children;

public:
    // node constructor
    Node(std::vector<NodePointer> _children) : children(_children)
    {
    }

    Node(std::vector<NodePointer> _children1, std::vector<NodePointer> _children2)
    {
        children = _children1;
        children.insert(children.end(), _children2.begin(), _children2.end());
    }
    // default constructor
    Node() : Node(std::vector<NodePointer>{})
    {
    }
    // node destructor, loop through children, deleting each one
    virtual ~Node()
    {
        for (unsigned i = 0; i < children.size(); i++)
        {
            delete children[i];
        }
    }
    // print for debugging
    virtual void Print(std::ostream &dst) const = 0;

    virtual void riscv(std::ostream &dst, Context &context, int destreg) const
    {
        std::cerr << std::endl
                  << "ASSEMBLY NOT IMPLEMENTED" << std::endl
                  << std::endl;
    }

    virtual void riscvwsymbol(std::ostream &dst, Context &context, int destreg, std::string pointer) const
    {
        std::cerr << std::endl
                  << "ASSEMBLY NOT IMPLEMENTED" << std::endl
                  << std::endl;
    }

    virtual int offset() const
    {
        return 0;
    }

    virtual std::string findname() const
    {
        return "nothin";
    }

    virtual std::string findtype() const
    {
        return "none";
    }

    virtual int evaluate(Context &context) const
    {
        return 0;
    }

    virtual bool isinit() const
    {
        return 0;
    }

    virtual bool isparam() const
    {
        return 0;
    }

    virtual bool isarray() const
    {
        return 0;
    }

    virtual int getarraysize() const
    {
        return 0;
    }

    virtual int findnum() const
    {
        return 0;
    }

    virtual NodePointer getindex() const
    {
        return 0;
    }

    virtual int evalconst() const
    {
        return 0;
    }

    virtual bool isfunction() const
    {
        return 0;
    }

    virtual NodePointer checkcase() const{
        return 0;
    }

    virtual bool ispointer() const{
        return 0;
    }

   virtual std::string findtype_expr(Context &context) const {
        return "none";
    }

    virtual bool ispointerwcontext(Context &context) const {
        return 0;
    }

};

#endif
