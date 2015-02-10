#include <set>
#include <queue>
#include "Zombie.h"
#include <iostream>
#include "Collision.h"

//Functor to compare Node pointers
bool compNode::operator() (Node* lhv, Node* rhv) { return lhv->getTotalValue() > rhv->getTotalValue(); }

Zombie::Zombie(Player* player, sf::Texture* texture, sf::Texture* pCorpseTexture, const int health, const int speed)
	: Humanoid(texture), pCorpseTexture_(pCorpseTexture), pPlayer_(player)
{
	corpseSprite_.setTexture(*pCorpseTexture_);
	corpseSprite_.setOrigin(17.0f, 14.0f);
	finalSpeed_ = 3 + std::rand() % (int)(speed * 1.5);
	health_ = 100 + health;

}
void Zombie::update(const sf::Time& dT)
{
	if (pTurret_ != nullptr && pTurret_->isDead())
		pTurret_ = nullptr;

	if (pBarricade_ != nullptr && pBarricade_->isDead())
		pBarricade_ = nullptr;

	if (!dead_)
	{
		float playerDistance = sqrt(pow(positionGlobal_.y - pPlayer_->getPositionGlobal().y, 2) + pow(positionGlobal_.x - pPlayer_->getPositionGlobal().x, 2));
		float turretDistance;
		float barricadeDistance;
		float closerDistance = playerDistance;

		if (pTurret_ != nullptr)
			turretDistance = sqrt(pow(positionGlobal_.y - pTurret_->getPositionGlobal().y, 2) + pow(positionGlobal_.x - pTurret_->getPositionGlobal().x, 2));
		if (pBarricade_ != nullptr)
			barricadeDistance = sqrt(pow(positionGlobal_.x - pBarricade_->getPositionGlobal().x, 2) + pow(positionGlobal_.y - pBarricade_->getPositionGlobal().y, 2));

		if (pTurret_ != nullptr && turretDistance < closerDistance)
		{
			targetPosition_ = sf::Vector2i(pTurret_->getPositionGlobal().x - fmod(pTurret_->getPositionGlobal().x, 32.0f) + 16.0f, pTurret_->getPositionGlobal().y - fmod(pTurret_->getPositionGlobal().y, 32.0f) + 16.0f);
			closerDistance = turretDistance;
		}
		if (pBarricade_ != nullptr && barricadeDistance < closerDistance)
		{
			targetPosition_ = sf::Vector2i(pBarricade_->getPositionGlobal().x - fmod(pBarricade_->getPositionGlobal().x, 32.0f) + 16.0f, pBarricade_->getPositionGlobal().y - fmod(pBarricade_->getPositionGlobal().y, 32.0f) + 16.0f);
			closerDistance = barricadeDistance;
		}
		else
			targetPosition_ = sf::Vector2i(pPlayer_->getPositionGlobal().x - fmod(pPlayer_->getPositionGlobal().x, 32.0f) + 16.0f, pPlayer_->getPositionGlobal().y - fmod(pPlayer_->getPositionGlobal().y, 32.0f) + 16.0f);

		if(targetPosition_ != lastTargetPosition_)
		{
		    readyToRepath_ = true;
		    lastTargetPosition_ = targetPosition_;
		}
		
		if (closerDistance > 33.5f)
		{
			if (!sPNodes_.empty())
			{
				Node node = sPNodes_.top();
				sf::Vector2i nodePos = node.getPosition();
				if (positionGlobal_.x <= nodePos.x + 16 && positionGlobal_.x >= nodePos.x - 16 && positionGlobal_.y <= nodePos.y + 16 && positionGlobal_.y >= nodePos.y - 16)
				{
					sPNodes_.pop();
					if (!sPNodes_.empty())
					{
						node = sPNodes_.top();
						nodePos = node.getPosition();
					}
				}				
				targetVector_ = (sf::Vector2f)nodePos - positionGlobal_;
			}
			else			
				targetVector_ = sf::Vector2f(0.0f, 0.0f);			

			if(targetVector_ != sf::Vector2f(0.0f, 0.0f))
			   targetVector_ /= (float)sqrt(pow(targetVector_.x, 2) + pow(targetVector_.y, 2)); // Normalize

			velocity_ = targetVector_ * speed_;
		}

		else
			velocity_ = sf::Vector2f(0.0f, 0.0f);

		if (closerDistance <= 43.5f && !attacking_)
		{
			attackClock_.restart();
			attacking_ = true;
		}
		animate(dT);
		sf::Vector2f perpVector = sf::Vector2f(cos((rotationGlobal_ + 90.0f) * 3.14159265358f / 180.0f), sin((rotationGlobal_ + 90.0f)  * 3.14159265358 / 180.0f));
		sf::Vector2f forwardVector = sf::Vector2f(cos((rotationGlobal_)* 3.14159265358f / 180.0f), sin((rotationGlobal_)* 3.14159265358 / 180.0f));

		headSprite_.setRotation(atan2(forwardVector.y, forwardVector.x) / 3.14159265358f * 180);

		armLeftSprite_.setPosition(positionGlobal_ + forwardVector * 6.0f + forwardVector * (float)sin(armClock_.getElapsedTime().asSeconds() * 4 + armLeftVerticalOffset_) - perpVector * 8.0f);
		armRightSprite_.setPosition(positionGlobal_ + forwardVector * 7.4f + forwardVector * (float)sin(armClock_.getElapsedTime().asSeconds() * 4 + armRightVerticalOffset_) + perpVector * 8.0f);
		armLeftSprite_.setRotation(headSprite_.getRotation() + sin(armClock_.getElapsedTime().asSeconds() * 6 + armLeftHorizontalOffset_) * 3);
		armRightSprite_.setRotation(headSprite_.getRotation() + sin(armClock_.getElapsedTime().asSeconds() * 6 + armRightHorizontalOffset_) * 3);

		//Attack animation
		if (attacking_)
		{
			sf::Time sinTime = attackClock_.getElapsedTime();
			if (sinTime.asSeconds() < 0.35f)
			{
				armLeftSprite_.setPosition(armLeftSprite_.getPosition() + forwardVector * (float)sin(sinTime.asSeconds() * 8.164f) * 4.0f);
				armRightSprite_.setPosition(armRightSprite_.getPosition() + forwardVector * (float)sin(sinTime.asSeconds() * 8.164f) * 4.0f);
				armLeftSprite_.setRotation(armLeftSprite_.getRotation() + (float)sin(sinTime.asSeconds() * 8.164f) * 8.0f);
				armRightSprite_.setRotation(armRightSprite_.getRotation() - (float)sin(sinTime.asSeconds() * 8.164f) * 8.0f);

				if (!hit_ && sinTime.asSeconds() > 0.175f)
				{
					if (closerDistance <= 43.5f)
					{
						if (closerDistance == playerDistance)
						{
							pPlayer_->setHealth(pPlayer_->getHealth() - 10);
							pPlayer_->injure();
						}
						else if (closerDistance == barricadeDistance)
						{
							pBarricade_->setHealth(pBarricade_->getHealth() - 10);
						}
						else
							pTurret_->setHealth(pTurret_->getHealth() - 10);

					}
					hit_ = true;
				}
			}
			else
			{
				attacking_ = false;
				hit_ = false;
			}
		}

		//Sets it to corpse mode if health <= 0
		if (health_ <= 0)
		{
			dead_ = true;
			corpseSpeed_ = finalSpeed_;
		}
	}
	else
	{
		if (!still_)
		{
			sf::Vector2f forwardVector = sf::Vector2f(cos((rotationGlobal_)* 3.14159265358f / 180.0f), sin((rotationGlobal_)* 3.14159265358 / 180.0f));
			positionGlobal_ += forwardVector * corpseSpeed_ * 13.5f * dT.asSeconds();
			corpseSprite_.setPosition(positionGlobal_);
			corpseSprite_.setRotation(rotationGlobal_);
			headSprite_.setPosition(positionGlobal_); //For collision detection of dead bodies
			corpseSpeed_ -= 7.5f * dT.asSeconds();
		}
		if (!still_ && corpseSpeed_ <= 0.0f)
		{
			still_ = true;
			deathClock_.restart();
		}
		else if (still_ && deathClock_.getElapsedTime().asSeconds() >= 3.0f)
			delete_ = true;		
	}
}

