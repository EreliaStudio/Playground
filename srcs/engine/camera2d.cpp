#include "engine/camera2d.hpp"

#include "engine/engine.hpp"
#include "engine/entity2d.hpp"
#include "engine/transform2d.hpp"
#include "rendering/command/camera_ubo_render_command.hpp"

namespace spk
{
	Camera2D::Camera2D(const std::string &name, Entity2D *owner) :
		System::Participant2D(name, owner)
	{
	}

	Camera2D::Camera2D(Entity2D *owner) :
		Camera2D("Unnamed camera 2D", owner)
	{
	}

	Matrix4x4 Camera2D::viewMatrix() const
	{
		const Vector2 center{
			static_cast<float>(geometry().width) * 0.5f,
			static_cast<float>(geometry().height) * 0.5f};
		const Entity2D *cameraOwner = owner();
		const Matrix4x4 inverseCamera = cameraOwner == nullptr ? Matrix4x4::identity() : cameraOwner->transform().inverseModelMatrix();

		return Matrix4x4::translation(center.x, center.y, 0.0f) * inverseCamera;
	}

	void Camera2D::_buildRenderSnapshot(RenderSnapshot::Builder &builder)
	{
		if (activeCamera(context()) == this)
		{
			builder.renderPass(Engine::PreSceneRenderPassKey)
				.emplace<CameraUBORenderCommand>(viewMatrix());
		}
	}

	Camera2D *Camera2D::activeCamera(Engine *engine)
	{
		for (Camera2D *camera : Registry<Engine *, Camera2D>::elements(engine))
		{
			if (camera != nullptr && camera->isActive())
			{
				return camera;
			}
		}

		return nullptr;
	}

	const Camera2D *Camera2D::activeCamera(const Engine *engine)
	{
		return activeCamera(const_cast<Engine *>(engine));
	}
}
