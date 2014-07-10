#include <typeindex>
#include <algorithm>
#include <string>
#include <sstream>
#include "Engine/Component.h"
#include "Engine/EntityManager.h"


EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

void EntityManager::remove_entity(unsigned int entity_id)
{
	verify_entity_exists(entity_id);

	for (ComponentData& component_data : entities[entity_id].components) {
		remove_component(entity_id, component_data.type);
		delete component_data.component;
	}

	entities.erase(entity_id);
}

void EntityManager::remove_component(unsigned int entity_id, const std::type_index& type)
{
	assert2(find_component(entity_id, type) != nullptr,
		"Entity %u has no component of type '%s'.", entity_id, type.name());

	/* notify each system that requests this type about this entity losing it */
	auto listener_itr = _component_type_listeners.find(type);
	for (System* system : listener_itr->second) {
		system->notify_destroyed(entity_id);
	}

	Entity& entity = entities[entity_id];
	auto itr = std::find_if(std::begin(entity.components), std::end(entity.components),
		[&](const ComponentData& component) { return component.type == type; });

	entity.components.erase(itr);
}

std::string EntityManager::pretty(unsigned int entity_id) const
{
	verify_entity_exists(entity_id);

	std::ostringstream str;
	str << "Entity " << entity_id << ":\n";

	const Entity& entity = entities.find(entity_id)->second;
	for (const ComponentData& component_data : entity.components) {
		str << "\t" << component_data.type.name() << "\t"
			<< component_data.component << std::endl;
	}

	return str.str();
}

Component* EntityManager::find_component(unsigned int entity_id, const std::type_index& component_type)
{
	auto entity_itr = entities.find(entity_id);
	if (entity_itr == std::end(entities)) {
		return nullptr;
	}

	Entity& entity = entity_itr->second;
	auto component = std::find_if(std::begin(entity.components), std::end(entity.components),
		[&](const ComponentData& component_data){return component_data.type == component_type; });
	if (component == std::end(entity.components)) {
		return nullptr;
	}

	return component->component;
}

void EntityManager::notify_systems_created(const ComponentData& component)
{
	auto listeners_itr = _component_type_listeners.find(component.type);
	if (listeners_itr == std::end(_component_type_listeners)) {
		return;
	}

	for (System* system : listeners_itr->second) {
		//todo maybe pass id as parameter to this function
		//todo maybe pass component to notify_created
		system->notify_created(component.entity_id);
	}
}

void EntityManager::verify_entity_exists(unsigned int entity_id) const
{
	assert2(entities.find(entity_id) != std::end(entities),
		"Unable to find entity with id=%u.", entity_id);
}
