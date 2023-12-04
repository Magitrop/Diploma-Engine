#include "runtime.h"

#include <engine/executable/editor_runtime_pipeline.h>

namespace engine
{
	class Runtime::Internal final
	{
	public:
		Internal();
		~Internal() = default;

		bool isRunning() const;

		void initialize();
		void run();

		void setPipeline(IRuntimePipeline* pipeline);

	private:
		std::shared_ptr<IRuntimePipeline> m_runtimePipeline = nullptr;
	};

	Runtime::Internal::Internal()
	{
		m_runtimePipeline = nullptr;
	}

	bool Runtime::Internal::isRunning() const
	{
		return m_runtimePipeline->isRunning();
	}

	void Runtime::Internal::initialize()
	{
		m_runtimePipeline->initialize();
	}

	void Runtime::Internal::run()
	{
		m_runtimePipeline->run();
	}

	void Runtime::Internal::setPipeline(IRuntimePipeline* pipeline)
	{
		m_runtimePipeline = std::shared_ptr<IRuntimePipeline>(pipeline);
	}

	Runtime::Runtime()
	{
		m_internal = std::make_unique<Internal>();
	}

	Runtime::~Runtime()
	{}

	bool engine::Runtime::isRunning() const
	{
		return m_internal->isRunning();
	}

	void Runtime::initialize()
	{
		m_internal->initialize();
	}

	void Runtime::run()
	{
		m_internal->run();
	}

	void Runtime::setPipeline(IRuntimePipeline* pipeline)
	{
		m_internal->setPipeline(pipeline);
	}
} // namespace engine