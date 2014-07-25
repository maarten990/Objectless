#include <typeindex>
#include <algorithm>
#include <string>
#include <sstream>
#include "Engine/Component.h"
#include "Engine/EntityManager.h"
#include "Engine/System.h"

void EntityManager::remove_entity(unsigned int entity_id)
{
	verify_entity_exists(entity_id);

	Entity& entity = entities[entity_id];
	while (!entity.components.empty()) {
		ComponentData& last = entity.components[entity.components.size() - 1];
		remove_component(entity_id, last.type);
		delete last.component;
	}

	entities.erase(entity_id);
}

void EntityManager::remove_component(unsigned int entity_id, const std::type_index& type)
{
	verify_entity_exists(entity_id);
	assert2(find_component(entity_id, type) != nullptr,
		"Entity %u has no component of type '%s'.", entity_id, type.name());

	Entity& entity = entities[entity_id];
	auto itr = std::find_if(std::begin(entity.components), std::end(entity.components),
		[&](const ComponentData& component) { return component.type == type; });

	notify_systems_will_destroy(*itr);

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
	for (Listener& listener : listeners)
	{
		const auto& ents = listener.system->getEntities();
		if (std::find(std::begin(ents), std::end(ents), component.entity_id)
			!= std::end(ents))
		{
			//This system has already been notified about this entity.
			continue;
		}

		if (std::all_of(std::begin(listener.types), std::end(listener.types),
			[this, &component](const std::type_index& type) {
			return find_component(component.entity_id, type) != nullptr
				|| type == component.type; }))
		{
			//The entity has all of the components required by this listener.
			listener.system->notify_created(component.entity_id);
		}
	}
}

void EntityManager::notify_systems_will_destroy(const ComponentData& component)
{
	for (Listener& listener : listeners)
	{
		const std::vector<unsigned int>& entities2 = listener.system->getEntities();
		if (!std::any_of(std::begin(entities2), std::end(entities2),
			[&component](unsigned int entity_id) { return entity_id == component.entity_id; }))
		{
			/* System does not have a reference to this entity.
			* This can be happen when a system requires multiple types of components
			* to be attached to an entity at the same time, and not all of those
			* components being attached currently.
			*/
			continue;
		}

		if (std::any_of(std::begin(listener.types), std::end(listener.types),
			[&component](const std::type_index& type) {return component.type == type; }))
		{
			listener.system->notify_will_destroy(component.entity_id);
		}
	}
}

void EntityManager::verify_entity_exists(unsigned int entity_id) const
{
	assert2(entities.find(entity_id) != std::end(entities),
		"Unable to find entity with id=%u.", entity_id);
}
