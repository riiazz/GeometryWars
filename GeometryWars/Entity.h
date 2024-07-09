#pragma once
#include <iostream>

class Entity
{
	friend class EntityManager;
	std::string m_tag = "default";
	bool m_active = true;
	size_t m_id;

	Entity(const std::string& tag, size_t id);
public:
	//component
	//here
	void Destroy();
	bool IsActive() const;
	std::string& Tag();
	size_t Id() const;
};

