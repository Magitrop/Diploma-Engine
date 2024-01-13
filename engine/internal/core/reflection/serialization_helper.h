#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <engine/core/components/component_registrar.h>

#define SERIALIZATION_BEGIN																					\
struct ComponentField																						\
{																											\
	std::uint8_t type;																						\
	const char* fieldName;																					\
	const char* componentName;																				\
	std::any defaultValue;																					\
};																											\
__declspec(dllexport) void __serialize_components(std::shared_ptr<engine::ComponentRegistrar> registrar)	\
{																											\
	std::vector<ComponentField> fields;
// TODO: return a bunch of shared_ptrs to every registered component instead of explicitly transfering the registrar as a parameter
#define COMPONENT_BEGIN(name)											\
registrar->registerComponent<name>(std::shared_ptr<name>(new name()));	\
{																		\
	const char* currentComponent = #name;

#define INTEGER(name, defaultValue) \
fields.emplace_back("integer", name, currentComponent, defaultValue);

#define FLOAT(name, defaultValue) \
fields.emplace_back("float", name, currentComponent, defaultValue);

#define COMPONENT_END \
}

#define SERIALIZATION_END \
}
