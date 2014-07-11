#pragma once
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <string>
#include "Component.h"
#include "Assert2.h"

class System;


/* Provides an interface to create, destroy or modify an entity and its
 * components. */
//todo maybe add listener for entity creation/destruction
class EntityManager
{
public:
	struct ComponentData
	{
		ComponentData(Component* component, const std::type_index& type,
			unsigned int entity_id)
			: component(component)
			, type(type)
			, entity_id(entity_id)
		{
		}

		Component* component;
		std::type_index type;
		unsigned int entity_id;
	};
	struct Entity
	{
		Entity()
			: id(~0u)
		{
		}

		unsigned int id;

		std::vector<ComponentData> components;
	};

	EntityManager();
	~EntityManager();


	unsigned int create_entity()
	{
		unsigned int id = _next_id++;
		Entity& entity = entities.insert({ id, Entity() }).first->second;
		entity.id = id;

		return id;
	}

	/* Adds an entity and returns a unique integer by which to identify it.
	 * The argument consists of a set of ids representing the components
	 * to be added to the entity.
	 */
	template <typename... Components>
	unsigned int create_entity()
	{
		unsigned int entity_id = create_entity();
		Entity& entity = entities[entity_id];
		entity.components.reserve(sizeof...(Components));
		create_entity_helper<Components...>(entity);

		return entity_id;
	}

	/* Remove the specified entity and deallocate its memory. */
	void remove_entity(unsigned int entity_id);

	/* Add or remove a component from an entity. Every entity is assumed to have
	 * a maximum of one instance of each type of component. */
	template <typename ComponentType>
	void add_component(unsigned int entity_id)
	{
		verify_entity_exists(entity_id);
		Entity& entity = entities[entity_id];

		ComponentData component_data(create_component<ComponentType>(entity));

		assert2(get_component<ComponentType>(entity_id) == nullptr,
			"Entity %u already has a component of type '%s'.", entity_id, component_data.type.name());
		
		entity.components.push_back(component_data);
	}

	template <typename ComponentType>
	void remove_component(unsigned int entity_id)
{
		remove_component(entity_id, typeid(ComponentType));
	}

	/* returns a pointer to a component belonging to a given entity */
	template <typename T>
	T *get_component(unsigned int entity_id) {
		return static_cast<T*>(find_component(entity_id, typeid(T)));
	}

	/* Register a system as being interested in a certain combination of
	 * component. The system will be notified when an entity containing these
	 * components is created, destroyed, or has changed its component layout
	 * such that it no longer contains the required ones. */
	template <typename... ComponentTypes>
	void register_system(System* system)
	{
		static_assert(sizeof...(ComponentTypes) > 0, "Cannot register a system for zero components.");
		register_system_helper<ComponentTypes...>(system);
	}

	/* Return a pretty string containing info about an entity. */
	std::string pretty(unsigned int entity_id) const;

private:
	void remove_component(unsigned int entity_id, const std::type_index& type);
	void add_component(unsigned int entity_id, std::type_index type);

	void notify_systems_created(const ComponentData& component);

	void verify_entity_exists(unsigned int entity_id) const;

	template <typename Component1>
	ComponentData create_component(const Entity& entity)
	{
		//todo maybe allocate somewhere to make all components of one type contiguous;
		//can improve cpu time in some cases.
		ComponentData component_data(new Component1(), typeid(Component1), entity.id);
		notify_systems_created(component_data);
		return component_data;
	}

	void create_entity_helper(Entity&)
	{ }

	template <typename Component1>
	void create_entity_helper(Entity& entity)
	{
		entity.components.push_back(create_component<Component1>(entity));
	}

	template <typename Component1, typename Component2, typename... ComponentRest>
	void create_entity_helper(Entity& entity)
	{
		create_entity_helper<Component1>(entity);
		create_entity_helper<Component2, ComponentRest...>(entity);
	}

	template <typename Component1>
	void register_system_helper(System* system)
	{
		_component_type_listeners[typeid(Component1)].push_back(system);
	}

	template <typename Component1, typename Component2, typename... ComponentRest>
	void register_system_helper(System* system)
	{
		register_system_helper<Component1>(system);
		register_system_helper<Component2, ComponentRest...>(system);
	}

	/* Find component belonging to the specified entity. Returns null if the entity
	 * does not have a component of that type. */
	Component* find_component(unsigned int entity_id, const std::type_index& component_type);


	// Used to generate unique IDs for entities.
	unsigned int _next_id = 0;

	//map<component type, vector<systems interested in component type>>
	std::unordered_map<std::type_index, std::vector<System*>> _component_type_listeners;

	std::unordered_map<unsigned int, Entity> entities;
};
