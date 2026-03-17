#include "QuadTree.hpp"
#include "AEOverload.hpp"
#include "../UI/Debug.hpp"

namespace QuadTree
{
	QuadTreeEntry::QuadTreeEntry(GameObjectEntity* gameObject)
	{
		this->gameObject = gameObject;
		this->bounds.min.x = gameObject->position.x - gameObject->scale.x * 0.5f;
		this->bounds.min.y = gameObject->position.y - gameObject->scale.y * 0.5f;
		this->bounds.max.x = gameObject->position.x + gameObject->scale.x * 0.5f;
		this->bounds.max.y = gameObject->position.y + gameObject->scale.y * 0.5f;
	}

	Node::Node(Physics::AABB bounds, size_t max_entries)
	{
		this->bounds = bounds;
		this->max_entries = max_entries;
	}

	Node::~Node()
	{
		for (QuadTreeEntry* entry : entries)
		{
			delete entry;
		}
		for (Node* node : subdivisions)
		{
			delete node;
		}
	}

	bool Node::Insert(QuadTreeEntry* entry)
	{
		if (!bounds.CanFit(entry->bounds))
		{
			return false;
		}
		if (isLeaf)
		{
			entries.push_back(entry);
			if (entries.size() > max_entries)
			{
				isLeaf = false;
				Subdivide();
				RedistributeEntries();
			}
		}
		else
		{
			InsertToValidSubdivision(entry);
		}
		return true;
	}

	void Node::Subdivide()
	{
		AEVec2 mid = bounds.min + (bounds.max - bounds.min) / 2;
		subdivisions[0] = new Node(Physics::AABB{ mid, bounds.max }, max_entries);
		subdivisions[1] = new Node(Physics::AABB{ bounds.min, mid}, max_entries);
		subdivisions[2] = new Node(Physics::AABB{ AEVec2{bounds.min.x, mid.y}, AEVec2{mid.x, bounds.max.y} }, max_entries);
		subdivisions[3] = new Node(Physics::AABB{ AEVec2{mid.x, bounds.min.y}, AEVec2{bounds.max.x, mid.y} }, max_entries);
	}

	void Node::RedistributeEntries()
	{
		std::vector<QuadTreeEntry*> tmp(entries);
		entries.clear();
		for (QuadTreeEntry* entry : tmp)
		{
			InsertToValidSubdivision(entry);
		}
	}

	void Node::InsertToValidSubdivision(QuadTreeEntry* entry)
	{
		for (Node* node : subdivisions)
		{
			if (node->Insert(entry))
			{
				return;
			}
		}
		entries.push_back(entry);
	}

	void Node::RenderDebug()
	{
		AEVec2 mid = bounds.min + (bounds.max - bounds.min) / 2;
		DebugUtils::RenderLine(bounds.min, AEVec2{ bounds.min.x, bounds.max.y }, { 64, 255, 255, 255 });
		DebugUtils::RenderLine(bounds.min, AEVec2{ bounds.max.x, bounds.min.y }, { 64, 255, 255, 255 });
		DebugUtils::RenderLine(AEVec2{ bounds.min.x, bounds.max.y }, bounds.max, { 64, 255, 255, 255 });
		DebugUtils::RenderLine(AEVec2{ bounds.max.x, bounds.min.y }, bounds.max, { 64, 255, 255, 255 });
		if (!isLeaf)
		{
			for (Node* node : subdivisions)
			{
				node->RenderDebug();
			}
		}
	}

	int Node::GetBranchCount()
	{
		int result{ 1 };
		if (!isLeaf)
		{
			for(Node* node : subdivisions)
			{
				result += node->GetBranchCount();
			}
		}
		return result;
	}

	std::vector<Node*> Node::GetValidCollisionNodes(Physics::AABB bounds)
	{
		std::vector<Node*> result;
		if (entries.size() > 0)
		{
			if (bounds.Intersects(this->bounds))
			{
				result.push_back(this);
			}
		}
		if (!isLeaf)
		{
			for (Node* node : subdivisions)
			{
				for (Node* n2 : node->GetValidCollisionNodes(bounds))
				{
					result.push_back(n2);
				}
			}
		}
		return result;
	}

	std::vector<GameObjectEntity*> Node::GetPotentialCollisionTargets(GameObjectEntity* gameObject, std::vector<GameObjectEntity*> ignoredObjects)
	{
		return GetPotentialCollisionTargets(gameObject, ignoredObjects, GameObjectEntity::PhysicsType::TOTAL);
	}

	std::vector<GameObjectEntity*> Node::GetPotentialCollisionTargets(GameObjectEntity* gameObject, std::vector<GameObjectEntity*> ignoredObjects, GameObjectEntity::PhysicsType type_filter)
	{
		Physics::AABB bounds{};
		bounds.min.x = gameObject->position.x - gameObject->scale.x * 0.5f;
		bounds.min.y = gameObject->position.y - gameObject->scale.y * 0.5f;
		bounds.max.x = gameObject->position.x + gameObject->scale.x * 0.5f;
		bounds.max.y = gameObject->position.y + gameObject->scale.y * 0.5f;
		std::vector<Node*> validNodes = GetValidCollisionNodes(bounds);

		std::vector<GameObjectEntity*> result;

		for (Node* node : validNodes)
		{
			for (QuadTreeEntry* entry : node->entries)
			{
				if (!entry->gameObject->isActive) { continue; }

				if (type_filter != GameObjectEntity::PhysicsType::TOTAL)
				{
					if (entry->gameObject->go_type != type_filter)
					{
						continue;
					}
				}

				bool isIgnored{ false };
				if (ignoredObjects.size() > 0)
				{
					for (GameObjectEntity* go : ignoredObjects)
					{
						if (entry->gameObject == go)
						{
							isIgnored = true;
							break;
						}
					}
				}

				if (!isIgnored)
				{
					result.push_back(entry->gameObject);
				}
			}
		}

		return result;
	}

	std::vector<GameObjectEntity*> Node::GetPotentialCollisionTargets(GameObjectEntity* gameObject, GameObjectEntity::PhysicsType type_filter)
	{
		return GetPotentialCollisionTargets(gameObject, {}, type_filter);
	}

	Tree::Tree(Physics::AABB bounds, std::vector<GameObjectEntity*> gameObjects, size_t max_entries)
	{
		head = new Node(bounds, max_entries);
		for (GameObjectEntity* goEntry : gameObjects)
		{
			QuadTreeEntry* entry = new QuadTreeEntry(goEntry);
			head->Insert(entry);
		}
	}

	Tree::~Tree()
	{
		delete head;
	}

	void Tree::RenderDebug()
	{
		head->RenderDebug();
	}
}