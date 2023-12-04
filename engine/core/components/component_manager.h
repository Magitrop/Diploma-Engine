#pragma once

#include <cstddef>

namespace engine
{
	//�������(�������� ? ) ComponentRegistrar ������� ����� ������� ��� ID �����������
	//���������� ���������� �������������� ������� ��� �������������
	//��������� ���������� ����� �������������� � �������� ����� registerComponent ������� ����� ����������� �� ��������� ID

	class ComponentManager
	{
	private:
		friend class EntityManager;

	protected:
		virtual ~ComponentManager() = 0;
		static std::size_t ID;
	};
} // namespace engine