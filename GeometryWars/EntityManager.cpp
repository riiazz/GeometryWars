#include "EntityManager.h"
#include "Entity.h"
#include <memory>

EntityManager::EntityManager()
{
}

void EntityManager::Update()
{
    for (auto& entity : this->m_toAdd) 
    {
        this->m_entities.push_back(entity);
        this->m_entityMap[entity->Tag()].push_back(entity);
    }
    this->m_toAdd.clear();
}

std::shared_ptr<Entity> EntityManager::AddEntity(const std::string& tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(tag, ++m_entityCounter));
    this->m_toAdd.push_back(entity);
    return entity;
}

const EntityVec& EntityManager::getEntities() const
{
    return this->m_entities;
}
