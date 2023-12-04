#pragma once

#include <cstddef>

namespace engine
{
	//сделать(синглтон ? ) ComponentRegistrar который будет хранить все ID компонентов
	//встроенные компоненты регистрировать вручную при инициализации
	//кастомные компоненты будут генерироваться с функцией вроде registerComponent которая будет присваивать им очередной ID

	class ComponentManager
	{
	private:
		friend class EntityManager;

	protected:
		virtual ~ComponentManager() = 0;
		static std::size_t ID;
	};
} // namespace engine