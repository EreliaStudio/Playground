#include "engine/contextualizable_trait.hpp"
#include "engine/engine.hpp"
#include "engine/entity.hpp"
#include "engine/entity2d.hpp"
#include "engine/entity3d.hpp"
#include "engine/reference_frame.hpp"
#include "engine/registry.hpp"
#include "engine/registry_query.hpp"
#include "engine/system_participant.hpp"
#include "engine/system_participant2d.hpp"
#include "engine/system_participant3d.hpp"
#include "engine/transform2d.hpp"
#include "engine/transform3d.hpp"

int main(int argc, char **argv)
{
	auto dataMerged = spk::Registry<spk::Engine *, spk::Entity2D>::query()
		.insert<spk::Registry<spk::Engine *, spk::Entity2D>::Query::IntersectWith<spk::Entity3D>>()
		.insert<spk::Registry<spk::Engine *, spk::Entity2D>::Query::UnionWith<spk::Entity>>()
		.collect(nullptr);

	return 0;
}
