#pragma once
#ifndef __QUADTREE_H
#define __QUADTREE_H
#include <vector>
#include <array>
#include "Physics.hpp"
#include "../Entities/GameObjectEntity.hpp"

namespace QuadTree
{
	struct QuadTreeEntry
	{
		GameObjectEntity* gameObject;
		Physics::AABB bounds;

		QuadTreeEntry(GameObjectEntity* gameObject);
	};

	class Node
	{
	public:
		Physics::AABB bounds;
		size_t max_entries;
		std::vector<QuadTreeEntry*> entries;
		std::array<Node*, 4> subdivisions{nullptr};
		bool isLeaf{ true };

		Node(Physics::AABB bounds, size_t max_entries);
		~Node();

		bool Insert(QuadTreeEntry* entry);
		void Subdivide();
		void RedistributeEntries();
		void InsertToValidSubdivision(QuadTreeEntry* entry);
		void RenderDebug();
		int GetBranchCount();
		std::vector<Node*> GetValidCollisionNodes(Physics::AABB bounds);
		std::vector<GameObjectEntity*> GetPotentialCollisionTargets(GameObjectEntity* gameObject, std::vector<GameObjectEntity*> ignoredObjects);
	};

	class Tree
	{
	public:
		Node* head;

		Tree(Physics::AABB bounds, std::vector<GameObjectEntity*> gameObjects, size_t max_entries = 4);
		~Tree();

		void RenderDebug();
	};
}

#endif