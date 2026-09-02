#pragma once

#include "engine/contextualizable_trait.hpp"
#include "engine/registry.hpp"
#include "math/matrix.hpp"
#include "math/vector2.hpp"

#include <variant>

namespace spk
{
	class Engine;

	class Camera : public ContextualizableTrait<Engine *>,
				   public Registry<Engine *, Camera>::Object
	{
	public:
		struct PerspectiveConfiguration
		{
			float verticalFieldOfViewRadians = 1.0f;
			float nearPlane = 0.1f;
			float farPlane = 1000.0f;
		};

		struct OrthographicConfiguration
		{
			float verticalSize = 10.0f;
			float nearPlane = 0.1f;
			float farPlane = 1000.0f;
		};

	private:
		using Configuration = std::variant<PerspectiveConfiguration, OrthographicConfiguration>;

		Configuration _configuration = PerspectiveConfiguration{};
		Vector2UInt _viewportSize{1, 1};
		Matrix4x4 _projection = Matrix4x4::identity();
		ContextualizableTrait<Engine *>::OnContextEditionContract _onContextEditionContract;

		[[nodiscard]] static Matrix4x4 _computeProjection(
			const PerspectiveConfiguration &configuration,
			const Vector2UInt &viewportSize);
		[[nodiscard]] static Matrix4x4 _computeProjection(
			const OrthographicConfiguration &configuration,
			const Vector2UInt &viewportSize);
		[[nodiscard]] static Matrix4x4 _computeProjection(
			const Configuration &configuration,
			const Vector2UInt &viewportSize);

	public:
		[[nodiscard]] static Camera *mainCamera(Engine *engine);
		[[nodiscard]] static const Camera *mainCamera(const Engine *engine);

		Camera();
		~Camera();

		void setAsMain(Engine &engine);
		[[nodiscard]] bool isMain() const;

		void configure(const PerspectiveConfiguration &configuration);
		void configure(const OrthographicConfiguration &configuration);
		void setViewportSize(const Vector2UInt &viewportSize);

		[[nodiscard]] const Vector2UInt &viewportSize() const noexcept;
		[[nodiscard]] Matrix4x4 projection() const noexcept;
	};
}
