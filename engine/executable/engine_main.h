#pragma once

int main();
namespace engine
{
	class Main final
	{
	private:
		friend int ::main();

	private:
		int run();
	};
} // namespace engine