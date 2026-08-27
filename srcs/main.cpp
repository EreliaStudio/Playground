#include "engine/component_query.hpp"
#include "engine/component.hpp"
#include "engine/component2d.hpp"
#include "engine/component3d.hpp"
#include "engine/contextualizable_trait.hpp"
#include "engine/engine.hpp"
#include "engine/entity_query.hpp"
#include "engine/entity.hpp"
#include "engine/entity2d.hpp"
#include "engine/entity3d.hpp"
#include "engine/query.hpp"
#include "engine/reference_frame.hpp"
#include "engine/registry_query.hpp"
#include "engine/registry.hpp"
#include "engine/transform2d.hpp"
#include "engine/transform3d.hpp"


struct Data
{
	spk::Transform3D *transform;
};

int main(int argc, char **argv)
{
	auto data =
		spk::EntityQuery<spk::Entity, Data>(nullptr)
			.intersectWith(spk::EntitySelection<spk::Entity3D>(nullptr))
			.withComponent<spk::Transform3D>(&Data::transform)
			.withComponent<spk::Transform3D>()
			.results();

	return 0;
}
