#pragma once
#include <iostream>
#include "Component.h"

class Entity
{
	friend class EntityManager;
	std::string m_tag = "default";
	bool m_active = true;
	size_t m_id;

	Entity(const std::string& tag, size_t id);
public:
	//component
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CInput> cInput;
	std::shared_ptr<CScore> cScore;
	std::shared_ptr<CLifespan> cLifespan;

	void Destroy();
	bool IsActive() const;
	std::string& Tag();
	size_t Id() const;
};

