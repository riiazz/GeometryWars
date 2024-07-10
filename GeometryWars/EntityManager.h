#pragma once
#include <vector>
#include <map>
#include <iostream>
#include "Entity.h"
#include <memory>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
	EntityVec m_entities;
	EntityMap m_entityMap;
	EntityVec m_toAdd;
	size_t m_entityCounter = 0;
public:
	EntityManager();
	void Update();
	std::shared_ptr<Entity> AddEntity(const std::string& tag);
	const EntityVec& GetEntities() const;
	const EntityVec& GetEntities(const std::string& tag) const;
};

