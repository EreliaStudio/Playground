#include <sparkle.hpp>

#define DEBUG_LINE() std::cout << __FUNCTION__ << "::" << __LINE__ << std::endl

class DebugWidget final : public spk::Widget
{
private:
	class TriangleRenderCommand : public spk::RenderCommand
	{
	public:
		struct Point
		{
			spk::Vector2 point;
		};

		struct GraphicalInfo
		{
			spk::Color color;
		};

		spk::Program& _program;
		spk::LayoutBuffer _layout;
		spk::UniformBuffer _ubo;
		static inline constexpr std::size_t GraphicalInfoBindingPoint = 0;

		spk::Program& getProgram()
		{
			static std::unique_ptr<spk::Program> result = nullptr;

			if (result == nullptr)
			{
				result = std::make_unique<spk::Program>(R"(#version 420 core
layout(location = 0) in vec2 inPosition;

void main()
{
	gl_Position = vec4(inPosition, 0.0, 1.0);
}
)",

R"(#version 420 core

layout(std140, binding = 0) uniform graphicalInfo
{
	vec4 color;
} GraphicalInfo;

out vec4 outColor;

void main()
{
	outColor = GraphicalInfo.color;
}
)");
				result->bindUniformBlock("graphicalInfo", GraphicalInfoBindingPoint);
			}

			return *result;
		}

	public:
		TriangleRenderCommand(const std::array<Point, 3>& points, const spk::Color &color) :
			_program(getProgram()),
			_ubo(GraphicalInfoBindingPoint, sizeof(GraphicalInfo))
		{
			auto &vbo = _layout.vertexBuffer();
			vbo.addAttribute(0, spk::VertexBuffer::Attribute::Type::Float, 2);
			vbo.insert<Point>(points);
			vbo.validate();

			auto &ibo = _layout.indexBuffer();
			ibo.setType(spk::IndexBuffer::Type::UnsignedInt);
			ibo.insert<std::uint32_t>({0, 1, 2});
			ibo.validate();

			_ubo.setData(GraphicalInfo{
				.color = color
			});
			_ubo.validate();
		}

		void execute(spk::RenderContext &renderContext) const override
		{
			const auto &ibo = _layout.indexBuffer();

			_program.activate(renderContext);
			_layout.activate(renderContext);
			_ubo.activate(renderContext);
			_program.render(spk::Program::Primitive::Triangles, *ibo.type(), 0, ibo.count());
		}
	};

	void _buildRenderSnapshot(spk::RenderSnapshot::Builder &builder) override
	{
		std::array<TriangleRenderCommand::Point, 3> points = {
			TriangleRenderCommand::Point{.point = {-0.5f, -0.5f}},
			TriangleRenderCommand::Point{.point = {0.5f, -0.5f}},
			TriangleRenderCommand::Point{.point = {0.0f, 0.5f}}
		};
		builder.renderPass(spk::Widget::OverlayKey).emplace<TriangleRenderCommand>(points, spk::Color{0.0f, 0.0f, 1.0f, 1.0f});
	}

	std::chrono::steady_clock::duration _lastUpdateReportTime = std::chrono::steady_clock::duration::zero();
	std::chrono::steady_clock::duration _nextUpdateReportTime = std::chrono::seconds(1);
	std::uint64_t _updatesSinceLastReport = 0;

	void _updateState(spk::UpdateContext &context) override
	{
		++_updatesSinceLastReport;
		if (context.time < _nextUpdateReportTime)
		{
			return;
		}

		const auto reportDuration = context.time - _lastUpdateReportTime;
		const double updatesPerSecond = static_cast<double>(_updatesSinceLastReport) /
			std::chrono::duration<double>(reportDuration).count();
		const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(context.time);
		const auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(context.deltaTime);

		std::cout
			<< "[UpdateState]\n"
			<< "Time = " << time.count() << " ms\n"
			<< "Delta time = " << deltaTime.count() << " ns\n"
			<< "Update rate = " << updatesPerSecond << " Hz\n"
			<< std::flush;

		_updatesSinceLastReport = 0;
		_lastUpdateReportTime = context.time;
		_nextUpdateReportTime = context.time + std::chrono::seconds(1);
	}

public:
	DebugWidget(std::string name, spk::Widget *parent) :
		spk::Widget(std::move(name), parent)
	{
	}
};

int main()
{
	spk::Application application;

	auto &window1 = application.createWindow(
		"First",
		spk::Window::Configuration{
			.title = "First window",
			.area = spk::Rect2D{
				.anchor = {100, 100},
				.size = {800, 600}},
			.backgroundColor = spk::Color{0.01f, 0.02f, 0.05f, 1.0f}});

	DebugWidget debugWidget("Debugger", &window1.root());
	debugWidget.setGeometry(window1.geometry());
	debugWidget.activate();

	return application.run();
}