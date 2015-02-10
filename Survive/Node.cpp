#include "Node.h"

Node::Node(Node* parentNodePtr, const sf::Vector2i& position)
  :parentNodePtr_(parentNodePtr)
{
  position_ = parentNodePtr_->getPosition() + position * 32;
}
Node::Node(const sf::Vector2i& position)
  :position_(position)
{}



//Getters
Node* Node::getParentNodePtr() const { return parentNodePtr_; }
float Node::getDistanceValue() const { return distanceValue_; }
float Node::getHeuristicValue() const { return heuristicValue_; }
float Node::getTotalValue() const { return totalValue_; }
bool Node::isStartNode() const { return startNode_; }
sf::Vector2i Node::getPosition() const { return position_; }
bool Node::isOnOpen() const { return isOnOpen_; }
bool Node::isOnClosed() const { return isOnClosed_; }

//Setters
void Node::setDistanceValue(float distanceValue) { distanceValue_ = distanceValue; }
void Node::setHeuristicValue(float heuristicValue) { heuristicValue_ = heuristicValue; }
void Node::setTotalValue() { totalValue_ = distanceValue_ + heuristicValue_; }
void Node::setIsStartNode(bool startNode) { startNode_ = startNode; }
void Node::setIsOnOpen(bool isOn) { isOnOpen_ = isOn; }
void Node::setIsOnClosed(bool isOn) { isOnClosed_ = isOn; }
void Node::setPosition(const sf::Vector2i& position) { position_ = position; }
//Comparison functor
bool Node::operator() (const Node& lhv, const Node& rhv) const {return lhv.totalValue_ < rhv.totalValue_; /*Backwards because I want a min-heap/min-priority-queue*/ }
bool Node::operator== (const Node& rhv) const {return position_ == rhv.position_; }
bool Node::operator< (const Node& rhv) const { return totalValue_ > rhv.totalValue_; }
