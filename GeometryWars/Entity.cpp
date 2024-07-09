#include "Entity.h"

Entity::Entity(const std::string& tag, size_t id)
	: m_tag(tag), m_id(id)
{
}

bool Entity::IsActive() const
{
	return m_active;
}

std::string& Entity::Tag() 
{
	return m_tag;
}

size_t Entity::Id() const
{
	return m_id;
}
