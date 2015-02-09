#ifndef NODE_H
#define NODE_H
#include <memory>
#include <SFML/Graphics.hpp>

class Node
{
public:
  //Constructors
  Node(std::shared_ptr<Node>, const sf::Vector2i&);
  Node(const sf::Vector2i&);
  //Getters
  std::shared_ptr<Node> getParentNodePtr() const;
  float getDistanceValue() const;
  float getHeuristicValue() const;
  float getTotalValue() const;
  bool isStartNode() const;
  sf::Vector2i getPosition() const;

  //Setters
  void setDistanceValue(float);
  void setHeuristicValue(float);
  void setTotalValue(float);
  void setIsStartNode(bool);
  
  //Comparison overloads
  bool operator() (const Node&, const Node&) const;
  bool operator== (const Node&) const;
  bool operator< (const Node&) const;
 private:
  //Values
  std::shared_ptr<Node> parentNodePtr_;
  sf::Vector2i position_;
  float distanceValue_ = 0.0f;
  float heuristicValue_ = 0.0f;
  float totalValue_ = 0.0f;
  bool startNode_ = false;
  
};

#endif
