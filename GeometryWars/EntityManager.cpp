#include "EntityManager.h"
#include "Entity.h"
#include <memory>

void EntityManager::RemoveEntity(const std::string& tag)
{
    auto& entities = this->m_entityMap[tag];
    bool isAlive = false;
    auto it = std::remove_if(entities.begin(), entities.end(), [isAlive](const std::shared_ptr<Entity>& entity) {
        return entity->IsActive() == isAlive;
        });
    entities.erase(it, entities.end());
}

EntityManager::EntityManager()
{
}

void EntityManager::Update()
{
    //Deleting Dead entity;
    //from vector
    bool isAlive = false;
    auto it = std::remove_if(this->m_entities.begin(), this->m_entities.end(), [isAlive](const std::shared_ptr<Entity>& entity) {
        return entity->IsActive() == isAlive;
        });
    if (it != this->m_entities.end())
        this->m_entities.erase(it, this->m_entities.end());

    //from map
    for (const auto& pair : this->m_entityMap) {
        RemoveEntity(pair.first);
    }

    //Add Entity;
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

const EntityVec& EntityManager::GetEntities() const
{
    return this->m_entities;
}

const EntityVec& EntityManager::GetEntities(const std::string& tag) const
{
    if (this->m_entityMap.count(tag) == 0)
        return EntityVec();
    return this->m_entityMap.at(tag);
}