void Zombie::findPath(std::vector< std::vector<Tile> >* pVTiles)
{
	//Creates a matrix of nodes
	std::vector< std::vector<Node> > mNodes_(257, std::vector<Node>(257, Node(sf::Vector2i(0, 0))));

	//Clears the stack
	while (!sPNodes_.empty())
		sPNodes_.pop();

	if (targetPosition_ != sf::Vector2i(0.0f, 0.0f))
	{
	  readyToRepath_ = false;
		
		//Initiates lists
		std::priority_queue<Node*, std::vector<Node*>, compNode> openList;
		std::vector<Node*> closedList;
		Node* currentNode;
		bool pathFound = false;

		//Initiates the great journey
		Node* pStartNode = &mNodes_.at((int)(positionGlobal_.x / 32)).at((int)(positionGlobal_.y / 32));
		pStartNode->setPosition(sf::Vector2i(positionGlobal_.x - fmod(positionGlobal_.x, 32.0f) + 16, positionGlobal_.y - fmod(positionGlobal_.y, 32.0f) + 16));
		pStartNode->setIsStartNode(true);
		pStartNode->setIsOnOpen(true);
		openList.push(pStartNode);

		while (!pathFound)
		{
		        std::cout << "openList size: " << openList.size() << std::endl;
		        std::cout << "closedList size: " << closedList.size() << std::endl;
		  
			//Gets the a pointer to the top item in the openList, then moves it to the closed list
			currentNode = openList.top();
			closedList.push_back(currentNode);
			currentNode->setIsOnClosed(true);
			currentNode->setIsOnOpen(true);
			openList.pop();

			//For the eight neighboring tiles/nodes
			for (int i = 0; i < 8; ++i)

			  {
				std::cout << i << std::endl;
				int xPos;
				int yPos;

				//xPos
				if (i == 0 || i == 4)
					xPos = 0;
				else if (i > 0 && i < 4)
					xPos = 1;
				else
					xPos = -1;

				//yPos
				if (i == 2 || i == 6)
					yPos = 0;
				else if (i < 2 || i > 6)
					yPos = 1;
				else
					yPos = -1;

				sf::Vector2i nodePosition = currentNode->getPosition() + sf::Vector2i(xPos * 32, yPos * 32);

				//Creates a node for the tile
				Node node(currentNode, sf::Vector2i(xPos, yPos));

				//Checks to see if it is the target adds node to stack and breaks if so
				if (node.getPosition() == targetPosition_)
				{
					pathFound = true;
					sPNodes_.push(node);
					break;
				}

				//Stop working if the node/tile is a wall or contains a tree
				if (pVTiles->at(nodePosition.x / 32).at(nodePosition.y / 32).getType() == "unwalkable" || pVTiles->at(nodePosition.y / 32).at(nodePosition.x / 32).hasItem())
					continue;

				//If it's not the target
				if (!pathFound)
				{
					float parentDistanceValue = node.getParentNodePtr()->getDistanceValue();

					//Distance is 1.4f x 32 if diagonal, 1 x 32 otherwise
					if (xPos == yPos)
						node.setDistanceValue(parentDistanceValue + 44.8f);
					else
						node.setDistanceValue(parentDistanceValue + 32.0f);

					//Gets the distance to the target(Heuristic) and then gets the total(Distance + Heuristic)
					node.setHeuristicValue(abs(targetPosition_.x - nodePosition.x) + abs(targetPosition_.y - nodePosition.y));
					node.setTotalValue(node.getHeuristicValue() + node.getDistanceValue());

					//If the node is not already on the open/closed list
					Node listCheckNode = mNodes_.at((int)(node.getPosition().x / 32)).at((int)(node.getPosition().y / 32));
					if (!listCheckNode.isOnClosed() && !listCheckNode.isOnOpen())
					{
						std::cout << "Node value: " << node.getTotalValue() << std::endl;
						mNodes_.at((int)(node.getPosition().x / 32)).at((int)(node.getPosition().y / 32)) = node;
						openList.push(&mNodes_.at((int)(node.getPosition().x / 32)).at((int)(node.getPosition().y / 32)));
					}
				}
			}
		}

		//Keeps stacking parent nodes until the start is reached
		while (!sPNodes_.top().isStartNode())
		{
			Node parent = *sPNodes_.top().getParentNodePtr();
			sPNodes_.push(parent);
		}
	}
}


//Operator overloading
bool Zombie::operator== (const Zombie& rhv) const { return positionGlobal_ == rhv.positionGlobal_; }
bool Zombie::bled()
{
	if (dead_ && !still_ && bleedClock_.getElapsedTime().asSeconds() > 0.1f)
	{
		bleedClock_.restart();
		return true;
	}
	else
		return false;
}
bool Zombie::countedDead()
{
	if (!countedDead_ && dead_)
	{
		countedDead_ = true;
		return true;
	}
	else
		return false;
}
bool Zombie::isStill() const { return still_; }
bool Zombie::isDead() const { return dead_; }
bool Zombie::isDeletable() const { return delete_; }
bool Zombie::isReadyToRepath() const { return readyToRepath_; }
sf::Sprite Zombie::getCorpseSprite() const { return corpseSprite_; }

//Setters
void Zombie::setTurretPtr(Turret* pTurret) { pTurret_ = pTurret; }
void Zombie::setBarricadePtr(Barricade* pBarricade) { pBarricade_ = pBarricade; }
