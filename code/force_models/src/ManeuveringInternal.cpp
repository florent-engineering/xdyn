/*
 * ManeuveringInternal.cpp
 *
 *  Created on: Jan 21, 2015
 *      Author: cady
 */


#include "ManeuveringInternal.hpp"

using namespace maneuvering;

Node::Node(const std::vector<NodePtr>& children_) : children(children_)
{
}

Node::~Node()
{
}

std::vector<NodePtr> Node::get_children() const
{
    return children;
}

Nullary::Nullary() : Node(std::vector<NodePtr>())
{
}

Binary::Binary(const NodePtr& lhs, const NodePtr& rhs) : Node({lhs,rhs})
{
}

Constant::Constant(const double val_) : val(val_)
{
}

Function Constant::get_lambda() const
{
    return [this](const BodyStates& , ssc::data_source::DataSource& , const double )
            {
                return val;
            };
}

Unary::Unary(const NodePtr operand) : Node({operand})
{
}

NodePtr Unary::get_operand() const
{
    return children.front();
}

Cos::Cos(const NodePtr& operand) : Unary(operand)
{
}
Function Cos::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return cos(op(states, ds, t));
            };
}

Abs::Abs(const NodePtr& operand) : Unary(operand)
{
}
Function Abs::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return std::abs(op(states, ds, t));
            };
}

NodePtr maneuvering::make_constant(const double val)
{
    return NodePtr(new Constant(val));
}

NodePtr maneuvering::make_cos(const NodePtr& n)
{
    return NodePtr(new Cos(n));
}

NodePtr maneuvering::make_abs(const NodePtr& n)
{
    return NodePtr(new Abs(n));
}
